#if !defined(_H_MAP_OBJECT)
#define _H_MAP_OBJECT
#include "../DataType/CType.h"
struct MapObject
{
	UINT32 _id;
	ResourceItem _res;//point to a sprite
	UINT32 _module_id;
	PinePoint _translate;
	float _scalex;
	float _scaley;
	float _angle;
};

#endif//_H_MAP_OBJECT