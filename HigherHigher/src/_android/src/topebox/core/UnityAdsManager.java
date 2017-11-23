package topebox.core;
import java.util.*;

import topebox.core.Actions.ActionArg;
import topebox.core.Actions.ActionFacebookLoginArg;
import topebox.core.Actions.ActionUnityVideoAdsAllowSkipShow;
import topebox.core.Actions.ActionUnityVideoAdsAllowSkipShowArg;
import topebox.core.Actions.ActionUnityVideoAdsShow;
import topebox.core.Actions.ActionUnityVideoAdsShowArg;
import android.app.Activity;
import android.util.Log;

public class UnityAdsManager{
	static private ArrayList<ActionArg> _lst_waiting_args = new ArrayList<ActionArg>();
	static private Activity CurrentActivity;
	
	static public void init(Activity activity){
		CurrentActivity = activity;
	}

	static public void showVideoAds(ActionUnityVideoAdsShowArg arg)
	{
		_lst_waiting_args.add(arg);
		ActionUnityVideoAdsShow act_unity_show_video = new ActionUnityVideoAdsShow(arg, CurrentActivity);
		act_unity_show_video.act();
	}

	static public void showSkippableVideoAds(ActionUnityVideoAdsAllowSkipShowArg arg)
	{
		_lst_waiting_args.add(arg);
		ActionUnityVideoAdsAllowSkipShow act_unity_show_skippable_video = new ActionUnityVideoAdsAllowSkipShow(arg, CurrentActivity);
		act_unity_show_skippable_video.act();
	}

	static public void onVideoCompleted()
	{
		ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();

		for(int s = 0; s< _lst_waiting_args.size(); s++)
		{
			ActionArg arg = _lst_waiting_args.get(s);
			if(arg instanceof ActionUnityVideoAdsShowArg)
			{
				tmp.add(arg);
			}
			else if(arg instanceof ActionUnityVideoAdsAllowSkipShowArg)
			{
				tmp.add(arg);
			}
		}

		if(tmp.size() > 0) Log.i("DEBUG", "Has pending ActionUnityVideoAdsShowArg");

		for(int i = 0; i< tmp.size(); i++){
			ActionArg arg = tmp.get(i);
			_lst_waiting_args.remove(arg);
			arg.onDone();
		}
	}

	static public void onUserSkipVideo()
	{
		ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();

		for(int s = 0; s< _lst_waiting_args.size(); s++)
		{
			ActionArg arg = _lst_waiting_args.get(s);
			if(arg instanceof ActionUnityVideoAdsShowArg)
			{
				tmp.add(arg);
			}
			else if(arg instanceof ActionUnityVideoAdsAllowSkipShowArg)
			{
				tmp.add(arg);
			}
		}

		if(tmp.size() > 0) Log.i("DEBUG", "Has pending ActionUnityVideoAdsShowArg");

		for(int i = 0; i< tmp.size(); i++){
			ActionArg arg = tmp.get(i);
			_lst_waiting_args.remove(arg);
			arg.onCancel();
		}
	}
}