//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core.Actions;
import topebox.core.*;
import topebox.core.GooglePlay.GooglePlayManager;

//#[001] import com.facebook.*;
//#[001] import com.facebook.model.*;

import 	android.util.Log;

public class ActionGooglePlayLogin implements Action
{

	ActionArg Arg = null;
	public ActionGooglePlayLogin( ActionArg arg)
	{

		Arg = arg;
	}
	
	public void act()
	{
		if(Arg == null )
		{
			Log.i("Debug", "action login google play error");
			return;
		}
		Log.i("DEBUG","begin do action login google play");
		Arg.onBegin();
		if (!GooglePlayManager.IsServiceAvailable()) {
			GooglePlayManager.connect((ActionGooglePlayLoginArg) Arg);
		}
	}
}