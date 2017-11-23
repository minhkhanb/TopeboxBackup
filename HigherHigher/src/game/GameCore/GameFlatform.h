#include "CGame.h"

#if !_GAMEFLATFORM_H
#define  _GAMEFLATFORM_H 1
#include "GameRect.h"
class GameFlatform;

struct Vecter2D
{
    float x;
    float y;
};
class CPos
{
public:
    int _col;
    int _row;
};
class GameFlatform
{
public:
    float _posX;
    float _posY;
    
    void SetPos(float x, float y);
    float GetPosX();
    float GetPosY();

    
    float _transformX;
    float _transformY;
    void SetTransformXY(float x, float y);
    float GetTransformX();
    float GetTransformY();
    

    bool IsInsideWorld(GameFlatform* world);
    bool CheckCollideWorld(GameFlatform* world, int* arraycollide,int& collide_num);
    int IsCollideWorld(GameFlatform* world);
    bool CheckCollideRect(GameRect* rect,int* arraycollide,int& collide_num);
    int IsCollideRect(GameRect* rect);
    
    bool CheckCollideLeft(GameFlatform* world,int* arraycollide,int& collide_num);
    bool CheckCollideRight(GameFlatform* world,int* arraycollide,int& collide_num);
    bool CheckColliedTop(GameFlatform* world,int* arraycollide,int& collide_num);
    bool CheckColliedBottom(GameFlatform* world,int* arraycollide,int& collide_num);
    
    Vecter2D GetXYOfTile(int col,int row);
    
    
    void UpdateFollowParent();
    float _tilew;
    float _tileh;
    
    int  _color_debug;
    int  *_flatform;
    int  _num_col;
    int  _num_row;
    int  _colordebug;
    void SetSizeTile(float w, float h);
    void SetSize(int col, int row);
    
    int GetValue(int col, int row);
    void SetValue(int col, int row, int value);
    GameFlatform();
    ~GameFlatform();
    CPos GetPosOfTile(float posX, float posY);
    void SetColorDebug(int color);
    void RenderDebug();
    
    int GetCols();
    int GetRows();
    float GetTileW();
    float GetTileH();
    GameFlatform** _childs;
    int _num_of_child;
    int _max_of_child;
    GameFlatform* Me();
    GameFlatform* _parent;
    void SetParent(GameFlatform* p);
    GameFlatform* GetParent();
    void AddChild(GameFlatform* child);
    GameFlatform* GetChildAtIndex(int index);
    int GetNumChilds();
    
    virtual void UpdateMoving() = 0;
    
    float GetXFromIndex(int index,int anchor);
    float GetYFromIndex(int index,int anchor);
    int GetIndexFromPos(float x, float y);
    
};

#endif