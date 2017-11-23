#if !defined(_PINE_ENGINE_IO_CBUFFER)
#define _PINE_ENGINE_IO_CBUFFER
#include <cstdint>
#include "../common.h"
#include "CBytes.h"
#include "Interface/IBuffer.h"

namespace Pine 
{
	namespace Io
	{
		class PE_EXPORT CBuffer:virtual public Interface::IBuffer, public CBytes
		{
		private:
			size_t _curr_pos;
		public:
			CBuffer();
			CBuffer(size_t len);
			CBuffer(const uint8_t* data, size_t len);
			CBuffer(const CBuffer& buff); //clone
			CBuffer& operator=(const CBuffer& obj); //copy
			CBuffer& operator=(const CBuffer&& obj); //move 

			static CBuffer* allocate(size_t size);
			static CBuffer* swap(uint8_t* bytes, size_t len);

			//implement IBuffer
			virtual void seek(ESeekType type, size_t count = 0);
			uint8_t* getCurrentDataPointer();
			void pass(size_t len);
			void resetPos();

			CBytes readBytes(size_t len);
			void writeBytes(uint8_t* data, size_t len);

			virtual size_t getLength();
			virtual void bind(uint8_t* data, size_t len);
			void bind(CBytes& bytes);

			static inline size_t getPointerLength() { return 1 + sizeof(void*); };

			virtual uint8_t readByte();
			virtual int16_t readInt16();
			virtual uint16_t readUInt16();
			virtual int32_t readInt32();
			virtual uint32_t readUInt32();
			virtual float readFloat();
			virtual int64_t readInt64();
			virtual uint64_t readUInt64();
			virtual double readDouble();
			CBytes readBin(int32_t len);
			CBytes readBBin();
			CBytes readSBin();
			CBytes readIBin();
			void* readPointer();

			virtual void writeByte(uint8_t val);
			virtual void writeInt16(int16_t val);
			virtual void writeUInt16(uint16_t val);
			virtual void writeInt32(int32_t val);
			virtual void writeUInt32(uint32_t val);
			virtual void writeFloat(float val);
			virtual void writeInt64(int64_t val);
			virtual void writeUInt64(uint64_t val);
			virtual void writeDouble(double val);
			
			void writeBin(const CBytes* bin);
			void writeBBin(const CBytes* bin);
			void writeSBin(const CBytes* bin);
			void writeIBin(const CBytes* bin);
			void writePointer(void* ptr);

			void passSBin();
			void passIBin();
		};
	}
}

#endif //_PINE_ENGINE_IO_CBUFFER
