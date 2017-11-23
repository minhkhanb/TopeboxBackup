package topebox.core.Firebase;


import android.app.Activity;
import android.util.Log;

import topebox.core.AppConfig;

public class FirebaseManager
{
	public static FirebaseServices currentFirebaseInstance;
	public static Activity currentActivity;

	public static void init(Activity activity)
	{
		currentActivity = activity;
		currentFirebaseInstance = new FirebaseServices(activity);

		if (AppConfig.UseFirebaseAnalystic)
		{
			currentFirebaseInstance.initAnalytics();
		}
		if (AppConfig.UseFirebaseRemoteConfig)
		{
			currentFirebaseInstance.initRemoteConfig();
		}
	}

	public static boolean FirebaseAnalyticsLogEvent(String itemId, String itemName, String itemType, String itemValue)
	{
		if (AppConfig.UseFirebaseAnalystic)
		{
			return currentFirebaseInstance.logEvent(itemId, itemName, itemType, itemValue);
		}
		else
		{
			Log.i("DEBUG", "Firebase analytics is on turn off! see string.xml");
			return false;
		}
	}

	public static boolean FirebaseAnalyticsSetUserProperty(String propertyId, String value)
	{
		if (AppConfig.UseFirebaseAnalystic)
		{
			return currentFirebaseInstance.setUserProperty(propertyId, value);
		}
		else
		{
			Log.i("DEBUG", "Firebase analytics is on turn off! see string.xml");
			return false;
		}
	}

	// Remote config
	public static boolean FireBaseRemoteConfigGetBoolean(String strId, boolean default_value)
	{
		if (AppConfig.UseFirebaseRemoteConfig)
		{
			return currentFirebaseInstance.GetBoolean(strId, default_value);
		}
		else
		{
			Log.i("DEBUG", "Firebase Remote config is on turn off! see string.xml");
			return default_value;
		}
	}

	public static long FireBaseRemoteConfigGetLong(String strId, long default_value)
	{
		if (AppConfig.UseFirebaseRemoteConfig)
		{
			return currentFirebaseInstance.GetLong(strId, default_value);
		}
		else
		{
			Log.i("DEBUG", "Firebase Remote config is on turn off! see string.xml");
			return default_value;
		}
	}

	public static String FireBaseRemoteConfigGetString(String strId, String default_value)
	{
		if (AppConfig.UseFirebaseRemoteConfig)
		{
			return currentFirebaseInstance.GetString(strId, default_value);
		}
		else
		{
			Log.i("DEBUG", "Firebase Remote config is on turn off! see string.xml");
			return default_value;
		}
	}
}