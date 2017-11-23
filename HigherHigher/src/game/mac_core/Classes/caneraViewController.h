//
//  caneraViewController.h
//  PineEntertainment
//
//  Created by Elita Thai on 7/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CAMERA_VIEW_H
#define CAMERA_VIEW_H 

#import <UIKit/UIKit.h>
#import <MobileCoreServices/MobileCoreServices.h>


@interface caneraViewController : UIViewController
            <UIImagePickerControllerDelegate, 
            UINavigationControllerDelegate>
            //,UIPopoverControllerDelegate>
{
    @public
    UIImagePickerController *imagePicker;
    BOOL newMedia;
    UIPopoverController *popoverController;
    int imageIndex;
    UIInterfaceOrientation _current_orientation;
    //UIImage *imageTake;
    //UIImageView *_imageView;
    NSURL *_urlPicture;
}
@property (nonatomic, retain) UIPopoverController *popoverController;
@property (nonatomic, retain) NSURL *_urlPicture;
@property (strong, retain) UIImage *imageTake;
- (IBAction)useCamera;
- (IBAction)useCameraRoll;
- (void)takePicture:(int)index;
- (bool)supportCameraFront;
- (void)switchCamera;
- (void)popUp:(int)type;
- (void)dismiss;
+ (UIImage *)croppedImageWithRect:(UIImage*)image;
+ (UIImage *)scaleAndRotateImage:(UIImage *)image;
@end

#endif