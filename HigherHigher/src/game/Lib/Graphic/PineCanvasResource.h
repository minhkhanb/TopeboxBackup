#if !defined(_H_PINE_CANVAS_RESOURCE)
#define _H_PINE_CANVAS_RESOURCE
#include "../DataType/CType.h"
#include "../OpenGLES/Graphic.h"

class PineCanvasResource
{
public:
	int _id;
	float _w, _h;
	float _uv[4];
	HTEXTURE _tex;
};

#endif//_H_PINE_CANVAS_RESOURCE