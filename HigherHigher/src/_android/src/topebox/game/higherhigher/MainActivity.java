package topebox.game.higherhigher;
import topebox.core.*;
import topebox.core.GooglePlay.*;
import topebox.core.Actions.*;
import topebox.game.Notification.TopeboxNotificationManager;

import android.Manifest;
import android.content.pm.PackageInfo;
import android.content.res.Configuration;
import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.util.Base64;
import android.util.Log;
import android.content.Intent;
import android.content.res.Resources;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.*;
import android.content.pm.ActivityInfo;
import android.view.Gravity;
import android.view.WindowManager;
import android.content.pm.PackageManager;
//
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.*;

//google play
import com.gamesparks.sdk.android.GSAndroidPlatform;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.common.api.GoogleApiClient;

public class MainActivity extends Activity
{
    /** Called when the activity is first created. */
	static boolean is_init = false;
	static public boolean is_open = false;
	static{
		if(Build.VERSION.SDK_INT < Build.VERSION_CODES.KITKAT)
		{
			System.loadLibrary("gnustl_shared");
		}
        System.loadLibrary("topebox_framework");
    }

	topebox.core.GLJNISurfaceView GLView;
	topebox.core.GLJNIRenderer GLRenderer;
	
	private GoogleApiClient mGoogleApiClient;
	
	public void init(){
	}

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
		Log.i("DEBUG", "onCreate");
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		
		// Init Pine Core Application Context
		Resources res = getResources();
		AppConfig.UseFacebook = res.getBoolean(R.bool.use_facebook);
		AppConfig.UseAdmob = res.getBoolean(R.bool.use_admob);
		AppConfig.StringAdmobApplicationId = res.getString(R.string.admob_application_id);
		AppConfig.UseAdmobInterstitial = res.getBoolean(R.bool.use_admob_interstitial);
		AppConfig.StringAdmobInterstitialUnitId = res.getString(R.string.ad_interstitial_unit_id);
		AppConfig.UseAdmobBanner = res.getBoolean(R.bool.use_admob_banner);
		AppConfig.StringAdmobBannerUnitId = res.getString(R.string.banner_ad_unit_id);
		AppConfig.UseAdmobRewardedVideo = res.getBoolean(R.bool.use_admob_rewarded_video);
        AppConfig.StringAdmobRewardedVideoUnitId = res.getString(R.string.rewarded_video_ad_unit_id);
		AppConfig.UseGooglePlay = res.getBoolean(R.bool.use_google_game_service);
		AppConfig.UseGoogleDrive = res.getBoolean(R.bool.use_google_drive);
		AppConfig.UseGooglePlus = res.getBoolean(R.bool.use_google_plus);
		AppConfig.UseGoogleFriend = res.getBoolean(R.bool.use_google_friend);
		AppConfig.UseGoogleSendGift = res.getBoolean(R.bool.use_google_send_gift);
		AppConfig.UseFirebaseAnalystic = res.getBoolean(R.bool.use_firebase_analytic);
		AppConfig.UseLocalNotification = res.getBoolean(R.bool.use_local_notification);
		AppConfig.UseGameSparkServices = res.getBoolean(R.bool.use_game_spark_services);
		AppConfig.GameSparkApiKey = res.getString(R.string.game_spark_api_key);
		AppConfig.GameSparkApiSecret = res.getString(R.string.game_spark_api_secret);

		AppConfig.UseChartBoost = res.getBoolean(R.bool.use_chartboost);
		AppConfig.ChartBoostAppId = res.getString(R.string.cb_app_id);
		AppConfig.ChartBoostAppSign = res.getString(R.string.cb_app_sign);
		
		AppConfig.UseUnityAds = res.getBoolean(R.bool.use_unityads);
		AppConfig.UnityAdsId = res.getString(R.string.unityads_id);

		AppConfig.UseVungleAds = res.getBoolean(R.bool.use_vungleads);
		AppConfig.VungleAdsId = res.getString(R.string.vungleads_id);
		AppConfig.VungleAdmobMediationPlacementId1 = res.getString(R.string.vungle_admob_mediation_placement_id_1);
		AppConfig.VungleAdmobMediationPlacementId2 = res.getString(R.string.vungle_admob_mediation_placement_id_2);

		Game.ChangeContext(this);

		AppContext.is_debug = BuildConfig.DEBUG;

		topebox.core.AppContext.Init(this, savedInstanceState);

		// Create GLView
		topebox.core.AppContext.GLView = new topebox.core.GLJNISurfaceView(getApplication());
		topebox.core.AppContext.GLView.setPreserveEGLContextOnPause(true);
		GLRenderer = new topebox.core.GLJNIRenderer();
		topebox.core.AppContext.GLView.setRenderer(GLRenderer);
		topebox.core.AppContext.CurrentContentView.addView(topebox.core.AppContext.GLView);
		if (AppConfig.UseAdmobBanner)
		{
			AdView adView = new AdView(topebox.core.AppContext.CurrentContext);
			if (adView != null) {
				RelativeLayout.LayoutParams adViewLp = new RelativeLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
				adViewLp.addRule(RelativeLayout.CENTER_HORIZONTAL, 1);
				adView.setLayoutParams(adViewLp);
				LinearLayout adViewLayout = new LinearLayout(this);
				adViewLayout.setGravity(Gravity.BOTTOM);
				adViewLayout.addView(adView);

				RelativeLayout.LayoutParams adViewLayoutParams = new RelativeLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
				adViewLayoutParams.addRule(RelativeLayout.CENTER_HORIZONTAL, 1);
				adViewLayoutParams.addRule(RelativeLayout.ALIGN_PARENT_TOP, 1);

				topebox.core.AppContext.CurrentContentView.addView(adViewLayout, adViewLayoutParams);
				AdmobManager.initAdmobBanner(adView);
			}
			else
			{
				Log.i("DEBUG", "Error: Unable to create adView");
			}
		}

		if (AppConfig.UseLocalNotification)
		{
			TopeboxNotificationManager.init(this, R.mipmap.icon);
		}

		setContentView(topebox.core.AppContext.CurrentContentView);

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
		//Support english only
		setDefaultLanguage("en");
		/////////////

		//Request permission at run time for android 6.0
		//requestPermission(permissionsNeedRequest);
		printKeyHash();
    }
	
	
	@Override
	protected void onStart() {
		super.onStart();
		topebox.core.AppContext.onAppStart();
		is_open = true;
		//GSAndroidPlatform.gs().stop();
		//GSAndroidPlatform.gs().start();
	}  

	@Override protected void onPause() {
        super.onPause();
		topebox.core.AppContext.onAppPause();
		is_open = false;
    }

    @Override protected void onResume() {
		super.onResume();
		topebox.core.AppContext.onAppResume();
		is_open = true;
    }
	// In onStop()
	@Override
	protected void onStop() { 
		super.onStop();
		topebox.core.AppContext.onAppStop();
		is_open = false;
	}
	
	// In onDestroy()
	@Override
	protected void onDestroy() 
	{ 
		super.onDestroy();
		topebox.core.AppContext.onAppDestroy();
		is_open = false;
		
	}
	public void callSuperBackPress()
	{
		super.onBackPressed();
	}
	// In onBackPressed()
	@Override
	public void onBackPressed() 
	{ 
		if(AppContext.GLView != null){
			AppContext.GLView.queueEvent(new Runnable(){
				@Override
				public void run() {
					if(!AppContext.onBackKeyPress()){
						((MainActivity)AppContext.CurrentContext).callSuperBackPress();
					}
				}
			});
		}else{
			super.onBackPressed();
		}
	}
	
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		topebox.core.AppContext.onActivityResult(requestCode, resultCode, data);
		Log.i("DEBUG","on activity result:" + requestCode + ":" + resultCode + " :");
		switch(requestCode){
			case ActivityCallback.GOOGLE_PLAY_SERVICE_MISSING:
			case ActivityCallback.GOOGLE_PLAY_SERVICE_UPDATE_REQUEST:
			case ActivityCallback.GOOGLE_PLAY_SERVICE_DISABLE:
			case ActivityCallback.GOOGLE_PLAY_SERVICE_COMMON_RESOLVE:
			case ActivityCallback.GOOGLE_PLAY_SERVICE_SHOW_LEADER_BOARD:
			case ActivityCallback.GOOGLE_PLAY_SERVICE_SHOW_ACHIEVEMENTS:
				GooglePlayManager.resolveCallback(requestCode, resultCode, data);
			break;
			case ActivityCallback.GOOGLE_PLAY_SERVICE_SAVED_GAMES:
				GooglePlayManager.resolveCallback(requestCode, resultCode, data);
			break;
			case ActivityCallback.RC_SIGN_IN:
			case ActivityCallback.RC_GOOGLE_PLUS_SHARE:
				GooglePlayManager.resolveCallback(requestCode, resultCode, data);
			case 64206: // facebook app 
				//#Session.getActiveSession().onActivityResult(this, requestCode, resultCode, data);
			break;
			case 100001:{
				if (ActionSynPurchase.mHelper != null)
				{
					ActionSynPurchase.mHelper.handleActivityResult(requestCode, resultCode, data);
				}
				else{
					ActionSynPurchase.resolveCallback(requestCode, resultCode, data);
				}
			}break;
		}
		
	}
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
		//Support english only
		setDefaultLanguage("en");
		topebox.core.AppContext.onConfigurationChanged(newConfig);
	}
	@Override
	public void onRequestPermissionsResult(int requestCode,
										   String permissions[], int[] grantResults) {
		switch (requestCode) {
			case ActivityCallback.PERMISSION_REQUEST: {
				// If request is cancelled, the result arrays are empty.
				for (int i = 0; i < permissions.length; i ++) {
					if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {

						// permission was granted, yay! Do the
						// contacts-related task you need to do.
						Log.i("DEBUG", "Permission: " + permissions[i] + "granded!");

					} else {

						// permission denied, boo! Disable the
						// functionality that depends on this permission.
						Log.i("DEBUG", "Permission: " + permissions[i] + "denied!");
					}
				}
				return;
			}

			// other 'case' lines to check for other
			// permissions this app might request
		}
	}

	public void requestPermission(String[] permissions)
	{
		ActivityCompat.requestPermissions(((Activity) AppContext.CurrentContext),
							new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},
							ActivityCallback.PERMISSION_REQUEST);
	}

	public void setDefaultLanguage(String language)
	{
		Locale locale = new Locale(language);
		Locale.setDefault(locale);
		Configuration config = new Configuration();
		if(Build.VERSION.SDK_INT < Build.VERSION_CODES.JELLY_BEAN_MR1) {
			config.locale = locale;
		}
		else {
			config.setLocale(locale);
		}
		getResources().updateConfiguration(config, getResources().getDisplayMetrics());
	}

	private static String [] permissionsNeedRequest = {
			Manifest.permission.READ_EXTERNAL_STORAGE,
			Manifest.permission.WRITE_EXTERNAL_STORAGE,
	};

	private void printKeyHash()
	{
		try {
			PackageInfo info = getPackageManager().getPackageInfo(
					getPackageName(),
					PackageManager.GET_SIGNATURES);
			for (android.content.pm.Signature signature : info.signatures) {
				MessageDigest md = MessageDigest.getInstance("SHA");
				md.update(signature.toByteArray());
				Log.i("DEBUG", "KeyHash:" + Base64.encodeToString(md.digest(), Base64.DEFAULT));
			}
		} catch (PackageManager.NameNotFoundException e) {
			Log.i("DEBUG", "NameNotFoundException " + e.getMessage());
		} catch (NoSuchAlgorithmException e) {
			Log.i("DEBUG", "NoSuchAlgorithmException " + e.getMessage());
		}
	}
}
