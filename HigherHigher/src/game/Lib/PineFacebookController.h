

#if !_PINE_FACEBOOK_CONTROLLER_H
#define _PINE_FACEBOOK_CONTROLLER_H 1

#define AVATAR_STATE_NONE				0
#define AVATAR_STATE_SAVE_AS			1
#define AVATAR_STATE_LOADING_BINARY		2
#define AVATAR_STATE_INIT_SUCCESS		3
#define AVATAR_STATE_INIT_FAILED		4


#define FACEBOOK_ERROR_NO_NETWORK				400
#define FACEBOOK_ERROR_PROCESSING				-1
#define FACEBOOK_ERROR_NONE						0
#define FACEBOOK_DATA_SUCCESS					1
#define FACEBOOK_ERROR_UNKNOW					2
#define FACEBOOK_ERROR_NO_RECORD				3
#define FACEBOOK_ERROR_NO_SESSION               4

#if USE_PINESOCIAL_AT_ID_STRING
#define USER_AVATAR_FILE_NAME			"A%s.png"
#else
#define USER_AVATAR_FILE_NAME			"A%lld.png"
#endif
#define USER_AVATAR_W_H					(43)
#define USER_AVATAR_MAX_LENGTH			(1024)


#define SERVER_DATA_ERROR_NO_NETWORK                    400
#define SERVER_DATA_SUCCESS                             1
#define SERVER_DATA_ERROR_UNKNOW                        2
#define SERVER_DATA_ERROR_NO_RECORD                     3


#include "PineSocial.h"
//extern "C" void OS_InitTwitter();

class PineFacebookController:public PineSocial
{
   
    
public:
    static PineFacebookController* _share_instance;
    static PineFacebookController* GetInstance();
    static void StaticInviteFriend();
    ~PineFacebookController(){;};
    PineFacebookController();
    bool Init();
	bool InitNoneWait();
    void LoadAvatarURL();
    bool PostToWall(const char* feed);
    bool PostScreenToWall(const char* feed);
    bool PostImageToWall(const char* feed, const char* realUrlImage);
    void LoadFriends();
    void LoadFollowers();
    void LoadFollowings();
    static bool HasLoginFacebook();
#if USE_PINESOCIAL_AT_ID_STRING
    const char* GetUserAvartarURL(const char* id);
#else
    const char* GetUserAvartarURL(INT_TIME id);
#endif
};


#endif