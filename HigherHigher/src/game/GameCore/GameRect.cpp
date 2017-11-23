//
//  GameRect.cpp
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 10/28/14.
//
//
#include "pch.h"
#include "GameRect.h"

GameRect::GameRect()
{
    _x  = 0;
    _y  = 0;
    _w  = 0;
    _h  = 0;
}
GameRect::GameRect(float x, float y, float w, float h)
{
    _x = x;
    _y = y;
    _w = w;
    _h = h;
}
bool GameRect::ISCollideRect(GameRect* rect)
{
    float xx1 = _x;
    float yy1 = _y;
    float ww1 = _w;
    float hh1 = _h;
    
    float xx2 = rect->_x;
    float yy2 = rect->_y;
    float ww2 = rect->_w;
    float hh2 = rect->_h;
    
    if(CHECK_POINT_IN_RECT(xx1, yy1, xx2, yy2, ww2, hh2) ||
       CHECK_POINT_IN_RECT(xx1+ww1, yy1, xx2, yy2, ww2, hh2) ||
       CHECK_POINT_IN_RECT(xx1, yy1+hh1, xx2, yy2, ww2, hh2) ||
       CHECK_POINT_IN_RECT(xx1+ww1, yy1+hh1, xx2, yy2, ww2, hh2))
    {
        return true;
    }
    return false;
}