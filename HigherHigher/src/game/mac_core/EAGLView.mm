#import "EAGLView.h"
#import <QuartzCore/QuartzCore.h>
//#import "WindowsCredential.h"
//#import "ACSCredential.h"
//#import "ACSUtil.h"
#import "CGame.h"
#include "pch.h"

extern "C" Device CurrentDevice;

EAGLContext* g_context = nil;
GLuint g_frame_buffer = 0;
GLuint g_render_buffer = 0;
EAGLView *g_EAGLViewInstance = nil;
int g_shaderProgram = 0;
#if OPENGLES_20_SUPPORT
GLES20Lib _gles20;
extern "C" GLES20Lib* GetGLES20Lib()
{
    return &_gles20;
}
#endif

GLuint _test_texture = -1;
extern "C" void SetCurrentVersion(int ver);
extern "C" void UpdateTouchBegan(unsigned char touchId, int x, int y);
extern "C" void UpdateTouchEnded(unsigned char touchId, int x, int y);
extern "C" void UpdateTouchMoved(unsigned char touchId, int x, int y);
extern "C" void UpdateTouchCancel(unsigned char touchId, int x, int y);
extern "C" float view_pixel_scalex;
extern "C" float view_pixel_scaley;
//set context
extern "C" void EAGLViewBeginRender()
{
    // Make sure that you are drawing to the current context
    @try {
        
        [EAGLContext setCurrentContext:g_context];
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, g_render_buffer);
        //glBindFramebufferOES(GL_FRAMEBUFFER_OES, g_frame_buffer);
    }
    @catch (NSException *exception) {
        
    }
    @finally {
    }
    
}

//swap buffer
extern "C" void forceRender()
{
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, g_render_buffer);
    [g_context presentRenderbuffer:GL_RENDERBUFFER_OES];
}
extern "C" void EAGLViewEndRender()
{
    
    
    [g_context presentRenderbuffer:GL_RENDERBUFFER_OES];
    
}
extern "C" void ClearEAGLViewRender()
{
    glFinish();
}


@implementation EAGLView

// You must implement this
+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (void) ClearTouch
{
    for (int i = 0; i < MAX_TOUCHES - 1; i++)
    {
        _current_touches[i] = nil;
    }
    _num_touches = 0;
}

- (id) initGL:(CGRect)frame
{
    self = [super initWithFrame:frame];
    
    return self;
}


- (id) initWithFrame:(CGRect)frame
{
    //[self retrieveProducts];
    _is_init_complete = false;
    g_EAGLViewInstance = self;
#if OPENGLES_20_SUPPORT
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    
    eaglLayer.opaque = YES;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
    
    // Make sure this is the right version!
    g_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    if (!g_context || ![EAGLContext setCurrentContext:g_context] ||![self CreateFramebuffer]) {
        [self release];
        return nil;
    }
    _gles20.InitGLES20();
#else
    // convert base layer
    CAEAGLLayer *eagl_layer = (CAEAGLLayer*)self.layer;
    
    eagl_layer.opaque = YES;//NEW
    
    //eagl_layer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys: [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil]; //NEW
    eagl_layer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGB565, kEAGLDrawablePropertyColorFormat, nil];
    
    // create opengl context
    g_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    
    if(!g_context || ![EAGLContext setCurrentContext:g_context] || ![self CreateFramebuffer])
    {
        [self release];
        return nil;
    }
#endif
    
    //[self createCameraController];
    
    //g_context = _context;
    
    for (int i = 0; i < MAX_TOUCHES; i++)
    {
        _current_touches[i] = nil;
        _touches_id[i] = -1;
    }
    _num_touches = 0;
    
    _touch_id_gen = 0;
    _is_init_complete = true;
    return self;
}

- (void)changeConfigForCamera:(BOOL)open
{
    
    CAEAGLLayer *eagl_layer = (CAEAGLLayer*)self.layer;
    //printf("\n open = %d ", (bool)open);
    if (open == TRUE)
    {
        
        eagl_layer.opaque = NO;//NEW
        //eagl_layer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys: [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        
    }
    else
    {
        eagl_layer.opaque = YES;//NEW
        //eagl_layer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGB565, kEAGLDrawablePropertyColorFormat, nil];
    }
    //g_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    //[EAGLContext setCurrentContext:g_context];
    //[self CreateFramebuffer];
}

- (void) createCameraController
{
    //UIImage *image = [UIImage imageNamed:@"hud_castleview_BG_200.png"] ;
    //UIImageView *imgView = [[UIImageView alloc] initWithImage:image] ;
    //imgView.bounds = CGRectMake(100,100, 128, 128) ;
    
    UIImagePickerController* imagePicker = [[UIImagePickerController alloc] init];
    imagePicker.delegate = self;
    imagePicker.navigationBarHidden = YES;
    imagePicker.toolbarHidden = NO;
    imagePicker.sourceType = UIImagePickerControllerSourceTypeCamera;
    imagePicker.showsCameraControls = YES;
    imagePicker.cameraOverlayView = self;
    
    //[self addSubview:imgView];
    /*CGAffineTransform pickerTransform = CGAffineTransformMakeScale(1.0, 1.2);
     imagePicker.cameraViewTransform = pickerTransform;*/
    
    m_viewController = [[UIViewController alloc] init];
    m_viewController.view = self;
    [m_viewController presentModalViewController:imagePicker animated:NO];
}
- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    if (toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft ||
        toInterfaceOrientation == UIInterfaceOrientationLandscapeRight)
    {
        NSLog(@"Change to custom UI for landscape");
    }
    else if (toInterfaceOrientation == UIInterfaceOrientationPortrait ||
             toInterfaceOrientation == UIInterfaceOrientationPortraitUpsideDown)
    {
        NSLog(@"Change to custom UI for portrait");
        
    }
}
#ifndef IOS8_MASTER_W
#define IOS8_MASTER_W (640.0f)
#define IOS8_MASTER_H (1136.0f)
#endif
- (BOOL) CreateFramebuffer
{
    float w = 640;
    float h = 960;
    float device_w = 640;
    float device_h = 960;
    float ver = [[[UIDevice currentDevice] systemVersion] floatValue];
    
    // You can't detect screen resolutions in pre 3.2 devices, but they are all 320x480
    if (ver >= 3.2f)
    {
        UIScreen* mainscr = [UIScreen mainScreen];
        w = mainscr.currentMode.size.width;
        h = mainscr.currentMode.size.height;
        device_w = w;
        device_h = h;
        printf("\ndevice test %f, %f",w,h);
        if((w == 1125.0f && h == 2001.0f) ||
           (w == 2001.0f && h == 1125.0f) ||
           (w == 640.0f && h == 1136.0f) || (h == 640.0f && w == 1136.0f) || (w == 750.0f && h == 1334.0f) || (h == 750.0f && w == 1334.0f) || (w == 1242.0f && h == 2208.0f) || (h == 1242.0f && w == 2208.0f))
        {
            self.contentScaleFactor = 2.0f;
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
            printf("device w:%0.2f h:%0.2f\n", w, h);
            view_pixel_scalex = 2*(float)IOS8_MASTER_W/w;
            view_pixel_scaley = 2*(float)IOS8_MASTER_H/h;
            w = IOS8_MASTER_W;
            h = IOS8_MASTER_H;
            if(SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"8")){
                view_pixel_scalex  = view_pixel_scaley = 2.0f;
            }
            printf("set pxel scale x:%0.2f y:%0.2f\n", view_pixel_scalex, view_pixel_scaley);
#else
            view_pixel_scalex  = view_pixel_scaley = 2.0f;
            printf("set pxel scale 2.0f\n");
#endif
            
        }else
        //printf("\ndevice test %f, %f",w,h);
        if((w == 640.0f && h == 960.0f)  ||(w == 640.0f && h == 1136.0f) ||
           (w == 960.0f && h == 640.0f) || (h == 640.0f && w == 1136.0f) ||
           (w == 750.0 && h == 1334.0f)  ||(w == 1334.0f && h == 750.0f)
           
           ){
            self.contentScaleFactor = 2.0f;
            view_pixel_scalex = view_pixel_scaley = 2.0f;
            
        }
        else if (
                 (w == 2208.0f && h == 1242.0f) ||(h == 2208.0f && w == 1242.0f)
                 )
        {
            self.contentScaleFactor = 3.0f;
            view_pixel_scalex =view_pixel_scaley = 3.0f;
            
        }

        if((w == 2048.0f && h == 1536.0f) ||(h == 2048.0f && w == 1536.0f))
        {
#if !USE_IPAD_RETINA
            w = w/2;
            h = h/2;
#else
            self.contentScaleFactor = 2.0f;
            view_pixel_scalex = view_pixel_scaley = 2.0f;

#endif
            
        }
    }
    
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
    if (UIInterfaceOrientationIsLandscape([[UIApplication sharedApplication] statusBarOrientation]))
    {
        CurrentDevice.ScreenSize.W = w>h?w:h;
        CurrentDevice.ScreenSize.H = w<h?w:h;
        CurrentDevice.DeviceScreenSize.W = device_w>device_h?device_w:device_h;
        CurrentDevice.DeviceScreenSize.H = device_w<device_h?device_w:device_h;
    }
    else
    {
        /*CurrentDevice.ScreenSize.W = w;
        CurrentDevice.ScreenSize.H = h;
        CurrentDevice.DeviceScreenSize.W = device_w;
        CurrentDevice.DeviceScreenSize.H = device_h;*/
        
        /*CurrentDevice.ScreenSize.W = device_w<device_h?device_w:device_h;
        CurrentDevice.ScreenSize.H = device_w>device_h?device_w:device_h;
        CurrentDevice.DeviceScreenSize.W = w<h?w:h;
        CurrentDevice.DeviceScreenSize.H = w>h?w:h;*/
        
        CurrentDevice.ScreenSize.W = w<h?w:h;
        CurrentDevice.ScreenSize.H = w>h?w:h;
        CurrentDevice.DeviceScreenSize.W = device_w<device_h?device_w:device_h;
        CurrentDevice.DeviceScreenSize.H = device_w>device_h?device_w:device_h;
        printf("\n screen w = %f; h = %f, device w = %f, device h = %f ", CurrentDevice.ScreenSize.W, CurrentDevice.ScreenSize.H, CurrentDevice.DeviceScreenSize.W, CurrentDevice.DeviceScreenSize.H);
    }
#else
    UIDevice *myDevice = [UIDevice currentDevice] ;
    [myDevice beginGeneratingDeviceOrientationNotifications];
    UIDeviceOrientation deviceOrientation = myDevice.orientation;
    BOOL isCurrentlyLandscapeView = UIDeviceOrientationIsLandscape(deviceOrientation);
    [myDevice endGeneratingDeviceOrientationNotifications];
    
    
    CurrentDevice.ScreenSize.W = isCurrentlyLandscapeView?(w>h?w:h):(w>h?h:w);
    CurrentDevice.ScreenSize.H = CurrentDevice.ScreenSize.W == w?h:w;
    CurrentDevice.DeviceScreenSize.W = device_w;
    CurrentDevice.DeviceScreenSize.H = device_h;
#endif
    _frame_buffer = 0;
    _render_buffer = 0;
#if USE_DEPTH_BUFFER
    _depth_buffer = 0;
#endif
    //back up old
    GLuint old_render_buffer;
    GLuint old_frame_buffer;
    glGetIntegerv(GL_RENDERBUFFER_BINDING_OES, (GLint *) &old_render_buffer);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, (GLint *) &old_frame_buffer);
    
    //create framebuffer
    glGenFramebuffersOES(1, &_frame_buffer);
    //bind to framebuffer
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, _frame_buffer);
    
    // create render buffer
    glGenRenderbuffersOES(1, &_render_buffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, _render_buffer);
    [g_context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(id<EAGLDrawable>)self.layer];
    //bind to renderbuffer
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, _render_buffer);
    
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &_backbuffer_width);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &_backbuffer_height);
    
#ifdef USE_DEPTH_BUFFER
    //create depth buffer
    glGenRenderbuffersOES(1, &_depth_buffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, _depth_buffer);
    glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, _backbuffer_width, _backbuffer_height);
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, _depth_buffer);
    
#endif
    
    if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
    {
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
        //restore old
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, old_frame_buffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, old_render_buffer);
        
        return NO;
    }
    
    g_frame_buffer = _frame_buffer;
    g_render_buffer = _render_buffer;
    
    
    //set viewport
    ::glViewport(0, 0, _backbuffer_width, _backbuffer_height);
    //::glShadeModel(GL_SMOOTH);
    ::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    ::glDepthMask(GL_FALSE);
    ::glDisable(GL_DEPTH_TEST);
    ::glDisable(GL_ALPHA_TEST);
    ::glDisable(GL_CULL_FACE);
    //::glClearDepthf(1.0f);
    //::glEnable(GL_DEPTH_TEST);
    //::glDepthFunc(GL_LEQUAL);
    //::glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    //config opengl
    //enable smooth shading
    //::glShadeModel(GL_SMOOTH);
    //set clear color value
    //::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //set clear depth buffer value
    //::glClearDepthf(1.0f);
    //enable z-buff
    //::glDepthMask(GL_FALSE);
    //::glDisable(GL_DEPTH_TEST);
    //::glDepthMask(GL_TRUE);
    //::glEnable(GL_DEPTH_TEST);
    //::glDepthFunc(GL_GEQUAL);
    //alpha test
    //::glDisable(GL_ALPHA_TEST);
    //::glEnable(GL_ALPHA_TEST);
    //::glAlphaFunc(GL_GREATER, 0.0f);
    //::glDisable(GL_CULL_FACE);
    //::glEnable(GL_CULL_FACE);
    //::glCullFace(GL_BACK);
    //::glFrontFace(GL_CCW);
    //::glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //::glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    //::glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //::glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    
    ::glMatrixMode(GL_PROJECTION);
    ::glLoadIdentity();
    
    ////#ifdef USE_VERSION_200_AS_100
    ////    ::glOrthof(0, _backbuffer_width*2, _backbuffer_height*2, 0, -1.0f, 1.0f);
    ////#else
    ////	::glOrthof(0, _backbuffer_width, _backbuffer_height, 0, -1.0f, 1.0f);
    ////#endif
    return YES;
}


- (void) DestroyFramebuffer
{
    glDeleteFramebuffersOES(1, &_frame_buffer);
    _frame_buffer = 0;
    glDeleteRenderbuffersOES(1, &_render_buffer);
    _render_buffer = 0;
#if USE_DEPTH_BUFFER
    if (_depth_buffer)
    {
        glDeleteRenderbuffersOES(1, &_depth_buffer);
        _depth_buffer = 0;
    }
#endif
}


//multi touches process

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    [self UpdateTouches:event];
}

- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    [self UpdateTouches:event];
}

- (void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    //check end multi touches
    if ([touches count] == [[event touchesForView:self] count])
    {
        // last finger has lifted....
    }
    
    [self UpdateTouches:event];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	[self UpdateTouches:event];
}
- (void) UpdateTouches:(UIEvent*)event
{
    NSSet *touches = [event allTouches];
    
    for (UITouch *atouch in touches)
    {
        //check to see whether this touch event already exists
        int index = [self GetTouchIndex: atouch];
        if (index == -1 && atouch.phase == UITouchPhaseBegan) //doesn't exist yet, add it
        {
            index = _num_touches;
            [self AddTouch:atouch];
        }else if(UITouchPhaseBegan == atouch.phase){
            break;
            
        }
        
        //get current touch coordinates
        CGPoint touch_point = [atouch locationInView:self];
        
        if (index != -1)
        {
            if (atouch.phase == UITouchPhaseBegan)
            {
                UpdateTouchBegan(_touches_id[index], touch_point.x, touch_point.y);
            }
            else if (atouch.phase == UITouchPhaseMoved)
            {
                UpdateTouchMoved(_touches_id[index], touch_point.x, touch_point.y);
            }
            else if (atouch.phase == UITouchPhaseEnded)
            {
                UpdateTouchEnded(_touches_id[index], touch_point.x, touch_point.y);
                [self RemoveTouch:atouch];
            }
			else if(atouch.phase == UITouchPhaseCancelled)
			{
				UpdateTouchCancel(_touches_id[index], touch_point.x, touch_point.y);
                [self RemoveTouch:atouch];
			}
        }
            
    }
}

- (int) GetTouchIndex: (UITouch*) touch
{
    int i = 0;
    for (i = 0 ; i < _num_touches ; i++)
    {
        if (touch == _current_touches[i])
        {
            return i;
        }
    }
    i = -1;
    
    return i;
}


- (void) AddTouch: (UITouch*) touch
{
	if (_num_touches < MAX_TOUCHES)
	{
		_current_touches[_num_touches] = touch;
		_touches_id[_num_touches] = _touch_id_gen++;
		_num_touches++;
	}
}

- (void) RemoveTouch: (UITouch*) touch
{
    int index = [self GetTouchIndex:touch];
    
    if (index != -1)
    {
        for (int i = index; i < _num_touches - 1; i++)
        {
            _current_touches[i] = _current_touches[i+1];
            _touches_id[i] = _touches_id[i+1];
        }
        if(_num_touches > 0)
        _num_touches--;
    }
}

/*- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
 {
 NSLog(@"\n rotateInterface = %d \n",interfaceOrientation);
 return UIInterfaceOrientationIsLandscape(interfaceOrientation);
 
 }*/

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    //NSLog(@"\nshouldAutorotateToInterfaceOrientation = %d\n",interfaceOrientation);
    //return NO;
    // Return YES for supported orientations
    /*return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft ||
     interfaceOrientation == UIInterfaceOrientationLandscapeRight);*/
    return NO;/*#ifdef USE_AUTO_ROTATE_VIEW
               return YES;
               #else
               return NO;
               #endif*/
    
}

- (void) dealloc
{
    [self DestroyFramebuffer];
    
    if ([EAGLContext currentContext] == g_context)
    {
        [EAGLContext setCurrentContext:nil];
    }
    
    [g_context release];
    g_context = nil;
    
    [super dealloc];
}

- (GLuint)setupTexture:(NSString *)fileName {
    
    // 1
    CGImageRef spriteImage = [UIImage imageNamed:fileName].CGImage;
    if (!spriteImage) {
        NSLog(@"Failed to load image %@", fileName);
        exit(1);
    }
    
    // 2
    size_t width = CGImageGetWidth(spriteImage);
    size_t height = CGImageGetHeight(spriteImage);
    
    GLubyte * spriteData = (GLubyte *) calloc(width*height*4, sizeof(GLubyte));
    
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width*4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
    
    // 3
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, width, height), spriteImage);
    
    CGContextRelease(spriteContext);
    
    // 4
    GLuint texName;
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
    
    free(spriteData);
    return texName;
    
}

+(EAGLView*)sharedInstance
{
    return g_EAGLViewInstance;
}

#if OPENGLES_20_SUPPORT



#endif
#if ADD_ADMOB_BANNER_ADS
-(void) adViewInit
{
    
    
    /// The view that displays banner ads. A minimum implementation to get an ad from within a
    /// UIViewController class is:
    ///
    ///   \code
    ///   // Create and setup the ad view, specifying the size and origin at {0, 0}.
    ///   GADBannerView *adView = [[GADBannerView alloc] initWithAdSize:kGADAdSizeBanner];
    ///   adView.rootViewController = self;
    ///   adView.adUnitID = @"ID created when registering your app";
    ///
    ///   // Place the ad view onto the screen.
    ///   [self.view addSubview:adView];
    ///
    ///   // Request an ad without any additional targeting information.
    ///   [adView loadRequest:[GADRequest request]];
    ///   \endcode
    ///
#if ADD_ADMOB_BANNER_ADS
    CGSize size = BANNER_WH;
    CGPoint root = BANNER_XY;
    double scale = 1.0f;
    if(Device::IsIpadDevice())
    {
        UIScreen* mainscr = [UIScreen mainScreen];
        scale = mainscr.currentMode.size.width/768.0f;
        size = IPAD_BANNER_WH;
        root = IPAD_BANNER_XY;
        printf("\n ipad scale = %f",scale);

    }
    double width = size.width/2*scale;
    double height = size.height/2*scale;
    double xx = root.x/2*scale;
    double yy = root.y/2*scale;
   if(Device::IsIpadDevice())
    {
         width = size.width/self.contentScaleFactor*scale;
         height = size.height/self.contentScaleFactor*scale;
         xx = root.x/self.contentScaleFactor*scale;
         yy = root.y/self.contentScaleFactor*scale;
    }
    if(bannerView == nil)
    {
        bannerView = [[GADBannerView alloc] initWithAdSize:GADAdSizeFromCGSize(
                                                CGSizeMake(width,height))
                                                origin:CGPointMake(xx,yy)];
    }
    bannerView.delegate = self;
    bannerView.rootViewController = (UIViewController*)self;
    bannerView.adUnitID = @ADD_ADMOB_BANNER_ID;
    [bannerView loadRequest:[GADRequest request]];
#endif
}
-(void) adViewShow
{
    [self addSubview:bannerView];
}
-(void) adViewDiss
{
    [bannerView removeFromSuperview];
}
- (void)adViewDidReceiveAd:(GADBannerView *)bannerView
{
    bannerView.hidden = NO;
    
}
- (void)adView:(GADBannerView *)bannerView didFailToReceiveAdWithError:(GADRequestError *)error
{
     NSLog(@"adView:didFailToReceiveAdWithError: %@", error.localizedDescription);
}
- (void)adViewWillPresentScreen:(GADBannerView *)bannerView
{
    
}
- (void)adViewDidDismissScreen:(GADBannerView *)bannerView
{
    
}
- (void)adViewWillDismissScreen:(GADBannerView *)bannerView
{
    
}
- (void)adViewWillLeaveApplication:(GADBannerView *)bannerView
{
    
}
#endif
@end


@implementation UIImage (Crop)

- (UIImage *)crop:(CGRect)rect {
    
    rect = CGRectMake(rect.origin.x*self.scale,
                      rect.origin.y*self.scale,
                      rect.size.width*self.scale,
                      rect.size.height*self.scale);
    
    CGImageRef imageRef = CGImageCreateWithImageInRect([self CGImage], rect);
    UIImage *result = [UIImage imageWithCGImage:imageRef
                                          scale:self.scale
                                    orientation:self.imageOrientation];
    CGImageRelease(imageRef);
    return result;
}

@end
