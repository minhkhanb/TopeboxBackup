package topebox.core.Firebase;


import android.app.Activity;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.util.Log;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.analytics.FirebaseAnalytics;
import com.google.firebase.remoteconfig.FirebaseRemoteConfig;
import com.google.firebase.remoteconfig.FirebaseRemoteConfigSettings;


import topebox.core.AppConfig;
import topebox.game.higherhigher.BuildConfig;

class FirebaseServices
{
	private Activity CurrentActivity;
	private FirebaseAnalytics mFirebaseAnalytics;
	private FirebaseRemoteConfig mFirebaseRemoteConfig;
	private boolean _is_fetch_FBRC_successful;

	public FirebaseServices(Activity activity){
		CurrentActivity = activity;
	}

	// Analytic

	public void initAnalytics()
	{
		mFirebaseAnalytics = FirebaseAnalytics.getInstance(CurrentActivity.getBaseContext());
		mFirebaseAnalytics.setAnalyticsCollectionEnabled(true);
	}

	public boolean setUserProperty(String propertyId, String value)
	{
		if (AppConfig.UseFirebaseAnalystic) {
			if (mFirebaseAnalytics == null) {
				Log.i("DEBUG", "Firebase analytics not instance is null!");
				return false;
			}
			try {
				if (propertyId != null && value != null) {
					mFirebaseAnalytics.setUserProperty(propertyId, value);
					return true;
				}
				else{
					Log.i("DEBUG", "Firebase analytics set user property is null!");
					return false;
				}
			} catch (Exception ex) {
				Log.i("DEBUG", "Firebase logevent fail with exception: " + ex.getMessage());
				return false;
			}
		}
		return false;
	}

	public boolean logEvent (String itemId, String itemName, String itemType, String itemValue) {
		if (AppConfig.UseFirebaseAnalystic) {
			if (mFirebaseAnalytics == null) {
				Log.i("DEBUG", "Firebase analytics not instance is null!");
				return false;
			}
			try {
				Bundle bundle = new Bundle();
				if (itemName != null)
				{
					//bundle.putString(FirebaseAnalytics.Param.ITEM_NAME, itemName);
					bundle.putString("Description", itemName);
				}
				if(itemValue != null)
				{
					bundle.putString(FirebaseAnalytics.Param.VALUE, itemValue);
				}
				if (itemId != null)
				{
					bundle.putString(FirebaseAnalytics.Param.CONTENT_TYPE, itemId);
				}
				if (itemId != null)
				{
					if (itemType != null && !itemType.isEmpty()){
						bundle.putString(FirebaseAnalytics.Param.ITEM_ID, itemId);
						String str_eventName = itemType.replace(" ", "_");
						mFirebaseAnalytics.logEvent(str_eventName, bundle);
					}
					else {
						String str_eventName = itemId.replace(" ", "_");
						mFirebaseAnalytics.logEvent(str_eventName, bundle);
					}
				}
				//mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.SELECT_CONTENT, bundle);
				return true;
			} catch (Exception ex) {
				Log.i("DEBUG", "Firebase logevent fail with exception: " + ex.getMessage());
				return false;
			}
		}
		return false;
	}

	// Remote config
	public void initRemoteConfig()
	{
		_is_fetch_FBRC_successful = false;
		mFirebaseRemoteConfig = FirebaseRemoteConfig.getInstance();
		FirebaseRemoteConfigSettings configSettings = new FirebaseRemoteConfigSettings.Builder()
				.setDeveloperModeEnabled(BuildConfig.DEBUG)
				.build();
		mFirebaseRemoteConfig.setConfigSettings(configSettings);
		long cacheExpiration = 3600; // 1 hour in seconds.
		if (mFirebaseRemoteConfig.getInfo().getConfigSettings().isDeveloperModeEnabled()) {
			cacheExpiration = 0;
		}
		mFirebaseRemoteConfig.fetch(cacheExpiration)
				.addOnCompleteListener(CurrentActivity, new OnCompleteListener<Void>() {
					@Override
					public void onComplete(@NonNull Task<Void> task) {
						if (task.isSuccessful()) {
							Log.i("DEBUG", "Firebase remote config Fetch Succeeded");
							// After config data is successfully fetched, it must be activated before newly fetched
							// values are returned.
							mFirebaseRemoteConfig.activateFetched();
							_is_fetch_FBRC_successful = true;
						} else {
							Log.i("DEBUG", "Firebase remote config Fetch fail");
						}
					}
				});
	}

	public boolean GetBoolean (String strId, boolean default_value)
	{
		if (_is_fetch_FBRC_successful)
		{
			try {
				boolean result = mFirebaseRemoteConfig.getBoolean(strId);
				Log.i("DEBUG", "mFirebaseRemoteConfig GetBoolean: " + result);
				return result;
			} catch (Exception ex) {
				Log.i("DEBUG", "Get boolean from FB remote config fail: " + ex.getMessage());
				return default_value;
			}
		}
		else {
			Log.i("DEBUG", "Get " + strId + " (type=boolean) from FB remote config fail because FBRC fetch fail return default = " + default_value);
			return default_value;
		}
	}

	public long GetLong(String strId, long default_value)
	{
		if (_is_fetch_FBRC_successful)
		{
			try {
				long result = mFirebaseRemoteConfig.getLong(strId);
				Log.i("DEBUG", "mFirebaseRemoteConfig GetLong: " + result);
				return result;
			} catch (Exception ex) {
				Log.i("DEBUG", "Get boolean from FB remote config fail: " + ex.getMessage());
				return default_value;
			}
		}
		else {
			Log.i("DEBUG", "Get " + strId + " (type=long) from FB remote config fail because FBRC fetch fail return default = " + default_value);
			return default_value;
		}
	}

	public String GetString(String strId, String default_value)
	{
		if (_is_fetch_FBRC_successful) {
			try {
				String result = mFirebaseRemoteConfig.getString(strId);
				Log.i("DEBUG", "mFirebaseRemoteConfig GetString: " + result);
				return result;
			} catch (Exception ex) {
				Log.i("DEBUG", "Get boolean from FB remote config fail: " + ex.getMessage());
				return default_value;
			}
		}
		else
		{
			Log.i("DEBUG", "Get " + strId + " (type=String) from FB remote config fail because FBRC fetch fail return default = " + default_value);
			return default_value;
		}
	}
}