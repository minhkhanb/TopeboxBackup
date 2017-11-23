#ifndef _H_PINE_ENGINE_SERVICE_SHELL_CCRYPTO_REQUEST
#define _H_PINE_ENGINE_SERVICE_SHELL_CCRYPTO_REQUEST
#include "../common.h"
#include "../Io/Interface/IStorable.h"
#include "CRequestStream.h"

namespace Pine
{
    namespace ServiceShell
    {
		using namespace Pine::Platform;
		using namespace Pine::Io::Interface;
		using namespace Pine::Io;

        class PE_EXPORT CCryptoRequest : virtual public IStorable
        {
        public:
            enum class PE_EXPORT ECryptoMethod
            {
                AES = 1
            };
            enum class PE_EXPORT ECryptoMode
            {
                CBC = 1
            };
            enum class PE_EXPORT ECryptoPadding
            {
                PKCS7 = 1
            };
        protected:
            ECryptoMethod _method;
            ECryptoMode _mode;
            ECryptoPadding _padding;

            CBytes _iv;
			CBytes _key;
			CBytes _raw_data;
            uint8_t* _encrypted;

        public:
            CCryptoRequest();
            virtual ~CCryptoRequest();
            void init(ECryptoMethod method = ECryptoMethod::AES, ECryptoMode mode = ECryptoMode::CBC, ECryptoPadding padding = ECryptoPadding::PKCS7);
            void setMethod(ECryptoMethod method);
            void setMode(ECryptoMode mode);
            void setPadding(ECryptoPadding padding);
            void setIv(CBytes iv);
            void setKey(CBytes key);
            void setData(CBytes data);

            virtual size_t getBufferLength();
            virtual void writeToBuffer(CBuffer* buffer);
            virtual void readFromBuffer(CBuffer* buffer);
			virtual void updateFromBuffer(CBuffer* buffer);
        };

        typedef void(*CallbackCryptoPtr)(int id, const char** data, int data_len);
		typedef std::function<void(int id, CBytes data)> CallbackCrypto;

        typedef CRequestStream<CCryptoRequest, PINE_SS_ID_CRYPTO , CallbackCrypto> CCryptoRequestStream;
    }
}

#endif//_H_PINE_ENGINE_SERVICE_SHELL_CCRYPTO_REQUEST

