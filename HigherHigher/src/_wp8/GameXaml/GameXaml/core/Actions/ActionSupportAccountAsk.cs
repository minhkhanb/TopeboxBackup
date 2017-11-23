using Pine.Dialogs;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace Pine.Actions
{
    class ActionSupportAccountAskArgs : ActionArgs
    {
        public Grid ParentLayout = null;
        public string AccountId;
    }
    class ActionSupportAccountAsk:Action
    {
        Dictionary<SupportAccountAskDialog, ActionSupportAccountAskArgs> _waiting = new Dictionary<SupportAccountAskDialog, ActionSupportAccountAskArgs>();
        public override bool precheckToDo(ActionArgs e)
        {
            return true;
        }

        public override void Do(ActionArgs e)
        {
            if (precheckToDo(e))
            {
                Begin(e);
                ActionSupportAccountAskArgs arg = e as ActionSupportAccountAskArgs;
                Deployment.Current.Dispatcher.BeginInvoke(() =>
                {
                    SupportAccountAskDialog dialog = new SupportAccountAskDialog();
                    dialog.OnComplete += dialog_OnComplete;
                    arg.ParentLayout.Children.Add(dialog.Dialog);
                    _waiting.Add(dialog, arg);
                    dialog.show();

                });
            }
            else
            {
                Cancel(e);
            }
        }

        private void dialog_OnComplete(object sender, SupportAccountAskEventArgs e)
        {
            SupportAccountAskDialog dialog = sender as SupportAccountAskDialog;
            if (dialog != null)
            {
                try
                {
                    ActionSupportAccountAskArgs arg = _waiting[dialog];
                    _waiting.Remove(dialog);
                    arg.ParentLayout.Children.Remove(dialog.Dialog);
                    arg.AccountId = e.AccountId;
                    Done(arg);
                }
                catch (Exception ex)
                {
                    //throw ex;
                }
            }
            else
            {
                //throw ex;
            }
        }
    }
}
