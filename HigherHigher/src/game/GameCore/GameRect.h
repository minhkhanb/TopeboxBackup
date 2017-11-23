//
//  GameRect.h
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 10/28/14.
//
//
#include "CGame.h"
#ifndef __PineEntertainment__GameRect__
#define __PineEntertainment__GameRect__

#include <stdio.h>
class GameRect
{
public:
    GameRect();
    GameRect(float x, float y, float w, float h);
    float _x;
    float _y;
    float _w;
    float _h;
    bool ISCollideRect(GameRect* rect);
};
#endif /* defined(__PineEntertainment__GameRect__) */
