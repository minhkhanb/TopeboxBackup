#if !defined(_PINE_ENGINE_IO_IBUFFER)
#define _PINE_ENGINE_IO_IBUFFER
#include <cstdint>
#include <cstring>
#include "../../common.h"
#include "../ESeekType.h"

namespace Pine 
{
	namespace Io
	{
		namespace Interface
		{
			class PE_EXPORT IBuffer
			{
				virtual size_t getLength() = 0;
				virtual void seek(ESeekType type, size_t count = 0) = 0;
				
				virtual uint8_t readByte() = 0;
				virtual int16_t readInt16() = 0;
				virtual uint16_t readUInt16() = 0;
				virtual int32_t readInt32() = 0;
				virtual uint32_t readUInt32() = 0;
				virtual float readFloat() = 0;
				virtual int64_t readInt64() = 0;
				virtual uint64_t readUInt64() = 0;
				virtual double readDouble() = 0;

				virtual void writeByte(uint8_t val) = 0;
				virtual void writeInt16(int16_t val) = 0;
				virtual void writeUInt16(uint16_t val) = 0;
				virtual void writeInt32(int32_t val) = 0;
				virtual void writeUInt32(uint32_t val) = 0;
				virtual void writeFloat(float) = 0;
				virtual void writeInt64(int64_t val) = 0;
				virtual void writeUInt64(uint64_t val) = 0;
				virtual void writeDouble(double) = 0;

			};
		}
	}
}
#endif//_PINE_ENGINE_IO_IBUFFER
