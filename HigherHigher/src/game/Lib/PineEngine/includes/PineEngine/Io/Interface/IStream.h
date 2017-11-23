#if !defined(_H_PINE_ENGINE_IO_ISTREAM)
#define _H_PINE_ENGINE_IO_ISTREAM

#include "../../common.h"
#include "IBuffer.h"

namespace Pine
{
	namespace Io
	{
		namespace Interface
		{
			class PE_EXPORT IStream
			{
			public:
				//can we read on stream
				virtual bool canRead() = 0;

				//can we write on stream
				virtual bool canWrite() = 0;

				//open stream
				virtual bool open() = 0;

				//close stream
				virtual void close() = 0;
			};
		}
	}
}
#endif //_H_PINE_ENGINE_IO_ISTREAM