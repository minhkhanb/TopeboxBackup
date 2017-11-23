using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Windows;
//using PocketArmy.PictureOData;
using System.Net;
using Microsoft.Phone.Info;
using System.Security.Cryptography;
using MD5;
using System.IO;
using System.Data.Services.Client;
using Microsoft.Phone.Controls;
using Pine.Tools;
namespace Pine{
    public class ODataService<T> where T:DataServiceContext{
        public T _context = null;
        public string UserId = "";
        public string UserPass = "";
        NetworkCredential Credential = null;
        static public string DefaultCredential = "Basic dHJpZXVuazpGUFQwMzEyUzE=";
        public ODataService(string url){
            Uri server_uri = new Uri(url);
            _context = (T)Activator.CreateInstance(typeof(T), new object[1]{server_uri});
            //_context.UseDefaultCredentials = false;

            Credential = new NetworkCredential().GetCredential(server_uri,DefaultCredential);
            //_context.Format.UseJson();
            _context.Credentials = Credential;
            _context.SendingRequest2 += (o, args) =>
            {
                var request = ((HttpWebRequestMessage)args.RequestMessage).HttpWebRequest;
                if (AppObject.UserAgent.Length > 0)
                {
                    request.Headers["X-Tag"] = Utils.EnscryptString(AppObject.UserAgent);
                    
                }
                if (AppObject.EnvironmentParam.Length > 0)
                {
                    request.Headers["X-Params"] = Convert.ToBase64String(Encoding.UTF8.GetBytes(AppObject.EnvironmentParam));
                }
            };
        }
        
        public void setUserId(UInt64 userid){
            //_context.UseDefaultCredentials = false;
            Credential.UserName = userid.ToString();
            Credential.Password = AppObject.ClientSvc.OuthorizeString(userid.ToString());
            _context.Credentials = Credential;
        }
        public void setUserId(String userid)
        {
            Credential.UserName = userid;
            Credential.Password = AppObject.ClientSvc.OuthorizeString(userid);
            _context.Credentials = Credential;
        }
        //public void load(
        public static byte[] StringToAscii(string s) {
            List<byte> rs = new List<byte>();
            for (int c = 0; c < s.Length; c++) {
                char chr = s[c];
                int chrint = (int)chr;
                if (chrint >=32 && chrint < 127) {
                    rs.Add((byte)chrint);
                }
            }
            return rs.ToArray();
        }

        public static byte[] GetDeviceUniqueID()  {  
            byte[] result = null;  
            object uniqueId;  
            if (DeviceExtendedProperties.TryGetValue("DeviceUniqueId", out uniqueId))  
                result = (byte[])uniqueId;
            return result;  
        }  
        private static readonly int ANIDLength = 32;  
        private static readonly int ANIDOffset = 2; 
        public static string GetWindowsLiveAnonymousID(){  
            string result = string.Empty;
            object anid;  
            if (UserExtendedProperties.TryGetValue("ANID2", out anid)){  
                result = anid.ToString();
            }  
            return result;  
        }
    }
}
