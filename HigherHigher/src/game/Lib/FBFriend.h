#if !_FB_FRIEND_H
#define _FB_FRIEND_H 1
#include "../GameCore/CGame.h"
#include "PineImage.h"
#define FRIEND_LIST_MAX 5000
#define FB_NAME_LENGTH_MAX 128
#define FRIEND_PINE_TYPE 1
#define FRIEND_GC_TYPE   2
#define FRIEND_FB_TYPE   3
#define FRIEND_LOCAL_FILE "PEFRIENDLOCALINFO.bpine"
class CFriend
{
public:
    int _friend_type;
    UINT64 _id;
    char _user_name[FB_NAME_LENGTH_MAX];
    PineImage _image;
    CFriend();
};

class CPineFriendList
{
public:
    CFriend *_list[FRIEND_LIST_MAX];
    int _num_friend;
    void InitFriendList();
    void LoadFriendFromLocal();
    void SaveFriendToLocal();
    void UnFriend(UINT64 id);
    void AddFriend(UINT64 id, int type, const char* name);
};
#endif