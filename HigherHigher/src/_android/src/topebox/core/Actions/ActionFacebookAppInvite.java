
package topebox.core.Actions;
import topebox.core.*;
//dialog
import android.app.Dialog;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Context;
import java.lang.*;
import android.app.Activity;

public class ActionFacebookAppInvite implements Action
{
	ActionFacebookAppInviteArg Arg = null;
	public ActionFacebookAppInvite(ActionFacebookAppInviteArg arg)
	{
		Arg = arg;
	}
	
	public void act()
	{
		if(Arg == null)
		{
			return;
		}
		Arg.onBegin();
		FacebookManager.OpenAppInvite(Arg);
	}
}