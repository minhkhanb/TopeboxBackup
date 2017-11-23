#include "pch.h"
#include "PinePolygon.h"

PinePolygon::PinePolygon():_vert_x(NULL), _vert_y(NULL), _vert_num(0){

}
PinePolygon::PinePolygon(int vert_num) : _vert_x(NULL), _vert_y(NULL), _vert_num(0){
	init(vert_num);
}
PinePolygon::PinePolygon(const PinePolygon& poly_in) : _vert_x(NULL), _vert_y(NULL), _vert_num(0){
	init(poly_in._vert_num);
	for (int v = 0; v < poly_in._vert_num; v++){
		setVertex(v, poly_in._vert_x[v], poly_in._vert_y[v]);
	}
}
PinePolygon::~PinePolygon(){
	release();
}

PinePolygon&  PinePolygon::operator=(const PinePolygon& poly_in){
	if (&poly_in != this){
		init(poly_in._vert_num);
		for (int v = 0; v < poly_in._vert_num; v++){
			setVertex(v, poly_in._vert_x[v], poly_in._vert_y[v]);
		}
	}
	return *this;
}

void PinePolygon::init(int vert_num){
	release();
	if (vert_num > 0){
		_vert_x = new float[vert_num];
		_vert_y = new float[vert_num];
		_vert_num = vert_num;
		for (int i = 0; i < vert_num; i++){
			_vert_x[i] = 0.0f;
			_vert_y[i] = 0.0f;
		}
	}
}
void PinePolygon::setVertex(int vert, float x, float y){
	if (vert < 0 || vert >= _vert_num)return;
	_vert_x[vert] = x;
	_vert_y[vert] = y;
}
void PinePolygon::setVertex(int vert, PinePoint point){
	setVertex(vert, point.X, point.Y);
}
int PinePolygon::getVertexNum(){
	return _vert_num;
}
PinePoint PinePolygon::getVertex(int vert){
	PinePoint vertex;
	vertex.X = 0;
	vertex.Y = 0;
	if (vert >= 0 && vert < _vert_num){
		vertex.X = _vert_x[vert];
		vertex.Y = _vert_y[vert];
	}
	return vertex;
}
bool PinePolygon::checkPointIn(float x, float y){
	int i, j, c = 0;
	for (i = 0, j = _vert_num - 1; i < _vert_num; j = i++) {
		if (((_vert_y[i]>y) != (_vert_y[j]>y)) &&
			(x < (_vert_x[j] - _vert_x[i]) * (y - _vert_y[i]) / (_vert_y[j] - _vert_y[i]) + _vert_x[i]))
			c = !c;
	}
	return c;
}
bool PinePolygon::checkPointIn(PinePoint point){
	return checkPointIn(point.X, point.Y);
}
PineRectangle PinePolygon::getBound(){
	PineRectangle rect;
	rect.X = rect.Y = rect.W = rect.H = 0;
	if (_vert_num > 0){
		PinePoint min, max;
		min.X = max.X = _vert_x[0];
		min.Y = max.Y = _vert_y[0];
		for (int v = 0; v < _vert_num; v++){
			if (_vert_x[v] < min.X) min.X = _vert_x[v];
			if (_vert_y[v] < min.Y) min.Y = _vert_y[v];
			if (_vert_x[v] > max.X) max.X = _vert_x[v];
			if (_vert_y[v] > max.Y) max.Y = _vert_y[v];
		}
		rect.X = min.X;
		rect.Y = min.Y;
		rect.W = max.X - min.X;
		rect.H = max.Y - min.Y;
	}
	return rect;
}
void PinePolygon::release(){
	if (_vert_x != NULL){
		delete[] _vert_x;
		_vert_x = NULL;
	}
	if (_vert_y != NULL){
		delete[] _vert_y;
		_vert_y = NULL;
	}
	_vert_num = 0;
}
void PinePolygon::translate(float x, float y){
	for (int v = 0; v < _vert_num; v++){
		_vert_x[v] += x;
		_vert_y[v] += y;
	}
}

void PinePolygon::transform(float m0, float m1, float m3, float m4, float m5, float m7, float m12, float m13, float m15)
{
	for (int v = 0; v < _vert_num; v++) {
		float x = _vert_x[v];
		float y = _vert_y[v];

		float w = 1.0f;// m3 * x + m7 *y + m15;

		
		_vert_x[v] = (m0 * x + m4 * y +m12)/w;
		_vert_y[v] = (m1 * x + m5 * y + m13)/w;
	}
}