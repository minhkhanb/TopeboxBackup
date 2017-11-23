#ifndef _CGAMECORE_H
#define _CGAMECORE_H

#include <stdio.h>
#include <time.h>
#ifdef MAC_OS
#include <unistd.h>
#endif

#include <PineEngine.h>

#include "../Lib/DataType/CType.h"
#include "../Define/Defines.h"
#include "../Lib/Sprite/CSprite.h"
#include "../Lib/DataInputStream/DataInputStream.h"
#include "../Lib/PineADVERT.h"
#include "../Lib/PineImage.h"
#include "../Lib/CKeyboard.h"
#include "../Lib/ArrayList/ArrayList.h"

#include "../Lib/PineLeaderBoard.h"
#if USE_PINESOCIAL
#include "../Lib/PineSocial.h"
#endif
#if ADD_TWITTER
#include "../Lib/PineTwitterController.h"
#endif
#if ADD_GAMECENTER
#include "../Lib/PineGameCenterController.h"
#endif
#if ADD_FACEBOOK
#include "../Lib/PineFacebookController.h"
#endif
//include sound engine
#include "../Lib/Sound/SoundEngine.h"
#include "../Lib/Debug/Test.h"

//Include Graphic Engine
#include "../Lib/OpenGLES/Graphic.h"
//if use new graphic engine we include appropriate machine (OpenGl or DirectX)
#	if ISGVER_GLES1
#		include "../Lib/OpenGLES/GLESLib1.h"
#	elif ISGVER_DIRECTX11
#		include "../Lib/OpenGLES/DirectX11.h"
#	endif 
typedef Graphic         GLESLib;

#include "CAnimObject.h"
//#include "ResourceInputStream.h"
#include "CTouch.h"

#include "../GamePlay/CGame_include_ex.h"
#include "GameContext.h"
#include "Device.h"

extern "C" const char* GetiPhoneUDID();
extern "C" const char* GetRealFilePath(const char* filename);

//text input
extern "C" void StartTextInput(char* text_buff, unsigned int buff_size, int x, int y, int w, int h);
extern "C" void CloseTextInput();
extern "C" bool IsTextInputDone();

#define DEVICE() (CGame::_this->_device)

/////////////////////////////////////
///EVENT SYSTEM
/////////////////////////////////////
#if !defined(EVENT_STACK_MAX_SIZE)
#define EVENT_STACK_MAX_SIZE 50
#endif

class CGame
{
private:
	/////////////////////////////
	//	MUSIC + SOUND MANAGER
	/////////////////////////////
public:
    enum enumStatusPostSocial
    {
        POST_SOCIAL_STATE_NONE,
        POST_SOCIAL_STATE_PROCESSING,
        POST_SOCIAL_STATE_COMPLETE,
        POST_SOCIAL_STATE_CANCEL,
    };
    SoundEngine*	_sound_engine;
	bool			_soundEnable;
	bool			_musicEnable;
	bool			_sound_loaded[NUM_SOUND];
	HSOUND			_sound_handle[NUM_SOUND];
    int             _sound_map_id[NUM_SOUND];
	
	GraphicContext	_graphic_context;
	ResVersion _curr_resversion;
	

	/////////////////////////////
	//	OPTIMIZE
	/////////////////////////////
	bool _is_texture_scale;

    static bool _is_portrail_exception;
#if USE_PINESOCIAL
	PineSocial* _pine_social;
#endif
	///////////////////////////////////////////////
	//	SOUND MANAGER
	///////////////////////////////////////////////
	inline bool IsEnableSound(){return _soundEnable;};
	inline bool IsEnableMusic(){return _musicEnable;};
	inline void SetSoundEngine(SoundEngine* sound_engine){
		_sound_engine = sound_engine;
	}
	inline SoundEngine* getSoundEngine(){
		return _sound_engine;
	}
	inline void SetMusicEnable(bool state){ _musicEnable = state; };
	inline void SetSoundEnable(bool state){ _soundEnable = state; };

	/////////////////////////////
	//	OPTIMIZE
	/////////////////////////////
	void SetTextureScale(bool is_scale);

	///////////////////////////////////////////////
	//	RES SOURCE VERSION
	///////////////////////////////////////////////
	static ResVersion GetAppropriateVersion(PineSize withsize, ResVersion* support_screen, int array_len, bool support_rotate = true);
	//return best version match with input size in array of supported versions.
	
	static Orientation GetOrientation(PineSize* size);
	//return orientation of input size

	static void CorrectSize(PineSize* size, Orientation ori);
	//correct width, heigh attribute appropriate with orientation.

	static PineSize GetResVersionSize(SupportVersion version);
	//return size of screen of version

	void OnScreenChange(PineSize size);
	void OnChangeOrientation(Orientation ori);
    
    Orientation GetGameOrientation();

	///////////////////////////////////////////////
	//	GAME CONTEXT
	///////////////////////////////////////////////
	void AlignmentContext(Alignment align, PineSize withsize, ResVersion res_version, bool allow_scale,int scanline);
	//alignment game context
	
	/////////////////////////////////////
	///EVENT SYSTEM
	/////////////////////////////////////
	bool _is_system_message_come;
	PineEvent _events[EVENT_STACK_MAX_SIZE];
	void addEvent(EVENTID id, const char* data, UINT32 datalen);
	const PineEvent* NextEvent(EVENTID id, EVENT_POINTER* pointer);
	const PineEvent* NextEventOnClass(PineEventClass class_fillter, EVENT_POINTER* pointer);
	void KeepEvent(const PineEvent* pevent);
	void CleanEvent(const PineEvent* pevent);
	void CleanEvents(EVENTID id);

	static float  _windows_screen_scale;
	void ClearAnotherTouch(int i);
    void ClearTouchID(int ids);
	bool			_isExit;
	bool			_isPause;
	bool			_isResumeMusicAfterPause;
	bool			_is_init;
	void ServerStart();
	void ServerRun();
	static CGame*	_this;
	//graphics
	static int FPS_LIMIT;
    static UINT64 FPS_LIMIT_TIME;
	Graphic*		_g;
	static Device			_device;
    std::shared_ptr<Pine::Platform::CDeviceContext> _device_context;
	//input & output
	//#define 				IO_BUFFER_LENGTH	1024
	UINT32					_output_offset;
	BYTE*					_output_buffer;
	//DataInputStream*		_data_input_stream;
	//game's sprite
	static CSprite**		_sprites;
	int*			_reg_sprites_unload;
	int				_num_reg_sprite_unload;
	int 			_num_sprites;
#if UNIVERSAL_BUILD
	int 			_gfx_version;
#endif
	//text
	UNICHAR*** 		_texts;
	UINT16** 		_text_lens;
	
	//sound
	int				_iCurrentSnd;
	//bool			_isActiveSound;
	
	bool			_notificationEnable;
	//game state
	int				_GameState;
	int				_GameSubState;
	int				_GameStateNew;
	int				_GameStateOld;
	
	//touch control
	CTouch**		_touches;
	int				_num_touches;
	static UINT32	_touch_id_gen;
	int				_active_touch;
	
	//phone orientation
	int				_device_orientation;
	char			_device_id[100];
	int				_device_model;
	
	//control fps
	UINT64			_last_frame_time;
	UINT64			_frame_dt;
	UINT64			_run_time;
	UINT64			_frame_counter;
	UINT64			_sync_sleep_time;
	
	//http online
	//CHttpOnline*	_http_online;
	
	//facebook user info
	UINT64 			_fb_userid;
	char 			_fb_username[100];
	StringList*		_fb_friendids;
	
	//twitter status code
	int _twitterStatusCode;
public:
	int _statusBasicPostSocial;
	CGame();
	~CGame();
	void InitDevice(Device dv);

	void Init();
	void Release();
	
	void InitEx();
	void ReleaseEx();
	
	//return true if use this event; false if not use
	bool OnBackKeyPress();
	
	//game update and render function
	void UpdateAndRender();
	void GameUpdate();
	void GameRerder();
	void UpdateTouch(int xx, int yy, int touch_state, int touch_id, int tap_count);
	void PauseEx();    
	void ResumeEx();
	void FlushGraphics();
	void Run();
	void Pause();
	void Resume();
	void Exit();

#if defined(_WINDOWS)
	void ProcessCommandLine(const char* cmd);
	
#endif
	
	
	void UpdateKeyInput(int c);
	static unsigned long GetTime();

	static void Sleep(UINT64 iTime);
	
	static struct tm* GetDateTime()
	{
		time_t rawtime;
		time(&rawtime);
		
		return localtime(&rawtime);
	}
	GameContext		_game_context;
	void SetGlobalContext(float tx, float ty,float scalex, float scaley);
	GraphicContext GetGraphicContext(){ return _graphic_context; }
	//game state function
	void SetGameState(int iNewState, bool isCleaning);
	void SetGameStateWithEffect(int iNewState);
	void SetSubState(int iNewSubState);
		
	//game resource function
    BYTE* GetResource(const char* sFile);
	BYTE* GetResource(int iResID);
	INT32 GetResourceSize(int iResID);
	//ResourceInputStream* GetResourceInputStream(int iResID);
	DataInputStream* GetDataInputStream(int iResID,const char* file_name=NULL);
	DataInputStream* GetDataInputStream(BYTE* buff,int length);
	const char* GetResourceFileName(UINT32 iResID);
	
	//load, unload and get sprite
	void LoadSpriteFromRGBA(int sprite_id,BYTE* rgba,int width,int height, bool isCacheTexture, bool linear_texture, int pal = 1, bool load_trueColor_texture = true,bool fullcache=true);
	void LoadSprite(int sprite_id, bool isCacheTexture, int linear_texture, int pal = 1, bool load_trueColor_texture = true,bool fullcache=true,const char* name_file=NULL);
    void LoadSpriteInThread(int sprite_id, bool isCacheTexture, bool linear_texture, int pal = 1, bool load_trueColor_texture = true,bool fullcache=true,const char* name_file=NULL);
	void LoadSprite(int sprite_id, bool isCacheTexture = true, int pal = 1,const char* name_file=NULL);
	void LoadSprite(int sprite_id, BYTE* buffer,int length, bool load_trueColor_texture=true,bool linear_texture=true, int pal=1);
	void LoadSpritePack(int iPackID);
	void UnloadSprite(int sprite_id);
	void RegUnloadSprite(int sprite_id);
	
	static CSprite* GetSprite(int sprite_id);
	
	//load and get text
	void LoadTextPack(int iTextPackID);	
	UNICHAR* GetText(int text_id, int pack_id);
	UINT16 GetTextLength(int text_id, int pack_id);
	void DrawText(CSprite* font, int text_id, int pack_id, float x, float y, int anchor);
	void DrawText(CSprite* font, const char* text, float x, float y, int anchor);
	void DrawTextPage(CSprite* font, int text_id, int pack_id, float x, float y, int w, int anchor);
	//free io buff
	void FreeOutputBuffer();
	
	//logic touch
	void NotifyTouchesBegan(int touch_id, float x, float y, int tap_count);
	void NotifyTouchesMoved(int touch_id, float x, float y, int tap_count);
	void NotifyTouchesEnd(int touch_id, float x, float y, int tap_count);
	void NotifyTouchesCancel(int touch_id);
	
	void ClearTouchesEnd();
	void ClearAllTouch();
	
	//write file
	void ClearIOBuffer();
	bool ClearFileContent(const char* filename);
	bool RemoveFile(const char* filename);
	bool WriteInt8(const char* filename, int data);
	bool WriteInt16(const char* filename, int data);
	bool WriteInt32(const char* filename, int data);
	bool FlushIOBuffer(const char* filename);
	
	//music
	bool LoadMusic(int music_id);
	bool LoadLibMusic();
	void PlayLibMusic(int num_loops);
	void StopLibMusic();
	void PauseLibMusic();
	int GetCurrentLibMusicTime();
	int GetLibSongTime();
	void UpdateSongList();
	int	 GetLibSongNum();
	char* GetLibSongTitle(int index);
	char* GetLibCurrentSongTitle();
	
	void PlayCurrentMusic(int num_loops);
	void StopCurrentMusic(float delta_volume = 1);
	void PauseCurrentMusic();
	bool IsCurrentMusicPlaying();
	void SetVolume(float vol);
	float _current_music_vol;
	BYTE _music_effect_volume_state;
	float _delta_volume;
	void PreviousMusic(int numSecond);
	void ForwardMusic(int numSecond);
	double GetCurrentMusicTime();
	double GetSongTime();
	
	//sound effect
    bool HasLoadSFX(int soundid);
	bool LoadSFX(int soundId);
	void PlaySFX(int soundId);
	void StopSFX(int sound_id);
	void StopAllSFX();

	//Lam Add
	void ClearAllSFX();

	void FreeSFX(int soundId);
	void FreeAllSFX();	

	//Reload anim
	#define ANIM_STOCK_MAX (10000)
	CAnimObject** _anim_stock;
	int *_current_anim_stock;
	int *_index_anim_stock;
	int *_anim_loop_stock;
	float *_anim_x_stock;
	float *_anim_y_stock;
	int *_anim_is_stop_stock;
	int _anim_stock_num;
	int _anim_stock_capacity;
	int _step_force_loading;
	
	bool _compete_backup_stock;
	int _sprite_loaded[TOTAL_SPRITE_INGAME];
	int _sprite_optimized_loaded[TOTAL_SPRITE_INGAME];
	int _sprite_antialias_loaded[TOTAL_SPRITE_INGAME];
	bool _sprite_from_rbga[TOTAL_SPRITE_INGAME];
	void InitAnimStock();
	void AddAnimStock(CAnimObject *anim,int index);
	void BackupAnimStock();
	void RestoreAnimStock();
	void ResetAnimStock();
	INT_TIME _game_millisecond_begin;
	INT_TIME _time_supen_begin;

	static UINT8*	_unzip_buff;
	static UINT32	_unzip_buff_length;
	static void FreeBuff()
	{
		if(_unzip_buff != NULL)
		{
			delete []_unzip_buff;
		}
		_unzip_buff = NULL;
	}
	static void InitBuff(UINT32 length)
	{
		FreeBuff();
		_unzip_buff = new UINT8[length];
		_unzip_buff_length = length;
		CSprite::BindUnzipBuffer(_unzip_buff,length);
		PineImage::BindUnzipBuffer(_unzip_buff, length);
	}

	public:
	
//[?] duy.nguyen add new resource manager tool
#if !defined(TOTAL_SPRITEID)
#define TOTAL_SPRITEID TOTAL_SPRITE_INGAME
#endif

	static const  int _syn_sprite_index[VERSION_NUMS*TOTAL_SPRITEID];
	static int _current_version;

	void LoadSpriteVer(int spriteID,int version,int antialias=true,bool truecolor = true,int pal = 0);
	CSprite* GetSpriteVer(int spriteID, int version);
	void LoadSpriteCurrentVer(int spriteID,BYTE* buffer,int length,bool antialias=true,bool truecolor=true,int pal=0);
	void LoadSpriteCurrentVer(int spriteID,int antialias=true,bool truecolor =  true,int pal = 0,const char *file_name=NULL);
    void LoadSpriteCurrentVerFromThread(int spriteID,int antialias=true,bool truecolor =  true,int pal = 0,const char *file_name=NULL);
	void LoadSpriteCurrentVerFromRGBA(int spriteID,BYTE* argb,int width,int height,int antialias=true,bool truecolor =  true,int pal = 0);
	void LoadSpriteCurrentVerFromFile(int spriteID,const char* file_name,bool true_color=true);
	void ReloadSpriteTextureCurrentVer(int spriteID);
	CSprite* GetSpriteVer(int spriteID);
	void UnloadSpriteVer(int spriteID,int version);
	void UnloadSpriteVer(int spriteID);
	void UnloadSpriteTextureVer(int spriteID);
	void RegUnloadSpriteVer(int spriteID, int version);
	void RegUnloadSpriteVer(int spriteId);
	int GetSpriteVerID(int spriteID);
	float GetScreenWidth();
	float GetScreenHeight();


	#define MAX_TABLE	3600
	static float _sin_table[MAX_TABLE];
	static float _cos_table[MAX_TABLE];
	static void InitSinTable();
	static void InitCosTable();
	static void InitMathTable();
	static float Math_Cos(float angle);
	static float Math_Sin(float angle);
	static float Math_AngleAbs(float angle);

	//include CGame_GamePlay.h which contain specific vars and methods for each game title
	#include "../GamePlay/CGame_prototype_ex.h"

	static CGame* GetInstace();
#if ADD_ADVERT
	PineADVERT _advert;
#endif
	int GetFixedTexture(int size);
	bool _game_incheck_flush_all;
	////PineHighScore _pineHighScore;
#if TEXTURE_MANAGER_MAX
	static int _texture_manager[TEXTURE_MANAGER_MAX];
	static bool  _texture_ticket_used[TEXTURE_MANAGER_MAX];
	void RegiterTexture();
	int _current_ticket_texture;
	int GetTextureTicket();
	void DeleteTicket(int ticket_id);
	void DeleteTicketAt(int index);
#endif
#if ADD_DEVICE_KEYBOARD
	CKeyboard _keyboard;
#endif
	bool isNetworkReal;
	//hao.ly add
	void setUseOptimizeDrawModule(BYTE type);
	BYTE _typeOtimizeDrawModule;
	INT_TIME _syn_time_start;
	INT_TIME GetTimePasses();
	void SetTimeSynStart();
	////INT_TIME _game_center_id[GAME_CENTER_FRIEND_NUM];
	////char _game_center_name[GAME_CENTER_FRIEND_NUM][51];
	////int _gc_friend_num;
	////bool _gc_loadfriend_complete;

	//free sprite
	int _sprite_id_will_free[TOTAL_SPRITE_INGAME];
	int _current_sprite_will_free;
	void InitWillFree();
	void SpriteWillFree(int id);
	void UpdateSpriteFree();
	
	
	void SetByteAt(char* buffer,int offset, int value);
	char GetByteAt(char* buffer,int offset);
	void SetCharAt(char* buffer,int offset, int value);
	char GetCharAt(char* buffer,int offset);
	void SetInt16At(char* buffer,int offset, int value);
	int	 GetInt16At(char* buffer,int offset);
	void SetInt32At(char* buffer,int offset, int value);
	int GetInt32At(char* buffer,int offset);
	void SetInt64At(char* buffer,int offset, INT_TIME value);
	INT_TIME GetInt64At(char* buffer,int offset);
    
    
    static int _key_1_state;
    static int _key_2_state;
    static int _key_3_state;
    static void ResetVirtualKeys();
    static void Key1Press();
    static void Key2Press();
    static void Key3Press();
    static bool HasKey1Press();
    static bool HasKey2Press();
    static bool HasKey3Press();
    static void ResetKey1();
    static void ResetKey2();
    static void ResetKey3();

};

#endif
