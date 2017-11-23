using PineShap.Object;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.ApplicationModel.Store;
using Windows.UI.Xaml.Controls;

namespace PineShap {
    class AppObject {
        public static AccountInfo AccInfo = new AccountInfo();
        public static GameInfomation GameInfo = null;
        public static ClientService ClientSvc = null;
        public static void init(GameInfomation game_info, ClientService service){
            GameInfo = game_info;
            ClientSvc = service;
        }
        public static string AppLink {
            get { return "ms-windows-store:Publisher?name=" + CurrentApp.AppId; }
        }
        public static Grid RootLayout = null;
        public static List<Actions.ActionArgLinker> PopupOfAction = new List<Actions.ActionArgLinker>();
    }
}
