
#undef WIN32
#import <UIKit/UIAccelerometer.h>
#include "Defines.h"
#import <CoreMotion/CoreMotion.h>

@interface Accelerometer : UIResponder//: NSObject <UIAccelerometerDelegate>
{
@public
    CMMotionManager *motionManager;
    UIAccelerationValue		x, y, z;
	UIAccelerationValue		dx, dy, dz;
    
    UIAccelerationValue shx,shy,shz,last_shx,last_shy,last_shz;
    int lastAcceleration;
}

- (id)init;
+ (Accelerometer*) getSharedInstance;

@end
extern "C" double GetAccelX();
extern "C" double GetAccelY();
extern "C" double GetAccelZ();