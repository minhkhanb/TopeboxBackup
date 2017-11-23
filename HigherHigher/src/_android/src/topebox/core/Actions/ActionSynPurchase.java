package topebox.core.Actions;
import topebox.core.*;
import android.util.Log;
import android.app.Activity;
import android.content.Context;
import android.os.AsyncTask;
import java.lang.Runnable;
import java.net.URI;
import java.util.*;
import java.nio.charset.Charset;
import java.security.NoSuchAlgorithmException;
import javax.crypto.NoSuchPaddingException;
import java.security.InvalidKeyException;
import java.security.spec.InvalidParameterSpecException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.BadPaddingException;
import java.io.UnsupportedEncodingException;
import javax.crypto.KeyGenerator;
import javax.crypto.*;
import javax.crypto.Cipher;
import android.util.Base64;
import javax.crypto.spec.SecretKeySpec;
import javax.crypto.spec.IvParameterSpec;
import java.lang.Long;

//iap
import 	android.content.Intent;


public class ActionSynPurchase implements Action{
	
	static ArrayList<ActionSynPurchaseArg> _watting = new ArrayList<ActionSynPurchaseArg>();
	
	ActionSynPurchaseArg Arg = null;
	public static IabHelper mHelper;
	
	public ActionSynPurchase(ActionSynPurchaseArg arg){
		Arg = arg;
		Arg.onBegin();
	}
	
	public void act()
	{
		try 
		{
			if(_watting.size() > 0)
			{
				Arg.onCancel();
				return;
			}
			String base64EncodedPublicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAkMjXADgmCkDQHI4SpRCXvsoK3LoYOfxDB/DRUN8ZB7HGzPDUWvkQKJqXATdqhF+F9baKvz+wyODXH4Q4BFmWtkoJdbv4CUXdg/iHuVh5/D2eZsAfWw+wSTblnaui6dAPt4zTtScHsORRdO+O7SHEVPIntqbAS7qmJcZP+CfeJH5TCFo88hhWfu35+rQQhLyS33Zx6duGYDSGQTytADY/whLn5zYizKk/7dIsqW32wtJ6m/Atjzs4C2gf60HHIhhfOoiH6nRRed6X6yTmDMXbv9g45qREENx2Bf8b83uxTs+/xDtRc1xno2fIfAcSkVZuZM2u29Asce3AF5Iyplnm2wIDAQAB";
		
			Log.d("DEBUG", "Creating IAB helper.");
			mHelper = new IabHelper(AppContext.CurrentContext, base64EncodedPublicKey);
			
			mHelper.enableDebugLogging(true);
			
			//Log.d("DEBUG", "Starting setup.");
			mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
				public void onIabSetupFinished(IabResult result) {
					Log.d("DEBUG", "Setup finished.");

					if (!result.isSuccess()) {
						// Oh noes, there was a problem.
						Log.i("DEBUG","Problem setting up in-app billing: " + result);
						Arg.onCancel();
						return;
					}

					// Have we been disposed of in the meantime? If so, quit.
					if (mHelper == null){
						Arg.onCancel();
						Log.i("DEBUG","Problem setting up in-app billing mHelper is null");
						return;
					}
					_watting.add(Arg);
					if (Arg._type == 0){		//subs
						mHelper.launchSubscriptionPurchaseFlow((Activity)AppContext.CurrentContext, Arg._sku, 100001, mPurchaseFinishedListener,"INAPP_PURCHASE_DATA");
					}
					else if (Arg._type == 1 || Arg._type == 3) {		//managed || managed-consumable
						mHelper.launchPurchaseFlow((Activity)AppContext.CurrentContext, Arg._sku, 100001, mPurchaseFinishedListener,"INAPP_PURCHASE_DATA");
					}
					else if (Arg._type == 2){				//restore
						mHelper.launchRestorePurchaseFlow((Activity)AppContext.CurrentContext, Arg._sku, 100001, mPurchaseFinishedListener,"INAPP_PURCHASE_DATA");
					}
				}
			});
		}
		catch (Exception ex) 
		{
			Arg.onCancel();
		}
	}

	IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener  = new IabHelper.OnIabPurchaseFinishedListener() 
	{
		public void onIabPurchaseFinished(IabResult result, Purchase purchase) 
		{
			Log.i("DEBUG", "onIabPurchaseFinished");
			if (result.isFailure()) 
			{
				Log.i("DEBUG", "Error purchasing: " + result);
				Arg.Success = false;
				if (result.getResponse() == IabHelper.BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED)
				{
					if (Arg._type == 3) //consumable
					{
						try {
							mHelper.consume(purchase);
						}catch (Exception e)
						{
							Log.i("DEBUG", "Consume purchase fail with exception: " + e.getMessage());
						}
					}
					else {
						Arg._isOwned = true;
					}
				}
				Arg.onDone();
				_watting.clear();
				return;
			}
			Arg.Success = true;
			Arg.onDone();
			_watting.clear();
			if (Arg._type == 3) //consumable
			{
				try {
					mHelper.consume(purchase);
				}catch (Exception e)
				{
					Log.i("DEBUG", "Consume purchase fail with exception: " + e.getMessage());
				}
			}
		}
	};
	
	
	static public void resolveCallback(int requestCode, int resultCode, Intent data)
	{
		Log.i("DEBUG","Purchase Resolve:");
		if(_watting.size() > 0)
		{
			ActionSynPurchaseArg arg  = _watting.get(0);
			Log.i("DEBUG", "arg sku:" + arg._sku);
			_watting.clear();
			arg.Success = (resultCode == Activity.RESULT_OK );
			arg.onDone();
		}
	}
}
