//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core;
import android.content.Context;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.content.res.Configuration;
import android.util.Log;
import android.app.Activity;

//net work
import android.net.ConnectivityManager;
import android.content.IntentFilter;
import android.content.Intent;

import topebox.core.Actions.ActionShareScreenShotIntentArg;
import topebox.core.Firebase.FirebaseManager;
import topebox.core.GameSparkServices.GameSparkManager;
import topebox.core.GooglePlay.*;

//chartboost
// import com.chartboost.sdk.CBLocation;
// import com.chartboost.sdk.Chartboost;
// import com.chartboost.sdk.ChartboostDelegate;
// import com.chartboost.sdk.Libraries.CBLogging.Level;
// import com.chartboost.sdk.Model.CBError.CBClickError;
// import com.chartboost.sdk.Model.CBError.CBImpressionError;

// unity3d ads
// import com.unity3d.ads.android.UnityAds;
// import topebox.core.Unity.*;

//vungle ads
// import com.vungle.publisher.VunglePub;
// import com.vungle.publisher.VungleInitListener;
// import com.vungle.publisher.AdConfig;

public class AppContext
{
	//class callback for google play
	
	static public Context CurrentContext;
	static public Bundle CurrentSavedInstanceState;
	static public AssetManager CurrentAssetManager;
	static public AppRegistry CurrentRegistry = new AppRegistry();
	static public TranformableRelativeLayout CurrentContentView;
	static public GLJNISurfaceView GLView;
	
	static ConnectivityChangeReceiver ReceiverConnectivity = new ConnectivityChangeReceiver();

	// static public AdsListener CurrentUnityAdsListener = new AdsListener();

	// get the VunglePub instance
	// static public VunglePub vunglePub = null;
	static public String placementIdForLevel = "videoReward";
	// static public AdConfig globalAdConfig;

	static public boolean is_debug = false;

	static public ActionShareScreenShotIntentArg _action_share_intent_arg;
	
	static public void Init(Context context, Bundle savedInstanceState)
	{
		CurrentContext = context;
		CurrentSavedInstanceState = savedInstanceState;
		CurrentAssetManager = context.getResources().getAssets();
		CurrentRegistry.clean();
		
		//OsFunctions.Init();

		//init facebook
		Log.i("DEBUG", "init FACEBOOK");
		if(AppConfig.UseFacebook) {
			FacebookManager.init(context);

			if (FacebookManager.AccountId.length() > 0) {
				OsFunctions.setFacebookInfo(FacebookManager.AccountId, FacebookManager.AccountName);
			}
		}

		//reg connection change receiver

		//call native init
		nativeInitContext(CurrentAssetManager);
		if(AppConfig.UseAdmob)
		{
			AdmobManager.init((Activity)context);
			if (AppConfig.UseAdmobInterstitial)
			{
				AdmobManager.initAdmobInterstitial();
			}
			if (AppConfig.UseAdmobRewardedVideo) {
				AdmobManager.initAdmobRewardedVideo();
			}
		}

		if(AppConfig.UseGooglePlay)
		{
			GooglePlayManager.init((Activity)context);
		}

		//init chartboost
		if(AppConfig.UseChartBoost)
		{
			try
			{
				//Log.i("DEBUG", "init chartboost:" + AppConfig.ChartBoostAppId);
				/*Chartboost.startWithAppId((Activity)context, AppConfig.ChartBoostAppId, AppConfig.ChartBoostAppSign);

				Chartboost.setLoggingLevel(Level.ALL);
				//Chartboost.setImpressionsUseActivities(true);
				Chartboost.setAutoCacheAds(true);
				Chartboost.setDelegate(_chartboost_delegate);
				Chartboost.onCreate((Activity)context);*/

			}
			catch(Exception ex)
			{
				Log.i("DEBUG","cb init fail: " + ex.getMessage());
			}
		}

		//init unity ads
		if(AppConfig.UseUnityAds)
		{
			try
			{
				/*UnityAdsManager.init((Activity)context);
				UnityAds.init((Activity)context, AppConfig.UnityAdsId, CurrentUnityAdsListener);
				if (is_debug)
				{
					UnityAds.setDebugMode(true);
					UnityAds.setTestMode(true);
				}*/
				Log.i("DEBUG","unity init success ");
			}
			catch(Exception ex)
			{
				Log.i("DEBUG","unity init fail: " + ex.getMessage());
			}
		}

		//init vungle ads
		if(AppConfig.UseVungleAds)
		{
			try
			{
				/*vunglePub = VunglePub.getInstance();
				VungleAdsManager.init((Activity)context);
				vunglePub.init((Activity)context, AppConfig.VungleAdsId, new String[]{placementIdForLevel}, new VungleInitListener() {
					@Override
					public void onSuccess() {
						// get a reference to the global AdConfig object
						globalAdConfig = vunglePub.getGlobalAdConfig();
						// For a full description of available options, see the 'Config Object'section.
						globalAdConfig.setSoundEnabled(true);
						vunglePub.clearAndSetEventListeners(VungleAdsManager.vungleListener);
					}
					@Override
					public void onFailure(Throwable e){
						Log.i("DEBUG","vungle ads init fail! ");
					}
				});*/
			}
			catch(Exception ex)
			{
				Log.i("DEBUG","vungle ads init fail: " + ex.getMessage());
			}
		}

		if (AppConfig.UseFirebaseAnalystic)
		{
			FirebaseManager.init((Activity)context);
		}

		if (AppConfig.UseGameSparkServices)
		{
			GameSparkManager.init((Activity)CurrentContext, AppConfig.GameSparkApiKey, AppConfig.GameSparkApiSecret);
		}
		
		CurrentContentView = new TranformableRelativeLayout((Activity)context);
	}

	static public void onConfigurationChanged(Configuration newConfig){
		if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
			//setContentView(R.layout.yourxmlinlayout-land);
			Log.i("DEBUG","Change view to langcape");
			Game.ChangeOrientation(1);
		} else if (newConfig.orientation == Configuration.ORIENTATION_PORTRAIT){
			//setContentView(R.layout.yourxmlinlayoutfolder);
			Log.i("DEBUG","Change view to portrait");
			Game.ChangeOrientation(0);
		}
	}
	
	static public void onAppStart()
	{
		Log.i("DEBUG", "onAppStart");
		//CurrentContext.registerReceiver(ReceiverConnectivity,
		//	new IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION)
		//);
		if(AppConfig.UseGooglePlay && GooglePlayManager.IsAvailable){
			GooglePlayManager.connect();
		}

		//if(AppConfig.UseChartBoost)
		{
			/*Chartboost.onStart((Activity)CurrentContext);*/
			//Chartboost.cacheInterstitial(CBLocation.LOCATION_MAIN_MENU);
			//Chartboost.cacheRewardedVideo(CBLocation.LOCATION_MAIN_MENU);
		}

		if (AppConfig.UseGameSparkServices)
		{
			GameSparkManager.stop();
			GameSparkManager.start();
		}
	}
	
	static public void onAppPause()
	{
		//GLView.onPause();
		CurrentContext.unregisterReceiver(ReceiverConnectivity);

		if(AppConfig.UseAdmob) {
			AdmobManager.onPause();
		}
		
		//if(AppConfig.UseChartBoost)
		{
			/*Chartboost.onPause((Activity)CurrentContext);*/
		}

		if(AppConfig.UseVungleAds)
		{
			/*vunglePub.onPause();*/
		}
	}
	
	static public void onAppResume()
	{
		//GLView.onResume();
		CurrentContext.registerReceiver(ReceiverConnectivity, 
			new IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION)
		);

		if(AppConfig.UseAdmob) {
			AdmobManager.onResume();
		}
		
		//if(AppConfig.UseChartBoost)
		{
			/*Chartboost.onResume((Activity)CurrentContext);*/
		}
		
		if(AppConfig.UseUnityAds)
		{
			/*UnityAds.changeActivity((Activity)CurrentContext);*/
		}

		if(AppConfig.UseVungleAds)
		{
			/*vunglePub.onResume();*/
		}

		if(AppConfig.UseLocalNotification)
		{
			//PineNotificationManager.cancelNotification();
		}
	}
	
	static public void onAppStop()
	{
		//CurrentContext.unregisterReceiver(ReceiverConnectivity);
		Game.Suppend();
		
		//if(AppConfig.UseChartBoost)
		{
			/*Chartboost.onStop((Activity)CurrentContext);*/
		}

		if (AppConfig.UseGameSparkServices)
		{
			GameSparkManager.stop();
		}
	}
	
	static public void onAppDestroy()
	{
		if(AppConfig.UseAdmob) {
			AdmobManager.onDestroy();
		}

		//if(AppConfig.UseChartBoost)
		{
			/*Chartboost.onDestroy((Activity)CurrentContext);*/
		}
		if (AppConfig.UseVungleAds)
		{
			/*vunglePub.clearEventListeners();*/
		}
	}
	
	static public boolean onBackKeyPress()
	{
		//if(AppConfig.UseChartBoost)
		{
			/*if(Chartboost.onBackPressed())
			{
				return true;
			}*/
		}
		
		return Game.onBackKeyPress();
	}
	
	static public void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		if(AppConfig.UseFacebook) {
			FacebookManager.onActivityResult(requestCode, resultCode, data);
		}
	}
	
	//ChartBoost Delegate
	// static private ChartboostDelegate _chartboost_delegate = new ChartboostDelegate() {
		// @Override
		// public boolean shouldRequestInterstitial(String location) {	
			// return true;
		// }
		// @Override
		// public boolean shouldDisplayInterstitial(String location) {
			// return true;
		// }
		// @Override
		// public void didCacheInterstitial(String location) {
		// }
		// @Override
		// public void didFailToLoadInterstitial(String location, CBImpressionError error) {
		// }
		// @Override
		// public void didDismissInterstitial(String location) {
		// }
		// @Override
		// public void didCloseInterstitial(String location) {
		// }
		// @Override
		// public void didClickInterstitial(String location) {
		// }
		// @Override
		// public void didDisplayInterstitial(String location) {
		// }
		// @Override
		// public boolean shouldRequestMoreApps(String location) {
			// return true;
		// }
		// @Override
		// public boolean shouldDisplayMoreApps(String location) {
			// return true;
		// }
		// @Override
		// public void didFailToLoadMoreApps(String location, CBImpressionError error) {
		// }
		// @Override
		// public void didCacheMoreApps(String location) {
		// }
		// @Override
		// public void didDismissMoreApps(String location) {
		// }
		// @Override
		// public void didCloseMoreApps(String location) {
		// }
		// @Override
		// public void didClickMoreApps(String location) {
		// }
		// @Override
		// public void didDisplayMoreApps(String location) {
		// }
		// @Override
		// public void didFailToRecordClick(String uri, CBClickError error) {
		// }
		// @Override
		// public boolean shouldDisplayRewardedVideo(String location) {
			// return true;
		// }
		// @Override
		// public void didCacheRewardedVideo(String location) {
		// }
		// @Override
		// public void didFailToLoadRewardedVideo(String location,
				// CBImpressionError error) {
		// }
		// @Override
		// public void didDismissRewardedVideo(String location) {
		// }
		// @Override
		// public void didCloseRewardedVideo(String location) {
		// }
		// @Override
		// public void didClickRewardedVideo(String location) {
		// }
		// @Override
		// public void didCompleteRewardedVideo(String location, int reward) {
		// }
		// @Override
		// public void didDisplayRewardedVideo(String location) {
		// }
		// @Override
		// public void willDisplayVideo(String location) {
		// }
	// };
	
	static public native void nativeInitContext(AssetManager asset);
}