#include "pch.h"
#include "CAnimObject.h"
#include "./CGame.h"
CAnimObject::CAnimObject()
{
	_anim_sprite = NULL;
	_current_anim = -1;
	_current_aframes = -1;
	_current_aframes_time = 0;
	_anim_status = ANIM_STOP;
	_is_used = false;
	_palette = -1;
	_overlay_module = false;
	_sub_time = 0;
	_sub_loop_time = 0;
}

CAnimObject::CAnimObject(CSprite* anim_spr)
{
	_anim_sprite = anim_spr;
	_current_anim = -1;
	_current_aframes = -1;
	_current_aframes_time = 0;
	_anim_status = ANIM_STOP;
	_is_used = true;
	_palette = -1;
	_overlay_module = false;
	_sub_time = 0;
	_sub_loop_time = 0;
}

CAnimObject::CAnimObject(CSprite* anim_spr, int x, int y)
{
	_anim_sprite = anim_spr;
	_current_anim = -1;
	_current_aframes = -1;
	_current_aframes_time = 0;
	_anim_status = ANIM_STOP;
	_is_used = true;
	
	_x = x;
	_y = y;
	_palette = -1;
	_overlay_module = false;
	_sub_time = 0;
	_sub_loop_time = 0;
}

CAnimObject::~CAnimObject()
{
	_anim_sprite = NULL;
}

void CAnimObject::SetAnimSprite(CSprite* anim_spr,int index)
{
	
	_anim_sprite = anim_spr;
	_current_anim = -1;
	_current_aframes = -1;
	_current_aframes_time = 0;
	_anim_status = ANIM_STOP;
	_overlay_module = false;
	_intdexSprite = index;
	//CGame::_this->AddAnimStock(this,index);
	_sub_time = 0;
}

void CAnimObject::SetAnim(int anim, bool is_loop, int transform)
{
	if (anim >= _anim_sprite->_num_anims)
	{
		return;
	}
	
	_current_anim = anim;
	ResetAnim();
	
	_is_loop = is_loop;
	_transform = transform;
	_anim_status = ANIM_PLAY;

	SetSubTime(0);
	_sub_loop_time = 0;

	_current_aframes = _anim_sprite->GetFirstAFrameID(_current_anim);
	_current_aframes_time = _anim_sprite->GetFirstAFrameTime(_current_anim);
}

void CAnimObject::SetPalette(INT8 palette)
{
	if (palette >= _anim_sprite->_num_pal)
	{
		_palette = 0;
		return;
	}
	_palette = palette;
}


void CAnimObject::StopAnim()
{
	ResetAnim();
}

void CAnimObject::PauseAnim()
{
	_anim_status = ANIM_PAUSE;
}

void CAnimObject::ResetAnim()
{
	_current_aframes = _anim_sprite->GetFirstAFrameID(_current_anim);
	_current_aframes_time = _anim_sprite->GetFirstAFrameTime(_current_anim);;
	_anim_status = ANIM_STOP;
	_overlay_module = false;
}

void CAnimObject::SetPos(float x, float y)
{
	_x = x;
	_y = y;
}

void CAnimObject::DrawAnimObject(Graphic* g)
{
	if ((_anim_status == ANIM_STOP && !_is_loop) || _anim_sprite == NULL)
	{
		return;
	}
	_anim_sprite->SetAnim(_current_anim, _current_aframes, _current_aframes_time);
	//_anim_sprite->DrawCurrentAFrame(g, _current_anim, _x, _y, _transform, _palette);
	_anim_sprite->DrawCurrentAFrame(g, _current_anim, _x, _y, _transform, _palette,_overlay_module,_module_need_overlay_id,_sprite_overlay,_module_overlay_id);
	
}

void CAnimObject::UpdateAnimObject()
{
	if ((_anim_status == ANIM_STOP && !_is_loop) && _anim_sprite == NULL)
	{
		return;
	}
	
	if (_anim_status == ANIM_PLAY)
	{
		_anim_sprite->SetAnim(_current_anim, _current_aframes, _current_aframes_time);
		if(_current_aframes_time + _sub_time <= 0)
		{
			_current_aframes_time = 1;
		}
		if(_current_aframes_time<= 1)
		{
			_sub_loop_time--;
			//printf("\n _sub_loop_time = %d", _sub_loop_time);
			if(_sub_loop_time <= 0)
			{
				_sub_loop_time  =  _sub_time;
				//_sub_time  =  _sub_loop_time;
				_anim_sprite->NextAFrame(_is_loop);
			}
		}
		else
		{
			_anim_sprite->NextAFrame(_is_loop);
		}
		
		if (_anim_sprite->IsAnimStop(_current_anim))
		{
			StopAnim();
		}
		else
		{
			_current_aframes = _anim_sprite->_current_aframes;
			_current_aframes_time = _anim_sprite->_current_aframes_time;
		}
	}
}

bool CAnimObject::IsInside(int x, int y)
{
	return _anim_sprite->IsInside(x - _x, y - _y, _current_aframes);
}

int* CAnimObject::GetCollisionCircle(int x, int y, int r)
{
	int *col = _anim_sprite->GetAFCollisionCircle(_current_aframes, x - _x, y - _y, r);
	if (col!=NULL)
	{
		col[0] += _x;
		col[1] += _y;
	}
	return col;
	
}

int* CAnimObject::GetBoundaryCircle()
{
	if (_anim_sprite!=NULL)
	{
		int *col = _anim_sprite->GetAFBoundaryCircle(_current_aframes);
		if (col!=NULL)
		{
			col[0] += _x;
			col[1] += _y;
		}
		return col;
	}
	else
	{
		return NULL;
	}
	
}

void CAnimObject::DrawAnimObject(Graphic* g, bool is_loop)
{
	_is_loop = is_loop;
		
	DrawAnimObject(g);
}

void CAnimObject::DrawAnimObject(Graphic* g, float x, float y, bool is_loop)
{
	_x = x;
	_y = y;
	_is_loop = is_loop;
		
	DrawAnimObject(g);
}
void CAnimObject::DrawAnimObject(Graphic* g, int transform)
{
	if (_anim_status == ANIM_STOP || _anim_sprite == NULL)
	{
		return;
	}
	
	_anim_sprite->SetAnim(_current_anim, _current_aframes, _current_aframes_time);
	_anim_sprite->DrawCurrentAFrame(g, _current_anim, _x, _y, transform, _palette,_overlay_module,_module_need_overlay_id,_sprite_overlay,_module_overlay_id);
}
void CAnimObject::DrawAnimObject(Graphic* g, int anim, float x, float y, bool is_loop)
{
	_x = x;
	_y = y;
	
	if (anim != _current_anim ||
		is_loop != _is_loop)
	{
		SetAnim(anim, is_loop);
	}
		
	DrawAnimObject(g);
}

bool CAnimObject::IsCurrentAnimStop()
{
	return (_anim_status == ANIM_STOP);
}

int CAnimObject::GetTotalTimeOfCurrentAnim()
{
	if (_anim_sprite != NULL)
	{
		return _anim_sprite->GetTotalAnimTime(_current_anim);
	}
	
	return 0;
}

int CAnimObject::GetCurrentAnim()
{
	return _current_anim;
}

int CAnimObject::GetCurrentAnimWidth()
{
	if (_anim_status == ANIM_STOP || _anim_sprite == NULL)
	{
		return -1;
	}
	
	_anim_sprite->SetAnim(_current_anim, _current_aframes, _current_aframes_time);
	return _anim_sprite->GetCurrentAFrameWidth(_current_anim);
}
int CAnimObject::GetCurrentAnimHeight()
{
	if (_anim_status == ANIM_STOP || _anim_sprite == NULL)
	{
		return -1;
	}
	
	_anim_sprite->SetAnim(_current_anim, _current_aframes, _current_aframes_time);
	return _anim_sprite->GetCurrentAFrameHeight(_current_anim);
}

void CAnimObject::CalculateCurrentAFrame()
{
	if (_anim_status == ANIM_STOP || _anim_sprite == NULL)
	{
		return;
	}
	_anim_sprite->SetAnim(_current_anim, _current_aframes, _current_aframes_time);
	_anim_sprite->CalculateCurrentAFrame(_current_anim);
}

void CAnimObject::SetOverlayModule(int module_id,CSprite *sprite,int overlay_id,int anchor)
{
	_overlay_module = true;
	_module_need_overlay_id = module_id;
	_module_overlay_id = overlay_id;
	_sprite_overlay = sprite;
	_anchor_overlay = anchor;
	//printf("\n_module_need_overlay_id = %d,_module_overlay_id = %d",_module_need_overlay_id,_module_overlay_id);
}

//hao.ly add
int CAnimObject::getMaxAframe()
{
	if (_anim_status == ANIM_STOP || _anim_sprite == NULL)
	{
		return -1;
	}
	INT32 _max_aframes = 0;
	
	/*printf("\n _anim_sprite->_num_anims = %d, _current_anim = %d, _num_aframes = %d ", _anim_sprite->_num_anims, _current_anim, _anim_sprite->_num_aframes);

	for (int i = 0; i < _anim_sprite->_num_anims; i++)
	{
		printf("\n _anim_sprite->_anim_offset[%d] = %d ", i, _anim_sprite->_anim_offset[i]);
	}*/

	if (_current_anim == (_anim_sprite->_num_anims - 1))
	{
		_max_aframes = _anim_sprite->_num_aframes - _anim_sprite->_anim_offset[_current_anim];
	}
	else
	{		
		_max_aframes = _anim_sprite->_anim_offset[_current_anim + 1] - _anim_sprite->_anim_offset[_current_anim];
	}

	return _max_aframes;
}

int CAnimObject::getMinAframe()
{
	if (_anim_status == ANIM_STOP || _anim_sprite == NULL)
	{
		return -1;
	}

	INT32 _max_aframes = 0;

	_max_aframes = _anim_sprite->GetFirstAFrameID(_current_anim);

	return _max_aframes;
}

void CAnimObject::setRandomAframe()
{
	_current_aframes = getMinAframe() + CMath::RANDOM(0, getMaxAframe() - 1);
}

void CAnimObject::setToAFrame(int aframe, int time)
{
	int maxAframe = getMaxAframe()-1;
	if(aframe > maxAframe)
	{
		aframe = maxAframe;
	}

	_current_aframes = _anim_sprite->_anim_offset[_current_anim] + aframe - 1;
	if (_current_aframes < 0) _current_aframes = 0;
	_current_aframes_time = time;

	
}
void CAnimObject::SetToFrameLast()
{
	int maxAframe = getMaxAframe()-1;
	_current_aframes = _anim_sprite->_anim_offset[_current_anim] + maxAframe - 1;
	if (_current_aframes < 0) _current_aframes = 0;
	_current_aframes_time = 0;
}
void CAnimObject::swapAnimWithSameFormat(int newAnim)
{
	int newAframe = _current_aframes - _anim_sprite->GetFirstAFrameID(_current_anim);
	newAframe += _anim_sprite->GetFirstAFrameID(newAnim);
	_current_aframes = newAframe;
	_current_anim = newAnim;

	/*GET_MAIN_CHARATER()._anim._current_anim = new_main_char_anim;
	GET_MAIN_CHARATER()._anim._anim_sprite->_current_anim = new_main_char_anim;
	int main_char_current_aframes = GET_MAIN_CHARATER()._anim._current_aframes - GET_MAIN_CHARATER()._anim._anim_sprite->GetFirstAFrameID(main_char_current_anim);
	GET_MAIN_CHARATER()._anim._current_aframes = GET_MAIN_CHARATER()._anim._anim_sprite->GetFirstAFrameID(new_main_char_anim) + main_char_current_aframes;					
	printf("\n main_char_current_anim = %d, new_main_char_anim = %d ", main_char_current_anim, new_main_char_anim);*/
}