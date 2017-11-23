using PineShap.FacebookUtils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using PineShap.Tools;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml;
using PineXAML;
using Facebook.Client;
using Windows.UI.Popups;

namespace PineShap.Dialogs {
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
        private FacebookSession session;
        public event DelegateLoginFacebookDialogComplete OnComplete;
        Popup _popup = null;

        override public Popup Dialog { get { return _popup; } }
        ////WebBrowser _browser;
        public LoginFacebookDialog() {
            _popup = new Popup();
            Border border_root = new Border();
            border_root.Margin = new Thickness(0, 0, 0, 0);
            float page_width = (float)Window.Current.Bounds.Width;
            float page_height = (float)Window.Current.Bounds.Height;

            border_root.Width = page_width;//// Application.Current.Host.Content.ActualHeight;
            border_root.Height = page_height;////// Application.Current.Host.Content.ActualWidth;

            Grid grid_root = new Grid();
            //grid_root.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Auto) });
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
            ////_browser = new WebBrowser();

            ////_browser.Navigated += _browser_Navigated;
            ////_browser.NavigationFailed += _browser_NavigationFailed;
            ////_browser.Navigating += _browser_Navigating;
            ////_browser.VerticalAlignment = VerticalAlignment.Stretch;
            ////_browser.HorizontalAlignment = HorizontalAlignment.Stretch;
            ////_browser.Height = 470;
            //_browser.Width = 800;

            ////panel_content.Children.Add(_browser);

            border_content.Child = panel_content;
            grid_root.Children.Add(border_content);
            border_root.Child = grid_root;
            _popup.Child = border_root;
            //
        }
        /*
        void _browser_Navigating(object sender, NavigatingEventArgs e) {
            //throw new NotImplementedException();
        }

        void _browser_NavigationFailed(object sender, System.Windows.Navigation.NavigationFailedEventArgs e) {
            fail();
        }

        void _browser_Navigated(object sender, System.Windows.Navigation.NavigationEventArgs e) {
            String uri = e.Uri.ToString();
            if (uri.StartsWith("https://www.facebook.com/connect/login_success.html") || uri.StartsWith("http://www.facebook.com/connect/login_success.html")
                || uri.StartsWith("https://m.facebook.com/connect/login_success.html") || uri.StartsWith("http://m.facebook.com/connect/login_success.html")) {
                if (uri.EndsWith("#_=_")) {
                    uri = uri.Substring(0, uri.Length - 4);
                }
                String queryString = e.Uri.Query.ToString();
                try {
                    IEnumerable<KeyValuePair<string, string>> pairs = UriToolKits.ParseQueryString(queryString);
                    string code = KeyValuePairUtils.GetValue(pairs, "code");
                    string tokenurl = FacebookClient.Instance.getAccessTokenRequestUrl(code);
                    WebClient client = new WebClient();
                    client.DownloadStringCompleted += new DownloadStringCompletedEventHandler(AccessTokenDownloadCompleted);
                    client.DownloadStringAsync(new Uri(tokenurl));
                } catch (Exception ex) {
                    fail();
                }
                }
            else if (uri.StartsWith("https://m.facebook.com/dialog/oauth/write") || uri.StartsWith("http://m.facebook.com/dialog/oauth/write"))
            {
                string url = FacebookClient.Instance.getLoginUrl();
                _browser.Source = new Uri(url);
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
        }*/
        private async Task Authenticate()
        {
            string message = String.Empty;
            try
            {

                session = await FacebookManager.SessionClient.LoginAsync("basic_info,publish_actions,user_about_me,read_stream");
                string state = session.State;
                if (session.AccessToken.Length > 0)
                {
                    state = session.State;
                    FacebookManager.setToken(session.AccessToken);
                    success(session.AccessToken, session.Expires.ToString());
                }
                //Facebook.FacebookClient client = new Facebook.FacebookClient(session.AccessToken);
                //var result = await client.GetTaskAsync("/me?fields=id,name");
                //state = session.State;
            }
            catch (InvalidOperationException e)
            {
                //message = "Login failed! Exception details: " + e.Message;
               // MessageDialog dialog = new MessageDialog(message);
                //dialog.ShowAsync();
                cancel();
            }
        }
        async public void show(float x, float y){
            string url = FacebookClient.Instance.getLoginUrl();
            await Authenticate();
            _popup.IsOpen = true;
        }
        public void cancel() {
            fail();
        }
        private void success(string token, string tokenex) {
            _popup.IsOpen = false;
            if (OnComplete != null) OnComplete(this, new LoginFacebookDialogCompleteArgs(true, token, tokenex));
        }
        private void fail() {
            _popup.IsOpen = false;
            if (OnComplete != null) {
                OnComplete(this, new LoginFacebookDialogCompleteArgs(false, "", ""));
            }
        }
    }
}
