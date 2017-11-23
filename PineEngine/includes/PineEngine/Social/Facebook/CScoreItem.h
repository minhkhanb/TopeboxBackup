#pragma once
#if !defined(_H_PINE_ENGINE_SOCIAL_FACEBOOK_CSCORE_ITEM)
#define _H_PINE_ENGINE_SOCIAL_FACEBOOK_CSCORE_ITEM

#include "../../common.h"

#include <string>

namespace Pine
{
	namespace Social
	{
		namespace Facebook
		{
			class PE_EXPORT CScoreItem
			{
			public:
                CScoreItem();
                std::string Id;
				std::wstring UserName;
				int Score;
			};
		}
	}
}

#endif//_H_PINE_ENGINE_SOCIAL_FACEBOOK_CSCORE_ITEM
