//
//  CTouchZoneManager.cpp
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 4/8/15.
//
//
#include "pch.h"
#include "CGame.h"
#include "CTouchZoneManager.h"

bool CCircleZone::CheckPointInZone(float x, float y)
{
    return (CMath::DISTANCE(x,y,this->x,this->y) <= r);
}
float CCircleZone::GetCenterX()
{
    return x;
}
float CCircleZone::GetCenterY()
{
    return y;
}
CCircleZone* CCircleZone::CreateZone(float x, float y, float r)
{
    CCircleZone* is = new CCircleZone();
    is->x = x;
    is->y = y;
    is->r = r;
    return is;
}


bool CRectZone::CheckPointInZone(float x, float y)
{
    return CHECK_POINT_IN_RECT(x,y,this->x,this->y,this->w,this->h);
}
float CRectZone::GetCenterX()
{
    return x+w/2;
}
float CRectZone::GetCenterY()
{
    return y+h/2;
}


void CTouchZoneManager::ClearAll()
{
    for(int i = 0; i < _max; i++)
    {
        if(list != NULL && list[i] != NULL)
        {
            delete[] list[i];
            list[i] = NULL;
        }
    }
}
CTouchZoneManager::CTouchZoneManager()
{
    _max = 100;
    _num = 0;
    list = new CTouchZone*[_max];
    for(int i = 0; i < _max; i++)
    {
        list[i] = NULL;
    }
}
CTouchZoneManager::~CTouchZoneManager()
{
    ClearAll();
    delete[] list;
}

bool CTouchZoneManager::RemoveZone(int index)
{
    if(index >= 0 && index < _num)
    {
        CTouchZone* tmp = list[index];
        for(int i = index; i < _num-1; i++)
        {
            list[i] = list[i+1];
        }
        _num--;
        if(tmp != NULL)
        {
            delete tmp;
        }
        return true;
    }
    return false;
}
void CTouchZoneManager::AddZone(CTouchZone* zone)
{
    if(_num < _max && zone != NULL)
    {
        list[_num] = zone;
        _num++;
    }
}
int CTouchZoneManager::GetZoneNearest(float x, float y)
{
    if(_num > 0)
    {
        int bestindex = 0;
        for(int i = 0; i < _num; i++)
        {
            if(list[i] != NULL && _DD_(x,y,list[i]->GetCenterX(),list[i]->GetCenterY()) < _DD_(x,y,list[bestindex]->GetCenterX(),list[bestindex]->GetCenterY()))
            {
                bestindex = i;
            }
        }
        return bestindex;
    }
    return -1;
    
}