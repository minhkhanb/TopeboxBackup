
#undef WIN32
#import <UIKit/UIKit.h>

@interface TextInput : UIViewController <UITextFieldDelegate>
{
@public
	UITextField *textField;
	char* textBuffer;
	unsigned int textBufferSize;
    NSMutableCharacterSet *_accii_char;
    int _lengthText;
    int _max_length;
    bool _need_clear_buffer;
}

//@property (nonatomic, retain) UITextField *textField;

-(id)initWithTextBuffer:(char*)textBuff WithSize:(unsigned int)buffSize AtX:(int)x Y:(int)y Width:(int)w Height:(int)h;
-(int)getTextLength;
+(TextInput*)getSharedInstance;
+(void)setSharedInstance:(TextInput*)instance;
-(void) close;
-(void) DisableReturnKey:(bool) disable;
-(void) ClearTextBuffer;
@end


@interface TextInputInitParams : NSObject
{
@public
	id textBuff;
	unsigned int buffSize;
	int x;
	int y;
	int w;
	int h;
}

@end
