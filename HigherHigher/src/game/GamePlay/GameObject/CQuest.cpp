#include "pch.h"
#include "../../GameCore/CGame.h"

CQuest::CQuest()
{

}
CQuest::~CQuest()
{

}

void CQuest::SetCurrentValue(int lvl, int value)
{
	int level = -1;
	switch (_currentQuest)
	{
	case Objective_1:
	case Objective_5:
	case Objective_10:
	case Objective_15:
	case Objective_18:
	case Objective_22:
#if ANDROID_OS
	case Objective_24:
#endif
		switch (lvl)
		{
		case Objective_1:
		case Objective_5:
		case Objective_10:
		case Objective_15:
		case Objective_18:
		case Objective_22:
			level = lvl;
			break;
		default:
			break;
		}
		break;
	case Objective_2:
	case Objective_7:
	case Objective_13:
	case Objective_17:
	case Objective_23:
		switch (lvl)
		{
		case Objective_2:
		case Objective_7:
		case Objective_13:
		case Objective_17:
		case Objective_23:
			level = lvl;
			break;
		default:
			break;
		}
		break;
	case Objective_3:
		switch (lvl)
		{
		case Objective_3:
			level = lvl;
			break;
		default:
			break;
		}
		break;
	case Objective_4:
	case Objective_11:
	case Objective_16:
	case Objective_25:
		switch (lvl)
		{
		case Objective_4:
		case Objective_11:
		case Objective_16:
		case Objective_25:
			level = lvl;
			break;
		default:
			break;
		}
		break;

	case Objective_6:
	case Objective_14:
	case Objective_21:
		switch (lvl)
		{
		case Objective_6:
		case Objective_14:
		case Objective_21:
			level = lvl;
			break;
		default:
			break;
		}
		break;
	case Objective_8:
	case Objective_20:
		switch (lvl)
		{
		case Objective_8:
		case Objective_20:
			level = lvl;
			break;
		default:
			break;
		}
		break;
#if !defined ANDROID_OS
	case Objective_9:
	case Objective_24:
		switch (lvl)
		{
		case Objective_9:
		case Objective_24:
			level = lvl;
			break;
		default:
			break;
		}
		break;
	case Objective_19:
		switch (lvl)
		{
		case Objective_19:
			level = lvl;
			break;
		default:
			break;
		}
		break;
#endif
	case Objective_12:
		switch (lvl)
		{
		case Objective_12:
			level = lvl;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	if (level != -1 && _status == CQuest::enumQuestStatus::_QUEST_DOING)
	{
		if (_currentValue != -1)
		{
			_currentValue = value;
		}
		_detaPer = 0;
		_newPercent = (_currentValue / (float)_maxValue) * 100;
		if (_newPercent > 100)
		{
			_newPercent = 100;
		}
		if (_percent < _newPercent)
		{
			_percent = _newPercent;
			if (_perScroll < _percent)
			{
				_detaPer = (_percent - _perScroll) / 40.0f;
			}
		}
        else if(_percent > _newPercent)
        {
            _percent = _newPercent;
            _perScroll = _percent;
        }
	}
	switch (_currentQuest)
	{
	case Objective_1:
	case Objective_5:
	case Objective_10:
	case Objective_15:
	case Objective_18:
	case Objective_22:
	case Objective_2:
	case Objective_7:
	case Objective_13:
	case Objective_17:
	case Objective_23:
		if (_percent < 100)
		{
			_percent = 0;
		}
		//_perScroll = _percent;
		break;
	default:
		break;
	}
}
void CQuest::AddCurrentValue(int lvl, int value)
{
	int level = -1;
	switch (_currentQuest)
	{
	case Objective_1:
	case Objective_5:
	case Objective_10:
	case Objective_15:
	case Objective_18:
	case Objective_22:
		switch (lvl)
		{
		case Objective_1:
		case Objective_5:
		case Objective_10:
		case Objective_15:
		case Objective_18:
		case Objective_22:
#if ANDROID_OS
		case Objective_24:
#endif
			level = lvl;
			break;
		default:
			break;
		}
		break;
	case Objective_2:
	case Objective_7:
	case Objective_13:
	case Objective_17:
	case Objective_23:
		switch (lvl)
		{
		case Objective_2:
		case Objective_7:
		case Objective_13:
		case Objective_17:
		case Objective_23:
			level = lvl;
			break;
		default:
			break;
		}
		break;
	case Objective_3:
		switch (lvl)
		{
		case Objective_3:
			level = lvl;
			break;
		default:
			break;
		}
		break;
	case Objective_4:
	case Objective_11:
	case Objective_16:
	case Objective_25:
		switch (lvl)
		{
		case Objective_4:
		case Objective_11:
		case Objective_16:
		case Objective_25:
			level = lvl;
			break;
		default:
			break;
		}
		break;

	case Objective_6:
	case Objective_14:
	case Objective_21:
		switch (lvl)
		{
		case Objective_6:
		case Objective_14:
		case Objective_21:
			level = lvl;
			break;
		default:
			break;
		}
		break;
	case Objective_8:
	case Objective_20:
		switch (lvl)
		{
		case Objective_8:
		case Objective_20:
			level = lvl;
			break;
		default:
			break;
		}
		break;
#if !defined ANDROID_OS
	case Objective_9:
	case Objective_24:
		switch (lvl)
		{
		case Objective_9:
		case Objective_24:
			level = lvl;
			break;
		default:
			break;
		}
		break;
	case Objective_19:
		switch (lvl)
		{
		case Objective_19:
			level = lvl;
			break;
		default:
			break;
		}
		break;
#endif
	case Objective_12:
		switch (lvl)
		{
		case Objective_12:
			level = lvl;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	if (/*lvl == level && */level != -1 && _status == CQuest::enumQuestStatus::_QUEST_DOING)
	{
		_currentValue += value;
		_detaPer = 0;
		_newPercent = (_currentValue / (float)_maxValue) * 100;
		if (_newPercent > 100)
		{
			_newPercent = 100;
		}
		if (_percent < _newPercent)
		{
			_percent = _newPercent;
			if (_perScroll < _percent)
			{
				_detaPer = (_percent - _perScroll) / 40.0f;
			}
		}
	}
	switch (_currentQuest)
	{
	case Objective_1:
	case Objective_5:
	case Objective_10:
	case Objective_15:
	case Objective_18:
	case Objective_22:
	case Objective_2:
	case Objective_7:
	case Objective_13:
	case Objective_17:
	case Objective_23:
		if (_percent < 100)
		{
			_percent = 0;
		}
		break;
	default:
		break;
	}
	//printf("\n _perScroll:%f,_percent:%d", _perScroll, _percent);
}
void CQuest::LoadInfoQuest(int lvl)
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
	int param_name = CGameDesign::FileAchievement::CSheetObjective::name;
	int param_des = CGameDesign::FileAchievement::CSheetObjective::Description;
	int param_GC = CGameDesign::FileAchievement::CSheetObjective::GC_ID;
	int param_GGPLAY = CGameDesign::FileAchievement::CSheetObjective::GGPLAY_ID;
	int param_value = CGameDesign::FileAchievement::CSheetObjective::value;
	sprintf(_name, "%s", game->GetDesignAchiveString(_currentQuest,param_name));
	sprintf(_description, "%s", game->GetDesignAchiveString(_currentQuest, param_des));
	sprintf(_gcID, "%s", game->GetDesignAchiveString(_currentQuest, param_GC));
	sprintf(_gplayID, "%s", game->GetDesignAchiveString(_currentQuest, param_GGPLAY));
	_maxValue = game->GetDesignAchiveInt(_currentQuest,param_value);
	//sprintf(_description, "%s", game->GetDesignAchiveString(23, param_des));
	_sizeText = 1.0f;
	float ww = _game->GetSpriteVer(SPRITEID_FONT_MISSION_01)->GetWidthAnsiText(_description)*_sizeText;
	if (ww > 230)
	{
		_sizeText = 1.0 - 0.7*(ww - 200) / (float)(450 - 200);
	}
	printf("\n %d ww = %f,_sizeText = %f", _currentQuest + 1, ww, _sizeText);
}

float _zooZoomQuest;
void CQuest::InitQuest()
{	
	
	_game = GAME();
	_gp = G();
	_state = 1;
	_stateEffect = 0;
	_posX = 0;
	_posY = k_SCREEN_HEIGHT / 2;
	_offDescX = 0;
	_zoomCheck = 1;
	_opaCheck = 100;
	_zoomLevel = 1; 
	_zoomMulti = 1;
	_offNextX = 0;
	_offNameY = 0;
	_zooZoomQuest = 1;
	
	if (_status == CQuest::enumQuestStatus::_QUEST_WAITE)
	{
		_status = CQuest::enumQuestStatus::_QUEST_DOING;
	}

	if (_status == CQuest::enumQuestStatus::_QUEST_NONE)
	{
		_offNameY = 60;
	}
	else
	{
		LoadInfoQuest(_currentQuest);
	}
	HSGame* game = (HSGame*)(GAME()->_current_game);
	
	if (_level > game->_game_design.Achievement.SheetObjective.int32Value._totalData - 1)
	{
		_level = game->_game_design.Achievement.SheetObjective.int32Value._totalData - 1;
	}
	int param_xscore = CGameDesign::FileAchievement::CSheetMultiplier::multi;
	_multi = game->GetDesignMultiScoreInt(_level, param_xscore);
	//_perScroll = _percent;
}

void CQuest::NewValueQuest()
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
	_currentValue = 0;
	switch (_currentQuest)
	{
	case Objective_1:
	case Objective_5:
	case Objective_10:
	case Objective_15:
	case Objective_18:
	case Objective_22:
#if ANDROID_OS
	case Objective_24:
#endif
		if (_currentValue != -1)
		{
			_currentValue = 0;
		}
		
		break;
	case Objective_2:
	case Objective_7:
	case Objective_13:
	case Objective_17:
	case Objective_23:
		if (_currentValue != -1)
		{
			_currentValue = 0;
		}
		break;
	case Objective_3:
		break;
	case Objective_4:
	case Objective_11:
	case Objective_16:
	case Objective_25:
		if (_currentValue != -1)
		{
			_currentValue = 0;
		}
		break;

	case Objective_6:
	case Objective_14:
	case Objective_21:
		if (_currentValue != -1)
		{
			_currentValue = 0;
		}
		break;

	
	case Objective_8:
	case Objective_20:
		if (_currentValue != -1)
		{
			_currentValue = 0;
		}
		break;
#if !defined ANDROID_OS
	case Objective_9:
	case Objective_24:
		if (_currentValue != -1)
		{
			_currentValue = 0;
		}
		break;
	case Objective_19:
		if (_currentValue != -1)
		{
			_currentValue = 0;
		}
		break;
#endif
	case Objective_12:

		break;
	default:
		break;
	}
	
	///////
	//printf("\n new value _perScroll:%f,_percent:%d", _perScroll, _percent);
	_percent = (_currentValue / (float)_maxValue) * 100;	
	if (_percent > 100)
	{
		_percent = 100;
	}
	_perScroll = _percent;
}
void CQuest::SetNextQuest()
{
	_status = CQuest::enumQuestStatus::_QUEST_WAITE;
	_stateEffect = 1;
	_offDescX = 0;
}
int CQuest::GetCurrentMulti()
{	
	return _multi;
}
void CQuest::RenderQuest(float x, float y, int opacity)
{
	HSGame *game = (HSGame*)(_game->_current_game);
	//printf("\n _state:%d -- _offNextX:%d,_posX:%f", _state, _offNextX, _posX);
	//printf("\n render _perScroll:%f,_percent:%d", _perScroll, _percent);
	if (_state != 0)
	{
		y -= 5;
		_gp->SetOpacity(opacity);

		int centerX = k_SCREEN_WIDTH/2 + x + _posX + _offNextX;
		int centerY = _game->GetSpriteVer(SPRITEID_HUD_RESULT)->GetFModuleCenterY(8, 0) + y;
		PineMatrix4 zoom = CMath::Matrix4Scale2D(_zooZoomQuest, _zooZoomQuest, centerX, centerY);
		if (_status != CQuest::enumQuestStatus::_QUEST_NONE)
		{
			_gp->SetTransform(CMath::Matrix4ToParam(zoom));
			_game->GetSpriteVer(SPRITEID_HUD_RESULT)->DrawFrame(_gp, 8, x + _posX + _offNextX, y);


			float clipX = _game->GetSpriteVer(SPRITEID_HUD_RESULT)->GetFModuleX(9, 1) + _posX + _offNextX - 1;
			float clipY = _game->GetSpriteVer(SPRITEID_HUD_RESULT)->GetFModuleY(9, 0) - 10;
			float clipW = (_game->GetSpriteVer(SPRITEID_HUD_RESULT)->GetFrameWidth(9)+4)*_perScroll*0.01;
			float clipH = _game->GetSpriteVer(SPRITEID_HUD_RESULT)->GetFrameHeight(9) + 20;
            //printf("\n _perScroll:%f -- clipW:%f",_perScroll,clipW);
			HSGame::SetTranformRect(zoom, clipX, clipY, clipW, clipH, clipX, clipY, clipW, clipH);
			_gp->SetClip(x + clipX, y + clipY, clipW, clipH);
			_game->GetSpriteVer(SPRITEID_HUD_RESULT)->DrawFrame(_gp, 9, x + _posX + _offNextX, y);
			_gp->ClearClip();
			_gp->ClearTransform();
		}

		if (_status != CQuest::enumQuestStatus::_QUEST_DOING && (_stateEffect > 1 &&  _stateEffect < 11))
		{			
			int checkX = _game->GetSpriteVer(SPRITEID_HUD_RESULT)->GetFModuleCenterX(10, 0) + x + _posX + _offNextX;
			int checkY= _game->GetSpriteVer(SPRITEID_HUD_RESULT)->GetFModuleCenterY(10, 0) + y;

			_gp->SetOpacity(_opaCheck* opacity / 100.0f);
			_gp->SetScale(_zoomCheck, _zoomCheck, checkX, checkY);
			_gp->SetColorMask(0x00ff23);
			_game->GetSpriteVer(SPRITEID_HUD_RESULT)->DrawFrame(_gp, 10, x + _posX + _offNextX, y);
			_gp->ClearColorMask();
			_gp->ClearScale();
		}
		_gp->SetOpacity(opacity);

		//level quest
		int charspacing = _game->GetSpriteVer(SPRITEID_FONT_LEVEL_01)->_char_spacing;
		_game->GetSpriteVer(SPRITEID_FONT_LEVEL_01)->SetCharSpacing(4);
		float posX = k_SCREEN_WIDTH/2 + x + _posX;
		float posY = _game->GetSpriteVer(SPRITEID_HUD_RESULT)->GetFModuleCenterY(8, 0) + y - 125 + _offNameY;
		_gp->SetOpacity(80 * opacity / 100.0f);
		_gp->SetScale(_zoomLevel, _zoomLevel, posX, posY);
		_game->GetSpriteVer(SPRITEID_FONT_LEVEL_01)->DrawAnsiTextFormat(_gp, posX, posY, _HCENTER | _VCENTER, ": %d", _level + 1);
		_gp->ClearScale();
		_game->GetSpriteVer(SPRITEID_FONT_LEVEL_01)->SetCharSpacing(charspacing);
		
		
		//multiplier	
		float speed_multi = game->_game_design.Achievement.SheetMultiplier.floatValue.getValue(game->_quest._level, CGameDesign::FileAchievement::CSheetMultiplier::speed_title);
		/*speed_multi *= 0.01f;
		speed_multi *= 100;*/
		charspacing = _game->GetSpriteVer(SPRITEID_FONT_TITLE_01)->_char_spacing;
		_game->GetSpriteVer(SPRITEID_FONT_TITLE_01)->SetCharSpacing(5);
		posY = _game->GetSpriteVer(SPRITEID_HUD_RESULT)->GetFModuleCenterY(8, 0) + y - 85 + _offNameY;
		_gp->SetOpacity(80 * opacity / 100.0f);
		_gp->SetScale(_zoomMulti, _zoomMulti, posX, posY);
		_game->GetSpriteVer(SPRITEID_FONT_TITLE_01)->DrawAnsiTextFormat(_gp, posX, posY, _HCENTER | _VCENTER, "score multiplier x%d - speed x%0.1f", GetCurrentMulti(), speed_multi);
		_gp->ClearScale();
		_game->GetSpriteVer(SPRITEID_FONT_TITLE_01)->SetCharSpacing(charspacing);
		//next objective		
		if (_status != CQuest::enumQuestStatus::_QUEST_NONE)
		{
			
			charspacing = _game->GetSpriteVer(SPRITEID_FONT_MISSION_01)->_char_spacing;
			_game->GetSpriteVer(SPRITEID_FONT_MISSION_01)->SetCharSpacing(4);
			posX = k_SCREEN_WIDTH / 2 + x + _posX + _offDescX + _offNextX;
			posY = _game->GetSpriteVer(SPRITEID_HUD_RESULT)->GetFModuleCenterY(8, 0) + y;
			_gp->SetOpacity(60 * opacity / 100.0f);
			_gp->SetTransform(CMath::Matrix4ToParam(CMath::Matrix4Multiply(CMath::Matrix4Scale2D(_sizeText, _sizeText, posX, posY), zoom)));
			_game->GetSpriteVer(SPRITEID_FONT_MISSION_01)->DrawAnsiTextFormat(_gp, posX, posY, _HCENTER | _VCENTER, _description);
			_gp->ClearTransform();
			_game->GetSpriteVer(SPRITEID_FONT_MISSION_01)->SetCharSpacing(charspacing);

			charspacing = _game->GetSpriteVer(SPRITEID_FONT_TITLE_02)->_char_spacing;
			_game->GetSpriteVer(SPRITEID_FONT_TITLE_02)->SetCharSpacing(4);
			posX = k_SCREEN_WIDTH / 2 + x + _posX + _offNextX;
			posY = _game->GetSpriteVer(SPRITEID_HUD_RESULT)->GetFModuleCenterY(8, 0) + y + 74;
			_gp->SetOpacity(60 * opacity / 100.0f);
			//_gp->SetScale(0.22, 0.22, posX, posY);
			_game->GetSpriteVer(SPRITEID_FONT_TITLE_02)->DrawAnsiTextFormat(_gp, posX, posY, _HCENTER | _VCENTER, "next level objective");
			//_gp->ClearScale();
			_game->GetSpriteVer(SPRITEID_FONT_TITLE_02)->SetCharSpacing(charspacing);
			_gp->SetOpacity(opacity);
		}
	}
}
void CQuest::UpdateQuest()
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
	//switch (_state)
	//{
	//case 1:
	//	_posX += 30;
	//	if (_posX >= 20)
	//	{
	//		_posX = 20;
	//		_state = 2;
	//	}
	//	break;

	//case 2:
	//	_posX -= 5;
	//	if (_posX <= -10)
	//	{
	//		_posX = -10;
	//		_state = 3;
	//	}
	//	break;

	//case 3:
	//	_posX += 3;
	//	if (_posX >= 0)
	//	{
	//		_posX = 0;
	//		_state = 4;
	//	}
	//	break;

	//case 4:	
	//	/*if ((_status == CQuest::enumQuestStatus::_QUEST_COMPLETE)
	//		|| game->_isNewScore || game->_isShowRate
	//		|| game->_stateEgg == 2 || game->_stateEgg == 1
	//		|| game->_dialog._typeDialog != CDialog::enumTypeDialog::DIALOG_NONE)
	//	{

	//	}
	//	else
	//	{
	//		if (_perScroll < _percent)
	//		{
	//			_perScroll += _detaPer;
	//			if (_perScroll > _percent)
	//			{
	//				_perScroll = _percent;
	//			}
	//		}
	//	}*/
	//	break;

	//case 10:
	//	_posX -= 30;
	//	if (_posX <= -k_SCREEN_WIDTH)
	//	{
	//		_posX = -k_SCREEN_WIDTH;
	//		_state = 11;
	//	}
	//	break;
	//}
	_effect_end = true;
	if (_state == 1)
	{
		if ((_status == CQuest::enumQuestStatus::_QUEST_COMPLETE))
		{

		}
		else
		{
			if (_perScroll < _percent)
			{
				_perScroll += _detaPer;
				if (_perScroll >= _percent)
				{
					_perScroll = _percent;
				}
			}
		}
	}
	//printf("\n _perScroll:%f,_percent:%d", _perScroll, _percent);
	if (_perScroll != _percent)
	{
		_effect_end = false;
		return;
	}
	switch (_stateEffect)
	{
	case 1:
		_offDescX -= 5;
		if (_offDescX<-40)
		{
			_offDescX = -40;
			_stateEffect = 2;
			_zoomCheck = 4;
			_opaCheck = 0;
			PLAY_SFX(SFX_OBJECTIVE_COMPLETE);
		}
		break;

	case 2:
		_opaCheck += 3;
		_zoomCheck -= 0.15;
		if (_zoomCheck <= 0.9)
		{
			_zoomCheck = 0.9;
			_stateEffect = 3;
		}
		break;

	case 3:
		_opaCheck += 3;
		_zoomCheck += 0.02;
		if (_zoomCheck >= 1.05)
		{
			_zoomCheck = 1.05;
			_stateEffect = 4;
		}
		break;

	case 4:
		_opaCheck += 3;
		_zoomCheck -= 0.01;
		if (_zoomCheck <= 1.05)
		{
			_zoomCheck = 1;
			_stateEffect = 5;
			_timeDelay = 10;
		}
		break;

	case 5:
		_timeDelay--;
		if (_timeDelay <= 0)
		{
			_zoomLevel += 0.04;
			if (_zoomLevel >= 1.15)
			{
				_zoomLevel = 1.15;
				_level++;
				_stateEffect = 6;
				PLAY_SFX(SFX_LEVEL_UP);
			}
		}
		break;

	case 6:
		_zoomLevel -= 0.02;
		if (_zoomLevel <= 1)
		{
			_zoomLevel = 1;
			_stateEffect = 7;
			_timeDelay = 10;
		}
		break;

	case 7:
		_timeDelay--;
		if (_timeDelay <= 0)
		{
			_zoomMulti += 0.04;
			if (_zoomMulti >= 1.15)
			{
				PLAY_SFX(SFX_MULTIPLIER_SCORE_UP);
				_zoomMulti = 1.15;
				HSGame* game = (HSGame*)(GAME()->_current_game);
				int param_xscore = CGameDesign::FileAchievement::CSheetMultiplier::multi;
				_multi = game->GetDesignMultiScoreInt(_level, param_xscore);
				_stateEffect = 8;
			}
		}
		break;

	case 8:
		_zoomMulti -= 0.02;
		if (_zoomMulti <= 1)
		{
			_zoomMulti = 1;
			_stateEffect = 9;
		}
		break;

	case 9:
		_offNextX += 50;
		if (_offNextX >= k_SCREEN_WIDTH)
		{
			HSGame* game = (HSGame*)(GAME()->_current_game);
			if (_currentQuest < game->_game_design.Achievement.SheetObjective.int32Value._totalData - 1)
			{
				_currentQuest++;
				LoadInfoQuest(_currentQuest);
				NewValueQuest();

				_status = CQuest::enumQuestStatus::_QUEST_DOING;
				_stateEffect = 11;
			}
			else
			{
				_status = CQuest::enumQuestStatus::_QUEST_NONE;
				_stateEffect = 15;
			}		
			game->_user.UserDataSave();			
			_offNextX = -k_SCREEN_WIDTH;
			_offDescX = 0;
			
		}
		break;

	case 11:
		_offNextX += 30;
		if (_offNextX >= 20)
		{			
			_offNextX = 20;
			_stateEffect = 12;
		}
		break;

	case 12:
		_offNextX -= 5;
		if (_offNextX <= -10)
		{
			_offNextX = -10;
			_stateEffect = 13;
		}
		break;

	case 13:
		_offNextX += 3;
		if (_posX >= 0)
		{
			_offNextX = 0;
			_stateEffect = 20;
		}
		break;

	case 20:
		_zooZoomQuest += 0.02;
		if (_zooZoomQuest >= 1.1)
		{
			//PLAY_SFX(SFX_NEW_OBJECTIVE_DING);
			_zooZoomQuest = 1.1;
			_stateEffect = 21;
		}
		break;

	case 21:
		_zooZoomQuest -= 0.02;
		if (_zooZoomQuest <= 1)
		{			
			_stateEffect = 0;			
		}
		break;

	case 15:
		_offNameY += 5;
		if (_offNameY>60)
		{
			_offNameY = 60;
			_stateEffect = 0;
		}
		break;
	}
	if (_stateEffect > 0 && _stateEffect < 11)
	{
		_effect_end = false;
	}
}
void CQuest::CheckQuestComplete()
{
	HSGame*game = (HSGame*)(GAME()->_current_game);
	if (_status == CQuest::enumQuestStatus::_QUEST_DOING && _stateEffect == 0)
	{
		
		if (_currentValue >= _maxValue)
		{	
			_status = CQuest::enumQuestStatus::_QUEST_COMPLETE;
			HSGame* game = (HSGame*)(GAME()->_current_game);
			game->_stateObj = 1;
#ifdef MAC_OS
			PineGameCenterController::UpdateAchievement(_gcID, 100, true);
#endif
			game->GooglePlaySubmitAchievement(_gplayID);
		}
		
	}
}
void CQuest::SetQuestComplete(int level)
{
	if (_status == CQuest::enumQuestStatus::_QUEST_DOING && _currentQuest == level)
	{		
		_status = CQuest::enumQuestStatus::_QUEST_COMPLETE;
		HSGame* game = (HSGame*)(GAME()->_current_game);
		game->_stateObj = 1;
	}
}