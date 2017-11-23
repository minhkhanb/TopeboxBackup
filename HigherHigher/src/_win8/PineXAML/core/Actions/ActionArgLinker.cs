using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PineShap.Actions {
    public class ActionArgLinker {
        public Action AttachAction = null;
        public ActionArgs AttachArg = null;
        public ActionArgLinker(Action act, ActionArgs arg) {
            AttachAction = act;
            AttachArg = arg;
        }
    }
}
