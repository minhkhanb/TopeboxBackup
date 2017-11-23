package topebox.core.Actions;
import topebox.core.*;


//#[001] import com.facebook.*;
//#[001] import com.facebook.model.*;

import 	android.util.Log;
import android.os.Bundle;

	
import android.app.Activity;
import android.content.Context;
import org.json.JSONObject;
//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

import org.json.JSONException;

public class ActionFacebookPost implements Action{
	ActionArg Arg = null;
	public ActionFacebookPost(ActionArg arg){
		Arg = arg;
	}
	public void act(){
		if(Arg == null){
			Log.i("Debug", "action post facebook error");
			return;
		}
		Arg.onBegin();
		final ActionFacebookPostArg arg = (ActionFacebookPostArg)Arg;
		//FacebookManager.post(arg);
		
		/*#[001]
		
		Log.i("DEBUG","begin do action post facebook");
		Arg.onBegin();
		Bundle postParams = new Bundle();
		postParams.putString("name", arg.Name);
        postParams.putString("caption", arg.Caption);
        postParams.putString("description", arg.Description);
        postParams.putString("link", arg.Link);
        postParams.putString("picture", arg.Picture);
		
		Facebook.post(postParams, new Request.Callback() {
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
		//arg.onCancel();
		*/
	}
}