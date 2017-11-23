//
//  CTouchZoneManager.h
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 4/8/15.
//
//

#ifndef __PineEntertainment__CTouchZoneManager__
#define __PineEntertainment__CTouchZoneManager__

#include <stdio.h>

class CTouchZone
{
    
public:
    CTouchZone(){;};
    int ID;
    float x;
    float y;
    virtual bool CheckPointInZone(float x, float y) = 0;
    virtual float GetCenterX()=0;
    virtual float GetCenterY()=0;
    
};
class CCircleZone:public CTouchZone
{
public:
    CCircleZone(){;};
    static CCircleZone* CreateZone(float x, float y, float r);
    float r;
    bool CheckPointInZone(float x, float y);
    float GetCenterX();
    float GetCenterY();
};
class CRectZone:public CTouchZone
{
   
public:
    CRectZone(){;};
    float w;
    float h;
    bool CheckPointInZone(float x, float y);
    float GetCenterX();
    float GetCenterY();
};

class CTouchZoneManager
{
public:
    CTouchZoneManager();
    ~CTouchZoneManager();
    void ClearAll();
    int _max;
    int _num;
    CTouchZone** list;
    void AddZone(CTouchZone* zone);
    bool RemoveZone(int index);
    int GetZoneNearest(float x, float y);
    CTouchZone* GetZoneAt(int index){ return list[index];};
    
};

#endif /* defined(__PineEntertainment__CTouchZoneManager__) */
