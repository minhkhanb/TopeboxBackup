#include "pch.h"
#include "CAnimManager.h"
#include "./CGame.h"

void CAnimManager::Reset()
{
    _current_num =0;
}

void CAnimManager::AddAnim(CAnimObject* anim, int weight)
{
    int index = _current_num;
    
    for(int i = _current_num-1; i >= 0 && _current_num > 0; i--)
    {
        if(_weight[i] > weight)
        {
            _anim[i+1] = _anim[i];
            _weight[i+1] = _weight[i];
            _opacity[i+1] = _opacity[i];
            _scale_x[i+1] = _scale_x[i];
            _scale_y[i+1] = _scale_y[i];
            _center_scalex[i+1] =  _center_scalex[i];
            _center_scaley[i+1] = _center_scaley[i];
            _colormask[i+1] = _colormask[i];
            index = i;
        }
        else
        {
            break;
        }
    }
    _anim[index] = anim;
    _weight[index] = weight;
    _opacity[index] = G()->GetCurrentOpacityPercent();
    _scale_x[index] = G()->GetCurrentScaleX();
    _scale_y[index] = G()->GetCurrentScaleY();
    _center_scalex[index] = G()->GetCurrentCenterScaleX();
    _center_scaley[index] = G()->GetCurrentCenterScaleY();
    _colormask[index] =  G()->GetColorMask();
    _current_num++;
    //printf("\n_current_num  = %d, index = %d",_current_num,index);
}
void CAnimManager::Render()
{
    int lastopaicty = G()->GetCurrentOpacityPercent();
    float scalex = G()->GetCurrentScaleX();
    float scaley = G()->GetCurrentScaleY();
    float center_scalex = G()->GetCurrentCenterScaleX();
    float center_scaley = G()->GetCurrentCenterScaleY();
    bool hasScale= G()->HasScale();
    COLOR mask = G()->GetColorMask();
   
   
    //printf("\n_current_num = %d",_current_num);
    for(int i = 0; i < _current_num; i++)
    {
        if(_anim[i] != NULL)
        {
            if(_scale_x[i] != 1.0 || _scale_y[i] != 1.0f)
            {
                G()->SetScale(_scale_x[i],_scale_y[i],_center_scalex[i],_center_scaley[i]);
            }
            G()->SetOpacity(_opacity[i]);
            if((_colormask[i]&0xffffffff) != 0)
            {
                G()->SetColorMask(_colormask[i]);
            }

            _anim[i]->DrawAnimObject(G());
            //_anim[i]->UpdateAnimObject();
            G()->ClearScale();
            G()->ClearColorMask();
        }
    }
    G()->SetOpacity(lastopaicty);
    if((mask&0xffffffff) != 0)
    {
        G()->SetColorMask(mask);
    }
    else
    {
        G()->ClearColorMask();
    }
    if(hasScale)
    {
        G()->SetScale(scalex,scaley,center_scalex,center_scaley);
    }
    else
    {
        G()->ClearScale();
    }
}

//1,2,3,4,5
//0