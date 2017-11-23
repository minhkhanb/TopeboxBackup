#if !defined (_PINE_COLOR_PALETTE)
#define _PINE_COLOR_PALETTE
#include "../DataType/CType.h"
#include "PineColor.h"
#include <vector>

class PineColorPalette
{
private:
	std::vector<PineColor> _palette;

public:
	void AddColor(PineColor color);
	int GetSize();
	PineColor At(int index);
};
#endif//_PINE_COLOR_PALETTE