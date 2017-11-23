#include "pch.h"
#include "../../game/GameCore/CGame.h"
#include "bass.h"
#include <ctime>

#include <chrono>
////////////////////////////////////////////////////
///WORK WITH DATA FILE
////////////////////////////////////////////////////
extern "C" bool OS_SaveAppData(const char* filename, const char* data_to_save, int data_length, bool is_append){
	FILE* stream = NULL;
	if (is_append){
		stream = fopen(filename, "ab");
	}else{
		stream = fopen(filename, "wb");
	}

	if (stream != NULL){
		fwrite(data_to_save, sizeof(BYTE), data_length, stream);
	}else{
		return false;
	}

	fclose(stream);
	return true;
}
extern "C" int OS_ReadAppData(const char* filename, char*& readed_data){
	FILE* stream = NULL;

	stream = fopen(filename, "rb");

	if (stream != NULL){
		fseek(stream, 0, SEEK_END);
		int size = ftell(stream);
		fseek(stream, 0, SEEK_SET);
		readed_data = new char[size];
		fread(readed_data, sizeof(char), size, stream);
		fclose(stream);
		return size;
	}
	return 0;
}

extern "C" Buffer*	OS_LoadAppData(const char* filename){
	FILE* stream = NULL;
	stream = fopen(filename, "rb");
	if (stream != NULL){
		fseek(stream, 0, SEEK_END);
		int size = ftell(stream);
		fseek(stream, 0, SEEK_SET);
		Buffer* buffer = new Buffer(size);
		fread(buffer->Data(), sizeof(char), size, stream);
		fclose(stream);
		return buffer;
	}
	return NULL;
}

extern "C" bool OS_DeleteAppData(const char* filename){	
	if (remove(filename) != 0){
		return false;
	}
	return true;
}
extern "C" char* OS_GetResourceFilePath(const char* filename){
	int len = strlen(filename) + strlen(SIMULATOR_RESOURCE_PATH) + 2;
	if(len > 1){
		char* path = new char[len];
		sprintf(path, "%s/%s",SIMULATOR_RESOURCE_PATH,filename);
		path[len-1] = '\0';
		return path;
	}
	return NULL;
}

extern "C" char*	OS_GetDocumentFilePath(const char* filename){
	int len = strlen(filename) + strlen(SIMULATOR_RESOURCE_PATH) + 2;
	if (len > 1){
		char* path = new char[len];
		sprintf(path, "%s/%s", SIMULATOR_RESOURCE_PATH, filename);
		path[len - 1] = '\0';
		return path;
	}
	return NULL;
}
extern "C" ResourceItem OS_GetResourceItem(const char* filename){
	char* real_path = OS_GetResourceFilePath(filename);
	ResourceItem resitem;
	sprintf(resitem.Path, "%s", real_path);
	resitem.Offset = 0;
	resitem.Length = -1;
	delete[] real_path;
	return resitem;
}
extern "C" void OS_DeleteFile(const char* file_name)
{
	if (remove(file_name) != 0){
	}
}

extern "C" bool OS_EnableScaleView()
{
	return false;
}
extern "C" bool OS_ScaleView(float scalex, float scaley)
{
	return false;
}
////////////////////////////////////////////////////
////INAPP
////////////////////////////////////////////////////
extern "C" void PurchaseProduct(const char *indentify){
}
////////////////////////////////////////////////////
///UTILITY
////////////////////////////////////////////////////

extern "C" unsigned long OS_GetTime(void){
	std::time_t time_now = std::chrono::system_clock::now().time_since_epoch().count();

	std::chrono::high_resolution_clock clock;
	return std::chrono::duration_cast<std::chrono::nanoseconds>(clock.now().time_since_epoch()).count();

}
extern "C" INT_TIME GetMilisecondOld()
{
	return clock();
}
extern "C" double OS_GetAccelX(){
	return 0;
}
extern "C" double OS_GetAccelY(){
	return 0;
}
extern "C" double OS_GetAccelZ(){
	return 0;
}
extern "C" bool OS_IsNetworkAvailable(){
	return false;
}

////////////////////////////////////////////////////
///FACEBOOK
////////////////////////////////////////////////////
extern "C" void InitFaceBook()
{

}

extern "C" bool FBSessionValid()
{
	return true;
}
extern "C" void InitFaceBookAsRequire(){
}
extern "C" bool HasLoginFB(){
	return false;
}
extern "C" int	getFBErrorCode(){
	return -1;
}
extern "C" void  loadFBAvatarPhoto(){
}
extern "C" const char* OS_GetFBUserId(){
	return "12345";
}
extern "C" const char* OS_GetFBUserName(){
	return "not support";
}

extern "C" SVRESULT OS_getInAppList(){
	return SVRESULT::SV_DATA_ERROR_NO_NETWORK;
}

extern "C" SVRESULT OS_InitFaceBookAsRequire(){
	return SVRESULT::SV_DATA_ERROR_NO_NETWORK;
}//require login to playgame

extern "C" void FaceBookPost(const char * msg)
{

}
extern "C" void FBLoadMe()
{
}
extern "C" void FBLoadListFriend(bool includeNotPlayGame)
{
}

extern "C" const char* GetFBName()
{
	return "Facebook User";
}

extern "C" void FBRequestToId(INT_TIME fbId)
{

}

extern "C" void FB_apprequests(const char * msg, const INT_TIME * arrayID, int count)
{

}
//twitter
extern "C" void InitTwitter()
{

}

extern "C" void TwitterLoginDialog()
{

}

extern "C" void TwitterPostTweet(const char * msg)
{

}

//game center
extern "C" void GetGCFriends()
{
}



extern "C" bool isGCAvailable()
{
	return true;
}

extern "C" void InitGCBoard()
{

}

extern "C" int getGCErrorCode()
{
	return -2;
}

extern "C" const char* GetGCName()
{
	return "GC User";
}

extern "C" bool saveGCAvatar()
{
	return true;
}

extern "C" INT_TIME GetGCIDNum()
{
	return 2000000089000000000;
}

extern "C" bool HasGCAuthenticated()
{
	return true;
}

extern "C" void loadGCAvatarPhoto()
{

}

extern "C" bool HasCompleteLoadGCAvatar()
{
	return true;
}
extern "C" void OS_DisableReturnKey(bool disable){

}
extern "C" void OS_ClearTextBuffer(){

}
extern "C" void OS_ShowLeaderAchivement(){

}