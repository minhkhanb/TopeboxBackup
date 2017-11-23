using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Pine;
using System.Diagnostics;
using Newtonsoft.Json;
using Windows.Storage; 

namespace PineShap.FacebookUtils{
    public class FacebookUpdateInfomationArgs:EventArgs{
        public string UserId = "";
        public string UserName = "";
        public FacebookUpdateInfomationArgs(string userid, string username){
            UserId = userid;
            UserName = username;
        }
    }

    public delegate void DelegateUpdateUserInfomation(object sender, FacebookUpdateInfomationArgs e);
    public delegate void DelegateFbCancelLogin(object sender, EventArgs e);
    public class FacebookClient{
        public event DelegateUpdateUserInfomation OnUserInfomationUpdate;
        public event DelegateFbCancelLogin OnCancelLogin;
        private CookieContainer _cookies = new CookieContainer();
        public CookieContainer Cookies{
            get { return _cookies; }
        }
        public string UserId = "";
        public string UserName = "";
        public string Token = "";
        public string TokenExprite = "";
        public int CancelNum = 0;
        private static FacebookClient instance;
        ////static public Transaction CurrentTransaction = null;
        private static readonly ApplicationDataContainer _appSettings = Windows.Storage.ApplicationData.Current.LocalSettings;

        private String scope = "publish_stream";
        public int RequestLogin = 0;

        private FacebookClient(){
            LoadUserData();
        }
        public static FacebookClient Instance{
            get{
                if (instance == null)instance = new FacebookClient();
                return instance;
            }
        }
        public virtual String getLoginUrl(){
            if (Token.Length > 0){
                return "https://m.facebook.com/dialog/oauth?client_id=" + AppObject.GameInfo.FacebookAppId + "&redirect_uri=https://www.facebook.com/connect/login_success.html&scope=" + scope + "&display=touch" + "&skip_api_login=1";
            }else{
                return "https://m.facebook.com/dialog/oauth?client_id=" + AppObject.GameInfo.FacebookAppId + "&redirect_uri=https://www.facebook.com/connect/login_success.html&scope=" + scope + "&display=touch" + "&skip_api_login=1";
            }
        }
        public virtual String getFeedUrl(){
            return "https://m.facebook.com/dialog/feed?app_id=" + AppObject.GameInfo.FacebookAppId + "&display=touch&link=https://pine-entertainment.com&redirect_uri=https://www.facebook.com/connect/login_success.html";
        }
        public virtual String getUserInfomationUrl(){
            return "https://graph.facebook.com/me?access_token=" + Token;
        }
        public virtual String getUserAvatarUrl() {
            return "https://graph.facebook.com/"+FacebookClient.Instance.UserId+"/picture?width=64&height=64";
        }
        public virtual String getUserAvatarUrl(UInt64 id) {
            return "https://graph.facebook.com/" + id + "/picture?width=64&height=64";
        }
        public virtual String getAccessTokenRequestUrl(string code){
            return "https://graph.facebook.com/oauth/access_token?client_id=" + AppObject.GameInfo.FacebookAppId + "&redirect_uri=https://www.facebook.com/connect/login_success.html&client_secret=" + AppObject.GameInfo.FacebookAppSecretKey + "&code=" + code;
            //return "https://graph.facebook.com/oauth/access_token?client_id=" + appId + "&client_secret=" + clientSecret + "&grant_type=client_credentials";
        }

        public virtual String getAccessTokenExchangeUrl(string accessToken){
            return "https://graph.facebook.com/oauth/access_token?client_id=" + AppObject.GameInfo.FacebookAppId + "&client_secret=" + AppObject.GameInfo.FacebookAppSecretKey + "&grant_type=fb_exchange_token&fb_exchange_token=" + Token;
        }
        
        public string getAppUserUrl() {
            string fql = "SELECT uid FROM user WHERE uid IN (SELECT uid2 FROM friend WHERE uid1 = " + UserId + ") AND is_app_user = 1";
            string url = "https://graph.facebook.com/fql?q=" + System.Uri.EscapeUriString(fql) + "&access_token=" + FacebookClient.Instance.Token;
            return url;
        }/*
        public void GetAppUser(DownloadStringCompletedEventHandler handler){
            string fql = "SELECT uid FROM user WHERE uid IN (SELECT uid2 FROM friend WHERE uid1 = "+ UserId+") AND is_app_user = 1";
            string test = "https://graph.facebook.com/fql?q=" + HttpUtility.UrlEncode(fql) + "&access_token=" + FacebookClient.Instance.Token;
            WebClient client = new WebClient();
            client.DownloadStringCompleted += handler;
            client.DownloadStringAsync(new Uri(test));
        }
        public void ExchangeAccessToken(UploadStringCompletedEventHandler handler){
            WebClient client = new WebClient();
            client.UploadStringCompleted += handler;
            client.UploadStringAsync(new Uri(getAccessTokenExchangeUrl(FacebookClient.Instance.Token)), "POST", "");
        }*/
        public void Logout(){
            UserId = "";
            Token = "";
            TokenExprite = "";
            SaveUserData();
        }
        public void updateUserInfomation(string userid, string username){
            UserId = userid;
            UserName = username;
            SaveUserData();
            if (OnUserInfomationUpdate != null) OnUserInfomationUpdate(this, new FacebookUpdateInfomationArgs(userid, username));
        }
        public void cancelLogin(){
            Token = "";
            TokenExprite = "";
            CancelNum++;
            SaveUserData();
            if (OnCancelLogin != null) OnCancelLogin(this, null);
        }
        public void SaveUserData(){
            if (!_appSettings.Containers.ContainsKey("AppSettings"))
            {
                _appSettings.CreateContainer("AppSettings", Windows.Storage.ApplicationDataCreateDisposition.Always);
            }
            if (_appSettings.Containers.ContainsKey("AppSettings"))
            {
                if (_appSettings.Containers["AppSettings"].Values.ContainsKey("fb_userid"))
                {
                    _appSettings.Containers["AppSettings"].Values["fb_userid"] = UserId;
                }
                else
                {
                    _appSettings.Containers["AppSettings"].Values.Add("fb_userid", UserId);
                }
                if (_appSettings.Containers["AppSettings"].Values.ContainsKey("fb_username"))
                {
                    _appSettings.Containers["AppSettings"].Values["fb_username"] = UserName;
                }
                else
                {
                    _appSettings.Containers["AppSettings"].Values.Add("fb_username", UserName);
                }
                if (_appSettings.Containers["AppSettings"].Values.ContainsKey("fb_token"))
                {
                    _appSettings.Containers["AppSettings"].Values["fb_token"] = Token;
                }
                else
                {
                    _appSettings.Containers["AppSettings"].Values.Add("fb_token", Token);
                }
                if (_appSettings.Containers["AppSettings"].Values.ContainsKey("fb_token_ex"))
                {
                    _appSettings.Containers["AppSettings"].Values["fb_token_ex"] = TokenExprite;
                }
                else
                {
                    _appSettings.Containers["AppSettings"].Values.Add("fb_token_ex", TokenExprite);
                }
                if (_appSettings.Containers["AppSettings"].Values.ContainsKey("fb_cancel_num"))
                {
                    _appSettings.Containers["AppSettings"].Values["fb_cancel_num"] = CancelNum;
                }
                else
                {
                    _appSettings.Containers["AppSettings"].Values.Add("fb_cancel_num", CancelNum);
                }
                if (_appSettings.Containers["AppSettings"].Values.ContainsKey("fb_request_login"))
                {
                    _appSettings.Containers["AppSettings"].Values["fb_request_login"] = RequestLogin;
                }
                else
                {
                    _appSettings.Containers["AppSettings"].Values.Add("fb_request_login", RequestLogin);
                }
            }
        }
        void LoadUserData(){
            if (_appSettings.Containers.ContainsKey("AppSettings"))
            {
                try
                {
                    UserId = (string)_appSettings.Containers["AppSettings"].Values["fb_userid"];
                }
                catch (Exception ex)
                {
                    UserId = "";
                }
                try
                {
                    UserName = (string)_appSettings.Containers["AppSettings"].Values["fb_username"];
                }
                catch (Exception ex)
                {
                    UserName = "";
                }
                try
                {
                    Token = (string)_appSettings.Containers["AppSettings"].Values["fb_token"];
                }
                catch (Exception ex)
                {
                    Token = "";
                }
                try
                {
                    TokenExprite = (string)_appSettings.Containers["AppSettings"].Values["fb_token_ex"];
                }
                catch (Exception ex)
                {
                    TokenExprite = "";
                }
                try
                {
                    CancelNum = (int)_appSettings.Containers["AppSettings"].Values["fb_cancel_num"];
                }
                catch (Exception ex)
                {
                    CancelNum = 0;
                }
                try
                {
                    RequestLogin = (int)_appSettings.Containers["AppSettings"].Values["fb_request_login"];
                }
                catch (Exception ex)
                {
                    RequestLogin = 0;
                }
            }
        }
        public bool IsUserLogedin(){
            return (UserId.Length > 0 && Token.Length > 0);
        }
        public bool IsHasToken() {
            return (Token != null && Token.Length > 0);
        }
    }
}
