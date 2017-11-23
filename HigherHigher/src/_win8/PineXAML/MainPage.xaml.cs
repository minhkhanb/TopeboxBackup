using Pine;
using PineShap;
using PineShap.FacebookUtils;
using PineShap.Actions;
using PineShap.TwitterUtils;
using PineShap.Object;
using PineShap.Tools;
using PineShap.Dialogs;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using System.Net;
using Windows.ApplicationModel.Store;
using System.Threading.Tasks;
using Windows.System;
using Windows.ApplicationModel;
using Windows.UI.ApplicationSettings;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace PineXAML
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            try
            {
                this.InitializeComponent();
                SettingsPane.GetForCurrentView().CommandsRequested += SettingsCommandsRequested;
                //Init Environment
                FacebookClient.Instance.OnUserInfomationUpdate += delegate_fb_update_infomation;
                PlayerInfomation.LoginedFacebook = false;

                //case have fb user id
                if (FacebookClient.Instance.UserId != null && FacebookClient.Instance.UserId.Length > 0)
                {
                    PlayerInfomation.FacebookId = FacebookClient.Instance.UserId;
                    UInt64 id = UInt64.Parse(FacebookClient.Instance.UserId);
                    id = 3000000000000000000 + id;
                    PlayerInfomation.UserId = id.ToString();
                    if (FacebookClient.Instance.Token != null && FacebookClient.Instance.Token.Length > 0)
                    {
                        PlayerInfomation.LoginedFacebook = true;
                    }
                }
                //case have no user id
                else
                {
                    PlayerInfomation.FacebookId = "";
                    PlayerInfomation.UserId = "";
                }

                if (FacebookClient.Instance.UserName != null & FacebookClient.Instance.UserName.Length > 0)
                {
                    PlayerInfomation.FacebookUserName = FacebookClient.Instance.UserName;
                }
                else
                {
                    PlayerInfomation.FacebookUserName = "";
                }

                //DeviceNetworkInformation.NetworkAvailabilityChanged += new EventHandler<NetworkNotificationEventArgs>(handler_network_ChangeDetected);
                DeviceInfomation.IsNetworkAvailable = System.Net.NetworkInformation.NetworkInterface.GetIsNetworkAvailable();
                ////foreach (Uri uri in AppObject.GameInfo.UriRequire)
                ////{
                ////    updateNetworkStatus(uri);
                ////}
                DeviceInfomation.RegionFormatShort = System.Globalization.RegionInfo.CurrentRegion.TwoLetterISORegionName.ToUpper();
                DeviceInfomation.RegionFormat = System.Globalization.RegionInfo.CurrentRegion.EnglishName;
                AppObject.RootLayout = LayoutRoot;
                ActionHelper.registeUIThread(Windows.UI.Core.CoreWindow.GetForCurrentThread().Dispatcher);
            }
            catch (Exception ex)
            {
                    Debug.WriteLine(ex.StackTrace);
            }
        }

        private void SettingsCommandsRequested(SettingsPane sender, SettingsPaneCommandsRequestedEventArgs args)
        {
            var privacyStatement = new SettingsCommand("privacy", "Privacy Statement",
                    async x => await Launcher.LaunchUriAsync(new Uri("http://www.pine-entertainment.com/policy.htm")));

            args.Request.ApplicationCommands.Clear();
            args.Request.ApplicationCommands.Add(privacyStatement);
        }
        void delegate_fb_update_infomation(object sender, FacebookUpdateInfomationArgs e)
        {
            PlayerInfomation.FacebookId = FacebookClient.Instance.UserId;
            PlayerInfomation.FacebookUserName = FacebookClient.Instance.UserName;
            PlayerInfomation.LoginedFacebook = true;
            UInt64 id = UInt64.Parse(FacebookClient.Instance.UserId);
            id = 3000000000000000000 + id;
            PlayerInfomation.UserId = id.ToString();
            ////ApplicationInfomation.onLoginFacebook();
        }
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            // Start rendering animated DirectX content on a background thread, which reduces load on the UI thread
            // and can help keep the app responsive to user input.
            DirectXPanel1.OnRequest += DirectXPanel1_OnRequest;
            DirectXPanel1.StartRenderLoop();
        }
        ServerStream _stream_loadFbFriend = null;
        ServerStream _stream_loginAccount = null;
        ServerStream _waiting_browser = null;
        ServerStream _waiting_rating = null;
        ServerStream _waiting_inapp = null;
        Dictionary<PineShap.Actions.Action, ServerStream> _stream_wating = new Dictionary<PineShap.Actions.Action, ServerStream>();
        void DirectXPanel1_OnRequest(uint constructor_id)
        {
            ConnectConstructor constructor = (ConnectConstructor)constructor_id;
            switch (constructor)
            {
                case ConnectConstructor.MAIL_SEND:
                    ////Dispatcher.BeginInvoke(() => mail_send(constructor_id));
                    break;
                case ConnectConstructor.SMS_SEND:
                    ////Dispatcher.BeginInvoke(() => sms_send(constructor_id));
                    break;
                case ConnectConstructor.NOTIFICATION_RUN:
                    ////runBackgroundTask();
                    break;
                case ConnectConstructor.NOTIFICATION_STOP:
                    ////removeBackgroundTask(TaskName);
                    break;
                case ConnectConstructor.GAME_EXIT:
                    ////Dispatcher.BeginInvoke(() => close_app(false));
                    break;
                case ConnectConstructor.SERVER_CHECK_AND_RUN:
                    //Dispatcher.BeginInvoke(() => {
                        ServerStreamManager sv = ServerStreamManager.getInstance();
                        for (int i = 0; i < sv.StreamNum; i++) {
                            if (sv.Streams[i].Status == (int)ServerStreamStatus.STREAM_STATUS_WAITING) {
                                sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_REQUESTING;
                                //sv.Streams[i].ResponseData = null;
                                ServerStreamConstructor svcode = (ServerStreamConstructor)sv.Streams[i].RequestCode;
                                switch (svcode) {
                                    case ServerStreamConstructor.REQUEST_CODE_GAME_EXIT:
                                        {
                                            StreamBuffer buf = new StreamBuffer(sv.Streams[i].PostData);
                                            int sudden = buf.readInt32();
                                            ////Dispatcher.BeginInvoke(() => close_app(sudden != 0));
                                        }
                                        break;
                                    case ServerStreamConstructor.REQUEST_CODE_FB_LOGIN: {
#if BUILD_SUPPORT
                                        ActionSupportAccountAskArgs arg = new ActionSupportAccountAskArgs();
                                        arg.ParentLayout = AppObject.RootLayout;
                                        ActionSupportAccountAsk support_act = new ActionSupportAccountAsk();
                                        _stream_wating.Add(support_act, sv.Streams[i]);
                                        support_act.OnDone += support_act_OnDone;
                                        support_act.OnCancel += support_act_OnCancel;
                                        support_act.Do(arg);
#else
                                            if (sv.Streams[i].PostData != null) {
                                                if (AppObject.GameInfo.IsRequireAccount)
                                                {
                                                    if (FacebookClient.Instance.RequestLogin == 0)
                                                    {
                                                        action_account_ask_stream_do(sv.Streams[i]);
                                                    }
                                                    else
                                                    {
                                                        //check session here
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
                                                        FacebookManager.login(sv.Streams[i], null, null, null);
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
                                            } else {
                                                FacebookManager.login(sv.Streams[i], null, null, null);
                                            }
#endif
                                        } break;
                                    case ServerStreamConstructor.REQUEST_CODE_FB_GET_AVATAR: {
                                            action_fbavar_stream_do(sv.Streams[i]);
                                        } break;
                                    case ServerStreamConstructor.REQUEST_CODE_OPEN_BROWSER: {
                                        
                                        if (_waiting_browser == null)
                                        {
                                            _waiting_browser = sv.Streams[i];
                                            var dispatcher = ActionHelper.getCurrentUIThread();
                                            dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
                                            {
                                                openBrowser();
                            
                                            });
                                        }
                                        else
                                        {
                                            sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                        }
                                        
                                        }break;
                                    case ServerStreamConstructor.REQUEST_CODE_RATING: {
                                        if (_waiting_rating == null)
                                        {
                                            _waiting_rating = sv.Streams[i];
                                            var dispatcher = ActionHelper.getCurrentUIThread();
                                            dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
                                            {
                                                rating();
                                            });
                                            
                                        }
                                        else
                                        {
                                            sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                        }
                                        }break;
                                    case ServerStreamConstructor.REQUEST_CODE_PURCHASE: {
                                        if (_waiting_inapp == null)
                                        {
                                             var dispatcher = ActionHelper.getCurrentUIThread();
                                             _waiting_inapp = sv.Streams[i];
                                             dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
                                             {
                                                 buyItem();
                                             });
                                        }
                                        else
                                        {
                                            sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                        }
                                        }
                                        break;
                                    case ServerStreamConstructor.REQUEST_CODE_PURCHASE_GETLIST: {
                                            listInapp(sv.Streams[i]);
                                        }break;
                                    case ServerStreamConstructor.REQUEST_CODE_FB_FEED_ME: {
                                        if (FacebookClient.Instance.Token.Length > 0 && FbFeedStreamWaiting == null)
                                        {
                                            StreamBuffer buf = new StreamBuffer(sv.Streams[i].PostData);
                                            string message = buf.readSStr();
                                            FbFeedStreamWaiting = sv.Streams[i];
                                            ActionFacebookFeedArgs arg = new ActionFacebookFeedArgs();
                                            arg.Title = AppObject.GameInfo.FacebookFeedTitle;
                                            arg.Message = message;
                                            arg.PageLink = AppObject.AppLink;
                                            arg.IconLink = AppObject.GameInfo.FacebookFeedImageUrl;
                                            FacebookManager.feed(arg, null, action_fbfeed_stream_onDone, action_fbfeed_stream_onCancel);
                                        }
                                        else
                                        {
                                            sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                        }
                                        }break;
                                    case ServerStreamConstructor.REQUEST_CODE_TW_LOGIN: {
                                        ////TwitterManager.login(sv.Streams[i],null,null,null);
                                        }break;
                                    case ServerStreamConstructor.REQUEST_CODE_TW_FEED: {
                                        ////TwitterManager.twfeed(sv.Streams[i]);
                                        }break;
                                    case ServerStreamConstructor.REQUEST_CODE_FB_GET_FRIEND: {
                                        if (_stream_loadFbFriend == null)
                                        {
                                            _stream_loadFbFriend = sv.Streams[i];
                                            FacebookManager.getFriends(null, action_fbfriend_stream_OnDone, action_fbfriend_stream_OnCancel);
                                        }
                                        else
                                        {
                                            sv.Streams[i].Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                                        }
                                        }break;
                                    default:
                                        AppObject.ClientSvc.process(new ClientServiceRequest((int)sv.Streams[i].RequestCode), sv.Streams[i]);
                                        break;
                                }
                            }
                        }
                    //});
                    break;
            }
        }
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
                //MessageBox.Show(ex.Message);
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }
        private async void buyItem()
        {
            if (_waiting_inapp != null)
            {
                String id = (new StreamBuffer(_waiting_inapp.PostData)).readSStr();
                try
                {
                    PurchaseResults purchaseResults = await CurrentApp.RequestProductPurchaseAsync(id);
                    switch (purchaseResults.Status)
                    {
                        case ProductPurchaseStatus.Succeeded:
                            //GrantFeatureLocally("product1", purchaseResults.TransactionId);
                            FulfillProduct("product1", purchaseResults.TransactionId);
                            _waiting_inapp.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                            _waiting_inapp = null;
                            break;
                        case ProductPurchaseStatus.NotFulfilled:
                            //if (!IsLocallyFulfilled("product1", purchaseResults.TransactionId))
                            //{
                            //GrantFeatureLocally("product1", purchaseResults.TransactionId);
                            //}
                            FulfillProduct("product1", purchaseResults.TransactionId);
                            _waiting_inapp.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                            _waiting_inapp = null;
                            break;
                        case ProductPurchaseStatus.NotPurchased:
                            _waiting_inapp.ErrorCode = 1;
                            _waiting_inapp.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                            
                            _waiting_inapp = null;
                            break;
                        default:
                            _waiting_inapp.ErrorCode = 1;
                            _waiting_inapp.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                           
                            _waiting_inapp = null;
                            break;
                    }
                }
                catch (Exception ex)
                {
                    _waiting_inapp.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    _waiting_inapp = null;
                }
            }
            //MessageBox.Show(recept);
        }
        private async void FulfillProduct(string productId, Guid transactionId)
        {
            try
            {
                FulfillmentResult result = await CurrentAppSimulator.ReportConsumableFulfillmentAsync(productId, transactionId);
                switch (result)
                {
                    case FulfillmentResult.Succeeded:
                        //Log("You bought and fulfilled product 1.", NotifyType.StatusMessage);
                        break;
                    case FulfillmentResult.NothingToFulfill:
                        //Log("There is no purchased product 1 to fulfill.", NotifyType.StatusMessage);
                        break;
                    case FulfillmentResult.PurchasePending:
                        //Log("You bought product 1. The purchase is pending so we cannot fulfill the product.", NotifyType.StatusMessage);
                        break;
                    case FulfillmentResult.PurchaseReverted:
                        //Log("You bought product 1. But your purchase has been reverted.", NotifyType.StatusMessage);
                        // Since the user's purchase was revoked, they got their money back.
                        // You may want to revoke the user's access to the consumable content that was granted.
                        break;
                    case FulfillmentResult.ServerError:
                        //Log("You bought product 1. There was an error when fulfilling.", NotifyType.StatusMessage);
                        break;
                }
            }
            catch (Exception)
            {
                //Log("You bought Product 1. There was an error when fulfilling.", NotifyType.ErrorMessage);
            }
        }
        #region ACCOUNT ASK
        void action_account_ask_stream_do(ServerStream stream)
        {
            PineShap.Actions.ActionAccountAskArg arg = new PineShap.Actions.ActionAccountAskArg(LayoutRoot);
            arg.IsRequire = AppObject.GameInfo.IsRequireAccount;
            ActionAccountAsk act = new ActionAccountAsk();
            _stream_wating.Add(act, stream);
            try
            {
                act.OnDone += action_account_ask_stream_OnDone;
                act.OnCancel += action_account_ask_stream_OnCancel;
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_REQUESTING;
                act.Do(arg);
                AppObject.PopupOfAction.Add(new ActionArgLinker(act, arg));
            }
            catch (Exception ex)
            {
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }

        private void action_account_ask_stream_OnCancel(object sender, ActionArgs e)
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
                ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_wating.Remove((PineShap.Actions.Action)sender);

            }
            catch (Exception ex)
            {
            }
        }

        private void action_account_ask_stream_OnDone(object sender, ActionArgs e)
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
                if (arg.Result != null)
                {
                    ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                    _stream_wating.Remove((PineShap.Actions.Action)sender);
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
                    ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    _stream_wating.Remove((PineShap.Actions.Action)sender);
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
            PineShap.Actions.ActionFacebookLoginArgs arg = new PineShap.Actions.ActionFacebookLoginArgs(LayoutRoot);
            PineShap.Actions.ActionFacebookLogin action_fblogin = new PineShap.Actions.ActionFacebookLogin();
            _stream_wating.Add(action_fblogin, stream);
            try
            {
                action_fblogin.OnDone += action_fblogin_stream_OnDone;
                action_fblogin.OnCancel += action_fblogin_stream_OnCancel;
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_REQUESTING;
                action_fblogin.Do(arg);
                AppObject.PopupOfAction.Add(new PineShap.Actions.ActionArgLinker(action_fblogin, arg));
            }
            catch (Exception ex)
            {
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }
        void action_fblogin_stream_OnCancel(object sender, PineShap.Actions.ActionArgs e)
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
                ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                _stream_wating.Remove((PineShap.Actions.Action)sender);
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
            foreach (PineShap.Actions.ActionArgLinker arglink in AppObject.PopupOfAction)
            {
                if (arglink.AttachArg == e)
                {
                    AppObject.PopupOfAction.Remove(arglink);
                    break;
                }
            }
            ////Dispatcher.BeginInvoke(() => 
                close_app(false);
                //);
        }

        void requireDialog_OnComplete(object sender, AccountRequireDialogArgs a)
        {
            AccountRequireDialog dialog = sender as AccountRequireDialog;
            if (dialog != null)
            {
                //Deployment.Current.Dispatcher.BeginInvoke(() => 
                    LayoutRoot.Children.Remove(dialog.Dialog);//);
            }
            if (_stream_loginAccount != null)
            {
                action_fblogin_stream_do(_stream_loginAccount);
                _stream_loginAccount = null;
            }
        }
        void action_fblogin_stream_OnDone(object sender, PineShap.Actions.ActionArgs e)
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
                PineShap.Actions.ActionFacebookLoginArgs fbarg = e as PineShap.Actions.ActionFacebookLoginArgs;
                if (fbarg != null && (fbarg.IsCancelByLogedin || (fbarg.Result != null && fbarg.Result.IsSuccess)))
                {
                    if (AppObject.GameInfo.IsRequireAccount)
                    {
                        FacebookClient.Instance.RequestLogin = 1;
                        FacebookClient.Instance.SaveUserData();
                    }
                    if (fbarg.IsCancelByLogedin)
                    {
                        ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                        svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    }
                    else
                    {
                        FacebookClient.Instance.Token = fbarg.Result.Token;
                        FacebookClient.Instance.TokenExprite = fbarg.Result.TokenExprite;
                        FacebookClient.Instance.SaveUserData();
                        ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                        //svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                        action_fbinfo_stream_do(svstream);
                        //_stream_wating.Remove((Actions.Action)sender);
                    }
                }
                else
                {
                    ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                    _stream_wating.Remove((PineShap.Actions.Action)sender);
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
            PineShap.Actions.ActionFacebookGetInfomationArgs arg = new PineShap.Actions.ActionFacebookGetInfomationArgs();
            PineShap.Actions.ActionFacebookGetInfomation action_fbinfo = new PineShap.Actions.ActionFacebookGetInfomation();
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
        void action_fbinfo_stream_onCancel(object sender, PineShap.Actions.ActionArgs e)
        {
            try
            {
                ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_wating.Remove((PineShap.Actions.Action)sender);
            }
            catch (Exception ex)
            {
            }
        }
        void action_fbinfo_stream_onDone(object sender, PineShap.Actions.ActionArgs e)
        {
            try
            {
                PineShap.Actions.ActionFacebookGetInfomationArgs fbarg = e as PineShap.Actions.ActionFacebookGetInfomationArgs;
                if (fbarg != null)
                {
                    ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                    PlayerInfomation.FacebookId = FacebookClient.Instance.UserId;
                    PlayerInfomation.FacebookUserName = FacebookClient.Instance.UserName;
                    UInt64 id = UInt64.Parse(FacebookClient.Instance.UserId);
                    id = 3000000000000000000 + id;
                    PlayerInfomation.UserId = id.ToString();

                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    _stream_wating.Remove((PineShap.Actions.Action)sender);
                }
                else
                {
                    ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    _stream_wating.Remove((PineShap.Actions.Action)sender);
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
                PineShap.Actions.ActionFacebookGetAvatarArgs arg = new PineShap.Actions.ActionFacebookGetAvatarArgs();
                PineShap.Actions.ActionFacebookGetAvatar action_fbavar = new PineShap.Actions.ActionFacebookGetAvatar();
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
        void action_fbavar_stream_OnCancel(object sender, PineShap.Actions.ActionArgs e)
        {
            try
            {
                ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_wating.Remove((PineShap.Actions.Action)sender);
            }
            catch (Exception ex)
            {
            }
        }
        void action_fbavar_stream_OnDone(object sender, PineShap.Actions.ActionArgs e)
        {
            try
            {
                PineShap.Actions.ActionFacebookGetAvatarArgs fbarg = e as PineShap.Actions.ActionFacebookGetAvatarArgs;
                if (fbarg != null)
                {
                    ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    _stream_wating.Remove((PineShap.Actions.Action)sender);
                }
                else
                {
                    ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    _stream_wating.Remove((PineShap.Actions.Action)sender);
                }
            }
            catch (Exception ex)
            {
            }
        }
        private void action_fbfeed_stream_onCancel(object sender, ActionArgs e)
        {
            if (FbFeedStreamWaiting != null)
            {
                FbFeedStreamWaiting.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                FbFeedStreamWaiting = null;
            }
        }

        private void action_fbfeed_stream_onDone(object sender, ActionArgs e)
        {
            if (FbFeedStreamWaiting != null)
            {
                FbFeedStreamWaiting.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                FbFeedStreamWaiting = null;
            }
        }
        ServerStream FbFeedStreamWaiting = null;
        private void action_fbfriend_stream_OnCancel(object sender, PineShap.Actions.ActionArgs e)
        {
            if (_stream_loadFbFriend != null)
            {
                _stream_loadFbFriend.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_loadFbFriend = null;
            }
        }
        private void action_fbfriend_stream_OnDone(object sender, PineShap.Actions.ActionArgs e)
        {
            try
            {
                if (_stream_loadFbFriend != null)
                {
                    PineShap.Actions.ActionFacebookGetFriendsArg fbarg = e as PineShap.Actions.ActionFacebookGetFriendsArg;
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
            PineShap.Actions.ActionTwitterLoginArgs arg = new PineShap.Actions.ActionTwitterLoginArgs(LayoutRoot);
            PineShap.Actions.ActionTwitterLogin action_twlogin = new PineShap.Actions.ActionTwitterLogin();
            _stream_wating.Add(action_twlogin, stream);
            try
            {
                action_twlogin.OnDone += action_twitter_login_OnDone;
                action_twlogin.OnCancel += action_twitter_login_OnCancel;
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_REQUESTING;
                AppObject.PopupOfAction.Add(new PineShap.Actions.ActionArgLinker(action_twlogin, arg));
                action_twlogin.Do(arg);
            }
            catch (Exception ex)
            {
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }
        private void action_twitter_login_OnDone(object sender, PineShap.Actions.ActionArgs e)
        {
            try
            {
                PineShap.Actions.ActionTwitterLoginArgs twarg = e as PineShap.Actions.ActionTwitterLoginArgs;
                if (twarg != null && (twarg.IsCancelByLogedin || (twarg.Result != null && twarg.Result.IsSuccess)))
                {
                    if (twarg.IsCancelByLogedin)
                    {
                        ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                        svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                        _stream_wating.Remove((PineShap.Actions.Action)sender);
                    }
                    else
                    {
                        ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                        svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                        _stream_wating.Remove((PineShap.Actions.Action)sender);
                    }
                }
                else
                {
                    ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    _stream_wating.Remove((PineShap.Actions.Action)sender);
                }
                foreach (PineShap.Actions.ActionArgLinker arglink in AppObject.PopupOfAction)
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
        private void action_twitter_login_OnCancel(object sender, PineShap.Actions.ActionArgs e)
        {
            try
            {
                ServerStream svstream = _stream_wating[(PineShap.Actions.Action)sender];
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_wating.Remove((PineShap.Actions.Action)sender);
                foreach (PineShap.Actions.ActionArgLinker arglink in AppObject.PopupOfAction)
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
                ////TwitterClient.PostTweet(message + " " + AppObject.AppLink, action_twitter_feed_callback);
            }
            else
            {
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }

        /*private void action_twitter_feed_callback(RestRequest request, RestResponse response, object userState)
        {
            if (TWFeedStreamWaiting != null)
            {
                ////Deployment.Current.Dispatcher.BeginInvoke(() =>
                ////{
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
                ////});
            }
        }*/

        #endregion
        #region CLOSE APP ASK
        PineShap.Dialogs.CloseAppAskDialog CloseDialog = null;
        void close_app(bool is_sudden)
        {
            if (is_sudden)
            {
                ////PhoneDirect3DXamlAppComponent.ApplicationInfomation.onClosing();
                ////Application.Current.Terminate();
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
            ////Dispatcher.BeginInvoke(() =>
            ////{
                try
                {
                    LayoutRoot.Children.Remove(((PineShap.Dialogs.CloseAppAskDialog)sender).Dialog);
                }
                catch (Exception ex)
                {
                }
                if (a.Result == CloseAppAskDialogResult.OK)
                {

                    ////PhoneDirect3DXamlAppComponent.ApplicationInfomation.onClosing();
                    ////Application.Current.Terminate();

                }
            ////});
        }
        #endregion
        /// <summary>
        /// Invoked when this page will no longer be displayed in a Frame.
        /// </summary>
        /// <param name="e"></param>
        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            // Stop rendering DirectX content when it will no longer be on screen.
            DirectXPanel1.StopRenderLoop();
        }
        async void openBrowser()
        {
            if (_waiting_browser != null)
            {
                StreamBuffer buff = new StreamBuffer(_waiting_browser.PostData);
                String url = buff.readSStr();
                var options = new Windows.System.LauncherOptions();
                options.DisplayApplicationPicker = true;
                options.TreatAsUntrusted = true;
                try
                {
                    await Launcher.LaunchUriAsync(new Uri(url), options);
                    _waiting_browser.Status = (int)ServerStreamStatus.STREAM_STATUS_FREE;
                    _waiting_browser = null;
                }
                catch (Exception ex)
                {
                    _waiting_browser.Status = (int)ServerStreamStatus.STREAM_STATUS_FREE;
                    _waiting_browser = null;
                }                
            }
        }
        
        async void  rating()
        {
            if (_waiting_rating != null)
            {
                StreamBuffer buff = new StreamBuffer(_waiting_rating.PostData);
                int is_opendialog = buff.readInt32();
                if (is_opendialog != 0)
                {
                    String title = buff.readSStr();
                    String message = buff.readSStr();

                    Uri uri = new Uri("ms-windows-store:REVIEW?PFN=" + Package.Current.Id.FamilyName);
                    try
                    {
                        bool success = await Launcher.LaunchUriAsync(uri);
                    }
                    catch (Exception ex)
                    {
                        String mess = ex.Message;
                    }
                    ////if (MessageBox.Show(message, title, MessageBoxButton.OKCancel) == MessageBoxResult.OK)
                    {
                        ////    MarketplaceReviewTask marketplaceReviewTask = new MarketplaceReviewTask();
                        ////    marketplaceReviewTask.Show();
                    }
                }
                else
                {
                    ///MarketplaceReviewTask marketplaceReviewTask = new MarketplaceReviewTask();
                    ////marketplaceReviewTask.Show();
                }
                _waiting_rating.Status = (int)ServerStreamStatus.STREAM_STATUS_FREE;
                _waiting_rating = null;
            }
        }
    }
}
