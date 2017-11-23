#if !C_OBJECENVIR_H
#define C_OBJECENVIR_H

enum enumObjectEnvirType {
	OBJECT_ENVIR_TYPE_NORMAL,
	OBJECT_ENVIR_TYPE_OB1,
	OBJECT_ENVIR_TYPE_OB2,
};
#define OFFSET_POLY_ROW		50
#define OFFSET_POLY_COL		50
#define OFFSET_POLY_W		96
#define OFFSET_POLY_H		80
class CObjectEnvir
{
public:
	CObjectEnvir();
	~CObjectEnvir();
	int _layer;
	float _x, _y;
	float _tarx, _tary;
	int _spriteid;
	int _frameid;
	int _type;
	int _opac;
	int _opac_speed;
	float _scale;
	float _speed;
	void Init();
	void Add(int sprite, int frame, float x, float y, int layer, int type = OBJECT_ENVIR_TYPE_NORMAL);
	void Render(float x, float y);
	void Update();
	void SetPos(float x, float y);
	void SetTarget(float x, float y,float speed);
private:
	CGame* _game;
	Graphic* _grap;
};


#endif
