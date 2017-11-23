using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Security.Cryptography;
using Windows.Security.Cryptography.Core;
using Windows.Storage.Streams;
using System.Runtime.InteropServices.WindowsRuntime;
namespace PineShap.Tools
{
    public class Security
    {
        /*public byte[] CryptographicEngineApp()
        {
            // Initialize the application.
            

            /////////////////////////////////////////////////////////////////////////////
            // Initialize input and output variables.
            String strMsg = String.Empty;           // Message string
            String strAlgName = String.Empty;       // Algorithm name
            UInt32 keyLength = 0;                   // Length of key
            BinaryStringEncoding encoding;          // Binary encoding type
            IBuffer iv;                             // Initialization vector
            CryptographicKey key;                   // Symmetric Key
            IBuffer buffNonce;                      // Buffer containing the nonce
            IBuffer buffMsg;                        // Buffer containing the message
            CryptographicKey hmacKey;               // HMAC key

            /////////////////////////////////////////////////////////////////////////////
            // Perform symmetric encryption and decryption.
            strMsg = "1234567812345678";
            strAlgName = SymmetricAlgorithmNames.AesCbc;
            keyLength = 32;

            IBuffer buffEncrypted = this.SampleCipherEncryption(
                strMsg,
                strAlgName,
                keyLength,
                out encoding,
                out iv,
                out key);

            this.SampleCipherDecryption(
                strAlgName,
                buffEncrypted,
                iv,
                encoding,
                key);

            /////////////////////////////////////////////////////////////////////////////
            // Encrypt and decrypt authenticated data.
            strMsg = "This is a message.";
            strAlgName = SymmetricAlgorithmNames.AesGcm;
            keyLength = 32;

            EncryptedAndAuthenticatedData objEncrypted = this.AuthenticatedEncryption(
                 strMsg,
                 strAlgName,
                 keyLength,
                 out encoding,
                 out buffNonce,
                 out key);

            this.AuthenticatedDecryption(
                 strAlgName,
                 key,
                 objEncrypted,
                 encoding,
                 buffNonce);

            /////////////////////////////////////////////////////////////////////////////
            // Derive Key material (Pbkdf2).
            strAlgName = KeyDerivationAlgorithmNames.Pbkdf2Sha256;
            keyLength = 32;

            String strHexKey = this.SampleDeriveFromPbkdf(
                strAlgName,
                keyLength);

            /////////////////////////////////////////////////////////////////////////////
            // Demonstrate signing and signature verification.
            strMsg = "This is a message to be signed.";
            strAlgName = MacAlgorithmNames.HmacSha384;

            IBuffer buffHMAC = this.SampleCreateHMAC(
                strMsg,
                strAlgName,
                out buffMsg,
                out hmacKey);

            this.SampleVerifyHMAC(
                buffMsg,
                hmacKey,
                buffHMAC);
        }*/
        static public byte[] encrypt(string id, string key)
        {
            BinaryStringEncoding encoding;
            IBuffer iv;
            IBuffer buffEncrypted = SampleCipherEncryption(
                id,
                SymmetricAlgorithmNames.AesCbcPkcs7,
                32,
                out encoding,
                out iv,
                key);
            return buffEncrypted.ToArray();
        }


        static public IBuffer SampleCipherEncryption(
            String strMsg,
            String strAlgName,
            UInt32 keyLength,
            out BinaryStringEncoding encoding,
            out IBuffer iv,
            string key)
        {
            CryptographicKey ckey;
            // Initialize the initialization vector.
            iv = null;

            // Initialize the binary encoding value.
            encoding = BinaryStringEncoding.Utf8;
            ////if (strMsg.Length < 32)
            ////{
            ////    for (int i = strMsg.Length; i < 32; i++)
             ////   {
             ////       strMsg += '\0';
            ////    }
            ////}
            // Create a buffer that contains the encoded message to be encrypted. 
            IBuffer buffMsg = CryptographicBuffer.ConvertStringToBinary(strMsg, encoding);
            
            // Open a symmetric algorithm provider for the specified algorithm. 
            SymmetricKeyAlgorithmProvider objAlg = SymmetricKeyAlgorithmProvider.OpenAlgorithm(strAlgName);
            //KeyDerivationAlgorithmProvider objKdfProv = KeyDerivationAlgorithmProvider.OpenAlgorithm(strAlgName);

            // Determine whether the message length is a multiple of the block length.
            // This is not necessary for PKCS #7 algorithms which automatically pad the
            // message to an appropriate length.
            if (!strAlgName.Contains("PKCS7"))
            {
                if ((buffMsg.Length % objAlg.BlockLength) != 0)
                {
                    throw new Exception("Message buffer length must be multiple of block length.");
                }
            }
            if (key.Length < 32)
            {
                for (int i = key.Length; i < 32; i++)
                {
                    key += '\0';
                }
            }
            // Create a symmetric key.
            ////IBuffer keyMaterial = CryptographicBuffer.GenerateRandom(keyLength);
            
            IBuffer keybuf = CryptographicBuffer.ConvertStringToBinary(key, BinaryStringEncoding.Utf8);
            ckey = objAlg.CreateSymmetricKey(keybuf);
            ////ckey = objKdfProv.CreateKey(CryptographicBuffer.ConvertStringToBinary(key, BinaryStringEncoding.Utf8));

            // CBC algorithms require an initialization vector. Here, a random
            // number is used for the vector.
            ////if (strAlgName.Contains("CBC"))
            ////{
            ////    iv = CryptographicBuffer.GenerateRandom(objAlg.BlockLength);
            ////}

            // Encrypt the data and return.
            IBuffer buffEncrypt = CryptographicEngine.Encrypt(ckey, buffMsg, iv);
            return buffEncrypt;
        }

        public void SampleCipherDecryption(
            String strAlgName,
            IBuffer buffEncrypt,
            IBuffer iv,
            BinaryStringEncoding encoding,
            CryptographicKey key)
        {
            // Declare a buffer to contain the decrypted data.
            IBuffer buffDecrypted;

            // Open an symmetric algorithm provider for the specified algorithm. 
            SymmetricKeyAlgorithmProvider objAlg = SymmetricKeyAlgorithmProvider.OpenAlgorithm(strAlgName);

            // The input key must be securely shared between the sender of the encrypted message
            // and the recipient. The initialization vector must also be shared but does not
            // need to be shared in a secure manner. If the sender encodes a message string 
            // to a buffer, the binary encoding method must also be shared with the recipient.
            buffDecrypted = CryptographicEngine.Decrypt(key, buffEncrypt, iv);

            // Convert the decrypted buffer to a string (for display). If the sender created the
            // original message buffer from a string, the sender must tell the recipient what 
            // BinaryStringEncoding value was used. Here, BinaryStringEncoding.Utf8 is used to
            // convert the message to a buffer before encryption and to convert the decrypted
            // buffer back to the original plaintext.
            String strDecrypted = CryptographicBuffer.ConvertBinaryToString(encoding, buffDecrypted);
        }
    }
}
