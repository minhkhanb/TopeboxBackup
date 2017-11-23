#ifndef _H_PINE_ENGINE_SERVICE_SHELL_CBASIC_AUTHORIZE_REQUEST
#define _H_PINE_ENGINE_SERVICE_SHELL_CBASIC_AUTHORIZE_REQUEST

#include "../common.h"
#include "CCryptoRequest.h"


namespace Pine
{
    namespace ServiceShell
    {
		using namespace Pine::Platform;
		using namespace Pine::Platform::Interface;
		using namespace Pine::Io;

        class PE_EXPORT CBasicAuthorizeV1Request : virtual public CCryptoRequest
        {
        public:
            //virtual int getBufferLength() = 0;
            //virtual void writeToBuffer(CBuffer* buffer) = 0;
            //virtual void readFromBuffer(CBuffer* buffer) = 0;
        };

        typedef CRequestStream<CBasicAuthorizeV1Request, PINE_SS_ID_CRYPTO , CallbackCrypto> CBasicAuthorizeV1RequestStream;
    }
}

#endif//_H_PINE_ENGINE_SERVICE_SHELL_CBASIC_AUTHORIZE_REQUEST