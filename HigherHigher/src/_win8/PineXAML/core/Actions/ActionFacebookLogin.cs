using PineShap.FacebookUtils;
using Pine.Dialogs;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Windows.UI.Xaml.Controls;
using PineShap.Dialogs;
using Facebook.Client;

namespace PineShap.Actions {
    class ActionFacebookLoginArgs : ActionArgs {
        public Grid ParentLayout = null;
        public LoginFacebookDialogCompleteArgs Result = null;
        public bool IsCancelByLogedin = false;
        public ActionFacebookLoginArgs(Grid layout) {
            ParentLayout = layout;
            Result = null;
        }
    }
    class ActionFacebookLogin:Action {
        private FacebookSession session;
        public override bool precheckToDo(ActionArgs e) {
            ActionFacebookLoginArgs arg = e as ActionFacebookLoginArgs;
            return arg.ParentLayout != null;
        }
        private async Task Authenticate(ActionFacebookLoginArgs arg)
        {
            string message = String.Empty;
            try
            {

                session = await FacebookManager.SessionClient.LoginAsync("basic_info,publish_actions,user_about_me,read_stream,offline_access");
                string state = session.State;
                if (session.AccessToken.Length > 0)
                {
                    state = session.State;
                    FacebookManager.setToken(session.AccessToken);
                    arg.Result = new LoginFacebookDialogCompleteArgs(true,session.AccessToken, session.Expires.ToString());
                    Done(arg);
                }
            }
            catch (InvalidOperationException e)
            {
                Cancel(arg);
            }
        }

        public override void Do(ActionArgs e) {
            Begin(e);
            ActionFacebookLoginArgs arg = e as ActionFacebookLoginArgs;
            if (precheckToDo(e)) {
                var dispatcher = ActionHelper.getCurrentUIThread();
                dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
                {
                    Authenticate(arg);

                });
                /*if (!FacebookClient.Instance.IsUserLogedin()) {
                    var dispatcher = ActionHelper.getCurrentUIThread();
                    dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
                    {
                        Authenticate(arg);
                        
                    });
                } else {
                    arg.IsCancelByLogedin = true;
                    Done(arg);
                }*/
            } else {
                Cancel(e);
            }
        }

        

        public override void Cancel(ActionArgs e) {
            ActionFacebookLoginArgs arg = e as ActionFacebookLoginArgs;
            if (arg != null) {
                try {
                    arg.Result = null;
                } catch (Exception ex) {
                    //throw ex;
                }
            } else {
                Exception ex = new Exception("Facebook login dialog error");
                //throw ex;
            }
            base.Cancel(e);
        }
    }
}
