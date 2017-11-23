using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml.Controls.Primitives;

namespace PineShap.Dialogs
{
    public abstract class DialogBase
    {
        abstract public Popup Dialog { get; }
    }
}
