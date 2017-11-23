using PineShap.FacebookUtils;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace PineShap.Actions {
    public class ActionFacebookGetInfomationArgs:ActionArgs {
        public UserInfomation UserInfomation = null;
    }
    public class ActionFacebookGetInfomation:Action {
        public override bool precheckToDo(ActionArgs e) {
            return FacebookClient.Instance.IsHasToken();
        }
        async void getMe(ActionFacebookGetInfomationArgs arg)
        {
            try
            {
                dynamic result = await FacebookManager.Client.GetTaskAsync("/me");
                if (result != null)
                {
                    FacebookClient.Instance.updateUserInfomation(result.id, result.name);
                    arg.UserInfomation = new UserInfomation();
                    arg.UserInfomation.id = result.id;
                    arg.UserInfomation.name = result.name;
                    arg.UserInfomation.last_name = result.last_name;
                    arg.UserInfomation.link = result.link;
                    arg.UserInfomation.locale = result.locale;
                    //arg.UserInfomation.timezone = result.timezone;
                    //arg.UserInfomation.updated_time = result.updated_time;
                    arg.UserInfomation.username = result.username;
                    arg.UserInfomation.verified = result.verified;
                    Done(arg);
                }
            }
            catch (Exception ex)
            {
                string a = ex.Message;
            }
        }

        public override void Do(ActionArgs e) {
            Begin(e);
            if (precheckToDo(e)) {
                try
                {
                    getMe(e as ActionFacebookGetInfomationArgs);
                }
                catch (Exception ex)
                {
                    throw ex;
                }
            } else {
                Cancel(e);
            }
        }
    }
}
