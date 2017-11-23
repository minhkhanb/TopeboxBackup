//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core;
import topebox.core.Actions.*;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Map;

public class StreamActionManager{
	Map dictionary = new HashMap();
	public void put(ActionArg arg, ServiceStream stream){
		dictionary.put(arg, stream);
	}
	public ServiceStream get(ActionArg arg){
		try{
			return (ServiceStream)dictionary.get(arg);
		}catch(Exception ex){
		
		}
		return null;
	}
	public ServiceStream remove(ActionArg arg){
		try{
			return (ServiceStream)dictionary.remove(arg);
		}catch(Exception ex){
		
		}
		return null;
	}
}