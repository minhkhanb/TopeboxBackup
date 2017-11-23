
#include <string.h>
#import "CBoxAppAppDelegate.h"
#import "TwitterConnection.h"



static TwitterConnection* _sharedInstance = nil;

@implementation TwitterConnection


- (BOOL)isDone
{
	return done;
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

- (void) showTWConnectDialog
{
	if(_engine) return;
	
	done = false;
	[self setView:[[CBoxAppAppDelegate sharedInstance] glView]];
	[self setModalTransitionStyle: UIModalTransitionStyleCoverVertical];
	_engine = [[SA_OAuthTwitterEngine alloc] initOAuthWithDelegate:self];
	_engine.consumerKey = @"npmdto2dZUiOYLfXyQt4fQ";
	_engine.consumerSecret = @"da3d26X87q5ul09NZVgkny2ZhxWpk2BkHY0pAxUc";
	
	UIViewController *controller = [SA_OAuthTwitterController controllerToEnterCredentialsWithTwitterEngine: _engine delegate: self];
	//controller.delegate = self;
	[controller setModalTransitionStyle: UIModalTransitionStyleCoverVertical];
	if (controller)
		[self presentModalViewController: controller animated: YES];
	else {
		tweets = [[NSMutableArray alloc] init];
		done = true;
	}
}




#pragma mark SA_OAuthTwitterEngineDelegate

- (void) storeCachedTwitterOAuthData: (NSString *) data forUsername: (NSString *) username {
	
	NSUserDefaults	*defaults = [NSUserDefaults standardUserDefaults];
	
	[defaults setObject: data forKey: @"authData"];
	[defaults synchronize];
}

- (NSString *) cachedTwitterOAuthDataForUsername: (NSString *) username {
	
	return [[NSUserDefaults standardUserDefaults] objectForKey: @"authData"];
}



#pragma mark SA_OAuthTwitterController Delegate

- (void) OAuthTwitterController: (SA_OAuthTwitterController *) controller authenticatedWithUsername: (NSString *) username {
	
	NSLog(@"Authenticated with user %@", username);
	
	tweets = [[NSMutableArray alloc] init];
	done = true;
	//[self updateStream:nil];
}

- (void) OAuthTwitterControllerFailed: (SA_OAuthTwitterController *) controller {
	
	NSLog(@"Authentication Failure");
}

- (void) OAuthTwitterControllerCanceled: (SA_OAuthTwitterController *) controller {
	
	NSLog(@"Authentication Canceled");
}



- (void) setTWStringBuffer: (NSString*) message
{
	_stringBuff = message;
}

- (void) publishTWStream
{
	printf("publishTWStream\n");
	[_engine sendUpdate: _stringBuff];
	//printf("end publish\n");
}

@end


extern "C" void ShowTWConnectDialog()
{
	//[[[CBoxAppAppDelegate sharedInstance] glView] showTWConnectDialog];
	[[CBoxAppAppDelegate sharedInstance] performSelectorOnMainThread:@selector(TWLoginDialog:) withObject:nil waitUntilDone:YES];
	
}

extern "C" void PublishTWStream(const char* msg)
{	
	NSString* message = [[NSString alloc] initWithUTF8String: msg];
	[[TwitterConnection getSharedInstance] setTWStringBuffer: message];
	[[CBoxAppAppDelegate sharedInstance] performSelectorOnMainThread:@selector(TWPublishStream:) withObject:nil waitUntilDone:YES];
}

extern "C" bool TWLoginIsDone()
{
	return [[TwitterConnection getSharedInstance] isDone];
}

