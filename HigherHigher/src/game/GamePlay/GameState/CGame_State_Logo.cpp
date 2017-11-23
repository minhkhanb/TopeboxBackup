#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"
//extern "C" void InitADManager();
//extern "C" void InitADBanner();
//extern "C" void InitTakePicture();
//extern "C" void takeAPicture();
//extern "C" void LoadImageToPngFile(const char* url,const char *file_name);
extern "C" void AuthTwitter();
extern "C" void CaptureScreenshot();
extern "C" void PostScreenToTwitter();
extern "C" void charboostShowVideoReward(const char* title);
extern "C" void ShowAdmobFullScreen();
extern "C" void OS_InitCaptureViewVideo();
extern "C" void OS_ScreenCaptureVideoStart();
extern "C" void OS_ScreenCaptureVideoStop();
extern "C" int OS_GetCaptureVideoState();
extern "C" void OS_SetCaptureVideoState(int state);
extern "C" void OS_VideoPostSocial(const char* feed);
extern "C" void OS_ShowWebPage(const char* url, bool isOffline);

#define LOGO_STATE_DEFAULT			0
#define LOGO_STATE_INIT_INFO		1
#define LOGO_STATE_CHECK_NETWORK	2
#define LOGO_STATE_SYNC_SAVE		3
#define LOGO_STATE_LOAD_FROM_SV		4
#define LOGO_STATE_SYNC_TO_SV		5
#define LOGO_STATE_FB_INIT          6

#define LOGO_STATE_LOAD_RES			50
#define LOGO_STATE_LOAD_SOUND		60

#define LOGO_STATE_END				150

void HSGame::UpdateLogo()
{

	if (_sub_state == k_INIT)
	{
		_topebox_game.InitGamePromo(0, 0, 64, 64);
		_topebox_game_scrollEff = 0;
		Device::InitDeviceMotion(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2, k_SCREEN_WIDTH, k_SCREEN_HEIGHT);
		_loading_step = 0;
		_game->_isNeedOptimize = false;
#if ADD_ADVERT
		/*while(!ADVERT().IsServerProssingComplete())
		{
		;
		}
		ADVERT().SetOpen(PINE_ADVERT_SCROLL_TYPE);*/
#endif
		GAME()->LoadSpriteCurrentVer(SPRITEID_OBJECT_01, true, !_game->_isNeedOptimize);
		GAME()->LoadSpriteCurrentVer(SPRITEID_OBJECT_02, true, !_game->_isNeedOptimize);
		GAME()->LoadSpriteCurrentVer(SPRITEID_OBJECT_03, true, !_game->_isNeedOptimize);
		GAME()->LoadSpriteCurrentVer(SPRITEID_OBJECT_CHARACTER_01, true, !_game->_isNeedOptimize);
		GAME()->LoadSpriteCurrentVer(SPRITEID_HUD_LOADINGSCREEN, true, !_game->_isNeedOptimize);
		GAME()->LoadSpriteCurrentVer(SPRITEID_HUB_ANDROID, true, !_game->_isNeedOptimize);
		_sub_state = k_LOOP;
		_game->_frame_auto_stop = 10000;
		_game_design.Level.Load();
		_game_design.Achievement.Load();
        _high_score_submited = true;
		_logo_scrolly = 0;
		_logo_scroll_state = 0;
		ResetBGCamera();
		_first_game = true;
		ObjectEnvirInit();
		ControlInit();
        _border_change_color_opac = 100;
		
	}
	else if (_sub_state == k_LOOP)
	{
		_game->_frame_auto_stop -= DT();
		//printf("\n AcellX = %f, AcellY = %f, AcellZ = %f", Device::GetDeviceMotionX(),Device::GetDeviceMotionY(),Device::GetDeviceMotionZ());
		switch (_loading_step)
		{
		case LOGO_STATE_DEFAULT:
			_user.UserDataLoad();
			_loading_step = LOGO_STATE_INIT_INFO;
			break;
		case LOGO_STATE_INIT_INFO:
#ifdef MAC_OS
                if (GAME()->_gcSocial == NULL)
                {
                    GAME()->_gcSocial = new PineGameCenterController();
                }
                GAME()->_gcSocial->Init();
                _loading_step = LOGO_STATE_CHECK_NETWORK;
#else
                _loading_step = LOGO_STATE_CHECK_NETWORK;
#endif
			break;
		case LOGO_STATE_CHECK_NETWORK:
#if defined (MAC_OS)
#if ADD_FACEBOOK
                GAME()->_serverThread._isNetworkAvailable = GAME()->_device.IsNetworkAvailable();
                if (strcmp(_user.GetFBID(), "fb_id") != 0 && _user._hadLoginFB)
                {
                    if (GAME()->_serverThread._isNetworkAvailable)
                    {
                        GAME()->_serverThread.AddCommand(CServerThread::enumTypeCommand::COMMAND_TYPE_INIT_FB);
                    }
                }
#endif
#endif
                
                
                _loading_step = LOGO_STATE_LOAD_RES;
			break;
		case LOGO_STATE_LOAD_RES:
		{
			int sprite_start = SPRITEID_FONT_SAMPLE;
			int sprite_end = SPRITEID_OBJECT_CHARACTER_10;
			for (int i = sprite_start; i <= sprite_end; i++)
			{
				GAME()->LoadSpriteCurrentVer(i, true, !_game->_isNeedOptimize);
			}
		}
		_loading_step++;
		break;
		case LOGO_STATE_LOAD_SOUND:
		{
			int sound_start = SFX_COMBO_01;
			int sound_end = SFX_POPUP;
			for (int i = sound_start; i <= sound_end; i++)
			{
				GAME()->LoadSFX(i);
			}
		}
			break;
		case LOGO_STATE_END:
		{
            
			_timeShowNotify = 0;
			_particle_lighting.Init(0, 0);
			_particle_lighting.Active(0, 0, 0, 0, 19, 1, 100);
			/*_particle_rain.Init(0, 2);
			_particle_rain.Active(0, 0, 0, 0, 19, 1, 100);*/
			AddEffectLight();
			_volume = 0;
			_char_selected = 0;
			_count_show_ads = 0;
            if(_user._stepTutorial < 100)
            {
                _count_show_ads = -1;
            }
			_count_show_advideos = 0;
			_char_selecte_opac = 100;
			GamePlayInit();
			AGamePlayInit();
			_quest._perScroll = _quest._percent;
			_quest._newPercent = _quest._percent;
			_volume_state_change = 1;
			_game->StopCurrentMusic(-1);
			_game->LoadMusic(BGM_ALLGAME);
			_game->PlayCurrentMusic(-1);
			SetState(k_GS_RESULT);

			/*if (_user._game_promo_delay_show < time(NULL))
			{
				_topebox_game.SetDeviceID(_user._game_promo_deviceID);
				_topebox_game.GetTopeboxPromoGameInfo(_user._game_promo_revision, _user._game_promo_order);
			}*/
		}
			break;
		default:
			break;
		}
		if (_loading_step > LOGO_STATE_LOAD_RES)
		{
			_loading_step++;
		}
#if ADD_ADVERT

#endif

	}
	else if (_sub_state == k_DESTROY)
	{
	}
}

void HSGame::RenderLogo(float x, float y, int opac)
{

	/*G()->SetColor(0xffffff);
	G()->FillRect(0, 0, k_SCREEN_WIDTH, k_SCREEN_HEIGHT);
	G()->SetColor(0xffff00ff);
	float scale = (_loading_step*1.0f) / LOGO_STATE_END;
	G()->SetScale(scale, 1.0f, x, k_SCREEN_HEIGHT / 2 + y);
	G()->FillRect(x, k_SCREEN_HEIGHT / 2 - 15 + y, k_SCREEN_WIDTH, 30);
	G()->ClearScale();*/
	switch (_bg_spriteid_current)
	{
	case 0:
		_grap->SetColor(0xffc9849c);
		break;
	case 1:
		_grap->SetColor(0xff1a2225);
		break;
	case 2:
		_grap->SetColor(0xff07213f);
		break;
	default:
		break;
	}
	_grap->FillFullScreen(true);
	ObjectEnvirRender(x, y);
	ControlRender(x, y, 100);
    _grap->SetColor(0xfffff4f0);
	_grap->FillRect(_point_o.X, _point_o.Y, _screen_width, 5, true);
	_grap->FillRect(_screen_width + _point_o.X - 5, _point_o.Y, 5, _screen_height, true);
	_grap->FillRect(_point_o.X, _screen_height - 5, _screen_width, 5, true);
	_grap->FillRect(_point_o.X, _point_o.Y, 5, _screen_height, true);
}



void CGame::UnloadGameBackground()
{
	//printf("\nUnload Game Back ground");
	if (!_compete_backup_stock)
	{
		_step_force_loading = 0;
		BackupAnimStock();
		for (int i = 0; i < TOTAL_SPRITE_INGAME; i++)
		{
			if (_sprite_loaded[i])
			{
				//printf("\nUnload SpriteID = %d",i);
				UnloadSpriteVer(i);
				_sprite_loaded[i] = 1;
			}
		}
		_compete_backup_stock = true;
		printf("\nUnload background");
	}

}
bool CGame::LoadGameForceground()
{

	if (_step_force_loading >= TOTAL_SPRITE_INGAME + 10)
	{
		return true;
	}

	_compete_backup_stock = false;
	//printf("\nloading force ground = %d",_step_force_loading);
	//G()->SetColor(0);
	//G()->FillRect(0,0,k_SCREEN_WIDTH,k_SCREEN_HEIGHT);


	if (_step_force_loading >= 10)
	{
		for (int i = 0; i < 100; i++)
		{

			if (_sprite_loaded[_step_force_loading - 10])
			{
				//printf("\n_step_force_loading-10 = %d",(_step_force_loading-10));
				LoadSpriteCurrentVer(_step_force_loading - 10, _sprite_antialias_loaded[_step_force_loading - 10], _sprite_optimized_loaded[_step_force_loading - 10]);
			}
			_step_force_loading++;
			if (_step_force_loading >= TOTAL_SPRITE_INGAME + 10)
			{
				RestoreAnimStock();

				return true;
			}

		}
	}
	else
	{
		_step_force_loading++;
	}
	return false;
}

