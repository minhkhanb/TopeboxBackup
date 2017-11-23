// Pine-Entertainment
// Author: Duy.Nguyen
// Last update: 10/2014

#include "pch.h"
#include "ButtonAnimation.h"
using namespace Game;
using namespace HUD;
ButtonAnimation::ButtonAnimation() :
_curr_sprite(NULL), _padding(0.0f)
{

}

ButtonAnimation::~ButtonAnimation(){

}

void ButtonAnimation::Update(){
	_anim.Update();
}

void ButtonAnimation::Render(Graphic* g){

	_anim.Pos = Pos;
	if (_anim.Visible) _anim.Render(g);
}

void ButtonAnimation::Init(CSprite* sprt, int anim_lock, int anim_normal, int anim_hover, int anim_active){
	
	Release();
	_anim_lock = anim_lock;
	_anim_normal = anim_normal;
	_anim_hover = anim_hover;
	_anim_active = anim_active;
	_curr_sprite = sprt;
	State = HUD::ButtonState::Normal;
	if (sprt != NULL){
		_anim.Init(sprt, anim_normal);
	}
}

void ButtonAnimation::Release(){
	_curr_sprite = NULL;
	_padding = 0.0f;
}

PineRectangle ButtonAnimation::GetBound(){

	int curr_aframe = _anim.GetCurrentAFrame();
	INT32 frame_id = _curr_sprite->_aframe_id[curr_aframe];
	PineRectangle bound = _curr_sprite->GetFrameBound(frame_id);
	bound.X += _curr_sprite->_aframe_x[curr_aframe];
	bound.Y += _curr_sprite->_aframe_y[curr_aframe];
	bound.X -= _padding;
	bound.Y -= _padding;
	bound.W += _padding * 2;
	bound.H += _padding * 2;
	return bound;
}

void ButtonAnimation::SetState(ButtonState state){

	State = state;
	switch (state)
	{
	case HUD::ButtonState::Normal:
		_anim.ChangeSpriteAnim(_anim_normal);
		break;
	case HUD::ButtonState::Active:
		_anim.ChangeSpriteAnim(_anim_active);
		break;
	case HUD::ButtonState::Hover:
		_anim.ChangeSpriteAnim(_anim_hover);
		break;
	case HUD::ButtonState::Lock:
		_anim.ChangeSpriteAnim(_anim_lock);
		break;
	default:
		break;
	}
}