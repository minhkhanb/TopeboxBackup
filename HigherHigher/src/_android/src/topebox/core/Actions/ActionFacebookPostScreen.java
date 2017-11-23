//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core.Actions;
import topebox.core.AppContext;
import topebox.core.AppRegistry;
import topebox.core.FacebookManager;
import java.io.ByteArrayOutputStream;
import android.graphics.Bitmap;
import android.util.Log;
import android.os.Bundle;
import org.json.JSONObject;
import org.json.JSONException;

//#[001] import com.facebook.*;
//#[001] import com.facebook.Request;


public class ActionFacebookPostScreen{
	ActionFacebookPostScreenArg Arg = null;

	public ActionFacebookPostScreen(ActionFacebookPostScreenArg arg){
		Arg = arg;
	}
	public void act(){
		Log.i("DEBUG", "ActionFacebookPostScreen act");
		if(Arg == null){
			Log.i("DEBUG", "ActionFacebookPostScreen arg is null!!");
			return;
		}
		Arg.onBegin();
		AppContext.CurrentRegistry.regTakeScreenShot(new AppRegistry.TakeScreenShotCallBack(){
			public void onDone(){
				FacebookManager.postPicture(Arg);
			}
		});
		/*
		Arg.onBegin();
		final ActionFacebookPostArg arg = (ActionFacebookPostArg)Arg;
		AppContext.CurrentRegistry.regTakeScreenShot(new AppRegistry.TakeScreenShotCallBack(){
			public void onDone(){
				
				//arg.onDone();
				Bitmap screen = AppContext.CurrentRegistry.getScreenShotBitmap();
				if(screen != null){
					byte[] data = null;
					ByteArrayOutputStream baos = new ByteArrayOutputStream();
					screen.compress(Bitmap.CompressFormat.JPEG, 100, baos);
					data = baos.toByteArray();
			 
					Bundle params = new Bundle();
					//params.putString("method", "photos.upload");
					params.putByteArray("picture", data);
					params.putString("name", arg.Name);
					params.putString("caption", arg.Caption);
					params.putString("description", arg.Description);
					params.putString("link", arg.Link);
					
					final ActionArg arg = (ActionArg)Arg;
					SharePhoto photo = new SharePhoto.Builder().setBitmap(screen).build();
                    SharePhotoContent content = new SharePhotoContent.Builder().addPhoto(photo).build();
					Facebook.postPhoto(params, new Request.Callback() {
						public void onCompleted(Response response) {
							if(response == null){
								Log.i("DEBUG FACEBOOK", "response null");
							}else{
								Log.i("DEBUG FACEBOOK", "response not null" + response.toString());
							}
							try{
							JSONObject graphResponse = response
													   .getGraphObject()
													   .getInnerJSONObject();
								if(graphResponse == null){
									Log.i("DEBUG FACEBOOK", "graphResponse null");
								}
								String postId = null;
								try {
									postId = graphResponse.getString("id");
									
								} catch (JSONException e) {
									Log.i("DEBUG FACEBOOK",
										"JSON error "+ e.getMessage());
									arg.onCancel();
								}
								FacebookRequestError error = response.getError();
								if (error != null) {
									Log.i("DEBUG FACEBOOK",error.getErrorMessage());
									arg.onCancel();
								} else {
									Log.i("DEBUG FACEBOOK","abc" + postId);
									arg.onDone();
								}
							}catch(Exception ex){
								Log.i("DEBUG FACEBOOK","error "+ ex.getMessage());
								arg.onCancel();
							}
						}
					});
				}else{
					arg.onCancel();
				}
			}
		});
		*/
	}
}