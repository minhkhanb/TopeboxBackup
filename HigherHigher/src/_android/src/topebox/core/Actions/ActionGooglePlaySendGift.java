//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core.Actions;

import 	android.util.Log;

public class ActionGooglePlaySendGift implements Action
{

	ActionGooglePlaySendGiftArg Arg = null;
	public ActionGooglePlaySendGift(ActionGooglePlaySendGiftArg arg)
	{
		Arg = arg;
	}
	
	public void act()
	{
		if(Arg == null )
		{
			Log.i("Debug", "action send gift google play error");
			return;
		}
		Log.i("DEBUG","begin do action send gift google play");
		Arg.onBegin();
	}
}