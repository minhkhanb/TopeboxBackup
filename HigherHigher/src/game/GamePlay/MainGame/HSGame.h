#include "../../GameCore/PineGame.h"

#ifndef __PineEntertainment__HS__
#define __PineEntertainment__HS__

#include <stdio.h>

class HSGame : public PineGame
{
	//camera manager
	float  _root_bg_x;
	float  _root_bg_y;
	float _root_bg_high;
	float  _bg_offset_x;
	float  _bg_offset_y;
	int _current_frame;
	int  _bg_state_jump;
#define BG_JUMP_FRAME 	(30)
#define BG_STATE_NONE 	(0)
#define BG_STATE_JUMP 	(1)
#define BG_HIGHT_JUMP (20)

#define BG_SPRITEID_NUM			3
#define CHAR_SPRITEID_NUM			10

	void ResetBGCamera();
	void SetBGJump();
	void UpdateBGJump();
	float GetBGTranslateX();
	float GetBGTranslateY();


	CGame*_game;
	Graphic*_grap;
public:
	HSGame();

	int _count_frame_show_ads;
	int _time_delay_show_ads;
	bool _show_ads_interrupt;

	bool _isWaiting, _notShowPineAdds, _not_showads_interrupt;
	bool _menu_ggplay_isclose;
	int _menu_ggplay_scroll_y;
	TopeBox_PromoGame _topebox_game;
	float _topebox_game_scrollEff;

	//--------------------IMPLEMENT GAMEPLAY
	void UpdateAndRender();
	void Init();
	//--------------------Logo--------------------//
	bool _optimize_data;
	bool _auto_loading;
	void UpdateLogo();
	void RenderLogo(float x, float y, int opac);
	int _loading_step;
	float _logo_scrolly;
	float _logo_scroll_state;
	//--------------------GamePlay--------------------//
	void GamePlayRender(float x, float y, int opac);
	void GamePlayUpdate();
	bool GamePlayUpdateTouch();
	void AGamePlayInit();
	void GamePlayInit();

	int _game_over_effect_state;
	float _game_over_effect_scale;
	int _game_over_opac;
	bool _game_over;
	int _game_over_hold;
	int _game_score;
	int _game_score_target;
	int _high_score;
	CCharacter _main_char;
	int _touch_index;
	int _count_frame_hold_flatform_return;
	float _control_speed_move;
	int _effect_plus_score_state;
	float _effect_plus_score_jump_speed;
	float _effect_plus_score_y;
	float _shakex, _shakey;
#define GAMEPLAY_NUM_BUTTON		1

#define GAMEPLAY_BUTTON_SOUND		0
	Button _gameplay_button[GAMEPLAY_NUM_BUTTON];
	bool _sound_music_enabled;
	CUserData _user;
	bool _high_score_submited;
	void BGRender(float x, float y);
	bool _first_game;
#define CONTROL_NUM_BUTTON		4

	struct SControl
	{
		bool _touch_true;
		int _time_delay;
		int _time_delay_up;
		bool _is_reset;
		bool _is_set_begin;
		float _beginx, _beginy;
		float _tarx, _tary;
		float _jump_speed;
		int _fake_in_opac;
		int _hold;
		int _touch_id;
		int _color_type;
		float _x, _y;
		float _w, _h;
		int _status;
		bool _is_lock;
		void SetPos(float x, float y);
		void Init(float x, float y, int w, int h, int colortype);
		void Render(float x, float y);
		void Update();
		void SetMove(float x, float y, float speed, bool charin, int status, int delay);
	};
	SControl _control[CONTROL_NUM_BUTTON];
	void ControlRender(float x, float y, int opac);
	void ControlUpdate();
	void ControlInit();
	bool ControlUpdateTouch();
	void ControlActive(int control_index);
	bool _has_change_color;
	void ControlChangeColor();
	int _border_change_color;
	int _border_change_color_opac;
	CObjectEnvir _object_envir[OBJECT_ENVIR_NUM];
	void ObjectEnvirInit();
	void ObjectEnvirRender(float x, float y);
	void ObjectEnvirUpdate();
	int _bg_spriteid_current;
	CFood _food;
	void FoodSpawn();
	int _game_combo;
	int _game_combo_timereset;
	void ComboResetTime();
	struct SEffectCombo
	{
		float _x, _y;
		float _scale;
		float _scale_speed;
		float _opac;
		float _opac_speed;
		float _speed_scroll;
		float _flash_opac;
		int _flash_state;
		int _value;
		int _state;
		int _color_type;
		int _color_type_old;
		void Init();
		void Add(float x, float y, float scale, float scalesps, float opac, float opacsps, float speed, int value, int colortype);
		void Render(float x, float y);
		void Update();
		void ForceEnd();
	};
#define EFFECT_COMBO_NUM			20
	SEffectCombo _effect_combo[EFFECT_COMBO_NUM];
	void EffectComboInit();
	void EffectComboAdd(float x, float y, float scale, float scalesps, float opac, float opacsps, float speed, int value, int colortype);
	void EffectComboRender(float x, float y);
	void EffectComboUpdate();
	void EffectComboForceEnd();
#define EFFECT_FLIPPING_NUM_CHAR			50
	struct SEffectFlipping
	{
		float _x[EFFECT_FLIPPING_NUM_CHAR], _y[EFFECT_FLIPPING_NUM_CHAR];
		float _scalex[EFFECT_FLIPPING_NUM_CHAR], _scaley[EFFECT_FLIPPING_NUM_CHAR];
		float _scale_spsx[EFFECT_FLIPPING_NUM_CHAR], _scale_spsy[EFFECT_FLIPPING_NUM_CHAR];
		int _hold_time[EFFECT_FLIPPING_NUM_CHAR];
		int _frameid[EFFECT_FLIPPING_NUM_CHAR];
		int _state[EFFECT_FLIPPING_NUM_CHAR];
		int _spriteid;
		int _type;
		int _num;
		int _hold_time_state;
		void Init(int spriteid, int type, int num, int timehold);
		void Add(int index, float x, float y, float scalex, float scaley, float scalespsx, float scalespsy, int holdtime, int frame);
		void Render(float x, float y);
		void Update();
		void ForceEnd();
		bool IsFree();
	};
	SEffectFlipping _new_highscore;
	bool _game_new_score;
	void EffectFlippingActive(int id);
	struct SCharSelect
	{
		int _state_move;
		float _x, _y;
		int _spriteid;
		float _tarx, _tary;
		char _name[50];
		float _speed;
		float _angle;
		bool _is_lock;
		void Init(float x, float y, int sprite, const char*name, bool islock);
		void Render(float x, float y);
		void Update();
		void SetTar(float x, float y);
	};
	SCharSelect _char_select[CHAR_SPRITEID_NUM];
	void CharSelectRender(float x, float y);
	void CharSelectInit();
	void CharSelectUpdate();
	int _char_selected;
	bool _char_selecte_scrolling;
	float _char_select_scrollx;
	float _char_select_scrollx_tar;
	int _char_select_scroll_state;
	float _char_selecte_opac;
	int _bounce_count_times;
	//--------------------Design--------------------//

	CGameDesign _game_design;
	float GetDesignGeneralint(int row, int param);
	float GetDesignItemFloat(int row, int param);
	int GetDesignAchiveInt(int row, int param);
	const char* GetDesignAchiveString(int row, int param);
	int GetDesignMultiScoreInt(int row, int param);
	float GetDesignSpeedFloat(int row, int param);
	const char* GetDesignCharString(int row, int param);
	static const int _bg_spriteid[BG_SPRITEID_NUM];
	static const int _char_spriteid[CHAR_SPRITEID_NUM];
#define BOSS_NUM_SPRITE			10
	static const int _boss_sprite_normal[BOSS_NUM_SPRITE];
	static const int _boss_sprite_gray[BOSS_NUM_SPRITE];
	//--------------------Ext Function--------------------//
	void CheckFoodActive();
	float _speed_decrease;
	struct EffectLight
	{
		float _x, _y;
		int _dir;
		float _opac;
		float _speed;
		float _speed_opac;
		float _rotate;
		int _frameid;
		void Init(float x, float y, int opac, float speedopac, float speed);
		void Render(float x, float y, int opac);
		void Update();
	};
#define EFFECT_LIGHT_NUM_MAX		5
	EffectLight _effect_lighting[EFFECT_LIGHT_NUM_MAX];
	void AddEffectLight();
	void RenderEffectLight(float x, float y, int opac);
	void UpdateEffectLight();
#define PARTICLE_LIGHT_NUM_MAX		100
	struct ParticleLight
	{
		float _x[PARTICLE_LIGHT_NUM_MAX], _y[PARTICLE_LIGHT_NUM_MAX];
		float _opac[PARTICLE_LIGHT_NUM_MAX];
		float _speed[PARTICLE_LIGHT_NUM_MAX];
		float _speed_opac[PARTICLE_LIGHT_NUM_MAX];
		float _scale[PARTICLE_LIGHT_NUM_MAX];
		float _speedx[PARTICLE_LIGHT_NUM_MAX];
		float _speedy[PARTICLE_LIGHT_NUM_MAX];
		float _max_speedx[PARTICLE_LIGHT_NUM_MAX];
		int _frame_id[PARTICLE_LIGHT_NUM_MAX];
		int _state[PARTICLE_LIGHT_NUM_MAX];
		int _num;
		void Init(float speedopac, float speed);
		void Render(float x, float y, int opac);
		void Update();
		void AddParticleLight(float x, float y, float speedopac, float speed, int frame, int numframe);
		void RenderParticleLight(float x, float y, int opac);
		void UpdateParticleLight();
		void Active(float x, float y, float speedopac, float speed, int frame, int numframe, int num);
	};

	ParticleLight _particle_lighting;

#define PARTICLE_RAIN_NUM_MAX		100
	struct ParticleRain
	{
		float _x[PARTICLE_RAIN_NUM_MAX], _y[PARTICLE_RAIN_NUM_MAX];
		float _opac[PARTICLE_RAIN_NUM_MAX];
		float _speed[PARTICLE_RAIN_NUM_MAX];
		float _speed_opac[PARTICLE_RAIN_NUM_MAX];
		float _scale[PARTICLE_RAIN_NUM_MAX];
		float _tarx[PARTICLE_RAIN_NUM_MAX];
		float _tary[PARTICLE_RAIN_NUM_MAX];
		int _frame_id[PARTICLE_RAIN_NUM_MAX];
		int _state[PARTICLE_RAIN_NUM_MAX];
		int _num;
		void Init(float speedopac, float speed);
		void Render(float x, float y, int opac);
		void Update();
		void AddParticleLight(float x, float y, float speedopac, float speed, int frame, int numframe);
		void RenderParticleLight(float x, float y, int opac);
		void UpdateParticleLight();
		void Active(float x, float y, float speedopac, float speed, int frame, int numframe, int num);
	};

	ParticleRain _particle_rain;
#define PARTICLE_FLY_NUM_MAX		20
	struct SParticleFly
	{
		float _x[PARTICLE_FLY_NUM_MAX], _y[PARTICLE_FLY_NUM_MAX];
		float _tarx[PARTICLE_FLY_NUM_MAX], _tary[PARTICLE_FLY_NUM_MAX];
		float _speed[PARTICLE_FLY_NUM_MAX];
		int _frameid[PARTICLE_FLY_NUM_MAX];
		int _num;
		float _cenx, _ceny;
		void Init(int num, int frame, int numframe);
		void Active(float x, float y, float speed);
		void Render(float x, float y);
		void Update();
	};
	SParticleFly _particle_fly;
#define PARTICLE_FIRE_NUM			200
	struct SParticleFire
	{
		int _status;
		int _num;
		int _state[PARTICLE_FIRE_NUM];
		float _x_old[PARTICLE_FIRE_NUM], _y_old[PARTICLE_FIRE_NUM];
		float _x[PARTICLE_FIRE_NUM], _y[PARTICLE_FIRE_NUM];
		float _alpha[PARTICLE_FIRE_NUM];
		float _speed[PARTICLE_FIRE_NUM];
		float _speed_delta[PARTICLE_FIRE_NUM];
		float _scale[PARTICLE_FIRE_NUM];
		float _scale_speed[PARTICLE_FIRE_NUM];
		float _opac[PARTICLE_FIRE_NUM];
		float _opac_speed[PARTICLE_FIRE_NUM];
		int _change_alpha_hold[PARTICLE_FIRE_NUM];
		float _rotate[PARTICLE_FIRE_NUM];
		float _rotate_speed[PARTICLE_FIRE_NUM];
		void Init(int num);
		void Active(float x, float y);
		void Render(float x, float y);
		void Update();
		void CheckColli(CLine line, int type);

	};
	SParticleFire _particle_fire;
	SParticleFire _highscore_particle[5];
	void HighScoreParticleInit();
	void HighScoreParticleAdd(float x, float y);
	void HighScoreParticleRender(float x, float y);
	void HighScoreParticleUpdate();
	void SetPause();
	void SetResume();
	void RenderPause(float x, float y);
	bool UpdatePause();
	bool _isPauseGame;
	int _statePause, _stateZoomResume;
	int _opacityScreen, _opaResume;
	int _frame;
	int _timeResume;
	float _posResumeX, _posResumeY, _speedResume, _zoomResume;
	static void SetTranformPiont(PineMatrix4 tranf, float clipX, float clipY, float &outX, float &outY);
	static void SetTranformRect(PineMatrix4 tranf, float clipX, float clipY, float clipW, float clipH, float &outX, float &outY, float &outW, float &outH);
	//--------------------Result--------------------//
#if ANDROID_OS
#define RESULT_NUM_BUTTON				7
#else
#define RESULT_NUM_BUTTON				6
#endif

#define RESULT_BUTTON_PLAY				0
#define RESULT_BUTTON_ARCHI				1
#define RESULT_BUTTON_LEADERBOARD		2
#define RESULT_BUTTON_SETTING			3
#define RESULT_BUTTON_LEFT				4
#define RESULT_BUTTON_RIGHT				5
#define RESULT_BUTTON_GGPLAY			6

	Button _result_button[RESULT_NUM_BUTTON];
	void ResultRender(float x, float y, int opac);
	void ResultUpdate();
	bool ResultUpdateTouch();
	int _result_touch_id;
	float _result_button_scrolly;
	float _result_interface_scrollx;
	int _result_scroll_state;
	float _result_button_scroll_speed;
	bool _result_is_close;
	float _result_opac;
	int _stateObj;
	float _offObjY;
	CQuest _quest;
	int _result_quest_opac;

	int _result_options_state;
	bool _ads_show_complete;

#define SETTING_NUM_BUTTON			10
#define SETTING_BUTTON_SOUND		0
#define SETTING_BUTTON_MUSIC		1
#define SETTING_BUTTON_TW			2
#define SETTING_BUTTON_FB			3
#define SETTING_BUTTON_ICLOUD		4
#define SETTING_BUTTON_ADS			5
#define SETTING_BUTTON_IAPS			6
#define SETTING_BUTTON_CONTACT		7
#define SETTING_BUTTON_CREDITS		8
#define SETTING_BUTTON_BACK			9
	Button _setting_button[SETTING_NUM_BUTTON];
	float _setting_scrollx;
	float _setting_scrolly;
	int _setting_scroll_state;

#define RESULT_NUM_BUTTON_GGPLAY				3
#define RESULT_BUTTON_GGPLAY_ARCHIVEMENT		0
#define RESULT_BUTTON_GGPLAY_LEADERBOARD		1
#define RESULT_BUTTON_GGPLAY_SYNC				2
	Button _menu_ggplay[RESULT_NUM_BUTTON_GGPLAY];
	void MenuGGPlayActive();
	void MenuGGPlayInit();
	void MenuGGPlayRender(float x, float y, int opac);
	void MenuGGPlayUpdate();
	bool MenuGGPlayUpdateTouch();
	void SettingInit();
	void SettingActive();
	void SettingRender(float x, float y);
	void SettingUpdate();
	bool SettingUpdateTouch();

	float _menu_ggplay_speed;
	int _menu_ggplay_state, _menu_ggplay_id, _menu_ggplay_index;
	bool _menu_ggplay_active;

	float _credits_scrollx;
	int _credits_scroll_state;
	void CreditsInit();
	void CreditsActive();
	void CreditsRender(float x, float y);
	void CreditsUpdate();
	bool _share_highscore;
	//touch advance

	char _stringFeed[512];

	CDialog _dialog;
	float _volume;
	int _volume_state_change;
	void PushNotify();
	int _timeShowNotify;
	int _stateAppDownload;
	int _count_show_pineads;
	int _count_show_ads;
	int _count_show_advideos;

	//--------------------Result--------------------//
	void TutorialInit();
	void TutorialRender(float x, float y);
	void TutorialUpdate();
	bool TutorialUpdateTouch();
	void TutorialSetState(int state);
	int _tutorial_state;
	float _tutorial_zoomX;
	float _tutorial_circle_zoom;
	int _tutorial_touch_id;
	float _tutorial_opac;

	//--------------------Port--------------------//
	PinePoint _point_o, _point_right_bottom;
	void MeasureButton(Button* button);
	void MeasureButton(Button* button, int anchor);
	void MeasureButtonX(float posx, Button* button, BYTE anchor_horizonal);
	void MeasureButtonY(float posy, Button* button, BYTE anchor_vertical);
	PinePoint MeasureFrame(CSprite* sprite, UINT32 frameid);
	void MeasureAnim(CAnimObject* obj);
	PinePoint MeasureFrame(CSprite* sprite, UINT32 frameid, float scalex, float scaley);
	PinePoint MeasureFrame(CSprite* sprite, UINT32 frameid, BYTE anchor);
	void GooglePlayShowLeaderBoard();
	void GooglePlaySynDriver();
	void GooglePlayShowAchievement();
	void GooglePlaySubmitAchievement(char ggPlayID_ACHIEVEMENT[]);
	void SubmitAchievement(int index);
	void UpdateStatusAchievement();
	void SetAchievementSubmitCompleted(int index);
	void Num_PlayGame_GA_logsContent(int num_play);
	float _screen_width;
	float _screen_height;
	float _port_scale_x;
	float _port_scale_y;

	INT_TIME _hight_score;


};

#endif  // __PineEntertainment__HS__ //