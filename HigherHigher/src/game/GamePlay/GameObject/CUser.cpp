#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"
#include <PineEngine.h>
PE_USING_ENGINE;
extern "C" void ShowAlertMessage_OKCancel(const char* feed, const char* title, const char* tag);
extern "C" void ShowAlertMessage_OK(const char* feed, const char* title, const char* tag);

CUserData::CUserData()
{
}

CUserData::~CUserData()
{
}

void CUserData::Init()
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
	sprintf(_userName, "%s", "user_name");
	sprintf(_userID, "%s", "user_id");
	sprintf(_user_fb_id, "%s", "fb_id");
	sprintf(_user_fb_name, "%s", "fb_name");
	sprintf(_avatar_URL, "%s", "avatar_url");
	GAME()->SetSoundEnable(true);
	GAME()->SetMusicEnable(true);
	GAME()->_notificationEnable = true;
	_save_cloud_version = 0;
	_hadRemovedAds = false;
	_hadLoginFB = false;
	game->_game_score = 0;
	game->_high_score = 0;

	_isShowNotifi = false;
	_isUseIcloud = CUserData::enumUseIcloud::ICLOUD_NONE;
	_levelMultiplier = 1;
	game->_quest._currentQuest = 0;
	game->_quest._currentValue = 0;
	game->_quest._status = CQuest::enumQuestStatus::_QUEST_DOING;
	game->_quest._totalCoin = 0;
	game->_quest._dayPlay = 0;
	game->_quest._level = 0;
	game->_quest._percent = 0;
	Pine_Date_Time localtime;
	localtime.setDateByLocalDate();
	_day_next_login = localtime.getDay()+1;
	game->_quest.SetCurrentValue(CQuest::enumObjective::Objective_12, 1);
    GAME()->SetSoundEnable(true);
    GAME()->SetMusicEnable(true);
    _isRating = false;
    _countShowRate = 10;
    _stepTutorial = 0;
    _numRetryGame = 0;
	_typeCharacterUse = 0;
    game->_count_show_pineads = 1;

	_play_game_num = 1;

	sprintf(_game_promo_icon_id, "%s", "gameID");
	sprintf(_game_promo_link_store, "%s", "linkStore");
	sprintf(_game_promo_deviceID, "%s", "deviceID");
	_game_promo_revision = 0;
	_game_promo_has_show = true;
	_game_promo_delay_show = time(NULL);
	_game_promo_order = 0;
	_has_touch_promo = false;
}
void CUserData::UserDefault()
{
	_version = GAME_CURRENT_VERSION;
	Init();
	//GAME()->DefaultICloud();
}

void CUserData::SynForNewVersion(INT_TIME fromVersion)
{
	for (INT_TIME i = fromVersion; i <= GAME_CURRENT_VERSION; i++)
	{
		NewDataDefault(i);
	}
	_version = GAME_CURRENT_VERSION;
}

void CUserData::NewDataDefault(INT_TIME version)
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
    printf("\n reset save version:%lld",version);
	if (version <= GAME_CURRENT_VERSION_3)
	{
        printf("\n reset version");
		Init();
	}
	if (version <= GAME_CURRENT_VERSION_4)
	{
		sprintf(_game_promo_icon_id, "%s", "gameID");
		sprintf(_game_promo_link_store, "%s", "linkStore");
		sprintf(_game_promo_deviceID, "%s", "deviceID");
		_game_promo_revision = 0;
		_game_promo_has_show = true;
		_game_promo_delay_show = time(NULL);
		_game_promo_order = 0;
		_has_touch_promo = false;
	}
}

void CUserData::UserDataSave()
{
	SaveToBuffer();

#if defined(MAC_OS) || defined(_WINDOWS_PHONE8) || ANDROID_OS
	OS_SaveAppData(SAVE_FILE_NAME, _buffer, MAX_SAVE_BUFFER_CACHE, false);
#else

	FILE * stream = NULL;

	stream = fopen(SAVE_FILE_NAME, "wb");

	if (stream != NULL)
	{
		fwrite((void *)_buffer, sizeof(BYTE), MAX_SAVE_BUFFER_CACHE, stream);
	}

	if (stream != NULL)
	{
		fclose(stream);
	}

#endif

	if (_isUseIcloud == CUserData::enumUseIcloud::ICLOUD_USE)
	{
		SaveToIcloud();
	}
}
#define GAME_TEST  0

void CUserData::UserDataLoad()
{
	int saved = 0;
	Buffer* buff = OS_LoadAppData(SAVE_FILE_NAME);
	if (buff != NULL)
	{
		saved = buff->Length();
		memcpy(_buffer, buff->Data(), saved);
		delete buff;
	}
	printf("Load data saved = %d", saved);

	if (saved == 0)
	{
#if GAME_TEST
		ResetIcloud();
#endif
		UserDefault();
		UserDataSave();
#if !GAME_TEST
		DefaultIcloud();
#endif
	}
	else
	{
		LoadFromBuffer();
		if (_isUseIcloud == CUserData::enumUseIcloud::ICLOUD_USE)
		{
			CheckIcloud();
		}
	}


}
void CUserData::LoadFromBuffer()
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
	_version = GAME()->GetInt64At(_buffer, SAVE_FORMAT_CURRENT_VERSION);
	for (int i = 0; i < 128; i++)
	{
		_userName[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_NAME + i);
		_userID[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_ID + i);
	}
	game->_high_score = GAME()->GetInt16At(_buffer, SAVE_FORMAT_GAME_HIGHT_SCORE);
	GAME()->SetSoundEnable(GAME()->GetByteAt(_buffer, SAVE_FORMAT_SOUND_ENABLE));
	GAME()->SetMusicEnable(GAME()->GetByteAt(_buffer, SAVE_FORMAT_MUSIC_ENABLE));
	GAME()->_notificationEnable = GAME()->GetByteAt(_buffer, SAVE_FORMAT_NOTIFY_ENABLE);	

	_save_cloud_version = GAME()->GetInt64At(_buffer, SAVE_FORMAT_SAVE_CLOUD_VERSION);
	_hadRemovedAds = GAME()->GetByteAt(_buffer, SAVE_FORMAT_SAVE_REMOVE_ADS);
	_hadLoginFB = GAME()->GetByteAt(_buffer, SAVE_FORMAT_SAVE_HAS_FB_LOGIN);

	for (int i = 0; i < SAVE_CHARACTER_NAME_LENGTH_CATCH; i++)
	{
		_userNameUnicode[i] = GAME()->GetInt32At(_buffer, SAVE_FORMAT_USER_NAME_UNICODE + i * 4);
	}
	_userNameUnicodeLenght = GAME()->GetInt16At(_buffer, SAVE_FORMAT_USER_NAME_LENGHT_UNICODE);
	_levelMultiplier = GAME()->GetInt16At(_buffer, SAVE_FORMAT_USER_LEVEL);

	game->_quest._status = GAME()->GetByteAt(_buffer, SAVE_FORMAT_CURRENT_STATUS_OBJECTIVE);
	game->_quest._currentQuest = GAME()->GetInt16At(_buffer, SAVE_FORMAT_CURRENT_OBJECTIVE);
	game->_quest._currentValue = GAME()->GetInt16At(_buffer, SAVE_FORMAT_CURRENT_VALUE_OBJECTIVE);
	game->_quest._totalCoin = GAME()->GetInt16At(_buffer, SAVE_FORMAT_OBJECTIVE_COIN);
	game->_quest._dayPlay = GAME()->GetInt16At(_buffer, SAVE_FORMAT_OBJECTIVE_DAY_PLAY);
	game->_quest._level = GAME()->GetInt16At(_buffer, SAVE_FORMAT_MULTI_LEVEL);
	game->_quest._percent = GAME()->GetInt16At(_buffer, SAVE_FORMAT_PERCENR_QUEST);
	_day_next_login = GAME()->GetInt64At(_buffer, SAVE_FORMAT_DAY_NEXT_LOGIN);
	_typeCharacterUse = GAME()->GetInt16At(_buffer, SAVE_FORMAT_CHARACTER_USING);
    _numRetryGame = GAME()->GetInt64At(_buffer, SAVE_FORMAT_NUM_RETRY);
    _isRating = GAME()->GetByteAt(_buffer, SAVE_FORMAT_HAS_RATE);
    _countShowRate = GAME()->GetInt16At(_buffer, SAVE_FORMAT_COUNT_SHOW_RATE);
    _stepTutorial = GAME()->GetInt16At(_buffer, SAVE_FORMAT_STEP_TUTORIAL);
    _isUseIcloud = GAME()->GetByteAt(_buffer, SAVE_FORMAT_IS_USED_ICLOUD);
    _isShowNotifi = GAME()->GetByteAt(_buffer, SAVE_FORMAT_NOTIFY_ENABLE);
    game->_count_show_pineads = GAME()->GetInt16At(_buffer, SAVE_FORMAT_COUNT_SHOW_PINEADS);

	_play_game_num = GAME()->GetInt32At(_buffer, SAVE_FORMAT_USER_PLAY_NUM);

	for (int i = 0; i<128; i++)
	{
		_game_promo_deviceID[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_DEVICE_ID);
		_game_promo_icon_id[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_ICON_ID);
		_game_promo_link_store[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_LINK_STORE);
	}
	_game_promo_delay_show = GAME()->GetInt64At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_DELAY_SHOW);
	_game_promo_has_show = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_HAS_SHOW);
	_game_promo_order = GAME()->GetInt16At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_ORDER);
	_game_promo_revision = GAME()->GetInt16At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_REVISION);
	_has_touch_promo = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_HAS_TOUCH_PROMO);

	if (_version < GAME_CURRENT_VERSION)
	{
		SynForNewVersion(_version + 1);
		UserDataSave();
	}
}
void CUserData::SaveToBuffer()
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
	_save_cloud_version++;
	GAME()->SetInt64At(_buffer, SAVE_FORMAT_CURRENT_VERSION, _version);
	for (int i = 0; i < 128; i++)
	{
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_NAME + i, _userName[i]);
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_ID + i, _userID[i]);
	}
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_GAME_HIGHT_SCORE, game->_high_score);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_NOTIFY_ENABLE, GAME()->_notificationEnable);
	GAME()->SetInt64At(_buffer, SAVE_FORMAT_SAVE_CLOUD_VERSION, _save_cloud_version);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_SAVE_REMOVE_ADS, _hadRemovedAds);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_SAVE_HAS_FB_LOGIN, _hadLoginFB);
	for (int i = 0; i < SAVE_CHARACTER_NAME_LENGTH_CATCH; i++)
	{
		GAME()->SetInt32At(_buffer, SAVE_FORMAT_USER_NAME_UNICODE + i * 4, _userNameUnicode[i]);
	}
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_USER_NAME_LENGHT_UNICODE, _userNameUnicodeLenght);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_SOUND_ENABLE,GAME()->_soundEnable);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_MUSIC_ENABLE, GAME()->_musicEnable);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_USER_LEVEL, _levelMultiplier);

	GAME()->SetByteAt(_buffer, SAVE_FORMAT_CURRENT_STATUS_OBJECTIVE, game->_quest._status);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_CURRENT_OBJECTIVE, game->_quest._currentQuest);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_CURRENT_VALUE_OBJECTIVE, game->_quest._currentValue);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_OBJECTIVE_COIN, game->_quest._totalCoin);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_OBJECTIVE_DAY_PLAY, game->_quest._dayPlay);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_MULTI_LEVEL, game->_quest._level);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_PERCENR_QUEST, game->_quest._percent);
	GAME()->SetInt64At(_buffer, SAVE_FORMAT_DAY_NEXT_LOGIN, (int)_day_next_login);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_CHARACTER_USING, _typeCharacterUse);
    GAME()->SetInt64At(_buffer, SAVE_FORMAT_NUM_RETRY,_numRetryGame);
    GAME()->SetByteAt(_buffer, SAVE_FORMAT_HAS_RATE,_isRating);
    GAME()->SetInt16At(_buffer, SAVE_FORMAT_COUNT_SHOW_RATE,_countShowRate);
    GAME()->SetInt16At(_buffer, SAVE_FORMAT_STEP_TUTORIAL,_stepTutorial);
    GAME()->SetByteAt(_buffer, SAVE_FORMAT_IS_USED_ICLOUD,_isUseIcloud);
    GAME()->SetByteAt(_buffer, SAVE_FORMAT_NOTIFY_ENABLE,_isShowNotifi);
    GAME()->SetInt16At(_buffer, SAVE_FORMAT_COUNT_SHOW_PINEADS,game->_count_show_pineads);

	GAME()->SetInt32At(_buffer, SAVE_FORMAT_USER_PLAY_NUM, _play_game_num);

	for (int i = 0; i<128; i++)
	{
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_DEVICE_ID, _game_promo_deviceID[i]);
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_ICON_ID, _game_promo_icon_id[i]);
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_LINK_STORE, _game_promo_link_store[i]);
	}
	GAME()->SetInt64At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_DELAY_SHOW, _game_promo_delay_show);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_HAS_SHOW, _game_promo_has_show);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_ORDER, _game_promo_order);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_REVISION, _game_promo_revision);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_HAS_TOUCH_PROMO, _has_touch_promo);
    
}
void CUserData::SetUserID(char* userid)
{
	sprintf(_userID, "%s", userid);
}
const char* CUserData::GetUserID()
{
	return _userID;
}
void CUserData::SetUserName(char* username)
{
	sprintf(_userName, "%s", username);
}
const char* CUserData::GetName()
{
	return _userName;
}
void CUserData::SetFBID(char* userid)
{
	sprintf(_user_fb_id, "%s", userid);
}
const char* CUserData::GetFBID()
{
	return _user_fb_id;
}
void CUserData::SetFBName(char* username)
{
	sprintf(_user_fb_name, "%s", username);
}
const char* CUserData::GetFBName()
{
	return _user_fb_name;
}
void CUserData::SetAvatarURL(char* url)
{
	sprintf(_avatar_URL, "%s", url);
}
const char* CUserData::GetAvatarURL()
{
	return _avatar_URL;
}
//check syn icloud

void callback(ECloudKeyValueChangeReason reason, std::vector<std::string> keys)
{
	switch (reason)
	{
	case ECloudKeyValueChangeReason::AccountChange:
		PDEBUG("\n #icloud Change reason Account Change\n");
		GAME()->_icloudStatus = 1;
		break;
	case ECloudKeyValueChangeReason::InitialSyncChange:
		PDEBUG("\n #icloud Change reason InitialSyncChange\n");
		GAME()->_icloudStatus = 2;
		break;
	case ECloudKeyValueChangeReason::QuotaViolationChange:
		PDEBUG("\n #icloud Change reason QuotaViolationChange\n");
		GAME()->_icloudStatus = 3;
		break;
	case ECloudKeyValueChangeReason::ServerChange:
		PDEBUG("\n #icloud Change reason ServerChange\n");
		GAME()->_icloudStatus = 4;
		break;
	}
	for (int i = 0; i< keys.size(); i++)
	{
		PDEBUG("key change:%s\n", keys[i].c_str());
	}
}
void CUserData::ResetIcloud()
{
#if defined(MAC_OS) /*|| defined(_WINDOWS)*/
	std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);
	if (service == nullptr)
	{
		PDEBUG("\n #icloud fail");
	}
	else
	{
		//PDEBUG("\n #icloud success");
		if (GAME()->_icloudStatus == 0)
		{
			service->synchronize();
			std::vector<BYTE> data = service->readBytes("data");
			{
				UserDefault();
				SaveToBuffer();
				service->write("data", _buffer, SAVE_FORMAT_END);
                PDEBUG("\n #icloud reset success");
			}
		}
	}
#endif
}
bool CUserData::SaveToIcloud()
{
	bool ret = false;
#if defined(MAC_OS)/*|| defined(_WINDOWS)*/
	std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);
	if (service == nullptr)
	{
		PDEBUG("\n #icloud fail");
	}
	else
	{
		//PDEBUG("\n #icloud success");
		if (GAME()->_icloudStatus == 0)
		{
			service->synchronize();
			/*int server_state = CheckCanPushDataSave();
			if (server_state == ICLOUD_STATE_READY_SYNS 
				|| server_state == ICLOUD_STATE_OLD_VERSION 
				|| server_state == ICLOUD_STATE_NODATA)*/
			{
				SaveToBuffer();
				service->write("data", _buffer, SAVE_FORMAT_END);
				PDEBUG("\n #icloud save success");
				ret = true;
			}
		}
	}
#endif
	return ret;
}
int CUserData::CheckCanPushDataSave()
{
#if defined(MAC_OS)/*|| defined(_WINDOWS)*/
	int state = CUserData::enumStateIcloud::ICLOUD_STATE_NODATA;
	if (!Device::IsNetworkAvailable())
	{
		state = CUserData::enumStateIcloud::ICLOUD_STATE_DISCONECT;
	}
	else
	{
		std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);
		if (service == nullptr)
		{
			PDEBUG("\n #icloud fail");
		}
		else
		{
			//PDEBUG("\n #icloud success");
			if (GAME()->_icloudStatus == 0)
			{
				service->synchronize();
				std::vector<BYTE> data = service->readBytes("data");
				if (data.size() > 0)
				{
					char buffer[MAX_SAVE_BUFFER_CACHE];
					sprintf(buffer, "");
					for (int i = 0; i < SAVE_FORMAT_END; i++)
					{
						buffer[i] = data[i];
					}
					INT_TIME save_version = GAME()->GetInt64At(buffer, SAVE_FORMAT_SAVE_CLOUD_VERSION);
					INT_TIME game_version = GAME()->GetInt64At(buffer, SAVE_FORMAT_CURRENT_VERSION);
					INT_TIME highScore = GAME()->GetInt16At(buffer, SAVE_FORMAT_GAME_HIGHT_SCORE);
                    INT_TIME numPlay = GAME()->GetInt64At(buffer, SAVE_FORMAT_NUM_RETRY);
                   PDEBUG("\n #icloud save_version = %lld, game_version = %lld, numPlay = %lld, highScore = %lld",save_version,game_version,numPlay,highScore);
					if (game_version < _version)
					{
						state = CUserData::enumStateIcloud::ICLOUD_STATE_OLD_VERSION;
					}
					else
					{
						if (save_version < _save_cloud_version)
						{
							state = ICLOUD_STATE_OLD_VERSION;
						}
						else if (save_version == _save_cloud_version)
						{
							HSGame *game = (HSGame*)GAME()->_current_game;
							if (highScore < game->_high_score)
							{
								state = CUserData::enumStateIcloud::ICLOUD_STATE_OLD_VERSION;
							}
							else if (highScore == game->_high_score)
							{
								if (numPlay < _numRetryGame)
								{
									state = CUserData::enumStateIcloud::ICLOUD_STATE_OLD_VERSION;
								}
								else if (numPlay == _numRetryGame)
								{
									state = CUserData::enumStateIcloud::ICLOUD_STATE_READY_SYNS;
								}
								else if (numPlay > _numRetryGame)
								{
									state = CUserData::enumStateIcloud::ICLOUD_STATE_NEW_VERSION;
								}
							}
							else if (highScore > game->_high_score)
							{
								state = CUserData::enumStateIcloud::ICLOUD_STATE_NEW_VERSION;
							}
						}
						else if (save_version > _save_cloud_version)
						{
							state = CUserData::enumStateIcloud::ICLOUD_STATE_NEW_VERSION;
						}
					}
				}
				else
				{
					state = CUserData::enumStateIcloud::ICLOUD_STATE_NODATA;
				}
			}
		}
	}
	return state;
#else
	return -1;
#endif
}
void CUserData::LoadFromIcloud()
{
#if defined(MAC_OS)/*|| defined(_WINDOWS)*/
	std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);
	if (service == nullptr)
	{
		PDEBUG("icloud fail \n");
	}
	else
	{
		//PDEBUG("\n #icloud success");
		if (GAME()->_icloudStatus == 0)
		{
			service->synchronize();
			std::vector<BYTE> data = service->readBytes("data");
			if (data.size() > 0)
			{
				PDEBUG("\n #icloud load success");
				for (int i = 0; i < SAVE_FORMAT_END; i++)
				{
					_buffer[i] = data[i];
				}
				LoadFromBuffer();
			}
		}
	}
#endif
}
void CUserData::DefaultIcloud()
{
#if defined(MAC_OS) /*|| defined(_WINDOWS)*/
	std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);
	if (service == nullptr)
	{
		PDEBUG("\n #icloud fail");
	}
	else
	{
		//PDEBUG("\n #icloud success");
		if (GAME()->_icloudStatus == 0)
		{
			service->synchronize();
			std::vector<BYTE> data = service->readBytes("data");
			HSGame *game = (HSGame*)GAME()->_current_game;
            game->_isWaiting = true;
			if (data.size() > 0)
			{
				char tag[10];
				sprintf(tag, "%d", ALERT_TAG_CONFRIM_ICOUND_LOAD_SAVE);
				ShowAlertMessage_OKCancel("Do you want to play with your latest iCloud Backup?", "iCloud Sync", tag);
                PDEBUG("\n #icloud request load");
			}
			else
			{
				char tag[10];
				sprintf(tag, "%d", ALERT_TAG_CONFRIM_SYNC_ICOUND);
				ShowAlertMessage_OKCancel("Do you want to safeguard your progress to iCloud?", "iCloud Backup", tag);
                PDEBUG("\n #icloud aks sync");
			}
		}
	}
#endif
}
void CUserData::CheckIcloud(bool showMessage)
{
    
#if defined(MAC_OS) /*|| defined(_WINDOWS)*/
	std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);
	if (service == nullptr)
	{
		PDEBUG("\n #icloud fail");
	}
	else
	{
		//PDEBUG("\n #icloud success");
		if (GAME()->_icloudStatus == 0)
		{
			service->synchronize();
			int server_state = CheckCanPushDataSave();
			PDEBUG("\n #icloud server_state = %d", server_state);
			if (server_state == CUserData::enumStateIcloud::ICLOUD_STATE_NODATA)
			{
				char tag[10];
				sprintf(tag, "%d", ALERT_TAG_CONFRIM_SYNC_ICOUND);
				ShowAlertMessage_OKCancel("Do you want to safeguard your progress to iCloud?", "iCloud Backup", tag);
                PDEBUG("\n #icloud ask sync");
			}
			else if (server_state == CUserData::enumStateIcloud::ICLOUD_STATE_NEW_VERSION)
			{
				if (showMessage)
				{
					char tag[10];
					sprintf(tag, "%d", ALERT_TAG_CONFRIM_ICOUND_LOAD_SAVE);
					ShowAlertMessage_OKCancel("Do you want to play with your latest iCloud Backup?", "iCloud Sync", tag);
                    PDEBUG("\n #icloud request load");
				}
				else
				{
					LoadFromIcloud();
					UserDataSave();
				}
			}
			else
			{
				if (_isUseIcloud == CUserData::enumUseIcloud::ICLOUD_USE)
				{                    
					if (SaveToIcloud())
					{
						if (showMessage)
						{
							char tag[10];
							sprintf(tag, "%d", ALERT_TAG_CONFRIM_ICOUND_SYNC_COMPLETE);
							ShowAlertMessage_OK("Your data has been saved!", "Congratulations!", tag);
						}
					}
				}
				else
				{
					char tag[10];
					sprintf(tag, "%d", ALERT_TAG_CONFRIM_SYNC_ICOUND);
					ShowAlertMessage_OKCancel("Do you want to safeguard your progress to iCloud?", "iCloud Backup", tag);
                    PDEBUG("\n #icloud ask sync");
				}
			}
		}
	}
#endif
}

void CUserData::NetWorkError()
{
#if defined(MAC_OS)
	char tag[10];
	sprintf(tag, "%d", ALERT_TAG_CONFRIM_NETWORK_ERROR);
	ShowAlertMessage_OK("Connection is not available!", "Error!", tag);
#endif
}


//#if defined (ANDROID_OS)
void CUserData::CheckLocalAndCloudSaveFile(bool isauto)
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
#if ANDROID_OS
	if (!Device::IsNetworkAvailable())
	{
		if (!isauto)
		{
			OS_ShowMessageBox("Error!", "Connection is not available!");
		}
		return;
	}
	
	if (!GAME()->_async_task_manager._is_login_google)
	{
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_IS_LOG_IN_GOOGLE_PLAY);
		GAME()->_async_task_manager._is_force_login_google_play = true;
		GAME()->_async_task_manager._on_process_sync_cloud = true;
		return;
	}
	Buffer * buff = OS_LoadAppData(CLOUD_SAVE_FILE_NAME);
	if (buff != NULL)
	{
		char* server_buff = (char*)buff->Data();
		if (!isauto)
		{
			GAME()->_async_task_manager._on_process_sync_cloud = true;
		}
		int svLevel_cur = GAME()->GetInt64At(server_buff, SAVE_FORMAT_MULTI_LEVEL);
		int svNumPlay = GAME()->GetInt16At(server_buff, SAVE_FORMAT_USER_PLAY_NUM);
		printf("\nfrom cloud battle_cur = %d", svLevel_cur);
		int ver = (int)GAME()->GetInt64At(server_buff, SAVE_FORMAT_CURRENT_VERSION);
		PDEBUG("\n #icloud save_version = %d, level_cur = %d", ver, svLevel_cur);
		if (svLevel_cur < game->_quest._level)
		{
			ConfirmSyncSaveToCloud(isauto);
		}
		else if (svLevel_cur == game->_quest._level)
		{
			if (!isauto)
			{
				if (svNumPlay > _play_game_num)
				{
					ConfirmSyncSaveFromCloud();
				}
				else
				{
					OS_ShowMessageBox("Congratulations!", "Your data has been saved!");
					GAME()->_async_task_manager._on_process_sync_cloud = false;
				}
			}
		}
		else if (svLevel_cur > game->_quest._level)
		{
			if (!isauto)
			{
				ConfirmSyncSaveFromCloud();
			}
		}
		if (ver < GAME_CURRENT_VERSION)
		{
			if (svLevel_cur > game->_quest._level)
			{
				if (!isauto)
				{
					ConfirmSyncSaveFromCloud();
				}
			}
			else
			{
				ConfirmSyncSaveToCloud(isauto);
			}

		}

		buff->Release();
	}
	else
	{
		ConfirmSyncSaveToCloud(isauto);
	}
#endif
}

void CUserData::ConfirmSyncSaveToCloud(bool isauto)
{
#if ANDROID_OS
	if (isauto)
	{
		UpdateCloudSaveFile();
		GAME()->_async_task_manager._is_force_sync_save_to_cloud = true;
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_UPDATE_CLOUND_SAVE);
	}
	else
	{
		sprintf(GAME()->_async_task_manager._message_box_title, "%s", "Google Drive sync");
		sprintf(GAME()->_async_task_manager._message_box_message, "%s", "Do you want to safeguard your progress to Google Drive?");
		GAME()->_async_task_manager._message_box_confirm_type = CAsyncTasks::enumCloudSaveConfirmType::USE_SAVE_FROM_LOCAL;
		GAME()->_async_task_manager._on_process_sync_cloud = true;
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_SHOW_MESSAGE_BOX);
	}
#endif
}
void CUserData::ConfirmSyncSaveFromCloud()
{
#if ANDROID_OS
	sprintf(GAME()->_async_task_manager._message_box_title, "%s", "Google Drive backup");
	sprintf(GAME()->_async_task_manager._message_box_message, "%s", "Do you want to play with your latest Google Drive Backup?");
	GAME()->_async_task_manager._message_box_confirm_type = CAsyncTasks::enumCloudSaveConfirmType::USE_SAVE_FROM_CLOUD;
	GAME()->_async_task_manager._on_process_sync_cloud = true;
	GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_SHOW_MESSAGE_BOX);
#endif
}
void CUserData::UpdateCloudSaveFile()
{
#if ANDROID_OS
	PDEBUG("UpdateCloudSaveFile with local");
	OS_SaveAppData(CLOUD_SAVE_FILE_NAME, _buffer, MAX_SAVE_BUFFER_CACHE, false);
#endif
}
void CUserData::UpdateLocalSaveFile(Buffer* buffer)
{
	HSGame *game = (HSGame*)GAME()->_current_game;
#if ANDROID_OS
	PDEBUG("UpdateLocalSaveFile with cloud");
	memcpy(_buffer, buffer->Data(), SAVE_FORMAT_END);
	LoadFromBuffer();
	SaveToBuffer();
	CheckUnlockAllAchievements();
	OS_SaveAppData(SAVE_FILE_NAME, _buffer, MAX_SAVE_BUFFER_CACHE, false);
	game->SetState(k_GS_PLAYGAME);
#endif
}
void CUserData::CheckUnlockAllAchievements()
{
	HSGame *game = (HSGame*)GAME()->_current_game;
#if ANDROID_OS
	bool has_achievement_need_unlock = false;
	//int total = GAME()->_poker_flat_game->_game_design.leveldesign.SheetAchievement.int32Value._totalData;
	if (GAME()->_async_task_manager._need_check_unlock_all_achievement)
	{
		GAME()->_async_task_manager._need_check_unlock_all_achievement = false;
		/*if (GAME()->num_big_flower >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[20] = true;
		}
		else if(GAME()->num_big_flower >= 500)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[19] = true;
		}
		if (GAME()->num_large_flower >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[22] = true;
		}
		else if (GAME()->num_large_flower >= 500)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[21] = true;
		}
		if (GAME()->num_lily >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[4] = true;
		}
		else if (GAME()->num_lily >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[5] = true;
		}
		else if(GAME()->num_lily >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[6] = true;
		}

		if (GAME()->num_sunflower >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[3] = true;
		}
		else if (GAME()->num_sunflower >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[2] = true;
		}
		else if (GAME()->num_sunflower >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[1] = true;
		}

		if (GAME()->num_violet >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[9] = true;
		}
		else if (GAME()->num_violet >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[8] = true;
		}
		else if (GAME()->num_violet >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[7] = true;
		}

		if (GAME()->num_lotus >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[12] = true;
		}
		else if (GAME()->num_lotus >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[11] = true;
		}
		else if (GAME()->num_lotus >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[10] = true;
		}

		if (GAME()->num_rose >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[15] = true;
		}
		else if (GAME()->num_rose >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[14] = true;
		}
		else if (GAME()->num_rose >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[13] = true;
		}

		if (GAME()->num_orchild >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[18] = true;
		}
		else if (GAME()->num_orchild >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[17] = true;
		}
		else if (GAME()->num_orchild >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[16] = true;
		}*/

		/*for (int i = 0; i < game->_hight_score; i++)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[i] = true;
		}*/
	}
	if (has_achievement_need_unlock && GAME()->_async_task_manager._is_login_google)
	{
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_UNLOCK_ACHIEVEMENT);
	}
#endif
}
