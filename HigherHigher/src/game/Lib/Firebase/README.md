# Firebase iOS SDKs

This directory contains the full Firebase distribution, packaged as static
frameworks that can be integrated into your app.

# Integration Instructions

Each Firebase component requires several frameworks in order to function
properly. Each section below lists the frameworks you'll need to include
in your project in order to use that Firebase SDK in your application.
The directions are based on Xcode 7. If you have a different version of
Xcode, the steps you need to take may differ slightly.

To integrate a Firebase SDK with your app:

1. Find the desired SDK in the list below.
2. Make sure you have an Xcode project open in Xcode.
3. In Xcode, hit `⌘-1` to open the Project Navigator pane. It will open on
   left side of the Xcode window if it wasn't already open.
4. Drag each framework from the "Analytics" directory into the Project
   Navigator pane. In the dialog box that appears, make sure the target you
   want the framework to be added to has a checkmark next to it, and that
   you've selected "Copy items if needed". If you already have Firebase
   frameworks in your project, make sure that you replace them with the new
   versions.
5. Drag each framework from the directory named after the SDK into the Project
   Navigator pane. Note that there may be no additional frameworks, in which
   case this directory will be empty. For instance, if you want the Database
   SDK, look in the Database folder for the required frameworks. In the dialog
   box that appears, make sure the target you want this framework to be added to
   has a checkmark next to it, and that you've selected "Copy items if needed."
6. If the SDK has resources, go into the Resources folders, which will be in
   the SDK folder. Drag all of those resources into the Project Navigator, just
   like the frameworks, again making sure that the target you want to add these
   resources to has a checkmark next to it, and that you've selected "Copy items
   if needed".
7. Add the -ObjC flag to "Other Linker Settings":
  a. In your project settings, open the Settings panel for your target
  b. Go to the Build Settings tab and find the "Other Linking Flags" setting
     in the Linking section.
  c. Double-click the setting, click the '+' button, and add "-ObjC" (without
     quotes)
8. Drag the `Firebase.h` header in this directory into your project. This will
   allow you to `#import "Firebase.h"` and start using any Firebase SDK that you
   have.
9. If you're using Swift, or you want to use modules, drag module.modulemap into
   your project and update your User Header Search Paths to contain the
   directory that contains your module map.
10. You're done! Compile your target and start using Firebase.

If you want to add another SDK, repeat the steps above with the frameworks for
the new SDK. You only need to add each framework once, so if you've already
added a framework for one SDK, you don't need to add it again. Note that some
frameworks are required by multiple SDKs, and so appear in multiple folders.

The Firebase frameworks list the system libraries and frameworks they depend on
in their modulemaps. If you have disabled the "Link Frameworks Automatically"
option in your Xcode project/workspace, you will need to add the system
frameworks and libraries listed in each Firebase framework's
<Name>.framework/Modules/module.modulemap file to your target's or targets'
"Link Binary With Libraries" build phase.

"(~> X)" below means that the SDK requires all of the frameworks from X. You
should make sure to include all of the frameworks from X when including the SDK.

## Analytics
  - FirebaseAnalytics.framework
  - FirebaseInstanceID.framework
  - GoogleInterchangeUtilities.framework
  - GoogleSymbolUtilities.framework
  - GoogleUtilities.framework
## AdMob (~> Analytics)
  - GoogleMobileAds.framework
## AppIndexing (~> Analytics)
  - FirebaseAppIndexing.framework
## Auth (~> Analytics)
  - FirebaseAuth.framework
  - GoogleNetworkingUtilities.framework
  - GoogleParsingUtilities.framework
## Crash (~> Analytics)
  - FirebaseCrash.framework
## Database (~> Analytics)
  - FirebaseDatabase.framework
## DynamicLinks (~> Analytics)
  - FirebaseDynamicLinks.framework
## Invites (~> Analytics)
  - FirebaseDynamicLinks.framework
  - FirebaseInvites.framework
  - GoogleAppUtilities.framework
  - GoogleAuthUtilities.framework
  - GoogleNetworkingUtilities.framework
  - GoogleParsingUtilities.framework
  - GooglePlusUtilities.framework
  - GoogleSignIn.framework

  You'll also need to add the resources in the
  Resources directory into your target's main
  bundle.
## Messaging (~> Analytics)
  - FirebaseMessaging.framework
  - GoogleIPhoneUtilities.framework
## RemoteConfig (~> Analytics)
  - FirebaseRemoteConfig.framework
  - GoogleIPhoneUtilities.framework
## Storage (~> Analytics)
  - FirebaseStorage.framework
  - GoogleNetworkingUtilities.framework

# Samples

You can get samples for Firebase from https://github.com/firebase/quickstart-ios:

    git clone https://github.com/firebase/quickstart-ios

# Versions

The frameworks in this directory map to these versions of the Firebase SDKs in
CocoaPods.

           CocoaPod           | Version
----------------------------- | -------
Firebase                      | 3.3.0
FirebaseAnalytics             | 3.2.1
FirebaseAppIndexing           | 1.0.3
FirebaseAuth                  | 3.0.3
FirebaseCrash                 | 1.0.6
FirebaseDatabase              | 3.0.2
FirebaseDynamicLinks          | 1.1.0
FirebaseInstanceID            | 1.0.7
FirebaseInvites               | 1.1.0
FirebaseMessaging             | 1.1.0
FirebaseRemoteConfig          | 1.1.1
FirebaseStorage               | 1.0.2
Google-Mobile-Ads-SDK         | 7.8.1
GoogleAppUtilities            | 1.1.1
GoogleAuthUtilities           | 2.0.1
GoogleIPhoneUtilities         | 1.2.1
GoogleInterchangeUtilities    | 1.2.1
GoogleNetworkingUtilities     | 1.2.1
GoogleParsingUtilities        | 1.1.1
GooglePlusUtilities           | 1.1.1
GoogleSignIn                  | 4.0.0
GoogleSymbolUtilities         | 1.1.1
GoogleUtilities               | 1.3.1

