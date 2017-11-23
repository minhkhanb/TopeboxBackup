#if !defined(_H_PINE_ENGINE_SOCIAL_FACEBOOK_CFACEBOOK_EXCEPTION)
#define _H_PINE_ENGINE_SOCIAL_FACEBOOK_CFACEBOOK_EXCEPTION

#include <exception>
#include <string>

#include "../../common.h"

namespace Pine
{
    namespace Social
    {
        namespace Facebook
        {
            enum class PE_EXPORT EFacebookException
            {
                INVALID_SESSION,
                INVALID_TOKEN,
                REASON,
            };
            
            class PE_EXPORT CFacebookException : public std::exception
            {
            protected:
                EFacebookException _code;
                std::string _reason;
                
            public:
                CFacebookException(EFacebookException code);
                CFacebookException(std::string reason);
                virtual const char * what () const throw ();
            };
        }
    }
}

#endif//_H_PINE_ENGINE_SOCIAL_FACEBOOK_CFACEBOOK_EXCEPTION