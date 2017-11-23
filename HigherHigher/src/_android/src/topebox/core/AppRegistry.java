//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core;
import android.graphics.Bitmap;
import 	android.util.Log;

public class AppRegistry{
	//WORK WITH SCREENSHOT
	public interface TakeScreenShotCallBack{
		public void onDone();
	}
	
	protected boolean FlagScreenShot;
	protected TakeScreenShotCallBack CallBackScreenShot;
	protected Bitmap BitmapScreenShot;
	
	public void regTakeScreenShot(TakeScreenShotCallBack callback){
		CallBackScreenShot = callback;
		FlagScreenShot = true;
		
	}
	public void doneTakeScreenShot(Bitmap bmp){
		BitmapScreenShot = bmp;
		FlagScreenShot = false;
		Log.i("DEBUG SCREENSHOT", "doneTakeScreenShot");
		CallBackScreenShot.onDone();
	}
	public boolean needTakeScreenShot(){
		return FlagScreenShot;
	}
	public Bitmap getScreenShotBitmap(){
		return BitmapScreenShot;
	}
	///
	public void clean(){
		FlagScreenShot = false;
		CallBackScreenShot = null;
		BitmapScreenShot = null;
	}
}