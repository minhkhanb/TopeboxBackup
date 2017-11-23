#include "pch.h"
#include "../GameCore/CGame.h"
#include <fstream>
#include "PineHighScore.h"
extern "C" bool getRankByTypeScore_extern(int type_score, BYTE type_time,  int count_to_get, bool isAscending);
extern "C" bool storeToResultFromFriendList_extern(int type_score, BYTE type_time, bool isAscending);
extern "C" int getMinuteLastTimeUpdate_extern(INT_TIME userID, INT_TIME gameID);
//rank highscore
extern "C" int getRankHighScore_ext(PineHighScore* user_pine,INT_TIME* list_gc_friend,
                                int total_friend,int scoreID,int scoreTIME,int scoreNUM,int sortby);
//send request
extern "C" int insertSendRequest(PineHighScore* user_pine,PineHighScore::SENDREQUEST *request,
                                 INT_TIME list_gc_friend[PINE_HIGH_SCORE_FRIEND_LIST_MAX_CACHE],int n_friend);
extern "C" int storeReceiveRequest(PineHighScore* user_pine);
extern "C" int load_data_sendrequest_from_save();
extern "C" int detele_data_save_local(int index);
//high score
extern "C" int IsEmpty();
extern "C" int insert_pine_highscore_extern(PineHighScore::USER_HIGHSCORE _highscore);
extern "C" int update_pine_highscore_extern(PineHighScore::USER_HIGHSCORE user_highscore);
extern "C" int storeUserResult_extern();
extern "C" int store_friend_highscore_extern(INT_TIME list_gc_friend[PINE_HIGH_SCORE_FRIEND_LIST_MAX_CACHE],int n_friend);
extern "C" int getRankOfUser(INT_TIME user_id,INT_TIME gameid,int scoreID,int scoreTIME,int scoreNUM,int sortby);
extern "C" int getRankHighScoreTest_ext(INT_TIME,INT_TIME,int scoreID,int scoreTIME,int scoreNUM,int sortby,double score);
extern "C" int get_top_highscore_ext(int _top, int scoreType, int timeGet, int scoreNumber, INT_TIME gameID);
//extern "C" int insert_pine_highscore_extern(USER_HIGHSCORE _highscore);
/*extern "C" bool getID_SendRequest_extern(INT_TIME GameID, INT_TIME userID);
 extern "C" bool delete_SendRequest_extern(INT_TIME ID);
 extern "C" bool insert_New_SendRequest_extern(INT_TIME GameID, INT_TIME userID,INT_TIME RecieveID,INT_TIME cout, INT_TIME type);*/
#ifdef MAC_OS
int PineHighScore::getRankByTypeScore(int type_score, BYTE type_time,  int count_to_get, bool isAscending)
{
	int ret = 0;
    
	if (!getRankByTypeScore_extern(type_score, type_time, count_to_get, isAscending))
	{
		return 1; //network error
	}
    
	return ret;
}

/*int PineHighScore::insert_highscore(USER_HIGHSCORE _highscore)
 {
 return insert_pine_highscore_extern(_highscore);
 }*/
int PineHighScore::Init(INT_TIME gameID, INT_TIME userID,const char* username, const char* avatarUrl, const char *token)
{
    int ret;
	if (!OS_IsNetworkAvailable())
		ret = 1; //network error
    _gameID = gameID;
    _userID = userID;
    /*if (IsEmpty() == 0)
     {
     insert_pine_highscore_extern();
     printf("\ninsert success");
     }
     else
     {
     
     printf("\n not empty");
     }*/
    if (storeUserResult_extern() == 0)
        ret = 0;//exist data
    else
    {
        ret = 2;//not exist data
        sprintf(_my_userName,"%s",username);
        sprintf(_my_avartarURL,"%s",avatarUrl);
        sprintf(_my_token,"%s",token);
        for (int i = 0;i < 10;i++)
        {
            _user_highscore._allTimeScore[i] = 0;
            _user_highscore._dailyScore[i] = 0;
            _user_highscore._weeklyScore[i] = 0;
        }
    }
    
	return ret;
}


int PineHighScore::UpdateLoging()
{
	if (!OS_IsNetworkAvailable())
		return 1; //network error
	return 0;
}



int PineHighScore::Update_user_highscore(USER_HIGHSCORE user)
{
    
    return update_pine_highscore_extern(user);
	
}

void PineHighScore::SortAscendingByScore()
{
	for (int i = 0; i < _total_result - 1; i++)
	{
		for (int j = i +1 ; j < _total_result ; j++)
		{
			if (_result[_array_index[i]]._score > _result[_array_index[j]]._score)
			{
				int temp = _array_index[j];
				_array_index[j] = _array_index[i];
				_array_index[i] = temp;
			}
		}
	}
}

void PineHighScore::SortDescendingByScore()
{
	for (int i = 0; i < _total_result - 1; i++)
	{
		for (int j = i +1 ; j < _total_result ; j++)
		{
			if (_result[_array_index[i]]._score < _result[_array_index[j]]._score)
			{
				int temp = _array_index[j];
				_array_index[j] = _array_index[i];
				_array_index[i] = temp;
			}
		}
	}
}

int PineHighScore::storeToResultFromFriendList(int type_score, BYTE type_time, bool isAscending)
{
	int ret = 0;
	if (storeToResultFromFriendList_extern(type_score, type_time, isAscending) == 1)
	{
		return 1;//network error
	}
	ret = storeToResultFromFriendList_extern(type_score, type_time, isAscending);
	
	_array_index = new int[_total_result];
    
	for (int i = 0; i < _total_result; i++)
	{
		_array_index[i] = i;
	}
    
	if (isAscending)
	{
        
		SortAscendingByScore();
	}
	else
	{
		SortDescendingByScore();
	}
	return ret;
}






int PineHighScore::getFriendHighScoreFromSave()
{
    int ret = 0;
    if (!OS_IsNetworkAvailable())
		return 1; //network error
    return ret;
}


void PineHighScore::store_friend_highscore(INT_TIME list_gc_friend[PINE_HIGH_SCORE_FRIEND_LIST_MAX_CACHE],int n_friend)
{
    printf("\n data in");
    for (int i= 0 ; i < n_friend;i++)
    {
        printf("\n gc[%d] = %lld",i,list_gc_friend[i]);
    }
    store_friend_highscore_extern(list_gc_friend ,n_friend);
}

int PineHighScore::get_top_highscore(int top,int scoreType, int timeGet, int scoreNumber)
{
    return get_top_highscore_ext(top,scoreType,timeGet,scoreNumber,APPLE_ID);
}

// SENDREQUEST
int PineHighScore::insertSQ(PineHighScore *user_pine, PineHighScore::SENDREQUEST *request, INT_TIME list_gc_friend[PINE_HIGH_SCORE_FRIEND_LIST_MAX_CACHE], int n_friend)
{
    return insertSendRequest(user_pine,request,list_gc_friend,n_friend);
}

int PineHighScore::load_data_SQ_save()
{
    return load_data_sendrequest_from_save();
}

int PineHighScore::storeSQ_from_server(PineHighScore* user_pine)
{
    return storeReceiveRequest(user_pine);
}

int PineHighScore::delete_SQ_save_at(int index)
{
    return detele_data_save_local(index);
}

//RANK HIGHSCORE
int PineHighScore:: getRankHighScore(PineHighScore *user,INT_TIME* list_gc_friend, int total_friend,int scoreID,int scoreTIME,int scoreNUM,int sortby)
{
    return getRankHighScore_ext(user,list_gc_friend,total_friend, scoreID, scoreTIME, scoreNUM, sortby);
}


int PineHighScore::getRankHighScoreAtUser(PineHighScore *user,int scoreID,int scoreTIME,int scoreNUM,int sortby)
{
    INT_TIME list_gc_friend[1];
    list_gc_friend[0] = user->_userID;
    int total_friend = 1;
    return getRankHighScore_ext(user,list_gc_friend,total_friend, scoreID, scoreTIME, scoreNUM, sortby);
}
 int PineHighScore::getRankHighScore(INT_TIME user_id,INT_TIME game_id,int scoreTYPE,int scoreTIME,int scoreNUM,int sortby)
{
   
    return getRankOfUser(user_id,game_id, scoreTYPE, scoreTIME, scoreNUM, sortby);
}

int PineHighScore::getRankHighScoreTest(INT_TIME game_id,INT_TIME user_id,int scoreID,int scoreTIME,int scoreNUM,int sortby,double score)
{
    return getRankHighScoreTest_ext(game_id,user_id,scoreID,scoreTIME,scoreNUM,sortby,score);
}

//RANKHIGHSCORE* _friend_rank_highscore;
//int _total_friend_rank_highscore;
int PineHighScore::GetRankOfFriend(INT_TIME uid)
{
    int rank = 9999;
    for(int i = 0; i < _total_friend_rank_highscore; i++)
    {
        if(_friend_rank_highscore[i].UserID == uid)
        {
            return _friend_rank_highscore[i].rank;
        }
    }
    return rank;
}
#endif