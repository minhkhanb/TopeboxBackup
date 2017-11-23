/*
//written by Duy Nguyen
*/
#if !defined(_H_BUFFER_STREAM)
#define _H_BUFFER_STREAM

#include "CType.h"
class BufferStream: public Buffer{
private:
	int _curr_pos;
public:
	BufferStream();
	BufferStream(int);
	BufferStream(const BYTE*, int);
	BufferStream(const BufferStream*);
	virtual void Init(int);
	virtual void Init(const BYTE*, int);
	virtual void Release();
	~BufferStream();
	void Seek(int pos, int begin_at = SEEK_SET);
	void Skip(int byte_num = 1);
	bool EOS();
	void WriteByte(BYTE data);
	BYTE ReadByte();

	void WriteBytes(const BYTE* data, int len);
	void WriteBytes(Buffer* buff);
	void WriteStr(const UNICHAR* str, int len);
	void WriteStr(PineString* str);
	Buffer ReadBytes(int len);
	
	INT16 ReadInt16();
	INT32 ReadInt32();
	INT64 ReadInt64();
	INT16 ReadInt16Revert();
	INT32 ReadInt32Revert();
	INT64 ReadInt64Revert();

	Buffer ReadBBin();
	Buffer ReadSBin();
	Buffer ReadIBin();
	PineString ReadStr(int len);
	PineString ReadBStr();
	PineString ReadSStr();
	PineString ReadIStr();

	void WriteInt16(INT16 value);
	void WriteInt32(INT32 value);
	void WriteInt64(INT64 value);
	void WriteInt16Revert(INT16 value);
	void WriteInt32Revert(INT32 value);
	void WriteInt64Revert(INT64 value);
	
	void WriteBBin(const BYTE* bin, int len);
	void WriteBBin(Buffer* bin);
	void WriteSBin(const BYTE* bin, int len);
	void WriteSBin(Buffer* bin);
	void WriteIBin(const BYTE* bin, int len);
	void WriteIBin(Buffer* bin);
	
	void WriteBStr(PineString* str);
	void WriteSStr(PineString* str);
	void WriteIStr(PineString* str);

	//void WriteBString(Buffer* str);
	//void WriteSString(Buffer* str);
	/*
	char* getBinary();
	int getSize();
	const char* getCurrentDataPointer();
	void pass(int len);

	INT8 readInt8();
	INT16 readInt16();
	INT32 readInt32();
	float readFloat();
	Buffer* readBin(int len);
	Buffer* readBBin();
	Buffer* readSBin();
	Buffer* readBString();
	Buffer* readSString();

	void writeInt8(INT8 value);
	void writeInt16(INT16 value);
	void writeInt32(INT32 value);
	void writeFloat(float value);
	void writeBin(Bytes* bin);
	void writeBBin(Bytes* bin);
	void writeSBin(Bytes* bin);
	void writeBString(Bytes* str);
	void writeSString(Bytes* str);*/
};
#endif//_H_CBYTEBUFFER