#include "pch.h"
#include "../../GameCore/CGame.h"

extern "C" void OS_Exit(bool suppen);
#if defined (ANDROID_OS)
extern "C" void				OS_Rating();//open rating page
#endif

HSGame::HSGame()
{

}
void HSGame::Init()
{
	_game = GAME();
	_grap = G();
	PineGame::Init();
	_dialog.SetInitGame(GAME());
	//port game//
	_point_o = GAME()->_game_context.MeasureAnchor(0, 0, ANCHOR_LEFT | ANCHOR_TOP);
	_point_right_bottom = GAME()->_game_context.MeasureAnchor(0, 0, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	_screen_width = _point_right_bottom.X - _point_o.X;
	_screen_height = _point_right_bottom.Y - _point_o.Y;
	_port_scale_x = _screen_width / k_SCREEN_WIDTH;
	_port_scale_y = _screen_height / k_SCREEN_HEIGHT;
	PDEBUG("\n _point_o.X: %f, _point_o.Y: %f, point_right_bottom.X: %f, point_right_bottom.Y: %f", _point_o.X, _point_o.Y, _point_right_bottom.X, _point_right_bottom.Y);
	//printf("\n ScaleOutX: %f, ScaleOutY: %f, scaleX: %f, scaleY: %f, ScaleOut: %f, TranslateX: %f, TranslateY: %f", GAME()->_game_context.ScaleOutX, GAME()->_game_context.ScaleOutY, GAME()->_game_context.ScaleX, GAME()->_game_context.ScaleY, GAME()->_game_context.ScaleOut, GAME()->_game_context.TranslateX, GAME()->_game_context.TranslateY);
	GAME()->_typeOtimizeDrawModule = true;
	////port game////

	//Update 10/11/2017
	_count_frame_show_ads = 0;
	_time_delay_show_ads = (TIME_DELAY_SHOW_ADS * 60);
	_show_ads_interrupt = false;
	GAME()->banner_ads_implement = false;
#if ANDROID_OS
	GAME()->_gameBoardScoreItem.AccountType = PineAccountType::GooglePlay;
	sprintf(GAME()->_gameBoardScoreItem.BoardId, "%s", GP_LEADERBOARD_ID);
	GAME()->_has_show_ads_banner = false;
#endif
#if ADD_ADMOB
	GAME()->reduceY = 105;
#else
	GAME()->reduceY = 0;

#endif
	GAME()->_times_play_game = 1;
	GAME()->isClosedInterstitialAd = false;
}

void HSGame::UpdateAndRender()
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
#if !defined (MAC_OS)
	if (GAME()->_isBackkeyPress)
	{
		if (_dialog.IsOpening())
		{
			_dialog._typeDialog = CDialog::enumTypeDialog::DIALOG_NONE;
			game->_user._countShowRate = 10;
			game->_user.UserDataSave();
			_dialog._state = 5;
			GAME()->_isBackkeyPress = false;
		}
		else {
			if (GAME()->_waitForProcessBackkeyCount >= 1)
			{
				PDEBUG("\n Press backkey");
				GAME()->_isBackkeyPress = false;
				GAME()->_waitForProcessBackkeyCount = 0;
				OS_Exit(false);
			}
			else
			{
				GAME()->_waitForProcessBackkeyCount++;
			}
		}
		
	}
#endif
	GAME()->setUseOptimizeDrawModule(false);
	//update

	switch (_current_state)
	{
	case k_GS_LOGO:
		//if (!PineSocial::HasloggingSystem)
	{
		UpdateLogo();
	}
	RenderLogo(0, 0, 100);
	break;
	case k_GS_PLAYGAME:
		GamePlayUpdate();
		GamePlayRender(0, 0, 100);
		break;
	case k_GS_RESULT:
		ResultUpdate();
		GamePlayRender(0, 0, 100);
		ResultRender(0, 0, 100);
		break;
	default:
		break;
	}

	if (_user._hadRemovedAds)
	{
		if (GAME()->_has_show_ads_banner)
		{
#if defined(ANDROID_OS)
			GAME()->HideBannerAds();
#endif
		}
	}

	if (_current_state > k_GS_LOGO)
	{
		if (!_user._hadRemovedAds)
		{
			if (!_result_is_close)
			{
				if (_time_delay_show_ads <= 0)
				{
					_time_delay_show_ads = TIME_DELAY_SHOW_ADS * 60;
					GAME()->ShowAds();
				}
			}
			if (_user._stepTutorial >= 100 && _time_delay_show_ads > 0)
			{
				_time_delay_show_ads--;
				if (_time_delay_show_ads <= 0)
				{
					_time_delay_show_ads = 0;
				}
			}

			if (_count_frame_show_ads > 0)
			{
				_count_frame_show_ads--;
				if (_count_frame_show_ads <= 0)
				{
					_count_frame_show_ads = 0;
					_time_delay_show_ads = TIME_DELAY_SHOW_ADS * 60;
					GAME()->ShowAds();
				}
			}
		}
	}

#pragma region TOPEBOX_PROMO_GAME
	if (game->_user._game_promo_has_show)
	{
		if (game->_topebox_game.GamePromoIsReady())
		{
			if (!game->_topebox_game.GamePromoHasTouchDown())
			{
				float max = game->_topebox_game._icon_w*1.3f;
				if (game->_topebox_game_scrollEff < max)
				{
					game->_topebox_game_scrollEff += 10.0f;
					if (game->_topebox_game_scrollEff >= max)
					{
						game->_topebox_game_scrollEff = max;
					}
				}
			}
		}
	}
	else
	{
		float max = -game->_topebox_game._icon_w*1.5f;
		if (game->_topebox_game_scrollEff > max)
		{
			game->_topebox_game_scrollEff += -10;
			if (game->_topebox_game_scrollEff <= max)
			{
				game->_topebox_game_scrollEff = max;
				game->_topebox_game.ResetGamePromoInfo();

			}
		}
	}
	int fw = GET_SPRITE(SPRITEID_HUB_ANDROID)->GetFrameWidth(1);
	char textname[128];
	if (_topebox_game.GamePromoIsReady())
	{
		int lenght = sprintf(textname, "%s", _topebox_game._des);
		float text_w = GET_SPRITE(SPRITEID_FONT_TITLE_01)->GetAnsiTextWidth(textname, 0, lenght);
		float scalex = text_w / fw + 0.3f;
		G()->SetOpacity(30);
		G()->SetScale(scalex, 2.5f, GAME_PROMO_RENDER_X + -_topebox_game._icon_w + _topebox_game_scrollEff + 0 - 60 + _point_o.X, GAME_PROMO_RENDER_Y - _topebox_game._icon_h + 0 - 65 + _point_o.Y);
		GET_SPRITE(SPRITEID_HUB_ANDROID)->DrawFrame(G(), 1, GAME_PROMO_RENDER_X + -_topebox_game._icon_w + _topebox_game_scrollEff + 0 - 55 + _point_o.X, GAME_PROMO_RENDER_Y - _topebox_game._icon_h + 0 - 65 + _point_o.Y);
		G()->ClearScale();
		G()->SetOpacity(100);
	}
	game->_topebox_game.RenderGamePromo(GAME_PROMO_RENDER_X + -_topebox_game._icon_w + game->_topebox_game_scrollEff + 0 + _point_o.X, GAME_PROMO_RENDER_Y - game->_topebox_game._icon_h + 0 - 5 + _point_o.Y);
#pragma endregion



	_dialog.Render(0, 0 + 40 * (1.0f - _port_scale_x));
	_dialog.Update();
	_dialog.WaiteProcessing();
#if ADD_ADVERT
	if (GAME()->_advert.HasPopup())
	{
		GAME()->_advert.Render();
		GAME()->_advert.Update();
	}
#endif
#if ADD_ADVERT
	if (GAME()->_advert.HasPopup())
	{
		GAME()->_advert.Render();
		GAME()->_advert.Update();

	}
	if (_stateAppDownload == 1 && CGame::_this->_advert.GetAppInstallState() == PINEADS_INSTALL_STATE_DOWNLOAD_DONE)
	{
		_stateAppDownload = 0;
		CGame::_this->_advert.SetDownloadCompete();
	}
	if (CGame::_this->_advert.GetAppInstallState() == PINEADS_INSTALL_STATE_DOWNLOAD_CANCEL && _stateAppDownload == 1)
	{
		_stateAppDownload = 0;
	}
#endif
	switch (_volume_state_change)
	{
	case 0:
		break;
	case 1:
		if (_volume < 100)
		{
			float vol_plus = 0.3f;
			if (_current_state == k_GS_PLAYGAME)
			{
				vol_plus = 2;
			}
			_volume += vol_plus;
			if (_volume >= 100)
			{
				_volume = 100;
			}
		}
		break;
	case 2:
		if (_volume > 0)
		{
			_volume -= 2;
			if (_volume <= 0)
			{
				_volume = 0;
				_volume_state_change = 1;
				if (_current_state == k_GS_RESULT)
				{
					GAME()->StopCurrentMusic();
					_game->LoadMusic(BGM_ALLGAME);
					_game->PlayCurrentMusic(-1);
				}
				else if (_current_state == k_GS_PLAYGAME)
				{
					GAME()->StopCurrentMusic();
					_game->LoadMusic(BGM_INGAME);
					_game->PlayCurrentMusic(-1);
				}
			}
		}
		break;
	default:
		break;

}

	GAME()->SetVolume(_volume);
	if (_logo_scrolly < k_SCREEN_WIDTH)
	{
		if (_current_state > k_GS_LOGO)
		{
			_logo_scrolly += 30;

		}
		_grap->SetScale(_port_scale_x, _port_scale_x, k_SCREEN_WIDTH / 2 - (SIZE_CHAR_WIDTH*_port_scale_x) / 2, k_SCREEN_HEIGHT - SIZE_FLATFORM_HEIGHT - SIZE_FLATFORM_HEADER_HEIGHT - SIZE_CHAR_HEIGHT*_port_scale_x);
		GET_SPRITE(SPRITEID_OBJECT_CHARACTER_01)->DrawFrame(_grap, 0, k_SCREEN_WIDTH / 2 - (SIZE_CHAR_WIDTH*_port_scale_x) / 2, k_SCREEN_HEIGHT - SIZE_FLATFORM_HEIGHT - SIZE_FLATFORM_HEADER_HEIGHT - SIZE_CHAR_HEIGHT*_port_scale_x);
		GET_SPRITE(SPRITEID_OBJECT_CHARACTER_01)->DrawFrame(_grap, 2, k_SCREEN_WIDTH / 2 - (SIZE_CHAR_WIDTH*_port_scale_x) / 2, k_SCREEN_HEIGHT - SIZE_FLATFORM_HEIGHT - SIZE_FLATFORM_HEADER_HEIGHT - SIZE_CHAR_HEIGHT*_port_scale_x);
		_grap->ClearScale();
		GET_SPRITE(SPRITEID_HUD_LOADINGSCREEN)->DrawFrame(_grap, 0, 0, -_logo_scrolly + _point_o.Y);
		//GET_SPRITE(SPRITEID_HUD_LOADINGSCREEN)->DrawFrame(_grap, 1, 0 - _point_o.X, _logo_scrolly);
		//_grap->SetScale(0.66f, 0.66f, k_SCREEN_WIDTH - 100, _logo_scrolly + k_SCREEN_HEIGHT - 100 - _point_o.Y);
		GET_SPRITE(SPRITEID_HUB_ANDROID)->DrawFrame(_grap, 0, 0, _logo_scrolly - _point_o.Y);
		//_grap->ClearScale();
	}
	if (_current_state > k_GS_LOGO)
	{

		_quest.CheckQuestComplete();
		_particle_lighting.UpdateParticleLight();
		//_particle_rain.UpdateParticleLight();
		UpdateEffectLight();
#if defined (MAC_OS)
		if (GAME()->_device.IsNetworkAvailable())
		{
			bool pinead_pop;
			pinead_pop = true;
			if (_dialog.IsOpening() || _user._hadRemovedAds)
			{
				pinead_pop = false;
			}
			if (!GAME()->_advert.HasPopup() && GAME()->_advert.IsAvailable() && _count_show_pineads <= 0 && pinead_pop)
			{
				GAME()->_advert.Init();
				_stateAppDownload = 1;
				_count_show_pineads = 1;
			}
		}
#endif
		if (GAME()->_notificationEnable != _user._isShowNotifi)
		{
			GAME()->_notificationEnable = _user._isShowNotifi;
		}
	}

	if (_game_over)
	{
		GAME()->FPS_LIMIT = FPS_MAX;
		//_new_highscore.ForceEnd();
	}
	if (_new_highscore.IsFree())
	{
		switch (_game_over_effect_state)
		{
		case 1:
		{
			if (_game_over_effect_scale <= 0)
			{
				_game_over_effect_scale = 0.1f;

		}
			_game_over_hold--;
			int shake = (60 - _game_over_hold) / 4;
			_shakex = CMath::RANDOM(-shake, shake);
			_shakey = CMath::RANDOM(-shake, shake);
			_game_over_effect_scale *= 1.05f;
			if (_game_over_effect_scale >= 1.5f)
			{
				_game_over_opac += 8;
				if (_game_over_opac >= 120)
				{
					_game_over_opac = 100;
					_game_over_hold = 0;
					_game_over_effect_state = 2;
					if (_game_score > _high_score)
					{
						_share_highscore = true;
						_high_score_submited = false;
						_high_score = _game_score;
						if (_high_score > 0)
						{
							_quest.AddCurrentValue(CQuest::enumObjective::Objective_3, 1);
						}

					}
					AGamePlayInit();
					_main_char._x = k_SCREEN_WIDTH / 2 - _main_char._w / 2;
					_volume_state_change = 2;
					EffectComboForceEnd();
					SetState(k_GS_RESULT);
					//GAME()->SetVolume(_volume);

				}
			}
	}
		break;
		case 2:
			//_game_over_effect_scale *= 1.2f;
			if (_game_over_opac > 0)
			{
				_game_over_opac -= 5;
				if (_game_over_opac <= 0)
				{
					_game_over_opac = 0;
					_game_over_effect_state = 3;
					//PLAY_SFX(SFX_GAMEOVER);
				}
			}
			break;
		case 3:

			_game_over_effect_state = 0;
			_user.UserDataSave();


			break;
		default:
			break;
		}
	}
	else
	{
		_game_over_effect_scale = 0;
	}
	_grap->SetOpacity(_game_over_opac);
	_grap->SetColor(0xffffffff);
	_grap->FillFullScreen(true);
	_grap->SetOpacity(100);

	/*_grap->SetColor(0xff000000);
	_grap->FillRect(-k_SCREEN_WIDTH, 0, k_SCREEN_WIDTH, k_SCREEN_HEIGHT,true);

	_grap->SetColor(0xff000000);
	_grap->FillRect(k_SCREEN_WIDTH, 0, k_SCREEN_WIDTH, k_SCREEN_HEIGHT,true);*/

	/*_grap->SetOpacity(100);
	_grap->SetColor(0xfffff4f0);
	_grap->FillRect(_point_o.X, _screen_height - 50, _screen_width, 50, true);*/


	if (!_high_score_submited)
	{
		_high_score_submited = true;
#ifdef MAC_OS
		PineGameCenterController::SubmitMyScore("HH_Leaderboard", _high_score);
#endif
	}
	}

//Lam --- Port function


void HSGame::MeasureButton(Button* button)
{
	PinePoint btpos;
	btpos.X = button->_posX + button->TZ_GetX();
	btpos.Y = button->_posY + button->TZ_GetY();
	float btn_width = button->TZ_GetWidth();
	float btn_height = button->TZ_GetHeight();
	btpos = GAME()->_game_context.MeasureDensity(btpos, btn_width, btn_height);
	button->_posX = btpos.X - button->TZ_GetX();
	button->_posY = btpos.Y - button->TZ_GetY();
}

void HSGame::MeasureButton(Button* button, int anchor)
{
	PinePoint btpos;
	btpos.X = button->_posX + button->TZ_GetX();
	btpos.Y = button->_posY + button->TZ_GetY();


	if ((anchor & ANCHOR_RIGHT) != 0) {
		btpos.X = btpos.X - k_SCREEN_WIDTH;
	}

	if ((anchor & ANCHOR_BOTTOM) != 0) {
		btpos.Y = btpos.Y - k_SCREEN_HEIGHT;
	}

	btpos = GAME()->_game_context.MeasureAnchor(btpos, anchor);
	button->_posX = btpos.X - button->TZ_GetX();
	button->_posY = btpos.Y - button->TZ_GetY();
}
void HSGame::MeasureButtonX(float posx, Button* button, BYTE anchor_horizonal) {
	PinePoint btpos;
	btpos = GAME()->_game_context.MeasureAnchor(posx, 0, anchor_horizonal);
	float button_fixed_x = button->GetFixX();
	button->_posX = btpos.X - button_fixed_x;
}
void HSGame::MeasureButtonY(float posy, Button* button, BYTE anchor_vertical) {
	PinePoint btpos;
	btpos = GAME()->_game_context.MeasureAnchor(posy, 0, anchor_vertical);
	float button_fixed_y = button->GetFixY();
	button->_posY = btpos.Y - button_fixed_y;
}
PinePoint HSGame::MeasureFrame(CSprite* sprite, UINT32 frameid) {
	float framex = sprite->GetFModuleX(frameid, 0);
	float framey = sprite->GetFModuleY(frameid, 0);
	float framew = sprite->GetFModuleWidth(frameid, 0);
	float frameh = sprite->GetFModuleHeight(frameid, 0);
	return GAME()->_game_context.MeasureDensity(framex, framey, framew, frameh);
}
void HSGame::MeasureAnim(CAnimObject* obj) {
	obj->CalculateCurrentAFrame();
	int fid = obj->_anim_sprite->GetFrameIDOfAFrame(obj->_current_aframes);
	PineRectangle bound = obj->_anim_sprite->GetFmoduleBound(fid, 0);
	PinePoint anim_pos = obj->_anim_sprite->GetAFramePos(obj->_current_aframes);
	PinePoint point = GAME()->_game_context.MeasureDensity(bound.X + anim_pos.X, bound.Y + anim_pos.Y, bound.W, bound.H);
	obj->_x = obj->_x + point.X - bound.X - anim_pos.X;
	obj->_y = obj->_y + point.Y - bound.Y - anim_pos.Y;

}

PinePoint HSGame::MeasureFrame(CSprite* sprite, UINT32 frameid, float scalex, float scaley)
{
	float framex = sprite->GetFModuleX(frameid, 0);
	float framey = sprite->GetFModuleY(frameid, 0);
	float framew = sprite->GetFModuleWidth(frameid, 0);
	float frameh = sprite->GetFModuleHeight(frameid, 0);
	float scale = scalex > scaley ? scalex : scaley;
	PinePoint point = GAME()->_game_context.MeasureDensity(framex - framex * (scale - 1), framey - framey * (scale - 1), framew * scale, frameh * scale);
	point.X -= framex;
	point.Y -= framey;
	return point;
}

PinePoint HSGame::MeasureFrame(CSprite* sprite, UINT32 frameid, BYTE anchor)
{
	PineRectangle bound = sprite->GetFrameBound(frameid);
	PinePoint btpos;
	if (anchor == ANCHOR_TOP)
	{
		btpos = GAME()->_game_context.MeasureAnchor(0, bound.Y, anchor);


	}

	return btpos;
}
//Lam End Port

//Update 10/11/2017
void HSGame::GooglePlayShowLeaderBoard()
{
	HSGame *game = (HSGame*)GAME()->_current_game;
#if ANDROID_OS
	if (GAME()->_async_task_manager._is_login_google)
	{
		PDEBUG("\nGAME()->_async_task_manager._on_process_show_leaderBoard: %d", GAME()->_async_task_manager._on_process_show_leaderBoard);
		if (!GAME()->_async_task_manager._on_process_show_leaderBoard)
		{

			//GAME()->_gameBoardScoreItem.Score = game->_hight_score;
			//GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_POST_SCORE);
			GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_SHOW_LEADERBOARD);
			GAME()->_async_task_manager._on_process_show_leaderBoard = true;
		}
	}
	else
	{
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_IS_LOG_IN_GOOGLE_PLAY);
		GAME()->_async_task_manager._is_force_login_google_play = true;
		GAME()->_async_task_manager._on_process_show_leaderBoard = true;
	}
#endif
}
void HSGame::GooglePlaySynDriver()
{
	HSGame *game = (HSGame*)GAME()->_current_game;

#if ANDROID_OS
	if (!GAME()->_async_task_manager._on_process_sync_cloud)
	{
		game->_user.CheckLocalAndCloudSaveFile(false);
	}
#endif
}

void HSGame::GooglePlayShowAchievement()
{
#if ANDROID_OS
	if (GAME()->_async_task_manager._is_login_google)
	{
		if (!GAME()->_async_task_manager._on_process_show_achievement)
		{
			GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_SHOW_ACHIEVEMENTS);
			GAME()->_async_task_manager._on_process_show_achievement = true;
		}
	}
	else
	{
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_IS_LOG_IN_GOOGLE_PLAY);
		GAME()->_async_task_manager._is_force_login_google_play = true;
		GAME()->_async_task_manager._on_process_show_achievement = true;
	}
#endif
}
void HSGame::GooglePlaySubmitAchievement(char ggPlayID_ACHIEVEMENT[])
{
#if ANDROID_OS	
	if (GAME()->_async_task_manager._is_login_google)
	{
		GAME()->_async_task_manager._need_check_unlock_achievement = true;
		PDEBUG("\n############## ggPlayID_ACHIEVEMENT gggg: %s", ggPlayID_ACHIEVEMENT);
		sprintf(GAME()->_async_task_manager._achivement_id, "%s", ggPlayID_ACHIEVEMENT);
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_UNLOCK_ACHIEVEMENT);
	}
	
#endif
}

void HSGame::Num_PlayGame_GA_logsContent(int num_play) {

	char title[128];
	char decs[1024];
	char type[128];

	const char*  times;
	if (num_play <= 5)
	{
		times = "Play <5 games";
	}
	else if (num_play <= 10)
	{
		times = "Play <10 games";
	}
	else {
		times = "Play >10 games";
	}

	sprintf(title, "%s", times);
	sprintf(decs, "%s", times);
	sprintf(type, "%s", "num_playgame logs");
#if defined (ANDROID_OS)	
	PDEBUG("\n logContent title:%s --- decs:%s --- type:%s", title, decs, type);
	Device::GA_LogContents(title, decs, type);
#endif
}