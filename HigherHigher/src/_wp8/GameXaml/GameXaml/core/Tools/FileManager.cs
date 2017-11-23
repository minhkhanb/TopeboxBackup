using System;
using System.Collections.Generic;
using System.IO;
using System.IO.IsolatedStorage;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pine.Tools
{
    public class FileManager
    {
        static public bool FileExists(string filename)
        {
            var userStoreForApplication = IsolatedStorageFile.GetUserStoreForApplication();
            return (userStoreForApplication.FileExists(filename));
        }
        static public FileStream GetFileStream(string filename)
        {
            var userStoreForApplication = IsolatedStorageFile.GetUserStoreForApplication();
            try
            {
                IsolatedStorageFileStream isofs = userStoreForApplication.OpenFile(filename, FileMode.Open);
                return isofs;
            }
            catch (Exception ex)
            {

            }
            return null;
        }
        static public Stream GetFileStreamForWrite(string filename)
        {
            var userStoreForApplication = IsolatedStorageFile.GetUserStoreForApplication();
            try
            {
                IsolatedStorageFileStream isofs = userStoreForApplication.OpenFile(filename, FileMode.OpenOrCreate);
                return isofs;
            }
            catch (Exception ex)
            {

            }
            return null;
        }
        static public DateTime GetVitualCreateDate(string filename)
        {
            var settings = IsolatedStorageSettings.ApplicationSettings;
            string key = "FILE-" + filename;
            if (settings.Contains(key))
            {
                return (DateTime)settings[key];
            }
            return new DateTime(0);
        }
        static public void SetVitualCreateDate(string filename, DateTime time)
        {
            string key = "FILE-" + filename;
            var settings = IsolatedStorageSettings.ApplicationSettings;
            if (settings.Contains(key))
            {
                settings[key] = time;
            }
            else
            {
                settings.Add(key, time);
            }
            settings.Save();
        }
        static public void OptimizeVitualCreateDate()
        {
            var settings = IsolatedStorageSettings.ApplicationSettings;
            List<string> cleanlist = new List<string>();
            foreach(KeyValuePair<string,object> item in settings)
            {
                if(item.Key.IndexOf("FILE-") == 0)
                {
                    if(item.Key.Length >= 5)
                    {
                        string filename = item.Key.Substring(5);
                        if(!FileExists(filename)){
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
                foreach(string key in cleanlist)
                {
                    settings.Remove(key);
                }
                settings.Save();
            }
        }
    }
}
