using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PhoneDirect3DXamlAppComponent;
using Pine.Actions;
using System.IO;
using System.Windows.Media.Imaging;

using Pine.Tools;
using ToolStackPNGWriterLib;
using System.Net;
using Microsoft.Phone.Tasks;
using Microsoft.Phone.Controls;
using System.Windows;
#if DEBUG
using MockIAPLib;
using Store = MockIAPLib;
#else
using Windows.ApplicationModel.Store;
#endif
using FacebookUtils;
using Pine.TwitterUtils;
using Microsoft.Phone.Net.NetworkInformation;
using Hammock;
using Pine.Dialogs;
using Microsoft.Phone.Scheduler;
using System.Net.Http;
using System.Security.Cryptography;
using Windows.ApplicationModel;
using System.Reflection;
using System.Xml;
using GoogleAds;
using System.Diagnostics;
using System.Threading;
using System.IO.IsolatedStorage;

namespace Pine
{
    partial class MainPage
    {
        #region SYSTEM
        void init()
        {
            ApplicationInfomation.AppId = CurrentApp.AppId.ToString();
            //create odata service
            //m_odata_service = new ODataService<OdataArmy.YouArmyContext>("http://odata.pine-entertainment.com/api/youarmy/");
            //m_odata_pocket_service = new ODataService<OdataArmyMain.PocketArmyContext>("http://odata.ya.pine-entertainment.com/api/pocketarmy/");
            //init facebook and syn infomation
            FacebookClient.Instance.OnUserInfomationUpdate += delegate_fb_update_infomation;
            PhoneDirect3DXamlAppComponent.PlayerInfomation.LoginedFacebook = false;

            //Check Facebook Account infomation
            if (FacebookClient.Instance.UserId != null && FacebookClient.Instance.UserId.Length > 0)
            {
                PhoneDirect3DXamlAppComponent.PlayerInfomation.FacebookId = FacebookClient.Instance.UserId;
                UInt64 id = UInt64.Parse(FacebookClient.Instance.UserId);
                id = 3000000000000000000 + id;
                PhoneDirect3DXamlAppComponent.PlayerInfomation.UserId = id.ToString();
                if (FacebookClient.Instance.Token != null && FacebookClient.Instance.Token.Length > 0)
                {
                    PhoneDirect3DXamlAppComponent.PlayerInfomation.LoginedFacebook = true;
                }
            }
            //case have no user id
            else
            {
                PhoneDirect3DXamlAppComponent.PlayerInfomation.FacebookId = "";
                PhoneDirect3DXamlAppComponent.PlayerInfomation.UserId = "";
            }
#if DEBUG
            ///PhoneDirect3DXamlAppComponent.PlayerInfomation.FacebookId = "100003159828022";
            ///PhoneDirect3DXamlAppComponent.PlayerInfomation.UserId = "3000100003159828022";
            ///PhoneDirect3DXamlAppComponent.PlayerInfomation.LoginedFacebook = true;
#endif
            if (FacebookClient.Instance.UserName != null & FacebookClient.Instance.UserName.Length > 0)
            {
                PhoneDirect3DXamlAppComponent.PlayerInfomation.FacebookUserName = FacebookClient.Instance.UserName;
            }
            else
            {
                PhoneDirect3DXamlAppComponent.PlayerInfomation.FacebookUserName = "";
            }

            if(FacebookClient.Instance.AvatarUrl != null && FacebookClient.Instance.AvatarUrl.Length > 0)
            {
                PhoneDirect3DXamlAppComponent.PlayerInfomation.FacebookAvatarUrl = FacebookClient.Instance.AvatarUrl;
            }
            else
            {
                PhoneDirect3DXamlAppComponent.PlayerInfomation.FacebookAvatarUrl = "";
            }
            //Check Twitter Account Infomation
            if (TwitterClient.isAlreadyLoggedIn())
            {
                PhoneDirect3DXamlAppComponent.PlayerInfomation.TwitterId = TwitterClient.UserID;
                PhoneDirect3DXamlAppComponent.PlayerInfomation.TwitterUserName = TwitterClient.UserScreenName;
            }
            else
            {
                PhoneDirect3DXamlAppComponent.PlayerInfomation.TwitterId = "";
                PhoneDirect3DXamlAppComponent.PlayerInfomation.TwitterUserName = "";
            }

            //Init Environment
            DeviceNetworkInformation.NetworkAvailabilityChanged += new EventHandler<NetworkNotificationEventArgs>(handler_network_ChangeDetected);
            DeviceInfomation.IsNetworkAvailable = Microsoft.Phone.Net.NetworkInformation.DeviceNetworkInformation.IsNetworkAvailable |
                Microsoft.Phone.Net.NetworkInformation.DeviceNetworkInformation.IsCellularDataEnabled |
                Microsoft.Phone.Net.NetworkInformation.DeviceNetworkInformation.IsWiFiEnabled;
            foreach (Uri uri in AppObject.GameInfo.UriRequire)
            {
                updateNetworkStatus(uri);
            }
            DeviceInfomation.RegionFormatShort = System.Globalization.RegionInfo.CurrentRegion.TwoLetterISORegionName.ToUpper();
            DeviceInfomation.RegionFormat = System.Globalization.RegionInfo.CurrentRegion.EnglishName;

            DeviceInfomation.IsSafe = (Microsoft.Devices.Environment.DeviceType != Microsoft.Devices.DeviceType.Emulator);
            AppObject.RootLayout = this.LayoutRoot;
            
            GetUserAgent((agent) => {
                var xmlReaderSettings = new XmlReaderSettings
                {
                    XmlResolver = new XmlXapResolver()
                };
                string name = "";
                using (var xmlReader = XmlReader.Create("WMAppManifest.xml", xmlReaderSettings))
                {
                    xmlReader.ReadToDescendant("App");

                    name = xmlReader.GetAttribute("Title");

                }

                var nameHelper = new AssemblyName(Assembly.GetExecutingAssembly().FullName);

                var version = nameHelper.Version;
                var full = nameHelper.FullName;
                if (name.Length == 0)
                {
                    name = nameHelper.Name;
                }
                string buildVersion = version.Major.ToString() + "." + version.Minor.ToString() + "." + version.Revision.ToString();
                //string buildName = Package.Current.Id.Name;
                AppObject.UserAgent = name+ "/" + buildVersion + " " + agent; 
            });
            
        }
        
        #endregion
        #region NETWORK CHECK AND UPDATE
        async void updateNetworkStatus(Uri uri)
        {
            try
            {
                HttpClient client = new HttpClient();
                client.Timeout = new TimeSpan(0, 0, 20);
                HttpResponseMessage res = await client.GetAsync(uri);
                if (!res.IsSuccessStatusCode)
                {
                    DeviceInfomation.IsNetworkAvailable = false;
                }
                else
                {
                }
            }
            catch
            {
                //DeviceInfomation.IsNetworkAvailable = false;
            }
        }
        void handler_network_ChangeDetected(object sender, NetworkNotificationEventArgs e)
        {
            DeviceInfomation.IsNetworkAvailable = Microsoft.Phone.Net.NetworkInformation.DeviceNetworkInformation.IsNetworkAvailable |
                Microsoft.Phone.Net.NetworkInformation.DeviceNetworkInformation.IsCellularDataEnabled |
                Microsoft.Phone.Net.NetworkInformation.DeviceNetworkInformation.IsWiFiEnabled;
            foreach (Uri uri in AppObject.GameInfo.UriRequire)
            {
                updateNetworkStatus(uri);
            }
            switch (e.NotificationType)
            {
                case NetworkNotificationType.InterfaceConnected:
                    break;
                case NetworkNotificationType.InterfaceDisconnected:
                    break;
                case NetworkNotificationType.CharacteristicUpdate:
                    break;
                default:
                    break;
            }
        }
        #endregion
        #region BACKGROUND TASK
        private void runBackgroundTask()
        {
            // Sample code to localize the ApplicationBar
            //BuildLocalizedApplicationBar();
            bool agentsAreEnabled = true;

            // Obtain a reference to the period task, if one exists
            periodicTask = ScheduledActionService.Find(TaskName) as PeriodicTask;

            if (periodicTask != null)
            {
                removeBackgroundTask(TaskName);
            }

            periodicTask = new PeriodicTask(TaskName);

            // The description is required for periodic agents. This is the string that the user
            // will see in the background services Settings page on the device.
            periodicTask.Description = "This demonstrates a periodic task.";

            // Place the call to Add in a try block in case the user has disabled agents.
            try
            {
                ScheduledActionService.Add(periodicTask);
                // If debugging is enabled, use LaunchForTest to launch the agent in one minute.
#if(DEBUG)
                ScheduledActionService.LaunchForTest(TaskName, TimeSpan.FromSeconds(10));
#endif
            }
            catch (InvalidOperationException exception)
            {
                if (exception.Message.Contains("BNS Error: The action is disabled"))
                {
                    //MessageBox.Show("Background agents for this application have been disabled by the user.");
                    agentsAreEnabled = false;
                }
                if (exception.Message.Contains("BNS Error: The maximum number of ScheduledActions of this type have already been added."))
                {
                    // No user action required. The system prompts the user when the hard limit of periodic tasks has been reached.
                }
            }
            catch (SchedulerServiceException)
            {
                // No user action required.  
            }
        }
        private void removeBackgroundTask(string name)
        {
            try
            {
                ScheduledActionService.Remove(name);
            }
            catch (Exception)
            {
            }
        }
        #endregion
        #region INAPP PURCHASE
        private async void listInapp(ServerStream stream)
        {
            try
            {
                ListingInformation li = await CurrentApp.LoadListingInformationAsync();
                if (li != null)
                {//infomation loaded
                    int size = 4;
                    foreach (KeyValuePair<string, ProductListing> pair in li.ProductListings)
                    {
                        string price = pair.Value.FormattedPrice;
                        string id = pair.Value.ProductId;
                        size += 2 + 2 * price.Length;
                        size += 2 + 2 * id.Length;
                        size += 2;
                    }
                    StreamBuffer buf = new StreamBuffer(size);
                    buf.writeInt32((int)li.ProductListings.Keys.Count());
                    foreach (KeyValuePair<string, ProductListing> pair in li.ProductListings)
                    {
                        string price = pair.Value.FormattedPrice;
                        string id = pair.Value.ProductId;
                        buf.writeSStr(id);
                        buf.writeSStr(price);
                        if (pair.Value.ProductType == Windows.ApplicationModel.Store.ProductType.Durable)
                        {
                            buf.writeInt16(0);
                        }
                        else
                        {
                            buf.writeInt16(1);
                        }
                    }
                    stream.ResponseData = buf.Data;
                    stream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                }
                else
                {//case not finish load infomation from server
                    stream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    stream.ErrorCode = 1;
                }
            }
            catch (Exception ex)
            {
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }
        private async void buyItem(ServerStream stream)
        {
            String id = (new StreamBuffer(stream.PostData)).readSStr();
            try
            {
                string recept = await CurrentApp.RequestProductPurchaseAsync(id, true);
                LicenseInformation license = CurrentApp.LicenseInformation;
                if (license.ProductLicenses[id].IsConsumable)
                {
                    if (license.ProductLicenses[id].IsActive)
                    {
                        CurrentApp.ReportProductFulfillment(id);
                        stream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    }
                    else
                    {
                        stream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                        stream.ErrorCode = 1;
                    }
                }
                else
                {
                    if (license.ProductLicenses[id].IsActive)
                    {
                        stream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    }
                    else
                    {
                        stream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                        stream.ErrorCode = 1;
                    }
                }
            }
            catch (Exception ex)
            {
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
            //MessageBox.Show(recept);
        }
        #endregion
        private void DrawingSurfaceBackground_Loaded(object sender, RoutedEventArgs e)
        {
            if (m_d3dBackground == null)
            {
                m_d3dBackground = new Direct3DBackground();

                // Set window bounds in dips
                m_d3dBackground.WindowBounds = new Windows.Foundation.Size(
                    (float)Application.Current.Host.Content.ActualWidth,
                    (float)Application.Current.Host.Content.ActualHeight
                    );

                // Set native resolution in pixels
                m_d3dBackground.NativeResolution = new Windows.Foundation.Size(
                    (float)Math.Floor(Application.Current.Host.Content.ActualWidth * Application.Current.Host.Content.ScaleFactor / 100.0f + 0.5f),
                    (float)Math.Floor(Application.Current.Host.Content.ActualHeight * Application.Current.Host.Content.ScaleFactor / 100.0f + 0.5f)
                    );

                // Set render resolution to the full native resolution
                m_d3dBackground.RenderResolution = m_d3dBackground.NativeResolution;
                m_d3dBackground.OnRequestBaseService += m_d3dBackground_OnRequestBaseService;
                //m_d3dBackground.OnRequestBaseService += m_d3dBackground_OnRequestBaseService;
                // Hook-up native component to DrawingSurfaceBackgroundGrid
                DrawingSurfaceBackground.SetBackgroundContentProvider(m_d3dBackground.CreateContentProvider());
                DrawingSurfaceBackground.SetBackgroundManipulationHandler(m_d3dBackground);
            }
        }
        ServerStream _stream_share_media = null;
        ServerStream _stream_loadTwFollower = null;
        ServerStream _stream_loadFbFriend = null;
        ServerStream _stream_loginAccount = null;
        ServerStream _stream_load_image = null;
        ServerStream _stream_load_file = null;
        ServerStream _stream_post_photo_fb = null;
        ServerStream _stream_post_highscore = null;
        Dictionary<Actions.Action, ServerStream> _stream_wating = new Dictionary<Actions.Action, ServerStream>();
        //List<Actions.ActionArgLinker> _popup_acts = new List<Actions.ActionArgLinker>();
        void m_d3dBackground_OnRequestBaseService(UInt32 contructor_id, UInt32 data_len)
        {
            Transaction transaction = new Transaction();
            transaction.Instructor = (ConnectConstructor)contructor_id;
            switch (transaction.Instructor)
            {
                case ConnectConstructor.MAIL_SEND:
                    Dispatcher.BeginInvoke(() => mail_send(transaction));
                    break;
                case ConnectConstructor.SMS_SEND:
                    Dispatcher.BeginInvoke(() => sms_send(transaction));
                    break;
                case ConnectConstructor.NOTIFICATION_RUN:
                    runBackgroundTask();
                    break;
                case ConnectConstructor.NOTIFICATION_STOP:
                    removeBackgroundTask(TaskName);
                    break;
                case ConnectConstructor.GAME_EXIT:
                    Dispatcher.BeginInvoke(() => close_app(false));
                    break;
                case ConnectConstructor.SERVER_CHECK_AND_RUN:
                    Dispatcher.BeginInvoke(() =>
                    {
                        ServerStreamManager sv = ServerStreamManager.getInstance();
                        for (int i = 0; i < sv.StreamNum; i++)
                        {
                            if (sv.Streams[i].Status == (int)ServerStreamStatus.STREAM_STATUS_WAITING)
                            {
                                sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_REQUESTING;
                                //sv.Streams[i].ResponseData = null;
                                ServerStreamConstructor svcode = (ServerStreamConstructor)sv.Streams[i].RequestCode;
                                switch (svcode)
                                {
                                    case ServerStreamConstructor.REQUEST_CODE_GAME_EXIT:
                                        {
                                            StreamBuffer buf = new StreamBuffer(sv.Streams[i].PostData);
                                            int sudden = buf.readInt32();
                                            sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                                            Dispatcher.BeginInvoke(() => close_app(sudden != 0));
                                        }
                                        break;
                                    case ServerStreamConstructor.REQUEST_CODE_FB_LOGIN:
                                        {
#if BUILD_SUPPORT
                                        ActionSupportAccountAskArgs arg = new ActionSupportAccountAskArgs();
                                        arg.ParentLayout = AppObject.RootLayout;
                                        ActionSupportAccountAsk support_act = new ActionSupportAccountAsk();
                                        _stream_wating.Add(support_act, sv.Streams[i]);
                                        support_act.OnDone += support_act_OnDone;
                                        support_act.OnCancel += support_act_OnCancel;
                                        support_act.Do(arg);
#else
                                            if (sv.Streams[i].PostData != null)
                                            {
                                                if (AppObject.GameInfo.IsRequireAccount)
                                                {
                                                    if (FacebookClient.Instance.RequestLogin == 0)
                                                    {
                                                        action_account_ask_stream_do(sv.Streams[i]);
                                                    }
                                                    else
                                                    {
                                                        if (FacebookClient.Instance.IsUserLogedin())
                                                        {
                                                            sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                                                        }
                                                        else
                                                        {
                                                            sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    if (FacebookClient.Instance.RequestLogin == 1)
                                                    {
                                                        FacebookManager.login(AppObject.GameInfo.FacebookApp,sv.Streams[i], null, null, null);
                                                    }
                                                    else if (FacebookClient.Instance.RequestLogin == 0)
                                                    {
                                                        action_account_ask_stream_do(sv.Streams[i]);
                                                    }
                                                    else
                                                    {
                                                        if (FacebookClient.Instance.IsUserLogedin())
                                                        {
                                                            sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                                                        }
                                                        else
                                                        {
                                                            sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                                        }
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                FacebookManager.login(AppObject.GameInfo.FacebookApp,sv.Streams[i], null, null, null);
                                            }
#endif
                                        } break;
                                    case ServerStreamConstructor.REQUEST_CODE_FB_GET_AVATAR:
                                        {
                                            action_fbavar_stream_do(sv.Streams[i]);
                                        } break;
                                    case ServerStreamConstructor.REQUEST_CODE_FB_POST_PHOTO:
                                        {
                                            StreamBuffer buf = new StreamBuffer(sv.Streams[i].PostData);
                                            string filename = buf.readSStr();
                                            string message = buf.readSStr();
                                            if (!FileManager.FileExists(filename))
                                            {
                                                if (_stream_post_photo_fb == null || sv.Streams[i] == _stream_post_photo_fb)
                                                {
                                                    sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_WAITING;
                                                    break;
                                                }
                                                else
                                                {
                                                    sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                                    break;
                                                }
                                            }
                                            if (_stream_post_photo_fb == null)
                                            {
                                                _stream_post_photo_fb = sv.Streams[i];
                                                
                                                Dispatcher.BeginInvoke(() =>
                                                {

                                                    var raw_stream = FileManager.GetFileStream(filename);
                                                    if (raw_stream != null)
                                                    {
                                                        byte[] tmp = new byte[4];
                                                        raw_stream.Read(tmp, 0, 4);
                                                        int width = BitConverter.ToInt32(tmp, 0);
                                                        raw_stream.Read(tmp, 0, 4);
                                                        int height = BitConverter.ToInt32(tmp, 0);
                                                        var stream = FileManager.GetFileStreamForWrite("tmp_screenshot.png");
                                                        if (stream != null)
                                                        {
                                                            PNGWriter.WritePNG(raw_stream, stream, width, height);
                                                            stream.Close();
                                                        }
                                                        raw_stream.Close();
                                                        ActionFacebookPostPhotoArgs arg = new ActionFacebookPostPhotoArgs() { FileName = filename, Message = message };
                                                        ActionFacebookPostPhoto action = new ActionFacebookPostPhoto();
                                                        action.OnDone += action_fbpost_photo_OnDone;
                                                        action.OnCancel += action_fbpost_photo_OnCancel;
                                                        action.Do(arg);
                                                    }
                                                    else
                                                    {
                                                        _stream_post_photo_fb.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                                    }
                                                });
                                            }
                                            else
                                            {
                                                sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                            }
                                            break;
                                        }
                                    case ServerStreamConstructor.REQUEST_CODE_OPEN_BROWSER:
                                        {
                                            openBrowser(sv.Streams[i]);
                                        } break;
                                    case ServerStreamConstructor.REQUEST_CODE_RATING:
                                        {
                                            rating(sv.Streams[i]);
                                        } break;
                                    case ServerStreamConstructor.REQUEST_CODE_PURCHASE:
                                        {
                                            buyItem(sv.Streams[i]);
                                        }
                                        break;
                                    case ServerStreamConstructor.REQUEST_CODE_PURCHASE_GETLIST:
                                        {
                                            listInapp(sv.Streams[i]);
                                        } break;
                                    case ServerStreamConstructor.REQUEST_CODE_FB_FEED_ME:
                                        {
                                            fbfeed(sv.Streams[i]);
                                        } break;
                                    case ServerStreamConstructor.REQUEST_CODE_TW_LOGIN:
                                        {
                                            TwitterManager.login(sv.Streams[i], null, action_twitter_login_OnDone, action_twitter_login_OnCancel);
                                        } break;
                                    case ServerStreamConstructor.REQUEST_CODE_TW_FEED:
                                        {
                                            TwitterManager.twfeed(sv.Streams[i]);
                                        } break;
                                    case ServerStreamConstructor.REQUEST_CODE_FB_GET_FRIEND:
                                        {
                                            if (_stream_loadFbFriend == null)
                                            {
                                                _stream_loadFbFriend = sv.Streams[i];
                                                FacebookManager.getFriends(null, action_fbfriend_stream_OnDone, action_fbfriend_stream_OnCancel);
                                            }
                                            else
                                            {
                                                sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                            }
                                        } break;
                                    case ServerStreamConstructor.REQUEST_CODE_LOAD_IMAGE:
                                        {
                                            if (_stream_load_image == null)
                                            {
                                                _stream_load_image = sv.Streams[i];
                                                loadImage();
                                            }
                                            else
                                            {
                                                sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                            }
                                        } break;
                                    case ServerStreamConstructor.REQUEST_CODE_TW_GET_FRIEND:
                                        {
                                            if (_stream_loadTwFollower == null)
                                            {
                                                _stream_loadTwFollower = sv.Streams[i];
                                                TwitterManager.getFollower(action_twfollower_stream_onBegin, action_twfollower_stream_onDone, action_twfollower_stream_onCancel);
                                            }
                                            else
                                            {
                                                sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                            }
                                        }
                                        break;
                                    case ServerStreamConstructor.REQUEST_CODE_SMS_SEND:
                                        {
                                            if (sv.Streams[i].PostData != null && sv.Streams[i].PostData.Length > 0)
                                            {
                                                StreamBuffer buff = new StreamBuffer(sv.Streams[i].PostData);
                                                int type = buff.readInt16();
                                                if (type == 1)
                                                {
                                                    string title = buff.readSStr();
                                                    string message = buff.readSStr();
                                                    sendSMS(title, message);
                                                    sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                                                }
                                                else
                                                {
                                                    sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                                }

                                            }
                                            else
                                            {
                                                sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                            }
                                        } break;
                                    case ServerStreamConstructor.REQUEST_CODE_MAIL_SEND:
                                        {
                                            if (sv.Streams[i].PostData != null && sv.Streams[i].PostData.Length > 0)
                                            {
                                                StreamBuffer buff = new StreamBuffer(sv.Streams[i].PostData);
                                                int type = buff.readInt16();
                                                if (type == 1)
                                                {
                                                    string title = buff.readSStr();
                                                    string message = buff.readSStr();
                                                    sendMail(title, message);
                                                    sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                                                }
                                                else
                                                {
                                                    sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                                }

                                            }
                                            else
                                            {
                                                sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                            }
                                        } break;
                                    case ServerStreamConstructor.REQUEST_CODE_LOAD_FILE:
                                        {
                                            if (_stream_load_file == null)
                                            {
                                                _stream_load_file = sv.Streams[i];
                                                loadFile();
                                            }
                                            else
                                            {
                                                sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                            }
                                            break;
                                        }
                                    case ServerStreamConstructor.REQUEST_CODE_ADMOB_INTERSTITIAL_PREPARE:
                                        {
                                            if (sv.Streams[i].PostData != null && sv.Streams[i].PostData.Length > 0)
                                            {
                                                AdmobPrepare(sv.Streams[i]);
                                            }
                                            else
                                            {
                                                sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                                            }
                                        } break;
                                    case ServerStreamConstructor.REQUEST_CODE_ADMOB_INTERSTITIAL_SHOW:
                                        {
                                            try
                                            {

                                                Deployment.Current.Dispatcher.BeginInvoke(() =>
                                                {
                                                    AdmobShow();
                                                    Thread.Sleep(50);
                                                    //sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                                                });
                                            }
                                            catch (Exception ex)
                                            {
                                                Debug.WriteLine(ex.Message);
                                            }
                                        }
                                        break;
                                    //Bo adds
                                    case ServerStreamConstructor.REQUEST_CODE_FB_POST_SCORE:
                                        {
                                            if (sv.Streams[i].PostData != null && sv.Streams[i].PostData.Length > 0)
                                            {
                                                _stream_post_highscore = sv.Streams[i];
                                                StreamBuffer buffer = new StreamBuffer(sv.Streams[i].PostData);
                                                int _score = buffer.readInt32();
                                                FacebookManager.UpdateNewScore(_score, sv.Streams[i], null, null, null);
                                            }
                                            else
                                            {
                                                sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                                            }
                                        }
                                        break;

                                    //for support
#if BUILD_SUPPORT
                                    case ServerStreamConstructor.REQUEST_CODE_SUPPORT_GET_ACCOUNT_ID:
                                        {
                                            ActionSupportAccountAskArgs arg = new ActionSupportAccountAskArgs();
                                            arg.ParentLayout = AppObject.RootLayout;
                                            ActionSupportAccountAsk support_act = new ActionSupportAccountAsk();
                                            _stream_wating.Add(support_act, sv.Streams[i]);
                                            support_act.OnDone += support_global_act_OnDone;
                                            support_act.OnCancel += support_act_OnCancel;
                                            support_act.Do(arg);
                                        }
                                        break;
#endif
                                    case ServerStreamConstructor.REQUEST_CODE_MEDIA_LINK_TASK:
                                        if (sv.Streams[i].PostData != null && sv.Streams[i].PostData.Length > 0)
                                        {
                                            StreamBuffer buff = new StreamBuffer(sv.Streams[i].PostData);
                                            if (buff.Data != null)
                                            {
                                                string title = buff.readSStr();
                                                string url = buff.readSStr();
                                                string message = buff.readSStr();
                                                ShareLinkTask(title, url, message);
                                                sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                                            }
                                            else
                                            {
                                                sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                            }

                                        }
                                        else
                                        {
                                            sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                        }
                                        break;
                                    case ServerStreamConstructor.REQUEST_CODE_SHARE_MEDIA_TASK:
                                        if (sv.Streams[i].PostData != null && sv.Streams[i].PostData.Length > 0)
                                        {
                                            StreamBuffer buff = new StreamBuffer(sv.Streams[i].PostData);
                                            string filename = buff.readSStr();
                                            if (buff.Data != null)
                                            {
                                                if (!FileManager.FileExists(filename))
                                                {
                                                    if (_stream_share_media == null || sv.Streams[i] == _stream_share_media)
                                                    {
                                                        sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_WAITING;
                                                        break;
                                                    }
                                                    else
                                                    {
                                                        sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                                        break;
                                                    }
                                                }
                                                if (_stream_share_media == null)
                                                {
                                                    _stream_share_media = sv.Streams[i];
                                                    Dispatcher.BeginInvoke(() =>
                                                    {
                                                        bool success = false;
                                                        var raw_stream = FileManager.GetFileStream(filename);
                                                        if (raw_stream != null)
                                                        {
                                                            byte[] tmp = new byte[4];
                                                            raw_stream.Read(tmp, 0, 4);
                                                            int width = BitConverter.ToInt32(tmp, 0);
                                                            raw_stream.Read(tmp, 0, 4);
                                                            int height = BitConverter.ToInt32(tmp, 0);
                                                            var stream = FileManager.GetFileStreamForWrite("tmp_screenshot.png");
                                                            if (stream != null)
                                                            {
                                                                //using (MemoryStream memoryStream = new MemoryStream())
                                                                {
                                                                    //raw_stream.CopyTo(memoryStream);
                                                                    //memoryStream.Position = 0;
                                                                    //byte[] buffer = null;
                                                                    //if (memoryStream != null && memoryStream.Length > 0)
                                                                    {
                                                                        //BinaryReader binaryReader = new BinaryReader(memoryStream);
                                                                        //buffer = binaryReader.ReadBytes((int)memoryStream.Length);
                                                                        //stream.Write(buffer, 0, buffer.Length);
                                                                        //stream.Seek(0, SeekOrigin.Begin);
                                                                        //BitmapImage bitmapImage = new BitmapImage { CreateOptions = BitmapCreateOptions.None };
                                                                        //bitmapImage.SetSource(stream);
                                                                        //WriteableBitmap writeableBitmap = new WriteableBitmap(bitmapImage);
                                                                        //writeableBitmap.SaveJpeg((IsolatedStorageFileStream)stream, writeableBitmap.PixelWidth, writeableBitmap.PixelHeight, 0, 100);
                                                                        PNGWriter.WritePNG(raw_stream, stream, width, height);
                                                                        stream.Close();
                                                                        //CameraCaptureTask cameraCaptureTask = new CameraCaptureTask();
                                                                        //cameraCaptureTask.Completed += cameraCaptureTask_Completed;
                                                                        //cameraCaptureTask.Show();
                                                                        //ShareMediaTask(((IsolatedStorageFileStream)raw_stream).Name.Replace(filename, "tmp_screenshot.png"));
                                                                        ShareMediaTask("tmp_screenshot.png");
                                                                        success = true;
                                                                    }
                                                                }
                                                            }
                                                            raw_stream.Close(); 
                                                        }
                                                        if (success)
                                                        {
                                                            _stream_share_media.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                                                        }
                                                        else
                                                        {
                                                            _stream_share_media.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                                        }
                                                    });
                                                }
                                            }                                              
                                            else
                                            {
                                                sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                            }

                                        }
                                        else
                                        {
                                            sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                        }
                                        break;
                                    default:
                                        AppObject.ClientSvc.process(new Object.ClientServiceRequest((int)sv.Streams[i].RequestCode), sv.Streams[i]);
                                        break;
                                }
                            }
                        }
                    });
                    break;
            }
        }

        
        void action_fbpost_photo_OnCancel(object sender, ActionArgs e)
        {
            if (_stream_post_photo_fb != null)
            {
                _stream_post_photo_fb.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_post_photo_fb = null;
            }
        }

        void action_fbpost_photo_OnDone(object sender, ActionArgs e)
        {
            if (_stream_post_photo_fb != null)
            {
                _stream_post_photo_fb.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                _stream_post_photo_fb = null;
            }
        }

        private void action_twfollower_stream_onCancel(object sender, ActionArgs e)
        {
            if (_stream_loadTwFollower != null)
            {
                _stream_loadTwFollower.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_loadTwFollower = null;
            }
        }

        private void action_twfollower_stream_onDone(object sender, ActionArgs e)
        {
            if (_stream_loadTwFollower != null)
            {
                var arg = e as Actions.ActionTwitterGetFollowerArgs;
                if (arg != null)
                {
                    int buffsize = 4 + arg.FriendIds.Count * 8;
                    StreamBuffer buff = new StreamBuffer(buffsize);
                    buff.writeInt32(arg.FriendIds.Count);
                    for (int i = 0; i < arg.FriendIds.Count; i++)
                    {
                        buff.writeUInt64(arg.FriendIds[i]);
                    }
                    _stream_loadTwFollower.ResponseData = buff.Data;
                    _stream_loadTwFollower.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                }
                else
                {
                    _stream_loadTwFollower.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    _stream_loadTwFollower = null;
                }
                _stream_loadTwFollower = null;
            }
        }

        private void action_twfollower_stream_onBegin(object sender, ActionArgs e)
        {
            //throw new NotImplementedException();
        }

       
        #region CLOSE APP ASK
        Dialogs.CloseAppAskDialog CloseDialog = null;
        void close_app(bool is_sudden)
        {
            if (is_sudden)
            {
                PhoneDirect3DXamlAppComponent.ApplicationInfomation.onClosing();
                Application.Current.Terminate();
            }
            else
            {
                if (CloseDialog == null)
                {
                    CloseDialog = new CloseAppAskDialog();
                    CloseDialog.OnComplete += close_app_dialog_OnComplete;
                }
                if (!CloseDialog.isShow)
                {
                    LayoutRoot.Children.Add(CloseDialog.Dialog);
                    CloseDialog.show();
                }
                else
                {
                    LayoutRoot.Children.Remove(CloseDialog.Dialog);
                    CloseDialog.hide();
                }
            }
        }

        void close_app_dialog_OnComplete(object sender, CloseAppAskDialogArgs a)
        {
            Dispatcher.BeginInvoke(() =>
            {
                try
                {
                    LayoutRoot.Children.Remove(((Dialogs.CloseAppAskDialog)sender).Dialog);
                }
                catch (Exception ex)
                {
                }
                if (a.Result == CloseAppAskDialogResult.OK)
                {

                    PhoneDirect3DXamlAppComponent.ApplicationInfomation.onClosing();
                    Application.Current.Terminate();

                }
            });
        }
        #endregion
        #region ACCOUNT ASK
        void action_account_ask_stream_do(ServerStream stream)
        {
            Actions.ActionAccountAskArg arg = new Actions.ActionAccountAskArg(LayoutRoot);
            arg.IsRequire = AppObject.GameInfo.IsRequireAccount;
            Actions.ActionAccountAsk act = new Actions.ActionAccountAsk();
            _stream_wating.Add(act, stream);
            try
            {
                act.OnDone += action_account_ask_stream_OnDone;
                act.OnCancel += action_account_ask_stream_OnCancel;
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_REQUESTING;
                act.Do(arg);
                AppObject.PopupOfAction.Add(new Actions.ActionArgLinker(act, arg));
            }
            catch (Exception ex)
            {
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }

        private void action_account_ask_stream_OnCancel(object sender, Actions.ActionArgs e)
        {
            try
            {
                ActionAccountAskArg arg = e as ActionAccountAskArg;
                foreach (ActionArgLinker link in AppObject.PopupOfAction)
                {
                    if (link.AttachArg == arg)
                    {
                        AppObject.PopupOfAction.Remove(link);
                        break;
                    }
                }
                ServerStream svstream = _stream_wating[(Actions.Action)sender];
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_wating.Remove((Actions.Action)sender);

            }
            catch (Exception ex)
            {
            }
        }

        private void action_account_ask_stream_OnDone(object sender, Actions.ActionArgs e)
        {
            try
            {
                Actions.ActionAccountAskArg arg = e as Actions.ActionAccountAskArg;
                foreach (ActionArgLinker link in AppObject.PopupOfAction)
                {
                    if (link.AttachArg == arg)
                    {
                        AppObject.PopupOfAction.Remove(link);
                        break;
                    }
                }
                if (arg.Result != null)
                {
                    ServerStream svstream = _stream_wating[(Actions.Action)sender];
                    _stream_wating.Remove((Actions.Action)sender);
                    AccountAskDialogResult rs = arg.Result.Result;
                    if (rs == AccountAskDialogResult.OK)
                    {//if ok open login fb
                        if (!arg.IsRequire)
                        {
                            FacebookClient.Instance.RequestLogin = 1;
                            FacebookClient.Instance.SaveUserData();
                        }
                        action_fblogin_stream_do(svstream);
                    }
                    else if (rs == AccountAskDialogResult.LATE)
                    {
                        FacebookClient.Instance.RequestLogin = 0;
                        FacebookClient.Instance.SaveUserData();
                        svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    }
                    else if (rs == AccountAskDialogResult.NOT_SHOW)
                    {
                        FacebookClient.Instance.RequestLogin = 2;
                        FacebookClient.Instance.SaveUserData();
                        svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    }
                    else
                    {
                        FacebookClient.Instance.RequestLogin = 1;
                        FacebookClient.Instance.SaveUserData();
                        svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    }
                }
                else
                {
                    ServerStream svstream = _stream_wating[(Actions.Action)sender];
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    _stream_wating.Remove((Actions.Action)sender);
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
        #endregion
        #region FACEBOOK
        void action_fblogin_stream_do(ServerStream stream)
        {
            
            Actions.ActionFacebookLoginArgs arg = new Actions.ActionFacebookLoginArgs(LayoutRoot){ AppInfo = AppObject.GameInfo.FacebookApp};
            Actions.ActionFacebookLogin action_fblogin = new Actions.ActionFacebookLogin();
            _stream_wating.Add(action_fblogin, stream);
            try
            {
                action_fblogin.OnDone += action_fblogin_stream_OnDone;
                action_fblogin.OnCancel += action_fblogin_stream_OnCancel;
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_REQUESTING;
                action_fblogin.Do(arg);
                AppObject.PopupOfAction.Add(new Actions.ActionArgLinker(action_fblogin, arg));
            }
            catch (Exception ex)
            {
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }
        void action_fblogin_stream_OnCancel(object sender, Actions.ActionArgs e)
        {
            try
            {
                foreach (ActionArgLinker link in AppObject.PopupOfAction)
                {
                    if (link.AttachArg == e)
                    {
                        AppObject.PopupOfAction.Remove(link);
                        break;
                    }
                }
                ServerStream svstream = _stream_wating[(Actions.Action)sender];
                _stream_wating.Remove((Actions.Action)sender);
                if (AppObject.GameInfo.IsRequireAccount)
                {
                    _stream_loginAccount = svstream;
                    AccountRequireDialog dialog = new AccountRequireDialog();
                    dialog.OnComplete += requireDialog_OnComplete;
                    FakeActionArgs arg = new FakeActionArgs();
                    FakeAction action = new FakeAction();
                    ActionArgLinker linker = new ActionArgLinker(action, arg);
                    AppObject.PopupOfAction.Add(linker);
                    action.OnCancel += fakeRequireAction_OnCancel;
                    LayoutRoot.Children.Add(dialog.Dialog);
                    dialog.show();
                }
                else
                {
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                }
            }
            catch (Exception ex)
            {
            }
        }

        void fakeRequireAction_OnCancel(object sender, ActionArgs e)
        {
            foreach (Actions.ActionArgLinker arglink in AppObject.PopupOfAction)
            {
                if (arglink.AttachArg == e)
                {
                    AppObject.PopupOfAction.Remove(arglink);
                    break;
                }
            }
            Dispatcher.BeginInvoke(() => close_app(false));
        }

        void requireDialog_OnComplete(object sender, AccountRequireDialogArgs a)
        {
            AccountRequireDialog dialog = sender as AccountRequireDialog;
            if (dialog != null)
            {
                Deployment.Current.Dispatcher.BeginInvoke(() => LayoutRoot.Children.Remove(dialog.Dialog));
            }
            if (_stream_loginAccount != null)
            {
                action_fblogin_stream_do(_stream_loginAccount);
                _stream_loginAccount = null;
            }
        }
        void action_fblogin_stream_OnDone(object sender, Actions.ActionArgs e)
        {
            try
            {
                foreach (ActionArgLinker link in AppObject.PopupOfAction)
                {
                    if (link.AttachArg == e)
                    {
                        AppObject.PopupOfAction.Remove(link);
                        break;
                    }
                }
                Actions.ActionFacebookLoginArgs fbarg = e as Actions.ActionFacebookLoginArgs;
                if (fbarg != null && (fbarg.IsCancelByLogedin || (fbarg.Result != null && fbarg.Result.IsSuccess)))
                {
                    if (AppObject.GameInfo.IsRequireAccount)
                    {
                        FacebookClient.Instance.RequestLogin = 1;
                        FacebookClient.Instance.SaveUserData();
                    }
                    if (fbarg.IsCancelByLogedin)
                    {
                        ServerStream svstream = _stream_wating[(Actions.Action)sender];
                        svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    }
                    else
                    {
                        FacebookClient.Instance.Token = fbarg.Result.Token;
                        FacebookClient.Instance.TokenExprite = fbarg.Result.TokenExprite;
                        FacebookClient.Instance.SaveUserData();
                        ServerStream svstream = _stream_wating[(Actions.Action)sender];
                        if (svstream.RequestCode == (int)ServerStreamConstructor.REQUEST_CODE_FB_FEED_ME)
                        {
                            fbfeed(svstream);
                        }
                        else
                        {
                            action_fbinfo_stream_do(svstream);
                        }
                    }
                }
                else
                {
                    ServerStream svstream = _stream_wating[(Actions.Action)sender];
                    _stream_wating.Remove((Actions.Action)sender);
                    if (AppObject.GameInfo.IsRequireAccount)
                    {
                        _stream_loginAccount = svstream;
                        AccountRequireDialog dialog = new AccountRequireDialog();
                        dialog.OnComplete += requireDialog_OnComplete;
                        FakeActionArgs arg = new FakeActionArgs();
                        FakeAction action = new FakeAction();
                        ActionArgLinker linker = new ActionArgLinker(action, arg);
                        AppObject.PopupOfAction.Add(linker);
                        action.OnCancel += fakeRequireAction_OnCancel;
                        LayoutRoot.Children.Add(dialog.Dialog);
                        dialog.show();
                    }
                    else
                    {
                        svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    }
                }
            }
            catch (Exception ex)
            {
            }
        }
        void action_fbinfo_stream_do(ServerStream onstream)
        {
            Actions.ActionFacebookGetInfomationArgs arg = new Actions.ActionFacebookGetInfomationArgs();
            Actions.ActionFacebookGetInfomation action_fbinfo = new Actions.ActionFacebookGetInfomation();
            _stream_wating.Add(action_fbinfo, onstream);
            try
            {
                action_fbinfo.OnDone += action_fbinfo_stream_onDone;
                action_fbinfo.OnCancel += action_fbinfo_stream_onCancel;
                onstream.Status = (int)ServerStreamStatus.STREAM_STATUS_REQUESTING;
                action_fbinfo.Do(arg);
            }
            catch (Exception ex)
            {
                onstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }
        void action_fbinfo_stream_onCancel(object sender, Actions.ActionArgs e)
        {
            try
            {
                ServerStream svstream = _stream_wating[(Actions.Action)sender];
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_wating.Remove((Actions.Action)sender);
            }
            catch (Exception ex)
            {
            }
        }
        void action_fbinfo_stream_onDone(object sender, Actions.ActionArgs e)
        {
            try
            {
                Actions.ActionFacebookGetInfomationArgs fbarg = e as Actions.ActionFacebookGetInfomationArgs;
                if (fbarg != null)
                {
                    ServerStream svstream = _stream_wating[(Actions.Action)sender];
                    PlayerInfomation.FacebookId = FacebookClient.Instance.UserId;
                    PlayerInfomation.FacebookUserName = FacebookClient.Instance.UserName;
                    PlayerInfomation.FacebookAvatarUrl = FacebookClient.Instance.AvatarUrl;
                    UInt64 id = UInt64.Parse(FacebookClient.Instance.UserId);
                    id = 3000000000000000000 + id;
                    PhoneDirect3DXamlAppComponent.PlayerInfomation.UserId = id.ToString();

                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    _stream_wating.Remove((Actions.Action)sender);
                }
                else
                {
                    ServerStream svstream = _stream_wating[(Actions.Action)sender];
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    _stream_wating.Remove((Actions.Action)sender);
                }
            }
            catch (Exception ex)
            {
            }
        }
        void action_fbavar_stream_do(ServerStream stream)
        {
            if (stream.PostData != null)
            {
                Actions.ActionFacebookGetAvatarArgs arg = new Actions.ActionFacebookGetAvatarArgs();
                Actions.ActionFacebookGetAvatar action_fbavar = new Actions.ActionFacebookGetAvatar();
                _stream_wating.Add(action_fbavar, stream);
                try
                {
                    action_fbavar.OnDone += action_fbavar_stream_OnDone;
                    action_fbavar.OnCancel += action_fbavar_stream_OnCancel;
                    stream.Status = (int)ServerStreamStatus.STREAM_STATUS_REQUESTING;
                    action_fbavar.Do(arg);
                }
                catch (Exception ex)
                {
                    stream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                }
            }
            else
            {
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }
        void action_fbavar_stream_OnCancel(object sender, Actions.ActionArgs e)
        {
            try
            {
                ServerStream svstream = _stream_wating[(Actions.Action)sender];
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_wating.Remove((Actions.Action)sender);
            }
            catch (Exception ex)
            {
            }
        }
        void action_fbavar_stream_OnDone(object sender, Actions.ActionArgs e)
        {
            try
            {
                Actions.ActionFacebookGetAvatarArgs fbarg = e as Actions.ActionFacebookGetAvatarArgs;
                if (fbarg != null)
                {
                    ServerStream svstream = _stream_wating[(Actions.Action)sender];
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    _stream_wating.Remove((Actions.Action)sender);
                }
                else
                {
                    ServerStream svstream = _stream_wating[(Actions.Action)sender];
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    _stream_wating.Remove((Actions.Action)sender);
                }
            }
            catch (Exception ex)
            {
            }
        }
        ServerStream FbFeedStreamWaiting = null;
        void fbfeed(ServerStream svstream)
        {
            if (FacebookClient.Instance.Token.Length > 0 && FbFeedStreamWaiting == null)
            {
                StreamBuffer buf = new StreamBuffer(svstream.PostData);
                string message = buf.readSStr();
                FbFeedStreamWaiting = svstream;
                FacebookClient.Instance.PostMessageOnWall(AppObject.GameInfo.FacebookFeedTitle,
                    message,
                    //AppObject.GameInfo.FacebookFeedAttahLink,
                    AppObject.AppLink,
                    AppObject.GameInfo.FacebookFeedImageUrl,
                    new UploadStringCompletedEventHandler(fb_feed_handle));
            }
            else
            {
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                //action_fblogin_stream_do(svstream);
            }
        }
        private void fb_feed_handle(object sender, UploadStringCompletedEventArgs e)
        {
            if (FbFeedStreamWaiting != null)
            {
                if (e != null && e.Error == null)
                {
                    FbFeedStreamWaiting.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    //MessageBox.Show("Completed");
                }
                else
                {
                    FbFeedStreamWaiting.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    //MessageBox.Show("Error! Please retry later");
                }
                FbFeedStreamWaiting = null;
            }
        }

        private void action_fbfriend_stream_OnCancel(object sender, Actions.ActionArgs e)
        {
            if (_stream_loadFbFriend != null)
            {
               // _stream_loadFbFriend.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_loadFbFriend.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
            }
        }
        private void action_fbfriend_stream_OnDone(object sender, Actions.ActionArgs e)
        {
            try
            {
                if (_stream_loadFbFriend != null)
                {
                    Actions.ActionFacebookGetFriendsArg fbarg = e as Actions.ActionFacebookGetFriendsArg;
                    if (fbarg != null)
                    {
                        if (fbarg.FriendIds.Count > 0)
                        {
                            int bufsize = 4 + fbarg.FriendIds.Count * 8;
                            StreamBuffer buf = new StreamBuffer(bufsize);
                            buf.writeInt32(fbarg.FriendIds.Count);
                            for (int i = 0; i < fbarg.FriendIds.Count; i++)
                            {
                                buf.writeInt64(fbarg.FriendIds[i]);
                            }
                            _stream_loadFbFriend.ResponseData = buf.Data;
                        }
                        else
                        {
                            int bufsize = 4;
                            StreamBuffer buf = new StreamBuffer(bufsize);
                            buf.writeInt32(0);
                            _stream_loadFbFriend.ResponseData = buf.Data;
                            _stream_loadFbFriend.ErrorCode = 1;
                        }
                        _stream_loadFbFriend.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    }
                    else
                    {
                        _stream_loadFbFriend.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    }
                    _stream_loadFbFriend = null;
                }
            }
            catch (Exception ex)
            {
            }
        }
        #endregion
        #region TWITTER
        void action_twitter_login_do(ServerStream stream)
        {
            Actions.ActionTwitterLoginArgs arg = new Actions.ActionTwitterLoginArgs(LayoutRoot);
            Actions.ActionTwitterLogin action_twlogin = new Actions.ActionTwitterLogin();
            _stream_wating.Add(action_twlogin, stream);
            try
            {
                action_twlogin.OnDone += action_twitter_login_OnDone;
                action_twlogin.OnCancel += action_twitter_login_OnCancel;
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_REQUESTING;
                AppObject.PopupOfAction.Add(new Actions.ActionArgLinker(action_twlogin, arg));
                action_twlogin.Do(arg);
            }
            catch (Exception ex)
            {
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }
        private void action_twitter_login_OnDone(object sender, Actions.ActionArgs e)
        {
            try
            {
                Actions.ActionTwitterLoginArgs twarg = e as Actions.ActionTwitterLoginArgs;
                if (twarg != null && (twarg.IsCancelByLogedin || (twarg.Result != null && twarg.Result.IsSuccess)))
                {
                    PhoneDirect3DXamlAppComponent.PlayerInfomation.TwitterId = TwitterClient.UserID;
                    PhoneDirect3DXamlAppComponent.PlayerInfomation.TwitterUserName = TwitterClient.UserScreenName;
                    PhoneDirect3DXamlAppComponent.PlayerInfomation.LoginedTwitter = true;
                    PhoneDirect3DXamlAppComponent.PlayerInfomation.LoginedTwitter = true;
                    if (twarg.IsCancelByLogedin)
                    {
                        ServerStream svstream = _stream_wating[(Actions.Action)sender];
                        svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                        _stream_wating.Remove((Actions.Action)sender);
                    }
                    else
                    {
                        ServerStream svstream = _stream_wating[(Actions.Action)sender];
                        svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                        _stream_wating.Remove((Actions.Action)sender);
                    }
                }
                else
                {
                    ServerStream svstream = _stream_wating[(Actions.Action)sender];
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    _stream_wating.Remove((Actions.Action)sender);
                }
                foreach (Actions.ActionArgLinker arglink in AppObject.PopupOfAction)
                {
                    if (arglink.AttachArg == twarg)
                    {
                        AppObject.PopupOfAction.Remove(arglink);
                        break;
                    }
                }
            }
            catch (Exception ex)
            {
            }
        }
        private void action_twitter_login_OnCancel(object sender, Actions.ActionArgs e)
        {
            try
            {
                ServerStream svstream = _stream_wating[(Actions.Action)sender];
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_wating.Remove((Actions.Action)sender);
                foreach (Actions.ActionArgLinker arglink in AppObject.PopupOfAction)
                {
                    if (arglink.AttachArg == e)
                    {
                        AppObject.PopupOfAction.Remove(arglink);
                        break;
                    }
                }
            }
            catch (Exception ex)
            {
            }
        }
        ServerStream TWFeedStreamWaiting = null;
        void twfeed(ServerStream svstream)
        {
            if (TwitterClient.isAlreadyLoggedIn() && TWFeedStreamWaiting == null)
            {
                StreamBuffer buf = new StreamBuffer(svstream.PostData);
                string message = buf.readSStr();
                TWFeedStreamWaiting = svstream;
                TwitterClient.PostTweet(message + " " + AppObject.AppLink, action_twitter_feed_callback);
            }
            else
            {
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }

        private void action_twitter_feed_callback(RestRequest request, RestResponse response, object userState)
        {
            if (TWFeedStreamWaiting != null)
            {
                Deployment.Current.Dispatcher.BeginInvoke(() =>
                {
                    if (response != null)
                    {
                        if (response.StatusCode == HttpStatusCode.OK)
                        {
                            TWFeedStreamWaiting.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                            //MessageBox.Show("Completed");
                        }
                        else if (response.StatusCode == HttpStatusCode.Forbidden)
                        {
                            //MessageBox.Show("You’ve reached the limit for number of posts today");
                            TWFeedStreamWaiting.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                            TWFeedStreamWaiting.ErrorCode = 1;
                        }
                        else
                        {
                            //MessageBox.Show("Error");
                            TWFeedStreamWaiting.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                            TWFeedStreamWaiting.ErrorCode = 1;
                        }
                    }
                    else
                    {
                        TWFeedStreamWaiting.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    }
                });
            }
        }

        #endregion
        #region BROWSER MAIL MESSAGE RATING
        void mail_send(Transaction transaction)
        {
            EmailComposeTask emailComposeTask = new EmailComposeTask();
            emailComposeTask.Subject = "Join me in Pocket Army!";
            emailComposeTask.Body = "Come visit my army in Pocket Army! You can build your own army and challenge my army too! If you don't already have it, go <a href='http://windowsphone.com/s?appId=d238a3b6-2ed8-4bbd-9425-8f17fedc22d9'>download it now</a> on your phone!.";
            emailComposeTask.Show();
        }
        void sms_send(Transaction transaction)
        {
            string ismsboby = "Come visit my army in Pocket Army! You can build your own army and challenge my army too! If you don't already have it, go <a href='http://windowsphone.com/s?appId=d238a3b6-2ed8-4bbd-9425-8f17fedc22d9'>download it now</a> on your phone!"; // Fetch the text from textbox created in XAML
            SmsComposeTask smsComposeTask = new SmsComposeTask();
            smsComposeTask.Body = ismsboby; // the string containing the sms body
            smsComposeTask.Show(); // this will invoke the native sms edtior
        }
        void openBrowser(ServerStream svstream)
        {
            StreamBuffer buff = new StreamBuffer(svstream.PostData);
            String url = buff.readSStr();
            Windows.System.Launcher.LaunchUriAsync(new Uri(url));
            svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_FREE;
        }
        void rating(ServerStream svstream)
        {
            StreamBuffer buff = new StreamBuffer(svstream.PostData);
            int is_opendialog = buff.readInt32();
            if (is_opendialog != 0)
            {
                String title = buff.readSStr();
                String message = buff.readSStr();
                if (MessageBox.Show(message, title, MessageBoxButton.OKCancel) == MessageBoxResult.OK)
                {
                    MarketplaceReviewTask marketplaceReviewTask = new MarketplaceReviewTask();
                    marketplaceReviewTask.Show();
                }
            }
            else
            {
                MarketplaceReviewTask marketplaceReviewTask = new MarketplaceReviewTask();
                marketplaceReviewTask.Show();
            }
        }
        #endregion BROWSER MAIL MESSAGE RATING
        void delegate_fb_update_infomation(object sender, FacebookUpdateInfomationArgs e)
        {
            PhoneDirect3DXamlAppComponent.PlayerInfomation.FacebookId = FacebookClient.Instance.UserId;
            PhoneDirect3DXamlAppComponent.PlayerInfomation.FacebookUserName = FacebookClient.Instance.UserName;
            PhoneDirect3DXamlAppComponent.PlayerInfomation.FacebookAvatarUrl = FacebookClient.Instance.AvatarUrl;
            PhoneDirect3DXamlAppComponent.PlayerInfomation.LoginedFacebook = true;
            UInt64 id = UInt64.Parse(FacebookClient.Instance.UserId);
            id = 3000000000000000000 + id;
            PhoneDirect3DXamlAppComponent.PlayerInfomation.UserId = id.ToString();
            PhoneDirect3DXamlAppComponent.ApplicationInfomation.onLoginFacebook();
        }

        private void OnBackKeyPress(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (AppObject.PopupOfAction.Count > 0)
            {
                Dispatcher.BeginInvoke(() =>
                {
                    ActionArgLinker lastpopup = AppObject.PopupOfAction[AppObject.PopupOfAction.Count - 1];
                    AppObject.PopupOfAction.Remove(lastpopup);
                    lastpopup.AttachAction.Cancel(lastpopup.AttachArg);
                });
                e.Cancel = true;
            }
            else if (m_d3dBackground != null)
            {
                e.Cancel = m_d3dBackground.OnBackKeyPress();
            }
        }
        #region LOAD FILE
        public void loadFile()
        {
            if (_stream_load_file != null)
            {
                StreamBuffer buff = new StreamBuffer(_stream_load_file.PostData);
                int data_type = buff.readInt16();
                if (data_type == 1)
                {
                    try
                    {
                        string url = buff.readSStr();
                        string fname = buff.readSStr();

                        ActionDownloadFileArg arg = new ActionDownloadFileArg(url,fname);
                        ActionDownloadFile action = new ActionDownloadFile();
                        action.OnDone += actionLoadFile_OnDone;
                        action.OnCancel += actionLoadFile_OnCancel;
                        action.Do(arg);
                    }
                    catch (Exception ex)
                    {
                        _stream_load_file.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                        _stream_load_file = null;
                    }
                }
                else
                {
                    _stream_load_file.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    _stream_load_file = null;
                }
            }
        }

        private void actionLoadFile_OnCancel(object sender, ActionArgs e)
        {
            if (_stream_load_file != null)
            {
                _stream_load_file.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_load_file = null;
            }
        }

        private void actionLoadFile_OnDone(object sender, ActionArgs e)
        {
            if (_stream_load_file != null)
            {
                _stream_load_file.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                _stream_load_file = null;
            }
        }
        #endregion
        #region LOAD IMAGE
        public void loadImage()
        {
            if (_stream_load_image != null)
            {
                StreamBuffer buff = new StreamBuffer(_stream_load_image.PostData);
                int data_type = buff.readInt16();
                if (data_type == 1)
                {
                    try
                    {
                        string url = buff.readSStr();
                        string fname = buff.readSStr();
                        int width = buff.readInt32();
                        int height = buff.readInt32();
                        ActionLoadImageArgs arg = new ActionLoadImageArgs() { ImageUri = new Uri(url, UriKind.Absolute), DesFileName = fname, DesHeight = height, DesWidth = width };
                        ActionLoadImage action = new ActionLoadImage();
                        action.OnDone += actionLoadImage_OnDone;
                        action.OnCancel += actionLoadImage_OnCancel;
                        action.Do(arg);
                    }
                    catch (Exception ex)
                    {
                        _stream_load_image.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                        _stream_load_image = null;
                    }
                }
                else
                {
                    _stream_load_image.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    _stream_load_image = null;
                }
            }
        }

        void actionLoadImage_OnCancel(object sender, ActionArgs e)
        {
            if (_stream_load_image != null)
            {
                _stream_load_image.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_load_image = null;
            }
        }

        void actionLoadImage_OnDone(object sender, ActionArgs e)
        {
            if (_stream_load_image != null)
            {
                var arg = e as ActionLoadImageArgs;
                if (arg != null)
                {
                    try
                    {
                        MemoryStream ms = new MemoryStream();
                        WriteableBitmap wbmp = new WriteableBitmap(arg.Image);
                        WriteableBitmap nbmp = new WriteableBitmap(arg.DesWidth, arg.DesHeight);
                        
                        wbmp.SaveJpeg(ms, arg.DesWidth, arg.DesHeight, 0, 10);
                        //ExtendedImage image = new ExtendedImage();
                        //image = wbmp.ToImage();
                        var stream = FileManager.GetFileStreamForWrite(arg.DesFileName);
                        
                        //PngEncoder encoder = new PngEncoder();
                        //encoder.IsWritingGamma = false;
                        //encoder.IsWritingUncompressed = false;
                        //encoder.Encode(image, stream);
                        

                        PNGWriter.WritePNG(wbmp, stream);
                        //debug
                        //stream.Position = 0;
                        //byte[] imgbyte = new byte[stream.Length];
                        //stream.Read(imgbyte, 0, (int)stream.Length);
                        //string pngcontent = Convert.ToBase64String(imgbyte);
                        //sendLogToServer(pngcontent);
                        stream.Close();
                        _stream_load_image.ErrorCode = 0;
                        _stream_load_image.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    }
                    catch (Exception ex)
                    {
                        _stream_load_image.ErrorCode = 1;
                        _stream_load_image.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    }
                }
                else
                {
                    _stream_load_image.ErrorCode = 1;
                    _stream_load_image.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                }
                _stream_load_image = null;
            }
        }
        #endregion
        #region MAIL
        public void sendMail(string title, string message)
        {
            EmailComposeTask emailComposeTask = new EmailComposeTask();
            emailComposeTask.Subject = title;
            emailComposeTask.Body = message;
            emailComposeTask.Show();
        }
        #endregion
        #region SMS
        public void sendSMS(string title, string message)
        {
            SmsComposeTask smsComposeTask = new SmsComposeTask();
            smsComposeTask.Body = message; // the string containing the sms body
            smsComposeTask.Show(); // this will invoke the native sms edtior
        }
        #endregion

        #region ADMOB
        List<InterstitialAd> _list_admob_interstitial = new List<InterstitialAd>();
        private InterstitialAd AdmobInterstitialAd;
        ServerStream StreamAdmobInterstitial = null;
        
        public void AdmobPrepare(ServerStream svstream)
        {
            if (StreamAdmobInterstitial == null)
            {
                StreamAdmobInterstitial = svstream;
                Deployment.Current.Dispatcher.BeginInvoke(() =>
                {
                    StreamBuffer buff = new StreamBuffer(StreamAdmobInterstitial.PostData);
                    int auto_show = buff.readInt16();
                    string adid = buff.readSStr();
                    //"ca-app-pub-6305464951596521/9662563899"
                    AdmobInterstitialAd = new InterstitialAd(adid);
                    _list_admob_interstitial.Add(AdmobInterstitialAd);
                    // NOTE: You can edit the event handler to do something custom here. Once the
                    // interstitial is received it can be shown whenever you want.
                    AdmobInterstitialAd.ReceivedAd += OnAdReceived;
                    AdmobInterstitialAd.FailedToReceiveAd += OnFailedToReceiveAd;
                    AdmobInterstitialAd.DismissingOverlay += OnDismissingOverlay;
                    AdRequest adRequest = new AdRequest();
#if DEBUG
                    adRequest.ForceTesting = true;
#endif
                    AdmobInterstitialAd.LoadAd(adRequest);
                });
            }
        }

        private void OnDismissingOverlay(object sender, AdEventArgs e)
        {
            _list_admob_interstitial.Remove(sender as InterstitialAd);
            if (StreamAdmobInterstitial != null)
            {
                StreamAdmobInterstitial.ErrorCode = 1;
                StreamAdmobInterstitial.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                StreamAdmobInterstitial = null;
            }
        }

        private void OnFailedToReceiveAd(object sender, AdErrorEventArgs e)
        {
            _list_admob_interstitial.Remove(sender as InterstitialAd);
            if (StreamAdmobInterstitial != null)
            {
                StreamAdmobInterstitial.ErrorCode = 1;
                StreamAdmobInterstitial.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                StreamAdmobInterstitial = null;
            }
        }

        private void OnAdReceived(object sender, AdEventArgs e)
        {
            if (StreamAdmobInterstitial != null)
            {
                StreamBuffer buff = new StreamBuffer(StreamAdmobInterstitial.PostData);
                int auto_show = buff.readInt16();
                if (auto_show == 1)//auto show = true
                {
                    AdmobShow();
                }
                StreamAdmobInterstitial.ErrorCode = 0;
                StreamAdmobInterstitial.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                StreamAdmobInterstitial = null;
            }
        }
        public void AdmobShow()
        {
            if (AdmobInterstitialAd != null)
            {
                try
                {
                    //Deployment.Current.Dispatcher.BeginInvoke(() =>
                    //{
                        AdmobInterstitialAd.ShowAd();
                    //});
                }
                catch (Exception ex)
                {

                }
            }
        }
        #endregion ADMOB

        #region SHARETASK
        public void ShareLinkTask (string title, string url, string message)
        {
            ShareLinkTask shareLinkTask = new ShareLinkTask();

            shareLinkTask.Title = title;
            shareLinkTask.LinkUri = new Uri(url, UriKind.Absolute);
            shareLinkTask.Message = message;

            shareLinkTask.Show();
        }

        public void cameraCaptureTask_Completed(object sender, PhotoResult e)
        {
            if (e.TaskResult == TaskResult.OK)
            {
                ShareMediaTask(e.OriginalFileName);
            }
        }

        public void ShareMediaTask(string path)
        {
            ShareMediaTask shareMediaTask = new ShareMediaTask();
            shareMediaTask.FilePath = path;
            //Type t = shareMediaTask.GetType();
            shareMediaTask.Show();
            _stream_share_media.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
        }
        #endregion
        public static void GetUserAgent(Action<string> callback)
        {
            string html = @"
                        <html>
                            <head>
                                <script language='javascript' type='text/javascript'>
                                    function notifyUA() {
                                        window.external.notify(navigator.userAgent);
                                    }
                                </script>
                            </head>
                            <body onload=""notifyUA();""></body>
                        </html>";
            WebBrowser browser = new WebBrowser();
            browser.IsScriptEnabled = true;
            browser.Visibility = Visibility.Collapsed;
            browser.Loaded += (sender, br_args) => browser.NavigateToString(html);
            browser.ScriptNotify += (sender, br_args) =>
            {
                string userAgent = br_args.Value;
                AppObject.RootLayout.Children.Remove(browser);
                callback(userAgent);
            };
            AppObject.RootLayout.Children.Add(browser);
        }
        public void sendLogToServer(string content)
        {
            WebClient wc = new WebClient();
            wc.UploadStringAsync(new Uri("http://192.168.1.62/test.php"), content);
        }
    }
}
