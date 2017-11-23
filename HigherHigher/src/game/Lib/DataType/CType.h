#if !defined(_H_CTYPE)
#define _H_CTYPE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
//#include <exception>

#include <assert.h>


#if defined(MAC_OS)
#   define ENUM enum class
#else
#   define ENUM enum class
#endif

#if defined(ANDROID_OS)
#include <android/log.h>
#endif

#ifdef __cplusplus

typedef unsigned char			UINT8;
typedef signed char				INT8;
typedef unsigned char			BYTE;

typedef unsigned short			UINT16;
typedef signed short			INT16;
typedef unsigned int			UINT32, EVENTID, COLOR, SVCID;

typedef signed int				INT32, EVENT_POINTER, HSERVICE;
typedef unsigned short			UNICHAR;
typedef long long               INT64;
typedef unsigned long long 		UINT64;

#if defined(MAC_OS)	
#	include <stdint.h>
#	define INT_TIME				int64_t
#	define NULL					0

#elif defined(ANDROID_OS)
#	define INT_TIME				INT64

#else
#	define INT_TIME				INT64
	typedef long long int		__INT64_TYPE__;
#endif

#if !defined(NULL)
#define NULL	0
#endif

struct PinePoint
{
	float X;
	float Y;
};

class PinePoint3D: public PinePoint
{
public:
	float Z;
};

struct PineSize
{
	float W;
	float H;
	//PineSize() :W(0), H(0){};
	//PineSize(float w, float h) :W(w), H(h){};
};

struct PineTransformParam
{
	float TranslateX;
	float TranslateY;
	float m11;
	float m12;
	float m21;
	float m22;
	PinePoint Transform(float x, float y) {
		PinePoint point;
		point.X = m11 * x + m12 *y + TranslateX;
		point.Y = m21 * x + m22 *y + TranslateY;
		return point;
	}
};

ENUM PineServiceCode
{
	FacebookLogin = 1,
	FacebookGetAvatar = 2,
	BrowserOpen = 3,
	Rating = 4,
	Purchase = 5,
	PurchaseGetList = 6,
	PurchaseRestore = 7,
	FacebookPost = 8,
	TwitterLogin = 9,
	TwitterPost = 10,
	FacebookGetAppFriend = 11,
	GameExit = 12,
	LoadImage = 13,
	TwitterGetFollwer = 14,
	SmsSend = 15,
	MailSend = 16,
	FileDownload = 17,
	FacebookPostPhoto = 18,
	AdmobInterstitialPrepare = 19,
	AdmobInterstitialShow = 20,
	AdmobBannerPrepare = 21,
	AdmobBannerShow = 22,
	AdmobBannerHide = 23,
	AdmobRewardedVideoPrepare = 24,
	AdmobRewardedVideoShow = 25,
	DialogShow = 26,
	FacebookPostScore = 27,
	TakeScreenShot = 28,
	FacebookPostScreen = 29,
	ShareScreenShotIntent = 30,
	ShareIntent = 31,
	SendEmailIntent = 32,
	GooglePlayIsLogin = 33,
	GooglePlayLogin = 34,
	GooglePlayUserInfo = 35,
	GooglePlayGetFriends = 36,
	GooglePlayPostScore = 37,
	GooglePlayGetLeaderBoardRank = 38,
	GooglePlayShowLeaderBoard = 39,
	GooglePlayShowAchievements = 40,
	GooglePlayShareToGooglePlus = 41,
	GooglePlayShareScreenToGooglePlus = 42,
	GooglePlayUnlockAchievement = 43,
	GooglePlayUpdateCloudSave = 44,
	GooglePlaySendGift = 45,

	// chartboost
	ChartboostInterstitialPrepare = 46,
	ChartboostInterstitialShow = 47,
	ChartboostRewardedVideoPrepare = 48,
	ChartboostRewardedVideoShow = 49,

	// unity3d ads 
	UnityAdsPrepareAds = 50,
	UnityAdsPrepareVideo = 51,
	UnityAdsPrepareSkippableVideo = 52,
	UnityAdsShowAd = 53,
	UnityAdsShowVideo = 54,
	UnityAdsShowSkippableVideo = 55,

	//vungle ads
#if defined (_WINDOWS_PHONE8) || defined (ANDROID_OS)
	AdVunglePrepare = 56,
	AdVungleShow = 57,
#endif

	FacebookAppInvite = 58,

	//firebase
	FirebaseAnalyticsLogEvent = 59,

	//Local notification
	SetLocalNotification = 60,

	//Game Spark back-end
	GameSparkLogin = 61,
	GameSparkPostScore = 62,
	GameSparkGetLeaderBoard = 63,
	GameSparkGetSave = 64,
	GameSparkUploadSave = 65,

#if defined(BUILD_SUPPORT)
	SupportGetAccountId = 900,
#endif
};

ENUM Orientation
{
	Portrait = 0,
	Landscape_90 = 1,
	Landscape_270 = 2,
};

ENUM BLEND_MODE
{
	ALPHA_FILLTER_NORMAL = 0,
	ALPHA_FILLTER_ADD = 1,
	ALPHA_FILLTER_CONSTRACT = 2,
	ALPHA_FILLTER_LIGHT = 3,
    ALPHA_FILLTER_LOW_BRIGHTNESS = 4,
	SCREEN = 5,
	ALPHA_FILTER_ONE = 6
} ;

ENUM SVRESULT
{
	SV_DATA_ERROR_NO_NETWORK = 400,
	SV_DATA_FREE = 0,
	SV_DATA_SUCCESS = 1,
	SV_DATA_ERROR_UNKNOW  = 2,
	SV_DATA_ERROR_NO_RECORD  = 3,
	SV_DATA_WAITING_RESPONSE = 4
    
};
ENUM PineServiceResult{
	Free = 0,
	WaitingResponse = 1,
	Responsed = 2
};

//struct to handle a server stream
//Handle is offset of stream
//Status is status of stream
struct SVHANDLE
{
	int RequestCode;
	int Handle;
	SVRESULT Status;
	int StatusCode;

	SVHANDLE(){ reset();};
	void reset(){ RequestCode = -1; Handle = -1; Status = (SVRESULT)0; StatusCode = 0; };
	bool isFree(){ return (Status == SVRESULT::SV_DATA_FREE) && (Handle == -1); };
	bool isSuccess(){ return (Status == SVRESULT::SV_DATA_SUCCESS); };
	bool isError(){ return (Status == SVRESULT::SV_DATA_ERROR_UNKNOW); };
	bool isFinish(){ return (Status != SVRESULT::SV_DATA_WAITING_RESPONSE); }
};

//RequestCode >= 1000
//ResponseStatus: > -1
class PineServiceHandle
{
public:
	HSERVICE Handle;
	SVCID RequestCode;
	PineServiceResult Status;
	int ResponseStatus;
	PineServiceHandle(){ reset();};
	void reset(){ RequestCode = 0; Handle = -1; Status = PineServiceResult::Free, ResponseStatus = -1; };
	bool isFree(){ return (Status == PineServiceResult::Free) && (Handle == -1); };
	bool isFinish(){ return (Status == PineServiceResult::Responsed); }
};

struct ResourceItem
{
	char Path[255];
	char FileName[100];
	int FileDescription;
	int	Offset;
	int Length;
	ResourceItem():Offset(0),Length(0),FileDescription(0){};
	ResourceItem(const char* path, int offset = 0, int length = 0, int fd = 0):Offset(offset), Length(length),FileDescription(fd){
		sprintf(Path, "%s", path);
	};
};

class Buffer
{
protected:
	BYTE* _data;
	int _data_len;
public:
	BYTE* Data(){return _data;};
	int Length(){return _data_len;};
	
	Buffer(const BYTE* data, int len) :_data(NULL),_data_len(0) {Init(data, len);};
	Buffer(int len) :_data(NULL),_data_len(0) {Init(len);};
	Buffer(const Buffer& buffer) :_data(NULL), _data_len(0){
		Release();
		Init(buffer._data, buffer._data_len);
	};
	Buffer() :_data(NULL),_data_len(0){};
	
	virtual void Release(){
		if(_data != NULL) {delete[] _data; _data = NULL;}
		_data_len = 0;
	};
	virtual void Init(int len){
		Release();
		_data = new BYTE[len + 1]; 
		_data[len] = '\0';
		_data_len = len;
	};
	virtual void Init(const BYTE* data, int len){
		Init(len);
		memcpy(_data, data, len);
	};
	Buffer& operator=(const Buffer& buff){
		if (this != &buff){
			Init(buff._data, buff._data_len);
		}
		return *this;
	};
	~Buffer(){ Release(); };
};



enum PineDisplayColorDepth
{
	ColorDepthUnknown = 0,
	ColorDepth8Bit = 8,
	ColorDepth16Bit = 16,
	ColorDepth24Bit = 24,
	ColorDepth32Bit = 32
};

struct PineDisplayProperties
{
	PineDisplayColorDepth ColorDepth;
};

struct PineRectangle
{
	float X;
	float Y;
	float W;
	float H;
	PineRectangle() :X(0), Y(0), W(0), H(0){};
	PineRectangle(float x, float y, float w, float h) :X(x), Y(y), W(w), H(h){};
	PineRectangle(PinePoint point, PineSize size) :X(point.X), Y(point.Y), W(size.W), H(size.H){};
	bool IsRectOutSide(float x, float y, float w, float h){ return (x > X + W || x + w < X || y > Y + H || y + h < Y); };
	bool IsCircleOutSize(float x, float y, float rad){ float dx = fabs(X - x), dy = fabs(Y - y); return sqrt(dx*dx + dy*dy) < rad; };
	bool IsPointIn(float x, float y){ return !(x< X || x> X + W || y< Y || y > Y + H); };
	bool IsPointIn(PinePoint p) { return !(p.X< X || p.X > X + W || p.Y < Y || p.Y > Y + H); };
	bool IsRectOutSide(PineRectangle rect) { return IsRectOutSide(rect.X, rect.Y, rect.W, rect.H); }
};

struct ScaleParam
{
	float ScaleX;
	float ScaleY;
	float X;
	float Y;
	ScaleParam(float sx, float sy, float x, float y) :ScaleX(sx), ScaleY(sy), X(x), Y(y){};
	ScaleParam() :ScaleX(0.0f), ScaleY(0.0f), X(0.0f), Y(0.0f){};
};



ENUM SupportVersion
{
	RESVER_667_375 = 0,
	RESVER_800_400 = 1,
	RESVER_800_480 = 2,
	RESVER_800_600 = 3,
	RESVER_854_480 = 4,
	RESVER_960_540 = 5,
	RESVER_960_640 = 6,
	RESVER_1024_600 = 7,
	RESVER_1024_768 = 8,
	RESVER_1024_800 = 9,
	RESVER_1136_640 = 10,
	RESVER_1280_768 = 11,
	RESVER_1280_720 = 12,
	RESVER_1280_800 = 13,
	RESVER_1366_768 = 14,
	RESVER_1920_1200 = 15,
	RESVER_2560_1600 = 16,
	RESVER_2048_1536 = 17,
	RESVER_1334_750 = 18,
	RESVER_2208_1242 = 19,
	RESVER_1000_562 = 20,

};

ENUM HorizontalAlign
{
	Left = 0,
	Center = 1,
	Right = 2
};

ENUM VerticalAlign
{
	Top = 0,
	Middle = 1,
	Bottom = 2
};

struct Alignment
{
	HorizontalAlign Horizontal;
	VerticalAlign Vertical;
};

struct SCREEN
{
	PineSize ScreenSize;
	Orientation ScreenOrientation;
};

struct ResVersion
{
	SupportVersion Version;
	Orientation VersionOrientation;
};

struct PineClip
{
	PineRectangle ClipArea;
	PineRectangle LastArea;
	PineClip(){};
	PineClip(float x, float y, float w, float h):ClipArea(x,y,w,h), LastArea(){}
	PineClip GetSubClip(float x, float y, float w, float h){
		float px = ClipArea.X;
		float py = ClipArea.Y;
		float pw = ClipArea.W;
		float ph = ClipArea.H;
		float cx = x;
		float cy = y;
		float cw = w;
		float ch = h;
		float nx = cx;
		float ny = cy;
		float nw = cw;
		float nh = ch;
		if (cx > px + pw || cy > py + ph || px > cx + cw || py > cy + ch){

		}
		else{
			nx = px > cx ? px : cx;
			ny = py > cy ? py : cy;
			nw = px + pw > cx + cw ? cx + cw - nx : px + pw - nx;
			nh = py + ph > cy + ch ? cy + ch - ny : py + ph - ny;
		}

		PineClip nclip(nx, ny, nw, nh);
		nclip.LastArea = ClipArea;
		return nclip;
	}
	void Restore(){
		ClipArea = LastArea;
	}
};

ENUM PineGraphicComplexOrientation
{
	ComplexNone = 0,
	ComplexToLeft = 1,
	ComplexToRight = 2,
	ComplexFlip = 3,
};

//NETWORK
#if defined(MAC_OS) || defined(ANDROID_OS)
	typedef int SOCKET;
#	define INVALID_SOCKET  -1
#endif

ENUM HttpMethod
{
	Get = 1,
	Post = 2,
	Del = 3
};

///HttpConnect http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
ENUM HttpStatus
{
	None = 0,
	ShouldContinue = 100,
	SWitchingProtocol = 101,
	Ok = 200,
	Create = 201,
	Accept = 202,
	NonAuthoritativeInfomation = 203,
	NoContent = 204,
	ResetContent = 205,
	PartialContent = 206,
	Multichoice = 300,
	MovedPermanently = 301,
	Found = 302,
	SeeOther = 303,
	NotModified = 304,
	UseProxy = 305,
	TemporaryRedirect = 307,
	BadRequest = 400,
	UnAuthorized = 401,
	PaymentRequired = 402,
	Forbitdden = 403,
	NotFound = 404,
	MethodNotAllowed = 405,
	NotAcceptable = 406,
	ProxyAuthenticationRequired = 407,
	RequestTimeout = 408,
	Conflict = 409,
	Gone = 410,
	LengthRequired = 411,
	PreconditionFailed = 412,
	EntityTooLarge = 413,
	UriTooLong = 414,
	UnsupportedMediaType = 415,
	RequestedRangeNotSatisfiable = 416,
	ExpectationFailed = 417,
	InternalServerError = 500,
	NotImplemented = 501,
	BadGateway = 502,
	ServiceUnavailable = 503,
	GatewayTimeout = 504,
	VersionNotSupported = 505
};

/////////////////////////////////////////////////////////
////ACCOUNT MANAGEMENT
/////////////////////////////////////////////////////////
ENUM PineAccountType
{
	Unknown = 0,
	GameCenter = 1,
	Facebook = 2,
	Twitter = 3,
	Bot = 4,
	GooglePlay = 5,
};

class PineAccountInfo
{
public:
	union{
		UINT64 Id;
		char StrId[255];
	};
	char Name[255];
	char AvatarUrl[255];
};

class PineAccountIdList
{
protected:
	UINT64* _accounts;
	int _account_num;
public:
	PineAccountIdList() :_accounts(NULL), _account_num(0){};
	PineAccountIdList(int acc_num) :_accounts(NULL), _account_num(acc_num){
		init(acc_num);
	};
	void init(int account_num){
		if (_accounts != NULL){
			delete[] _accounts; _accounts = NULL;
		}
		if (account_num > 0){
			_accounts = new UINT64[account_num];
		}
		_account_num = account_num;
	};
	int Length(){
		return _account_num;
	};
	UINT64 getAccountAt(int offset){
		if (offset >= 0 && offset < _account_num){
			return _accounts[offset];
		}
	};
	void setAccountAt(int offset, UINT64 account_id){
		if (offset >= 0 && offset < _account_num){
			_accounts[offset] = account_id;
		}
	};
	~PineAccountIdList(){
		if (_accounts != NULL){
			delete[] _accounts;
		}
		_accounts = NULL; _account_num = 0;
	};
};

class PineAccountInfoList
{
protected:
	PineAccountInfo* _accounts;
	int _account_num;

public:
	PineAccountInfoList() :_accounts(NULL), _account_num(0){};
	PineAccountInfoList(int acc_num) :_accounts(NULL), _account_num(0){
		init(acc_num);
	};
	void init(int account_num){
		if (_accounts != NULL){
			delete[] _accounts; _accounts = NULL;
		}
		if (account_num > 0){
			_accounts = new PineAccountInfo[account_num];
		}
		_account_num = account_num;
	};
	int Length(){
		return _account_num;
	};
	PineAccountInfo getAccountAt(int offset){
		if (offset >= 0 && offset < _account_num){
			return _accounts[offset];
		}
	};
	void setAccountAt(int offset, PineAccountInfo account_id){
		if (offset >= 0 && offset < _account_num){
			_accounts[offset] = account_id;
		}
	};
	~PineAccountInfoList(){
		if (_accounts != NULL){
			delete[] _accounts;
		}
		_accounts = NULL; _account_num = 0;
	};
};

struct BoardScoreItem
{
	PineAccountType AccountType;
	char BoardId[255];
	int Score;
	BoardScoreItem(){};
	BoardScoreItem(PineAccountType type, const char* board_id, int score){
		AccountType = type;
		sprintf(BoardId, "%s", board_id);
		Score = score;
	};
};

class PineLocalPushItem
{
	int _id;
	char _message[100];
	char _ticker[100];
	int _second;
public:
	PineLocalPushItem()
	{
		_id = -1;
		for (int i = 0; i < 100; i++)
		{
			_message[i] = '\0';
		}
		for (int i = 0; i < 100; i++)
		{
			_ticker[i] = '\0';
		}
		_second = -1;
	};

	PineLocalPushItem(int id, const char * message, const char * ticker, int second)
	{
		_id = id;
		sprintf(_message, "%s\\0", message);
		sprintf(_ticker, "%s\\0", ticker);
		_second = second;
	};
	static int GetDataLength() { return sizeof(int) * 2 + sizeof(char) * 200; };
	int GetId() { return _id; };
	const char * GetMessage() { return _message; };
	const char * GetTicker() { return _ticker; };
	int GetSecond() { return _second; };

	void SetId(int id) { _id = id; };
	void SetMessage(const char * message) { sprintf(_message, "%s", message); };
	void SetTicker(const char * ticker) { sprintf(_ticker, "%s", ticker); };
	void SetSecond(int second) { _second = second; };

	void LoadFromBuffer(char * buffer, int offset)
	{
		memcpy(&_id, buffer + offset, sizeof(int));
		memcpy(&_message, buffer + offset + sizeof(int), sizeof(char) * 100);
		memcpy(&_ticker, buffer + offset + sizeof(int) + sizeof(char) * 100, sizeof(char) * 100);
		memcpy(&_second, buffer + offset + sizeof(int) + sizeof(char) * 200, sizeof(int));
	};

	void SaveToBuffer(char * buffer, int offset)
	{
		memcpy(buffer + offset, &_id, sizeof(int));
		memcpy(buffer + offset + sizeof(int), &_message, sizeof(char) * 100);
		memcpy(buffer + offset + sizeof(int) + sizeof(char) * 100, &_ticker, sizeof(char) * 100);
		memcpy(buffer + offset + sizeof(int) + sizeof(char) * 200, &_second, sizeof(int));
	};
};

ENUM PineEventClass
{
	System = 0xFF,
	GameLower = 0xFE,
	Game = 0x00
};

ENUM enumIAPResponseStatus
{
	IAP_RESPONSE_STATUS_FAIL,
	IAP_RESPONSE_STATUS_SUCCESS,
    IAP_RESPONSE_STATUS_SUCCESS_RESTORE,
    IAP_RESPONSE_STATUS_FAIL_RESTORE,
    IAP_RESPONSE_STATUS_END_RESTORE,
};

class PineEvent
{
public:
	EVENTID ID;
	BYTE State;
	Buffer* Data;
	PineEvent(){
		Data = NULL;
	};
	void init(const char* data, int len){
		release();
		if (len > 0){
			Data = new Buffer((const BYTE*)data, len);
		}
	}
	void release(){
		if (Data != NULL){
			delete Data;
			Data = NULL;
		}
	};
	~PineEvent(){
		release();
	};
};

class PineKeyEvent
{
public:
	char Key;
	PineKeyEvent(const PineEvent* evt){
		if (evt->Data != NULL){
			Key = *(char*)evt->Data->Data();
		}
	}
};

class PineString
{
protected:
	UNICHAR* _data;
	int _len;
public:
	UNICHAR* Data(){return _data;};
	int Length(){return _len;};
	PineString(const UNICHAR* data, int len) :_data(NULL),_len(0) {Init(data, len);};
	PineString(int len) :_data(NULL),_len(0) {Init(len);};
	PineString(const PineString& buffer) :_data(NULL), _len(0){
		Release();
		Init(buffer._data, buffer._len);
	};
	PineString() :_data(NULL),_len(0){};
	PineString(const char* str):_data(NULL),_len(0){
		if(str == NULL)return;
		int len = strlen(str);
		Init(len);
		for(int i = 0; i< len; i++){
			_data[i] = str[i] & 0xFFFF;
		}
		_data[len] = '\0';
	}
	virtual void Release(){
		if(_data != NULL) {delete[] _data; _data = NULL;}
		_len = 0;
	};
	virtual void Init(int len){
		Release();
		_data = new UNICHAR[len + 1]; _data[len] = '\0';
		_len = len;
	};
	virtual void Init(const UNICHAR* data, int len){
		Init(len);
		for(int i = 0; i< len; i++){
			_data[i] = data[i] & 0xFFFF;
		}
	};
	PineString& operator=(const PineString& buff){
		if (this != &buff){
			Init(buff._data, buff._len);
		}
		return *this;
	};
	~PineString(){ Release(); };
};
#endif

#if defined(ANDROID_OS)
#define ASSERT(cond) if (!(cond)){ __android_log_assert("conditional", "DEBUG",__func__ ,__FILE__, __LINE__);}
#else
#define ASSERT		assert
#endif

#define TRACE() PDEBUG("%s:%d",__FILE__,__LINE__)

#endif//_H_CTYPE