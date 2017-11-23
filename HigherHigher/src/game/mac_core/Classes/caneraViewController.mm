 //
//  caneraViewController.m
//  PineEntertainment
//
//  Created by Elita Thai on 7/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "caneraViewController.h"
#import "CGame.h"
#import "PineGameDelegate.h"
#import <AssetsLibrary/AssetsLibrary.h>

extern "C" void saveToAlbum(int index);

//extern "C" char* GetRealFilePath(const char* filename);

static inline CGFloat degreesToRadians(CGFloat degrees)
{
    return M_PI * (degrees / 180.0);
}


@implementation caneraViewController

@synthesize imageTake;
@synthesize popoverController;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
        //[self useCamera];
    }
    return self;
}

-(void) popUp:(int)type
{
    if (self)
    {
        switch (type) {
            case 0:
                //[self testGetImageHTTP];
                printf("\n before use camera \n");
                [self presentModalViewController:imagePicker animated:YES];
                printf("\n after use camera \n");
                break;
            case 1:
                //[self presentModalViewController:_libPicker animated:YES];
                //[self presentViewController:_libPicker animated:NO completion:nil];
                //[self useCameraRoll];
                break;
        }
    }
}

- (void) testGetImageHTTP
{
    NSLog(@"\n Downloading...");
	// Get an image from the URL below
	UIImage *image = [[UIImage alloc] initWithData:[NSData dataWithContentsOfURL:[NSURL URLWithString:@IMAGE_DOWNLOAD_URL]]];
    
	NSLog(@"\n size %f,%f",image.size.width,image.size.height);
    
	// Let's save the file into Document folder.
	// You can also change this to your desktop for testing. (e.g. /Users/kiichi/Desktop/)
	// NSString *deskTopDir = @"/Users/kiichi/Desktop";
    
	NSString *docDir = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    
	// If you go to the folder below, you will find those pictures
	NSLog(@"\n folder %@",docDir);
    
	/*NSLog(@"\n saving png");
	NSString *pngFilePath = [NSString stringWithFormat:@"%@/test.png",docDir];
	NSData *data1 = [NSData dataWithData:UIImagePNGRepresentation(image)];
	[data1 writeToFile:pngFilePath atomically:YES];
    
	NSLog(@"\n saving jpeg");
	NSString *jpegFilePath = [NSString stringWithFormat:@"%@/test.jpeg",docDir];
	NSData *data2 = [NSData dataWithData:UIImageJPEGRepresentation(image, 1.0f)];//1.0f = 100% quality
	[data2 writeToFile:jpegFilePath atomically:YES];*/
    
	NSLog(@"\n saving image done");
#ifndef TEST_NO_RELEASE
	[image release];
#endif
}

-(void) dismiss
{
    //self.imageView = nil;
#ifndef TEST_NO_RELEASE
    [imagePicker release];
#endif
    //[self dismissModalViewControllerAnimated:NO];
    [imagePicker dismissModalViewControllerAnimated:NO];
    [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait animated:NO];
    
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    //[self useCamera];
	// Do any additional setup after loading the view.
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (void) useCamera
{
    if ([UIImagePickerController isSourceTypeAvailable:
         UIImagePickerControllerSourceTypeCamera])
    {
        imagePicker = [[UIImagePickerController alloc] init];
        //imagePicker = [[UIImagePickerController alloc] initWithNibName:@"imagePicker" bundle:nil];
        //self.modalPresentationStyle = UIModalPresentationFullScreen;
        
        imagePicker.delegate = self;
        imagePicker.sourceType = UIImagePickerControllerSourceTypeCamera;
        
        //imagePicker.mediaTypes = [NSArray arrayWithObjects:(NSString *) kUTTypeImage, nil];
        //imagePicker.allowsEditing = NO;
        
        imagePicker.navigationBarHidden = YES;
        imagePicker.toolbarHidden = YES;
        imagePicker.showsCameraControls = NO;
        imagePicker.wantsFullScreenLayout = YES;
        

        imagePicker.cameraDevice = UIImagePickerControllerCameraDeviceRear;
        [imagePicker setCameraFlashMode:UIImagePickerControllerCameraFlashModeAuto];

        
        
        //[self presentModalViewController:imagePicker animated:YES];
        
        newMedia = YES;
        //printf("\n end init");
    }
}

-(void) viewDidAppear:(BOOL)animated
{
    printf("\n viewDidAppear _type = %d \n", [PineGameDelegate sharedInstance]->_camera_load_type);
    [super viewDidAppear:animated];
    if ([PineGameDelegate sharedInstance]->_camera_load_type == 1)
    {
        [self useCameraRoll];
    }
    else if ([PineGameDelegate sharedInstance]->_camera_load_type == 0)
    {
        //[self useCamera];
    }
}

- (void) useCameraRoll
{
    if ([UIImagePickerController isSourceTypeAvailable:
         UIImagePickerControllerSourceTypeSavedPhotosAlbum])
    {

        imagePicker = [[UIImagePickerController alloc] init];
        
        imagePicker.delegate = self;
        imagePicker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
        imagePicker.allowsEditing = NO;
        
        [self presentModalViewController:imagePicker animated:YES];

        newMedia = NO;
        
        
    }
}


+ (UIImage *)croppedImageWithRect:(UIImage*)image {
    CGImageRef imgRef = image.CGImage;
    
    CGFloat width = CGImageGetWidth(imgRef);
    CGFloat height = CGImageGetHeight(imgRef);
    
    NSLog(@"\n before crop -> width = %f, height = %f \n", width, height);
    
    
    
    int square = width < height?width:height;

    int beginH, endH, beginW, endW;
    //case landcape
    if (width > height)
    {
        beginH = 0;
        endH = height;
        beginW = (width - height)/2;
        endW = beginW + square;
    }
    else //case portal
    {
        beginW = 0;
        endW = width;
        beginH = (height - width)/2;
        endH = beginH + square;
        
    }
    
    CGRect bounds = CGRectMake(beginW, beginH, square, square);
    CGImageRef imageRef = CGImageCreateWithImageInRect(imgRef, bounds);
    UIImage *croppedImage = [UIImage imageWithCGImage:imageRef];
    CGImageRelease(imageRef);
    CGImageRelease(imgRef);
    return croppedImage;
    /*CGRect        bnds = CGRectZero;
    
    bnds.size.height = IMAGE_FIX_SIZE;
    bnds.size.width = IMAGE_FIX_SIZE;
    
    UIImage*      copy = nil;
    CGContextRef  ctxt = nil;
    CGFloat       scal = 1.0;
    
    UIGraphicsBeginImageContext(bnds.size);
    ctxt = UIGraphicsGetCurrentContext();
    
    scal = bnds.size.width / square;
    CGContextSetInterpolationQuality(ctxt, kCGInterpolationHigh);
    CGContextScaleCTM(ctxt, scal, -scal);
    CGContextTranslateCTM(ctxt, 0.0, -square);
    CGContextDrawImage(ctxt, CGRectMake(0, 0, square, square), croppedImage.CGImage);
    copy = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    
    return copy;*/
    
}

+ (UIImage *)scaleAndRotateImage:(UIImage *)image {
    //int kMaxResolution = 640; // Or whatever
    
    //CGImageRef test = image.CGImage;
    //NSLog(@"\n test -> w = %ld, h = %ld \n", CGImageGetWidth(test), CGImageGetHeight(test));
    
    image = [caneraViewController croppedImageWithRect:image]; //[self croppedImageWithRect:image];
    //return image;
    
    CGImageRef imgRef = image.CGImage;
    
    CGFloat width = CGImageGetWidth(imgRef);
    CGFloat height = CGImageGetHeight(imgRef);
    
    NSLog(@"\n after crop -> width = %f, height = %f \n", width, height);
    
    CGAffineTransform transform = CGAffineTransformIdentity;
    CGRect bounds = CGRectMake(0, 0, width, height);
    if (width > IMAGE_FIX_SIZE || height > IMAGE_FIX_SIZE) {
        CGFloat ratio = width/height;
        if (ratio > 1) {
            bounds.size.width = IMAGE_FIX_SIZE;
            bounds.size.height = roundf(bounds.size.width / ratio);
        }
        else {
            bounds.size.height = IMAGE_FIX_SIZE;
            bounds.size.width = roundf(bounds.size.height * ratio);
        }
    }
    
    CGFloat scaleRatio = bounds.size.width / width;
    //NSLog(@"\n scaleRatio = %f", scaleRatio);
    CGSize imageSize = CGSizeMake(CGImageGetWidth(imgRef), CGImageGetHeight(imgRef));
    CGFloat boundHeight;
    UIImageOrientation orient = image.imageOrientation;
    if (GET_IMAGE()._take_camera_state == CAMERA_STATE_TAKING)
    {
        orient = (UIImageOrientation)3;
    }
    
    switch(orient) {
            
        case UIImageOrientationUp: //EXIF = 1
            transform = CGAffineTransformIdentity;
            break;
            
        case UIImageOrientationUpMirrored: //EXIF = 2
            transform = CGAffineTransformMakeTranslation(imageSize.width, 0.0);
            transform = CGAffineTransformScale(transform, -1.0, 1.0);
            break;
            
        case UIImageOrientationDown: //EXIF = 3
            transform = CGAffineTransformMakeTranslation(imageSize.width, imageSize.height);
            transform = CGAffineTransformRotate(transform, M_PI);
            break;
            
        case UIImageOrientationDownMirrored: //EXIF = 4
            transform = CGAffineTransformMakeTranslation(0.0, imageSize.height);
            transform = CGAffineTransformScale(transform, 1.0, -1.0);
            break;
            
        case UIImageOrientationLeftMirrored: //EXIF = 5
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(imageSize.height, imageSize.width);
            transform = CGAffineTransformScale(transform, -1.0, 1.0);
            transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);
            break;
            
        case UIImageOrientationLeft: //EXIF = 6
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(0.0, imageSize.width);
            transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);
            break;
            
        case UIImageOrientationRightMirrored: //EXIF = 7
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeScale(-1.0, 1.0);
            transform = CGAffineTransformRotate(transform, M_PI / 2.0);
            break;
            
        case UIImageOrientationRight: //EXIF = 8
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(imageSize.height, 0.0);
            transform = CGAffineTransformRotate(transform, M_PI / 2.0);
            break;
            
        default:
            [NSException raise:NSInternalInconsistencyException format:@"Invalid image orientation"];
            
    }
    
    UIGraphicsBeginImageContext(bounds.size);
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    if (orient == UIImageOrientationRight || orient == UIImageOrientationLeft) {
        CGContextScaleCTM(context, -scaleRatio, scaleRatio);
        CGContextTranslateCTM(context, -height, 0);
    }
    else {
        CGContextScaleCTM(context, scaleRatio, -scaleRatio);
        CGContextTranslateCTM(context, 0, -height);
    }
    
    CGContextConcatCTM(context, transform);
    
    CGContextDrawImage(UIGraphicsGetCurrentContext(), CGRectMake(0, 0, width, height), imgRef);
    UIImage *imageCopy = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    CGImageRelease(imgRef);
    //NSLog(@"\n after scale -> width = %f, height = %f \n", width, height);
    
    return imageCopy;
     
}

- (void)takePicture:(int)index
{
    [imagePicker takePicture];
    imageIndex = index;
}

- (bool)supportCameraFront
{
    return [UIImagePickerController isCameraDeviceAvailable: UIImagePickerControllerCameraDeviceFront];
}

- (void)switchCamera
{
    if (imagePicker.cameraDevice == UIImagePickerControllerCameraDeviceFront)
    {
        imagePicker.cameraDevice = UIImagePickerControllerCameraDeviceRear;
    }
    else if (imagePicker.cameraDevice == UIImagePickerControllerCameraDeviceRear)
    {
        imagePicker.cameraDevice = UIImagePickerControllerCameraDeviceFront;
    }
}

-(void)completeTakePicture
{

    printf("\n complete _camera_load_type = %d \n", [PineGameDelegate sharedInstance]->_camera_load_type);

    
    
        //NSLog(@"\n url - %@ \n", [_urlPicture absoluteString]);
   /* if ([PineGameDelegate sharedInstance]->_camera_load_type == 2)
    {
        [PineGameDelegate sharedInstance]->_camera_load_type = 0;
        NSData * _imgdata = UIImagePNGRepresentation([self scaleAndRotateImage:imageTake] );
        
        NSString *appfilename = [[NSString alloc] initWithUTF8String:GetDocumentFilePath(IMAGE_TEMP_FILE)];
        
        
        [_imgdata writeToFile:appfilename  atomically:NO];
        [_imgdata release];
        [appfilename release];
        //[self lib_dismiss:imagePicker];
    }
    else if ([PineGameDelegate sharedInstance]->_camera_load_type == 0)
    {
        [[PineGameDelegate sharedInstance] convertToCImage:[self scaleAndRotateImage:imageTake]  index:imageIndex];

    }*/
    //[imageTake release];
    
}

/*extern "C" void Convert_Image()
{
    if ([PineGameDelegate sharedInstance]->_camera_load_type == 0)
    {
        [[PineGameDelegate sharedInstance].cameraController completeTakePicture];
    }
    else if ([PineGameDelegate sharedInstance]->_camera_load_type == 2)
    {
        [[PineGameDelegate sharedInstance].lib_controller completeTakePicture];
    }
}*/



-(void)imagePickerController:(UIImagePickerController *)picker
didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    NSString *mediaType = [info objectForKey:UIImagePickerControllerMediaType];
    //[imageTake release];
    if ([mediaType isEqualToString:(NSString *)kUTTypeImage]) {
        if ([PineGameDelegate sharedInstance]->_camera_load_type == 1)
        {
            UIImage * imageOriginal=[info objectForKey:UIImagePickerControllerOriginalImage];
            CGImageRef imageRef = imageOriginal.CGImage;
            imageTake = [[UIImage alloc] initWithCGImage:imageRef];
            imageIndex = GET_IMAGE()._take_index[GET_IMAGE()._total_image_take];
            
            GET_THREAD().CompleteThread(THREAD_TYPE_GET_FROM_LIB);
            GET_THREAD()._error[THREAD_TYPE_GET_FROM_LIB] = 0;
            
           
            [self lib_dismiss:picker];
            [PineGameDelegate sharedInstance]->_camera_load_type = 2;
            
        }
        else if ([PineGameDelegate sharedInstance]->_camera_load_type == 0)
        {
            UIImage * imageOriginal=[info objectForKey:UIImagePickerControllerOriginalImage];
            CGImageRef imageRef = imageOriginal.CGImage;
            NSLog(@"\n camera w = %ld, h = %ld \n", CGImageGetWidth(imageRef), CGImageGetHeight(imageRef));
            imageTake = [[UIImage alloc] initWithCGImage:imageRef];
            //imageTake = [[info objectForKey:UIImagePickerControllerOriginalImage] retain];
            GET_THREAD().CompleteThread(THREAD_TYPE_TAKE_A_PICTURE);
        }
        
 
    }
    else if ([mediaType isEqualToString:(NSString *)kUTTypeMovie])
    {
		// Code here to support video if enabled
	}
}
-(void)image:(UIImage *)image
finishedSavingWithError:(NSError *)error 
 contextInfo:(void *)contextInfo
{
    if (error) {
        UIAlertView *alert = [[UIAlertView alloc]
                              initWithTitle: @"Save failed"
                              message: @"Failed to save image"\
                              delegate: nil
                              cancelButtonTitle:@"OK"
                              otherButtonTitles:nil];
        [alert show];
#ifndef TEST_NO_RELEASE
        [alert release];
#endif
    }
}

-(void)lib_dismiss:(UIImagePickerController *)picker
{
    [picker dismissModalViewControllerAnimated:YES];
    
    [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait animated:NO];
    CGame::_this->ClearTouchesEnd();
    [self removeFromParentViewController];
    [[[PineGameDelegate sharedInstance] window] bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
}

-(void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    printf("\n did cancel camera \n");
    
    GET_THREAD()._error[THREAD_TYPE_GET_FROM_LIB] = 1;
    GET_THREAD().CompleteThread(THREAD_TYPE_GET_FROM_LIB);
    
    [self lib_dismiss:picker];
    
    /*[picker dismissModalViewControllerAnimated:YES];
    [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait animated:NO];
    CGame::_this->ClearTouchesEnd();
    _type = -1;
    [self removeFromParentViewController];
    [[[PineGameDelegate sharedInstance] window] bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];*/
    
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    _current_orientation = interfaceOrientation;
    //NSLog(@"\n auto _current_orientation = %d", _current_orientation);
    //[[NSNotificationCenter defaultCenter] removeObserver:[[self view] window]];
    return UIDeviceOrientationIsPortrait(interfaceOrientation);
}

/*-(UIImage*)rotate:(UIImageOrientation)orient
{
    CGRect             bnds = CGRectZero;
    UIImage*           copy = nil;
    CGContextRef       ctxt = nil;
    CGRect             rect = CGRectZero;
    CGAffineTransform  tran = CGAffineTransformIdentity;
    
    bnds.size = self.size;
    rect.size = self.size;
    
    switch (orient)
    {
        case UIImageOrientationUp:
            return self;
            
        case UIImageOrientationUpMirrored:
            tran = CGAffineTransformMakeTranslation(rect.size.width, 0.0);
            tran = CGAffineTransformScale(tran, -1.0, 1.0);
            break;
            
        case UIImageOrientationDown:
            tran = CGAffineTransformMakeTranslation(rect.size.width,
                                                    rect.size.height);
            tran = CGAffineTransformRotate(tran, degreesToRadians(180.0));
            break;
            
        case UIImageOrientationDownMirrored:
            tran = CGAffineTransformMakeTranslation(0.0, rect.size.height);
            tran = CGAffineTransformScale(tran, 1.0, -1.0);
            break;
            
        case UIImageOrientationLeft:
            bnds.size = swapWidthAndHeight(bnds.size);
            tran = CGAffineTransformMakeTranslation(0.0, rect.size.width);
            tran = CGAffineTransformRotate(tran, degreesToRadians(-90.0));
            break;
            
        case UIImageOrientationLeftMirrored:
            bnds.size = swapWidthAndHeight(bnds.size);
            tran = CGAffineTransformMakeTranslation(rect.size.height,
                                                    rect.size.width);
            tran = CGAffineTransformScale(tran, -1.0, 1.0);
            tran = CGAffineTransformRotate(tran, degreesToRadians(-90.0));
            break;
            
        case UIImageOrientationRight:
            bnds.size = swapWidthAndHeight(bnds.size);
            tran = CGAffineTransformMakeTranslation(rect.size.height, 0.0);
            tran = CGAffineTransformRotate(tran, degreesToRadians(90.0));
            break;
            
        case UIImageOrientationRightMirrored:
            bnds.size = swapWidthAndHeight(bnds.size);
            tran = CGAffineTransformMakeScale(-1.0, 1.0);
            tran = CGAffineTransformRotate(tran, degreesToRadians(90.0));
            break;
            
        default:
            // orientation value supplied is invalid
            assert(false);
            return nil;
    }
    
    UIGraphicsBeginImageContext(bnds.size);
    ctxt = UIGraphicsGetCurrentContext();
    
    switch (orient)
    {
        case UIImageOrientationLeft:
        case UIImageOrientationLeftMirrored:
        case UIImageOrientationRight:
        case UIImageOrientationRightMirrored:
            CGContextScaleCTM(ctxt, -1.0, 1.0);
            CGContextTranslateCTM(ctxt, -rect.size.height, 0.0);
            break;
            
        default:
            CGContextScaleCTM(ctxt, 1.0, -1.0);
            CGContextTranslateCTM(ctxt, 0.0, -rect.size.height);
            break;
    }
    
    CGContextConcatCTM(ctxt, tran);
    CGContextDrawImage(ctxt, rect, self.CGImage);
    
    copy = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return copy;
}

/-(UIImage*)rotateAndScaleFromCameraWithMaxSize:(CGFloat)maxSize
{
    UIImage*  imag = self;
    
    imag = [imag rotate:imag.imageOrientation];
    imag = [imag scaleWithMaxSize:maxSize];
    
    return imag;
}

-(UIImage*)scaleWithMaxSize:(CGFloat)maxSize
{
    return [self scaleWithMaxSize:maxSize quality:kCGInterpolationHigh];
}

-(UIImage*)scaleWithMaxSize:(CGFloat)maxSize
                    quality:(CGInterpolationQuality)quality
{
    CGRect        bnds = CGRectZero;
    UIImage*      copy = nil;
    CGContextRef  ctxt = nil;
    CGRect        orig = CGRectZero;
    CGFloat       rtio = 0.0;
    CGFloat       scal = 1.0;
    
    bnds.size = self.size;
    orig.size = self.size;
    rtio = orig.size.width / orig.size.height;
    
    if ((orig.size.width <= maxSize) && (orig.size.height <= maxSize))
    {
        return self;
    }
    
    if (rtio > 1.0)
    {
        bnds.size.width  = maxSize;
        bnds.size.height = maxSize / rtio;
    }
    else
    {
        bnds.size.width  = maxSize * rtio;
        bnds.size.height = maxSize;
    }
    
    UIGraphicsBeginImageContext(bnds.size);
    ctxt = UIGraphicsGetCurrentContext();
    
    scal = bnds.size.width / orig.size.width;
    CGContextSetInterpolationQuality(ctxt, quality);
    CGContextScaleCTM(ctxt, scal, -scal);
    CGContextTranslateCTM(ctxt, 0.0, -orig.size.height);
    CGContextDrawImage(ctxt, orig, self.CGImage);
    
    copy = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return copy;
}*/

@end
