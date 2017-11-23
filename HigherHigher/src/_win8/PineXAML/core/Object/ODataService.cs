using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.Services.Client;
using Microsoft.Data.Edm;
using System.Diagnostics;
using System.Windows;
//using PocketArmy.PictureOData;
using System.Net;
//using System.Security.Cryptography;
using MD5;
using System.IO;
using PineShap;
using System.Net.Http;
using Windows.UI.Xaml.Controls;
using PineShap.Actions;
namespace Pine{
    public class ODataService<T> where T:DataServiceContext{
        public T _context = null;
        public string UserId = "";
        public string UserPass = "";
        NetworkCredential Credential = null;

        
        public ODataService(string url){
            Uri server_uri = new Uri(url);
            _context = (T)Activator.CreateInstance(typeof(T), new object[1]{server_uri});
            //_context.UseDefaultCredentials = false;
            Credential = new NetworkCredential().GetCredential(server_uri, "Basic");
            
            //_context.SendingRequest2 += _context_SendingRequest2;
            _context.Credentials = Credential;
        }
        public async void setUserId(UInt64 userid){
           // _context.UseDefaultCredentials = false;
            Credential.UserName = userid.ToString();
            Credential.Password = AppObject.ClientSvc.OuthorizeString(userid.ToString());
            _context.Credentials = Credential;
           
            //try
            //{
                //Environment.
                //String devicename = Microsoft.Phone.Info.DeviceStatus.DeviceName;
                //Environment.OSVersion.Version;
               // HttpClient client = new HttpClient();
                //var agent = client.DefaultRequestHeaders.UserAgent;

                //var httpRequest = (HttpWebRequest)WebRequest.Create(new Uri("http://google.com"));
                //var userAgent = await WindowsStoreSystemInformation.GetSystemUserAgent();
                //if (null != userAgent)
                //{
                    //userAgent.SetValue(httpRequest, "Your value", null);
                //}
            //}
            //catch (Exception ex)
            //{

            //}
        }

        /*void _context_SendingRequest2(object sender, SendingRequest2EventArgs e)
        {
            HttpClient httpclient = new HttpClient();
            var userAgent = httpclient.DefaultRequestHeaders.UserAgent;
            string useragent = userAgent.ToString();
            var request = ((HttpWebRequestMessage)e.RequestMessage).HttpWebRequest;
            //request.Headers["Authorization"] =  "Basic dHJpZXVuazpGUFQwMzEyUzE=";
        }*/
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
    }
}
