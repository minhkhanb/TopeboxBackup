//
//  PineGameCenterController.h
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 4/16/14.
//
//
#include "PineSocial.h"
#ifndef __PineEntertainment__PineGameCenterController__
#define __PineEntertainment__PineGameCenterController__
#define LEADERBOARD_MAX_NUM (100)
class PineGameCenterController: public PineSocial
{
    
public:
    static int _num_of_leaderboard;
    static int _current_leaderboard_loaded;
    static char _leader_boardid[LEADERBOARD_MAX_NUM][128];
    static INT_TIME _my_score[LEADERBOARD_MAX_NUM];
    static bool _used[LEADERBOARD_MAX_NUM];
    void InviteFriend();
    static INT_TIME GetMyScore(const char* leaderboardid);
    static void SubmitMyScore(const char* leaderboardid, INT_TIME value);
    static void ShowLeaderBoard(const char* leaderboarid);
    static void UpdateAchievement(const char* leaderboarid,float percent,bool show);
    ~PineGameCenterController(){};
    PineGameCenterController();
    bool Init();
    bool InitNoneWait();
    void LoadAvatarURL();
    bool PostToWall(const char* feed);
    bool PostScreenToWall(const char* feed);
    bool PostImageToWall(const char* feed, const char* realUrlImage);
    void LoadFriends();
    void LoadFollowers();
    void LoadFollowings();
    void LoadMyScore();
#if USE_PINESOCIAL_AT_ID_STRING
    const char* GetUserAvartarURL(const char* id);
#else
    const char* GetUserAvartarURL(INT_TIME id);
#endif
    
    //hao.ly add
    static void ShowLeaderAchivement();
    static void SubmitAchivement(int percent, const char* leaderboard, bool noti);
    //Lam add
    static void ResetAllAchivement();
    static void LoadAchievements();
    static int _iGameCenterAuthenStatus;
};

#endif /* defined(__PineEntertainment__PineGameCenterController__) */
