using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pine.core.FacebookUtils
{
    public class AvatarRedirectItem
    {
        public string url { get; set; }
        public int width { get; set; }
        public int height { get; set; }
        public bool is_silhouette { get; set; }
    }
    public class AvatarRedirect
    {
        public AvatarRedirectItem data { get; set; }
    }
}
