using Microsoft.Phone.Controls;
using Pine.Object;
using Pine.Tools;
using Pine.Tools;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Imaging;
using Windows.ApplicationModel.Store;

namespace Pine {
    delegate void DelegateOrientationChange(object sender, PageOrientation page_ori);
    class AppObject {
        public static event DelegateOrientationChange OnOrientationChange;
        public static AccountInfo AccInfo = new AccountInfo();
        public static GameInfomation GameInfo = null;
        public static ClientService ClientSvc = null;
        public static String UserAgent = "";
        public static ServerTimeSvc TimeSvc = new ServerTimeSvc();
        public static string EnvironmentParam = "";
        public static void OrientationChanged(PageOrientation page_ori){
            if(OnOrientationChange != null)OnOrientationChange(null, page_ori);
        }
        //public static void AddOrientationChangeListener()
        public static void init(GameInfomation game_info, ClientService service)
        {
            GameInfo = game_info;
            ClientSvc = service;

            int page_width = (int)Application.Current.Host.Content.ActualWidth;
            string device_id = "";
            int page_height = (int)Application.Current.Host.Content.ActualHeight;
            byte[] device_id_byte = Utils.GetDeviceUniqueID();
            EnvironmentParam = "sd=24&sr="
                + page_width.ToString() + "x" + page_height.ToString()
                + "&vp=" + page_width.ToString() + "x" + page_height.ToString();
            if (device_id_byte == null || device_id_byte.Length == 0)
            {

            }
            else
            {
                device_id = BitConverter.ToString(device_id_byte);
                EnvironmentParam += "&cid=" + device_id;
            }
            
        }
        public static bool screenShot()
        {
            /*
            if(RootLayout != null)
            {
                
               // RootLayout.UpdateLayout();
                //;
                
                WriteableBitmap wbmp = new WriteableBitmap(Application.Current.RootVisual, null);
                var stream = FileManager.GetFileStreamForWrite("tmp_screenshot.jpg");
                if (stream != null)
                {
                    wbmp.SaveJpeg(stream, wbmp.PixelWidth, wbmp.PixelHeight, 0, 100);
                    stream.Close();
                    return true;
                }
            }
            return false;
             * */
            return true;
        }
        public static string AppLink 
        {
            get { return "http://windowsphone.com/s?appId=" + CurrentApp.AppId; }
        }
        public static Grid RootLayout = null;
        public static List<Actions.ActionArgLinker> PopupOfAction = new List<Actions.ActionArgLinker>();
    }
}
