//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core;
import 	android.content.Context;
import 	android.util.Log;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.os.Bundle;

public class ConnectivityChangeReceiver extends BroadcastReceiver {
	@Override
	public void onReceive(Context context, Intent intent) {
		debugIntent(intent, "grokkingandroid");
	}
	private void debugIntent(Intent intent, String tag) {
		Log.v(tag, "action: " + intent.getAction());
		Log.v(tag, "component: " + intent.getComponent());
		Bundle extras = intent.getExtras();
		if (extras != null) {
			for (String key: extras.keySet()) {
				Log.v(tag, "key [" + key + "]: " +
				extras.get(key));
			}
		}
		else {
			Log.v(tag, "no extras");
		}
	}
}