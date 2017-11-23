#include "pch.h"
#include "../../GameCore/CGame.h"

void CLine::CreateLine(float x1, float y1, float x2, float y2)
{
	SetPoint(x1, y1, x2, y2);
	float vtpt_x = (y2 - y1)*-1;
	float vtpt_y = x2 - x1;
	_radix_b = vtpt_y;
	_radix_a = vtpt_x;
	_radix_c = -(vtpt_x*x1 + vtpt_y*y1);
}
void CLine::CreateLine(PinePoint point1, PinePoint point2) {
	CreateLine(point1.X, point1.Y, point2.X, point2.Y);
}
bool CLine::CheckIntersect(float x1, float y1, float x2, float y2)
{
	float result = (_radix_a*x1 + _radix_b*y1 + _radix_c)*(_radix_a*x2 + _radix_b*y2 + _radix_c);
	if (result > 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool CLine::CheckIntersect(CLine line)
{
	bool ret = CheckIntersect(line._point_A._x, line._point_A._y, line._point_B._x, line._point_B._y);
	return ret;
}


void CLine::SetPoint(float x1, float y1, float x2, float y2)
{
	_point_A._x = x1;
	_point_A._y = y1;
	_point_B._x = x2;
	_point_B._y = y2;
}

void CLine::RenderLine()
{
	//G()->SetColor(0xffff0000);
	G()->DrawLine(_point_A._x, _point_A._y, _point_B._x, _point_B._y);
}

bool CLine::GetIntersectPoin(CLine line)
{
	bool is_intersect = true;
	float result = (_radix_a)*line._radix_b + (_radix_b)*line._radix_a;
	if (result != 0)
	{
		float D = _radix_a*line._radix_b - _radix_b*line._radix_a;
		float Dx = -_radix_c*line._radix_b + _radix_b*line._radix_c;
		float Dy = -_radix_a*line._radix_c + _radix_c*line._radix_a;
		_intersection_poin._x = Dx/D;
		_intersection_poin._y = Dy/D;
	}
	else
	{
		is_intersect = false;
	}
	return is_intersect;
}
bool CLine::GetIntersectPoin(float x1, float y1, float x2, float y2)
{
	bool is_intersect = true;
	CLine line;
	line.CreateLine(x1, y1, x2, y2);
	is_intersect = GetIntersectPoin(line);
	return is_intersect;
}

float CLine::GetAlpha(CLine line)
{
	float alpha = 0;
	float a = _radix_a*line._radix_a;
	float b = _radix_b*line._radix_b;
	float ab1 = CMath::SQRT(_radix_a*_radix_a + _radix_b*_radix_b);
	float ab2 = CMath::SQRT(line._radix_a*line._radix_a + line._radix_b*line._radix_b);
	float numerator = CMath::ABSOLUTE_VALUE(a+b);
	float denominator = ab1*ab2;
	float Cos = numerator / denominator;
	alpha = CMath::ARCCOS(Cos);
	return alpha;
}

float CLine::GetDisWithPoint(float x, float y)
{
	float res = 0;
	float numerator = CMath::ABSOLUTE_VALUE(x*_radix_a + y*_radix_b + _radix_c);
	float denomirator = CMath::SQRT(_radix_a*_radix_a + _radix_b*_radix_b);
	res = numerator / denomirator;
	return res;
}
bool CLine::CheckPointInRange(float x, float y)
{
	float direct = (_radix_a*x + _radix_b*y + _radix_c);
	if (direct != 0){
		float angle = direct > 0 ? 90 : -90;

		float d = GetDisWithPoint(x, y);
		float nx = x + d* CMath::COS(angle);
		float ny = y + d* CMath::SIN(angle);
		return CheckIntersect(x, y, nx, ny);
	}
	else{

	}
	return false;
}