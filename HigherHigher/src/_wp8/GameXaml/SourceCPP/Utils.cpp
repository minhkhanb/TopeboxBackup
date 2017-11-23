#include "pch.h"
#include "Utils.h"
#include <string>
#include <time.h>
#include <GameCore\CGame.h>
#include <ppltasks.h>
#include "ServerStream/ServerStreamManager.h"

using namespace Windows::Storage;
using namespace Windows::Networking;
using namespace std;
using namespace Platform;
using namespace Windows::System::Threading;
using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;
//using namespace Microsoft::WindowsAzure::MobileServices;

using namespace Concurrency;
const int Utils::INSTALL_PATH = 0;
const int Utils::LOCAL_PATH = 1;
const int Utils::ROAMING_PATH = 2;
const int Utils::TEMP_PATH = 3;
const int Utils::RESOURCE_PATH = 4;
const int Utils::DATA_PATH = 5;

 void Utils::DownloadFile(const char* url, const char *file_full_path)
 {
	 /*
	Windows::Storage::StorageFolder^ folder = ApplicationData::Current->RoamingFolder;
	String^ file_name = "index.html";
	String^ strUrl = "http://youarmy.blob.core.windows.net/pinepromotions/529961657.bpine";
	Uri^ uri = ref new Uri(strUrl);
	StorageFile^ file = nullptr;
	RandomAccessStreamReference^ thum = RandomAccessStreamReference::CreateFromUri(uri);
	task<StorageFile> streamTask = StorageFile::CreateStreamedFileFromUriAsync(file_name, uri, thum);//.AsTask<StorageFile>();
    streamTask.Wait();
    StorageFile storageFile = streamTask.Result;
	*/
 }

 void Utils::Sleep(long long time)
 {
	 clock_t c = clock();
	 while(clock()-c < time);
 }
 
bool Utils::FileExists(const char* file_name)
{
	FILE* fp = fopen(file_name, "rb");
	if(fp != NULL)
	{
		fclose(fp);
		return true;
	}
	return false;
}
Platform::String^ Utils::ConvertCharToString(const char* string)
{
	
	std::string s_str = std::string(string);
	std::wstring wid_str = std::wstring(s_str.begin(), s_str.end());
	const wchar_t* w_char = wid_str.c_str();
	Platform::String^ p_string = ref new Platform::String(w_char);
	return p_string;
}
char* Utils::getAbsoluteURL(const char* file_name,int path_mode)
{
	Platform::String^ output = ConvertCharToString(file_name);
	switch(path_mode)
	{
	case INSTALL_PATH:
		{
			Windows::ApplicationModel::Package^ package = Windows::ApplicationModel::Package::Current;
			Windows::Storage::StorageFolder^ installedLocation = package->InstalledLocation;
			output =installedLocation->Path+"\\" + output;
			return ConvertStringToCharArray(output);
		}
		break;
	case LOCAL_PATH:
		{
			Windows::Storage::StorageFolder^ local =  ApplicationData::Current->LocalFolder;
			output =local->Path+"\\" + output;
			return ConvertStringToCharArray(output);
		}
		break;
	case ROAMING_PATH:
		{
			Windows::Storage::StorageFolder^ roaming =  ApplicationData::Current->RoamingFolder;
			output =roaming->Path+"\\" + output;
			return ConvertStringToCharArray(output);
		}
		break;
	case TEMP_PATH:
		{
			Windows::Storage::StorageFolder^ temp =  ApplicationData::Current->TemporaryFolder;
			output =temp->Path+"\\" + output;
			return ConvertStringToCharArray(output);
		}
		break;
	case RESOURCE_PATH:
		{
			Windows::ApplicationModel::Package^ package = Windows::ApplicationModel::Package::Current;
			Windows::Storage::StorageFolder^ installedLocation = package->InstalledLocation;
			output =installedLocation->Path+"\\Res\\" + output;
			return ConvertStringToCharArray(output);
		}break;
	case DATA_PATH:
		{
			Windows::Storage::StorageFolder^ local =  ApplicationData::Current->LocalFolder;
			output = local->Path + "\\" + output;
			return ConvertStringToCharArray(output);
			//Windows::ApplicationModel::Package^ package = Windows::ApplicationModel::Package::Current;
			//Windows::Storage::StorageFolder^ local = ApplicationData::Current->LocalFolder;
			//Windows::Storage::StorageFile^ file = local->
			//return getURLLocalPath();

		}break;
	}
	return ConvertStringToCharArray(output);
}
char* Utils::getURLRoamingPath()
{
	Windows::Storage::StorageFolder^ roaming =  ApplicationData::Current->RoamingFolder;
	Platform::String^ output = roaming->Path;
	return ConvertStringToCharArray(output);
}
char* Utils::getURLTempPath()
{
	Windows::Storage::StorageFolder^ temp =  ApplicationData::Current->TemporaryFolder;
	Platform::String^ output = temp->Path;
	return ConvertStringToCharArray(output);
}
 char* Utils::getURLLocalPath()
 {
	
	 Windows::Storage::StorageFolder^ local =  ApplicationData::Current->LocalFolder;
	 Platform::String^ output = local->Path;
	 return ConvertStringToCharArray(output);
 }
 char* Utils::getURLAppInstalled()
 {
	 //get local folder
	Windows::ApplicationModel::Package^ package = Windows::ApplicationModel::Package::Current;
	Windows::Storage::StorageFolder^ installedLocation = package->InstalledLocation;
	Platform::String^ output =installedLocation->Path;
	return ConvertStringToCharArray(output);
 }

char* Utils::ConvertStringToCharArray(Platform::String^ string)
 {
	 if(string != nullptr)
	 {
		const wchar_t *W = string->Data();

		int Size = wcslen( W );
		char *CString= new char[Size + 1];
		CString[Size] = 0;
		int offset = 0;
		for(int y=0;y<Size; y++)
		{
			if((W[y] >= 32 && W[y] <= 126)){
				CString[offset] = (char)W[y];
				offset++;
			}else{
				CString[offset] = '!';
				offset++;
			}
		}
		CString[offset] = '\0';
		return CString;
	 }
	 return NULL;
 }

 wchar_t* Utils::ConverCharToUnichar(const char* string)
 {
	 int size = strlen(string);
	 wchar_t * ret = new wchar_t[size+1];
	 ret[size] = '\0';
	 for(int i = 0; i < size; i++)
	 {
		 ret[i] = string[i]&0xff;
	 }
	 return ret;
 }

void Utils::Log(Platform::String^ string)
{
//#if _DEBUG
	const wchar_t *W = string->Data();
	OutputDebugString(W);
//#endif
}
void Utils::Log2(const char* string)
{
//#if _DEBUG
	OutputDebugString(ConverCharToUnichar(string));
//#endif
}

void Utils::Loga(const char* str,...)
{
#if _DEBUG
  char buf[2048];
  va_list ptr;
  va_start(ptr,str);
  vsprintf(buf,str,ptr);
  //Log2(buf);
  OutputDebugStringA(buf);
#endif
  
}
bool Utils::DeleteFile(const char* url){
	Windows::Storage::StorageFolder^ folder = ApplicationData::Current->LocalFolder;
	Platform::String^ fname = ConvertCharToString(url);
	create_task(ApplicationData::Current->LocalFolder->GetFileAsync(fname)).then([](task<Windows::Storage::StorageFile^> task)
	{
		try{
			Windows::Storage::StorageFile^ file = task.get();
			if (file != nullptr){
				create_task(file->DeleteAsync()).then([](){
					Loga("success");
				});
			}
		}
		catch (...){
			Loga("error");
		}
		//Windows::Storage::StorageFile^ file =
	});
	//Windows::Storage::StorageFile^ storage_file = ApplicationData::Current->LocalFolder->GetFileAsync(url)
	return true;
}
SVHANDLE* _tmp_handle_delete_file = NULL;
void Utils::AsynDeleteFile(SVHANDLE* handle, const char* file_name){
	if (_tmp_handle_delete_file == NULL){
		if (handle->Status == SVRESULT::SV_DATA_FREE && handle->Handle == -1){
			//get a free stream
			handle->Handle = PhoneDirect3DXamlAppComponent::ServerStreamManager::Request();
			if (handle->Handle > -1){
				//delete async
				_tmp_handle_delete_file = handle;
				Platform::String^ fname = ConvertCharToString(file_name);
				create_task(ApplicationData::Current->LocalFolder->GetFileAsync(fname)).then([](task<Windows::Storage::StorageFile^> task)
				{
					try{
						Windows::Storage::StorageFile^ file = task.get();
						if (file != nullptr){
							create_task(file->DeleteAsync()).then([](){
								//when delete finished
								_tmp_handle_delete_file->Status = SVRESULT::SV_DATA_SUCCESS;
								_tmp_handle_delete_file = NULL;
							});
						}
						else{
							//when file doesn't exists
							_tmp_handle_delete_file->Status = SVRESULT::SV_DATA_SUCCESS;
							_tmp_handle_delete_file = NULL;
						}
					}
					catch (...){
						Loga("error");
						//when deleting file get error
						_tmp_handle_delete_file->Status = SVRESULT::SV_DATA_ERROR_UNKNOW;
						_tmp_handle_delete_file = NULL;
					}
				});
			}
			else{
				//case have no free stream
				handle->Status = SVRESULT::SV_DATA_ERROR_UNKNOW;
			}
		}
		else{

		}
	}
}
extern "C" const char* GetiPhoneUDID()
{
	return 0;
}
extern "C" const char* GetDocumentFilePath(const char* filename)
{
	return 0;
}



//text input
extern "C" void StartTextInput(char* text_buff, unsigned int buff_size, int x, int y, int w, int h)
{
	return ;
}
extern "C" void CloseTextInput()
{
	return ;
}
extern "C" bool IsTextInputDone()
{
	return 0;
}

extern "C" void encrypttionBuffer()
{
}
extern "C" bool decryptionBuffer()
{
	return true;
}
