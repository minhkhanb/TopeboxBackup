using System;
using System.Net;
using System.Windows;
using System.Windows.Input;

namespace PineShap.TwitterUtils
{
    public class AppSettings
    {
        // twitter
        public static string RequestTokenUri = "https://api.twitter.com/oauth/request_token";
        public static string AuthorizeUri = "https://api.twitter.com/oauth/authorize";
        public static string AccessTokenUri = "https://api.twitter.com/oauth/access_token";
        public static string CallbackUri = "http://www.google.com";        

        //#error TODO REGISTER YOUR APP WITH TWITTER TO GET YOUR KEYS AND FILL THEM IN HERE

        public static string oAuthVersion = "1.0a";
    }
}
