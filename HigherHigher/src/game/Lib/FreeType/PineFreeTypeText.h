#if !defined(_H_PINE_FREETYPE_TEXT)
#define _H_PINE_FREETYPE_TEXT

#include <vector>
#include "PineFreeTypeTextChar.h"

class PineFreeTypeCanvas;
#ifndef _LEFT
#define		_LEFT								1//(1<<0)
#define		_RIGHT								2//(1<<1)
#define		_HCENTER							4//(1<<2)
#define		_TOP								8//(1<<3)
#define		_BOTTOM								16//(1<<4)
#define		_VCENTER							32//(1<<5)
#endif

#define PAGARAP_MAX (10240)

class PineFreeTypeText
{
private:

	std::vector<PineFreeTypeTextChar> _chr;
	PineFreeTypeCanvas* _cvas;//parent canvas
	PineFreeTypeText();
    
    static float infox[PAGARAP_MAX],infoy[PAGARAP_MAX];

public:

	void Init(const char* str);
	void Init(INT32* str, int len);
	void Render(float x, float y,int anchor = _LEFT);
    void RenderPage(float x, float y,int width, int anchor = _LEFT);

	~PineFreeTypeText();

	friend class PineFreeTypeCanvas;
};
#endif//_H_PINE_FREETYPE_TEXT