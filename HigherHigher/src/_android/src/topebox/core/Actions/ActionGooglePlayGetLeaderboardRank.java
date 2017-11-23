//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core.Actions;
import topebox.core.AppContext;
import topebox.core.AppRegistry;
import topebox.core.ActivityCallback;
import topebox.core.GooglePlay.GooglePlayManager;

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


public class ActionGooglePlayGetLeaderboardRank {
	ActionGooglePlayGetLeaderboardRankArg Arg = null;

	public ActionGooglePlayGetLeaderboardRank(ActionGooglePlayGetLeaderboardRankArg arg) {
		Arg = arg;
	}

	public void act() {
		Log.i("DEBUG", "ActionGooglePlayGetLeaderboardRank act");
		if (Arg == null) {
			Log.i("DEBUG", "ActionGooglePlayGetLeaderboardRank arg is null!!");
			return;
		}
		Arg.onBegin();
		try {
			GooglePlayManager.getLeaderBoardRank(Arg);
		} catch (Exception e) {
			Arg.onCancel();
			Log.e("DEBUG", "ActionGooglePlayGetLeaderboardRank fail with exception:" + e.getMessage());
		}
	}
}