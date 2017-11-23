
#undef WIN32

#import <MediaPlayer/MediaPlayer.h>

@protocol MediaPickerDelegate; // forward declaration

@interface MediaPicker : UIViewController <MPMediaPickerControllerDelegate>
{
	Boolean done;
}


+(MediaPicker*)getSharedInstance;
+(void)setSharedInstance:(MediaPicker*)instance;
- (void) updatePlayerQueueWithMediaCollection: (MPMediaItemCollection *) mediaItemCollection;
- (void) showMediaPicker;
@end

@protocol MediaPickerDelegate

// implemented in MainViewController.m
- (void) mediaPickerDidFinish: (MediaPicker *) controller;


@end

@interface MediaPickerParams : NSObject
{
@public
	id textBuff;
	unsigned int buffSize;
	int x;
	int y;
	int w;
	int h;
}

@end
