#include "pch.h"
#include "../../GameCore/CGame.h"
CCharacter::CCharacter()
{
}

CCharacter::~CCharacter()
{
}

void CCharacter::Init(int spriteid, int anim, float x, float y)
{
	_game = GAME();
	_grap = G();
	HSGame *game = (HSGame*)(_game->_current_game);
	SetPos(x, y);
	_spriteid = spriteid;
	int param = CGameDesign::FileLevel::CSheetGeneral::paramater;
	float speed_multi = game->_game_design.Achievement.SheetMultiplier.floatValue.getValue(game->_quest._level, CGameDesign::FileAchievement::CSheetMultiplier::speed_change);
	//_speed = (game->GetDesignGeneralint(DESIGN_PARAM_SPEED_START, param)*speed_multi)*game->_port_scale_x;
	_speed = speed_multi * game->_port_scale_x;
	//_speed_increase = game->GetDesignGeneralint(DESIGN_PARAM_SPEED_INCREASE, param)*game->_port_scale_x;
	int param_speed = CGameDesign::FileLevel::CSheetlevel::speed_increase;
	_speed_increase = game->GetDesignSpeedFloat(game->_bounce_count_times, param_speed)*game->_port_scale_x;
	_speed_max = game->GetDesignGeneralint(DESIGN_PARAM_SPEED_MAX, param)*game->_port_scale_x;

	int param_name = CGameDesign::FileLevel::CSheetcharacter::name;
	sprintf(_name, "%s", game->GetDesignCharString(game->_user._typeCharacterUse, param_name));

    _state = CHARACTER_STATE_RUN;
    _orien = CHARACTER_DIR_RIGHT;
	_w = SIZE_CHAR_WIDTH*game->_port_scale_x;
	_h = SIZE_CHAR_HEIGHT*game->_port_scale_x;
	_area = 0;
	_alpha = 0;
	_color_type = -1;
	ChangeColor(CMath::RANDOM(0,3));
	game->ControlChangeColor();
	
	_effect_tail.EffectInit(_spriteid, 1, 100);
	_count_frame_add_effect = -1;
	_die_opac = 0;
}
void CCharacter::Render(float x, float y)
{
	HSGame *game = (HSGame*)(_game->_current_game);
	COLOR _color;
	switch (_color_type)
	{
	case 0:
		_color = 0xff78556d;
		break;
	case 1:
		_color = 0xffe8d469;
		break;
	case 2:
		_color = 0xfff3616f;
		break;
	case 3:
		_color = 0xff364b75;
		break;
	case 4:
		_color = 0xff83a471;
		break;
	case 5:
		_color = 0xffc56446;
		break;
	default:
		break;
	}
	_effect_tail.EffectRender(x, y);
	if (!game->_game_over || !game->_new_highscore.IsFree())
	{
		/*_grap->SetColor(_color);
		_grap->FillRect(_x + x + 7, _y + y + 7, 24, 24, true);*/
		_grap->SetScale(game->_port_scale_x, game->_port_scale_x, _x + x, _y + y);
		_grap->SetColorMask(_color);
		GET_SPRITE(_spriteid)->DrawFrame(_grap, 2, _x + x, _y + y);
		_grap->ClearColorMask();
		GET_SPRITE(_spriteid)->DrawFrame(_grap, 0, _x + x, _y + y);
		_grap->ClearScale();
	}
	/*if (game->_game_over && _die_opac < 200)
	{
		_grap->SetColor(_color);
		_grap->FillRect(_x + x + 7, _y + y + 7, 24, 24, true);
		_die_opac += 100;
		_grap->SetOpacity(_die_opac);
		_grap->SetColor(0xffffffff);
		_grap->FillRect(_x + x, _y + y, 38, 38, true);
		_grap->SetOpacity(100);
		GET_SPRITE(_spriteid)->DrawFrame(_grap, 0, _x + x, _y + y);
	}*/
}
void CCharacter::Update()
{
	HSGame *game = (HSGame*)(_game->_current_game);
	if (game->_tutorial_state != 0 && game->_user._stepTutorial < 100)
	{
		return;
	}
	/*if (game->_user._stepTutorial < 100)
	{
		_speed = 3.0f;
	}*/
	if (_speed >= _speed_max)
	{
		_speed = _speed_max;
	}
    //printf("\n _x:%f",_x);
	_effect_tail.EffectUpdate();
	if (game->_game_over)
	{
		return;
	}
	UpdateState();
	UpdateOrien();
	if (_x <= game->_point_o.X)
	{
		_x = game->_point_o.X;
		SetOrien(CHARACTER_DIR_RIGHT);
	}
	if (_x + _w >= game->_screen_width + game->_point_o.X)
	{
		_x = game->_screen_width + game->_point_o.X - _w;
		SetOrien(CHARACTER_DIR_LEFT);
	}
	//float scale = CMath::RANDOM(1, 4) / 10.0f;
	_count_frame_add_effect++;
    int trigger = 18/_speed;
    //printf("\n trigger:%d",trigger);
	if (_count_frame_add_effect >= trigger)
	{
		_effect_tail.EffectAdd(_x + _w / 2, _y + _h / 2, 0.0f, 1.0f, 0.08f, 0, 10, 70, 5);
		_count_frame_add_effect = 0;
	}
	
}
void CCharacter::SetPos(float x, float y)
{
	_x = x;
	_y = y;
}

void CCharacter::SetState(int state)
{
	if (_state == state)
	{
		return;
	}
	_state = state;
	switch (_state)
	{
	case CHARACTER_STATE_IDLE:
		//game->_grid.SetScaleState(2);
		//_anim.SetAnim(CHARACTER_ANIM_IDLE, true);
		break;
	case CHARACTER_STATE_RUN:
		//game->_grid.SetScaleState(0);
		//_anim.SetAnim(CHARACTER_ANIM_RUN, true);
		break;
	case CHARACTER_STATE_ATTACK:
		//game->_grid.SetScaleState(1);
		//_anim.SetAnim(CHARACTER_ANIM_ATTACK, true);
		break;
	case CHARACTER_STATE_DIE:
		//_anim.SetAnim(CHARACTER_ANIM_DIE, true);
		break;
	default:
		break;
	}
}
void CCharacter::SetOrien(int orien)
{
	HSGame *game = (HSGame*)(_game->_current_game);
	if (_orien == orien)
	{
		return;
	}
	_orien_old = _orien;
	_orien = orien;
	int param_speed = CGameDesign::FileLevel::CSheetlevel::speed_increase;
	printf("\n change Orien");
	switch (_orien)
	{
	case CHARACTER_DIR_NORMAL:
		break;
	case CHARACTER_DIR_LEFT:
		_alpha = 180;
		_speed_increase = game->GetDesignSpeedFloat(game->_bounce_count_times, param_speed)*game->_port_scale_x;
		game->_bounce_count_times++;
		_speed += _speed_increase;
		break;
	case CHARACTER_DIR_RIGHT:
		_alpha = 0;
		_speed_increase = game->GetDesignSpeedFloat(game->_bounce_count_times, param_speed)*game->_port_scale_x;
		game->_bounce_count_times++;
		_speed += _speed_increase;
		break;
	default:
		break;
	}
    //ChangeColor();
	game->ControlChangeColor();
	game->_border_change_color = 10;
	PLAY_SFX(SFX_CUBE_BOUNCE);
}

void CCharacter::UpdateState()
{
	HSGame *game = (HSGame*)(_game->_current_game);
	switch (_state)
	{
	case CHARACTER_STATE_IDLE:
		
		break;
	case CHARACTER_STATE_RUN:
	{
		float disx = _speed*CMath::COS(_alpha);
		_x += _speed*CMath::COS(_alpha);
		_y += _speed*CMath::SIN(_alpha);
		/*if (_x - disx <= 0 && _count_frame_add_effect >=0)
		{
			game->_particle_fire.Active(10, _y + _h / 2);
		}
		if (_x + _w + disx >= k_SCREEN_WIDTH)
		{
			game->_particle_fire.Active(k_SCREEN_WIDTH - 10, _y + _h / 2);
		}*/
		
	}
            break;

	case CHARACTER_STATE_ATTACK:
        
		break;
	case CHARACTER_STATE_DIE:
		
		break;
	default:
		break;
	}
	//printf("\n state:%d _speed:%f",_state,_speed);
}
void CCharacter::UpdateOrien()
{
	HSGame *game = (HSGame*)(_game->_current_game);
	float alpha_sps_limit = 10;// game->GetDesignGeneral(DESIGN_GENERAL_SHIP_ROTATE, 0);
	switch (_orien)
	{
	case CHARACTER_DIR_NORMAL:
            
		break;
	case CHARACTER_DIR_RIGHT:
		if (game->_user._stepTutorial < 100)
		{
			if (game->_user._stepTutorial == 1)
			{
				if (_x >= game->_control[2]._x + game->_control[2]._w / 3)
				{
					game->TutorialSetState(1);
				}
			}
			else if (game->_user._stepTutorial == 2)
			{
				if (_x >= game->_control[3]._x + game->_control[3]._w / 3)
				{
					game->TutorialSetState(1);
				}
			}
			else if (game->_user._stepTutorial == 3)
			{
				if (_x >= game->_control[3]._x + game->_control[3]._w / 3)
				{
					game->TutorialSetState(1);
				}
			}
			else if (game->_user._stepTutorial == 9)
			{
				if (_x >= game->_control[0]._x + game->_control[0]._w / 2)
				{
					game->TutorialSetState(1);
				}
			}
			else if (game->_user._stepTutorial == 10)
			{
				if (_x >= game->_control[1]._x + game->_control[1]._w/2)
				{
					game->TutorialSetState(1);
				}
			}
			else if (game->_user._stepTutorial == 11)
			{
				if (_x >= game->_control[1]._x + game->_control[1]._w / 2)
				{
					game->TutorialSetState(1);
				}
			}
		}
		break;
	case CHARACTER_DIR_LEFT:
		if (game->_user._stepTutorial < 100)
		{
			if (game->_user._stepTutorial == 4)
			{
				if (_x < game->_control[3]._x + game->_control[3]._w / 3)
				{
					game->TutorialSetState(1);
				}
			}
			else if (game->_user._stepTutorial == 5)
			{
				if (_x < game->_control[3]._x + game->_control[3]._w / 3)
				{
					game->TutorialSetState(1);
				}
			}
			else if (game->_user._stepTutorial == 6)
			{
				if (_x < game->_control[2]._x + game->_control[2]._w / 2)
				{
					game->TutorialSetState(1);
				}
			}
			else if (game->_user._stepTutorial == 7)
			{
				if (_x < game->_control[1]._x + game->_control[1]._w / 2)
				{
					game->TutorialSetState(1);
				}
			}
			else if (game->_user._stepTutorial == 8)
			{
				if (_x < game->_control[0]._x + game->_control[0]._w * 0.6)
				{
					game->TutorialSetState(1);
				}
			}
		}
		break;
	default:
		break;
	}
}
void CCharacter::ChangeColor(int typecolor)
{
	HSGame *game = (HSGame*)(_game->_current_game);

	//int arr_color[CONTROL_NUM_BUTTON];
	//for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
	//{
	//	arr_color[i] = i;
	//}
	//int num = 0;
	//while (num < 20)
	//{
	//	int index = CMath::RANDOM(0, 2);
	//	int temp = arr_color[index];
	//	arr_color[index] = arr_color[index + 1];
	//	arr_color[index + 1] = temp;
	//	num++;
	//}
	//int percent = 100 / CONTROL_NUM_BUTTON;
	//for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
	//{
	//	game->_control[i]._color_type = arr_color[i];
	//	/*int rand = CMath::RANDOM(1, 100);
	//	if (rand <= percent)
	//	{
	//		game->_control[i]._color_type = _color_type;
	//		percent = -1000;
	//	}
	//	else
	//	{
	//		type_color = CMath::RANDOM(0, 3);
	//		while (_color_type == type_color)
	//		{
	//			type_color = CMath::RANDOM(0, 3);
	//		}
	//		game->_control[i]._color_type = type_color;
	//		percent += 100 / CONTROL_NUM_BUTTON;
	//	}*/
	//}
	int type_color = CMath::RANDOM(0, 3);
	int color0 = game->_control[0]._color_type;
	int color3 = game->_control[3]._color_type;
	bool samecolor = false;
	if (_orien == CHARACTER_DIR_LEFT)
	{
		if (type_color == game->_control[3]._color_type)
		{
			samecolor = true;
		}
	}
	else if (_orien == CHARACTER_DIR_RIGHT)
	{
		if (type_color == game->_control[0]._color_type)
		{
			samecolor = true;
		}
	}
	//if(_color_type == -1)
	{
		while (_color_type == type_color || samecolor)
		{
			type_color = CMath::RANDOM(0, 3);
			samecolor = false;
			if (_orien == CHARACTER_DIR_LEFT)
			{
				if (type_color == game->_control[3]._color_type)
				{
					samecolor = true;
				}
			}
			else if (_orien == CHARACTER_DIR_RIGHT)
			{
				if (type_color == game->_control[0]._color_type)
				{
					samecolor = true;
				}
			}
		}
		_color_type = type_color;
	}
	_color_type = typecolor;
	
	game->_has_change_color = true;
	game->_border_change_color = 10;
}