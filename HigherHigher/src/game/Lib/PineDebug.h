//
//  PineDebug.h
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 12/19/14.
//
//

#ifndef __PineEntertainment__PineDebug__
#define __PineEntertainment__PineDebug__

#include <stdio.h>
#define LOG_MAX (10)
class PineDebug
{
public:
    static unsigned long long _time[LOG_MAX];
    static unsigned long long _time_used[LOG_MAX];
    static int _spriteid;
    static void BeginLog(int slot);
    static void EndLog(int slot);
    static void SetSpriteLog(int Sprite);
    static void RenderLog(float x, float y,int from =0,int to = LOG_MAX-1);
    
    
};

#endif /* defined(__PineEntertainment__PineDebug__) */
