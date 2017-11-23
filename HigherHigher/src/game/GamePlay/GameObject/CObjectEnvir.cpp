#include "pch.h"
#include "../../GameCore/CGame.h"

CObjectEnvir::CObjectEnvir()
{
}

CObjectEnvir::~CObjectEnvir()
{
}

void CObjectEnvir::Init()
{
	_game = GAME();
	_grap = G();
	_x = 0;
	_y = 0;
	_spriteid = 0;
	_frameid = 0;
	_type = 0;
	_layer = 1;
	_opac = 0;
	_opac_speed = 5;
}
void CObjectEnvir::Add(int sprite, int frame, float x, float y, int layer, int type)
{
	_x = x;
	_y = y;
	SetTarget(_x, _y,1);
	_spriteid = sprite;
	_frameid = frame;
	_type = type;
	_layer = layer;
	_opac = 100;
	_opac_speed = 3;
	_scale = 1.0f;
}
void CObjectEnvir::Render(float x, float y)
{
	_grap->SetScale(_scale, _scale, _x + x, _y + y);
	_grap->SetOpacity(_opac);
	GET_SPRITE(_spriteid)->DrawFrame(_grap, _frameid, _x +x, _y + y);
	_grap->SetOpacity(100);
	_grap->ClearScale();
}
void CObjectEnvir::Update()
{
	HSGame*game = (HSGame*)(GAME()->_current_game);
	float scale = 0.8f / _layer;
	switch (_type)
	{
	case OBJECT_ENVIR_TYPE_NORMAL:
		
		break;
	case OBJECT_ENVIR_TYPE_OB1:
		
		break;
	case OBJECT_ENVIR_TYPE_OB2:
		
		break;
	default:
		break;
	}
	if (CHECK_POINT_IN_CIRCLE(_x, _y, _tarx, _tary,_speed))
	{
		_x = _tarx;
		_y = _tary;
	}
	else
	{
		float angle = CMath::ANGLE(_x, _y, _tarx, _tary);
		_x += _speed*CMath::COS(angle);
		_y += _speed*CMath::SIN(angle);
	}
}
void CObjectEnvir::SetPos(float x, float y)
{
	_x = x;
	_y = y;
}
void CObjectEnvir::SetTarget(float x, float y,float speed)
{
	_tarx = x;
	_tary = y;
	float dis = CMath::DISTANCE(_x, _y, _tarx, _tary);
	_speed = dis/(10*speed);
	if (_speed <= 1)
	{
		_speed = 1;
	}
}