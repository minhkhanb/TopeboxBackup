//
//  PineCameraUtils.h
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 5/19/14.
//
//

#ifndef __PineEntertainment__PineCameraUtils__
#define __PineEntertainment__PineCameraUtils__

#include <iostream>
//#endif
#if ADD_PICK_IMAGE

- (UIImage*)imageByScalingAndCroppingForSize:(UIImage*)sourceImage withSize:(CGSize)targetSize
{
    UIImage *newImage = nil;
    CGSize imageSize = sourceImage.size;
    CGFloat width = imageSize.width;
    CGFloat height = imageSize.height;
    CGFloat targetWidth = targetSize.width;
    CGFloat targetHeight = targetSize.height;
    CGFloat scaleFactor = 0.0;
    CGFloat scaledWidth = targetWidth;
    CGFloat scaledHeight = targetHeight;
    CGPoint thumbnailPoint = CGPointMake(0.0,0.0);
    
    if (CGSizeEqualToSize(imageSize, targetSize) == NO)
    {
        CGFloat widthFactor = targetWidth / width;
        CGFloat heightFactor = targetHeight / height;
        
        if (widthFactor > heightFactor)
        {
            scaleFactor = widthFactor; // scale to fit height
        }
        else
        {
            scaleFactor = heightFactor; // scale to fit width
        }
        
        scaledWidth  = width * scaleFactor;
        scaledHeight = height * scaleFactor;
        
        // center the image
        if (widthFactor > heightFactor)
        {
            thumbnailPoint.y = (targetHeight - scaledHeight) * 0.5;
        }
        else
        {
            if (widthFactor < heightFactor)
            {
                thumbnailPoint.x = (targetWidth - scaledWidth) * 0.5;
            }
        }
    }
    
    UIGraphicsBeginImageContext(targetSize); // this will crop
    
    CGRect thumbnailRect = CGRectZero;
    thumbnailRect.origin = thumbnailPoint;
    thumbnailRect.size.width  = scaledWidth;
    thumbnailRect.size.height = scaledHeight;
    
    [sourceImage drawInRect:thumbnailRect];
    
    newImage = UIGraphicsGetImageFromCurrentImageContext();
    
    if(newImage == nil)
    {
        NSLog(@"could not scale image");
    }
    
    //pop the context to get back to the default
    UIGraphicsEndImageContext();
    
    return newImage;
}
///-(void) SaveImageToPath(UIImage* image, NSString *file,int width_target,int hieght_target)
//{

///}
- (NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
{
    return UIInterfaceOrientationMaskPortrait;
}
///////////////////

-(void) SwitchCamera
{
    if (_image_take_pic.cameraDevice == UIImagePickerControllerCameraDeviceRear) {
        //self.imagePicker.cameraDevice = UIImagePickerControllerCameraDeviceFront;
        [UIView transitionWithView:_image_take_pic.view duration:0 options:UIViewAnimationOptionAllowAnimatedContent | UIViewAnimationOptionTransitionFlipFromLeft animations:^{
            _image_take_pic.cameraDevice = UIImagePickerControllerCameraDeviceFront;
        } completion:NULL];

    }
    else
    {
        [UIView transitionWithView:_image_take_pic.view duration:0 options:UIViewAnimationOptionAllowAnimatedContent | UIViewAnimationOptionTransitionFlipFromLeft animations:^{
            _image_take_pic.cameraDevice = UIImagePickerControllerCameraDeviceRear;
        } completion:NULL];
        //self._image_take_pic.cameraDevice = UIImagePickerControllerCameraDeviceRear;
    }
}
-(void) ShowCamera
{

   //dispatch_sync(dispatch_get_main_queue(), ^{
    if(_image_take_pic != nil)
    {
        [_image_take_pic release];
        _image_take_pic = nil;
    }
    _image_take_pic = [[UIImagePickerController alloc] init];
    _image_take_pic.delegate = self;
    _image_take_pic.sourceType = UIImagePickerControllerSourceTypeCamera;
    //[_image_take_pic setShowsCameraControls:NO];
    _image_take_pic.view.hidden = NO;
    
    _image_take_pic.showsCameraControls = NO;
    _image_take_pic.navigationBarHidden = YES;
    _image_take_pic.toolbarHidden = YES;
    ///[window addSubview:_image_take_pic.view];
    [glView removeFromSuperview];
    [_image_take_pic.view removeFromSuperview];
    [glView changeConfigForCamera:YES];
    //[_image_take_pic.view addSubview:glView];
    [window addSubview:_image_take_pic.view];
    [window addSubview:glView];
    
     //});
    
}
-(void) DisableCamera
{
    [glView changeConfigForCamera:NO];
    [glView removeFromSuperview];
    [_image_take_pic.view removeFromSuperview];
    [window addSubview:glView];
    [_image_take_pic release];
    _image_take_pic = nil;
}
-(void)captureCamera
{
    [_image_take_pic takePicture];
 }
-(void) InitPickImage
{
     dispatch_sync(dispatch_get_main_queue(), ^{
    _PickState =  PICK_STATE_CHOICE_PICTURE;
    if(_image_pick != nil)
    {
        [_image_pick release];
        _image_pick = nil;
    }
    _image_pick = [[UIImagePickerController alloc] init];

    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) {
        UIPopoverController *popover = [[UIPopoverController alloc] initWithContentViewController:_image_pick];
        _image_pick.delegate = self;
        _image_pick.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
        
        [popover presentPopoverFromRect:CGRectMake(0, k_SCREEN_HEIGHT,1,1) inView:glView permittedArrowDirections:UIPopoverArrowDirectionDown animated:YES];
        
    }
    else
    {
        
        _image_pick.delegate = self;
        _image_pick.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
        [window addSubview:_image_pick.view];
    }
          });
}

-(void) imagePickerController:(UIImagePickerController*)picker
didFinishPickingImage:(UIImage *)image
editingInfo:(NSDictionary *)editingInfo
{
    UIImage* img = [self imageByScalingAndCroppingForSize:image withSize:CGSizeMake(511,511)];
    NSData *imageData = UIImagePNGRepresentation(img);
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *savedImagePath = [documentsDirectory stringByAppendingPathComponent:@"picture_taked.png"];
    [imageData writeToFile:savedImagePath atomically:NO];
    
    if(picker == _image_take_pic)
    {
        
        //[self DisableCamera];
    }
    else
    {
        [picker dismissModalViewControllerAnimated:YES];
        [glView removeFromSuperview];
        _image_pick.view.hidden =  YES;
        [_image_pick.view removeFromSuperview];
        [window addSubview:glView];
        [_image_pick release];
        _image_pick = nil;
    }
    Device::_camera_state = CAMERA_COMPLETE_STATE;
}
-(void) imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [picker dismissModalViewControllerAnimated:YES];
    [glView removeFromSuperview];
    _image_pick.view.hidden =  YES;
    [_image_pick.view removeFromSuperview];
    [window addSubview:glView];
     [_image_pick release];
    _image_pick = nil;
    //exit(0);
    Device::_camera_state = CAMERA_FREE_STATE;
}

extern "C" void OS_ShowPicLibrary()
{
    [[PineGameDelegate sharedInstance] InitPickImage];
}
extern "C" void OS_ShowCamera()
{
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(ShowCamera) withObject:nil waitUntilDone:NO];
    //[[PineGameDelegate sharedInstance] ShowCamera];
    
}
extern "C" void OS_CaptureCamera()
{
    //[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(takeAPictureSaveToPath) withObject:nil waitUntilDone:NO];
    [[PineGameDelegate sharedInstance] captureCamera];
    
}
extern "C" void OS_DisableCamera()
{
     [[PineGameDelegate sharedInstance] DisableCamera];
}
extern "C" void OS_SwitchCamera()
{
    [[PineGameDelegate sharedInstance] SwitchCamera];

    
}
#else
extern "C" void OS_ShowPicLibrary()
{
   
}
extern "C" void OS_ShowCamera()
{
    
    
}
extern "C" void OS_CaptureCamera()
{
   
    
}
extern "C" void OS_DisableCamera()
{
    
}
extern "C" void OS_SwitchCamera()
{
        
    
}
#endif
#endif /* defined(__PineEntertainment__PineCameraUtils__) */
