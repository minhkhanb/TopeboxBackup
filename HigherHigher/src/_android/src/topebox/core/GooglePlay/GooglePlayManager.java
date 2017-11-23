package topebox.core.GooglePlay;

import topebox.core.ActivityCallback;
import topebox.core.Actions.*;
import topebox.core.UpdateCloudSaveResultCode;

import android.util.Log;
import android.app.Activity;
import android.app.Dialog;
import android.content.Intent;

//google play
import com.google.android.gms.auth.api.Auth;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInResult;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.snapshot.SnapshotMetadata;
import com.google.android.gms.games.snapshot.Snapshots;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.games.GamesActivityResultCodes;
//import com.google.firebase.analytics.FirebaseAnalytics;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Random;


public class GooglePlayManager{
	static public boolean IsAvailable = false;
	static public boolean IsConnected = false;

	static private GoogleSignInAccount mCurrentAccount;
	static private ArrayList<String> mFriendIds = new ArrayList<String>();
	static private ArrayList<ActionArg> _lst_waiting_args = new ArrayList<ActionArg>();
	static private GooglePlayService CurrentClient;
	static public void init(Activity activity){
		CurrentClient = new GooglePlayService(activity);
		int result = GooglePlayServicesUtil.isGooglePlayServicesAvailable(activity);
		if(result == ConnectionResult.SUCCESS){
			Log.i("DEBUG", "Google Play Service connect Success");
			IsAvailable = true;
		}else{
			Log.i("DEBUG", "Google Play Service connect not success:" +result );
			switch(result){
			case ConnectionResult.SERVICE_MISSING:{
				Dialog dialog = GooglePlayServicesUtil.getErrorDialog(result, activity, ActivityCallback.GOOGLE_PLAY_SERVICE_MISSING);
				if(dialog != null){
					dialog.show();
				}
			}break;
			case ConnectionResult.SERVICE_VERSION_UPDATE_REQUIRED:{
				Dialog dialog = GooglePlayServicesUtil.getErrorDialog(result, activity, ActivityCallback.GOOGLE_PLAY_SERVICE_UPDATE_REQUEST);
				if(dialog != null){
					dialog.show();
				}
			}break;
			case ConnectionResult.SERVICE_DISABLED:{
				Dialog dialog = GooglePlayServicesUtil.getErrorDialog(result, activity, ActivityCallback.GOOGLE_PLAY_SERVICE_DISABLE);
				if(dialog != null){
					dialog.show();
				}
			}break;
			/*case ConnectionResult.SIGN_IN_REQUIRED:{
				ConnectionResult conn3 = new ConnectionResult(
						ConnectionResult.SIGN_IN_REQUIRED, null);
				try {
					conn3.startResolutionForResult((Activity)context, GOOGLE_PLAY_SERVICE_SIGN_IN_CODE);
				} catch (SendIntentException e) {
					e.printStackTrace();
				}
			}break;
			case ConnectionResult.INVALID_ACCOUNT:{
				ConnectionResult conn6 = new ConnectionResult(
						ConnectionResult.INVALID_ACCOUNT, null);
				try {
					conn6.startResolutionForResult((Activity)context,
							GOOGLE_PLAY_SERVICE_INVALID_ACCT_CODE);
				} catch (SendIntentException e) {
					e.printStackTrace();
				}
			}break;
			case ConnectionResult.RESOLUTION_REQUIRED:{
				ConnectionResult conn7 = new ConnectionResult(
						ConnectionResult.RESOLUTION_REQUIRED, null);
				try {
					conn7.startResolutionForResult((Activity)context,
							GOOGLE_PLAY_SERVICE_RESOLUTION_CODE);
				} catch (SendIntentException e) {
					e.printStackTrace();
				}
			}break;
			*/
			}
		}
	}

	static public boolean IsServiceAvailable(){
		//return CurrentClient.IsAvailableToUse;
		return CurrentClient.isConnected();
	}
	static public void connect(){
		if(IsAvailable){
			CurrentClient.connect();
		}
	}
	
	static public void connect(ActionGooglePlayLoginArg arg){
		if(IsAvailable){
			CurrentClient.connect(arg);
		}
		else{
			arg.onCancel();
			Log.i("DEBUG", "Google Play Services is not available");
		}
			
	}
	
	static public void resolveCallback(int request_code, int result, Intent data){
		ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();
		switch(request_code){
			case ActivityCallback.GOOGLE_PLAY_SERVICE_MISSING:
			break;
			
			case ActivityCallback.GOOGLE_PLAY_SERVICE_UPDATE_REQUEST:
			break;
			
			case ActivityCallback.GOOGLE_PLAY_SERVICE_DISABLE:
			break;
			
			case ActivityCallback.GOOGLE_PLAY_SERVICE_COMMON_RESOLVE:
				if(result == GamesActivityResultCodes.RESULT_SIGN_IN_FAILED){
					//sign in false
					CurrentClient.cancelConnect();
					Log.i("DEBUG", "resolveCallback for GOOGLE_PLAY_SERVICE_COMMON_RESOLVE RESULT_SIGN_IN_FAILED");
				}else if(result == GamesActivityResultCodes.RESULT_APP_MISCONFIGURED){
					Log.i("DEBUG", "MisConfigured");
				}else if(result == Activity.RESULT_OK){
					if(IsAvailable){
						CurrentClient.connect();
						Log.i("DEBUG", "resolveCallback for GOOGLE_PLAY_SERVICE_COMMON_RESOLVE ok call reconnect");
					}
					//Log.i("DEBUG", "Try submit score");
					//CurrentClient.submitScore("CgkI3vju06QDEAIQAA", 10000);
				}
				else{		//also fail
					CurrentClient.cancelConnect();
				}
			break;
			case ActivityCallback.GOOGLE_PLAY_SERVICE_SHOW_LEADER_BOARD:
			case ActivityCallback.GOOGLE_PLAY_SERVICE_SHOW_ACHIEVEMENTS:
				if (result == GamesActivityResultCodes.RESULT_RECONNECT_REQUIRED)		//User sign out from Google API UI
				{
					Log.i("DEBUG", "Sign out google play");
					CurrentClient.disConnect();
				}
			break;
			case ActivityCallback.GOOGLE_PLAY_SERVICE_SAVED_GAMES:
			if (data != null) {
				if (data.hasExtra(Snapshots.EXTRA_SNAPSHOT_METADATA)) {
					// Load a snapshot.
					SnapshotMetadata snapshotMetadata = (SnapshotMetadata)
							data.getParcelableExtra(Snapshots.EXTRA_SNAPSHOT_METADATA);
					CurrentClient.mSnapshotName = snapshotMetadata.getUniqueName();
					
					// Load the game data from the Snapshot
					CurrentClient.savedGamesLoad(CurrentClient.mSnapshotName);
				} else if (data.hasExtra(Snapshots.EXTRA_SNAPSHOT_NEW)) {
					// Create a new snapshot named with a unique string
					String unique = new BigInteger(281, new Random()).toString(13);
					CurrentClient.mSnapshotName = "snapshotTemp-" + unique;

					// Create the new snapshot
					// ...
				}
				Log.i("DEBUG", "Save game activity result callback");
			}
			break;
			case ActivityCallback.RC_SIGN_IN:
				GoogleSignInResult signInresult = Auth.GoogleSignInApi.getSignInResultFromIntent(data);
				if (signInresult.isSuccess()) {
					mCurrentAccount = signInresult.getSignInAccount();
					Log.i("DEBUG", "Sign in google service successful get account info: User ID: " + mCurrentAccount.getId().toString() + " Display Name: " + mCurrentAccount.getDisplayName());
				}
				break;
			case ActivityCallback.RC_GOOGLE_PLUS_SHARE:
				Log.i("DEBUG", "Google plus share screen result call back: " + result);
				if (result == -1) //success
				{
					for(int s = 0; s< _lst_waiting_args.size(); s++)
					{
						ActionArg arg = _lst_waiting_args.get(s);
						if(arg instanceof ActionGooglePlayShareScreenToGooglePlusArg)
						{
							tmp.add(arg);
						}
					}

					if(tmp.size() > 0) Log.i("DEBUG", "Has pending ActionGooglePlayShareScreenToGooglePlusArg");

					for(int i = 0; i< tmp.size(); i++){
						ActionArg arg = tmp.get(i);
						_lst_waiting_args.remove(arg);
						arg.onDone();
					}
				}
				else
				{
					for(int s = 0; s< _lst_waiting_args.size(); s++)
					{
						ActionArg arg = _lst_waiting_args.get(s);
						if(arg instanceof ActionGooglePlayShareScreenToGooglePlusArg)
						{
							tmp.add(arg);
						}
					}

					if(tmp.size() > 0) Log.i("DEBUG", "Has pending ActionGooglePlayShareScreenToGooglePlusArg");

					for(int i = 0; i< tmp.size(); i++){
						ActionArg arg = tmp.get(i);
						_lst_waiting_args.remove(arg);
						arg.onCancel();
					}
				}
			break;
			case ActivityCallback.SEND_GIFT_CODE:
				for(int s = 0; s< _lst_waiting_args.size(); s++)
				{
					ActionArg arg = _lst_waiting_args.get(s);
					if(arg instanceof ActionGooglePlaySendGiftArg)
					{
						tmp.add(arg);
					}
				}

				if(tmp.size() > 0) Log.i("DEBUG", "Has wating callback ActionGooglePlaySendGiftArg");

				if (result == GamesActivityResultCodes.RESULT_SEND_REQUEST_FAILED)
				{
					for(int i = 0; i< tmp.size(); i++){
						ActionArg arg = tmp.get(i);
						_lst_waiting_args.remove(arg);
						arg.onCancel();
					}
				}
				else
				{
					for(int i = 0; i< tmp.size(); i++){
						ActionArg arg = tmp.get(i);
						_lst_waiting_args.remove(arg);
						arg.onDone();
					}
				}
				break;
			case ActivityCallback.SEND_REQUEST_CODE:
				for(int s = 0; s< _lst_waiting_args.size(); s++)
				{
					ActionArg arg = _lst_waiting_args.get(s);
					if(arg instanceof ActionGooglePlaySendGiftArg)
					{
						tmp.add(arg);
					}
				}

				if(tmp.size() > 0) Log.i("DEBUG", "Has wating callback ActionGooglePlaySendGiftArg");

				if (result == GamesActivityResultCodes.RESULT_SEND_REQUEST_FAILED)
				{
					for(int i = 0; i< tmp.size(); i++){
						ActionArg arg = tmp.get(i);
						_lst_waiting_args.remove(arg);
						arg.onCancel();
					}
				}
				else
				{
					for(int i = 0; i< tmp.size(); i++){
						ActionArg arg = tmp.get(i);
						_lst_waiting_args.remove(arg);
						arg.onDone();
					}
				}
				break;
			case ActivityCallback.GIFT_REQUEST_INBOX:
				for(int s = 0; s< _lst_waiting_args.size(); s++)
				{
					ActionArg arg = _lst_waiting_args.get(s);
					if(arg instanceof ActionGooglePlaySendGiftArg)
					{
						tmp.add(arg);
					}
				}
				if(tmp.size() > 0) Log.i("DEBUG", "Has wating callback ActionGooglePlaySendGiftArg");
				if (result == Activity.RESULT_OK && data != null) {
					int numGift = CurrentClient.handleRequests(Games.Requests
							.getGameRequestsFromInboxResponse(data));
					for(int i = 0; i< tmp.size(); i++){
						ActionArg arg = tmp.get(i);
						_lst_waiting_args.remove(arg);
						ActionGooglePlaySendGiftArg arg1 = (ActionGooglePlaySendGiftArg)arg;
						arg1._returnGift = numGift;
						arg1.onDone();
					}
				} else {
					Log.i("DEBUG", "Failed to process inbox result: resultCode = "
							+ result + ", data = "
							+ (data == null ? "null" : "valid"));
					for(int i = 0; i< tmp.size(); i++){
						ActionGooglePlaySendGiftArg arg = (ActionGooglePlaySendGiftArg)tmp.get(i);
						_lst_waiting_args.remove(arg);
						arg.onCancel();
					}
				}
				break;
		}
	}
	static public boolean submitScore(String board_id, int score){
		if(CurrentClient.IsAvailableToUse){
			CurrentClient.submitScore(board_id, score);
			return true;
		}
		return false;
	}
	
	static public void showLeaderBoard(String board_id){
		if(CurrentClient.IsAvailableToUse){
			CurrentClient.showLeaderBoard(board_id);
		}else {
			Log.i("DEBUG", "google play services is not connected");
		}
	}
	
	static public void showAchievements(){
		if(CurrentClient.IsAvailableToUse){
			CurrentClient.showAchievements();
		}else {
			Log.i("DEBUG", "google play services is not connected");
		}
	}
	
	static public void unlockAchievement(String achi_id){
		if(CurrentClient.IsAvailableToUse){
			CurrentClient.unlockAchievement(achi_id);
		}
	}
	
	static public void setUserCancelLogin(boolean userChoose)
	{
		if(CurrentClient != null){
			CurrentClient.setUserCancelLogin(userChoose);
		}
	}

	static public boolean isUserCancelLogin()
	{
		if(CurrentClient != null){
			return CurrentClient.isUserCancelLogin();
		}
		return false;
	}
	
	static public int updateCloudSave(String localSaveName)
	{
		if(CurrentClient != null && CurrentClient.IsAvailableToUse){
			return (int)CurrentClient.savedGamesUpdate(localSaveName);
		}
		else{
			return UpdateCloudSaveResultCode.GOOGLE_UPDATE_CLOUD_SAVE_FAIL;
		}
	}

	static public String getCurrentUserId ()
	{
		if(CurrentClient != null && CurrentClient.IsAvailableToUse){
			return CurrentClient.getCurrentUserId();
		}
		else
		{
			Log.i("DEBUG", "Get user id fail! Not loged in yet");
			return "";
		}
	}

	static public String getCurrentUserName ()
	{
		if(CurrentClient != null && CurrentClient.IsAvailableToUse){
			return CurrentClient.getCurrentUserName();
		}
		else
		{
			Log.i("DEBUG", "Get user id fail! Not loged in yet");
			return "";
		}
	}

	static public void getGooglePlayFriendIds (ActionGooglePlayFriendsArg arg)
	{
		if (CurrentClient != null && CurrentClient.IsAvailableToUse)
		{
			CurrentClient.getGooglePlayFriendIds(arg);
		}
		else
		{
			Log.i("DEBUG", "Get google play friend ids fail. Reason: google play service is not init or not connected");
		}
	}

	static public void shareScreenShotToGooglePlus (ActionGooglePlayShareScreenToGooglePlusArg arg)
	{
		_lst_waiting_args.add(arg);
		ActionGooglePlayShareScreenToGooglePlus action = new ActionGooglePlayShareScreenToGooglePlus(arg);
		action.act();
	}

	static public void showSendIntent(ActionGooglePlaySendGiftArg arg)
	{
		if (arg._type > 0) {
			_lst_waiting_args.add(arg);
			ActionGooglePlaySendGift action = new ActionGooglePlaySendGift(arg);
			action.act();
			if (arg._type == 1 || arg._type == 2)
			{
				CurrentClient.showSendIntent(arg._type, arg._message, arg._icon);
			}
			else if (arg._type == 3)
			{
				CurrentClient.showInboxGiftIntent();
			}
		}
		else
		{
			Log.i ("DEBUG", "Send gift type = " + arg._type + " is not supported!");
		}
	}

	static public void showInboxGiftIntent(ActionGooglePlayInboxGiftShowArg arg)
	{
		_lst_waiting_args.add(arg);
		ActionGooglePlayInboxGiftShow action = new ActionGooglePlayInboxGiftShow(arg);
		action.act();
		CurrentClient.showInboxGiftIntent();
	}

	static public void getLeaderBoardRank(ActionGooglePlayGetLeaderboardRankArg arg)
	{
		if(CurrentClient != null && CurrentClient.IsAvailableToUse) {
			CurrentClient.getLeaderBoardRank(arg);
		}
		else
		{
			Log.i ("DEBUG", "Get current leaderboard rank fail because of google play is not available!");
		}
	}
}