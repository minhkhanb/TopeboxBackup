#if !defined(_PINEGAMECENTER_H)
#define _PINEGAMECENTER_H
PineSocial *currentSocial;
extern BOOL dispatch_queue_is_empty(dispatch_queue_t queue);
-(void) GC_Authentication
{
    dispatch_queue_t dispatchQueue = dispatch_queue_create("GC_Authentication.Pine.Entertainment.com", 0);   //ie. your made-up queue name
    dispatch_queue_is_empty(dispatchQueue);
    NSLog(@"\n Authencation");
    [_pine_view_controller GameCenterAuthentication:currentSocial];
    NSLog(@"\n Authencation 111");
}
extern "C" void OS_GameCenterAuthentication(PineSocial* social, bool wait)
{
    currentSocial = social;
    [[PineGameDelegate sharedInstance] GC_Authentication];
    //[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(GC_Authentication) withObject:nil waitUntilDone:wait];

}
extern "C" int GetGameCenterAuthenticationStatus()
{
    return PineGameCenterController::_iGameCenterAuthenStatus;
    
}
extern "C" bool isGameCenterAuthentication()
{
     return [PineGameDelegate sharedInstance]->_pine_view_controller->_hasGamecenterAuthentication;
}

extern "C" void  OS_GameCenterLoadLeaderboardInfo()
{
    [[PineGameDelegate sharedInstance]->_pine_view_controller GameCenterLoadLeaderboardInfo];
}

extern "C" void OS_ShowLeaderboard(const char*  leaderboard)
{
    if(leaderboard == NULL)
    {
        [[PineGameDelegate sharedInstance]->_pine_view_controller GameCenterShowLeaderboard:nil];
    }
    else
    {
        NSString* board = [[NSString alloc] initWithFormat:@"%s", leaderboard];
        [[PineGameDelegate sharedInstance]->_pine_view_controller GameCenterShowLeaderboard:board];
    }
}
extern "C" void OS_GameCenterSubmitScore(INT_TIME value, const char* leaderboard)
{
    dispatch_queue_t dispatchQueue = dispatch_queue_create("OS_GameCenterSubmitScore.Pine.Entertainment.com", 0);   //ie. your made-up queue name
    dispatch_queue_is_empty(dispatchQueue);
    NSString* board = [[NSString alloc] initWithFormat:@"%s", leaderboard];
    [[PineGameDelegate sharedInstance]->_pine_view_controller reportScore:value forCategory:board];
}
extern "C" void OS_GameCenterSubmitAchievement(int percent, const char* leaderboard, bool noti)
{
    dispatch_queue_t dispatchQueue = dispatch_queue_create("OS_GameCenterSubmitAchievement.Pine.Entertainment.com", 0);   //ie. your made-up queue name
    dispatch_queue_is_empty(dispatchQueue);

    NSString* board = [[NSString alloc] initWithFormat:@"%s", leaderboard];
    [[PineGameDelegate sharedInstance]->_pine_view_controller updateAchievements:board progress:percent show:noti];
}
extern "C" void OS_ResetAllAchievement()
{
   
    [[PineGameDelegate sharedInstance]->_pine_view_controller ResetAllAchievements];
}
extern "C" void OS_GameCenterLoadScore(PineSocial* social)
{
    dispatch_queue_t dispatchQueue = dispatch_queue_create("OS_GameCenterLoadScore.Pine.Entertainment.com", 0);   //ie. your made-up queue name
    dispatch_queue_is_empty(dispatchQueue);
    [[PineGameDelegate sharedInstance]->_pine_view_controller GameCenterLoadScore:social];
    
}

extern "C" void OS_GameCenterLoadFriend(PineSocial* social)
{
    dispatch_queue_t dispatchQueue = dispatch_queue_create("OS_GameCenterLoadFriend.Pine.Entertainment.com", 0);   //ie. your made-up queue name
    dispatch_queue_is_empty(dispatchQueue);
    [[PineGameDelegate sharedInstance]->_pine_view_controller GameCenterRetrieveFriends:social];
}


//hao.ly add
extern "C" void OS_ShowLeaderAchivement()
{
    [[PineGameDelegate sharedInstance]->_pine_view_controller GameCenterShowAchievement:nil];
}

extern "C" void OS_GameCenterLoadAchievement()
{
    [[PineGameDelegate sharedInstance]->_pine_view_controller GameCenterLoadAchievements];
}
#endif
