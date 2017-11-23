using PineShap.FacebookUtils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace PineShap.Actions
{
    public class ActionFacebookFeedArgs : ActionArgs
    {
        public String Title;
        public String Message;
        public String IconLink;
        public String PageLink;

    }
    class ActionFacebookFeed:Action
    {
        public override bool precheckToDo(ActionArgs e)
        {
            return (e as ActionFacebookFeedArgs) != null;
        }
        async void feed(ActionFacebookFeedArgs arg)
        {
            HttpClient client = new HttpClient();
            Uri uri = new Uri("https://graph.beta.facebook.com/me/feed");
            string param =
                "message=" + WebUtility.UrlEncode(arg.Message)
                + "&access_token=" + FacebookClient.Instance.Token
                + "&link=" + arg.PageLink
                + "&picture=" + arg.IconLink
                + "&caption=" + WebUtility.UrlEncode(arg.Title);
            StringContent content = new StringContent(param);
            HttpResponseMessage rs = await client.PostAsync(uri, content);
            if (rs.IsSuccessStatusCode)
            {
                Done(arg);
            }
            else
            {
                Cancel(arg);
            }
        }
        public override void Do(ActionArgs e)
        {
            Begin(e);
            if (precheckToDo(e))
            {
                try
                {
                    feed(e as ActionFacebookFeedArgs);
                }
                catch (Exception ex)
                {
                    throw ex;
                }
            }
            else
            {
                Cancel(e);
            }
        }
    }
}
