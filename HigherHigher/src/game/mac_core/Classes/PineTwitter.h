extern "C" const char* OS_convertSStringTocharArray(NSString* str);

- (BOOL)userHasAccessToTwitter
{
    return [SLComposeViewController
            isAvailableForServiceType:SLServiceTypeTwitter];
}
-(void) openTwitterSetting
{
    
}
-(void) TwitterInit
{
    if(![self userHasAccessToTwitter])
    {
        CGame::_this->_pine_social->SetState(PINESOCIAL_STATE_COMPLETED);
        //CGame::_this->_pine_social->SetFlag(SOCIAL_FLAG_NO_ACCOUNT_LOGIN);
        CGame::_this->_pine_social->SetFlag(SOCIAL_FLAG_NO_ACCOUNT_LOGIN);
        PineSocial::ShowMessageBox("Require Twitter log in to access! Check your device setting for loggin in and re-luanch the game again to active this feature.");
        return ;
    }
    if(tw_waccountStore == nil)
    {
        tw_waccountStore = [[ACAccountStore alloc] init];
    }
    ACAccountType *accountType = [tw_waccountStore accountTypeWithAccountTypeIdentifier:ACAccountTypeIdentifierTwitter];
    [tw_waccountStore requestAccessToAccountsWithType:accountType options:nil completion:^(BOOL granted, NSError *error)
     {
         // If we have access to the Twitter accounts configured on the device we will contact the Twitter API.
         
         if (granted == YES)
         {
             // If there is a leat one account we will contact the Twitter API.
            NSArray *arrayOfAccounts = [tw_waccountStore accountsWithAccountType:accountType]; // Retrieves an array of Twitter
            int count = [arrayOfAccounts count];
             if (count > 0) {
                ACAccount *twitterAccount = [arrayOfAccounts lastObject];
                NSDictionary *properties = [twitterAccount dictionaryWithValuesForKeys:[NSArray arrayWithObject:@"properties"]];
                 NSDictionary *details = [properties objectForKey:@"properties"];
        
                 CGame::_this->_pine_social->SetInfomation([[NSString stringWithFormat:@"%@",[details objectForKey:@"user_id"]] longLongValue],[[twitterAccount.username retain] UTF8String]);

             }
             
            CGame::_this->_pine_social->SetFlag(SOCIAL_FLAG_FULL_ACCESS);
            CGame::_this->_pine_social->SetState(PINESOCIAL_STATE_SUCCESS);
         }
         else
         {
            CGame::_this->_pine_social->SetFlag(SOCIAL_FLAG_ACCESS_DENINE);
             CGame::_this->_pine_social->SetState(PINESOCIAL_STATE_COMPLETED);
             
         }
       
         
     }];
    

}

#define GET_URL_AVATAR_TIME_OUT (10)
 NSString *profileImageUrl = nil;
-(NSString*) GetURLTwitterAvartar:(NSString*) user_id
{
    if(![self userHasAccessToTwitter])
    {
        PineSocial::ShowMessageBox("Require Twitter log in to access! Check your device setting for loggin in and re-luanch the game again to active this feature.");
        return nil;
    }
    if(tw_waccountStore == nil)
    {
        [self TwitterInit];
    }
    profileImageUrl = nil;
    ACAccountType *accountType = [tw_waccountStore accountTypeWithAccountTypeIdentifier:ACAccountTypeIdentifierTwitter];
    
    [tw_waccountStore requestAccessToAccountsWithType:accountType options:nil completion:^(BOOL granted, NSError *error)
     {
         // If we have access to the Twitter accounts configured on the device we will contact the Twitter API.
         
         if (granted == YES)
         {
             // If there is a leat one account we will contact the Twitter API.
             NSArray *arrayOfAccounts = [tw_waccountStore accountsWithAccountType:accountType]; // Retrieves an array of Twitter accounts configured on the device.
             
             // If there is a leat one account we will contact the Twitter API.
             int count = [arrayOfAccounts count];
             //printf("\nNum of Account %d",count);
             if (count > 0) {
                 
                 ACAccount *twitterAccount = [arrayOfAccounts lastObject];
                 NSURL *followingURL = [NSURL URLWithString:@"https://api.twitter.com/1.1/users/show.json"];
                 // Pass in the parameters (basically '.ids.json?screen_name=[screen_name]')
                 //NSDictionary *properties = [twitterAccount dictionaryWithValuesForKeys:[NSArray arrayWithObject:@"properties"]];
                 //NSDictionary *details = [properties objectForKey:@"properties"];
                 //NSLog(@"details = %@",details);//full name
                 //NSLog(@"user name = %@",[details objectForKey:@"fullName"]);//full name
                 //NSLog(@"user_id  =  %@",[details objectForKey:@"user_id"]);//user id
                 NSDictionary *parameters = [NSDictionary dictionaryWithObjectsAndKeys:user_id, @"user_id", nil];
                 // Setup the request
                 TWRequest *twitterRequest = [[TWRequest alloc] initWithURL:followingURL
                                                                 parameters:parameters
                                                              requestMethod:SLRequestMethodGET];
                
                 [twitterRequest setAccount:twitterAccount];
                 
                 [twitterRequest performRequestWithHandler:^(NSData *responseData, NSHTTPURLResponse *urlResponse, NSError *error) {
                     if (error) {
                         
                     }
                     NSError *jsonError = nil;
                    
                     NSDictionary *twitterFriends = [NSJSONSerialization JSONObjectWithData:responseData options:NSJSONWritingPrettyPrinted error:&jsonError];
                     profileImageUrl = [[NSString alloc] initWithFormat:@"%@",[twitterFriends objectForKey:@"profile_image_url"]];

                     //profileImageUrl = [twitterFriends objectForKey:@"profile_image_url"];
                     
                 }];
                 

             }
             
             
         }
         else
         {
             
             // Handle failure to get account access
             if(error == 0)
             {
                 //old post;
             }
             NSLog(@"%@", [error localizedDescription]);
            
         }
         
     }];
    unsigned long long t = time(NULL);
    while(profileImageUrl == nil && (time(NULL)-t)<(GET_URL_AVATAR_TIME_OUT))
    {
        ;
    }
     NSLog(@"profileImageUrl %@ time processing = %lld", profileImageUrl,(time(NULL)-t));
    return  profileImageUrl;
}
-(void) AuthTwitter
{
    if(![self userHasAccessToTwitter])
    {
        //old post
          PineSocial::ShowMessageBox("Require Twitter log in to access! Check your device setting for loggin in and re-luanch the game again to active this feature.");
        return;
    }
    if(tw_waccountStore == nil)
    {
        tw_waccountStore = [[ACAccountStore alloc] init];
    }
    // Asks for the Twitter accounts configured on the device.
    
    ACAccountType *accountType = [tw_waccountStore accountTypeWithAccountTypeIdentifier:ACAccountTypeIdentifierTwitter];
    
    [tw_waccountStore requestAccessToAccountsWithType:accountType options:nil completion:^(BOOL granted, NSError *error)
     {
         // If we have access to the Twitter accounts configured on the device we will contact the Twitter API.
         
         if (granted == YES)
         {
             // If there is a leat one account we will contact the Twitter API.
             NSArray *arrayOfAccounts = [tw_waccountStore accountsWithAccountType:accountType]; // Retrieves an array of Twitter accounts configured on the device.
             
             // If there is a leat one account we will contact the Twitter API.
             int count = [arrayOfAccounts count];
             printf("\nNum of Account %d",count);
             if (count > 0) {
                 
                 ACAccount *twitterAccount = [arrayOfAccounts lastObject];
                 
                 printf("\nEnd twitter");
                 
                 NSLog(@"%@",twitterAccount.username);
                 NSDictionary *properties = [twitterAccount dictionaryWithValuesForKeys:[NSArray arrayWithObject:@"properties"]];
                 NSDictionary *details = [properties objectForKey:@"properties"];
                 NSLog(@"details = %@",details);//full name
                 NSLog(@"user name = %@",[details objectForKey:@"fullName"]);//full name
                 NSLog(@"user_id  =  %@",[details objectForKey:@"user_id"]);//user id
                 
                 [self GetURLTwitterAvartar:@"465542207"];
                 [self GetFollowers];
                 
                 
             }
             
             
         }
         else
         {
             
             // Handle failure to get account access
             if(error == 0)
             {
                 //old post;
             }
             NSLog(@"%@", [error localizedDescription]);
             
         }
         
     }];
}
-(void)getTwitterFlowwerForAccount:(ACAccount*)account
{
   //
    // In this case I am creating a dictionary for the account
    // Add the account screen name
    NSMutableDictionary *accountDictionary = [NSMutableDictionary dictionaryWithObjectsAndKeys:account.username, @"screen_name", nil];
    // Add the user id (I needed it in my case, but it's not necessary for doing the requests)
    [accountDictionary setObject:[[[account dictionaryWithValuesForKeys:[NSArray arrayWithObject:@"properties"]] objectForKey:@"properties"] objectForKey:@"user_id"] forKey:@"user_id"];
    // Setup the URL, as you can see it's just Twitter's own API url scheme. In this case we want to receive it in JSON
    NSURL *followingURL = [NSURL URLWithString:@"https://api.twitter.com/1.1/followers/ids.json"];
    // Pass in the parameters (basically '.ids.json?screen_name=[screen_name]')
    NSDictionary *parameters = [NSDictionary dictionaryWithObjectsAndKeys:account.username, @"screen_name", nil];
    // Setup the request
    TWRequest *twitterRequest = [[TWRequest alloc] initWithURL:followingURL
                                                    parameters:parameters
                                                 requestMethod:SLRequestMethodGET];
    // This is important! Set the account for the request so we can do an authenticated request. Without this you cannot get the followers for private accounts and Twitter may also return an error if you're doing too many requests
    [twitterRequest setAccount:account];
    // Perform the request for Twitter friends
    [twitterRequest performRequestWithHandler:^(NSData *responseData, NSHTTPURLResponse *urlResponse, NSError *error) {
        if (error) {
            // deal with any errors - keep in mind, though you may receive a valid response that contains an error, so you may want to look at the response and ensure no 'error:' key is present in the dictionary
        }
        NSError *jsonError = nil;
        // Convert the response into a dictionary
        NSDictionary *twitterFriends = [NSJSONSerialization JSONObjectWithData:responseData options:NSJSONWritingPrettyPrinted error:&jsonError];
        // Grab the Ids that Twitter returned and add them to the dictionary we created earlier
        [accountDictionary setObject:[twitterFriends objectForKey:@"ids"] forKey:@"friends_ids"];
        NSLog(@"%@", accountDictionary);
    }];


}
-(void)getTwitterFriendsForAccount:(ACAccount*)account {
    // In this case I am creating a dictionary for the account
    // Add the account screen name
    NSMutableDictionary *accountDictionary = [NSMutableDictionary dictionaryWithObjectsAndKeys:account.username, @"screen_name", nil];
    // Add the user id (I needed it in my case, but it's not necessary for doing the requests)
    [accountDictionary setObject:[[[account dictionaryWithValuesForKeys:[NSArray arrayWithObject:@"properties"]] objectForKey:@"properties"] objectForKey:@"user_id"] forKey:@"user_id"];
    // Setup the URL, as you can see it's just Twitter's own API url scheme. In this case we want to receive it in JSON
    NSURL *followingURL = [NSURL URLWithString:@"https://api.twitter.com/1.1/friends/ids.json"];
    // Pass in the parameters (basically '.ids.json?screen_name=[screen_name]')
    NSDictionary *parameters = [NSDictionary dictionaryWithObjectsAndKeys:account.username, @"screen_name", nil];
    // Setup the request
    TWRequest *twitterRequest = [[TWRequest alloc] initWithURL:followingURL
                                                    parameters:parameters
                                                 requestMethod:SLRequestMethodGET];
    // This is important! Set the account for the request so we can do an authenticated request. Without this you cannot get the followers for private accounts and Twitter may also return an error if you're doing too many requests
    [twitterRequest setAccount:account];
    // Perform the request for Twitter friends
    [twitterRequest performRequestWithHandler:^(NSData *responseData, NSHTTPURLResponse *urlResponse, NSError *error) {
        if (error) {
            // deal with any errors - keep in mind, though you may receive a valid response that contains an error, so you may want to look at the response and ensure no 'error:' key is present in the dictionary
        }
        NSError *jsonError = nil;
        // Convert the response into a dictionary
        NSDictionary *twitterFriends = [NSJSONSerialization JSONObjectWithData:responseData options:NSJSONWritingPrettyPrinted error:&jsonError];
        NSArray* list_id= [twitterFriends objectForKey:@"ids"];
        // Grab the Ids that Twitter returned and add them to the dictionary we created earlier
        //[accountDictionary setObject:[twitterFriends objectForKey:@"ids"] forKey:@"friends_ids"];
        //NSLog(@"%@ count = %d", twitterFriends,[list_id count]);
        CGame::_this->_pine_social->ResetFriendList();
        for(int i= 0; i < [list_id count]; i++)
        {
            
            CGame::_this->_pine_social->AddFriend([[NSString stringWithFormat:@"%@",[list_id objectAtIndex:i]] longLongValue]);
        }
        CGame::_this->_pine_social->SetFlag(SOCIAL_FLAG_FULL_ACCESS);
        CGame::_this->_pine_social->SetState(PINESOCIAL_STATE_SUCCESS);
    }];
}
-(void) GetFriends
{
    if(![self userHasAccessToTwitter])
    {
          PineSocial::ShowMessageBox("Require Twitter log in to access! Check your device setting for loggin in and re-luanch the game again to active this feature.");
        //old post
        //CGame::_this->_pine_social->SetFlag(SOCIAL_FLAG_NO_ACCOUNT_LOGIN);
        return;
    }
    if(tw_waccountStore == nil)
    {
        tw_waccountStore = [[ACAccountStore alloc] init];
    }
    // Asks for the Twitter accounts configured on the device.
    
    ACAccountType *accountType = [tw_waccountStore accountTypeWithAccountTypeIdentifier:ACAccountTypeIdentifierTwitter];
    
    [tw_waccountStore requestAccessToAccountsWithType:accountType options:nil completion:^(BOOL granted, NSError *error)
     {
         // If we have access to the Twitter accounts configured on the device we will contact the Twitter API.
         
         if (granted == YES)
         {
             // If there is a leat one account we will contact the Twitter API.
             NSArray *arrayOfAccounts = [tw_waccountStore accountsWithAccountType:accountType]; // Retrieves an array of Twitter accounts configured on the device.
             
             // If there is a leat one account we will contact the Twitter API.
             int count = [arrayOfAccounts count];
             if (count > 0) {
                 
                 ACAccount *twitterAccount = [arrayOfAccounts lastObject];
                 [self getTwitterFriendsForAccount:twitterAccount];
                 
             }
             
         }
         else
         {
             CGame::_this->_pine_social->SetFlag(SOCIAL_FLAG_ACCESS_DENINE);
             CGame::_this->_pine_social->SetState(PINESOCIAL_STATE_COMPLETED);
         }
         
     }];
}
-(void) GetFollowers
{
    if(![self userHasAccessToTwitter])
    {
        //old post
        return;
    }
    if(tw_waccountStore == nil)
    {
        tw_waccountStore = [[ACAccountStore alloc] init];
    }
    
    // Asks for the Twitter accounts configured on the device.
    
    ACAccountType *accountType = [tw_waccountStore accountTypeWithAccountTypeIdentifier:ACAccountTypeIdentifierTwitter];
    
    [tw_waccountStore requestAccessToAccountsWithType:accountType options:nil completion:^(BOOL granted, NSError *error)
     {
         // If we have access to the Twitter accounts configured on the device we will contact the Twitter API.
         
         if (granted == YES)
         {
             // If there is a leat one account we will contact the Twitter API.
             NSArray *arrayOfAccounts = [tw_waccountStore accountsWithAccountType:accountType]; // Retrieves an array of Twitter accounts configured on the device.
             
             // If there is a leat one account we will contact the Twitter API.
             int count = [arrayOfAccounts count];
             printf("\nNum of Account %d",count);
             if (count > 0) {
                 
                 ACAccount *twitterAccount = [arrayOfAccounts lastObject];
                 [self getTwitterFlowwerForAccount:twitterAccount];
                 //NSString *username = twitterAccount.username;
                 
                 
                

             
             }
           
             
         }
         else
         {
             
             // Handle failure to get account access
             if(error == 0)
             {
                 //old post;
             }
             NSLog(@"%@", [error localizedDescription]);
             
         }
         
     }];
    printf("\nEnd twitter");
}
-(void) PostScreenToTwitter:(NSString*) feed
{
    //[self capture_screenshot];
    if(![self userHasAccessToTwitter])
    {
          PineSocial::ShowMessageBox("Require Twitter log in to access! Check your device setting for loggin in and re-luanch the game again to active this feature.");
        //old post
        return;
    }
    if(tw_waccountStore == nil)
    {
        tw_waccountStore = [[ACAccountStore alloc] init];
    }
    
    // Asks for the Twitter accounts configured on the device.
    
    ACAccountType *accountType = [tw_waccountStore accountTypeWithAccountTypeIdentifier:ACAccountTypeIdentifierTwitter];
    
    [tw_waccountStore requestAccessToAccountsWithType:accountType options:nil completion:^(BOOL granted, NSError *error)
     {
         // If we have access to the Twitter accounts configured on the device we will contact the Twitter API.
         
         if (granted == YES)
         {
             // If there is a leat one account we will contact the Twitter API.
             NSArray *arrayOfAccounts = [tw_waccountStore accountsWithAccountType:accountType]; // Retrieves an array of Twitter accounts configured on the device.
             
             // If there is a leat one account we will contact the Twitter API.
             int count = [arrayOfAccounts count];
             printf("\nNum of Account %d",count);
             if (count > 0)
             {
                 //dispatch_sync(dispatch_get_main_queue(), ^{
                 SLRequestHandler requestHandler =
                 ^(NSData *responseData, NSHTTPURLResponse *urlResponse, NSError *error) {
                     if (responseData) {
                         NSInteger statusCode = urlResponse.statusCode;
                         if (statusCode >= 200 && statusCode < 300) {
                             NSDictionary *postResponseData =
                             [NSJSONSerialization JSONObjectWithData:responseData
                                                             options:NSJSONReadingMutableContainers
                                                               error:NULL];
                             NSLog(@"[SUCCESS!] Created Tweet with ID: %@", postResponseData[@"id_str"]);
                         }
                         else {
                             NSLog(@"[ERROR] Server responded: status code %d %@", statusCode,
                                   [NSHTTPURLResponse localizedStringForStatusCode:statusCode]);
                         }
                     }
                     else {
                         NSLog(@"[ERROR] An error occurred while posting: %@", [error localizedDescription]);
                     }
                 };
                 
                 UIGraphicsBeginImageContextWithOptions(glView.bounds.size, NO, 1.0f);
                 
                 
                 //UIGraphicsBeginImageContextWithOptions(glView.bounds.size, NO, 1.0f);
                 UIImage *image = nil;
                 @try
                 {
                     [glView drawViewHierarchyInRect:glView.bounds afterScreenUpdates:NO];
                     image = UIGraphicsGetImageFromCurrentImageContext();
                 }
                 @catch(NSException* eeee)
                 {
                     [glView.layer renderInContext:UIGraphicsGetCurrentContext()];
                     image = [UIImage imageNamed:@"ios6newhighscore.png"];
                 }
                 

                 //[glView drawViewHierarchyInRect:glView.bounds afterScreenUpdates:NO];
                 
                 //UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
                 UIGraphicsEndImageContext();
                 
                 ACAccount *accounts = [arrayOfAccounts lastObject];
                
                 NSURL *url = [NSURL URLWithString:@"https://api.twitter.com"
                               @"/1.1/statuses/update_with_media.json"];
                 NSDictionary *params = @{@"status" : [NSString stringWithFormat:@"%@ https://itunes.apple.com/us/app/id%d %s",feed,APPLE_ID,TAG_LINE]};
                 SLRequest *request = [SLRequest requestForServiceType:SLServiceTypeTwitter
                                                         requestMethod:SLRequestMethodPOST
                                                                   URL:url
                                                            parameters:params];
                 NSData *imageData = UIImagePNGRepresentation(image);
                 [request addMultipartData:imageData
                                  withName:@"media[]"
                                      type:@"image/jpeg"
                                  filename:@"image.jpg"];
                 [request setAccount:accounts];
                 [request performRequestWithHandler:requestHandler];
                // });
                 
                 
                 
             }
             
             
         }
         else
         {
             
             // Handle failure to get account access
             if(error == 0)
             {
                 //old post;
             }
             NSLog(@"%@", [error localizedDescription]);
             
         }
         
     }];
    printf("\nEnd twitter");
    
    
}
-(void) PostInviteToTwitter:(NSString*) feed
{
    //[self capture_screenshot];
    if(![self userHasAccessToTwitter])
    {
          PineSocial::ShowMessageBox("Require Twitter log in to access! Check your device setting for loggin in and re-luanch the game again to active this feature.");
        //old post
        return;
    }
    if(tw_waccountStore == nil)
    {
        tw_waccountStore = [[ACAccountStore alloc] init];
    }
    // Asks for the Twitter accounts configured on the device.
    
    ACAccountType *accountType = [tw_waccountStore accountTypeWithAccountTypeIdentifier:ACAccountTypeIdentifierTwitter];
    
    [tw_waccountStore requestAccessToAccountsWithType:accountType options:nil completion:^(BOOL granted, NSError *error)
     {
         // If we have access to the Twitter accounts configured on the device we will contact the Twitter API.
         
         if (granted == YES)
         {
             // If there is a leat one account we will contact the Twitter API.
             NSArray *arrayOfAccounts = [tw_waccountStore accountsWithAccountType:accountType]; // Retrieves an array of Twitter accounts configured on the device.
             
             // If there is a leat one account we will contact the Twitter API.
             int count = [arrayOfAccounts count];
             printf("\nNum of Account %d",count);
             if (count > 0)
             {
                 SLRequestHandler requestHandler =
                 ^(NSData *responseData, NSHTTPURLResponse *urlResponse, NSError *error) {
                     if (responseData) {
                         NSInteger statusCode = urlResponse.statusCode;
                         if (statusCode >= 200 && statusCode < 300) {
                             NSDictionary *postResponseData =
                             [NSJSONSerialization JSONObjectWithData:responseData
                                                             options:NSJSONReadingMutableContainers
                                                               error:NULL];
                             NSLog(@"[SUCCESS!] Created Tweet with ID: %@", postResponseData[@"id_str"]);
                         }
                         else {
                             NSLog(@"[ERROR] Server responded: status code %d %@", statusCode,
                                   [NSHTTPURLResponse localizedStringForStatusCode:statusCode]);
                         }
                     }
                     else {
                         NSLog(@"[ERROR] An error occurred while posting: %@", [error localizedDescription]);
                     }
                 };
                 
                 //
                 
                 UIImage *image = [UIImage imageNamed:@"logotwitterinvite.png"];
                 UIGraphicsEndImageContext();
                 
                 ACAccount *accounts = [arrayOfAccounts lastObject];
                 
                 NSURL *url = [NSURL URLWithString:@"https://api.twitter.com"
                               @"/1.1/statuses/update_with_media.json"];
                 NSDictionary *params = @{@"status" : [NSString stringWithFormat:@"%@ https://itunes.apple.com/us/app/id%d %s",feed,APPLE_ID,TAG_LINE]};
                 SLRequest *request = [SLRequest requestForServiceType:SLServiceTypeTwitter
                                                         requestMethod:SLRequestMethodPOST
                                                                   URL:url
                                                            parameters:params];
                 NSData *imageData = UIImagePNGRepresentation(image);
                 [request addMultipartData:imageData
                                  withName:@"media[]"
                                      type:@"image/jpeg"
                                  filename:@"image.jpg"];
                 [request setAccount:accounts];
                 [request performRequestWithHandler:requestHandler];
                 
                 
                 
                 
             }
             
             
         }
         else
         {
             
             // Handle failure to get account access
             if(error == 0)
             {
                 //old post;
             }
             NSLog(@"%@", [error localizedDescription]);
             
         }
         
     }];
    printf("\nEnd twitter");
    
    
}

-(void) PostToTwitter:(NSString *)feed
{
    if(![self userHasAccessToTwitter])
    {
          PineSocial::ShowMessageBox("Require Twitter log in to access! Check your device setting for loggin in and re-luanch the game again to active this feature.");
        //old post
        return;
    }
    if(tw_waccountStore == nil)
    {
        tw_waccountStore = [[ACAccountStore alloc] init];
    }
    
    // Asks for the Twitter accounts configured on the device.
    
    ACAccountType *accountType = [tw_waccountStore accountTypeWithAccountTypeIdentifier:ACAccountTypeIdentifierTwitter];
    
    [tw_waccountStore requestAccessToAccountsWithType:accountType options:nil completion:^(BOOL granted, NSError *error)
     {
         // If we have access to the Twitter accounts configured on the device we will contact the Twitter API.
         
         if (granted == YES)
         {
             // If there is a leat one account we will contact the Twitter API.
             NSArray *arrayOfAccounts = [tw_waccountStore accountsWithAccountType:accountType]; // Retrieves an array of Twitter accounts configured on the device.
             
             // If there is a leat one account we will contact the Twitter API.
             int count = [arrayOfAccounts count];
             printf("\nNum of Account %d",count);
             if (count > 0) {
                 
                 ACAccount *twitterAccount = [arrayOfAccounts lastObject];
                 
                 NSDictionary *message = @{@"status": feed};
                 
                 NSURL *requestURL = [NSURL
                                      URLWithString:@"http://api.twitter.com/1/statuses/update.json"];
                 
                 SLRequest *postRequest = [SLRequest
                                           requestForServiceType:SLServiceTypeTwitter
                                           requestMethod:SLRequestMethodPOST
                                           URL:requestURL parameters:message];
                 
                 postRequest.account = twitterAccount;
                 
                 [postRequest performRequestWithHandler:^(NSData *responseData,
                                                          NSHTTPURLResponse *urlResponse, NSError *error)
                  {
                      NSLog(@"Twitter HTTP response: %i", [urlResponse 
                                                           statusCode]);
                  }];
                 
             }
             else
             {
                 //old post
             }
             
         }
         else
         {
             
             // Handle failure to get account access
             if(error == 0)
             {
                 //old post;
             }
             NSLog(@"%@", [error localizedDescription]);
             
         }
         
     }];
    printf("\nEnd twitter");
  
}

- (void) getProfileDetailsFromTwitter
{
    
}

/*
NSString* someText = self.textView.text;
NSArray* dataToShare = @[someText];  // ...or whatever pieces of data you want to share.

UIActivityViewController* activityViewController =
[[UIActivityViewController alloc] initWithActivityItems:dataToShare
                                  applicationActivities:nil];
[self presentViewController:activityViewController animated:YES completion:^{}];
*/
extern "C" void OS_LoadFriendTwitter()
{
     [[PineGameDelegate sharedInstance] GetFriends];
}
extern "C" void OS_InitTwitter()
{
    [[PineGameDelegate sharedInstance] TwitterInit];
}
extern "C" void OS_PostToTwitter(const char* feed)
{
    
    [[PineGameDelegate sharedInstance] PostToTwitter:[NSString stringWithFormat:@"%s",feed]];
}
extern "C" void OS_PostScreenToTwitter(const char* feed)
{
    [[PineGameDelegate sharedInstance] PostScreenToTwitter:[NSString stringWithFormat:@"%s",feed]];

}
extern "C" void OS_PostInviteToTwitter(const char* feed)
{
    [[PineGameDelegate sharedInstance] PostInviteToTwitter:[NSString stringWithFormat:@"%s",feed]];
    
}
extern "C" const char* OS_GetTwitterAvartar(INT_TIME id)
{
    
    NSString* url = [[PineGameDelegate sharedInstance] GetURLTwitterAvartar:[NSString stringWithFormat:@"%lld",id]];
    if(url == nil)
    {
        return NULL;
    }
    return OS_convertSStringTocharArray(url);
    
}
extern "C" bool OS_IsLogginTwitter()
{
    return [[PineGameDelegate sharedInstance] userHasAccessToTwitter];
}
