#ifndef _CANIMOBJECT_MANAGER_H
#define _CANIMOBJECT_MANAGER_H

#include "../Lib/Sprite/CSprite.h"
#include "CAnimObject.h"
#ifndef ANIM_LIST_MAX 
#define ANIM_LIST_MAX (2000)
#endif
class CAnimManager
{
    CAnimObject* _anim[ANIM_LIST_MAX];
    int _weight[ANIM_LIST_MAX];
    int _opacity[ANIM_LIST_MAX];
    float _scale_x[ANIM_LIST_MAX];
    float _scale_y[ANIM_LIST_MAX];
    float _center_scalex[ANIM_LIST_MAX];
    float _center_scaley[ANIM_LIST_MAX];
    COLOR _colormask[ANIM_LIST_MAX];
    int _current_num;
public:
    void Reset();
    void AddAnim(CAnimObject* anim, int weight);
    void Render();
    
};

#endif