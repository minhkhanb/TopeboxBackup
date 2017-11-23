#import <AVFoundation/AVFoundation.h>
#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/ExtendedAudioFile.h>


#define	_MAX_SOUND_SOURCE	10
#define kDefaultDistance	25.0

@interface oalPlayback : NSObject <AVAudioSessionDelegate, AVAudioPlayerDelegate>
{
	ALCcontext*				context;
	ALCdevice*				device;
	
	NSMutableArray* 		_sources;
	NSMutableDictionary* 	_buffers;
	NSMutableDictionary*	_sound_ids;

	ALfloat					sourceVolume;
	BOOL					isPlaying;
	BOOL					wasInterrupted;
	
	NSURL*					_bg_music_url;
	AVAudioPlayer*			_bg_music_player;
	
	BOOL					iPodIsPlaying;
    UInt32                  audioIsAlreadyPlaying;
    AVAudioSession*         _audioSession;
    BOOL                    _isInterupByPhoneCall;
}

//@property (nonatomic, assign)	NSMutableArray* 		_sources;
//@property (nonatomic, assign)	NSMutableDictionary* 	_buffers;
@property (nonatomic, retain)    NSMutableArray*		_sources;
@property (nonatomic, retain)	NSMutableDictionary* 	_buffers;
@property (nonatomic, retain)	NSMutableDictionary* 	_sound_ids;
@property (nonatomic, retain)	AVAudioSession* 	_audioSession;
@property (nonatomic, assign)	BOOL		isPlaying;			// Whether the sound is playing or stopped
@property (nonatomic, assign)	BOOL		iPodIsPlaying;		// Whether the iPod is playing
@property (nonatomic, assign)	BOOL		wasInterrupted;		// Whether playback was interrupted by the system
@property (nonatomic, assign)	UInt32		audioIsAlreadyPlaying;		
@property (nonatomic, assign)	BOOL		_isInterupByPhoneCall;
@property (nonatomic, assign) NSMutableArray* _paused_sounds;
//- (void)checkForMusic;

- (void) initOpenAL;
- (void) releaseOpenAL;

- (void)loadMusic:(NSString*) filename;
- (void)loadMusicWithData:(const char*) buf:(int) buf_len;
- (void)setMusicVolume:(float) vol;
- (void)startMusic:(NSInteger) numberOfLoops;
- (void)stopMusic;
- (void)resumeMusic;
- (BOOL)isMusicPlaying;
- (void)tryPlayMusic;

- (float)getPeakMusicPower: (int) channel;
- (float)getAverangeMusicPower: (int) channel; 

- (void)loadSound:(NSString*)filename soundID:(NSInteger)sound_id;
-(void)loadSoundWidthData:(const char*) data:(int) data_len :(NSInteger)sound_id;
- (void)stopSound:(NSInteger)sound_id;
- (void)startSound:(NSInteger)sound_id;
- (void)stopAllSound;
-(void)pauseAllSound;
-(void)resumeAllSound;
-(void)unloadSound:(NSInteger)sound_id;
-(void)endInterruption;

+ (oalPlayback*) getSharedInstance; // access to our instance

@end
