package topebox.core;
import topebox.core.*;
import topebox.core.Admob.*;
import topebox.core.Actions.*;
import 	android.util.Log;

public class ActionAdmodPrepareInterstitial implements Action{
	ActionArg Arg = null;
	public ActionAdmodPrepareInterstitial(ActionArg arg){
		Arg = arg;
	}
	public void act(){
		if(Arg == null){
			return;
		}
		Arg.onBegin();
		final ActionArg arg = Arg;
		if(AdmobManager.isPrepareInterstitialCompleted()){
			Arg.onDone();
		}else{
			final AdmobWaitingLoadCallback callback = new AdmobWaitingLoadCallback(){
				public void onDone(boolean completed){
					if(completed){
						arg.onDone();
					}else{
						arg.onCancel();
					}
					AdmobManager.removeWaitingInterstitialCallback(this);
				}
			};
			AdmobManager.addWaitingInterstitialCallback(callback);
			AdmobManager.prepareInterstitial(arg);
		}
	}
}