#ifndef C_EFFECTTAIL_H
#define C_EFFECTTAIL_H

#define EFFECT_TAIL_NUM			500

class CEffectTail
{
public:
	CEffectTail();
	~CEffectTail();
	struct SEffect
	{
		float _x, _y;
		float _scale_min;
		float _scale_max;
		float _scale;
		float _scale_speed;
		float _rotate;
		float _rotate_speed;
		int _status;
		float _opac;
		float _opac_speed;
		int _opac_max;
		int _spriteid;
		int _frameid;
		void Init(int sprite, int frame);
		void Add(float x, float y, float scale, float scalemax, float speedscale, float rotate,float rotspeed, int opac, float speedopac);
		void Render(float x, float y);
		void Update();
		
	};
	int _num;
	SEffect _effect[EFFECT_TAIL_NUM];
	void EffectInit(int sprite, int frame, int num);
	void EffectRender(float x, float y);
	void EffectAdd(float x, float y, float scale, float scalemax, float speedscale, float rotate, float rotspeed, int opac, float speedopac);
	void EffectUpdate();
    void ForceEnd();
private:
	CGame* _game;
	Graphic* _grap;
};


#endif