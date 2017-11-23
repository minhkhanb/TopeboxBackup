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
import com.android.vending.billing.IInAppBillingService;
import com.android.vending.billing.*;
import android.content.ServiceConnection;
import android.content.ComponentName;
import android.os.IBinder;
import android.os.RemoteException;
import org.json.JSONException;
import android.content.Context;

public class ActionSynPurchaseGetList implements Action{
	
	ActionSynPurchaseGetListArg Arg = null;
	IabHelper mHelper;
	
	public ActionSynPurchaseGetList(ActionSynPurchaseGetListArg arg){
		Arg = arg;
		Arg.onBegin();
	}
	
	public void act()
	{
		try 
		{
			String base64EncodedPublicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAkMjXADgmCkDQHI4SpRCXvsoK3LoYOfxDB/DRUN8ZB7HGzPDUWvkQKJqXATdqhF+F9baKvz+wyODXH4Q4BFmWtkoJdbv4CUXdg/iHuVh5/D2eZsAfWw+wSTblnaui6dAPt4zTtScHsORRdO+O7SHEVPIntqbAS7qmJcZP+CfeJH5TCFo88hhWfu35+rQQhLyS33Zx6duGYDSGQTytADY/whLn5zYizKk/7dIsqW32wtJ6m/Atjzs4C2gf60HHIhhfOoiH6nRRed6X6yTmDMXbv9g45qREENx2Bf8b83uxTs+/xDtRc1xno2fIfAcSkVZuZM2u29Asce3AF5Iyplnm2wIDAQAB";
		
			Log.d("IAP", "Creating IAB helper.");
			mHelper = new IabHelper(AppContext.CurrentContext, base64EncodedPublicKey);
			
			mHelper.enableDebugLogging(true);
			
			Log.d("IAP", "Starting setup.");
			mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
				public void onIabSetupFinished(IabResult result) {
					Log.d("IAP", "Setup finished.");

					if (!result.isSuccess()) {
						// Oh noes, there was a problem.
						Log.i("IAP","Problem setting up in-app billing: " + result);
						Arg.onCancel();
						return;
					}

					// Have we been disposed of in the meantime? If so, quit.
					if (mHelper == null) return;

					// IAB is fully set up. Now, let's get an inventory of stuff we own.
					Log.d("IAP", "Setup successful. Querying inventory.");
					
					// create list sku
					mHelper.queryInventoryAsync(true, Arg._listSkus, mGotInventoryListener);
				}
			});
		}
		catch (Exception ex) 
		{
			Arg.onCancel();
		}
	}
	
	IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
		
        public void onQueryInventoryFinished(final IabResult result, final Inventory inventory) {
            Log.d("DEBUG", "Query inventory finished.");
			final ActionSynPurchaseGetListArg arg = (ActionSynPurchaseGetListArg) Arg;
            // Have we been disposed of in the meantime? If so, quit.
            if (mHelper == null) return;

            // Is it a failure?
            if (result.isFailure()) {
				arg.onCancel();
                Log.d("DEBUG","Failed to query inventory: " + result);
                return;
            }
			
			//Log.d("IAP", "result: " + result.toString());
			//new Thread()
			//{
				//public void run()
				//{
					//((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable()
					//{
						//public void run()
						//{
							for (int i = 0; i < arg._listSkus.size(); i++)
							{
								Log.d("DEBUG", "iap:" + i + " request:" + arg._listSkus.get(i));
								SkuDetails detail = inventory.getSkuDetails(arg._listSkus.get(i));
								SkuInfo sku = new SkuInfo();
								if(detail != null)
								{
									String price = detail.getPrice();//.substring(1);
									Log.d("DEBUG", "price:" + price);
									sku._price = price;

									Purchase purchasedItem = inventory.getPurchase(arg._listSkus.get(i));
									if (purchasedItem != null) {
										Log.i("DEBUG", "Owned item: " + purchasedItem.getSku());
										sku._isOwned = true;
									}
									else{
										Log.i("DEBUG", "Not yet owned item: " + arg._listSkus.get(i));
										sku._isOwned = false;
									}

									arg._listResponsedSkus.add(sku);
								}else{
									Log.d("DEBUG", "detail null");
								}
								//arg._listPrices.add(inventory.getSkuDetails(arg._listSkus.get(i)).getPrice().substring(1));
							}
							arg.onDone();
						//}
					//});
				//}
			//}.start();
			
        }
    };
	
}
