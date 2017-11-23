#if !defined(_H_PINE_ENGINE_IO_CRESOURCE_STREAM)
#define _H_PINE_ENGINE_IO_CRESOURCE_STREAM

#include "../common.h"
#include "../Resource/CResourceSegment.h"
#include "Interface/IBuffer.h"
#include "Interface/IStream.h"

namespace Pine
{
	namespace Io
	{
		class PE_EXPORT CResourceStreamInside;
		class PE_EXPORT CResourceStream : public Interface::IStream, public Interface::IBuffer
		{
		private:
			CResourceStreamInside* _inside;
		public:
			static CResourceStream* fromResourceSegment(Resource::CResourceSegment rseg);

			CResourceStream();
			~CResourceStream();

			//implement  IStream
			virtual bool canRead();
			virtual bool canWrite();
			virtual bool open();
			virtual void close();

			//implement IBuffer
			virtual size_t getLength();
			virtual void seek(ESeekType type, size_t count = 0);

			virtual uint8_t readByte();
			virtual int16_t readInt16();
			virtual uint16_t readUInt16();
			virtual int32_t readInt32();
			virtual uint32_t readUInt32();
			virtual float readFloat();
			virtual int64_t readInt64();
			virtual uint64_t readUInt64();
			virtual double readDouble();

			virtual void writeByte(uint8_t val);
			virtual void writeInt16(int16_t val);
			virtual void writeUInt16(uint16_t val);
			virtual void writeInt32(int32_t val);
			virtual void writeUInt32(uint32_t val);
			virtual void writeFloat(float val);
			virtual void writeInt64(int64_t val);
			virtual void writeUInt64(uint64_t val);
			virtual void writeDouble(double val);
		};
	}
}

#endif//_H_PINE_ENGINE_IO_CRESOURCE_STREAM