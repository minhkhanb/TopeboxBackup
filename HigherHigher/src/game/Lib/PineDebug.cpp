//
//  PineDebug.cpp
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 12/19/14.
//
//

#include "PineDebug.h"
#include <time.h>
#include "CGame.h"
unsigned long long PineDebug::_time[LOG_MAX];
unsigned long long PineDebug::_time_used[LOG_MAX];
void PineDebug::BeginLog(int slot)
{
    _time[slot] = clock();
}
void PineDebug::EndLog(int slot)
{
    _time_used[slot] =clock()-_time[slot];
    
}
void PineDebug::SetSpriteLog(int Sprite)
{
    _spriteid = Sprite;
    if(GET_SPRITE(Sprite) == NULL)
    {
       // CGame::_this->LoadSprite(Sprite,true,true);
    }
}
void PineDebug::RenderLog(float x, float y,int from, int to)
{
    for(int i = from; i <= to; i++)
    {
        printf("\nslot %d time:%llu",i,_time_used[i]);
    }
}
