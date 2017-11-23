#if !C_USER_H
#define C_USER_H

#include "../../Lib/DataType/CType.h"

#define MAX_CHARACTER_VER_0			100
#define SAVE_CHARACTER_NAME_LENGTH_CATCH			32

#define SAVE_FILE_NAME									"HSGame.pine"
#define SAVE_BACKUP_FILE_NAME							"HSGameBK.pine"

#if defined (ANDROID_OS)
#define CLOUD_SAVE_FILE_NAME							"clouddata.pine"
#endif


#define GAME_CURRENT_VERSION_1							1
#define GAME_CURRENT_VERSION_2							2//31/8/2016
#define GAME_CURRENT_VERSION_3							3 //10/11/2017
#define GAME_CURRENT_VERSION_4							4 //13/11/2017
#define GAME_CURRENT_VERSION							GAME_CURRENT_VERSION_4

#define SAVE_FORMAT_CURRENT_VERSION						0
#define SAVE_FORMAT_USER_ID					(SAVE_FORMAT_CURRENT_VERSION+8)
#define SAVE_FORMAT_USER_NAME				(SAVE_FORMAT_USER_ID + 128)

#define SAVE_FORMAT_GAME_HIGHT_SCORE			(SAVE_FORMAT_USER_NAME + 128)
#define SAVE_FORMAT_MUSIC_ENABLE				(SAVE_FORMAT_GAME_HIGHT_SCORE + 2)
#define SAVE_FORMAT_SOUND_ENABLE				(SAVE_FORMAT_MUSIC_ENABLE + 1)
#define SAVE_FORMAT_NOTIFY_ENABLE				(SAVE_FORMAT_SOUND_ENABLE + 1)
#define SAVE_FORMAT_SAVE_CLOUD_VERSION			(SAVE_FORMAT_NOTIFY_ENABLE + 1)
#define SAVE_FORMAT_SAVE_REMOVE_ADS				(SAVE_FORMAT_SAVE_CLOUD_VERSION + 8)
#define SAVE_FORMAT_SAVE_HAS_FB_LOGIN			(SAVE_FORMAT_SAVE_REMOVE_ADS + 1)
#define SAVE_FORMAT_USER_NAME_UNICODE			(SAVE_FORMAT_SAVE_HAS_FB_LOGIN + 1)
#define SAVE_FORMAT_USER_NAME_LENGHT_UNICODE    (SAVE_FORMAT_USER_NAME_UNICODE + SAVE_CHARACTER_NAME_LENGTH_CATCH*4)
#define SAVE_FORMAT_USER_LEVEL					(SAVE_FORMAT_USER_NAME_LENGHT_UNICODE + 2) 

//objective
#define SAVE_FORMAT_CURRENT_OBJECTIVE					(SAVE_FORMAT_USER_LEVEL + 2)
#define SAVE_FORMAT_CURRENT_VALUE_OBJECTIVE				(SAVE_FORMAT_CURRENT_OBJECTIVE + 2)
#define SAVE_FORMAT_CURRENT_STATUS_OBJECTIVE			(SAVE_FORMAT_CURRENT_VALUE_OBJECTIVE + 2)
#define SAVE_FORMAT_MULTI_LEVEL							(SAVE_FORMAT_CURRENT_STATUS_OBJECTIVE + 1)
#define SAVE_FORMAT_OBJECTIVE_COIN						(SAVE_FORMAT_MULTI_LEVEL + 2)
#define SAVE_FORMAT_OBJECTIVE_DAY_PLAY					(SAVE_FORMAT_OBJECTIVE_COIN + 2)
#define SAVE_FORMAT_PERCENR_QUEST						(SAVE_FORMAT_OBJECTIVE_DAY_PLAY + 2)
#define SAVE_FORMAT_DAY_NEXT_LOGIN						(SAVE_FORMAT_PERCENR_QUEST + 2)
#define SAVE_FORMAT_CHARACTER_USING						(SAVE_FORMAT_DAY_NEXT_LOGIN + 8)
#define SAVE_FORMAT_NUM_RETRY                           (SAVE_FORMAT_CHARACTER_USING + 2)
#define SAVE_FORMAT_HAS_RATE                            (SAVE_FORMAT_NUM_RETRY + 8)
#define SAVE_FORMAT_COUNT_SHOW_RATE						(SAVE_FORMAT_HAS_RATE + 1)
#define SAVE_FORMAT_STEP_TUTORIAL						(SAVE_FORMAT_COUNT_SHOW_RATE + 2)
#define SAVE_FORMAT_IS_USED_ICLOUD                      (SAVE_FORMAT_STEP_TUTORIAL + 2)
#define SAVE_FORMAT_COUNT_SHOW_PINEADS                  (SAVE_FORMAT_IS_USED_ICLOUD + 1)

#define SAVE_FORMAT_USER_HAS_TOUCH_PROMO				(SAVE_FORMAT_COUNT_SHOW_PINEADS + 2)	
#define SAVE_FORMAT_USER_GAME_PROMO_ICON_ID				(SAVE_FORMAT_USER_HAS_TOUCH_PROMO + 1)
#define SAVE_FORMAT_USER_GAME_PROMO_LINK_STORE			(SAVE_FORMAT_USER_GAME_PROMO_ICON_ID + 128)
#define SAVE_FORMAT_USER_GAME_PROMO_REVISION			(SAVE_FORMAT_USER_GAME_PROMO_LINK_STORE + 128)
#define SAVE_FORMAT_USER_GAME_PROMO_ORDER				(SAVE_FORMAT_USER_GAME_PROMO_REVISION + 2)
#define SAVE_FORMAT_USER_GAME_PROMO_HAS_SHOW			(SAVE_FORMAT_USER_GAME_PROMO_ORDER + 2)
#define SAVE_FORMAT_USER_GAME_PROMO_DEVICE_ID			(SAVE_FORMAT_USER_GAME_PROMO_HAS_SHOW + 1)
#define SAVE_FORMAT_USER_GAME_PROMO_DELAY_SHOW			(SAVE_FORMAT_USER_GAME_PROMO_DEVICE_ID + 128)

#define SAVE_FORMAT_USER_PLAY_NUM						(SAVE_FORMAT_USER_GAME_PROMO_DELAY_SHOW + 2)

#define SAVE_FORMAT_END							(SAVE_FORMAT_USER_PLAY_NUM + 2)

#define SAVE_FORMAT_LENGTH						(SAVE_FORMAT_END)

#define SAVE_MAX_BUFFER				10000
#define MAX_SAVE_BUFFER_CACHE		10000


//icloud
#define BEGIN_TAG								10
#define ALERT_TAG_CONFRIM_SYNC_ICOUND			(BEGIN_TAG + 0)
#define ALERT_TAG_CONFRIM_ICOUND_LOAD_SAVE		(ALERT_TAG_CONFRIM_SYNC_ICOUND + 1)
#define ALERT_TAG_CONFRIM_ICOUND_SYNC_COMPLETE	(ALERT_TAG_CONFRIM_ICOUND_LOAD_SAVE + 1)
#define ALERT_TAG_CONFRIM_NETWORK_ERROR			(ALERT_TAG_CONFRIM_ICOUND_SYNC_COMPLETE + 1)

class CUserData
{
public:
	CUserData();
	~CUserData();
	char _buffer[SAVE_MAX_BUFFER];
	INT64 _version;

	void Init();
	void SynForNewVersion(INT_TIME fromVersion);
	void NewDataDefault(INT_TIME version);

	void UserDefault();
	void UserDataSave();
	void UserDataLoad();
	void LoadFromBuffer();
	void SaveToBuffer();
	void SetUserID(char* userid);
	const char* GetUserID();
	void SetUserName(char* username);
	const char* GetName();


	void SetFBID(char* userid);
	const char* GetFBID();
	void SetFBName(char* username);
	const char* GetFBName();
	void SetAvatarURL(char* url);
	const char* GetAvatarURL();
	INT_TIME _save_cloud_version;
	bool _hadRemovedAds;

	bool _charaterUnlock[MAX_CHARACTER_VER_0];

	//PineImage _avatar;
	int _avatarState;
	bool _hadLoginFB;
	INT32 _userNameUnicode[SAVE_CHARACTER_NAME_LENGTH_CATCH];
	int _userNameUnicodeLenght;
	char _user_fb_id[128];
	char _user_fb_name[128];
	//int _coin;
	int _levelMultiplier;
	int _typeCharacterUse;
	INT_TIME _day_next_login;

	INT_TIME _numRetryGame;
	bool _isRating;
	int _countShowRate;
	int _stepTutorial;

	//check syn iloud  18/May/2016    
	void ResetIcloud();
	bool SaveToIcloud();
	int CheckCanPushDataSave();
	void LoadFromIcloud();
	void DefaultIcloud();
	void CheckIcloud(bool showMessage = false);
	BYTE _isUseIcloud;
	bool _isShowNotifi;
	enum enumUseIcloud
	{
		ICLOUD_NONE,
		ICLOUD_USE,
		ICLOUD_DONT_USE,
	};
	enum enumStateIcloud
	{
		ICLOUD_STATE_NODATA,
		ICLOUD_STATE_DISCONECT,
		ICLOUD_STATE_OLD_VERSION,
		ICLOUD_STATE_NEW_VERSION,
		ICLOUD_STATE_READY_SYNS,
	};

	void NetWorkError();
	void CheckLocalAndCloudSaveFile(bool isauto);
	void ConfirmSyncSaveToCloud(bool isauto);
	void ConfirmSyncSaveFromCloud();
	void UpdateCloudSaveFile();
	void UpdateLocalSaveFile(Buffer * buffer);
	void CheckUnlockAllAchievements();

	int _play_game_num;

	bool _has_touch_promo;
	char _game_promo_icon_id[128], _game_promo_link_store[128];
	int _game_promo_revision, _game_promo_has_show;
	int _game_promo_order;
	char _game_promo_deviceID[128];
	INT_TIME _game_promo_delay_show;

private:

	char _userName[128];
	char _userID[128];
	char _avatar_URL[1024];

};

#endif
