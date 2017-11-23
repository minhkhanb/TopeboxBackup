#if !defined(_H_DEVICE)
#define _H_DEVICE
#include "../Lib/DataType/CType.h"
#include <string>

#define CAPTURE_VIDEO_STATE_NONE        0
#define CAPTURE_VIDEO_STATE_RECORDING   1
#define CAPTURE_VIDEO_STATE_COMPLETED   2
#define CAPTURE_VIDEO_STATE_STOPED      3
/*NOT SURE*/		extern "C" double			OS_GetAccelX();
/*NOT SURE*/		extern "C" double			OS_GetAccelY();
/*NOT SURE*/		extern "C" double			OS_GetAccelZ();
#if USE_PINEGAMEPLAYREPLAY
#define RECORD_GAMEPLAY_ACCESS_DENINE_STATE -1
#define RECORD_GAMEPLAY_IDLE_STATE          0
#define RECORD_GAMEPLAY_RECORDING_STATE     1
#define RECORD_GAMEPLAY_WAIT_SHARE          2
#define RECORD_GAMEPLAY_SHARING             3
#define RECORD_GAMEPLAY_SHARE_SUCESSFULL    4
#define RECORD_GAMEPLAY_SHARE_DISSMISS      5
#endif


#define NETWORK_TYPE_UNKOWN 0
#define NETWORK_TYPE_WIFI   1
#define NETWORK_TYPE_3G     2

#define AUDIO_RECORD_NONE_STATE                 (0)
#define AUDIO_RECORD_WAIT_AUTHENTICAION_STATE   (1)
#define AUDIO_RECORD_AUTHENTICATION_SUCESSFULL  (2)
#define AUDIO_RECORD_AUTHENTICATION_FAIL        (3)
#define AUDIO_RECORD_AUTHENTICATION_PROCESSING  (4)
#define AUDIO_RECORD_AUTHENTICATION_STOPPED     (5)
class Device{
protected:
	char* _user_agent;
	char* _model;
	bool _is_safe;
    
public:
    static float GetVolumeSpeaker();
#if USE_AUDIO_RECORDER
    static int AudioRecordState;
    static void StartDetectBlow();
    static void StopDetectBlow();
    static double GetBlowAudioPassVolume();
    static void SetBlowPassCurrentVolume(double v);
#endif
#if USE_PINEGAMEPLAYREPLAY
    static bool IsChangingSetting;
    static bool CurrentReplaySetting;
    static int FrameReplayToStop;
    static int CurrentReplayType;
    static void OnOffReplay();
    static void StartRecordGameplay();
    static void StopRecordGameplay();
    static int ReplayGamePlayState;
    static void ShareRecordGameplay();
    static void LoadReplaySetting();
    static void SaveReplaySetting();
    static bool HasDeviceSupportReplay();
    static void ForceStopRecordGameplay();
#endif
	Device(const Device& other);
	Device();
	~Device();
    SupportVersion _version;
	//size of display area
	PineSize ScreenSize;

	//real size of screen
	PineSize DeviceScreenSize;

	//device orientation
	Orientation Device_Orientation;

	//display properties of screen
	PineDisplayProperties DisplayProperties;

	//device ability
	bool IsSupportAppleGameCenter;
	bool IsSupportTwitter;
	bool IsSupportFacebook;
	static bool IsNetworkAvailable();
	bool IsSafe();//if true, device hadn't been hacked yet, safe to transfer data.
	void NoticeUnSafe();
    
	const char* getDeviceAgent();
	void setDeviceAgent(const char* agent);
	const char* getDeviceModel();
	void setDeviceModel(const char* model);
	void operator=(const Device other);
    
    static void ShowPictureLibrary();
    static void ShowCamera();
    static void DisableCamera();
    static void CaptureCamera();
    static void SwitchCamera();
#define CAMERA_FREE_STATE       (0)
#define CAMERA_PICKING_STATE    (1)
#define CAMERA_TAKING_STATE     (2)
#define CAMERA_COMPLETE_STATE   (3)
    static int _camera_state;
    
    /////////////KEY BOARD////////////////
    static char* _keyboard_character;
#define KEYBOARD_BUFFER_LENGTH (512)
    static bool disableReturnKey;
    static void ShowKeyboard(int x, float y, float w, float h);
    static void CloseKeyboard();
    static void DisableReturnKey(bool disable);
    static void  ClearTextBuffer();
    static void SetTextBufferMax(int len_max);
    static bool HasForceCloseKeyboard();
    
    //motion detect.
    static double _device_motionX;
    static double _device_motionY;
    static double _device_motionZ;
    
    
    static double _device_root_motionX;
    static double _device_root_motionY;
    static double _device_root_motionZ;
    static double _motion_view2D_x;
    static double _motion_view2D_y;
    static double _motion_view2D_z;
    static double _motion_view2D_w;
    static double _motion_view2D_h;
    
    static double _motion_balance_angle_x;
    static double _motion_balance_angle_y;
    static double _motion_balance_angle_z;
    
    static double _motion_balance_x;
    static double _motion_balance_y;
    static double _motion_balance_z;
    
    
    static double _motion_balance;
    
    static void InitDeviceMotion(double viewX,double viewY, double viewW, double viewH);
    static void SetRootOfMotion(double x, double y, double z);
    static double GetDeviceMotionX();
    static double GetDeviceMotionY();
    static double GetDeviceMotionZ();
    
    static double GetDeviceMotionTransformX();
    static double GetDeviceMotionTransformY();
    static double GetDeviceMotionTransformZ();
    
    static double GetCursorViewX();
    static double GetCursorViewY();
    
    static double GetCursorPhysicX();
    static double GetCursorPhysicY();
    
    static double CaculateMotionSpeed();
    static double SetCurrentBalance();
    static double CaculateMotionXDivY();
    static double CaculateMotionYDivX();
    static double CaculateMotionSpeedX();
    static double CaculateMotionSpeedY();
    static double CaculateMotionSpeedZ();
    
    static void DeviceUpdateMotion();
    static double _angle_moving;
    
    static double _logic_motion_speed;
    static double _logic_scroll_x;
    static double _logic_scroll_y;
    static double _use_physic_view;
    static bool _safe_to_shake;

	
	static bool IsEnableScaleView();
	static bool ScaleView(float scalex = 1.0f, float scaley = 1.0f);
    
    static bool HasAppInstalled(const char* apple_id, int with_version = 0);
    static void SetAppInstalled(const char* apple_id, int with_version = 0);
    static bool DownloadFile(const char* url, const char* file_name,int time_out);
    static int GetNetworkSupport();
    //static bool PINEZ_Extractor(const char* input,const char* file_name_inzip,const char *output);
    static bool PINEZ_Extractor(std::wstring input,std::string file_name_inzip,std::wstring output);
#if defined (MAC_OS)
    static bool IsIpadDevice();
#endif
    static int GetDayOfYear();
    static int GetHourOfDay();
    
#define MAX_QUEUE (1000)
    static int _queue_sound_top;
    static int _queue_sound_rear;
    static int _queue_sound_ids[MAX_QUEUE];
    static int _queue_sound_skip[MAX_QUEUE];
    static long _queue_sound_frame[MAX_QUEUE];
    static void PlayQueueSFX(int soundid, int frameToSkip = -1);
    static void UpdateAndPlayQueueSFX();
    
    static bool IsDeviceWeak();
    
    
#if USE_PINE_DATE_SYSTEM
    static bool _time_be_interupted;
    static long _pine_tsecond;
    static long _pine_tminute;
    static long _pine_thour;
    static long _pine_tday;
    static long _pine_tmonth;
    static long _pine_tyear;
    
    static void RefeshTimer();
    static void UpdateTimer();
    static bool AddSecond();
    static time_t MakeTime(long sec,long min, long hour, long day, long month, long year);
    static void PrintTime(time_t time);
    static double TimeDiff(time_t t1, time_t t2);
    static void InitTimer();
    static time_t GetPTTime();
    static int GetTimeZone();
#endif
    
    static void LinkToRatingApp(unsigned long apple_id);
    
    //Notification
#if  NOTIFICATION_MAX
    static int _current_num_notification;
    static char _notification_string[NOTIFICATION_MAX][1024];
    static long long _notification_time[NOTIFICATION_MAX];
    static void AskShowNotification();
    static void AddNotification(const char* notification,long long time, bool replay_if_exits= true);
    static void PushNotifyResignActive();
    static void PushNotifyBecameActive();
#endif
    
#if USE_COMPRESSOR_DATA_INGAME
    static int GetPackCompressed(const char* pack_name);
    static const char* GetPackShortNameCompressed(int index);
    static const char* GetPackFullNameCoompressed(int index);
    
    static bool HasUnzipAll();
    static bool HasUnzipPack(int index);
    static bool UnzipPack(int index);
    
#endif
    static time_t GetTimeModifyOfFile(const char* file);
    static time_t GetTimeCreateOfFile(const char* file);
#if ADD_GOOGLE_ANALYTICS
    static void GA_Init();
    static void GA_LogPurchase(const char *TransactionID,double price);
    static void GA_LogBuyItem(const char *CurrencyName,const char* ItemName,double price);
    static void GA_LogAchievements(const char *achiID,const char* achiName);
    static void GA_LogContents(const char *title,const char* decs, const char* type = "");
    static void GA_LogLevelup(double level_up);
    static void GA_SetUserLevelProperty(double level);
#if ADD_REMOTE_CONFIG
    static const char* RemoteConfigGetString(const char*  strID, const char* default_value = NULL);
    static const long RemoteConfigGetInteger(const char*  strID, long default_value = EOF);
    static const bool RemoteConfigGetBoolean(const char*  strID, bool default_value = false);
#endif
#endif
    
    static void OS_ShowRatingBasic();
};
#endif//_H_DEVICE
