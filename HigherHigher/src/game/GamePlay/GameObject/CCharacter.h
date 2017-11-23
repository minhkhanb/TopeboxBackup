#if !C_CHARACTER_H
#define C_CHARACTER_H

enum enumCharacterOrien {
	CHARACTER_DIR_NORMAL,
	CHARACTER_DIR_LEFT,
	CHARACTER_DIR_RIGHT,
};

enum enumCharacterState {
	CHARACTER_STATE_IDLE,
	CHARACTER_STATE_RUN,
	CHARACTER_STATE_ATTACK,
	CHARACTER_STATE_DIE,
};
enum enumCharacterAnim {
	CHARACTER_ANIM_IDLE,
	CHARACTER_ANIM_RUN,
	CHARACTER_ANIM_ATTACK,
	CHARACTER_ANIM_DIE,
};

class CCharacter
{
public:
	
	CCharacter();
	~CCharacter();
	char _name[50];
	int _area;
	float _x, _y;
	float _tarx, _tary;
	float _speed;
	int _state;
	float _alpha;
	int _spriteid;
	int _orien;
	int _orien_old;
	int _w, _h;
	int _color_type;
	float _speed_increase;
	float _speed_max;
	CAnimObject _anim;
	void Init(int spriteid, int anim, float x, float y);
	void Render(float x, float y);
	void Update();
	void UpdateState();
	void UpdateOrien();
	void SetPos(float x, float y);
	void SetAlphaTarget(float alpha);
	void SetState(int state);
	void SetOrien(int orien);
	void ChangeColor(int typecolor);
	CEffectTail _effect_tail;
	int _count_frame_add_effect;
	float _die_opac;
private:
	CGame* _game;
	Graphic* _grap;
	//HSGame* _hsgame;
};

#endif
