#include "pch.h"
#include "../../GameCore/CGame.h"
#include "string.h"
#include <ctype.h>
#include "CScrollbar.h"
void CScrollBar::SetTarget(float target)
{
    _target = target;
}
float CScrollBar::GetDistace()
{
    return CMath::ABSOLUTE_VALUE(_target-_current);
}
void CScrollBar::ForceToTarget()
{
    _current = _target;
}
void CScrollBar::ReduceOffset(float mul)
{
    _sub_speed *= mul;
}
void CScrollBar::Init(int min,int max,float speed,float mul)
{
    _state = -1;
    _min = min;
    _max = max;
	_noLimit = 0;
	if(min == max && min == 0)
	{
		_noLimit = 1;
	}
    _target = min;
    _current = min;
    _speed = speed;
    _speed_pined = _speed;
    _mul_speed = mul;
    _accel = 0.90f;
    _limit_speed_min = 0.8f;
}
void CScrollBar::AddOffset(float offset)
{
    _speed = _speed_pined;
	_offset = offset;
	_target += offset;
    _sub_speed = CMath::ABSOLUTE_VALUE(offset);
    //ValidateRange();
    
   
}
void CScrollBar::Set()
{
    _target = _current;
    _offset = 0;
    _collided = false;
}
void CScrollBar::Pause()
{
	_target = _current;
    _offset = 0;
    _state = 0;
    
}
void CScrollBar::Resume()
{
	ValidateRange();
}
void CScrollBar::End()
{
    if(_offset == 0)
    {
        return;
    }
    float dis = CMath::ABSOLUTE_VALUE(_offset);
    float offset = ((_offset)>0?1:-1)*6*dis;
    //printf("\noffset = %f",offset);
    AddOffset(offset);
    _sub_speed = dis*_mul_speed;
}
void CScrollBar::ValidateRange()
{
	if(_noLimit)
	{
		return;
	}
	if(_target > _max)
	{
		_target = _max;
	}
    else
	if(_target < _min)
	{
		_target = _min;
	}
}
void CScrollBar::ValidateRangeExt()
{
	if(_noLimit)
	{
		return;
	}
    if(_target > _max+40)
    {
        
        _target = _max + 40;
        if(_current > _target)
        {
            _current = _target;
        }
    }
    else
    if(_target < _min-40)
    {
       
        _target = _min-40;
        if(_current < _target)
        {
            _current = _target;
        }
    }
    
    if(_current > _max)
	{
         _collided = true;
		
	}
    else
    if(_current < _min)
    {
         _collided = true;
    }
    else
    {
        _collided = false;
    }
    
    //printf("\n _target = %f",_target);
}
void CScrollBar::Update()
{
    //printf("\n_state = %d",_state);
	if(_state == 1)
    {
        return;
    }
    _sub_speed = _sub_speed*_accel;
    
    
    float speed = _sub_speed;
    if(_collided)
    {
        speed = _speed_pined;
        if(_current > _target)
        {
            _current = _current - speed;
            if(_current < _target)
            {
                _current = _target;
                _collided = false;
                _sub_speed = 0;
            }
        }
        else if(_current < _target)
        {
            _current = _current + speed;
            if(_current > _target)
            {
                _current = _target;
                _collided = false;
                _sub_speed = 0;
            }
        }
        ValidateRange();
        return;
    }

  
    if(speed < _limit_speed_min)
    {
        speed = 0;
        Pause();
    }
       //printf("\n_current = %f   _target = %f speed = %f",_current,_target,speed);
	if(_current > _target)
	{
		_current = _current - speed;
		if(_current < _target)
		{
			_current = _target;
            if(speed > _limit_speed_min)
            {
                _target += (2*speed*((_offset)>0?1:-1));
            }
		}
	}
	else if(_current < _target)
	{
		_current = _current + speed;
		if(_current > _target)
		{
			_current = _target;
			if(speed > _limit_speed_min)
            {
                _target += (2*speed*((_offset)>0?1:-1));
            }
		}
	}
    else
    {
        ValidateRange();
        
    }
    ValidateRangeExt();
}