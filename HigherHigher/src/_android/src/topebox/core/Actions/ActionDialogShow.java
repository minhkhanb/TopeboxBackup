//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core.Actions;
import android.app.*;
import android.content.Intent;
import topebox.core.*;
//dialog
import android.content.DialogInterface;
import android.content.Context;
import java.lang.*;

import android.os.Build;
import android.util.Log;

public class ActionDialogShow implements Action{
	ActionDialogShowArg Arg = null;
	Context CurrentContext = null;
	public ActionDialogShow(ActionDialogShowArg arg, Context context){
		Arg = arg;
		CurrentContext = context;
	}
	public void act(){
		if(Arg == null || CurrentContext == null){
			return;
		}
		Arg.onBegin();

		AlertDialog.Builder dialog;
		if(Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP){
			dialog = new AlertDialog.Builder(CurrentContext, android.R.style.Theme_DeviceDefault_Panel);
		}
		else {
			dialog = new AlertDialog.Builder(CurrentContext, android.R.style.Theme_Material_Light_Dialog_Alert);
		}

		if (Arg.Message != null && Arg.Type >= 0)
		{
			dialog.setMessage(Arg.Message);
			if (Arg.Title != null && Arg.Title != "")
			{
				dialog.setTitle(Arg.Title);
			}

			switch (Arg.Type)
			{
				case 0:  //Popup dialog
					dialog.setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							Arg.Which = which;
							Arg.onDone();
						}
					});
					break;
				case 1:	//Exit dialog
					dialog.setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							Arg.Which = which;
							Arg.onDone();
						}
					});
					dialog.setNegativeButton(android.R.string.no, new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							Arg.Which = which;
							Arg.onCancel();
						}
					});
					break;
				case 2:	//network error required restart dialog
					dialog.setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							Arg.Which = which;
							Arg.onDone();
							//android.os.Process.killProcess(android.os.Process.myPid());
							//System.exit(1);
							if (Game.IsInited)
							{
								Game.restart();
							}
						}
					});
					break;
				default:
					dialog.setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							Arg.Which = which;
							Arg.onDone();
						}
					});
					break;
			}
		}
		else
		{
			Log.i("DEBUG", "Show message box fail because missing message!");
		}

		final AlertDialog.Builder showDialog = dialog;

		((Activity)CurrentContext).runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				showDialog.show();
			}
		});
	}
}