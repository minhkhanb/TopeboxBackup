//
//  PineGameCenterController.cpp
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 4/16/14.
//
//
#include "pch.h"
#include "PineGameCenterController.h"
int PineGameCenterController::_num_of_leaderboard = 0;
int PineGameCenterController::_current_leaderboard_loaded = 0;
char PineGameCenterController::_leader_boardid[LEADERBOARD_MAX_NUM][128]={};
INT_TIME PineGameCenterController::_my_score[LEADERBOARD_MAX_NUM]={};
bool PineGameCenterController::_used[LEADERBOARD_MAX_NUM]={};
extern "C" void OS_GameCenterAuthentication(PineSocial* social, bool wait = true);
extern "C" void OS_GameCenterLoadScore(PineSocial* social);
extern "C" void OS_GameCenterSubmitScore(INT_TIME value, const char* leaderboard);
extern "C" void OS_ShowLeaderboard(const char*  leaderboard);
extern "C" void OS_ShowLeaderAchivement();
extern "C" void OS_GameCenterLoadFriend(PineSocial* social);
extern "C" void OS_GameCenterSubmitAchievement(int percent, const char* leaderboard, bool noti);
extern "C" void OS_ResetAllAchievement();
extern "C" void OS_GameCenterLoadAchievement();

void PineGameCenterController::UpdateAchievement(const char* leaderboarid,float percent,bool noti)
{
    OS_GameCenterSubmitAchievement(percent,leaderboarid,noti);
}
void PineGameCenterController::SubmitMyScore(const char* leaderboardid, INT_TIME value)
{
    OS_GameCenterSubmitScore(value,leaderboardid);
}
void PineGameCenterController::LoadMyScore()
{
    _state = PINESOCIAL_STATE_LOAD_SCORE;
    OS_GameCenterLoadScore(_this);
}
INT_TIME PineGameCenterController::GetMyScore(const char* leaderboardid)
{
    INT_TIME ret = -1;
    for(int i = 0; i < _num_of_leaderboard; i++)
    {
        if(_used[i])
        {
            if(strcmp(_leader_boardid[i],leaderboardid) == 0)
            {
                ret = _my_score[i];
                break;
            }
        }
    }
    return ret;
}
void PineGameCenterController::ShowLeaderBoard(const char* leaderboarid)
{
    OS_ShowLeaderboard(leaderboarid);
}

bool PineGameCenterController::PostImageToWall(const char* feed, const char* realUrlImage)
{
    return true;
}
PineGameCenterController::PineGameCenterController()
{
    _type_system = GC_SYSTEM;
    _flag =  SOCIAL_FLAG_NONE;
}
bool PineGameCenterController::Init()
{
    _currentUnicodeUserNameLength = 0;
    _state = PINESOCIAL_STATE_INIT;
    _flag = SOCIAL_FLAG_LOGGING;
    OS_GameCenterAuthentication(_this);
    return true;
}

bool PineGameCenterController::InitNoneWait()
{
    _state = PINESOCIAL_STATE_INIT;
    _flag = SOCIAL_FLAG_LOGGING;
    OS_GameCenterAuthentication(_this, false);
    return true;
}

void PineGameCenterController::LoadAvatarURL()
{
    
}
bool PineGameCenterController::PostToWall(const char* feed)
{
    return true;
}
bool PineGameCenterController::PostScreenToWall(const char* feed)
{
    return true;
}
void PineGameCenterController::LoadFriends()
{
    _state =   PINESOCIAL_STATE_LOAD_FRIEND;
    OS_GameCenterLoadFriend(_this);
}
void PineGameCenterController::LoadFollowers()
{
    _state =   PINESOCIAL_STATE_LOAD_FRIEND;
    OS_GameCenterLoadFriend(_this);
}
void PineGameCenterController::LoadFollowings()
{
    _state =   PINESOCIAL_STATE_LOAD_FRIEND;
    OS_GameCenterLoadFriend(_this);
}

#if USE_PINESOCIAL_AT_ID_STRING

const char* PineGameCenterController::GetUserAvartarURL(const char* id)
{
    return NULL;
}

#else
const char* PineGameCenterController::GetUserAvartarURL(INT_TIME id)
{
    return NULL;
}
#endif

//hao.ly add

void PineGameCenterController::ShowLeaderAchivement()
{
    OS_ShowLeaderAchivement();
}

void PineGameCenterController::SubmitAchivement(int percent, const char* leaderboard, bool noti)
{
    OS_GameCenterSubmitAchievement(percent, leaderboard, noti);
}

//Lam add
void PineGameCenterController::ResetAllAchivement()
{
    OS_ResetAllAchievement();
}

void PineGameCenterController::LoadAchievements()
{
    //OS_GameCenterLoadAchievement();
}