#ifndef _GameBaseProtocal_H_
#define _GameBaseProtocal_H_

#import <FirebaseAnalytics/FIRApp.h>
#import <FirebaseAuth/FIRAuth.h>
#import <FirebaseAuth/FIRFacebookAuthProvider.h>
#import <FirebaseAuth/FIRUser.h>
#import <FirebaseDatabase/FIRDatabase.h>
#import <FirebaseDatabase/FIRDataSnapshot.h>



FIRDatabaseReference *_database_ref;
FIRAuth *_handleFIRAuth;
extern "C" void OS_TestCase(int type)
{
    FIRUser *user = [FIRAuth auth].currentUser;
    if (user) {
        // The user's ID, unique to the Firebase project.
        // Do NOT use this value to authenticate with your backend server,
        // if you have one. Use getTokenWithCompletion:completion: instead.
        NSString *uid = user.uid;
        NSString *email = user.email;
        NSURL *photoURL = user.photoURL;
        _database_ref = [[FIRDatabase database] reference];
        //create
        
        [[[_database_ref child:@"users"] child:user.uid]
         setValue:@{@"username": user.displayName}];

        // ...
    }
}
extern "C" void OS_InitFireBase()
{
    // Use Firebase library to configure APIs
    [FIRApp configure];

}
extern "C" void OS_InitDatabaseFireBase()
{
    _database_ref = [[FIRDatabase database] reference];
}
extern "C" void OS_OpenSessionFireBase()
{
    _handleFIRAuth = [[FIRAuth auth]
              addAuthStateDidChangeListener:^(FIRAuth *_Nonnull auth, FIRUser *_Nullable user) {
                  NSLog(@"new sesion with auth %@ user %@",auth,user);
                  // ...
              }];
}
extern "C" void OS_CloseSessionFireBase()
{
    [[FIRAuth auth] removeAuthStateDidChangeListener:_handleFIRAuth];
}

extern "C" void OS_AuthFacebookWithFireBase()
{
    int slot = 0;
    FBSDKAccessToken *token = [SUCache itemForSlot:slot].token;
    if (token) {
        // We have a saved token, issue a request to make sure it's still valid.
        [FBSDKAccessToken setCurrentAccessToken:token];
        FIRAuthCredential *credential = [FIRFacebookAuthProvider
                                     credentialWithAccessToken:[FBSDKAccessToken currentAccessToken].tokenString];
                                [[FIRAuth auth] signInWithCredential:credential
                              completion:^(FIRUser *user, NSError *error) {
                                  if (error) {
                                      // ...
                                      return;
                                  }
                                  
                                NSLog(@"User successfully signed in. Get user data from the FIRUser object");
                                  // ...
                              }];
    }
}

extern "C" void OS_SignOutFacebookFireBase()
{
    NSError *signOutError;
    BOOL status = [[FIRAuth auth] signOut:&signOutError];
    if (!status) {
        NSLog(@"Error signing out: %@", signOutError);
        return;
    }
}


#endif
