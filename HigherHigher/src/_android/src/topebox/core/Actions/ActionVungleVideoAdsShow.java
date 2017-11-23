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

import topebox.core.AppContext;

public class ActionVungleVideoAdsShow implements Action {
	ActionVungleVideoAdsShowArg Arg = null;
	Activity CurrentActivity = null;

	public ActionVungleVideoAdsShow(ActionVungleVideoAdsShowArg arg, Activity activity) {
		Arg = arg;
		CurrentActivity = activity;
	}

	public void act() {
		if (Arg == null || CurrentActivity == null) {
			return;
		}
		Arg.onBegin();
		/*if (AppContext.vunglePub.isAdPlayable(AppContext.placementIdForLevel)) {
			AppContext.vunglePub.playAd(AppContext.placementIdForLevel, AppContext.globalAdConfig);
			Log.i("DEBUG", "Action show Vungle Ads begin");
		} else {
			Log.i("DEBUG", "Vungle Ads is not ready");
			Arg.onCancel();
		}*/
	}
}