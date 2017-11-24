#include "pch.h"
#include "../../GameCore/CGame.h"

extern "C" void OS_BrowserWebPage(const char* url);
extern "C" void SendMailSupport(const char *body, const char *title);
extern "C" void IapRestore();
extern "C" void RequestRegisterNotificationSetting();
extern "C" void PurchaseProduct(const char *indentify);
extern "C" void IapRestore();
#if defined (ANDROID_OS)
extern "C" void				OS_Rating();//open rating page
#endif

void HSGame::ResultRender(float x, float y, int opac)
{
	G()->SetColor(0xff000000);
	G()->SetOpacity(_result_opac);
	G()->FillFullScreen(true);
	G()->SetOpacity(100);
	//int my = GET_SPRITE(SPRITEID_HUD_RESULT)->GetFModuleY(8,0);
	//GET_SPRITE(SPRITEID_FONT_LEVEL_01)->DrawAnsiTextFormat(_grap, k_SCREEN_WIDTH / 2 + x + _result_interface_scrollx, y + my - 65, _HCENTER, ": %d", _user._levelMultiplier);
	if (!_dialog.IsOpening())
	{
		if (_result_quest_opac < 100)
		{
			_result_quest_opac += 10;
		}
	}
	else
	{
		if (_result_quest_opac > 0)
		{
			_result_quest_opac -= 10;
		}
	}
	_quest.RenderQuest(x + _result_interface_scrollx, y + _point_o.Y, _result_quest_opac);
	G()->SetOpacity(opac);
	CharSelectRender(x - _char_select_scrollx, y);
	
	for (int i = 0; i < RESULT_NUM_BUTTON; i++)
	{
		if (i >= RESULT_BUTTON_LEFT)
		{
			G()->SetOpacity(_char_selecte_opac);
		}
		else
		{
			float px = x;
			float py = y - _result_interface_scrollx - GAME()->_banner_ads_scrolly - _point_o.Y;
			switch (i)
			{
			case RESULT_BUTTON_ARCHI:
				px += _point_o.X;
				break;
			case RESULT_BUTTON_LEADERBOARD:
				px += _point_o.X / 2;
				break;
			case RESULT_BUTTON_SETTING:
				px -= _point_o.X;
				break;
			default:
				break;
			}
			_result_button[i].SetPos(px, py);
		}
#if ANDROID_OS
		if (i == RESULT_BUTTON_GGPLAY)
		{
			_result_button[i].SetPos(x, y - _result_interface_scrollx + _menu_ggplay_scroll_y*0.75f - _point_o.Y - GAME()->_banner_ads_scrolly);
		}
#endif
		_result_button[i].Render(_grap);
		_result_button[i].Update();
		G()->SetOpacity(opac);
	}
	if (_result_options_state > 0)
	{
		SettingRender(x, y - _point_o.Y);
		if (_credits_scroll_state > 0)
		{
			CreditsRender(x, y - _point_o.Y);
		}
	}
	MenuGGPlayRender(x, y - _point_o.Y, opac);
	_grap->SetOpacity(100);
	_grap->SetColor(0xfffff4f0);
	_grap->FillRect(_point_o.X, _screen_height + _point_o.Y - GAME()->_banner_ads_scrolly, _screen_width, GAME()->reduceY, true);
	G()->SetColor(0xff000000);
	G()->SetOpacity(_result_opac);
	G()->FillRect(_point_o.X, _screen_height + _point_o.Y - GAME()->_banner_ads_scrolly, _screen_width, GAME()->reduceY, true);
	G()->SetOpacity(100);
}
void HSGame::ResultUpdate()
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
	if (_sub_state == k_INIT)
	{
		_count_show_ads++;
		if (_user._hadRemovedAds)
		{
			_count_show_ads = 0;
		}
		_result_quest_opac = 100;
		_result_scroll_state = 0;
		_result_options_state = 0;
		SettingInit();
		CreditsInit();
		MenuGGPlayInit();
		_result_opac = 50;
		_result_is_close = false;
		_result_button_scroll_speed = 5;
		_result_button_scrolly = 0;
		_result_interface_scrollx = 0;
		for (int i = 0; i < RESULT_NUM_BUTTON; i++)
		{
			int anim = 0;
			_result_button[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER);
			if (i == RESULT_BUTTON_GGPLAY)
			{
				_result_button[i].SetAnim(SPRITEID_HUB_ANDROID, 0);
				_result_button[i].SetPos(0, 0);
			}
			else
			{
				_result_button[i].SetAnim(SPRITEID_HUD_RESULT, anim + i * 4);
				_result_button[i].SetPos(0, _result_button_scrolly);
			}

#if ANDROID_OS
			if (i == RESULT_BUTTON_ARCHI || i == RESULT_BUTTON_LEADERBOARD)
			{
				_result_button[i].SetState(BUTTON_STATE_HIDE);
			}
#else
			if (i == RESULT_BUTTON_GGPLAY)
			{
				_result_button[i].SetState(BUTTON_STATE_HIDE);
			}
#endif

			


		}
		//_quest.SetQuestComplete(_quest._level);
		_sub_state = k_LOOP;
		_quest.AddCurrentValue(CQuest::enumObjective::Objective_4, 1);
#if ANDROID_OS
		_quest.AddCurrentValue(CQuest::enumObjective::Objective_9, 1);
		_quest.AddCurrentValue(CQuest::enumObjective::Objective_19, 1);
#else
		if (_user._typeCharacterUse == 3)
		{
			_quest.AddCurrentValue(CQuest::enumObjective::Objective_9, 1);
		}
		else if (_user._typeCharacterUse == 7)
		{
			_quest.AddCurrentValue(CQuest::enumObjective::Objective_19, 1);
		}
#endif 

		
		Pine_Date_Time localtime;
		localtime.setDateByLocalDate();
		int day_current = localtime.getDay();
		//printf("\n day_current:%d,_user._day_next_login:%lld", day_current, _user._day_next_login);
		if (_user._day_next_login == day_current)
		{
			_user._day_next_login = day_current + 1;
			_quest.AddCurrentValue(CQuest::enumObjective::Objective_12, 1);
		}
		else if (_user._day_next_login < day_current)
		{
			_user._day_next_login = day_current + 1;
			_quest.SetCurrentValue(CQuest::enumObjective::Objective_12, 1);
		}
		_user._countShowRate--;
		PDEBUG("\n_user._countShowRate = %d", _user._countShowRate);
		if (_char_selected != _user._typeCharacterUse)
		{
			_char_selected = _user._typeCharacterUse;
			_char_select_scrollx_tar = _char_selected*SIZE_FLATFORM_WIDTH*1.5f;
			if (_char_select_scrollx_tar >= (SIZE_FLATFORM_WIDTH*1.5f) * 10)
			{
				_char_select_scrollx_tar = (SIZE_FLATFORM_WIDTH*1.5f) * 10;
			}
			_char_select_scroll_state = 1;
		}
		else
		{
			_char_select_scrollx_tar = _char_selected*SIZE_FLATFORM_WIDTH*1.5f;
			_char_select_scrollx = _char_select_scrollx_tar;
		}
		_ads_show_complete = false;
		if (_first_game)
		{
			_result_opac = 0;
			_result_scroll_state = 1;
			_result_interface_scrollx = -k_SCREEN_WIDTH;
			_first_game = false;
		}
		//_dialog.AddDialogPopup(CDialog::enumTypeDialog::COMFIRM_SHOW_SHARE, CDialog::enumTypeTrans::ZOOM_IN);
		
	}

	_char_selecte_opac = 100 + _result_interface_scrollx*0.3f;
	if (_result_scroll_state != 5)
	{
		if (_result_opac < 50)
		{
			_result_opac += 2;
			if (_result_opac >= 50)
			{
				_result_opac = 50;
			}
		}
	}

	if (_setting_scroll_state > 0)
	{
		if (_setting_scroll_state != 5)
		{
			if (_result_opac < 80)
			{
				_result_opac += 3;
				if (_result_opac >= 80)
				{
					_result_opac = 80;
				}
			}
		}
		else
		{
			if (_result_opac > 50)
			{
				_result_opac -= 3;
				if (_result_scroll_state != 5)
				{
					if (_result_opac <= 50)
					{
						_result_opac = 50;
					}
				}
			}
		}
	}
	if (_result_options_state > 0)
	{
		SettingUpdate();
		if (_credits_scroll_state > 0)
		{
			CreditsUpdate();
		}
	}

	if (_game_over_opac <= 0)
	{
		_quest.UpdateQuest();
	}
	if (_quest._status == CQuest::enumQuestStatus::_QUEST_COMPLETE)
	{
		_quest.SetNextQuest();
	}
	if ((_quest._status == CQuest::enumQuestStatus::_QUEST_DOING ||
		_quest._status == CQuest::enumQuestStatus::_QUEST_NONE) && _quest._stateEffect == 0)
	{
		if (_game_over_opac <= 0)
		{
			if (_share_highscore && !_dialog.IsOpening())
			{
				_share_highscore = false;

				_dialog.AddDialogPopup(CDialog::enumTypeDialog::COMFIRM_SHOW_SHARE, CDialog::enumTypeTrans::ZOOM_IN);
			}
			if (_user._countShowRate <= 0 && !_dialog.IsOpening() && !_user._isRating)
			{
				_dialog.AddDialogPopup(CDialog::enumTypeDialog::COMFIRM_SHOW_RATTING, CDialog::enumTypeTrans::ZOOM_IN);
			}
#if defined (MAC_OS)
			if (!_ads_show_complete && GAME()->_device.IsNetworkAvailable() && !_dialog.IsOpening())
			{
				if (_count_show_advideos >= 2 && _count_show_ads >= 5)
				{
					if (PineAdBox::AdsAvailableNum > 0)
					{
						int adboxtype = PineAdBox::HasVideoAds();
						if (adboxtype >= 0)
						{
							PineAdBox::ShowVideoAds(adboxtype);
						}
					}
					_count_show_ads = 0;
					_count_show_advideos = 0;
				}
				else
				{
					if (_count_show_ads >= 5)
					{
						if (PineAdBox::AdsAvailableNum > 0)
						{
							PineAdBox::SetReactiveAdvert();
							PineAdBox::ShowAdvertFullScreen();
							_count_show_advideos++;
							_count_show_ads = 0;
						}
					}

				}
				_ads_show_complete = true;

			}

#endif
		}

		if (_timeShowNotify > 0)
		{

			if (!_dialog.IsOpening())
			{
				_timeShowNotify--;
				if (_timeShowNotify <= 0)
				{
#if defined(MAC_OS)
					RequestRegisterNotificationSetting();
					_timeShowNotify = 0;
#endif
				}
			}
		}
	}
	//printf("\n _result_scroll_state:%d,_result_interface_scrollx:%f", _result_scroll_state, _result_interface_scrollx);
	switch (_result_scroll_state)
	{
	case 0:
		break;
	case 1:
		_result_interface_scrollx += 30;
		if (_result_interface_scrollx >= 20)
		{
			_result_interface_scrollx = 20;
			_result_scroll_state = 2;
		}

		break;
	case 2:
		_result_interface_scrollx -= 4;
		if (_result_interface_scrollx <= -5)
		{
			_result_interface_scrollx = -5;
			_result_scroll_state = 3;
		}
		break;
	case 3:
		_result_interface_scrollx += 1;
		if (_result_interface_scrollx >= 0)
		{
			_result_interface_scrollx = 0;
			_result_scroll_state = 0;
		}
		break;
	case 4:
		_result_interface_scrollx -= _result_button_scroll_speed;
		//_result_button_scrolly += _result_button_scroll_speed;
		_result_button_scroll_speed *= 1.2f;
		if (_result_button_scroll_speed >= 40)
		{
			_result_button_scroll_speed = 40;
		}
		if (_result_interface_scrollx <= -k_SCREEN_WIDTH)
		{
			_result_interface_scrollx = -k_SCREEN_WIDTH;
			_result_scroll_state = 0;
		}
		break;
	case 5:
		if (!_char_selecte_scrolling)
		{
			_result_interface_scrollx -= _result_button_scroll_speed;
			//_result_button_scrolly += _result_button_scroll_speed;
			_result_button_scroll_speed *= 1.2f;
			if (_result_button_scroll_speed >= 40)
			{
				_result_button_scroll_speed = 40;
			}
			if (_result_interface_scrollx <= -k_SCREEN_WIDTH)
			{
				_result_interface_scrollx = -k_SCREEN_WIDTH;
				_result_scroll_state = 0;
				_user._typeCharacterUse = _char_selected;
				_volume_state_change = 2;
				SetState(k_GS_PLAYGAME);
				if (_result_is_close)
				{

					PDEBUG("\n######endgame: num_play_game--> %d", GAME()->_times_play_game);
					Num_PlayGame_GA_logsContent(GAME()->_times_play_game);
				}
			}
			_result_opac -= 5;
			if (_result_opac <= 0)
			{
				_result_opac = 0;
			}
		}
		break;
	default:
		break;
	}



	CharSelectUpdate();
	MenuGGPlayUpdate();
	ResultUpdateTouch();

}
bool HSGame::ResultUpdateTouch()
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
	bool press = false;
	int index = -1;
	bool ret = false;

	if (_game_over_opac > 0 || _menu_ggplay_active || _menu_ggplay_id != -1 || _result_is_close || _result_interface_scrollx != 0 || !_quest._effect_end || _dialog.IsOpening())
	{
		return ret;
	}

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

	for (int i = 0; i < RESULT_NUM_BUTTON; i++)
	{
		if (_result_button[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}



	if (ret)
	{
		if (_result_button[index].NeedAction())
		{
			//printf("\n index = %d",index);
			switch (index)
			{
			case RESULT_BUTTON_PLAY:
			{
#if ANDROID_OS
				if (!GAME()->_async_task_manager._is_login_google)
				{
					GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_IS_LOG_IN_GOOGLE_PLAY);
				}
#endif

				_result_is_close = true;
				_result_scroll_state = 5;
				if (_char_select[_char_selected]._is_lock)
				{
					if (_char_selected != _user._typeCharacterUse)
					{
						_char_selected = _user._typeCharacterUse;
						if (_char_select_scrollx_tar > _char_selected*SIZE_FLATFORM_WIDTH*1.5f)
						{
							_char_select_scroll_state = 5;
						}
						else if (_char_select_scrollx_tar > _char_selected*SIZE_FLATFORM_WIDTH*1.5f)
						{
							_char_select_scroll_state = 1;
						}
						_char_select_scrollx_tar = _char_selected*SIZE_FLATFORM_WIDTH*1.5f;
						if (_char_select_scrollx_tar >= (SIZE_FLATFORM_WIDTH*1.5f) * 10)
						{
							_char_select_scrollx_tar = (SIZE_FLATFORM_WIDTH*1.5f) * 10;
						}
					}
					_char_selecte_scrolling = true;
				}
				else
				{
					_main_char._spriteid = _char_select[_char_selected]._spriteid;
				}
			}
			break;
			case RESULT_BUTTON_ARCHI:
#ifdef MAC_OS
				PineGameCenterController::ShowLeaderAchivement();
#endif
				break;
			case RESULT_BUTTON_LEADERBOARD:
#ifdef MAC_OS
				PineGameCenterController::SubmitMyScore("HH_Leaderboard", _high_score);
				PineGameCenterController::ShowLeaderBoard("HH_Leaderboard");
#endif

				break;
			case RESULT_BUTTON_SETTING:
				_result_options_state = 1;
				_result_scroll_state = 4;
				SettingActive();
				break;
			case RESULT_BUTTON_RIGHT:
				_char_select_scrollx_tar += SIZE_FLATFORM_WIDTH*1.5f;

				if (_char_select_scrollx_tar > (SIZE_FLATFORM_WIDTH*1.5f) * 9)
				{
					_char_select_scrollx_tar = (SIZE_FLATFORM_WIDTH*1.5f) * 9;
				}
				else
				{
					_char_selected++;
				}
				_char_select_scroll_state = 1;
				_char_selecte_scrolling = true;
				break;
			case RESULT_BUTTON_LEFT:
				_char_select_scrollx_tar -= SIZE_FLATFORM_WIDTH*1.5f;
				if (_char_select_scrollx_tar < 0)
				{
					_char_select_scrollx_tar = 0;
				}
				else
				{
					_char_selected--;
				}
				_char_select_scroll_state = 5;
				_char_selecte_scrolling = true;
				break;

			case RESULT_BUTTON_GGPLAY:
				MenuGGPlayActive();

				break;

			default:
				break;
			}
			if (_result_button[index]._state != BUTTON_STATE_LOCK && _result_button[index]._state != BUTTON_STATE_HIDE)
			{
				_result_button[index].SetState(BUTTON_STATE_NORMAL);
			}
		}
	}

	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		for (int touch = 0; touch < RESULT_NUM_BUTTON; touch++)
		{
			int x = _result_button[touch].TZ_GetX() - 20;
			int y = _result_button[touch].TZ_GetY() - 20;
			int w = _result_button[touch].TZ_GetWidth() + 40;
			int h = _result_button[touch].TZ_GetHeight() + 40;
			Button *button = &_result_button[touch];
			if (button->_state == BUTTON_STATE_LOCK || button->_state == BUTTON_STATE_HIDE)
			{
				continue;
			}
			if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT);
						button->_touch_id = TOUCHS(i)->_id;
						press = true;
						PLAY_SFX(SFX_BUTTON);
						//PDEBUG("\n GGPlay touch button index:%d -- TOUCHS(i)->_id:%d", touch,TOUCHS(i)->_id);
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					/*if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT);
						button->_touch_id = TOUCHS(i)->_id;
						press = true;
					}*/
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
				/*else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
					}
				}*/
			}
		}

	}
	return press;
}
#pragma region MenuGGPlay
void HSGame::MenuGGPlayActive() {
	_menu_ggplay_speed = 34;
	_menu_ggplay_active = true;
	_menu_ggplay_state = 1;
	_menu_ggplay_isclose = false;
	_menu_ggplay_scroll_y = 0;
	_menu_ggplay_id = -1;
	_menu_ggplay_index = -1;
}
void HSGame::MenuGGPlayInit() {
	_menu_ggplay_isclose = false;
	_menu_ggplay_scroll_y = 0;
	_menu_ggplay_active = false;
	_menu_ggplay_state = 0;
	_menu_ggplay_speed = 34;
	_menu_ggplay_id = -1;
	_menu_ggplay_index = -1;
	PDEBUG("\nggplay init");
	for (int i = 0; i < RESULT_NUM_BUTTON_GGPLAY; i++)
	{
		_menu_ggplay[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER, 0);
		_menu_ggplay[i].SetAnim(SPRITEID_HUB_ANDROID, 4 * i + 4);
		_menu_ggplay[i].SetPos(0, 0);
	}

}
void HSGame::MenuGGPlayRender(float x, float y, int opac) {
	if (!_menu_ggplay_active)
	{
		return;
	}
	G()->SetOpacity(100);
	G()->SetColor(0xa90f1d24);
	//G()->FillRect(_point_o.X, _point_o.Y, _screen_width, _screen_height, true);
	G()->SetColor(0xf5193b33);

	int fill_h = MENU_GGPlay_HEIGHT;
	//G()->FillRect(_point_o.X, _point_o.Y, _screen_width, fill_h, true);
	GET_SPRITE(SPRITEID_HUB_ANDROID)->DrawFrame(G(), 10, x, y + MENU_GGPlay_HEIGHT - _menu_ggplay_scroll_y - GAME()->_banner_ads_scrolly);

	for (int i = 0; i < RESULT_NUM_BUTTON_GGPLAY; i++)
	{
		int offsetX = 0;
		int offsetY = 0;

		_menu_ggplay[i].SetPos(offsetX + x, offsetY + y + MENU_GGPlay_HEIGHT - _menu_ggplay_scroll_y - GAME()->_banner_ads_scrolly);
		_menu_ggplay[i].Render(G());
		_menu_ggplay[i].Update();

	}
	G()->SetOpacity(opac);
}
void HSGame::MenuGGPlayUpdate() {
	if (!_menu_ggplay_isclose)
	{

		switch (_menu_ggplay_state)
		{
		case 0:
			break;
		case 1:
			if (_menu_ggplay_scroll_y < MENU_GGPlay_HEIGHT + 50)
			{
				if (_menu_ggplay_scroll_y >= MENU_GGPlay_HEIGHT + 10)
				{
					_menu_ggplay_speed *= 0.43f;
					if (_menu_ggplay_speed <= 2)
					{
						_menu_ggplay_speed = 2;
					}
				}
				printf("\n _menu_ext_scroll_speed = %f", _menu_ggplay_speed);
				_menu_ggplay_scroll_y += _menu_ggplay_speed;
				if (_menu_ggplay_scroll_y >= MENU_GGPlay_HEIGHT + 50)
				{
					_menu_ggplay_state = 2;
					_menu_ggplay_speed = 15;
				}
			}
			break;
		case 2:
			if (_menu_ggplay_scroll_y > MENU_GGPlay_HEIGHT)
			{
				int speed = (CMath::ABSOLUTE_VALUE(_menu_ggplay_scroll_y - MENU_GGPlay_HEIGHT));

				if (speed <= 40)
				{
					_menu_ggplay_speed *= 0.68f;
				}
				if (_menu_ggplay_speed <= 0.2)
				{
					_menu_ggplay_speed = 0.2f;
				}
				printf("\n _menu_ext_scroll_speed = %f", _menu_ggplay_speed);
				_menu_ggplay_scroll_y -= _menu_ggplay_speed;
				if (_menu_ggplay_scroll_y <= MENU_GGPlay_HEIGHT)
				{
					_menu_ggplay_state = 3;
					_menu_ggplay_scroll_y = MENU_GGPlay_HEIGHT;
				}
			}
			break;
		case 3:
			break;
		default:
			break;
		}

	}
	else
	{
		if (_menu_ggplay_scroll_y > 0)
		{
			_menu_ggplay_scroll_y -= 10;
			if (_menu_ggplay_scroll_y <= 0)
			{
				_menu_ggplay_scroll_y = 0;

			}
		}
		if (_menu_ggplay_scroll_y <= 0 && _menu_ggplay_index == -1)
		{
			_menu_ggplay_active = false;
		}
	}

	if (!MenuGGPlayUpdateTouch()) {
		if (!_dialog.IsOpening())
		{
			for (int i = GAME()->_num_touches - 1; i >= 0; i--)
			{
				int x = _point_o.X;
				int y = _point_o.Y;
				int w = _screen_width;
				int h = k_SCREEN_HEIGHT - MENU_GGPlay_HEIGHT;
				if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
				{
					if (TOUCHS(i)->_state == TOUCH_DOWN && _menu_ggplay_active)
					{
						if (_menu_ggplay_index == -1)
						{
							if (!_menu_ggplay_isclose)
							{
								_menu_ggplay_isclose = true;
								TOUCHS(i)->_state = TOUCH_DRAG;
							}
							_menu_ggplay_index = TOUCHS(i)->_id;
						}

					}
					else if (TOUCHS(i)->_state == TOUCH_UP)
					{
						if (_menu_ggplay_index == TOUCHS(i)->_id)
						{
							_menu_ggplay_index = -1;
						}

					}
				}
				else
				{
					if (TOUCHS(i)->_state == TOUCH_UP)
					{
						if (_menu_ggplay_id == TOUCHS(i)->_id)
						{
							_menu_ggplay_id = -1;
						}
					}

					if (TOUCHS(i)->_state == TOUCH_UP)
					{
						if (_menu_ggplay_index == TOUCHS(i)->_id)
						{
							_menu_ggplay_index = -1;
						}
					}
				}
			}

		}
	}
}
bool HSGame::MenuGGPlayUpdateTouch() {
	bool press = false;
	HSGame *game = (HSGame*)GAME()->_current_game;
	if (_menu_ggplay_isclose || _menu_ggplay_scroll_y < MENU_GGPlay_HEIGHT)
	{
		return false;
	}
	bool ret = false;
	int index = -1;
	if (_dialog.IsOpening())
	{
		return ret;
	}
	for (int i = 0; i < RESULT_NUM_BUTTON_GGPLAY; i++)
	{
		if (_menu_ggplay[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}

#if !defined (MAC_OS)
	if (GAME()->_isBackkeyPress && _menu_ggplay_active)
	{
		_menu_ggplay_isclose = true;
		GAME()->_isBackkeyPress = false;
		return true;
	}
#endif


	if (ret)
	{
		if (_menu_ggplay[index].NeedAction())
		{
			switch (index)
			{
			case RESULT_BUTTON_GGPLAY_SYNC:
				PDEBUG("\nRESULT_BUTTON_GGPLAY_SYNC");
				game->GooglePlaySynDriver();
				
				break;

			case RESULT_BUTTON_GGPLAY_LEADERBOARD:
				PDEBUG("\nRESULT_BUTTON_GGPLAY_LEADERBOARD");
				game->GooglePlayShowLeaderBoard();
				break;

			case RESULT_BUTTON_GGPLAY_ARCHIVEMENT:
				PDEBUG("\nRESULT_BUTTON_GGPLAY_ARCHIVEMENT");
				game->GooglePlayShowAchievement();
				break;

			default:
				break;
			}
			if (_menu_ggplay[index]._state != BUTTON_STATE_LOCK && _menu_ggplay[index]._state != BUTTON_STATE_HIDE)
			{
				_menu_ggplay[index].SetState(BUTTON_STATE_NORMAL);
			}
		}

	}
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		for (int touch = 0; touch < RESULT_NUM_BUTTON_GGPLAY; touch++)
		{
			if (_menu_ggplay[touch]._state == BUTTON_STATE_LOCK || _menu_ggplay[touch]._state == BUTTON_STATE_HIDE)
			{
				continue;
			}
			int x = _menu_ggplay[touch].TZ_GetX();
			int y = _menu_ggplay[touch].TZ_GetY();
			int w = _menu_ggplay[touch].TZ_GetWidth();
			int h = _menu_ggplay[touch].TZ_GetHeight();
			Button *button = &_menu_ggplay[touch];

			if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x - 5, y - 5, w + 5, h + 5))
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT);
						button->_touch_id = TOUCHS(i)->_id;
						TOUCHS(i)->_state = TOUCH_DRAG;
						press = true;
						PLAY_SFX(SFX_BUTTON);
						//PDEBUG("\n Menu GGPlay touch button");
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT);
						button->_touch_id = TOUCHS(i)->_id;
						//TOUCHS(i)->_state = TOUCH_DRAG;
						press = true;
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						//button->SetState(BUTTON_STATE_NORMAL);
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
#pragma endregion
#pragma region SETTING
void HSGame::SettingInit()
{
	_setting_scrollx = k_SCREEN_WIDTH;
	_setting_scroll_state = 0;
	for (int i = 0; i < SETTING_NUM_BUTTON; i++)
	{
		int anim = 0;
		if (i == SETTING_BUTTON_SOUND)
		{
			anim = 4;
			if (_game->_soundEnable)
			{
				anim = 0;
			}
		}
		else if (i == SETTING_BUTTON_MUSIC)
		{
			anim = 12;
			if (_game->_musicEnable)
			{
				anim = 8;
			}
		}
		else
		{
			anim = 16 + (i - SETTING_BUTTON_TW) * 4;
		}



		_setting_button[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER);
		if (i == SETTING_BUTTON_TW || i == SETTING_BUTTON_IAPS)
		{
			_setting_button[i].SetState(BUTTON_STATE_HIDE);
		}
		_setting_button[i].SetAnim(SPRITEID_HUD_SETTING, anim);
		if (i == SETTING_BUTTON_ADS)
		{
			if (_user._hadRemovedAds)
			{
				_setting_button[i].SetState(BUTTON_STATE_LOCK);
			}
		}
	}
}
void HSGame::SettingActive()
{
	_setting_scroll_state = 1;
	_setting_scrollx = k_SCREEN_WIDTH;
}
void HSGame::SettingRender(float x, float y)
{
	GET_SPRITE(SPRITEID_HUD_SETTING)->DrawFrame(_grap, 30, _setting_scrollx + x, y);
	if (_user._hadRemovedAds)
	{
		if (_setting_button[SETTING_BUTTON_ADS]._state != BUTTON_STATE_LOCK)
		{
			_setting_button[SETTING_BUTTON_ADS].SetState(BUTTON_STATE_LOCK);
		}
	}
	for (int i = 0; i < SETTING_NUM_BUTTON; i++)
	{
		if (i == SETTING_BUTTON_TW || i == SETTING_BUTTON_IAPS)
		{
			continue;
		}

		if (i == SETTING_BUTTON_ICLOUD)
		{
			y = y - _setting_button[i].TZ_GetHeight();
			continue;
		}
		if (i == SETTING_BUTTON_BACK)
		{
			if (_setting_scroll_state != 4 && _setting_scroll_state < 6)
			{
				_setting_button[i].SetPos(x + _point_o.X, y + _setting_scrollx*0.3f - GAME()->_banner_ads_scrolly*0.5f);
			}
		}
		else if (i == SETTING_BUTTON_FB)
		{
			_setting_button[i].SetPos(_setting_scrollx + x + GET_SPRITE(SPRITEID_HUD_SETTING)->GetFModuleWidth(4, 0) *2*0.42f,y - 0.42f*GET_SPRITE(SPRITEID_HUD_SETTING)->GetFModuleCenterY(4, 0));
		}
		else if (i == SETTING_BUTTON_SOUND || i == SETTING_BUTTON_MUSIC)
		{
			_setting_button[i].SetPos(_setting_scrollx + x - 2*0.42f*GET_SPRITE(SPRITEID_HUD_SETTING)->GetFModuleWidth(4, 0), y);
		}
		else if (i == SETTING_BUTTON_CONTACT || i == SETTING_BUTTON_CREDITS)
		{
			_setting_button[i].SetPos(_setting_scrollx + x, y - 0.75f*GET_SPRITE(SPRITEID_HUD_SETTING)->GetFModuleCenterY(4, 0));
		}
		else
		{
			_setting_button[i].SetPos(_setting_scrollx + x, y - 0.42f*GET_SPRITE(SPRITEID_HUD_SETTING)->GetFModuleCenterY(4, 0));
		}
#if defined(MAC_OS)
		if (!Device::IsNetworkAvailable())
		{
			if (i >= SETTING_BUTTON_TW && i <= SETTING_BUTTON_CONTACT)
			{
				if (_setting_button[i]._state != BUTTON_STATE_LOCK)
				{
					_setting_button[i].SetState(BUTTON_STATE_LOCK);
				}
			}
		}
		else
		{
			if (i >= SETTING_BUTTON_TW && i <= SETTING_BUTTON_CONTACT)
			{
				if (i == SETTING_BUTTON_ADS)
				{
					if (_user._hadRemovedAds)
					{
						if (_setting_button[SETTING_BUTTON_ADS]._state != BUTTON_STATE_LOCK)
						{
							_setting_button[SETTING_BUTTON_ADS]._state = BUTTON_STATE_LOCK;
						}
					}
					else
					{
						if (_setting_button[i]._state == BUTTON_STATE_LOCK)
						{
							_setting_button[i].SetState(BUTTON_STATE_NORMAL);
						}
					}
				}
				else
				{
					if (_setting_button[i]._state == BUTTON_STATE_LOCK)
					{
						_setting_button[i].SetState(BUTTON_STATE_NORMAL);
					}
				}

		}

	}
#endif

		_setting_button[i].Render(_grap);
		_setting_button[i].Update();
}
}
void HSGame::SettingUpdate()
{
	switch (_setting_scroll_state)
	{
	case 1:
		_setting_scrollx -= 30;
		if (_setting_scrollx <= -20)
		{
			_setting_scrollx = -20;
			_setting_scroll_state = 2;
		}
		break;
	case 2:
		_setting_scrollx += 4;
		if (_setting_scrollx >= 5)
		{
			_setting_scrollx = 5;
			_setting_scroll_state = 3;
		}
		break;
	case 3:
		_setting_scrollx -= 1;
		if (_setting_scrollx <= 0)
		{
			_setting_scrollx = 0;
			_setting_scroll_state = 0;
		}
		break;
	case 4:
		_setting_scrollx -= 30;
		if (_setting_scrollx <= -k_SCREEN_WIDTH)
		{
			_setting_scrollx = -k_SCREEN_WIDTH;
		}
		break;
	case 5:
		_setting_scrollx += 30;
		if (_setting_scrollx >= k_SCREEN_WIDTH)
		{
			_setting_scrollx = k_SCREEN_WIDTH;
			_result_options_state = 0;
		}
		break;
	case 6:
		_setting_scrollx += 30;
		if (_setting_scrollx >= 20)
		{
			_setting_scrollx = 20;
			_setting_scroll_state = 7;
		}
		break;
	case 7:
		_setting_scrollx -= 4;
		if (_setting_scrollx <= -5)
		{
			_setting_scrollx = -5;
			_setting_scroll_state = 8;
		}
		break;
	case 8:
		_setting_scrollx += 1;
		if (_setting_scrollx >= 0)
		{
			_setting_scrollx = 0;
		}
		break;
	default:
		break;
	}

	SettingUpdateTouch();
}
bool HSGame::SettingUpdateTouch()
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
	bool press = false;
	int index = -1;
	bool ret = false;
	if (_dialog.IsOpening())
	{
		return ret;
	}
	for (int i = 0; i < SETTING_NUM_BUTTON; i++)
	{
		if (_setting_button[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}

#if !defined (MAC_OS)
	if (GAME()->_isBackkeyPress)
	{
		PDEBUG("\nfire close credit");
		if (_credits_scroll_state != 0)
		{
			_setting_scroll_state = 6;
			_credits_scroll_state = 4;
		}
		else
		{
			_setting_scroll_state = 5;
			_result_scroll_state = 1;
		}
		GAME()->_isBackkeyPress = false;
		return true;
	}
#endif

	if (ret)
	{
		if (_setting_button[index].NeedAction())
		{
			//printf("\n index = %d",index);
			switch (index)
			{
			case SETTING_BUTTON_SOUND:
			{
				int anim = 0;
				GAME()->_soundEnable = !GAME()->_soundEnable;
				if (!GAME()->_soundEnable)
				{
					anim = 4;
				}
				_setting_button[index].SetAnim(SPRITEID_HUD_SETTING, anim + index * 4);
				_setting_button[index].SetPos(0, 0);
			}
			break;
			case SETTING_BUTTON_MUSIC:
			{
				int anim = 4;
				GAME()->_musicEnable = !GAME()->_musicEnable;
				if (!GAME()->_musicEnable)
				{
					anim = 8;
					_game->PauseCurrentMusic();
				}
				else
				{
					_game->LoadMusic(BGM_ALLGAME);
					_game->PlayCurrentMusic(-1);
					_volume = 0;
					_volume_state_change = 1;
				}
				_setting_button[index].SetAnim(SPRITEID_HUD_SETTING, anim + index * 4);
				_setting_button[index].SetPos(0, 0);
			}
			break;
			case SETTING_BUTTON_TW:
#ifdef MAC_OS
				//game->_notShowPineAdds = true;
				if (Device::IsNetworkAvailable())
				{
					PineSocial::OpenURL(TWITTER_URL);
				}
				else
				{
					//game->_user.NetWorkError();
				}
#elif ANDROID_OS
				if (Device::IsNetworkAvailable())
				{
					PineSocial::OpenURL(TWITTER_URL);
				}
				else
				{
					game->_user.NetWorkError();
				}

#endif

				break;
			case SETTING_BUTTON_FB:
#ifdef MAC_OS 
				//game->_notShowPineAdds = true;
				if (Device::IsNetworkAvailable())
				{
					PineSocial::OpenURL(FACEBOOK_URL);
				}
				else
				{
					//game->_user.NetWorkError();
				}
#elif ANDROID_OS
				if (Device::IsNetworkAvailable())
				{
					PineSocial::OpenURL(FACEBOOK_URL);
				}
				else
				{
					game->_user.NetWorkError();
				}
#endif
				_show_ads_interrupt = false;
				break;
			case SETTING_BUTTON_ADS:
#if defined(MAC_OS)
				PurchaseProduct("HH_NoAds");
#elif ANDROID_OS
				if (Device::IsNetworkAvailable())
				{
					PurchaseProduct("hh_no_ads_1_99");
					game->_isWaiting = true;
					game->_notShowPineAdds = true;
					game->_not_showads_interrupt = true;
				}
				else
				{
					game->_user.NetWorkError();
				}
#endif
				_show_ads_interrupt = false;
				break;
			case SETTING_BUTTON_IAPS:
#if defined(MAC_OS)
				IapRestore();
#elif ANDROID_OS
				if (Device::IsNetworkAvailable())
				{
					PurchaseProduct("hh_no_ads_1_99");
					game->_isWaiting = true;
					game->_notShowPineAdds = true;
					game->_not_showads_interrupt = true;
				}
				else
				{
					game->_user.NetWorkError();
				}
#endif
				_show_ads_interrupt = false;
				break;
			case SETTING_BUTTON_ICLOUD:
#ifdef MAC_OS
				_isWaiting = true;
				if (Device::IsNetworkAvailable())
				{

					_user.CheckIcloud(true);
				}
				else
				{
					_user.NetWorkError();
				}
#endif
				break;
			case SETTING_BUTTON_CONTACT:
#ifdef MAC_OS
				//game->_notShowPineAdds = true;
				if (Device::IsNetworkAvailable())
				{
					SendMailSupport("", "[Higher Higher!]");
				}
				else
				{
					//game->_user.NetWorkError();
				}
#elif ANDROID_OS
				sprintf(GAME()->_async_task_manager._send_email_title, "%s", "[Higher Higher! - Android]");
				sprintf(GAME()->_async_task_manager._send_email_body, "%s", "");
				GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_SEND_EMAIL_INTENT);
#endif
				_show_ads_interrupt = false;
				break;
			case SETTING_BUTTON_CREDITS:
				_setting_scroll_state = 4;
				CreditsActive();
				break;
			case SETTING_BUTTON_BACK:
				PDEBUG("\n_credits_scroll_state: %d", _credits_scroll_state);
				if (_credits_scroll_state != 0)
				{
					_setting_scroll_state = 6;
					_credits_scroll_state = 4;
				}
				else
				{
					_setting_scroll_state = 5;
					_result_scroll_state = 1;
				}

				break;
			default:
				break;
				}
			if (_setting_button[index]._state != BUTTON_STATE_LOCK && _setting_button[index]._state != BUTTON_STATE_HIDE)
			{
				_setting_button[index].SetState(BUTTON_STATE_NORMAL);
			}
			}
			}

	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		for (int touch = 0; touch < SETTING_NUM_BUTTON; touch++)
		{
			int x = _setting_button[touch].TZ_GetX() - 20;
			int y = _setting_button[touch].TZ_GetY() - 20;
			int w = _setting_button[touch].TZ_GetWidth() + 40;
			int h = _setting_button[touch].TZ_GetHeight() + 40;
			Button *button = &_setting_button[touch];
			if (button->_state == BUTTON_STATE_LOCK || button->_state == BUTTON_STATE_HIDE)
			{
				continue;
			}
			if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT);
						button->_touch_id = TOUCHS(i)->_id;
						press = true;
						PLAY_SFX(SFX_BUTTON);
						PDEBUG("\n Setting touch button");
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
#pragma endregion
#pragma region CREDITS
void HSGame::CreditsInit()
{
	_credits_scrollx = k_SCREEN_WIDTH;
	_credits_scroll_state = 0;
}
void HSGame::CreditsActive()
{
	_credits_scrollx = k_SCREEN_WIDTH;
	_credits_scroll_state = 1;
}
void HSGame::CreditsRender(float x, float y)
{
	GET_SPRITE(SPRITEID_HUD_SETTING)->DrawFrame(_grap, 29, _credits_scrollx + x, y);

}
void HSGame::CreditsUpdate()
{
	switch (_credits_scroll_state)
	{
	case 1:
		_credits_scrollx -= 30;
		if (_credits_scrollx <= -20)
		{
			_credits_scrollx = -20;
			_credits_scroll_state = 2;
		}
		break;
	case 2:
		_credits_scrollx += 4;
		if (_credits_scrollx >= 5)
		{
			_credits_scrollx = 5;
			_credits_scroll_state = 3;
		}
		break;
	case 3:
		_credits_scrollx -= 1;
		if (_credits_scrollx <= 0)
		{
			_credits_scrollx = 0;
			_credits_scroll_state = 5;
		}
		break;
	case 4:
		_credits_scrollx += 30;
		if (_credits_scrollx >= k_SCREEN_WIDTH)
		{
			_credits_scrollx = k_SCREEN_WIDTH;
			_credits_scroll_state = 0;
		}
		break;
	default:
		break;
	}
}
#pragma endregion
