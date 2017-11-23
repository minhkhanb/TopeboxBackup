#import "PineGameDelegate.h"
#import "EAGLView.h"
#import "CGame.h"
#import "Os_Functions.h"
#import "GameCenterManager.h"
#import <UIKit/UIView.h>
extern "C" void DevicePushRemoteNotification(char* device_token,char *message);
extern "C" bool OS_IsNetworkAvailable();
extern "C" bool isGCAvailable();
extern "C" void InitGCBoard();
extern const char* _tmpString = NULL;
extern const char* _tmpString2 = NULL;
extern  int _resume_count = 0;
extern int	_tmpnum = 0;
extern "C" void GamePause();
extern "C" void GameResume();
extern "C" void GameExit();
extern "C" void StartThread();
PineGameDelegate *g_PineGameInstance = nil;
extern "C" void EndGame();
@implementation PineGameDelegate
extern "C" bool HasOpenFeintInit();
extern "C" void ShutdownIfNotOnline();
extern "C" int  GetMotionAnchor();
extern "C" void SetMotionAnchor(int anchor);
extern "C" void InitInAppPurchase();
@synthesize AppPurchase;
@synthesize window;
@synthesize glView;
@synthesize _opgenGLESViewController;
UIBackgroundTaskIdentifier bgTask = NULL;



#if ADD_CHARTBOOST
#include "PineChartBoost.h"
#endif
#if ADD_IADVERT
#include "PineIAdvert.h"
#endif
#if USE_ADBOX
extern "C" void OS_AdBoxDownloadFile();
#endif

#if ADD_UNITYADS
PineViewController* _pine_view_unityads;
NSString* _unity_app_id;
#endif

extern "C" const  char* GetBundlePath();
extern "C" void DevicePushRemoteNotification(char* device_token,char *message);


- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    NSLog(@"\nshouldAutorotateToInterfaceOrientation = %d\n",interfaceOrientation);
    return YES;
    
}


- (void) applicationDidFinishLaunching:(UIApplication*)application
{
    //printf("\nsizeof(long) = %d sizeof(int) = %d",sizeof(long),sizeof(int));
    //launch game
#if ADD_FACEBOOK
    [FBSDKProfile enableUpdatesOnAccessTokenChange:YES];
    [self PineFacebookTokenRegister];
#endif
    _resume_count =0;
    _version_display = 0;
    g_PineGameInstance = self;
    application.idleTimerDisabled = YES;
    //[[self navigationController] setNavigationBarHidden:YES animated:NO];
    [[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
    
    [application setStatusBarOrientation:UIInterfaceOrientationPortrait animated:NO];
    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
    
#if ENABLE_SERVER_PUSH_NOTIFICATION
    [[UIApplication sharedApplication] registerForRemoteNotificationTypes:((UIRemoteNotificationType)(UIRemoteNotificationTypeBadge| UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert))];
#endif
    [self InitDisplay];
    _openfeint_init = false;
    AppPurchase = [InAppPurchase alloc];
    
#if ADD_PICK_IMAGE
    
    _PickState =  PICK_STATE_NONE;
#endif
    //sleep(3);
    //SetMotionAnchor(MOTION_LANDSCAPE_RIGHT_ANCHOR);
    //[self InitPickImage];
    
    //sleep(3);
    
    /*
     if(isGCAvailable())
     {
     InitGCBoard();
     }
     */
#if ADD_TWITTER
    tw_waccountStore = nil;
#endif
#if ADD_FACEBOOK
    fb_accountStore = nil;
#endif
    
#if ADD_MAIL_SUPPORT
    _mailViewController = [MailTutorialViewController alloc];
#endif
    
#if ADD_SCREEN_CAPTUREVIEW
    _pine_view_captureScreen = [PineViewController alloc];
#endif
    
#if ADD_IADVERT
    _pine_view_iAD = [PineViewController alloc];
    _pine_view_iAD->requestingAd = NO;
#endif
    //_mailViewController = [MailTutorialViewController alloc];
    StartThread();
    sleep(3);
    while(CGame::_this && CGame::_this->_GameState == k_GS_LOGO && ((HSGame*)CGame::_this->_current_game)->_loading_step < 1)
    {
        sleep(1);
    }
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    HSGame*game = (HSGame*)(GAME()->_current_game);
    UIApplication*    app = [UIApplication sharedApplication];
    
    bgTask = [app beginBackgroundTaskWithExpirationHandler:^{
        [app endBackgroundTask:bgTask];
        bgTask = UIBackgroundTaskInvalid;
    }];
    
    // Start the long-running task and return immediately.
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        // Do the work associated with the task.
        // Do the work associated with the task.
        game->_count_show_pineads--;
        if(GAME()->_notificationEnable)
        {
            UILocalNotification *notification = [[UILocalNotification alloc] init];
            NSDate *notificationDate;
            
            
            //printf("\n slot %d -> time = %d, text = %s\n", i, OS_NOTIFY().time_in_slot[i], OS_NOTIFY()._text_display[i]);
            
            /*for(int i= 0; i < game->_push_notify_manager._numNotification; i++)
             {
             UILocalNotification *notification = [[UILocalNotification alloc] init];
             NSDate *notificationDate;
             
             if(game->_push_notify_manager._notifiction[i]._timePush <= 0)
             {
             continue;
             }
             printf("\n slot %d -> time = %lld, type = %d \n", i, game->_push_notify_manager._notifiction[i]._timePush, game->_push_notify_manager._notifiction[i]._type);
             notificationDate = [NSDate dateWithTimeIntervalSinceNow:game->_push_notify_manager._notifiction[i]._timePush];
             notification.fireDate  = notificationDate;
             notification.timeZone  = [NSTimeZone systemTimeZone];
             notification.applicationIconBadgeNumber = 1;
             notification.alertBody = [[NSString alloc] initWithUTF8String:game->_push_notify_manager._notifiction[i]._text];
             //notification.soundName = @"sfx_notify.wav";
             
             [[UIApplication sharedApplication] scheduleLocalNotification:notification];
             }*/
        }
        
        
        [app endBackgroundTask:bgTask];
        bgTask = UIBackgroundTaskInvalid;
    });
    
    sleep(0.1);
    printf("app did entr background");
}
- (void) applicationDidBecomeActive:(UIApplication*)application
{
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
#if USE_ADBOX
    [self AdBoxDidBecameActive:application];
#else
#if ADD_CHARTBOOST
    [self chartboostDidBecameActive:application];
#endif
#endif
#if ADD_FACEBOOK
    [FBSDKAppEvents activateApp];
#endif
    GameResume();
    
#if ADD_PINE_ADVERT
    CGame::_this->_advert.SetBecomeActive();
#endif
#if NOTIFICATION_MAX
    Device::PushNotifyBecameActive();
#endif
    if(_resume_count != 0)
    {
        /*if([PineGameDelegate sharedInstance]->_motion_anchor == MOTION_LANDSCAPE_RIGHT_ANCHOR)
         {
         [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight animated:NO];
         }
         else if([PineGameDelegate sharedInstance]->_motion_anchor == MOTION_LANDSCAPE_LEFT_ANCHOR)
         {
         [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeLeft animated:NO];
         }*/
        
    }
    _resume_count++;
    /*if((SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"8.0")))
    {
        [[UIApplication sharedApplication]  registerUserNotificationSettings:[UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeAlert|UIUserNotificationTypeBadge|UIUserNotificationTypeSound categories:nil]];
    }*/
}

- (void) applicationWillResignActive:(UIApplication *)application
{
    GamePause();
    GAME()->PauseEx();
#if NOTIFICATION_MAX
    if (CGame::_this->_notificationEnable)
    {
        Device::PushNotifyResignActive();
    }
#endif
#if ADD_ADVERT
    CGame::_this->_advert.ResignActive();
#endif
    /*
     if(HasOpenFeintInit())
     {
     [OpenFeint shutdown];
     _openfeint_init = false;
     }
     */
    //[OpenFeint applicationWillResignActive];
}

- (void) applicationWillTerminate:(UIApplication*)aNotification
{
    //save data
    //GameExit();
    //if(HasOpenFeintInit())
    //{
    //[OpenFeint shutdown];
    //_openfeint_init = false;
    //}
    EndGame();
}

#ifdef DEBUG_MEMORY_WARNING
- (void) applicationDidReceiveMemoryWarning:(UIApplication*)application
{
    //show a warning dialog
}
#endif
-(void) iOS_ShowWebPage:(NSString*) url isoffline:(BOOL) offline
{
    [_pine_view_controller iOS_ShowWebPage:url isoffline:offline];
}
- (void) InitDisplay
{
    _pine_view_controller = [PineViewController alloc];
    
    CGRect rect = [[UIScreen mainScreen] bounds];
    CGRect windowRect = [[UIScreen mainScreen] applicationFrame];
    
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
    _opgenGLESViewController = [[OpenGLESViewController alloc] init];
    
    float correctW, correctH;
    if (UIInterfaceOrientationIsLandscape([[UIApplication sharedApplication] statusBarOrientation]))
    {
        correctW = windowRect.size.width>windowRect.size.height?windowRect.size.width:windowRect.size.height;
        correctH = windowRect.size.height<windowRect.size.width?windowRect.size.height:windowRect.size.width;
        
    }
    else
    {
        correctW = windowRect.size.width<windowRect.size.height?windowRect.size.width:windowRect.size.height;
        correctH = windowRect.size.height>windowRect.size.width?windowRect.size.height:windowRect.size.width;
    }
    windowRect.size.width = correctW;
    windowRect.size.height = correctH;
    NSLog(@"windowRect %f %f ",correctW,correctH);
    
#else
    UIDevice *myDevice = [UIDevice currentDevice] ;
    [myDevice beginGeneratingDeviceOrientationNotifications];
    UIDeviceOrientation deviceOrientation = myDevice.orientation;
    BOOL isCurrentlyLandscapeView = UIDeviceOrientationIsLandscape(deviceOrientation);
    [myDevice endGeneratingDeviceOrientationNotifications];
    float tmp = rect.size.width;
    
    if(isCurrentlyLandscapeView){
        
        rect.size.width = rect.size.height> rect.size.width?rect.size.height:rect.size.width;
        rect.size.height = rect.size.width!=tmp? tmp:rect.size.height;
        printf("if :%f :%f\n", rect.size.width, rect.size.height);
    }else{
        rect.size.width = rect.size.height< rect.size.width?rect.size.height:rect.size.width;
        rect.size.height = rect.size.width!=tmp? tmp:rect.size.height;
        printf("else :%f :%f\n", rect.size.width, rect.size.height);
    }
#endif
    bool i6Later = false;
    float scale_factor = 2.0f;
    
    if((rect.size.width == 375.000000&& rect.size.height ==667.000000)||(rect.size.width == 414.000000&& rect.size.height ==736.000000)
       ||(rect.size.width == 667.000000&& rect.size.height == 375.000000)||(rect.size.width == 736.000000&& rect.size.height == 414.000000)
       ||(rect.size.width == 320.000000&& rect.size.height == 568.000000)||(rect.size.width == 568.000000&& rect.size.height == 320.000000)
       )
    {
        i6Later = true;
        if((rect.size.width == 375.000000&& rect.size.height == 667.000000)
           || (rect.size.width == 667.000000&& rect.size.height == 375.000000))
        {
            scale_factor = 2.0f;
        }
        if((rect.size.width == 414.000000&& rect.size.height ==736.000000)
           || (rect.size.width == 736.000000&& rect.size.height ==414.000000))
        {
            scale_factor = 3.0f;
        }
        printf("dkfjkdfjkdjfdkfkd fdkf dfjkdjfdkfkdjfefkdkfkdf \n");
    }
    
    CGRect rt =  CGRectMake(0,0,IOS8_MASTER_W/2,IOS8_MASTER_H/2);
    
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
    if (rect.size.width > rect.size.height)
    {
        rt = CGRectMake(0,0,IOS8_MASTER_H/2,IOS8_MASTER_W/2);
    }
#else
#endif
    
    window = [[UIWindow alloc] initWithFrame:rect];
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
    [window setRootViewController:_opgenGLESViewController];
#endif
    window.backgroundColor = [UIColor blackColor];
    
    CGRect fromRect = rt;
    CGRect viewRect = rect;
    
    CGSize scales = CGSizeMake(viewRect.size.width/fromRect.size.width,
                               viewRect.size.height/fromRect.size.height);
    
    CGAffineTransform transform = CGAffineTransformMakeScale(scales.width,
                                                             scales.height);
    
    
    //[window sizeThatFits:rect.size];
    //Create the OpenGL ES view and add it to the window
    if(i6Later)
    {
        glView = [[EAGLView alloc] initGL:rt];
        [glView initWithFrame:rt];
        //NSLog(@"dkfjkjf jkj kfdjfkdfj rect %f %f ",rt.size.width,rt.size.height);
    }
    else
    {
        glView = [[EAGLView alloc] initGL:windowRect];
        [glView initWithFrame:windowRect];
        
    }
    glView.multipleTouchEnabled = YES;
    
    //add OpenGL ES view to window
    if(i6Later)
    {
        window.contentScaleFactor = scale_factor;
        glView.contentScaleFactor = scale_factor;
        glView.transform = transform;
        glView.center = CGPointMake(rect.size.width/2,rect.size.height/2);
    }
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
    _opgenGLESViewController.view = glView;
    [window addSubview:_opgenGLESViewController.view];
#else
    [window addSubview:glView];
#endif
    //Show the window
    [window makeKeyAndVisible];
}

#if CAPTURE_SCREENSHOT
extern "C" bool ExitsFile(const char *file_name);
-(void) capture_screenshot
{
    UIGraphicsBeginImageContextWithOptions(glView.bounds.size, NO, 1.0f);
    
    [glView drawViewHierarchyInRect:glView.bounds afterScreenUpdates:NO];
    
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    NSData*theImageData=UIImagePNGRepresentation(image); //you can use PNG too
    NSString* fileToSaveTo = [NSString stringWithFormat:@"%s" , "pine_capture.png"];
    
    //NSString* fileURL = [NSString stringWithFormat:@"%s" , url];
    NSArray* path = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
    NSString* documentsDirectory = [path objectAtIndex:0];
    
    //[theImageData writeToFile:@"pine_capture.png" atomically:YES];
    [theImageData writeToFile:[NSString stringWithFormat:@"%@/%@",documentsDirectory,fileToSaveTo] atomically:NO];
    while(!ExitsFile("pine_capture.png")){
        ;
        printf("\nCheck Exits file");
        sleep(1);
    }
    printf("\nCAPTURE complete");
    
}
#endif

#ifdef USE_VIDEO
- (void) MoviePlayerInit:(NSString*)movie_name
{
    MoviePlayer* mp = [[MoviePlayer alloc] initWithFile:movie_name];
    [MoviePlayer setSharedInstance:mp];
}
#endif

#ifndef USE_CUSTOM_KEYBOARD

+ (PineGameDelegate*) sharedInstance
{
    return g_PineGameInstance;
}

- (void) TextInputInit:(TextInputInitParams*)params
{
    TextInput* ti = [[TextInput alloc] initWithTextBuffer:(char*)params->textBuff WithSize:params->buffSize AtX:params->x Y:params->y Width:params->w Height:params->h];
    [TextInput setSharedInstance:ti];
}

#endif

- (void) MediaPickerInit:(MediaPickerParams*)params
{
    //printf("MediaPickerInit \n");
    MediaPicker* mi = [[MediaPicker alloc] init];
    
    [MediaPicker setSharedInstance: mi];
    [mi showMediaPicker];
}

//////////////////////////////////////////////////////////////////////////
- (void) dealloc
{
    [glView release];
    [window release];
    [super dealloc];
}

- (void) CleanTouch
{
    [glView ClearTouch];
}


- (void) GCLeaderBoard:(NSObject*)params
{
    /*
     if(![GameCenterManager getSharedInstance])
     {
     GameCenterManager* gc = [[GameCenterManager alloc] init];
     [GameCenterManager setSharedInstance: gc];
     }
     [[GameCenterManager getSharedInstance] initGCBoard];
     */
    [[GameCenterManager getSharedInstance] showLeaderboard];
}

- (void) GCAchievementBoard:(NSObject*)params
{
    /*
     if(![GameCenterManager getSharedInstance])
     {
     GameCenterManager* gc = [[GameCenterManager alloc] init];
     [GameCenterManager setSharedInstance: gc];
     }
     [[GameCenterManager getSharedInstance] initGCBoard];
     */
    [[GameCenterManager getSharedInstance] showAchievementsboard];
}



//In app Purchase




//#endif
#if ADD_PICK_IMAGE
///-(void) SaveImageToPath(UIImage* image, NSString *file,int width_target,int hieght_target)
//{

///}
-(void) InitTakePictureDelegate
{
    
    _PickState =  PICK_STATE_TAKE_PICTURE;
    bool init = false;
    if(_image_pick == nil)
    {
        _image_pick = [[UIImagePickerController alloc] init];
        init = true;
    }
    _image_pick.delegate = self;
    _image_pick.sourceType = UIImagePickerControllerSourceTypeCamera;
    [_image_pick setShowsCameraControls:NO];
    _image_pick.view.hidden = NO;
    [glView removeFromSuperview];
    [window addSubview:_image_pick.view];
    [_image_pick.view addSubview:glView];
    
    
    
}
-(void)takeAPictureSaveToPath
{
    [_image_pick takePicture];
    
    /*[_image_pick dismissModalViewControllerAnimated:YES];
     [glView removeFromSuperview];
     _image_pick.view.hidden =  YES;
     [_image_pick.view removeFromSuperview];
     [window addSubview:glView];
     */
}
-(void) InitPickImage
{
    _PickState =  PICK_STATE_CHOICE_PICTURE;
    if(_image_pick == nil)
    {
        _image_pick = [[UIImagePickerController alloc] init];
    }
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) {
        UIPopoverController *popover = [[UIPopoverController alloc] initWithContentViewController:_image_pick];
        _image_pick.delegate = self;
        _image_pick.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
        
        [popover presentPopoverFromRect:CGRectMake(0, k_SCREEN_HEIGHT,1,1) inView:glView permittedArrowDirections:UIPopoverArrowDirectionDown animated:YES];
        
    }
    else
    {
        
        _image_pick.delegate = self;
        _image_pick.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
        [window addSubview:_image_pick.view];
    }
}

-(void) imagePickerController:(UIImagePickerController*)picker
        didFinishPickingImage:(UIImage *)image
                  editingInfo:(NSDictionary *)editingInfo
{
    
    [picker dismissModalViewControllerAnimated:YES];
    [glView removeFromSuperview];
    _image_pick.view.hidden =  YES;
    [_image_pick.view removeFromSuperview];
    [window addSubview:glView];
    
}
-(void) imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [picker dismissModalViewControllerAnimated:YES];
    [glView removeFromSuperview];
    _image_pick.view.hidden =  YES;
    [_image_pick.view removeFromSuperview];
    [window addSubview:glView];
    //exit(0);
}
- (void) PushRemoteNotification:(NSObject*)params
{
    
    DevicePushRemoteNotification("f270c51076723179cca57e93b85c4935927f219d3791bccc700a1232ba77530c","Hello world!");
    
}

#endif
#if ENABLE_SERVER_PUSH_NOTIFICATION
- (void)application:(UIApplication *)app didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    NSString *str =
    [NSString stringWithFormat:@"\n Device token %@",deviceToken];
    printf("\nbondle path = %s",GetBundlePath());
    //[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(PushRemoteNotification:) withObject:nil waitUntilDone:YES];
    NSLog(str);
    ///385ee17eb8de4c0306fe42979cc67ba2dbd332c812e3dcbd07d858aa69df6146
}

- (void)application:(UIApplication *)app didFailToRegisterForRemoteNotificationsWithError:(NSError *)err
{
    NSString *str = [NSString stringWithFormat: @"Error: %@", err];
    NSLog(str);
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
    
    for (id key in userInfo) {
        NSLog(@"key: %@, value: %@", key, [userInfo objectForKey:key]);
    }
    
}
#endif

- (void) Thread_LoadImageToPngFile:(NSArray*)params
{
    const char *url = [(NSString*)[params objectAtIndex:0] UTF8String];
    const char *file_name = [(NSString*)[params objectAtIndex:1] UTF8String];
    printf("url = %s _file_name = %s",url,file_name);
    //const char *file_name = params
    //create URL load
    
    NSString *_url = [NSString stringWithUTF8String:url];
    
    //load data image from URL (can be jpg)
    UIImage *image = [[UIImage alloc] initWithData:[NSData dataWithContentsOfURL:[NSURL URLWithString:_url ]]];;
    
    //convert imagedata to PNG
    NSData * _imgdata = UIImagePNGRepresentation(image);
    [image release];
    
    //create path to save file
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    NSString* fname = [[NSString alloc] initWithUTF8String: file_name];
    NSString *appfilename = [documentsDirectory stringByAppendingPathComponent:fname];
    
    //save file to document
    [_imgdata writeToFile:appfilename  atomically:NO];
    [_imgdata release];
    
}

- (void) LoadIAP
{
    AppPurchase = [InAppPurchase alloc];
    if([AppPurchase CanPayment])
    {
        printf("\nYou can buy item");
        InitInAppPurchase();
        
    }

}

#if ADD_MAIL_SUPPORT
-(void) SendMailSupport:(NSString*) body withTitle:(NSString*) title
{
    [_mailViewController PopupEmail:body withTitle:title];
}

UIAlertView* _alertMessage_OKCancel;
-(void)showAlertMessage_OKCancel:(NSArray*) stringArray
{
    NSString *feed, *title,*tag;
    feed = [[NSString alloc] initWithFormat:@"%@", [stringArray valueForKey:@"Feed"]];
    title = [[NSString alloc] initWithFormat:@"%@", [stringArray valueForKey:@"Title"] ];
    tag = [[NSString alloc] initWithFormat:@"%@", [stringArray valueForKey:@"Tag"] ];
    
    _alertMessage_OKCancel = [[UIAlertView alloc] initWithTitle:title
                                                        message:feed
                                                       delegate:self
                                              cancelButtonTitle:@"Cancel"
                                              otherButtonTitles:@"OK", nil];
    _alertMessage_OKCancel.tag = [tag intValue];
    [_alertMessage_OKCancel show];
    [feed release];
    [title release];
}

UIAlertView* _alertMessage_OK;
-(void)showAlertMessage_OK:(NSArray*) stringArray
{
    NSString *feed, *title,*tag;
    feed = [[NSString alloc] initWithFormat:@"%@", [stringArray valueForKey:@"Feed"]];
    title = [[NSString alloc] initWithFormat:@"%@", [stringArray valueForKey:@"Title"] ];
    tag = [[NSString alloc] initWithFormat:@"%@", [stringArray valueForKey:@"Tag"] ];
    
    _alertMessage_OK = [[UIAlertView alloc] initWithTitle:title
                                                  message:feed
                                                 delegate:self
                                        cancelButtonTitle:@"OK"
                                        otherButtonTitles:nil];
    _alertMessage_OK.tag = [tag intValue];
    [_alertMessage_OK show];
    [feed release];
    [title release];
}
#endif

#if ADD_TWITTER
#include "PineTwitter.h"
#endif
#if ADD_FACEBOOK
#include "PineFacebook.h"
#endif
#include "PineMacOSFunc.h"
@end

extern "C" bool isChartboostAvailable()
{
    return [[PineGameDelegate sharedInstance] chartboostAvailable];
    //[[PineGameDelegate sharedInstance] InitTakePicture];
    
}

extern "C" void InitTakePicture()
{
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(InitTakePictureDelegate) withObject:nil waitUntilDone:NO];
    //[[PineGameDelegate sharedInstance] InitTakePicture];
    
}
extern "C" void takeAPicture()
{
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(takeAPictureSaveToPath) withObject:nil waitUntilDone:NO];
    //[[PineGameDelegate sharedInstance] takeAPictureSaveToPath];
    
}

extern "C" void LoadImageToPngFile(const char* url,const char *file_name)
{
    NSArray* a = [[NSArray alloc] init];
    NSString* _url = [NSString stringWithFormat:@"%s",url];
    NSString* _file_name = [NSString stringWithFormat:@"%s",file_name];
    a = [a arrayByAddingObject:_url];
    a = [a arrayByAddingObject:_file_name];
    [[PineGameDelegate sharedInstance] Thread_LoadImageToPngFile:a];
    
}


#if CAPTURE_SCREENSHOT
extern "C" void CaptureScreenshot()
{
    [[PineGameDelegate sharedInstance] capture_screenshot];
    
}
#endif

#if ADD_MAIL_SUPPORT
//@quoc.nguyen
extern "C" void	GoToReviewInItune()
{
    NSString *reviewURL = [APPIRATER_TPL_URL stringByReplacingOccurrencesOfString:@"APP_ID" withString:[NSString stringWithFormat:@"%d", APPLE_APP_ID]];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:reviewURL]];
    
}

extern "C" void ShowAlertMessage_OKCancel(const char* feed, const char* title, const char* tag)
{
    NSString *feedString = [NSString stringWithUTF8String:feed];
    NSString *feedTitle = [NSString stringWithUTF8String:title];
    NSString *TagNum = [NSString stringWithUTF8String:tag];
    NSDictionary *stringArray = [[NSDictionary alloc] initWithObjectsAndKeys:
                                 feedString, @"Feed", feedTitle, @"Title", TagNum, @"Tag", nil];
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(showAlertMessage_OKCancel:) withObject:stringArray waitUntilDone:YES];
    //[[PineGameDelegate sharedInstance] showMessageWarning:sfeed];
    //[feedString release];
    //[feedTitle release];
    [stringArray release];
}

extern "C" void ShowAlertMessage_OK(const char* feed, const char* title, const char* tag)
{
    NSString *feedString = [NSString stringWithUTF8String:feed];
    NSString *feedTitle = [NSString stringWithUTF8String:title];
    NSString *TagNum = [NSString stringWithUTF8String:tag];
    NSDictionary *stringArray = [[NSDictionary alloc] initWithObjectsAndKeys:
                                 feedString, @"Feed", feedTitle, @"Title", TagNum, @"Tag", nil];
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(showAlertMessage_OK:) withObject:stringArray waitUntilDone:YES];
    //[[PineGameDelegate sharedInstance] showMessageWarning:sfeed];
    //[feedString release];
    //[feedTitle release];
    [stringArray release];
}

extern "C" void SendMailSupport(const char *body, const char *title)
{
    
    NSString* sbody = [NSString stringWithFormat:@"%s",body];
    NSString* stitle = [NSString stringWithFormat:@"%s",title];
    [[PineGameDelegate sharedInstance]  SendMailSupport:sbody withTitle:stitle];
}

extern "C" int getConfigAds()
{
    if(Device::IsNetworkAvailable() && PineSocial::PingToHost(ADS_CONFIG_URL,5))
    {
        if (Device::DownloadFile(ADS_CONFIG_URL, ADS_CONFIG_FILE, 5))
        {
            int len =GetSizeFile(ADS_CONFIG_FILE);
            if (len > 0)
            {
                BYTE *infomation = new BYTE[len+1];
                ReadAppDataBinary(ADS_CONFIG_FILE,infomation);
                infomation[len]='\0';
                
                char version[128];
                sprintf(version,"%d",0);
                for (int i  = 0; i < len; i ++)
                {
                    version[i] = infomation[i];
                }
                version[len]='\0';
                delete[] infomation;
                return atoi(version);
            }
        }
    }
    return 3;
}

#endif
extern "C" void RequestRegisterNotificationSetting()
{
    //NSLog(@"\n has notttt registerUserNotificationSettings");
    /*UIRemoteNotificationType myTypes = UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeSound;
    [[UIApplication sharedApplication] registerForRemoteNotificationTypes:myTypes];*/
    if((SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"8.0")))
     {
     [[UIApplication sharedApplication]  registerUserNotificationSettings:[UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeAlert|UIUserNotificationTypeBadge|UIUserNotificationTypeSound categories:nil]];
     }

}

