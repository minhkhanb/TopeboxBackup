#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <Foundation/NSPathUtilities.h>
#import <Foundation/NSUserDefaults.h>
#import "PineGameDelegate.h"
#import <UIKit/UIKit.h>
#if ADD_PINE_HIGHT_SCORE
#import "PineHighScoreContext.h"

#endif
//#import <GameKit/GameKit.h>
#import <Foundation/NSThread.h>
//#import "NSData+MD5.h"
#import <SystemConfiguration/SystemConfiguration.h>
#import "NSDataEncryption.h"
#include <pthread.h>
#include <sys/time.h>
#include "Defines.h"
#include "CGame.h"

#import <sys/xattr.h>
#include "../GameCore/Os_Functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#include <chrono>
#define IFT_ETHER   0x6     /* Ethernet CSMACD */
//"http://youarmy-sea.cloudapp.net/api/youarmy/"
//http://odata.pine-entertainment.com/api/youarmy/
//http://youarmy.pine-entertainment.com/api/youarmy/
#define APPLE_LINK @"itms-apps://itunes.apple.com/us/app/idAPP_ID?mt=8"
//#define URL_SERVER  "http://odata.pine-entertainment.com/api/youarmy/"
//#define MY_SERCET_PASSWORD  @"pineEntertainment"

#include "OpenALSoundManager.h"

#include <PineEngine.h>
PE_USING_ENGINE;

extern "C" void EAGLViewBeginRender();
extern "C" void EAGLViewEndRender();
extern "C" Orientation GetPhoneOrientation();
//extern "C" bool HasGCAuthenticated();
extern "C" Device CurrentDevice = {};
char _str_url_resoucre[10240];
extern "C" float view_pixel_scalex = 1.0f;
extern "C" float view_pixel_scaley = 1.0f;
float touch_scale_w = 1.0f;
float touch_scale_h = 1.0f;
extern "C" void OS_GetResourceFilePathToChar(const char* filename, char* output);
extern "C" void OpenAppStore(INT64 appleID)
{
    NSString *reviewURL = [APPLE_LINK stringByReplacingOccurrencesOfString:@"APP_ID" withString:[NSString stringWithFormat:@"%lld", appleID]];
    NSLog(@"%@",reviewURL);
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:reviewURL]];
}
extern "C" void OS_LinkToRatingApp(unsigned long apple_id)
{
    NSString *reviewURL = [APPIRATER_TPL_URL stringByReplacingOccurrencesOfString:@"APP_ID" withString:[NSString stringWithFormat:@"%lu", apple_id]];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:reviewURL]];
}
extern "C" bool HasAppInstalled(const char* boudle_id)
{
    NSString* strUrl = [ NSString stringWithFormat:@"myapp:%s",boudle_id];
    //[[UIApplication sharedApplication] openURL:[NSURL URLWithString:strUrl]];
    NSLog(@"\n strUrl strUrl strUrl ret = %@ \n", strUrl);
    if ([[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:strUrl]])
    {
         NSLog(@"\n strUrl strUrl strUrl ret = %@ \n", strUrl);
        return 1;
    }
    return 0;
}
extern "C" char* getMacAddress(char* macAddress, char* ifName)
{
    int  success;
    struct ifaddrs * addrs;
    struct ifaddrs * cursor;
    const struct sockaddr_dl * dlAddr;
    const unsigned char* base;
    int i;
    success = getifaddrs(&addrs) == 0;
    if (success) {
        cursor = addrs;
        while (cursor != 0) {
            if ( (cursor->ifa_addr->sa_family == AF_LINK)
                && (((const struct sockaddr_dl *) cursor->ifa_addr)->sdl_type == IFT_ETHER) && strcmp(ifName,  cursor->ifa_name)==0 ) {
                dlAddr = (const struct sockaddr_dl *) cursor->ifa_addr;
                base = (const unsigned char*) &dlAddr->sdl_data[dlAddr->sdl_nlen];
                strcpy(macAddress, "");
                for (i = 0; i < dlAddr->sdl_alen; i++) {
                    /*if (i != 0) {
                     strcat(macAddress, ":");
                     }*/
                    char partialAddr[3];
                    sprintf(partialAddr, "%02X", base[i]);
                    strcat(macAddress, partialAddr);
                    
                }
            }
            cursor = cursor->ifa_next;
        }
        
        freeifaddrs(addrs);
    }
    return macAddress;
}

/*extern "C" NSString* getStrDec(const char* GC_ID)
 {
 NSString* ret = [NSString alloc];
 char* macAddressString= (char*)malloc(18);
 NSString* macAddress= [[NSString alloc] initWithCString:getMacAddress(macAddressString,"en0")
 encoding:NSMacOSRomanStringEncoding];
 NSString* NS_GC = [[NSString alloc] initWithUTF8String:GC_ID];
 //NSLog(@"\n NS_GC = %@, _gc_id = %s\n",NS_GC, GC_ID);
 NSData *result = [[PineGameDelegate sharedInstance] encryptString:NS_GC withKey:MY_SERCET_PASSWORD ];
 NSString* newStr = [[PineGameDelegate sharedInstance] base64forData:result];
 ret = [[NSString alloc] initWithFormat:@"%@:%@", macAddress, newStr];
 
 //NSLog(@"\n ret = %@ \n", ret);
 ret = [[PineGameDelegate sharedInstance] base64Encode:ret];
 // NSLog(@"\n retStrDec = %@\n", ret);
 return ret;
 }*/




extern "C" unsigned long OS_GetTime()
{
    //std::time_t time_now = std::chrono::system_clock::now().time_since_epoch().count();
    
    std::chrono::high_resolution_clock clock;
    return std::chrono::duration_cast<std::chrono::nanoseconds>(clock.now().time_since_epoch()).count();

}
extern "C" const  char* GetBundlePath()
{
    NSString* real_path = [[NSBundle mainBundle] executablePath];
    
    const char* ret = [real_path UTF8String];
    
    return ret;
    
}
extern "C" const char* GetAppDataUrl()
{
    NSBundle *bundle = [NSBundle mainBundle];
    
    NSString *path = [bundle bundlePath];
    const char* ret = [path UTF8String];
    
    return ret;
}
extern "C" const char* GetDocumentUrl()
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    const char* ret = [documentsDirectory UTF8String];
    
    return ret;
}
extern "C" const char* GetRealFileFullPath(const char* filename,const char* dir)
{
    NSString* logic_path = [[NSString alloc] initWithUTF8String: filename];
    NSString* d = [[NSString alloc] initWithUTF8String: dir];
    //NSLog(@"%@, %@",logic_path,d);
    NSString* real_path = [[NSBundle mainBundle] pathForResource: logic_path ofType: @"" inDirectory:d];
    //NSLog(@"%@",real_path);
    const char* ret = [real_path UTF8String];
    
    [d release];
    [logic_path release];
    [real_path release];
    return ret;
}
extern "C" char* OS_GetResourceFilePath(const char* filename)
{
    NSString* logic_path = [[NSString alloc] initWithUTF8String: filename];
    NSString* real_path = [[NSBundle mainBundle] pathForResource: logic_path ofType: @""];
    
    if(real_path != nil)
    {
        const char* ret = [real_path UTF8String];
    
        [logic_path release];
        [real_path release];
        char* retstr = new char[strlen(ret) + 1];
        sprintf(retstr,"%s", ret);
        return retstr;
    }
    return NULL;
}
extern "C" const char* GetRealFilePath(const char* filename)
{
    NSString* logic_path = [[NSString alloc] initWithUTF8String: filename];
    NSString* real_path = [[NSBundle mainBundle] pathForResource: logic_path ofType: @""];
    if(real_path != nil)
    {
        const char* ret = [real_path UTF8String];
    
        [logic_path release];
        [real_path release];
        return ret;
    }
    return NULL;
}

extern "C" char* OS_GetDocumentFilePath(const char* filename)
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    NSString* fname = [[NSString alloc] initWithUTF8String: filename];
    NSString *appfilename = [documentsDirectory stringByAppendingPathComponent:fname];
    
    //const char* ret = [appfilename UTF8String];
    if(appfilename != nil)
    {
        sprintf(_str_url_resoucre,"%s",[appfilename UTF8String]);
        [fname release];
        //[appfilename release];
        [documentsDirectory release];
        [paths release];
        //return [appfilename UTF8String];
        char* retstr = new char[strlen(_str_url_resoucre) + 1];
        sprintf(retstr,"%s", _str_url_resoucre);
        return retstr;
    }
    return NULL;
}

extern "C" ResourceItem OS_GetResourceItem(const char* filename){
    
#if USE_COMPRESSOR_DATA_INGAME
    char* sFile;
    char* real_path = new char[1024];
    //Compressor_Exporter::_zipcompress_file[i*TOTAL__ZIPCOMPRESS_FILE+_ZIPCOMPRESS_FILE_PACK_SHORT_ZIP];
    int index = Device::GetPackCompressed(filename);
    if(index >= 0)
    {
        
        bool exitsfile = OS_IsDocumentFileExist(Device::GetPackShortNameCompressed(index));
        bool isLastFile =  false;
        if(exitsfile)
        {
            char name[256];
            sprintf(name,"%s",Device::GetPackFullNameCoompressed(index));
            time_t t = Device::GetTimeModifyOfFile(OS_GetResourceFilePath(name));
            sprintf(name,"log_%ld_%s",t,Device::GetPackShortNameCompressed(index));
             //printf("\nLog 0: %s",name);
            if(Device::HasAppInstalled(name,GAME_VERSION))
            {
                isLastFile = true;
            }
            //printf("\n File: %s, time modify date:%ld",name,t);
        }
        if(!exitsfile || !isLastFile)
        {
            std::string str_archive(Device::GetPackFullNameCoompressed(index));
            std::wstring archive_file(str_archive.begin(), str_archive.end());
            std::wstring archive_path = OS::findResourcePath(archive_file, EFileLocation::APP_DIR);
            
            std::string str_output(Device::GetPackShortNameCompressed(index));
            std::wstring output_file(str_output.begin(), str_output.end());
            std::wstring output_path = OS::findResourcePath(output_file, EFileLocation::DOCUMENT_DIR);
            
            std::string str_identify(Device::GetPackShortNameCompressed(index));
            
            
            if(Device::PINEZ_Extractor(archive_path, str_identify, output_path))
            {
            /*if(Device::PINEZ_Extractor(OS_GetResourceFilePath(Device::GetPackFullNameCoompressed(index)),
                                Device::GetPackShortNameCompressed(index),
                                OS_GetDocumentFilePath(Device::GetPackShortNameCompressed(index))))
            {*/
                char name[256];
                sprintf(name,"%s",Device::GetPackFullNameCoompressed(index));
                time_t t = Device::GetTimeModifyOfFile(OS_GetResourceFilePath(name));
                sprintf(name,"log_%ld_%s",t,Device::GetPackShortNameCompressed(index));
                //printf("\nLog 1: %s",name);
                Device::SetAppInstalled(name,GAME_VERSION);
                OS::setFileAttribute(output_path, EFileLocation::PATH, EFileAttribute::ARCHIVE, false);
                
            }
        }
        sFile = OS_GetDocumentFilePath(Device::GetPackShortNameCompressed(index));
        sprintf(real_path,"%s",sFile);
        
    }
    else
    {
         OS_GetResourceFilePathToChar(filename, real_path);
    }
    
#else
#ifdef MAC_OS
    char* real_path = new char[1024];
    OS_GetResourceFilePathToChar(filename, real_path);
#else
    real_path = OS_GetResourceFilePath(filename);
#endif
#endif
    
    ResourceItem resitem(real_path,0);
    FILE* file = fopen(real_path,"rb");
    if (file != NULL){
        fseek(file, 0, SEEK_END);
        resitem.Length = ftell(file);
        fclose(file);
    }
    resitem.Offset = 0;
    delete[] real_path;
    return resitem;
}
/*
 extern "C" bool SaveAppData(const char* filename, const char* data_to_save, int data_length)
 {
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	
	if (!documentsDirectory)
	{
 NSLog(@"Documents directory not found!");
 return NO;
 }
	
	NSString* fname = [[NSString alloc] initWithUTF8String: filename];
	NSString *appfilename = [documentsDirectory stringByAppendingPathComponent:fname];
	
	NSData *appdata = [[NSData alloc] initWithBytes: data_to_save length: data_length];
	
	return ([appdata writeToFile:appfilename atomically:YES]);
 }
 */

extern "C" bool OS_SaveAppData(const char* filename, const char* data_to_save, int data_length, bool is_append)
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    if (!documentsDirectory)
    {
        NSLog(@"Documents directory not found!");
        return NO;
    }
    
    NSString* fname = [[NSString alloc] initWithUTF8String: filename];
    NSString *appfilename = [documentsDirectory stringByAppendingPathComponent:fname];
    [fname release];
    FILE* stream = NULL;
    if (is_append)
    {
        stream = fopen([appfilename UTF8String], "ab");
    }
    else
    {
        stream = fopen([appfilename UTF8String], "wb");
    }
    
    if (stream != NULL)
    {
        fwrite(data_to_save, sizeof(BYTE), data_length, stream);
    }
    else
    {
        return false;
    }
    
    fclose(stream);
    
    return true;
}

extern "C" Buffer* OS_LoadAppData(const char* filename)
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    if (!documentsDirectory)
    {
        NSLog(@"Documents directory not found!");
        return NULL;
    }
    
    NSString* fname = [[NSString alloc] initWithUTF8String:filename];
    NSString *appfilename = [documentsDirectory stringByAppendingPathComponent:fname];
    [fname release];
    NSData *appdata = [[[NSData alloc] initWithContentsOfFile:appfilename] autorelease];
    
    int size = [appdata length];
    Buffer* buff = new Buffer(size);
    //readed_data = new char[size];
    //char* tmp = new char[size];
    [appdata getBytes:buff->Data() length:size];
    
    return buff;
}
extern "C" const char* GetFullDocumentURL(const char* file_name)
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    if (!documentsDirectory)
    {
        NSLog(@"Documents directory not found!");
        return 0;
    }
    
    NSString* fname = [[NSString alloc] initWithUTF8String:file_name];
    NSString *appfilename = [documentsDirectory stringByAppendingPathComponent:fname];
    const char* ret = [appfilename UTF8String];
    
    [fname release];
    
    return ret;
}
extern "C" int GetSizeFile(const char *file_name)
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    if (!documentsDirectory)
    {
        NSLog(@"Documents directory not found!");
        return 0;
    }
    
    NSString* fname = [[NSString alloc] initWithUTF8String:file_name];
    NSString *appfilename = [documentsDirectory stringByAppendingPathComponent:fname];
    [fname release];
    NSData *appdata = [[[NSData alloc] initWithContentsOfFile:appfilename] autorelease];
    
    int size = [appdata length];
    return size;
    
}
extern "C" int ReadAppDataBinary(const char* filename, BYTE* readed_data)
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    if (!documentsDirectory)
    {
        NSLog(@"Documents directory not found!");
        return 0;
    }
    
    NSString* fname = [[NSString alloc] initWithUTF8String:filename];
    NSString *appfilename = [documentsDirectory stringByAppendingPathComponent:fname];
    [fname release];
    NSLog(@"%@",appfilename);
    NSData *appdata = [[[NSData alloc] initWithContentsOfFile:appfilename] autorelease];
    
    int size = [appdata length];
    printf("\n%d",size);
    //readed_data = new BYTE[size];
    //char* tmp = new char[size];
    [appdata getBytes:readed_data length:size];
    
    return size;
}
extern "C" bool ExitsFileFromUrl(const char* url)
{
    //printf("\nCheck Exists file: %s",url);
    FILE *fp;
    fp = fopen(url,"rb");
    if(fp)
    {
        //printf(" Ton Tai\n");
        fclose(fp);
        return true;
    }
    // printf("Khong Ton Tai\n");
    return false;
    
}
extern "C" bool ExitsFileFromApp(const char *file_name)
{
    
    
    NSString* fileTodel = [NSString stringWithFormat:@"%s" , OS_GetResourceFilePath(file_name)];
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if ([fileManager fileExistsAtPath:fileTodel]){
        return true;
    }
    return false;
    
    /*
     const char* sFile = GetRealFilePath(file_name);
     FILE *fp;
     fp = fopen(sFile,"rb");
     if(fp)
     {
     fclose(fp);
     return true;
     }
     return false;
     
     */
}
extern "C" bool OS_AddSkipBackupAttributeToItemAtURL(const char* file)
{
    NSString* fileTodel = [NSString stringWithFormat:@"%s" , file];
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectoryPath = [paths objectAtIndex:0];
    NSString *databaseFile = [documentsDirectoryPath stringByAppendingPathComponent:fileTodel];
    NSURL *URL = [NSURL fileURLWithPath:databaseFile];
    //NSURL *URL = [NSURL URLWithString:[databaseFile stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    
    assert([[NSFileManager defaultManager] fileExistsAtPath: [URL path]]);
    
    NSError *error = nil;
    bool success = [URL setResourceValue: [NSNumber numberWithBool: YES]
                                  forKey: NSURLIsExcludedFromBackupKey error: &error];
    if(!success){
        NSLog(@"Error excluding %@ from backup %@", [URL lastPathComponent], error);
    }
    return success;
}
extern "C" bool ExitsFile(const char *file_name)
{
    NSString* fileTodel = [NSString stringWithFormat:@"%s" , file_name];
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectoryPath = [paths objectAtIndex:0];
    NSString *databaseFile = [documentsDirectoryPath stringByAppendingPathComponent:fileTodel];
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if ([fileManager fileExistsAtPath:databaseFile]){
        return true;
    }
    return false;
    
}
extern "C" bool	OS_IsDocumentFileExist(const char* filename)
{
    return ExitsFile(filename);
}

extern "C" void OS_DeleteFile(const char *file_name);
extern "C" void DownloadFile(const char* url, const char* file_name,int time_out)
{
    INT_TIME t = time(NULL);
    printf("\ndownload file %s save to %s",url,file_name);
    NSString* fileToSaveTo = [NSString stringWithFormat:@"%s" , file_name];
    
    NSString* fileURL = [NSString stringWithFormat:@"%s" , url];
    NSArray* path = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
    NSString* documentsDirectory = [path objectAtIndex:0];
    NSData* data = [NSData dataWithContentsOfURL:[NSURL URLWithString:fileURL]];
    BOOL sucess = [data writeToFile:[NSString stringWithFormat:@"%@/%@",documentsDirectory,fileToSaveTo] atomically:YES];
    
    while(!ExitsFile(file_name) && (time(NULL)-t)<time_out){
        ;
        printf("\nCheck Exits file");
        sleep(1);
    }
    if(!sucess)
    {
        OS_DeleteFile(file_name);
    }
    printf("\ndownload complete");
}

extern "C" bool OS_DownloadFile(const char* url, const char* file_name,int time_out)
{
    INT_TIME t = time(NULL);
    printf("\ndownload file %s save to %s",url,file_name);
    NSString* fileToSaveTo = [NSString stringWithFormat:@"%s" , file_name];
    
    NSString* fileURL = [NSString stringWithFormat:@"%s" , url];
    NSArray* path = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
    NSString* documentsDirectory = [path objectAtIndex:0];
    NSData* data = [NSData dataWithContentsOfURL:[NSURL URLWithString:fileURL]];
    BOOL sucess = [data writeToFile:[NSString stringWithFormat:@"%@/%@",documentsDirectory,fileToSaveTo] atomically:YES];
    
    while(!ExitsFile(file_name) && (time(NULL)-t)<time_out){
        ;
        printf("\nCheck Exits file");
        sleep(1);
    }
    if(!sucess)
    {
        OS_DeleteFile(file_name);
    }
    printf("\ndownload complete");
    return sucess;
}

/*extern "C" void DeleteFile(const char *file_name)
 {
 
 NSString* fileTodel = [NSString stringWithFormat:@"%s" , file_name];
 NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectoryPath = [paths objectAtIndex:0];
 NSString *databaseFile = [documentsDirectoryPath stringByAppendingPathComponent:fileTodel];
 
 NSFileManager *fileManager = [NSFileManager defaultManager];
 if ([fileManager fileExistsAtPath:databaseFile]){
 [fileManager removeItemAtPath:databaseFile error:NULL];
 printf("\nDeleted file %s",file_name);
 }
 }*/

extern "C" void OS_DeleteFile(const char *file_name)
{
    
    NSString* fileTodel = [NSString stringWithFormat:@"%s" , file_name];
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectoryPath = [paths objectAtIndex:0];
    NSString *databaseFile = [documentsDirectoryPath stringByAppendingPathComponent:fileTodel];
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if ([fileManager fileExistsAtPath:databaseFile]){
        [fileManager removeItemAtPath:databaseFile error:NULL];
        printf("\nDeleted file %s",file_name);
    }
}
extern "C" bool OS_DeleteAppData(const char *file_name)
{
    
    NSString* fileTodel = [NSString stringWithFormat:@"%s" , file_name];
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectoryPath = [paths objectAtIndex:0];
    NSString *databaseFile = [documentsDirectoryPath stringByAppendingPathComponent:fileTodel];
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if ([fileManager fileExistsAtPath:databaseFile]){
        [fileManager removeItemAtPath:databaseFile error:NULL];
        printf("\nDeleted file %s",file_name);
    }
    return true;
}

/*
 extern "C" int ReadAppData(const char* filename, char* &readed_data)
 {
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	
	if (!documentsDirectory)
	{
 NSLog(@"Documents directory not found!");
 return 0;
 }
	
	NSString* fname = [[NSString alloc] initWithUTF8String:filename];
	NSString *appfilename = [documentsDirectory stringByAppendingPathComponent:fname];
	
	NSData *appdata = [[[NSData alloc] initWithContentsOfFile:appfilename] autorelease];
	
	int size = [appdata length];
	readed_data = new char[size];
	
	[appdata getBytes:readed_data length:size];
	
	return size;
 }
 */
extern "C" bool DeleteAppData(const char* filename)
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    if (!documentsDirectory)
    {
        NSLog(@"Documents directory not found!");
        return NO;
    }
    
    NSString* fname = [[NSString alloc] initWithUTF8String: filename];
    NSString *appfilename = [documentsDirectory stringByAppendingPathComponent:fname];
    [fname release];
    if (remove([appfilename UTF8String]) != 0)
    {
        return false;
    }
    
    return true;
}



extern "C" const char* OS_GetDeviceUserAgent()
{
    UIWebView* webView = [[UIWebView alloc]initWithFrame:CGRectZero];
    
    NSString* userAgent = [[NSString alloc] initWithFormat:@"%@", [webView stringByEvaluatingJavaScriptFromString:@"navigator.userAgent"]];
    
    [webView release];
    
    return [userAgent UTF8String];
}

bool g_isAppExit = false;
CGame* g_pGame = NULL;
Orientation last_orientation = Orientation::Portrait;
OpenALSoundManager* sound_mng;
//init game
extern "C" void GameInit()
{
    /*
     NSLog(@"device model: %@", [[UIDevice currentDevice] model]);
     NSLog(@"device name:%@",[[UIDevice currentDevice] name]);
     NSLog(@"device system name:%@",[[UIDevice currentDevice] systemName]);
     NSLog(@"device system version:%@",[[UIDevice currentDevice] systemVersion]);
     NSLog(@"device localized model:%@",[[UIDevice currentDevice] localizedModel]);
     NSLog(@"device identifierForVendor:%@",[[UIDevice currentDevice] identifierForVendor]);
     //*/
    
    //set model
    NSString *device_model = [NSString stringWithFormat:@"%@;%@;%@;",[[UIDevice currentDevice] model],[[UIDevice currentDevice] systemName],[[UIDevice currentDevice] systemVersion]];
    CurrentDevice.setDeviceModel([device_model UTF8String]);
    
    //set agent
   	//CurrentDevice.setDeviceAgent(OS_GetDeviceUserAgent());
   
    CurrentDevice.DisplayProperties.ColorDepth = PineDisplayColorDepth::ColorDepth32Bit;
    //game init

    touch_scale_h *= view_pixel_scalex;
    touch_scale_w *= view_pixel_scaley;
    printf("set touch scale w:%0.2f h:%0.2f\n", view_pixel_scalex, view_pixel_scaley);

    g_pGame = new CGame();
    last_orientation = GetPhoneOrientation();
    g_pGame->InitDevice(CurrentDevice);
    g_pGame->OnChangeOrientation(last_orientation);
    sound_mng = new OpenALSoundManager();
    g_pGame->SetSoundEngine(sound_mng);
    g_pGame->Init();
}
extern "C" void EndGame()
{
    g_isAppExit = true;
}
//run

extern "C" void GameRun()
{
    bool need_change_ori = false;
    Orientation ori = GetPhoneOrientation();
    if(ori != last_orientation){
        need_change_ori = true;
    }
    if(g_pGame->_isPause)
    {
        //g_pGame->UnloadGameBackground();
    }
    if(g_pGame != nil && !g_pGame->_isPause)
    {
        EAGLViewBeginRender();
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
#else
        if(need_change_ori){
#if !USE_ONLY_SUPPORT_LANDCAPE_MODE
            if (ori == Orientation::Landscape_90)
            {
                [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(setBarLandscapeRight:) withObject:nil waitUntilDone:YES];
                
            }
            else if (ori == Orientation::Landscape_270)
            {
                [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(setBarLandscapeLeft:) withObject:nil waitUntilDone:YES];
            }
            else if (CGame::_this->_is_portrail_exception && ori == Orientation::Portrait)
            {
                [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(setBarPortrait:) withObject:nil waitUntilDone:YES];
            }
            //last_orientation = ori;
            
            
            g_pGame->OnChangeOrientation(ori);
            
            last_orientation = ori;
#endif
        }
#endif
        g_pGame->Run();
        
        if (g_pGame->_isExit == true)
        {
            g_isAppExit = true;
        }
        if(!g_pGame->_isPause)
        {
            EAGLViewEndRender();
        }
    }
    
    
}

//pause
extern "C" void GamePause()
{
    //sleep(3);
    g_pGame->Pause();
}
extern "C" bool EnableNotification();
//resume
extern "C" void GameResume()
{
    if(sound_mng != NULL) sound_mng->endInterruption();
    if (g_pGame != NULL)
    {
        //OS_NOTIFY().Reset_Notify();
        g_pGame->_notificationEnable = EnableNotification();
        g_pGame->Resume();
        //g_pGame->ClearAllTouch();
        
    }
}

void JoinThread();

//end game
extern "C" void GameExit()
{
    //release res
    g_pGame->Exit();
    g_pGame->Release();
    SAFE_DEL(g_pGame);
    g_isAppExit = true;
    JoinThread();
}

pthread_t g_thread_id;
pthread_t g_serverthread_id;
pthread_t downloadThread_id;
pthread_t advertThread_id;
pthread_t fb_thread_id;
void* ThreadRun(void* param);
void* ServerTheadRun(void* param);
void* downloadInfo(void *param);
void* downloadGameData(void *param);
void *ServerADVERTRun(void* param);
void* FBServerThreadRun(void* param);
static bool _server_ads_started = false;
extern "C" void ServerADVERTStart()
{
    if(!_server_ads_started)
    {
        _server_ads_started = true;
        if (pthread_create(&advertThread_id, NULL, &ServerADVERTRun, NULL))
        {
            exit(1);
        }
    }
}

extern "C" void DownloadGameInfo()
{
    //[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(downloadGameInfo:) withObject:nil waitUntilDone:YES];
    
    if (pthread_create(&downloadThread_id, NULL, &downloadInfo, NULL))
    {
        exit(1);
    }
    
}

extern "C" void DownloadGameData()
{
    
    if (pthread_create(&downloadThread_id, NULL, &downloadGameData, NULL))
    {
        exit(1);
    }
    
    //[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(downloadGameData:) withObject:nil waitUntilDone:YES];
    
}

extern "C" void StartServerTheard()
{
    if (pthread_create(&g_serverthread_id, NULL, &ServerTheadRun, NULL))
    {
        exit(1);
    }
    
}
extern "C" void StartThread()
{
    if (pthread_create(&g_thread_id, NULL, &ThreadRun, NULL))
    {
        exit(1);
    }
}

extern "C" void StartFBThread()
{
    if (pthread_create(&fb_thread_id, NULL, &FBServerThreadRun, NULL))
    {
        exit(1);
    }
}

void* downloadInfo(void *param)
{
#if FREE_GAME_OF_THEDAY
    NSAutoreleasePool*		_pool = [NSAutoreleasePool new];
    CGame::_this->LoadGameFreeInfo();
    [_pool release];
#endif
    
    return 0;
}
void* downloadGameData(void *param)
{
#if FREE_GAME_OF_THEDAY
    NSAutoreleasePool*		_pool = [NSAutoreleasePool new];
    CGame::_this->CheckAndDownload();
    [_pool release];
#endif
    return 0;
}
void *ServerADVERTRun(void* param)
{
    NSAutoreleasePool*		_pool = [NSAutoreleasePool new];
#if ADD_ADVERT
    ADVERT().ServerStart();
#endif
    [_pool release];
    return 0;
}
void* ServerTheadRun(void* param)
{
    //CGame::_this->ServerStart();
    CGame::_this->ServerRun();
    
    
    return 0;
}

void* FBServerThreadRun(void* param)
{
    //CGame::_this->FBServerStart();
    //CGame::_this->FBServerRun();
    
    return 0;
}

void* ThreadRun(void* param)
{
    //init game
    //NSAutoreleasePool*		_pool = [NSAutoreleasePool new];
    
    GameInit();
    
    while (g_isAppExit == false)
    {
        GameRun();
    }
    
    GameExit();
    //[_pool release];
    return 0;
}

void JoinThread()
{
    if (pthread_join(g_thread_id, NULL))
    {
        exit(1);
    }
    exit(0);
}


extern "C" void UpdateTouchBegan(int touchId, int x, int y)
{
    //printf("\n touch begin x = %d, y =%d",x,y);
    if(g_pGame == NULL)return;
    x *= touch_scale_w;
    y *= touch_scale_h;
    //printf("\ntouch scale: x:%0.2f y:%0.2f", touch_scale_w, touch_scale_h);
    //printf("\n touch begin 2: x = %d, y =%d",x,y);
    g_pGame->NotifyTouchesBegan(touchId, x, y, 1);
}

extern "C" void UpdateTouchEnded(int touchId, int x, int y)
{
    if(g_pGame == NULL)return;
    x *= touch_scale_w;
    y *= touch_scale_h;
    
    short tmp;
    //call process multi touch
    
    g_pGame->NotifyTouchesEnd(touchId, x, y, 1);
}

extern "C" void UpdateTouchMoved(int touchId, int x, int y)
{
    if(g_pGame == NULL)return;
    x *= touch_scale_w;
    y *= touch_scale_h;
    //call process multi touch
    g_pGame->NotifyTouchesMoved(touchId, x, y, 1);
}

extern "C" void UpdateTouchCancel(unsigned char touchId, int x, int y)
{
	if(g_pGame == NULL)return;
    x *= touch_scale_w;
    y *= touch_scale_h;
    //call process multi touch
    g_pGame->NotifyTouchesEnd(touchId, x, y, 1);
	g_pGame->NotifyTouchesCancel(touchId);
}


extern "C" int GetGameRotate()
{
    /*
     //UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
     //if (orientation == UIDeviceOrientationLandscapeLeft)
     //if(GetPhoneOrientation() == 2)
     {
     //	return 1;
     }
     //else if (orientation == UIDeviceOrientationLandscapeRight)
     {
     //	return 0;
     
     }
     // return 0;
     */
    return 0;
}

extern "C" void SetBarLandscapeLeft()
{
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(setBarLandscapeLeft:) withObject:nil waitUntilDone:YES];
}
extern "C" void SetBarLandscapeRight()
{
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(setBarLandscapeRight:) withObject:nil waitUntilDone:YES];
}

extern "C" bool EnableNotification()
{
    
    return true;
}

extern "C" void OS_sleep(int iTime)
{
    if(iTime > 0)
    {
        [NSThread sleepForTimeInterval:iTime*0.001f];
    }
}

extern "C" int getMinuteLastTimeUpdate_extern(INT_TIME userID, INT_TIME gameID)
{
    
}
extern "C" int storeToResultFromFriendList_extern(int type_score, BYTE type_time, bool isAscending)
{
    
}


/*extern "C" bool OS_PingToHost(const char* url,int time_out)
 {
 NSString* str = [NSString stringWithUTF8String:url];
 
 NSURL *url_host = [[NSURL alloc] initWithString:str];
	NSURLRequest *lRequest = [NSURLRequest requestWithURL:url_host
 cachePolicy:NSURLRequestUseProtocolCachePolicy
 timeoutInterval:time_out];
 if (![NSURLConnection sendSynchronousRequest:lRequest returningResponse:nil error:nil])
 {
 return false;
 }
 return true;
 }*/

#if OPENGLES_20_SUPPORT
extern "C" bool ReadFile(uint8_t* buffer, int bufferSize, NSString* filename)
{
    memset(buffer, 0, bufferSize);
    
    NSString* pathStr = [[NSString alloc] initWithFormat:@"%@/%@", [[NSBundle mainBundle] resourcePath], filename];
    const char* path = [pathStr cStringUsingEncoding:NSASCIIStringEncoding];
    FILE* fin = fopen(path, "ra");
    if (fin == NULL)
        return false;
    [pathStr release];
    
    fread(buffer, 1, bufferSize, fin);
    
    fclose(fin);
    return true;
}

extern "C" int LoadShader (GLenum type, const uint8_t* source)
{
    const unsigned int shader = glCreateShader(type);
    if (shader == 0)
        return 0;
    
    glShaderSource(shader, 1, (const GLchar**)&source, NULL);
    glCompileShader(shader);
    
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == 0)
    {
        char errorMsg[2048];
        glGetShaderInfoLog(shader, sizeof(errorMsg), NULL, errorMsg);
        printf("Compile error: %s\n", errorMsg);
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}


#endif
// svenkas

pthread_t download_image_thread_id;
void* DownloadImageThreadRun(void* param);

void* DownloadImageThreadRun(void* param)
{
    
    
    return 0;
}

extern "C" void ThreadDownloadStart()
{
    if (pthread_create(&download_image_thread_id, NULL, &DownloadImageThreadRun, NULL))
    {
        exit(1);
    }
}

extern char* addString(const char* addThis, const char* toThis)
{
    char* destination = (char*)malloc( strlen( addThis ) + strlen( toThis ) + 1 );
    strcpy( destination, toThis );
    strcat( destination, addThis );
    return destination;
}
extern NSString* OS_GetTimeODataFromat(INT_TIME interval)
{
    NSDateFormatter *dateFormat = [[NSDateFormatter alloc] init];
    [dateFormat setTimeZone:[NSTimeZone timeZoneForSecondsFromGMT:0]];
    //[dateFormat setDateFormat:@"yyyy-MM-dd'T'HH:mm:ss.SS"];
    [dateFormat setDateFormat:@"yyyy-MM-dd'T'HH:mm:ss"];
    NSDate *localDate = [NSDate date];
    localDate = [localDate dateByAddingTimeInterval: -time(NULL)];
    //NSDate *date = [dateFormat dateFromString:datestr];
    localDate = [localDate dateByAddingTimeInterval: interval];
    
    NSString *dateString = [dateFormat stringFromDate: localDate];
    return dateString;
}
/*extern "C" void	OS_RatingWithDialog(const char* title, const char* message)
 {
 NSString* stitle = [NSString stringWithFormat:@"%s",title];
 NSString* sfeed = [NSString stringWithFormat:@"%s",message];
 [[PineGameDelegate sharedInstance] showRating:stitle body:sfeed];
 }*/
/*extern "C" void OS_BrowserWebPage(const char* url) {
 NSString* strUrl = [ NSString stringWithFormat:@"http://%s",url];
 
 [[UIApplication sharedApplication] openURL:[NSURL URLWithString:strUrl]];
 
 }*/
extern "C" void OS_AsynLoadIAP(PineServiceHandle* handle)
{
    handle->Status= PineServiceResult::WaitingResponse;
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(LoadIAP) withObject:nil waitUntilDone:YES];
    handle->Status= PineServiceResult::Responsed;
}
#import "iPhoneGameUtils_ext.h"

extern "C" bool OS_EnableScaleView()
{
	return false;
}
extern "C" bool OS_ScaleView(float scalex, float scaley)
{
	return false;
}
////////////////////////////////////////////
///NOT SUPPORT
////////////////////////////////////////////

extern "C" void 		OS_Restart(bool suppen) {}
extern "C" void			OS_AsynInitFacebook(PineServiceHandle* handle){}
extern "C" void			OS_LoginFacebook(){}
extern "C" void			OS_AsynLoginFacebook(PineServiceHandle* handle){}
extern "C" void			OS_PostFacebook(const char* msg){}
extern "C" void			OS_AsynPostFacebook(PineServiceHandle* handle, const char* msg){}
extern "C" void			OS_LogoutFacebook(){}
extern "C" void			OS_AsynLogoutFacebook(PineServiceHandle* handle){}
extern "C" UINT64		OS_GetFacebookUserId(){return 0;}
extern "C" const char*	OS_GetFacebookUserName(){return "";}
extern "C" void			OS_AsynFacebookLoadFriendIdList(PineServiceHandle* handle, PineAccountIdList* account_list){}
extern "C" void			OS_AsynFacebookPostScore(PineServiceHandle* hsvc, BoardScoreItem item) {
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void 		OS_AsynRestoreIAP(PineServiceHandle* hsvc, const char *sku){
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
//GOOGLE PLAY

extern "C" bool OS_IsLoginGooglePlay() {
	return false;
}
extern "C" bool OS_AsynIsLoginGooglePlay(PineServiceHandle* hsvc)
{
	return false;
}
extern "C" void	OS_AsynLoginGooglePlay(PineServiceHandle* hsvc, bool userConfirm) {
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void	OS_AsynGetGooglePlayUserInfo(PineServiceHandle* hsvc, char * userId, char * gpId, char * userName){
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void	OS_AsynGetGoogleFriendIdList(PineServiceHandle* hsvc, PineAccountIdList* account_list) {
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" UINT64 OS_GetGooglePlayId() {
	return 0;
}
extern "C" const char* OS_GetGooglePlayUserName() {
	return NULL;
}
extern "C" const char* OS_GetGooglePlayAvatarUrl() {
	return NULL;
}
extern "C" void OS_AsynGooglePlayPost(PineServiceHandle* hsvc, const char* msg){
	hsvc->Status = PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynGooglePlayPostScore(PineServiceHandle* hsvc, BoardScoreItem item) {
	hsvc->Status = PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynGooglePlayGetLeaderBoardRank(PineServiceHandle* hsvc, const char * board_id, int * current_rank){
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynGooglePlayShowLeaderBoard(PineServiceHandle* hsvc, const char* board_id) {
	hsvc->Status = PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynGooglePlayUnlockAchievement(PineServiceHandle* hsvc, const char* achi_id){
	hsvc->Status = PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynGoogleUpdateCloudSave(PineServiceHandle* hsvc, const char* localSaveName)
{
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}

extern "C" void OS_AsynShareIntent(PineServiceHandle* hsvc, const char * name, const char* msg)
{
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}

extern "C" void OS_AsynShareScreenShotIntent(PineServiceHandle* hsvc, const char* msg)
{
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;	
}
extern "C" void OS_AsynSendEmailIntent(PineServiceHandle* hsvc, const char* title, const char* body)
{
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}

extern "C" const char*	OS_GetFacebookAvatarUrl()
{
    return NULL;
}
extern "C" void	OS_AsynFacebookPostScreen(PineServiceHandle* hsvc, const char* msg, PineGraphicComplexOrientation rotate)
{
    hsvc->Status = PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
//TWITTER
extern "C" bool OS_IsLogginTwitter()
{
    return false;
}
extern "C" UINT64 OS_GetTwitterUserId()
{
    return 0;
}
extern "C" const char*	OS_GetTwitterUserName()
{
    return NULL;
}
extern "C" void OS_AsynPostTwitter(PineServiceHandle* hsvc, const char* msg)
{
    hsvc->Status = PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void	OS_AsynLoginTwitter(PineServiceHandle* hsvc)
{
    hsvc->Status = PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
///////////////////////////////////////////////////
///ADMOB
///////////////////////////////////////////////////
extern "C" void OS_AsynAdmobInterstitialPrepare(PineServiceHandle* svc, bool auto_show, const char* ad_id)
{
    svc->Status = PineServiceResult::Responsed; svc->ResponseStatus = 1;
}
extern "C" void OS_AsynAdmobInterstitialShow(PineServiceHandle* svc)
{
    svc->Status = PineServiceResult::Responsed; svc->ResponseStatus = 1;
}
extern "C" void OS_AsynAdmobBannerPrepare(PineServiceHandle* svc, bool auto_show, const char* ad_id) {
	svc->Status = PineServiceResult::Responsed;
	svc->ResponseStatus = 0;
}
extern "C" void OS_AsynAdmobBannerShow(PineServiceHandle* svc) {
	svc->Status = PineServiceResult::Responsed;
	svc->ResponseStatus = 0;
}
extern "C" void OS_AsynAdmobBannerHide(PineServiceHandle* svc) {
	svc->Status = PineServiceResult::Responsed;
	svc->ResponseStatus = 0;
}

///////////////////////////////////////////////////
///CHARTBOOST
///////////////////////////////////////////////////
extern "C" void OS_AsynCBInterstitialPrepare(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 0;
}
extern "C" void OS_AsynCBInterstitialShow(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 0;
}

extern "C" void OS_AsynCBRewardedVideoPrepare(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 0;
}
extern "C" void OS_AsynCBRewardedVideoShow(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 0;
}

///////////////////////////////////////////////////
///UNITY ADS
///////////////////////////////////////////////////
extern "C" void OS_AsynUnityAdsInterstitialPrepare(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynUnityAdsInterstitialShow(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 1;
}

extern "C" void OS_AsynUnityAdsRewardedVideoPrepare(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 1;
}

extern "C" void OS_AsynUnityAdsRewardedVideoShow(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynUnityAdsSkippableVideoPrepare(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 0;
}
extern "C" void OS_AsynUnityAdsSkippableVideoShow(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 0;
}
//Hao Ly
extern "C" void OS_GetResourceFilePathToChar(const char* filename, char* output)
{
    NSString* logic_path = [[NSString alloc] initWithUTF8String: filename];
    NSString* real_path = [[NSBundle mainBundle] pathForResource: logic_path ofType: @""];
    
    sprintf(output, "%s", [real_path UTF8String]);
    
    [logic_path release];
    [real_path release];
}
//LamNguyen

extern "C" NSString* getStrDec(const char* GC_ID)
{
    NSString* ret = [NSString alloc];
    char* macAddressString= (char*)malloc(18);
    NSString* macAddress= [[NSString alloc] initWithCString:getMacAddress(macAddressString,"en0")
                                                   encoding:NSMacOSRomanStringEncoding];
    NSString* NS_GC = [[NSString alloc] initWithUTF8String:GC_ID];
    //NSLog(@"\n NS_GC = %@, _gc_id = %s\n",NS_GC, GC_ID);
    NSData *result = [[PineGameDelegate sharedInstance] encryptString:NS_GC withKey:MY_SERCET_PASSWORD ];
    NSString* newStr = [[PineGameDelegate sharedInstance] base64forData:result];
    ret = [[NSString alloc] initWithFormat:@"%@:%@", macAddress, newStr];
    
    //NSLog(@"\n ret = %@ \n", ret);
    ret = [[PineGameDelegate sharedInstance] base64Encode:ret];
    // NSLog(@"\n retStrDec = %@\n", ret);
    
    [NS_GC release];
    return ret;
}

extern "C" bool OS_writeRawImage (const unsigned char * rgba_buffer, int width, int height, const char* filename) {
    
    
    size_t bufferLength = width * height * 4;
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, rgba_buffer, bufferLength, NULL);
    size_t bitsPerComponent = 8;
    size_t bitsPerPixel = 32;
    size_t bytesPerRow = 4 * width;
    
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    if(colorSpaceRef == NULL) {
        NSLog(@"Error allocating color space");
        CGDataProviderRelease(provider);
        return false;
    }
    
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault | kCGImageAlphaPremultipliedLast;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
    
    CGImageRef iref = CGImageCreate(width,
                                    height,
                                    bitsPerComponent,
                                    bitsPerPixel,
                                    bytesPerRow,
                                    colorSpaceRef,
                                    bitmapInfo,
                                    provider,   // data provider
                                    NULL,       // decode
                                    YES,            // should interpolate
                                    renderingIntent);
    
    uint32_t* pixels = (uint32_t*)malloc(bufferLength);
    
    if(pixels == NULL) {
        NSLog(@"Error: Memory not allocated for bitmap");
        CGDataProviderRelease(provider);
        CGColorSpaceRelease(colorSpaceRef);
        CGImageRelease(iref);
        return false;
    }
    
    CGContextRef context = CGBitmapContextCreate(pixels,
                                                 width,
                                                 height,
                                                 bitsPerComponent,
                                                 bytesPerRow,
                                                 colorSpaceRef,
                                                 bitmapInfo);
    
    if(context == NULL) {
        NSLog(@"Error context not created");
        free(pixels);
    }
    
    UIImage *image = nil;
    if(context) {
        
        CGContextDrawImage(context, CGRectMake(0.0f, 0.0f, width, height), iref);
        
        CGImageRef imageRef = CGBitmapContextCreateImage(context);
        
        // Support both iPad 3.2 and iPhone 4 Retina displays with the correct scale
        if([UIImage respondsToSelector:@selector(imageWithCGImage:scale:orientation:)]) {
            float scale = [[UIScreen mainScreen] scale];
            image = [UIImage imageWithCGImage:imageRef scale:scale orientation:UIImageOrientationUp];
        } else {
            image = [UIImage imageWithCGImage:imageRef];
        }
        
        CGImageRelease(imageRef);
        CGContextRelease(context);
    }
    
    CGColorSpaceRelease(colorSpaceRef);
    CGImageRelease(iref);
    CGDataProviderRelease(provider);
    
    if(pixels) {
        free(pixels);
    }
    
    NSData *pngData = UIImagePNGRepresentation(image);
    
    NSString* fileToSaveTo = [NSString stringWithFormat:@"%s" , filename];
    
 
    NSArray* path = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
    NSString* documentsDirectory = [path objectAtIndex:0];
    
    BOOL sucess = [pngData writeToFile:[NSString stringWithFormat:@"%@/%@",documentsDirectory,fileToSaveTo] atomically:YES];
    
    return sucess;
    //return image;
}
extern "C" bool  OS_IsIpadDevice()
{
     //NSLog(@"UIDevice = %@ version %@",[UIDevice currentDevice].model,[UIDevice currentDevice].systemVersion);
    if ( [(NSString*)[UIDevice currentDevice].model hasPrefix:@"iPad"] )
    {
        //NSLog(@"UIDevice = %@ version %@",[UIDevice currentDevice].model,[UIDevice currentDevice].systemVersion);
        return true; /* Device is iPad */
    }
    else
    {
        return false;
    }
}

#include "GameBaseProtocal.h"
