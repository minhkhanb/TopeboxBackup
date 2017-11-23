//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core.Actions;
import topebox.core.AppContext;
import topebox.core.AppRegistry;
import topebox.core.ActivityCallback;

import android.Manifest;
import android.app.Activity;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.OutputStream;
import java.io.FileOutputStream;
import static topebox.core.OsFunctions.GetDataPath;


public class ActionShareIntent {
	ActionShareIntentArg Arg = null;

	public ActionShareIntent(ActionShareIntentArg arg) {
		Arg = arg;
	}

	public void act() {
		Log.i("DEBUG", "ActionShareIntent act");
		if (Arg == null) {
			Log.i("DEBUG", "ActionShareIntent arg is null!!");
			return;
		}
		Arg.onBegin();
		try {
			Log.i("DEBUG", "ActionShareIntent_start share intent msg: " + Arg.Caption);
			Intent shareIntent = new Intent();
			shareIntent.setAction(Intent.ACTION_SEND);
			shareIntent.putExtra(Intent.EXTRA_TEXT, Arg.Caption);
			shareIntent.putExtra(Intent.EXTRA_SUBJECT, Arg.Description);
			shareIntent.setType("text/plain");
			shareIntent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
			((Activity) AppContext.CurrentContext).startActivity(Intent.createChooser(shareIntent, Arg.Name));
			Arg.onDone();
		} catch (Exception e) {
			Arg.onCancel();
			Log.e("DEBUG", "ActionShareIntent fail with exception:" + e.getMessage());
		}

	}
}