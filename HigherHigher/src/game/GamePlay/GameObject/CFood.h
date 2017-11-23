#if !C_FOOD_H
#define C_FOOD_H

class CFood
{
public:
	CFood();
	~CFood();
	CEffectTail _tail;
	float _scale;
	float _scale_speed;
	float _opac;
	float _opac_speed;
	int _state;
	float _x, _y;
	float _tarx, _tary;
	float _angle;
	float _speed;
	int _spriteid;
	int _frameid;
	void Init(int sprite);
	void Add(float x, float y, int frame, float speed);
	void Render(float x, float y);
	void Update();
	void SetPos(float x, float y);
	void SetTar(float x, float y);
private:
	CGame* _game;
	Graphic* _grap;
};


#endif
