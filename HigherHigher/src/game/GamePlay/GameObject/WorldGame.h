//
//  WorldGame.h
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 10/28/14.
//
//
#include "GameFlatform.h"

#ifndef __PineEntertainment__WorldGame__
#define __PineEntertainment__WorldGame__
#include <stdio.h>
class WorldGame:public GameFlatform
{
public:
    float _angle;
    void UpdateMoving();
};
#endif /* defined(__PineEntertainment__WorldGame__) */
