package topebox.core;
import android.graphics.Point;
import android.opengl.GLSurfaceView;
import android.content.Context;
import android.view.Surface;
import android.view.View;
import 	android.view.MotionEvent;
import 	android.util.Log;
import android.view.Display;
import android.view.WindowManager;
import android.content.res.Configuration;

public class GLJNISurfaceView extends GLSurfaceView {
	int _next_touch_id = 1;
	int[] _touch_id = new int[10];
	int[] _touch_gen_id = new int[10];
    public GLJNISurfaceView (Context context) {
        super(context);
		for(int i = 0; i< 10; i++){
			_touch_id[i] = -1;
		}
		
		/*setOnTouchListener(new OnTouchListener() {
			public boolean onTouch(View v, MotionEvent e) {
				// ... Respond to touch events 
				int touch_index = e.getActionIndex();
		
				int touch_id = e.getPointerId (touch_index);
				float x = e.getX(touch_index);
				float y = e.getY(touch_index);
				
					switch (e.getActionMasked()) {
						case MotionEvent.ACTION_DOWN:
							
							Game.TouchBegin(touch_id, (int) x, (int) y);
							Log.i("DEBUG","touch down:"+ touch_id +" x:"+ x + " " + y);
						break;
						case MotionEvent.ACTION_UP:
						case MotionEvent.ACTION_POINTER_UP:
						case MotionEvent.ACTION_CANCEL:
							
							Game.TouchEnd(touch_id, (int) x, (int) y);
									Log.i("DEBUG","touch up:"+ touch_id +" x:"+ x + " " + y);
						break;
						case MotionEvent.ACTION_MOVE:

							for (int i = 0; i< e.getPointerCount(); i++) {
							
								Game.TouchDrag(tmp_touch_id, (int) e.getX(i), (int) e.getY(i));
							}
						break;
					}

				return true;
			}
		});
		*/
    }
	
	public void onConfigChanged(){
		WindowManager wm = (WindowManager) topebox.core.AppContext.CurrentContext.getSystemService(Context.WINDOW_SERVICE);
		Display display = wm.getDefaultDisplay(); 
		int width = display.getWidth();  // deprecated
		int height = display.getHeight();
		Game.onSizeChange(width, height);
	}
	
	@Override
	public void onWindowFocusChanged (boolean hasWindowFocus){
		Log.i("DEBUG", "focus changed");
		if (hasWindowFocus){
			Game.Resume();
		} else {
			
		}
	}
	
	@Override
	public void onSizeChanged (int w, int h, int oldw, int oldh) {
		WindowManager wm = (WindowManager) topebox.core.AppContext.CurrentContext.getSystemService(Context.WINDOW_SERVICE);
		Display display = wm.getDefaultDisplay();
        //int orientation = display.getOrientation(); // deprecated
		//int orientation = display.getRotation();

		//Display display = getWindowManager().getDefaultDisplay();

		//int width = display.getWidth();  // deprecated
		//int height = display.getHeight();// deprecated

		Point outsize = new Point();
		display.getSize(outsize);
		int width = outsize.x;
		int height = outsize.y;
		Log.i("DEBUG", "GLJNISurfaceView onSizeChanged width = " + width + " height = " + height);
		/*switch(orientation) {
            //case Configuration.ORIENTATION_PORTRAIT:{
			case Surface.ROTATION_90:
			case Surface.ROTATION_270:{
                int tmp = width > height?height:width;
				height = tmp == width? height:width;
				width = tmp;
				Log.i("DEBUG", "GLJNISurfaceView onSizeChanged orientation lanscape");
                }break;
            //case Configuration.ORIENTATION_LANDSCAPE:{
			case Surface.ROTATION_0:
			case Surface.ROTATION_180:{
                int tmp = width > height? width:height;
				height = tmp == width ? height:width;
				width = tmp;
				Log.i("DEBUG", "GLJNISurfaceView onSizeChanged orientation portrait");
                }break;   
        }*/
		Game.onSizeChange(width, height);
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent e) {
		// MotionEvent reports input details from the touch screen
		// and other input controls. In this case, you are only
		// interested in events where the touch position changed.

		
		int touch_index = e.getActionIndex();
		
		/*int touch_id = e.getPointerId (touch_index);
		float x = e.getX(touch_index);
		float y = e.getY(touch_index);
		
		switch (e.getActionMasked()) {
			case MotionEvent.ACTION_DOWN:
			case MotionEvent.ACTION_POINTER_DOWN:
				Game.TouchBegin(touch_id, (int) x, (int) y);
				Log.i("DEBUG","touch down:"+ touch_id +" x:"+ x + " " + y);
			break;
			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_POINTER_UP:
			case MotionEvent.ACTION_CANCEL:
				
				Game.TouchEnd(touch_id, (int) x, (int) y);
					Log.i("DEBUG","touch up:"+ touch_id +" x:"+ x + " " + y);
			break;
			case MotionEvent.ACTION_MOVE:
				for (int i = 0; i< e.getPointerCount(); i++) {
					int tmp_touch_id = e.getPointerId(i);
					Game.TouchDrag(tmp_touch_id, (int) e.getX(i), (int) e.getY(i));
				}
			break;
		}*/
		
		int touch_id = e.getPointerId (touch_index);
		float x = e.getX(touch_index);
		float y = e.getY(touch_index);
		
		
		switch (e.getActionMasked()) {
			case MotionEvent.ACTION_DOWN:
			case MotionEvent.ACTION_POINTER_DOWN:
				int gen_touch_id = _next_touch_id++;
				int gen_offset = -1;
				for(int i = 0; i< 10; i++){
					if(_touch_id[i] == -1){
						gen_offset = i;
						break;
					}
				}
				if(gen_offset == -1){
					//system error
					break;
				}
				_touch_id[gen_offset] = touch_id;
				_touch_gen_id[gen_offset] = gen_touch_id;
				
				Game.TouchBegin(gen_touch_id, (int) x, (int) y);
				//Log.i("DEBUG","touch down:"+ touch_id +" x:"+ x + " " + y);
			break;
			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_POINTER_UP:
			case MotionEvent.ACTION_CANCEL:
				for(int i = 0; i< 10; i++){
					if(_touch_id[i] == touch_id){
						Game.TouchEnd(_touch_gen_id[i], (int) x, (int) y);
						//Log.i("DEBUG","touch up:"+ touch_id +" x:"+ x + " " + y);
						_touch_id[i] = -1;
						break;
					}
				}
			break;
			case MotionEvent.ACTION_MOVE:
				for (int i = 0; i< e.getPointerCount(); i++) {
					int tmp_touch_id = e.getPointerId(i);
					for(int j = 0; j< 10; j++){
						if(_touch_id[j] == tmp_touch_id){
							//Log.i("DEBUG","touch move:" + tmp_touch_id + " index:" + i + " " + e.getX(i) + " " + e.getY(i));
							Game.TouchDrag(_touch_gen_id[j], (int) e.getX(i), (int) e.getY(i));
							break;
						}
					}
				}
			break;
		}
		return true;
	}
	
}