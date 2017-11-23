#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import "Defines.h"
#if ADD_ADMOB_BANNER_ADS
#import <GoogleMobileAds/GoogleMobileAds.h>
#endif
//#define MAX_TOUCHES 100

//#import "ODataDelegate.h"

//#import "GLES20Lib.h"
@interface EAGLView : UIView <
    //ODataDelegate,
    UIImagePickerControllerDelegate,
#if ADD_ADMOB_BANNER_ADS
    GADBannerViewDelegate,
#endif
    UINavigationControllerDelegate
    >

{
@public
    /* The pixel dimensions of the backbuffer */
    GLint 					_backbuffer_width;
    GLint 					_backbuffer_height;
@private

	
	//EAGLContext*			_context;
	
	/* OpenGL names for the renderbuffer and framebuffers used to render to this view */
	GLuint					_frame_buffer;
	GLuint					_render_buffer;
	
			
	int						_num_touches;
	UITouch*				_current_touches[MAX_TOUCHES];
	int						_touch_id_gen;
	int						_touches_id[MAX_TOUCHES];
    
    NSArray *resultArray;
    
    UIViewController *m_viewController;

#if USE_DEPTH_BUFFER
    /* OpenGL name for the depth buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist) */
	GLuint					_depth_buffer;

#endif
	bool 					_is_init_complete;
#if ADD_ADMOB_BANNER_ADS
    GADBannerView *bannerView;
#endif
}
+(EAGLView*)sharedInstance;
- (id) initGL:(CGRect)frame;
- (id) initWithFrame:(CGRect)frame;
- (void) initGLFrame:(CGRect)frame;
- (void) changeConfigForCamera:(BOOL)open;

- (BOOL) CreateFramebuffer;
- (void) createCameraController;
- (void) DestroyFramebuffer;

- (void) UpdateTouches:(UIEvent*)event;
- (int) GetTouchIndex: (UITouch*) touch;
- (void) AddTouch: (UITouch*) touch;
- (void) RemoveTouch: (UITouch*) touch;
- (void) ClearTouch;
- (GLuint)setupTexture:(NSString *)fileName;
#if ADD_ADMOB_BANNER_ADS
- (void)adViewDidReceiveAd:(GADBannerView *)bannerView;
- (void)adView:(GADBannerView *)bannerView didFailToReceiveAdWithError:(GADRequestError *)error;
- (void)adViewWillPresentScreen:(GADBannerView *)bannerView;
- (void)adViewDidDismissScreen:(GADBannerView *)bannerView;
- (void)adViewWillDismissScreen:(GADBannerView *)bannerView;
- (void)adViewWillLeaveApplication:(GADBannerView *)bannerView;
-(void) adViewShow;
-(void) adViewDiss;
#endif

@end
