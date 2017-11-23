using FacebookUtils;
using Newtonsoft.Json;
using Pine.core.FacebookUtils;
using Pine.FacebookUtils;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace Pine.Actions {
    public class ActionFacebookGetInfomationArgs:ActionArgs {
        public UserInfomation UserInfomation = null;
    }
    public class ActionFacebookGetInfomation:Action {
        Dictionary<WebClient, ActionFacebookGetInfomationArgs> _wating = new Dictionary<WebClient, ActionFacebookGetInfomationArgs>();
        public override bool precheckToDo(ActionArgs e) {
            return FacebookClient.Instance.IsHasToken();
        }

        public override void Do(ActionArgs e) {
            Begin(e);
            if (precheckToDo(e)) {
                WebClient client = new WebClient();
                _wating.Add(client, e as ActionFacebookGetInfomationArgs);
                client.DownloadStringCompleted += new DownloadStringCompletedEventHandler(LoadInfomationComplete);
                client.DownloadStringAsync(new Uri(FacebookClient.Instance.getUserInfomationUrl()));
            } else {
                Cancel(e);
            }
        }

        private void LoadInfomationComplete(object sender, DownloadStringCompletedEventArgs e) {
            if (!e.Cancelled && e.Error == null) {
                try {
                    
                    ActionFacebookGetInfomationArgs arg = _wating[(WebClient)sender];
                    string data = e.Result;
                    var juser = JsonConvert.DeserializeObject<UserInfomation>(data);
                    if (juser != null) {
                        FacebookClient.Instance.updateUserInfomation(juser.id, juser.name);
                        arg.UserInfomation = juser;
                        WebClient client = new WebClient();
                        _wating.Add(client, arg);
                        client.DownloadStringCompleted += avatar_DownloadStringCompleted;
                        client.DownloadStringAsync(new Uri(FacebookClient.Instance.getUserAvatarUrlNotRedirect()));
                        
                    }
                    else
                    {
                        Cancel(arg);
                    }
                } catch (Exception ex) {
                }
            } else {
                try {
                    string data = e.Result;
                    ActionFacebookGetInfomationArgs arg = _wating[(WebClient)sender];
                    Cancel(arg);
                } catch (Exception ex) {
                }
            }
        }

        void avatar_DownloadStringCompleted(object sender, DownloadStringCompletedEventArgs e)
        {
            if (!e.Cancelled && e.Error == null)
            {
                try
                {
                    ActionFacebookGetInfomationArgs arg = _wating[(WebClient)sender];
                    string data = e.Result;
                    var juser = JsonConvert.DeserializeObject<AvatarRedirect>(data);
                    if (juser != null)
                    {
                        FacebookClient.Instance.updateUserAvatarUrl(juser.data.url);
                    }
                    Done(arg);
                    _wating.Remove((WebClient)sender);
                }
                catch (Exception ex)
                {
                }
            }
            else
            {
                try
                {
                    ActionFacebookGetInfomationArgs arg = _wating[(WebClient)sender];
                    Done(arg);
                    _wating.Remove((WebClient)sender);
                }
                catch (Exception ex)
                {
                }
            }
        }
    }
}
