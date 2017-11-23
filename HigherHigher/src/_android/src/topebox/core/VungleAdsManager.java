package topebox.core;
import java.util.*;

import topebox.core.Actions.ActionArg;
import topebox.core.Actions.ActionFacebookLoginArg;
import topebox.core.Actions.ActionUnityVideoAdsAllowSkipShow;
import topebox.core.Actions.ActionUnityVideoAdsAllowSkipShowArg;
import topebox.core.Actions.ActionUnityVideoAdsShow;
import topebox.core.Actions.ActionUnityVideoAdsShowArg;
import topebox.core.Actions.ActionVungleVideoAdsShow;
import topebox.core.Actions.ActionVungleVideoAdsShowArg;

import android.app.Activity;
import android.util.Log;
// import com.vungle.publisher.VungleAdEventListener;

public class VungleAdsManager{
	/*public static final VungleAdEventListener vungleListener = new VungleAdEventListener(){

		@Override
		public void onAdEnd(String placementReferenceId, boolean wasSuccessfulView, boolean wasCallToActionClicked) {
			// Called when user exits the ad and control is returned to your application
			// if wasSuccessfulView is true, the user watched the ad and should be rewarded
			// (if this was a rewarded ad).
			// if wasCallToActionClicked is true, the user clicked the call to action
			// button in the ad.
			onVideoCompleted(wasSuccessfulView);
		}

		@Override
		public void onAdStart(String placementReferenceId) {
			// Called before playing an ad
		}

		@Override
		public void onUnableToPlayAd(String placementReferenceId, String reason) {
			// Called after playAd(placementId, adConfig) is unable to play the ad
		}

		@Override
		public void onAdAvailabilityUpdate(String placementReferenceId, boolean isAdAvailable) {
			// Notifies ad availability for the indicated placement
			// There can be duplicate notifications
		}
	};*/

	static private ArrayList<ActionArg> _lst_waiting_args = new ArrayList<ActionArg>();
	static private Activity CurrentActivity;
	
	static public void init(Activity activity){
		CurrentActivity = activity;
	}

	static public void showVideoAds(ActionVungleVideoAdsShowArg arg)
	{
		_lst_waiting_args.add(arg);
		ActionVungleVideoAdsShow act_vungle_show_video = new ActionVungleVideoAdsShow(arg, CurrentActivity);
		act_vungle_show_video.act();
	}


	static public void onVideoCompleted(boolean success)
	{
		ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();

		for(int s = 0; s< _lst_waiting_args.size(); s++)
		{
			ActionArg arg = _lst_waiting_args.get(s);
			if(arg instanceof ActionVungleVideoAdsShowArg)
			{
				tmp.add(arg);
			}
		}

		if(tmp.size() > 0) Log.i("DEBUG", "Has pending ActionVungleVideoAdsShowArg");

		for(int i = 0; i< tmp.size(); i++){
			ActionArg arg = tmp.get(i);
			_lst_waiting_args.remove(arg);
			if (success) {
				arg.onDone();
			}
			else
			{
				arg.onCancel();
			}
		}
	}
}