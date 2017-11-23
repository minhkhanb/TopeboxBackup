#if !defined(_H_PE_SECURITY_IBASIC_AUTHORIZE)
#define _H_PE_SECURITY_IBASIC_AUTHORIZE

#include "../../common.h"
#include <string>
#include <memory>

namespace Pine
{
	namespace Security
	{
		namespace Interface {
			class PE_EXPORT IBasicAuthorize
			{
			public:
				virtual std::string getPass(std::string id) = 0;
			};
		}
	}
}

#endif//_H_PE_SECURITY_IBASIC_AUTHORIZE