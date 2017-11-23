
#undef WIN32
#import "SA_OAuthTwitterEngine.h"
#import "SA_OAuthTwitterController.h"


@interface TwitterConnection : UIViewController
{
	Boolean done;
	SA_OAuthTwitterEngine *_engine;
	NSMutableArray *tweets;
	NSString* _stringBuff;
}

+(TwitterConnection*)getSharedInstance;
+(void)setSharedInstance:(TwitterConnection*)instance;
- (void) showMediaPicker;
@end
