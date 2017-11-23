package topebox.core;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.opengl.GLSurfaceView;
import android.view.WindowManager;
import android.view.Display;
import 	android.content.Context;
import 	android.util.Log;
import android.graphics.Bitmap;
import java.nio.ByteBuffer;
import java.nio.ShortBuffer;
import java.nio.ByteOrder;

public class GLJNIRenderer implements GLSurfaceView.Renderer{
	protected int DisplayWidth;
	protected int DisplayHeight;
	public void onSurfaceCreated (GL10 gl, EGLConfig config){
		//OsUtils.Init();
		Log.i("DEBUG", "surface created");
		WindowManager wm = (WindowManager) topebox.core.AppContext.CurrentContext.getSystemService(Context.WINDOW_SERVICE);
		Display display = wm.getDefaultDisplay();
		//Display display = getWindowManager().getDefaultDisplay(); 
		DisplayWidth = display.getWidth();  // deprecated
		DisplayHeight = display.getHeight();
		Log.i("RENDERER","screen w:" + DisplayWidth + " screen h:" + DisplayHeight);
		if(!Game.IsInited){
			Game.Init(DisplayWidth, DisplayHeight);
		}else{
			Game.Suppend();
		}
    }
    public void onSurfaceChanged (GL10 gl, int w, int h){
		Log.i("DEBUG", "surface changed");
    }

    public void onDrawFrame (GL10 gl){
		//Log.i("DEBUG","Update - Render");
		Game.Update();
		Game.Render();
		
		if(AppContext.CurrentRegistry.needTakeScreenShot()){                     
			int screenshotSize = DisplayWidth * DisplayHeight;
			ByteBuffer bb = ByteBuffer.allocateDirect(screenshotSize * 4);
			bb.order(ByteOrder.nativeOrder());
			gl.glReadPixels(0, 0, DisplayWidth, DisplayHeight, GL10.GL_RGBA, GL10.GL_UNSIGNED_BYTE, bb);
			int pixelsBuffer[] = new int[screenshotSize];
			bb.asIntBuffer().get(pixelsBuffer);
			bb = null;
			Bitmap bitmap = Bitmap.createBitmap(DisplayWidth, DisplayHeight, Bitmap.Config.RGB_565);
			bitmap.setPixels(pixelsBuffer, screenshotSize-DisplayWidth, -DisplayWidth, 0, 0, DisplayWidth, DisplayHeight);
			pixelsBuffer = null;

			short sBuffer[] = new short[screenshotSize];
			ShortBuffer sb = ShortBuffer.wrap(sBuffer);
			bitmap.copyPixelsToBuffer(sb);

			//Making created bitmap (from OpenGL points) compatible with Android bitmap
			for (int i = 0; i < screenshotSize; ++i) {                  
				short v = sBuffer[i];
				sBuffer[i] = (short) (((v&0x1f) << 11) | (v&0x7e0) | ((v&0xf800) >> 11));
			}
			sb.rewind();
			bitmap.copyPixelsFromBuffer(sb);
			//lastScreenshot = bitmap;

			AppContext.CurrentRegistry.doneTakeScreenShot(bitmap);
		}
		//Log.i("DEBUG","--------");
		//try{
		//Thread.sleep(20);
		//}catch(Exception ex){}
    }
}