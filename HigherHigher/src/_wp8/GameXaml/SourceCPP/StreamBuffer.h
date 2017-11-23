#if !defined(_H_STREAM_BUFFER)
#define _H_STREAM_BUFFER
namespace PhoneDirect3DXamlAppComponent{
		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class StreamBuffer sealed {
			public:
				property Platform::Array<uint8>^ Data;
				property int CurPos;
				void resetPos(){
					CurPos = 0;
				};
				StreamBuffer(int size);
				StreamBuffer(const Platform::Array<uint8>^ data);
				uint8 readByte();
				int16 readInt16();
				uint16 readUInt16();
				int32 readInt32();
				uint32 readUInt32();
				int64 readInt64();
				uint64 readUInt64();
				Platform::Array<uint8>^ readSBin();
				Platform::Array<uint8>^ readIBin();
				Platform::String^ readSStr();
				Platform::String^ readIStr();

				void writeByte(uint8 value);
				void writeInt16(int16 value);
				void writeUInt16(uint16 value);
				void writeInt32(int32 value);
				void writeUInt32(uint32 value);
				void writeInt64(int64 value);
				void writeUInt64(uint64 value);
				void writeSBin(const Platform::Array<uint8>^ data);
				void writeIBin(const Platform::Array<uint8>^ data);
				void writeSStr(Platform::String^ str);
				void writeIStr(Platform::String^ str);
		};
}
#endif//_H_STREAM_BUFFER