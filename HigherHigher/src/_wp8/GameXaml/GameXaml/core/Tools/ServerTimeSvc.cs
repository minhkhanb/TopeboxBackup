using FacebookUtils;
using PhoneDirect3DXamlAppComponent;
using Pine;
using Pine.Object;
using System;
using System.Collections.Generic;
//using System.Data.Services.Client;
using System.IO.IsolatedStorage;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pine.Tools {

    public delegate void DelegateNeedUpdateServerTime(object sender, EventArgs e );
    public class ServerTimeSvc {
        public event DelegateNeedUpdateServerTime OnNeedUpdateTime;
        public void UpdateServerTime() {
            if (OnNeedUpdateTime != null)
            {
                OnNeedUpdateTime(this, new EventArgs());
            }
        }
        
        public void CalculateDeltaTime() {
            IsolatedStorageSettings appSettings = IsolatedStorageSettings.ApplicationSettings;
            if(appSettings.Contains("app_time")){
                DateTime time = (DateTime)appSettings["app_time"];
                TimeSpan delta = DateTime.Now - time;
                int deltaseconds = (int)delta.TotalSeconds;
                ApplicationInfomation.DeltaTimeFromLastRun = deltaseconds;
                if (DateTime.Now > time) {
                    appSettings["app_time"] = DateTime.Now;
                }
            } else {
                appSettings.Add("app_time",DateTime.Now);
                ApplicationInfomation.DeltaTimeFromLastRun = 0;
            }
        }
    }
}
