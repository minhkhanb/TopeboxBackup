#if !defined(_H_PINE_COLOR)
#define _H_PINE_COLOR
#include "../DataType/CType.h"

struct PineColorARGB
{
	BYTE A;
	BYTE R;
	BYTE G;
	BYTE B;
};

struct PineColorRGBA
{
	BYTE R;
	BYTE G;
	BYTE B;
	BYTE A;
};

struct PineColorRBG
{
	BYTE R;
	BYTE G;
	BYTE B;
};

struct PineColor4444
{
	BYTE B1;
	BYTE B2;

};

union PineColor
{
	PineColorARGB ARGB;
	PineColorRGBA RGBA;
	PineColorRBG RGB;
	PineColor4444 C4444;
};
#endif//_H_PINE_COLOR