using System.Windows;
#if DEBUG
using MockIAPLib;
using Store = MockIAPLib;
#else
using Windows.ApplicationModel.Store;
#endif
using FacebookUtils;
using Pine.TwitterUtils;
using Microsoft.Phone.Net.NetworkInformation;
using Hammock;
using Pine.Dialogs;
using Microsoft.Phone.Scheduler;
using System.Net.Http;
using System.Security.Cryptography;
using Windows.ApplicationModel;
using System.Reflection;
using System.Xml;
using GoogleAds;
using System.Diagnostics;
using System.Threading;
using Pine.Actions;
using System;
using PhoneDirect3DXamlAppComponent;

namespace Pine
{
    partial class MainPage
    {
#if BUILD_SUPPORT
        void support_act_OnCancel(object sender, ActionArgs e)
        {
            ActionSupportAccountAskArgs arg = e as ActionSupportAccountAskArgs;
            ServerStream sv = _stream_wating[(Actions.Action)sender];
            _stream_wating.Remove((Actions.Action)sender);
            sv.ErrorCode = 1;
            sv.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
        }

        void support_act_OnDone(object sender, ActionArgs e)
        {
            ActionSupportAccountAskArgs arg = e as ActionSupportAccountAskArgs;
            ServerStream sv = _stream_wating[(Actions.Action)sender];
            _stream_wating.Remove((Actions.Action)sender);
            try
            {


                UInt64 acc_id = UInt64.Parse(arg.AccountId);
                if (acc_id > 3000000000000000000)
                {
                    PlayerInfomation.FacebookId = (acc_id - 3000000000000000000).ToString();
                }
                else if (acc_id > 300000000000000000)
                {

                    PlayerInfomation.FacebookId = (acc_id - 300000000000000000).ToString();
                }
                else
                {
                    PlayerInfomation.FacebookId = arg.AccountId.ToString();
                }
                PlayerInfomation.UserId = arg.AccountId.ToString();
                PlayerInfomation.FacebookUserName = "AccountSupport";
                sv.ErrorCode = 0;
                sv.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
            }
            catch (Exception ex)
            {
                sv.ErrorCode = 1;
                sv.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
            }
        }

        void support_global_act_OnDone(object sender, ActionArgs e)
        {
            ActionSupportAccountAskArgs arg = e as ActionSupportAccountAskArgs;
            ServerStream sv = _stream_wating[(Actions.Action)sender];
            _stream_wating.Remove((Actions.Action)sender);

            int buff_len = 2 + 2 * arg.AccountId.Length;

            StreamBuffer buff = new StreamBuffer(buff_len);
            buff.writeSStr(arg.AccountId);

            sv.ResponseData = buff.Data;
            sv.ErrorCode = 0;
            sv.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
        }
#endif
    }
}