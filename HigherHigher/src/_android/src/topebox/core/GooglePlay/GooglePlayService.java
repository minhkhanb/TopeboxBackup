package topebox.core.GooglePlay;

import android.app.DialogFragment;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.app.Activity;
import android.app.Dialog;
import android.widget.Toast;

import topebox.core.ActivityCallback;
import topebox.core.Actions.*;
import topebox.core.AppConfig;
import topebox.core.AppContext;
import topebox.core.UpdateCloudSaveResultCode;

//google play
import com.google.android.gms.common.GoogleApiAvailability;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.PendingResult;
import com.google.android.gms.common.api.ResultCallback;
import com.google.android.gms.drive.Drive;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.GamesStatusCodes;
import com.google.android.gms.games.Player;
import com.google.android.gms.games.PlayerBuffer;
import com.google.android.gms.games.Players;
import com.google.android.gms.games.achievement.*;
import com.google.android.gms.games.leaderboard.LeaderboardVariant;
import com.google.android.gms.games.leaderboard.Leaderboards;
import com.google.android.gms.games.request.GameRequest;
import com.google.android.gms.games.request.GameRequestBuffer;
import com.google.android.gms.games.request.OnRequestReceivedListener;
import com.google.android.gms.games.request.Requests;
import com.google.android.gms.games.snapshot.Snapshot;
import com.google.android.gms.games.snapshot.SnapshotMetadata;
import com.google.android.gms.games.snapshot.SnapshotMetadataChange;
import com.google.android.gms.games.snapshot.Snapshots;
import com.google.android.gms.common.ConnectionResult;

import static topebox.core.OsFunctions.GetDataPath;

import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;

class GooglePlayService
		implements GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener
{
	//Interface
	private String mCurrentUserID = "";
	private String mCurrentUserName = "";
	public ArrayList<String> mFriendIds = new ArrayList<String>();
	public ArrayList <GameRequest> mRequests = new ArrayList<GameRequest>();
	ArrayList<String> mUserNameList = new ArrayList<String>();
	ArrayList<Long> mUserTimeAcceptList = new ArrayList<Long>();

	public boolean IsAvailableToUse = false;
	public String mSnapshotName= "snapshotTemp";
	public String mAppSavedName = "clouddata.pine";
	//
	private boolean mResolvingError = false;
	private static final String DIALOG_ERROR = "dialog_error";
	private static final int MAX_SNAPSHOT_RESOLVE_RETRIES = 3;
	private int mSnapshotResolveConflictRetryCount = 0;
	private GoogleApiClient CurrentClient;
	//private GoogleApiClient SocialClient;
	private Activity CurrentActivity;
	private AchievementBuffer Achie_buffer;

	static ArrayList<ActionArg> _waitting_arg = new ArrayList<ActionArg>();

	private static String package_name;

	public GooglePlayService(Activity activity){
		CurrentActivity = activity;

		Log.i("DEBUG", "init google play services");
		package_name = CurrentActivity.getApplicationContext().getPackageName();
		//GoogleSignInOptions.Builder gso = new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN);
		GoogleApiClient.Builder builder = new GoogleApiClient.Builder(activity);
		builder.addConnectionCallbacks(this);
		builder.addOnConnectionFailedListener(this);
		builder.addApi(Games.API).addScope(Games.SCOPE_GAMES);
		if (AppConfig.UseGoogleDrive)
		{
			Log.i("DEBUG", "init GoogleSignInOptions add drive api");

			//gso.requestScopes(new Scope(Scopes.DRIVE_APPFOLDER));
			//builder.addApi(Drive.API);
			//isUseOAuth2 = true;

			builder.addApi(Drive.API).addScope(Drive.SCOPE_APPFOLDER);
		}
		if (AppConfig.UseGooglePlus)
		{
			// Require Google + so we can use it to look at our circles.
			//Games.GamesOptions gamesOptions = Games.GamesOptions.builder().setRequireGooglePlus(true).build();
			//builder.addApi(Games.API, gamesOptions).addScope(Games.SCOPE_GAMES);

			//gso.requestScopes(new Scope(Scopes.PLUS_LOGIN));
			//gso.requestScopes(new Scope(Scopes.PROFILE));
			//gso.requestScopes(new Scope(Scopes.GAMES));
			//gso.requestEmail();
			//SocialClient = new GoogleApiClient.Builder(activity)
					//.addApi(Auth.GOOGLE_SIGN_IN_API, gso.build())
					//.build();
			//isUseOAuth2 = true;

		}
		Log.i("DEBUG", "init google play services build");
		CurrentClient = builder.build();
	}

	////IMPLIMENT GOOGLE PLAY CONNECT
	@Override
	public void onConnected(Bundle bundle) {
		Log.d("DEBUG", "onConnected() called. Sign in successful!");
		//Games.Achievements.unlock(mGoogleApiClient,
		//      getString(R.string.achievement_trivial_victory));
		//Log.i("DEBUG", "Try submit score");
		ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();

		for(int s = 0; s< _waitting_arg.size(); s++)
		{
			ActionArg arg = _waitting_arg.get(s);
			if(arg instanceof ActionGooglePlayLoginArg)
			{
				ActionGooglePlayLoginArg fbarg = (ActionGooglePlayLoginArg) arg;

				tmp.add(arg);
			}
		}

		for(int i = 0; i< tmp.size(); i++)
		{
			ActionArg arg = tmp.get(i);
			_waitting_arg.remove(arg);
			arg.onDone();
		}
		IsAvailableToUse = true;
		setUserCancelLogin(false);

		mCurrentUserID = Games.Players.getCurrentPlayer(CurrentClient).getPlayerId();
		mCurrentUserName =  Games.Players.getCurrentPlayer(CurrentClient).getName();
		Log.i("DEBUG", "Sign in google service successful get account info: User ID: " + mCurrentUserID + " Display Name: " + mCurrentUserName);

		loadAchievements();
		if (AppConfig.UseGoogleDrive) {
			mSnapshotName = CurrentActivity.getPackageName();
			savedGamesLoad(mSnapshotName);
		}

		if (AppConfig.UseGoogleFriend)
		{
			loadGooglePlayFriends();
		}

		if (AppConfig.UseGoogleSendGift)
		{
			Games.Requests.registerRequestListener(CurrentClient, mRequestListener);
			//mRequests = Games.Requests.getGameRequestsFromBundle(bundle);
			//handleRequests(mRequests);
		}
	}

	@Override
	public void onConnectionSuspended(int i) {
		Log.d("DEBUG", "onConnectionSuspended() called. Trying to reconnect.");
		IsAvailableToUse = false;
		new Handler().postDelayed(new Runnable() {
			@Override
			public void run() {
				if(CurrentClient != null){
					CurrentClient.connect();
				}
			}
		}, 1000);
		//if(CurrentClient != null && !CurrentClient.isConnecting() &&
		//!CurrentClient.isConnected()){
		//CurrentClient.connect();
		//}
	}

	@Override
	public void onConnectionFailed(ConnectionResult connectionResult) {
		Log.d("DEBUG", "onConnectionFailed() called, result: " + connectionResult);
		IsAvailableToUse = false;
		if (mResolvingError) {
			// Already attempting to resolve an error.
			return;
		}
		if(connectionResult.hasResolution()){
			try{
				Log.d("DEBUG", "******************************startResolutionForResult() called");
				connectionResult.startResolutionForResult(CurrentActivity, ActivityCallback.GOOGLE_PLAY_SERVICE_COMMON_RESOLVE);
			}catch(Exception ex){
				Log.i("DEBUG", ex.toString());
				//GooglePlayServicesUtil.getErrorDialog();
				Log.d("DEBUG", "******************************onConnectionFailed ->reconnect() called");
				if (CurrentClient != null && !CurrentClient.isConnecting() &&
						!CurrentClient.isConnected())
				{
					CurrentClient.connect();
				}
			}
		}else{
			// Show dialog using GoogleApiAvailability.getErrorDialog()
			Log.d("DEBUG", "******************************onConnectionFailed no resolution to resolve");
			//showErrorDialog(connectionResult.getErrorCode());
			//mResolvingError = true;
		}
	}

	public void connect(){
		if (CurrentClient != null && !CurrentClient.isConnecting() &&
				!CurrentClient.isConnected() && !isUserCancelLogin()) {
			Log.i("DEBUG", "call connect");
			CurrentClient.connect();
		}
	}

	public void connect(ActionGooglePlayLoginArg arg){
		if (CurrentClient != null && !CurrentClient.isConnecting() &&
				!CurrentClient.isConnected()) {
			_waitting_arg.add(arg);
			Log.i("DEBUG", "call connect");
			CurrentClient.connect();
		}
		else {
			arg.onCancel();
			Log.i("DEBUG", "Google Play Services client is not available or busy");
		}
	}
	
	public void cancelConnect()
	{
		Log.i("DEBUG", "Google play service on cancel connect");
		setUserCancelLogin(true);
		ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();

		for(int s = 0; s< _waitting_arg.size(); s++)
		{
			ActionArg arg = _waitting_arg.get(s);
			if(arg instanceof ActionGooglePlayLoginArg)
			{
				arg.onCancel();
				tmp.add(arg);
			}
		}

		for(int i = 0; i< tmp.size(); i++){
			_waitting_arg.remove(tmp.get(i));
		}
	}
	
	public boolean isConnected()
	{
		return CurrentClient != null && CurrentClient.isConnected();
	}
	
	public void disConnect(){
		if (CurrentClient != null && CurrentClient.isConnected()) {
			Log.i("DEBUG", "GoogleApiClient call disconnect");
			setUserCancelLogin(true);
			CurrentClient.disconnect();
		}
	}

	public void submitScore(String boardid, int score){
		if(CurrentClient != null && CurrentClient.isConnected()){
			if (!AppContext.is_debug) {
				Log.i("DEBUG", "Post to Google Play board:" + boardid + " score:" + score);
				Games.Leaderboards.submitScore(CurrentClient, boardid, (long) score);
			}
			else{
				Log.i("DEBUG", "Not submit score on debug build beacause of strange bug");
			}
		}else{
			Log.i("DEBUG", "CurrentClient null");
		}
	}
	public void showLeaderBoard(String boardid){
		if(CurrentClient != null && CurrentClient.isConnected()){
			CurrentActivity.startActivityForResult(Games.Leaderboards.getLeaderboardIntent(CurrentClient,boardid), ActivityCallback.GOOGLE_PLAY_SERVICE_SHOW_LEADER_BOARD);
		}
	}
	public void showAchievements(){
		if(CurrentClient != null && CurrentClient.isConnected()){
			CurrentActivity.startActivityForResult(Games.Achievements.getAchievementsIntent(CurrentClient), ActivityCallback.GOOGLE_PLAY_SERVICE_SHOW_ACHIEVEMENTS);
		}
	}
	public void unlockAchievement(String achi_id){
		if (Achie_buffer != null)
		{
			int bufSize = Achie_buffer.getCount();
			for ( int i = 0; i < bufSize; i++ )  {
				Achievement ach = Achie_buffer.get( i );

				// here you now have access to the achievement's data
				String id = ach.getAchievementId();  // the achievement ID string
				//Log.i("DEBUG", id + "\n");
				if (id.equals(achi_id))
				{
					boolean unlocked = ach.getState() == Achievement.STATE_UNLOCKED;  // is unlocked
					boolean incremental = ach.getType() == Achievement.TYPE_INCREMENTAL;  // is incremental
					if ( incremental ) {
						int steps = ach.getCurrentSteps();  // current incremental steps
					}
					if(!unlocked && CurrentClient != null && CurrentClient.isConnected()){
						//Log.i("DEBUG", "Request unlock achievement id: " + achi_id);
						Games.Achievements.unlock(CurrentClient, achi_id);
					}
					else
					{
						//Log.i("DEBUG", "achievement id: " + achi_id + "has already unlocked or google service is not available");
					}
					return;
				}
			}
			//Log.i("DEBUG", "achievement id: " + achi_id + " not found in Achie_buffer");
		}
		else
		{
			//Log.i("DEBUG", "Achie_buffer is null");
		}
	}
	public void loadAchievements()
	{
		boolean fullLoad = true;  // set to 'true' to reload all achievements (ignoring cache)
		long waitTime = 60;    // seconds to wait for achievements to load before timing out

		try {
			// load achievements
			Games.Achievements.load(CurrentClient, fullLoad).setResultCallback(new ResultCallback<Achievements.LoadAchievementsResult>(){
				@Override
				public void onResult(Achievements.LoadAchievementsResult loadAchievementsResult) {
					Achie_buffer = loadAchievementsResult.getAchievements();
					Log.i("DEBUG", "Load achievement success!");
					for( Achievement achievement : loadAchievementsResult.getAchievements() ){
					//Log.i("DEBUG", achievement.getAchievementId() + "\n");
					// here you can work with the achievement objects
					// ...
					//boolean unlocked = (achievement.getState() == Achievement.STATE_UNLOCKED);
					}
				}
			});
		} catch (Exception ex){
			Log.i("DEBUG", "Load achievement fail with exception: " + ex.getMessage());
		}
	}

	/* Creates a dialog for an error message */
	private void showErrorDialog(int errorCode) {
		// Create a fragment for the error dialog
		ErrorDialogFragment dialogFragment = new ErrorDialogFragment();
		// Pass the error that should be displayed
		Bundle args = new Bundle();
		args.putInt(DIALOG_ERROR, errorCode);
		dialogFragment.setArguments(args);
		dialogFragment.show(CurrentActivity.getFragmentManager(), "errordialog");
	}

	/* Called from ErrorDialogFragment when the dialog is dismissed. */
	public void onDialogDismissed() {
		mResolvingError = false;
	}

	/* A fragment to display an error dialog */
	public static class ErrorDialogFragment extends DialogFragment {
		public ErrorDialogFragment() { }

		@Override
		public Dialog onCreateDialog(Bundle savedInstanceState) {
			// Get the error code and retrieve the appropriate dialog
			int errorCode = this.getArguments().getInt(DIALOG_ERROR);
			return GoogleApiAvailability.getInstance().getErrorDialog(
					this.getActivity(), errorCode, ActivityCallback.GOOGLE_PLAY_SERVICE_COMMON_RESOLVE);
		}

		@Override
		public void onDismiss(DialogInterface dialog) {
			dialog.dismiss();
		}
	}

	static public boolean isUserCancelLogin()
	{
		boolean isCancel = false;
		try
		{
			String dataPath = "data/data/" + GetDataPath();
			FileInputStream f = new FileInputStream(dataPath + "/" + package_name + "loginGPGS.p");
			if (f != null) {
				DataInputStream fIn = new DataInputStream(f);
				if (fIn != null) {
					isCancel = fIn.readBoolean();
					fIn.close();
					Log.i("DEBUG", "isUserCancelLogin: " + isCancel);
				}else {
					Log.i("DEBUG", "cannot read from: " + dataPath + package_name + "loginGPGS.p");
				}
				f.close();
			}else {
				Log.i("DEBUG", "cannot read from: " + dataPath + package_name + "loginGPGS.p");
			}
			return isCancel;
		}
		catch (IOException e)
		{
			System.out.println("isUserCancelLogin IOException : " + e);
			return false;
		}
	}

	static public void setUserCancelLogin(boolean userChoose)
	{
		try
		{
			String dataPath = "data/data/" + GetDataPath();
			FileOutputStream f = new FileOutputStream(dataPath + "/" + package_name + "loginGPGS.p", false);
			if (f != null) {
				DataOutputStream fOut = new DataOutputStream(f);
				if (fOut != null) {
					fOut.writeBoolean(userChoose);
					fOut.close();
					Log.i("DEBUG", "setUserCancelLogin: " + userChoose);
				} else {
					Log.i("DEBUG", "cannot write to : " + dataPath + package_name + "loginGPGS.p");
				}
				f.close();
			}
			else {
				Log.i("DEBUG", "cannot write to : " + dataPath + package_name + "loginGPGS.p");
			}
		}
		catch (Exception e)
		{
			System.out.println("setUserCancelLogin Exception : " + e);
		}
	}

	public void showSavedGamesUI() {
		try {
			int maxNumberOfSavedGamesToShow = 1;
			Intent savedGamesIntent = Games.Snapshots.getSelectSnapshotIntent(CurrentClient,
					"See My Saves", true, true, maxNumberOfSavedGamesToShow);
			CurrentActivity.startActivityForResult(savedGamesIntent, ActivityCallback.GOOGLE_PLAY_SERVICE_SAVED_GAMES);
		}catch (Exception ex){
			Log.i("DEBUG", "Show save game intent fail with exception: " + ex.getMessage());
		}
	}

	public PendingResult<Snapshots.CommitSnapshotResult> writeSnapshot(Snapshot snapshot,
																	   byte[] data, Bitmap coverImage, String desc) {

		// Set the data payload for the snapshot
		snapshot.getSnapshotContents().writeBytes(data);

		// Create the change operation
		SnapshotMetadataChange metadataChange = new SnapshotMetadataChange.Builder()
				.setCoverImage(coverImage)
				.setDescription(desc)
				.build();

		// Commit the operation
		return Games.Snapshots.commitAndClose(CurrentClient, snapshot, metadataChange);
	}

	/**
	 * Load a Snapshot from the Saved Games service based on its unique name.  After load, the UI
	 * will update to display the Snapshot data and SnapshotMetadata.
	 * @param snapshotName the unique name of the Snapshot.
	 */
	public void savedGamesLoad(String snapshotName) {
		PendingResult<Snapshots.OpenSnapshotResult> pendingResult = Games.Snapshots.open(
				CurrentClient, snapshotName, false);

		//showProgressDialog("Loading Saved Game");
		ResultCallback<Snapshots.OpenSnapshotResult> callback =
				new ResultCallback<Snapshots.OpenSnapshotResult>() {
					@Override
					public void onResult(Snapshots.OpenSnapshotResult openSnapshotResult) {
						if (openSnapshotResult.getStatus().isSuccess()) {
							//displayMessage(getString(R.string.saved_games_load_success), false);
							byte[] data = null;
							try {
								data = openSnapshotResult.getSnapshot().getSnapshotContents().readFully();
							} catch (IOException e) {
								//displayMessage("Exception reading snapshot: " + e.getMessage(), true);
								Log.i("DEBUG", "Read snapshot data error!");
							}
							setDataSave(data);
							displaySnapshotMetadata(openSnapshotResult.getSnapshot().getMetadata());
						} else {
							Log.i("DEBUG", "Load snapshots fail! Reason: " + openSnapshotResult.getStatus());
							//displayMessage(getString(R.string.saved_games_load_failure), true);
							//clearDataUI();
						}

						//dismissProgressDialog();
					}
				};
		pendingResult.setResultCallback(callback);
	}

	public int savedGamesUpdate(String localSaveName) {
		if (!AppConfig.UseGoogleDrive)
		{
			Log.i("DEBUG", "Google Drive is not enable for this game, see string.xml");
			return UpdateCloudSaveResultCode.GOOGLE_UPDATE_CLOUD_SAVE_FAIL;
		}
		mAppSavedName = localSaveName;
		final String snapshotName = mSnapshotName;
		final boolean createIfMissing = true;

		// Use the data from local as the new Snapshot data.
		final byte[] data = getDataSave();
		if (data == null)
		{
			savedGamesLoad(snapshotName);
			return UpdateCloudSaveResultCode.GOOGLE_UPDATE_CLOUD_SAVE_CONFLICT_RESOLVED;
		}
		
		if (CurrentClient == null || !isConnected())
		{
			Log.i("DEBUG", "GoogleApiClient is null or not connected");
			return UpdateCloudSaveResultCode.GOOGLE_UPDATE_CLOUD_SAVE_FAIL;
		}

		AsyncTask<Void, Void, Integer> updateTask = new AsyncTask<Void, Void, Integer>() {
			
			protected void onPreExecute() {
				Log.i("DEBUG", "Updating Saved Game");
			}

			protected Integer doInBackground(Void... params) {
				Snapshots.OpenSnapshotResult open = Games.Snapshots.open(
						CurrentClient, snapshotName, createIfMissing).await();
				Snapshot snapshot = null;
				Boolean isConflict = false;
				if (!open.getStatus().isSuccess()) {
					Log.i("DEBUG", "Update cloud save conflict. Try to resolve.");
					snapshot = processSnapshotOpenResult(open, 0);
					try {
						final byte[] data1 = snapshot.getSnapshotContents().readFully();
						setDataSave(data1);
						snapshot.getSnapshotContents().writeBytes(data1);
					} catch (IOException e) {
						//displayMessage("Exception reading snapshot: " + e.getMessage(), true);
						Log.i("DEBUG", "savedGamesUpdate Read snapshot data fail with exception: " + e.getMessage());
					}
					isConflict = true;
					if(snapshot == null) {
						return UpdateCloudSaveResultCode.GOOGLE_UPDATE_CLOUD_SAVE_FAIL;
					}
				}
				else {
					// Change data but leave existing metadata
					snapshot = open.getSnapshot();
					snapshot.getSnapshotContents().writeBytes(data);
				}
				
				Snapshots.CommitSnapshotResult commit = Games.Snapshots.commitAndClose(
							CurrentClient, snapshot, SnapshotMetadataChange.EMPTY_CHANGE).await();

				if (!commit.getStatus().isSuccess()) {
					Log.i("DEBUG", "Failed to commit Snapshot.");
					return UpdateCloudSaveResultCode.GOOGLE_UPDATE_CLOUD_SAVE_FAIL;
				}
				Log.i("DEBUG", "commit Snapshot success.");
				
				// No failures
				if (isConflict){
					return UpdateCloudSaveResultCode.GOOGLE_UPDATE_CLOUD_SAVE_CONFLICT_RESOLVED;
				}
				else{
					return UpdateCloudSaveResultCode.GOOGLE_UPDATE_CLOUD_SAVE_SUCCESS;
				}
			}

			protected void onPostExecute(Boolean result) {
				if (result) {
					//displayMessage(getString(R.string.saved_games_update_success), false);
					Log.i("DEBUG", "update saved success!");
				} else {
					//displayMessage(getString(R.string.saved_games_update_failure), true);
					Log.i("DEBUG", "update saved fail!");
				}
			}
		};
		updateTask.execute();
		try{
			return (int)updateTask.get();
		}catch (Exception ex){
			Log.i("DEBUG", "Update cloud save get result from async task fail with exception: " + ex.getMessage());
			return UpdateCloudSaveResultCode.GOOGLE_UPDATE_CLOUD_SAVE_FAIL;
		}
	}

	public byte [] getDataSave()
	{
		String dataPath = "data/data/" + GetDataPath();
		File file = new File(dataPath, mAppSavedName);
		Log.i("DEBUG", "Load local app data from: " + file.getAbsoluteFile());
		InputStream fIn = null;
		if (file.exists()) {
			try {
				int length = (int)file.length();
				byte [] data = new byte[length];
				fIn = new FileInputStream(file);
				if (fIn != null) {
					fIn.read(data, 0, length);
					fIn.close();
					Log.i("DEBUG", "Get appdata saved file done. Data length = " + length);
					return data;
				}
			}catch (Exception ex)
			{
				Log.i("DEBUG", "Cannot get appdata saved file");
			}
		}
		else
		{
			Log.i("DEBUG", "Appdata save file doesnot exists!");
		}
		return null;
	}

	public void setDataSave(byte [] data)
	{
		String dataPath = "data/data/" + GetDataPath();
		File file = new File(dataPath, mAppSavedName);
		OutputStream fOut = null;
		if (file != null) {
			try {
				fOut = new FileOutputStream(file);
				if (fOut != null) {
					fOut.write(data);
					Log.i("DEBUG", "Set appdata saved file done.");
					fOut.close();
				}
			}catch (Exception ex)
			{
				Log.i("DEBUG", "Cannot saved appdata file");
			}
		}
		else{
			Log.i("DEBUG", "can not open or create cloud save file: " + file.getAbsoluteFile());
		}
	}

	/**
	 * Conflict resolution for when Snapshots are opened.  Must be run in an AsyncTask or in a
	 * background thread,
	 */
	Snapshot processSnapshotOpenResult(Snapshots.OpenSnapshotResult result, int retryCount) {
		Snapshot mResolvedSnapshot = null;
		if(retryCount == 0)
			mSnapshotResolveConflictRetryCount = 0;
		else
			mSnapshotResolveConflictRetryCount++;

		int status = result.getStatus().getStatusCode();
		Log.i("DEBUG", "Save Result status: " + status);

		if (status == GamesStatusCodes.STATUS_OK) {
			return result.getSnapshot();
		} else if (status == GamesStatusCodes.STATUS_SNAPSHOT_CONTENTS_UNAVAILABLE) {
			return result.getSnapshot();
		} else if (status == GamesStatusCodes.STATUS_SNAPSHOT_CONFLICT) {
			Snapshot snapshot = result.getSnapshot();
			Snapshot conflictSnapshot = result.getConflictingSnapshot();

			// Resolve between conflicts by selecting the newest of the conflicting snapshots.
			mResolvedSnapshot = snapshot;

			if (snapshot.getMetadata().getLastModifiedTimestamp() <
					conflictSnapshot.getMetadata().getLastModifiedTimestamp()) {
				mResolvedSnapshot = conflictSnapshot;
			}

			Snapshots.OpenSnapshotResult resolveResult = Games.Snapshots.resolveConflict(
					CurrentClient, result.getConflictId(), mResolvedSnapshot).await();

			if (retryCount < MAX_SNAPSHOT_RESOLVE_RETRIES) {
				// Recursively attempt again
				return processSnapshotOpenResult(resolveResult, mSnapshotResolveConflictRetryCount);
			} else {
				// Failed, log error and show Toast to the user
				Log.i("DEBUG", "Could not resolve snapshot conflicts");
			}

		}

		// Fail, return null.
		return null;
	}

	private void displaySnapshotMetadata(SnapshotMetadata metadata) {
		String metadataStr = "Source: Saved Games" + '\n'
				+ "Description: " + metadata.getDescription() + '\n'
				+ "Name: " + metadata.getUniqueName() + '\n'
				+ "Last Modified: " + String.valueOf(metadata.getLastModifiedTimestamp()) + '\n'
				+ "Played Time: " + String.valueOf(metadata.getPlayedTime()) + '\n';

		Log.i("DEBUG", metadataStr);
	}

	public void loadGooglePlayFriends()
	{
		try {
			// load google friend
			Log.i("DEBUG", "Call load google friend!");
			Games.Players.loadConnectedPlayers(CurrentClient, true).setResultCallback(new ResultCallback<Players.LoadPlayersResult>(){
				@Override
				public void onResult(Players.LoadPlayersResult loadPlayersResult) {
					if (loadPlayersResult.getStatus().getStatusCode() == GamesStatusCodes.STATUS_OK) {
						PlayerBuffer players = loadPlayersResult.getPlayers();
						Log.i("DEBUG", "Load google friend success! num: " + players.getCount());

						for (Player player : players) {
							Log.i("DEBUG", "Friend: " + player.getDisplayName() + " id: " + player.getPlayerId() + "\n");
							String id = player.getPlayerId();
							mFriendIds.add(id);
						}
						onGetGooglePlayFriendDone();
					}
					else
					{
						Log.i("DEBUG", "Load friend fail with code: " + loadPlayersResult.getStatus().getStatusCode());
					}
				}
			});
		} catch (Exception ex){
			Log.i("DEBUG", "Load google friend fail with exception: " + ex.getMessage());
		}
	}

	public void getGooglePlayFriendIds (ActionGooglePlayFriendsArg arg)
	{
		if (mFriendIds.size() > 0)
		{
			arg._friend_ids = mFriendIds;
			arg.onDone();
		}
		else
		{
			_waitting_arg.add(arg);
			if (CurrentClient != null && CurrentClient.isConnected())
			{
				loadGooglePlayFriends();
			}
		}
	}

	private void onGetGooglePlayFriendDone()
	{
		ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();

		for(int s = 0; s< _waitting_arg.size(); s++)
		{
			ActionArg arg = _waitting_arg.get(s);
			if(arg instanceof ActionGooglePlayFriendsArg)
			{
				ActionGooglePlayFriendsArg fbarg = (ActionGooglePlayFriendsArg) arg;
				fbarg._friend_ids = mFriendIds;
				tmp.add(arg);
			}
		}

		for(int i = 0; i< tmp.size(); i++){
			ActionArg arg = tmp.get(i);
			_waitting_arg.remove(arg);
			arg.onDone();
		}
	}

	public void getLeaderBoardRank(ActionGooglePlayGetLeaderboardRankArg arg)
	{
		//if (!AppContext.is_debug) {
			Log.i("DEBUG", "Begin call getLeaderBoardRank from googleplayservice!");
			PendingResult result = Games.Leaderboards.loadCurrentPlayerLeaderboardScore(CurrentClient,
					arg.LeaderBoardId, LeaderboardVariant.TIME_SPAN_ALL_TIME,
					LeaderboardVariant.COLLECTION_PUBLIC);
			Log.i("DEBUG", "getLeaderBoardRank on waiting result!");

			final ActionGooglePlayGetLeaderboardRankArg fArg = arg;
			result.setResultCallback(new ResultCallback<Leaderboards.LoadPlayerScoreResult>() {
				@Override
				public void onResult(Leaderboards.LoadPlayerScoreResult result) {
					Log.i("DEBUG", "Get leaderboard rank on result!");
					// check if valid score
					if (result != null
							&& GamesStatusCodes.STATUS_OK == result.getStatus().getStatusCode()
							&& result.getScore() != null) {
						try {
							fArg.CurrentRank = (int)result.getScore().getRank();
							Log.i("DEBUG", "current rank = " + fArg.CurrentRank);
							fArg.onDone();
						}
						catch (Exception ex)
						{
							Log.i("DEBUG", "Get leaderboard rank parse result fail: " + ex.getMessage());
							fArg.onCancel();
						}
					} else {
						fArg.onCancel();
					}
				}
			});
		/*}
		else
		{
			Log.i("DEBUG", "Not get leader board rank in debug build");
		}*/
	}

	public String getCurrentUserId()
	{
		return mCurrentUserID;
	}

	public String getCurrentUserName()
	{
		return mCurrentUserName;
	}

	public void showSendIntent(int type, String desc, Bitmap sendIcon) {
		// Make sure we have a valid API client.
		if (CurrentClient != null && CurrentClient.isConnected()) {
			String description;
			int intentCode;
			Bitmap icon = null;
			int id;
			Drawable d;
			switch (type) {
				case GameRequest.TYPE_GIFT:
					//description = getString(R.string.send_gift_description);
					intentCode = ActivityCallback.SEND_GIFT_CODE;
					try {
						id = CurrentActivity.getResources().getIdentifier("ic_send_gift", "drawable", CurrentActivity.getPackageName());
						d = CurrentActivity.getResources().getDrawable(id, null);
						icon = ((BitmapDrawable) d).getBitmap();
					}catch (Exception e)
					{
						Log.i ("DEBUG", "Get icon send gift fail with exception: " + e.getMessage());
					}
					break;
				case GameRequest.TYPE_WISH:
					//description = getString(R.string.send_request_description);
					intentCode = ActivityCallback.SEND_REQUEST_CODE;
					try {
						id = CurrentActivity.getResources().getIdentifier("ic_send_request", "drawable", CurrentActivity.getPackageName());
						d = CurrentActivity.getResources().getDrawable(id, null);
						icon = ((BitmapDrawable) d).getBitmap();
					}catch (Exception e)
					{
						Log.i ("DEBUG", "Get icon send gift fail with exception: " + e.getMessage());
					}
					break;
				default:
					return;
			}
			try {
				Intent intent = Games.Requests.getSendIntent(CurrentClient, type,
						"".getBytes(), 1, icon, desc);
				CurrentActivity.startActivityForResult(intent, intentCode);
			} catch (Exception e) {
				Log.i("DEBUG", "Show send gift dialog fail with exception: " + e.getMessage());
			}
		} else {
			Log.i("DEBUG", "Send gift fail. Reason: CurrentClient is not available!");
		}
	}

	public void showInboxGiftIntent ()
	{
		if (CurrentClient != null && CurrentClient.isConnected()) {
			CurrentActivity.startActivityForResult(Games.Requests.getInboxIntent(CurrentClient), ActivityCallback.GIFT_REQUEST_INBOX);
		}else {
			Log.i("DEBUG", "Send gift fail. Reason: CurrentClient is not available!");
		}
	}

	public int handleRequests(ArrayList<GameRequest> requests) {
		if (requests == null || requests.size() <= 0) {
			return 0;
		}
		readAcceptedUsers();
		final ArrayList<GameRequest> theRequests = requests;
		ArrayList<GameRequest> bannedRequests = new ArrayList<GameRequest>();

		int count = 0;

		for (int i = 0; i < theRequests.size(); i ++) {
			GameRequest request = theRequests.get(i);
			if (request.getType() == GameRequest.TYPE_WISH)
			{
				// We only need to lock gift receive
				continue;
			}
			boolean isExist = false;
			//Log.i("DEBUG", "Sender Name: " + request.getSender().getDisplayName());
			for (int j = 0; j < mUserNameList.size(); j++)
			{
				//Log.i("DEBUG", "Sender Name in saved: " + mUserNameList.get(j) + " time: " + mUserTimeAcceptList.get(j));
				if (request.getSender().getDisplayName().equals(mUserNameList.get(j)))
				{
					//Log.i("DEBUG", "Has saved. System time: " + System.currentTimeMillis());
					if( System.currentTimeMillis() - mUserTimeAcceptList.get(j) >= 24*60*60*1000)
					{
						//Log.i("DEBUG", "Has saved and time OK");
						mUserTimeAcceptList.set(j, System.currentTimeMillis());
						count ++;
					}
					else {
						//Log.i("DEBUG", "Has saved and time NOT OK");
						bannedRequests.add(request);
					}
					isExist = true;
					break;
				}
			}
			if (!isExist)
			{
				Log.i("DEBUG", "Not Saved");
				mUserNameList.add(request.getSender().getDisplayName());
				mUserTimeAcceptList.add(System.currentTimeMillis());
				count ++;
			}
		}
		writeAcceptedUsers();
		acceptRequests(theRequests);

		if (bannedRequests.size() > 0)
		{
			String message = "You'd already recieved gift from: ";
			for (int i = 0; i < bannedRequests.size(); i ++)
			{
				GameRequest request = bannedRequests.get(i);
				message += request.getSender().getDisplayName();
			}
			message += " today! Please wait for 24 hours!";
			Toast.makeText(CurrentActivity, message,
					Toast.LENGTH_LONG).show();
		}
		Log.i("DEBUG", "Num gift received: " + count);
		return count;
		/*// Must have final for anonymous function
		final ArrayList<GameRequest> theRequests = requests;

		AlertDialog.Builder builder = new AlertDialog.Builder(CurrentActivity, android.R.style.Theme_Material_Light_Dialog_Alert);
		builder.setMessage(getRequestsString(requests))
				.setPositiveButton("Absolutely!",
						new DialogInterface.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int id) {
								acceptRequests(theRequests);
							}
						})
				.setNegativeButton("No thanks",
						new DialogInterface.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int id) {
								// Do nothing---requests will remain un-created.
							}
						});
		// Create the AlertDialog object and return it
		builder.create().show();*/
	}

	public void readAcceptedUsers()
	{
		boolean isCancel = false;
		try
		{
			String dataPath = "data/data/" + GetDataPath();
			FileInputStream f = new FileInputStream(dataPath + "/" + "fg.p");
			if (f != null) {
				DataInputStream fIn = new DataInputStream(f);
				if (fIn != null) {
					mUserNameList.clear();
					mUserTimeAcceptList.clear();
					int num = fIn.readInt();
					for (int i = 0; i < num; i++) {
						mUserNameList.add(fIn.readUTF());
						mUserTimeAcceptList.add(fIn.readLong());
					}
					fIn.close();
				}else {
					Log.i("DEBUG", "cannot read: " + dataPath + "fg.p");
				}
				f.close();
			}else {
				Log.i("DEBUG", "cannot read: " + dataPath + "fg.p");
			}
		}
		catch (IOException e)
		{
			System.out.println("isUserCancelLogin IOException : " + e);
		}
	}

	public void writeAcceptedUsers()
	{
		try
		{
			int size = mUserNameList.size();
			if (size <= 0) {
				return;
			}
			String dataPath = "data/data/" + GetDataPath();
			FileOutputStream f = new FileOutputStream(dataPath + "/" + "fg.p", false);

			if (f != null) {
				DataOutputStream fOut = new DataOutputStream(f);
				if (fOut != null) {
					fOut.writeInt(size);
					for (int i = 0; i < size; i++) {
						fOut.writeUTF(mUserNameList.get(i));
						fOut.writeLong(mUserTimeAcceptList.get(i));
					}
					fOut.close();
					//Log.i("DEBUG", "setUserCancelLogin: " + );
				} else {
					Log.i("DEBUG", "cannot write to : " + dataPath + "fg.p");
				}
				f.close();
			}else {
				Log.i("DEBUG", "cannot write to : " + dataPath + "fg.p");
			}
		}
		catch (Exception e)
		{
			System.out.println("setUserCancelLogin Exception : " + e);
		}
	}

	// Actually accepts the requests
	private void acceptRequests(ArrayList<GameRequest> requests) {
		// Attempt to accept these requests.
		ArrayList<String> requestIds = new ArrayList<String>();

		/**
		 * Map of cached game request ID to its corresponding game request
		 * object.
		 */
		final HashMap<String, GameRequest> gameRequestMap = new HashMap<String, GameRequest>();

		// Cache the requests.
		for (GameRequest request : requests) {
			String requestId = request.getRequestId();
			requestIds.add(requestId);
			gameRequestMap.put(requestId, request);

			Log.i("DEBUG", "Processing request " + requestId);
		}
		// Accept the requests.
		Games.Requests.acceptRequests(CurrentClient, requestIds).setResultCallback(
				new ResultCallback<Requests.UpdateRequestsResult>() {
					@Override
					public void onResult(Requests.UpdateRequestsResult result) {
						int numGifts = 0;
						int numRequests = 0;
						// Scan each result outcome.
						for (String requestId : result.getRequestIds()) {
							// We must have a local cached copy of the request
							// and the request needs to be a
							// success in order to continue.
							if (!gameRequestMap.containsKey(requestId)
									|| result.getRequestOutcome(requestId) != Requests.REQUEST_UPDATE_OUTCOME_SUCCESS) {
								continue;
							}
							// Update succeeded here. Find the type of request
							// and act accordingly. For wishes, a
							// responding gift will be automatically sent.
							switch (gameRequestMap.get(requestId).getType()) {
								case GameRequest.TYPE_GIFT:
									// Toast the player!
									++numGifts;
									break;
								case GameRequest.TYPE_WISH:
									++numRequests;
									break;
							}
						}

						if (numGifts != 0) {
							// Toast our gifts.
						}
						if (numGifts != 0 || numRequests != 0) {
							// if the user accepted any gifts or requests,
							// update
							// the displayed counts
							updateRequestCounts();
						}
					}
				});
	}

	// Called back after you load the current requests
	private final ResultCallback<Requests.LoadRequestsResult> mLoadRequestsCallback =
			new ResultCallback<Requests.LoadRequestsResult>() {
				@Override
				public void onResult(Requests.LoadRequestsResult result) {
					int giftCount = countNotExpired(result.getRequests(GameRequest.TYPE_GIFT));
					int wishCount = countNotExpired(result.getRequests(GameRequest.TYPE_WISH));
					Log.i ("DEBUG", "giftCount = " + giftCount + " " + " wishCount = " +  wishCount);
				}
			};

	// Count GameRequests in a GameRequestBuffer that have not yet expired
	private int countNotExpired(GameRequestBuffer buf) {
		if (buf == null) {
			return 0;
		}

		int giftCount = 0;
		for (GameRequest gr : buf) {
			if (gr.getExpirationTimestamp() > System.currentTimeMillis()) {
				giftCount++;
			}
		}
		return giftCount;
	}

	// Changes the numbers at the top of the layout
	private void updateRequestCounts() {
		PendingResult<Requests.LoadRequestsResult> result = Games.Requests
				.loadRequests(CurrentClient,
						Requests.REQUEST_DIRECTION_INBOUND,
						GameRequest.TYPE_ALL,
						Requests.SORT_ORDER_EXPIRING_SOON_FIRST);
		result.setResultCallback(mLoadRequestsCallback);
	}

	private String getRequestsString(ArrayList<GameRequest> requests) {
		if (requests.size() == 0) {
			return "You have no requests to accept.";
		}

		if (requests.size() == 1) {
			return "Do you want to accept this request from "
					+ requests.get(0).getSender().getDisplayName() + "?";
		}

		StringBuffer retVal = new StringBuffer(
				"Do you want to accept the following requests?\n\n");

		for (GameRequest request : requests) {
			retVal.append("  A "
					+ (request.getType() == GameRequest.TYPE_GIFT ? "gift"
					: "game request") + " from "
					+ request.getSender().getDisplayName() + "\n");
		}

		return retVal.toString();
	}
	// This shows how to set up a listener for requests received. It is not
	// necessary; it only is useful if you do not want the default notifications
	// to happen when someone sends a request to someone.
	private final OnRequestReceivedListener mRequestListener = new OnRequestReceivedListener() {

		/*
         * (non-Javadoc)
         *
         * @see com.google.android.gms.games.request.OnRequestReceivedListener#
         * onRequestReceived(com.google.android.gms.games.request.GameRequest)
         */
		@Override
		public void onRequestReceived(GameRequest request) {
			String requestStringResource = "You have just received a ";
			switch (request.getType()) {
				case GameRequest.TYPE_GIFT:
					requestStringResource += "gift ";
					break;
				case GameRequest.TYPE_WISH:
					requestStringResource += "request ";
					break;
				default:
					return;
			}
			requestStringResource += "from " + request.getSender().getDisplayName() + ".";
			Toast.makeText(CurrentActivity, requestStringResource,
					Toast.LENGTH_LONG).show();
			updateRequestCounts();
		}

		/*
         * (non-Javadoc)
         *
         * @see com.google.android.gms.games.request.OnRequestReceivedListener#
         * onRequestRemoved(java.lang.String)
         */
		@Override
		public void onRequestRemoved(String requestId) {
			updateRequestCounts();
		}
	};
}