//
//  NSDataEncryption.h
//  MailComposer
//
//  Created by Elita Thai on 6/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef MailComposer_NSDataEncryption_h
#define MailComposer_NSDataEncryption_h

#import <Foundation/Foundation.h>


@interface NSData (AES256)

- (NSData *)AES256EncryptWithKey:(NSString *)key;
- (NSData *)AES256DecryptWithKey:(NSString *)key;

- (NSData *)DesEncryptWithKey:(NSString *)key;
- (NSData *)DesDecryptWithKey:(NSString *)key;

@end


#endif
