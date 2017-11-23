package topebox.core;
import java.util.*;

import android.os.Bundle;
import android.os.Handler;
import 	android.util.Log;

import topebox.core.Actions.ActionAdmobRewardedVideoAdsShow;
import topebox.core.Actions.ActionAdmobRewardedVideoAdsShowArg;
import topebox.core.Actions.ActionArg;
import topebox.core.Admob.*;

import android.app.Activity;
import android.view.View;
import android.widget.LinearLayout;
//admob
// import com.chartboost.sdk.Chartboost;
// import com.google.ads.mediation.chartboost.ChartboostAdapter;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.reward.RewardItem;
import com.google.android.gms.ads.reward.RewardedVideoAd;
import com.google.android.gms.ads.reward.RewardedVideoAdListener;
// import com.jirbo.adcolony.AdColonyAdapter;
// import com.jirbo.adcolony.AdColonyBundleBuilder;
// import com.vungle.mediation.VungleAdapter;
// import com.vungle.mediation.VungleExtrasBuilder;

public class AdmobManager{
	static private ArrayList<AdmobWaitingLoadCallback> _waiting = new ArrayList<AdmobWaitingLoadCallback>();
	static private ArrayList<InterstitialAd> _lst_ads = new ArrayList<InterstitialAd>();
	static private ArrayList<ActionArg> _lst_waiting_args = new ArrayList<ActionArg>();
	static private InterstitialAd _interstitialAd;
	static private AdView _adView;
	static private ActionArg _waiting_banner_arg;
	static private boolean _adBannerLoaded = false;
	static private RewardedVideoAd _videoAds;
	static private AdRequest _rewardedVideoRequest;
	static private Activity CurrentActivity;
	static private boolean _is_onWaitForVideoRewardAdsShow;
	static public long _time_wait_for_show_ads = 10 * 1000;
	static private long _time_start_waiting_for_show_ads = 0;
	static private boolean _is_onWaitForVideoRewardReinitAds;
	static public long _time_wait_for_reinit_ads = 60 * 1000;
	static private long _time_start_waiting_for_reinit_ads;

	static public void addWaitingInterstitialCallback(AdmobWaitingLoadCallback callback){
		_waiting.add(callback);
	}
	static public void removeWaitingInterstitialCallback(AdmobWaitingLoadCallback callback){
		_waiting.remove(callback);
	}
	
	static public void init(Activity activity){
		CurrentActivity = activity;
		MobileAds.initialize(CurrentActivity, AppConfig.StringAdmobApplicationId);
		/*AdColonyBundleBuilder.setUserId("USER_ID");
		AdColonyBundleBuilder.setShowPrePopup(true);
		AdColonyBundleBuilder.setShowPostPopup(true);*/
	}

	static public void initAdmobInterstitial ()
	{
		_interstitialAd = new InterstitialAd(CurrentActivity);
		_interstitialAd.setAdUnitId(AppConfig.StringAdmobInterstitialUnitId);
		_interstitialAd.setAdListener(new AdListener(){
			@Override
			public void onAdLoaded() {
				//for(int i = 0; i< AdmobManager._waiting.size(); i++){
				//AdmobManager._waiting.get(i).onDone(true);
				//}
				for (int i = 0; i < _lst_waiting_args.size(); i++)
				{
					_lst_waiting_args.get(i).onDone();
					_lst_waiting_args.remove(i);
				}
			}

			@Override
			public void onAdFailedToLoad (int errorCode) {
				//for(int i = 0; i< AdmobManager._waiting.size(); i++){
				//AdmobManager._waiting.get(i).onDone(false);
				//}
				for (int i = 0; i < _lst_waiting_args.size(); i++)
				{
					_lst_waiting_args.get(i).onCancel();
					_lst_waiting_args.remove(i);
				}
			}

			@Override
			public void onAdClosed(){
				Log.i("DEBUG","Ads Closed");
				for (int i = 0; i < _lst_waiting_args.size(); i++)
				{
					_lst_waiting_args.get(i).onDone();
					_lst_waiting_args.remove(i);
				}
				for(int i = 0; i< _lst_ads.size(); i++){
					InterstitialAd tmp = _lst_ads.get(i);
					if(tmp.getAdListener () == this){
						_lst_ads.remove(tmp);
						Log.i("DEBUG","Ads Removed: remain:" + _lst_ads.size());
						break;
					}
				}
			}
		});
	}

	static public boolean isPrepareInterstitialCompleted(){
		return _interstitialAd.isLoaded();
	}

	static public void prepareInterstitial(ActionArg arg){
		/*Bundle chartboostBundle = new ChartboostAdapter.ChartboostExtrasBundleBuilder()
				.setFramework(Chartboost.CBFramework.CBFrameworkOther, "7.0.0.0")
				.build();*/
		AdRequest adRequest = new AdRequest.Builder()
				//.addNetworkExtrasBundle(ChartboostAdapter.class, chartboostBundle)
				//.addNetworkExtrasBundle(AdColonyAdapter.class, AdColonyBundleBuilder.build())
				.build();
        _interstitialAd.loadAd(adRequest);
		_lst_waiting_args.add(arg);
		//_lst_ads.add(_interstitialAd);
	}
	
	static public boolean showInterstitial(ActionArg arg){
		/*for(int i = 0; i< _lst_ads.size(); i++){
			InterstitialAd tmp = _lst_ads.get(i);
			if (tmp != null && tmp.isLoaded()) {
				tmp.show();
				_lst_waiting_args.add(arg);
				return true;
			}
		}*/
		if (_interstitialAd != null && _interstitialAd.isLoaded())
		{
			_interstitialAd.show();
			_lst_waiting_args.add(arg);
			return true;
		}
		else {
			arg.onCancel();
			return false;
		}
	}

	static public void initAdmobBanner (AdView ad_view)
	{
		Log.i("DEBUG", "Init admob banner!");
		_adView = ad_view;
		_adView.setAdUnitId(AppConfig.StringAdmobBannerUnitId);
		//_adView.setAdSize(AdSize.SMART_BANNER);
		_adView.setAdSize(AdSize.BANNER);
		_adView.setVisibility(View.GONE);
	}

	static public void prepareBannerAds(ActionArg arg)
	{
		if (_adView != null) {
			_waiting_banner_arg = arg;
			AdRequest adRequest = new AdRequest.Builder().build();
			_adView.setAdListener(new AdListener() {
				@Override
				public void onAdClosed() {
					if (_waiting_banner_arg != null)
					{
						_waiting_banner_arg.onCancel();
						_waiting_banner_arg = null;
					}
					_adBannerLoaded = false;
					Log.i("DEBUG", "admob banner onAdClosed");
					super.onAdClosed();
				}

				@Override
				public void onAdFailedToLoad(int i) {
					_adBannerLoaded = false;
					if (_waiting_banner_arg != null)
					{
						_waiting_banner_arg.onCancel();
						_waiting_banner_arg = null;
					}
					Log.i("DEBUG", "admob banner onAdFailedToLoad");
					super.onAdFailedToLoad(i);
				}

				@Override
				public void onAdOpened() {
					Log.i("DEBUG", "admob banner onAdOpened");
					super.onAdOpened();
				}

				@Override
				public void onAdLeftApplication() {
					Log.i("DEBUG", "admob banner onAdLeftApplication");
					super.onAdLeftApplication();
				}

				@Override
				public void onAdLoaded() {
					Log.i("DEBUG", "admob banner onAdLoaded");
					if (_waiting_banner_arg != null)
					{
						_waiting_banner_arg.onDone();
						_waiting_banner_arg = null;
					}
					_adBannerLoaded = true;
					super.onAdLoaded();
				}
			});
			_adView.loadAd(adRequest);
		}
		else
		{
			Log.i("DEBUG", "AdView for admob banner is null");
			arg.onCancel();
		}
	}

	static public boolean showBannerAds(ActionArg arg){
		if (_adView != null && _adBannerLoaded)
		{
			_adView.setVisibility(View.VISIBLE);
			arg.onDone();
			return true;
		}
		else {
			arg.onCancel();
			return false;
		}
	}

	static public boolean hideBannerAds(ActionArg arg){
		if (_adView != null)
		{
			_adView.setVisibility(View.GONE);
			arg.onDone();
			return true;
		}
		else {
			arg.onCancel();
			return false;
		}
	}


	static public void onPause()
	{
		if (_adView != null) {
			_adView.pause();
		}
		if (_videoAds != null){
			_videoAds.pause(CurrentActivity);
		}
	}

	static public void onResume()
	{
		if (_adView != null) {
			_adView.resume();
		}
		if (_videoAds != null){
			_videoAds.resume(CurrentActivity);
		}
	}

	static public void onDestroy()
	{
		if (_adView != null) {
			_adView.destroy();
		}
		if (_videoAds != null){
			_videoAds.destroy(CurrentActivity);
		}
	}

	static public void GameUpdate()
	{
		if (_is_onWaitForVideoRewardAdsShow)
		{
			if (System.currentTimeMillis() - _time_start_waiting_for_show_ads >= _time_wait_for_show_ads)
			{
				onVideoCompleted(false, false);
				_is_onWaitForVideoRewardAdsShow = false;
			}
		}
		if (_is_onWaitForVideoRewardReinitAds)
		{
			if (System.currentTimeMillis() - _time_start_waiting_for_reinit_ads >= _time_wait_for_reinit_ads)
			{
				initAdmobRewardedVideo();
				_is_onWaitForVideoRewardReinitAds = false;
			}
		}
	}

	//rewarded video
	static public void initAdmobRewardedVideo()
	{
		_videoAds = MobileAds.getRewardedVideoAdInstance(CurrentActivity);
		_videoAds.setRewardedVideoAdListener(admobRewardedVideoListener);
		String[] placements = new String[] { AppConfig.VungleAdmobMediationPlacementId1, AppConfig.VungleAdmobMediationPlacementId2 };
		/*_rewardedVideoRequest = new AdRequest.Builder()
				.addNetworkExtrasBundle(VungleAdapter.class, new VungleExtrasBuilder(placements).build())
				.addNetworkExtrasBundle(AdColonyAdapter.class, AdColonyBundleBuilder.build())
				.build();*/
		LoadRewardedVideo();
	}

	static public RewardedVideoAd getVideoAdsInstance () {
		if (_videoAds == null)
		{
			initAdmobRewardedVideo();
		}
		return _videoAds;
	}

	static public void LoadRewardedVideo()
	{
		if(!_videoAds.isLoaded()) {
			_videoAds.loadAd(AppConfig.StringAdmobRewardedVideoUnitId, _rewardedVideoRequest);
		}
	}

	static public void ShowRewardedVideo(ActionAdmobRewardedVideoAdsShowArg arg)
	{
		_lst_waiting_args.add(arg);
		ActionAdmobRewardedVideoAdsShow act_unity_show_rewarded_video = new ActionAdmobRewardedVideoAdsShow(arg, CurrentActivity);
		act_unity_show_rewarded_video.act();
		_is_onWaitForVideoRewardAdsShow = true;
		_time_start_waiting_for_show_ads = System.currentTimeMillis();
	}

	static public void onVideoCompleted(boolean success, boolean needWait)
	{
		if (!needWait) {
			//LoadRewardedVideo();
			if (!success) {
				//initAdmobRewardedVideo();
				LoadRewardedVideo();
			}
		}else
		{
			_is_onWaitForVideoRewardReinitAds = true;
			_time_start_waiting_for_reinit_ads = System.currentTimeMillis();
		}
		ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();

		for(int s = 0; s< _lst_waiting_args.size(); s++)
		{
			ActionArg arg = _lst_waiting_args.get(s);
			if(arg instanceof ActionAdmobRewardedVideoAdsShowArg)
			{
				tmp.add(arg);
			}
		}

		if(tmp.size() > 0) Log.i("DEBUG", "Has pending ActionAdmobRewardedVideoAdsShowArg");

		for(int i = 0; i< tmp.size(); i++){
			ActionArg arg = tmp.get(i);
			_lst_waiting_args.remove(arg);
			if (success){
				arg.onDone();
			}
			else {
				arg.onCancel();
			}
		}
	}

	public static final RewardedVideoAdListener admobRewardedVideoListener = new RewardedVideoAdListener(){
		@Override
		public void onRewardedVideoAdLoaded() {
			Log.i("DEBUG", "onRewardedVideoAdLoaded");
		}

		@Override
		public void onRewardedVideoAdOpened() {
			_is_onWaitForVideoRewardAdsShow = false;
			Log.i("DEBUG", "onRewardedVideoAdOpened");
		}

		@Override
		public void onRewardedVideoStarted() {
			Log.i("DEBUG", "onRewardedVideoStarted");
		}

		@Override
		public void onRewardedVideoAdClosed() {
			Log.i("DEBUG", "onRewardedVideoAdClosed");
			onVideoCompleted(false, false);
		}

		@Override
		public void onRewarded(RewardItem rewardItem) {
			Log.i("DEBUG", "onRewarded");
			onVideoCompleted(true, false);
		}

		@Override
		public void onRewardedVideoAdLeftApplication() {
			Log.i("DEBUG", "onRewardedVideoAdLeftApplication");
			//onVideoCompleted(false);
		}

		@Override
		public void onRewardedVideoAdFailedToLoad(int i) {
			Log.i("DEBUG", "onRewardedVideoAdFailedToLoad");
			onVideoCompleted(false, true);
		}
	};
}