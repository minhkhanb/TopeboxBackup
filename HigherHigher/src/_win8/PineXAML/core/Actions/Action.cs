using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PineShap.Actions {
    public delegate void DelegateBegin(object sender, ActionArgs e);
    public delegate void DelegateDone(object sender, ActionArgs e);
    public delegate void DelegateCancel(object sender, ActionArgs e);
    public abstract class Action {
        public event DelegateBegin OnBegin;
        public event DelegateDone OnDone;
        public event DelegateCancel OnCancel;
        protected void Begin(ActionArgs e) {
            if (OnBegin != null) OnBegin(this, e);
        }
        protected void Done(ActionArgs e) {
            if (OnDone != null) OnDone(this, e);
        }
        public virtual void Cancel(ActionArgs e) {
            if (OnCancel != null) OnCancel(this, e);
        }
        public abstract bool precheckToDo(ActionArgs e);
        public abstract void Do(ActionArgs e);
    }
    public class FakeActionArgs:ActionArgs
    {
        public object AlterObject;
    }
    public class FakeAction:Action
    {
        public override bool precheckToDo(ActionArgs e)
        {
            return true;
        }

        public override void Do(ActionArgs e)
        {
            
        }
        
    }
}
