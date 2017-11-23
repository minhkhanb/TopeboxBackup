using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls.Primitives;

namespace Pine.Dialogs
{
    public abstract class DialogBase
    {
        abstract public Popup Dialog { get; }
    }
}
