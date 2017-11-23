#include "pch.h"
#include "../../GameCore/CGame.h"

void HSGame::ResetBGCamera()
{
	_root_bg_x = 0;
	_root_bg_y = 0;
	_bg_offset_x = 0;
	_bg_offset_y = 0;
	_current_frame = 0;
	_bg_state_jump = BG_STATE_NONE;
	_root_bg_high = 0;
}

float HSGame::GetBGTranslateX()
{
	return _root_bg_x + _bg_offset_x;
}
float HSGame::GetBGTranslateY()
{
	return _root_bg_y + _bg_offset_y;
}
void HSGame::SetBGJump()
{
	_root_bg_x = GetBGTranslateX();
	_root_bg_y = GetBGTranslateY();
	_bg_state_jump = BG_STATE_JUMP;
	_current_frame = 0;
	_root_bg_high += 2;
}
void HSGame::UpdateBGJump()
{
	if (_bg_state_jump == BG_STATE_JUMP)
	{
		float angle = (_current_frame * 180) / BG_JUMP_FRAME;
		_bg_offset_y = CMath::SIN(angle)*BG_HIGHT_JUMP;
		if (_current_frame >= BG_JUMP_FRAME)
		{
			_bg_offset_x = 0;
			_bg_offset_y = 0;
			_bg_state_jump = BG_STATE_NONE;
		}
		else
		{
			_current_frame++;
		}
	}
	else if (_bg_state_jump == BG_STATE_NONE)
	{
		_root_bg_y -= (BG_HIGHT_JUMP / (BG_JUMP_FRAME*1.0f));
		if (_root_bg_y < _root_bg_high)
		{
			_root_bg_y = _root_bg_high;
		}

	}
	//printf("\n _root_bg_y = %f -- _bg_offset_y = %f",_root_bg_y,_bg_offset_y);
	//printf("\nGetBGTranslateY = %f -- _bg_state_jump = %d",GetBGTranslateY(),_bg_state_jump);
}
void HSGame::AGamePlayInit()
{
	_food.Init(SPRITEID_HUD_01);
	_particle_fly.Init(2, 2, 4);
	_particle_fly.Active(-150 + _point_o.X, 0, 7);
	_particle_fire.Init(150);
	ResetBGCamera();
	ObjectEnvirInit();

	_bounce_count_times = 0;
	_effect_plus_score_state = 0;
	_effect_plus_score_jump_speed = 1.0f;
	_effect_plus_score_y = 0;

	_count_frame_hold_flatform_return = 0;
	_touch_index = -1;
	ControlInit();
	_has_change_color = false;
	_border_change_color = 0;
	_border_change_color_opac = 100;
	_main_char.Init(_char_spriteid[_user._typeCharacterUse], 0, k_SCREEN_WIDTH / 2 - SIZE_CHAR_WIDTH*_port_scale_x / 2, k_SCREEN_HEIGHT - SIZE_FLATFORM_HEIGHT - SIZE_FLATFORM_HEADER_HEIGHT - SIZE_CHAR_HEIGHT*_port_scale_x);
	_game_over = false;
	_shakex = 0;
	_shakey = 0;
	_quest.InitQuest();
	CharSelectInit();
	TutorialInit();
	EffectComboInit();
	_game_combo = 0;
	ComboResetTime();
}
void HSGame::GamePlayInit()
{
	_share_highscore = false;
	HighScoreParticleInit();
	_game_new_score = false;
	_new_highscore.Init(SPRITEID_OBJECT_HIGHSCORE, 0, 12, 80);
	_result_interface_scrollx = 0;

	_quest.SetCurrentValue(CQuest::enumObjective::Objective_1, 0);

	for (int i = 0; i < GAMEPLAY_NUM_BUTTON; i++)
	{
		_gameplay_button[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER);
		int anim = 0;
		if (!_sound_music_enabled)
		{
			anim = 4;
		}
		_gameplay_button[i].SetAnim(SPRITEID_HUD_02, anim + i * 4);
		_gameplay_button[i].SetPos(-_point_o.X, 0);

	}

	_game_over_effect_state = 0;
	_game_over_effect_scale = 0;
	_isPauseGame = false;
	_game_over_opac = 0;

	_game_over_hold = 0;
	_game_score = 0;
	/*_high_score = 1000;*/
	_game_score_target = 0;
	_control_speed_move = 2;
}
void HSGame::GamePlayRender(float x, float y, int opac)
{
	HSGame *game = (HSGame*)GAME()->_current_game;

	_sound_music_enabled = false;
	if (_game->_soundEnable || _game->_musicEnable)
	{
		_sound_music_enabled = true;
	}
	if (!_sound_music_enabled)
	{
		_gameplay_button[GAMEPLAY_BUTTON_SOUND].SetAnim(SPRITEID_HUD_02, 4);
		_gameplay_button[GAMEPLAY_BUTTON_SOUND].SetPos(-_point_o.X, 0);
	}
	else
	{
		_gameplay_button[GAMEPLAY_BUTTON_SOUND].SetAnim(SPRITEID_HUD_02, 0);
		_gameplay_button[GAMEPLAY_BUTTON_SOUND].SetPos(-_point_o.X, 0);
	}

	BGRender(x, y);
	if ((_result_scroll_state == 5 && !_char_selecte_scrolling) || _current_state == k_GS_PLAYGAME)
	{
		_main_char.Render(x, y);
	}

	_particle_fire.Render(x, y);
	if (_game_over_effect_state == 1)
	{
		_grap->SetScale(_game_over_effect_scale, _game_over_effect_scale, _main_char._x + _main_char._w / 2 + x, _main_char._y + _main_char._w / 2 + y);
		GET_SPRITE(SPRITEID_OBJECT_CIRCLE_01)->DrawFrame(_grap, 0, _main_char._x + _main_char._w / 2 + x, _main_char._y + _main_char._w / 2 + y);
		_grap->ClearScale();
	}
	_food.Render(x, y);
	float scrolly = _result_interface_scrollx;
	if (_result_scroll_state == 5)
	{
		scrolly = 0;
	}
	int spacing = GET_SPRITE(SPRITEID_FONT_SCORE_01)->_char_spacing;
	GET_SPRITE(SPRITEID_FONT_SCORE_01)->_char_spacing = 6;
	GET_SPRITE(SPRITEID_FONT_SCORE_01)->DrawAnsiTextFormat(_grap, k_SCREEN_WIDTH / 2 + x, 100 + _effect_plus_score_y + y + scrolly*0.5f + _point_o.Y, _HCENTER, "%d", _game_score);
	GET_SPRITE(SPRITEID_FONT_SCORE_01)->_char_spacing = spacing;
	spacing = GET_SPRITE(SPRITEID_FONT_BESTSCORE_01)->_char_spacing;
	GET_SPRITE(SPRITEID_FONT_BESTSCORE_01)->_char_spacing = 4;
	GET_SPRITE(SPRITEID_FONT_BESTSCORE_01)->DrawAnsiTextFormat(_grap, k_SCREEN_WIDTH / 2 + x, 145 + y + scrolly*0.5f + _point_o.Y, _HCENTER, "*%d", _high_score);
	//GET_SPRITE(SPRITEID_FONT_BESTSCORE_01)->DrawAnsiTextFormat(_grap,  x + 40 + _point_o.X, 50 + y + scrolly*0.5f, _LEFT, "%f", _main_char._speed);
	GET_SPRITE(SPRITEID_FONT_BESTSCORE_01)->_char_spacing = spacing;
	//_grap->SetScale(2, 2, 20, 40);
 //   //_main_char._speed = 4.0f;
	//GET_SPRITE(SPRITEID_FONT_SCORE_01)->DrawAnsiTextFormat(_grap, 20, 100, _LEFT, "%d", _game_combo);
	//GET_SPRITE(SPRITEID_FONT_SAMPLE)->DrawAnsiTextFormat(_grap, 20, 40, _LEFT, "fps :  %d", GAME()->FPS_LIMIT);
	//_grap->ClearScale();
	for (int i = 0; i < GAMEPLAY_NUM_BUTTON; i++)
	{
		_gameplay_button[i].SetPos(x - _point_o.X, y + scrolly*0.3f + _point_o.Y);
		_gameplay_button[i].Render(_grap);
		_gameplay_button[i].Update();
	}
	EffectComboRender(x, y);
	_new_highscore.Render(x, y);
	HighScoreParticleRender(x, y);
	//if (_current_state == k_GS_PLAYGAME)
	{
		if (_user._stepTutorial < 100)
		{
			TutorialRender(x, y);
		}
	}
	//------------Render Border
	_grap->SetOpacity(100 - _border_change_color_opac);
	_grap->SetColor(0xff5172b2);
	_grap->FillRect(_point_o.X, _point_o.Y, _screen_width, 5, true);
	_grap->FillRect(_screen_width + _point_o.X - 5, _point_o.Y, 5, _screen_height, true);
	_grap->FillRect(_point_o.X, _screen_height - 5, _screen_width, 5, true);
	_grap->FillRect(_point_o.X, _point_o.Y, 5, _screen_height, true);
	if (_border_change_color > 0)
	{
		if (_border_change_color_opac < 100)
		{
			_border_change_color_opac = 100;
		}
	}
	else
	{
		if (_border_change_color_opac > 0)
		{
			_border_change_color_opac -= 20;
		}
	}
	if (_border_change_color_opac > 0)
	{
		_grap->SetOpacity(_border_change_color_opac);
		_grap->SetColor(0xfffff4f0);
		_grap->FillRect(_point_o.X, _point_o.Y, _screen_width, 5, true);
		_grap->FillRect(_screen_width + _point_o.X - 5, _point_o.Y, 5, _screen_height, true);
		_grap->FillRect(_point_o.X, _screen_height - 5, _screen_width, 5, true);
		_grap->FillRect(_point_o.X, _point_o.Y, 5, _screen_height, true);
	}
	_grap->SetOpacity(_border_change_color_opac);
	_grap->SetOpacity(100);
	_grap->SetColor(0xfffff4f0);
	_grap->FillRect(_point_o.X, _screen_height + _point_o.Y - GAME()->_banner_ads_scrolly, _screen_width, GAME()->reduceY, true);
	RenderPause(x, y);

	

}
void HSGame::GamePlayUpdate()
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
	if (_sub_state == k_INIT)
	{
		GamePlayInit();

		_sub_state = k_LOOP;
		GAME()->_times_play_game++;
		
	}

	UpdatePause();
	if (_isPauseGame)
	{
		return;
	}
	//if (_current_state == k_GS_PLAYGAME)
	{
		if (_user._stepTutorial < 100)
		{
			if (_user._stepTutorial == 0)
			{
				if (_tutorial_state == 0)
				{
					TutorialSetState(1);
				}
			}
		}
	}

	if (_user._stepTutorial < 100)
	{
		TutorialUpdate();
	}
	if (_game_combo_timereset > 0)
	{
		if (_tutorial_state == 0)
		{
			_game_combo_timereset--;
		}
		if (_game_combo_timereset <= 0)
		{
			_game_combo = 0;
			_quest.SetCurrentValue(CQuest::enumObjective::Objective_2, 0);
			//ComboResetTime();
		}
	}
	if (_game_score > _high_score && !_game_new_score && _high_score > 0)
	{
		PLAY_SFX(SFX_HIGHSCORE);
		EffectFlippingActive(0);
		_game_new_score = true;
	}
	HighScoreParticleUpdate();
	_new_highscore.Update();
	UpdateBGJump();
	if (_border_change_color > 0)
	{
		_border_change_color--;
	}
	switch (_effect_plus_score_state)
	{
	case 0:
		_effect_plus_score_y = 0;
		_effect_plus_score_jump_speed = 1.0f;
		break;
	case 1:
		_effect_plus_score_y -= _effect_plus_score_jump_speed;
		_effect_plus_score_jump_speed *= 1.8f;
		if (_effect_plus_score_y <= -10)
		{
			_effect_plus_score_y = -10;
			_effect_plus_score_state = 2;
			_effect_plus_score_jump_speed = 1.0f;
			_game_score = _game_score_target;
			PLAY_SFX(SFX_COUTING);
		}
		break;
	case 2:
		_effect_plus_score_y += _effect_plus_score_jump_speed;
		_effect_plus_score_jump_speed *= 1.8f;
		if (_effect_plus_score_y >= 0)
		{
			_effect_plus_score_y = 0;
			_effect_plus_score_state = 0;
		}
		break;
	case 3:
		break;
	default:
		break;
	}
#pragma region CHECK_COLLI
	/*CLine line1, line2;
	line1.CreateLine(0, 0, 0, k_SCREEN_HEIGHT);
	_particle_fire.CheckColli(line1,0);
	line2.CreateLine(k_SCREEN_WIDTH, 0, k_SCREEN_WIDTH, k_SCREEN_HEIGHT);
	_particle_fire.CheckColli(line2,0);
	for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
	{
		CLine line3, line4,line5;
		line3.CreateLine(_control[i]._x - 10, _control[i]._y - 10, _control[i]._x + _control[i]._w + 10, _control[i]._y - 10);
		_particle_fire.CheckColli(line3, 1);
		line4.CreateLine(_control[i]._x - 10, _control[i]._y , _control[i]._x - 10, _control[i]._y + _control[i]._h);
		_particle_fire.CheckColli(line4, 0);
		line5.CreateLine(_control[i]._x + _control[i]._w+10, _control[i]._y, _control[i]._x + _control[i]._w+10, _control[i]._y + _control[i]._h);
		_particle_fire.CheckColli(line5, 0);
	}*/

#pragma endregion


	EffectComboUpdate();
	_particle_fire.Update();
	_particle_fly.Update();

	ObjectEnvirUpdate();
	FoodSpawn();
	_main_char.Update();
	if (_game_over)
	{
		return;
	}
	if (_tutorial_touch_id == -1)
	{
		ControlUpdate();
		GamePlayUpdateTouch();
	}

	_food.Update();
	CheckFoodActive();
}
void HSGame::ControlRender(float x, float y, int opac)
{
	G()->SetColor(0xff000000);
	G()->FillRect(_point_o.X, k_SCREEN_HEIGHT - SIZE_FLATFORM_HEIGHT, _screen_width, k_SCREEN_HEIGHT);
	for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
	{
		_control[i].Render(x, y);
	}
}
void HSGame::ControlUpdate()
{
	bool scrolling = false;
	int index_current = -1;
	ControlUpdateTouch();
	_control_speed_move = 8;
	switch (_main_char._orien)
	{
	case CHARACTER_DIR_LEFT:
		for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
		{
			if (_main_char._x >= _control[i]._x && _main_char._x <= _control[i]._x + _control[i]._w)
			{
				_main_char._area = i;
			}
		}
		break;
	case CHARACTER_DIR_RIGHT:
		for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
		{
			if (_main_char._x + _main_char._w >= _control[i]._x && _main_char._x + _main_char._w <= _control[i]._x + _control[i]._w)
			{
				_main_char._area = i;
			}
		}
		break;
	default:
		break;
	}
	bool hold_touch = false;
	for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
	{
		if (_main_char._y + _main_char._h > _control[_main_char._area]._y)
		{
			_game_over = true;
			_game_over_effect_state = 1;
			_game_over_effect_scale = 0.1f;
			_game_over_hold = 60;
			PLAY_SFX(SFX_CUBE_DIE);
			//_particle_fire.Active(_main_char._x + _main_char._w / 2, _main_char._y + _main_char._h / 2);
		}
		_control[i].Update();
		if (!_game_over)
		{
			_main_char._y = _control[_main_char._area]._y - _main_char._h;
		}
		if (_control[i]._status > 0)
		{
			scrolling = true;
		}
		if (_control[i]._hold >= 1)
		{
			hold_touch = true;
		}
		//printf("\n 000 _control[%d]._status:%d", i, _control[i]._status);
	}
	if (!scrolling)
	{
		bool all_reset = true;
		for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
		{
			//printf("\n _control[%d]._status:%d", i, _control[i]._status);
			if (_control[i]._is_reset)
			{
				if (_control[i]._status != 0)
				{
					all_reset = false;
				}
			}
		}
		if (all_reset)
		{
			for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
			{
				//printf("\n _control[%d]._hold:%d", i, _control[i]._hold);
				if (_control[i]._is_reset)
				{
					if (_control[i]._status == 0)
					{
						float dis = _main_char._h;
						dis = _control_speed_move;
						if (hold_touch)
						{
							//printf("\n _control[%d]._hold:%d", i, _control[i]._hold);
							dis = 1;
							_control[i].SetMove(_control[i]._tarx, _control[i]._tary + _main_char._h, dis, false, 3, 0);
						}
						else
						{
							dis = _control_speed_move;
							float plusy = -1;
							if (_control[i]._is_set_begin)
							{
								_control[i]._beginy += plusy;
								_control[i].SetMove(_control[i]._beginx, _control[i]._beginy, dis, false, 1, 0);
								_control[i]._is_set_begin = false;
							}
							else
							{
								_control[i].SetMove(_control[i]._tarx, _control[i]._tary + _main_char._h + plusy, dis, false, 1, 0);
							}
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
	{
		if (!hold_touch &&  _control[i]._status == 3)
		{
			_control[i].SetMove(_control[i]._tarx, _control[i]._tary, _control_speed_move, false, 1, 0);
		}
	}

#if defined(MAC_OS)

#else

#endif

}
void HSGame::ControlInit()
{
	for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
	{
		float px, py;
		int w, h;
		int f_width = SIZE_FLATFORM_WIDTH*_port_scale_x;
		px = i*(f_width)+i * 2 + _point_o.X;
		py = k_SCREEN_HEIGHT - SIZE_FLATFORM_HEIGHT - SIZE_FLATFORM_HEADER_HEIGHT;
		w = f_width;
		h = k_SCREEN_HEIGHT;
		_control[i].Init(px, py, w, h, i);
	}
}
bool HSGame::ControlUpdateTouch()
{
	bool press = false;
	int num_drag_down, num_drag_up;
	num_drag_up = num_drag_down = 0;

	for (int i = 0; i < GAME()->_num_touches; i++)
	{
		for (int touch = 0; touch < CONTROL_NUM_BUTTON; touch++)
		{
			if (_control[touch]._is_lock)
			{
				continue;
			}
			int x = _control[touch]._x;
			int y = _control[touch]._y;
			int w = _control[touch]._w;
			int h = _control[touch]._h;
			if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					if (_control[touch]._touch_id == -1)
					{
						_control[touch]._touch_id = TOUCHS(i)->_id;
						_control[touch]._hold = 0;
						//TOUCHS(i)->_state = TOUCH_DRAG;
						ControlActive(touch);
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (_control[touch]._touch_id == -1)
					{
						_control[touch]._touch_id = TOUCHS(i)->_id;
						_control[touch]._hold = 0;
						//TOUCHS(i)->_state = TOUCH_DRAG;
						ControlActive(touch);
					}
					///*if (_control[touch]._touch_id == -1)
					//{
					//	_control[touch]._touch_id = TOUCHS(i)->_id;
					//	_control[touch]._hold = 0;
					//	ControlActive(touch);
					//}
					//else */if(_control[touch]._touch_id == TOUCHS(i)->_id)
					//{
					//	_control[touch]._hold++;
					//	if (_control[touch]._hold >= 11)
					//	{
					//		_control[touch]._hold = 1;
					//		_control[touch]._touch_id = TOUCHS(i)->_id;
					//		ControlActive(touch);
					//	}
					//}
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (_control[touch]._touch_id == TOUCHS(i)->_id)
					{
						_control[touch]._touch_id = -1;
						_control[touch]._hold = 0;
					}
				}
			}
			else
			{
				if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (_control[touch]._touch_id == TOUCHS(i)->_id)
					{
						_control[touch]._touch_id = -1;
						_control[touch]._hold = 0;
					}
				}
			}
		}

	}

	return press;
}
bool HSGame::GamePlayUpdateTouch()
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
	/*for (int i = 0; i <GAME()->_num_touches; i++)
	{
		int x = _point_o.X;
		int y = 0;
		int w = 300;
		int h = 300;
		if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
		{
			if (TOUCHS(i)->_state == TOUCH_DOWN)
			{
#if defined (_WINDOWS)
				//_quest.AddCurrentValue(CQuest::enumObjective::Objective_6, 1);
				//_quest.SetCurrentValue(CQuest::enumObjective::Objective_19, _quest._maxValue-2);
#else
				if (GAME()->FPS_LIMIT == FPS_MAX)
				{

					GAME()->FPS_LIMIT = 5;
				}
				else
				{
					GAME()->FPS_LIMIT = FPS_MAX;
				}
#endif
				//_main_char._speed += 0.5;
				TOUCHS(i)->_state =TOUCH_DRAG;
			}
			else if (TOUCHS(i)->_state == TOUCH_DRAG)
			{
			}
			else if (TOUCHS(i)->_state == TOUCH_UP)
			{

			}
		}
	}*/
	bool press = false;
	int index = -1;
	bool ret = false;
	
#pragma region TOPEBOX_PROMO_GAME
	if (_user._game_promo_has_show)
	{
		if (_topebox_game.GamePromoHasTouchDown())
		{
#if !defined (_WINDOWS)
			if (Device::IsNetworkAvailable())
#endif
			{
				_show_ads_interrupt = false;
				_user._game_promo_has_show = false;
				_user._game_promo_order++;
				_user._game_promo_delay_show = time(NULL) + TIME_DELAY_SHOW_TOPEBOX_ADS;
				_topebox_game.ResponseRequest();
#if defined (ANDROID_OS) || defined (MAC_OS)
				PDEBUG("\n ##### Open URL: %s \n", _topebox_game._link_store);
				PineSocial::OpenURL(_topebox_game._link_store);

#endif
				char title[128];
				char decs[1024];
				sprintf(title, "%s Click", "Topebox-Ads");
				sprintf(decs, "%s Click", "Topebox-Ads");
				char type[128];
				sprintf(type, "%s", "topebox_ads");
#if defined (MAC_OS) || (ANDROID_OS)
				PDEBUG("\n logContent title:%s --- decs:%s --- type:%s", title, decs, type);
				Device::GA_LogContents(title, decs, type);
#endif
				//_user.UserDataSave();
			}
		}
	}
	//if (!(_menu_button_active || _menu_button_scrolling))
	{
		if (_user._game_promo_has_show)
		{
			if (_topebox_game.UpdateTouchGamePromo())
			{
				return true;
			}
		}
	}

#pragma endregion

	for (int i = 0; i < GAMEPLAY_NUM_BUTTON; i++)
	{
		if (_gameplay_button[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}

#if !defined (MAC_OS)
	if (GAME()->_isBackkeyPress)
	{
		GAME()->isClosedInterstitialAd = false;
		game->SetPause();
		game->SetResume();
		game->_volume = 0;
		GAME()->_isBackkeyPress = false;
		return true;
	}
#endif

	if (ret)
	{
		if (_gameplay_button[index].NeedAction())
		{
			//printf("\n index = %d",index);
			switch (index)
			{
			case GAMEPLAY_BUTTON_SOUND:
			{
				int anim = 4;
				_sound_music_enabled = !_sound_music_enabled;
				if (_sound_music_enabled)
				{
					anim = 0;
					_game->_soundEnable = true;
					_game->_musicEnable = true;
					_game->LoadMusic(BGM_INGAME);
					_game->PlayCurrentMusic(-1);
					_volume = 0;
					_volume_state_change = 1;
				}
				else
				{
					_game->_soundEnable = false;
					_game->_musicEnable = false;
					_game->PauseCurrentMusic();
				}
				_gameplay_button[index].SetAnim(SPRITEID_HUD_02, anim + index * 4);
				_gameplay_button[index].SetPos(0 - _point_o.X, 0);
			}
			break;
			default:
				break;
			}
			if (_gameplay_button[index]._state != BUTTON_STATE_LOCK && _gameplay_button[index]._state != BUTTON_STATE_HIDE)
			{
				_gameplay_button[index].SetState(BUTTON_STATE_NORMAL);
			}
		}

	}

	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		for (int touch = 0; touch < GAMEPLAY_NUM_BUTTON; touch++)
		{
			int x = _gameplay_button[touch].TZ_GetX() - 20;
			int y = _gameplay_button[touch].TZ_GetY() - 20;
			int w = _gameplay_button[touch].TZ_GetWidth() + 40;
			int h = _gameplay_button[touch].TZ_GetHeight() + 40;
			Button *button = &_gameplay_button[touch];
			if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT);
						button->_touch_id = TOUCHS(i)->_id;
						press = true;
						//PLAY_SFX(SFX_BUTTON);
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					button->SetState(BUTTON_STATE_SELECT);
					button->_touch_id = TOUCHS(i)->_id;
					press = true;
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_ACTIVE);
						button->_touch_id = -1;
						press = true;
					}
				}
			}
			else
			{
				if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
					}
				}
			}
		}

	}
	return press;
}

void HSGame::SControl::SetPos(float x, float y)
{
	_x = x;
	_y = y;
}
void HSGame::SControl::Init(float x, float y, int w, int h, int colortype)
{
	_x = x;
	_y = y;
	_beginx = x;
	_beginy = y;
	_w = w;
	_h = h;
	_status = 0;
	_touch_id = -1;
	_hold = 0;
	_tarx = _x;
	_tary = _y;
	_jump_speed = 0;
	_is_reset = false;
	_touch_true = false;
	_time_delay = 0;
	_color_type = colortype;
	_time_delay_up = 0;
	_fake_in_opac = 0;
	_is_set_begin = false;
	_is_lock = false;
}
void HSGame::SControl::Render(float x, float y)
{
	HSGame*game = (HSGame*)(GAME()->_current_game);
	COLOR color_footer, color_header;
	switch (_color_type)
	{
	case 0:
		color_footer = 0xff78556d;
		color_header = 0xffd3b1c8;
		break;
	case 1:
		color_footer = 0xffe8d469;
		color_header = 0xfff5ebb4;
		break;
	case 2:
		color_footer = 0xfff3616f;
		color_header = 0xffeea9af;
		break;
	case 3:
		color_footer = 0xff364b75;
		color_header = 0xff7e96c4;
		break;
	case 4:
		color_footer = 0xff83a471;
		color_header = 0xffb4d39b;
		break;
	case 5:
		color_footer = 0xffc56446;
		color_header = 0xffec9c83;
		break;
	default:
		break;
	}

	float dis_plus = 0;
	if (_time_delay_up > 0)
	{
		float plus = game->_main_char._h / 3;
		dis_plus = _time_delay_up*plus;
		dis_plus = game->_main_char._h;
	}
	G()->SetColor(color_footer);
	G()->FillRect(_x + x, _y + SIZE_FLATFORM_HEADER_HEIGHT + dis_plus + y, _w, _h, true);
	if (_touch_true)
	{
		_fake_in_opac -= 20;
		if (_fake_in_opac <= 0)
		{
			_touch_true = false;
			/*for (int i = 0; i < OBJECT_ENVIR_NUM; i++)
			{
				float dis = (game->_main_char._h*0.1f - 3)/game->_object_envir[i]._layer;
				if (i == 0)
				{
					dis = game->_main_char._h - 5;
				}
				else if (i==1)
				{
					dis = game->_main_char._h/3 - 2;
				}
				game->_object_envir[i].SetTarget(game->_object_envir[i]._x, game->_object_envir[i]._tary - dis,3);
			}*/
		}
		G()->SetOpacity(_fake_in_opac);
		G()->SetColor(0xfffff4f0);
		G()->FillRect(_x + x, _y + SIZE_FLATFORM_HEADER_HEIGHT + dis_plus + y, _w, _h, true);
		G()->SetOpacity(100);
	}
	//--------------Render Border
	color_header = 0xff5172b2;
	G()->SetOpacity(100 - game->_border_change_color_opac);
	G()->SetColor(color_header);
	G()->FillRect(_x + x, _y + y, _w, SIZE_FLATFORM_HEADER_HEIGHT, true);
	if (game->_border_change_color_opac > 0)
	{
		G()->SetOpacity(game->_border_change_color_opac);
		G()->SetColor(0xfffff4f0);
		G()->FillRect(_x + x, _y + y, _w, SIZE_FLATFORM_HEADER_HEIGHT, true);
	}
	G()->SetOpacity(100);
}
void HSGame::SControl::Update()
{
	if (_time_delay_up > 0)
	{
		_time_delay_up--;
	}

	HSGame*game = (HSGame*)(GAME()->_current_game);
	if (CHECK_POINT_IN_CIRCLE(_x, _y, _tarx, _tary, _jump_speed))
	{
		if (_status == -1 || _status == 2)
		{
			if (_jump_speed != 0)
			{
				if (!CHECK_POINT_IN_CIRCLE(_beginx, _beginy, _tarx, _tary, _jump_speed))
				{
					_x = _tarx;
					_y = _tary;
					_tarx = _beginx;
					_tary = _beginy;
					//_jump_speed = 1;
					if (_status == 2)
					{
						_status = 0;
						_jump_speed = game->_control_speed_move;
					}
				}
				else
				{
					_x = _tarx;
					_y = _tary;
					_jump_speed = 0;
					_status = 0;
				}
			}
		}
		else
		{
			_time_delay--;
			_x = _tarx;
			_y = _tary;

			if (_time_delay < 0)
			{
				_jump_speed = 0;
				_time_delay = 0;
				_status = 0;
			}
		}
	}
	else
	{
		float alpha = CMath::ANGLE(_x, _y, _tarx, _tary);
		//_jump_speed *= 1.1f;
		/*if (_jump_speed >= 20)
		{
			_jump_speed = 20;
		}*/
		_x += _jump_speed*CMath::COS(alpha);
		_y += _jump_speed*CMath::SIN(alpha);
		/*if (_touch_true)
		{
			if (CHECK_POINT_IN_CIRCLE(_x, _y, _tarx, _tary, _jump_speed*2))
			{
				_touch_true = false;
			}
		}*/
	}
}

void HSGame::SControl::SetMove(float x, float y, float speed, bool reset, int status, int delay)
{
	HSGame*game = (HSGame*)(GAME()->_current_game);
	_tarx = x;
	_tary = y;
	_is_reset = reset;
	_status = status;
	_time_delay = delay;
	//if (_jump_speed == 0)
	{
		_jump_speed = speed;
	}
	if (_tary <= 0)
	{
		_tary = 0;
	}
}
void HSGame::ControlActive(int control_index)
{
	int touch = control_index;
	if ((_main_char._x >= _control[touch]._x && _main_char._x <= _control[touch]._x + _control[touch]._w)
		|| (_main_char._x + _main_char._w >= _control[touch]._x && _main_char._x + _main_char._w <= _control[touch]._x + _control[touch]._w))
	{
		//printf("\n down _control[%d]._status:%d", touch, _control[touch]._status);
		if (_main_char._color_type == _control[touch]._color_type)
		{
			if (_user._stepTutorial == 1 || _user._stepTutorial == 3
				|| _user._stepTutorial == 6 || _user._stepTutorial == 7
				|| _user._stepTutorial == 8)
			{
				TutorialSetState(5);
				//_user._stepTutorial++;
			}
			SetBGJump();
			float x = _control[touch]._x;
			float y = _control[touch]._tary - _main_char._h;
			if (y < k_SCREEN_HEIGHT / 2 - 3 * _main_char._h)
			{
				y = _control[touch]._beginy;
			}
			_control[touch].SetMove(x, y, _main_char._h, true, 1, 10);
			_control[touch]._touch_true = true;
			_control[touch]._fake_in_opac = 200;

			if (_main_char._orien == CHARACTER_DIR_LEFT)
			{
				int nextindex = touch - 1;
				if (nextindex < 0)
				{
					nextindex = 0;
				}
				_main_char.ChangeColor(_control[nextindex]._color_type);
			}
			else if (_main_char._orien == CHARACTER_DIR_RIGHT)
			{
				int nextindex = touch + 1;
				if (nextindex >= CONTROL_NUM_BUTTON)
				{
					nextindex = CONTROL_NUM_BUTTON - 1;
				}
				_main_char.ChangeColor(_control[nextindex]._color_type);
			}
			_effect_plus_score_state = 1;
			int score = _quest._multi;
			_game_score_target += score;
			_food.SetTar(_food._tarx, _food._tary + _main_char._h);
			_quest.AddCurrentValue(CQuest::enumObjective::Objective_1, score);
			ComboResetTime();
			if (_game_combo_timereset > 0)
			{
				int soundid = SFX_COMBO_01 + _game_combo;
				if (soundid > SFX_COMBO_17)
				{
					soundid = SFX_COMBO;
				}
				PLAY_SFX(soundid);
				_game_combo++;
				if (_game_combo > 1)
				{
					int cbx = _main_char._x + _main_char._w / 2;
					int cby = _main_char._y + _main_char._h / 2 - 100;
					if (cbx < SIZE_FLATFORM_WIDTH / 2)
					{
						cbx = SIZE_FLATFORM_WIDTH / 2;
					}
					if (cbx > _screen_width - SIZE_FLATFORM_WIDTH / 2)
					{
						cbx = _screen_width - SIZE_FLATFORM_WIDTH / 2;
					}
					EffectComboAdd(cbx, cby, 1.2, 0.05f, 240, 5, 2, _game_combo, _control[control_index]._color_type);
				}
				_quest.AddCurrentValue(CQuest::enumObjective::Objective_2, 1);
			}
			/*for (int i = 0; i < OBJECT_ENVIR_NUM; i++)
			{
				float dis = _main_char._h*0.1f/_object_envir[i]._layer;
				if (i == 0)
				{
					dis = _main_char._h;
				}
				else if (i==1)
				{
					dis = _main_char._h/3;
				}
				_object_envir[i].SetTarget(_object_envir[i]._x, _object_envir[i]._tary + dis,1);
			}*/
			for (int j = 0; j < CONTROL_NUM_BUTTON; j++)
			{
				_control[j]._is_set_begin = true;
				if (j != touch)
				{
					x = _control[j]._x;
					y = _control[j]._tary - _main_char._h;
					bool isreset = true;

					if (_control[touch]._tary >= _control[j]._tary)
					{
						y += _main_char._h;
						if (_control[touch]._tary > _control[j]._tary)
						{
							_control[j]._is_set_begin = false;
						}
					}
					_control[j].SetMove(x, y, _main_char._h, isreset, 1, 10);
					_control[j]._time_delay_up = 8;

					/*_control[j]._status = -1;
					_control[j]._is_reset = true;
					_control[j]._jump_speed = 0;*/
				}
			}
		}
		else
		{
			if (_control[touch]._status == 0)
			{
				_main_char._y -= _main_char._h;
				//_particle_fire.Active(_main_char._x + _main_char._w / 2, _main_char._y + _main_char._h / 2);
				_game_over = true;
				_game_over_effect_state = 1;
				_game_over_effect_scale = 0.1f;
				_game_over_hold = 60;
				_control[touch]._touch_true = false;
				float x = _control[touch]._x;
				float y = _control[touch]._tary - _main_char._h;
				_control[touch].SetMove(x, y, _main_char._h, false, 1, 0);
				PLAY_SFX(SFX_CUBE_DIE);
			}
		}
	}
	else
	{
		_control[touch]._touch_true = false;
		float x = _control[touch]._x;
		float y = _control[touch]._tary - _main_char._h;
		_control[touch].SetMove(x, y, _main_char._h, false, 1, 0);
		PLAY_SFX(SFX_WRONG_TAP);
		_game_combo = 0;
		_quest.SetCurrentValue(CQuest::enumObjective::Objective_2, 0);
	}
}
void HSGame::ControlChangeColor()
{
	printf("\n change color");
	int arr_color[COLOR_NUM];
	int colorindex = -1;
	for (int i = 0; i < COLOR_NUM; i++)
	{
		arr_color[i] = i;
	}
	int index = CMath::RANDOM(0, COLOR_NUM - 2);
	int num = 0;
	while (num < 10 /*|| _control[index]._color_type == arr_color[index]*/)
	{
		index = CMath::RANDOM(0, COLOR_NUM - 2);
		int temp = arr_color[index];
		arr_color[index] = arr_color[index + 1];
		arr_color[index + 1] = temp;
		num++;
	}
	if (_main_char._orien == CHARACTER_DIR_LEFT)
	{
		colorindex = 3;
		if (_has_change_color)
		{
			index = CMath::RANDOM(1, 2);
			int newcolor = arr_color[index];
			if (_main_char._color_type == arr_color[colorindex] || arr_color[0] == _main_char._color_type)
			{
				if (arr_color[0] == _main_char._color_type)
				{
					colorindex = 0;
				}
				arr_color[index] = arr_color[colorindex];
				arr_color[colorindex] = newcolor;
			}
			_has_change_color = false;
		}
	}
	else if (_main_char._orien == CHARACTER_DIR_RIGHT)
	{
		colorindex = 0;
		if (_has_change_color)
		{
			index = CMath::RANDOM(1, 2);
			int newcolor = arr_color[index];
			if (_main_char._color_type == arr_color[colorindex] || arr_color[3] == _main_char._color_type)
			{
				if (arr_color[3] == _main_char._color_type)
				{
					colorindex = 3;
				}
				arr_color[index] = arr_color[colorindex];
				arr_color[colorindex] = newcolor;
			}
			_has_change_color = false;
		}
	}
	bool has_color = false;
	for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
	{
		if (arr_color[i] == _main_char._color_type)
		{
			has_color = true;
		}
		_control[i]._color_type = arr_color[i];
	}
	if (!has_color)
	{
		int index = CMath::RANDOM(1, 2);
		_control[index]._color_type = _main_char._color_type;
	}
	if (_first_game)
	{
		for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
		{
			_control[i]._color_type = i;
		}
		_main_char._color_type = CMath::RANDOM(0, CONTROL_NUM_BUTTON - 1);
	}

	if (_user._stepTutorial == 0)
	{
		_main_char._color_type = _control[2]._color_type;
	}
	if (_user._stepTutorial >= 3 && _user._stepTutorial <= 6)
	{
		if (_control[2]._color_type != _main_char._color_type)
		{
			int index = 0;
			for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
			{
				if (_control[i]._color_type == _main_char._color_type)
				{
					index = i;
					break;
				}
			}
			int temp = _control[2]._color_type;
			_control[2]._color_type = _main_char._color_type;
			_control[index]._color_type = temp;
		}
	}
}
void HSGame::FoodSpawn()
{
	if (_food._state != 0)
	{
		return;
	}

	int index = CMath::RANDOM(0, CONTROL_NUM_BUTTON - 1);
	int param_min = CGameDesign::FileLevel::CSheetitem::score_min;
	int param_max = CGameDesign::FileLevel::CSheetitem::score_max;
	int row = 0;
	if (_game_score > 0)
	{
		row = 1;
	}
	int min = GetDesignItemFloat(row, param_min);
	int max = GetDesignItemFloat(row, param_max);
	index = 3;
	int posx = _point_o.X + SIZE_FLATFORM_WIDTH*_port_scale_x / 2;
	posx += index*(SIZE_FLATFORM_WIDTH*_port_scale_x);
	//int posy = k_SCREEN_HEIGHT - SIZE_FLATFORM_HEIGHT - SIZE_FLATFORM_HEADER_HEIGHT - SIZE_CHAR_HEIGHT - CMath::RANDOM(min,max)*_main_char._h;
	int posy = -CMath::RANDOM(min, max)*_main_char._h;
	int param_speed = CGameDesign::FileLevel::CSheetitem::speed_decrease;
	_speed_decrease = GetDesignItemFloat(row, param_speed);
	if (_user._stepTutorial < 100)
	{
		posy = k_SCREEN_HEIGHT - SIZE_FLATFORM_HEIGHT - SIZE_FLATFORM_HEADER_HEIGHT - SIZE_CHAR_HEIGHT*_port_scale_x - CMath::RANDOM(4, 6)*_main_char._h;
	}
	_food.Add(posx, posy, 0, 1);
}