using Microsoft.Phone.Info;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.NetworkInformation;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace Pine.Tools
{
    public class Utils
    {
        static public string EnscryptString(string id)
        {
            string rs = "";
            byte[] keybytes = new byte[32];
            string key = OdataServerDefine.Secret;
            byte[] tmp_keybytes = UTF8Encoding.UTF8.GetBytes(key);
            AesManaged aes = new AesManaged();
            byte[] iv = new byte[aes.BlockSize / 8];
            try
            {
                Array.Resize(ref tmp_keybytes, aes.KeySize / 8);
                keybytes = tmp_keybytes;
                aes.Key = keybytes;
                aes.IV = iv;
                ICryptoTransform aes_transform = aes.CreateEncryptor(aes.Key, aes.IV);
                // Create the streams used for encryption. 
                using (MemoryStream msEncrypt = new MemoryStream())
                {
                    using (CryptoStream csEncrypt = new CryptoStream(msEncrypt, aes_transform, CryptoStreamMode.Write))
                    {
                        using (StreamWriter swEncrypt = new StreamWriter(csEncrypt))
                        {
                            swEncrypt.Write(id);
                        }
                        rs = Convert.ToBase64String(msEncrypt.ToArray());
                    }
                }
            }
            catch (Exception e)
            {
            }
            return rs;
        }
        public static byte[] GetDeviceUniqueID()
        {
            byte[] result = null;
            object uniqueId;
            if (DeviceExtendedProperties.TryGetValue("DeviceUniqueId", out uniqueId))
                result = (byte[])uniqueId;
            return result;
        }
        public static void sendToLogServer(byte[] content){
            WebClient wc = new WebClient();
            string base64 = Convert.ToBase64String(content);
            wc.UploadStringAsync(new Uri("http://192.168.1.40/test1.php"), base64);
        }
    }
}
