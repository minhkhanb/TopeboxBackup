using FacebookUtils;
using Newtonsoft.Json;
using Pine.FacebookUtils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace Pine.Actions {
    class ActionFacebookGetFriendsArg : ActionArgs {
        public List<Int64> FriendIds = new List<long>();
        bool IsNextAvailable = false;
    }
    class ActionFacebookGetFriends:Action {
        Dictionary<WebClient, ActionFacebookGetFriendsArg> _wating = new Dictionary<WebClient, ActionFacebookGetFriendsArg>();
        public override bool precheckToDo(ActionArgs e) {
            return FacebookClient.Instance.IsHasToken();
        }

        public override void Do(ActionArgs e) {
            Begin(e);
            if (precheckToDo(e)) {
                WebClient client = new WebClient();
                _wating.Add(client, e as ActionFacebookGetFriendsArg);
                client.DownloadStringCompleted += new DownloadStringCompletedEventHandler(LoadFriendComplete);
                string url = FacebookClient.Instance.getAppUserUrl();
                try
                {
                    client.DownloadStringAsync(new Uri(url));
                }
                catch (Exception ex)
                {
                    Cancel(e);
                }
            } else {
                Cancel(e);
            }
        }
        private void LoadFriendComplete(object sender, DownloadStringCompletedEventArgs e) {
            try {
                if (_wating.ContainsKey((WebClient)sender)) {
                    ActionFacebookGetFriendsArg arg = _wating[(WebClient)sender];
                    arg.FriendIds.Clear();
                    _wating.Remove((WebClient)sender);
                    if (e != null && !e.Cancelled && e.Error == null) {
                        string result = e.Result;
                        if (result.IndexOf("uid") > 0)
                        {
                            var juser = JsonConvert.DeserializeObject<FriendListVer1>(result);
                            if (juser != null && juser.data.Count > 0)
                            {
                                for (int f = 0; f < juser.data.Count; f++)
                                {
                                    arg.FriendIds.Add(Int64.Parse(juser.data[f].uid));
                                }
                            }
                        }
                        else
                        {
                            var juser = JsonConvert.DeserializeObject<FriendsList>(result);
                            if (juser != null && juser.data.Count > 0)
                            {
                                for (int f = 0; f < juser.data.Count; f++)
                                {
                                    arg.FriendIds.Add(Int64.Parse(juser.data[f].id));
                                }
                            }
                        }
                        //if(juser.paging.next)
                        Done(arg);
                    } else {
                        Cancel(arg);
                    }
                }
                else
                {
                    Debug.WriteLine("abc");
                }
            } catch (Exception ex) {
                try
                {
                    ActionFacebookGetFriendsArg arg = _wating[(WebClient)sender];
                    Cancel(arg);
                }
                catch (Exception nex)
                {

                }
            }
        }
    }
}
