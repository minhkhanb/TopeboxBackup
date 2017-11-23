#include "pch.h"
#include "PineLeaderBoard.h"
accountInfo* PineLeaderBoard::GetAccountAt(int index)
{
    return _globalInfo[index];
}
void PineLeaderBoard::AddUserGlobal(accountInfo *account)
{
    if(_globalcur + 1 >= _globalmax)
    {
        ExpandGlobalBoard(20);
    }
    int index = _globalcur;
    for(int i = 0; i < _globalcur; i++)
    {
        if(account->_highScore > _globalInfo[i]->_highScore)
        {
            index= i;
            break;
        }
        
        
    }
    for(int i= _globalcur-1; i >= index; i--)
    {
        _globalInfo[i+1] = _globalInfo[i];
    }
    _globalInfo[index] = account;
    //printf("\n\\n\nadd account info: id=%lld, name =%s, avatar  =%s",account->_id,account->_userName,account->_avatarUrl);
    _globalcur++;
}
PineLeaderBoard::PineLeaderBoard()
{
    _globalInfo = NULL;
    _globalmax = 0;
    _globalcur =0;
}

void PineLeaderBoard::InitGlobalBoard(int num)
{
    int temp = _globalcur;
    _globalcur = 0;
    for(int i = 0; i < temp; i++)
    {
        SAFE_DEL(_globalInfo[i]);
    }
    SAFE_DEL_ARRAY(_globalInfo);
    _globalInfo = new accountInfo*[num];
    for(int i= 0; i< num;i++)
    {
        _globalInfo[i] = NULL;
    }
    //_globalcur = 0;
    _globalmax = num;
    
}
void PineLeaderBoard::RemoveAccount(INT_TIME accountid)
{
    int index = -1;
    //search account;
    for(int i = 0; i< _globalcur; i++)
    {
        if(_globalInfo[i] != NULL&& _globalInfo[i]->_id == accountid)
        {
            index= i;
            break;
        }
    }
    if(index >= 0)
    {
        for(int i = index; i < _globalcur-1; i++)
        {
            _globalInfo[i] = _globalInfo[i+1];
        }
        _globalInfo[_globalcur-1] = NULL;
        _globalcur--;
    }
}
void PineLeaderBoard::ExpandGlobalBoard(int num)
{
    accountInfo** temp= new accountInfo*[num+_globalmax];
    for(int i = 0; i<num+_globalmax;i++)
    {
        if(i < _globalcur)
        {
            temp[i] = _globalInfo[i];
        }
        else
        {
            temp[i] = NULL;
        }
    }
    _globalInfo = temp;
    _globalmax+=num;
}