#include "pch.h"
#include "GameFlatform.h"




float GameFlatform::GetXFromIndex(int index,int anchor)
{
    float offsetX = 0;
    if((anchor & _HCENTER) != 0)
    {
        offsetX = _tilew/2;
    }
    else if((anchor & _RIGHT) != 0)
    {
        offsetX = _tilew;
    }
    int col = index%_num_col;
    return  _posX + col*_tilew + offsetX;
}
float GameFlatform::GetYFromIndex(int index,int anchor)
{

    float offsetY = 0;
    if((anchor & _VCENTER) != 0)
    {
        offsetY = _tileh/2;
    }
    else if((anchor & _BOTTOM) != 0)
    {
        offsetY = _tileh;
    }
    int row = index/_num_col;
    return  _posY + row*_tileh + offsetY;
}

int GameFlatform::GetIndexFromPos(float x, float y)
{
    float xx = x - _posX;
    float yy = y - _posY;
    if(xx >= 0 && yy >= 0 && xx <= _num_col*_tilew && yy <= _num_row*_tileh)
    {
        int col = (int)(xx/_tilew);
        int row = (int)(yy/_tileh);
        return row*_num_col + col;
    }
    return -1;
    
}
float GameFlatform::GetPosX()
{
    return _posX;
}
float GameFlatform::GetPosY()
{
    return _posY;
}

bool GameFlatform::CheckCollideRect(GameRect* rect,int* arraycollide,int& collide_num)
{
    bool ret = false;
    collide_num = 0;
    for(int i =0; i < _num_col; i++)
    {
        for(int j = 0; j < _num_row; j++)
        {
            if(GetValue(i,j) != 0)
            {
                GameRect rt;
                rt._x = _posX+i*_tilew;
                rt._y = _posY+j*_tileh;
                rt._w = _tilew;
                rt._h = _tileh;
                if(rt.ISCollideRect(rect))
                {
                    arraycollide[collide_num] = j*_num_col+i;
                    collide_num++;
                    ret = true;
                }
            }
        }
    }
    return ret;

}
int GameFlatform::IsCollideRect(GameRect* rect)
{
    for(int i =0; i < _num_col; i++)
    {
        for(int j = 0; j < _num_row; j++)
        {
            if(GetValue(i,j) != 0)
            {
                GameRect rt;
                rt._x = _posX+i*_tilew;
                rt._y = _posY+j*_tileh;
                rt._w = _tilew;
                rt._h = _tileh;
                if(rt.ISCollideRect(rect))
                {
                    return  j*_num_col+i;
                }
            }
        }
    }
    return -1;
}
int GameFlatform::IsCollideWorld(GameFlatform* world)
{
    if(world != NULL)
    {
 
        for(int i =0; i < _num_col; i++)
        {
            for(int j = 0; j < _num_row; j++)
            {
                if(GetValue(i,j) != 0)
                {
                    GameRect rt;
                    rt._x = _posX+i*_tilew;
                    rt._y = _posY+j*_tileh;
                    rt._w = _tilew;
                    rt._h = _tileh;
                    int index = world->IsCollideRect(&rt);
                    if(index >= 0)
                    {
                        return  index;
                    }
                }
            }
        }
    }
    return -1;
}
Vecter2D GameFlatform::GetXYOfTile(int col,int row)
{
    Vecter2D pos;
    pos.x = _posX + col*_tilew;
    pos.y = _posY + col*_tileh;
    return pos;
}
bool GameFlatform::CheckCollideWorld(GameFlatform* world, int* arraycollide,int& collide_num)
{
    bool ret = false;
    collide_num = 0;
    if(world != NULL)
    {
        
        for(int i =0; i < _num_col; i++)
        {
            for(int j = 0; j < _num_row; j++)
            {
                if(GetValue(i,j) != 0)
                {
                    GameRect rt;
                    rt._x = _posX+i*_tilew;
                    rt._y = _posY+j*_tileh;
                    rt._w = _tilew;
                    rt._h = _tileh;
                    int a[100];
                    int num;
                    world->CheckCollideRect(&rt,a,num);
                    if(num > 0)
                    {
                        for(int c = 0; c < num; c++)
                        {
                            arraycollide[collide_num] = a[c];
                            collide_num++;
                        }
                        ret = true;
                    }
                }
            }
        }
    }
    return ret;
}

bool CheckCollideLeft(GameFlatform* world,int* arraycollide,int& collide_num)
{
	return false;
}
//bool CheckCollideRight(GameFlatform* world,int* arraycollide,int& collide_num);
//bool CheckColliedTop(GameFlatform* world,int* arraycollide,int& collide_num);
//bool CheckColliedBottom(GameFlatform* world,int* arraycollide,int& collide_num);
bool GameFlatform::IsInsideWorld(GameFlatform* world)
{
    float xx1 = _posX;
    float yy1 = _posY;
    float ww1 = _tilew*_num_col;
    float hh1 = _tileh*_num_row;
    
    float xx2 = world->GetPosX();
    float yy2 = world->GetPosY();
    float ww2 = world->GetTileW()*world->GetCols();
    float hh2 = world->GetTileH()*world->GetRows();
    
    if(CHECK_POINT_IN_RECT(xx1, yy1, xx2, yy2, ww2, hh2) &&
       CHECK_POINT_IN_RECT(xx1+ww1, yy1, xx2, yy2, ww2, hh2) &&
       CHECK_POINT_IN_RECT(xx1, yy1+hh1, xx2, yy2, ww2, hh2) &&
       CHECK_POINT_IN_RECT(xx1+ww1, yy1+hh1, xx2, yy2, ww2, hh2))
    {
        return true;
    }
    return false;
}
int GameFlatform::GetNumChilds()
{
    return _num_of_child;
}
void GameFlatform::AddChild(GameFlatform* child)
{
    if(_childs == NULL || _num_of_child == _max_of_child)
    {
        GameFlatform** tmp = _childs;
        _max_of_child += 10;
        _childs = new GameFlatform*[_max_of_child];
        for(int i = 0; i < _num_of_child; i++)
        {
            _childs[i] = tmp[i];
        }
    }
    _childs[_num_of_child] = child;
    _num_of_child++;
}
GameFlatform* GameFlatform::GetChildAtIndex(int index)
{
    return  _childs[index];
}

GameFlatform* GameFlatform::Me()
{
    return this;
}
void GameFlatform::SetParent(GameFlatform* p)
{
    _parent = p;
}
GameFlatform* GameFlatform::GetParent()
{
    return _parent;
}

void GameFlatform::UpdateFollowParent()
{
    if(_parent != NULL)
    {
        _posX += _parent->GetTransformX();
        _posY += _parent->GetTransformY();
    }
}
float GameFlatform::GetTransformX()
{
    return _transformX;
}
float GameFlatform::GetTransformY()
{
    return _transformY;
}
void GameFlatform::SetTransformXY(float x, float y)
{
    _transformY = y;
    _transformX = x;
}
float GameFlatform::GetTileW()
{
    return _tilew;
}
float GameFlatform::GetTileH()
{
    return _tileh;
}
void GameFlatform::SetPos(float x, float y)
{
    _posX = x;
    _posY = y;
}
GameFlatform::GameFlatform()
{
    _tilew = 0;
    _tileh = 0;
    _flatform = NULL;
    _childs = NULL;
    _num_of_child = 0;
    _max_of_child = 0;
}
GameFlatform::~GameFlatform()
{
        if(_flatform)
        {
            delete[] _flatform;
        }
        if(_childs != NULL)
        {
            delete[] _childs;
        }
}
void GameFlatform::SetSizeTile(float w, float h)
{
    _tilew = w;
    _tileh = h;
    
}

int GameFlatform::GetCols()
{
    return _num_col;
}
int GameFlatform::GetRows()
{
    return _num_row;
}

void GameFlatform::SetSize(int col, int row)
{
    _num_col = col;
    _num_row = row;
    if(_flatform != NULL)
    {
        delete[] _flatform;
    }
    _flatform = new int[_num_col*_num_row];
    for(int  i = 0; i < _num_col*_num_row; i++)
    {
        _flatform[i] = 0;
    }
}

int GameFlatform::GetValue(int col, int row)
{
    if(col < 0 || row < 0 || col >= _num_col || row >= _num_row)
    {
        return -1;
    }
    return _flatform[row*_num_col + col];
}
void GameFlatform::SetValue(int col, int row, int value)
{
    _flatform[row*_num_col + col] = value;
}

CPos GameFlatform::GetPosOfTile(float posX, float posY)
{
    CPos pos = {-1,-1};
    float offX = posX - _posX;
    float offY = posY - _posY;
    pos._col = (int)(offX / _tilew);
    pos._row = (int)(offY / _tileh);
    return pos;
}

void GameFlatform::SetColorDebug(int color)
{
    _color_debug = color;
}


void GameFlatform::RenderDebug()
{
    //draw rect
    G()->SetColor(_color_debug);
    G()->DrawRect(_posX, _posY,_tilew*_num_col,_tileh*_num_row);
    for(int i = 0; i < _num_row; i++)
    {
        for(int j = 0; j < _num_col; j++)
        {
            int value = GetValue(j,i);
            if(value > 0)
            {
                float xx = _posX +  j*_tilew;
                float yy = _posY + i*_tileh;
                G()->FillRect(xx,yy,_tilew,_tileh);
            }
        }
    }
}