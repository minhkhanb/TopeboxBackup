#if !defined(_H_GAMECONTEXT)
#define _H_GAMECONTEXT
#include "../Lib/DataType/CType.h"
class GameContext{
public:
	float TranslateX;
	float TranslateY;
	float ScaleX;
	float ScaleY;
	float BorderLeft;
	float ScaleOut;
	float ScaleOutX;
	float ScaleOutY;

	PinePoint PointTopLeft;
	PinePoint PointBottomRight;
	//margin left 

	float BorderRight;
	//margin left + game width

	float BorderTop;
	//margin top

	float BorderBottom;
	//margin top + height

	PineSize Size;
	ResVersion CurrentResourceVersion;

	GameContext() :
		TranslateX(0), TranslateY(0), ScaleX(1.0f), ScaleY(1.0f), ScaleOut(1.0f),
		BorderLeft(0), BorderRight(0), BorderTop(0), BorderBottom(0)
	{
		Size.W = 0;
		Size.H = 0;
	};

	static GameContext getDefaultContext(float scr_w, float scr_h){
		GameContext gc;
		gc.Size.W = scr_w;
		gc.Size.H = scr_h;
		gc.BorderRight = scr_w;
		gc.BorderBottom = scr_h;
		return gc;
	}
	static GameContext* getNewDefaultContext(float scr_w, float scr_h){
		GameContext* gc = new GameContext();
		*gc = getDefaultContext(scr_w, scr_h);
		return gc;
	}
	PinePoint MeasureAnchor(PinePoint point, INT8 anchor);
	PinePoint MeasureAnchor(float x, float y, INT8 anchor);
	PinePoint MeasureAnchor(PinePoint point);
	PinePoint MeasureAnchor(float x, float y);
	
	float MeasureDensityX(float objx, float objw){
		objx *= ScaleX;
		if (((BorderRight - BorderLeft) != objw * ScaleX)){
			float s = (2 * TranslateX) / ((BorderRight - BorderLeft) - objw * ScaleX);
			float nobjx = objx - TranslateX + s* objx;
			return nobjx / ScaleX;
		}
		return objx;
	}
	float MeasureDensityY(float objy, float objh){
		objy *= ScaleY;
		if (((BorderBottom - BorderTop) != objh * ScaleY)){
			float s = (2 * TranslateY) / ((BorderBottom - BorderTop) - objh * ScaleY);
			float nobjy = objy - TranslateY + s* objy;
			return nobjy / ScaleY;
		}
		return objy;
	}
	PinePoint MeasureDensity(PinePoint objpos, float objw, float objh){
		PinePoint point;
		point.X = MeasureDensityX(objpos.X, objw);
		point.Y = MeasureDensityY(objpos.Y, objh);
		return point;
	}
	PinePoint MeasureDensity(float objx, float objy, float objw, float objh){
		PinePoint point;
		point.X = MeasureDensityX(objx, objw);
		point.Y = MeasureDensityY(objy, objh);
		return point;
	}
};
#endif//_H_GAMECONTEXT