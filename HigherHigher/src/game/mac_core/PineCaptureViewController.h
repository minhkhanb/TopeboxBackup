
#ifndef PineCaptureViewController_h
#define PineCaptureViewController_h
NSMutableArray *tempVideoCaptureArray = nil;
int _capture_state;


extern "C" int OS_GetCaptureVideoState();
extern "C" void OS_SetCaptureVideoState(int state);
-(void) iOS_CaptureRepare
{
    if(tempVideoCaptureArray != nil)
    {
        [tempVideoCaptureArray release];
        tempVideoCaptureArray = nil;
    }
    tempVideoCaptureArray = [[NSMutableArray alloc] init];
    OS_SetCaptureVideoState(CAPTURE_VIDEO_STATE_NONE);
}

-(UIImage*) iOS_CaptureScreenToArray
{
    OS_SetCaptureVideoState(CAPTURE_VIDEO_STATE_RECORDING);
    UIGraphicsBeginImageContextWithOptions(glView.bounds.size, NO, CAPTURE_VIDEO_W/glView.bounds.size.width);
    //UIGraphicsBeginImageContextWithOptions(CGSizeMake(k_SCREEN_WIDTH, k_SCREEN_HEIGHT), NO, 1.0f);
    UIImage *image = nil;
    
    @try
    {
        [glView drawViewHierarchyInRect:glView.bounds afterScreenUpdates:YES];
        
        if (GAME()->GetGraphicContext().ComplexMode == PineGraphicComplexOrientation::ComplexNone)
        {
            image = UIGraphicsGetImageFromCurrentImageContext();
            UIGraphicsEndImageContext();
        }
        else if (GAME()->GetGraphicContext().ComplexMode == PineGraphicComplexOrientation::ComplexToLeft
                 || GAME()->GetGraphicContext().ComplexMode == PineGraphicComplexOrientation::ComplexToRight
                 )
        {
            float landcapeWidth = glView.bounds.size.height;
            float landcapeHeight = glView.bounds.size.width;
            
            UIImage *imageCopy = UIGraphicsGetImageFromCurrentImageContext();
            UIGraphicsEndImageContext();
            UIImageOrientation imageOrientation = UIImageOrientationLeft;
            if (GAME()->GetGraphicContext().ComplexMode == PineGraphicComplexOrientation::ComplexToLeft)
            {
                imageOrientation = UIImageOrientationRight;
            }
            
            //printf("\n w = %f, h = %f ", landcapeWidth, landcapeHeight);
            UIImage * landscapeImage = [[UIImage alloc] initWithCGImage: imageCopy.CGImage scale: 1.0 orientation:imageOrientation];
            UIGraphicsBeginImageContext(CGSizeMake(landcapeWidth, landcapeHeight));
            [landscapeImage drawInRect:CGRectMake(0, 0, landcapeWidth, landcapeHeight)];
            image = [UIGraphicsGetImageFromCurrentImageContext() retain];
            [landscapeImage release];
            UIGraphicsEndImageContext();
            
        }
        
        
    }
    @catch(NSException* eeee)
    {
        [glView.layer renderInContext:UIGraphicsGetCurrentContext()];
        image = nil;
    }
    if(image != nil && tempVideoCaptureArray != nil)
    {
        [tempVideoCaptureArray addObject:image];

    }
    return image;
}
-(void) InitCaptureViewVideo
{
    [self iOS_CaptureRepare];
    //[_pine_view_captureScreen ScreenCaptureViewInit];
}
-(void) CaptureScreenVideoStart
{
    if(tempVideoCaptureArray == nil)
    {
        return;
    }
    [self iOS_CaptureScreenToArray];
    //[_pine_view_captureScreen ScreenCaptureVideoStart];
}
extern "C" int OS_GetCaptureVideoState()
{
    return _capture_state;
}
extern "C" void OS_SetCaptureVideoState(int state)
{
    _capture_state = state;
}
extern "C" void OS_VideoPostSocial(const char* feed);
-(void) CaptureScreenVideoStop
{
    printf("\n Video Stop");
     OS_SetCaptureVideoState(CAPTURE_VIDEO_STATE_STOPED);
    if(tempVideoCaptureArray == nil)
    {
        return;
    }
    //NSArray *imgArray = [NSArray arrayWithArray:tempVideoCaptureArray];
    NSString *path = [NSHomeDirectory() stringByAppendingPathComponent:
                      [NSString stringWithFormat:@"Documents/pine_video_capture.mp4"]];
    [[NSFileManager defaultManager] removeItemAtPath:path error:NULL];
   
    CGSize size;
    size.height = CAPTURE_VIDEO_H;
    size.width = CAPTURE_VIDEO_W;
    [HJImagesToVideo videoFromImages:tempVideoCaptureArray toPath:path  withSize:size withFPS:FPS_VIDEO_RECORD animateTransitions:NO withCallbackBlock:^(BOOL success){
            if(tempVideoCaptureArray != nil)
            {
                [tempVideoCaptureArray autorelease];
                //[tempVideoCaptureArray dealloc];
                tempVideoCaptureArray = nil;
            }
        OS_SetCaptureVideoState(CAPTURE_VIDEO_STATE_COMPLETED);
        
    }];
    //[_pine_view_captureScreen ScreenCaptureVideoStop];
}

extern "C" int OS_GetGLViewWidth()
{
    return [[PineGameDelegate sharedInstance] glView].bounds.size.width;
    //[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(takeAPictureSaveToPath) withObject:nil waitUntilDone:NO];
}
extern "C" int OS_GetGLViewHeight()
{
    return [[PineGameDelegate sharedInstance] glView].bounds.size.height;
    //[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(takeAPictureSaveToPath) withObject:nil waitUntilDone:NO];
}
extern "C" void OS_InitCaptureViewVideo()
{
    [[PineGameDelegate sharedInstance] InitCaptureViewVideo];
     //[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(takeAPictureSaveToPath) withObject:nil waitUntilDone:NO];
}

extern "C" void OS_ScreenCaptureVideoStart()
{
    if(GAME()->_frame_counter % (60/FPS_VIDEO_RECORD) == 0)
    {
        [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(CaptureScreenVideoStart) withObject:nil waitUntilDone:NO];
        //[[PineGameDelegate sharedInstance] CaptureScreenVideoStart];
    }
    //
    //[[PineGameDelegate sharedInstance] CaptureScreenVideoStart];

}
extern "C" void OS_ScreenCaptureVideoStop()
{
    [[PineGameDelegate sharedInstance] CaptureScreenVideoStop];
}
#endif