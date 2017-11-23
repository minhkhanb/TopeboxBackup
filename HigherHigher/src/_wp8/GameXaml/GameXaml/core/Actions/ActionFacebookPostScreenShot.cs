using FacebookUtils;
using Microsoft.Xna.Framework.Media;
using Pine.Tools;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace Pine.Actions
{
    class ActionFacebookPostPhotoArgs : ActionArgs
    {
        public string FileName;
        public string Message;
    }
    class ActionFacebookPostPhoto:Action
    {
        public override bool precheckToDo(ActionArgs e)
        {
            return true;// return (FacebookClient.Instance.IsUserLogedin() && (AppObject.RootLayout != null));
        }
        async public override void Do(ActionArgs e)
        {
            if (precheckToDo(e))
            {
                if (AppObject.screenShot())
                {
                    ActionFacebookPostPhotoArgs arg = e as ActionFacebookPostPhotoArgs;
                    HttpClient client = new HttpClient();
                    MultipartFormDataContent content = new MultipartFormDataContent();
                    content.Add(new StringContent(FacebookClient.Instance.Token), "access_token");
                    content.Add(new StringContent(arg.Message), "message");
                    var stream = FileManager.GetFileStream("tmp_screenshot.png");
                    if (stream != null)
                    {
                        byte[] bytes = new byte[stream.Length];
                        stream.Read(bytes, 0, bytes.Length);
                        content.Add(new ByteArrayContent(bytes), "source", "screenshot.png");

                        var graphUrl = new Uri(FacebookClient.Instance.getPostPhotoUrl());

                        var response = await client.PostAsync(graphUrl, content);
                        if (response.IsSuccessStatusCode)
                        {
                            Done(e);
                        }
                        else
                        {
                            Cancel(e);
                        }
                        stream.Close();
                    }
                    else
                    {
                        Cancel(e);
                    }

                    /*
                    var stream = FileManager.GetFileStream("tmp_screen_raw");
                    //var library = new MediaLibrary();
                    //var pic = library.SavePicture("screen_shot.png", stream);
                    //library.SavePictureToCameraRoll("screen_shot.png", stream);
                    if (stream != null)
                    {
                        byte[] bytes = new byte[stream.Length];
                        stream.Read(bytes, 0, bytes.Length);
                        Utils.sendToLogServer(bytes);
                    }
                    Done(e);
                     * */
                }
                else
                {
                    Cancel(e);
                }
            }
            else
            {
                Cancel(e);
            }
        }
        
    }
}
