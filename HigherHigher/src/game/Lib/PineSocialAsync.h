#if !defined(_H_PINE_SOCIAL_ASYNC)
#include "PineSocial.h"
#include "DataType/CType.h"
#define _H_PINE_SOCIAL_ASYNC

class PineSocialAsync{
public:
	//request to ask user to login their social account
	//if user haven't loged in yet. 
	//		this function show a dialog(by os) that ask user want to login their account or not.
	//		if user choice login with their acount. system change them to login screen.
	//		function change handle's status to Error if user cancel or login fail.
	//		function change handle's status to Completed if user loged in with their account.
	//if user loged their account before. this function chang handle's Status to Completed with 
	//		PARAM
	//		- handle: the handle object to manage request.
	//		- account_type: type of account we want ask user
	void RequestLoginAccount(PineServiceHandle* hsvc, PineAccountType account_type);

	//in some reason. we know that have some one use application with their account. 
	//We must validate the account to sure that it valid to continue using application.
	//this function validate account.
	//		change handle's status to Error if validate fail.
	//		change handle's status to Complete if validate success and account can continue using application safely.
	//		PARAM
	//		- handle: the handle object to manage request
	//		- account_type: type of account we want verify.
	void ValidateAccount(PineServiceHandle* hsvc, PineAccountType account_type);

	//in some reason. We know user want to log in their account. 
	//So we no need ask them anymore, we show them a dialog to they can log account in.
	//this function show login dialog to user log their acount in.
	//		it changes handle's status to Error if user can't login their account or account type not supported in current system.
	void LoginAccount(PineServiceHandle* hsvc, PineAccountType account_type);

	//load account basic info
	void LoadAccountInfo(PineServiceHandle* hsvc, PineAccountType account_type, PineAccountInfo* info);

	//load Inapp purchase list async
	void LoadIAP(PineServiceHandle* hsvc);

	//load friends
	//Facebook Account load app user friend.
	//Twitter Account load follower
	void LoadFriends(PineServiceHandle* hsvc, PineAccountType account_type, PineAccountIdList* account_list);
	
	void ShowLeaderBoard(PineServiceHandle* hsvc, PineAccountType account_type, const char* board_id);
	
	static void Post(PineServiceHandle* hsvc,PineAccountType account_type, const char* msg);
	
	static void Post(PineAccountType account_type, const char* msg);
	
	void UnlockAchievement(PineServiceHandle* hsvc,PineAccountType account_type, const char* achi_id);
	//Bo add
	void PostNewScore(PineServiceHandle* hsvc, BoardScoreItem item);
	
	
#if defined(BUILD_SUPPORT)
	void AskSupportAccount(PineServiceHandle* hsvc);
#endif
};
#endif