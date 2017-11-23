#include "pch.h"
#include "PineTwitterController.h"
#include "../GameCore/Os_Functions.h"

extern "C" void OS_PostScreenToTwitter(const char* feed);
extern "C" void OS_PostImageToTwitter(const char* feed, const char* realUrlImage);
extern "C" void OS_PostToTwitter(const char* feed);
extern "C" void OS_LoadFriendTwitter();
#if USE_PINESOCIAL_AT_ID_STRING
extern "C" const char* OS_GetTwitterAvartar(const char* id);
#else
extern "C" const char* OS_GetTwitterAvartar(INT_TIME id);
#endif

void PineTwitterController::InviteFriend()
{
    
}
void PineTwitterController::LoadAvatarURL()
{
#if USE_PINESOCIAL_AT_ID_STRING
    const char *temp = GetUserAvartarURL(_idString);
#else
    const char *temp = GetUserAvartarURL(_user_id);
#endif
    if(temp != NULL)
    {
        sprintf(_url_avatar,"%s",temp);
    }
    else
    {
        _url_avatar[0] = '\0';
    }
}

#if USE_PINESOCIAL_AT_ID_STRING

const char* PineTwitterController::GetUserAvartarURL(const char* id)
{
#if defined(MAC_OS)
    return OS_GetTwitterAvartar(id);
#else
    return NULL;
#endif
}

#else

const char* PineTwitterController::GetUserAvartarURL(INT_TIME id)
{
#if defined(MAC_OS)
    return OS_GetTwitterAvartar(id);
#else
	return NULL;
#endif
}
#endif

PineTwitterController::PineTwitterController()
{
    _type_system = TW_SYSTEM;
    _flag =  SOCIAL_FLAG_NONE;
}
bool PineTwitterController::Init()
{
    _state = PINESOCIAL_STATE_INIT;
    _flag = SOCIAL_FLAG_LOGGING;
	OS_InitTwitter();
    return true;
}
bool PineTwitterController::PostToWall(const char* feed)
{
#if defined(MAC_OS)
    OS_PostToTwitter(feed);
#else
	OS_PostTwitter(feed);
#endif
    return true;
}
bool PineTwitterController::PostScreenToWall(const char* feed)
{
    OS_PostScreenToTwitter(feed);
    return true;
}

bool PineTwitterController::PostImageToWall(const char* feed, const char* realUrlImage)
{
    OS_PostImageToTwitter(feed, realUrlImage);
    return true;
}

void PineTwitterController::LoadFriends()
{
    _state= PINESOCIAL_STATE_LOAD_FRIEND;
    _flag = SOCIAL_FLAG_NONE;
    OS_LoadFriendTwitter();
}

void PineTwitterController::LoadFollowers()
{
    
}
void PineTwitterController::LoadFollowings()
{
    
}