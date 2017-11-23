using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PineShap.FacebookUtils{
    public class ResponseData{
        public string id { get; set; }
        public ErrorData error { get; set; }
    }
    public class ErrorData{
        public int code { get; set; }
        public int error_subcode { get; set; }
    }
}
