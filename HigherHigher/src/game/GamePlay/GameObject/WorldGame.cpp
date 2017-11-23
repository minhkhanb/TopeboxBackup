//
//  WorldGame.cpp
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 10/28/14.
//
//
#include "pch.h"
#include "WorldGame.h"
void WorldGame::UpdateMoving()
{
    float xx = _posX;
    float yy = _posY;
    //_angle += 1;
    //_posX = 100*CMath::COS(_angle);
    _transformX = _posX - xx;
    _transformY = _posY - yy;
}

