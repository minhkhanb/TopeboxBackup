
package topebox.core.Actions;
import topebox.core.GameSparkServices.GameSparkManager;


import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.AsyncTask;
import 	android.util.Log;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class ActionGameSparkUploadSave implements Action
{

	ActionGameSparkUploadSaveArg Arg = null;
	public ActionGameSparkUploadSave( ActionGameSparkUploadSaveArg arg)
	{
		Arg = arg;
	}
	
	public void act()
	{
		if(Arg == null )
		{
			Log.i("Debug", "action game spark upload save error");
			return;
		}
		Arg.onBegin();
		if (GameSparkManager.isInit()) {
			Arg.mSaveFileName = "clouddata.pine";
			GameSparkManager.UploadSave(Arg, "clouddata.pine");
		}
	}
}