//
//  PineGame.h
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 12/26/14.
//
//
#include "../GameCore/CGame.h"
#ifndef __PineEntertainment__PineGame__
#define __PineEntertainment__PineGame__

#include <stdio.h>
class PineGame
{
public:
    int _last_state;
    int _next_state;
    int _current_state;
    int _game_id;
    int _sub_state;
    void SetState(int newstate);
    void SetSubState(int newsubstate);
    
    virtual void UpdateAndRender();
    virtual void Init();
	virtual void PauseEx();
	virtual void ResumeEx();
	virtual void OnBackKeyPressEx();
};
#endif /* defined(__PineEntertainment__PineGame__) */
