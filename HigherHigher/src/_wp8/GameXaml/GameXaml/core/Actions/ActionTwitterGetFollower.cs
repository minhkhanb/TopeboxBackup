using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pine.Actions
{
    class ActionTwitterGetFollowerArgs : ActionArgs
    {
        public List<UInt64> FriendIds = new List<UInt64>();
        bool IsNextAvailable = false;
    }
    public class ActionTwitterGetFollower:Action
    {
        ActionTwitterGetFollowerArgs CurrentArgs = null;
        public override bool precheckToDo(ActionArgs e)
        {
            var arg = e as ActionTwitterGetFollowerArgs;
            return (arg != null && CurrentArgs == null && TwitterUtils.TwitterClient.isAlreadyLoggedIn());
        }

        public override void Do(ActionArgs e)
        {
            Begin(e);
            if (precheckToDo(e))
            {
                var arg = e as ActionTwitterGetFollowerArgs;
                CurrentArgs = arg;
                TwitterUtils.TwitterClient.getFollower(onLoaded);
            }
            else
            {
                Cancel(e);
            }
        }

        private void onLoaded(Hammock.RestRequest request, Hammock.RestResponse response, object userState)
        {
            if (response != null)
            {
                if (CurrentArgs != null)
                {
                    var juser = JsonConvert.DeserializeObject<TwitterUtils.FollowerList>(response.Content);
                    if (juser != null && juser.ids.Count > 0)
                    {
                        for (int f = 0; f < juser.ids.Count; f++)
                        {
                            CurrentArgs.FriendIds.Add(juser.ids[f]);
                        }
                    }
                    Done(CurrentArgs);
                    CurrentArgs = null;
                }
            }
            else
            {
                if (CurrentArgs != null)
                {
                    Cancel(CurrentArgs);
                    CurrentArgs = null;
                }
            }
        }
    }
}
