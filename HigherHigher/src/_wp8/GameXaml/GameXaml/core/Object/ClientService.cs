using PhoneDirect3DXamlAppComponent;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace Pine.Object {
    public class ClientServiceRequest {
        protected int RequestCode = 0;
        public ClientServiceRequest(int code) {
            RequestCode = code;
        }
        public static bool operator ==(ClientServiceRequest t1, ClientServiceRequest t2) {
            return (t1.RequestCode == t2.RequestCode);
        }
        public static bool operator !=(ClientServiceRequest t1, ClientServiceRequest t2) {
            return (t1.RequestCode != t2.RequestCode);
        }
    }
    public abstract class ClientService {
        abstract public void process(ClientServiceRequest request, ServerStream svstream);
        abstract public string OuthorizeString(string id);
    }
}
