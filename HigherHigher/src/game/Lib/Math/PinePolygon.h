#if !defined(_H_POLYGON)
#define _H_POLYGON
#include "../DataType/CType.h"
//duy.nguyen 2014

class PinePolygon
{
protected:
	float* _vert_x;
	float* _vert_y;
	int _vert_num;
	void release();
public:
	PinePolygon();
	PinePolygon(const PinePolygon&);
	PinePolygon(int vert_num);
	~PinePolygon();
	PinePolygon& operator=(const PinePolygon&);
	//PinePolygon& operator=(PinePolygon&&);
	//PinePolygon& operator=(PinePolygon);
	virtual void init(int vert_num);
	virtual void setVertex(int vert, float x, float y);
	virtual void setVertex(int vert, PinePoint point);
	virtual int getVertexNum();
	virtual PinePoint getVertex(int vert);
	virtual bool checkPointIn(float x, float y);
	virtual bool checkPointIn(PinePoint point);
	virtual PineRectangle getBound();
	virtual void transform(float m0, float m1,float m3, float m4, float m5, float m7, float m12, float m13, float m15);
	void translate(float x, float y);
};
#endif