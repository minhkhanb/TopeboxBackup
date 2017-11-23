using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pine
{
    public class Transaction
    {
        public ConnectConstructor Instructor = ConnectConstructor.INVALID;
        public ConnectConstructor NextInstructor = ConnectConstructor.INVALID;
        public object NextHandler = null;
        public object Data = null;
        public object FailHandler = null;
    }
}
