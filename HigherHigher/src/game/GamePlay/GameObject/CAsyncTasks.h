#ifndef _CASYNC_TASKS_H
#define _CASYNC_TASKS_H

#include "../../Lib/PineSocialAsync.h"
#define MAX_COMMAND		10
#define MAX_ACHIEVEMENT	46
#define MAX_LEADERBOARD  6

class  CAsyncTasks
{
public:
	CAsyncTasks()
	{
#if ANDROID_OS
		_is_login_google = false;
#endif
		for (int i = 0; i < MAX_COMMAND; i++)
		{
			_command[i]._typeCommand = enumTypeCommand::COMMAND_TYPE_NONE;
			_command[i]._stateCommand = enumStateCommand::COMMAND_STATE_NONE;
		}
	};

	enum enumTypeCommand
	{
		COMMAND_TYPE_NONE
		, COMMAND_TYPE_ASYNC_CHECK_NETWORK_STATUS
#if defined(ANDROID_OS)
		, COMMAND_TYPE_ASYN_LOAD_IAP
		, COMMAND_TYPE_ASYN_RESTORE_PURCHASE
		, COMMAND_TYPE_ASYN_LOG_IN_GOOGLE_PLAY
		, COMMAND_TYPE_ASYN_IS_LOG_IN_GOOGLE_PLAY
		, COMMAND_TYPE_ASYN_GOOGLE_PLAY_POST_SCORE
		, COMMAND_TYPE_ASYN_GOOGLE_PLAY_GET_LEADERBOARD_RANK
		, COMMAND_TYPE_ASYN_GOOGLE_PLAY_SHOW_LEADERBOARD
		, COMMAND_TYPE_ASYN_GOOGLE_PLAY_SHOW_ACHIEVEMENTS
		, COMMAND_TYPE_ASYN_GOOGLE_PLAY_UNLOCK_ACHIEVEMENT
		, COMMAND_TYPE_ASYN_UPDATE_CLOUND_SAVE
		, COMMAND_TYPE_ASYN_TAKE_SCREEN_SHOT
		, COMMAND_TYPE_ASYN_SHARE_SCREEN_SHOT_INTENT
		, COMMAND_TYPE_ASYN_SEND_EMAIL_INTENT
		, COMMAND_TYPE_ASYN_SHOW_MESSAGE_BOX
		, COMMAND_TYPE_ASYN_GOOGLE_PLUS_POST
		, COMMAND_TYPE_ASYN_GOOGLE_PLUS_POST_SCREEN
		, COMMAND_TYPE_ASYN_FIREBASE_ANALYTICS_LOG_EVENT
		, COMMAND_TYPE_ASYN_FACEBOOK_LOGIN
		, COMMAND_TYPE_ASYN_GAME_SPARK_LOGIN
		, COMMAND_TYPE_ASYN_GAME_SPARK_UPLOAD_SAVE
		, COMMAND_TYPE_ASYN_SHOW_MESSAGE_BOX_IAP
		, COMMAND_TYPE_ASYN_SHOW_MESSAGE_BOX_RESET_SAVE
#elif defined (_WINDOWS_PHONE8)
		, COMMAND_TYPE_ASYN_SHARE_LINK
		, COMMAND_TYPE_ASYN_SHARE_MEDIA
#endif
	};

	enum enumStateCommand
	{
		COMMAND_STATE_NONE,
		COMMAND_STATE_INIT,
		COMMAND_STATE_PROCESSING,
	};

	enum enumCloudSaveConfirmType
	{
		CONFIRM_TYPE_NONE,
		USE_SAVE_FROM_CLOUD,
		USE_SAVE_FROM_LOCAL,
	};

	enum enumBuyiAPConfirmType
	{
		iAPCONFIRM_TYPE_NONE,
		iAPCONFIRM_TYPE_BUY,
		iAPCONFIRM_TYPE_CANCEL,
	};
    
	struct structCommnad
	{
		enumTypeCommand _typeCommand;
		enumStateCommand _stateCommand;
	};

	structCommnad _command[MAX_COMMAND];
#if ANDROID_OS
	PineServiceHandle _svc_log_in_google_play, _svc_is_log_in_google_play, _svc_post_score_google_play, _svc_show_leaderBoard_google_play, 
		_svc_show_achievements_google_play,	_svc_unlock_achievements_google_play, _svc_google_plus_post, _svc_google_plus_post_screen, _svc_firebase_analytic_log_event,
		_svc_update_cloud_save, _svc_load_iap, _svc_retore_iap, _svc_take_screen_shot, _svc_share_screen_shot_intent, _svc_send_email_intent, _svc_show_messagebox,
		_svc_google_play_get_leaderboard_rank, _svc_facebook_login, _svc_game_spark_login, _svc_game_spark_upload_save;
	bool _list_id_group_achievement_need_check_unlock[MAX_ACHIEVEMENT] = { false };
	bool _need_check_unlock_all_achievement = true;
	bool _need_check_unlock_achievement = false;
	bool _process_unlock_achievement_done = false;
	bool _on_process_login_google_play = false;
	bool _on_process_share_screen_intent = false;

	int _id_group_achievement_current_check_unlock = 0;
	char _ach_id[128];
	char _iap_sku[128];
	char _share_screen_shot_message[256];
	char _send_email_title[128];
	char _send_email_body[256];
	char _message_box_title[64];
	char _message_box_message[128];
	bool _on_process_sync_cloud = false;
	enumCloudSaveConfirmType _message_box_confirm_type = enumCloudSaveConfirmType::CONFIRM_TYPE_NONE;
	bool _is_login_google;
	bool _on_process_show_leaderBoard = false;
	bool _is_force_sync_save_to_cloud = false;
	bool _is_force_login_google_play = false;
	char _current_achievement_unlock_id[128];
	bool _is_waiting_for_post_reward = false;
	bool _is_show_game_services_menu = false;
	bool _on_process_show_achievement = false;
	bool _on_post_reward = false;
	int _current_leader_board_rank = -1;

	//Firebase analytic log event
	char _log_title[64];
	char _log_description[128];
	char _log_type[32];
	char _log_value[32];
	char* _list_archive[MAX_ACHIEVEMENT] = {
		"CgkI2dLB4PgeEAIQAg",			//1
		"CgkI2dLB4PgeEAIQAw",			//2
		"CgkI2dLB4PgeEAIQBA",			//3
		"CgkI2dLB4PgeEAIQBQ",			//4
		"CgkI2dLB4PgeEAIQBg",			//5
		"CgkI2dLB4PgeEAIQBw",			//6
		"CgkI2dLB4PgeEAIQCA",			//7
		"CgkI2dLB4PgeEAIQCQ",			//8
		"CgkI2dLB4PgeEAIQCg",			//9
		"CgkI2dLB4PgeEAIQCw",			//10
		"CgkI2dLB4PgeEAIQDA",			//11
		"CgkI2dLB4PgeEAIQDQ",			//12
		"CgkI2dLB4PgeEAIQDg",			//13
		"CgkI2dLB4PgeEAIQDw",			//14
		"CgkI2dLB4PgeEAIQEA",			//15
		"CgkI2dLB4PgeEAIQEQ",			//16
		"CgkI2dLB4PgeEAIQEg",			//17
		"CgkI2dLB4PgeEAIQEw",			//18
		"CgkI2dLB4PgeEAIQFA",			//19
		"CgkI2dLB4PgeEAIQFQ",			//20
		"CgkI2dLB4PgeEAIQFg",			//21
		"CgkI2dLB4PgeEAIQFw",			//22
		"CgkI2dLB4PgeEAIQGA",			//23
		"CgkI2dLB4PgeEAIQGQ",			//24
		"CgkI2dLB4PgeEAIQGg",			//25
		"CgkI2dLB4PgeEAIQGw"			//26
	};
	char _achivement_id[128];
#endif
	void UpdateStateInit(int i);
	void UpdateStateProcessing(int i);
	void printfDebugType(int i);

	bool AddCommand(enumTypeCommand typeCommand);
	void EndCommand(int index);
	bool AlreadyHasCommand(enumTypeCommand typeCommand);
    void ClearAllCommand();

	void Run();
};

#endif