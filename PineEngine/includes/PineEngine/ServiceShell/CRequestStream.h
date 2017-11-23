#ifndef _H_PINE_ENGINE_SERVICE_SHELL_CREQUEST_STREAM
#define _H_PINE_ENGINE_SERVICE_SHELL_CREQUEST_STREAM

#include "../common.h"
#include "../Io/Interface/IStorable.h"
#include "../Io/CBuffer.h"
#include "../Os/OS.h"
#include <string>
#include <functional>

#define PINE_SS_ID_CRYPTO		1
#define PINE_SS_ID_HTTP			2
#define PINE_SS_ID_CLOUD_WRITE	3
#define PINE_SS_ID_CLOUD_READ	4

namespace Pine
{
    namespace ServiceShell
    {
		using namespace Pine::Io;
		using namespace Pine::Platform;
		using namespace Pine::Io::Interface;
        using namespace Pine::Os;

        class PE_EXPORT CMiddleRequest : virtual public IStorable, virtual public CBuffer
        {
        protected:
            int32_t _id;
        public:
			CMiddleRequest();
			inline int32_t getId() { return _id; }
            virtual size_t getBufferLength() = 0;
            virtual void writeToBuffer(CBuffer* buffer) = 0;
            virtual void readFromBuffer(CBuffer* buffer) = 0;
			
			virtual void responsed() {
			
			};

            void update();
        };

        //T is class that extend IStorable
        //I is Instruction code
        //C is callback function's signature
        template <class T, size_t I, typename C>
        class PE_EXPORT CRequestStream : virtual public CMiddleRequest, virtual public T
        {
        private:
            C _callback;
            
        public:
            CRequestStream<T, I, C>(C callback) {
                
                _callback = callback;
            };

			virtual ~CRequestStream<T, I, C>() 
			{

			}

            virtual size_t getBufferLength()
            {
                return 8 + T::getBufferLength();
            };

            virtual void writeToBuffer(CBuffer* buffer)
            {
				if (buffer->length() < getBufferLength())
				{
					if (buffer->getType() != CBytes::Type::ABSTRACT)
					{
						buffer->init(getBufferLength());
					}
					else
					{
						throw "Cannot write to smaller buffer";
					}
				}
				
				buffer->resetPos();
	
                buffer->writeInt32(_id);
                buffer->writeInt32(I);
                T::writeToBuffer(buffer);
            };

            virtual void readFromBuffer(CBuffer* buffer)
            {
                buffer->resetPos();
                _id = buffer->readInt32();
                buffer->pass(4);
                T::readFromBuffer(buffer);
            };

			virtual void updateFromBuffer(CBuffer* buffer)
			{
				buffer->resetPos();
				_id = buffer->readInt32();
				buffer->pass(4);
				T::updateFromBuffer(buffer);
			};

			virtual void responsed() 
            {
				updateFromBuffer(this);
				resolve();
			};

            void setCallback(C callback)
            {
                _callback = callback;
            };

            void resolve();
        };
    }
}
#endif//_H_PINE_ENGINE_SERVICE_SHELL_CREQUEST_STREAM