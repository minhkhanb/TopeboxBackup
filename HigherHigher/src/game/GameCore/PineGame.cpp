//
//  PineGame.cpp
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 12/26/14.
//
//
#include "pch.h"
#include "PineGame.h"

 void PineGame::UpdateAndRender()
{
    
}
void PineGame::Init()
{
    
}

void PineGame::PauseEx()
{

}

void PineGame::ResumeEx()
{

}

void PineGame::OnBackKeyPressEx()
{

}

void PineGame::SetState(int newstate)
{
    _last_state = _current_state;
    _current_state = newstate;
    _sub_state = k_INIT;
}
void PineGame::SetSubState(int newsubstate)
{
    _sub_state = newsubstate;
    
}
