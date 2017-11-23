using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PineShap.FacebookUtils{
    public class FriendsList{
        public List<FriendsListItem> data { get; set; }
        public Paging paging { get; set; }
    }
    public class FriendsListItem{
        public string uid { get; set; }
    }
    public class Paging{
        public string next { get; set; }
    }
}
