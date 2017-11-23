using System;
using System.Collections.Generic;
using System.IO.IsolatedStorage;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pine.Object {
    class AccountInfo {
        bool _is_load_from_save = false;
        UInt64 _id = 0;
        string _name = "";
        public UInt64 ID {
            get {
                IsolatedStorageSettings appSettings = IsolatedStorageSettings.ApplicationSettings;
                if (!_is_load_from_save) {
                    try {
                        _id = (UInt64)appSettings["acc_id"];
                        _name = (string)appSettings["acc_name"];
                        _is_load_from_save = true;
                    } catch (Exception ex) {

                    }
                }
                return _id;
            }
            set {
                _id = value;
                IsolatedStorageSettings appSettings = IsolatedStorageSettings.ApplicationSettings;
                if (appSettings.Contains("acc_id")) {
                    appSettings["acc_id"] = _id;
                } else {
                    appSettings.Add("acc_id", _id);
                }
            }
        }
        public string Name {
            get {
                IsolatedStorageSettings appSettings = IsolatedStorageSettings.ApplicationSettings;
                if (!_is_load_from_save) {
                    try {
                        _id = (UInt64)appSettings["acc_id"];
                        _name = (string)appSettings["acc_name"];
                        _is_load_from_save = true;
                    } catch (Exception ex) {

                    }
                }
                return _name;
            }
            set {
                _name = value;
                IsolatedStorageSettings appSettings = IsolatedStorageSettings.ApplicationSettings;
                if (appSettings.Contains("acc_name")) {
                    appSettings["acc_name"] = _name;
                } else {
                    appSettings.Add("acc_name", _name);
                }
            }
        }
        public bool IsValid() {
            return ID > 0;
        }
    }
}
