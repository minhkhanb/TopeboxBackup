#ifndef _PINE_HIGH_SCORE_H
#define _PINE_HIGH_SCORE_H

#define PINE_HIGH_SCORE_FRIEND_LIST_MAX_CACHE	5000
#define ID_PINE_HIGH_SCORE_MAX_LENGTH   36
#define PINE_HIGH_SCORE_MAX_TYPE_SCORE		10
#define PINE_HIGH_SCORE_MAX_TYPE_CURRENCY	3
#define PINE_HIGH_SCORE_MAX_TYPE_ACHIVEMENT	5
#define PINE_HIGH_SCORE_MAX_STRING_LENGTH	256

//rank highscore
#define PINE_HIGHSCORE_TYPE_DAILY       0
#define PINE_HIGHSCORE_TYPE_WEEKLY      1
#define PINE_HIGHSCORE_TYPE_ALLTIME     2

#define PINE_HIGHSCORE_TIME_CURRENT     0
#define PINE_HIGHSCORE_TIME_LAST        1

#define PINE_HIGHSCORE_SORTBY_ASCENDING 0
#define PINE_HIGHSCORE_SORTBY_DECENDING 1

//send request
#define PINE_SENREQUEST_TYPE_LIVE       0
#define PINE_SENREQUEST_TYPE_UNLOCK     1

//atribute friend highscore -> pine_friend_hs[INE_HIGH_SCORE_FRIEND_LIST_MAX_CACHE]
/*
 #define PINR_HIGHSCORE_FRIENDLIST_TOTAL_ATRIBUTE    35
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_ID_HIGHSCORE       0
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_GAMEID   1
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_USERID  2
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_USERNAME  3
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_AVATARURL 4
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_TOKEN  5
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_ALL_TIME_HS_1 6
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_ALL_TIME_HS_2  7
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_ALL_TIME_HS_3  8
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_ALL_TIME_HS_4  9
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_ALL_TIME_HS_5  10
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_ALL_TIME_HS_6  11
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_ALL_TIME_HS_7  12
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_ALL_TIME_HS_8  13
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_ALL_TIME_HS_9  14
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_ALL_TIME_HS_10  15
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_DAILY_HS_1  16
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_DAILY_HS_2  17
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_DAILY_HS_3  18
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_DAILY_HS_4  19
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_DAILY_HS_5  20
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_DAILY_HS_6  21
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_DAILY_HS_7  22
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_DAILY_HS_8  23
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_DAILY_HS_9  24
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_DAILY_HS_10  25
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_WEEKLY_HS_1  26
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_WEEKLY_HS_2  27
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_WEEKLY_HS_3  28
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_WEEKLY_HS_4  29
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_WEEKLY_HS_5  30
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_WEEKLY_HS_6  31
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_WEEKLY_HS_7  32
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_WEEKLY_HS_8  33
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_WEEKLY_HS_9  34
 #define PINE_HIGHSCORE_FRIENDLIST_ATRIBUTE_WEEKLY_HS_10  35
 
 */

//atribute friend highscore

class PineHighScore
{
	int *_array_index;
public:
	PineHighScore()
	{
		_total_result = 0;
		//_total_result_send_request = 0;
	};
    //int _total_friend;
    //INT_TIME _GC_Friend[PINE_HIGH_SCORE_FRIEND_LIST_MAX_CACHE];
    //char _id_high_score_friend_list[PINE_HIGH_SCORE_FRIEND_LIST_MAX_CACHE][ID_PINE_HIGH_SCORE_MAX_LENGTH];
	INT_TIME _gameID;
	INT_TIME _userID;
    char _my_id_high_score[ID_PINE_HIGH_SCORE_MAX_LENGTH];
    char _my_userName[PINE_HIGH_SCORE_MAX_STRING_LENGTH];
    char _my_avartarURL[PINE_HIGH_SCORE_MAX_STRING_LENGTH];
    char _my_token[PINE_HIGH_SCORE_MAX_STRING_LENGTH];
    
    struct UserHighScore
    {
        INT_TIME _currency[PINE_HIGH_SCORE_MAX_TYPE_CURRENCY];
        INT32 _achivement_count[PINE_HIGH_SCORE_MAX_TYPE_ACHIVEMENT];
        /**/
        double _allTimeScore[PINE_HIGH_SCORE_MAX_TYPE_SCORE];
        double _weeklyScore[PINE_HIGH_SCORE_MAX_TYPE_SCORE];
        double _dailyScore[PINE_HIGH_SCORE_MAX_TYPE_SCORE];
        //field only GET
        double _lastWeeklyScore[PINE_HIGH_SCORE_MAX_TYPE_SCORE];
        double _lastDailyScore[PINE_HIGH_SCORE_MAX_TYPE_SCORE];
    public:
        UserHighScore()
        {
            for(int i = 0; i < PINE_HIGH_SCORE_MAX_TYPE_CURRENCY; i++)
            {
                _currency[i] = 0;
            }
            for(int i = 0; i < PINE_HIGH_SCORE_MAX_TYPE_ACHIVEMENT; i++)
            {
                _achivement_count[i] = 0;
            }
            for(int i = 0; i< PINE_HIGH_SCORE_MAX_TYPE_SCORE; i++)
            {
                _allTimeScore[i] = 0;
                _weeklyScore[i] = 0;
                _dailyScore[i] = 0;
                
            }
        }
    };
    typedef struct UserHighScore USER_HIGHSCORE;
    
    //UserHighScore _struct;
    
    USER_HIGHSCORE _user_highscore;
    
	struct Record
	{
		INT_TIME _userID;
		char _userName[PINE_HIGH_SCORE_MAX_STRING_LENGTH];
		char _avartarURL[PINE_HIGH_SCORE_MAX_STRING_LENGTH];
		char _token[PINE_HIGH_SCORE_MAX_STRING_LENGTH];
		double _score;
		double _lastScore;
	};
    
    struct pine_friend_highscore
    {
        char _id_highscore[PINE_HIGH_SCORE_MAX_STRING_LENGTH];
        INT_TIME _userID;
        INT_TIME _gameID;
        char _userName[PINE_HIGH_SCORE_MAX_STRING_LENGTH];
        char _avartarURL[PINE_HIGH_SCORE_MAX_STRING_LENGTH];
        char _token[PINE_HIGH_SCORE_MAX_STRING_LENGTH];
        INT_TIME _currency[3];
        INT32 _archivementCount[5];
        INT_TIME _all_time_score[10];
        INT_TIME _daily_score[10];
        INT_TIME _weekly_score[10];
        INT_TIME _last_daily_score[10];
        INT_TIME _last_weekly_score[10];
    };
    typedef struct pine_friend_highscore PINE_FRIEND_HIGHSCORE;
    int _num_friend;
    //PINE_FRIEND_HIGHSCORE  _friend_highscore[PINE_HIGH_SCORE_FRIEND_LIST_MAX_CACHE];
    PINE_FRIEND_HIGHSCORE* _friend_highscore;
    
	struct SendRequest
	{
		INT_TIME _gameID;
		INT_TIME _receiverID;
        INT_TIME _senderID;
		int _type_request;
	};
    typedef struct SendRequest SENDREQUEST;
	//SENDREQUEST _friend_sendrequest[PINE_HIGH_SCORE_FRIEND_LIST_MAX_CACHE];
    SENDREQUEST* _friend_sendrequest;
    int _total_data_save_sendrequest;
    
    struct RankHighScore
    {
        //INT_TIME ID;
        INT_TIME GameID;
        INT_TIME UserID;
        int rank;
        double score;
        double highscore_server;
    };
    typedef struct RankHighScore RANKHIGHSCORE;
    //RANKHIGHSCORE _friend_rank_highscore[PINE_HIGH_SCORE_FRIEND_LIST_MAX_CACHE];
    RANKHIGHSCORE* _friend_rank_highscore;
    int _total_friend_rank_highscore;
    int GetRankOfFriend(INT_TIME uid);
    //get top highscore
    struct tophighscore
    {
        INT_TIME _userID;
        double _score;
        char _username[PINE_HIGH_SCORE_MAX_STRING_LENGTH];
        char _avatarURL[PINE_HIGH_SCORE_MAX_STRING_LENGTH];
    };
    typedef struct tophighscore TOPHIGHSCORE;
    TOPHIGHSCORE *_tophighscore;
    //method get top highscore
    int get_top_highscore(int top, int scoreType, int timeGet, int scoreNumber);
    
    
    //methods of SENDREQUEST
    //insert send request    
    int insertSQ(PineHighScore *user_pine, PineHighScore::SENDREQUEST *request, INT_TIME list_gc_friend[PINE_HIGH_SCORE_FRIEND_LIST_MAX_CACHE], int n_friend);
    // store sendrequest from server
    int storeSQ_from_server(PineHighScore* user_pine);
    // delete sendrequest at save local
    int delete_SQ_save_at(int index);
    // load data from save, data afer being loaded, be saved at _friend_sendrequest
    int load_data_SQ_save();
    
    //methods get rank highscore
    int getRankHighScore(PineHighScore *user,INT_TIME* list_gc_friend, int total_friend,int scoreTYPE,int scoreTIME,int scoreNUM,int sortby);
    
    int getRankHighScoreAtUser(PineHighScore *user,int scoreTYPE,int scoreTIME,int scoreNUM,int sortby);
    
    static int getRankHighScore(INT_TIME user_id,INT_TIME game_id,int scoreTYPE,int scoreTIME,int scoreNUM,int sortby);
    
    static int getRankHighScoreTest(INT_TIME game_id,INT_TIME user_id,int scoreID,int scoreTIME,int scoreNUM,int sortby,double score);
	//int _total_result_send_request;
	//-------------------------------------------------------------
	//	loging server and set value for _gameID, _userId, _currency, _achivement_count
	//	return 0 -> success
	//--------------------------------------------------------------
	int Init(INT_TIME gameID, INT_TIME userID,const char* username, const char* avatarUrl, const char *token);
	Record *_result;
	int _total_result;
	//sort
	void SortAscendingByScore();
	void SortDescendingByScore();
	//pushScore
    /*
     _scoreArray[30] consist of:
     0 -> 9 :Alltime HighScore
     10 -> 19: daily HighScore
     20 -> 29: weekly HighScore
     */
	//double _scoreArray[30];
	//----------------------------------------------------------
	//	pushScore to server by array _scoreArray
	//	return 0 -> success
	//----------------------------------------------------------
	
    //update
    int Update_user_highscore(USER_HIGHSCORE user);
    //store friend highscore (load save)
    void store_friend_highscore(INT_TIME list_gc_friend[PINE_HIGH_SCORE_FRIEND_LIST_MAX_CACHE],int n_friend);
    
	//loging
	//return 0 -> success
	int UpdateLoging();
    
	//-------------------------------------------------------------
	//	get Special last update
	//	return minute last update : -1 if not exist
	//--------------------------------------------------------------
	int getMinuteLastTimeUpdate(INT_TIME userID, INT_TIME gameID);
	
	//-----------------------------------------------------------
	//	get Rank in database
	//	return 0 -> success
	//	@param type_score : 1 -> PINE_HIGH_SCORE_MAX_TYPE_SCORE
 	//	@param type_time : 0:HighScore; 1:DailyScore ; 2:WeeklyScore
	//	@param count_to_get : max result will get
	//	@param isAscending
	//-----------------------------------------------------------
	int getRankByTypeScore(int type_score, BYTE type_time,  int count_to_get, bool isAscending);
	//------------------------------------------------------------
	//	store score to _result, _result must have list _userID
	//	befor use this function, _total_result is count of friend list get
	//	after use this function, _total_result is count of friend list respone
	//	return 0 -> success
	//	@param type_score : 1 -> PINE_HIGH_SCORE_MAX_TYPE_SCORE
 	//	@param type_time : 0:HighScore; 1:WeeklyScore; 2:DailyScore
	//	if type_time == 0, _lastScore = 0;
	//	@param isAscending
	//------------------------------------------------------------
	int storeToResultFromFriendList(int type_score, BYTE type_time, bool isAscending);//isAscending->false: Descending
    
    
	//send request
    
	/*------------------------------------------------------------
	 get _result_send_request store in _result_send_request
     _total_result_send_request
     ------------------------------------------------------------*/
	//int storeSendRequest();// select all
	//int storeSendRequest_type(int type);// select by type
	/*------------------------------------------------------------
	 delete _result_send_request;
     return 1 -> fasle (fail)
     return 0 -> true (success)
     ------------------------------------------------------------*/
	int deleteSendRequest(INT_TIME sendrequestID);
    
	/*------------------------------------------------------------
     insert _result_send_request;
     return 1 -> fasle (fail)
     return 0 -> true (success)
     ------------------------------------------------------------*/
	//int insertSendRequest(INT_TIME RecieverID,int cout, int type);
	/*
     */
    
    int getFriendHighScoreFromSave();
    int storeFriendHighScoreToSave();
    
	Record getResultRankingRecord(int rank)
	{
		return _result[_array_index[rank]];
	};
};

#endif