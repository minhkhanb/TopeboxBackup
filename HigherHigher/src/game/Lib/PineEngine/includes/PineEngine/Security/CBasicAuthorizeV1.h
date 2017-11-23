#if !defined (_H_PE_SECURITY_CBASIC_AUTHORIZE_V1)
#define _H_PE_SECURITY_CBASIC_AUTHORIZE_V1

#include "../common.h"
#include "Interface/IBasicAuthorize.h"
#include <memory>

namespace Pine
{
	namespace Security
	{
		class PE_EXPORT CBasicAuthorizeV1Inside;
		class PE_EXPORT CBasicAuthorizeV1:Interface::IBasicAuthorize
		{
		protected:
			CBasicAuthorizeV1Inside* _inside;
		public:
			CBasicAuthorizeV1(std::string encrypt_key);
			~CBasicAuthorizeV1();
			virtual std::string getPass(std::string id);
			std::string simpleEncrypt(std::string content);
		};
	}
}

#endif//_H_PE_SECURITY_CBASIC_AUTHORIZE_V1