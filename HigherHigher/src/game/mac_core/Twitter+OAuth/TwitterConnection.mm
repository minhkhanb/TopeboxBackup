
#include <string.h>
#import "PineGameDelegate.h"
#import "TwitterConnection.h"

extern "C" void PublishTWStream(const char* msg);

static TwitterConnection* _sharedInstance = nil;

@implementation TwitterConnection


- (BOOL)isDone
{
	return done;
}

-(int)isAuthor
{
    
   // NSLog(@"\n %d \n", [[_engine isCancel] intValue]);
    return [[_engine isCancel] intValue];
}

+(TwitterConnection*)getSharedInstance
{
	return _sharedInstance;
}

+(void)setSharedInstance:(TwitterConnection*)instance
{
	if (_sharedInstance != nil && _sharedInstance != instance)
		[_sharedInstance release];
	
	_sharedInstance = instance;
}


- (void)dealloc
{
	[super dealloc];
}

- (void)showTWConnectDialog
{
    if(_engine) return;
	done = false;
       
    UIWindow *window = [[PineGameDelegate sharedInstance] window];
    [window addSubview:self.view];
    [window bringSubviewToFront:self.view];

    //[self setView:[[PineGameDelegate sharedInstance] window]];
	//[self setView:(UIView*)[[PineGameDelegate sharedInstance] glView]];
	//[self setModalTransitionStyle: UIModalTransitionStyleCoverVertical];
    
	_engine = [[SA_OAuthTwitterEngine alloc] initOAuthWithDelegate:self];
	_engine.consumerKey = @"Q7ojKiYKWnXRRIi21BS6ZA";
	_engine.consumerSecret = @"wkgC7aahf62h12ZJln6DFQAL2sRnaXF41PZTSmBiRs";
    _engine.isCancel = [NSNumber numberWithInt:0];
    //if(_engine){ 
    if(![_engine isAuthorized]){ 
        UIViewController *controller = [SA_OAuthTwitterController controllerToEnterCredentialsWithTwitterEngine:_engine delegate:self];
     
        if (controller){
            _engine.isCancel = [NSNumber numberWithInt:1];
            //[self setView:(UIView*)[[PineGameDelegate sharedInstance] glView]];
            [self presentModalViewController: controller animated: YES];
        }
	}
    else {
        _engine.isCancel = [NSNumber numberWithInt:2];
    }
    //NSLog(@"\n _engine.isCancel = %d\n", _engine.isCancel);
    //else {
         /*MGTwitterEngine* twitter = [[MGTwitterEngine alloc] initWithDelegate:self];
        [twitter set]
         [twitter setUsername:@"ginhamac" password:@"ginhatwitter"];
         [twitter setClientName: @"You Army" version:@"1" URL:@"http://google.com" token:@""];
         [twitter sendUpdate: @"Hello world!"];
         [twitter release];*/
        
    //}
    //[_engine sendUpdate:@"You Armmy Hello World"];
}




#pragma mark SA_OAuthTwitterEngineDelegate  
- (void) storeCachedTwitterOAuthData: (NSString *) data forUsername: (NSString *) username {  
    NSUserDefaults          *defaults = [NSUserDefaults standardUserDefaults];  
    
    [defaults setObject: data forKey: @"authData"];  
    [defaults synchronize];  
}  

- (NSString *) cachedTwitterOAuthDataForUsername: (NSString *) username {  
    return [[NSUserDefaults standardUserDefaults] objectForKey: @"authData"];  
}  

//=============================================================================================================================  
#pragma mark TwitterEngineDelegate  
- (void) requestSucceeded: (NSString *) requestIdentifier {  
    NSLog(@"Request %@ succeeded", requestIdentifier);  
}  

- (void) requestFailed: (NSString *) requestIdentifier withError: (NSError *) error {  
    NSLog(@"Request %@ failed with error: %@", requestIdentifier, error);  
}  


- (void) setTWStringBuffer: (NSString*) message
{
	_stringBuff = message;
}

- (void) publishTWStream: (NSString*) message
{
	[_engine sendUpdate: message];
}

@end


extern "C" void ShowTWConnectDialog()
{
	//[[[CBoxAppAppDelegate sharedInstance] glView] showTWConnectDialog];
	[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(TWLoginDialog:) withObject:nil waitUntilDone:YES];
	
}

extern "C" void PublishTWStream(const char* msg)
{	
    NSObject *object = [[NSString alloc] initWithUTF8String:(msg)];
	[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(TWPublishStream:) withObject:object waitUntilDone:YES];
}

extern "C" int TWLoginIsDone()
{
    
    return [[TwitterConnection getSharedInstance] isAuthor];
	//return [[TwitterConnection getSharedInstance] isDone];
}

