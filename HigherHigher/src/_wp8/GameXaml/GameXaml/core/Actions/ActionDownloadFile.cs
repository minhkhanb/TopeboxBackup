using System;
using System.Collections.Generic;
using System.IO;
using System.IO.IsolatedStorage;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Imaging;
using System.Windows.Resources;

namespace Pine.Actions {
    class ActionDownloadFileArg : ActionArgs {
        public string FileUrl;
        public string FileSaveName;
        public ActionDownloadFileArg(string furl, string fsavename) {
            FileUrl = furl;
            FileSaveName = fsavename;
        }
    }
    class ActionDownloadFile:Action {
        Dictionary<WebClient, ActionDownloadFileArg> _waiting = new Dictionary<WebClient, ActionDownloadFileArg>();
        public override bool precheckToDo(ActionArgs e) {
            ActionDownloadFileArg arg = e as ActionDownloadFileArg;
            return (arg != null && arg.FileUrl.Length > 0 && arg.FileSaveName.Length > 0);
        }

        public override void Do(ActionArgs e) {
            Begin(e);
            if (precheckToDo(e)) {
                download(e as ActionDownloadFileArg);
            } else {
                Cancel(e);
            }
        }

        void download(ActionDownloadFileArg arg) {
            WebClient wclient = new WebClient();
            _waiting.Add(wclient, arg);
            wclient.OpenReadCompleted += wclient_OpenReadCompleted;
            wclient.OpenReadAsync(new Uri(arg.FileUrl, UriKind.Absolute));
        }

        private void wclient_OpenReadCompleted(object sender, OpenReadCompletedEventArgs e) {
            Deployment.Current.Dispatcher.BeginInvoke(() => {
                if(_waiting.ContainsKey((WebClient)sender)){
                    ActionDownloadFileArg arg = _waiting[(WebClient)sender];
                    try {
                        var streamResourceInfo = new StreamResourceInfo(e.Result, null);
                        var userStoreForApplication = IsolatedStorageFile.GetUserStoreForApplication();
                        if (userStoreForApplication.FileExists(arg.FileSaveName)) {
                            userStoreForApplication.DeleteFile(arg.FileSaveName);
                        }
                        using(IsolatedStorageFileStream isolatedStorageFileStream = userStoreForApplication.CreateFile(arg.FileSaveName)){
                            var buf = new byte[1024];
                            int readcount = 0;
                            while((readcount = streamResourceInfo.Stream.Read(buf,0,buf.Length))!= 0){
                                isolatedStorageFileStream.Write(buf,0,readcount);
                            }
                        }
                        Done(arg);
                    }  catch (Exception ex) {
                        Cancel(arg);
                    }
                }
            });
        }
    }
}
