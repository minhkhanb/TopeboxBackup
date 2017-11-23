#ifndef _DATA_INPUT_STREAM_H_
#define _DATA_INPUT_STREAM_H_

#include "stdio.h"
#include "../DataType/CType.h"
//#ifdef MAC_OS
//#include "../Define/Defines.h"
//#endif

class DataInputStream
{
private:
	void GetFileDataToBuffer();
	int _buffer_data_readed;
public:
    static BYTE *_buffer;
	static UINT32 _buffer_length;
	char _file_path[255];

	DataInputStream();
	~DataInputStream();
	
	bool Open(const char* filename, INT32 offset,int count);
	bool Open(BYTE* buffer,int length);
	void Close();
	
	unsigned char  ReadInt8();
	short ReadInt16();
	INT32 ReadInt24();
	INT32 ReadInt32();
	INT64 ReadInt64();

	static INT32 ReadInt8(BYTE* buffer,int offset);
	static INT32 ReadInt16(BYTE* buffer,int offset);
	static INT32 ReadInt24(BYTE* buffer,int offset);
	static INT32 ReadInt32(BYTE* buffer,int offset);

	static INT32 ReadUInt8(BYTE* buffer,int offset){return (ReadInt8(buffer,offset) & 0xFF);};
	static INT32 ReadUInt16(BYTE* buffer,int offset){return (ReadInt16(buffer,offset) & 0xFFFF);};
	static INT32 ReadUInt24(BYTE* buffer,int offset){return (ReadInt24(buffer,offset) & 0xFFFFFF);};
	static INT32 ReadUInt32(BYTE* buffer,int offset){return (ReadInt32(buffer,offset) & 0xFFFFFFFF);};

	BYTE  Read();
	float ReadFloat32();
	float RevertFloat32();
	static INT32 RevertInt32(INT32 value);
	static UINT32 RevertUInt32(UINT32 value);
	
	unsigned char  ReadUInt8()	{ return (ReadInt8() & 0xFF); }
	INT32 ReadUInt16()	{return (ReadInt16() & 0xFFFF);}
	UINT32 ReadUInt32()	{return (ReadInt32() & 0xFFFFFFFF);}
	void ReadData(BYTE* output, int len);
	void Reset();
	int GetPos();
	void Seek(int, int);

	static int _opening_count;
private:
	
	
public:
	
	FILE*	_file_pointer;
	INT32	_file_offset;
	//BYTE*	_buffer;
	int		_offset;
	int		Length;
	
};

#endif