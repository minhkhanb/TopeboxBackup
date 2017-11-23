package topebox.core;
import 	android.content.BroadcastReceiver;
import android.widget.Toast;
import 	android.content.Intent;
import 	android.content.Context;
class SentReceiver extends BroadcastReceiver{
	@Override
	public void onReceive(Context context, Intent arg1) {
		/*switch (getResultCode())
		{
		case Activity.RESULT_OK:
			Toast.makeText(getBaseContext(), "SMS sent", 
			Toast.LENGTH_SHORT).show();
			break;
		case SmsManager.RESULT_ERROR_GENERIC_FAILURE:
			Toast.makeText(getBaseContext(), "Generic failure", 
			Toast.LENGTH_SHORT).show();
			break;
		case SmsManager.RESULT_ERROR_NO_SERVICE:
			Toast.makeText(getBaseContext(), "No service", 
			Toast.LENGTH_SHORT).show();
			break;
		case SmsManager.RESULT_ERROR_NULL_PDU:
			Toast.makeText(getBaseContext(), "Null PDU", 
			Toast.LENGTH_SHORT).show();
			break;
		case SmsManager.RESULT_ERROR_RADIO_OFF:
			Toast.makeText(getBaseContext(), "Radio off", 
			Toast.LENGTH_SHORT).show();
			break;
		}
		*/

		
	}
}