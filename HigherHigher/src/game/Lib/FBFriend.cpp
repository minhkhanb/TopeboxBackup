#include "pch.h"
#include "FBFriend.h"

#include <string.h>
#include <math.h>
#include "../GameCore/Os_Functions.h"
#include "Sprite/CSprite.h"


CFriend::CFriend()
{
    _id = 0;
    _image.SetTextureMaxSize(64,64);
}


void CPineFriendList::InitFriendList()
{
    for(int i = 0; i < FRIEND_LIST_MAX; i++)
    {
        _list[i] = NULL;
    }
}

void CPineFriendList::UnFriend(UINT64 id)
{
    int index = -1;
    for(int i = 0; i < _num_friend; i++)
    {
        if(_list[i]->_id == id)
        {
            
        }
    }
}
void CPineFriendList::AddFriend(UINT64 id, int type, const char* name)
{
    
}