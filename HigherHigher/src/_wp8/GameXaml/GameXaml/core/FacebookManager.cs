using FacebookUtils;
using PhoneDirect3DXamlAppComponent;
using Pine.Actions;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pine {
    public class FacebookManager {
        static Dictionary<Actions.Action, ServerStream> _stream_wating = new Dictionary<Actions.Action, ServerStream>();
        static public void login(FacebookAppInfomation appinfo, ServerStream stream, Actions.DelegateBegin onBegin, Actions.DelegateDone onDone, Actions.DelegateCancel onCancel) {
            Actions.ActionFacebookLoginArgs arg = new Actions.ActionFacebookLoginArgs(AppObject.RootLayout) { AppInfo = appinfo };
            Actions.ActionFacebookLogin action_fblogin = new Actions.ActionFacebookLogin();
            _stream_wating.Add(action_fblogin, stream);
            try {
                action_fblogin.OnBegin += onBegin;
                action_fblogin.OnDone += action_fblogin_stream_OnDone;
                action_fblogin.OnDone += onDone;
                action_fblogin.OnCancel += action_fblogin_stream_OnCancel;
                action_fblogin.OnCancel += onCancel;
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_REQUESTING;
                AppObject.PopupOfAction.Add(new Actions.ActionArgLinker(action_fblogin, arg));
                ApplicationInfomation.onPopupOn();
                action_fblogin.Do(arg);
            } catch (Exception ex) {
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }
        static private void action_fblogin_stream_OnCancel(object sender, Actions.ActionArgs e) {
            try {
                ServerStream svstream = _stream_wating[(Actions.Action)sender];
                ActionFacebookLoginArgs arg = e as ActionFacebookLoginArgs;
                foreach (ActionArgLinker link in AppObject.PopupOfAction) {
                    if (link.AttachArg == arg) {
                        AppObject.PopupOfAction.Remove(link);
                        break;
                    }
                }
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_wating.Remove((Actions.Action)sender);
                ApplicationInfomation.onPopupOff();
            } catch (Exception ex) {
            }
        }
        static private void action_fblogin_stream_OnDone(object sender, Actions.ActionArgs e) {
            try {
                Actions.ActionFacebookLoginArgs fbarg = e as Actions.ActionFacebookLoginArgs;
                ActionFacebookLoginArgs arg = e as ActionFacebookLoginArgs;
                foreach (ActionArgLinker link in AppObject.PopupOfAction) {
                    if (link.AttachArg == arg) {
                        AppObject.PopupOfAction.Remove(link);
                        break;
                    }
                }
                if (fbarg != null && (fbarg.IsCancelByLogedin || (fbarg.Result != null && fbarg.Result.IsSuccess))) {
                    if (fbarg.IsCancelByLogedin) {
                        ServerStream svstream = _stream_wating[(Actions.Action)sender];
                        svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                        ApplicationInfomation.onPopupOff();
                    } else {
                        FacebookClient.Instance.Token = fbarg.Result.Token;
                        FacebookClient.Instance.TokenExprite = fbarg.Result.TokenExprite;
                        FacebookClient.Instance.SaveUserData();
                        ServerStream svstream = _stream_wating[(Actions.Action)sender];
                        action_fbinfo_stream_do(svstream);
                    }
                } else {
                    ServerStream svstream = _stream_wating[(Actions.Action)sender];
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    _stream_wating.Remove((Actions.Action)sender);
                    ApplicationInfomation.onPopupOff();
                }
            } catch (Exception ex) {
                Debug.WriteLine(ex.Message);
            }
        }
        static private void action_fbinfo_stream_do(ServerStream onstream) {
            Actions.ActionFacebookGetInfomationArgs arg = new Actions.ActionFacebookGetInfomationArgs();
            Actions.ActionFacebookGetInfomation action_fbinfo = new Actions.ActionFacebookGetInfomation();
            _stream_wating.Add(action_fbinfo, onstream);
            try {
                action_fbinfo.OnDone += action_fbinfo_stream_onDone;
                action_fbinfo.OnCancel += action_fbinfo_stream_onCancel;
                onstream.Status = (int)ServerStreamStatus.STREAM_STATUS_REQUESTING;
                action_fbinfo.Do(arg);
            } catch (Exception ex) {
                onstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                ApplicationInfomation.onPopupOff();
            }
        }
        static private void action_fbinfo_stream_onCancel(object sender, Actions.ActionArgs e) {
            try {
                ServerStream svstream = _stream_wating[(Actions.Action)sender];
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                _stream_wating.Remove((Actions.Action)sender);
                ApplicationInfomation.onPopupOff();
            } catch (Exception ex) {
            }
        }
        static private void action_fbinfo_stream_onDone(object sender, Actions.ActionArgs e) {
            try {
                Actions.ActionFacebookGetInfomationArgs fbarg = e as Actions.ActionFacebookGetInfomationArgs;
                if (fbarg != null) {
                    ServerStream svstream = _stream_wating[(Actions.Action)sender];
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    _stream_wating.Remove((Actions.Action)sender);
                } else {
                    ServerStream svstream = _stream_wating[(Actions.Action)sender];
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    _stream_wating.Remove((Actions.Action)sender);
                }
                ApplicationInfomation.onPopupOff();
            } catch (Exception ex) {
            }
        }


        static public void loadAvatar(Actions.DelegateBegin onBegin, Actions.DelegateDone onDone, Actions.DelegateCancel onCancel) {
            Actions.ActionFacebookGetAvatarArgs arg = new Actions.ActionFacebookGetAvatarArgs();
                
            Actions.ActionFacebookGetAvatar action_fbavar = new Actions.ActionFacebookGetAvatar();
            try {
                if(onBegin != null) action_fbavar.OnBegin += onBegin;
                if (onDone != null) action_fbavar.OnDone += onDone;
                if (onCancel != null) action_fbavar.OnCancel += onCancel;
                action_fbavar.Do(arg);
            } catch (Exception ex) {
                if (onCancel != null) onCancel(null, arg);
            }
        }
        /*ServerStream FbFeedStreamWaiting = null;
        static void feed(ServerStream svstream) {
            if (FacebookClient.Instance.Token.Length > 0 && FbFeedStreamWaiting == null) {
                StreamBuffer buf = new StreamBuffer(svstream.PostData);
                string message = buf.readSStr();
                FbFeedStreamWaiting = svstream;
                FacebookClient.Instance.PostMessageOnWall(AppObject.GameInfo.FacebookFeedTitle,
                    message,
                    //AppObject.GameInfo.FacebookFeedAttahLink,
                    AppObject.AppLink,
                    AppObject.GameInfo.FacebookFeedImageUrl,
                    new UploadStringCompletedEventHandler(fb_feed_handle));
            } else {
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }
        static private void fb_feed_handle(object sender, UploadStringCompletedEventArgs e) {
            if (FbFeedStreamWaiting != null) {
                if (e != null && e.Error == null) {
                    FbFeedStreamWaiting.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    //MessageBox.Show("Completed");
                } else {
                    FbFeedStreamWaiting.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    //MessageBox.Show("Error! Please retry later");
                }
                FbFeedStreamWaiting = null;
            }
        }
         * */
        static public void getFriends(Actions.DelegateBegin onBegin, Actions.DelegateDone onDone, Actions.DelegateCancel onCancel) {
            Actions.ActionFacebookGetFriendsArg arg = new Actions.ActionFacebookGetFriendsArg();
            Actions.ActionFacebookGetFriends action_fbfriend = new Actions.ActionFacebookGetFriends();

            try {
                if (onBegin != null) action_fbfriend.OnBegin += onBegin;
                if (onDone != null) action_fbfriend.OnDone += onDone;
                if (onCancel != null) action_fbfriend.OnCancel += onCancel;
                action_fbfriend.Do(arg);
            } catch (Exception ex) {
                if (onCancel != null) onCancel(null, arg);
            }
        }
		//Bo add
        static public void UpdateNewScore(int score,ServerStream stream, Actions.DelegateBegin onBegin, Actions.DelegateDone onDone, Actions.DelegateCancel onCancel)
        {
            Actions.ActionFacebookPostHighScoreArg arg_post_score = new Actions.ActionFacebookPostHighScoreArg();
            Actions.ActionFacebookPostHighScore action_post_score = new Actions.ActionFacebookPostHighScore();
            _stream_wating.Add(action_post_score, stream);
            try
            {
                if (onBegin != null) action_post_score.OnBegin += onBegin;
                action_post_score.OnDone+=action_post_score_OnDone;
                if (onCancel != null) action_post_score.OnCancel += onCancel;
                arg_post_score.Score = score;
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_REQUESTING;
                action_post_score.Do(arg_post_score);
            }
            catch (Exception ex)
            {
                stream.ErrorCode = 1;
                stream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }
        static public void action_post_score_OnDone(object sender, Actions.ActionArgs e)
        {
            Actions.ActionFacebookPostHighScoreArg arg = e as Actions.ActionFacebookPostHighScoreArg;
            if (arg != null)
            {
                if(arg.score_inform.success)
                {
                    ServerStream svstream = _stream_wating[(Actions.Action)sender];
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    _stream_wating.Remove((Actions.Action)sender);
                }
                else
                {
                    ServerStream svstream = _stream_wating[(Actions.Action)sender];
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    _stream_wating.Remove((Actions.Action)sender);
                }
            }
        }
    }
}
