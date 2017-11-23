using PineShap.Tools;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace PineShap.Actions {
    class ActionDownloadFileArg : ActionArgs {
        public string FileUrl;
        public string FileSaveName;
        public ActionDownloadFileArg(string furl, string fsavename) {
            FileUrl = furl;
            FileSaveName = fsavename;
        }
    }
    class ActionDownloadFile:Action {
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

        async void download(ActionDownloadFileArg arg) {
            HttpClient wclient = new HttpClient();
            byte[] data = await wclient.GetByteArrayAsync(arg.FileUrl);
            using (Stream s = await FileManager.GetFileStreamForWrite(arg.FileSaveName))
            {
                s.Write(data, 0, data.Length);
                s.Dispose();
                
                Done(arg);
                return;
            }
            Cancel(arg);
        }
    }
}
