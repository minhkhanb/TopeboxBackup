//Bo add

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using FacebookUtils;
using Pine.FacebookUtils;
using Newtonsoft.Json;
namespace Pine.Actions
{
    public class ActionFacebookPostHighScoreArg:ActionArgs
    {
        public int Score = 0;
        public ScoreInformation score_inform = null;
    }
    public class ActionFacebookPostHighScore : Action
    {
        Dictionary<WebClient, ActionFacebookPostHighScoreArg> _waiting = new Dictionary<WebClient, ActionFacebookPostHighScoreArg>();
        public override bool precheckToDo(ActionArgs e)
        {
            return FacebookClient.Instance.IsHasToken();
        }
        public override void Do(ActionArgs e)
        {
            Begin(e);
            if (precheckToDo(e))
            {
                WebClient client = new WebClient();
                _waiting.Add(client, e as ActionFacebookPostHighScoreArg);
                ActionFacebookPostHighScoreArg arg = new ActionFacebookPostHighScoreArg();
                arg = e as ActionFacebookPostHighScoreArg;
                client.UploadStringCompleted += client_UploadStringCompleted;
                client.UploadStringAsync(new Uri("https://graph.facebook.com/me/scores"), "POST", "score="+arg.Score.ToString()+"&access_token="+FacebookClient.Instance.Token);
            }
        }
        public void client_UploadStringCompleted(object sender, UploadStringCompletedEventArgs e)
        {
            if (!e.Cancelled && e.Error == null)
            {
                try
                {
                    ActionFacebookPostHighScoreArg arg = _waiting[(WebClient)sender];
                    string data = e.Result;
                    ScoreInformation succeed = new ScoreInformation();
                    succeed=JsonConvert.DeserializeObject<ScoreInformation>(data);
                    if (succeed != null)
                    {
                        if (succeed.success)
                        {
                            arg.score_inform = new ScoreInformation();
                            arg.score_inform = succeed;
                            Done(arg);
                            _waiting.Remove((WebClient)sender);
                        }
                    }
                }
                catch (Exception ex)
                {

                }
            }
        }
    }
}
