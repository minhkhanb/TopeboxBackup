#include "pch.h"
#include "CTouch.h"


CTouch::CTouch(int id, int xx, int yy, int touch_state, int tap_count, unsigned int time)
{
	_id = id;
	x = xx;
	y = yy;
	_state = touch_state;
	_tap_count = tap_count;
	_touch_time = time;
	_flag = true;
	_is_cancelled = false;
}

CTouch::~CTouch()
{
}

void CTouch::Init(int id, int xx, int yy, int touch_state, int tap_count, unsigned int time)
{
	_id = id;
	x = xx;
	y = yy;
	_state = touch_state;
	_tap_count = tap_count;
	_touch_time = time;
	_flag = true;
	_is_cancelled = false;
}

void CTouch::Set(int xx, int yy, int touch_state, int tap_count, unsigned int time)
{
	x = xx;
	y = yy;
	_state = touch_state;
	_tap_count = tap_count;
	_touch_time = time;
	_flag = true;
}