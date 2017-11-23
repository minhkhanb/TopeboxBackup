using PineShap.FacebookUtils;
using PineShap.Tools;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Resources;
using Windows.Graphics.Imaging;
using Windows.Storage.Streams;
using Windows.UI.Xaml.Media.Imaging;

namespace PineShap.Actions {
    public class ActionFacebookGetAvatarArgs:ActionArgs {
        public IRandomAccessStream Avatar = null;
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
        async void loadAvatar(ActionFacebookGetAvatarArgs arg)
        {
            HttpClient client = new HttpClient();
            string url = "";
            if (arg.IsGuest) {
                url = FacebookClient.Instance.getUserAvatarUrl(arg.FacebookId);
            } else {
               url = FacebookClient.Instance.getUserAvatarUrl();    
            }
#if DEBUG
            Test.sendLog("C# begin loadAvatar url:" + url);
#endif
             byte[] data = await client.GetByteArrayAsync(url);

             var dispatcher = ActionHelper.getCurrentUIThread();
             dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
             {
                 try
                 {
                     CreateBitmap(arg, data);
                 }
                 catch (Exception ex)
                 {
#if DEBUG
                     Test.sendLog("C# loadAvatar err:" + ex.ToString());
#endif
                 }
             });
        }
        public override void Do(ActionArgs e) {
            Begin(e);
            if (precheckToDo(e)) {
                try {
                    ActionFacebookGetAvatarArgs arg = e as ActionFacebookGetAvatarArgs;
                    loadAvatar(arg);
                } catch (Exception ex) {
                    Cancel(e);
                }
            } else {
                Cancel(e);
            }
        }
        public static async Task<IRandomAccessStream> ConvertToRandomAccessStream(MemoryStream memoryStream)
        {
            var randomAccessStream = new InMemoryRandomAccessStream();
            var outputStream = randomAccessStream.GetOutputStreamAt(0);
            var dw = new DataWriter(outputStream);
            var task = Task.Factory.StartNew(() => dw.WriteBytes(memoryStream.ToArray()));
            await task;
            await dw.StoreAsync();
            await outputStream.FlushAsync();
            return randomAccessStream;
        }
        
        async void CreateBitmap(ActionFacebookGetAvatarArgs arg,byte[] data)
        {
            MemoryStream ms = new MemoryStream(data);
            IRandomAccessStream a1 = await ConvertToRandomAccessStream(ms);
            ////var bitmapImage = new WriteableBitmap(64,64);
            ////await bitmapImage.SetSourceAsync(a1);
            ////MemoryStream ms_out = new MemoryStream();
            ////IRandomAccessStream a2 = await ConvertToRandomAccessStream(ms_out);


            ////BitmapEncoder encoder = await BitmapEncoder.CreateAsync(BitmapEncoder.PngEncoderId,a2);
            
            ////IBuffer buff = bitmapImage.PixelBuffer;
            ////DataReader reader = DataReader.FromBuffer(buff);
            ////byte[] pixels = new byte[buff.Length];
            /*reader.ReadBytes(pixels);
            encoder.SetPixelData(BitmapPixelFormat.Bgra8, BitmapAlphaMode.Ignore, 64, 64, 96, 96, pixels);
            await a2.ReadAsync(pixels.as(), (uint)myMemoryStream.Size, Windows.Storage.Streams.InputStreamOptions.None);
            using (Stream s = await FileManager.GetFileStreamForWrite("tmpAvatar"))
            {
                byte[] out_data = new byte[a2.Size];
                
            }*/
            arg.Avatar = a1;
            Done(arg);
        }
    }
}
