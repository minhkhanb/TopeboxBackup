//
//  Jumper.h
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 10/26/14.
//
//
#include "GameFlatform.h"
#ifndef __PineEntertainment__Jumper__
#define __PineEntertainment__Jumper__

#include <stdio.h>

#define JUMPER_STATE_IDLE   0
#define JUMPER_STATE_RUN    1
#define JUMPER_STATE_JUMP   2
#define JUMPER_STATE_FLYING 3
#define JUMPER_STATE_FALL   4

#define JUMPER_NONE_ANCHOR  0
#define JUMPER_LEFT_ANCHOR  1
#define JUMPER_RIGHT_ANCHOR 2
class Jumper:public GameFlatform
{
public:
    Jumper();
    int _state;
    int _anchor;
    
    float _accel_speed;
    float _accel_jump;
    
    
    void UpdateMoving();
    int GetState();
    void SetState(int state);
    int _sign_x;
    int _sign_y;
    
};
#endif /* defined(__PineEntertainment__Jumper__) */
