#include "pch.h"
#include "../DataType/CType.h"
#include "../../Define/Defines.h"
#include "DataInputStream.h"
#include <string.h>
#include "../Debug/Test.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


//FILE* DataInputStream::_file_pointer = NULL;
BYTE *DataInputStream::_buffer = new BYTE[IO_BUFFER_LENGTH];
UINT32 DataInputStream::_buffer_length = IO_BUFFER_LENGTH;

int DataInputStream::_opening_count = 0;
DataInputStream::DataInputStream()
{
	_file_pointer = NULL;
	_file_offset = 0;
	//_buffer = NULL;
	Length = 0;
	_buffer_data_readed = 0;
	_file_path[0] = '\0';
}

DataInputStream::~DataInputStream()
{
	Close();
	//delete[] _buffer;
}
bool DataInputStream::Open(BYTE* buffer,int length)
{
    if(length > _buffer_length)
    {
        SAFE_DEL(_buffer);
        _buffer_length = length;
        _buffer = new BYTE[_buffer_length];
    }
	Close();

	_offset = 0;
	_file_pointer = NULL;
	memcpy(_buffer,buffer,length);
	return true;
}
int DataInputStream::GetPos(){
	return _file_offset - _buffer_data_readed + _offset;
}
void DataInputStream::Seek(int target, int pos){
	if (target == SEEK_SET){
		_file_offset = pos;
	}
	GetFileDataToBuffer();
}
bool DataInputStream::Open(const char* filename, INT32 offset,int count)
{
	Close();

	sprintf(_file_path, "%s", filename);

	_file_pointer = fopen(filename, "rb");
	//ASSERT(_file_pointer);
	PDEBUG("open file %s \n stream openning %d\n", filename, _opening_count);
	if(_file_pointer == NULL){
		perror(filename);
		//PDEBUG("open fail %s \n stream openning %d\n", filename, _opening_count);
		PDEBUG("Failed to open %s\n"
			"The reason *may* have been %s\n",
			filename, strerror(errno));
		
		return false;
	}
	_opening_count++;
	_file_offset = offset;
	GetFileDataToBuffer();   
	return true;
}
void DataInputStream::Reset()
{
	_offset = 0;
}
void DataInputStream::Close()
{
	if(_file_pointer != NULL)
	{
		fclose(_file_pointer);
		_file_pointer = NULL;
		_opening_count--;
	}
	//delete[] _buffer;
}
BYTE  DataInputStream::Read()
{
	if (_offset >= IO_BUFFER_LENGTH)
	{
		GetFileDataToBuffer();
	}
	return _buffer[_offset++]&0xFF;
}
INT32 DataInputStream::ReadInt8(BYTE* buffer,int offset)
{
	return buffer[offset]&0xFF;
}
INT32 DataInputStream::ReadInt16(BYTE* buffer,int offset)
{
	return (ReadInt8(buffer,offset)<<8) | (ReadInt8(buffer,offset+1));
}
INT32 DataInputStream::ReadInt24(BYTE* buffer,int offset)
{
	return (ReadInt8(buffer,offset+2) | (ReadInt8(buffer,offset+1)<<8) | (ReadInt8(buffer,offset)<<16));
}

INT32 DataInputStream::ReadInt32(BYTE* buffer,int offset)
{
	return (ReadInt8(buffer,offset+3)) | (ReadInt8(buffer,offset+2)<<8) | (ReadInt8(buffer,offset+1)<<16) | (ReadInt8(buffer,offset)<<24);
}
unsigned char DataInputStream::ReadInt8()
{
	if (_offset >= IO_BUFFER_LENGTH)
	{
		GetFileDataToBuffer();
	}
	return _buffer[_offset++]&0xFF;
}

short  DataInputStream::ReadInt16()
{
	unsigned char byte1 = ReadInt8();
	unsigned char byte2 = ReadInt8();
	short value = (byte1 | (byte2 << 8));
	return value;
}

INT32 DataInputStream::ReadInt24()
{
	return (ReadInt8()) | (ReadInt8()<<8) | (ReadInt8()<<16);
}

INT32 DataInputStream::ReadInt32()
{
	
	unsigned char byte1 = ReadInt8();
	unsigned char byte2 = ReadInt8();
	unsigned char byte3 = ReadInt8();
	unsigned char byte4 = ReadInt8();
	int value = byte1 | (byte2 << 8) | (byte3 << 16) | (byte4<<24);
	
	return value;
	//return (ReadInt8()) | (ReadInt8()<<8) | (ReadInt8()<<16) | (ReadInt8()<<24);
}

INT64 DataInputStream::ReadInt64()
{
    char tmp[8];
    tmp[0] = ReadInt8();
    tmp[1] = ReadInt8();
    tmp[2] = ReadInt8();
    tmp[3] = ReadInt8();
    tmp[4] = ReadInt8();
    tmp[5] = ReadInt8();
    tmp[6] = ReadInt8();
    tmp[7] = ReadInt8();
    
    return *(INT64*)tmp;
}

float DataInputStream::ReadFloat32()
{
	float f;
	INT32 i = (ReadInt8()<< 24) | (ReadInt8()<<16) | (ReadInt8()<<8) | (ReadInt8());
	memcpy(&f, &i, sizeof(float));
	return f;
}
void DataInputStream::ReadData(BYTE* output, int len)
{
	for (int i =0; i< len; i++)
	{
		output[i] = ReadInt8();
	}
}

float DataInputStream::RevertFloat32()
{
	float f;
	INT32 i = ((ReadInt8()) | (ReadInt8() << 8) | (ReadInt8() << 16) | (ReadInt8() << 24));
	memcpy(&f, &i, sizeof(float));
	return f;
}

INT32 DataInputStream::RevertInt32(INT32 value){
	char tmp[4];
	tmp[3] = value&0xFF;
	tmp[2] = (value>>8)&0xFF;
	tmp[1] = (value>>16)&0xFF;
	tmp[0] = (value>>24)&0xFF;
	return *(INT32*)tmp;

}

UINT32 DataInputStream::RevertUInt32(UINT32 value){
	char tmp[4];
	tmp[3] = value&0xFF;
	tmp[2] = (value>>8)&0xFF;
	tmp[1] = (value>>16)&0xFF;
	tmp[0] = (value>>24)&0xFF;
	return *(UINT32*)tmp;
}
void DataInputStream::GetFileDataToBuffer()
{
	//PDEBUG("DataInputStream: getdata to buffer at :%d\n", _file_offset);
	if(_file_pointer != NULL)
	{
		fseek(_file_pointer, _file_offset, SEEK_SET);
		_buffer_data_readed = fread(_buffer, 1, IO_BUFFER_LENGTH, _file_pointer);
		_offset = 0;
		_file_offset += _buffer_data_readed;
		//PDEBUG("\tloaded:%d _offset:%d _file_offset:%d\n", _buffer_data_readed, _offset, _file_offset);
	}
	//PDEBUG("DataInputStream: GetFileDataToBuffer done");
}