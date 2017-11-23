using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Security;
//using System.Security.Cryptography;
using Windows.Security.Cryptography;
using System.Text;
using Pine;
using PineShap.Actions;
using PineShap.Object;
using System.Threading.Tasks;
using Pine.Tools;
using MD5;
using ImageTools;
using ImageTools.IO.Png;
using System.Windows;
using System.Windows.Media;
using ToolStackPNGWriterLib;
using System.Windows.Input;
using System.Data.Services.Client;
using PocketAvenger.ServerReal;
using PocketAvenger.ServerHighScore;
using PocketAvenger.ServerAvatar;
using PineShap.Tools;
using Windows.UI.Xaml.Media.Imaging;
using Windows.Storage.Streams;
using Windows.Graphics.Imaging;

namespace PineShap
{
    public class OdataServerDefine
    {
        public const string UrlServerReal = "http://odata.pa.pine-entertainment.com/api/PocketAvenger/";
        public const string UrlServerRealTest = "http://odata.pa.pine-entertainment.com/WATMmonitorfile.htm";
        public const string UrlServerDev = "http://fa9b7faa9cbe4241bac5123983a5b03d.cloudapp.net/api/PocketAvenger/";
        public const string UrlServerAvatar = "http://services.pine-entertainment.com/api/avatar/";

        public const string UrlServerDevTest =
            "http://fa9b7faa9cbe4241bac5123983a5b03d.cloudapp.net/WATMmonitorfile.htm";

        public const string UrlServerSendRequest = "http://odata.phs.pine-entertainment.com/api/PineHighScore/";

        public const string Secret = "pineEntertainment";
    }

    public class CurrentGameServiceRequest : ClientServiceRequest
    {
        public static ClientServiceRequest Exsample = new ClientServiceRequest(1001);
        public CurrentGameServiceRequest(int requestCode)
            : base(requestCode)
        {
        }
    }
    public class CurrentGameService : ClientService
    {
        //url server
        private string Context_URL = OdataServerDefine.UrlServerReal;

        public override string OuthorizeString(string id)
        {
            string rs = "";
            var keybytes = new byte[32];
            string key = OdataServerDefine.Secret;
            byte[] tmpKeybytes = Encoding.UTF8.GetBytes(key);
            byte[] rsbyte = Security.encrypt(id, key);
            rs = Convert.ToBase64String(rsbyte);
            return rs;
        }

        


        public override void process(ClientServiceRequest request, ServerStream svstream)
        {
            if (request == CurrentGameServiceRequest.Exsample)
            {

            }
            else
            {
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }

    }
}