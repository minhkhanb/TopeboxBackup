//Pine-entertainment
//Android Framework
//duy.nguyen 2014

package topebox.core;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.pm.ApplicationInfo;
import 	android.util.Log;

import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import android.content.Context;
import android.app.Activity;
import android.content.Intent;
import android.net.Uri;

import topebox.core.Actions.*;
import topebox.core.Firebase.FirebaseManager;
import topebox.core.GooglePlay.*;
import topebox.game.Notification.TopeboxNotificationManager;

import 	com.google.android.gms.plus.PlusShare;

// chartboost 
// import com.chartboost.sdk.CBLocation;
// import com.chartboost.sdk.Chartboost;
// import com.chartboost.sdk.ChartboostDelegate;
// import com.chartboost.sdk.InPlay.CBInPlay;
// import com.chartboost.sdk.Libraries.CBLogging.Level;
// import com.chartboost.sdk.Model.CBError.CBClickError;
// import com.chartboost.sdk.Model.CBError.CBImpressionError;
// import com.chartboost.sdk.Tracking.CBAnalytics;

// unity3d ads
// import com.unity3d.ads.android.BuildConfig;
// import com.unity3d.ads.android.UnityAds;
// import com.unity3d.ads.android.IUnityAdsListener;
// import com.unity3d.ads.android.view.*;

import static topebox.core.OsFunctions.GetDataPath;

public class Game
{
	static public boolean IsInited = false;
	static public ClientService GameService = null;
	static public StreamActionManager _stream_mng = new StreamActionManager();
	static public ArrayList<ServiceStream> _stream_response = new ArrayList<ServiceStream>();
	static public void RegGameService(ClientService service)
	{
		GameService = service;
	}
	
	static public void processService(final ServiceStream stream){
		//if(AppContext.GLView != null){
		//	AppContext.GLView.queueEvent(new Runnable(){
		//		@Override
		//		public void run() {
					
				
		switch(stream.ServiceCode){
			//FACEBOOK LOGIN
			case ServiceCode.FacebookLogin:
			{
				((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
					public void run() {
						Log.i("DEBUG","call service Login Facebook");

						ActionFacebookLoginArg arg = new ActionFacebookLoginArg(){
							public void onBegin(){
								Log.i("DEBUG","Action facebook login begin");
							}
							public void onDone(){
								final ActionFacebookLoginArg fbarg = this;
								((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
									public void run() {
										Log.i("DEBUG","Action facebook login done");
										FacebookManager.AccountName = fbarg.UserName;
										FacebookManager.AccountId = fbarg.UserId;
										
										int data_size = 2 + (2 * FacebookManager.AccountName.length()) + 2 + (2* FacebookManager.AccountId .length());
										StreamBuffer buff = new StreamBuffer(data_size);
										buff.writeSStrRevert(FacebookManager.AccountId );
										buff.writeSStrRevert(FacebookManager.AccountName);
										Log.i("DEBUG STREAM","facebook login response data len:" + buff.Data().length);
										ServiceStream actstream = Game._stream_mng.get(fbarg);
										if(actstream != null){
											actstream.ErrorCode = 0;
											actstream.ResponseData = buff.Data();
											Log.i("DEBUG STREAM", "facebook response data:" + actstream.ResponseData.length);
											_stream_response.add(actstream);
											Game._stream_mng.remove(fbarg);
										}else{
											Log.i("DEBUG","some thing wrong!!");
										}
									}
								});
							}
							public void onCancel() {
								Log.i("DEBUG", "Action facebook login cancel");
								ServiceStream actstream = Game._stream_mng.get(this);
								if (actstream != null) {
									Game._stream_mng.remove(this);
									actstream.ErrorCode = 1;
									_stream_response.add(actstream);
								}

							}
						};
						_stream_mng.put(arg, stream);
						ActionFacebookLogin act_fblogin = new ActionFacebookLogin(arg);
						act_fblogin.act();
					}
				});
			}break;
			
			//FACEBOOK AVATAR
			case ServiceCode.FacebookGetAvatar:
			{
				try {
					((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							Log.i("DEBUG", "call service Download Facebook avatar");
							ActionFacebookDownloadAvatarArg arg = new ActionFacebookDownloadAvatarArg() {
								public void onBegin() {
									Log.i("DEBUG", "Action Download Facebook avatar begin");
								}

								public void onDone() {
									Log.i("DEBUG", "Action Download Facebook avatar done");
									try {
										ServiceStream actstream = Game._stream_mng.get(this);
										if (actstream != null) {
											actstream.ErrorCode = 0;
											actstream.ResponseData = null;
											_stream_response.add(actstream);
											Game._stream_mng.remove(this);
											Log.i("DEBUG", "Action Download Facebook avatar remove this stream success");
										} else {
											Log.i("DEBUG", "Action Download Facebook avatar some thing wrong");
										}
									} catch (Exception e){
										Log.e("DEBUG", "Action Download Facebook avatar done but error when remove stream with exception: " + e.getMessage());
									}
								}

								public void onCancel() {
									Log.i("DEBUG", "Action Download Facebook avatar cancel");
									ServiceStream actstream = Game._stream_mng.get(this);
									if (actstream != null) {
										Game._stream_mng.remove(this);
										actstream.ErrorCode = 1;
										_stream_response.add(actstream);
									}
								}
							};
							if (stream.RequestData != null) {
								StreamBuffer buf = new StreamBuffer(stream.RequestData);
								String userURL = buf.readSStrRevert();
								String fileName = buf.readSStrRevert();
								int imgWidth = buf.readInt32Revert();
								int imgHeight = buf.readInt32Revert();

								arg.userURL = userURL.replaceAll("FB_3", "");
								arg.fileName = fileName;
								arg.width = imgWidth;
								arg.height = imgHeight;
							}
							_stream_mng.put(arg, stream);
							ActionFacebookDownloadAvatar act_fbavatar = new ActionFacebookDownloadAvatar(arg);
							act_fbavatar.act();
						}
					});
				}catch (Exception e){
					Log.e("DEBUG", "call service Download facebook avatar error with exception: " + e.getMessage());
				}
			}break;
			
			//FACEBOOK POST
			case ServiceCode.FacebookPost:
			{
				((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
					public void run() {
						ActionFacebookPostArg arg = new ActionFacebookPostArg(){
							public void onBegin(){
									Log.i("DEBUG","Action facebook post begin  ");
								}
								public void onDone(){
									Log.i("DEBUG","Action facebook post done");
									ServiceStream actstream = Game._stream_mng.get(this);
									if(actstream != null){
										actstream.ErrorCode = 0;
										_stream_response.add(actstream);
										Game._stream_mng.remove(this);
									}else{
										Log.i("DEBUG","some thing wrong!!!");
									}
								}
								public void onCancel(){
									Log.i("DEBUG","Action facebook post cancel");
									ServiceStream actstream = Game._stream_mng.get(this);
									if(actstream != null){
										actstream.ErrorCode = 1;
										_stream_response.add(actstream);
										Game._stream_mng.remove(this);
									}
								}
						};
						if(stream.RequestData != null){
							StreamBuffer buf = new StreamBuffer(stream.RequestData);
							String msg = buf.readSStrRevert();
					
							//arg.Name = "Test";
							arg.Caption = msg;
							//arg.Description = "Test Android Description";
							arg.Link = "http://www.pine-entertainment.com";
							//arg.Picture = "";
						}
						_stream_mng.put(arg, stream);
						ActionFacebookPost act_fbpost = new ActionFacebookPost( arg);
						act_fbpost.act();
					}
				});
				
			}break;
			//FACEBOOK GET FRIEND
			case ServiceCode.FacebookGetAppFriend:{
				((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
					public void run() {
						ActionFacebookAppFriendsArg arg = new ActionFacebookAppFriendsArg(){
							public void onBegin(){
									Log.i("DEBUG","Action facebook get frined begin");
								}
								public void onDone(){
									Log.i("DEBUG","Action facebook get friend done");
									ServiceStream actstream = Game._stream_mng.get(this);
									if(actstream != null){
										actstream.ErrorCode = 0;
										
										int data_size = 4;
										if (this._friend_list.size() > 0)
										{
											for (int i = 0; i < this._friend_list.size(); i ++)
											{
												data_size += 8;
												data_size += 2 + 2 * this._friend_list.get(i)._name.length();
											}
										}
										StreamBuffer buff = new StreamBuffer(data_size);
										
										buff.writeInt32Revert(this._friend_list.size());
										for(int i = 0; i< this._friend_list.size(); i++)
										{
											buff.writeInt64Revert(this._friend_list.get(i)._id);
											buff.writeSStrRevert(this._friend_list.get(i)._name);
										}

										actstream.ResponseData = buff.Data();
							
							
										_stream_response.add(actstream);
										Game._stream_mng.remove(this);
									}else{
										Log.i("DEBUG","some thing wrong");
									}
								}
								public void onCancel(){
									Log.i("DEBUG","Action facebook get friend cancel");
									ServiceStream actstream = Game._stream_mng.get(this);
									if(actstream != null){
										actstream.ErrorCode = 1;
										_stream_response.add(actstream);
										Game._stream_mng.remove(this);
									}
								}
						};
						
						_stream_mng.put(arg, stream);
						ActionFacebookAppFriends act = new ActionFacebookAppFriends(arg);
						act.act();
					}
				});
				
			}break;
			
			case ServiceCode.AdmobInterstitialPrepare:{
				try {
					if (!AppConfig.UseAdmobInterstitial)
					{
						Log.i("DEBUG", "Admob is not enable! See _android/res/string/string.xml");
						stream.ErrorCode = 2;
						_stream_response.add(stream);
						break;
					}
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							ActionArg arg = new ActionArg(){
								public void onBegin(){
									Log.i("DEBUG","Action admod prepare interstitial begin");
								}
								public void onDone(){
									final ActionArg gparg = this;
									((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
										public void run() {
											Log.i("DEBUG","Action admod prepare interstitial done");

											ServiceStream actstream = Game._stream_mng.get(gparg);
											if(actstream != null){
												actstream.ErrorCode = 0;
												_stream_response.add(actstream);
												Game._stream_mng.remove(gparg);
											}else{
												Log.i("DEBUG","Action admod prepare interstitial fail - some thing wrong in action stream!!!");
											}
										}
									});
								}
								public void onCancel() {
									Log.i("DEBUG", "Action admod prepare interstitial cancel");
									ServiceStream actstream = Game._stream_mng.get(this);
									if (actstream != null) {
										Game._stream_mng.remove(this);
										actstream.ErrorCode = 1;
										_stream_response.add(actstream);
									}
								}
							};
							_stream_mng.put(arg, stream);
							AdmobManager.prepareInterstitial(arg);
						}
					});
				} catch (Exception ex) {
					stream.ErrorCode = 1;
					_stream_response.add(stream);
					Log.i("DEBUG", "AdMob prepare ADS error: %s" + ex.getMessage());
				}
			}break;
			case ServiceCode.AdmobInterstitialShow:{
				try {
					if (!AppConfig.UseAdmobInterstitial)
					{
						Log.i("DEBUG", "Admob is not enable! See _android/res/string/string.xml");
						stream.ErrorCode = 1;
						_stream_response.add(stream);
						break;
					}
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							ActionArg arg = new ActionArg(){
								public void onBegin(){
									Log.i("DEBUG","Action admod show interstitial begin");
								}
								public void onDone(){
									final ActionArg gparg = this;
									((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
										public void run() {
											Log.i("DEBUG","Action admod show interstitial show done");

											ServiceStream actstream = Game._stream_mng.get(gparg);
											if(actstream != null){
												actstream.ErrorCode = 0;
												_stream_response.add(actstream);
												Game._stream_mng.remove(gparg);
											}else{
												Log.i("DEBUG","Action admod show interstitial fail - some thing wrong in action stream!!!");
											}
										}
									});
								}
								public void onCancel() {
									Log.i("DEBUG", "Action admod show interstitial cancel");
									ServiceStream actstream = Game._stream_mng.get(this);
									if (actstream != null) {
										Game._stream_mng.remove(this);
										actstream.ErrorCode = 1;
										_stream_response.add(actstream);
									}
								}
							};
							_stream_mng.put(arg, stream);
							AdmobManager.showInterstitial(arg);
						}
					});
				} catch (Exception ex) {
					stream.ErrorCode = 1;
					_stream_response.add(stream);
					Log.i("DEBUG", "SHOW ADMOB interstitial ads error: %s" + ex.getMessage());
				}
			}break;
			case ServiceCode.AdmobBannerPrepare:{
				try {
					if (!AppConfig.UseAdmobBanner)
					{
						Log.i("DEBUG", "Admob banner is not enable! See _android/res/string/string.xml");
						stream.ErrorCode = 2;
						_stream_response.add(stream);
						break;
					}
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							ActionArg arg = new ActionArg(){
								public void onBegin(){
									Log.i("DEBUG","Action admod prepare banner begin");
								}
								public void onDone(){
									final ActionArg gparg = this;
									((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
										public void run() {
											Log.i("DEBUG","Action admod prepare banner done");

											ServiceStream actstream = Game._stream_mng.get(gparg);
											if(actstream != null){
												actstream.ErrorCode = 0;
												_stream_response.add(actstream);
												Game._stream_mng.remove(gparg);
											}else{
												Log.i("DEBUG","Action admod prepare banner fail - some thing wrong in action stream!!!");
											}
										}
									});
								}
								public void onCancel() {
									Log.i("DEBUG", "Action admod prepare banner cancel");
									ServiceStream actstream = Game._stream_mng.get(this);
									if (actstream != null) {
										Game._stream_mng.remove(this);
										actstream.ErrorCode = 1;
										_stream_response.add(actstream);
									}
								}
							};
							_stream_mng.put(arg, stream);
							AdmobManager.prepareBannerAds(arg);
						}
					});
				} catch (Exception ex) {
					stream.ErrorCode = 1;
					_stream_response.add(stream);
					Log.i("DEBUG", "AdMob prepare banner ADS error: %s" + ex.getMessage());
				}
			}break;
			case ServiceCode.AdmobBannerShow:{
					try {
						if (!AppConfig.UseAdmobBanner)
						{
							Log.i("DEBUG", "Admob banner is not enable! See _android/res/string/string.xml");
							stream.ErrorCode = 1;
							_stream_response.add(stream);
							break;
						}
						((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
							public void run() {
								ActionArg arg = new ActionArg(){
									public void onBegin(){
										Log.i("DEBUG","Action admod show banner begin");
									}
									public void onDone(){
										final ActionArg gparg = this;
										((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
											public void run() {
												Log.i("DEBUG","Action admod show banner show done");

												ServiceStream actstream = Game._stream_mng.get(gparg);
												if(actstream != null){
													actstream.ErrorCode = 0;
													_stream_response.add(actstream);
													Game._stream_mng.remove(gparg);
												}else{
													Log.i("DEBUG","Action admod show banner fail - some thing wrong in action stream!!!");
												}
											}
										});
									}
									public void onCancel() {
										Log.i("DEBUG", "Action admod show banner cancel");
										ServiceStream actstream = Game._stream_mng.get(this);
										if (actstream != null) {
											Game._stream_mng.remove(this);
											actstream.ErrorCode = 1;
											_stream_response.add(actstream);
										}
									}
								};
								_stream_mng.put(arg, stream);
								AdmobManager.showBannerAds(arg);
							}
						});
					} catch (Exception ex) {
						stream.ErrorCode = 1;
						_stream_response.add(stream);
						Log.i("DEBUG", "SHOW ADMOB banner ads error: %s" + ex.getMessage());
					}
			}break;
			case ServiceCode.AdmobBannerHide:{
				try {
					if (!AppConfig.UseAdmobBanner)
					{
						Log.i("DEBUG", "Admob banner is not enable! See _android/res/string/string.xml");
						stream.ErrorCode = 1;
						_stream_response.add(stream);
						break;
					}
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							ActionArg arg = new ActionArg(){
								public void onBegin(){
									Log.i("DEBUG","Action admod hide banner begin");
								}
								public void onDone(){
									final ActionArg gparg = this;
									((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
										public void run() {
											Log.i("DEBUG","Action admod hide banner show done");

											ServiceStream actstream = Game._stream_mng.get(gparg);
											if(actstream != null){
												actstream.ErrorCode = 0;
												_stream_response.add(actstream);
												Game._stream_mng.remove(gparg);
											}else{
												Log.i("DEBUG","Action admod hide banner fail - some thing wrong in action stream!!!");
											}
										}
									});
								}
								public void onCancel() {
									Log.i("DEBUG", "Action admod hide banner cancel");
									ServiceStream actstream = Game._stream_mng.get(this);
									if (actstream != null) {
										Game._stream_mng.remove(this);
										actstream.ErrorCode = 1;
										_stream_response.add(actstream);
									}
								}
							};
							_stream_mng.put(arg, stream);
							AdmobManager.hideBannerAds(arg);
						}
					});
				} catch (Exception ex) {
					stream.ErrorCode = 1;
					_stream_response.add(stream);
					Log.i("DEBUG", "SHOW ADMOB banner ads error: %s" + ex.getMessage());
				}
			}break;
			case ServiceCode.AdmobRewardedVideoPrepare: {
				try {
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							if (AdmobManager.getVideoAdsInstance().isLoaded()) {
								stream.ErrorCode = 0;
								_stream_response.add(stream);
							} else {
								stream.ErrorCode = 1;
								_stream_response.add(stream);
							}

						}
					});
				}catch (Exception ex) {}
			} break;
			case ServiceCode.AdmobRewardedVideoShow: {
				try {
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							ActionAdmobRewardedVideoAdsShowArg arg = new ActionAdmobRewardedVideoAdsShowArg(){
								public void onBegin(){
									Log.i("DEBUG","Action Admob rewarded video ads show begin");
								}
								public void onDone(){
									final ActionAdmobRewardedVideoAdsShowArg gparg = this;
									((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
										public void run() {
											Log.i("DEBUG","Action Admob rewarded video ads show done");

											ServiceStream actstream = Game._stream_mng.get(gparg);
											if(actstream != null){
												actstream.ErrorCode = 0;
												_stream_response.add(actstream);
												Game._stream_mng.remove(gparg);
											}else{
												Log.i("DEBUG","Action Admob rewarded video ads show some thing wrong!!");
											}
										}
									});
								}
								public void onCancel() {
									Log.i("DEBUG", "Action Admob rewarded video ads show cancel");
									ServiceStream actstream = Game._stream_mng.get(this);
									if (actstream != null) {
										Game._stream_mng.remove(this);
										actstream.ErrorCode = 1;
										_stream_response.add(actstream);
									}
								}
							};
							_stream_mng.put(arg, stream);
							AdmobManager.ShowRewardedVideo(arg);
						}
					});
				} catch (Exception ex) {
					Log.i("DEBUG", "SHOW ADMOD REWARDED VIDEO ADS error: %s" + ex.getMessage());
				}
			} break;
			case ServiceCode.TakeScreenShot:{
				
				ActionArg arg = new ActionArg(){
					public void onBegin(){
						Log.i("DEBUG SCREENSHOT","Action take screenshot begin");
					}
					public void onDone(){
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							actstream.ErrorCode = 0;
							_stream_response.add(actstream);
							Game._stream_mng.remove(this);
						}else{
							Log.i("DEBUG","some thing wrong");
						}
					}
					public void onCancel(){
						Log.i("DEBUG","Action facebook login cancel");
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							actstream.ErrorCode = 1;
							_stream_response.add(actstream);
							//reportService(actstream);
							Game._stream_mng.remove(this);
						}
					}
				};
				_stream_mng.put(arg, stream);
				ActionTakeScreenShot action = new ActionTakeScreenShot(arg);
				action.act();
			}break;
			case ServiceCode.FacebookPostScreen:{
				
				ActionFacebookPostScreenArg arg = new ActionFacebookPostScreenArg(){
					public void onBegin(){
						Log.i("DEBUG","Action facebook post screen begin");
					}
					public void onDone(){
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							Log.i("DEBUG","Action facebook post screen done");
							actstream.ErrorCode = 0;
							_stream_response.add(actstream);
							Game._stream_mng.remove(this);
						}else{
							Log.i("DEBUG","some thing wrong");
						}
					}
					public void onCancel(){
						Log.i("DEBUG","Action facebook post screen cancel");
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							actstream.ErrorCode = 1;
							_stream_response.add(actstream);
							Game._stream_mng.remove(this);
						}
					}
				};
				if(stream.RequestData != null){
					StreamBuffer buf = new StreamBuffer(stream.RequestData);
					String msg = buf.readSStrRevert() + "\nhttps://play.google.com/store/apps/details?id=" + GetDataPath() + "&hl=en";
					arg.Name = "";
					arg.Caption = msg;
					arg.Description = "";
					arg.Link = "http://www.pine-entertainment.com";
					Log.i("DEBUG", "post fb:" + msg);
				}else{
					Log.i("DEBUG", "stream request null");
				}
				_stream_mng.put(arg, stream);
				ActionFacebookPostScreen action = new ActionFacebookPostScreen(arg);
				action.act();
				
			}break;
			case ServiceCode.ShareIntent:{
				ActionShareIntentArg arg = new ActionShareIntentArg(){
					public void onBegin(){
						Log.i("DEBUG","Action share screen shot intent begin");
					}
					public void onDone(){
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							Log.i("DEBUG","Action share screen shot intent done");
							actstream.ErrorCode = 0;
							_stream_response.add(actstream);
							Game._stream_mng.remove(this);
						}else{
							Log.i("DEBUG","Action share screen shot intent some thing wrong");
						}
					}
					public void onCancel(){
						Log.i("DEBUG","Action share screen shot intent cancel");
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							actstream.ErrorCode = 1;
							_stream_response.add(actstream);
							Game._stream_mng.remove(this);
						}
					}
				};
				if(stream.RequestData != null){
					StreamBuffer buf = new StreamBuffer(stream.RequestData);
					String msg = buf.readSStrRevert();
					String name = buf.readSStrRevert();
					arg.Name = name;
					arg.Caption = msg;
					arg.Description = "https://play.google.com/store/apps/details?id=" + GetDataPath() + "&hl=en";;
					arg.Link = "http://www.pine-entertainment.com";
					Log.i("DEBUG", "share screen shot intent fb:" + msg);
				}else{
					Log.i("DEBUG", "stream request null");
				}
				_stream_mng.put(arg, stream);
				ActionShareIntent action = new ActionShareIntent(arg);
				action.act();

			}break;
			case ServiceCode.ShareScreenShotIntent:{
				ActionShareScreenShotIntentArg arg = new ActionShareScreenShotIntentArg(){
					public void onBegin(){
						Log.i("DEBUG","Action share screen shot intent begin");
					}
					public void onDone(){
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							Log.i("DEBUG","Action share screen shot intent done");
							actstream.ErrorCode = 0;
							_stream_response.add(actstream);
							Game._stream_mng.remove(this);
						}else{
							Log.i("DEBUG","Action share screen shot intent some thing wrong");
						}
					}
					public void onCancel(){
						Log.i("DEBUG","Action share screen shot intent cancel");
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							actstream.ErrorCode = 1;
							_stream_response.add(actstream);
							Game._stream_mng.remove(this);
						}
					}
				};
				if(stream.RequestData != null){
					StreamBuffer buf = new StreamBuffer(stream.RequestData);
					String msg = buf.readSStrRevert() + "\nhttps://play.google.com/store/apps/details?id=" + GetDataPath() + "&hl=en";
					arg.Name = "";
					arg.Caption = msg;
					arg.Description = "";
					arg.Link = "http://www.pine-entertainment.com";
					Log.i("DEBUG", "share screen shot intent fb:" + msg);
				}else{
					Log.i("DEBUG", "stream request null");
				}
				_stream_mng.put(arg, stream);
				ActionShareScreenShotIntent action = new ActionShareScreenShotIntent(arg);
				action.act();
				
			}break;
			case ServiceCode.SendEmailIntent:{
				ActionSendEmailIntentArg arg = new ActionSendEmailIntentArg(){
					public void onBegin(){
						Log.i("DEBUG","Action send email intent begin");
					}
					public void onDone(){
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							Log.i("DEBUG","Action send email intent done");
							actstream.ErrorCode = 0;
							_stream_response.add(actstream);
							Game._stream_mng.remove(this);
						}else{
							Log.i("DEBUG","Action send email intent some thing wrong");
						}
					}
					public void onCancel(){
						Log.i("DEBUG","Action send email intent cancel");
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							actstream.ErrorCode = 1;
							_stream_response.add(actstream);
							Game._stream_mng.remove(this);
						}
					}
				};
				if(stream.RequestData != null){
					StreamBuffer buf = new StreamBuffer(stream.RequestData);
					String title = buf.readSStrRevert();
					String body = buf.readSStrRevert();
					arg.Title = title;
					arg.Body = body;
					arg.To = new String [] {"tope@topebox.com"};
					Log.i("DEBUG", "Send email intent title:" + title + "; body: " + body);
				}else{
					Log.i("DEBUG", "stream request null");
				}
				_stream_mng.put(arg, stream);
				ActionSendEmailIntent action = new ActionSendEmailIntent(arg);
				action.act();
			}break;
			case ServiceCode.FacebookAppInvite:{
				ActionFacebookAppInviteArg arg = new ActionFacebookAppInviteArg(){
					public void onBegin(){
						Log.i("DEBUG","Action facebook appinvite dialog show begin");
					}
					public void onDone(){
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							actstream.ErrorCode = 0;
							_stream_response.add(actstream);
							Game._stream_mng.remove(this);
							Log.i("DEBUG","Action facebook appinvite dialog show done");
						}else{
							Log.i("DEBUG","Action facebook some thing wrong");
						}
					}
					public void onCancel(){
						Log.i("DEBUG","Action facebook appinvite cancel");
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							actstream.ErrorCode = 1;
							_stream_response.add(actstream);
							//reportService(actstream);
							Game._stream_mng.remove(this);
						}
					}
				};
				arg.CurrentContext = AppContext.CurrentContext;
				if(stream.RequestData != null){
					StreamBuffer buf = new StreamBuffer(stream.RequestData);
					String appurl = buf.readSStrRevert();
					String imgurl = buf.readSStrRevert();
					//arg.Name = "Test";
					arg.AppLinkUrl = appurl;
					arg.PreviewImageUrl = imgurl;
					//arg.Description = msg;
				}else{
					Log.i("DEBUG", "Action facebook appinvite stream request null");
				}
				_stream_mng.put(arg, stream);
				ActionFacebookAppInvite act_fbinvite = new ActionFacebookAppInvite(arg);
				act_fbinvite.act();
			}break;
			case ServiceCode.DialogShow:{
				ActionDialogShowArg arg = new ActionDialogShowArg(){
					public void onBegin(){
						Log.i("DEBUG","Action dialog show begin");
					}
					public void onDone(){
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							if(this.Which == -1) {
								actstream.ErrorCode = 0;
							}
							else
							{
								actstream.ErrorCode = 1;
							}
							_stream_response.add(actstream);
							Game._stream_mng.remove(this);
						}else{
							Log.i("DEBUG","some thing wrong");
						}
					}
					public void onCancel(){
						Log.i("DEBUG","Action dialog show cancel");
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							actstream.ErrorCode = 1;
							_stream_response.add(actstream);
							//reportService(actstream);
							Game._stream_mng.remove(this);
						}
					}
				};
				if(stream.RequestData != null){
					StreamBuffer buf = new StreamBuffer(stream.RequestData);
					arg.Title = buf.readSStrRevert();
					arg.Message = buf.readSStrRevert();
					arg.Type = buf.readInt32Revert();
				}else{
					Log.i("DEBUG", "Action dialog show stream request null!");
				}
				_stream_mng.put(arg, stream);
				ActionDialogShow act_dialogshow = new ActionDialogShow(arg, AppContext.CurrentContext);
				act_dialogshow.act();
			}break;
			case ServiceCode.GameExit:{
				Log.i("DEBUG","exit game");
				StreamBuffer buf = new StreamBuffer(stream.RequestData);
				int suppen = buf.readInt32Revert();
				
				if(suppen == 1 || suppen == 3)
				{
					stream.ErrorCode = 0;
					_stream_response.add(stream);
					if (suppen == 1) {
						android.os.Process.killProcess(android.os.Process.myPid());
						System.exit(1);
					}
					else
					{
						restart();
					}
				}
				else
				{
					final int fSuppen = suppen;
					ActionDialogShowArg arg = new ActionDialogShowArg(){
						
						public void onBegin(){
							Log.i("DEBUG","Action dialog show begin");
						}
						public void onDone(){
							ServiceStream actstream = Game._stream_mng.get(this);
							if(actstream != null){
								actstream.ErrorCode = 0;
								_stream_response.add(actstream);
								Game._stream_mng.remove(this);
								if(this.Which == -1){
									if (fSuppen == 0) {
										android.os.Process.killProcess(android.os.Process.myPid());
										System.exit(1);
									}
									else
									{
										restart();
									}
								}
							}else{
								Log.i("DEBUG","some thing wrong");
							}
							Log.i("DEBUG","Which:" + this.Which);
						}
						public void onCancel(){
							Log.i("DEBUG","Action dialog show cancel");
							ServiceStream actstream = Game._stream_mng.get(this);
							if(actstream != null){
								actstream.ErrorCode = 1;
								_stream_response.add(actstream);
								//reportService(actstream);
								Game._stream_mng.remove(this);
							}
						}
					};
					arg.Title = "";
					if (suppen == 0) {
						arg.Message = "Do you really want to exit?";
					}
					else if (suppen == 2)
					{
						arg.Message = "Do you really want to restart?";
					}
					arg.Type = 1; //need confirm
					_stream_mng.put(arg, stream);
					ActionDialogShow act_dialogshow = new ActionDialogShow(arg, AppContext.CurrentContext);
					act_dialogshow.act();
				}
				
			}break;
			case ServiceCode.BrowserOpen:{
				((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
					public void run() {
						_stream_response.add(stream);
						if(stream.RequestData != null){
							StreamBuffer buf = new StreamBuffer(stream.RequestData);
							String url = buf.readSStrRevert();
							Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
							((Activity)AppContext.CurrentContext).startActivity(browserIntent);
						}
					}
				});
			}break;
			case ServiceCode.Rating:{
				((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
					public void run() {
							_stream_response.add(stream);
							Intent intent = new Intent(Intent.ACTION_VIEW);                     
							intent.setData(Uri.parse("market://details?id=" + AppContext.CurrentContext.getPackageName()));
							((Activity)AppContext.CurrentContext).startActivity(intent);
						
					}
				});
			}break;
			case ServiceCode.GooglePlayIsLogin:{
				try {
					boolean success = false;
					if(AppConfig.UseGooglePlay) {
						success = GooglePlayManager.IsServiceAvailable();
						stream.ErrorCode = success ? 0 : 1;
						if (stream.ErrorCode == 1 && GooglePlayManager.isUserCancelLogin()) {
							stream.ErrorCode = 2;
						}
					}
					else {
						stream.ErrorCode = 1;
					}
					_stream_response.add(stream);
				}catch (Exception ex) {

				}
			}break;
			case ServiceCode.GooglePlayUserInfo:{
				try {
					if(AppConfig.UseGooglePlay && GooglePlayManager.IsServiceAvailable()) {
						String userId = GooglePlayManager.getCurrentUserId();
						String userName = GooglePlayManager.getCurrentUserName();
						if (userId != null) {
							if (userName == null) {
								userName = "Unknown";
								Log.i("DEBUG", "User name null!!!");
							}
							int data_size = 2 + (2 * userId.length()) + 2 + (2 * userName.length());
							StreamBuffer buff = new StreamBuffer(data_size);
							buff.writeSStrRevert(userId);
							buff.writeSStrRevert(userName);
							Log.i("DEBUG", "Get google play user info response data len:" + buff.Data().length);
							stream.ResponseData = buff.Data();
							stream.ErrorCode = 0;
							_stream_response.add(stream);
						}
						else{
							Log.i("DEBUG", "User id null!!!");
							stream.ErrorCode = 1;
							_stream_response.add(stream);
						}
					}
					else{
						Log.i("DEBUG", "Google play is not connect!!!");
						stream.ErrorCode = 1;
						_stream_response.add(stream);
					}
				}catch (Exception ex) {
					Log.i("DEBUG", "Get google play user info fail with exception: " + ex.getMessage());
					stream.ErrorCode = 1;
					_stream_response.add(stream);
				}
			}break;
			case ServiceCode.GooglePlayGetFriends:{
				try {
					((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							ActionGooglePlayFriendsArg arg = new ActionGooglePlayFriendsArg(){
								public void onBegin(){
									Log.i("DEBUG","Action google play get frined begin");
								}
								public void onDone(){
									Log.i("DEBUG","Action google play get friend done");
									ServiceStream actstream = Game._stream_mng.get(this);
									if(actstream != null){
										actstream.ErrorCode = 0;

										int data_size = 4;
										for(int i = 0; i< this._friend_ids.size(); i++)
										{
											data_size += 2 + 2* this._friend_ids.get(i).length();
										}
										StreamBuffer buff = new StreamBuffer(data_size);

										buff.writeInt32Revert(this._friend_ids.size());
										for(int i = 0; i< this._friend_ids.size(); i++)
										{
											buff.writeSStrRevert(this._friend_ids.get(i));
										}

										actstream.ResponseData = buff.Data();


										_stream_response.add(actstream);
										Game._stream_mng.remove(this);
									}else{
										Log.i("DEBUG","GooglePlayGetFriends some thing wrong");
									}
								}
								public void onCancel(){
									Log.i("DEBUG","Action google play get friend cancel");
									ServiceStream actstream = Game._stream_mng.get(this);
									if(actstream != null){
										actstream.ErrorCode = 1;
										_stream_response.add(actstream);
										Game._stream_mng.remove(this);
									}
								}
							};

							_stream_mng.put(arg, stream);
							ActionGooglePlayFriends act = new ActionGooglePlayFriends(arg);
							act.act();
						}
					});
				}catch (Exception ex) {
					Log.i("DEBUG", "Get google play friend fail with exception: " + ex.getMessage());
					stream.ErrorCode = 1;
					_stream_response.add(stream);
				}
			}break;
			case ServiceCode.GooglePlayPostScore:{
				boolean success = false;
				try {
					if (AppConfig.UseGooglePlay && GooglePlayManager.IsServiceAvailable()) {
						if (stream.RequestData != null) {
							StreamBuffer buf = new StreamBuffer(stream.RequestData);
							String boar_id = buf.readSStrRevert();
							int score = buf.readInt32Revert();
							if (GooglePlayManager.submitScore(boar_id, score)) {
								success = true;
							}
						}
					}
					stream.ErrorCode = success ? 0 : 1;
					_stream_response.add(stream);
				}catch (Exception ex) {}
			}break;
			case ServiceCode.GooglePlayGetLeaderBoardRank:{
				try {
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							ActionGooglePlayGetLeaderboardRankArg arg = new ActionGooglePlayGetLeaderboardRankArg() {
								public void onBegin() {
									Log.i("DEBUG", "Action google play get leaderboard rank begin");
								}

								public void onDone() {
									Log.i("DEBUG", "Action google play get leaderboard rank done");
									ServiceStream actstream = Game._stream_mng.get(this);
									if (actstream != null) {
										actstream.ErrorCode = 0;

										int data_size = 4;
										StreamBuffer buff = new StreamBuffer(data_size);

										buff.writeInt32Revert(this.CurrentRank);
										actstream.ResponseData = buff.Data();
										_stream_response.add(actstream);
										Game._stream_mng.remove(this);
									} else {
										Log.i("DEBUG", "GooglePlayGetLeaderboardRank some thing wrong");
									}
								}

								public void onCancel() {
									Log.i("DEBUG", "Action google play get leaderboard rank cancel");
									ServiceStream actstream = Game._stream_mng.get(this);
									if (actstream != null) {
										actstream.ErrorCode = 1;
										_stream_response.add(actstream);
										Game._stream_mng.remove(this);
									}
								}
							};

							if(stream.RequestData != null){
								StreamBuffer buf = new StreamBuffer(stream.RequestData);
								arg.LeaderBoardId = buf.readSStrRevert();
								Log.i("DEBUG", "Action google play leaderboard rank stream request board id = " + arg.LeaderBoardId);
							}else{
								Log.i("DEBUG", "Action google play leaderboard rank stream request null");
							}
							_stream_mng.put(arg, stream);
							ActionGooglePlayGetLeaderboardRank act = new ActionGooglePlayGetLeaderboardRank(arg);
							act.act();
						}
					});
				}catch (Exception ex) {
					Log.i("DEBUG", "Get google play leaderboard rank fail with exception: " + ex.getMessage());
					stream.ErrorCode = 1;
					_stream_response.add(stream);
				}
			}break;
			case ServiceCode.GooglePlayLogin:{
				((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
					public void run() {
						Log.i("DEBUG","call service Login Google");

						ActionGooglePlayLoginArg arg = new ActionGooglePlayLoginArg(){
							public void onBegin(){
								Log.i("DEBUG","Action google play login begin");
							}
							public void onDone(){
								final ActionGooglePlayLoginArg gparg = this;
								((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
									public void run() {
										Log.i("DEBUG","Action Google login done");
										
										ServiceStream actstream = Game._stream_mng.get(gparg);
										if(actstream != null){
											actstream.ErrorCode = 0;
											_stream_response.add(actstream);
											Game._stream_mng.remove(gparg);
										}else{
											Log.i("DEBUG","login google play some thing wrong!!");
										}
									}
								});
							}
							public void onCancel() {
								Log.i("DEBUG", "Action google play login cancel");
								ServiceStream actstream = Game._stream_mng.get(this);
								if (actstream != null) {
									Game._stream_mng.remove(this);
									actstream.ErrorCode = 1;
									_stream_response.add(actstream);
								}

							}
						};
						_stream_mng.put(arg, stream);
						ActionGooglePlayLogin act_fblogin = new ActionGooglePlayLogin(arg);
						act_fblogin.act();
					}
				});
			}break;
			case ServiceCode.GooglePlayShareToGooglePlus:{
				try {
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							StreamBuffer buf = new StreamBuffer(stream.RequestData);
							String msg = buf.readSStrRevert();
							// Launch the Google+ share dialog with attribution to your app.
							Intent shareIntent = new PlusShare.Builder(((Activity) AppContext.CurrentContext))
									.setType("text/plain")
									.setText(msg + "\nhttps://play.google.com/store/apps/details?id=" + GetDataPath() + "&hl=en")
									//.setContentUrl(Uri.parse("https://www.pine-entertainment.com"))
									.getIntent();

							((Activity) AppContext.CurrentContext).startActivityForResult(shareIntent, 0);
							_stream_response.add(stream);
						}
					});
				}catch (Exception ex) {}
			}break;
			case ServiceCode.GooglePlayShareScreenToGooglePlus:{
				try {
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							ActionGooglePlayShareScreenToGooglePlusArg arg = new ActionGooglePlayShareScreenToGooglePlusArg(){
								public void onBegin(){
									Log.i("DEBUG","ActionGooglePlayShareScreenToGooglePlus begin");
								}
								public void onDone(){
									ServiceStream actstream = Game._stream_mng.get(this);
									if(actstream != null){
										Log.i("DEBUG","ActionGooglePlayShareScreenToGooglePlus done");
										actstream.ErrorCode = 0;
										_stream_response.add(actstream);
										Game._stream_mng.remove(this);
									}else{
										Log.i("DEBUG","ActionGooglePlayShareScreenToGooglePlus some thing wrong");
									}
								}
								public void onCancel(){
									Log.i("DEBUG","ActionGooglePlayShareScreenToGooglePlus cancel");
									ServiceStream actstream = Game._stream_mng.get(this);
									if(actstream != null){
										actstream.ErrorCode = 1;
										_stream_response.add(actstream);
										Game._stream_mng.remove(this);
									}
								}
							};
							if(stream.RequestData != null){
								StreamBuffer buf = new StreamBuffer(stream.RequestData);
								String msg = buf.readSStrRevert() + "\nhttps://play.google.com/store/apps/details?id=" + GetDataPath() + "&hl=en";
								arg.Name = "";
								arg.Caption = msg;
								arg.Description = "";
								arg.Link = "http://www.pine-entertainment.com";
								Log.i("DEBUG", "ActionGooglePlayShareScreenToGooglePlus msg:" + msg);
							}else{
								Log.i("DEBUG", "stream request null");
							}
							_stream_mng.put(arg, stream);
							GooglePlayManager.shareScreenShotToGooglePlus(arg);
						}
					});
				}catch (Exception ex) {}
			}break;
			case ServiceCode.GooglePlayShowLeaderBoard:{
				boolean success = false;
				try {
					if (AppConfig.UseGooglePlay && GooglePlayManager.IsServiceAvailable()) {
						if (stream.RequestData != null) {
							StreamBuffer buf = new StreamBuffer(stream.RequestData);
							String boar_id = buf.readSStrRevert();
							GooglePlayManager.showLeaderBoard(boar_id);
							success = true;
						}
					}
					stream.ErrorCode = success ? 0 : 1;
					_stream_response.add(stream);
				}catch (Exception ex) {}
			}break;
			case ServiceCode.GooglePlayShowAchievements:{
				boolean success = false;
				try {
					if (AppConfig.UseGooglePlay && GooglePlayManager.IsServiceAvailable()) {
						GooglePlayManager.showAchievements();
						success = true;
					}
					stream.ErrorCode = success ? 0 : 1;
					_stream_response.add(stream);
				}catch (Exception ex) {}
			}break;
			case ServiceCode.GooglePlayUnlockAchievement:{
				boolean success = false;
				try {
					if (AppConfig.UseGooglePlay && GooglePlayManager.IsServiceAvailable()) {
						if (stream.RequestData != null) {
							StreamBuffer buf = new StreamBuffer(stream.RequestData);
							String achi_id = buf.readSStrRevert();
							GooglePlayManager.unlockAchievement(achi_id);
							success = true;
						}
					}
					stream.ErrorCode = success ? 0 : 1;
					Log.i("DEBUG", "Unlock achievement success return code: " + stream.ErrorCode);
					_stream_response.add(stream);
				}catch (Exception ex) {
					Log.i("DEBUG", "Unlock achievement fail with exception: " + ex.getMessage());
				}
			}break;
			case ServiceCode.GooglePlayUpdateCloudSave:{
				int result = 0;
				try {
					if (stream.RequestData != null) {
						StreamBuffer buf = new StreamBuffer(stream.RequestData);
						String localSaveName = buf.readSStrRevert();
						result = GooglePlayManager.updateCloudSave(localSaveName);
					}
					stream.ErrorCode = result;
					Log.i("DEBUG", "Update cloud save return code: " + stream.ErrorCode);
					_stream_response.add(stream);
				}catch (Exception ex) {
					Log.i("DEBUG", "Update cloud save fail with exception: " + ex.getMessage());
				}
			}break;
			case ServiceCode.GooglePlaySendGift: {
				try {
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							ActionGooglePlaySendGiftArg arg = new ActionGooglePlaySendGiftArg(){
								public void onBegin(){
									Log.i("DEBUG","ActionGooglePlaySendGift begin");
								}
								public void onDone(){
									ServiceStream actstream = Game._stream_mng.get(this);
									if(actstream != null){
										Log.i("DEBUG","ActionGooglePlaySendGift done");
										StreamBuffer buff = new StreamBuffer(4);
										buff.writeInt32Revert(this._returnGift);
										actstream.ErrorCode = 0;
										actstream.ResponseData = buff.Data();
										_stream_response.add(actstream);
										Game._stream_mng.remove(this);
									}else{
										Log.i("DEBUG","ActionGooglePlaySendGift some thing wrong");
									}
								}
								public void onCancel(){
									Log.i("DEBUG","ActionGooglePlaySendGift cancel");
									ServiceStream actstream = Game._stream_mng.get(this);
									if(actstream != null){
										actstream.ErrorCode = 1;
										_stream_response.add(actstream);
										Game._stream_mng.remove(this);
									}
								}
							};
							if(stream.RequestData != null){
								StreamBuffer buf = new StreamBuffer(stream.RequestData);
								arg._type = buf.readInt32Revert();
								arg._message = buf.readSStrRevert();
								arg._icon = null;
								Log.i("DEBUG", "ActionGooglePlaySendGift msg:" + arg._message);
							}else{
								Log.i("DEBUG", "stream request null");
							}
							_stream_mng.put(arg, stream);
							GooglePlayManager.showSendIntent(arg);
						}
					});
				}catch (Exception ex) {}
			}break;
			case ServiceCode.FirebaseAnalyticsLogEvent: {
				if(AppConfig.UseFirebaseAnalystic)
				{
					try {
						((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
							public void run() {
								if(stream.RequestData != null){
									StreamBuffer buf = new StreamBuffer(stream.RequestData);
									String title = buf.readSStrRevert();
									String decs = buf.readSStrRevert();
									String type = buf.readSStrRevert();
									String value = buf.readSStrRevert();

									Log.i("DEBUG", "FirebaseAnalyticsLogEvent title:" + title + ", decs = " + decs + ", type = " + type + ", value = " + value);
									stream.ErrorCode = FirebaseManager.FirebaseAnalyticsLogEvent(title, decs, type, value) ? 0: 1;
									_stream_response.add(stream);
								}else {
									Log.i("DEBUG", "FirebaseAnalyticsLogEvent stream request null");
								}
							}
						});
					}catch (Exception ex) {
						Log.i("DEBUG", "FirebaseAnalyticsLogEvent fail with exception: " + ex.getMessage());
					}
				}
			}break;
			case ServiceCode.FirebaseAnalyticsSetUserProperty: {
				if(AppConfig.UseFirebaseAnalystic)
				{
					try {
						((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
							public void run() {
								if(stream.RequestData != null){
									StreamBuffer buf = new StreamBuffer(stream.RequestData);
									String id = buf.readSStrRevert();
									String value = buf.readSStrRevert();

									Log.i("DEBUG", "FirebaseAnalyticsSetProperty title:" + id + ", decs = " +  ", value = " + value);
									stream.ErrorCode = FirebaseManager.FirebaseAnalyticsSetUserProperty(id, value) ? 0: 1;
									_stream_response.add(stream);
								}else {
									Log.i("DEBUG", "FirebaseAnalyticsSetUserProperty stream request null");
								}
							}
						});
					}catch (Exception ex) {
						Log.i("DEBUG", "FirebaseAnalyticsSetUserProperty fail with exception: " + ex.getMessage());
					}
				}
			}break;
			case ServiceCode.SetLocalNotification: {
				try {
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							if(stream.RequestData != null){
								StreamBuffer buf = new StreamBuffer(stream.RequestData);
								int second = buf.readInt32Revert();
								String message = buf.readSStrRevert();

								Log.i("DEBUG", "SetLocalNotification message:" + message);
								ApplicationInfo applicationInfo = AppContext.CurrentContext.getApplicationInfo();
								int stringId = applicationInfo.labelRes;
								String appName = stringId == 0 ? applicationInfo.nonLocalizedLabel.toString() : AppContext.CurrentContext.getString(stringId);
								stream.ErrorCode = TopeboxNotificationManager.setNotification(appName, message, message, second) ? 0: 1;
								_stream_response.add(stream);
							}else {
								Log.i("DEBUG", "SetLocalNotification stream request null");
							}
						}
					});
				}catch (Exception ex) {
					Log.i("DEBUG", "SetLocalNotification fail with exception: " + ex.getMessage());
				}
			}break;
			case ServiceCode.ChartboostInterstitialShow: {
				/*try {
					if (AppConfig.UseChartBoost) {
						((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
							public void run() {
								//if () {
								Log.i("DEBUG","ChartBoostRequestCheckInters");
								Chartboost.showInterstitial(CBLocation.LOCATION_MAIN_MENU);
								stream.ErrorCode = 0;
								_stream_response.add(stream);
								//} else {
								//	stream.ErrorCode = 1;
								//	_stream_response.add(stream);
								//}
							}
						});
					}
				} catch (Exception ex) {}*/
			} break;
			case ServiceCode.ChartboostRewardVideoPrepare: {
				/*try {
					if (AppConfig.UseChartBoost) {
						((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
							public void run() {
								if (Chartboost.hasRewardedVideo(CBLocation.LOCATION_MAIN_MENU)) {
									stream.ErrorCode = 0;
									_stream_response.add(stream);
								} else {
									Chartboost.cacheRewardedVideo(CBLocation.LOCATION_MAIN_MENU);
									stream.ErrorCode = 1;
									_stream_response.add(stream);
								}
							}
						});
					}
				}catch (Exception ex) {}*/
			} break;
			case ServiceCode.ChartboostRewardVideoShow: {
				try {
					if (AppConfig.UseChartBoost) {
						((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
							public void run() {
								/*try {
									Chartboost.showRewardedVideo(CBLocation.LOCATION_MAIN_MENU);
								} catch (Exception ex) {

								}*/
								//Chartboost.showRewardedVideo(CBLocation.LOCATION_MAIN_MENU);
								//if () {
								Log.i("DEBUG","ChartBoostRequestCheckInters");
								stream.ErrorCode = 0;
								_stream_response.add(stream);
								//} else {
								//	stream.ErrorCode = 1;
								//	_stream_response.add(stream);
								//}
							}
						});
					}
				} catch (Exception ex) {}
			} break;
			// unity 3d ads
			case ServiceCode.UnityAdsPrepareAds: {
				/*try {
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {

							if (UnityAds.canShow()) {
								stream.ErrorCode = 0;
								_stream_response.add(stream);
							} else {
								stream.ErrorCode = 1;
								_stream_response.add(stream);
							}

						}
					});
				}catch (Exception ex) {}*/
			} break;
			case ServiceCode.UnityAdsPrepareSkippableVideo: {
				/*try {
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {

							if (UnityAds.canShow()) {
								stream.ErrorCode = 0;
								_stream_response.add(stream);
							} else {
								stream.ErrorCode = 1;
								_stream_response.add(stream);
							}

						}
					});
				}catch (Exception ex) {}*/
			} break;
			case ServiceCode.UnityAdsShowAds: {
				/*try {
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							if (UnityAds.canShow()){
								if (UnityAds.setZone("mainMenu")) {
									UnityAds.show();
								}
								else{
									Log.i("DEBUG", "Unity Ads Setzone fail.");
								}
							}
							else{
								Log.i("DEBUG", "Unity Ads is not ready");
							}
						}
					});
				} catch (Exception ex) {
					Log.i("DEBUG", "SHOW ADS error: %s" + ex.getMessage());
				}*/
			} break;
			case ServiceCode.UnityAdsShowVideo: {
				/*try {
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							ActionUnityVideoAdsShowArg arg = new ActionUnityVideoAdsShowArg(){
								public void onBegin(){
									Log.i("DEBUG","Action Unity video ads show begin");
								}
								public void onDone(){
									final ActionUnityVideoAdsShowArg gparg = this;
									((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
										public void run() {
											Log.i("DEBUG","Action Unity video ads show done");

											ServiceStream actstream = Game._stream_mng.get(gparg);
											if(actstream != null){
												actstream.ErrorCode = 0;
												_stream_response.add(actstream);
												Game._stream_mng.remove(gparg);
											}else{
												Log.i("DEBUG","Action Unity video ads show some thing wrong!!");
											}
										}
									});
								}
								public void onCancel() {
									Log.i("DEBUG", "Action Unity video ads show cancel");
									ServiceStream actstream = Game._stream_mng.get(this);
									if (actstream != null) {
										Game._stream_mng.remove(this);
										actstream.ErrorCode = 1;
										_stream_response.add(actstream);
									}
								}
							};
							_stream_mng.put(arg, stream);
							UnityAdsManager.showVideoAds(arg);
						}
					});
				} catch (Exception ex) {
				Log.i("DEBUG", "SHOW UNITY VIDEO ADS error: %s" + ex.getMessage());
			}*/
			} break;

			case ServiceCode.UnityAdsShowSkippableVideo: {
				/*try {
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							ActionUnityVideoAdsAllowSkipShowArg arg = new ActionUnityVideoAdsAllowSkipShowArg(){
								public void onBegin(){
									Log.i("DEBUG","Action Unity video allow skip ads show begin");
								}
								public void onDone(){
									final ActionUnityVideoAdsAllowSkipShowArg gparg = this;
									((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
										public void run() {
											Log.i("DEBUG","Action Unity video allow skip ads show done");

											ServiceStream actstream = Game._stream_mng.get(gparg);
											if(actstream != null){
												actstream.ErrorCode = 0;
												_stream_response.add(actstream);
												Game._stream_mng.remove(gparg);
											}else{
												Log.i("DEBUG","Action Unity video allow skip ads show some thing wrong!!");
											}
										}
									});
								}
								public void onCancel() {
									Log.i("DEBUG", "Action Unity video allow skip ads show cancel");
									ServiceStream actstream = Game._stream_mng.get(this);
									if (actstream != null) {
										Game._stream_mng.remove(this);
										actstream.ErrorCode = 1;
										_stream_response.add(actstream);
									}
								}
							};
							_stream_mng.put(arg, stream);
							UnityAdsManager.showSkippableVideoAds(arg);
						}
					});
				} catch (Exception ex) {
					Log.i("DEBUG", "SHOW UNITY VIDEO ADS error: %s" + ex.getMessage());
				}*/
			} break;

			case ServiceCode.VungleAdsPrepare: {
				/*if (AppConfig.UseVungleAds) {
					try {
						((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
							public void run() {
								AppContext.vunglePub.loadAd(AppContext.placementIdForLevel);
								if (AppContext.vunglePub.isAdPlayable(AppContext.placementIdForLevel)) {
									stream.ErrorCode = 0;
									Log.i("DEBUG", "Prepare vungle ads isAdPlayable = true");
								}
								else {
									stream.ErrorCode = 1;
									Log.i("DEBUG", "Prepare vungle ads isAdPlayable = false");
								}
								_stream_response.add(stream);
							}
						});
					} catch (Exception ex) {
						stream.ErrorCode = 1;
						_stream_response.add(stream);
						Log.i("DEBUG", "Prepare vungle ads error: %s" + ex.getMessage());
					}
				}
				else
				{
					stream.ErrorCode = 1;
					_stream_response.add(stream);
					Log.i("DEBUG", "Prepare vungle ads false because not use");
				}*/
			} break;

			case ServiceCode.VungleAdsShow: {
				/*try {
					((Activity) AppContext.CurrentContext).runOnUiThread(new Runnable() {
						public void run() {
							ActionVungleVideoAdsShowArg arg = new ActionVungleVideoAdsShowArg(){
								public void onBegin(){
									Log.i("DEBUG","Action Vungle video ads show begin");
								}
								public void onDone(){
									final ActionVungleVideoAdsShowArg gparg = this;
									((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
										public void run() {
											Log.i("DEBUG","Action Vungle video ads show done");

											ServiceStream actstream = Game._stream_mng.get(gparg);
											if(actstream != null){
												actstream.ErrorCode = 0;
												_stream_response.add(actstream);
												Game._stream_mng.remove(gparg);
											}else{
												Log.i("DEBUG","Action Vungle video ads show some thing wrong!!");
											}
										}
									});
								}
								public void onCancel() {
									Log.i("DEBUG", "Action Vungle video ads show cancel");
									ServiceStream actstream = Game._stream_mng.get(this);
									if (actstream != null) {
										Game._stream_mng.remove(this);
										actstream.ErrorCode = 1;
										_stream_response.add(actstream);
									}
								}
							};
							_stream_mng.put(arg, stream);
							VungleAdsManager.showVideoAds(arg);
						}
					});
				} catch (Exception ex) {
					Log.i("DEBUG", "SHOW VUNGLE VIDEO ADS error: %s" + ex.getMessage());
				}*/
			}  break;
			
			// IAP
			case ServiceCode.PurchaseGetList:{
						ActionSynPurchaseGetListArg arg = new ActionSynPurchaseGetListArg(){
							public void onBegin(){
								Log.i("DEBUG","ServiceCode.PurchaseGetList begin");
								this._listSkus.clear();
								StreamBuffer buffs = new StreamBuffer(stream.RequestData);
								int num_skus = buffs.readInt32Revert();
								for (int i = 0; i < num_skus; i++)
								{
									String iap_id = buffs.readSStrRevert();
									Log.i("DEBUG", "appid:" + iap_id);
									this._listSkus.add(iap_id);//.replaceAll("\\.", "_").toLowerCase());
								}
								
								for (String str: this._listSkus)
								{
									Log.i("DEBUG", "iap: " + str);
								}
							}
			
							public void onDone(){
								Log.i("DEBUG","ServiceCode.PurchaseGetList done");
								
								int data_size = 4;
								
								for (int i = 0; i < this._listResponsedSkus.size(); i++)
								{
									data_size += 4 + (2 + 2*this._listResponsedSkus.get(i)._price.length());
								}
								
								Log.i("DEBUG","data_size: " + data_size);
								
								StreamBuffer buff = new StreamBuffer(data_size);
								
								buff.writeInt32Revert(this._listResponsedSkus.size());
								
								for (SkuInfo sku: this._listResponsedSkus)
								{
									buff.writeSStrRevert(sku._price);
									buff.writeInt32Revert(sku._isOwned?1:0);
								}
								this._listResponsedSkus.clear();
									
								ServiceStream actstream = Game._stream_mng.get(this);
								if(actstream != null){
									actstream.ErrorCode = 0;
									actstream.ResponseData = buff.Data();
									Log.i("DEBUG", "response data:" + actstream.ResponseData.length);
									_stream_response.add(actstream);
									Game._stream_mng.remove(this);
								}else{
									Log.i("DEBUG","some thing wrong");
								}
							}
							public void onCancel(){
								Log.i("DEBUG","ServiceCode.PurchaseGetList cancel");
								ServiceStream actstream = Game._stream_mng.get(this);
								if(actstream != null){
									actstream.ErrorCode = -1;
									_stream_response.add(actstream);
									Game._stream_mng.remove(this);
								}else{
									Log.i("DEBUG","some thing wrong");
								}
							}
						};
					
					_stream_mng.put(arg, stream);
					ActionSynPurchaseGetList act_syn_create = new ActionSynPurchaseGetList(arg);
					act_syn_create.act();
					
				
			} break;
			case ServiceCode.Purchase:{
				
				ActionSynPurchaseArg arg = new ActionSynPurchaseArg(){
					public void onBegin(){
						Log.i("DEBUG","ServiceCode.Purchase begin");
					}
	
					public void onDone(){
						Log.i("DEBUG","Purchase ondone");
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							int size = 2 + this._sku.length() * 2;
							StreamBuffer buff = new StreamBuffer(size);
							buff.writeSStrRevert(this._sku);
							Log.i("DEBUG", "purchase done:" + this._sku);
							actstream.ResponseData = buff.Data();

							if (this.Success) {
								actstream.ErrorCode = 0;
							}
							else if(this._isOwned){
								actstream.ErrorCode = 2;
							}
							else{
								actstream.ErrorCode = 1;
							}

							_stream_response.add(actstream);
							Game._stream_mng.remove(this);
						}else{
							Log.i("DEBUG","some thing wrong");
						}
					}
					public void onCancel(){
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							
							int size = 1 + this._sku.length() * 2;
							StreamBuffer buff = new StreamBuffer(size);
							buff.writeSStrRevert(this._sku);
							
							actstream.ErrorCode = 1;
							_stream_response.add(actstream);
							Game._stream_mng.remove(this);
						}else{
							Log.i("DEBUG","some thing wrong");
						}
					}
				};
					
				StreamBuffer buffs = new StreamBuffer(stream.RequestData);
				arg._sku = buffs.readSStrRevert();
				arg._type = buffs.readInt32Revert();
				Log.i("DEBUG", "purchase: " + arg._sku + "; Type = " + arg._type);
				_stream_mng.put(arg, stream);
				
				ActionSynPurchase act = new ActionSynPurchase(arg);
				act.act();
			} break;

			case ServiceCode.PurchaseRestore:{

				ActionSynPurchaseArg arg = new ActionSynPurchaseArg(){
					public void onBegin(){
						Log.i("DEBUG","ServiceCode.PurchaseRestore begin");
					}

					public void onDone(){
						Log.i("DEBUG","Restore purchase ondone");
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){
							int size = 2 + this._sku.length() * 2;
							StreamBuffer buff = new StreamBuffer(size);
							buff.writeSStrRevert(this._sku);
							Log.i("DEBUG", "restore done:" + this._sku);
							actstream.ResponseData = buff.Data();

							actstream.ErrorCode = this._isOwned? 0 : 1;

							_stream_response.add(actstream);
							Game._stream_mng.remove(this);
						}else{
							Log.i("DEBUG","some thing wrong");
						}
					}
					public void onCancel(){
						ServiceStream actstream = Game._stream_mng.get(this);
						if(actstream != null){

							int size = 1 + this._sku.length() * 2;
							StreamBuffer buff = new StreamBuffer(size);
							buff.writeSStrRevert(this._sku);

							actstream.ErrorCode = 1;
							_stream_response.add(actstream);
							Game._stream_mng.remove(this);
						}else{
							Log.i("DEBUG","some thing wrong");
						}
					}
				};

				StreamBuffer buffs = new StreamBuffer(stream.RequestData);
				arg._sku = buffs.readSStrRevert();
				arg._type = buffs.readInt32Revert();
				Log.i("DEBUG", "restore: " + arg._sku + "; Type = " + arg._type);
				_stream_mng.put(arg, stream);

				ActionSynPurchase act = new ActionSynPurchase(arg);
				act.act();
			} break;

			//Game spark back-end services
			case ServiceCode.GameSparkLogin:{
				((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
					public void run() {
						Log.i("DEBUG","call service Login Game spark");

						ActionGameSparkLoginArg arg = new ActionGameSparkLoginArg(){
							public void onBegin(){
								Log.i("DEBUG","Action game spark login begin");
							}
							public void onDone(){
								final ActionGameSparkLoginArg gparg = this;
								((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
									public void run() {
										Log.i("DEBUG","Action game spark login done");

										ServiceStream actstream = Game._stream_mng.get(gparg);
										if(actstream != null){
											actstream.ErrorCode = 0;
											_stream_response.add(actstream);
											Game._stream_mng.remove(gparg);
										}else{
											Log.i("DEBUG","login game spark some thing wrong!!");
										}
									}
								});
							}
							public void onCancel() {
								Log.i("DEBUG", "Action game spark login cancel");
								ServiceStream actstream = Game._stream_mng.get(this);
								if (actstream != null) {
									Game._stream_mng.remove(this);
									actstream.ErrorCode = 1;
									_stream_response.add(actstream);
								}
							}
						};
						_stream_mng.put(arg, stream);
						ActionGameSparkLogin act_gsLogin = new ActionGameSparkLogin(arg);
						act_gsLogin.act();
					}
				});
			}break;
			case ServiceCode.GameSparkUploadSave:{
				((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
					public void run() {
						Log.i("DEBUG","call service Game spark upload save");

						ActionGameSparkUploadSaveArg arg = new ActionGameSparkUploadSaveArg(){
							public void onBegin(){
								Log.i("DEBUG","Action game spark upload save");
							}
							public void onDone(){
								final ActionGameSparkUploadSaveArg gparg = this;
								((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
									public void run() {
										Log.i("DEBUG","Action game spark upload save done");

										ServiceStream actstream = Game._stream_mng.get(gparg);
										if(actstream != null){
											actstream.ErrorCode = 0;
											_stream_response.add(actstream);
											Game._stream_mng.remove(gparg);
										}else{
											Log.i("DEBUG","upload save game spark some thing wrong!!");
										}
									}
								});
							}
							public void onCancel() {
								Log.i("DEBUG", "Action game spark upload save cancel");
								ServiceStream actstream = Game._stream_mng.get(this);
								if (actstream != null) {
									Game._stream_mng.remove(this);
									actstream.ErrorCode = 1;
									_stream_response.add(actstream);
								}
							}
						};
						_stream_mng.put(arg, stream);
						ActionGameSparkUploadSave act_gsUpload = new ActionGameSparkUploadSave(arg);
						act_gsUpload.act();
					}
				});
			}break;
			default:
			if(GameService != null){
				GameService.process(stream);
			}else{
				
			}
			break;
		}//switch
		
		//}//run function
		//	});
		//}
	}
	
	static public void reportService(ServiceStream stream)
	{
		if(stream != null)
		{
			OsFunctions.ReportServiceStream(stream.getBinary());
		}
	}
	
	static public void ChangeContext(Context context)
	{
		_init++;
		/*if(_init > 3){
			OsFunctions._is_init = false;
			OsFunctions.Init();
		}*/
	}
	static int _init = 0;
	
	static public void Init(int device_screen_w, int device_screen_h)
	{
		
		Log.i("DEBUG","game Init");
		
		if(!IsInited)
		{
			_init++;
			OsFunctions.Init();
			nativeInit(device_screen_w, device_screen_h);
			
			/*#[001] if(Facebook == null && AppContext.CurrentContext != null){
				Facebook = new FacebookManager(AppContext.CurrentContext, AppContext.CurrentSavedInstanceState);
			}*/
			
			//load game setting
			GameSetting setting = new GameSetting(AppContext.CurrentContext);
			String fbid = setting.loadSettingString("fb_id");
			String fbname = setting.loadSettingString("fb_name");
			String fbtoken = setting.loadSettingString("fb_token");
			IsInited = true;
		}else{
			Resume();
		}
	}
	
	static public void onSizeChange(int device_screen_w, int device_screen_h)
	{
		nativeSizeChange(device_screen_w, device_screen_h);
	}
	
	static public void Update()
	{
		try
		{
			//Log.i("DEBUG","init:" + _init);
			if(_stream_response.size() > 0 && _init < 4)
			{
				for(int s = 0; s< _stream_response.size(); s++)
				{
					ServiceStream stream = _stream_response.get(s);
					reportService(stream);
				}
				_stream_response.clear();
			}
			nativeUpdate();
			if (AppConfig.UseAdmobRewardedVideo)
			{
				((Activity)AppContext.CurrentContext).runOnUiThread(new Runnable() {
					public void run() {
						AdmobManager.GameUpdate();
					}
				});
			}
		}
		catch(Exception ex)
		{
			Log.i("DEBUG",ex.getMessage());
		}
	}

	static public void restart()
	{
		Activity currentActivity = (Activity)AppContext.CurrentContext;
		Intent mStartActivity = new Intent(currentActivity, AppContext.CurrentContext.getClass());
		int mPendingIntentId = android.os.Process.myPid();
		PendingIntent mPendingIntent = PendingIntent.getActivity(currentActivity, mPendingIntentId, mStartActivity,
				PendingIntent.FLAG_CANCEL_CURRENT);
		AlarmManager mgr = (AlarmManager) currentActivity.getSystemService(Context.ALARM_SERVICE);
		mgr.set(AlarmManager.RTC, System.currentTimeMillis(), mPendingIntent);
		System.exit(0);
	}
	
	static public void Render()
	{
		nativeRender();
	}
	
	static public void TouchBegin(int id, int x, int y)
	{
		nativeTouchBegin(id, x, y);
	}
	
	static public void TouchDrag(int id, int x, int y)
	{
		nativeTouchDrag(id, x, y);
	}
	
	static public void TouchEnd(int id, int x, int y)
	{
		nativeTouchEnd(id, x, y);
	}
	
	static public void Suppend()
	{
		nativePause();
	}
	
	static public void Resume()
	{
		nativeResume();
	}
	
	static public boolean onBackKeyPress()
	{
		return nativeBackKeyPress();
	}
	
	static public void ChangeOrientation(int ori)
	{
		nativeChangeOrientation(ori);
	}
	
	public static native boolean nativeBackKeyPress();
	public static native void nativeInit(int device_screen_w, int device_screen_h);
	public static native void nativeSizeChange(int device_screen_w, int device_screen_h);
	public static native void nativePause();
	public static native void nativeResume();
	public static native void nativeUpdate();
	public static native void nativeRender();
	public static native void nativeTouchBegin(int id, int x, int y);
	public static native void nativeTouchDrag(int id, int x, int y);
	public static native void nativeTouchEnd(int id, int x, int y);
	public static native void nativeChangeOrientation(int ori);
}