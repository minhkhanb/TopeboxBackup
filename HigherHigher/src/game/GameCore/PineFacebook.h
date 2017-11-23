extern BOOL dispatch_queue_is_empty(dispatch_queue_t queue);
extern "C" const char* OS_convertSStringTocharArray(NSString* str);
#import <FBSDKCoreKit/FBSDKMacros.h>
#import <objc/message.h>
#ifndef FACEBOOK_IMPLEMENT_H
#define FACEBOOK_IMPLEMENT_H
//#import "FBSDKLikeActionController.h"
#define REQUIRES_FACEBOOOK_LOGIN "requires Facebook account to back up your data online. Check your device setting for logging in and re-launch the app to secure your progress."
FBSDK_EXTERN NSString *const FBSDKLikeActionControllerDidUpdateNotification;
-(void) PineFacebookTokenRegister
{
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(AccessTokenFBChanged:)
                                                 name:FBSDKAccessTokenDidChangeNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(CurrentProfileFBChanged:)
                                                 name:FBSDKProfileDidChangeNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(facebookLikeNotification:) name:FBSDKLikeActionControllerDidUpdateNotification object:nil];
    
}
- (void) AccessTokenFBChanged:(NSNotification *)notification
{
    if ([notification.name isEqualToString:FBSDKAccessTokenDidChangeNotification]) {
        
        FBSDKAccessToken* oldToken = [notification.userInfo valueForKey: FBSDKAccessTokenChangeOldKey];
        FBSDKAccessToken* newToken = [notification.userInfo valueForKey: FBSDKAccessTokenChangeNewKey];
  /*
        if ([[FBSDKAccessToken currentAccessToken] hasGranted:@"publish_actions"])
        {
            NSInteger slot = 1;
            SUCacheItem *item = [SUCache itemForSlot:1] ?: [[SUCacheItem alloc] init];
            if (![item.token isEqualToAccessToken:[FBSDKAccessToken currentAccessToken]]) {
                //[FBSDKAccessToken setCurrentAccessToken:nil];
                //[FBSDKProfile setCurrentProfile:nil];
                //[SUCache deleteItemInSlot:slot];
                item.token = [FBSDKAccessToken currentAccessToken];
                [SUCache saveItem:item slot:1];
            }
            return;
        }
   */
        NSLog(@"FB access token did change notification\nOLD token:\t%@\nNEW token:\t%@", oldToken.tokenString, newToken.tokenString);
        
        // initial token setup when user is logged in
        
        if (newToken != nil && oldToken == nil) {
            
            NSDate *nowDate = [NSDate date];
            NSDate *fbExpirationDate = [FBSDKAccessToken currentAccessToken].expirationDate;
            if ([fbExpirationDate compare:nowDate] != NSOrderedDescending) {
                NSLog(@"FB token: expired");
                [self DeleteCacheFB];
                //[self ResetFBToken];
                
            }
            else {
                
            }
        }
        else if (newToken != nil && oldToken != nil
                 && ![oldToken.tokenString isEqualToString:newToken.tokenString])
        {
            [self DeleteCacheFB];
            //[self ResetFBToken];
            NSLog(@"FB access token string did change");

        }
        
        // moving from "logged in" state to "logged out" state
        // e.g. user canceled FB re-login flow
        else if (newToken == nil && oldToken != nil) {
            [self DeleteCacheFB];
            //[self ResetFBToken];;
            NSLog(@"FB access token string did become nil");
        }
        
    }
    FBSDKAccessToken *token = notification.userInfo[FBSDKAccessTokenChangeNewKey];
    if (!token) {
        [self DeleteCacheFB];
        //[self ResetFBToken];;
        
    } else {
        
        NSInteger slot = 0;
        SUCacheItem *item = [SUCache itemForSlot:0] ?: [[SUCacheItem alloc] init];
        if (![item.token isEqualToAccessToken:token]) {
            //[FBSDKAccessToken setCurrentAccessToken:nil];
            //[FBSDKProfile setCurrentProfile:nil];
            //[SUCache deleteItemInSlot:slot];
            item.token = token;
            NSLog(@"FB access token did change notification\nOLD token:\t%@\nNEW token:\t%@",item.token.tokenString, item.token.tokenString);

            [SUCache saveItem:item slot:0];
        }
        
    }
}

// The profile information has changed, update the cell and cache.
- (void) CurrentProfileFBChanged:(NSNotification *)notification
{
    /*
    NSInteger slot = 0;
    
    FBSDKProfile *profile = notification.userInfo[FBSDKProfileChangeNewKey];
    if (profile) {
        SUCacheItem *cacheItem = [SUCache itemForSlot:slot];
        cacheItem.profile = profile;
        [SUCache saveItem:cacheItem slot:slot];
        
    }
     */
}

- (void) ResetFBToken
{
    [FBSDKAccessToken setCurrentAccessToken:nil];
    [FBSDKProfile setCurrentProfile:nil];
}

-(void) DeleteCacheFB
{
    NSInteger slot = 0;
    [SUCache deleteItemInSlot:slot];
    [SUCache clearCache];
}

/*!
 @abstract Sent to the delegate when the app invite completes without error.
 @param appInviteDialog The FBSDKAppInviteDialog that completed.
 @param results The results from the dialog.  This may be nil or empty.
 */
- (void)appInviteDialog:(FBSDKAppInviteDialog *)appInviteDialog didCompleteWithResults:(NSDictionary *)results
{
    NSString *ids = [results objectForKey:@"completionGesture"];
    if(ids != nil && [ids isEqual:@"cancel"])
    {
        
        CGame::_this->_stateInviteFriend = 0;
        
    }
    else
    {
        CGame::_this->_stateInviteFriend = 2;
    }
    NSLog(@"fetched results:%@", results);
}

/*!
 @abstract Sent to the delegate when the app invite encounters an error.
 @param appInviteDialog The FBSDKAppInviteDialog that completed.
 @param error The error.
 */
- (void)appInviteDialog:(FBSDKAppInviteDialog *)appInviteDialog didFailWithError:(NSError *)error
{
    NSLog(@"fetched error:%@", error);
    CGame::_this->_stateInviteFriend = 0;
}

-(NSString*) getFacebookToken
{
    NSInteger slot = 0;
    FBSDKAccessToken *token = [SUCache itemForSlot:slot].token;
    return [token tokenString];
}

extern "C" std::string OS_GetFacebookToken()
{
    NSString* token =[[PineGameDelegate sharedInstance] getFacebookToken];
    if(token != nil)
    {
        return std::string([token UTF8String]);
    }
    return "";
}

- (BOOL)userHasAccessToFacebook
{
    NSInteger slot = 0;
    FBSDKAccessToken *token = [SUCache itemForSlot:slot].token;
    if (token) {
        return true;
    }
    return false;

}

-(void) LoadFriendFacebook:(PineSocial*)social
{
    NSInteger slot = 0;
    FBSDKAccessToken *token = [SUCache itemForSlot:slot].token;
    if (token) {
        NSDictionary *parameters = @{
                                     @"fields": @"name",
                                     @"limit" : @"200"
                                     };
        // This will only return the list of friends who have this app installed
        FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc] initWithGraphPath:@"/me/friends" parameters:parameters];
        [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
            // Since we're only requesting /me, we make a simplifying assumption that any error
            // means the token is bad.'
            if (!error) {
                //NSLog(@"fetched user:%@", result);
                NSArray *list_id = result[@"data"];
                social->ResetFriendList();
                
                for(int i = 0; i < [list_id count]; i++)
                {
                    
                    NSString *ids = [[list_id objectAtIndex:i] objectForKey:@"id"];
                     NSString *name = [[list_id objectAtIndex:i] objectForKey:@"name"];
                    if(ids != nil && name != nil)
                    {
                        printf("\nAdd friend %d id = %s",i,[ids UTF8String]);
#if USE_PINESOCIAL_AT_ID_STRING
                        social->AddFriend([ids UTF8String],[name UTF8String]);
#else
                        social->AddFriend([ids longLongValue],[name UTF8String]);
#endif
                    }
                    
                }
                social->SetFlag(SOCIAL_FLAG_FULL_ACCESS);
                social->SetState(PINESOCIAL_STATE_SUCCESS);
            }
            else
            {
                
                social->SetFlag(SOCIAL_FLAG_ACCESS_DENINE);
                social->SetState(PINESOCIAL_STATE_COMPLETED);
                NSLog(@"%@", error);
            }
        }];
    }
    else
    {
        social->SetFlag(SOCIAL_FLAG_ACCESS_DENINE);
        social->SetState(PINESOCIAL_STATE_COMPLETED);
    }
    
    
}

-(void) FacebookInviteSuggets: (NSString*) feed withGame:(NSString*) gameTitle
{
    NSInteger slot = 0;
    FBSDKAccessToken *token = [SUCache itemForSlot:slot].token;
    if (token) {
        // We have a saved token, issue a request to make sure it's still valid.
        [FBSDKAccessToken setCurrentAccessToken:token];
        FBSDKAppInviteContent *content = [[FBSDKAppInviteContent alloc] init];
        
        content.appLinkURL = [NSURL URLWithString:@FACEBOOK_APP_LINK];
        [FBSDKAppInviteDialog showFromViewController:nil withContent:content
                                     delegate:self];
    }
    else
    {
        CGame::_this->_stateInviteFriend = 0;
    }
    
}

-(void) Logout:(PineSocial*)social
{
    NSInteger slot = 0;
    FBSDKAccessToken *token = [SUCache itemForSlot:slot].token;
    if (token)
    {
        social->SetState(PINESOCIAL_STATE_COMPLETED);
        social->SetFlag(SOCIAL_FLAG_ID_LOGOUT);
        social->_logined = false;
        [self DeleteCacheFB];
        [self ResetFBToken];
        FBSDKLoginManager *loginManager = [[FBSDKLoginManager alloc] init];
        [loginManager logOut];
    }

}
#ifndef FACEBOOK_REQUEST_GET_ME
#define FACEBOOK_REQUEST_GET_ME @"me?fields=id,name,picture"
#endif
extern "C" bool grapme_complete_thead = false;
-(void) GetMe:(PineSocial*)social
{
    NSInteger slot = 0;
    FBSDKAccessToken *token = [SUCache itemForSlot:slot].token;
    
    //check swap account
    
    NSLog(@"FB access token did change notification\nOLD token:\t%@\nNEW token:\t%@", token, token);
    if (token) {
        dispatch_async(dispatch_get_main_queue(), ^(){
        // We have a saved token, issue a request to make sure it's still valid.
        [FBSDKAccessToken setCurrentAccessToken:token];
        //FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc] initWithGraphPath:@"me?fields=id,name,picture" parameters:nil];
        FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc] initWithGraphPath:FACEBOOK_REQUEST_GET_ME parameters:nil];
        [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
            // Since we're only requesting /me, we make a simplifying assumption that any error
            // means the token is bad.'
            NSLog(@"informations: %@",result);
            if (error) {
                social->SetState(PINESOCIAL_STATE_COMPLETED);
                social->SetFlag(SOCIAL_FLAG_NO_ACCOUNT_LOGIN);
                social->_logined = false;
                [self DeleteCacheFB];
                //grapme_complete_thead = true;
                //[self ResetFBToken];
            }
            else
            {
                social->SetState(PINESOCIAL_STATE_COMPLETED);
                social->SetFlag(SOCIAL_FLAG_FULL_ACCESS);
                NSString* scope_id = [NSString stringWithFormat:@"%@",[result objectForKey:@"id"]];
                NSString* name = [NSString stringWithFormat:@"%@",[result objectForKey:@"name"]];
                if(scope_id != nil)
                {

#if USE_PINESOCIAL_AT_ID_STRING
                    social->SetInfomation([scope_id UTF8String], [name UTF8String]);
                    social->SetUserIDScope([scope_id UTF8String]);
#else
                    social->SetInfomation([scope_id longLongValue], [name UTF8String]);
                    social->SetUserIDScope([scope_id longLongValue]);
#endif
                    //get fb name unicode
                    social->_currentUnicodeUserNameLength = [name length];
                    if (social->_currentUnicodeUserNameLength > 32)
                    {
                        social->_currentUnicodeUserNameLength = 32;
                    }
                    for (int i = 0; i < social->_currentUnicodeUserNameLength; i++)
                    {
                        social->_unicodeUserName[i] = [name characterAtIndex:i];
                    }
                    //if(ret != nil)
                    {
                        //NSData *data = [result dataUsingEncoding:NSUTF8StringEncoding];
                        //id json = [NSJSONSerialization JSONObjectWithData:result options:0 error:nil];
                        profileFBImageURL =[NSString stringWithFormat:@"%@",[[[result objectForKey:@"picture"] objectForKey:@"data"] objectForKey:@"url"]];
                        
                    }
                    social->SetURLAvartar((const char*)[profileFBImageURL UTF8String]);
                    
                }
                PineFacebookController::_share_instance = (PineFacebookController*)social;
                social->_logined = true;
                
                
                


            }
            grapme_complete_thead = true;
            
        }];
         });
        
#if !DISABLE_FACEBOOK_PUBLISHING_SCORE
        while(!grapme_complete_thead)
        {
            CGame::_this->Sleep(30);
        }
        printf("\nExit thread");
        [self FacebookRequestPulishRequest:social];
        
#endif

    }
    
}
extern "C" void OS_GraphUserInformation(PineSocial *social);
int gobal_facebook_access = 0;
-(bool) FacebookRequestPulishRequest:(PineSocial*)social
{
    dispatch_queue_t dispatchQueue = dispatch_queue_create("FacebookInit.Pine.Entertainment.com", 0);   //ie. your made-up queue name
    dispatch_queue_is_empty(dispatchQueue);
    gobal_facebook_access = 1;
    NSInteger slot = 1;
    /*
    FBSDKAccessToken *token = [SUCache itemForSlot:slot].token;
    if (token) {
        // We have a saved token, issue a request to make sure it's still valid.
        [FBSDKAccessToken setCurrentAccessToken:token];
    }*/
    
    if ([[FBSDKAccessToken currentAccessToken] hasGranted:@"publish_actions"])
    {
        gobal_facebook_access = true;
        return true;
    }
    else
    {
        FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
        
        login.loginBehavior = FBSDKLoginBehaviorSystemAccount;//FBSDKLoginBehaviorBrowser;//FBSDKLoginBehaviorWeb
        login.defaultAudience = FBSDKDefaultAudienceFriends;
        //[login logInWithReadPermissions:@FACEBOOK_PERMISSION handler:^(FBSDKLoginManagerLoginResult *result, NSError *error)
        
      
        dispatch_async(dispatch_get_main_queue(), ^(){
            [login logInWithPublishPermissions:@[@"publish_actions"]
                         fromViewController:_opgenGLESViewController
                                    handler:^(FBSDKLoginManagerLoginResult *result, NSError *error)
             {
                 if (error || result.isCancelled)
                 {
                     gobal_facebook_access = 2;
                 }
                 else
                 {
                     gobal_facebook_access = 0;
                     
                 }
             }];
        });
        while(gobal_facebook_access == 1)
        {
            CGame::_this->Sleep(30);
        }
    }
    return (gobal_facebook_access == 0);

}
-(void) FacebookInit:(PineSocial*)social
{
    
    dispatch_queue_t dispatchQueue = dispatch_queue_create("FacebookInit.Pine.Entertainment.com", 0);   //ie. your made-up queue name
    dispatch_queue_is_empty(dispatchQueue);
    NSInteger slot = 0;
    FBSDKAccessToken *token = [SUCache itemForSlot:slot].token;
    if (token) {
        OS_GraphUserInformation(social);


    }
    else
    {
        //[[FBSDKLoginManager alloc] logOut];
        FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
        
        login.loginBehavior = FBSDKLoginBehaviorSystemAccount;//FBSDKLoginBehaviorBrowser;//FBSDKLoginBehaviorWeb
        
        //[login logInWithReadPermissions:@FACEBOOK_PERMISSION handler:^(FBSDKLoginManagerLoginResult *result, NSError *error)
        dispatch_async(dispatch_get_main_queue(), ^(){
            [login logInWithReadPermissions:@FACEBOOK_PERMISSION
                         fromViewController:_opgenGLESViewController
                                    handler:^(FBSDKLoginManagerLoginResult *result, NSError *error)
            {
                if (error || result.isCancelled) {
                    social->SetState(PINESOCIAL_STATE_COMPLETED);
                    social->SetFlag(SOCIAL_FLAG_NO_ACCOUNT_LOGIN);
                    [self DeleteCacheFB];
                    //[self ResetFBToken];
                    social->_logined = false;
                }
                else
                {
                    PineFacebookController::_share_instance = (PineFacebookController*)social;
                    social->_logined = true;
                    social->SetFlag(SOCIAL_FLAG_WAIT_GET_ME);
                }
            }];
        });
        while(social->GetFlag() != SOCIAL_FLAG_WAIT_GET_ME && social->GetFlag() != SOCIAL_FLAG_NO_ACCOUNT_LOGIN)
        {
            
        }
        if(social->GetFlag() == SOCIAL_FLAG_WAIT_GET_ME)
        {
            OS_GraphUserInformation(social);
            
        }
    }
       
  

}

-(void)PostImageToFacebook:(NSString*) feed withRealUrl:(NSString*) url
{
}

extern "C" void OS_BasicPostSocial(const char* feed);
- (void)sharer:(id<FBSDKSharing>)sharer didCompleteWithResults :(NSDictionary*)results {
    NSLog(@"FB: SHARE RESULTS=%@\n",[results debugDescription]);
    PineAdBox::showAdState = SHOW_AD_COMPLETED;
    
}

- (void)sharer:(id<FBSDKSharing>)sharer didFailWithError:(NSError *)error {
    NSLog(@"FB: ERROR=%@\n",[error debugDescription]);
      PineAdBox::showAdState = SHOW_AD_CANCEL;
}

- (void)sharerDidCancel:(id<FBSDKSharing>)sharer {
    NSLog(@"FB: CANCELED SHARER=%@\n",[sharer debugDescription]);
     PineAdBox::showAdState = SHOW_AD_CANCEL;
}
-(void) AddLikePage:(CGRect) rect
{
    PineSocial* social = PineFacebookController::_share_instance;
    if(social != NULL && social->HasLogined() && [self FacebookRequestPulishRequest:social])
    {
        dispatch_async(dispatch_get_main_queue(), ^(){
            FBSDKLikeControl *button = [[FBSDKLikeControl alloc] init];
            //[button sizeToFit];
            button.foregroundColor = [UIColor clearColor];
            float width = button.frame.size.width;
            float height = button.frame.size.height;
            float sx = rect.size.width/width;
            float sy = rect.size.height/height;
             button.center = CGPointMake(0.0f,0.0f);
            //printf("\nsx = %f, sy = %f, rect->size.width = %f, rect->size.height = %f",sx,sy,rect.size.width,rect.size.height);
            [button setTransform:CGAffineTransformMakeScale(sx, sy)];
           
            [button setTransform:CGAffineTransformTranslate(button.transform ,0+width/2 + rect.origin.x, 0+height/2+ rect.origin.y)];
            
            //[button sizeThatFits:CGSizeMake(200,200)];
            
            //CGAffineTransformMakeScale
            button.objectID = @PINE_FANPAGE_FACEBOOK;
            PineAdBox::showAdState = SHOW_AD_PROCESSING;
            [_opgenGLESViewController.view addSubview:button];
        });
    }
    
}

- (void)facebookLikeNotification:(NSNotification*)notification
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundeclared-selector"
    SEL selector = @selector(objectIsLiked);
    if (notification.object && [notification.object respondsToSelector:selector]) {
        
        BOOL (*BOOLMsgSend)(id, SEL) = (typeof(BOOLMsgSend))objc_msgSend;
        BOOL isLiked = BOOLMsgSend(notification.object, selector);
        if(isLiked)
        {
            PineAdBox::showAdState = SHOW_AD_COMPLETED;
        }
        else
        {
            PineAdBox::showAdState = SHOW_AD_CANCEL;
        }
        //NSLog(@"is liked: %d",isLiked);
        
    }
#pragma clang diagnostic pop
}
-(void) PostScreenToFacebook:(NSString*) feed
{
    //CGRect rect = (CGRectMake(100.0f, 100.0f, 100.0f, 50.0f));
    //[[PineGameDelegate sharedInstance] AddLikePage:rect];
    //return;
    PineSocial* social = PineFacebookController::_share_instance;
    if(social != NULL && social->HasLogined() && [self FacebookRequestPulishRequest:social])
    {
        
        
        dispatch_async(dispatch_get_main_queue(), ^(){
        UIGraphicsBeginImageContextWithOptions(glView.bounds.size, NO, 0.0f);
        //UIGraphicsBeginImageContextWithOptions(CGSizeMake(k_SCREEN_WIDTH, k_SCREEN_HEIGHT), NO, 1.0f);
        UIImage *image = nil;
        
        @try
        {
            [glView drawViewHierarchyInRect:glView.bounds afterScreenUpdates:YES];
            
            if (GAME()->GetGraphicContext().ComplexMode == PineGraphicComplexOrientation::ComplexNone)
            {
                image = UIGraphicsGetImageFromCurrentImageContext();
                UIGraphicsEndImageContext();
            }
            else if (GAME()->GetGraphicContext().ComplexMode == PineGraphicComplexOrientation::ComplexToLeft
                     || GAME()->GetGraphicContext().ComplexMode == PineGraphicComplexOrientation::ComplexToRight
                     )
            {
                float landcapeWidth = glView.bounds.size.height;
                float landcapeHeight = glView.bounds.size.width;
                
                UIImage *imageCopy = UIGraphicsGetImageFromCurrentImageContext();
                UIGraphicsEndImageContext();
                UIImageOrientation imageOrientation = UIImageOrientationLeft;
                if (GAME()->GetGraphicContext().ComplexMode == PineGraphicComplexOrientation::ComplexToLeft)
                {
                    imageOrientation = UIImageOrientationRight;
                }
                
                //printf("\n w = %f, h = %f ", landcapeWidth, landcapeHeight);
                UIImage * landscapeImage = [[UIImage alloc] initWithCGImage: imageCopy.CGImage scale: 1.0 orientation:imageOrientation];
                UIGraphicsBeginImageContext(CGSizeMake(landcapeWidth, landcapeHeight));
                [landscapeImage drawInRect:CGRectMake(0, 0, landcapeWidth, landcapeHeight)];
                image = [UIGraphicsGetImageFromCurrentImageContext() retain];
                UIGraphicsEndImageContext();
                
            }
            
            
        }
        @catch(NSException* eeee)
        {
            [glView.layer renderInContext:UIGraphicsGetCurrentContext()];
            image = [UIImage imageNamed:@"ios6newhighscore.png"];
        }
        //sleep(1);
        
        
        FBSDKSharePhoto *photo = [[FBSDKSharePhoto alloc] init];
        photo.image = image;
        photo.userGenerated = YES;
        FBSDKSharePhotoContent *content = [[FBSDKSharePhotoContent alloc] init];
        content.photos = @[photo];
       
        //NSString* sfeed = [NSString stringWithFormat:@"%@ %s https://itunes.apple.com/us/app/id%d ",feed,TAG_LINE,APPLE_ID];
        //content.ref = sfeed;
        //content.contentTitle = sfeed;
        //content.contentURL = [NSURL URLWithString:sfeed];
        PineAdBox::showAdState = SHOW_AD_PROCESSING;
        
        [FBSDKShareDialog showFromViewController:nil
                                     withContent:content
                                        delegate:nil];
        });
    }
    else
    {
        PineAdBox::showAdState = SHOW_AD_PROCESSING;
        NSString* sfeed = [NSString stringWithFormat:@"%@ %s https://itunes.apple.com/us/app/id%d ",feed,TAG_LINE,APPLE_ID];
        [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(iOS_BasicPostSocial:) withObject:sfeed waitUntilDone:YES];
    }
    
}
#define GET_URL_AVATAR_TIME_OUT (10)

- (NSString *)stringWithUrl:(NSURL *)url
{
    NSURLRequest *urlRequest = [NSURLRequest requestWithURL:url
                                                cachePolicy:NSURLRequestReturnCacheDataElseLoad
                                            timeoutInterval:30];
    // Fetch the JSON response
    NSData *urlData;
    NSURLResponse *response;
    NSError *error;
    
    // Make synchronous request
    urlData = [NSURLConnection sendSynchronousRequest:urlRequest
                                    returningResponse:&response
                                                error:&error];
    
    // Construct a String around the Data from the response
    return [[NSString alloc] initWithData:urlData encoding:NSUTF8StringEncoding];
}
NSString* profileFBImageURL;

-(NSString*) GetURLFacebookAvartar:(NSString*) user_id
{
    FBSDKAccessToken *token = [SUCache itemForSlot:0].token;
    
    profileFBImageURL = nil;
    NSString* query ;
    
    if(token)
    {
        query = [NSString stringWithFormat:@"https://graph.facebook.com/%@/?fields=picture&access_token=%@",user_id,[token tokenString]];
    }
    else
    {
        query = [NSString stringWithFormat:@"https://graph.facebook.com/%@/?fields=picture",user_id];
    }
    
    NSURL *fbURL = [NSURL URLWithString:query];
    NSString* ret =  [self stringWithUrl:fbURL];
    //profileFBImageURL = [self stringWithUrl:fbURL];
    
    if(ret != nil)
    {
        NSData *data = [ret dataUsingEncoding:NSUTF8StringEncoding];
        id json = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
        profileFBImageURL =[NSString stringWithFormat:@"%@",[[[json objectForKey:@"picture"] objectForKey:@"data"] objectForKey:@"url"]];
        
    }
    return  profileFBImageURL; //profileFBImageURL;
}

-(void)getFacebookFriendsForAccount:(ACAccount*)account withSocial:(PineSocial *)social
{
    
    NSDictionary *param=[NSDictionary dictionaryWithObjectsAndKeys:@"id,installed",@"fields", nil];
    
    NSURL *feedURL = [NSURL URLWithString:@"https://graph.facebook.com/me/friends"];
    SLRequest *feedRequest = [SLRequest
                              requestForServiceType:SLServiceTypeFacebook
                              requestMethod:SLRequestMethodGET
                              URL:feedURL
                              parameters:param];
    feedRequest.account = account;
    [feedRequest performRequestWithHandler:^(NSData *responseData,
                                             NSHTTPURLResponse *urlResponse, NSError *error)
     {
         if(!error)
         {
             NSDictionary *json = [NSJSONSerialization JSONObjectWithData:responseData options:NSJSONWritingPrettyPrinted error:&error];
            NSArray* list_id= [json objectForKey:@"data"];
             //NSArray* list_id= [json objectForKey:@"id"];
            //NSLog(@"Dictionary contains data: %@", list_id);
             // NSLog(@"url contains data: %@", urlResponse);
             social->ResetFriendList();
             if(!([json objectForKey:@"error"]!=nil))
             {
                 for(int i = 0; i < [list_id count]; i++)
                 {
                     
                     NSString *ids = [[list_id objectAtIndex:i] objectForKey:@"id"];
                     NSString *installed = [[list_id objectAtIndex:i] objectForKey:@"installed"];
                     NSString *name = [[list_id objectAtIndex:i] objectForKey:@"name"];
                     if(ids != nil && installed != nil && name != nil)
                     {
#if USE_PINESOCIAL_AT_ID_STRING
                         social->AddFriend([ids UTF8String],[name UTF8String]);
#else
                         social->AddFriend([ids longLongValue],[name UTF8String]);
#endif
                     }
                 }
               
             }
             social->SetFlag(SOCIAL_FLAG_FULL_ACCESS);
             social->SetState(PINESOCIAL_STATE_SUCCESS);
             dispatch_async(dispatch_get_main_queue(),^{
                 //nameLabel.text = [json objectForKey:@"username"];
             });
         }
         else{
             social->SetFlag(SOCIAL_FLAG_FULL_ACCESS);
             social->SetState(PINESOCIAL_STATE_SUCCESS);
             //handle error gracefully
             NSLog(@"error from get%@",error);
             //attempt to revalidate credentials
         }
     }];
   
}

extern "C" void OS_LoadFriendFacebook(PineSocial *social)
{
    dispatch_async(dispatch_get_main_queue(),
                   ^{
                       [[PineGameDelegate sharedInstance] LoadFriendFacebook:social];
                   });
    
}

#if USE_PINESOCIAL_AT_ID_STRING
extern "C" const char* OS_GetFacebookAvartar(const char* id)
#else
extern "C" const char* OS_GetFacebookAvartar(INT_TIME id)
#endif
{
#if USE_PINESOCIAL_AT_ID_STRING
    NSString* url = [[PineGameDelegate sharedInstance] GetURLFacebookAvartar:[NSString stringWithFormat:@"%s",id]];
#else
    NSString* url = [[PineGameDelegate sharedInstance] GetURLFacebookAvartar:[NSString stringWithFormat:@"%lld",id]];
#endif
    if(url == nil)
    {
        return NULL;
    }
    return [url UTF8String];
    
}
extern "C" void FacebookInviteSuggetsFriend(const char* feed,const char* title)
{
    //extern char* addString(const char* addThis, const char* toThis)
    dispatch_queue_t dispatchQueue = dispatch_queue_create("FacebookInviteSuggetsFriend.Pine.Entertainment.com", 0);   //ie. your made-up queue name
    dispatch_queue_is_empty(dispatchQueue);
    dispatch_async(dispatch_get_main_queue(),
    ^{
        NSString* strfeed = [NSString stringWithFormat:@"%s",feed];
        NSString* strTitle = [NSString stringWithFormat:@"%s",title];
    
    
    [[PineGameDelegate sharedInstance] FacebookInviteSuggets:strfeed withGame:strTitle];
    });
    //[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(FacebookInvite) withObject:nil waitUntilDone:YES];
}

extern "C" void OS_GraphUserInformation(PineSocial *social)
{
   
        [[PineGameDelegate sharedInstance] GetMe:social];
    
}

extern "C" void OS_InitFacebook(PineSocial *social)
{
    //[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(FacebookInit:) withObject:(id)social waitUntilDone:YES];
    [[PineGameDelegate sharedInstance] FacebookInit:social];
}
extern "C" void OS_FacebookLogout(PineSocial *social)
{
    [[PineGameDelegate sharedInstance] Logout:social];
    //-(void) Logout:(PineSocial*)social
}
extern "C" void OS_PostScreenToFacebook(const char* feed)
{
    [[PineGameDelegate sharedInstance] PostScreenToFacebook:[NSString stringWithFormat:@"%s",feed]];
    
}

extern "C" void OS_PostImageToFacebook(const char* feed, const char* realUrl)
{
    [[PineGameDelegate sharedInstance] PostImageToFacebook:[NSString stringWithFormat:@"%s",feed] withRealUrl:[NSString stringWithFormat:@"%s",realUrl]];
}

extern "C" void OS_FacebookInviteFriend(const char* feed,const char* title,INT_TIME* friendlist,int numfriend)
{
    //extern char* addString(const char* addThis, const char* toThis)
    NSString *listSend = nil;
    if(numfriend > 0)
    {
        listSend = [NSString stringWithUTF8String:""];
    }
    NSString* strfeed = [NSString stringWithFormat:@"%s",feed];
    NSString* strTitle = [NSString stringWithFormat:@"%s",title];
    
    for(int i = 0; i < numfriend; i++)
    {
        listSend = [listSend stringByAppendingString:[NSString stringWithFormat:@"%lld,", friendlist[i]]];
    }
    [[PineGameDelegate sharedInstance] FacebookInvite:strfeed withGame:strTitle withFriends:listSend];
    //[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(FacebookInvite) withObject:nil waitUntilDone:YES];
    
}

extern "C" bool OS_IsLogginFacebook()
{
    return [[PineGameDelegate sharedInstance] userHasAccessToFacebook];
}

extern "C" void OS_FacebookAddPageLikeButton()
{
    CGRect rect = (CGRectMake(100.0f, 100.0f, 100.0f, 50.0f));
    [[PineGameDelegate sharedInstance] AddLikePage:rect];
}

//Send friend Request
-(void) SendRequest:(NSString*)friendID withTile:(NSString*)tile withMessage:(NSString*)message
{
    NSArray *arrayID = [[NSArray alloc] initWithObjects: friendID, nil];
    FBSDKGameRequestContent *gameRequestContent = [[FBSDKGameRequestContent alloc] init];
    // Look at FBSDKGameRequestContent for futher optional properties
    
    [gameRequestContent setTitle:tile];
    [gameRequestContent setMessage:message];
    [gameRequestContent setRecipients:arrayID];
    
    // Assuming self implements <FBSDKGameRequestDialogDelegate>
    [FBSDKGameRequestDialog showWithContent:gameRequestContent delegate:self];
}
#ifndef COMPLETE_SEND_REQUET
#define COMPLETE_SEND_REQUET() 
#endif
- (void)gameRequestDialog:	(FBSDKGameRequestDialog *)gameRequestDialog didCompleteWithResults:	(NSDictionary *)results
{
    if (results)
    {
        COMPLETE_SEND_REQUET();
        NSLog(@"Send Request is Complete");
    }
}

- (void)gameRequestDialog:	(FBSDKGameRequestDialog *)gameRequestDialog didFailWithError:	(NSError *)error
{
    if (error)
    {
        NSLog(@"Send Request Error: %@",error.localizedDescription);
    }
}

- (void) gameRequestDialogDidCancel:(FBSDKGameRequestDialog *)gameRequestDialog
{
    NSLog(@"Cancelled by user");
}

extern "C" void OS_SendRequest(const char* ID, const char* title, const char* message)
{
    dispatch_queue_t dispatchQueue = dispatch_queue_create("FBSendRequest.Pine.Entertainment.com", 0);   //ie. your made-up queue name
    dispatch_queue_is_empty(dispatchQueue);
    dispatch_async(dispatch_get_main_queue(),
                   ^{
                       NSString* sfriendID = [NSString stringWithFormat:@"%s",ID];
                       NSString* stitle = [NSString stringWithFormat:@"%s",title];
                       NSString* smessage = [NSString stringWithFormat:@"%s",message];
                       
                       [[PineGameDelegate sharedInstance] SendRequest:sfriendID withTile:stitle withMessage:smessage];
                   });
}
extern "C" bool OS_HasFacebookToken()
{
    FBSDKAccessToken *token = [SUCache itemForSlot:0].token;
    if(token != nil)
    {
        return true;
    }
    return false;
}
#endif
