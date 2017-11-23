using FacebookUtils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pine.Object {
    abstract class GameInfomation {
        public abstract bool IsRequireAccount { get; }
        //public abstract string FacebookAppId { get; }
        //public abstract string FacebookAppSecretKey { get; }
        public abstract string FacebookFeedTitle { get; }
        public abstract string FacebookFeedAttahLink { get; }
        public abstract string FacebookFeedImageUrl { get; }
        //public abstract string FacebookPermission { get; }
        //public abstract string FacebookRedirectLink { get; }

        public abstract string TwitterConsumerKey { get; }
        public abstract string TwitterConsumerSecret { get; }
        public abstract List<Uri> UriRequire { get; }
        public abstract void changeNetworkStatus(Uri foruri, bool status);
        public abstract bool AskForCreateAccount { get; }
        public abstract string AccountDialogTitle { get; }
        public abstract string AccountDialogMessage { get; }
        
        public abstract FacebookAppInfomation FacebookApp{get;}

        public abstract AccountAskDialogType AccountDialogType { get; }
    }
}
