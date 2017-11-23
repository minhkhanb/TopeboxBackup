#pragma once
#if !defined(_H_MAP_BUILDER)
#define _H_MAP_BUILDER
#include "../DataType/CType.h"
#include "MapObject.h"


class MapBuilder
{
public:
	MapBuilder(PineSize map_size,PineSize cvas_size, int tex_size);
	~MapBuilder();
	void Build();
	void Update();
	void IsFinish();
};

#endif//_H_MAP_BUILDER