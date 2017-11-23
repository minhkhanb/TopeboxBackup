#if !defined(_H_PINE_SPRITE_MODULE)
#define _H_PINE_SPRITE_MODULE

#include "../DataType/CType.h"

struct PineSpriteModuleLine
{
	COLOR Color;
};
struct PineSpriteModuleRectangle
{
	COLOR Color;
};
struct PineSpriteModuleImage
{
	UINT16 X;
	UINT16 Y;
	UINT16 W;
	UINT16 H;
	INT8 ImageIndex;
};

struct PineSpriteModule
{
	UINT8 _type;
	union {
		PineSpriteModuleLine Line;
		PineSpriteModuleRectangle Rect;
		PineSpriteModuleImage Image;
	};
};
#endif//_H_PINE_SPRITE_MODULE