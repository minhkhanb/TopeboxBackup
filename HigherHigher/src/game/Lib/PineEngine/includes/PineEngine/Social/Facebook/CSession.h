#if !defined(_H_PINE_ENGINE_SOCIAL_FACEBOOK_CSESSION)
#define _H_PINE_ENGINE_SOCIAL_FACEBOOK_CSESSION

#include "../../common.h"
#include <string>

namespace Pine 
{
	namespace Social 
	{
		namespace Facebook
		{
			class PE_EXPORT CSessionInside;
			class PE_EXPORT CSession 
			{
			protected:
				CSessionInside* _inside;
			public:
				CSession();
				~CSession();
				void setToken(std::string token);
				std::string getToken();
			};
		}
	}
}
#endif//_H_PINE_ENGINE_SOCIAL_FACEBOOK_CSESSION