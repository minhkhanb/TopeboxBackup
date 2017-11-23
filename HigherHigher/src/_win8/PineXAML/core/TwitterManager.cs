////using Hammock;
using Pine;
using PineShap.Actions;
using PineShap.TwitterUtils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace PineShap {
    class TwitterManager {
        static Dictionary<Actions.Action, ServerStream> _stream_wating = new Dictionary<Actions.Action, ServerStream>();
        public static void login(ServerStream stream, Actions.DelegateBegin onBegin, Actions.DelegateDone onDone, Actions.DelegateCancel onCancel) {
            Actions.ActionTwitterLoginArgs arg = new Actions.ActionTwitterLoginArgs(AppObject.RootLayout);
            Actions.ActionTwitterLogin action_twlogin = new Actions.ActionTwitterLogin();
            _stream_wating.Add(action_twlogin, stream);
            try {
                if (onBegin != null) action_twlogin.OnBegin += onBegin;
                action_twlogin.OnDone += action_twitter_login_OnDone;
                if (onDone != null) action_twlogin.OnDone += onDone;
                action_twlogin.OnCancel += action_twitter_login_OnCancel;
                if (onCancel != null) action_twlogin.OnCancel += onCancel;
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_REQUESTING;
                ////ApplicationInfomation.onPopupOn();
                AppObject.PopupOfAction.Add(new Actions.ActionArgLinker(action_twlogin, arg));
                action_twlogin.Do(arg);
            } catch (Exception ex) {
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }
        static private void action_twitter_login_OnDone(object sender, Actions.ActionArgs e) {
            try {
                Actions.ActionTwitterLoginArgs twarg = e as Actions.ActionTwitterLoginArgs;
                foreach (ActionArgLinker link in AppObject.PopupOfAction) {
                    if (link.AttachArg == twarg) {
                        AppObject.PopupOfAction.Remove(link);
                        break;
                    }
                }
                if (twarg != null && (twarg.IsCancelByLogedin || (twarg.Result != null && twarg.Result.IsSuccess))) {
                    if (twarg.IsCancelByLogedin) {
                        ServerStream svstream = _stream_wating[(Actions.Action)sender];
                        svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                        _stream_wating.Remove((Actions.Action)sender);
                        ////ApplicationInfomation.onPopupOff();
                    } else {
                        ServerStream svstream = _stream_wating[(Actions.Action)sender];
                        svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                        _stream_wating.Remove((Actions.Action)sender);
                        ////ApplicationInfomation.onPopupOff();
                    }
                } else {
                    ServerStream svstream = _stream_wating[(Actions.Action)sender];
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    _stream_wating.Remove((Actions.Action)sender);
                   //// ApplicationInfomation.onPopupOff();
                }
                AppObject.PopupOfAction.Clear();
            } catch (Exception ex) {
            }
        }
        static private void action_twitter_login_OnCancel(object sender, Actions.ActionArgs e) {
            try {
                Actions.ActionTwitterLoginArgs twarg = e as Actions.ActionTwitterLoginArgs;
                foreach (ActionArgLinker link in AppObject.PopupOfAction) {
                    if (link.AttachArg == twarg) {
                        AppObject.PopupOfAction.Remove(link);
                        break;
                    }
                }
                ServerStream svstream = _stream_wating[(Actions.Action)sender];
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_wating.Remove((Actions.Action)sender);
                ////ApplicationInfomation.onPopupOff();
                AppObject.PopupOfAction.Clear();
            } catch (Exception ex) {
            }
        }
        static ServerStream TWFeedStreamWaiting = null;
        public static void twfeed(ServerStream svstream) {
            if (TwitterClient.isAlreadyLoggedIn() && TWFeedStreamWaiting == null) {
                StreamBuffer buf = new StreamBuffer(svstream.PostData);
                string message = buf.readSStr();
                TWFeedStreamWaiting = svstream;
                ////TwitterClient.PostTweet(message + " " + AppObject.AppLink, action_twitter_feed_callback);
            } else {
                TWFeedStreamWaiting = svstream;
                login(TWFeedStreamWaiting, null, twfeed_login_done, twfeed_login_cancel);
            }
        }

        private static void twfeed_login_cancel(object sender, Actions.ActionArgs e) {
            if (TWFeedStreamWaiting != null) {
                TWFeedStreamWaiting.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                TWFeedStreamWaiting = null;
            }
        }

        private static void twfeed_login_done(object sender, Actions.ActionArgs e) {
            ServerStream stream = TWFeedStreamWaiting;
            TWFeedStreamWaiting = null;
            if (stream != null) {
                twfeed(stream);
            }
        }

        /*static private void action_twitter_feed_callback(RestRequest request, RestResponse response, object userState) {
            if (TWFeedStreamWaiting != null) {
                Deployment.Current.Dispatcher.BeginInvoke(() => {
                    if (response != null) {
                        if (response.StatusCode == HttpStatusCode.OK) {
                            TWFeedStreamWaiting.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                            //MessageBox.Show("Completed");
                        } else if (response.StatusCode == HttpStatusCode.Forbidden) {
                            //MessageBox.Show("You’ve reached the limit for number of posts today");
                            TWFeedStreamWaiting.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                            TWFeedStreamWaiting.ErrorCode = 1;
                        } else {
                            //MessageBox.Show("Error");
                            TWFeedStreamWaiting.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                            TWFeedStreamWaiting.ErrorCode = 1;
                        }
                    } else {
                        TWFeedStreamWaiting.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    }
                    if (TWFeedStreamWaiting.AutoFinish) TWFeedStreamWaiting.Status = (int)ServerStreamStatus.STREAM_STATUS_FREE;
                });
            }
        }*/
    }
}
