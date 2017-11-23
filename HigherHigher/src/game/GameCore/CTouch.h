#ifndef _CTOUCH_H_
#define _CTOUCH_H_

#define TOUCH_STATE_BEGIN
#define TOUCH_STATE_MOVE
#define TOUCH_STATE_END

class CTouch
{
public:

	CTouch(int id, int xx, int yy, int touch_state, int tap_count, unsigned int time);
	~CTouch();
	
	void Init(int id, int xx, int yy, int touch_state, int tap_count, unsigned int time);
	void Set(int xx, int yy, int touch_state, int tap_count, unsigned int time);
	
public:
	int _id;
	int x;
	int y;
	int _state;
	int _tap_count;
	unsigned int _touch_time;
	bool _flag;
	bool _is_cancelled;
};

#endif