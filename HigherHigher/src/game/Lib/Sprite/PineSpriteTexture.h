#if !defined(_H_PINE_SPRITE_TEXTURE)
#define _H_PINE_SPRITE_TEXTURE
#include "../DataType/CType.h"
#include "../OpenGLES/Graphic.h"
#include <vector>
#include "PineSpriteTextureData.h"
class PineSpriteTexture
{
public:
	BYTE _format;
	UINT16 _w;
	UINT16 _h;
	
	std::vector<HTEXTURE> _tex;
	BYTE _num_pal;
	bool _has_mask;
	UINT32 _backup_mask_pos;
	UINT32 _num_mask_color;
	std::vector<PineSpriteTextureData> _textures;
	
};

#endif//_H_PINE_SPRITE_TEXTURE