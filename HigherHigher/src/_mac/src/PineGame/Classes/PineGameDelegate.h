//
//  PineGameDelegate.h

#import "TextInput.h"
#import "MediaPicker.h"
#import <UIKit/UIKit.h>
#import "CGame.h"
#if ADD_TWITTER
#import <Social/Social.h>
#import <Accounts/Accounts.h>
#import <Twitter/TWTweetComposeViewController.h>
#import <Twitter/TWRequest.h>
#endif
#define OF_PRODUCT_KEY  "8vHMY83lQiNLQWx9u89dw"
#define OF_SECRET_KEY   "wYAkdRQJUJKe694h7n1u2WYmTS32YxMekeE4sbyxM"

#if ADD_CHARTBOOST
#import <Chartboost/Chartboost.h>
#endif
#import "PineViewController.h"

#import "OpenGLESViewController.h"


#define APPLE_APP_ID				APPLE_ID
#define APPIRATER_MESSAGE_TITLE		@"Rate 5 Stars!"
#define APPIRATER_MESSAGE			@""


#define APPIRATER_RATE_BUTTON		@"Rate 5 Stars!"
#define APPIRATER_CANCEL_BUTTON		@"Remind me later"
#define APPIRATER_RATE_LATER		@"Remind me later"

#define APPIRATER_TPL_URL			@"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=APP_ID&onlyLatestVersion=true&pageNumber=0&sortOrdering=1&type=Purple+Software"

#import "InAppPurchase.h"
#import "HJImagesToVideo.h"

#if ADD_MAIL_SUPPORT
#import "MailTutorialViewController.h"
#endif

@class EAGLView;

@interface PineGameDelegate : NSObject <UIApplicationDelegate
#if ADD_PICK_IMAGE
,UINavigationControllerDelegate
,UIImagePickerControllerDelegate
#endif
#if ADD_CHARTBOOST
,ChartboostDelegate
#endif
> {
    UIWindow *window;
    EAGLView *glView;
@public
    bool _openfeint_init;
    int _version_display;
    InAppPurchase *AppPurchase;
    int _motion_anchor;
    bool _openfeint_showdialog;
#if ADD_PICK_IMAGE
    UIImagePickerController* _image_pick;
    //UIImage* _image_data;
    //UIImageView* _image_view;
    int _PickState;
#endif
#if ADD_TWITTER
    int _TW_status;
    ACAccountStore *tw_waccountStore;
    
#endif
#if ADD_FACEBOOK
    ACAccountStore *fb_accountStore;
#endif
#if CAPTURE_SCREENSHOT
#endif
    
#if ADD_MAIL_SUPPORT
    MailTutorialViewController *_mailViewController;
#endif
    
#if ADD_CHARTBOOST
    NSString *_chartboost_app_id;
    NSString *_chartboost_app_syn;
#endif
    //#if ADD_GAMECENTER
    PineViewController* _pine_view_controller;
    //#endif
#if ADD_IADVERT
    PineViewController* _pine_view_iAD;
#endif
    
    OpenGLESViewController *_opgenGLESViewController;
#if ADD_ADMOB
    PineViewController* _pine_view_admob;
    NSString* _admob_app_id;
    NSString* _admob_app_syn;
#endif
    
#if ADD_SCREEN_CAPTUREVIEW
    PineViewController *_pine_view_captureScreen;
#endif
    
#if ADD_VUNGLEADS
    PineViewController* _pine_view_vungle;
    NSString* _vungle_app_id;
#endif
    
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet EAGLView *glView;
@property (nonatomic, retain) IBOutlet InAppPurchase *AppPurchase;
@property (nonatomic, retain) IBOutlet OpenGLESViewController *_opgenGLESViewController;
#if ADD_TWITTER
@property (nonatomic, retain) IBOutlet ACAccountStore *tw_waccountStore;
#endif
#if ADD_FACEBOOK
@property (nonatomic, retain) IBOutlet ACAccountStore *fb_accountStore;
#endif
#if ADD_MAIL_SUPPORT
@property (nonatomic, retain) IBOutlet MailTutorialViewController *_mailViewController;
#endif
- (void) InitDisplay;
+(PineGameDelegate*)sharedInstance;
//#ifdef USE_IPHONE_KEYBOARD
-(void)TextInputInit:(TextInputInitParams*)params;
-(void)MediaPickerInit:(MediaPickerParams*)params;
#if ADD_PICK_IMAGE
-(void) InitPickImage;

#endif
#if ADD_TWITTER
-(void) TwitterInit;
-(void) PostToTwitter:(NSString *)feed;
- (BOOL)userHasAccessToTwitter;
-(void) PostScreenToTwitter:(NSString*) feed;
#endif
#if ADD_FACEBOOK
-(void)getFacebookFriendsForAccount:(ACAccount*)account;
#endif
#if ADD_MAIL_SUPPORT
-(void) SendMailSupport:(NSString*) body withTitle:(NSString*) title;
-(void)showAlertMessage_OKCancel:(NSArray*) stringArray;
-(void)showAlertMessage_OK:(NSArray*) stringArray;
#endif
@end

