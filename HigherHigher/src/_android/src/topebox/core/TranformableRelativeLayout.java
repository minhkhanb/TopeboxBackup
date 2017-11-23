package topebox.core;

import android.widget.RelativeLayout;
import android.widget.RelativeLayout.*;

import android.content.Context;

import android.util.AttributeSet;

import android.graphics.Canvas;
import android.graphics.Rect;
import android.view.ViewParent;
import android.view.*;
import 	android.util.Log;
import android.content.Context;
import android.app.Activity;
import java.lang.*;


public class TranformableRelativeLayout extends RelativeLayout {


	private float _scale_x = 1.0f;
	private float _scale_y = 1.0f;
    // private long lastTouchTime = -1;

    public TranformableRelativeLayout(Context context)
    {
        super(context);

    //  setWillNotDraw(false);

    }

    public TranformableRelativeLayout(Context context, AttributeSet attrs) {
        super(context, attrs);
        //setWillNotDraw(false);


        // TODO Auto-generated constructor stub
    }


/*  @Override 
    public boolean dispatchTouchEvent(MotionEvent event) { 
                return super.dispatchTouchEvent(event); 
       } */

	/*
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        // TODO Auto-generated method stub

        return super.onTouchEvent(event);
    }


    @Override
      public boolean onInterceptTouchEvent(MotionEvent ev) {

        if (ev.getAction() == MotionEvent.ACTION_DOWN) {

          long thisTime = System.currentTimeMillis();
          if (thisTime - lastTouchTime < 250) {

            // Double tap
              mScaleFactor=1.5f;
              invalidate();
            lastTouchTime = -1;

          } else {

            // Too slow :)
            /*  mScaleFactor=1.0f;
              invalidate();*//*
            lastTouchTime = thisTime;
          }
        }

        return super.onInterceptTouchEvent(ev);
      }

*/

	public void setScale(final float scalex,final float scaley)
	{
		((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
			public void run() {
				//_scale_x = 1.0f/scalex;
				//_scale_y = 1.0f/scaley;
				Log.i("DEBUG", "setscale: sx:" + scalex + " sy:" + scaley);
			}
		});
	}
    @Override
    protected void dispatchDraw(Canvas canvas) {
        // TODO Auto-generated method stub

            canvas.save(Canvas.MATRIX_SAVE_FLAG);
            canvas.scale(_scale_x, _scale_y);
            super.dispatchDraw(canvas);
            canvas.restore();
    }


    @Override
    public ViewParent invalidateChildInParent(int[] location, Rect dirty) {
        // TODO Auto-generated method stub
        return super.invalidateChildInParent(location, dirty);
    }

    protected void onLayout(boolean changed, int l, int t, int r, int b)
    {
        int count = getChildCount();
        for(int i=0;i<count;i++){
            View child = getChildAt(i); 
            if(child.getVisibility()!=GONE){
                RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams)child.getLayoutParams();
                child.layout(
                    (int)(params.leftMargin * _scale_x), 
                    (int)(params.topMargin * _scale_y), 
                    (int)((params.leftMargin + child.getMeasuredWidth()) * _scale_x), 
                    (int)((params.topMargin + child.getMeasuredHeight()) * _scale_y) 
                    );
            }
        }
    }
}