using FacebookUtils;
using PhoneDirect3DXamlAppComponent;
using Pine.Object;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Pine {
    class CurrentGameInfomation : GameInfomation {
        List<Uri> _require_uri = new List<Uri>();
        FacebookAppInfomation _fb_app = new FacebookAppInfomation()
        {
            AppId = "141399492731036",
            AppSecret = "d4fd6152993bf81fcce1bc6b4f848434",
            RedirectLink = "http://www.pine-entertainment.com/index.php",
            RequirePermission = "publish_stream,publish_actions,user_about_me,read_stream,user_friends,user_photos"
        };
        public CurrentGameInfomation()
        {
            _require_uri.Add(new Uri("https://m.facebook.com"));
            //_require_uri.Add(new Uri("http://odata.pvp.pine-entertainment.com"));
        }
        
        public override string TwitterConsumerKey {
            get { return "CJBPRdBOUqvpitvm5fhOQw"; }
        }

        public override string TwitterConsumerSecret {
            get { return "VuMuSYpkco0q359WxohLNYcyUYWq5UTtMu6fko10u4Q"; }
        }

        public override string FacebookFeedTitle {
            get { return "Pocket Video Poker for Windows Phone."; }
        }

        public override string FacebookFeedAttahLink {
            get { return "http://pine-entertainment.com"; }
        }

        public override string FacebookFeedImageUrl {
            get { return "http://youarmy.blob.core.windows.net/icongame/videopocker.png"; }
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
            get { return "We strongly recommend you to play Video Pocker with Facebook account. We will be backing up your data online via your Facebook account."; }
        }

        public override bool IsRequireAccount
        {
            get { return false; }
        }

        public override AccountAskDialogType AccountDialogType
        {
            get { return AccountAskDialogType.TypeOkLateNever; }
        }

        
        public override global::FacebookUtils.FacebookAppInfomation FacebookApp
        {
            get { return _fb_app; }
        }
    }
}