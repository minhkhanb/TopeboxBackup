using PineShap;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using Windows.UI.Xaml.Controls;


namespace PineShap.Actions {
    class ActionAccountAskArg : ActionArgs {
        public Grid ParentLayout = null;
        public AccountAskDialogArgs Result = null;
        public bool IsRequire = false;
        public ActionAccountAskArg(Grid layout) {
            ParentLayout = layout;
        }
    }
    class ActionAccountAsk:Action {
        Dictionary<Dialogs.DialogBase, ActionAccountAskArg> _waiting = new Dictionary<Dialogs.DialogBase, ActionAccountAskArg>();
        public override bool precheckToDo(ActionArgs e) {
            return true;
        }
        void show(ActionAccountAskArg arg)
        {
            var dispatcher = ActionHelper.getCurrentUIThread();
            dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
            {
                AccountAskDialog dialog = null;
                if (arg.IsRequire)
                {
                    dialog = new AccountAskDialog(AccountAskDialogType.TypeOk);
                }
                else
                {
                    dialog = new AccountAskDialog(AppObject.GameInfo.AccountDialogType);
                }
                dialog.OnComplete += dialog_OnComplete;
                arg.ParentLayout.Children.Add(dialog.Dialog);
                dialog.show();
                _waiting.Add(dialog, arg);
            });
        }
        public override void Do(ActionArgs e) {
            Begin(e);
            if (precheckToDo(e)) {
                ActionAccountAskArg arg = e as ActionAccountAskArg;
                show(arg);
            } else {
                Cancel(e);
            }
        }

        void dialog_OnComplete(object sender, AccountAskDialogArgs e) {
            AccountAskDialog dialog = sender as AccountAskDialog;
            if (dialog != null) {
                try {
                    ActionAccountAskArg arg = _waiting[dialog];
                    arg.ParentLayout.Children.Remove(dialog.Dialog);
                    arg.Result = e;
                    Done(arg);
                } catch (Exception ex) {
                    //throw ex;
                }
            } else {
                Exception ex = new Exception("Action Ask dialog error");
                //throw ex;
            }
        }

        public override void Cancel(ActionArgs e) {
            bool cancel_base = false;
            ActionAccountAskArg arg = e as ActionAccountAskArg;
            
            if (arg != null) {
                try {
                    var dialog = _waiting.FirstOrDefault(x => x.Value == arg).Key;
                    arg.ParentLayout.Children.Remove(dialog.Dialog);
                    arg.Result = null;
                    if (arg.IsRequire)
                    {
                        cancel_base = true;
                        ////Deployment.Current.Dispatcher.BeginInvoke(() =>
                        ////{
                            AccountRequireDialog notify_dialog = new AccountRequireDialog();
                            notify_dialog.OnComplete += notify_dialog_OnComplete;
                            _waiting.Add(notify_dialog, arg);
                            arg.ParentLayout.Children.Add(notify_dialog.Dialog);
                            notify_dialog.show();
                        ////});
                    }
                } catch (Exception ex) {
                    //throw ex;
                }
            } else {
                Exception ex = new Exception("Action Ask dialog error");
                //throw ex;
            }
            if (!cancel_base)
            {
                base.Cancel(e);
            }
        }

        void notify_dialog_OnComplete(object sender, AccountRequireDialogArgs a)
        {
            var dialog = sender as Dialogs.DialogBase;
            var arg = _waiting[dialog];
            arg.ParentLayout.Children.Remove(dialog.Dialog);
            ////Deployment.Current.Dispatcher.BeginInvoke(() =>
            ////{
                AccountAskDialog askdialog = null;
                if (arg.IsRequire)
                {
                    askdialog = new AccountAskDialog(AccountAskDialogType.TypeOk);
                }
                else
                {
                    askdialog = new AccountAskDialog(AppObject.GameInfo.AccountDialogType);
                }
                askdialog.OnComplete += dialog_OnComplete;
                arg.ParentLayout.Children.Add(askdialog.Dialog);
                askdialog.show();
                _waiting.Add(askdialog, arg);
            ////});
        }

        
    }
}
