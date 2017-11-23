
#include <string.h>
#import "PineGameDelegate.h"
#import "TextInput.h"

#define ALPHA                   @"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define NUMERIC                 @"1234567890"
#define ALPHA_NUMERIC           ALPHA NUMERIC
extern "C" bool IsCharacterSupported(int c); 
#ifndef ENABLE_RETURN_KEY_AUTOMATICALLY
#define ENABLE_RETURN_KEY_AUTOMATICALLY YES
#endif
static TextInput* g_sharedTextInputInstance = nil;
extern "C" bool IsSystemForceClose = false;
@implementation TextInput




- (void) keyboardWillHideHandler:(NSNotification *)notification
{
    
   
    NSLog(@"notification = %@ ", notification);
    if(g_sharedTextInputInstance != nil)
    {
        IsSystemForceClose = true;
         //[textField resignFirstResponder];
    }
    //show another viewcontroller here
}
-(void) DisableReturnKey:(bool) disable
{
    if(textField != nil)
    {
        textField.enablesReturnKeyAutomatically = !disable;
    }
}
-(void) ClearTextBuffer
{
    _need_clear_buffer = true;
}
-(void) close
{
    printf("\n close \n");
    //[textField  removeFromSuperview];
    [g_sharedTextInputInstance release];
    //[g_sharedTextInputInstance dealloc];
	g_sharedTextInputInstance = nil;
    
}
- (id)initWithTextBuffer:(char*)textBuff WithSize:(unsigned int)buffSize AtX:(int)x Y:(int)y Width:(int)w Height:(int)h
{
	self = [super init];
	
	if (self != nil)
	{
        IsSystemForceClose = true;
        _need_clear_buffer = true;
        //sprintf(Device::_keyboard_character,"");
		textBuffer = textBuff;
		textBufferSize = buffSize;
        
		CGRect rect = CGRectMake(x, y, w, h);

		textField = [[UITextField alloc] initWithFrame:rect];
		rect.~CGRect();
		textField.delegate = self;
		textField.borderStyle = UITextBorderStyleNone;//UITextBorderStyleRoundedRect;
		//textField.borderStyle = UITextBorderStyleRoundedRect;
		//textField.autocapitalizationType = UITextAutocapitalizationTypeAllCharacters;
		
        textField.keyboardType = UIKeyboardTypeDefault;
        
        textField.autocapitalizationType = UITextAutocapitalizationTypeWords;
        
        textField.returnKeyType = UIReturnKeyDone;
        //textField.set = true;
        
        
        textField.autocorrectionType = UITextAutocorrectionTypeNo;
        textField.enablesReturnKeyAutomatically = ENABLE_RETURN_KEY_AUTOMATICALLY;
        //textField.secureTextEntry = YES;
        //[textField setSecureTextEntry:YES];
        
		//NSString* placeHolderString = [[NSString alloc] initWithUTF8String:textBuff];
		//textField.placeholder = placeHolderString;
		//[placeHolderString release];
		
		// When the user starts typing, show the clear button in the text field.
		//textField.clearButtonMode = UITextFieldViewModeWhileEditing;
		
		textField.textColor = [UIColor blackColor];//blackColor];
        textField.hidden = true;
        
        
		//CGAffineTransform transform = CGAffineTransformMakeRotation(3.14159/2);
		//textField.transform = transform;
		//textField.clearsOnBeginEditing = YES;
        
       // [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait animated:NO];
        
		
		// Register to receive a notification when the movie has finished playing. 
		[[NSNotificationCenter defaultCenter] addObserver:self 
												 selector:@selector(textFieldDidChange:) 
													 name:UITextFieldTextDidChangeNotification 
												   object:textField];
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(keyboardWillHideHandler:)
                                                     name:UIKeyboardWillHideNotification
                                                   object:nil];
		
		[[[PineGameDelegate sharedInstance] glView] addSubview:textField];
        //[[[PineGameDelegate sharedInstance] glView] bringSubviewToFront:textField];
        //[[[PineGameDelegate sharedInstance] window] addSubview:textField];
        //[[[PineGameDelegate sharedInstance] window] bringSubviewToFront:textField];
        [textField becomeFirstResponder];
        _accii_char = [[NSMutableCharacterSet alloc] init];
        NSRange range;
        range.location = 0;
        range.length = 255; //A -> z
        [_accii_char addCharactersInRange:range];
        
        NSString* placeHolderString = [[NSString alloc] initWithUTF8String:textBuff];
        //NSLog(@"\n textBuffer = %s, placeHolderString = %@ ", textBuff, placeHolderString);
		textField.text = placeHolderString;
        _lengthText = [textField.text length];
        _max_length = 10000;
        //printf("\n init input text");
	}
	return self;
}

+(TextInput*)getSharedInstance
{
	return g_sharedTextInputInstance;
}

+(void)setSharedInstance:(TextInput*)instance
{
	if (g_sharedTextInputInstance != nil && g_sharedTextInputInstance != instance)
		[g_sharedTextInputInstance release];
	
	g_sharedTextInputInstance = instance;
}

- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField
{
	return YES;
}
- (void) SetTextMax:(int) max
{
    _max_length = max;
}
- (BOOL)textFieldShouldReturn:(UITextField *)theTextField
{
    if(Device::disableReturnKey)
    {
        return true;
    }
    bool flag = false;
	// When the user presses return, take focus away from the text field so that the keyboard is dismissed.
	if (theTextField == textField)
	{
        

        [textField resignFirstResponder];
	}

    if (flag)
    {
        return YES;
    }
    else
    {
        return NO;
    }
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string 
{
    if(_need_clear_buffer)
    {
        textField.text = @"";
        _need_clear_buffer = false;
    }
    else
    {
        if(strlen(Device::_keyboard_character) > _max_length)
        {
            return NO;
        }
        #if ONLY_SUPPORT_KEYBOARD_ABC
        NSCharacterSet *unacceptedInput = nil;
        unacceptedInput = [[NSCharacterSet characterSetWithCharactersInString:ALPHA_NUMERIC] invertedSet];
        return ([[string componentsSeparatedByCharactersInSet:unacceptedInput] count] <= 1);
        #endif
    }
    return YES;
}

- (int)getTextLength
{
    return _lengthText;//[textField.text length];
}

//  Notification called when the movie finished playing.

- (void) textFieldDidChange:(NSNotification*)notification
{
    
    
	int length = [textField.text length];
    _lengthText = length;

	if (length < 0)
		return;

    sprintf(textBuffer, "%s", [textField.text  UTF8String]);
    printf("\n%s", textBuffer);
    textBuffer[length] = '\0';
    sprintf(Device::_keyboard_character,"%s",textBuffer);
    if(strlen(Device::_keyboard_character) > _max_length)
    {
        Device::_keyboard_character[_max_length] = '\0';
        textField.text = [[NSString alloc] initWithUTF8String:Device::_keyboard_character];
    }


}

- (BOOL)textFieldShouldEndEditing:(UITextField *)textField
{
#if ADD_DEVICE_KEYBOARD
    GAME()->DismissKeyBoard();
#endif
    
    return YES;
}

- (void)dealloc
{
	// remove movie notifications
	[[NSNotificationCenter defaultCenter] removeObserver:self
		name:UITextFieldTextDidChangeNotification
		object:textField];
	
    [_accii_char release];
    
	[textField removeFromSuperview];
	
	[textField release];
	
	[super dealloc];
}


@end


@implementation TextInputInitParams

@end
extern "C" bool OS_IsSystemForceClose()
{
    return IsSystemForceClose;
}
// Wrappers
extern "C" void OS_TextInputStart(char* textBuf, unsigned int bufSize, int x, int y, int w, int h)
{
	TextInputInitParams* p = [[TextInputInitParams alloc] init];
	
	p->textBuff = (id)textBuf;
	p->buffSize = bufSize;
	p->x = x;
	p->y = y;
	p->w = w;
	p->h = h;
	
	[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(TextInputInit:) withObject:p waitUntilDone:YES];
	//[[PineGameDelegate sharedInstance] TextInputInit:p];
	[p release];
}

extern "C" void OS_TextInputClose()
{
	//[g_sharedTextInputInstance close];
    [[TextInput getSharedInstance] close];
    //[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(TextInputClose) withObject:nil waitUntilDone:YES];
    
}

extern "C" int OS_SetTextMax(int max)
{
    [g_sharedTextInputInstance SetTextMax:max];
    //- (void) SetTextMax:(int) max
}
extern "C" int OS_TextLength()
{
    return [g_sharedTextInputInstance getTextLength];
}
extern "C" void OS_DisableReturnKey(bool disable)
{
    return [g_sharedTextInputInstance DisableReturnKey:disable];
}
extern "C" void OS_ClearTextBuffer()
{
    [g_sharedTextInputInstance ClearTextBuffer];
    
}
