using Hammock;
using Hammock.Authentication.OAuth;
using Hammock.Web;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Pine.Tools;
using TwitterUtils;

namespace Pine.TwitterUtils
{
    class TwitterClient
    {
        public const string KeyAcessToken = "tw_token";
        public const string KeyAcessTokenSecret = "tw_token_secret";
        public const string KeyUserName = "tw_username";
        public const string KeyUserID = "tw_userid";

        static public string OAuthTokenKey = string.Empty;
        static public string TokenSecret = string.Empty;
        static public string AccessToken = string.Empty;
        static public string AccessTokenSecret = string.Empty;
        static public string UserID = string.Empty;
        static public string UserScreenName = string.Empty;
        static public Transaction CurrentTransaction = null;
        static public bool isAlreadyLoggedIn()
        {
            AccessToken = KeyValuePairUtils.GetKeyValue<string>(KeyAcessToken);
            AccessTokenSecret = KeyValuePairUtils.GetKeyValue<string>(KeyAcessTokenSecret);
            UserScreenName = KeyValuePairUtils.GetKeyValue<string>(KeyUserName);
            UserID = KeyValuePairUtils.GetKeyValue<string>(KeyUserID);

            if (string.IsNullOrEmpty(AccessToken) || string.IsNullOrEmpty(AccessTokenSecret))
                return false;
            else
                return true;
        }
        static public void PostTweet(string message, RestCallback callback)
        {
            var credentials = new OAuthCredentials
            {
                Type = OAuthType.ProtectedResource,
                SignatureMethod = OAuthSignatureMethod.HmacSha1,
                ParameterHandling = OAuthParameterHandling.HttpAuthorizationHeader,
                ConsumerKey = AppObject.GameInfo.TwitterConsumerKey,
                ConsumerSecret = AppObject.GameInfo.TwitterConsumerSecret,
                Token = TwitterClient.AccessToken,
                TokenSecret = TwitterClient.AccessTokenSecret,
                Version = "1.0"
            };

            var restClient = new RestClient
            {
                Authority = "https://api.twitter.com",
                HasElevatedPermissions = true
            };

            var restRequest = new RestRequest
            {
                Credentials = credentials,
                Path = "/1.1/statuses/update.json",
                Method = WebMethod.Post
            };

            restRequest.AddParameter("status", message);
            restClient.BeginRequest(restRequest, callback);
        }
        static public void getFollower(RestCallback callback)
        {
            var credentials = new OAuthCredentials
            {
                Type = OAuthType.ProtectedResource,
                SignatureMethod = OAuthSignatureMethod.HmacSha1,
                ParameterHandling = OAuthParameterHandling.HttpAuthorizationHeader,
                ConsumerKey = AppObject.GameInfo.TwitterConsumerKey,
                ConsumerSecret = AppObject.GameInfo.TwitterConsumerSecret,
                Token = TwitterClient.AccessToken,
                TokenSecret = TwitterClient.AccessTokenSecret,
                Version = "1.0"
            };

            var restClient = new RestClient
            {
                Authority = "https://api.twitter.com",
                HasElevatedPermissions = true
            };

            var restRequest = new RestRequest
            {
                Credentials = credentials,
                Path = "/1.1/followers/ids.json?cursor=-1&user_id=" + TwitterClient.UserID,
                Method = WebMethod.Get
            };

            restClient.BeginRequest(restRequest, callback);
        }
        static private void PostTweetRequestCallback(RestRequest request, RestResponse response, object obj)
        {
            Deployment.Current.Dispatcher.BeginInvoke(() =>
            {
                if (response.StatusCode == HttpStatusCode.OK)
                {
                    MessageBox.Show("TWEET_POSTED_SUCCESSFULLY");
                }
                else if (response.StatusCode == HttpStatusCode.Forbidden)
                {
                    MessageBox.Show("TWEET_POST_ERR_UPDATE_LIMIT");
                }
                else
                {
                    MessageBox.Show("WEET_POST_ERR_FAILED");
                }
            });
            //var requestTokenQuery = OAuthUtil.GetRequestTokenQuery(); 
            //requestTokenQuery.RequestAsync(AppSettings.RequestTokenUri, null); 
            //requestTokenQuery.QueryResponse += new EventHandler<WebQueryResponseEventArgs>(requestTokenQuery_QueryResponse); 
        }
        static public void Logout()
        {
            KeyValuePairUtils.SetKeyValue<string>(TwitterClient.KeyAcessToken, string.Empty);
            KeyValuePairUtils.SetKeyValue<string>(TwitterClient.KeyAcessTokenSecret, string.Empty);
            KeyValuePairUtils.SetKeyValue<string>(TwitterClient.KeyUserName, string.Empty);
            KeyValuePairUtils.SetKeyValue<string>(TwitterClient.KeyUserID, string.Empty);
        }
    }
}
