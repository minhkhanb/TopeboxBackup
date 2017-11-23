//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.FileNameMap;
import java.util.*;

import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
import android.provider.ContactsContract;
import android.util.Log;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.view.View;
import android.widget.ImageButton;

import android.view.View.OnClickListener;
import android.widget.ImageView;


import org.json.*;
import org.json.JSONObject;
import org.json.JSONException;

import com.facebook.*;
import com.facebook.login.*;
import com.facebook.FacebookSdk;
import com.facebook.share.model.*;
import com.facebook.share.widget.*;

import topebox.core.Actions.*;

import java.io.ByteArrayOutputStream;
import android.graphics.Bitmap;
import android.os.AsyncTask;
import java.net.URL;
import java.net.URI;

import static topebox.core.OsFunctions.GetDataPath;
import static topebox.core.OsFunctions.GetResourcePath;


public class FacebookManager
{
	static public String AccountName = "";
	static public String AccountId = "";

	static public String FriendName = "";
	static public int NumWaitLoadFriendName;
	static public ArrayList<FacebookFriendInfo> friend_infos = new ArrayList<FacebookFriendInfo>();
	
	static CallbackManager _callback_mng;
	static ProfileTracker _profile_tracker;
	static AccessTokenTracker _token_tracker;
	static AccessToken _token;
	
	static ArrayList<ActionArg> _waitting_arg = new ArrayList<ActionArg>();
	
	static Profile _profile;
	
	public static final List<String> PermissionsList = Arrays.asList("publish_stream","publish_actions");
	public static final List<String> PermissionsRead = Arrays.asList("user_about_me","read_stream","user_friends","user_photos");

	public static void init(Context context)
	{

		FacebookSdk.sdkInitialize(((Activity)context).getApplicationContext());
		
		_callback_mng = CallbackManager.Factory.create();

		//Log.i("DEBUG", "facebook debug");
		
		LoginManager.getInstance().registerCallback(_callback_mng,
			new FacebookCallback<LoginResult>() {
				@Override
				public void onSuccess(LoginResult loginResult) {
					// App code
					Log.i("DEBUG", "login facebook success!");
					//get infomation
					_profile = Profile.getCurrentProfile();
					_token_tracker = new AccessTokenTracker() {
						@Override
						protected void onCurrentAccessTokenChanged(
							AccessToken oldAccessToken,
							AccessToken currentAccessToken) {
								// Set the access token using 
								// currentAccessToken when it's loaded or set.
								//Log.i("DEBUG", "onCurrentAccessTokenChanged");
								_token = currentAccessToken;
						}
						
						
					};
					if(_profile != null)
					{
						AccountName = _profile.getName();
						AccountId = _profile.getId();
						if (FacebookManager.AccountId.length() > 0) {
							OsFunctions.setFacebookInfo(AccountId, AccountName);
						}
						ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();
					
						for(int s = 0; s< _waitting_arg.size(); s++)
						{
							ActionArg arg = _waitting_arg.get(s);
							if(arg instanceof ActionFacebookLoginArg)
							{
								ActionFacebookLoginArg fbarg = (ActionFacebookLoginArg) arg;
								fbarg.UserName = _profile.getName();
								fbarg.UserId = _profile.getId();
								tmp.add(arg);
							}
						}
						
						if(tmp.size() > 0) Log.i("DEBUG", "facebook test this");
						
						for(int i = 0; i< tmp.size(); i++){
							ActionArg arg = tmp.get(i);
							_waitting_arg.remove(arg);
							arg.onDone();
						}
					}
					else
					{
						Log.i("DEBUG", "facebook login success but profile NULLLLL!");
					}
				}

				@Override
				public void onCancel() 
				{
					
					 // App code
					Log.i("DEBUG", "login facebook cancel");
					//showAlert();
					ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();
					
					for(int s = 0; s< _waitting_arg.size(); s++)
					{
						ActionArg arg = _waitting_arg.get(s);
						if(arg instanceof ActionFacebookLoginArg)
						{
							arg.onCancel();
							tmp.add(arg);
						}
					}
					
					for(int i = 0; i< tmp.size(); i++){
						_waitting_arg.remove(tmp.get(i));
					}
					
				}

				@Override
				public void onError(FacebookException exception) 
				{
					 // App code
					Log.i("DEBUG", "login error:" + exception);
					//showAlert();
					ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();
					
					for(int s = 0; s< _waitting_arg.size(); s++)
					{
						ActionArg arg = _waitting_arg.get(s);
						if(arg instanceof ActionFacebookLoginArg)
						{
							arg.onCancel();
							tmp.add(arg);
						}
					}
					
					for(int i = 0; i< tmp.size(); i++){
						_waitting_arg.remove(tmp.get(i));
					}
				}
				/*
				private void showAlert() {
                        new AlertDialog.Builder(((Activity)context).getApplicationContext())
								.setTitle(R.string.cancelled)
                                .setMessage(R.string.permission_not_granted)
                                .setPositiveButton(R.string.ok, null)
                                .show();
                }*/
			}
		);
		
		_profile_tracker = new ProfileTracker() 
		{
			@Override
			protected void onCurrentProfileChanged(Profile oldProfile, Profile currentProfile) {
				// App code
				Log.i("DEBUG","profile tracked");
				Log.i("DEBUG", "id:" + currentProfile.getId());
				Log.i("DEBUG", "name:" + currentProfile.getName());
				Log.i("DEBUG", "linkUri:" + currentProfile.getLinkUri());

				AccountName = currentProfile.getName();
				AccountId = currentProfile.getId();

				if (FacebookManager.AccountId.length() > 0) {
					OsFunctions.setFacebookInfo(AccountId, AccountName);
				}
				
				//Log.i("DEBUG", "login cancel");
				ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();
				
				for(int s = 0; s< _waitting_arg.size(); s++)
				{
					ActionArg arg = _waitting_arg.get(s);
					if(arg instanceof ActionFacebookLoginArg)
					{
						ActionFacebookLoginArg fbarg = (ActionFacebookLoginArg) arg;
						fbarg.UserName = currentProfile.getName();
						fbarg.UserId = currentProfile.getId();
						
						tmp.add(arg);
					}
				}
				
				if(tmp.size() > 0) Log.i("DEBUG", "test this 2");
				
				for(int i = 0; i< tmp.size(); i++)
				{
					ActionArg arg = tmp.get(i);
					_waitting_arg.remove(arg);
					arg.onDone();
				}
			}
		};
		
		
		_token_tracker = new AccessTokenTracker() {
			@Override
			protected void onCurrentAccessTokenChanged(
				AccessToken oldAccessToken,
				AccessToken currentAccessToken) {
					// Set the access token using 
					// currentAccessToken when it's loaded or set.
					//Log.i("DEBUG", "onCurrentAccessTokenChanged");
					_token = currentAccessToken;
			}
			
			
		};
		// If the access token is available already assign it.
		_token = AccessToken.getCurrentAccessToken();
		
		if(_token != null)
		{
			Log.i("DEBUG", "facebook loged in");
			_profile = Profile.getCurrentProfile();
			
			if(_profile != null)
			{
				AccountName = _profile.getName();
				AccountId = _profile.getId();
				Log.i("DEBUG","facebook profile trackeda");
				Log.i("DEBUG", "facebook id:" + _profile.getId());
				Log.i("DEBUG", "facebook name:" + _profile.getName());
				Log.i("DEBUG", "facebook linkUri:" + _profile.getLinkUri());
			}else {
				Log.i("DEBUG", "facebook cannot get profile!!");
			}
		}
    }
	
	
	public static void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		_callback_mng.onActivityResult(requestCode, resultCode, data);
	}

	public static String GetCurrentAccessToken ()
	{
		if (_token == null)
		{
			Log.i("DEBUG", "Facebook access token is null");
			return "";
		}
		return _token.getToken();
	}
	
	public static void login(ActionFacebookLoginArg arg)
	{
		Log.i("DEBUG", "FACEBOOK LOGIN");
		_waitting_arg.add(arg);
		((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
				public void run() {
					LoginManager.getInstance().logInWithReadPermissions((Activity)AppContext.CurrentContext, Arrays.asList("public_profile", "user_friends"));
				}
			});
	}
	
	public static void downloadAvatar(ActionFacebookDownloadAvatarArg arg)
	{
		Log.i("DEBUG", "facebook LOAD AVATAR...");
		_waitting_arg.add(arg);
		final String userId = arg.userURL;
		final String fileName = arg.fileName;
		final int imgWidth = arg.width;
		final int imgHeight = arg.height;
		Log.i("DEBUG", "facebook LOAD AVATAR URL " + userId);
		Log.i("DEBUG", "facebook LOAD AVATAR save to file with name " + fileName);
		Log.i("DEBUG", "facebook LOAD AVATAR width" + imgWidth);
		Log.i("DEBUG", "facebook LOAD AVATAR height" + imgHeight);
		AsyncTask.execute(new Runnable() {
			public void run() {
				URL bitmapURL;
				try {
					bitmapURL = new URL("http://graph.facebook.com/" + userId +
							"/picture?redirect=false&width=" + imgWidth + "&height=" + imgHeight);
					InputStream bitmapURLInputStream = bitmapURL.openConnection().getInputStream();
					BufferedReader r = new BufferedReader(new InputStreamReader(bitmapURLInputStream));
					StringBuilder bitmapURLString = new StringBuilder();
					String line;
					while ((line = r.readLine()) != null) {
						bitmapURLString.append(line);
					}
					try {
						JSONObject obj = new JSONObject(bitmapURLString.toString());
						JSONObject jsonObject = obj.getJSONObject("data");
						String imageURLString = jsonObject.getString("url");
						URL imageURL = new URL(imageURLString);
						Log.i("DEBUG", "facebook LOAD AVATAR real imageURL: " + imageURL);
						Bitmap bmpImage = BitmapFactory.decodeStream(imageURL.openConnection().getInputStream());
						boolean isSuccess = false;
						if (bmpImage != null) {
							try {
								String dataPath = "data/data/" + GetDataPath();
								Log.i("DEBUG", "facebook LOAD AVATAR dataPath " + dataPath);
								OutputStream fOut = null;
								File file = new File(dataPath, fileName);
								if (!file.exists()) {
									fOut = new FileOutputStream(file);
									bmpImage.compress(Bitmap.CompressFormat.PNG, 100, fOut);
									fOut.flush();
									fOut.close();
									Log.i("DEBUG", "facebook LOAD AVATAR saved image in " + file.getAbsolutePath());
								} else {
									Log.i("DEBUG", "facebook LOAD AVATAR " + file.getAbsolutePath() + "already existed");
								}
								isSuccess = true;
							}catch (Exception e) {
								Log.e("ERROR", "Downloading facebook avatar fail when process save image with exception:" + e.getMessage());
							}
						}
						else{
							Log.i("DEBUG", "facebook LOAD AVATAR bitmap data is null");
						}
						for (int s = 0; s < _waitting_arg.size(); s++) {
							ActionArg arg = _waitting_arg.get(s);
							if (arg instanceof ActionFacebookDownloadAvatarArg) {
								if (isSuccess) {
									arg.onDone();
								}
								else{
									arg.onCancel();
								}
							}
						}
					} catch (Exception e) {
						Log.e("ERROR", "Downloading facebook avatar fail when process JSON file & decode with exception:" + e.getMessage());
					}
				} catch (Exception e) {
					Log.e("ERROR", "Downloading facebook avatar fail when building URL with exception: " + e.getMessage());
				}
			}
		});
	}
	
	public static void getAppFriends(ActionFacebookAppFriendsArg arg){
		
		_waitting_arg.add(arg);
		Log.i("DEBUG", "getAppFriends");
		NumWaitLoadFriendName = 0;
		GraphRequest request = GraphRequest.newMyFriendsRequest(
        _token,
        new GraphRequest.GraphJSONArrayCallback() {
            @Override
            public void onCompleted(
                   JSONArray objects,
                   GraphResponse response) {
                // Application code
				
				boolean success = response.getError() == null;
				
				if(!success) Log.i("DEBUG",response.getError().getErrorMessage());

				if(success)
				{
					try 
					{
						Log.i("INFO", response.toString());
						if (objects.length() > 0) {
							for (int i = 0; i < objects.length(); i++) {
								JSONObject obj = objects.getJSONObject(i);
								long id = Long.parseLong(obj.getString("id"));
								GetFriendName(id);
							}
						}
						else
						{
							Log.i("DEBUG", "not have any friend");
						}
					} 
					catch (JSONException e) 
					{
						Log.i("DEBUG", "parser Error");
						success = false;
					}
				}


				if (!success || objects.length() <= 0) {
					ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();
					for(int s = 0; s< _waitting_arg.size(); s++)
					{
						ActionArg arg = _waitting_arg.get(s);
						if(arg instanceof ActionFacebookAppFriendsArg)
						{
							tmp.add(arg);
						}
					}

					for(int i = 0; i< tmp.size(); i++){
						_waitting_arg.remove(tmp.get(i));
					}
					for (int i = 0; i < tmp.size(); i++) {
							tmp.get(i).onCancel();
					}
				}
            }
        });
		Bundle parameters = new Bundle();
		parameters.putString("fields", "id, installed");
		request.setParameters(parameters);
		request.executeAsync();
	}

	public static void GetFriendName(long friendId)
	{
		final long id = friendId;
		NumWaitLoadFriendName ++;
		new GraphRequest(
				AccessToken.getCurrentAccessToken(),
				"/" + id,
				null,
				HttpMethod.GET,
				new GraphRequest.Callback() {
					public void onCompleted(GraphResponse response) {
						NumWaitLoadFriendName --;
						try {
							//Log.i("DEBUG", "GraphResponse: " + response.toString());
							JSONObject obj = response.getJSONObject();
							//Log.i("DEBUG", "JSONObject: " + obj.toString());
							FriendName = obj.getString("name");
							FacebookFriendInfo friend_info = new FacebookFriendInfo();
							friend_info._id = id;
							friend_info._name = FriendName;
							Log.i("DEBUG", "facebook LOAD Friend id: " + id + " name: " + FriendName);
							friend_infos.add(friend_info);
						} catch (Exception e) {
							Log.e("ERROR", "Downloading facebook name fail when process JSON file & decode with exception:" + e.getMessage());
						}
						if (NumWaitLoadFriendName <= 0)
						{
							ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();
							for(int s = 0; s< _waitting_arg.size(); s++)
							{
								ActionArg arg = _waitting_arg.get(s);
								if(arg instanceof ActionFacebookAppFriendsArg)
								{
									tmp.add(arg);
								}
							}

							for(int i = 0; i< tmp.size(); i++){
								_waitting_arg.remove(tmp.get(i));
							}
							for(int i = 0; i< tmp.size(); i++)
							{
								ActionFacebookAppFriendsArg fbarg = (ActionFacebookAppFriendsArg) tmp.get(i);
								fbarg._friend_list = friend_infos;
							}
							for (int i = 0; i < tmp.size(); i++) {
								tmp.get(i).onDone();
							}
							Log.i("DEBUG", "get friends success");
						}
					}
				}
		).executeAsync();
	}
	
	public static void post(ActionFacebookPostScreenArg arg)
	{
		_waitting_arg.add(arg);
		GraphRequest request = GraphRequest.newPostRequest(_token,
                "me/feeds", null, new GraphRequest.Callback() {
			@Override
			public void onCompleted(GraphResponse  response) {

				boolean success = response.getError() == null;	
				ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();
				
				for(int s = 0; s< _waitting_arg.size(); s++)
				{
					ActionArg arg = _waitting_arg.get(s);
					if(arg instanceof ActionFacebookPostScreenArg)
					{
						ActionFacebookPostScreenArg fbarg = (ActionFacebookPostScreenArg) arg;
						tmp.add(arg);
					}
				}
				
				for(int i = 0; i< tmp.size(); i++)
				{
					ActionArg arg = tmp.get(i);
					_waitting_arg.remove(arg);
					if(success)
						arg.onDone();
					else
						arg.onCancel();
				}
			}
		});
				
				
		Bundle postParams = new Bundle();
		postParams.putString("name", arg.Name);
        postParams.putString("caption", arg.Caption);
        postParams.putString("description", arg.Description);
        postParams.putString("link", arg.Link);
        //postParams.putString("picture", arg.Picture);
		

        request.setParameters(postParams);

        request.executeAsync();
	}
	
	public static void postPicture(ActionFacebookPostScreenArg arg)
	{
		_waitting_arg.add(arg);
		
		Bitmap screen = AppContext.CurrentRegistry.getScreenShotBitmap();
		if(screen != null){	
			GraphRequest request = GraphRequest.newPostRequest(AccessToken.getCurrentAccessToken(),
				"me/photos", null, new GraphRequest.Callback() {
				@Override
				public void onCompleted(GraphResponse  response) {
					Log.i("DEBUG", "postPicture Completed");
					ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();
					boolean success = false;
					if (response.getError() != null)
					{
						Log.i("DEBUG", "Facebook postPicture error with code: " + response.getError());				
					}
					else
					{
						success = true;
					}
					
					for(int s = 0; s< _waitting_arg.size(); s++)
					{
						ActionArg arg = _waitting_arg.get(s);
						if(arg instanceof ActionFacebookPostScreenArg)
						{
							ActionFacebookPostScreenArg fbarg = (ActionFacebookPostScreenArg) arg;
							tmp.add(arg);
						}
					}
					
					for(int i = 0; i< tmp.size(); i++)
					{
						ActionArg arg = tmp.get(i);
						_waitting_arg.remove(arg);
						
						if(success)
							arg.onDone();
						else
							arg.onCancel();
					}
				}
			});
			
			byte[] data = null;
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			screen.compress(Bitmap.CompressFormat.JPEG, 100, baos);
			data = baos.toByteArray();

			Bundle params = new Bundle();
			//params.putString("method", "photos.upload");
			params.putByteArray("picture", baos.toByteArray());
			params.putString("name", arg.Name);
			params.putString("caption", arg.Caption);
			params.putString("description", arg.Description);
			params.putString("link", arg.Link);
			Log.i("DEBUG", "postPicture message: " + arg.Caption);

			request.setParameters(params);

			request.executeAsync();
		
			/*
			String dataPath = "data/data/" + GetDataPath();
			Log.i("DEBUG", "facebook LOAD AVATAR dataPath " + dataPath);
			OutputStream fOut = null;
			File file = new File(dataPath, "FacebookShareImage");
			fOut = new FileOutputStream(file);
			screen.compress(Bitmap.CompressFormat.JPEG, 100, fOut);
			fOut.flush();
			fOut.close();

			if (ShareDialog.canShow(ShareLinkContent.class)) {
				ShareLinkContent linkContent = new ShareLinkContent.Builder()
						.setImageUrl(Uri.parse(file.getAbsolutePath()))
						.setContentTitle(arg.Caption)
						.setContentDescription(arg.Description)
						.setContentUrl(Uri.parse(arg.Link))
						.build();

				((Activity)arg.CurrentContext).shareDialog.show(linkContent);
			}*/
		}
		else
		{
			Log.i("DEBUG", "getScreenShotBitmap is null!!!");
		}
	}
	
	public static void OpenAppInvite(ActionFacebookAppInviteArg arg)
	{
		if (AppInviteDialog.canShow()) 
		{
			AppInviteContent content = new AppInviteContent.Builder()
						.setApplinkUrl(arg.AppLinkUrl)
						.setPreviewImageUrl(arg.PreviewImageUrl)
						.build();
			AppInviteDialog.show((Activity)arg.CurrentContext, content);
			arg.onDone();
		}
		else
		{
			arg.onCancel();
		}
	}
}