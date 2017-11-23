using FacebookUtils;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.IsolatedStorage;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Resources;

namespace Pine.Actions {
    public class ActionFacebookGetAvatarArgs:ActionArgs {
        public BitmapImage Avatar = null;
        public bool IsGuest = false;
        public UInt64 FacebookId = 0;
        public ActionFacebookGetAvatarArgs() {
        }
        //load avatar of an user that know id
        public ActionFacebookGetAvatarArgs(UInt64 fbid){
            FacebookId = fbid;
            IsGuest = true;
        }
    }
    public class ActionFacebookGetAvatar:Action {
        public override bool precheckToDo(ActionArgs e) {
            ActionFacebookGetAvatarArgs arg = e as ActionFacebookGetAvatarArgs;
            if (arg != null) {
                if (arg.IsGuest) {
                    return (arg.FacebookId > 0);
                } else {
                    return (FacebookClient.Instance.IsUserLogedin());
                }
            }
            return false;
        }
        Dictionary<WebClient, ActionFacebookGetAvatarArgs> _waiting = new Dictionary<WebClient, ActionFacebookGetAvatarArgs>();
        public override void Do(ActionArgs e) {
            Begin(e);
            if (precheckToDo(e)) {
                try {
                    ActionFacebookGetAvatarArgs arg = e as ActionFacebookGetAvatarArgs;
                    WebClient client = new WebClient();
                    client.OpenReadCompleted += WebClientOpenReadCompleted;
                    if (arg.IsGuest) {
                        client.OpenReadAsync(new Uri(FacebookClient.Instance.getUserAvatarUrl(arg.FacebookId), UriKind.Absolute));
                    } else {
                        client.OpenReadAsync(new Uri(FacebookClient.Instance.getUserAvatarUrl(), UriKind.Absolute));
                    }
                    _waiting.Add(client, arg);
                } catch (Exception ex) {
                    Cancel(e);
                }
            } else {
                Cancel(e);
            }
        }

        private void WebClientOpenReadCompleted(object sender, OpenReadCompletedEventArgs e) {
            Deployment.Current.Dispatcher.BeginInvoke(() => {
                try {
                    ActionFacebookGetAvatarArgs arg = _waiting[(WebClient)sender];
                    var streamResourceInfo = new StreamResourceInfo(e.Result, null);
                    var bitmapImage = new BitmapImage ();
                    bitmapImage.SetSource(streamResourceInfo.Stream);
                    arg.Avatar = bitmapImage;
                    Done(arg);
                } catch (Exception ex) {

                }
            });
        }
    }
}
