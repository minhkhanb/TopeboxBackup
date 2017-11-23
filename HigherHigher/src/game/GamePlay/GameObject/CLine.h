#ifndef _LINE_H
#define _LINE_H
#include "../../Lib/DataType/CType.h"
class CLine
{

public:
	
	float _radix_a;
	float _radix_b;
	float _radix_c;
public:
	struct Point
	{
		float _x;
		float _y;
	};
	Point _point_A;
	Point _point_B;
	Point _intersection_poin;
	void CreateLine(float x1, float y1, float x2, float y2);
	void CreateLine(PinePoint point1, PinePoint point2);
	bool CheckIntersect(float x1, float y1, float x2, float y2);
	bool CheckIntersect(CLine line);
	void SetPoint(float x1, float y1, float x2, float y2);
	void RenderLine();
	bool GetIntersectPoin(CLine line);
	bool GetIntersectPoin(float x1, float y1, float x2, float y2);
	float GetAlpha(CLine line);
	float GetDisWithPoint(float x, float y);
	bool CheckPointInRange(float x, float y);
};
#endif