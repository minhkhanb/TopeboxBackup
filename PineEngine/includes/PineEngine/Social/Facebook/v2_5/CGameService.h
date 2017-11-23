#pragma once
#if !defined(_H_PINEENGINE_SOCIAL_FACEBOOK_v2_5_CGAME_SERVICE)
#define _H_PINEENGINE_SOCIAL_FACEBOOK_v2_5_CGAME_SERVICE

#include "../../../common.h"
#include <memory>
#include <vector>

#include "../CSession.h"
#include "../CApplication.h"
#include "../CScoreItem.h"
#include "../../../Os/OS.h"


namespace Pine
{
	namespace Social
	{
		namespace Facebook
		{
			namespace v2_5
			{
				class PE_EXPORT CGameServiceInside;
				class PE_EXPORT CGameService
				{
				private:
					CGameServiceInside* _inside;
					CGameService();
					
				public:
					~CGameService();

                    static std::shared_ptr<CGameService> get(std::shared_ptr<CApplication> app, std::shared_ptr<CSession> session);
					//get user score
					std::vector<CScoreItem> getScore();

					//put user score
					bool putScore(int score);
                    
                    bool deleteScore();
                    

					//get friends score
					std::vector<CScoreItem> getFriendsScore();

				};
			}
		}
	}
}

#endif //_H_PINEENGINE_SOCIAL_FACEBOOK_v2_5_CGAME_SERVICE
