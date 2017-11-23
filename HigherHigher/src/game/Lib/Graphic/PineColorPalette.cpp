#include "pch.h"
#include "PineColorPalette.h"

void PineColorPalette::AddColor(PineColor color)
{
	_palette.push_back(color);
}
int PineColorPalette::GetSize()
{
	return _palette.size();
}
PineColor PineColorPalette::At(int index)
{
	return _palette[index];
}