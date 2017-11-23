using FacebookUtils;
using Microsoft.Phone.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Media;
using Pine.Tools;
using System.Diagnostics;

namespace Pine.Dialogs {
    public class LoginFacebookDialogCompleteArgs:EventArgs {
        public bool IsSuccess = false;
        public string Token = "";
        public string TokenExprite = "";
        public LoginFacebookDialogCompleteArgs(bool success, string token, string tokenexprite) {
            IsSuccess = success;
            Token = token;
            TokenExprite = tokenexprite;
        }
    }
    public delegate void DelegateLoginFacebookDialogComplete(object sender, LoginFacebookDialogCompleteArgs e);

    public class LoginFacebookDialog : DialogBase
    {
        public event DelegateLoginFacebookDialogComplete OnComplete;
        Popup _popup = null;
        FacebookAppInfomation _app_info;

        override public Popup Dialog { get { return _popup; } }
        WebBrowser _browser;
        public LoginFacebookDialog(FacebookAppInfomation appinfo) {
            _popup = new Popup();
            _app_info = appinfo;
            createContent((App.Current.RootVisual as PhoneApplicationFrame).Orientation);
            
        }
        public void createContent(PageOrientation ori)
        {
            Border border_root = new Border();
            border_root.Margin = new Thickness(0, 0, 0, 0);
            float page_width = (float)Application.Current.Host.Content.ActualWidth;
            float page_height = (float)Application.Current.Host.Content.ActualHeight;
            if (ori == PageOrientation.Portrait || ori == PageOrientation.PortraitUp || ori == PageOrientation.PortraitDown)
            {
                float tmp = page_width;
                page_width = page_width > page_height ? page_height : page_width;
                page_height = page_width == page_height ? tmp : page_height;
            }
            else
            {
                float tmp = page_width;
                page_width = page_width > page_height ? page_width : page_height;
                page_height = page_width == page_height ? tmp : page_height;
            }
            border_root.Width = page_width;
            border_root.Height = page_height;

            Grid grid_root = new Grid();
            grid_root.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Star) });

            Border border_content = new Border();
            border_content.VerticalAlignment = VerticalAlignment.Stretch;
            border_content.HorizontalAlignment = HorizontalAlignment.Stretch;
            border_content.Margin = new Thickness(15, 15, 15, 15);

            StackPanel panel_content = new StackPanel();
            panel_content.VerticalAlignment = VerticalAlignment.Stretch;
            panel_content.HorizontalAlignment = HorizontalAlignment.Stretch;
            TextBlock title = new TextBlock();
            title.Height = 25;
            title.Text = "Login to Facebook";
            //panel_content.Children.Add(title);
            _browser = new WebBrowser();

            _browser.Navigated += _browser_Navigated;
            _browser.NavigationFailed += _browser_NavigationFailed;
            _browser.Navigating += _browser_Navigating;
            _browser.VerticalAlignment = VerticalAlignment.Stretch;
            _browser.HorizontalAlignment = HorizontalAlignment.Stretch;
            _browser.Height = page_height - 10;
            _browser.Width = page_width - 10;
            string url = FacebookClient.Instance.getLoginUrl(_app_info);
            _browser.Source = new Uri(url);

            panel_content.Children.Add(_browser);

            border_content.Child = panel_content;
            grid_root.Children.Add(border_content);
            border_root.Child = grid_root;
            _popup.Child = border_root;
        }

        void _browser_Navigating(object sender, NavigatingEventArgs e) {
            //throw new NotImplementedException();
        }

        void _browser_NavigationFailed(object sender, System.Windows.Navigation.NavigationFailedEventArgs e) {
            fail();
        }

        void _browser_Navigated(object sender, System.Windows.Navigation.NavigationEventArgs e) 
        {
            try
            {
                String uri = e.Uri.ToString();
                
                if (uri.StartsWith("https://www.facebook.com/connect/login_success.html") || uri.StartsWith("http://www.facebook.com/connect/login_success.html")
                    || uri.StartsWith("https://m.facebook.com/connect/login_success.html") || uri.StartsWith("http://m.facebook.com/connect/login_success.html")
                    || uri.StartsWith("https://www.facebook.com/v2.0/connect/login_success.html") || uri.StartsWith("http://www.facebook.com/v2.0/connect/login_success.html")
                    || uri.StartsWith("https://m.facebook.com/v2.0/connect/login_success.html") || uri.StartsWith("http://m.facebook.com/v2.0/connect/login_success.html")
                    || uri.StartsWith("https://www.facebook.com/v1.0/connect/login_success.html") || uri.StartsWith("http://www.facebook.com/v1.0/connect/login_success.html")
                    || uri.StartsWith("https://m.facebook.com/v1.0/connect/login_success.html") || uri.StartsWith("http://m.facebook.com/v1.0/connect/login_success.html")
                    || uri.StartsWith(_app_info.RedirectLink))
                {
                    if (uri.EndsWith("#_=_"))
                    {
                        uri = uri.Substring(0, uri.Length - 4);
                    }
                    String queryString = e.Uri.Query.ToString();
                    try
                    {
                        IEnumerable<KeyValuePair<string, string>> pairs = UriToolKits.ParseQueryString(queryString);
                        string code = KeyValuePairUtils.GetValue(pairs, "code");
                        string tokenurl = FacebookClient.Instance.getAccessTokenRequestUrl(_app_info,code);
                        WebClient client = new WebClient();
                        client.DownloadStringCompleted += new DownloadStringCompletedEventHandler(AccessTokenDownloadCompleted);
                        client.DownloadStringAsync(new Uri(tokenurl));
                    }
                    catch (Exception ex)
                    {
                        fail();
                    }
                }
                else if (uri.StartsWith("https://m.facebook.com/dialog/oauth/write")
                    || uri.StartsWith("http://m.facebook.com/dialog/oauth/write")
                    || uri.StartsWith("https://m.facebook.com/v2.0/dialog/oauth/write")
                    || uri.StartsWith("http://m.facebook.com/v2.0/dialog/oauth/write")
                    || uri.StartsWith("https://m.facebook.com/v1.0/dialog/oauth/write")
                    || uri.StartsWith("http://m.facebook.com/v1.0/dialog/oauth/write"))
                {
                    string url = FacebookClient.Instance.getLoginUrl(_app_info);
                    _browser.Source = new Uri(url);
                }
        
            }
            catch (Exception ex2)
            {
                fail();
                Debug.WriteLine(ex2.Message);
            }
        }
        void AccessTokenDownloadCompleted(object sender, DownloadStringCompletedEventArgs e) {
            if (!e.Cancelled && e.Error == null) {
                try {
                    string data = e.Result;
                    data = "?" + data;
                    IEnumerable<KeyValuePair<string, string>> pairs = UriToolKits.ParseQueryString(data);
                    string accessToken = KeyValuePairUtils.GetValue(pairs, "access_token");
                    string expires = KeyValuePairUtils.GetValue(pairs, "expires");
                    success(accessToken, expires);
                } catch (Exception ex) {
                    fail();
                }
            } else {
                fail();
            }
        }
        public void show(float x, float y){
            
            AppObject.OnOrientationChange += AppObject_OnOrientationChange;
            _popup.IsOpen = true;
        }

        void AppObject_OnOrientationChange(object sender, PageOrientation page_ori)
        {
            createContent(page_ori);
        }
        public void cancel() {
            fail();
        }
        private void success(string token, string tokenex) {
            _popup.IsOpen = false;
            AppObject.OnOrientationChange -= AppObject_OnOrientationChange;
            if (OnComplete != null) OnComplete(this, new LoginFacebookDialogCompleteArgs(true, token, tokenex));
        }
        private void fail() {
            _popup.IsOpen = false;
            AppObject.OnOrientationChange -= AppObject_OnOrientationChange;
            if (OnComplete != null) {
                OnComplete(this, new LoginFacebookDialogCompleteArgs(false, "", ""));
            }
        }
    }
}
