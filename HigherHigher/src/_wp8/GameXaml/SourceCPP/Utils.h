#if !_UTILS_H
#define _UTILS_H 1
#define ID3D11TEXTURE ID3D11Texture2D
#define printf Utils::Loga
#include "../../../game/Lib/DataType/CType.h"
class Utils
{
private:
	//static PhoneDirect3DXamlAppComponent::ServerStream^ _stream_delete_file;
public:
	static const int INSTALL_PATH;
	static const int LOCAL_PATH;
	static const int ROAMING_PATH;
	static const int TEMP_PATH;
	static const int RESOURCE_PATH;
	static const int DATA_PATH;

	static char* getURLRoamingPath();
	static char* getURLTempPath();
	static char* getURLAppInstalled();
	static char* getURLLocalPath();
	static char* getAbsoluteURL(const char* file_name,int path_mode=INSTALL_PATH);
	static char* ConvertStringToCharArray(Platform::String^ string);
	static wchar_t* ConverCharToUnichar(const char* string);
	static  Platform::String^ ConvertCharToString(const char* string);

	static bool FileExists(const char* file_name);
	static void Log(Platform::String^ string);
	static void Log2(const char* string);
	static void Loga(const char* string,...);

	static void Sleep(long long time);
	static void DownloadFile(const char* url, const char *file_full_path);
	static bool DeleteFile(const char* url);
	static void AsynDeleteFile(SVHANDLE* handle, const char* file_name);
	//sprite manager
};
	 
#endif