/**
*	will be included in CGame.h
*	specific vars and methods for gameplay
*/

public:
	// game state
	void UpdateLogo();
	void RenderLogo();


private:

public:





	//Save System.
	void GameDataLoad();
	void GameDataSave();
	void GameDataDefault();
	void GameDataToBuffer();
	void GameLoadDataFromBuffer();

	BYTE red, green, blue, alpha;
#define GAME_FILE	"pinerecord.dat"

#define GAME_VERSION				100

#define GAME_DATA_VERSION			(0)

#define GAME_DATA_LENGTH				(GAME_DATA_VERSION + 4)



	char _buffer_save[GAME_DATA_LENGTH];
	int _game_version;
	int _rate;

	//PineHighScore _pineHighScore;

	long _sys_begin_second;
	long _sys_millisecond;
	long _game_millisecond;
	long _sys_second;

	void UnloadGameBackground();
	bool LoadGameForceground();

	bool _needresume_when_playing;


	void PurchaseIAP(const char* strID, enumIAPResponseStatus sucessed);

	int  _frame_auto_stop;
	PineImage _image;
	PineGame *_main_game;
	PineGame  *_feature_game;
	PineGame    *_current_game;
	bool _isNeedOptimize;



	PineSocial *_gcSocial;
	bool InitGCComplete();
	void ProcessingTag(int tag, int indexButton);
	int _icloudStatus;
	INT_TIME _timeOutGame;

	//--------------------PORT GAME--------------------//
	PinePoint _pointLeftTop;
	void MeasureButton(Button* button);
	void MeasureButton(Button* button, int anchor);
	void MeasureButtonX(float posx, Button* button, BYTE anchor_horizonal);
	void MeasureButtonY(float posy, Button* button, BYTE anchor_vertical);
	PinePoint MeasureFrame(CSprite* sprite, UINT32 frameid);
	void MeasureAnim(CAnimObject* obj);
	PinePoint MeasureFrame(CSprite* sprite, UINT32 frameid, float scalex, float scaley);
	PinePoint MeasureFrame(CSprite* sprite, UINT32 frameid, BYTE anchor);
	float _screenWidth;
	float _screenHeight;
	//////////////////////PORT GAME/////////
#if ANDROID_OS
	CAsyncTasks _async_task_manager;
	PineAdmobAsync _admob_manager;
	BoardScoreItem		_gameBoardScoreItem;
#endif
#if !defined (MAC_OS)
	bool			_isBackkeyPress;
	int				_waitForProcessBackkeyCount;
#endif
	bool _has_show_ads_banner,
		_on_show_video_ads;
	int _time_count_for_show_ads_banner, _admob_play_count;

	float reduceY;
	int _times_play_game;
	float _banner_ads_scrolly;
	bool banner_ads_implement;
	bool InitAds();
	void ShowAds();
	void ShowBannerAds();
	void HideBannerAds();
	void UpdateAds();

	void ResetStateBannerAds();
	bool isClosedInterstitialAd;