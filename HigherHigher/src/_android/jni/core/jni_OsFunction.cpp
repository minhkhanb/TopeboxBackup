#include <stdio.h>

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../../../game/Lib/DataType/CType.h"
#include "../../../game/Lib/DataType/BufferStream.h"
#include "jni_config_header.h"
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "jni_log.h"
#include <unistd.h>
#include "jni_service_manager.h"
#include "Android_Connector.h"
#include <vector>

#ifdef  __cplusplus
extern "C" {
#endif
static AAssetManager* _jni_asset_mng = NULL;
static JNIEnv* _jni_env = NULL;
static jclass 	_jni_class = NULL;
static JavaVM *_jvm = NULL;

static jmethodID _jni_methodGetResourcePath = 0;
static jmethodID _jni_methodGetDataPath = 0;
static jmethodID _jni_methodGetAssetOffset = 0;
static jmethodID _jni_methodProcessServiceStream = 0;
static jmethodID _jni_methodGetDeviceOsVersion = 0;
static jmethodID _jni_methodIsNetworkAvailable = 0;
static jmethodID _jni_methodScaleView = 0;


void JNI_CORE_FUNCTION(OsFunctions_nativeInit) (JNIEnv* env, jobject jclassobj);
void JNI_CORE_FUNCTION(AppContext_nativeInitContext)(JNIEnv* env, jobject jclassobj,jobject assetManager);
void JNI_CORE_FUNCTION(OsFunctions_nativeReportService)(JNIEnv* env, jobject jclassobj, jbyteArray svcdata);

void JNI_CORE_FUNCTION(OsFunctions_nativeSetFacebookInfo) (JNIEnv* env, jobject jclassobj, jstring id, jstring use_name);

#ifdef  __cplusplus
}
#endif


#define MAX_RESITEM_TABLE_CATCH 200
static char _jni_fb_username[256];
static INT64 _jni_fb_id;
static bool _jni_fb_isloged = false;
static ResourceItem resItemsTable[MAX_RESITEM_TABLE_CATCH];

void JNI_CORE_FUNCTION(AppContext_nativeInitContext)(JNIEnv* env, jobject jclassobj,jobject assetManager)
{
	_jni_asset_mng = AAssetManager_fromJava(env, assetManager);
	//PDEBUG("init asset manager\n");
	_jni_fb_id = 0;
	_jni_fb_username[0] = '\0';
}

extern "C" AAssetManager* getAssetManager()
{
	return _jni_asset_mng;
}

char* jni_getResourcePath();
void JNI_CORE_FUNCTION(OsFunctions_nativeInit) (JNIEnv* env, jobject jclassobj)
{
	PDEBUG("INIT OSFunctions Native\n");
	/*if(_jni_env != env){
		PDEBUG("CHANGE ENV\n");
		
		JavaVM* jvm;
		env->GetJavaVM(&jvm);
		JNIEnv* myNewEnv;
		JavaVMAttachArgs args;
		args.version = JNI_VERSION_1_6; // choose your JNI version
		args.name = NULL; // you might want to give the java thread a name
		args.group = NULL; // you might want to assign the java thread to a ThreadGroup
		jvm->AttachCurrentThread(&myNewEnv, &args);
		env = myNewEnv;
	}*/
	_jni_env = env;
	_jni_class = (jclass)(_jni_env)->NewGlobalRef(jclassobj);
	_jni_methodGetResourcePath = (_jni_env)->GetStaticMethodID (_jni_class, "GetResourcePath", "()Ljava/lang/String;");	
	_jni_methodGetDataPath = (_jni_env)->GetStaticMethodID (_jni_class, "GetDataPath", "()Ljava/lang/String;");	
	_jni_methodGetAssetOffset = _jni_env->GetStaticMethodID(_jni_class,"GetAssetOffset","(Ljava/lang/String;)I");
	_jni_methodProcessServiceStream = _jni_env->GetStaticMethodID(_jni_class, "ProcessServiceStream", "([B)V");
	_jni_methodGetDeviceOsVersion = _jni_env->GetStaticMethodID(_jni_class, "GetDeviceOsVersion", "()I");
	_jni_methodIsNetworkAvailable = _jni_env->GetStaticMethodID(_jni_class, "IsNetworkAvailable", "()Z");
	_jni_methodScaleView = _jni_env->GetStaticMethodID(_jni_class, "ScaleView","(FF)Z");
	
	PDEBUG("init OsFunctions jni finish :%d\n", _jni_methodScaleView);
}

void JNI_CORE_FUNCTION(OsFunctions_nativeReportService)(JNIEnv* env, jobject jclassobj, jbyteArray svcdata){
	PDEBUG("[Report Service]");
	jboolean rs = JNI_FALSE;
	int length = _jni_env->GetArrayLength(svcdata);
	PDEBUG("Report Service: lenght%d\n", length);
	jbyte* bytes = _jni_env->GetByteArrayElements(svcdata,&rs);
	BufferStream buff(length);
	for(int d = 0; d< length; d++){
		buff.WriteByte(bytes[d]);
	}
	buff.Seek(0, SEEK_SET);
	//
	int ver = buff.ReadByte();
	PDEBUG("ver:%d",ver);
	
	if(ver == 0)
	{
		int id = buff.ReadInt32();
		int svc_code = buff.ReadInt32();
		int status = buff.ReadInt32();
		int error_code = buff.ReadInt32();
		//request data
		Buffer request_buf = buff.ReadIBin();
		Buffer response_buf = buff.ReadIBin();
		ServiceStreamManager::response(id, response_buf, error_code);
		//response data
		PDEBUG("id: %d svc_code:%d status:%d error_code:%d response_data_len:%d\n", id, svc_code, status, error_code, response_buf.Length());
	}
	
	//if(rs == JNI_TRUE){y
		//PDEBUG("[Report Service success]");
		_jni_env->ReleaseByteArrayElements(svcdata,(jbyte *)bytes,0);
	//}else{
	//	PDEBUG("[Report Service false]");
	//}
	//_jni_env->ReleaseByteArrayElements(*_jni_env,data_array,(jbyte *)bytes,0);
}

void JNI_CORE_FUNCTION(OsFunctions_nativeSetFacebookInfo) (JNIEnv* env, jobject jclassobj, jstring id, jstring use_name)
{
	jboolean is_copy;
	bool is_fail = false;
	const char *cstr = (env)->GetStringUTFChars(id, &is_copy);
	if(cstr != NULL){
		_jni_fb_id = 0;
		char* p = (char*)cstr;
		while(*p++)
		{
			_jni_fb_id = _jni_fb_id* 10 + (*p - '0');
		}
		if(is_copy == JNI_TRUE){
			(env)->ReleaseStringUTFChars(id, cstr);
		}
	}else {
		is_fail = true;
	}
	cstr = (env)->GetStringUTFChars(use_name, &is_copy);
	if(cstr != NULL)
	{
		_jni_fb_id = 0;
		sprintf(_jni_fb_username, "%s", cstr);
		if(is_copy == JNI_TRUE)
		{
			(env)->ReleaseStringUTFChars(id, cstr);
		}
	}
	else {
		is_fail = true;
	}
	_jni_fb_isloged = !is_fail;
}

char* jni_getResourcePath(){
	jstring jstr;
	jboolean is_copy;
	char* path = NULL;
	jstr = (jstring)(_jni_env)->CallStaticObjectMethod(_jni_class, _jni_methodGetResourcePath, 0);
	const char *cstr = (_jni_env)->GetStringUTFChars(jstr, &is_copy);
	if(cstr != NULL){
		path = new char[255];
		sprintf(path,"%s",cstr);
		printf("path:%s\n", path);
		if(is_copy == JNI_TRUE){
			(_jni_env)->ReleaseStringUTFChars(jstr, cstr);
		}
		return path;
	}
	if(is_copy == JNI_TRUE){
		(_jni_env)->ReleaseStringUTFChars(jstr, cstr);
	}
	return path;
}

char* jni_getDataPath(){
	jstring jstr;
	jboolean is_copy;
	char* path = NULL;
	jstr = (jstring)(_jni_env)->CallStaticObjectMethod(_jni_class, _jni_methodGetDataPath, 0);
	const char *cstr = (_jni_env)->GetStringUTFChars(jstr, &is_copy);
	if(cstr != NULL){
		path = new char[255];
		sprintf(path,"%s",cstr);
		printf("path:%s\n", path);
		if(is_copy == JNI_TRUE){
			(_jni_env)->ReleaseStringUTFChars(jstr, cstr);
		}
		return path;
	}
	if(is_copy == JNI_TRUE){
		(_jni_env)->ReleaseStringUTFChars(jstr, cstr);
	}
	return path;
}

int jni_getAssetOffset(const char* filename){
	jstring jstr = _jni_env->NewStringUTF(filename);
	int offset = (int)(_jni_env)->CallStaticObjectMethod(_jni_class, _jni_methodGetAssetOffset, jstr);
	_jni_env->DeleteLocalRef(jstr);
	return offset;
}

char* jni_respath = NULL;
char* jni_data_path = NULL;

extern "C" void jni_serviceRequest(ServiceStream* svc){
	//PDEBUG("begin request\n");
	int buff_size = 1;//version
	buff_size += 4;//id
	buff_size += 4;//service code
	buff_size += 4;//Status
	buff_size += 4;//ErrorCode
	buff_size += 4 + svc->RequestData.Length();//ErrorCode
	buff_size += 4 + svc->ResponseData.Length();//ErrorCode
	BufferStream buf;
	buf.Init(buff_size);
	buf.WriteByte(0);
	buf.WriteInt32(svc->ID);
	//PDEBUG("send request totalsize:%d :%d rdata:%d",buff_size, svc->ServiceCode,svc->RequestData.Length());
	buf.WriteInt32(svc->ServiceCode);
	buf.WriteInt32((int)svc->Status);
	buf.WriteInt32(svc->ErrorCode);
	buf.WriteIBin(&svc->RequestData);
	buf.WriteIBin(&svc->ResponseData);
	//for(int i = 0; i< buff_size; i++)
	//{
	//	PDEBUG("%d\n", buf.Data()[i]);
	//}
	
	jbyteArray data_array = _jni_env->NewByteArray(buff_size);
	_jni_env->SetByteArrayRegion( data_array, 0, buff_size, (const jbyte*) buf.Data() );
	_jni_env->CallStaticVoidMethod(_jni_class,_jni_methodProcessServiceStream, data_array);
	_jni_env->DeleteLocalRef(data_array);
	//PDEBUG("end request service\n\n");
}

////////////////////////////////////////////////////
///WORK WITH DATA FILE
////////////////////////////////////////////////////
char* GetAppDataPath (const char* filename)
{
	if(jni_data_path == NULL){
		jni_data_path = jni_getDataPath();
	}
	char* data_path = new char[255];
	sprintf(data_path,"data/data/%s/%s", jni_data_path, filename);
	return data_path;
}
extern "C" bool OS_SaveAppData(const char* filename, const char* data_to_save, int data_length, bool is_append){
	char* filepath = GetAppDataPath(filename);
	PDEBUG("Save app data to: %s", filepath);
	FILE* stream = NULL;
	if (is_append){
		stream = fopen(filepath, "ab");
	}else{
		stream = fopen(filepath, "wb");
	}
	delete[] filepath;
	if (stream != NULL){
		fwrite(data_to_save, sizeof(BYTE), data_length, stream);
		fclose(stream);
	}else{
		return false;
	}
	return true;
}

extern "C" Buffer*	OS_LoadAppData(const char* filename){
	FILE* stream = NULL;
	char* filepath = GetAppDataPath(filename);
	stream = fopen(filepath, "rb");
	delete[] filepath;
	if (stream != NULL){
		fseek(stream, 0, SEEK_END);
		int size = ftell(stream);
		fseek(stream, 0, SEEK_SET);
		Buffer* buff = new Buffer(size);
		fread(buff->Data(), sizeof(char), size, stream);
		fclose(stream);
		return buff;
	}
	return NULL;
}
extern "C" int OS_ReadAppData(const char* filename, char* &readed_data){
	FILE* stream = NULL;
	char* filepath = GetAppDataPath(filename);
	stream = fopen(filepath, "rb");
	delete[] filepath;
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
extern "C" bool OS_DeleteAppData(const char* filename){
	char* filepath = GetAppDataPath(filename);
	if (remove(filepath) != 0){
		delete[] filepath;
		return false;
	}
	delete[] filepath;
	return true;
}
extern "C" char* OS_GetResourceFilePath(const char* filename){
	if(jni_respath == NULL){
		jni_respath = jni_getResourcePath();
	}
	char* res_path = new char[255];
	sprintf(res_path,"%s", jni_respath);
	return res_path;
}

extern "C" char*	OS_GetDocumentFilePath(const char* filename){
	char* res_path = GetAppDataPath(filename);
	return res_path;
}

extern "C" void OS_DeleteFile(const char* file_name)
{
	if (remove(file_name) != 0){
	}
}
extern "C" ResourceItem CheckResourceItem (const char* filename)
{
    ResourceItem resitem("", 0);
    for (int i = 0; i < 50; i++)
    {
        //PDEBUG("Compare resItemsTable[%d].FileName: %s & filename = %s", i, resItemsTable[i].FileName, filename);
        if (strcmp(resItemsTable[i].FileName, filename) == 0)
        {
            //PDEBUG("Found resource item");
            return resItemsTable[i];
        }
    }
    //PDEBUG("NOT Found resource Return new empty");
    return resitem ;
}
extern "C" void AddResourceItem (ResourceItem rI)
{
    //PDEBUG("Add resource item");
    for (int i = 0; i < MAX_RESITEM_TABLE_CATCH; i++)
    {
        if (resItemsTable[i].Length <= 0)
        {
            sprintf(resItemsTable[i].Path, "%s", rI.Path);
            sprintf(resItemsTable[i].FileName, "%s", rI.FileName);
            resItemsTable[i].FileDescription = rI.FileDescription;
            resItemsTable[i].Offset = rI.Offset;
            resItemsTable[i].Length = rI.Length;
            //PDEBUG("add resource item success: %s\nfilname: %s\nfile des:%d\noffset: %d \nlength: %d", resItemsTable[i].Path,
            //                                    resItemsTable[i].FileName,
            //                                    resItemsTable[i].FileDescription,
            //                                    resItemsTable[i].Offset,
            //                                    resItemsTable[i].Length);
            break;
        }
    }
}
extern "C" ResourceItem OS_GetResourceItem(const char* filename){
	if(jni_respath == NULL){
		jni_respath = jni_getResourcePath();
	}
	//bool needclose = true;
	char fname[255];
	sprintf(fname,"%s.pack",filename);
	ResourceItem rI = CheckResourceItem(fname);
	if (rI.Length > 0) return rI;


	ResourceItem resitem(jni_respath, 0);
	sprintf(resitem.FileName,"%s", fname);
	//ResourceItem resitem(fname, 0);
	//PDEBUG("Try Get Resource Asset:%s %s\n", resitem.Path,fname);

	long int offset = 0;
	long int length = 0;
	AAsset* lAsset = AAssetManager_open(_jni_asset_mng, fname, AASSET_MODE_UNKNOWN);
	if (lAsset != NULL) {
		resitem.FileDescription = AAsset_openFileDescriptor(lAsset, &offset, &length); //This function not have method to close opened file so it leak handle
		AAsset_close(lAsset);
		resitem.Offset = (int)offset;
		resitem.Length = (int)length;
		AddResourceItem(resitem);           //Use resItemsTable to cache result for reuse later.
		//PDEBUG("resitem:%s offset:%d - %ld len:%d - %ld\n", resitem.Path, resitem.Offset, offset, resitem.Length, length);
	}else{
		PDEBUG("Try to read expansion file");
		FILE* stream = NULL;
		char* filepath = GetAppDataPath("");
		
		char fullPath[256];
		sprintf(fname,"%s.pack",filename);
		sprintf(fullPath,"%s/%s", filepath, fname);
		PDEBUG("Try to read expansion file: %s", fullPath);
		ResourceItem resitem1(fullPath, 0);
		sprintf(resitem1.FileName,"%s", fname);
		
		stream = fopen(filepath, "rb");
		if (stream != NULL){
			PDEBUG("read expansion file");
			fseek(stream, 0, SEEK_END);
			length = ftell(stream);
			fseek(stream, 0, SEEK_SET);
			fclose(stream);
		}
		else
		{
			PDEBUG("open expansion file fail");
		}
		if (length > 0)
		{
			resitem1.Offset = 0;
			resitem1.Length = (int)length;
			resitem = resitem1;
		}
		else
		{
			PDEBUG("cannot read Asset\n");
		}
		delete [] filepath;
	}
	return resitem;
}
extern "C" void PurchaseProduct(const char *indentify){
    int type = 1; //0: subs; 1: managed(inapp)
	PineString msg_str(indentify);

	int data_size = 2 + 2 * msg_str.Length() + 4;
	BufferStream data_request(data_size);
	data_request.WriteSStr(&msg_str);
	data_request.WriteInt32(type);
	ServiceStreamManager::request((int)PineServiceCode::Purchase, data_request, false);
}

extern "C" void PurchaseProductWithType(const char *indentify, int type){
	PineString msg_str(indentify);

	int data_size = 2 + 2 * msg_str.Length() + 4;
	BufferStream data_request(data_size);
	data_request.WriteSStr(&msg_str);
	data_request.WriteInt32(type);
	ServiceStreamManager::request((int)PineServiceCode::Purchase, data_request, false);
}

extern "C" void	OS_AsynRestoreIAP(PineServiceHandle* hsvc, const char *sku){
    if(hsvc->isFree())
	{
    	PineString sku_str(sku);

    	int data_size = 2 + 2 * sku_str.Length() + 4;
    	BufferStream data_request(data_size);
    	data_request.WriteSStr(&sku_str);
    	data_request.WriteInt32(2);

		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::PurchaseRestore, data_request, false);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream == NULL)
			{
				hsvc->ResponseStatus = -1;
				ServiceStreamManager::freeStream(hsvc->Handle);
				hsvc->Handle = -1;
				hsvc->Status = PineServiceResult::Responsed;
				return;
			}

			PDEBUG("PurchaseRestore responsed error_code:%d", stream->ErrorCode);
            hsvc->ResponseStatus = stream->ErrorCode;
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;

			hsvc->Status = PineServiceResult::Responsed;
		}
	}
}

extern "C" void	OS_Exit(bool sudden)
{
	BufferStream data_request(4);

	if (sudden){
		data_request.WriteInt32(1);
	}
	else{
		data_request.WriteInt32(0);
	}

	ServiceStreamManager::request((int)PineServiceCode::GameExit, data_request, true);
}

extern "C" void	OS_Restart(bool sudden)
{
	BufferStream data_request(4);

	if (sudden){
		data_request.WriteInt32(3);
	}
	else{
		data_request.WriteInt32(2);
	}

	ServiceStreamManager::request((int)PineServiceCode::GameExit, data_request, true);
}
////////////////////////////////////////////////////
///UTILITY
////////////////////////////////////////////////////
extern "C" long long OS_GetTime(void){
	//struct timeval	cur_time;
	//gettimeofday(&cur_time, 0);
    //return (cur_time.tv_sec*1000 + cur_time.tv_usec/1000);
	struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;
}
extern "C" INT_TIME GetMilisecondOld()
{
	return OS_GetTime();
}
extern "C" float OS_GetAccelX(){
	return 0;
}
extern "C" float OS_GetAccelY(){
	return 0;
}
extern "C" float OS_GetAccelZ(){
	return 0;
}
extern "C" int OS_GetDeviceOsVersion(){
	if(_jni_env == NULL || _jni_class == NULL) return 0;
	int rs = (int)(_jni_env)->CallStaticIntMethod(_jni_class,_jni_methodGetDeviceOsVersion);
	return rs;
}
extern "C" bool OS_IsNetworkAvailable(){
	if(_jni_env == NULL || _jni_class == NULL) return true;
	jboolean rs = _jni_env->CallStaticBooleanMethod(_jni_class,_jni_methodIsNetworkAvailable);
	return (rs == JNI_TRUE);
}
extern "C" bool OS_EnableScaleView()
{
	return false;
}
extern "C" bool OS_ScaleView(float scalex, float scaley)
{
	if(_jni_methodScaleView != 0)
	{
		jfloat sx = scalex;
		jfloat sy = scaley;
		jboolean rs = _jni_env->CallStaticBooleanMethod(_jni_class,_jni_methodScaleView, sx, sy);
		return (rs == JNI_TRUE);
	}
	return false;
}
extern "C" void	OS_ShowMessageBox(const char * title, const char * message, int type)
{
	PineString title_str(title);
	PineString msg_str(message);
	int data_size = 2 + 2 * title_str.Length() + 2 + 2 * msg_str.Length() + 4;
	BufferStream data_request(data_size);
	data_request.WriteSStr(&title_str);
	data_request.WriteSStr(&msg_str);
	data_request.WriteInt32(type);
	ServiceStreamManager::request((int)PineServiceCode::DialogShow, data_request, true);
}
extern "C" void	OS_AsyncShowMessageBox(PineServiceHandle* hsvc, const char * title, const char * message, int type)
{
    if(hsvc->isFree())
    {
        PDEBUG("OS_AsyncShowMessageBox type = %d\n", type);
        PineString title_str(title);
        PineString msg_str(message);
        int data_size = 2 + 2 * title_str.Length() + 2 + 2 * msg_str.Length() + 4;
        BufferStream data_request(data_size);
        data_request.WriteSStr(&title_str);
        data_request.WriteSStr(&msg_str);
        data_request.WriteInt32(type);
        hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::DialogShow, data_request, false);
    	if(hsvc->Handle > -1){
    		hsvc->Status = PineServiceResult::WaitingResponse;
    	}
    }
    else if(hsvc->Handle > -1){
    	if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
    		hsvc->Status = PineServiceResult::Responsed;
            const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
            if (stream != NULL)
    		{
    			hsvc->ResponseStatus = stream->ErrorCode;
    		}
    		else
    		{
    			hsvc->ResponseStatus = -1;
    		}
            ServiceStreamManager::freeStream(hsvc->Handle);
            hsvc->Handle = -1;
            PDEBUG("OS_AsyncShowMessageBox responsed: ResponseStatus = %d\n", hsvc->ResponseStatus);
    	}
    }
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
	return _jni_fb_isloged;
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

extern "C" bool OS_HasFacebookToken(){
	return false;
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
extern "C" void FBLoadListFriend()
{
}
extern "C" bool Check_FB_ID_list()
{
	return true;
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

extern "C" bool Check_GC_ID_list()
{
	return true;
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
extern "C" void OS_TextInputStart(char* textBuf, unsigned int bufSize, int x, int y, int w, int h){}
extern "C" void OS_TextInputClose(){}
extern "C" void OS_ShowPicLibrary(){}
extern "C" void OS_ShowCamera(){}
extern "C" void OS_DisableCamera(){}
extern "C" void OS_CaptureCamera(){}
extern "C" void OS_SwitchCamera(){}
extern "C" void OS_ShowAskNotify(){}

/////////////////////////////////////////
///FACEBOOK
/////////////////////////////////////////
extern "C" bool OS_IsLogginFacebook(){
	return _jni_fb_isloged;
}

extern "C" void OS_InitFacebook(){

}

extern "C" void OS_AsynInitFacebook(PineServiceHandle* hsvc){

}

extern "C" void OS_LoginFacebook(){
	PDEBUG("Function Not support");
}

extern "C" void OS_AsynLoginFacebook(PineServiceHandle* hsvc){
	if(hsvc->isFree())
	{
	    PDEBUG("call AsyncLoginFacebook");
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::FacebookLogin);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){

			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream == NULL)
			{
				hsvc->ResponseStatus = -1;
				ServiceStreamManager::freeStream(hsvc->Handle);
				hsvc->Handle = -1;
				hsvc->Status = PineServiceResult::Responsed;
				return;
			}
			hsvc->ResponseStatus = stream->ErrorCode;
			Buffer* pbuff = (Buffer*) &stream->ResponseData;
			BufferStream buff(pbuff->Data(), pbuff->Length());
			PDEBUG("facebook get responsedata: lenght:%d", ((Buffer)stream->ResponseData).Length());
			PineString buff_uid = buff.ReadSStr();
			PineString buff_uname = buff.ReadSStr();
			int offset = 0;
			UNICHAR* temp_data = (UNICHAR*)buff_uname.Data();
			for(int n = 0; n< buff_uname.Length(); n++){
				_jni_fb_username[offset++] = (char)temp_data[n];
			}
			_jni_fb_username[offset] = '\0';
			PDEBUG("facebook username:%s", _jni_fb_username);

			temp_data = (UNICHAR*)buff_uid.Data();
			_jni_fb_id = 0;
			for(int n = 0; n< buff_uid.Length(); n++){
				_jni_fb_id = _jni_fb_id * 10 + (temp_data[n] - '0');
			}
			if(_jni_fb_id > 0){
				_jni_fb_isloged = true;
			}
			PDEBUG("facebook user id:%lld", _jni_fb_id);

			PDEBUG("facebook error_code:%d", stream->ErrorCode);

			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;

			hsvc->Status = PineServiceResult::Responsed;
		}
	}
}

extern "C" void OS_AsynGetAvatarFacebook(PineServiceHandle* hsvc, const char* userURL, const char* fileName, int width, int height){
	//PDEBUG("call OS_AsynGetAvatarFacebook");
	if(hsvc->isFree())
	{
	    PineString url_str(userURL);
	    PineString fn_str(fileName);
        int buff_len = 2 + 2* url_str.Length() + 2 + 2*fn_str.Length() + 8;
        BufferStream buff(buff_len);
        buff.WriteSStr(&url_str);
        buff.WriteSStr(&fn_str);
        buff.WriteInt32(width);
        buff.WriteInt32(height);
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::FacebookGetAvatar, buff, false);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			hsvc->Status = PineServiceResult::Responsed;
            const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
            if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else 
			{
				hsvc->ResponseStatus = -1;
			}
            ServiceStreamManager::freeStream(hsvc->Handle);
            hsvc->Handle = -1;
            PDEBUG("facbook avatar OS_AsynGetAvatarFacebook jni responsed");
		}
	}
}

extern "C" void OS_PostFacebook(const char* msg){

	PDEBUG("post facebook %s\n", msg);
	PineString msg_str(msg);
	int buff_len = 2 + 2* msg_str.Length();
	BufferStream buff(buff_len);
	buff.WriteSStr(&msg_str);
	ServiceStreamManager::request((int)PineServiceCode::FacebookPost, buff, true);
}

extern "C" void OS_AsynPostFacebook(PineServiceHandle* hsvc, const char* msg){

	if(hsvc->isFree()){
		//PineString msg_str(msg);
		//int buff_size = 2 + (2 * msg_str.Length());
		//BufferStream buff(buff_size);
		//buff.WriteSStr(&msg_str);
		PDEBUG("post facebook %s\n", msg);
		PineString msg_str(msg);
		int buff_len = 2 + 2* msg_str.Length();
		BufferStream buff(buff_len);
		buff.WriteSStr(&msg_str);
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::FacebookPost, buff);
		//hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::DialogShow);
		//PDEBUG("free:%d", hsvc->Handle);

		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			hsvc->Status = PineServiceResult::Responsed;
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else 
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
		}
	}
}

extern "C" void OS_LogoutFacebook(){
}

extern "C" void OS_AsynLogoutFacebook(PineServiceHandle* hsvc){
}

extern "C" UINT64 OS_GetFacebookUserId(){
	return _jni_fb_id;
}

extern "C" const char*	OS_GetFacebookUserName(){
	return _jni_fb_username;
}

extern "C" const char*	OS_GetFacebookAvatarUrl(){
	return "not support yet";
}

extern "C" void OS_AsynFacebookLoadFriendIdList(PineServiceHandle* hsvc, PineAccountIdList* account_list){
	if(hsvc->isFree()){
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::FacebookGetAppFriend);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1)
	{
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){

			PDEBUG("get fb friends responsed");
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);

			if(stream != NULL && stream->ErrorCode == 0)
			{
				Buffer* pbuff = (Buffer*)&stream->ResponseData;
				BufferStream buff(pbuff->Data(), pbuff->Length());
				int account_num = buff.ReadInt32();
				account_list->init(account_num);

				for(int a = 0; a< account_num; a++)
				{
					UINT64 id = (UINT64)buff.ReadInt64();
					account_list->setAccountAt(a, id);
					PDEBUG("friend:%lld", id);
					PDEBUG("account_list: %lld\n", account_list->getAccountAt(a));
				}
			}

			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else 
			{
				hsvc->ResponseStatus = -1;
			}
			hsvc->Status = PineServiceResult::Responsed;
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
		}
	}
}

extern "C" void OS_FacebookPostPhoto(const char* file_name, const char* msg){
}

extern "C" void OS_AsynFacebookPostPhoto(PineServiceHandle* hsvc, const char* file_name, const char* msg){

}
extern "C" void OS_AsynFacebookPostScore(PineServiceHandle* hsvc,BoardScoreItem item){
	if(hsvc->isFree()){

	}else if(hsvc->Handle > -1){
	}
}

extern "C" void	OS_AsynFacebookPostScreen(PineServiceHandle* hsvc, const char* msg, PineGraphicComplexOrientation rotate){
	if(hsvc->isFree()){
		PineString msg_str(msg);
		int buff_len = 2 + 2* msg_str.Length();
		PDEBUG("PostFacebook Msg Length:%d", buff_len);
		BufferStream buff(buff_len);
		buff.WriteSStr(&msg_str);
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::FacebookPostScreen, buff, false);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else 
			{
				hsvc->ResponseStatus = -1;
			}
			hsvc->Status = PineServiceResult::Responsed;
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
		}
	}
}

extern "C" void OS_AsynShareIntent(PineServiceHandle* hsvc, const char* name, const char* msg)
{
	if(hsvc->isFree()){
		PineString msg_str(msg);
		PineString name_str(name);
		int buff_len = 2 + 2* msg_str.Length() + 2 + 2 * name_str.Length();
		PDEBUG("Share intent data stream Length:%d", buff_len);
		BufferStream buff(buff_len);
		buff.WriteSStr(&msg_str);
		buff.WriteSStr(&name_str);
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::ShareIntent, buff, false);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			hsvc->Status = PineServiceResult::Responsed;
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
		}
	}
}

extern "C" void OS_AsynShareScreenShotIntent(PineServiceHandle* hsvc, const char* msg)
{
	if(hsvc->isFree()){
		PineString msg_str(msg);
		int buff_len = 2 + 2* msg_str.Length();
		PDEBUG("Share screen shot intent Msg Length:%d", buff_len);
		BufferStream buff(buff_len);
		buff.WriteSStr(&msg_str);
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::ShareScreenShotIntent, buff, false);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else 
			{
				hsvc->ResponseStatus = -1;
			}
			hsvc->Status = PineServiceResult::Responsed;
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
		}
	}
}

extern "C" void OS_AsynSendEmailIntent(PineServiceHandle* hsvc, const char* title, const char* body)
{
	if(hsvc->isFree()){
		PineString title_str(title);
		PineString body_str(body);
		int buff_len = 2 + 2* title_str.Length() + 2 + 2* body_str.Length();
		PDEBUG("Send intent data Length:%d", buff_len);
		BufferStream buff(buff_len);
		buff.WriteSStr(&title_str);
		buff.WriteSStr(&body_str);
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::SendEmailIntent, buff, false);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else 
			{
				hsvc->ResponseStatus = -1;
			}
			hsvc->Status = PineServiceResult::Responsed;
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
		}
	}
}

extern "C" void OS_AsyncShowSendGiftIntent(PineServiceHandle* hsvc, int type, const char* message, int * numGiftReward)
{
	if(hsvc->isFree()){
		PineString msg_str(message);
		int buff_len = 4 + 2 + 2* msg_str.Length();
		PDEBUG("Send intent data Length:%d", buff_len);
		BufferStream buff(buff_len);
		buff.WriteInt32(type);
		buff.WriteSStr(&msg_str);
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::GooglePlaySendGift, buff, false);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
                Buffer* pbuff = (Buffer*) &stream->ResponseData;
                if (pbuff != NULL)
                {
                    BufferStream buff(pbuff->Data(), pbuff->Length());
                    int a = buff.ReadInt32();
                    PDEBUG("Num gift recieved: %d", a);
                    *numGiftReward = a;
                }
			}
			else 
			{
				hsvc->ResponseStatus = -1;
			}
			hsvc->Status = PineServiceResult::Responsed;
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
		}
	}
}

extern "C" void OS_AsynCaptureScreen(PineServiceHandle* hsvc,PineGraphicComplexOrientation rotate){
	if(hsvc->isFree()){
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::TakeScreenShot);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else 
			{
				hsvc->ResponseStatus = -1;
			}
			hsvc->Status = PineServiceResult::Responsed;
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
		}
	}
}

extern "C" void OS_AsynAdmobInterstitialPrepare(PineServiceHandle* hsvc, bool auto_show, const char* ad_id){
	if(hsvc->isFree()){
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::AdmobInterstitialPrepare);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}else if(hsvc->Handle > -1){
		if (ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
			hsvc->Status = PineServiceResult::Responsed;
			PDEBUG("PREPARE AdMob Finish");
		}
	}
}
extern "C" void OS_AsynAdmobInterstitialShow(PineServiceHandle* hsvc){
	if(hsvc->isFree()){
		PDEBUG("CALL SHOW ADMOB");
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::AdmobInterstitialShow);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}else if(hsvc->Handle > -1){
		if (ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
			hsvc->Status = PineServiceResult::Responsed;
			PDEBUG("CALL SHOW ADMOB  Finish");
		}
	}
}

extern "C" void OS_AsynAdmobBannerPrepare(PineServiceHandle* hsvc, bool auto_show, const char* ad_id){
	if(hsvc->isFree()){
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::AdmobBannerPrepare);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}else if(hsvc->Handle > -1){
		if (ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
			hsvc->Status = PineServiceResult::Responsed;
			PDEBUG("PREPARE AdMob Banner Finish");
		}
	}
}
extern "C" void OS_AsynAdmobBannerShow(PineServiceHandle* hsvc){
	if(hsvc->isFree()){
		PDEBUG("CALL SHOW ADMOB BANNER");
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::AdmobBannerShow);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}else if(hsvc->Handle > -1){
		if (ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
			hsvc->Status = PineServiceResult::Responsed;
			PDEBUG("CALL SHOW ADMOB BANNER  Finish");
		}
	}
}

extern "C" void OS_AsynAdmobBannerHide(PineServiceHandle* hsvc){
	if(hsvc->isFree()){
		PDEBUG("CALL HIDE ADMOB BANNER");
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::AdmobBannerHide);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}else if(hsvc->Handle > -1){
		if (ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
			hsvc->Status = PineServiceResult::Responsed;
			PDEBUG("CALL HIDE ADMOB BANNER  Finish");
		}
	}
}

extern "C" void OS_AsynAdmobRewardedVideoPrepare(PineServiceHandle* hsvc){
	if(hsvc->isFree()){
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::AdmobRewardedVideoPrepare);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}else if(hsvc->Handle > -1){
		if (ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
			hsvc->Status = PineServiceResult::Responsed;
			//PDEBUG("PREPARE AdMob Rewarded video Finish with code: %d", hsvc->ResponseStatus);
		}
	}
}
extern "C" void OS_AsynAdmobRewardedVideoShow(PineServiceHandle* hsvc){
	if(hsvc->isFree()){
		PDEBUG("CALL SHOW ADMOB REWARDED VIDEO");
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::AdmobRewardedVideoShow);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}else if(hsvc->Handle > -1){
		if (ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
			hsvc->Status = PineServiceResult::Responsed;
			//PDEBUG("CALL SHOW ADMOB Rewarded video Finish with code: %d", hsvc->ResponseStatus);
		}
	}
}

///////////////////////////////////////////////////
///CHARTBOOST
///////////////////////////////////////////////////
extern "C" void OS_AsynCBInterstitialPrepare(PineServiceHandle* hsvc)
{
	if (hsvc->isFree())
	{
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::ChartboostInterstitialPrepare);
		if (hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if (hsvc->Handle > -1){
		if (ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			hsvc->Status = PineServiceResult::Responsed;
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
			PDEBUG("init cb respoinse:%d\n", hsvc->ResponseStatus);
		}
	}
}
extern "C" void OS_AsynCBInterstitialShow(PineServiceHandle* hsvc)
{
	if (hsvc->isFree())
	{
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::ChartboostInterstitialShow);
		if (hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if (hsvc->Handle > -1){
		if (ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			hsvc->Status = PineServiceResult::Responsed;

			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
		}
	}
}

extern "C" void OS_AsynCBRewardedVideoPrepare(PineServiceHandle* hsvc)
{
	if (hsvc->isFree())
	{
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::ChartboostRewardedVideoPrepare);
		if (hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if (hsvc->Handle > -1){
		if (ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			hsvc->Status = PineServiceResult::Responsed;
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}

			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
		}
	}
}
extern "C" void OS_AsynCBRewardedVideoShow(PineServiceHandle* hsvc)
{
	if (hsvc->isFree())
	{
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::ChartboostRewardedVideoShow);
		if (hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if (hsvc->Handle > -1){
		if (ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			hsvc->Status = PineServiceResult::Responsed;
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}

			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
		}
	}
}



//ADD 1/5/2015
extern "C" const char* OS_GetFacebookAvartar(INT_TIME id){return NULL;}
extern "C" void OS_PostScreenToFacebook(const char* feed){}
extern "C" void OS_PostImageToFacebook(const char* feed, const char* realUrl){}
extern "C" void OS_LoadFriendFacebook(){}
extern "C" void OS_GameCenterAuthentication(){}
extern "C" void OS_GameCenterLoadFriend(){}
extern "C" void OS_GameCenterSubmitAchievement(int percent, const char* leaderboard, bool noti){}
extern "C" void OS_ResetAllAchievement(){}
extern "C" void OS_GameCenterSubmitScore(INT_TIME value, const char* leaderboard){}
extern "C" void OS_GameCenterLoadScore(){}
extern "C" void OS_ShowLeaderboard(const char*  leaderboard){}
extern "C" void OS_ShowLeaderAchivement(){}

extern "C" const char* OS_GetTwitterAvartar(INT_TIME id){return NULL;}
extern "C" void OS_PostToTwitter(const char* feed){}
extern "C" void OS_PostScreenToTwitter(const char* feed){}
extern "C" void OS_PostImageToTwitter(const char* feed, const char* realUrlImage){}
extern "C" void OS_LoadFriendTwitter(){}
extern "C" void	OS_InitTwitter(){}
extern "C" void OS_PostTwitter(const char* msg){}
extern "C" void OS_SystemMessageAsk(const char* feed,int code){}
extern "C" bool OS_PingToHost(const char* url, int time_out){return false;}
extern "C" void OS_LoadIAP(){}

extern "C" void iOS_ShowRatingBasic() {}

extern "C" void OS_Rating()
{
	ServiceStreamManager::request((int)PineServiceCode::Rating,NULL,true);
}

extern "C" void OS_RatingWithDialog(const char* title, const char* message){}
extern "C" bool OS_IsDeviceHacked(){return false;}
extern "C" bool OS_IsLogginTwitter(){return false;}
extern "C" void OS_SystemWarning(const char* feed){}
extern "C" void	OS_LoadImageToPngFile(const char* url, const char *file_name, float width_target, float height_target){}
extern "C" void OS_AsynLoginTwitter(PineServiceHandle* hsvc){
	hsvc->Status = PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" UINT64 OS_GetTwitterUserId(){return 0;}
extern "C" const char* OS_GetTwitterUserName(){return NULL;}



extern "C" void OS_AsynLoadIAP(PineServiceHandle* hsvc){

	//hsvc->Status = PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
	//return;
	/*
	const char* sku_0 = Android_GetIAPDesList(0);
	PDEBUG("list sku_0 : %s", sku_0);
	*/
	if (hsvc->isFree())
	{
		int num_skus = Android_GetIAPDesNum();
		int data_size = 4;
		const char* tmp_str = NULL;
		std::vector<PineString> iaps;
		for (int i = 0; i < num_skus; i++)
		{
			tmp_str = Android_GetIAPDesList(i);
			if(tmp_str != NULL)
			{
				PineString str_tmp(tmp_str);
				//PDEBUG("begin pine string:%d\n", strlen(tmp_str));
				//for(int s = 0; s< str_tmp.Length(); s++)
				//{
				//	PDEBUG("%d\n", str_tmp.Data()[s]);
				//}
				data_size += (2 + 2*str_tmp.Length());
				//PDEBUG("end pine string\n\n");

				tmp_str = NULL;
				iaps.push_back(str_tmp);
			}
		}

		BufferStream data_request(data_size);
		data_request.WriteInt32(num_skus);

		for (int i = 0; i < num_skus; i++)
		{
			data_request.WriteSStr(&iaps.at(i));
		}

		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::PurchaseGetList, data_request, false);
		if (hsvc->Handle > -1)
		{
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
		iaps.clear();
		//PDEBUG("OS_AsynLoadIAP");
	}
	else if (hsvc->Handle > -1)
	{
		if (ServiceStreamManager::isStreamResponse(hsvc->Handle))
		{
			hsvc->Status = PineServiceResult::Responsed;
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			hsvc->ResponseStatus = stream->ErrorCode;
			Buffer* pbuff = (Buffer*)&stream->ResponseData;
			BufferStream buff(
			pbuff->Data(),
			pbuff->Length()
			);

			int num_prices = buff.ReadInt32();

			PDEBUG("OS_AsynLoadIAP responsed num_prices = %d", num_prices);

			for (int i = 0; i < num_prices; i++)
			{
				PineString data = buff.ReadSStr();
				int isOwned = buff.ReadInt32();
				char price[128];
				UNICHAR* temp_data = (UNICHAR*)data.Data();
				int offset = 0;
				for(int n = 0; n< data.Length(); n++){
					price[offset++] = (char)temp_data[n];
				}
				price[offset] = '\0';
				PDEBUG("data: %s", price);
				Android_OnUpdateInAppItem(i, (const char *)price, isOwned==0?false:true);
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
		}
	}
}
extern "C" void OS_AsynTwitterLoadFollowerIdList(PineServiceHandle* hsvc, PineAccountIdList* account_list){
	hsvc->Status = PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_DisableReturnKey(bool disable){}
extern "C" void OS_ClearTextBuffer(){}

extern "C" void	OS_AsynPostTwitter(PineServiceHandle* hsvc, const char* msg){
	hsvc->Status = PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}

//Implement OS_BrowserWebPage - 09/02/2015
extern "C" void OS_BrowserWebPage(const char* url){
PDEBUG("open browser:%s", url);
	PineString msg_str(url);
	int buff_size = 2 + (2 * msg_str.Length());
	BufferStream buff(buff_size);
	buff.WriteSStr(&msg_str);
	int sid = ServiceStreamManager::request((int)PineServiceCode::BrowserOpen, buff,true);
}



//GOOGLE PLAY
extern "C" bool OS_IsLoginGooglePlay(){
}
extern "C" bool OS_AsynIsLoginGooglePlay(PineServiceHandle* hsvc){
	if(hsvc->isFree())
	{
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::GooglePlayIsLogin);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			PDEBUG("Is Login GooglePlay:%d",  hsvc->ResponseStatus);
			hsvc->Status = PineServiceResult::Responsed;
			hsvc->Handle = -1;
		}
	}
	
	return false;
}
extern "C" void	OS_AsynLoginGooglePlay(PineServiceHandle* hsvc, bool needUserConfirm){
	
	BufferStream data_request(4);

	if (needUserConfirm){
		data_request.WriteInt32(1);
	}
	else{
		data_request.WriteInt32(0);
	}
	if(hsvc->isFree())
	{
		PDEBUG("OS_AsynLoginGooglePlay request login");
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::GooglePlayLogin, data_request, false);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			PDEBUG("Login GooglePlay:%d",  hsvc->ResponseStatus);
			hsvc->Status = PineServiceResult::Responsed;
			hsvc->Handle = -1;
		}
	}
}
extern "C" void OS_GetGooglePlayId(){

}
extern "C" void OS_GetGooglePlayUserName(){

}

extern "C" void OS_AsynGetGooglePlayUserInfo(PineServiceHandle* hsvc, char * userId, char * gpId, char * userName){
	if(hsvc->isFree())
	{
	    PDEBUG("OS_AsyncGetGooglePlayUserInfo");
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::GooglePlayUserInfo);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){

			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream == NULL)
			{
				hsvc->ResponseStatus = -1;
				ServiceStreamManager::freeStream(hsvc->Handle);
				hsvc->Handle = -1;
				hsvc->Status = PineServiceResult::Responsed;
				return;
			}
			hsvc->ResponseStatus = stream->ErrorCode;
			if (hsvc->ResponseStatus == 0)
			{
                Buffer* pbuff = (Buffer*) &stream->ResponseData;
                BufferStream buff(pbuff->Data(), pbuff->Length());
                PDEBUG("GetGooglePlayUserInfo get responsedata: lenght:%d", ((Buffer)stream->ResponseData).Length());
                PineString buff_uid = buff.ReadSStr();
                PineString buff_uname = buff.ReadSStr();

                UNICHAR* temp_data = (UNICHAR*)buff_uid.Data();

                int userIdLength = buff_uid.Length();
                int offset = userIdLength - 20; //19 is max support id length on server
                int start = 0;
                if (offset > 0) start = offset;
                offset = 0;
                for(int n = start; n< buff_uid.Length(); n++)
                {
                    userId[offset++] = (char)temp_data[n];
                }
                userId[offset] = '\0';

                sprintf(gpId, "%s", userId);
                PDEBUG("GetGooglePlayUserInfo userId:%s", userId);
                PDEBUG("GetGooglePlayUserInfo gpId:%s", gpId);

                temp_data = (UNICHAR*)buff_uname.Data();
                //userName = new char[buff_uname.Length() + 1];
                offset = 0;
                for(int n = 0; n< buff_uname.Length(); n++)
                {
                    userName[offset++] = (char)temp_data[n];
                }
                userName[offset] = '\0';
                PDEBUG("GetGooglePlayUserInfo user name: %s", userName);
			}

			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;

			hsvc->Status = PineServiceResult::Responsed;
		}
	}
}

extern "C" unsigned long long StringToU64(const char * sz)
{
	unsigned long long u64Result = 0;
	while (*sz != '\0')
	{
		u64Result *= 10;
		u64Result += *sz
			- '0';
		sz++;
	}
	return u64Result;
}

extern "C" void	OS_AsynGetGoogleFriendIdList(PineServiceHandle* hsvc, PineAccountIdList* account_list) {
	if(hsvc->isFree()){
    	hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::GooglePlayGetFriends);
    	if(hsvc->Handle > -1){
    		hsvc->Status = PineServiceResult::WaitingResponse;
    	}
    }
    else if(hsvc->Handle > -1){
    	if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){

    		PDEBUG("get google play friends responsed");
    		const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);

    		if(stream != NULL && stream->ErrorCode == 0){
    			Buffer* pbuff = (Buffer*)&stream->ResponseData;
    			BufferStream buff(pbuff->Data(), pbuff->Length());
    			int account_num = buff.ReadInt32();
    			account_list->init(account_num);

    			for(int a = 0; a< account_num; a++)
    			{
    			    PineString buff_uid = buff.ReadSStr();
    			    unsigned long long ullId = StringToU64((const char *)buff_uid.Data());

                	PDEBUG("ullId = %llu\n", ullId);
                	long long llId = ullId;
                	PDEBUG("llId = %lld\n", llId);

    				account_list->setAccountAt(a, llId);
    				PDEBUG("friend:%lld", llId);
    			}
    		}

    		if (stream != NULL)
    		{
    			hsvc->ResponseStatus = stream->ErrorCode;
    		}
    		else
    		{
    			hsvc->ResponseStatus = -1;
    		}
    		hsvc->Status = PineServiceResult::Responsed;
    		ServiceStreamManager::freeStream(hsvc->Handle);
    		hsvc->Handle = -1;
    	}
    }
}
extern "C" const char* OS_GetGooglePlayAvatarUrl(){

}

extern "C" void OS_AsynGooglePlayPost(PineServiceHandle* hsvc, const char* msg)
 {
 	if(hsvc->isFree())
 	{
 		PineString msg_str(msg);
 		int buff_size = 2 + (2 * msg_str.Length());
 		BufferStream buff(buff_size);
 		buff.WriteSStr(&msg_str);
 		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::GooglePlayShareToGooglePlus, buff);
 		if(hsvc->Handle > -1){
 			hsvc->Status = PineServiceResult::WaitingResponse;
 		}
 	}
 	else if(hsvc->Handle > -1){
 		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
 			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
 			if (stream != NULL)
 			{
 				hsvc->ResponseStatus = stream->ErrorCode;
 			}
 			else
 			{
 				hsvc->ResponseStatus = -1;
 			}
 			ServiceStreamManager::freeStream(hsvc->Handle);
 			hsvc->Handle = -1;
 			hsvc->Status = PineServiceResult::Responsed;
 		}
 	}
 }

 extern "C" void OS_AsynGooglePlayPostScreen(PineServiceHandle* hsvc, const char* msg)
 {
 	if(hsvc->isFree())
 	{
 		PineString msg_str(msg);
 		int buff_size = 2 + (2 * msg_str.Length());
 		BufferStream buff(buff_size);
 		buff.WriteSStr(&msg_str);
 		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::GooglePlayShareScreenToGooglePlus, buff);
 		if(hsvc->Handle > -1){
 			hsvc->Status = PineServiceResult::WaitingResponse;
 		}
 	}
 	else if(hsvc->Handle > -1){
 		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
 			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
 			if (stream != NULL)
 			{
 				hsvc->ResponseStatus = stream->ErrorCode;
 			}
 			else
 			{
 				hsvc->ResponseStatus = -1;
 			}
 			ServiceStreamManager::freeStream(hsvc->Handle);
 			hsvc->Handle = -1;
 			hsvc->Status = PineServiceResult::Responsed;
 		}
 	}
 }

extern "C" void OS_AsynGooglePlayPostScore(PineServiceHandle* hsvc, BoardScoreItem item)
 {
 	if(hsvc->isFree())
 	{
 		PineString boar_id(item.BoardId);
 		int buff_size = 2 + (2 * boar_id.Length()) + 4;
 		BufferStream buff(buff_size);
 		buff.WriteSStr(&boar_id);
 		buff.WriteInt32(item.Score);
 		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::GooglePlayPostScore, buff);
 		if(hsvc->Handle > -1){
 			hsvc->Status = PineServiceResult::WaitingResponse;
 		}
 	}
 	else if(hsvc->Handle > -1){
 		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
 			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
 			if (stream != NULL)
 			{
 				hsvc->ResponseStatus = stream->ErrorCode;
 			}
 			else
 			{
 				hsvc->ResponseStatus = -1;
 			}
 			ServiceStreamManager::freeStream(hsvc->Handle);
 			hsvc->Handle = -1;
 			hsvc->Status = PineServiceResult::Responsed;
 		}
 	}
 }

 extern "C" void OS_AsynGooglePlayGetLeaderBoardRank(PineServiceHandle* hsvc, const char * boardId, int * currentRank)
 {
 	if(hsvc->isFree())
 	{
 		PineString boar_id(boardId);
 		int buff_size = 2 + (2 * boar_id.Length());
 		BufferStream buff(buff_size);
 		buff.WriteSStr(&boar_id);
 		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::GooglePlayGetLeaderBoardRank, buff);
 		if(hsvc->Handle > -1){
 			hsvc->Status = PineServiceResult::WaitingResponse;
 		}
 	}
 	else if(hsvc->Handle > -1){
 		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
 			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
 			if (stream != NULL)
 			{
 			    Buffer* pbuff = (Buffer*)&stream->ResponseData;
                BufferStream buff(pbuff->Data(), pbuff->Length());
                *currentRank = buff.ReadInt32();
                PDEBUG("jni - currentRank = %d", currentRank);
 				hsvc->ResponseStatus = stream->ErrorCode;
 			}
 			else
 			{
 				hsvc->ResponseStatus = -1;
 			}
 			ServiceStreamManager::freeStream(hsvc->Handle);
 			hsvc->Handle = -1;
 			hsvc->Status = PineServiceResult::Responsed;
 		}
 	}
 }

extern "C" void OS_AsynGooglePlayShowLeaderBoard(PineServiceHandle* hsvc, const char* board_id)
{
	if(hsvc->isFree())
	{
		PineString boar_id(board_id);
		int buff_size = 2 + (2 * boar_id.Length());
		BufferStream buff(buff_size);
		buff.WriteSStr(&boar_id);
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::GooglePlayShowLeaderBoard, buff, false);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
			hsvc->Status = PineServiceResult::Responsed;
		}
	}
}

extern "C" void OS_AsynGooglePlayShowAchievements(PineServiceHandle* hsvc)
{
	if(hsvc->isFree())
	{
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::GooglePlayShowAchievements, false);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
			hsvc->Status = PineServiceResult::Responsed;
		}
	}
}

extern "C" void OS_AsynGooglePlayUnlockAchievement(PineServiceHandle* hsvc, const char* achi_id)
{
	
	if(hsvc->isFree())
	{
		PDEBUG("jni_OsFunction OS_AsynGooglePlayUnlockAchievement id: %s", achi_id);
		PineString str_achi_id(achi_id);
		int buff_size = 2 + (2 * str_achi_id.Length());
		BufferStream buff(buff_size);
		buff.WriteSStr(&str_achi_id);
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::GooglePlayUnlockAchievement, buff, false);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			PDEBUG("jni_OsFunction OS_AsynGooglePlayUnlockAchievement ResponseStatus: %i", hsvc->ResponseStatus);
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
			hsvc->Status = PineServiceResult::Responsed;
		}
	}
}

extern "C" void OS_AsynGoogleUpdateCloudSave(PineServiceHandle* hsvc, const char* localSaveName)
{
	if(hsvc->isFree())
	{
		PineString local_save_name(localSaveName);
		int buff_size = 2 + (2 * local_save_name.Length());
		BufferStream buff(buff_size);
		buff.WriteSStr(&local_save_name);
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::GooglePlayUpdateCloudSave, buff, false);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
				
			hsvc->Handle = -1;
			hsvc->Status = PineServiceResult::Responsed;
		}
	}
}


extern "C" void OpenAppStore(INT_TIME appleID)
{
}

extern "C" void OS_CaptureScreen()
{
}

extern "C" const char*		OS_GetAppId()
{
	return NULL;
}

extern "C" Orientation GetViewOrientation()
{
	return Orientation::Portrait;
}

extern "C" void OS_DeviceClearAllPushNotify() {}
extern "C" void OS_DeviceAskPushNotify() {}
extern "C" void OS_DevicePushNotify() {}

extern "C" bool OS_IsDocumentFileExist(const char* filename)
{
    char * filePath = GetAppDataPath(filename);
    PDEBUG("check exists file: %s", filePath);
    FILE * stream = fopen(filePath, "rb");
    delete [] filePath;
    if (stream != NULL)
    {
        PDEBUG("file EXISTS!");
        fclose(stream);
        return true;
    }
    PDEBUG("file NOT EXISTS!");
	return false;
}

extern "C" bool chartboostHasMoreGame()
{
	return false;
}

extern "C" void OS_AsynFacebookAppInvite(PineServiceHandle* hsvc, const char* applink, const char* preview_img_url)
{
	if(hsvc->isFree())
	{
		PineString str_applink(applink);
		PineString str_img_url(preview_img_url);
		int buff_size = 2 + (2 * str_applink.Length()) + (2 * str_img_url.Length());
		BufferStream buff(buff_size);
		buff.WriteSStr(&str_applink);
		buff.WriteSStr(&str_img_url);
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::FacebookAppInvite, buff);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			ServiceStreamManager::freeStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			hsvc->Handle = -1;
			hsvc->Status = PineServiceResult::Responsed;
		}
	}
}

extern "C" void OS_FacebookAppInvite(const char* applink, const char* preview_img_url)
{
	PineString str_applink(applink);
	PineString str_img_url(preview_img_url);
	int buff_size = 2 + (2 * str_applink.Length()) + (2 * str_img_url.Length());
	BufferStream buff(buff_size);
	buff.WriteSStr(&str_applink);
	buff.WriteSStr(&str_img_url);
	ServiceStreamManager::request((int)PineServiceCode::FacebookAppInvite, buff,true);
}


///////////////////////////////////////////////////
///UNITY ADS
///////////////////////////////////////////////////



extern "C" void OS_AsynUnityAdsInterstitialPrepare(PineServiceHandle* hsvc)
{
	if(hsvc->isFree())
	{
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::UnityAdsPrepareAds);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Status = PineServiceResult::Responsed;
			hsvc->Handle = -1;
		}
	}
}

extern "C" void OS_AsynUnityAdsInterstitialShow(PineServiceHandle* hsvc)
{
	if(hsvc->isFree())
	{
	    PDEBUG("CALL OS_AsynUnityAdsInterstitialShow\n");
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::UnityAdsShowAd);
		if(hsvc->Handle > -1){
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1){
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Status = PineServiceResult::Responsed;
			hsvc->Handle = -1;
		}
	}
}

extern "C" void OS_AsynUnityAdsRewardedVideoPrepare(PineServiceHandle* hsvc)
{
	if(hsvc->isFree())
	{
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::UnityAdsPrepareVideo);
		if(hsvc->Handle > -1)
		{
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1)
	{
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Status = PineServiceResult::Responsed;
			hsvc->Handle = -1;
		}
	}
}

extern "C" void OS_AsynUnityAdsRewardedVideoShow(PineServiceHandle* hsvc)
{
	if(hsvc->isFree())
	{
	    PDEBUG("OS_AsynUnityAdsRewardedVideoShow request UnityAdsShowVideo\n");
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::UnityAdsShowVideo);
		if(hsvc->Handle > -1)
		{
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1)
	{
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle))
		{
		    PDEBUG("OS_AsynUnityAdsRewardedVideoShow responed\n");
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			PDEBUG("error code: %d\n", hsvc->ResponseStatus);
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Status = PineServiceResult::Responsed;
			hsvc->Handle = -1;
		}
	}
}

extern "C" void OS_AsynUnityAdsSkippableVideoPrepare(PineServiceHandle* hsvc)
{
	if(hsvc->isFree())
	{
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::UnityAdsPrepareSkippableVideo);
		if(hsvc->Handle > -1)
		{
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1)
	{
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Status = PineServiceResult::Responsed;
			hsvc->Handle = -1;
		}
	}
}

extern "C" void OS_AsynUnityAdsSkippableVideoShow(PineServiceHandle* hsvc)
{
	if(hsvc->isFree())
	{
	    PDEBUG("OS_AsynUnityAdsSkippableVideoShow request UnityAdsShowVideo\n");
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::UnityAdsShowSkippableVideo);
		if(hsvc->Handle > -1)
		{
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1)
	{
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle))
		{
		    PDEBUG("OS_AsynUnityAdsSkippableVideoShow responed\n");
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			PDEBUG("error code: %d\n", hsvc->ResponseStatus);
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Status = PineServiceResult::Responsed;
			hsvc->Handle = -1;
		}
	}
}

//////////////////////////////////////////////////////
///////Vungle ads
/////////////////////////////////////////////////////
extern "C" void OS_AsynVungleAdsPrepare(PineServiceHandle* hsvc)
{
	if(hsvc->isFree())
	{
	    PDEBUG("OS_AsynVungleAdsPrepare");
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::AdVunglePrepare);
		if(hsvc->Handle > -1)
		{
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1)
	{
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
			    PDEBUG("OS_AsynVungleAdsPrepare respond stream is null???");
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Status = PineServiceResult::Responsed;
			hsvc->Handle = -1;
		}
	}
}

extern "C" void OS_AsynVungleAdsShow(PineServiceHandle* hsvc)
{
	if(hsvc->isFree())
	{
		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::AdVungleShow);
		if(hsvc->Handle > -1)
		{
			hsvc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else if(hsvc->Handle > -1)
	{
		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
			if (stream != NULL)
			{
				hsvc->ResponseStatus = stream->ErrorCode;
			}
			else
			{
				hsvc->ResponseStatus = -1;
			}
			ServiceStreamManager::freeStream(hsvc->Handle);
			hsvc->Status = PineServiceResult::Responsed;
			hsvc->Handle = -1;
		}
	}
}

/*
extern "C" void OS_AsyncGoogleAnalyticsLogEvent(PineServiceHandle* hsvc, const char *title, const char* decs, const char * type, const char * value)
 {
     if(hsvc->isFree())
     {
         PineString title_str(title);
         PineString decs_str(decs);
         PineString type_str(type);
         PineString value_str(value);
     	int buff_len = 2 + 2 * title_str.Length() + 2 + 2 * decs_str.Length() + 2 + 2 * type_str.Length() + 2 + 2 * value_str.Length();
     	PDEBUG("Firebase analytic log event request data len:%d", buff_len);
     	BufferStream buff(buff_len);
     	buff.WriteSStr(&title_str);
     	buff.WriteSStr(&decs_str);
     	buff.WriteSStr(&type_str);
     	buff.WriteSStr(&value_str);
     	hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::FirebaseAnalyticsLogEvent, buff, true);
     	if(hsvc->Handle > -1)
     	{
     		hsvc->Status = PineServiceResult::WaitingResponse;
     	}
     }
     else if(hsvc->Handle > -1)
     {
     	if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
     		const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
     		if (stream != NULL)
     		{
     			hsvc->ResponseStatus = stream->ErrorCode;
     		}
     		else
     		{
     			hsvc->ResponseStatus = -1;
     		}
     		ServiceStreamManager::freeStream(hsvc->Handle);
     		hsvc->Status = PineServiceResult::Responsed;
     		hsvc->Handle = -1;
     	}
     }
 }
*/
 extern "C" void OS_GoogleAnalyticsLogEvent(const char *title, const char* decs, const char * type, const char * value)
 {
    PineString title_str(title);
    PineString decs_str(decs);
    PineString type_str(type);
    PineString value_str(value);
    int buff_len = 2 + 2 * title_str.Length() + 2 + 2 * decs_str.Length() + 2 + 2 * type_str.Length() + 2 + 2 * value_str.Length();
    PDEBUG("Firebase analytic log event request data len:%d, title len = %d, decs len = %d, type len = %d, value len = %d", buff_len, title_str.Length(), decs_str.Length(), type_str.Length(), value_str.Length());
    BufferStream buff(buff_len);
    buff.WriteSStr(&title_str);
    buff.WriteSStr(&decs_str);
    buff.WriteSStr(&type_str);
    buff.WriteSStr(&value_str);
    ServiceStreamManager::request((int)PineServiceCode::FirebaseAnalyticsLogEvent, buff, true);
 }


 #define LOCAL_PUSH_SAVE_FILE_NAME "localPush.p"
 #define LOCAL_PUSH_MAX_ITEM 5
 static PineLocalPushItem localPushItems [LOCAL_PUSH_MAX_ITEM];

 extern "C" void OS_SetLocalNotification(int id, const char *message, const char * ticker, int second)
 {
 /*
     PineString msg_str(message);
     int buff_len = 4 + 2 + 2 * msg_str.Length();
     PDEBUG("Set local notification request message: %s, second: %d, data len:%d", message, second, buff_len);
     BufferStream buff(buff_len);
     buff.WriteInt32(second);
     buff.WriteSStr(&msg_str);
     ServiceStreamManager::request((int)PineServiceCode::SetLocalNotification, buff, true);
    */
    PDEBUG("Set local notification request id: %d, message: %s, ticker: %s, second: %d", id, message, ticker, second);
    Buffer* buff = OS_LoadAppData(LOCAL_PUSH_SAVE_FILE_NAME);
    bool has_change = false;
    if (buff != NULL)
    {
        bool is_success = false;
        char * buff_data = (char*)buff->Data();
        for (int i = 0; i < LOCAL_PUSH_MAX_ITEM; i ++)
        {
            localPushItems[i].LoadFromBuffer(buff_data, PineLocalPushItem::GetDataLength()*i);
            if (localPushItems[i].GetId() == -1)
            {
                localPushItems[i].SetId(id);
                localPushItems[i].SetMessage(message);
                localPushItems[i].SetTicker(ticker);
                localPushItems[i].SetSecond(second);
                is_success = true;
                has_change = true;
                break;
            }
            else if (localPushItems[i].GetId() == id)
            {
                is_success = true;
                localPushItems[i].SetMessage(message);
                localPushItems[i].SetTicker(ticker);
                localPushItems[i].SetSecond(second);
                has_change = true;
                break;
            }
        }
        if (!is_success)
        {
            PDEBUG("LocalPushItemList is full!\n");
        }
    }
    else
    {
        localPushItems[0].SetId(id);
        localPushItems[0].SetMessage(message);
        localPushItems[0].SetTicker(ticker);
        localPushItems[0].SetSecond(second);
        has_change = true;
    }
    if (has_change)
    {
        int data_length = PineLocalPushItem::GetDataLength()*LOCAL_PUSH_MAX_ITEM;
        Buffer * buff = new Buffer(data_length);
        for (int i = 0; i < LOCAL_PUSH_MAX_ITEM; i ++)
        {
            localPushItems[i].SaveToBuffer((char*)buff->Data(), PineLocalPushItem::GetDataLength()*i);
        }
        //PDEBUG("Save local push data: %s", (char*)buff->Data());
        OS_SaveAppData(LOCAL_PUSH_SAVE_FILE_NAME, (char*)buff->Data(), data_length, false);
        if (buff != NULL)
        {
            PDEBUG("Create new local push save file success!");
            delete buff;
        }
    }
 }

 extern "C" void OS_AsyncLoginGameSpark(PineServiceHandle* hsvc){

 	if(hsvc->isFree())
 	{
 		PDEBUG("Game spark request login");
 		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::GameSparkLogin);
 		if(hsvc->Handle > -1){
 			hsvc->Status = PineServiceResult::WaitingResponse;
 		}
 	}
 	else if(hsvc->Handle > -1){
 		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
 			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
 			if (stream != NULL)
 			{
 				hsvc->ResponseStatus = stream->ErrorCode;
 			}
 			else
 			{
 				hsvc->ResponseStatus = -1;
 			}
 			ServiceStreamManager::freeStream(hsvc->Handle);
 			PDEBUG("Login game spark:%d",  hsvc->ResponseStatus);
 			hsvc->Status = PineServiceResult::Responsed;
 			hsvc->Handle = -1;
 		}
 	}
 }

 extern "C" void OS_AsyncGameSparkUploadSave(PineServiceHandle* hsvc){

  	if(hsvc->isFree())
  	{
  		PDEBUG("Game spark request upload save");
  		hsvc->Handle = ServiceStreamManager::request((int)PineServiceCode::GameSparkUploadSave);
  		if(hsvc->Handle > -1){
  			hsvc->Status = PineServiceResult::WaitingResponse;
  		}
  	}
  	else if(hsvc->Handle > -1){
  		if(ServiceStreamManager::isStreamResponse(hsvc->Handle)){
  			const ServiceStream* stream = ServiceStreamManager::getStream(hsvc->Handle);
  			if (stream != NULL)
  			{
  				hsvc->ResponseStatus = stream->ErrorCode;
  			}
  			else
  			{
  				hsvc->ResponseStatus = -1;
  			}
  			ServiceStreamManager::freeStream(hsvc->Handle);
  			PDEBUG("Game spark upload save:%d",  hsvc->ResponseStatus);
  			hsvc->Status = PineServiceResult::Responsed;
  			hsvc->Handle = -1;
  		}
  	}
  }