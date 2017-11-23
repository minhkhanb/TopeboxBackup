//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core.Actions;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Context;
import java.lang.*;
import android.app.Activity;
import android.util.Log;
import android.view.ContextThemeWrapper;

//import com.unity3d.ads.android.UnityAds;

import topebox.core.AdmobManager;

public class ActionAdmobRewardedVideoAdsShow implements Action {
	ActionAdmobRewardedVideoAdsShowArg Arg = null;
	Activity CurrentActivity = null;

	public ActionAdmobRewardedVideoAdsShow(ActionAdmobRewardedVideoAdsShowArg arg, Activity activity) {
		Arg = arg;
		CurrentActivity = activity;
	}

	public void act() {
		if (Arg == null || CurrentActivity == null) {
			return;
		}
		Arg.onBegin();
		if (AdmobManager.getVideoAdsInstance().isLoaded()) {
			try {
				AdmobManager.getVideoAdsInstance().show();
			}catch (Exception ex)
			{
				AdmobManager.onVideoCompleted(false, true);
				Log.i("DEBUG", "Show Admob rewarded video Ads fail with exception: " + ex.getMessage());
			}
		} else {
			Log.i("DEBUG", "Admob rewarded video Ads is not ready");
			Arg.onCancel();
		}
	}
}