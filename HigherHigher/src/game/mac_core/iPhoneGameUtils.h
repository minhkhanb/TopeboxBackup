#ifndef _GameEngine_H_
#define _GameEngine_H_

extern "C" unsigned long MACOS_GetTime();
//extern "C" void LOG(char *error, ...);
extern "C" void GameInit();
extern "C" void GameRun();
extern "C" void GamePause();
extern "C" void GameResume();
extern "C" void GameExit();
extern "C" void StartThread();
extern "C" void UpdateTouchPress(int touchId, int x, int y);
extern "C" void UpdateTouchRelease(int touchId, int x, int y);
extern "C" void UpdateTouchMove(int touchId, int x, int y);
extern "C" char* OS_GetDocumentFilePath(const char* filename);
extern "C" char* OS_GetResourceFilePath(const char* filename);
extern "C" const char* OS_GetDeviceUserAgent();
//extern "C" const char* SendHttpRequest(char* url, char* request, UINT16 port, char* httpData);


extern "C" char* getMacAddress(char* macAddress, char* ifName);

//extern "C" NSString* getStrDec(const char* GC_ID);
extern "C" void testPrintf();

extern "C" void DownloadFile(const char* url, const char* file_name,int timeout);
//extern "C" const char* Get_FB_ID()
extern "C" int getMinuteLastTimeUpdate_extern(INT_TIME userID, INT_TIME gameID);
extern "C" int storeToResultFromFriendList_extern(int type_score, BYTE type_time, bool isAscending);
extern "C" int storeUserResult_extern();

// svenkas
extern "C" void ThreadDownloadStart();
//extern "C" int getImageFromUrl2(char* path, char* fileName);
extern "C" int getImageFromUrl(char* path, char* fileName);
extern "C" int getImageFromUrlWithSize(char* path, char* fileName, int width, int height);

#endif