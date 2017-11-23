using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.IO.IsolatedStorage;
using System.Windows;
using Pine;
using System.Diagnostics;
using Newtonsoft.Json;
using Pine.FacebookUtils;
using System.Net.Http;

namespace FacebookUtils{
    public class FacebookAppInfomation
    {
        public string AppId;
        public string AppSecret;
        public string RedirectLink;
        public string RequirePermission;
    }
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
        public string AvatarUrl = "";
        public int CancelNum = 0;
        private static FacebookClient instance;
        static public Transaction CurrentTransaction = null;
        private static readonly IsolatedStorageSettings _appSettings = IsolatedStorageSettings.ApplicationSettings;

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
        public virtual String getLoginUrl(FacebookAppInfomation appinfo){
            return "https://m.facebook.com/dialog/oauth?client_id=" + appinfo.AppId + "&redirect_uri=" + appinfo.RedirectLink + "&scope=" + appinfo.RequirePermission + "&display=touch" + "&skip_api_login=1";
        }
        public virtual String getFeedUrl(FacebookAppInfomation appinfo)
        {
            return "https://m.facebook.com/dialog/feed?app_id=" + appinfo.AppId + "&display=touch&link=https://pine-entertainment.com&redirect_uri=" + appinfo.RedirectLink;
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
        public String getUserAvatarUrlNotRedirect()
        {
            return "https://graph.facebook.com/" + FacebookClient.Instance.UserId + "/picture?redirect=false&width=64&height=64";
        }
        public virtual String getAccessTokenRequestUrl(FacebookAppInfomation appinfo, string code){
            return "https://graph.facebook.com/oauth/access_token?client_id=" + appinfo.AppId + "&redirect_uri=" + appinfo.RedirectLink + "&client_secret=" + appinfo.AppSecret + "&code=" + code;
        }

        public virtual String getAccessTokenExchangeUrl(FacebookAppInfomation appinfo, string accessToken)
        {
            return "https://graph.facebook.com/oauth/access_token?client_id=" + appinfo.AppId + "&client_secret=" + appinfo.AppSecret + "&grant_type=fb_exchange_token&fb_exchange_token=" + Token;
        }
        public void PostMessageOnWall(string message, UploadStringCompletedEventHandler handler){
            WebClient client = new WebClient();
            client.UploadStringCompleted += handler;
            client.UploadStringAsync(new Uri("https://graph.facebook.com/me/feed"), "POST", "message=" + HttpUtility.UrlEncode(message) + "&access_token=" + FacebookClient.Instance.Token);
        }
        public async void PostMessageOnWall(string caption, string message, string link, string icon_url, UploadStringCompletedEventHandler handler){

            WebClient client = new WebClient();
            client.UploadStringCompleted += handler;
            client.UploadStringAsync(new Uri("https://graph.beta.facebook.com/me/links"), "POST", 
                "message=" + HttpUtility.UrlEncode(message) + "&access_token=" + FacebookClient.Instance.Token 
                + "&link="+link
                + "&picture=" + icon_url
                + "&caption=" + HttpUtility.UrlEncode(caption)
                );
            //client.UploadStringAsync(new Uri(url), "POST",
            //   ""
            //   );*/

            /*
            HttpClient http = new HttpClient();
            string url = "https://graph.facebook.com/me/links";//?" + "message=" + HttpUtility.UrlEncode(message) + "&link=" + HttpUtility.UrlEncode(link) + "&picture=" + HttpUtility.UrlEncode(icon_url) + "&caption=" + HttpUtility.UrlEncode(caption);
            
            string post_content = "message=" + HttpUtility.UrlEncode(message) + "&access_token=" + FacebookClient.Instance.Token
                + "&link=" + link
                //+ "&icon=" + icon_url
                + "&caption=" + HttpUtility.UrlEncode(caption);
            StringContent content = new StringContent(post_content);

            var response = await http.PostAsync(url, content);
            if (response.IsSuccessStatusCode)
            {
                Debug.WriteLine("success");
            }
            else
            {
                Debug.WriteLine("Fail");
            }*/
        }
        public string getPostPhotoUrl()
        {
            return "https://graph.facebook.com/" + UserId + "/photos";
        }
        public string getAppUserUrl() {

            string url = "https://graph.facebook.com/me/friends?" + "&access_token=" + FacebookClient.Instance.Token;

            //string fql = "SELECT uid FROM user WHERE uid IN (SELECT uid2 FROM friend WHERE uid1 = " + UserId + ") AND is_app_user = 1";
           // string url = "https://graph.facebook.com/fql?q=" + HttpUtility.UrlEncode(fql) + "&access_token=" + FacebookClient.Instance.Token;
            //string url = "https://graph.facebook.com/me/friends?access_token=" + Token;
            return url;
        }
        public void GetAppUser(DownloadStringCompletedEventHandler handler){
            string fql = "SELECT uid FROM user WHERE uid IN (SELECT uid2 FROM friend WHERE uid1 = "+ UserId+") AND is_app_user = 1";
            string test = "https://graph.facebook.com/fql?q=" + HttpUtility.UrlEncode(fql) + "&access_token=" + FacebookClient.Instance.Token;
            WebClient client = new WebClient();
            client.DownloadStringCompleted += handler;
            client.DownloadStringAsync(new Uri(test));
        }
        /*public void ExchangeAccessToken(UploadStringCompletedEventHandler handler){
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
        public void updateUserAvatarUrl(string avatar_url)
        {
            AvatarUrl = avatar_url;
            SaveUserData();
            if (OnUserInfomationUpdate != null) OnUserInfomationUpdate(this, new FacebookUpdateInfomationArgs(UserId, UserName));
        }
        public void cancelLogin(){
            Token = "";
            TokenExprite = "";
            CancelNum++;
            SaveUserData();
            if (OnCancelLogin != null) OnCancelLogin(this, null);
        }
        public void SaveUserData(){
            try{
                if (_appSettings.Contains("fb_userid")){
                    _appSettings["fb_userid"] = UserId;
                }else{
                    _appSettings.Add("fb_userid", UserId);
                }
                if (_appSettings.Contains("fb_username")){
                    _appSettings["fb_username"] = UserName;
                }else{
                    _appSettings.Add("fb_username", UserName);
                }
                if (_appSettings.Contains("fb_avatar_url"))
                {
                    _appSettings["fb_avatar_url"] = AvatarUrl;
                }
                else
                {
                    _appSettings.Add("fb_avatar_url", AvatarUrl);
                }
                if (_appSettings.Contains("fb_token"))
                {
                    _appSettings["fb_token"] = Token;
                }else{
                    _appSettings.Add("fb_token", Token);
                }
                if (_appSettings.Contains("fb_token_ex")){
                    _appSettings["fb_token_ex"] = TokenExprite;
                }else{
                    _appSettings.Add("fb_token_ex", TokenExprite);
                }
                if (_appSettings.Contains("fb_cancel_num")){
                    _appSettings["fb_cancel_num"] = CancelNum;
                }else{
                    _appSettings.Add("fb_cancel_num", CancelNum);
                }
                if (_appSettings.Contains("fb_request_login")){
                    _appSettings["fb_request_login"] = RequestLogin;
                }else{
                    _appSettings.Add("fb_request_login", RequestLogin);
                }
                _appSettings.Save();
            }catch (Exception ex){
                //MessageBox.Show("save false");
            }
        }
        void LoadUserData(){
            try{
                UserId = (string)_appSettings["fb_userid"];
            }catch (Exception ex){
                UserId = "";
            }
            try
            {
                UserName = (string)_appSettings["fb_username"];
            }
            catch (Exception ex)
            {
                UserName = "";
            }

            try{
                AvatarUrl = (string)_appSettings["fb_avatar_url"];
            }catch (Exception ex){
                AvatarUrl = "";
            }
           

            try{
                Token = (string)_appSettings["fb_token"];
            }catch (Exception ex){
                Token = "";
            }
            try{
                TokenExprite = (string)_appSettings["fb_token_ex"];
            }catch (Exception ex){
                TokenExprite = "";
            }
            try{
                CancelNum = (int)_appSettings["fb_cancel_num"];
            }catch (Exception ex){
                CancelNum = 0;
            }
            try
            {
                RequestLogin = (int)_appSettings["fb_request_login"];
            }catch (Exception ex){
                RequestLogin = 0;
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
