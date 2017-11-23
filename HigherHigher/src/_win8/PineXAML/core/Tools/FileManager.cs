using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Storage;

namespace PineShap.Tools
{
    public class FileManager
    {
        static async public Task<bool> FileExists(string filename)
        {
            try
            {
                StorageFile file = await ApplicationData.Current.LocalFolder.GetFileAsync(filename);
                //var f = await KnownFolders.DocumentsLibrary.GetFileAsync("Pine/Avenger/" + filename);
                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
        }
        
        static async public Task<Stream> GetFileStream(string filename)
        {
            try
            {
                var f = await ApplicationData.Current.LocalFolder.OpenStreamForReadAsync(filename);
                return f;
            }
            catch (Exception ex)
            {
                return null;
            }
        }
        static async public Task<Stream> GetFileStreamForWrite(string filename)
        {
            try
            {
                var f = await ApplicationData.Current.LocalFolder.OpenStreamForWriteAsync(filename, CreationCollisionOption.ReplaceExisting);
                return f;
            }
            catch (Exception ex)
            {
                return null;
            }
        }
        static public DateTime GetVitualCreateDate(string filename)
        {
            ApplicationDataContainer settings = Windows.Storage.ApplicationData.Current.LocalSettings;
            if (settings.Containers.ContainsKey("FileAttribute"))
            {
                string key = "FILE-" + filename;
                try
                {
                    return (DateTime)settings.Containers["FileAttribute"].Values[key];
                }
                catch (Exception ex)
                {
                    
                }
            }
            return new DateTime(0);
        }
        static public void SetVitualCreateDate(string filename, DateTime time)
        {
            ApplicationDataContainer settings = Windows.Storage.ApplicationData.Current.LocalSettings;
            if (!settings.Containers.ContainsKey("FileAttribute"))
            {
                settings.CreateContainer("FileAttribute", Windows.Storage.ApplicationDataCreateDisposition.Always);
            }
            string key = "FILE-" + filename;
            if (settings.Containers.ContainsKey("FileAttribute"))
            {
                if (settings.Containers["FileAttribute"].Values.ContainsKey(key))
                {
                    settings.Containers["FileAttribute"].Values[key] = time;
                }
                else
                {
                    settings.Containers["FileAttribute"].Values.Add(key, time);
                }
            }
        }
        static async public void OptimizeVitualCreateDate()
        {
            ApplicationDataContainer settings_container = Windows.Storage.ApplicationData.Current.LocalSettings;
            if (settings_container.Containers.ContainsKey("FileAttribute"))
            {
                var settings = settings_container.Containers["FileAttribute"];
                List<string> cleanlist = new List<string>();
                foreach (KeyValuePair<string, object> item in settings.Values)
                {
                    if (item.Key.IndexOf("FILE-") == 0)
                    {
                        if (item.Key.Length >= 5)
                        {
                            string filename = item.Key.Substring(5);
                            bool exits = await FileExists(filename);
                            if (!exits)
                            {
                                cleanlist.Add(item.Key);
                            }
                        }
                        else
                        {
                            cleanlist.Add(item.Key);
                        }
                    }
                }
                if (cleanlist.Count > 0)
                {
                    foreach (string key in cleanlist)
                    {
                        settings.Values.Remove(key);
                    }
                }
            }
        }
    }
}
