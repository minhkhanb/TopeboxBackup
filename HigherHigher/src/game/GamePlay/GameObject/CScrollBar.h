#if !_CSCROLL_BAR_H
#define _CSCROLL_BAR_H 1
class CScrollBar
{
public:
	float _max;
	float _min;
	float _speed;
    float _speed_pined;
	float _current;
	float _target;
	float _offset;
    float _sub_speed;
    float _accel;
    bool _collided;
    
    float _mul_speed;
    int _state;
	bool _noLimit;
    bool _limit_speed_min;
    void SetAccelSpeed(float accel){_accel = accel;};
	void Init(int min,int max, float speed,float mul=1);
	void Update();
	void AddOffset(float offset);
	void SetTarget(float target);
	void ValidateRange();
    void ValidateRangeExt();
	void Pause();
    void Set();
	void Resume();
    void End();
    
    float GetCurrent(){return _current;}
    bool IsScrolling(){return _current == _target && _state != 0;};
    void ReduceOffset(float mul);
    void ForceToTarget();
    float GetDistace();
    void SetState(int state){_state =state;};
	
};

#endif