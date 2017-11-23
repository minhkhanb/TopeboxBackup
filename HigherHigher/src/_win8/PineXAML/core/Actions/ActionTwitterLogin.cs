using Pine.Dialogs;
using PineShap.Dialogs;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Windows.UI.Xaml.Controls;

namespace PineShap.Actions {
    class ActionTwitterLoginArgs : ActionArgs {
        public Grid ParentLayout = null;
        public LoginTwitterDialogCompleteArgs Result = null;
        public bool IsCancelByLogedin = false;
        public ActionTwitterLoginArgs(Grid layout) {
            ParentLayout = layout;
            Result = null;
        }
    }
    class ActionTwitterLogin:Action {
        Dictionary<LoginTwitterDialog, ActionTwitterLoginArgs> _waiting = new Dictionary<LoginTwitterDialog, ActionTwitterLoginArgs>();
        public override bool precheckToDo(ActionArgs e) {
            ActionTwitterLoginArgs arg = e as ActionTwitterLoginArgs;
            return arg.ParentLayout != null;
        }

        public override void Do(ActionArgs e) {
            Begin(e);
            ActionTwitterLoginArgs arg = e as ActionTwitterLoginArgs;
            if (precheckToDo(e)) {
                if (!TwitterUtils.TwitterClient.isAlreadyLoggedIn()) {
                    var dispatcher = ActionHelper.getCurrentUIThread();
                    dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
                    {
                        LoginTwitterDialog dialog = new LoginTwitterDialog();
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

        void dialog_OnComplete(object sender, LoginTwitterDialogCompleteArgs e) {
            LoginTwitterDialog dialog = sender as LoginTwitterDialog;
            if (dialog != null) {
                try {
                    ActionTwitterLoginArgs arg = _waiting[dialog];
                    arg.ParentLayout.Children.Remove(dialog.Dialog);
                    arg.Result = e;
                    Done(arg);
                } catch (Exception ex) {
                    //throw ex;
                }
            } else {
                Exception ex = new Exception("Twitter login dialog error");
                //throw ex;
            }
        }

        public override void Cancel(ActionArgs e) {
            ActionTwitterLoginArgs arg = e as ActionTwitterLoginArgs;
            if (arg != null) {
                try {
                    LoginTwitterDialog dialog = _waiting.FirstOrDefault(x => x.Value == arg).Key;
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
