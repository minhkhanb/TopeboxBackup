#ifndef _CANIMOBJECT_H
#define _CANIMOBJECT_H

#include "../Lib/Sprite/CSprite.h"

//anim status
#define	ANIM_STOP	0
#define	ANIM_PLAY	1
#define	ANIM_PAUSE	2

class CAnimObject
{
public:	
	CSprite*		_anim_sprite;
	float			_x;
	float			_y;
	INT32			_current_anim;
	INT32			_current_aframes;
	INT32			_current_aframes_time;
	INT8			_anim_status;
	INT32			_transform;
	bool			_is_used;
	INT8			_palette;
	int				_sub_time;
	int				_sub_loop_time;
	int				_intdexSprite;
public:
	void SetSubTime(int time){_sub_time = time;}
	CAnimObject();
	CAnimObject(CSprite* anim_spr);
	CAnimObject(CSprite* anim_spr, int x, int y);
	~CAnimObject();
	
	void SetAnimSprite(CSprite* anim_spr,int index);
	void SetPalette(INT8 palette);
	void SetPos(float x, float y);
	void DrawAnimObject(Graphic* g);
	void DrawAnimObject(Graphic* g, bool is_loop);
	void DrawAnimObject(Graphic* g, int transform);
	void DrawAnimObject(Graphic* g, float x, float y, bool is_loop = false);
	void DrawAnimObject(Graphic* g, int anim, float x, float y, bool is_loop = false);
	void UpdateAnimObject();
	void SetAnim(int anim, bool is_loop = false, int transform = 0);
	void StopAnim();
	void PauseAnim();
	void ResetAnim();
	bool IsInside(int x, int y);
	bool IsCurrentAnimStop();
	//nghia.dothanh add
	int GetCurrentAnimWidth();
	int GetCurrentAnimHeight();

	int GetTotalTimeOfCurrentAnim();
	int *GetCollisionCircle(int x, int y, int r);
	int *GetBoundaryCircle();
	int GetCurrentAnim();

	void CalculateCurrentAFrame();
	
	bool _overlay_module;
	int _module_need_overlay_id;
	int _module_overlay_id;
	int _anchor_overlay;
	CSprite *_sprite_overlay;	
	void ResetOverlay(){_overlay_module = false;};
	void SetOverlayModule(int module_id,CSprite *sprite,int overlay_id,int anchor=_FRONT);

	//hao.ly add
	bool			_is_loop;
	int getMaxAframe();
	int getMinAframe();
	void setRandomAframe();
	void setToAFrame(int Aframe, int time = 0);
	void SetToFrameLast();
	void swapAnimWithSameFormat(int newAnim);
};

#endif