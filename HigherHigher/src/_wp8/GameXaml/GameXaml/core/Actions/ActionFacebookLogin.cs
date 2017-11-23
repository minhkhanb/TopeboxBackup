using FacebookUtils;
using Pine.Dialogs;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;

namespace Pine.Actions {
    class ActionFacebookLoginArgs : ActionArgs {
        public FacebookAppInfomation AppInfo = null;
        public Grid ParentLayout = null;
        public LoginFacebookDialogCompleteArgs Result = null;
        public bool IsCancelByLogedin = false;
        public ActionFacebookLoginArgs(Grid layout) {
            ParentLayout = layout;
            Result = null;
        }
    }
    class ActionFacebookLogin:Action {
        Dictionary<LoginFacebookDialog, ActionFacebookLoginArgs> _waiting = new Dictionary<LoginFacebookDialog, ActionFacebookLoginArgs>();
        public override bool precheckToDo(ActionArgs e) {
            ActionFacebookLoginArgs arg = e as ActionFacebookLoginArgs;
            return arg.ParentLayout != null;
        }

        public override void Do(ActionArgs e) {
            Begin(e);
            ActionFacebookLoginArgs arg = e as ActionFacebookLoginArgs;
            if (precheckToDo(e)) {
                if (!FacebookClient.Instance.IsUserLogedin()) {
                    Deployment.Current.Dispatcher.BeginInvoke(() => {
                        LoginFacebookDialog dialog = new LoginFacebookDialog(arg.AppInfo);
                        dialog.OnComplete += dialog_OnComplete;
                        arg.ParentLayout.Children.Add(dialog.Dialog);
                        dialog.show(0, 0);
                        _waiting.Add(dialog, arg);
                    });
                } else {
                    arg.IsCancelByLogedin = true;
                    Done(arg);
                }
            } else {
                Cancel(e);
            }
        }

        void dialog_OnComplete(object sender, LoginFacebookDialogCompleteArgs e) {
            LoginFacebookDialog dialog = sender as LoginFacebookDialog;
            if (dialog != null) {
                try {
                    ActionFacebookLoginArgs arg = _waiting[dialog];
                    arg.ParentLayout.Children.Remove(dialog.Dialog);
                    arg.Result = e;
                    Done(arg);
                } catch (Exception ex) {
                    //throw ex;
                }
            } else {
                Exception ex = new Exception("Facebook login dialog error");
                //throw ex;
            }
        }

        public override void Cancel(ActionArgs e) {
            ActionFacebookLoginArgs arg = e as ActionFacebookLoginArgs;
            if (arg != null) {
                try {
                    LoginFacebookDialog dialog = _waiting.FirstOrDefault(x => x.Value == arg).Key;
                    arg.ParentLayout.Children.Remove(dialog.Dialog);
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
