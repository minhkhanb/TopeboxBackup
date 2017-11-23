using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Imaging;
using System.Windows.Resources;

namespace Pine.Actions
{
    public class ActionLoadImageArgs : ActionArgs
    {
        public BitmapImage Image = null;
        public Uri ImageUri = null;
        public int DesWidth;
        public int DesHeight;
        public string DesFileName;
    }
    public class ActionLoadImage:Action
    {
        Dictionary<WebClient, ActionLoadImageArgs> _waiting = new Dictionary<WebClient, ActionLoadImageArgs>();
        public override bool precheckToDo(ActionArgs e)
        {
            var arg = e as ActionLoadImageArgs;
            return (arg != null && arg.ImageUri != null);
        }

        public override void Do(ActionArgs e)
        {
            Begin(e);
            if (precheckToDo(e))
            {
                var arg = e as ActionLoadImageArgs;
                WebClient client = new WebClient();
                client.OpenReadCompleted += client_OpenReadCompleted;
                _waiting.Add(client, arg);
                client.OpenReadAsync(arg.ImageUri);
            }
            else
            {
                Cancel(e);
            }
        }

        void client_OpenReadCompleted(object sender, OpenReadCompletedEventArgs e)
        {
            Deployment.Current.Dispatcher.BeginInvoke(() =>
            {
                try
                {
                    ActionLoadImageArgs arg = _waiting[(WebClient)sender];
                    if (e.Error == null)
                    {
                        var streamResourceInfo = new StreamResourceInfo(e.Result, null);
                        var bitmapImage = new BitmapImage();
                        bitmapImage.DecodePixelWidth = arg.DesWidth;
                        bitmapImage.DecodePixelHeight = arg.DesHeight;
                        bitmapImage.SetSource(streamResourceInfo.Stream);
                        arg.Image = bitmapImage;
                        Done(arg);
                    }
                    else
                    {
                        Cancel(arg);
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
            });
        }
    }
}
