using Pine;
using PineShap.Object;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace PineShap {
    class CurrentGameInfomation : GameInfomation {
        List<Uri> _require_uri = new List<Uri>();
        public CurrentGameInfomation() {
            _require_uri.Add(new Uri("https://m.facebook.com"));
            //_require_uri.Add(new Uri("http://odata.pvp.pine-entertainment.com"));
        }
        public override string FacebookAppId {
            get { return "537695042944353"; }
        }
        public override string FacebookAppSecretKey {
            get { return "5b8d632b34067ce2fa14cd9d018658d4"; }
        }

        public override string TwitterConsumerKey {
            get { return "O1rade9oSQKYijhSxqQQoA"; }
        }

        public override string TwitterConsumerSecret {
            get { return "JWJkJDpVf6nZYimNOfCCqfXmNSz15hTWxeKOGoZ4"; }
        }

        public override string FacebookFeedTitle {
            get { return "Pocket Avenger for Windows Phone."; }
        }

        public override string FacebookFeedAttahLink {
            get { return "http://pine-entertainment.com"; }
        }

        public override string FacebookFeedImageUrl {
            get { return "https://youarmy.blob.core.windows.net/icongame/pocketavenger2.png"; }
        }

        public override List<Uri> UriRequire {
            get { return _require_uri; }
        }

        public override void changeNetworkStatus(Uri foruri, bool status) {
            
        }

        public override bool AskForCreateAccount {
            get { return true; }
        }


        public override string AccountDialogTitle {
            get { return "Facebook Login"; }
        }

        public override string AccountDialogMessage {
            get { return "You must play Pocket Avenger with Facebook account. We will be backing up your data online via your Facebook account."; }
        }

        public override bool IsRequireAccount
        {
            get { return false; }
        }

        public override AccountAskDialogType AccountDialogType
        {
            get { return AccountAskDialogType.TypeOkLate; }
        }
    }
}