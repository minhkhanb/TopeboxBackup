#ifndef _H_PINE_ENGINE_SERVICE_SHELL_CHTTP_REQUEST
#define _H_PINE_ENGINE_SERVICE_SHELL_CHTTP_REQUEST
#include "../common.h"
#include "../Io/Interface/IStorable.h"
#include "CRequestStream.h"
#include <utility>
#include <string>
#include <vector>
#include <map>

namespace Pine
{
    namespace ServiceShell
    {
		using namespace Pine::Platform;
		using namespace Pine::Io::Interface;
		using namespace Pine::Io;

        class PE_EXPORT CHttpRequest : virtual public IStorable
        {
        public:
            enum class PE_EXPORT EHttpMethod
            {
                GET = 1,
                PUT = 2,
                POST = 3,
                DEL = 4,
            };
            
        protected:
            std::string _url;
            EHttpMethod _method;
            CBytes _raw_data;
            std::map<std::string, std::string> _header;
            char* _responsed;

        public:
            CHttpRequest();
            virtual ~CHttpRequest();
            void init(std::string url,EHttpMethod method = EHttpMethod::GET);
            void setUrl(std::string url);
            void setMethod(EHttpMethod method);
            void setData(CBytes data);
            void setHeader(std::string key, std::string value);

            virtual size_t getBufferLength();
            virtual void writeToBuffer(CBuffer* buffer);
            virtual void readFromBuffer(CBuffer* buffer);
			virtual void updateFromBuffer(CBuffer* buffer);
        };

        typedef void(*CallbackHttpRequestPtr)(int id, int status,char**, int length);
		typedef std::function<void(int id, int status, CBytes data)> CallbackHttpRequest;

        typedef CRequestStream<CHttpRequest, PINE_SS_ID_HTTP , CallbackHttpRequest> CHttpRequestStream;
    }
}

#endif//_H_PINE_ENGINE_SERVICE_SHELL_CCRYPTO_REQUEST

