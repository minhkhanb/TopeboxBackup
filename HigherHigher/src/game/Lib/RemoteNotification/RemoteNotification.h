/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose. You are free to modify it and use it in any way you want,
** but you have to leave this header intact. Also if you find it useful,  
** a comment about it on blog.toshsoft.de would be nice.
**               
**
** RemoteNotification.h
** The main class which sends the push notification and
** prepares the payload.
**
** Author: Oliver Pahl
** -------------------------------------------------------------------------*/

#ifndef __REMOTE_NOTIFICATION_H
#define __REMOTE_NOTIFICATION_H

//#define IS_DEVELOPMENT_VERSION

extern "C" const  char* GetBundlePath();
extern "C" const char* GetRealFilePath(const char* filename);
#include "../../GameCore/CGame.h"
#include "SSLHelper.h"
/*
#define IS_DEVELOPMENT_VERSION 1
#define CA_CERT_PATH        (GetBundlePath())
#if defined(IS_DEVELOPMENT_VERSION)
    
    #define RSA_CLIENT_CERT     GetRealFilePath("apns-dev-cert.pem")
    #define RSA_CLIENT_KEY      GetRealFilePath("apns-dev-key.pem")

    
    #define APPLE_HOST          "gateway.sandbox.push.apple.com"
    #define APPLE_PORT          2195

    #define APPLE_FEEDBACK_HOST "feedback.sandbox.push.apple.com"
    #define APPLE_FEEDBACK_PORT 2196
#else
    
    #define RSA_CLIENT_CERT     GetRealFilePath("apns-production-cert.pem")
    #define RSA_CLIENT_KEY      GetRealFilePath("apns-production-key.pem")

    
    #define APPLE_HOST          "gateway.push.apple.com"
    #define APPLE_PORT          2195

    #define APPLE_FEEDBACK_HOST "feedback.push.apple.com"
    #define APPLE_FEEDBACK_PORT 2196
#endif
*/
#define DEVICE_BINARY_SIZE  32
#define MAXPAYLOAD_SIZE     256


typedef struct {
    /* The Message that is displayed to the user */
    char *message;

    /* The name of the Sound which will be played back */
    char *soundName;

    /* The Number which is plastered over the icon, 0 disables it */
    int badgeNumber;

    /* The Caption of the Action Key the user needs to press to launch the Application */
    char *actionKeyCaption;

    /* Custom Message Dictionary, which is accessible from the Application */
    char* dictKey[5];
    char* dictValue[5];
} Payload;

/* Initialize the payload with zero values */
void init_payload(Payload *payload);

/* Send a Notification to a specified iPhone */
int send_remote_notification(const char *deviceTokenHex, Payload *payload);

#endif
