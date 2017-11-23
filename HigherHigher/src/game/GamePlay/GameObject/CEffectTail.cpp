#include "pch.h"
#include "../../GameCore/CGame.h"

CEffectTail::CEffectTail()
{
}

CEffectTail::~CEffectTail()
{
}

void CEffectTail::EffectInit(int sprite, int frame, int num)
{
	_game = GAME();
	_grap = G();
	_num = num;
	for (int i = 0; i < _num; i++)
	{
		_effect[i].Init(sprite,frame);
	}
}
void CEffectTail::EffectRender(float x, float y)
{
	for (int i = 0; i < _num; i++)
	{
		_effect[i].Render(x, y);
	}
}
void CEffectTail::EffectAdd(float x, float y, float scale, float scalemax, float speedscale, float rotate, float rotspeed, int opac, float speedopac)
{
	for (int i = 0; i < _num; i++)
	{
		if (_effect[i]._status == 0)
		{

			_effect[i].Add(x, y, scale, scalemax, speedscale, rotate, rotspeed, opac, speedopac);
			break;
		}
	}
}
void CEffectTail::EffectUpdate()
{
	for (int i = 0; i < _num; i++)
	{
		_effect[i].Update();
	}
}

void CEffectTail::SEffect::Init(int sprite, int frame)
{
	_x = 0;
	_y = 0;
	_status = 0;
	_scale_speed = 0;
	_scale = 1.0f;
	_opac = 100;
	_rotate_speed;
	_rotate = 0;
	_scale_max = 1;
	_scale_min = 0;
	_spriteid = sprite;
	_frameid = frame;
}
void CEffectTail::SEffect::Add(float x, float y, float scale, float scalemax, float speedscale, float rotate, float rotspeed, int opac, float speedopac)
{
	_x = x;
	_y = y;
	_status = 1;
	_scale_speed = speedscale;
	_scale = scale;
	_opac_max = opac;
	_opac = _opac_max;
	_rotate = rotate;
	_rotate_speed = rotspeed;
	_opac_speed = speedopac;
	_scale_min = 0;
	_scale_max = scalemax;
}
void CEffectTail::SEffect::Render(float x, float y)
{
	if (_status == 0)
	{
		return;
	}
	
	G()->SetScale(_scale, _scale, _x + x, _y + y);
	G()->SetRotate(_rotate, _x + x, _y + y);
	G()->SetOpacity(_opac);
	GET_SPRITE(_spriteid)->DrawFrame(G(), _frameid, _x + x, _y + y);
	G()->SetOpacity(100);
	G()->ClearRotate();
	G()->ClearScale();
	
}
void CEffectTail::SEffect::Update()
{
	if (_status == 0)
	{
		return;
	}
	switch (_status)
	{
	case 1:
		_scale += _scale_speed;
		if (_scale >= _scale_max)
		{
			_scale = _scale_max;
			_status = 2;
		}
		if (_opac < _opac_max)
		{
			_opac += _opac_speed;
		}
		break;
	case 2:
		_scale -= _scale_speed;
		if (_scale <= 0)
		{
			_scale = 0;
		}
		_opac -= _opac_speed;
		if (_opac <= 0)
		{
			_status = 0;
		}
		break;
	default:
		break;
	}
	_rotate += _rotate_speed;
	if (_rotate>= 360)
	{
		_rotate -= 360;
	}
}
void CEffectTail::ForceEnd()
{
    for (int i = 0; i < _num; i++)
    {
        _effect[i]._status = 0;
    }
}