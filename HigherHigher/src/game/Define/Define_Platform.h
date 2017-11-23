#define MAX_TOUCHES 					10

// hardware type

#define DEVICE_IPHONE				0
#define DEVICE_IPHONE_3G			1
#define DEVICE_IPOD					2
#define DEVICE_IPHONE_3GS			3
#define DEVICE_IPOD_4				4
#define DEVICE_IPHONE_4G			5
#define DEVICE_IPAD					6
#define DEVICE_SIMULATOR			7

#define UNIVERSAL_BUILD				0

#define DEFAULT_SCR_W				480
#define DEFAULT_SCR_H				320
#define DEFAULT_DIV_W				480
#define DEFAULT_DIV_H				320

#define D4_SCR_W					480
#define D4_SCR_H					320
#define D4_DIV_W					480
#define D4_DIV_H					320

#define IPAD_SCR_W					1024
#define IPAD_SCR_H					768
#define IPAD_DIV_W					1024
#define IPAD_DIV_H					768

#define FREE_GAME_OF_THEDAY         0
#define ADD_PICK_IMAGE              0

#define PICK_STATE_NONE             0
#define PICK_STATE_TAKE_PICTURE     1
#define PICK_STATE_CHOICE_PICTURE   2
#define ENABLE_SERVER_PUSH_NOTIFICATION 1
#define IS_DEVELOPMENT_VERSION 1

#define USE_GL_SCALE

#define APPLE_ID 1141304181

#define ADVERT_APPLE_ID APPLE_ID

#define GAME_CENTER_FRIEND_NUM 1

#define CA_CERT_PATH        (GetBundlePath())
#if defined(IS_DEVELOPMENT_VERSION)

#define RSA_CLIENT_CERT     GetRealFilePath("apns-dev-cert.pem")
#define RSA_CLIENT_KEY      GetRealFilePath("apns-dev-key.pem")


#define APPLE_HOST          "gateway.sandbox.push.apple.com"
#define APPLE_PORT          2195

#define APPLE_FEEDBACK_HOST "feedback.sandbox.push.apple.com"
#define APPLE_FEEDBACK_PORT 2196
#else

#define RSA_CLIENT_CERT     GetRealFilePath("apns-production-cert.pem")
#define RSA_CLIENT_KEY      GetRealFilePath("apns-production-key.pem")


#define APPLE_HOST          "gateway.push.apple.com"
#define APPLE_PORT          2195

#define APPLE_FEEDBACK_HOST "feedback.push.apple.com"
#define APPLE_FEEDBACK_PORT 2196
#endif

#define USE_NEW_SYSTEM_VIEW_IOS_7_8 1
#define IOS8_MASTER_W (750.0f)
#define IOS8_MASTER_H (1334.0f)

#define ADD_CHARTBOOST 0
#define ADD_ADVERT  0
#define ADD_IADVERT 0
#define USE_ADBOX 1
#define ADD_ADMOB   1
#if defined(_WINDOWS)
#define ADD_ADVERT  0
#define ADD_IADVERT 0
#define USE_ADBOX	0
#define ADD_ADMOB   1

#endif
#define ADBOX_CONFIG_FILE "adbox.config"
#define CHARTBOOST_CONFIG_FILE "chartboost.config"
#define TAG_LINE "#HIGHERHIGHER"

#define USE_PINESOCIAL  0
#define ADD_GAMECENTER  1
#define ADD_TWITTER     0
#define ADD_FACEBOOK    0
#define ADD_IAP         1

#define FACEBOOK_TRANSFER_IAP       (0)
#define FACEBOOK_API                (20)

#define FACEBOOK_PERMISSION [@"email",@"basic_info"]
#define FACEBOOK_PERMISSION_FOR_STREAM [@"email",@"basic_info",@"publish_actions"]
#define FACEBOOOKAUDIENCE ACFacebookAudienceEveryone

#define PURCHASE_FUNC(x,y) {CGame::_this->PurchaseIAP(x,y);}
#define PROCESSING_TAG(a,b)     {CGame::_this->ProcessingTag(a,b);}

#define ADD_SCREEN_CAPTUREVIEW 0
#define FPS_VIDEO_RECORD 30
#define CAPTURE_VIDEO_W 480
#define CAPTURE_VIDEO_H 640

#define FACEBOOK_URL    "https://www.facebook.com/topebox/"
#define TWITTER_URL     "https://twitter.com/PineGames"
#define ADD_MAIL_SUPPORT    1
#define ADS_CONFIG_URL      "https://datancus.blob.core.windows.net/pinecrossingads/1141304181/ads.config"
#define ADS_CONFIG_FILE     "ads.config"

#define USE_IPAD_RETINA     1

#ifdef MAC_OS
#define NOTIFICATION_MAX        50
#define NOTIFY_RANGE_NUM        17
#define NOTIFY_RANGE_VALUE     {6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22}
#endif

#define ADD_VIDEO_REWARD 1
#define ADD_UNITYADS    0
#define UNITY_ADS_ID_DEFAULT "126238"
#define ADMOB_INTERSTITIAL_ID "ca-app-pub-6305464951596521/3540796298"
#define CHARTBOOST_ID1_DEFAULT "56fb53d60d602542491eb70dApp"
#define CHARTBOOST_ID2_DEFAULT "ebb86d2ae0922eff9f50336973480ebaf700b9cf"
#define ADMOB_ID1_DEFAULT "6305464951596521"
#define ADMOB_ID2_DEFAULT "3540796298"

#define ADD_VUNGLEADS   0
#define ADD_PINE_ADVERT 0
#define VUNGLE_ID_DEFAULT   "57b6d4778b0c52691a000070"
#define ADS_VERSION "100"
#define DEFINE_MASTER_IOS "i6"

//Update 10/11/2017
#define GP_LEADERBOARD_ID	"CgkI2of_sJgDEAIQGg"		//Top User
#if ANDROID_OS
#define USE_CALC_BY_VECTOR_AND_REUSE_RESULT
#endif
