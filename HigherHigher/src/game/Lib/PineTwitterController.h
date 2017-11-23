#if !_PINE_TWITTER_CONTROLLER_H
#define _PINE_TWITTER_CONTROLLER_H 1
#include "PineSocial.h"

class PineTwitterController:public PineSocial
{
   
    
public:
    void InviteFriend();
    ~PineTwitterController();
    PineTwitterController();
    bool Init();
    void LoadAvatarURL();
    bool PostToWall(const char* feed);
    bool PostScreenToWall(const char* feed);
    bool PostImageToWall(const char* feed, const char* realUrlImage);
    void LoadFriends();
    void LoadFollowers();
    void LoadFollowings();
#if USE_PINESOCIAL_AT_ID_STRING
    const char* GetUserAvartarURL(const char* id);
#else
    const char* GetUserAvartarURL(INT_TIME id);
#endif
};


#endif