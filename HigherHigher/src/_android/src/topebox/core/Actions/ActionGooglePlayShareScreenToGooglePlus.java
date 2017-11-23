//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core.Actions;

import topebox.core.ActivityCallback;
import topebox.core.AppContext;
import topebox.core.AppRegistry;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.util.Log;
import android.content.pm.PackageManager;

import com.google.android.gms.plus.PlusShare;

import java.io.File;
import java.io.OutputStream;
import java.io.FileOutputStream;


public class ActionGooglePlayShareScreenToGooglePlus {
	ActionGooglePlayShareScreenToGooglePlusArg Arg = null;

	public ActionGooglePlayShareScreenToGooglePlus(ActionGooglePlayShareScreenToGooglePlusArg arg) {
		Arg = arg;
	}

	public void act() {
		Log.i("DEBUG", "ActionGooglePlayShareScreenToGooglePlus act");
		if (Arg == null) {
			Log.i("DEBUG", "ActionGooglePlayShareScreenToGooglePlus arg is null!!");
			return;
		}
		Arg.onBegin();
		if (!isGooglePlusInstalled())
		{
			Intent myIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("https://play.google.com/store/apps/details?id=com.google.android.apps.plus"));
			AppContext.CurrentContext.startActivity(myIntent);
			Arg.onCancel();
			return;
		}
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
						Log.i("DEBUG", "ActionShareScreenShotIntent_start share intent msg: " + Arg.Caption);

						PlusShare.Builder share = new PlusShare.Builder(AppContext.CurrentContext);
						share.setText(Arg.Caption);
						share.addStream(screenshotUri);
						share.setType("image/*");
						share.getIntent().addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
						((Activity) AppContext.CurrentContext).startActivityForResult(share.getIntent(), ActivityCallback.RC_GOOGLE_PLUS_SHARE);
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
	private boolean isGooglePlusInstalled()
	{
		try
		{
			AppContext.CurrentContext.getPackageManager().getApplicationInfo("com.google.android.apps.plus", 0 );
			return true;
		}
		catch(PackageManager.NameNotFoundException e)
		{
			return false;
		}
	}
}