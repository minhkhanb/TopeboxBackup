
#import "Accelerometer.h"
#include <math.h>
#include "CGame.h"

#define kAccelerometerFrequency		60 //Hz
#define kFilteringFactor			1.0
#ifndef DO_SHAKEKING()
#define DO_SHAKEKING()          {};
#endif
static Accelerometer* g_sharedInstance = nil;

@implementation Accelerometer

- (id) init
{
    
    self = [super init];
    x = 0;
    y = 0;
    z = 0;
    lastAcceleration = 0;
    if (self != nil) 
	{
        if (!motionManager)
        {
            motionManager = [[CMMotionManager alloc] init];
            [motionManager
             startAccelerometerUpdatesToQueue:[[NSOperationQueue alloc] init]
             withHandler:^(CMAccelerometerData *data, NSError *error)
             {
                 
                 dispatch_async(dispatch_get_main_queue(),
                                ^{
                                    dx = x;
                                    dy = y;
                                    dz = z;
                                    
                                    //Use a basic low-pass filter to only keep the gravity in the accelerometer values
                                    double newx = data.acceleration.x * kFilteringFactor + x * (1.0 - kFilteringFactor);
                                    double newy = data.acceleration.y * kFilteringFactor + y * (1.0 - kFilteringFactor);
                                    double newz = data.acceleration.z * kFilteringFactor + z * (1.0 - kFilteringFactor);
                                    
                                    if(abs((newx-x)*100) > 1.0f && newx <= 1.0f && newx >= -1.0)
                                    {
                                        x = newx;
                                    }
                                    if(abs((newy-y)*100) > 1.0f && newy <= 1.0f && newy >= -1.0)
                                    {
                                        y = newy;
                                    }
                                    if(abs((newz-z)*100) > 1.0f && newz <= 1.0f && newz >= -1.0)
                                    {
                                        z = newz;
                                    }
                                    //x = acceleration.x;
                                    //y = acceleration.y;
                                    //z = acceleration.z;
                            
                                    
                                    dx = x - dx;
                                    dy = y - dy;
                                    dz = z - dz;
                                    shx = x;
                                    shy = y;
                                    shz = z;
                                    
                                    if (Device::_safe_to_shake)
                                    {
                                        if ([self AccelerationIsShakingLast:0.05] && shakeCount >= 5)
                                        {
                                            //printf("\nShaking here!!!");
                                            shakeCount = 0;
                                            DO_SHAKEKING();
                                            
                                        } else if ([self AccelerationIsShakingLast:0.05]) {
                                            //printf("\nshakeCount = %d",shakeCount);
                                            shakeCount = shakeCount + 3;
                                        }else if (![self AccelerationIsShakingLast:0.03]) {
                                            if (shakeCount > 0) {
                                                shakeCount--;
                                            }
                                            if (shakeCount < 0)
                                            {
                                                shakeCount = 0;
                                            }
                                        }
                                    }
                                    last_shx = x;
                                    last_shy = y;
                                    last_shz = z;
                                    dx = x;
                                    dy = y;
                                    dz = z;
                                    
                                    
                                   
                                    
                                }
                                );
             }
             ];
        }
        
        /*
		//Configure and start accelerometer
		[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / kAccelerometerFrequency)];
		[[UIAccelerometer sharedAccelerometer] setDelegate:self];
         */
    }
    lastAcceleration++;
    return self;
}


- (BOOL) AccelerationIsShakingLast:(double)threshold {
    double
    deltaX = fabs(last_shx - shx),
    deltaY = fabs(last_shy - shy),
    deltaZ = fabs(last_shz - shz);
    
    return
    (deltaX > threshold && deltaY > threshold) ||
    (deltaX > threshold && deltaZ > threshold) ||
    (deltaY > threshold && deltaZ > threshold);
}
int shakeCount = 0;

+ (Accelerometer*) getSharedInstance 
{
	if (g_sharedInstance == nil)
	{
		g_sharedInstance = [[Accelerometer alloc] init];
	}
	
	return g_sharedInstance;
}

@end


//#define ORIENTATION_INVALID				(-1)
//#define ORIENTATION_PORTRAIT			(1 << 0)1
//#define ORIENTATION_LANDSCAPE_90		(1 << 1)2
//#define ORIENTATION_LANDSCAPE_270		(1 << 2)4

#if USE_ONLY_SUPPORT_LANDCAPE_MODE
    #define ORIENTATION_TRIGER				0.45f
#else 
    #define ORIENTATION_TRIGER				0.7f
#endif


#define BALLANCE_MAX					0.5f

#define ONE 4096

#if USE_ONLY_SUPPORT_LANDCAPE_MODE
Orientation last_orientation_detect = Orientation::Landscape_270;//left
#endif
extern "C" Orientation GetViewOrientation()
{
    UIInterfaceOrientation ori = [[UIApplication sharedApplication] statusBarOrientation];
    if(ori == UIInterfaceOrientationLandscapeLeft)
    {
        return Orientation::Landscape_270;
    }
    else if(ori == UIInterfaceOrientationLandscapeRight)
    {
        return Orientation::Landscape_90;
    }
    return Orientation::Portrait;
    
}
extern "C" Orientation GetPhoneOrientation()
{
    Accelerometer* accel = [Accelerometer getSharedInstance];
#if USE_ONLY_SUPPORT_LANDCAPE_MODE
    
	
    float x = (float)(accel->x);
    if (last_orientation_detect == Orientation::Landscape_270)
    {
        if (x < -ORIENTATION_TRIGER)
        {
            last_orientation_detect = Orientation::Landscape_90;
            return Orientation::Landscape_90;
        }
    }
    else if (last_orientation_detect == Orientation::Landscape_90)
    {
        if (x > ORIENTATION_TRIGER)
        {
            last_orientation_detect = Orientation::Landscape_270;
            return Orientation::Landscape_270;
        }
    }
    
    return last_orientation_detect;
#else
    
	float x = (float)(accel->x);
	float y = (float)(accel->y);
	//float z = (float)(accel->z);
		
	if (y < -ORIENTATION_TRIGER)
		return Orientation::Portrait;
	else if (x > ORIENTATION_TRIGER)
		return Orientation::Landscape_270;
	else if (x < -ORIENTATION_TRIGER)
		return Orientation::Landscape_90;
	else
		return Orientation::Portrait;
#endif
}

extern "C" int GetPhoneRotLeftRight(int phoneOrientation)
{
	Accelerometer* accel = [Accelerometer getSharedInstance];
	
	float x = (float)(accel->x);
	float y = (float)(accel->y);
	float angle = 0.0f;
	
	switch (phoneOrientation)
	{
		case ORIENTATION_PORTRAIT:
			angle = atan2f(x, -y);
			break;
		
		case ORIENTATION_LANDSCAPE_90:
			angle = atan2f(y, x);
			break;
			
		case ORIENTATION_LANDSCAPE_270:
			angle = atan2f(-y, -x);
			break;
	}
	
	return angle * ONE / M_PI_2;
}

extern "C" int GetPhoneRotSpeedLeftRight(int phoneOrientation)
{
	Accelerometer* accel = [Accelerometer getSharedInstance];
	
	float x = (float)(accel->x);
	float y = (float)(accel->y);
	
	float dx = (float)(accel->dx * kAccelerometerFrequency);
	float dy = (float)(accel->dy * kAccelerometerFrequency);

	
	float dAlpha = 0.0f;
	
	switch (phoneOrientation)
	{
		case ORIENTATION_PORTRAIT:
			dAlpha = (dx * (-y) - x * (-dy)) / (x * x + y * y);
			break;
			
		case ORIENTATION_LANDSCAPE_90:
			dAlpha = (dy * x - y * dx) / (x * x + y * y);
			break;
			
		case ORIENTATION_LANDSCAPE_270:
			dAlpha = ((-dy) * (-x) - (-y) * (-dx)) / (x * x + y * y);
			break;
	}
	
	return dAlpha * ONE;
}


extern "C" int GetPhoneRotFwdBack(int phoneOrientation)
{
	Accelerometer* accel = [Accelerometer getSharedInstance];
	
	float x = (float)(accel->x);
	float y = (float)(accel->y);
	float z = (float)(accel->z);
	
	float angle = 0.0f;
	
	switch (phoneOrientation)
	{
		case ORIENTATION_PORTRAIT:
			angle = atan2f(-z, -y);
			break;
			
		case ORIENTATION_LANDSCAPE_90:
			angle = atan2f(-z, x);
			break;
			
		case ORIENTATION_LANDSCAPE_270:
			angle = atan2f(-z, -x);
			break;
	}
	
	return angle * ONE / M_PI_2; 	// rad per second
}


extern "C" int GetPhoneRotSpeedFwdBack(int phoneOrientation)
{
	Accelerometer* accel = [Accelerometer getSharedInstance];
	
	float x = (float)(accel->x);
	float y = (float)(accel->y);
	float z = (float)(accel->z);
	
	float dx = (float)(accel->dx * kAccelerometerFrequency);
	float dy = (float)(accel->dy * kAccelerometerFrequency);
	float dz = (float)(accel->dz * kAccelerometerFrequency);
	
	float dAlpha = 0.0f;
	
	switch (phoneOrientation)
	{
		case ORIENTATION_PORTRAIT:
			dAlpha = ((-dz) * (-y) - (-z) * (-dy)) / ( z * z + y * y);
			break;
			
		case ORIENTATION_LANDSCAPE_90:
			dAlpha = ((-dz) * x - (-z) * dx) / ( z * z + x * x);
			break;
			
		case ORIENTATION_LANDSCAPE_270:
			dAlpha = ((-dz) * (-x) - (-z) * (-dx)) / ( z * z + x * x);
			break;
	}
	
	return dAlpha * ONE; 	// rad per second
}
extern "C" double GetAccelX()
{
	Accelerometer* accel = [Accelerometer getSharedInstance];
	return (double)(accel->x);
}
extern "C" double GetAccelY()
{
	Accelerometer* accel = [Accelerometer getSharedInstance];
	return (double)(accel->y);
}
extern "C" double GetAccelZ()
{
	Accelerometer* accel = [Accelerometer getSharedInstance];
	return (double)(accel->z);
}

/*NOT SURE*/		extern "C" double			OS_GetAccelX()
{
    return GetAccelX();
}
/*NOT SURE*/		extern "C" double			OS_GetAccelY()
{
    return GetAccelY();
}
/*NOT SURE*/		extern "C" double			OS_GetAccelZ()
{
    return GetAccelZ();
}
