using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PineShap.Actions
{
    public class ActionHelper
    {
        static Windows.UI.Core.CoreDispatcher UIThread;
        static public Windows.UI.Core.CoreDispatcher getCurrentUIThread()
        {
            return UIThread;
        }
        static public void registeUIThread(Windows.UI.Core.CoreDispatcher thread){
            UIThread = thread;
        }
    }
}
