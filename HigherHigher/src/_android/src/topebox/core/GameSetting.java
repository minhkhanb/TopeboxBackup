package topebox.core;
import android.content.SharedPreferences;
import android.content.Context;
public class GameSetting{
	public Context CurrentContext = null;
	public static final String PREFS_NAME = "AppSetting";
	public GameSetting(Context context){
		CurrentContext = context;
	}
	public void saveSettingString(String key, String value){
		SharedPreferences settings = CurrentContext.getSharedPreferences(PREFS_NAME, 0);
		SharedPreferences.Editor editor = settings.edit();
		editor.putString("silentMode", value);
		editor.commit();
	}
	public void saveSettingBoolean(String key, boolean value){
		SharedPreferences settings = CurrentContext.getSharedPreferences(PREFS_NAME, 0);
		SharedPreferences.Editor editor = settings.edit();
		editor.putBoolean("silentMode", value);
		editor.commit();
	}
	public String loadSettingString(String key){
		SharedPreferences settings = CurrentContext.getSharedPreferences(PREFS_NAME, 0);
		return settings.getString(key, "");
	}
	public boolean loadSettingBoolean(String key){
		SharedPreferences settings = CurrentContext.getSharedPreferences(PREFS_NAME, 0);
		return settings.getBoolean(key, false);
	}
}