//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core.Actions;
import topebox.core.AppContext;
import topebox.core.AppRegistry;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;

import static topebox.core.OsFunctions.GetDataPath;


//#[001] import com.facebook.*;
//#[001] import com.facebook.Request;


public class ActionSendEmailIntent{
	ActionSendEmailIntentArg Arg = null;

	public ActionSendEmailIntent(ActionSendEmailIntentArg arg){
		Arg = arg;
	}
	public void act(){
		Log.i("DEBUG", "ActionSendEmailIntent act");
		if(Arg == null){
			Log.i("DEBUG", "ActionSendEmailIntent arg is null!!");
			return;
		}
		Arg.onBegin();
		try {
			Log.i("DEBUG", "ActionSendEmailIntent mailto: " + Arg.To);
			Intent emailIntent = new Intent(Intent.ACTION_SENDTO);
			emailIntent.setData(Uri.parse("mailto:"));
			emailIntent.putExtra(Intent.EXTRA_EMAIL, Arg.To);
			emailIntent.putExtra(Intent.EXTRA_SUBJECT, Arg.Title);
			emailIntent.putExtra(Intent.EXTRA_TEXT, Arg.Body);
			((Activity) AppContext.CurrentContext).startActivity(Intent.createChooser(emailIntent, "Send email..."));
			Arg.onDone();
		}catch (Exception e) {
			Log.e("DEBUG", "ActionSendEmailIntent fail with exception:" + e.getMessage());
			Arg.onCancel();
		}
	}
}