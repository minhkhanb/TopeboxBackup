using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Pine
{
    public enum ConnectConstructor
    {
        INVALID = 0,
        FB_CONNECT = 1,
        FB_FEED = 2,
        TW_CONNECT = 3,
        TW_FEED = 4,
        FB_LOGOUT = 5,
        TW_LOGOUT = 6,
        GAME_DATA_UPDATE = 7,
        GAME_DATA_UPLOAD = 8,
        FB_GET_APP_USER = 9,
        IAP_BUY_ITEM = 10,
        FB_FEED_MESSAGE = 11,
        TW_FEED_MESSAGE = 12,
        GAME_USER_RANDOM = 13,
        MAIL_SEND=16,
        SMS_SEND=17,
        GAME_RATING=18,
        NOTIFICATION_RUN = 19,
        NOTIFICATION_STOP = 20,
        GAME_EXIT = 21,
        SERVER_CHECK_AND_RUN = 22,
    }

    public enum ServerStreamConstructor
    {
        REQUEST_CODE_FB_LOGIN = 1,
        REQUEST_CODE_FB_GET_AVATAR = 2,
        REQUEST_CODE_OPEN_BROWSER = 3,
        REQUEST_CODE_RATING = 4,
        REQUEST_CODE_PURCHASE = 5,
        REQUEST_CODE_PURCHASE_GETLIST = 6,
        REQUEST_CODE_FB_FEED_ME = 7,
        REQUEST_CODE_TW_LOGIN = 8,
        REQUEST_CODE_TW_FEED = 9,
        REQUEST_CODE_FB_GET_FRIEND = 10,
        REQUEST_CODE_GAME_EXIT = 11,
        REQUEST_CODE_LOAD_IMAGE = 12,
        REQUEST_CODE_TW_GET_FRIEND = 13,
        REQUEST_CODE_SMS_SEND = 14,
        REQUEST_CODE_MAIL_SEND = 15,
        REQUEST_CODE_LOAD_FILE = 16,
		REQUEST_CODE_FB_POST_PHOTO = 17,
        REQUEST_CODE_ADMOB_INTERSTITIAL_PREPARE = 18,
        REQUEST_CODE_ADMOB_INTERSTITIAL_SHOW = 19,
        REQUEST_CODE_DIALOG_SHOW = 20,
        REQUEST_CODE_FB_POST_SCORE = 21,
        REQUEST_CODE_MEDIA_LINK_TASK = 22,
        REQUEST_CODE_SHARE_MEDIA_TASK = 23,
#if BUILD_SUPPORT
        REQUEST_CODE_SUPPORT_GET_ACCOUNT_ID = 900
#endif
    }

    public enum ServerStreamStatus
    {
        STREAM_STATUS_FREE = 0,
        STREAM_STATUS_WAITING = 1,
        STREAM_STATUS_REQUESTING = 2,
        STREAM_STATUS_ERROR = 3,
        STREAM_STATUS_RESPONSED = 4,
    }
}
