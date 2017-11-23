#include "pch.h"
#include "../../GameCore/CGame.h"

CFood::CFood()
{
}

CFood::~CFood()
{
}

void CFood::Init(int sprite)
{
	_game = GAME();
	_grap = G();
	_spriteid = sprite;
	_x = 0;
	_y = 0;
	_tarx = 0;
	_tary = 0;
	_frameid = 0;
	_angle = 0;
	_state = 0;
	_scale = 1.0f;
	_opac = 100;
	_scale_speed = 0;
	_tail.EffectInit(_spriteid, 0, 5);
}
void CFood::Add(float x, float y, int frame, float speed)
{
	_x = x;
	_y = y;
	_frameid = frame;
	_speed = speed;
	_state = 1;
	_scale = 0.7f;
	_opac = 100;
	_scale_speed = 0;
	SetTar(_x, _y);
	_opac_speed = 3;
}
void CFood::Render(float x, float y)
{
	if (_state == 0)
	{
		return;
	}
	_grap->SetScale(_scale, _scale, _x + x, _y + y);
	_grap->SetOpacity(_opac);
	GET_SPRITE(_spriteid)->DrawFrame(_grap, _frameid, _x + x, _y + y);
	_grap->SetOpacity(100);
	_grap->ClearScale();
	_tail.EffectRender(x, y);
}
void CFood::Update()
{
	HSGame *game = (HSGame*)(_game->_current_game);
	if (_state == 0)
	{
		return;
	}
	_tail.EffectUpdate();
	if (_y > game->_control[0]._beginy)
	{
		_state = 0;
	}
	switch (_state)
	{
	case 1:
		if (CHECK_POINT_IN_CIRCLE(_x, _y, _tarx, _tary, _speed))
		{
			_x = _tarx;
			_y = _tary;
		}
		else
		{
			_x += _speed*CMath::COS(_angle);
			_y += _speed*CMath::SIN(_angle);
			_tail.EffectAdd(_x, _y, _scale, _scale, 0, 0, 0, 100, 10);
		}
		_opac += _opac_speed;
		if (_opac >= 120)
		{
			_opac_speed = -3;
		}
		if (_opac <= 50)
		{
			_opac_speed = 3;
		}
		break;
	case 2:
		_state = 3;
		break;
	case 3:
		_scale += _scale_speed;
		_opac -= 2;
		if (_opac <= 70)
		{
			_opac -= 5;
			GAME()->FPS_LIMIT = FPS_MAX;
			_scale_speed *= 1.25f;
			_y -= _speed/2;
		}
		else
		{
			GAME()->FPS_LIMIT = 10;
			_y -= _speed/3;
			_scale_speed *= 1.1f;
		}
		if (_opac <= 0)
		{
			_opac = 0;
			_state = 0;
		}
		break;
	default:
		break;
	}
}
void CFood::SetPos(float x, float y)
{
	_x = x;
	_y = y;
}
void CFood::SetTar(float x, float y)
{
	if (_state != 1)
	{
		return;
	}
	_tarx = x;
	_tary = y;
	_angle = CMath::ANGLE(_x, _y, _tarx, _tary);
	_speed = CMath::DISTANCE(_x, _y, _tarx, _tary) / 5.0f;
}