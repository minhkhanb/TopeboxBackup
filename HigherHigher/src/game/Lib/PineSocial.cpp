#include "pch.h"
#include "PineSocial.h"
#include "../GameCore/CGame.h"
#include "../GameCore/Os_Functions.h"
#include <stdlib.h>

int PineSocial::_rating_checked = false;
bool PineSocial::_capture_screenshot = false;
bool PineSocial::_iap_loaded = false;
bool PineSocial::_has_message_box = false;
bool PineSocial::HasloggingSystem = false;;
#if defined (MAC_OS)
extern "C" void OS_SystemMessageAsk(const char* feed,int code);
#endif
extern "C" void OS_BasicPostSocialWithCrop(const char* feed,float x, float y, float w, float h);
extern "C" void OS_CaptureScreenWithCrop(const char* file,float x, float y, float w, float h);
extern "C" void OS_ShareImageWithURLDocument(const char* feeds, const char* file);
void PineSocial::SaveScreenToDocument(const char* file_name, float x, float y, float w, float h)
{
#ifdef MAC_OS
    OS_CaptureScreenWithCrop(file_name,x/k_SCREEN_WIDTH,y/k_SCREEN_HEIGHT,w/k_SCREEN_WIDTH,h/k_SCREEN_HEIGHT);
#endif
}
void PineSocial::ShareImageWithURLDocument(const char* feed, const char* file_name)
{
#ifdef MAC_OS
	OS_ShareImageWithURLDocument(feed, file_name);
#endif
}
void PineSocial::ShareScreen(const char* feed, float x, float y, float w, float h)
{
#ifdef MAC_OS
    OS_BasicPostSocialWithCrop(feed,x/k_SCREEN_WIDTH,y/k_SCREEN_HEIGHT,w/k_SCREEN_WIDTH,h/k_SCREEN_HEIGHT);
#endif
}
bool PineSocial::HasLogined()
{
    return _logined;
}

void PineSocial::SetURLAvartar(const char* url)
{
    sprintf(_url_avatar,"");
    printf("\nurl avartar %s",url);
    if(url)
    {
        sprintf(_url_avatar,"%s",url);
    }
}
const char* PineSocial::GetURLAvatar()
{
    return _url_avatar;
}
void PineSocial::ShowMessageAsk(const char* feed,int code)
{
#if defined (MAC_OS)
	OS_SystemMessageAsk(feed,code);
#endif
}
void PineSocial::GoFacebookPineFanPage()
{
	PineSocial::OpenURL("https://www.facebook.com/345487638795425");
}

#if USE_PINESOCIAL_AT_ID_STRING
void PineSocial::SetUserIDScope(const char* user_id)
{
    sprintf(_user_id_app_scope, "%s", user_id);
}

const char* PineSocial::GetUserScopeID()
{
    return _user_id_app_scope;
}
#else

void PineSocial::SetUserIDScope(INT_TIME user_id)
{
	_user_id_app_scope = user_id;
}
INT_TIME PineSocial::GetUserScopeID()
{
	return _user_id_app_scope;
}

#endif

INT_TIME PineSocial::GetLastUser()
{
	
	char filename[256];
	sprintf(filename,"%s",OS_GetDocumentFilePath(LOCAL_USER_INFO));
	FILE* f= fopen(filename,"r");
	if(f == NULL)
	{
		return 0;
	}
	fseek(f,0,SEEK_END);
	long size = ftell(f);
	fseek(f,0,SEEK_SET);
	char* buff= new char[size+1];
	fread((void *)buff,sizeof(char),size,f);
	
	int offset = 0;
	INT_TIME local_id = CGame::_this->GetInt64At(buff,offset);
	offset += 8;
	char str_id[21];
	sprintf(str_id,"%lld",local_id);
	int step = CGame::_this->GetInt32At(buff,offset);
	offset += 16;
	for(int i= 0; i < strlen(str_id); i++)
	{
		int index = offset + (step*i);
		char c = '0'+(CGame::_this->GetCharAt(buff,index) - 'a');
		if(c != str_id[i])
		{
			local_id = 0;
			break;
		}
	}
	offset += (step*strlen(str_id));
	if(f!= NULL)
	{
		fclose(f);
	}
	//printf("\n Load local id = %lld",local_id);
    SAFE_DEL_ARRAY(buff);
	return local_id;
}
void PineSocial::SaveLastUser()
{
	if(this->GetUserID() > 0)
	{
		char filename[256];
		sprintf(filename,"%s",OS_GetDocumentFilePath(LOCAL_USER_INFO));
		FILE* f= fopen(filename,"w");
		if(f != NULL)
		{
			char str_id[21];
			sprintf(str_id,"%lld",this->GetUserIDInGameID());
			int step =  CMath::RANDOM(3,7);
			long length = 8+4+step*strlen(str_id)+1;
			char* buff = new char[length];
			for(int i= 0; i < length; i++)
			{
				buff[i]= CMath::RANDOM(0,255);
			}
			printf("\n save user id = %s",str_id);
			int offset = 0;
			CGame::_this->SetInt64At(buff,offset, this->GetUserIDInGameID());
			offset += 8;
			CGame::_this->SetInt32At(buff,offset,step);
			offset += 4;
			for(int i = 0; i < strlen(str_id);i++)
			{
				int index = offset+ i*step;
				char c = str_id[i]- '0' + 'a';
				CGame::_this->SetCharAt(buff,index,c);
			}
			offset += (strlen(str_id)*step);
			
			fwrite((void *)buff, sizeof(BYTE), length, f);
			fclose(f);
            SAFE_DEL_ARRAY(buff);
		}
	   
	}
}
int PineSocial::GetSizeDataLastUser()
{
	return 0;
}
void PineSocial::LoadDataLastUser(char* buff)
{
	
}


bool PineSocial::PingToHost(const char* host, int time_out)
{
	return OS_PingToHost(host,time_out);
}
 void PineSocial::LoadIAP()
{
	if(!_iap_loaded)
	{
		_iap_loaded = true;
		OS_LoadIAP();
	}
}
void PineSocial::OpenURL(const char* url)
{
	OS_BrowserWebPage(url);
}
bool PineSocial::HasRating()
{
    char filename[256];
    sprintf(filename,"%s",OS_GetDocumentFilePath(RATING_FILE));
    FILE* f= fopen(filename,"r");
    if(f != NULL)
    {
        fclose(f);
        return true;
    }
    return false;
}
void PineSocial::ShowPopUpRating(const char* title,const char* body)
{
    if(_rating_checked)
	{
		return;
	}
    //save checked as rating.
    char filename[256];
    sprintf(filename,"%s",OS_GetDocumentFilePath(RATING_FILE));
    FILE* f= fopen(filename,"r");
    if(f != NULL)
    {
        _rating_checked = true;
        fclose(f);
        return;
    }
	OS_RatingWithDialog(title, body);
    
   
	
}

bool PineSocial::IsDeviceHacked()
{
	return OS_IsDeviceHacked();
}
bool PineSocial::IsLoginTwitter()
{
	return OS_IsLogginTwitter();
}
bool PineSocial::IsLoginFacebook()
{
	return OS_IsLogginFacebook();
}
void PineSocial::ShowMessageBox(const char* feed, const char * title)
{
	if(!_has_message_box)
	{
		_has_message_box = true;
		OS_SystemWarning(feed, title);
	}
	//show mesagebox.
}
 void PineSocial::PostScreenToFacebook(const char* feed)
{
#if defined(MAC_OS) || defined(_WINDOWS)
	OS_PostScreenToFacebook(feed);
#elif defined(_WINDOWS_PHONE8) || defined(_WIN8)
	 OS_PostFacebook(feed);
#endif
}
void PineSocial::PostScreenToTwitter(const char* feed)
{
#if defined(MAC_OS) || defined(_WINDOWS)
	OS_PostScreenToTwitter(feed);
#elif defined(_WINDOWS_PHONE8) || defined(_WIN8)
	OS_PostTwitter(feed);
#endif
}
void PineSocial::InviteFacebookFriend(const char* feed, const char* game_title,INT_TIME* friendids,int num_friend)
{
#if defined(MAC_OS) || defined(_WINDOWS)
	OS_FacebookInviteFriend(feed,game_title,friendids,num_friend);
#elif defined(_WINDOWS_PHONE8) || defined(_WIN8)
	OS_PostFacebook(feed);
#endif
}
void PineSocial::InviteTwitterFriends(const char* feed, const char* game_title)
{
#if defined(MAC_OS) || defined(_WINDOWS)
	OS_PostInviteToTwitter(feed);
#elif defined(_WINDOWS_PHONE8) || defined(_WIN8)
	OS_PostTwitter(feed);
#endif
}
void PineSocial::InviteMessage(const char* feed, const char* game_title)
{
#if defined(MAC_OS) || defined(_WINDOWS)
	OS_Popup_Message(feed,game_title);
#elif defined(_WINDOWS_PHONE8) || defined(_WIN8)
	OS_SendSMS(game_title, feed);
#endif
}
void PineSocial::InviteMail(const char* feed, const char* game_title)
{
#if defined(MAC_OS) || defined(_WINDOWS)
	OS_Popup_Mail(feed,game_title);
#elif defined(_WINDOWS_PHONE8) || defined(_WIN8)
	OS_SendMail(game_title, feed);
#endif
}

void PineSocial::DownloadAvatar(const char* url,const char *filename,float w,float h)
{
	//printf("\nDownload avartar from url %s, to file %s w = %f,h=%f",url,filename,w,h);
	OS_LoadImageToPngFile(url,filename,w,h);
}
#if USE_PINESOCIAL_AT_ID_STRING
int PineSocial::GetIndexFriendByUIDInGame(const char* user_id)
{
	char tmp[32];
	for (int i = 0; i < _num_of_friend; i++)
	{
		GetFriendUIDInGame(i, tmp);
		if (strcmp(user_id, tmp) == 0)
		{
			return i;
		}
	}
	return -1;
}
#else
int PineSocial::GetIndexFriendByUIDInGame(INT_TIME user_id)
{
	for(int i = 0; i < _num_of_friend; i++)
	{
		if(user_id == GetFriendUIDInGame(i))
		{
			return i;
		}
	}
	return -1;
}
#endif


#if USE_PINESOCIAL_AT_ID_STRING
int PineSocial::GetIndexFriendByID(const char* user_id)
{
	for(int i = 0; i < _num_of_friend; i++)
	{
		if (strcmp(user_id, GetFriendUID(i)) == 0)
		{
			return i;
		}
	}
	return -1;
}
#else
int PineSocial::GetIndexFriendByID(INT_TIME user_id)
{
	for(int i = 0; i < _num_of_friend; i++)
	{
		if(user_id == GetFriendUID(i))
		{
			return i;
		}
	}
	return -1;
}
#endif


#if USE_PINESOCIAL_AT_ID_STRING
char* PineSocial::GetFriendUID(int index)
{
	if (index >= 0 && index <_num_of_friend)
	{
		return _socialFriendIdString[index];
	}
	return NULL;
}
#else
INT_TIME PineSocial::GetFriendUID(int index)
{
	if(index >= 0 && index <_num_of_friend)
	{
		return _social_friend[index];
	}
	return 0;
}
#endif


#if USE_PINESOCIAL_AT_ID_STRING
void PineSocial::GetFriendUIDInGame(int index, char* output)
{
	GetUserIdString(GetFriendUID(index), output);
}
#else
INT_TIME PineSocial::GetFriendUIDInGame(int index)
{
	return _type_system + GetFriendUID(index);
}
#endif


bool PineSocial::IsSystem(INT_TIME system)
{
	return (_type_system == system);
}
void PineSocial::ResetFriendList()
{
	_num_of_friend =0;
}

#if USE_PINESOCIAL_AT_ID_STRING
void PineSocial::AddFriend(const char * friendID, const char* friendName)
{
	if (_num_of_friend + 1 < FRIEND_LIST_MAX)
	{
		sprintf(_socialFriendIdString[_num_of_friend], "%s", friendID);
        sprintf(_socialFriendName[_num_of_friend], "%s", friendName);
		_num_of_friend++;
	}
}
#else
void PineSocial::AddFriend(INT_TIME friend_id, const char* friendName)
{
	if(_num_of_friend + 1 < FRIEND_LIST_MAX)
	{
		//printf("\nnew friend: %lld",friend_id);
		_social_friend[_num_of_friend] = friend_id;
        sprintf(_socialFriendName[_num_of_friend], "%s", friendName);
		_num_of_friend++;
	}
}
#endif

const char* PineSocial::GetUserNameAtIndex(int index)
{
    return _socialFriendName[index];
}
const char* PineSocial::GetUserName()
{
	return _user_name;
}

PineSocial::PineSocial()
{
    _logined = false;
#if USE_PINESOCIAL_AT_ID_STRING
	_user_id_app_scope[0] = '\0';
#else
    _user_id_app_scope = 0;
#endif
	_user_name[0] = '\0';
    _this = this;
}

INT_TIME PineSocial::GetUserID()
{
#if FACEBOOK_TRANSFER_IAP == 1
	return _user_id;
#endif
	
#if USE_PINESOCIAL_AT_ID_STRING
#else
#if FACEBOOK_API == 20
	if (IsSystem(FB_SYSTEM))
		return _user_id_app_scope;
#endif
#endif
	return _user_id;

}

INT_TIME PineSocial::GetUserIDInGameID()
{
#if FACEBOOK_TRANSFER_IAP == 1
	return _user_id + _type_system;
#endif
	
#if USE_PINESOCIAL_AT_ID_STRING
#else
#if FACEBOOK_API == 20
	if (IsSystem(FB_SYSTEM))
		return _user_id_app_scope + _type_system;
#endif
#endif
	return _user_id + _type_system;
}

#if USE_PINESOCIAL_AT_ID_STRING
#else
INT_TIME PineSocial::GetUserIDScopeInGameID()
{
	return _user_id_app_scope + _type_system;
}
#endif

void PineSocial::SetFlag(int flag)
{
	_flag = flag;
}
int PineSocial::GetFlag()
{
	return _flag;
}
void PineSocial::SetState(int state)
{
	_state = state;
}
int PineSocial::GetState()
{
	return _state;
}

#if USE_PINESOCIAL_AT_ID_STRING
void PineSocial::SetInfomation(const char* user_id, const char* user_name)
{
	sprintf(_idString, "%s", user_id);
	int count = 0;
	for(int i = 0; i < strlen(user_name); i++)
	{
		if(((user_name[i] >= 'a' && user_name[i] <= 'z')||
			(user_name[i]>='A' && user_name[i]<='Z') ||
			user_name[i] ==' '))
		{
			_user_name[count]=user_name[i];
			count++;
		}
	}
	_user_name[count] = '\0';
	if(strlen(_user_name) >= 8)
	{
		_user_name[8] = '.';
		_user_name[9] = '.';
		_user_name[10] = '.';
		_user_name[11] = '\0';
	}
}
#else
void PineSocial::SetInfomation(INT_TIME user_id,const char* user_name)
{
    //printf("\n id %lld name %s",user_id,user_name);
	_user_id = user_id;
	//snprintf(_user_name,50,"%s",user_name);
	int count = 0;
	for(int i = 0; i < strlen(user_name); i++)
	{
		if(((user_name[i] >= 'a' && user_name[i] <= 'z')||
			 (user_name[i]>='A' && user_name[i]<='Z') ||
			 user_name[i] ==' '))
		{
			_user_name[count]=user_name[i];
			count++;
		}
	}
	_user_name[count] = '\0';
	if(strlen(_user_name) >= 8)
	{
		_user_name[8] = '.';
		_user_name[9] = '.';
		_user_name[10] = '.';
		_user_name[11] = '\0';
	}
	//printf("\nInit information user_id = %lld user name = %s",_user_id,_user_name);
}
#endif

#if USE_PINESOCIAL_AT_ID_STRING
const char* PineSocial::GetIdString()
{
	return _idString;
}

void PineSocial::GetUserIdString(const char *idIn, char * idOut)
{
	switch (_type_system)
	{
	case PINE_SYSTEM:
		sprintf(idOut, "PI_%s", idIn);
		break;
	case GC_SYSTEM:
		sprintf(idOut, "GC_%s", idIn);
		break;
	case FB_SYSTEM:
		sprintf(idOut, "FB_%s", idIn);
		break;
	case TW_SYSTEM:
		sprintf(idOut, "TW_%s", idIn);
		break;
	case POT_SYSTEM:
		sprintf(idOut, "PO_%s", idIn);
		break;
	default:
		sprintf(idOut, "%s", idIn);
		break;
	}
	//duy.nguyen rem. please don't use strcat. It causes error in some platform
	//strcat(idOut, idIn);
}
#endif
