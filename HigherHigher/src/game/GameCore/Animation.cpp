// Pine-Entertainment
// Author: Duy.Nguyen
// Last update: 10/2014

#include "pch.h"
#include "Animation.h"
using namespace Game;
Animation::Animation() :RenderObject(), 
_curr_sprite(NULL), _curr_anim(-1), _curr_aframe(0), _curr_aframe_time(0),
_is_stop(false), _num_loop(0),
_num_aframe(0),_frame_time(NULL), _total_frame_time(0)
{

}

Animation::~Animation(){
	Release();
}

void Animation::Init(CSprite* sprt, int anim, int loop_num){
	Release();
	_curr_sprite = sprt;
	if (_curr_sprite != NULL){
		//get min max aframe
		_min_aframe = _curr_sprite->_anim_offset[anim];
		int max_aframes = _min_aframe;
		if (anim == (_curr_sprite->_num_anims - 1))
		{
			max_aframes = _curr_sprite->_num_aframes - 1;
		}
		else
		{
			max_aframes = _curr_sprite->_anim_offset[anim + 1] - 1;
		}

		_num_aframe = max_aframes - _min_aframe + 1;
		_frame_time = new int[_num_aframe];

		_total_frame_time = 0;
		for (int f = _min_aframe; f <= max_aframes; f++){
			_frame_time[f - _min_aframe] = _curr_sprite->_aframe_time[f];
		}
	}
}

void Animation::Release(){
	_curr_sprite = NULL;
	_curr_anim = -1;
	if (_frame_time != NULL) delete[] _frame_time; _frame_time = NULL;

	_curr_aframe = 0;
	_curr_aframe_time = 0;
	_is_stop = false;
	_num_loop = 0;
	_num_aframe = 0;
	_total_frame_time = 0;
}

//Implement RenderObject
void Animation::Update(){
	if (_curr_sprite == NULL || _is_stop)return;
	_curr_aframe_time--;
	if (_curr_aframe_time <= 0){
		_curr_aframe++;
		if (_curr_aframe < _num_aframe - 1){
			_curr_aframe_time = _frame_time[_curr_aframe];
		}
		else if (_num_loop > 0 || _num_loop == -1){
			if(_num_loop != -1)_num_loop--;
			_curr_aframe = 0;
			_curr_aframe_time = _frame_time[_curr_aframe];
		}
		else{
			Stop();
			_curr_aframe--;
		}
	}
}

void Animation::Render(Graphic* g){
	if (_curr_sprite == NULL)return;
	_curr_sprite->DrawAFrame(g, _min_aframe + _curr_aframe, Pos.X, Pos.Y);
}
void Animation::Stop(){
	_is_stop = true;
}
void Animation::Resume(){
	_is_stop = false;
}
void Animation::SeekAFrame(int from, int aframe_offset){

}
void Animation::SeekToTime(int from, int time_offset){

}
bool Animation::IsStoped(){
	return _is_stop;
}
void Animation::ChangeSpriteAnim(int anim, int num_loop){
	if (_curr_sprite != NULL) Init(_curr_sprite,anim, num_loop);
}
int Animation::GetCurrentAFrame(){
	return _min_aframe + _curr_aframe;
}