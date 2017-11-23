//
//  PineMacOSFunc.h
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 5/19/14.
//
//
#import <SystemConfiguration/SCNetworkReachability.h>
#ifndef __PineEntertainment__PineMacOSFunc__
#define __PineEntertainment__PineMacOSFunc__
#define IS_IPAD (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
#define IS_IPHONE (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
#define IS_RETINA ([[UIScreen mainScreen] scale] >= 2.0)

#define DEVICE_WIDTH ([[UIScreen mainScreen] applicationFrame].size.width)
#define DEVICE_HEIGHT ([[UIScreen mainScreen] applicationFrame].size.height)
#define SCREEN_MAX_LENGTH (MAX(DEVICE_WIDTH, DEVICE_HEIGHT))
#define SCREEN_MIN_LENGTH (MIN(DEVICE_WIDTH, DEVICE_HEIGHT))

#define IS_IPHONE_4_OR_LESS (IS_IPHONE && SCREEN_MAX_LENGTH < 568.0)
#define IS_IPHONE_5 (IS_IPHONE && SCREEN_MAX_LENGTH == 568.0)
#define IS_IPHONE_6 (IS_IPHONE && SCREEN_MAX_LENGTH == 667.0)
#define IS_IPHONE_6P (IS_IPHONE && SCREEN_MAX_LENGTH == 736.0)

#include "PineAdBox.h"

extern "C" float OS_GetVolume()
{
    return [[AVAudioSession sharedInstance] outputVolume] ;
}
extern "C" bool Os_IsDeviceWeak()
{
    
    return (IS_IPHONE_4_OR_LESS) || (IS_IPHONE_5) || (!IS_RETINA);
}
-(int) GetNetworkSupport
{
    SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithName(NULL, "8.8.8.8");
    SCNetworkReachabilityFlags flags;
    BOOL success = SCNetworkReachabilityGetFlags(reachability, &flags);
    CFRelease(reachability);
    if (!success) {
        return NETWORK_TYPE_UNKOWN;
    }
    BOOL isReachable = ((flags & kSCNetworkReachabilityFlagsReachable) != 0);
    BOOL needsConnection = ((flags & kSCNetworkReachabilityFlagsConnectionRequired) != 0);
    BOOL isNetworkReachable = (isReachable && !needsConnection);
    
    if (!isNetworkReachable) {
        return NETWORK_TYPE_UNKOWN;
        //return ConnectionTypeNone;
    } else if ((flags & kSCNetworkReachabilityFlagsIsWWAN) != 0) {
        //return ConnectionType3G;
        return NETWORK_TYPE_3G;
    } else {
        //return ConnectionTypeWiFi;
        return NETWORK_TYPE_WIFI;
    }
}

extern "C" int OS_GetNetworkSupport()
{
    return [[PineGameDelegate sharedInstance] GetNetworkSupport];
}
BOOL dispatch_queue_is_empty(dispatch_queue_t queue)
{
    dispatch_group_t group = dispatch_group_create();
    
    dispatch_group_enter(group);
    dispatch_async(queue, ^{
        dispatch_group_leave(group);
    });
    
    int64_t maxWaitTime = 0.00000005 * NSEC_PER_SEC;
    BOOL isReady = dispatch_group_wait(group, maxWaitTime) == 0;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
        dispatch_group_wait(group, DISPATCH_TIME_FOREVER);
        dispatch_release(group);
    });
    
    return isReady;
}
extern "C" void OS_SetAdboxActive(bool isThread)
{
    dispatch_queue_t dispatchQueue = dispatch_queue_create("SetAdboxActive.Pine.Entertainment.com", 0);   //ie. your made-up queue name
    dispatch_queue_is_empty(dispatchQueue);
    //[[PineGameDelegate sharedInstance] AdBoxDidBecameActive:[UIApplication sharedApplication]];
    if(isThread)
    {
        [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(AdBoxDidBecameActive:) withObject:[UIApplication sharedApplication] waitUntilDone:NO];
    }
    else
    {
        [[PineGameDelegate sharedInstance] AdBoxDidBecameActive:[UIApplication sharedApplication]];
    }
}

extern "C" bool ExitsFile(const char *file_name);
extern "C" bool OS_IsNetworkAvailable();

UIAlertView* _alertwarning;
UIAlertView* _rating;
-(void)showMessageWarning:(NSArray*) stringArray
{
    NSString *feed, *title;
    feed = [[NSString alloc] initWithFormat:@"%@", [stringArray valueForKey:@"Feed"]];
    title = [[NSString alloc] initWithFormat:@"%@", [stringArray valueForKey:@"Title"] ];
    
    _alertwarning = [[UIAlertView alloc] initWithTitle:title
                                               message:feed
                                              delegate:self
                                     cancelButtonTitle:@"OK"
                                     otherButtonTitles:nil];
    _alertwarning.tag = 1;
    [_alertwarning show];
    [feed release];
    [title release];
}
//In app Purchase

- (void)showRating:(NSString*)title body:(NSString*)body {
    dispatch_sync(dispatch_get_main_queue(), ^{
        UIAlertView *_rating = [[UIAlertView alloc] initWithTitle:title
                                                          message:body
                                                         delegate:self
                                                cancelButtonTitle:APPIRATER_CANCEL_BUTTON
                                                otherButtonTitles:APPIRATER_RATE_BUTTON, nil];
        _rating.tag = 2;
        [_rating show];
    });
}


- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    if(2 == alertView.tag)
    {
        printf("Raing checking");
        switch (buttonIndex) {
            case 0:
                
                break;
            case 1:
            {
                char filename[256];
                sprintf(filename,"%s",OS_GetDocumentFilePath(RATING_FILE));
                FILE* f= fopen(filename,"w");
                if(f!= NULL)
                {
                    fclose(f);
                }
                NSString *reviewURL = [APPIRATER_TPL_URL stringByReplacingOccurrencesOfString:@"APP_ID" withString:[NSString stringWithFormat:@"%d", APPLE_APP_ID]];
                [[UIApplication sharedApplication] openURL:[NSURL URLWithString:reviewURL]];
                //[userDefaults setBool:YES forKey:kAppiraterRatedCurrentVersion];
                break;
            }
                
        }
    }
    
    if(_alertwarning == alertView)
    {
        
    }
#ifdef  PROCESSING_TAG
    PROCESSING_TAG(alertView.tag,buttonIndex);
#endif
    [alertView release];
    CGame::_this->ClearAllTouch();
    PineSocial::_has_message_box = false;
    //[self release];
}

-(void) iOS_CaptureScreenWithRect:(NSString*) filename withCrop:(CGRect) rectScale
{
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
    
    if(image!= nil)
    {
        float xx = rectScale.origin.x*glView.bounds.size.width;
        float yy = rectScale.origin.y*glView.bounds.size.height;
        float ww = rectScale.size.width*glView.bounds.size.width;
        float hh = rectScale.size.height*glView.bounds.size.height;
        
        image = [image crop:CGRectMake(xx, yy, ww, hh)];
        // Create path.
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *filePath = [[paths objectAtIndex:0] stringByAppendingPathComponent:filename];
        
        // Save image.
        [UIImagePNGRepresentation(image) writeToFile:filePath atomically:YES];
        
    }

}
-(void) iOS_BasicPostSocialWithFileInDocument:(NSString*) feed withURLDocument:(NSString*) withURLDocument
{
    NSArray  *documentPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDir  = [documentPaths objectAtIndex:0];
    NSString  *pngfile = [documentsDir stringByAppendingPathComponent:withURLDocument];

    if ([[NSFileManager defaultManager] fileExistsAtPath:pngfile]) {
        NSData *imageData = [NSData dataWithContentsOfFile:pngfile];
        UIImage *img = [UIImage imageWithData:imageData];
        [_pine_view_controller iOS_BasicPostSocial:feed withImage:img];
    }
}
-(void) iOS_BasicPostSocialWithRect:(NSString*) feed withCrop:(CGRect) rectScale
{
 
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
    
    if(image!= nil)
    {
        float xx = rectScale.origin.x*glView.bounds.size.width;
        float yy = rectScale.origin.y*glView.bounds.size.height;
        float ww = rectScale.size.width*glView.bounds.size.width;
        float hh = rectScale.size.height*glView.bounds.size.height;
    
        image = [image crop:CGRectMake(xx, yy, ww, hh)];
        
    }

}
-(void) iOS_BasicPostSocial:(NSString*) feed
{
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
    
#if !ADD_CHEAT
    CGame::_this->Sleep(100);
    [_pine_view_controller iOS_BasicPostSocial:feed withImage:image];
#endif
}

-(void) iOS_VideoPostSocial:(NSString*) feed
{
    [_pine_view_controller iOS_VideoPostSocial:feed withVideoURL:@"pine_video_capture.mp4"];
}

extern "C" void OS_VideoPostSocial(const char* feed)
{
    NSString* sfeed = [NSString stringWithFormat:@"%s %s https://itunes.apple.com/us/app/id%d ",feed,TAG_LINE,APPLE_ID];
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(iOS_VideoPostSocial:) withObject:sfeed waitUntilDone:YES];
}


-(void) iOS_BasicPostSocialNonImage:(NSString*) feed
{
#if !ADD_CHEAT
    CGame::_this->Sleep(100);
    [_pine_view_controller iOS_BasicPostSocial:feed withImage:nil];
#endif
}

-(void)getDeviceUserAgent
{
    NSString* userAgentString = nil;
    
    UIWebView* webView = [[UIWebView alloc]initWithFrame:CGRectZero];
    
    userAgentString = [[NSString alloc] initWithFormat:@"%@", [webView stringByEvaluatingJavaScriptFromString:@"navigator.userAgent"]];
    
    [webView release];
    
    GAME()->_device.setDeviceAgent([userAgentString UTF8String]);
    
    if (userAgentString)
    {
        [userAgentString release];
    }
}

extern "C" void OS_BasicPostSocialWithNonImage(const char* feed)
{
    PineAdBox::showAdState = SHOW_AD_PROCESSING;
    PineAdBox::typePostIOSBasic = PineAdBox::enumTypePostIOSBasic::TYPE_POST_BASIC_NONE;
#if DISABLE_AUTO_FORMAT_POST
     NSString* sfeed = [NSString stringWithFormat:@"%s",feed];
#else
    NSString* sfeed = [NSString stringWithFormat:@"%s %s https://itunes.apple.com/us/app/id%d ",feed,TAG_LINE,APPLE_ID];
#endif
    
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(iOS_BasicPostSocialNonImage:) withObject:sfeed waitUntilDone:YES];
}

extern "C" void OS_BasicPostSocialWithUTF8(u_int32_t* feed, int len)
{
    PineAdBox::showAdState = SHOW_AD_PROCESSING;
    PineAdBox::typePostIOSBasic = PineAdBox::enumTypePostIOSBasic::TYPE_POST_BASIC_NONE;
    NSData *data = [NSData dataWithBytes:feed length:len];
    NSString *string = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
#if DISABLE_AUTO_FORMAT_POST
    NSString* sfeed = [NSString stringWithFormat:@"%@",string];
#else
    NSString* sfeed = [NSString stringWithFormat:@"%@ %s https://itunes.apple.com/us/app/id%d ",string,TAG_LINE,APPLE_ID];
#endif
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(iOS_BasicPostSocial:) withObject:sfeed waitUntilDone:YES];

}
extern "C" void OS_ShareImageWithURLDocument(const char* feeds, const char* file)
{
    [[NSOperationQueue mainQueue] addOperationWithBlock:^ {
        
        //Your code goes in here
#if DISABLE_AUTO_FORMAT_POST
         NSString* sfeed = [NSString stringWithFormat:@"%s",feeds];
#else
        NSString* sfeed = [NSString stringWithFormat:@"%s %s https://itunes.apple.com/us/app/id%d ",feeds,TAG_LINE,APPLE_ID];
#endif
        NSString* sfile = [NSString stringWithFormat:@"%s",file];
        [[PineGameDelegate sharedInstance] iOS_BasicPostSocialWithFileInDocument:sfeed withURLDocument:sfile];
        NSLog(@"Main Thread Code");
        
    }];
    
}
extern "C" void OS_CaptureScreenWithCrop(const char* file,float x, float y, float w, float h)
{
    [[NSOperationQueue mainQueue] addOperationWithBlock:^ {
        
        //Your code goes in here
        NSString* sfeed = [NSString stringWithFormat:@"%s",file];
        [[PineGameDelegate sharedInstance] iOS_CaptureScreenWithRect:sfeed withCrop:CGRectMake(x, y, w, h)];
        NSLog(@"Main Thread Code");
        
    }];

}
extern "C" void OS_BasicPostSocialWithCrop(const char* feed,float x, float y, float w, float h)
{
    [[NSOperationQueue mainQueue] addOperationWithBlock:^ {
        
        //Your code goes in here
        
        PineAdBox::showAdState = SHOW_AD_PROCESSING;
        PineAdBox::typePostIOSBasic = PineAdBox::enumTypePostIOSBasic::TYPE_POST_BASIC_NONE;
#if DISABLE_AUTO_FORMAT_POST
        NSString* sfeed = [NSString stringWithFormat:@"%s",feed];
#else
        NSString* sfeed = [NSString stringWithFormat:@"%s %s https://itunes.apple.com/us/app/id%d ",feed,TAG_LINE,APPLE_ID];
#endif
        [[PineGameDelegate sharedInstance] iOS_BasicPostSocialWithRect:sfeed withCrop:CGRectMake(x, y, w, h)];
        NSLog(@"Main Thread Code");
        
    }];

    //-(void) iOS_BasicPostSocialWithRect:(NSString*) feed withCrop:(CGRect) rectScale
}

extern "C" void OS_BasicPostSocial(const char* feed)
{
   // OS_BasicPostSocialWithCrop(feed,0,0,0.5f,0.5f);

    PineAdBox::showAdState = SHOW_AD_PROCESSING;
    PineAdBox::typePostIOSBasic = PineAdBox::enumTypePostIOSBasic::TYPE_POST_BASIC_NONE;
#if DISABLE_AUTO_FORMAT_POST
    NSString* sfeed = [NSString stringWithFormat:@"%s",feed];
#else
    NSString* sfeed = [NSString stringWithFormat:@"%s %s https://itunes.apple.com/us/app/id%d ",feed,TAG_LINE,APPLE_ID];
#endif
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(iOS_BasicPostSocial:) withObject:sfeed waitUntilDone:YES];
    
}


extern "C" void ShowGCLeader()
{
	[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(GCLeaderBoard:) withObject:nil waitUntilDone:YES];
	
}

extern "C" void ShowGCAchievement()
{
	[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(GCAchievementBoard:) withObject:nil waitUntilDone:YES];
	
}

extern "C" void ShowRateDialog()
{
	[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(showPrompt) withObject:nil waitUntilDone:NO];
}
extern "C" void OF_LoadDashboard()
{
	[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(_OFLauchDashBoard:) withObject:nil waitUntilDone:NO];
    
	
}
extern "C" bool HasOpenFeintInit()
{
	return [PineGameDelegate sharedInstance]->_openfeint_init;
}

extern "C" void SetCurrentVersion(int ver)
{
    [PineGameDelegate sharedInstance]->_version_display = ver;
    
}

extern "C" int GetCurrentVersion()
{
    return  [PineGameDelegate sharedInstance]->_version_display;
}
extern "C" void CleanTouch()
{
	[[PineGameDelegate sharedInstance] CleanTouch];
}


//In App Purchase
extern "C" void InitInAppPurchase()
{
    printf("\nInit in app purchase");
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(InitInAppPurchase:) withObject:nil waitUntilDone:NO];
    
}

extern "C" void SetPurchase(int time)
{
    [[PineGameDelegate sharedInstance] SetPurchaseTimeOut:time];
}

extern "C" void UpdatePurchaseTimeOut(int time)
{
    [[PineGameDelegate sharedInstance] UpdatePurchaseTimeOut:time];
}

extern "C" void PurchaseProduct(const char *indentify)
{
    NSString* iden = [[NSString alloc] initWithUTF8String:indentify];
    [[PineGameDelegate sharedInstance] PurchaseProduct:iden];
    [iden release];
}


extern "C" void OnOff_Gamefeed(bool on)
{
    if(on)
    {
        //[[PineGameDelegate sharedInstance] setupGameFeedWithAdditionalSettings:nil];
        [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(setupGameFeedWithAdditionalSettings:) withObject:nil waitUntilDone:NO];
    }
    else
    {
        //[[PineGameDelegate sharedInstance] switchGameFeed];
        [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(switchGameFeed:) withObject:nil waitUntilDone:NO];
    }
}



extern "C" int  GetMotionAnchor()
{
    return  [PineGameDelegate sharedInstance]->_motion_anchor;
}

extern "C" void SetMotionAnchor(int anchor)
{
    /*
     [PineGameDelegate sharedInstance]->_motion_anchor = anchor;
     if(anchor == MOTION_LANDSCAPE_RIGHT_ANCHOR)
     {
     [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(setBarLandscapeRight:) withObject:nil waitUntilDone:NO];
     }
     else if(anchor == MOTION_LANDSCAPE_LEFT_ANCHOR)
     {
     
     [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(setBarLandscapeLeft:) withObject:nil waitUntilDone:NO];
     }
     */
}

extern "C" INT_TIME GetCurrentMilisecond()
{
	NSTimeInterval startTime = [NSDate timeIntervalSinceReferenceDate];
	return startTime;
}

extern "C" void DevicePushRemoteNotification(char* device_token,char *message)
{
    
    /*
    Payload *payload = new Payload();
    init_payload(payload);
    payload->message = message;
    payload->badgeNumber= 1;
    payload->actionKeyCaption = "0K";
    //payload->dictKey[0] = "Key1";
    //payload->dictValue[0]= "0984312005";
    
    //payload->dictKey[1] = "Key2";
    //payload->dictValue[1]= "0984312005";
    printf("\nSend to device %s",device_token);
    send_remote_notification(device_token,payload);
    */
}


extern "C" void OS_LoadImageToPngFile(const char* url,const char *file_name,float width_target,float height_target)
{
    NSArray* a = [[NSArray alloc] init];
    NSString* _url = [NSString stringWithFormat:@"%s",url];
    NSString* _file_name = [NSString stringWithFormat:@"%s",file_name];
    a = [a arrayByAddingObject:_url];
    a = [a arrayByAddingObject:_file_name];
    [[PineGameDelegate sharedInstance] Thread_LoadImageToPngFile:a width:width_target height:height_target];
    
}

//facebook
extern "C" void InitFaceBook()
{
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(InitFaceBook:) withObject:nil waitUntilDone:YES];
}

extern "C" void FBLoadMe()
{
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(fbAPILoadMe:) withObject:nil waitUntilDone:YES];
}

extern "C" int getImageFromFaceBookID(INT_TIME userID, INT_TIME fbID, int width, int height)
{
#if ADD_FACEBOOK
    if (!OS_IsNetworkAvailable())
    {
        return FACEBOOK_ERROR_NO_NETWORK;
    }
    int ret = FACEBOOK_DATA_SUCCESS;
    
    NSMutableURLRequest *request = [[NSMutableURLRequest alloc] init];
    [request setCachePolicy:NSURLRequestUseProtocolCachePolicy];
    [request setHTTPShouldHandleCookies:NO];
    [request setTimeoutInterval:5];
    [request setHTTPMethod:@"GET"];
    
    [request setURL:[NSURL URLWithString:[NSString stringWithFormat:@"http://graph.facebook.com/%lld/picture?width=%d&height=%d", fbID, width, height]]];
    
    NSURLResponse* response;
    // make the connection to the web
    NSData *returnData = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:nil];
    
    NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)response;
    int statusCode = [httpResponse statusCode];
    
    //printf("\n @@@@@@@@@@@@@@@@ statusCode = %d \n", statusCode);
    
    if (statusCode == 304 || statusCode == 200)
    {
        
    }
    else
    {
        [returnData release];
        [request release];
        return FACEBOOK_ERROR_NO_RECORD;
    }
    
    //printf("\n fb URL = %s ", GET_MAIN_CHARATER()._fb_avatar_url);
    
    UIImage *image = [[UIImage alloc] initWithData:returnData];
    
    //scale
    image = [[PineGameDelegate sharedInstance] scaleAndRotateImage:image width:width height:height];
    
    //save
    NSData * _imgdata = UIImagePNGRepresentation(image);
    
    //create path to save file
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [[paths objectAtIndex:0] autorelease];
    
    char avatarFile[25];
    sprintf(avatarFile, USER_AVATAR_FILE_NAME, userID);
    
    NSString* fname = [[[NSString alloc] initWithUTF8String: avatarFile] autorelease];
    NSString *appfilename = [[documentsDirectory stringByAppendingPathComponent:fname] autorelease];
    
    //save file to document
    [_imgdata writeToFile:appfilename  atomically:NO];
    [_imgdata release];
    
    [image release];
    
    [returnData release];
    [request release];
    [fname release];
    return ret;
#else
    return 0;
#endif
}

extern "C" void FBLoadListFriend(bool includeNotPlayGame)
{
    NSNumber * number = [[NSNumber alloc] initWithBool:includeNotPlayGame];
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(fbAPILoadFriend:) withObject:number waitUntilDone:YES];
}

extern "C" const char* OS_convertSStringTocharArray(NSString* str)
{
    return [str UTF8String];
}
extern "C" void OS_Popup_Mail(const char* body, const char* title)
{
    
    NSString* sbody = [NSString stringWithFormat:@"%s",body];
    NSString* stitle = [NSString stringWithFormat:@"%s",title];
    [[PineGameDelegate sharedInstance] PopupMail:sbody withTitle:stitle];
}

extern "C" void OS_Popup_Message(const char* body, const char* title)
{
    //[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(PopupMessages:) withObject:nil waitUntilDone:YES];
    NSString* sbody = [NSString stringWithFormat:@"%s",body];
    NSString* stitle = [NSString stringWithFormat:@"%s",title];
    [[PineGameDelegate sharedInstance] PopupMessages:sbody withTitle:stitle];
}

extern "C" void OS_OpenURL(const char* URL)
{
    NSString *_url = [[NSString alloc] initWithUTF8String:URL];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:_url]];
    [_url release];
    
}
extern "C" void OS_ShowRating(const char* title, const char* body)
{
    NSString* stitle = [NSString stringWithFormat:@"%s",title];
    NSString* sfeed = [NSString stringWithFormat:@"%s",body];
    [[PineGameDelegate sharedInstance] showRating:stitle body:sfeed];
    
}
/////////////
extern "C" void OS_SystemMessageAsk(const char* feed,int code)
{
    //
    NSString* sfeed = [NSString stringWithFormat:@"%s",feed];
    [[PineGameDelegate sharedInstance] showMessageConfirm:sfeed code:code];
    //[[PineGameDelegate sharedInstance] showMessageWarning:sfeed];
}
extern "C" void OS_SystemWarning(const char* feed, const char* title)
{
    NSString *feedString = [NSString stringWithUTF8String:feed];
    NSString *feedTitle = [NSString stringWithUTF8String:title];
    NSDictionary *stringArray = [[NSDictionary alloc] initWithObjectsAndKeys:
                                 feedString, @"Feed", feedTitle, @"Title", nil];
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(showMessageWarning:) withObject:stringArray waitUntilDone:YES];
    //[[PineGameDelegate sharedInstance] showMessageWarning:sfeed];
    //[feedString release];
    //[feedTitle release];
    [stringArray release];
}

extern "C" bool OS_IsDeviceHacked()
{
    //
    NSURL* url = [NSURL URLWithString:@"cydia://package/com.example.package"];
    if([[UIApplication sharedApplication] canOpenURL:url])
    {
        printf("\n detect cydian 1");
        //return true;
        return true;
    }
    if ([[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"cydia://"]])
    {
        //insert action if cydia is installed
        printf("\n detect cydian 2");
        return true;
    }
    
    NSError* error=nil;
    NSArray* files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:@"/System" error:&error];
    //NSLog(@"blah %i error %@",(int)[files count], error);
    
    if (error==nil) {
        //A non-jailbroken device will have an operation not permitted error.
        //Jailbroken device should have a list of files and a nil error.
        if (files) {
            NSLog(@"jailbreak? %i",(int)[files count]);
        }
        return true;
    }
    
    //can write to system.
    {
        FILE *f = fopen("/bin/bash", "r");
        if (f != NULL)
        {
            printf("\n can read to system");
            fclose(f);
            return true;
        }
        
    }
    return false;
}
extern "C" void OS_CaptureScreenshot()
{
    [[PineGameDelegate sharedInstance]     CaptureTheScreenShot];
}
extern "C" void OS_LoadIAP()
{
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(LoadIAP) withObject:nil waitUntilDone:YES];
    
}


extern "C" bool OS_PingToHost(const char* url,int time_out)
{
    NSString* str = [NSString stringWithUTF8String:url];
    
    NSURL *url_host = [[NSURL alloc] initWithString:str];
	NSURLRequest *lRequest = [NSURLRequest requestWithURL:url_host
                                              cachePolicy:NSURLRequestUseProtocolCachePolicy
                                          timeoutInterval:time_out];
    if (![NSURLConnection sendSynchronousRequest:lRequest returningResponse:nil error:nil])
    {
		return false;
    }
    return true;
}
extern "C" void	OS_RatingWithDialog(const char* title, const char* message)
{
    NSString* stitle = [NSString stringWithFormat:@"%s",title];
    NSString* sfeed = [NSString stringWithFormat:@"%s",message];
    [[PineGameDelegate sharedInstance] showRating:stitle body:sfeed];
}


extern "C" void OS_BrowserWebPage(const char* url) {
    NSString* strUrl = [ NSString stringWithFormat:@"%s",url];
    
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:strUrl]];
    
}

extern "C" bool OS_IsNetworkAvailable()
{
	// Create zero addy
	struct sockaddr_in zeroAddress;
	bzero(&zeroAddress, sizeof(zeroAddress));
	zeroAddress.sin_len = sizeof(zeroAddress);
	zeroAddress.sin_family = AF_INET;
	
	// Recover reachability flags
	//SCNetworkReachabilityRef defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&zeroAddress);
    SCNetworkReachabilityRef defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(kCFAllocatorDefault, (struct sockaddr *)&zeroAddress);
	SCNetworkReachabilityFlags flags;
	
	BOOL didRetrieveFlags = SCNetworkReachabilityGetFlags(defaultRouteReachability, &flags);
	CFRelease(defaultRouteReachability);
	
	if (!didRetrieveFlags)
	{
		return NO;
	}
	
	BOOL isReachable = flags & kSCNetworkFlagsReachable;
	BOOL needsConnection = flags & kSCNetworkFlagsConnectionRequired;
	
	return (isReachable && !needsConnection) ? YES : NO;
}

//In app Purchase
-(void) InitInAppPurchase:(NSObject*)params
{
    printf("\nload store");
    [AppPurchase loadStore];
}
-(void) SetPurchaseTimeOut:(int)time
{
    [AppPurchase SetTimeOut:time];
}
-(void) UpdatePurchaseTimeOut:(int)time
{
    [AppPurchase UpdateTimeOut:time];
}

-(void) PurchaseProduct:(NSString *)identify
{
    [AppPurchase PurchaseProduct:identify];
}

extern "C" void IapRestore()
{
    [[[PineGameDelegate sharedInstance] AppPurchase] RefeshStore];
}

- (void) setBarPortrait:(NSObject*)params
{
    
    [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait animated:NO];
    
}

- (void) setBarLandscapeLeft:(NSObject*)params
{
    
    [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeLeft animated:NO];
    
}

- (void) setBarLandscapeRight:(NSObject*)params
{
    
    
    [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight animated:NO];
    
}

extern "C" void OS_ClearTouchView()
{
    
    [[PineGameDelegate sharedInstance] CleanTouch];
    
}

#endif /* defined(__PineEntertainment__PineMacOSFunc__) */
#if ADD_GAMECENTER
#include "PineGameCenter.h"
#endif
#if ADD_IADVERT
#include "PineIAdvert.h"
#endif
#if ADD_CHARTBOOST
#include "PineChartBoost.h"
#endif
#if ADD_ADMOB
#include "PineAdMob.h"
#endif
#if USE_AUDIO_RECORDER
PineViewController* _pine_view_audio_recorder = nil;


-(void) iOS_BlowSetCurrentPass:(double) v
{
    [_pine_view_audio_recorder SetMicroAudioPass:v];
    //-(void) SetMicroAudioPass:(double)v
}
-(void) iOS_BlowDevice:(NSObject*)params
{
    if(_pine_view_audio_recorder == nil)
    {
        _pine_view_audio_recorder = [PineViewController alloc];
    }
    [_pine_view_audio_recorder readyToBlow];
    
}

extern "C" bool _os_device_micro_permission = false;

-(void) iOS_RequestMicroPermission
{
    AVAudioSession *session = [AVAudioSession sharedInstance];

    if ([session respondsToSelector:@selector(requestRecordPermission:)]) {
        [session performSelector:@selector(requestRecordPermission:) withObject:^(BOOL granted) {
            if (granted) {
                //granted
                _os_device_micro_permission = true;
            }
            else {
                //not granted
                _os_device_micro_permission = false;
            }
        }
         ];
    }else{
        //iOS6 seems to be missing this feature -> always available
        _os_device_micro_permission = true;
    }
    
   
}
-(bool) iOS_GetMicroPermission{
    bool rs = false;
    AVAudioSession *session = [AVAudioSession sharedInstance];
     // AZ DEBUG @@ iOS 7+
     AVAudioSessionRecordPermission sessionRecordPermission = [session recordPermission];
     switch (sessionRecordPermission) {
     case AVAudioSessionRecordPermissionUndetermined:
     NSLog(@"Mic permission indeterminate. Call method for indeterminate stuff.");
     break;
     case AVAudioSessionRecordPermissionDenied:
     NSLog(@"Mic permission denied. Call method for denied stuff.");
     break;
     case AVAudioSessionRecordPermissionGranted:
     NSLog(@"Mic permission granted.  Call method for granted stuff.");
     rs = true;
     break;
     default:
     break;
     }
     
    return rs;
}
-(double) iOS_GetMicroAudioPass
{
    return [_pine_view_audio_recorder GetMicroAudioPass];
}
-(void) iOS_StopBlowDevice
{
    [_pine_view_audio_recorder StopToBlow];
}
extern "C" void OS_StopBlowDevice()
{
    [[PineGameDelegate sharedInstance] iOS_StopBlowDevice];
}
extern "C" double OS_GetMicroAudioPass()
{
    return [[PineGameDelegate sharedInstance] iOS_GetMicroAudioPass];
}
extern "C" void OS_BeginListenBlowDevice()
{
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(iOS_BlowDevice:) withObject:nil waitUntilDone:YES];
}
extern "C" void OS_BlowSetCurrentPass(double v)
{
    [[PineGameDelegate sharedInstance] iOS_BlowSetCurrentPass:v];
}
extern "C" bool OS_GetMicroPermission()
{
    return [[PineGameDelegate sharedInstance] iOS_GetMicroPermission];
    
}
extern "C" void OS_RequestMicroPermission()
{
    [[PineGameDelegate sharedInstance] iOS_RequestMicroPermission];
}

#endif


#if ADD_UNITYADS


//////////////////////////
//// README
/////////////////////////



extern "C" void DownloadFile(const char* url, const char* file_name,int time_out);
extern "C" int GetSizeFile(const char *file_name);
extern "C" int ReadAppDataBinary(const char* filename, BYTE* readed_data);
//_pine_view_admob
#ifndef PineEntertainment_UnityAds
#define PineEntertainment_UnityAds
-(bool) UnityAdsAvailable
{
    return _unity_app_id != nil;
}
-(bool) UnityAds_HasAdToShow
{
    return [_pine_view_unityads UnityHasAdsToShow];
}
-(bool) UnityAds_HasVideoRewardToShow
{
    return [_pine_view_unityads UnityHasVideoAdsToShow];
}
-(void) ShowUnityAds
{
    [_pine_view_unityads UnityAdsShow];
}

-(void) ShowUnityVideoAds
{
    [_pine_view_unityads UnityAdsVideoRewardShow];
}

extern "C" bool HasUnityAdsToShow()
{
    return [[PineGameDelegate sharedInstance] UnityAds_HasAdToShow];
}
extern "C" bool HasUnityVideoAds()
{
    return [[PineGameDelegate sharedInstance] UnityAds_HasVideoRewardToShow];
}
extern "C" void ShowUnityAdsFullScreen()
{
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(ShowUnityAds) withObject:nil waitUntilDone:YES];
    //[[PineGameDelegate sharedInstance] ShowUnityAds];
    
}
extern "C" void ShowUnityVideoAds()
{
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(ShowUnityVideoAds) withObject:nil waitUntilDone:YES];
    //[[PineGameDelegate sharedInstance] ShowUnityVideoAds];
}

extern "C" bool OS_UnityAvailable()
{
    return  [[PineGameDelegate sharedInstance] UnityAdsAvailable];
}






#endif
#endif
#if ADD_SCREEN_CAPTUREVIEW
#include "PineCaptureViewController.h"
#endif

#if USE_PINEGAMEPLAYREPLAY
#ifndef DUPLICATION_USE_PINEGAMEPLAYREPLAY
#define DUPLICATION_USE_PINEGAMEPLAYREPLAY
-(PineViewController*) GetGameplayRecordView
{
    return _pine_view_record_controller;
}

-(void) shareRecordGamePlay
{
    [[[PineGameDelegate sharedInstance] GetGameplayRecordView] shareRecordGamePlay ];
}
extern "C" bool OS_SupportReplay()
{
    if((SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"9.0")))
    {
        
        return true;
    }
    return false;
}
extern "C" void OS_ShareRecordGamePlay()
{
    //Device::ReplayGamePlayState = RECORD_GAMEPLAY_SHARING;
    //[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(shareRecordGamePlay) withObject:nil waitUntilDone:YES];
    [[[PineGameDelegate sharedInstance] GetGameplayRecordView] shareRecordGamePlay ];
}
extern "C" void OS_StartGameplayRecord()
{
    [[[PineGameDelegate sharedInstance] GetGameplayRecordView] GamePlayRecordStart ];
}
extern "C" void OS_StopGameplayRecord()
{
    [[[PineGameDelegate sharedInstance] GetGameplayRecordView] GamePlayRecordStop ];
}
extern "C" void OS_ForceStopGameplayRecord()
{
    [[[PineGameDelegate sharedInstance] GetGameplayRecordView] GamePlayRecordForceStop ];
}

#endif
#endif

#if  NOTIFICATION_MAX
#ifndef NOTIFICATION_MAX_ONLY_ONE
#define NOTIFICATION_MAX_ONLY_ONE
extern "C" void OS_DeviceClearAllPushNotify()
{
    [UIApplication sharedApplication].applicationIconBadgeNumber = 0;
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
}
extern "C" void OS_DeviceAskPushNotify()
{
    if((SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"8.0")))
    {
        [[UIApplication sharedApplication]  registerUserNotificationSettings:[UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeAlert|UIUserNotificationTypeBadge|UIUserNotificationTypeSound categories:nil]];
    }
}
extern "C" void OS_DevicePushNotify()
{
   
    
    for(int i= 0; i < Device::_current_num_notification; i++)
    {
        //(60*60*12*(i+1))
        UILocalNotification *notification = [[UILocalNotification alloc] init];
        NSDate *notificationDate;
        long long time = Device::_notification_time[i];
#if NOTIFY_RANGE_NUM
        int range_hour[NOTIFY_RANGE_NUM] = NOTIFY_RANGE_VALUE;
        int current_hour = Device::GetHourOfDay();
        long long  target_hour = current_hour + (time/60/60);
        int mode_second = time%3600;
        bool hastime = false;
        
        for(int t = 0; t < NOTIFY_RANGE_NUM;)
        {
            int mod = target_hour%24;
            if(mod == range_hour[t])
            {
                hastime = true;
                break;
            }
            else if(mod < range_hour[t])
            {
                target_hour++;
            }
            else if(mod > range_hour[t])
            {
                t++;
            }
            
        }
        if(!hastime)
        {
            for(int t = 0; t < NOTIFY_RANGE_NUM; t++)
            {
                range_hour[t] = range_hour[t] + 24;
            }
            
            for(int t = 0; t < NOTIFY_RANGE_NUM;)
            {
                int mod = target_hour%48;
                printf("\nt = %d mod  =%d range_hour = %d",t,mod,range_hour[i]);
                if(mod == range_hour[t])
                {
                    hastime = true;
                    break;
                }
                else if(mod < range_hour[t])
                {
                    target_hour++;
                }
                else if(mod > range_hour[t])
                {
                    t++;
                }
                
            }
        }
       
        
        time = (target_hour-current_hour)*60*60 + mode_second;
        //printf("\ncurrent_hour = %ld taret hour = %ld second wait = %ld mode_second = %d, Device::_notification_string[i] = %s",current_hour,target_hour,time,mode_second,Device::_notification_string[i]);
#endif
        notificationDate = [NSDate dateWithTimeIntervalSinceNow:time];
        notification.fireDate  = notificationDate;
        notification.timeZone  = [NSTimeZone systemTimeZone];
        
        notification.alertBody = [[NSString alloc] initWithUTF8String:Device::_notification_string[i]];
        notification.soundName = nil;
        notification.applicationIconBadgeNumber = i+1;
        
        [[UIApplication sharedApplication] scheduleLocalNotification:notification];
    }
}
#endif
#endif






