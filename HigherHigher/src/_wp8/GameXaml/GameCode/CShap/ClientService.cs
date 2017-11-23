using PhoneDirect3DXamlAppComponent;
using Pine.Object;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Threading;
using System.Data.Services.Client;
using System.Windows;
using System.Security.Cryptography;
using System.IO;
namespace Pine {

    public class OdataServerDefine {
        public const string UrlServerReal = "http://odata.pvp.pine-entertainment.com/api/PocketVideoPocker";
        public const string Secret = "pineEntertainment";
    }
    public class CurrentGameServiceRequest:ClientServiceRequest {
        public CurrentGameServiceRequest(int request_code)
            : base(request_code)
        {
        }
        public static ClientServiceRequest Exsample = new ClientServiceRequest(1001);
    }
    public class CurrentGameService:ClientService {
        public override string OuthorizeString(string id) {
            string rs = "";
            byte[] keybytes = new byte[32];
            string key = OdataServerDefine.Secret;
            byte[] tmp_keybytes = UTF8Encoding.UTF8.GetBytes(key);
            AesManaged aes = new AesManaged();
            byte[] iv = new byte[aes.BlockSize / 8];
            try {
                Array.Resize(ref tmp_keybytes, aes.KeySize / 8);
                keybytes = tmp_keybytes;
                aes.Key = keybytes;
                aes.IV = iv;
                ICryptoTransform aes_transform = aes.CreateEncryptor(aes.Key, aes.IV);
                // Create the streams used for encryption. 
                using (MemoryStream msEncrypt = new MemoryStream()) {
                    using (CryptoStream csEncrypt = new CryptoStream(msEncrypt, aes_transform, CryptoStreamMode.Write)) {
                        using (StreamWriter swEncrypt = new StreamWriter(csEncrypt)) {
                            swEncrypt.Write(id);
                        }
                        rs = Convert.ToBase64String(msEncrypt.ToArray());
                    }
                }
            } catch (Exception e) {
            }
            return rs;
        }

        public CurrentGameService()
        {
            
        }
        public override void process(ClientServiceRequest request, ServerStream svstream) {
            if (request == CurrentGameServiceRequest.Exsample)
            {
                
            } else {
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }
    }
}