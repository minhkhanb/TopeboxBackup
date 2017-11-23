using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pine.TwitterUtils
{
    class TwitterTransaction
    {
        public bool Finish = false;
        public bool Success = false;
        public ConnectConstructor NextInstructor = ConnectConstructor.INVALID;
    }
}
