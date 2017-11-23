using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pine.FacebookUtils{
    public class FriendsList{
        public List<FriendsListItem> data { get; set; }
        public Paging paging { get; set; }
    }
   
    public class FriendsListItem{
        public string id { get; set; }
        public string name { get; set; }
    }
    public class Paging{
        public string next { get; set; }
    }
    public class FriendsListItemVer1
    {
        public string uid { get; set; }
    }
    public class FriendListVer1
    {
        public List<FriendsListItemVer1> data { get; set; }
        public Paging paging { get; set; }
    }
}
