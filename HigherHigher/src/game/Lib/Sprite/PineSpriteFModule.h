#if !defined(_H_PINE_SPRITE_FMODULE)
#define _H_PINE_SPRITE_FMODULE
#include "../DataType/CType.h"
struct PineSpriteFModule
{
	UINT16 _mid;
	UINT8 _flag;
	float _tx;
	float _ty;
	float _m11, _m12, _m21, _m22;
	UINT8 _opacity;
	
	PinePoint GetRealPos();
};
#endif//_H_PINE_SPRITE_FMODULE
