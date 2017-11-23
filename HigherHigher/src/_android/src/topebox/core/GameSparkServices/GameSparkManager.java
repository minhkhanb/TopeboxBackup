package topebox.core.GameSparkServices;


import android.app.Activity;
import android.util.Log;

import com.gamesparks.sdk.GSEventConsumer;
import com.gamesparks.sdk.android.GSAndroidPlatform;
import com.gamesparks.sdk.api.autogen.GSResponseBuilder;

import java.util.ArrayList;

import topebox.core.Actions.ActionArg;

public class GameSparkManager{

	static public String mCurrentUserId = "";
	private String mCurrentUserName = "";
	private String mCurrentPassword = "";
	private String mCurrentDisplayName = "";
	private String mCurrentSocialLoginToken = "";

	static private ArrayList<String> mFriendIds = new ArrayList<String>();
	static private ArrayList<ActionArg> _lst_waiting_args = new ArrayList<ActionArg>();

	static private GameSparkServices mGameSparkServicesInstance;

	static public void init(Activity activity, String apiKey, String apiSecret){
		mGameSparkServicesInstance = new GameSparkServices(activity, apiKey, apiSecret);
	}

	static public boolean isInit()
	{
		if (mGameSparkServicesInstance != null) return true;
		return false;
	}

	static public void CreateNewUser(ActionArg arg, final String userName, String password, final String displayName)
	{
		if (mGameSparkServicesInstance == null)
		{
			Log.i("DEBUG", "Game spark is not init yet!");
			return;
		}
		mGameSparkServicesInstance.CreateNewUser(arg, userName, password, displayName);
	}

	static public void LoginWithDeviceId(ActionArg arg, String deviceId, String osInfo)
	{
		if (mGameSparkServicesInstance == null)
		{
			Log.i("DEBUG", "Game spark is not init yet!");
			return;
		}

		mGameSparkServicesInstance.LoginWithDeviceId(arg, deviceId, osInfo);
	}

	static public void LinkFacebookAccount (ActionArg arg, String accessToken, boolean isLinkToCurrentPlayer)
	{
		if (mGameSparkServicesInstance == null)
		{
			Log.i("DEBUG", "Game spark is not init yet!");
			return;
		}
		mGameSparkServicesInstance.LinkFacebookAccount(arg, accessToken, isLinkToCurrentPlayer);
	}

	static public void LinkGooglePlusAccount (ActionArg arg, String accessToken, boolean isLinkToCurrentPlayer)
	{
		if (mGameSparkServicesInstance == null)
		{
			Log.i("DEBUG", "Game spark is not init yet!");
			return;
		}
		mGameSparkServicesInstance.LinkGooglePlusAccount(arg, accessToken, isLinkToCurrentPlayer);
	}

	static public void start ()
	{
		if (mGameSparkServicesInstance == null)
		{
			Log.i("DEBUG", "Game spark is not init yet!");
			return;
		}
		mGameSparkServicesInstance.start();
	}

	static public void stop ()
	{
		if (mGameSparkServicesInstance == null)
		{
			Log.i("DEBUG", "Game spark is not init yet!");
			return;
		}
		mGameSparkServicesInstance.stop();
	}

	static public void UploadSave (ActionArg arg, String saveFileName)
	{
		if (mGameSparkServicesInstance == null)
		{
			Log.i("DEBUG", "Game spark is not init yet!");
			return;
		}
		mGameSparkServicesInstance.UploadSave(arg, saveFileName);
	}
}