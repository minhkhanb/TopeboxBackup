#import "oalPlayback.h"
#import "MyOpenALSupport.h"

#define SYSTEM_VERSION_EQUAL_TO(v)                  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedSame)
#define SYSTEM_VERSION_GREATER_THAN(v)              ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedDescending)
#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN(v)                 ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN_OR_EQUAL_TO(v)     ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedDescending)

@implementation oalPlayback

@synthesize _sources;
@synthesize _buffers;
@synthesize _sound_ids;
@synthesize isPlaying;
@synthesize wasInterrupted;
@synthesize iPodIsPlaying;
@synthesize audioIsAlreadyPlaying;
@synthesize _audioSession;
@synthesize _isInterupByPhoneCall;
@synthesize _paused_sounds;

static oalPlayback* g_oalPlaybackInstance = nil;

+ (oalPlayback*) getSharedInstance
{
	if (g_oalPlaybackInstance == nil)
	{
		g_oalPlaybackInstance = [[oalPlayback alloc] init]; // assignment not done here
	}
	
	
	return g_oalPlaybackInstance;
}

/*- (void) audioPlayerBeginInterruption: (AVAudioPlayer *) player
{
    NSLog(@"Audio Session Interruption case started. ");
    // fork to handling method here...
    // EG:[self handleInterruptionStarted];
    if (self.isPlaying)
    {
        [self stopMusic];
        self.isPlaying = true;
    }
    [self stopAllSound];
    self.wasInterrupted = true;
}

- (void) audioPlayerEndInterruption: (AVAudioPlayer *) player
{
    NSLog(@"Audio Session Interruption case ended. ");
    // fork to handling method here...
    // EG:[self handleInterruptionEnded];
    self.wasInterrupted = false;
    iPodIsPlaying = [self._audioSession isOtherAudioPlaying];
    if (self.isPlaying)
    {
        [self startMusic:_bg_music_player.numberOfLoops];
    }
 
}*/

- (void)tryPlayMusic
{
    if (self.wasInterrupted && self.isPlaying && _isInterupByPhoneCall)
    {
        self.wasInterrupted = false;
        
        self.iPodIsPlaying = [self._audioSession isOtherAudioPlaying];
        
        NSError *audioSessionError = nil;
        
        [self._audioSession setCategory:AVAudioSessionCategoryAmbient error:&audioSessionError];
        
        [self._audioSession setActive:YES error:&audioSessionError];
        
        // register for notifications
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(interruption:)
                                                     name:AVAudioSessionInterruptionNotification
                                                   object:nil];
        if((SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"8.0")))
        {
            [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(ortherAppPlayAudio:)
                                                     name:AVAudioSessionSilenceSecondaryAudioHintNotification
                                                       object:nil];
        }
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(routeChange:)
                                                     name:AVAudioSessionRouteChangeNotification
                                                   object:nil];
        
        if (!self.iPodIsPlaying)
        {
            printf("\n tryPlayMusic success with no orther audio");
            [_bg_music_player prepareToPlay];
            [_bg_music_player play];
        }
        
        _isInterupByPhoneCall = NO;
    }
}

- (void)interruption:(NSNotification*)notification
{
   
    // get the user info dictionary
    NSDictionary *interuptionDict = notification.userInfo;
    // get the AVAudioSessionInterruptionTypeKey enum from the dictionary
    NSInteger interuptionType = [[interuptionDict valueForKey:AVAudioSessionInterruptionTypeKey] integerValue];
    // decide what to do based on interruption type here...
    switch (interuptionType)
    {
        case AVAudioSessionInterruptionTypeBegan:{

            // fork to handling method here...
            // EG:[self handleInterruptionStarted];
            if (self.isPlaying)
            {
                [_bg_music_player pause];
            }
            self.wasInterrupted = true;
            _isInterupByPhoneCall = YES;
            
            alcMakeContextCurrent(NULL);
            //alcDestroyContext(context);
            //alcCloseDevice(device);
            
            NSError *audioSessionError = nil;
            [self._audioSession setActive:NO error:&audioSessionError];
            if (audioSessionError)
            {
                NSLog(@"ERROR - SoundManager: Unable to set the audio session state to NO.");
                
            }
            
        }break;
        case AVAudioSessionInterruptionTypeEnded:{

            // fork to handling method here...
            // EG:[self handleInterruptionEnded];
            self.wasInterrupted = false;
            
            
            

        }break;
            
        default:
            NSLog(@"Audio Session Interruption Notification case default.");
            break;
    }
}

- (void)routeChange:(NSNotification*)notification
{
    
    NSDictionary *interuptionDict = notification.userInfo;
    
    NSInteger routeChangeReason = [[interuptionDict valueForKey:AVAudioSessionRouteChangeReasonKey] integerValue];
    
    switch (routeChangeReason) {
        case AVAudioSessionRouteChangeReasonUnknown:
            NSLog(@"routeChangeReason : AVAudioSessionRouteChangeReasonUnknown");
            break;
            
        case AVAudioSessionRouteChangeReasonNewDeviceAvailable:
            // a headset was added or removed
            NSLog(@"routeChangeReason : AVAudioSessionRouteChangeReasonNewDeviceAvailable");
            break;
            
        case AVAudioSessionRouteChangeReasonOldDeviceUnavailable:
            // a headset was added or removed
            NSLog(@"routeChangeReason : AVAudioSessionRouteChangeReasonOldDeviceUnavailable");
            break;
            
        case AVAudioSessionRouteChangeReasonCategoryChange:
            // called at start - also when other audio wants to play
            NSLog(@"routeChangeReason : AVAudioSessionRouteChangeReasonCategoryChange");
            break;
            
        case AVAudioSessionRouteChangeReasonOverride:
            NSLog(@"routeChangeReason : AVAudioSessionRouteChangeReasonOverride");
            break;
            
        case AVAudioSessionRouteChangeReasonWakeFromSleep:
            NSLog(@"routeChangeReason : AVAudioSessionRouteChangeReasonWakeFromSleep");
            break;
            
        case AVAudioSessionRouteChangeReasonNoSuitableRouteForCategory:
            NSLog(@"routeChangeReason : AVAudioSessionRouteChangeReasonNoSuitableRouteForCategory");
            break;
            
        default:
            break;
    }
}

- (void)ortherAppPlayAudio:(NSNotification*)notification
{
    NSDictionary *interuptionDict = notification.userInfo;
    NSInteger stateOrtherAppPlaySound = [[interuptionDict valueForKey:AVAudioSessionSilenceSecondaryAudioHintTypeKey] integerValue];
    
    //1 orther play : 0 orther off
    
    if (stateOrtherAppPlaySound == 1)
    {
        // fork to handling method here...
        // EG:[self handleInterruptionStarted];
        if (self.isPlaying)
        {
            [self stopMusic];
            self.isPlaying = true;
        }
        [self stopAllSound];
        self.wasInterrupted = true;
        alcMakeContextCurrent(NULL);
    }
    else if (stateOrtherAppPlaySound == 0)
    {
        // fork to handling method here...
        // EG:[self handleInterruptionEnded];
        self.wasInterrupted = false;
        iPodIsPlaying = [self._audioSession isOtherAudioPlaying];
        if (self.isPlaying)
        {
            [self startMusic:_bg_music_player.numberOfLoops];
        }
        ALenum error;
        
        // Create a new OpenAL Device
        // Pass NULL to specify the system’s default output device
        /*device = alcOpenDevice(NULL);
        if (device != NULL)
        {
            // Create a new OpenAL Context
            // The new context will render to the OpenAL Device just created
            context = alcCreateContext(device, 0);
            if (context != NULL)
            {
                // Make the new context the Current OpenAL Context
                alcMakeContextCurrent(context);
            }
        }*/
        //device = alcOpenDevice(NULL);
        //context = alcCreateContext(device, 0);
        
        if (context != NULL)
        {
            // Make the new context the Current OpenAL Context
            alcMakeContextCurrent(context);

        }else{
            NSLog(@"reuse al context fail\n");
        }
        // clear any errors
        alGetError();
        
    }
    
}

- (id) init
{
	if (self = [super init]) 
	{
		// setup our audio session
        
        NSError *audioSessionError = nil;
        
        self._audioSession = [AVAudioSession sharedInstance];
        
        
        if ([self._audioSession isOtherAudioPlaying])
        {
            self.iPodIsPlaying = true;
        }
        else
        {
            self.iPodIsPlaying = false;
        }
        
        [self._audioSession setCategory:AVAudioSessionCategoryAmbient error:&audioSessionError];
        
        
        if (audioSessionError)
        {
            NSLog(@"Error setting category! %ld", (long)[audioSessionError code]);
        }
        
        [self._audioSession setActive:YES error:&audioSessionError];
        
        // register for notifications
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(interruption:)
                                                     name:AVAudioSessionInterruptionNotification
                                                   object:nil];
        if((SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"8.0")))
        {
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(ortherAppPlayAudio:)
                                                     name:AVAudioSessionSilenceSecondaryAudioHintNotification
                                                   object:nil];
        }
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(routeChange:)
                                                     name:AVAudioSessionRouteChangeNotification
                                                   object:nil];
        
        
        _bg_music_player = [AVAudioPlayer alloc];
        
        self._sources = [[[NSMutableArray alloc] init] autorelease];
        self._buffers = [[[NSMutableDictionary alloc] init] autorelease];
        self._sound_ids = [[[NSMutableDictionary alloc] init] autorelease];
        
        wasInterrupted = NO;
        isPlaying = NO;
        _isInterupByPhoneCall = NO;
     
        [self initOpenAL];
	}
	
	return self;
}

- (void) dealloc
{
	[super dealloc];

	[self releaseOpenAL];
	
	[_bg_music_url release];
    
    if(_bg_music_player != nil)
    {
        [_bg_music_player release];
    }
	
}

- (void) initOpenAL
{
	ALenum error;
	
	// Create a new OpenAL Device
	// Pass NULL to specify the system’s default output device
	device = alcOpenDevice(NULL);
	if (device != NULL)
	{
		// Create a new OpenAL Context
		// The new context will render to the OpenAL Device just created 
		context = alcCreateContext(device, 0);
		if (context != NULL)
		{
			// Make the new context the Current OpenAL Context
			alcMakeContextCurrent(context);
		}
	}
	// clear any errors
	alGetError();
	
	[self initSourceArray];
}

- (void) releaseOpenAL
{	
	// Delete the Sources
	for (NSNumber* sourceNumber in _sources) 
	{
		NSUInteger source = [sourceNumber unsignedIntegerValue];
		alDeleteSources(1, &source);
	}
	
	// Delete the Buffers
	for (NSNumber* bufferNumber in _buffers) 
	{
		NSUInteger buffer = [bufferNumber unsignedIntegerValue];
		alDeleteBuffers(1, &buffer);
	}
	
    //Release context
    alcDestroyContext(context);
    //Close device
    alcCloseDevice(device);
}

- (void) initSourceArray
{
	ALenum error = AL_NO_ERROR;
	alGetError(); // Clear the error
	
	for (int i = 0; i < _MAX_SOUND_SOURCE; i++)
	{
		NSUInteger source;
		
		// Create some OpenAL Source Objects
		alGenSources(1, &source);
		if (alGetError() != AL_NO_ERROR) 
		{
			NSLog(@"Error generating sources! %x\n", error);  
		}
		
		//alSourcef(source, AL_PITCH, 1.0f);
		//alSourcef(source, AL_GAIN, 1.0f);
		//alSourcef(source, AL_REFERENCE_DISTANCE, 50.0f);
		// Turn Looping OFF
		//alSourcei(source, AL_LOOPING, AL_FALSE);
		
		//not set buffer here
		
		if ((error = alGetError()) != AL_NO_ERROR) 
		{
			NSLog(@"Error attaching buffer to source: %x\n", error);
		}
		
		//add source to _sources array
		[_sources addObject:[NSNumber numberWithUnsignedInt: source]];
	}
}

- (void)loadMusic:(NSString*) filename
{
	@try
	{
		
        
		_bg_music_url = [[NSURL alloc] initFileURLWithPath: [[NSBundle mainBundle] pathForResource:filename ofType:@""]];
        NSData *_mp3data = [NSData dataWithContentsOfURL:_bg_music_url];
        //_bg_music_player = [_bg_music_player initWithContentsOfURL:_bg_music_url error:nil];
        
        NSError *error;
        if(_bg_music_player != nil) {
            [_bg_music_player release];
        }
        _bg_music_player = [[AVAudioPlayer alloc] initWithData:_mp3data error:&error];
        [_mp3data release];
        [_bg_music_player  setVolume:1.0f];
	}
	@catch(NSException* ex)
	{
        NSLog(@"\n loadMusic Error %@ \n", [ex debugDescription]);
	}
	
		
}
- (void)loadMusicWithData:(const char*) data:(int) data_len
{
    @try
	{
        NSData * music_data = [NSData dataWithBytes:data length:data_len];
        NSError *error;
        if(_bg_music_player != nil) {
            [_bg_music_player release];
        }
        _bg_music_player = [[AVAudioPlayer alloc] initWithData:music_data error:&error];
        //NSLog(@"abc %@",[error debugDescription]);
        [music_data release];
        [_bg_music_player  setVolume:1.0f];
    }
    @catch(NSException* ex)
	{
        NSLog(@"\n loadMusic Error %@ \n", [ex debugDescription]);
	}
}
- (void)setMusicVolume:(float) vol
{
	if (_bg_music_player && isPlaying)
	{
		[_bg_music_player  setVolume:vol];
        //printf("\nGet Volume %f",_bg_music_player.volume);
	}
}

- (void)startMusic:(NSInteger) numberOfLoops
{
    if ([self wasInterrupted])
    {
        return;
    }
	@try
	{
		if (_bg_music_player)
		{
			_bg_music_player.numberOfLoops = numberOfLoops;
			[_bg_music_player play];
			_bg_music_player.meteringEnabled = YES;
            
            isPlaying = YES;
            
            if (iPodIsPlaying)
            {
                [self stopMusic];
            }
            
            isPlaying = YES;
            
           
            
            //printf("\n volume = %f ", _bg_music_player.volume);
		}
	}
	@catch(NSException* ex)
	{
        NSLog(@"\n loadMusic Error %@ \n", [ex debugDescription]);
	}
	
}

- (void)stopMusic
{
	@try
	{
		if (_bg_music_player && isPlaying)
		{
			[_bg_music_player stop];
            isPlaying = NO;
		}
	}@catch(NSException* ex)
	{
	}
}

- (void)resumeMusic{
	@try
	{
		if (_bg_music_player)
		{
			[_bg_music_player play];
            isPlaying = YES;
		}
	}
	@catch(NSException* ex)
	{
        NSLog(@"\n resume music Error %@ \n", [ex debugDescription]);
	}
}

- (BOOL)isMusicPlaying
{
	if (_bg_music_player && isPlaying)
	{
		return _bg_music_player.playing;
	}
	
	return NO;
}

- (float)getPeakMusicPower: (int) channel
{
	float db;
	[ _bg_music_player updateMeters ];
	db = [ _bg_music_player peakPowerForChannel: channel ];
	return db;
}

- (float)getAverangeMusicPower: (int) channel
{
	float db;
	[ _bg_music_player updateMeters ];
	db = [ _bg_music_player averagePowerForChannel: channel ];
	return db;
}


- (void)loadSound:(NSString*)filename soundID:(NSInteger)sound_id
{
	ALenum  error = AL_NO_ERROR;
	ALvoid* data;
	ALuint buffer;
	
	// Create some OpenAL Buffer Objects
	alGenBuffers(1, &buffer);
	
	if ((error = alGetError()) != AL_NO_ERROR) 
	{
		NSLog(@"Error Generating Buffers: %x", error);
	}
			
	
	ALenum  format;
	ALsizei size;
	ALsizei freq;
	
	NSBundle* bundle = [NSBundle mainBundle];
	
	// get some audio data from a wave file
	CFURLRef fileURL = (CFURLRef)[[NSURL fileURLWithPath:[bundle pathForResource:filename ofType:@""]] retain];
    
	if (fileURL)
	{	
		data = GetOpenALAudioData(fileURL, &size, &format, &freq);
        
        //NSLog(@"loading sound: size:%d, format:%d, freq:%d %x\n", size, format, freq, error);
        char* tmp_test = (char*) data;
        /*for(int i = 0; i< 5; i++){
            NSLog(@"test :%d %c\n", i, *tmp_test++);
        }*/
		CFRelease(fileURL);
		
		/*if ((error = alGetError()) != AL_NO_ERROR)
		{
			NSLog(@"error loading sound: %x\n", error);
		}*/
		
		// use the static buffer data API
		alBufferDataStaticProc(buffer, format, data, size, freq);
		
		/*if ((error = alGetError()) != AL_NO_ERROR)
		{
			NSLog(@"error attaching audio to buffer: %x\n", error);
		}*/
		
		[_buffers setObject:[NSNumber numberWithUnsignedInt:buffer] forKey:[NSNumber numberWithUnsignedInt:sound_id]];
	}
	else
	{
		NSLog(@"Could not find file!\n");
	}
}

-(void)unloadSound:(NSInteger)sound_id{
    ALuint al_id = [_buffers objectForKey:[NSNumber numberWithUnsignedInt:sound_id]];
    
    alDeleteBuffers(1,&al_id);
    
    [_buffers removeObjectForKey:[NSNumber numberWithUnsignedInt:sound_id]];
}
-(void)endInterruption{
    NSError *audioSessionError = nil;
    [self._audioSession setActive:YES error:&audioSessionError];
    if (audioSessionError)
    {
        NSString *errorDesc = [NSString stringWithFormat:
                               NSLocalizedString(@"MyGreatApp cannot open the file. %@", @""),
                               [audioSessionError localizedDescription]];
        NSLog(@"ERROR - SoundManager: Unable to set the audio session state to YES. %@", errorDesc);
        
    }
    //device = alcOpenDevice(NULL);
    //context = alcCreateContext(device, 0);
    if (context != NULL)
    {
        // Make the new context the Current OpenAL Context
        alcMakeContextCurrent(context);
        
    }else{
        NSLog(@"reuse al context fail\n");
    }
    // clear any errors
    ALenum error = AL_NO_ERROR;
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        NSLog(@"Error attaching set context :%d:\n", error);
    }
    else
    {
        self.wasInterrupted = false;
    }

}


void* MyGetOpenALAudioDataWithBuffer(const char* buff,int buff_len, ALsizei *outDataSize, ALenum *outDataFormat, ALsizei *outSampleRate);
-(void)loadSoundWidthData:(const char*) buf:(int) buf_len:(NSInteger)sound_id
{
    ALenum  error = AL_NO_ERROR;
	ALvoid* data;
	ALuint buffer;
	
	// Create some OpenAL Buffer Objects
	alGenBuffers(1, &buffer);
	
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		NSLog(@"Error Generating Buffers: %x", error);
	}
    
	
	ALenum  format;
	ALsizei size;
	ALsizei freq;
	
	NSBundle* bundle = [NSBundle mainBundle];
	
	// get some audio data from a wave file
	data = MyGetOpenALAudioDataWithBuffer(buf,buf_len, &size, &format, &freq);
    //NSLog(@"loading sound: size:%d, format:%d, freq:%d %x\n", size, format, freq, error);
    char* tmp_test = (char*) data;
    /*for(int i = 0; i< 5; i++){
        NSLog(@"test :%d %d\n", i, *tmp_test++);
    }*/
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        NSLog(@"error loading sound: %x\n", error);
    }
    
    // use the static buffer data API
    alBufferDataStaticProc(buffer, format, data, size, freq);
    
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        NSLog(@"error attaching audio to buffer: %x\n", error);
    }
    
    [_buffers setObject:[NSNumber numberWithUnsignedInt:buffer] forKey:[NSNumber numberWithUnsignedInt:sound_id]];
	
}
- (void)stopSound:(NSInteger)sound_id
{
	ALenum  error = AL_NO_ERROR;
	NSNumber* sourceNumber = [_sound_ids objectForKey:[NSNumber numberWithUnsignedInt:sound_id]];
	NSUInteger source = [sourceNumber unsignedIntegerValue];
	alSourceStop(source);
	if ((error = alGetError()) != AL_NO_ERROR) 
	{
		//NSLog(@"error stopping source: %x\n", error);
	}
}
- (void)startSound: (NSInteger)sound_id
{
    if ([self wasInterrupted])
    {
        NSLog(@"sound was interrupted\n");
        return;
    }
    if(_paused_sounds != nil){
        [_paused_sounds removeAllObjects];
    }
	ALenum error;
	ALenum state;
	
	for (NSNumber* sourceNumber in _sources) 
	{
		NSUInteger source = [sourceNumber unsignedIntegerValue];
		
	    alGetSourcei(source, AL_SOURCE_STATE, &state);
	 
	    if (state != AL_PLAYING)
		{
			//get buffer
			NSNumber* bufferNumber = [_buffers objectForKey:[NSNumber numberWithUnsignedInt:sound_id]];
			
			if (bufferNumber != nil)
			{
				NSUInteger buffer = [bufferNumber unsignedIntegerValue];
				
				// Turn Looping OFF
				alSourcei(source, AL_LOOPING, AL_FALSE);
				
				// Set Source Reference Distance
				alSourcef(source, AL_REFERENCE_DISTANCE, 50.0f);
				
				// attach the buffer to the source
				alSourcei(source, AL_BUFFER, buffer);
				
				//play sound
				alSourcePlay(source);
				[_sound_ids setObject:[NSNumber numberWithUnsignedInt:source] forKey:[NSNumber numberWithUnsignedInt:sound_id]];
				if ((error = alGetError()) != AL_NO_ERROR)
				{
					NSLog(@"error starting source: %x\n", error);
                }else{
                }
				
				return;
            }else{
                NSLog(@"sound buffer nil\n");
            }
		}
	}
}
-(void)pauseAllSound
{
    if ([self wasInterrupted])
    {
        return;
    }
    ALenum error;
    ALenum state;
    _paused_sounds = nil;
    _paused_sounds = [NSMutableArray init];
    
    for (NSNumber* sourceNumber in _sources)
    {
        NSUInteger source = [sourceNumber unsignedIntegerValue];
        
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        
        if (state == AL_PLAYING)
        {
            [_paused_sounds addObject:source];
            alSourcePause(source);
        }
    }
}
-(void)resumeAllSound
{
    if(_paused_sounds != nil)
    {
        for(int i = 0; i< [_paused_sounds count]; i++)
        {
            alSourcePlay([_paused_sounds objectAtIndex:i]);
        }
        [_paused_sounds removeAllObjects];
    }
}
- (void)stopAllSound
{
	ALenum error;
	
	for (NSNumber* sourceNumber in _sources) 
	{
		NSUInteger source = [sourceNumber unsignedIntegerValue];
		
		// Stop playing
		alSourceStop(source);
		if ((error = alGetError()) != AL_NO_ERROR) 
		{
			//NSLog(@"error stopping source: %x\n", error);
		}
	}
}

- (void)setSourcePos:(NSUInteger)source x:(NSInteger)xx y:(NSInteger)yy distance:(NSInteger)dd
{
	// Position of the source sound.
	ALfloat SourcePos[] = { xx, dd, yy };

	// Move our audio source coordinates
	alSourcefv(source, AL_POSITION, SourcePos);
}

- (void)setListenerPos
{
	// Position of the Listener.
	ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };

	// Velocity of the Listener.
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };

	// Orientation of the Listener. (first 3 elements are "at", second 3 are "up")
	// Also note that these should be units of '1'.
	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

	alListenerfv(AL_POSITION,    ListenerPos);
	alListenerfv(AL_VELOCITY,    ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
}

@end

void OS_LoadMusic(const char* filename)
{
	NSString* fname = [[NSString alloc] initWithUTF8String: filename];
	[[oalPlayback getSharedInstance] loadMusic: fname];
    [fname release];
}

void OS_PlayCurrentMusic(int num_loop)
{
	[[oalPlayback getSharedInstance] startMusic: num_loop];
}

void OS_StopCurrentMusic()
{
	[[oalPlayback getSharedInstance] stopMusic];
}
void OS_PauseCurrentMusic()
{
	[[oalPlayback getSharedInstance] stopMusic];
}

bool OS_IsCurrentMusicPlaying()
{
    if([oalPlayback getSharedInstance] == nil)
    {
        return false;
    }
	return [[oalPlayback getSharedInstance] isMusicPlaying];
}

void OS_setMusicVolume(float vol)
{
    [[oalPlayback getSharedInstance] setMusicVolume:vol];
}

float OS_getPeakMusicPower(int channel)
{
	return [[oalPlayback getSharedInstance] getPeakMusicPower: channel];
}

float OS_getAverangeMusicPower(int channel)
{
	return [[oalPlayback getSharedInstance] getAverangeMusicPower: channel];
}

void OS_LoadSFX(const char* filename, int sound_id)
{
	NSString* fname = [[NSString alloc] initWithUTF8String: filename];
	
	[[oalPlayback getSharedInstance] loadSound:fname soundID:sound_id];
    [fname release];
}

void OS_PlaySFX(int sound_id)
{
	[[oalPlayback getSharedInstance] startSound:sound_id];
}
void OS_StopSFX(int sound_id)
{
	[[oalPlayback getSharedInstance] stopSound:sound_id];
}
void OS_StopAllSFX()
{
	[[oalPlayback getSharedInstance] stopAllSound];
}
void OS_TryPlayMusic()
{
    [[oalPlayback getSharedInstance] tryPlayMusic];
}
