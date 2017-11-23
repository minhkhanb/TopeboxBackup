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


public class ActionShareScreenShotIntent {
	ActionShareScreenShotIntentArg Arg = null;

	public ActionShareScreenShotIntent(ActionShareScreenShotIntentArg arg) {
		Arg = arg;
	}

	public void act() {
		Log.i("DEBUG", "ActionShareScreenShotIntent act");
		if (Arg == null) {
			Log.i("DEBUG", "ActionShareScreenShotIntent arg is null!!");
			return;
		}
		Arg.onBegin();
		AppContext.CurrentRegistry.regTakeScreenShot(new AppRegistry.TakeScreenShotCallBack() {
			public void onDone() {
				Bitmap bmpImage = AppContext.CurrentRegistry.getScreenShotBitmap();
				if (bmpImage != null) {
					try {
						//String dataPath = "data/data/" + GetDataPath();
						File catchDir = AppContext.CurrentContext.getExternalCacheDir();
						Log.i("DEBUG", "facebook LOAD AVATAR dataPath " + catchDir);
						OutputStream fOut = null;
						File file = new File(catchDir, "screenshot.jpeg");

						if (file != null) {
							fOut = new FileOutputStream(file);
							bmpImage.compress(Bitmap.CompressFormat.JPEG, 100, fOut);
							fOut.flush();
							fOut.close();
						}
						else {
							Log.i("DEBUG", "Can not create screenshot file!");
							Arg.onCancel();
							return;
						}

						Uri screenshotUri = Uri.fromFile(file);
						Log.i("DEBUG", "Uri = " + screenshotUri.toString());
						//ByteArrayOutputStream baos = new ByteArrayOutputStream();
						//bmpImage.compress(Bitmap.CompressFormat.JPEG, 100, baos);
						//String path = MediaStore.Images.Media.insertImage(((Activity) AppContext.CurrentContext).getContentResolver(),
								//bmpImage, "abc", "def");
						//Uri screenshotUri2 = Uri.parse(path);
						//Log.i("DEBUG", "Uri2 = " + screenshotUri2.toString());
						Log.i("DEBUG", "ActionShareScreenShotIntent_start share intent msg: " + Arg.Caption);
						//Uri screenshotUri = Uri.parse(path);
						Intent shareIntent = new Intent();
						shareIntent.setAction(Intent.ACTION_SEND);
						shareIntent.putExtra(Intent.EXTRA_TEXT, Arg.Caption);
						shareIntent.putExtra(Intent.EXTRA_STREAM, screenshotUri);
						shareIntent.setType("image/jpeg");
						shareIntent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
						((Activity) AppContext.CurrentContext).startActivity(Intent.createChooser(shareIntent, "Share to..."));
						Arg.onDone();
					} catch (Exception e) {
						Arg.onCancel();
						Log.e("DEBUG", "ActionShareScreenShotIntent fail with exception:" + e.getMessage());
					}
				} else {
					Arg.onCancel();
					Log.i("DEBUG", "facebook LOAD AVATAR bitmap data is null");
				}
			}
		});
	}
/*
	public static Uri getImageContentUri(Context context, File imageFile) {
		String filePath = imageFile.getAbsolutePath();
		Cursor cursor = context.getContentResolver().query(
				MediaStore.Images.Media.INTERNAL_CONTENT_URI,
				new String[]{MediaStore.Images.Media._ID},
				MediaStore.Images.Media.DATA + "=? ",
				new String[]{filePath}, null);
		if (cursor != null && cursor.moveToFirst()) {
			int id = cursor.getInt(cursor.getColumnIndex(MediaStore.MediaColumns._ID));
			cursor.close();
			return Uri.withAppendedPath(MediaStore.Images.Media.INTERNAL_CONTENT_URI, "" + id);
		} else {
			if (imageFile.exists()) {
				ContentValues values = new ContentValues();
				values.put(MediaStore.Images.Media.DATA, filePath);
				return context.getContentResolver().insert(
						MediaStore.Images.Media.INTERNAL_CONTENT_URI, values);
			} else {
				return null;
			}
		}
	}
	*/
}