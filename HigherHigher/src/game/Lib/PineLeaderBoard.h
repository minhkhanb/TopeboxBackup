#if !_PINE_LEADERBOARD_H
#define _PINE_LEADERBOARD_H 1
#include "PineImage.h"



struct accountInfo
{
    INT_TIME _id;
    char _userName[128];
    PineImage _avatar;
    BYTE _avatarState;
    INT_TIME _highScore;
    char _avatarUrl[1024];
    int _rank_logic;
};

class PineLeaderBoard
{
public:
   PineLeaderBoard();
   accountInfo **_globalInfo;
    void RemoveAccount(INT_TIME accountid);
    int _globalmax;
    int _globalcur;
    void InitGlobalBoard(int num);
    void ExpandGlobalBoard(int num);
    void AddUserGlobal(accountInfo *account);
    accountInfo* GetAccountAt(int index);
    int GetNumCurrentUser(){return _globalcur;};
    
};


#endif