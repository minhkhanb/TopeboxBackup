//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core.Actions;

import 	android.util.Log;

public class ActionGooglePlayInboxGiftShow implements Action
{

	ActionGooglePlayInboxGiftShowArg Arg = null;
	public ActionGooglePlayInboxGiftShow(ActionGooglePlayInboxGiftShowArg arg)
	{
		Arg = arg;
	}
	
	public void act()
	{
		if(Arg == null )
		{
			Log.i("Debug", "action Show Inbox Gift google play error");
			return;
		}
		Log.i("DEBUG","begin do action Show Inbox Gift google play");
		Arg.onBegin();
	}
}