#ifndef _H_PINE_ENGINE_ISTORABLE
#define _H_PINE_ENGINE_ISTORABLE
#include "../../Io/CBuffer.h"

namespace Pine
{
    namespace Io
    {
        namespace Interface
        {
            class PE_EXPORT IStorable
            {
            public:
                IStorable() = default;
                virtual size_t getBufferLength() = 0;
                virtual void writeToBuffer(CBuffer* buffer) = 0;
                virtual void readFromBuffer(CBuffer* buffer) = 0;
				virtual void updateFromBuffer(CBuffer* buffer) = 0;
            };
        }
    }
}


#endif//_H_PINE_ENGINE_ISTORABLE