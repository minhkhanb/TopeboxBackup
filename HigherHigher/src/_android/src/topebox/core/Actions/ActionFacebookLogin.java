//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core.Actions;
import topebox.core.*;

//#[001] import com.facebook.*;
//#[001] import com.facebook.model.*;

import 	android.util.Log;

public class ActionFacebookLogin implements Action
{

	ActionArg Arg = null;
	public ActionFacebookLogin( ActionArg arg)
	{

		Arg = arg;
	}
	
	public void act()
	{
		if(Arg == null )
		{
			Log.i("Debug", "action login facebook error");
			return;
		}
		Log.i("DEBUG","begin do action login facebook");
		Arg.onBegin();
		
		FacebookManager.login((ActionFacebookLoginArg)Arg);
		
		//Arg.onDone();
		/*#[001]
		Facebook.login(new Session.StatusCallback (){
			public void call(Session session, SessionState state, Exception exception) {
				Log.i("DEBUG","callback login facebook");
				
				if(session.isOpened()){
					Log.i("DEBUG","callback with session is opened");
					Request request_me = Request.newMeRequest(session, new Request.GraphUserCallback(){
						public void onCompleted(GraphUser user, Response response){
							if(user != null){
								ActionFacebookLoginArg arg = (ActionFacebookLoginArg)Arg;
								arg.UserName = user.getName();
								arg.UserId = user.getId();
								Arg.onDone();
							}else{
								Arg.onCancel();
							}
						}
					});
					request_me.executeAsync();
				}else{
					if(state != SessionState.OPENING && state != SessionState.CREATED && state != SessionState.CREATED_TOKEN_LOADED){
						Log.i("DEBUG","callback with session is closed" + state);
						Arg.onCancel();
					}else{
						Log.i("DEBUG","callback with session state: " + state);
					}
					
				}
			}
		});
		//arg.onCancel();
		*/
	}
}