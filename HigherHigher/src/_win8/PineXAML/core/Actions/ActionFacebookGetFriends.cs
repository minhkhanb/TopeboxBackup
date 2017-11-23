using PineShap.FacebookUtils;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Net.Http;
using System.IO;

namespace PineShap.Actions {
    class ActionFacebookGetFriendsArg : ActionArgs {
        public List<Int64> FriendIds = new List<long>();
        bool IsNextAvailable = false;
    }
    class ActionFacebookGetFriends:Action {
        Dictionary<WebClient, ActionFacebookGetFriendsArg> _wating = new Dictionary<WebClient, ActionFacebookGetFriendsArg>();
        public override bool precheckToDo(ActionArgs e) {
            return FacebookClient.Instance.IsHasToken();
        }
        async void loadFriend(ActionFacebookGetFriendsArg arg)
        {
            HttpClient client = new HttpClient();
            string url = FacebookClient.Instance.getAppUserUrl();
            try
            {
                string rs = await client.GetStringAsync(new Uri(url));
                arg.FriendIds.Clear();
                var juser = JsonConvert.DeserializeObject<FriendsList>(rs);
                if (juser != null && juser.data.Count > 0)
                {
                    for (int f = 0; f < juser.data.Count; f++)
                    {
                        arg.FriendIds.Add(Int64.Parse(juser.data[f].uid));
                    }
                }
                Done(arg);
            }
            catch (Exception ex)
            {
                Cancel(arg);
            }
        }
        public override void Do(ActionArgs e) {
            Begin(e);
            if (precheckToDo(e)) {
                loadFriend(e as ActionFacebookGetFriendsArg);
            } else {
                Cancel(e);
            }
        }
    }
}
