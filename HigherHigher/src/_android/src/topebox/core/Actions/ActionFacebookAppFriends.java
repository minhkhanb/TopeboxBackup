package topebox.core.Actions;

import topebox.core.*;


//#[001] import com.facebook.*;
//#[001] import com.facebook.model.*;


import 	android.util.Log;
import android.os.Bundle;

	
import android.app.Activity;
import android.content.Context;
import org.json.*;

import java.util.*;
//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

import org.json.JSONException;

public class ActionFacebookAppFriends implements Action{
	ActionArg Arg = null;
	public ActionFacebookAppFriends(ActionArg arg){
		Arg = arg;
	}
	public void act(){
		if(Arg == null ){
			Log.i("Debug", "action get facebook friend error");
			return;
		}
		final ActionFacebookAppFriendsArg arg = (ActionFacebookAppFriendsArg)Arg;
		Log.i("DEBUG","begin do action get facebook friend");
		Arg.onBegin();
		FacebookManager.getAppFriends(arg);
		/*#[001]
		Facebook.getAppFriends( new Request.Callback() {
            public void onCompleted(Response response) {
				if(response == null){
					Log.i("DEBUG", "response null");
				}else{
					Log.i("DEBUG", "response not null" + response.toString());
				}
				try{
				JSONObject graphResponse = response
										   .getGraphObject()
										   .getInnerJSONObject();
										   
				JSONArray jsonMainArr = graphResponse.getJSONArray("data");
					
					arg._friend_ids.clear();
					try {
						for(int i = 0; i< jsonMainArr.length(); i++)
						{
							JSONObject obj = jsonMainArr.getJSONObject(i);
							arg._friend_ids.add(Long.parseLong(obj.getString("id")));
						}
						
						
					} catch (JSONException e) {
						Log.i("DEBUG",
							"JSON error "+ e.getMessage());
						arg.onCancel();
					}
					FacebookRequestError error = response.getError();
					if (error != null) {
						Log.i("DEBUG",error.getErrorMessage());
						arg.onCancel();
					} else {
						Log.i("DEBUG","abc");
						arg.onDone();
					}
				}catch(Exception ex){
					Log.i("DEBUG","error "+ ex.getMessage());
					arg.onCancel();
				}
				//arg.onCancel();
			}
		});
		//arg.onCancel();
		*/
	}
}