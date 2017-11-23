using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace PineShap
{
#if DEBUG
    public class Test
    {
        public static void sendLog(string log){
            HttpClient client = new HttpClient();
            var content = new FormUrlEncodedContent(new[] 
            {
                new KeyValuePair<string, string>("log", log)
            });
            client.PostAsync("http://localhost/log.php", content);
        }
    }
#endif
}
