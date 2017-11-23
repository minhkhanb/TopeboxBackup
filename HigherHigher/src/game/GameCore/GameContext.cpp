#include "pch.h"
#include "GameContext.h"
#include "CGame.h"
PinePoint GameContext::MeasureAnchor(PinePoint point, INT8 anchor){
	PinePoint measure_point = point;
	if (anchor& ANCHOR_TOP){
		measure_point.Y = (measure_point.Y - TranslateY) / ScaleY;
	}
	else if (anchor& ANCHOR_VCENTER){
		measure_point.Y = (BorderBottom + TranslateY) / 2 + point.Y;
		measure_point.Y = (measure_point.Y - TranslateY) / ScaleY;
	}
	else if (anchor& ANCHOR_BOTTOM){
		measure_point.Y = BorderBottom + TranslateY + point.Y;
		measure_point.Y = (measure_point.Y - TranslateY) / ScaleY;
	}


	if (anchor& ANCHOR_LEFT)
	{
		measure_point.X = (point.X - TranslateX) / ScaleX;
	}
	else if (anchor & ANCHOR_HCENTER)
	{
		measure_point.X = (BorderRight + TranslateX) / 2 + point.X;
		measure_point.X = (measure_point.X - TranslateX) / ScaleX;
	}
	else if (anchor & ANCHOR_RIGHT)
	{
		measure_point.X = (BorderRight + TranslateX) + point.X;
		measure_point.X = (measure_point.X - TranslateX) / ScaleX;
	}

	return measure_point;
}

PinePoint GameContext::MeasureAnchor(float x, float y, INT8 anchor){
	PinePoint point;
	point.X = x;
	point.Y = y;
	return MeasureAnchor(point, anchor);
}
PinePoint GameContext::MeasureAnchor(PinePoint point){
	PinePoint measure_point = point;
	float sx = point.X * ScaleX / (BorderRight - BorderLeft);
	float sy = point.Y * ScaleY / (BorderBottom - BorderTop);
	
	measure_point.X = sx * (BorderRight + TranslateX);
	measure_point.Y = sy *(BorderBottom + TranslateY);
	
	measure_point.X = (measure_point.X - TranslateX) / ScaleX;
	measure_point.Y = (measure_point.Y - TranslateY) / ScaleY;

	return measure_point;
}

PinePoint GameContext::MeasureAnchor(float x, float y){
	PinePoint point;
	point.X = x;
	point.Y = y;
	return MeasureAnchor(point);
}