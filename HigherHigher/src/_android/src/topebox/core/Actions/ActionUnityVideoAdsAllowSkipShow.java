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

public class ActionUnityVideoAdsAllowSkipShow implements Action {
	ActionUnityVideoAdsAllowSkipShowArg Arg = null;
	Activity CurrentActivity = null;

	public ActionUnityVideoAdsAllowSkipShow(ActionUnityVideoAdsAllowSkipShowArg arg, Activity activity) {
		Arg = arg;
		CurrentActivity = activity;
	}

	public void act() {
		if (Arg == null || CurrentActivity == null) {
			return;
		}
		Arg.onBegin();
		/*if (UnityAds.canShow()) {
			if (UnityAds.setZone("mainMenu")) {
				UnityAds.show();
			} else {
				Log.i("DEBUG", "Unity Ads Setzone fail.");
				Arg.onCancel();
			}
		} else {
			Log.i("DEBUG", "Unity Ads is not ready");
			Arg.onCancel();
		}*/
	}
}