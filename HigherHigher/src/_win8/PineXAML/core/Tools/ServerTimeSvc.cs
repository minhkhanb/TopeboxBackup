using PineShap.FacebookUtils;
using Pine;
using PineShap.Object;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pine.Tools {
    public class ServerTimeSvc {
        //static ODataService<Pine.PockerReal.PocketVideoPockerContext> m_odata_pocket_service = null;
        //static DataServiceCollection<Pine.PockerReal.ServerInfoDto> _dataservice_serverinfo = null;
        static public void UpdateServerTime() {
            //if (_dataservice_serverinfo == null) {
            //    _dataservice_serverinfo = new DataServiceCollection<Pine.PockerReal.ServerInfoDto>();
            //    _dataservice_serverinfo.LoadCompleted += new EventHandler<LoadCompletedEventArgs>(handler_server_info_complete);
            //}
            //_dataservice_serverinfo.Clear();
            //if (m_odata_pocket_service == null) {
            //    m_odata_pocket_service = new ODataService<Pine.PockerReal.PocketVideoPockerContext>(OdataServerDefine.UrlServerReal);
            //}
            //UInt64 id = 3000000000000000000;
            //m_odata_pocket_service.setUserId(id);
            //var query = from t in m_odata_pocket_service._context.ServerInfos select t;
            //_dataservice_serverinfo.LoadAsync(query);
        }
        ////static private void handler_server_info_complete(object sender, LoadCompletedEventArgs e) {
            //if (e.Error == null) {
            //    if (_dataservice_serverinfo != null && _dataservice_serverinfo.Count > 0) {
            //        Pine.PockerReal.ServerInfoDto sinfo = _dataservice_serverinfo[0];
            //        //DateTime server_utctime = sinfo.UtcNow;
            //        //ApplicationInfomation.HaveServerTime = true;
            //        //TimeSpan span = DateTime.UtcNow - server_utctime;
            //        //ApplicationInfomation.DeltaServerTimeWithNow = (int)span.TotalSeconds;
            //    } else {
                    
            //    }
            //} else {
            //}
        ////}
        static public void CalculateDeltaTime() {
            /*IsolatedStorageSettings appSettings = IsolatedStorageSettings.ApplicationSettings;
            if(appSettings.Contains("app_time")){
                DateTime time = (DateTime)appSettings["app_time"];
                TimeSpan delta = DateTime.Now - time;
                int deltaseconds = (int)delta.TotalSeconds;
                //ApplicationInfomation.DeltaTimeFromLastRun = deltaseconds;
                if (DateTime.Now > time) {
                    appSettings["app_time"] = DateTime.Now;
                }
            } else {
                appSettings.Add("app_time",DateTime.Now);
                //ApplicationInfomation.DeltaTimeFromLastRun = 0;
            }*/
        }
    }
}
