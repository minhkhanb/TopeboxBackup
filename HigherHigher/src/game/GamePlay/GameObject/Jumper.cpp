//
//  Jumper.cpp
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 10/26/14.
//
//
#include "pch.h"
#include "Jumper.h"
Jumper::Jumper()
{
    _accel_jump = 0;
    _accel_speed = 0;
    _state = JUMPER_STATE_IDLE;
    _anchor = JUMPER_NONE_ANCHOR;
    _sign_x = 1;
    _sign_y = 1;
}
int Jumper::GetState()
{
    return _state;
}
void Jumper::SetState(int state)
{
    _state = state;
}
void Jumper::UpdateMoving()
{
    for(int i = 0; i< CGame::_this->_num_touches; i++)
    {
        if(TOUCHS(i)->_flag)
        {
            if(TOUCHS(i)->_state == TOUCH_UP)
            {
                _sign_y *= -1;
                CGame::_this->ClearAllTouch();
            }
        }
    }
    float xx = _posX;
    float yy = _posY;
    //UpdateFollowParent();
    int array_collid[100];
    int num_collide;
    float minx = _posX,miny = _posY;
    float maxx = _posX,maxy = _posY;
    float speed = 18;
    CheckCollideWorld(_parent,array_collid,num_collide);
    {
       
        int anchor_collide = 0;
        //printf("\nnum_collide = %d",num_collide);
        for(int i = 0; i < num_collide; i++)
        {
            
            Vecter2D pos = _parent->GetXYOfTile(array_collid[i]%_parent->_num_col,array_collid[i]/_parent->_num_col);
            //printf("\ncolli %d, index = %d pos(%f,%f)",i,array_collid[i],pos.x,pos.y);
            if(_posY > pos.y) //has collide below
            {
                anchor_collide = (anchor_collide|_BOTTOM);
                if(miny > pos.y)
                {
                    miny = pos.y;
                }
            }
            if(_posY < pos.y)
            {
                anchor_collide = (anchor_collide | _TOP);
                if(maxy < pos.y)
                {
                    maxy = pos.y;
                }
            }
            if(_posX > pos.x)
            {
                anchor_collide = (anchor_collide | _LEFT);
                if(minx > pos.x)
                {
                    minx = pos.x;
                }
            }
            if(_posX < pos.x)
            {
                anchor_collide = (anchor_collide | _RIGHT);
                if(maxx < pos.x)
                {
                    maxx = pos.x;
                }
            }
            
        }
        //_posY += (5*_sign_y);
        /*
        if(_sign_y >= 0 && (anchor_collide&_BOTTOM) == 0)
        {
            _posY += 5;
        }
        if(_sign_y <= 0 && (anchor_collide&_TOP) == 0)
        {
            _posY -= 5;
        }
         */
        
        
        if((_sign_x >= 0 && (anchor_collide & _RIGHT) == 0)||(_sign_x <= 0 &&(anchor_collide & _LEFT) == 0))
        {
            _posX += (speed*_sign_x);
        }
        
        if((anchor_collide & _RIGHT) != 0 || (anchor_collide & _LEFT) != 0)
        {
            //printf("\n_posX = %f, maxx = %f",posX,maxx);
            if((anchor_collide & _RIGHT) != 0)
            {
                _posX = maxx-_tilew -0.01f;
            }
            if((anchor_collide & _LEFT) != 0)
            {
                _posX = minx+ _parent->GetTileW() + 0.01f;
            }
            _sign_x *= -1;
            
        }
        
    }
    
    //printf("\nnum_collide = %d",num_collide);
    
    
    _transformX = _posX-xx;
    _transformY = _posY-yy;
}