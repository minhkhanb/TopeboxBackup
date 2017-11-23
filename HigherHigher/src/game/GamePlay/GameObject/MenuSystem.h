#include "../../Lib/Math/CMath.h"
#include "../../GameCore/CAnimObject.h"
#include "../../Lib/Sprite/CSprite.h"
#include "stdio.h"
#if !_MENU_SYSTEM_H
#define _MENU_SYSTEM_H 1
#define A_NORMAL	0
#define A_HVCENTER	1

#define BUTTON_STATE_LOCK	0
#define BUTTON_STATE_NORMAL 1
#define BUTTON_STATE_SELECT 2
#define BUTTON_STATE_ACTIVE 3
#define BUTTON_STATE_HIDE	4
#define DEBUG_BUTTON 0
 
class Button
{
public:
	static Button** _button_namager;
	static int _num_button;
	static void InitButtonManager();
	static void AddButton(Button* button);
	CAnimObject* _anim;
	CAnimObject* _anim_overplay;
	static Button* _bt_actived;
    float _pos_scalex;
    float _pos_scaley;
    float GetFixX();
    float GetFixY();
	float _posX;
	float _posY;
	int _anchor;
	int _extWidth;
	int _extHeight;
	int _state;
	
	int _lockID;
	int _normalID;
	int _selectID;
	int _activeID;
	bool _sound;
	Button();
	int GetLockID(){return _lockID;};
    void SetPosScale(float scalex, float scaley);
    static void DrawFrameWithScale(CSprite* sprite,int frame_id,float scalex,float scaley,int posX = 0, int posY = 0);
	void Init(int state,float x, float y,int anchor,bool sound = true);
	void SetExt(int width, int height);
	float GetWidth();
	float GetHeight();
	void SetPos(float x, float y);
	void SetState(int state);
	bool NeedAction();
	void Render(Graphic* g,int offx=0,int offy=0,bool overplay=false);
	void Update();
    int _fixedX;
    int _fixedY;
    int _fixedW;
    int _fixedH;
	int TZ_GetX();
	int TZ_GetY();
	int TZ_GetWidth();
	int TZ_GetHeight();
	int GetCenterX(){return TZ_GetX()+TZ_GetWidth()/2;};
	int GetCenterY(){return TZ_GetY() + TZ_GetHeight()/2;};
	void SetAnim(int spriteID,int lockID, int normalID, int selectID, int activeID);
	void SetAnim(int spriteID,int lockID);
	//void SetAnim(CSprite *sprite,int lockID, int normalID, int selectID, int activeID);
	//void SetAnim(CSprite *sprite,int lockID);
	bool CurrentAnimStop();
	static void ResetPlaySound();
	static Button* GetButtonActive();

	// Lam add touch id
	int _touch_id;
	bool IsAction(int touchID);

	//Utils
	
	static int GetFrameX(CSprite* spr, int frameID);
	static int GetFrameY(CSprite* spr, int frameID);
	static int GetFrameWidth(CSprite* spr, int frameID);
	static int GetFrameHeight(CSprite* spr, int frameID);
	static int GetCurrentAnimFrameX(CAnimObject *anim,int frameID);
	static int GetCurrentAnimFrameY(CAnimObject *anim,int frameID);
	static int GetCurrentAnimFrameWidth(CAnimObject *anim,int frameID);
	static int GetCurrentAnimFrameHeight(CAnimObject *anim,int frameID);

	static int GetCurrentAnimFModuleAngle(CAnimObject *anim,int module);
	static int GetCurrentAnimFModuleX(CAnimObject *anim,int module);
	static int GetCurrentAnimFModuleY(CAnimObject *anim,int module);
	static int GetCurrentAnimFModuleWidth(CAnimObject *anim,int module);
	static int GetCurrentAnimFModuleHeight(CAnimObject *anim,int module);
	static int GetCurrentAnimMarkerX(CAnimObject *anim,int module);
	static int GetCurrentAnimMarkerY(CAnimObject *anim,int module);

	static int GetModuleMarkerX(CSprite* _target_sprite,int frame_id,int module);
	static int GetModuleMarkerY(CSprite* _target_sprite,int frame_id,int module);
	static int GetModuleWidth(CSprite* _target_sprite,int frame_id,int module);
	static int GetModuleHeight(CSprite* _target_sprite,int frame_id,int module);
	static int GetModuleID(CSprite *sprite,int frame_id, int moduleOfFrame);
	static int GetModuleOpacity(CSprite *sprite,int frame_id, int moduleOfFrame);
	static int GetModuleNum(CSprite* sprite,int frame_id);
};
#endif