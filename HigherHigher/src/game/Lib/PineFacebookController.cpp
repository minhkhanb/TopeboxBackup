#include "pch.h"
#include "PineFacebookController.h"
extern "C" void OS_InitFacebook(PineSocial *social);
extern "C" void OS_PostScreenToFacebook(const char* feed);
extern "C" void OS_PostImageToFacebook(const char* feed, const char* realUrl);

#if defined (MAC_OS)
#if USE_PINESOCIAL_AT_ID_STRING
extern "C" const char* OS_GetFacebookAvartar(const char* id);
#else
extern "C" const char* OS_GetFacebookAvartar(INT_TIME id);
#endif
extern "C" void OS_LoadFriendFacebook(PineSocial *social);
#endif

extern "C" void OS_PostToTwitter(const char* feed);
extern "C" void OS_LoadFriendTwitter();
extern "C" const char* OS_GetTwitterAvartar(INT_TIME id);

//extern "C" void OS_FacebookInviteFriend();
extern "C" void OS_FacebookPostToFeedToWall(const char* feed);
extern "C" void OS_FacebookInviteFriend(const char* feed,const char* title,INT_TIME* friendlist,int numfriend);
PineFacebookController* PineFacebookController::_share_instance = NULL;
extern "C" bool OS_HasFacebookToken();
bool PineFacebookController::HasLoginFacebook()
{
    return OS_HasFacebookToken();
}
PineFacebookController* PineFacebookController::GetInstance()
{
    return _share_instance;
}
void PineFacebookController::StaticInviteFriend()
{
    //INT_TIME friendids[10];
    //friendids[0]= 100003734713059;
    //friendids[1]= 100000949356420;
    
    //OS_FacebookInviteFriend("play game now","Space Breaker",NULL,0);
}

/*void PineFacebookController::InviteFriend()
{
    StaticInviteFriend();
}*/

void PineFacebookController::LoadAvatarURL()
{
    /*
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
     */
}


#if USE_PINESOCIAL_AT_ID_STRING

const char* PineFacebookController::GetUserAvartarURL(const char* id)
{
#if defined (MAC_OS)
    return OS_GetFacebookAvartar(id);
#else
	return "";
#endif
}

#else

const char* PineFacebookController::GetUserAvartarURL(INT_TIME id)
{
#if defined (MAC_OS)
	return OS_GetFacebookAvartar(id);
#else
	return "";
#endif
}

#endif

PineFacebookController::PineFacebookController()
{
    _type_system = FB_SYSTEM;
    _flag =  SOCIAL_FLAG_NONE;
}
bool PineFacebookController::Init()
{
    _state = PINESOCIAL_STATE_INIT;
    _flag = SOCIAL_FLAG_LOGGING;
    OS_InitFacebook(_this);
    return true;
}
bool PineFacebookController::InitNoneWait()
{
	return false;
}

bool PineFacebookController::PostToWall(const char* feed)
{
    //OS_FacebookPostToFeedToWall(feed);
    return true;
}
bool PineFacebookController::PostScreenToWall(const char* feed)
{
    OS_PostScreenToFacebook(feed);
    return true;
}

bool PineFacebookController::PostImageToWall(const char* feed, const char* realUrlImage)
{
    OS_PostImageToFacebook(feed, realUrlImage);
    return true;
}

void PineFacebookController::LoadFriends()
{
    _state= PINESOCIAL_STATE_LOAD_FRIEND;
    _flag = SOCIAL_FLAG_NONE;
#if defined (MAC_OS)
    OS_LoadFriendFacebook(_this);
#endif
    
}
void PineFacebookController::LoadFollowers()
{
    
}
void PineFacebookController::LoadFollowings()
{
    
}