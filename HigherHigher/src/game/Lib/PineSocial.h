#if !_PINE_SOCAIL_H
#define _PINE_SOCAIL_H 1
#include "PineImage.h"
#define FRIEND_LIST_MAX 5000
#define NAME_LENGTH (128)
#define SOCIAL_ID_STRING_LENGTH	(128)

#define  PINESOCIAL_STATE_SUCCESS       (0)
#define  PINESOCIAL_STATE_COMPLETED     (0)
#define  PINESOCIAL_STATE_NONE          (0)
#define  PINESOCIAL_STATE_INIT          (1)
#define  PINESOCIAL_STATE_LOAD_FRIEND   (2)
#define  PINESOCIAL_STATE_LOAD_SCORE    (3)
#define  PINESOCIAL_STATE_GC_LOGIN      (4)

#define  NON_SYSTEM     (0)
#define  PINE_SYSTEM    (100000000000000000)
#define  GC_SYSTEM      (200000000000000000)
#define  FB_SYSTEM      (300000000000000000)
#define  TW_SYSTEM      (400000000000000000)
#define  POT_SYSTEM     (900000000000000000)

#define SOCIAL_FLAG_NONE                    (0)
#define SOCIAL_FLAG_LOGGING                 (1)
#define SOCIAL_FLAG_NO_ACCOUNT_LOGIN        (2)
#define SOCIAL_FLAG_ACCESS_DENINE           (3)
#define SOCIAL_FLAG_FULL_ACCESS             (4)
#define SOCIAL_FLAG_NO_NETWORK              (5)
#define SOCIAL_FLAG_WAIT_GET_ME             (6)
#define SOCIAL_FLAG_GET_ME_SUCCESS          (7)
#define SOCIAL_FLAG_ID_IS_STRING_FORMAT     (8)
#define SOCIAL_FLAG_ID_LOGOUT               (9)

#define RATING_FILE "gamerated.dat"
#define LOCAL_USER_INFO "local_info.dat"

class PineSocial
{
protected:
    char _user_name[NAME_LENGTH];
    char _url_avatar[1024];

#if USE_PINESOCIAL_AT_ID_STRING
	char _idString[SOCIAL_ID_STRING_LENGTH];
#endif

	INT_TIME _user_id;
#if USE_PINESOCIAL_AT_ID_STRING
    char _user_id_app_scope[SOCIAL_ID_STRING_LENGTH];
#else
    INT_TIME _user_id_app_scope;
#endif
    int _flag;
    int _state;
    INT_TIME _type_system;
    int _num_of_friend;
    int _num_of_following;
    int _num_of_follower;

#if USE_PINESOCIAL_AT_ID_STRING
	char _socialFriendIdString[FRIEND_LIST_MAX][SOCIAL_ID_STRING_LENGTH];
#else
	INT_TIME _social_friend[FRIEND_LIST_MAX];
#endif
    char _socialFriendName[FRIEND_LIST_MAX][SOCIAL_ID_STRING_LENGTH];

	
    INT_TIME _social_follwing[FRIEND_LIST_MAX];
    INT_TIME _social_follower[FRIEND_LIST_MAX];
    
public:
    void SetURLAvartar(const char* url);
    const char* GetUserNameAtIndex(int index);
    const char* GetURLAvatar();
    static bool  _has_message_box;
    static bool _iap_loaded;
    static bool HasloggingSystem;

    static bool PingToHost(const char* host, int time_out);
    static void LoadIAP();
    static bool _capture_screenshot;
    static bool wasCaptureScreen(){return !_capture_screenshot;};
    static void SetCaptureScreen(){_capture_screenshot = true;};
    static void SetCaptureCompleted(){_capture_screenshot = false;};
    static int _rating_checked;
    static void OpenURL(const char* url);
    static void ShowPopUpRating(const char* title,const char* body);
    static bool HasRating();
    static void ShowMessageBox(const char* feed, const char * title = "Notice");
    static void ShowMessageAsk(const char* feed,int code);
    static void ProcessMessageAsk(int code);
    static void InviteFacebookFriend(const char* feed, const char* game_title,INT_TIME* friendids,int num_friend);
    static void InviteTwitterFriends(const char* feed, const char* game_title);
    static void InviteMessage(const char* feed, const char* game_title);
    static void InviteMail(const char* feed, const char* game_title);
    static void DownloadAvatar(const char* url,const char *filename,float w,float h);
    static void PostScreenToFacebook(const char* feed);
    static void PostScreenToTwitter(const char* feed);
    static bool IsLoginTwitter();
    static bool IsLoginFacebook();
    static bool IsDeviceHacked();
    
    int GetNumOfFriend(){return _num_of_friend;};
#if USE_PINESOCIAL_AT_ID_STRING
	char *GetFriendUID(int index);
#else
    INT_TIME GetFriendUID(int index);
#endif

#if USE_PINESOCIAL_AT_ID_STRING
	void GetFriendUIDInGame(int index, char * output);
#else
    INT_TIME GetFriendUIDInGame(int index);
#endif

    const char* GetAvatarURL(){return _url_avatar;};
    
    ~PineSocial(){;};
    
#if USE_PINESOCIAL_AT_ID_STRING
    void SetUserIDScope(const char* user_id);
    const char* GetUserScopeID();
#else
    void SetUserIDScope(INT_TIME user_id);
    INT_TIME GetUserScopeID();
#endif
    
    bool IsSystem(INT_TIME system);
    void ResetFriendList();

#if USE_PINESOCIAL_AT_ID_STRING
	void AddFriend(const char * friendID, const char* friendName);
#else
    void AddFriend(INT_TIME friend_id, const char* friendName);
#endif

#if USE_PINESOCIAL_AT_ID_STRING
	void SetInfomation(const char* user_id,const char* user_name);
#else
    void SetInfomation(INT_TIME user_id,const char* user_name);
#endif

#if USE_PINESOCIAL_AT_ID_STRING
	int GetIndexFriendByUIDInGame(const char* user_id);
#else
	int GetIndexFriendByUIDInGame(INT_TIME user_id);
#endif

#if USE_PINESOCIAL_AT_ID_STRING
	int GetIndexFriendByID(const char* user_id);
#else
    int GetIndexFriendByID(INT_TIME user_id);
#endif

    void GoFacebookPineFanPage();
    virtual void LoadAvatarURL() = 0;
    virtual void LoadFriends() = 0;
    virtual void LoadFollowers() = 0;
    virtual void LoadFollowings() = 0;
    //virtual void InviteFriend() = 0;
    
    virtual bool PostToWall(const char* feed)=0;
    virtual bool PostScreenToWall(const char* feed)=0;
#if USE_PINESOCIAL_AT_ID_STRING
    virtual const char* GetUserAvartarURL(const char* id) = 0;
#else
    virtual const char* GetUserAvartarURL(INT_TIME id) = 0;
#endif
    virtual bool PostImageToWall(const char* feed, const char* realUrlImage) = 0;
    PineSocial();
    virtual bool Init() = 0;
    virtual bool InitNoneWait() = 0;
    INT_TIME GetUserID();
    INT_TIME GetUserIDInGameID();
    INT_TIME GetUserIDScopeInGameID();
    const char* GetUserName();

#ifdef USE_PINESOCIAL_AT_ID_STRING
	const char* GetIdString();
	void GetUserIdString(const char *idIn, char * idOut);
#endif

    void SetFlag(int flag);
    int GetFlag();
    void SetState(int state);
    int GetState();
    
    INT_TIME GetLastUser();
    void SaveLastUser();
    int GetSizeDataLastUser();
    void LoadDataLastUser(char* buff);
    
    PineSocial *_this;
  
    INT32 _unicodeUserName[NAME_LENGTH];
    int _currentUnicodeUserNameLength;
    bool _logined;
    bool HasLogined();
    
    static void ShareScreen(const char* feed, float x, float y, float w, float h);
    static void SaveScreenToDocument(const char* file_name, float x, float y, float w, float h);
    static void ShareImageWithURLDocument(const char* feed, const char* file_name);
};


#endif
