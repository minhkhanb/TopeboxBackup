package topebox.core.Actions;

import topebox.core.*;
import topebox.core.GooglePlay.GooglePlayManager;


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

public class ActionGooglePlayFriends implements Action{
	ActionArg Arg = null;
	public ActionGooglePlayFriends(ActionArg arg){
		Arg = arg;
	}
	public void act(){
		if(Arg == null ){
			Log.i("Debug", "action get Google Play friend error");
			return;
		}
		final ActionGooglePlayFriendsArg arg = (ActionGooglePlayFriendsArg)Arg;
		Log.i("DEBUG","begin do action get Google Play friend");
		Arg.onBegin();

		if(GooglePlayManager.IsServiceAvailable()) {
			GooglePlayManager.getGooglePlayFriendIds(arg);
		}
		else{
			Log.i("DEBUG", "Google play is not connect!!!");
			Arg.onCancel();
		}
	}
}