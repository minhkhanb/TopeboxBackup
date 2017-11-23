package topebox.game.Notification;


import android.app.Activity;
import android.app.AlarmManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.os.Build;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

import java.util.Calendar;

import topebox.core.AppContext;

public class TopeboxNotificationManager {
    public static Activity mCurrentActivity;
    public static PendingIntent mCurrentBroadcast;
    public static int mIconId = -1;

    public static void init (Activity activity, int iconId)
    {
        mCurrentActivity = activity;
        mIconId = iconId;
        try {
            AlarmManager alarmManager = (AlarmManager) mCurrentActivity.getSystemService(Context.ALARM_SERVICE);

            Intent notificationIntent = new Intent("android.media.action.DISPLAY_NOTIFICATION");
            notificationIntent.addCategory("android.intent.category.DEFAULT");
            ApplicationInfo applicationInfo = AppContext.CurrentContext.getApplicationInfo();
            int stringId = applicationInfo.labelRes;
            String appName = stringId == 0 ? applicationInfo.nonLocalizedLabel.toString() : AppContext.CurrentContext.getString(stringId);
            notificationIntent.putExtra("title", appName);
            notificationIntent.putExtra("iconId", mIconId);

            mCurrentBroadcast = PendingIntent.getBroadcast(mCurrentActivity, 100, notificationIntent, PendingIntent.FLAG_UPDATE_CURRENT);
            Log.i("DEBUG", "setNotification broadcast = " + mCurrentBroadcast.toString());
            Calendar cal = Calendar.getInstance();
            cal.add(Calendar.SECOND, 12*60*60);
            if(Build.VERSION.SDK_INT < Build.VERSION_CODES.KITKAT) {
                alarmManager.setRepeating(AlarmManager.RTC_WAKEUP, cal.getTimeInMillis(), 12*60*60*1000, mCurrentBroadcast);
            }
            else
            {
                alarmManager.setInexactRepeating(AlarmManager.RTC_WAKEUP, cal.getTimeInMillis(), 12*60*60*1000, mCurrentBroadcast);
            }
            //Log.i("DEBUG", "Set notification title: " + title + ", content: " + content + ", ticker: " + ticker + ", second: " + second);
        }
        catch (Exception ex)
        {
            Log.i("DEBUG", "SetNotification fail with exception: " + ex.getMessage());
        }
    }

    public static boolean setNotification(String title, String content, String ticker, int second)
    {
        try {
            AlarmManager alarmManager = (AlarmManager) mCurrentActivity.getSystemService(Context.ALARM_SERVICE);

            Intent notificationIntent = new Intent("android.media.action.DISPLAY_NOTIFICATION");
            notificationIntent.addCategory("android.intent.category.DEFAULT");
            notificationIntent.putExtra("title", title);
            //notificationIntent.putExtra("content", content);
            //notificationIntent.putExtra("ticker", ticker);
            notificationIntent.putExtra("iconId", mIconId);

            mCurrentBroadcast = PendingIntent.getBroadcast(mCurrentActivity, 100, notificationIntent, PendingIntent.FLAG_UPDATE_CURRENT);
            Log.i("DEBUG", "setNotification broadcast = " + mCurrentBroadcast.toString());
            Calendar cal = Calendar.getInstance();
            cal.add(Calendar.SECOND, second);
            if(Build.VERSION.SDK_INT < Build.VERSION_CODES.KITKAT) {
                alarmManager.set(AlarmManager.RTC_WAKEUP, cal.getTimeInMillis(), mCurrentBroadcast);
            }
            else
            {
                alarmManager.setExact(AlarmManager.RTC_WAKEUP, cal.getTimeInMillis(), mCurrentBroadcast);
            }
            Log.i("DEBUG", "Set notification title: " + title + ", content: " + content + ", ticker: " + ticker + ", second: " + second);

            return true;
        }
        catch (Exception ex)
        {
            Log.i("DEBUG", "SetNotification fail with exception: " + ex.getMessage());
            return false;
        }
    }

    public static boolean cancelNotification ()
    {
        if (mCurrentActivity == null)
        {
            Log.i("DEBUG", "Local notification is NOT init yet!");
            return false;
        }
        if (mCurrentBroadcast == null)
        {
            Log.i("DEBUG", "Local notification NOT have any pending intent!");
            return false;
        }
        try {
            AlarmManager alarmManager = (AlarmManager) mCurrentActivity.getSystemService(Context.ALARM_SERVICE);
            Log.i("DEBUG", "cancelNotification broadcast = " + mCurrentBroadcast.toString());
            alarmManager.cancel(mCurrentBroadcast);
            Log.i("DEBUG", "Cancel notification success!");
            return true;
        }
        catch (Exception ex)
        {
            Log.i("DEBUG", "Cancel notification fail with exception: " + ex.getMessage());
            return false;
        }
    }
}