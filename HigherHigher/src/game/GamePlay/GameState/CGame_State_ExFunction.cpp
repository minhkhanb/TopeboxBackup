#include "pch.h"
#include "../../GameCore/CGame.h"

void HSGame::ObjectEnvirInit()
{
	_bg_spriteid_current = CMath::RANDOM(0, BG_SPRITEID_NUM - 1);
	if (_first_game)
	{
		_bg_spriteid_current = 0;
	}
	int spriteid = _bg_spriteid[_bg_spriteid_current];
	for (int i = 0; i < OBJECT_ENVIR_NUM; i++)
	{
		float posx = 0;
		float posy = 0;
		_object_envir[i].Init();
		_object_envir[i].Add(spriteid, 14 + i, posx, posy, i+1);
	}
}
void HSGame::ObjectEnvirRender(float x, float y)
{
	for (int i = OBJECT_ENVIR_NUM - 1; i >= 0; i--)
	{
		_object_envir[i].Render(x, y + GetBGTranslateY()/(i+1));
	}
}
void HSGame::ObjectEnvirUpdate()
{
	for (int i = 0; i < OBJECT_ENVIR_NUM; i++)
	{
		float disx = (k_SCREEN_WIDTH / 2 - _main_char._x);
		float move_x = (disx / 150)*(OBJECT_ENVIR_NUM - i);
		_object_envir[i]._x = move_x;
		_object_envir[i]._tarx = _object_envir[i]._x;
		_object_envir[i].Update();
	}
}

void HSGame::AddEffectLight()
{
	for (int i = 0; i < EFFECT_LIGHT_NUM_MAX; i++)
	{

		float x = CMath::RANDOM(0, 15) * 75;
		float y = CMath::RANDOM(1, 5) * 15;
		float speed = CMath::RANDOM(1, 5) / 20.0f;

		_effect_lighting[i].Init(x, y, 0, -(CMath::RANDOM(1, 2) / 2.0f), speed);
		_effect_lighting[i]._rotate = 135;
		_effect_lighting[i]._frameid = CMath::RANDOM(7, 8);
	}
}
void HSGame::RenderEffectLight(float x, float y, int opac)
{
	for (int i = 0; i < EFFECT_LIGHT_NUM_MAX; i++)
	{
		_effect_lighting[i].Render(x, y, opac);
	}
}
void HSGame::UpdateEffectLight()
{
	for (int i = 0; i < EFFECT_LIGHT_NUM_MAX; i++)
	{
		_effect_lighting[i].Update();
	}
}

#pragma region PARTICLE_EFFECT
void HSGame::EffectLight::Init(float x, float y, int opac, float speedopac, float speed)
{
	_x = x;
	_y = y;
	_opac = opac;
	_speed = speed;
	_speed_opac = speedopac;
	_dir = CMath::RANDOM(0, 1);
	if (_dir == 0)
	{
		_y = 0;
	}
	else
	{
		_x = 0;
	}
	_frameid = 0;
	_rotate = 0;
}
void HSGame::EffectLight::Render(float x, float y, int opac)
{
	G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
	G()->SetOpacity(_opac*(opac / 100.0f));
	G()->SetRotate(_rotate, _x + x, _y + y);
	//G()->SetScale(1.5f, 1.5f, 0, 0);
	GET_SPRITE(SPRITEID_OBJECT_01)->DrawFrame(G(), _frameid, _x + x, _y + y);
	//G()->ClearScale();
	G()->ClearRotate();
	G()->SetOpacity(opac);
	G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
}
void HSGame::EffectLight::Update()
{
	_opac = (_opac - _speed_opac);
	if (_opac <= 0 && _speed_opac > 0.0f)
	{
		_opac = 0;
		_speed_opac = -(CMath::RANDOM(1, 2) / 2.0f);
		_speed = CMath::RANDOM(1, 5) / 20.0f;
		if (_dir == 0)
		{
			_x = CMath::RANDOM(0, 15) * 75;
			_y = 0;
		}
		else
		{
			_y = CMath::RANDOM(1, 5) * 15;
			_x = 0;
		}

	}
	if (_opac >= 40)
	{
		_speed_opac *= -1;
	}
	if (_dir == 0)
	{
		_x -= _speed;
	}
	else
	{
		_y -= _speed;
	}
}
void HSGame::ParticleLight::Init(float speedopac, float speed)
{
	_num = 0;
	for (int i = 0; i < PARTICLE_LIGHT_NUM_MAX; i++)
	{
		_x[i] = 0;
		_y[i] = 0;
		_opac[i] = CMath::RANDOM(120, 160);
		_speed[i] = CMath::RANDOM(1, 5) / 20.0f + speed;
		_speed_opac[i] = (CMath::RANDOM(1, 5) / 10.0f) + speedopac;
		_speedx[i] = CMath::RANDOM(-5, 5) / 5.0f;
		_speedy[i] = CMath::RANDOM(5, 15) / 10.0f;
		_scale[i] = CMath::RANDOM(1, 10) / 10.0f;
		_frame_id[i] = CMath::RANDOM(0,1);
		_state[i] = 0;
	}
}
void HSGame::ParticleLight::Render(float x, float y, int opac)
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{
			continue;
		}
		if (_frame_id[i] == 1)
		{
			G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
		}

		G()->SetOpacity(_opac[i] * (opac / 100.0f)*0.8f);
		G()->SetScale(/*_scale[i]*/1.0f, _scale[i], _x[i] + x, _y[i] + y);
		GET_SPRITE(SPRITEID_OBJECT_01)->DrawFrame(G(), _frame_id[i], _x[i] + x, _y[i] + y);
		G()->ClearScale();
		G()->SetOpacity(opac);
		G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
	}
}
void HSGame::ParticleLight::Update()
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{
			continue;
		}
		_opac[i] -= _speed_opac[i];
		if (_opac[i] <= 0 || _y[i] > k_SCREEN_HEIGHT*0.7f)
		{
			_opac[i] = CMath::RANDOM(70, 100);
			_speed_opac[i] = (CMath::RANDOM(1, 5) / 10.0f);
			_x[i] = CMath::RANDOM(-5, 60) * 15;
			_y[i] = -50;
			_speedx[i] = CMath::RANDOM(-5, 5) / 5.0f;
			_speedy[i] = CMath::RANDOM(80, 120) / 10.0f;
			_scale[i] = CMath::RANDOM(5, 10) / 10.0f;
		}
		if (_y[i] > k_SCREEN_HEIGHT / 2)
		{
			_speed_opac[i] *= 1.02f;
		}
		_speedx[i] *= 0.99f;
		if (CMath::ABSOLUTE_VALUE(_speedx[i]) <= 0.1f)
		{
			_speedx[i] *= -1;
			_speedx[i] = (0.5 - CMath::ABSOLUTE_VALUE(_speedx[i]));
		}
		//_speed[i] *= 0.95f;
		if (_y[i] > k_SCREEN_HEIGHT*0.80f)
		{
			_speedy[i] *= 0.98f;
			_speed[i] *= 0.99f;
			if (_speedy[i] <= 2.0f)
			{
				_speedy[i] = 2.0f;
			}
		}
		else
		{
			/*if (_speed[i] > 0)
			{
				_speedy[i] *= 0.995f;
			}
			if (_speedy[i] <= 2.0f)
			{
				_speedy[i] = 2.0f;
			}*/
		}
		_x[i] += _speedx[i] + _speed[i];
		_y[i] += _speedy[i];
	}
}

void HSGame::ParticleLight::AddParticleLight(float x, float y, float speedopac, float speed, int frame, int numframe)
{
	if (_num >= PARTICLE_LIGHT_NUM_MAX)
	{
		return;
	}
	_x[_num] = CMath::RANDOM(-15, 40) * 15 + x;
	_y[_num] = -CMath::RANDOM(1, 25) * 20 + y;
	_opac[_num] = CMath::RANDOM(120, 160);
	_speed[_num] = speed / 3;
	_speed_opac[_num] = /*(CMath::RANDOM(1, 5) / 10.0f)*/ +speedopac;
	_speedx[_num] = CMath::RANDOM(-5, 5) / 5.0f;
	_speedy[_num] = CMath::RANDOM(40, 60) / 10.0f + speed;
	_scale[_num] = CMath::RANDOM(8, 10) / 10.0f;
	_frame_id[_num] = CMath::RANDOM(0, numframe - 1) + frame;
	_state[_num] = 1;
	_num++;
}
void HSGame::ParticleLight::RenderParticleLight(float x, float y, int opac)
{
	Render(x, y, opac);
}
void HSGame::ParticleLight::UpdateParticleLight()
{
	Update();
}
void HSGame::ParticleLight::Active(float x, float y, float speedopac, float speed, int frame, int numframe, int num)
{
	_num = num;
	for (int i = 0; i < _num; i++)
	{
		_x[i] = CMath::RANDOM(-5, 60) * 15 + x;
		_y[i] = -CMath::RANDOM(1, 80) * 10 + y;
		_opac[i] = CMath::RANDOM(70, 100);
		_speed[i] = speed;
		_speed_opac[i] = (CMath::RANDOM(1, 5) / 10.0f) + speedopac;
		_speedx[i] = CMath::RANDOM(-5, 5) / 5.0f;
		_speedy[i] = CMath::RANDOM(80, 120)/ 10.0f;
		_scale[i] = CMath::RANDOM(10, 10) / 10.0f;
		_frame_id[i] = CMath::RANDOM(0, numframe - 1) + frame;
		_state[i] = 1;
	}
}

void HSGame::ParticleRain::Init(float speedopac, float speed)
{
	_num = 0;
	for (int i = 0; i < PARTICLE_LIGHT_NUM_MAX; i++)
	{
		_x[i] = 0;
		_y[i] = 0;
		_opac[i] = CMath::RANDOM(120, 160);
		_speed[i] = CMath::RANDOM(1, 5) / 20.0f + speed;
		_speed_opac[i] = (CMath::RANDOM(1, 5) / 10.0f) + speedopac;
		_scale[i] = CMath::RANDOM(5, 10) / 10.0f;
		_frame_id[i] = 19;
		_state[i] = 0;
	}
}
void HSGame::ParticleRain::Render(float x, float y, int opac)
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{
			continue;
		}
		if (_frame_id[i] == 1)
		{
			G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_LIGHT);
		}

		G()->SetOpacity(_opac[i] * (opac / 100.0f));
		G()->SetScale(1.0f, _scale[i], _x[i] + x, _y[i] + y);
		G()->SetRotate(30, _x[i] + x, _y[i] + y);
		GET_SPRITE(SPRITEID_OBJECT_01)->DrawFrame(G(), _frame_id[i], _x[i] + x, _y[i] + y);
		G()->ClearRotate();
		G()->ClearScale();
		G()->SetOpacity(opac);
		G()->SetFillterMode(BLEND_MODE::ALPHA_FILLTER_NORMAL);
	}
}
void HSGame::ParticleRain::Update()
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{
			continue;
		}
		_opac[i] -= _speed_opac[i];
		if (_opac[i] <= 0)
		{

			_x[i] = CMath::RANDOM(0, 70) * 20;
			_y[i] = -CMath::RANDOM(1, 25) * 20;
			_opac[i] = CMath::RANDOM(120, 160);
			_speed_opac[i] = (CMath::RANDOM(1, 5) / 10.0f);
			_tarx[i] = _x[i] - CMath::RANDOM(10, 20) * 10;
			_tary[i] = k_SCREEN_HEIGHT;
			_scale[i] = CMath::RANDOM(5, 10) / 10.0f;
			_state[i] = 1;
		}
		if (_y[i] > k_SCREEN_HEIGHT / 2)
		{
			_speed_opac[i] *= 1.02f;
		}
		/*_speedx[i] *= 0.99f;
		if (CMath::ABSOLUTE_VALUE(_speedx[i]) <= 0.1f)
		{
			_speedx[i] *= -1;
			_speedx[i] = (0.5 - CMath::ABSOLUTE_VALUE(_speedx[i]));
		}*/
		//_speed[i] *= 0.95f;
		/*if (_y[i] > k_SCREEN_HEIGHT*0.80f)
		{
			_speedy[i] *= 0.98f;
			_speed[i] *= 0.99f;
			if (_speedy[i] <= 2.0f)
			{
				_speedy[i] = 2.0f;
			}
		}
		else
		{
			if (_speed[i] > 0)
			{
				_speedy[i] *= 0.995f;
			}
			if (_speedy[i] <= 2.0f)
			{
				_speedy[i] = 2.0f;
			}
		}*/

	}
}

void HSGame::ParticleRain::AddParticleLight(float x, float y, float speedopac, float speed, int frame, int numframe)
{
	if (_num >= PARTICLE_LIGHT_NUM_MAX)
	{
		return;
	}
	_x[_num] = CMath::RANDOM(-15, 40) * 15 + x;
	_y[_num] = -CMath::RANDOM(1, 25) * 20 + y;
	_opac[_num] = CMath::RANDOM(120, 160);
	_speed[_num] = speed / 3;
	_speed_opac[_num] = /*(CMath::RANDOM(1, 5) / 10.0f)*/ +speedopac;
	_tarx[_num] = CMath::RANDOM(-10, -5) / 5.0f;
	_tary[_num] = CMath::RANDOM(35, 50) / 10.0f;
	_scale[_num] = CMath::RANDOM(8, 10) / 10.0f;
	_frame_id[_num] = CMath::RANDOM(0, numframe - 1) + frame;
	_state[_num] = 1;
	_num++;
}
void HSGame::ParticleRain::RenderParticleLight(float x, float y, int opac)
{
	Render(x, y, opac);
}
void HSGame::ParticleRain::UpdateParticleLight()
{
	Update();
}
void HSGame::ParticleRain::Active(float x, float y, float speedopac, float speed, int frame, int numframe, int num)
{
	_num = num;
	for (int i = 0; i < _num; i++)
	{
		_x[i] = CMath::RANDOM(0, 70) * 20 + x;
		_y[i] = -CMath::RANDOM(1, 25) * 20 + y;
		_opac[i] = CMath::RANDOM(120, 160);
		_speed[i] = speed;
		_speed_opac[i] = (CMath::RANDOM(1, 5) / 10.0f) + speedopac;
		_tarx[i] = _x[i] - CMath::RANDOM(10,20)*10;
		_tary[i] = k_SCREEN_HEIGHT;
		_scale[i] = CMath::RANDOM(5, 10) / 10.0f;
		_frame_id[i] = CMath::RANDOM(0, numframe - 1) + frame;
		_state[i] = 1;
	}
}

void HSGame::SParticleFly::Init(int num, int frame, int numframe)
{
	_num = num;
	for (int i = 0; i < _num; i++)
	{
		_x[i] = 0;
		_y[i] = 0;
		_tarx[i] = 0;
		_tary[i] = 0;
		_speed[i] = 0;
		_frameid[i] = CMath::RANDOM(0, numframe - 1) + frame;
	}
}
void HSGame::SParticleFly::Render(float x, float y)
{
	for (int i = 0; i < _num; i++)
	{
		GET_SPRITE(SPRITEID_OBJECT_01)->DrawFrame(G(), _frameid[i], _x[i], _y[i]);
	}
}
void HSGame::SParticleFly::Update()
{
	for (int i = 0; i < _num; i++)
	{
		if (CHECK_POINT_IN_CIRCLE(_x[i],_y[i],_tarx[i],_tary[i],_speed[i]))
		{
			_x[i] = _cenx - i*CMath::RANDOM(2, 5) * 300;
			_y[i] = _ceny + CMath::RANDOM(2, 10) * 10;
			_tarx[i] = -_x[i] + 2 * k_SCREEN_WIDTH;
			_tary[i] = _y[i];
		}
		else
		{
			float angle = CMath::ANGLE(_x[i], _y[i], _tarx[i], _tary[i]);
			_x[i] += _speed[i] * CMath::COS(angle);
			_y[i] += _speed[i] * CMath::SIN(angle);
		}
	}
}
void HSGame::SParticleFly::Active(float x, float y, float speed)
{
	_cenx = x;
	_ceny = y;
	for (int i = 0; i < _num; i++)
	{
		_x[i] = _cenx - i*CMath::RANDOM(2,5)*300;
		_y[i] = _ceny + CMath::RANDOM(2, 10) * 10;
		_tarx[i] = -_x[i] + 2*k_SCREEN_WIDTH;
		_tary[i] = _y[i];
		_speed[i] = speed + speed/CMath::RANDOM(2,6);
	}
}
void HSGame::SParticleFire::Init(int num)
{
	_num = num;
	for (int i = 0; i < _num; i++)
	{
		_x[i] = 0;
		_x_old[i] = 0;
		_y[i] = 0;
		_y_old[i] = 0;
		_alpha[i] = 0;
		_speed[i] = 0;
		_speed_delta[i] = 0;
		_scale[i] = 0;
		_scale_speed[i] = 0;
		_opac[i] = 0;
		_opac_speed[i] = 0;
		_state[i] = 0;
		_change_alpha_hold[i] = 0;
	}
	_status = 0;
}
void HSGame::SParticleFire::Active(float x, float y)
{
	_status = 1;
	for (int i = 0; i < _num; i++)
	{
		_x[i] = x;
		_x_old[i] = _x[i];
		_y[i] = y;
		_y_old[i] = _y[i];
		_alpha[i] = /*CMath::RANDOM(1,360)*/ i*(360/_num);
		_speed[i] = 0 + CMath::RANDOM(2,2);
		_speed_delta[i] = -CMath::RANDOM(1, 1)/10.0f;
		_scale[i] = 0.8f;
		_scale_speed[i] = CMath::RANDOM(1, 1)/500.0f;
		_opac[i] = 100;
		_opac_speed[i] = 1;
		_state[i] = 1;
		_change_alpha_hold[i] = 0;
		_rotate[i] = 0;
		_rotate_speed[i] = CMath::RANDOM(2, 2);
	}
}
void HSGame::SParticleFire::Render(float x, float y)
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{
			continue;
		}
		G()->SetRotate(_rotate[i], _x[i], _y[i]);
		G()->SetScale(_scale[i], _scale[i], _x[i], _y[i]);
		G()->SetOpacity(_opac[i]);
		GET_SPRITE(SPRITEID_OBJECT_HIGHSCORE)->DrawFrame(G(), 12, _x[i], _y[i]);
		G()->SetOpacity(100);
		G()->ClearScale();
		G()->ClearRotate();
	}
}
void HSGame::SParticleFire::Update()
{
	bool all_null = true;
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{
			continue;
		}
		if (_change_alpha_hold[i] > 0)
		{
			_change_alpha_hold[i]--;
		}
		else
		{
			_state[i] = 2;
		}
		_rotate[i] += _rotate_speed[i];
		if (_rotate[i] >= 360)
		{
			_rotate[i] -= 360;
		}
		all_null = false;
		_x_old[i] = _x[i];
		_y_old[i] = _y[i];
		_x[i] += _speed[i] * CMath::COS(_alpha[i]);
		_y[i] += _speed[i] * CMath::SIN(_alpha[i]);
		//_speed[i] += _speed_delta[i];
		if (_speed[i] <= 0.01)
		{
			_speed[i] = 0;
			
		}
		/*if (_speed[i] < 3.5f)
		{
			_speed_delta[i]*=0.9f;
			_state[i] = 2;
		}*/
		/*if (_speed[i] <= 1.5)
		{
			_state[i] = 2;
		}*/
		if (_state[i] == 2)
		{
			_speed[i] *= 0.985f;
			if (_speed[i] < 0.05)
			{
				_speed[i] = 0.05f;
			}
			if (_speed[i] < 1.8)
			{
				_opac[i] -= _opac_speed[i];
			}
			_speed_delta[i] *= 1.05f;
			_opac[i] -= _opac_speed[i];
			if (_opac[i] <= 0)
			{
				_opac[i] = 0;
				_state[i] = 0;
			}
			_scale[i] -= _scale_speed[i];
			if (_scale[i] <=0 )
			{
				_scale[i] = 0;
			}
		}
	}
	if (all_null)
	{
		_status = 0;
	}
}
void HSGame::SParticleFire::CheckColli(CLine line, int type)
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0 || _change_alpha_hold[i] > 0)
		{
			continue;
		}
		CLine line1;
		line1.CreateLine(_x[i], _y[i], _x_old[i], _y_old[i]);
		if (line.CheckIntersect(line1)&&line1.CheckIntersect(line))
		{
			line1.GetIntersectPoin(line);
			float disx = line1._intersection_poin._x - _x_old[i];
			float disy = line1._intersection_poin._y - _y_old[i];
			//_speed[i] = 0;
			if (type == 0)
			{
				//_x[i] += 2*disx;
				_x_old[i] = _x[i];
				_y_old[i] = _y[i];
				_y[i] += 2*disy;
				_alpha[i] = CMath::ANGLE(_x_old[i], _y_old[i], _x[i], _y[i]);
				_x[i] += _speed[i] * CMath::COS(_alpha[i]);
				_y[i] += _speed[i] * CMath::SIN(_alpha[i]);
				
			}
			else if (type == 1)
			{
				_x_old[i] = _x[i];
				_y_old[i] = _y[i];
				_x[i] += 2 * disx;
				//_y[i] += 2 * disy;
				_alpha[i] = CMath::ANGLE(_x_old[i], _y_old[i], _x[i], _y[i]);
				_x[i] += _speed[i] * CMath::COS(_alpha[i]);
				_y[i] += _speed[i] * CMath::SIN(_alpha[i]);
			}
			
			_change_alpha_hold[i] = 0;
		}
	}
}
#pragma endregion

void HSGame::CheckFoodActive()
{
	if (_food._y < _main_char._y - 200 || _food._state >= 2)
	{
		return;
	}
	CLine charline1, charline2, charline3, charline4;
	charline1.CreateLine(_main_char._x, _main_char._y, _main_char._x + _main_char._w, _main_char._y);
	charline2.CreateLine(_main_char._x + _main_char._w, _main_char._y, _main_char._x + _main_char._w, _main_char._y+_main_char._h);
	charline3.CreateLine(_main_char._x + _main_char._w, _main_char._y + _main_char._h, _main_char._x, _main_char._y + _main_char._h);
	charline4.CreateLine(_main_char._x, _main_char._y + _main_char._h, _main_char._x, _main_char._y);
	CLine foodline1, foodline2, foodline3, foodline4;
	foodline1.CreateLine(_food._x - 23, _food._y, _food._x, _food._y - 23);
	foodline2.CreateLine(_food._x, _food._y - 23, _food._x + 23, _food._y);
	foodline3.CreateLine(_food._x + 23, _food._y, _food._x, _food._y + 23);
	foodline4.CreateLine(_food._x, _food._y + 23, _food._x - 23, _food._y);
	bool behit = false;
	if ((charline1.CheckIntersect(foodline1) && foodline1.CheckIntersect(charline1))
		|| (charline1.CheckIntersect(foodline2) && foodline2.CheckIntersect(charline1))
		|| (charline1.CheckIntersect(foodline3) && foodline3.CheckIntersect(charline1))
		|| (charline1.CheckIntersect(foodline4) && foodline4.CheckIntersect(charline1)))
	{
		behit = true;
	}
	if ((charline2.CheckIntersect(foodline1) && foodline1.CheckIntersect(charline2))
		|| (charline2.CheckIntersect(foodline2) && foodline2.CheckIntersect(charline2))
		|| (charline2.CheckIntersect(foodline3) && foodline3.CheckIntersect(charline2))
		|| (charline2.CheckIntersect(foodline4) && foodline4.CheckIntersect(charline2)))
	{
		behit = true;
	}
	if ((charline3.CheckIntersect(foodline1) && foodline1.CheckIntersect(charline3))
		|| (charline3.CheckIntersect(foodline2) && foodline2.CheckIntersect(charline3))
		|| (charline3.CheckIntersect(foodline3) && foodline3.CheckIntersect(charline3))
		|| (charline3.CheckIntersect(foodline4) && foodline4.CheckIntersect(charline3)))
	{
		behit = true;
	}
	if ((charline4.CheckIntersect(foodline1) && foodline1.CheckIntersect(charline4))
		|| (charline4.CheckIntersect(foodline2) && foodline2.CheckIntersect(charline4))
		|| (charline4.CheckIntersect(foodline3) && foodline3.CheckIntersect(charline4))
		|| (charline4.CheckIntersect(foodline4) && foodline4.CheckIntersect(charline4)))
	{
		behit = true;
	}
	if (behit)
	{
		_main_char._speed -= _speed_decrease;
        _bounce_count_times = 1;
        float speed_start = GetDesignGeneralint(DESIGN_PARAM_SPEED_START,0)*_port_scale_x;
        if(_main_char._speed < speed_start)
        {
            _main_char._speed = speed_start;
        }
		_food._state = 2;
		_food._scale_speed = 0.05f;
		_food._opac = 100;
		_quest.AddCurrentValue(CQuest::enumObjective::Objective_8, 1);
		PLAY_SFX(SFX_POWERUP_COLLECTED);
	}
}
#pragma region GAME_PAUSE
void HSGame::SetPause()
{
	_isPauseGame = true;
	_statePause = 0;
	_opacityScreen = 0;
}
void HSGame::SetResume()
{
	_statePause = 1;
	_frame = 1;
	_posResumeX = k_SCREEN_WIDTH / 2;
	_posResumeY = k_SCREEN_HEIGHT / 2 - 160;
	_timeResume = 60;
	_speedResume = (k_SCREEN_WIDTH / 2 + 100 + OFF_PORT) / 15.0f;
	_opaResume = 0;
	_zoomResume = 1;
	_stateZoomResume = 0;
	//PLAY_SFX(SFX_COUNTDOWN_3);
}
void HSGame::RenderPause(float x, float y)
{
	if (_isPauseGame)
	{

		G()->SetColor(0xff000000);
		G()->SetOpacity(_opacityScreen);
		G()->FillFullScreen(true);
		G()->SetOpacity(100);
		if (_statePause > 0)
		{
			G()->SetScale(_zoomResume, _zoomResume, _posResumeX + x, _posResumeY + y);
			G()->SetOpacity(_opaResume);
			GET_SPRITE(SPRITEID_HUD_02)->DrawFrame(G(), _frame, _posResumeX + x, _posResumeY + y);
			G()->SetOpacity(100);
			G()->ClearScale();
		}
	}
}
bool HSGame::UpdatePause()
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
	if (_isPauseGame)
	{
#if !defined (MAC_OS)
		if (GAME()->_isBackkeyPress)
		{
			GAME()->isClosedInterstitialAd = true;
			GAME()->_isBackkeyPress = false;
		}
#endif
		_timeResume--;
		switch (_statePause)
		{
		case 1:
			if (_frame == 1)
			{
				_opacityScreen += 5;
				if (_opacityScreen >= 50)
				{
					_opacityScreen = 50;
				}
				_opaResume = _opacityScreen * 2;
			}
			if (_timeResume <= 15)
			{
				if (!GAME()->isClosedInterstitialAd)
				{
					return false;
				}
				_opaResume -= 7;
				_posResumeX -= _speedResume;
				_zoomResume -= 0.05;
				if (_posResumeX <= -100)
				{
					_posResumeX = k_SCREEN_WIDTH + 100 + OFF_PORT;
					_timeResume = 60;
					_statePause = 2;
					_opaResume = 0;

					_frame++;
					if (_frame>3)
					{
						_frame = 0;
					}
				}
			}
			break;

		case 2:
			if (_frame == 0)
			{
				_opacityScreen -= 2;
			}
			_zoomResume += 0.03;
			_opaResume += 7;
			_posResumeX -= _speedResume;
			if (_posResumeX <= k_SCREEN_WIDTH / 2)
			{
				_posResumeX = k_SCREEN_WIDTH / 2;
				_statePause = 3;
				_opaResume = 100;
				_stateZoomResume = 1;
				_zoomResume = 1;
			}
			break;

		case 3:
			if (_frame == 0)
			{
				if (_timeResume <= 0)
				{
					_statePause = 4;
				}
			}
			else
			{
				if (_timeResume <= 15)
				{
					_statePause = 1;
				}
			}
			switch (_stateZoomResume)
			{
			case 1:
				_zoomResume += 0.022;
				if (_zoomResume >= 1.2)
				{
					_zoomResume = 1.2;
					_stateZoomResume = 2;

					/*if (_frame == 0)
					{
						PLAY_SFX(SFX_COUNTDOWN_READY);
					}
					else if (_frame == 2)
					{
						PLAY_SFX(SFX_COUNTDOWN_2);
					}
					else if (_frame == 3)
					{
						PLAY_SFX(SFX_COUNTDOWN_1);
					}*/
				}
				break;

			case 2:
				_zoomResume -= 0.022;
				if (_zoomResume <= 0.5)
				{
					_zoomResume = 0.5;
					_stateZoomResume = 0;
				}
				break;
			}
			break;

		case 4:
			_opacityScreen -= 2;
			_opaResume -= 5;
			if (_opacityScreen <= 0 && _opaResume <= 0)
			{
				_isPauseGame = false;
			}
			break;
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

		return true;
	}
	return false;
}
#pragma endregion
void HSGame::SetTranformPiont(PineMatrix4 tranf, float clipX, float clipY, float &outX, float &outY)
{
	PineMatrix4 clipMatrix;
	clipMatrix.m0 = clipX;
	clipMatrix.m1 = clipY;
	clipMatrix.m2 = 0;
	clipMatrix.m3 = 1;

	clipMatrix.m4 = clipX + 0;
	clipMatrix.m5 = clipY;
	clipMatrix.m6 = 0;
	clipMatrix.m7 = 1;

	clipMatrix.m8 = clipX + 0;
	clipMatrix.m9 = clipY + 0;
	clipMatrix.m10 = 0;
	clipMatrix.m11 = 1;

	clipMatrix.m12 = clipX;
	clipMatrix.m13 = clipY + 0;
	clipMatrix.m14 = 0;
	clipMatrix.m15 = 1;

	clipMatrix = CMath::Matrix4Multiply(clipMatrix, tranf);

	outX = clipMatrix.m0;
	outY = clipMatrix.m1;
}
void HSGame::SetTranformRect(PineMatrix4 tranf, float clipX, float clipY, float clipW, float clipH, float &outX, float &outY, float &outW, float &outH)
{
	PineMatrix4 clipMatrix;
	clipMatrix.m0 = clipX;
	clipMatrix.m1 = clipY;
	clipMatrix.m2 = 0;
	clipMatrix.m3 = 1;

	clipMatrix.m4 = clipX + clipW;
	clipMatrix.m5 = clipY;
	clipMatrix.m6 = 0;
	clipMatrix.m7 = 1;

	clipMatrix.m8 = clipX + clipW;
	clipMatrix.m9 = clipY + clipH;
	clipMatrix.m10 = 0;
	clipMatrix.m11 = 1;

	clipMatrix.m12 = clipX;
	clipMatrix.m13 = clipY + clipH;
	clipMatrix.m14 = 0;
	clipMatrix.m15 = 1;

	clipMatrix = CMath::Matrix4Multiply(clipMatrix, tranf);

	outX = clipMatrix.m0;
	outY = clipMatrix.m1;
	outW = clipMatrix.m4 - clipMatrix.m0;
	outH = clipMatrix.m9 - clipMatrix.m1;
}
void HSGame::ComboResetTime()
{
	int param = CGameDesign::FileLevel::CSheetGeneral::paramater;
	_game_combo_timereset = GetDesignGeneralint(DESIGN_PARAM_COMBO_TIME, param);
}
#pragma region EFFECT_COMBO
void HSGame::EffectComboInit()
{
	for (int i = 0; i < EFFECT_COMBO_NUM; i++)
	{
		_effect_combo[i].Init();
	}
}
void HSGame::EffectComboAdd(float x, float y, float scale, float scalesps, float opac, float opacsps, float speed, int value, int colortype)
{
	for (int i = 0; i < EFFECT_COMBO_NUM; i++)
	{
		if (_effect_combo[i]._state == 0)
		{
			_effect_combo[i].Add(x, y, scale, scalesps, opac, opacsps, speed, value, colortype);
			break;
		}
	}
}
void HSGame::EffectComboRender(float x, float y)
{
	for (int i = 0; i < EFFECT_COMBO_NUM; i++)
	{
		_effect_combo[i].Render(x, y);
	}
}
void HSGame::EffectComboUpdate()
{
	for (int i = 0; i < EFFECT_COMBO_NUM; i++)
	{
		_effect_combo[i].Update();
	}
}
void HSGame::EffectComboForceEnd()
{
	for (int i = 0; i < EFFECT_COMBO_NUM; i++)
	{
		_effect_combo[i].ForceEnd();
	}
}
void HSGame::SEffectCombo::Init()
{
	_x = 0;
	_y = 0;
	_scale = 1;
	_scale_speed = 0;
	_opac = 100;
	_opac_speed = 0;
	_state = 0;
	_value = 0;
	_speed_scroll = 0;
	_flash_opac = 0;
	_flash_state = 0;
	_color_type_old = -1;
	_color_type = -1;
}
void HSGame::SEffectCombo::Add(float x, float y, float scale, float scalesps, float opac, float opacsps, float speed, int value, int colortype)
{
	_x = x;
	_y = y;
	_scale = scale;
	_scale_speed = scalesps;
	_opac = opac;
	_opac_speed = opacsps;
	_state = 1;
	_value = value;
	_speed_scroll = speed;
	_color_type = colortype;
	_flash_opac = 100;
	_flash_state = 0;
	_color_type_old = _color_type;
}
void HSGame::SEffectCombo::Render(float x, float y)
{
	if (_state == 0 || _value == 0)
	{
		return;
	}
	HSGame* game = (HSGame*)(GAME()->_current_game);
	int charspacing = GAME()->GetSpriteVer(SPRITEID_FONT_COMBO)->_char_spacing;
	GAME()->GetSpriteVer(SPRITEID_FONT_COMBO)->SetCharSpacing(4);
	COLOR _color = 0xffffffff;
	switch (_color_type)
	{
	case 0:
		_color = 0xffa642b8;
		break;
	case 1:
		_color = 0xffe8d469;
		break;
	case 2:
		_color = 0xfff3616f;
		break;
	case 3:
		_color = 0xff00a2ff;
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
	float scale_plus = 0;
	if (_value > 17)
	{
		scale_plus = 0.1f;
		switch (_flash_state)
		{
		case 0:
			_flash_opac += 10;
			if (_flash_opac >= 160)
			{
				_flash_opac = 0;
				_flash_state = 1;
				int newtype = CMath::RANDOM(0, 5);
				while (newtype == _color_type_old)
				{
					newtype = CMath::RANDOM(0, 5);

				}
				_color_type = newtype;
				_color_type_old = _color_type;
			}
			break;
		case 1:
			_flash_opac -= 10;
			if (_flash_opac <= -60)
			{
				_flash_opac = 100;
				_flash_state = 0;
			}
			break;
		default:
			break;
		}
		
	}
	G()->SetColorMask(_color);
	G()->SetOpacity(_opac);
	G()->SetScale(_scale + scale_plus, _scale + scale_plus, _x + x, _y + y);
	GET_SPRITE(SPRITEID_FONT_COMBO)->DrawAnsiTextFormat(G(), _x + x, _y + y, _HCENTER, ":%d", _value);
	G()->ClearScale();
	G()->SetOpacity(100);
	G()->ClearColorMask();
	if (_value > 17)
	{
		G()->SetOpacity(_flash_opac * (_opac/180));
		G()->SetScale(_scale + scale_plus, _scale + scale_plus, _x + x, _y + y);
		GET_SPRITE(SPRITEID_FONT_COMBO)->DrawAnsiTextFormat(G(), _x + x, _y + y, _HCENTER, ":%d", _value);
		G()->ClearScale();
		G()->SetOpacity(100);
	}
	
	GAME()->GetSpriteVer(SPRITEID_FONT_COMBO)->SetCharSpacing(charspacing);
}
void HSGame::SEffectCombo::Update()
{
	if (_state == 0)
	{
		return;
	}
	switch (_state)
	{
	case 1:
		_scale -= _scale_speed;
		_scale_speed *= 1.1f;
		if (_scale <= 1)
		{
			_scale = 1.0f;
			_opac--;
			if (_opac <= 180)
			{
				_state = 2;
			}
			_y -= _speed_scroll;
		}
		break;
	case 2:
		_y -= _speed_scroll;
		_opac -= _opac_speed;
		if (_opac <= 0)
		{
			_state = 0;
		}
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 10:
		_y -= _speed_scroll;
		_opac -= 20;
		if (_opac <= 0)
		{
			_state = 0;
		}
		break;
	default:
		break;
	}
}
void HSGame::SEffectCombo::ForceEnd()
{
	if (_state == 0)
	{
		_state = 10;
	}
}
#pragma endregion

#pragma region FLIPPING
void HSGame::EffectFlippingActive(int id)
{
	switch (id)
	{
	case 0://new high score
	{
		char text[50];
		int lenght = sprintf(text, "%s", "new highscore");
		int strw = 0;
		int strx = 0;
		int count = 0;
		for (int i = 0; i < lenght; i++)
		{
			if (text[i] != ' ')
			{
				strw += GET_SPRITE(SPRITEID_OBJECT_HIGHSCORE)->GetFModuleWidth(count, 0) + 8;
				count++;
			}
			else
			{
				strw += 40;
			}
		}
		count = 0;
		for (int i = 0; i < lenght; i++)
		{
			int w = GET_SPRITE(SPRITEID_OBJECT_HIGHSCORE)->GetFModuleWidth(count, 0) + 8;
			int px = k_SCREEN_WIDTH/2 - strw/2 + strx + w/2;
			int py = k_SCREEN_HEIGHT/3;
			if (text[i] != ' ')
			{
				_new_highscore.Add(count, px, py, 0, 1, 0.06f, 0.0f, count * 7, count);
				strx += w;
				count++;
			}
			else
			{
				strx += 40;
			}
		}
	}
		break;
	default:
		break;
	}
}

void HSGame::SEffectFlipping::Init(int spriteid, int type, int num, int timehold)
{
	_spriteid = spriteid;
	_type = type;
	_num = num;
	_hold_time_state = timehold;
	for (int i = 0; i < _num; i++)
	{
		_x[i] = 0;
		_y[i] = 0;
		_scalex[i] = 1;
		_scaley[i] = 1;
		_scale_spsx[i] = 0;
		_scale_spsy[i] = 0;
		_hold_time[i] = 0;
		_state[i] = 0;
		_frameid[i] = 0;
	}
	
}
void HSGame::SEffectFlipping::Add(int i, float x, float y, float scalex, float scaley, float scalespsx, float scalespsy, int holdtime, int frame)
{
	_x[i] = x;
	_y[i] = y;
	_scalex[i] = scalex;
	_scaley[i] = scaley;
	_scale_spsx[i] = scalespsx;
	_scale_spsy[i] = scalespsy;
	_hold_time[i] = holdtime;
	_state[i] = 1;
	_frameid[i] = frame;
}
void HSGame::SEffectFlipping::Render(float x, float y)
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{
			continue;
		}
		G()->SetScale(_scalex[i], _scaley[i], _x[i] + x, _y[i] + y);
		GET_SPRITE(_spriteid)->DrawFrame(G(), _frameid[i], _x[i] + x, _y[i] + y);
		G()->ClearScale();
	}
}
void HSGame::SEffectFlipping::Update()
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
	bool completed_all = true;
	bool active_all = true;
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 0)
		{
			active_all = false;
			continue;
		}
		if (_hold_time[i] > 0)
		{
			_hold_time[i]--;
			continue;
		}
		if (_state[i] != 2)
		{
			completed_all = false;
		}
		switch (_state[i])
		{
		case 1:
			_scalex[i] += _scale_spsx[i];
			_scaley[i] += _scale_spsy[i];
			if (_scalex[i] > 1.0f || _scaley[i] > 1.0f)
			{
				_scalex[i] = 1;
				_scaley[i] = 1;
				_state[i] = 2;
			}
			break;
		case 2:

			break;
		case 3:
			_scalex[i] -= _scale_spsx[i];
			_scaley[i] -= _scale_spsy[i];
			if (_scalex[i] < 0.0f || _scaley[i] < 0.0f)
			{
				_scalex[i] = 0;
				_scaley[i] = 0;
				_state[i] = 0;
			}
			break;
		default:
			break;
		}
	}
	if (completed_all && active_all)
	{
		//printf("\n _hold_time_state:%d", _hold_time_state);
		_hold_time_state--;
		if (_hold_time_state % 10 == 0)
		{
			PLAY_SFX(CMath::RANDOM(SFX_FIREWORKS1, SFX_FIREWORKS3));
			float px = CMath::RANDOM(-20,20)*10 + k_SCREEN_WIDTH/2;
			float py = CMath::RANDOM(-5, 5) * 10 + k_SCREEN_HEIGHT/3;
			game->HighScoreParticleAdd(px,py);
		}
		if (_hold_time_state <= 0)
		{
			for (int i = 0; i < _num; i++)
			{
				_hold_time[i] = i * 7;
				_state[i] = 3;
			}
		}
	}
}
void HSGame::SEffectFlipping::ForceEnd()
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] == 2)
		{
			_hold_time[i] = i * 7;
			_state[i] = 3;
		}
	}
}
bool HSGame::SEffectFlipping::IsFree()
{
	for (int i = 0; i < _num; i++)
	{
		if (_state[i] != 0)
		{
			return false;
		}
	}
	return true;
}
void HSGame::HighScoreParticleInit()
{
	for (int i = 0; i < 5; i++)
	{
		_highscore_particle[i].Init(30);
	}
}
void HSGame::HighScoreParticleAdd(float x, float y)
{
	for (int i = 0; i < 5; i++)
	{
		if (_highscore_particle[i]._status == 0)
		{
			_highscore_particle[i].Active(x,y);
			break;
		}
	}
}
void HSGame::HighScoreParticleRender(float x, float y)
{
	for (int i = 0; i < 5; i++)
	{
		_highscore_particle[i].Render(x, y);
	}
}
void HSGame::HighScoreParticleUpdate()
{
	for (int i = 0; i < 5; i++)
	{
		_highscore_particle[i].Update();
	}
}

#pragma endregion

void HSGame::PushNotify()
{
#if defined(MAC_OS)
    printf("\n _user._isShowNotifi:%d",_user._isShowNotifi);
    if(_user._isShowNotifi)
    {
        
        Device::_current_num_notification = 0;
       /* Device::AddNotification("Will you going higher?", 1*60,false);
        Device::AddNotification("Will you going higher?", 7*60,false);*/
        Device::AddNotification("Will you going higher?", 1*24*60*60,false);
        Device::AddNotification("Will you going higher?", 7*24*60*60,false);
    }
#endif
}
#pragma region CHAR_SELECT
void HSGame::CharSelectRender(float x, float y)
{
	for (int i = 0; i < CHAR_SPRITEID_NUM; i++)
	{
		_char_select[i].Render(x, y);
	}
}
void HSGame::CharSelectInit()
{
	_char_select_scrollx = 0;
	_char_select_scrollx_tar = 0;
	_char_select_scroll_state = 0;
	_char_selecte_scrolling = false;
	for (int i = 0; i < CHAR_SPRITEID_NUM; i++)
	{
		int posx = k_SCREEN_WIDTH/2 + SIZE_FLATFORM_WIDTH*1.5f*i - (SIZE_CHAR_WIDTH*_port_scale_x) / 2;
		int posy = k_SCREEN_HEIGHT - SIZE_FLATFORM_HEIGHT - SIZE_FLATFORM_HEADER_HEIGHT - SIZE_CHAR_HEIGHT*_port_scale_x;
		int sprite = _char_spriteid[i];
		int param_name = CGameDesign::FileLevel::CSheetcharacter::name;
		bool lock = true;
		if (i == 0)
		{
			lock = false;
		}
		else
		{
			if (_user._isRating || _user._hadRemovedAds)
			{
#ifndef ANDROID_OS
				lock = false;		//remove unlock character on rating
#endif	
			}
		}
		_char_select[i].Init(posx,posy, sprite,GetDesignCharString(i, param_name),lock);
	}
}
void HSGame::CharSelectUpdate()
{
	switch (_char_select_scroll_state)
	{
	case 1:
		_char_select_scrollx += 15;
		if (_char_select_scrollx > _char_select_scrollx_tar+15)
		{
			_char_select_scrollx = _char_select_scrollx_tar+15;
			_char_select_scroll_state = 2;
		}
		break;
	case 2:
			_char_select_scrollx -= 3;
			if (_char_select_scrollx < _char_select_scrollx_tar - 5)
			{
				_char_select_scrollx = _char_select_scrollx_tar - 5;
				_char_select_scroll_state = 3;
			}
		break;
	case 3:
			_char_select_scrollx += 1;
			if (_char_select_scrollx >= _char_select_scrollx_tar)
			{
				_char_select_scrollx = _char_select_scrollx_tar;
				_char_select_scroll_state = 4;
				_char_selecte_scrolling = false;
				//_char_selected = (int)(_char_select_scrollx / SIZE_FLATFORM_WIDTH*1.5f);
			}
		break;
	case 5:
			_char_select_scrollx -= 15;
			if (_char_select_scrollx < _char_select_scrollx_tar - 15)
			{
				_char_select_scrollx = _char_select_scrollx_tar - 15;
				_char_select_scroll_state = 6;
			}
		break;
	case 6:
		_char_select_scrollx += 3;
		if (_char_select_scrollx > _char_select_scrollx_tar + 5)
		{
			_char_select_scrollx = _char_select_scrollx_tar + 5;
			_char_select_scroll_state = 7;
		}
		break;
	case 7:
		_char_select_scrollx -= 1;
		if (_char_select_scrollx <= _char_select_scrollx_tar)
		{
			_char_select_scrollx = _char_select_scrollx_tar;
			_char_select_scroll_state = 8;
			_char_selecte_scrolling = false;
			//_char_selected = (int)(_char_select_scrollx / SIZE_FLATFORM_WIDTH*1.5f);
		}
		break;
	default:
		break;
	}
	for (int i = 0; i < CHAR_SPRITEID_NUM; i++)
	{
		_char_select[i].Update();
	}
}
void HSGame::SCharSelect::Init(float x, float y, int sprite, const char*name, bool islock)
{
	_x = x;
	_y = y;
	_tarx = x;
	_tary = y;
	_spriteid = sprite;
	_is_lock = islock;
	sprintf(_name, "%s", name);
	_speed = 0;
	_state_move = 0;
	_angle = 0;
}
void HSGame::SCharSelect::Render(float x, float y)
{
	HSGame *game = (HSGame*)(GAME()->_current_game);
	float dis = CMath::DISTANCE(_x + x, _y + y, k_SCREEN_WIDTH / 2, _y + y);
	float opac = 120 - dis;
	G()->SetOpacity(game->_char_selecte_opac*opac*0.01f);
	G()->SetClip(k_SCREEN_WIDTH / 2 - 150, 0, 300, k_SCREEN_HEIGHT);
	G()->SetScale(game->_port_scale_x, game->_port_scale_x, _x + x, _y + y);
	if (_is_lock)
	{
		GET_SPRITE(SPRITEID_HUD_RESULT)->DrawFrame(G(), 17, _x + x, _y + y);
	}
	else
	{
		GET_SPRITE(_spriteid)->DrawFrame(G(), 0, _x + x, _y + y);
		GET_SPRITE(_spriteid)->DrawFrame(G(), 2, _x + x, _y + y);
	}
	G()->ClearScale();
	int charspacing = GAME()->GetSpriteVer(SPRITEID_FONT_MISSION_01)->_char_spacing;
	GAME()->GetSpriteVer(SPRITEID_FONT_MISSION_01)->SetCharSpacing(3);
	GET_SPRITE(SPRITEID_FONT_MISSION_01)->DrawAnsiTextFormat(G(), _x + x + SIZE_CHAR_WIDTH*game->_port_scale_x/2, _y + y - 38, _HCENTER, "%s", _name);
	GAME()->GetSpriteVer(SPRITEID_FONT_MISSION_01)->SetCharSpacing(charspacing);
	G()->ClearClip();
	G()->SetOpacity(100);
}
void HSGame::SCharSelect::Update()
{
	switch (_state_move)
	{
	case 1:
		break;
	case 2:
		break;
	default:
		break;
	}
	if (CHECK_POINT_IN_CIRCLE(_x,_y,_tarx,_tary,_speed))
	{
		_x = _tarx;
		_y = _tary;
	}
	else
	{
		_x += _speed*CMath::COS(_angle);
		_y += _speed*CMath::SIN(_angle);
	}
}
void HSGame::SCharSelect::SetTar(float x, float y)
{
	_tarx = x;
	_tary = y;
	_angle = CMath::ANGLE(_x, _y, _tarx, _tary);
	_speed = CMath::DISTANCE(_x, _y, _tarx, _tary)/10.0f;
}
#pragma endregion

#pragma region TUTORIAL
void HSGame::TutorialInit()
{
	_tutorial_state = 0;
	_tutorial_zoomX = 0;
	_tutorial_touch_id = -1;
	_tutorial_opac = 0;
	_tutorial_circle_zoom = 1.0f;
}
void HSGame::TutorialRender(float x, float y)
{
	if (_tutorial_state == 0)
	{
		return;
	}
	int mx = k_SCREEN_WIDTH/2;
	int my = 400;
	
	switch (_user._stepTutorial)
	{
	case 0:
		_grap->SetOpacity(_tutorial_opac);
		_grap->SetColor(0xff000000);
		_grap->FillFullScreen(true);
		_grap->SetOpacity(100);
		_grap->SetScale(_tutorial_zoomX, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 2, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 4, mx, my);
		_grap->ClearScale();
		break;
	case 1:
		_grap->SetScale(_tutorial_zoomX, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 5, mx, my);
		_grap->ClearScale();
		_grap->SetScale(_tutorial_circle_zoom, _tutorial_circle_zoom, _control[2]._x + _control[0]._w / 2, _control[2]._y + SIZE_FLATFORM_HEIGHT / 2);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 0, _control[2]._x + _control[0]._w / 2, _control[2]._y + SIZE_FLATFORM_HEIGHT / 2);
		_grap->ClearScale();
		break;
	case 2:
		_grap->SetOpacity(_tutorial_opac);
		_grap->SetColor(0xff000000);
		_grap->FillFullScreen(true);
		_grap->SetOpacity(100);
		_grap->SetScale(_tutorial_zoomX, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 6, mx, my);
		_grap->ClearScale();
		break;
	case 3:
		_grap->SetScale(_tutorial_zoomX, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 2, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 7, mx, my);
		_grap->ClearScale();
		_grap->SetScale(_tutorial_circle_zoom, _tutorial_circle_zoom, _control[3]._x + _control[0]._w / 2, _control[3]._y + SIZE_FLATFORM_HEIGHT / 2);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 0, _control[3]._x + _control[0]._w / 2, _control[3]._y + SIZE_FLATFORM_HEIGHT / 2);
		_grap->ClearScale();
		break;
	case 4:
		_grap->SetOpacity(_tutorial_opac);
		_grap->SetColor(0xff000000);
		_grap->FillFullScreen(true);
		_grap->SetOpacity(100);
		_grap->SetScale(_tutorial_zoomX, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 2, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 8, mx, my);
		_grap->ClearScale();
		break;
	case 5:
		_grap->SetOpacity(_tutorial_opac);
		_grap->SetColor(0xff000000);
		_grap->FillFullScreen(true);
		_grap->SetOpacity(100);
		_grap->SetScale(_tutorial_zoomX, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 9, mx, my);
		_grap->ClearScale();
		break;
	case 6:
		_grap->SetScale(_tutorial_zoomX, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 10, mx, my);
		_grap->ClearScale();
		_grap->SetScale(_tutorial_circle_zoom, _tutorial_circle_zoom, _control[2]._x + _control[0]._w / 2, _control[2]._y + SIZE_FLATFORM_HEIGHT / 2);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 0, _control[2]._x + _control[0]._w / 2, _control[2]._y + SIZE_FLATFORM_HEIGHT / 2);
		_grap->ClearScale();
		break;
	case 7:
		_grap->SetScale(_tutorial_zoomX, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 11, mx, my);
		_grap->ClearScale();
		_grap->SetScale(_tutorial_circle_zoom, _tutorial_circle_zoom, _control[1]._x + _control[0]._w / 2, _control[1]._y + SIZE_FLATFORM_HEIGHT / 2);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 0, _control[1]._x + _control[0]._w / 2, _control[1]._y + SIZE_FLATFORM_HEIGHT / 2);
		_grap->ClearScale();
		break;
	case 8:
		_grap->SetScale(_tutorial_zoomX, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 12, mx, my);
		_grap->ClearScale();
		_grap->SetScale(_tutorial_circle_zoom, _tutorial_circle_zoom, _control[0]._x + _control[0]._w / 2, _control[0]._y + SIZE_FLATFORM_HEIGHT / 2);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 0, _control[0]._x + _control[0]._w / 2, _control[0]._y + SIZE_FLATFORM_HEIGHT / 2);
		_grap->ClearScale();
		break;
	case 9:
		_grap->SetOpacity(_tutorial_opac);
		_grap->SetColor(0xff000000);
		_grap->FillFullScreen(true);
		_grap->SetOpacity(100);
		_grap->SetScale(_tutorial_zoomX, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 2, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 13, mx, my);
		_grap->ClearScale();
		break;
	case 10:
		_grap->SetOpacity(_tutorial_opac);
		_grap->SetColor(0xff000000);
		_grap->FillFullScreen(true);
		_grap->SetOpacity(100);
		_grap->SetScale(_tutorial_zoomX, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 2, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 14, mx, my);
		_grap->ClearScale();
		_grap->SetScale(0.5f, 0.5f, _food._x, _food._y);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 0, _food._x, _food._y);
		_grap->SetScale(_tutorial_zoomX, 1, mx, my);
		_food.Render(x, y);
		break;
	case 11:
		_grap->SetOpacity(_tutorial_opac);
		_grap->SetColor(0xff000000);
		_grap->FillFullScreen(true);
		_grap->SetOpacity(100);
		_grap->SetScale(_tutorial_zoomX, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 1, mx, my);
		GET_SPRITE(SPRITEID_HUD_TUT_01)->DrawFrame(_grap, 15, mx, my);
		_grap->ClearScale();
		break;
	}
}
void HSGame::TutorialUpdate()
{
	if (_tutorial_state == 0)
	{
		return;
	}
	if (_tutorial_circle_zoom > 0.1f)
	{
		_tutorial_circle_zoom *= 0.95f;
		if (_tutorial_circle_zoom <= 0.1f)
		{
			_tutorial_circle_zoom = 1.0f;
		}
	}
	if (_user._stepTutorial == 1 || _user._stepTutorial == 3
		|| _user._stepTutorial == 6 || _user._stepTutorial == 7
		|| _user._stepTutorial == 8)
	{
		for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
		{
			_control[i]._is_lock = true;
			switch (_user._stepTutorial)
			{
			case 1:
				_control[2]._is_lock = false;
				break;
			case 3:
				_control[3]._is_lock = false;
				break;
			case 6:
				_control[2]._is_lock = false;
				break;
			case 7:
				_control[1]._is_lock = false;
				break;
			case 8:
				_control[0]._is_lock = false;
				break;
			default:
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
		{
			_control[i]._is_lock = true;
		}
		
	}
	TutorialUpdateTouch();
	switch (_tutorial_state)
	{
	case 1:
		_tutorial_opac += 3;
		if (_tutorial_opac >= 50)
		{
			_tutorial_opac = 50;
		}
		_tutorial_zoomX += 0.1;
		if (_tutorial_zoomX >= 1.2)
		{
			_tutorial_zoomX = 1.2;
			_tutorial_state = 2;
		}
		break;

	case 2:
		_tutorial_zoomX -= 0.05;
		if (_tutorial_zoomX <= 0.9)
		{
			_tutorial_zoomX = 0.9;
			_tutorial_state = 3;
		}
		break;

	case 3:
		_tutorial_zoomX += 0.03;
		if (_tutorial_zoomX >= 1)
		{
			_tutorial_zoomX = 1;
			_tutorial_state = 4;
		}
		break;

	case 5:
		_tutorial_zoomX -= 0.1;
		_tutorial_opac -= 5;
		if (_tutorial_opac <= 0)
		{
			_tutorial_opac = 0;
		}
		if (_tutorial_zoomX <= 0)
		{
			_tutorial_zoomX = 0;
			_tutorial_opac = 0;
			_tutorial_state = 0;
			_user._stepTutorial++;
			if (_user._stepTutorial >= 12)
			{
				_user._stepTutorial = 100;
				_user.UserDataSave();
				_tutorial_touch_id = -1;
				for (int i = 0; i < CONTROL_NUM_BUTTON; i++)
				{
					_control[i]._is_lock = false;
				}
				_time_delay_show_ads = 0;
			}
		}
		break;

	default:
		break;
	}
}
void HSGame::TutorialSetState(int state)
{
	if (_tutorial_state == state)
	{
		return;
	}
	_tutorial_state = state;
	if (_tutorial_state == 1)
	{
		PLAY_SFX(SFX_POPUP);
		_tutorial_zoomX = 0;
		_tutorial_circle_zoom = 1.0f;
	}
}
bool HSGame::TutorialUpdateTouch()
{
	bool press = false;
	int index = -1;
	bool ret = false;
	
	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		int x = _point_o.X;
		int y = _point_o.Y;
		int w = _screen_width;
		int h = _screen_height;
		if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
		{
			if (TOUCHS(i)->_state == TOUCH_DOWN)
			{
				if (_user._stepTutorial != 1 && _user._stepTutorial != 3
					&& _user._stepTutorial != 6 && _user._stepTutorial != 7
					&& _user._stepTutorial != 8)
				{
					if (_tutorial_touch_id == -1)
					{
						printf("\n _user._stepTutorial:%d", _user._stepTutorial);
						
						_tutorial_touch_id = TOUCHS(i)->_id;
						_tutorial_state = 5;
						press = true;
						TOUCHS(i)->_state = TOUCH_DRAG;
					}
				}
			}
			else if (TOUCHS(i)->_state == TOUCH_DRAG)
			{
				press = true;
			}
			else if (TOUCHS(i)->_state == TOUCH_UP)
			{
				if (_tutorial_touch_id == TOUCHS(i)->_id)
				{
					_tutorial_touch_id = -1;
					press = true;
				}
			}
		}
	}
	return press;
}
#pragma endregion
void HSGame::BGRender(float x, float y)
{
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
	x += _shakex;
	y += _shakey;
	
	ObjectEnvirRender(x, y);
	_particle_fly.Render(x, y);
	_particle_lighting.RenderParticleLight(x, y, 100);
	//_particle_rain.RenderParticleLight(x, y, 100);
	if (_bg_spriteid_current != 2)
	{
		RenderEffectLight(x, -150, 100);
	}
	ControlRender(x, y, 100);
}