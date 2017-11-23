package topebox.game.Notification;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlarmManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.provider.MediaStore;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.TaskStackBuilder;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.List;

import topebox.game.higherhigher.MainActivity;

public class HigherHigherAlarmReceiver extends BroadcastReceiver {

	@Override
	public void onReceive(Context context, Intent intent) {
		try {
			Intent notificationIntent = new Intent(context, MainActivity.class);
			notificationIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			PendingIntent contentIntent = PendingIntent.getActivity(context, 100, notificationIntent, PendingIntent.FLAG_UPDATE_CURRENT);

			int iconId = intent.getIntExtra("iconId", -1);
			String title = intent.getStringExtra("title");
			//String content = intent.getStringExtra("content");
			//String ticker = intent.getStringExtra("ticker");

			PackageManager m = context.getPackageManager();
			String package_name = context.getPackageName();
			//PackageInfo p = m.getPackageInfo(s, 0);
			//s = p.applicationInfo.dataDir;

			String s = "data/data/" + package_name + "/localPush.p";
			Log.i("DEBUG", "Data path = " + s);

			File file = new File(s);
			if (file == null) {
				Log.i("DEBUG", "Cannot read local push save file!");
				//AlarmManager alarmManager = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
				//alarmManager.cancel(contentIntent);
				return;
			}
			if (!file.exists())
			{
				Log.i("DEBUG", "Local push save file is not exists!");
				return;
			}
			int length = (int) file.length();
			byte[] data = new byte[length];
			FileInputStream fin = null;
			try {
				fin = new FileInputStream(file);
				fin.read(data);
				fin.close();
			} catch (Exception ex) {
				Log.i("DEBUG", "Read local push save file with exception: " + ex.getMessage());
				//AlarmManager alarmManager = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
				//alarmManager.cancel(contentIntent);
				return;
			}

			TopeboxLocalPushItem localPushItem = new TopeboxLocalPushItem();
			if (data == null) {
				Log.i("DEBUG", "No data for local push!");
				//AlarmManager alarmManager = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
				//alarmManager.cancel(contentIntent);
				return;
			}
			for (int i = 0; i < TopeboxLocalPushItem.LOCAL_PUSH_MAX_ITEM; i++) {
				localPushItem.LoadFromBufffer(data, i * TopeboxLocalPushItem.GetDataLength());
				Log.i("DEBUG", "Read local notification title: " + title + ", id: " + localPushItem.GetId() + ", content: " + localPushItem.GetMessage() + ", ticker: " + localPushItem.GetTicker() + ", get second: " + localPushItem.GetSecond());
				if (localPushItem.GetId() >= 0) {
					if (localPushItem.GetSecond() > 0) {
						localPushItem.SetSecond(localPushItem.GetSecond() - 12*60*60);
						if (localPushItem.GetSecond() < 0) localPushItem.SetSecond(0);
						localPushItem.SaveToBuffer(data, i * TopeboxLocalPushItem.GetDataLength());
					}
					if (localPushItem.GetSecond() == 0) {
						localPushItem.SetSecond(-1);
						if (MainActivity.is_open)
						{
							Log.i("DEBUG", "Not show push because app is open currently");
							return;
						}
						String content = localPushItem.GetMessage();
						String ticker = localPushItem.GetTicker();
						if (iconId < 0 || title == "" || content == null || ticker == null) {
							Log.i("DEBUG", "Can not show notification because of invalid data");
							return;
						}
						NotificationCompat.Builder b = new NotificationCompat.Builder(context);

						b.setAutoCancel(true)
								.setDefaults(Notification.DEFAULT_ALL)
								.setWhen(System.currentTimeMillis())
								.setSmallIcon(iconId)
								.setTicker(ticker)
								.setContentTitle(title)
								.setContentText(content)
								.setDefaults(Notification.DEFAULT_LIGHTS | Notification.DEFAULT_SOUND)
								.setContentIntent(contentIntent)
								.setContentInfo("");

						NotificationManager notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
						notificationManager.notify(100, b.build());
						localPushItem.SaveToBuffer(data, i * TopeboxLocalPushItem.GetDataLength());
						Log.i("DEBUG", "Show local notification title: " + title + ", id: " + localPushItem.GetId() + ", content: " + content + ", ticker: " + ticker + ", get second: " + localPushItem.GetSecond());
						break;
					}
				}
			}

			FileOutputStream fout = null;
			try {
				fout = new FileOutputStream(file);
				if (fout != null && data != null) {
					fout.write(data);
				}
				fout.close();
			} catch (Exception ex) {
				Log.i("DEBUG", "Write local push save file with exception: " + ex.getMessage());
			}


			/*if (iconId < 0 || title == "" || content == "" || ticker == "")
			{
				Log.i("DEBUG", "Can not show notification because of invalid data from intent");
				return;
			}
			NotificationCompat.Builder b = new NotificationCompat.Builder(context);

			b.setAutoCancel(true)
					.setDefaults(Notification.DEFAULT_ALL)
					.setWhen(System.currentTimeMillis())
					.setSmallIcon(iconId)
					.setTicker(ticker)
					.setContentTitle(title)
					.setContentText(content)
					.setDefaults(Notification.DEFAULT_LIGHTS | Notification.DEFAULT_SOUND)
					.setContentIntent(contentIntent)
					.setContentInfo("Info");

			NotificationManager notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
			notificationManager.notify(1, b.build());
			Log.i("DEBUG", "Show local notification title: " + title + ", content: " + content + ", ticker: " + ticker);
		} catch (Exception ex) {
			Log.i("DEBUG", "Show local notification fail with exception: " + ex.getMessage());
		}*/
		} catch (Exception ex) {
			Log.i("DEBUG", "Show notification fail with exception: " + ex.getMessage());
		}
	}
}