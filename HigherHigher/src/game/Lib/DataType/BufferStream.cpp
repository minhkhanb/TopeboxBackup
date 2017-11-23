/*
///written by Duy Nguyen
*/
#include "pch.h"
#include "BufferStream.h"
#include <stdio.h>
BufferStream::BufferStream(){
}
BufferStream::BufferStream(int len){
	Init(len);
}
BufferStream::BufferStream(const BYTE* data, int data_len){
	Init(data, data_len);
}
BufferStream::BufferStream(const BufferStream* buffstream){
	if (buffstream != NULL){
		Buffer::Init((const BYTE*)buffstream->_data, buffstream->_data_len);
	}
	_curr_pos = 0;
}
bool BufferStream::EOS(){
	return _curr_pos >= _data_len;
}
void BufferStream::Seek(int pos, int begin_at){
	if (begin_at == SEEK_SET){
		_curr_pos = pos;
	}
	else if (begin_at == SEEK_END){
		_curr_pos = _data_len - pos;
	}
	else{
		_curr_pos += pos;
	}
	if (_curr_pos < 0) _curr_pos = 0;
}
void BufferStream::Skip(int byte_num){
	_curr_pos += byte_num;
}
void BufferStream::Init(int len){
	Buffer::Init(len);
	_curr_pos = 0;
}
void BufferStream::Init(const BYTE* data, int data_len){
	Buffer::Init(data, data_len);
	_curr_pos = 0;
}

void BufferStream::Release(){
	Buffer::Release();
	_curr_pos = 0;
}
BufferStream::~BufferStream(){
	Release();
}
void BufferStream::WriteByte(BYTE data){
	if (_curr_pos >= _data_len)return;
	_data[_curr_pos++] = data;
}
BYTE BufferStream::ReadByte(){
	if (_curr_pos >= _data_len)return '\0';
	return _data[_curr_pos++];
}

void BufferStream::WriteBytes(const BYTE* data, int len){
	if (_curr_pos + len <= _data_len){
		for (int d = 0; d < len; d++){
			_data[_curr_pos + d] = data[d]; 
		}
		_curr_pos += len;
	}
}
void BufferStream::WriteBytes(Buffer* buff){
	WriteBytes(buff->Data(), buff->Length());
}
void BufferStream::WriteStr(const UNICHAR* str, int len){
	for (int d = 0; d < len; d++){
		WriteInt16(str[d]);
	}
}
void BufferStream::WriteStr(PineString* str){
	WriteStr(str->Data(), str->Length());
}
Buffer BufferStream::ReadBytes(int len){
	Buffer buff;
	if (_curr_pos + len <= _data_len){
		buff.Init(len);
		//PDEBUG("read bytes:len :%d", len);
		BYTE* buff_data = buff.Data();
		for (int i = 0; i < len; i++){
			buff_data[i] = ReadByte();
		}
	}else{
		//PDEBUG("read bytes error: _curr_pos:%d request len:%d data_len:%d", _curr_pos, len, _data_len);
	}
	return buff;
}

INT16 BufferStream::ReadInt16(){
	return ((ReadByte() & 0xFF) | ((ReadByte() << 8) & 0xFF00));
}
INT32 BufferStream::ReadInt32(){
	int b1 = ReadByte() & 0xFF;
	int b2 = (ReadByte() << 8) & 0xFFFF;
	int b3 = (ReadByte() << 16) & 0xFFFFFF;
	int b4 = (ReadByte() << 24) & 0xFFFFFFFF;
	return (b1 | b2 | b3 | b4);
}
INT64 BufferStream::ReadInt64(){
	BYTE buff[8];
	buff[0] = ReadByte();
	buff[1] = ReadByte();
	buff[2] = ReadByte();
	buff[3] = ReadByte();
	buff[4] = ReadByte();
	buff[5] = ReadByte();
	buff[6] = ReadByte();
	buff[7] = ReadByte();
	
	return *(INT64*)buff;
}
INT16 BufferStream::ReadInt16Revert(){
	return (((ReadByte()<<8) & 0xFF00) | ((ReadByte()) & 0xFF));
}
INT32 BufferStream::ReadInt32Revert(){
	int b4 = ReadByte() & 0xFF;
	int b3 = (ReadByte() << 8) & 0xFFFF;
	int b2 = (ReadByte() << 16) & 0xFFFFFF;
	int b1 = (ReadByte() << 24) & 0xFFFFFFFF;
	return (b1 | b2 | b3 | b4);
}
INT64 BufferStream::ReadInt64Revert(){
	BYTE buff[8];
	buff[7] = ReadByte();
	buff[6] = ReadByte();
	buff[5] = ReadByte();
	buff[4] = ReadByte();
	buff[3] = ReadByte();
	buff[2] = ReadByte();
	buff[1] = ReadByte();
	buff[0] = ReadByte();

	return *(INT64*)buff;
}
Buffer BufferStream::ReadBBin(){
	int len = ReadByte();
	return ReadBytes(len);
}
Buffer BufferStream::ReadSBin(){
	int len = ReadInt16();
	return ReadBytes(len);
}
Buffer BufferStream::ReadIBin(){
	int len = ReadInt32();
	//PDEBUG("read IBin len:%d", len);
	return ReadBytes(len);
}

PineString BufferStream::ReadStr(int len){
	PineString str;
	if (_curr_pos + (len * 2) <= _data_len){
		str.Init(len);
		//PDEBUG("read bytes:len :%d", len);
		UNICHAR* buff_data = str.Data();
		for (int i = 0; i < len; i++){
			buff_data[i] = ReadInt16();
		}
	}else{
		//PDEBUG("read str error: _curr_pos:%d request len:%d data_len:%d", _curr_pos, len, _data_len);
	}
	return str;
}

PineString BufferStream::ReadBStr(){
	int len = ReadByte();
	return ReadStr(len);
}
PineString BufferStream::ReadSStr(){
	int len = ReadInt16();
	return ReadStr(len);
}
PineString BufferStream::ReadIStr(){
	int len = ReadInt32();
	return ReadStr(len);
}
	
void BufferStream::WriteInt16(INT16 value){
	WriteByte(value & 0xFF);
	WriteByte((value >> 8) & 0xFF);
}
void BufferStream::WriteInt32(INT32 value){
	WriteByte(value & 0xFF);
	WriteByte((value >> 8) & 0xFF);
	WriteByte((value >> 16) & 0xFF);
	WriteByte((value >> 24) & 0xFF);
}
void BufferStream::WriteInt64(INT64 value){
	WriteByte(value & 0xFF);
	WriteByte((value >> 8) & 0xFF);
	WriteByte((value >> 16) & 0xFF);
	WriteByte((value >> 24) & 0xFF);
	WriteByte((value >> 32) & 0xFF);
	WriteByte((value >> 40) & 0xFF);
	WriteByte((value >> 48) & 0xFF);
	WriteByte((value >> 56) & 0xFF);
}
void BufferStream::WriteInt16Revert(INT16 value){
	WriteByte((value >> 8) & 0xFF);
	WriteByte(value & 0xFF);
}
void BufferStream::WriteInt32Revert(INT32 value){
	WriteByte((value >> 24) & 0xFF);
	WriteByte((value >> 16) & 0xFF);
	WriteByte((value >> 8) & 0xFF);
	WriteByte(value & 0xFF);
}
void BufferStream::WriteInt64Revert(INT64 value){
	WriteByte((value >> 56) & 0xFF);
	WriteByte((value >> 48) & 0xFF);
	WriteByte((value >> 40) & 0xFF);
	WriteByte((value >> 32) & 0xFF);
	WriteByte((value >> 24) & 0xFF);
	WriteByte((value >> 16) & 0xFF);
	WriteByte((value >> 8) & 0xFF);
	WriteByte(value & 0xFF);
}
void BufferStream::WriteBBin(const BYTE* bin, int len){
	if (len > 0 && len < 255){
		WriteByte(len);
		for (int i = 0; i < len; i++){
			WriteByte(bin[i]);
		}
	}
}
void BufferStream::WriteBBin(Buffer* bin){
	if (bin != NULL){
		WriteBBin(bin->Data(), bin->Length());
	}
}
void BufferStream::WriteSBin(const BYTE* bin, int len){
	WriteInt16(len);
	WriteBytes(bin, len);
}
void BufferStream::WriteSBin(Buffer* bin){
	if (bin != NULL){
		WriteInt16(bin->Length());
		WriteBytes(bin);
	}
}
void BufferStream::WriteIBin(const BYTE* bin, int len){
	if(bin != NULL){
		WriteInt32(len);
		WriteBytes(bin, len);
	}
}
void BufferStream::WriteIBin(Buffer* bin){
	if (bin != NULL){
		WriteInt32(bin->Length());
		WriteBytes(bin);
	}
}

void BufferStream::WriteBStr(PineString* str){
	if(str != NULL){
		WriteByte(str->Length());
		WriteStr(str);
	}
}
void BufferStream::WriteSStr(PineString* str){
	if(str != NULL){
		WriteInt16(str->Length());
		WriteStr(str);
	}
}
void BufferStream::WriteIStr(PineString* str){
	if(str != NULL){
		WriteInt32(str->Length());
		WriteStr(str);
	}
}
/*
CByteBuffer* CByteBuffer::allocate(int size){
	CByteBuffer* obj = new CByteBuffer();
	obj->_data = new char[size];
	obj->_data_len = size;
	obj->_curr_pos = 0;
	return obj;
}
const char* CByteBuffer::getCurrentDataPointer(){
	return _data + _curr_pos;
}
void CByteBuffer::pass(int len){
	_curr_pos += len;
}
CByteBuffer* CByteBuffer::swap(const char* bytes, int len){
	CByteBuffer* obj = allocate(len);
	for(int i = 0; i< len; i++){
		obj->_data[i] = bytes[i];
	}
	obj->_curr_pos = 0;
	return obj;
}
void CByteBuffer::writeByte(char data){
	if(_curr_pos < _data_len){
		_data[_curr_pos] = data;
		_curr_pos++;
	}else{
		//throw error;
	}
}
char CByteBuffer::readByte(){
	if(_curr_pos < _data_len){
		char data = _data[_curr_pos];
		_curr_pos++;
		return data;
	}else{
		//throw error;
	}
}
void CByteBuffer::writeBytes(char* data, int len){
	if(_curr_pos + len <= _data_len){
		for(int i = 0; i < len; i++){
			_data[_curr_pos] = data[i];
			_curr_pos++;
		}
	}else{
		//throw error;
		printf("write out of len:%d %d %d\n",_curr_pos,len,_data_len);
	}
}
char* CByteBuffer::readBytes(int len){
	if(_curr_pos + len > _data_len)len = _data_len - _curr_pos;
	char* data = new char[len];
	for(int i = 0; i < len; i++){
		data[i] = _data[_curr_pos];
		_curr_pos++;
	}
	return data;
}
char* CByteBuffer::getBinary(){
	char* data = new char[_data_len];
	memcpy(_data,data,_data_len);
	return data;
}
int CByteBuffer::getSize(){
	return _data_len;
}
////
///
///
INT8 CByteBuffer::readInt8(CByteBuffer* buf){
	return buf->readByte()&0xFF;
}
INT16 CByteBuffer::readInt16(CByteBuffer* buf){
	return ( (buf->readByte()&0xFF) | ((buf->readByte()<<8)&0xFF00) );
}
INT32 CByteBuffer::readInt32(CByteBuffer* buf){
	int b1 = buf->readByte()&0xFF;
	int b2 = (buf->readByte()<<8)&0xFFFF;
	int b3 = (buf->readByte()<<16)&0xFFFFFF;
	int b4 = (buf->readByte()<<24)&0xFFFFFFFF;
	return (b1|b2|b3|b4);
}
float CByteBuffer::readFloat(CByteBuffer* buf){
	return 0.0f;
}
Bytes* CByteBuffer::readBin(CByteBuffer* buf, int len){
	Bytes* bytes = new Bytes();
	bytes->Data = buf->readBytes(len);
	bytes->Lenght = len;
	return bytes;
}
Bytes* CByteBuffer::readBBin(CByteBuffer* buf){
	int len = buf->readByte();
	Bytes* bytes = new Bytes();
	bytes->Lenght = len;
	bytes->Data = buf->readBytes(len);
	return bytes;
}
Bytes* CByteBuffer::readSBin(CByteBuffer* buf){
	int len = CByteBuffer::readInt16(buf);
	Bytes* bytes = new Bytes();
	bytes->Lenght = len;
	bytes->Data = buf->readBytes(len);
	return bytes;
}
Bytes* CByteBuffer::readBString(CByteBuffer* buf){
	int len = buf->readByte();
	Bytes* bytes = new Bytes();
	bytes->Lenght = len;
	bytes->Data = new char[len+1];
	char* data = buf->readBytes(len);
	memcpy(data,bytes->Data,len);
	bytes->Data[len] = '\0';
	if(data != NULL)delete[] data;
	return bytes;
}
Bytes* CByteBuffer::readSString(CByteBuffer* buf){
	int len = CByteBuffer::readInt16(buf);
	Bytes* bytes = new Bytes();
	bytes->Lenght = len;
	bytes->Data = new char[len+1];
	char* data = buf->readBytes(len);
	memcpy(data,bytes->Data,len);
	bytes->Data[len] = '\0';
	if(data != NULL)delete[] data;
	return bytes;
}

void CByteBuffer::writeInt8(INT8 val, CByteBuffer* buf){
	buf->writeByte(val);
}
void CByteBuffer::writeInt16(INT16 val, CByteBuffer* buf){
	buf->writeByte(val&0xFF);
	buf->writeByte((val>>8)&0xFF);
}
void CByteBuffer::writeInt32(INT32 val, CByteBuffer* buf){
	buf->writeByte(val&0xFF);
	buf->writeByte((val>>8)&0xFF);
	buf->writeByte((val>>16)&0xFF);
	buf->writeByte((val>>24)&0xFF);
}
void CByteBuffer::writeFloat(float val, CByteBuffer* buf){
}
void CByteBuffer::writeBin(Bytes* bin, CByteBuffer* buf){
	buf->writeBytes(bin->Data,bin->Lenght);
}
void CByteBuffer::writeBBin(Bytes* bin, CByteBuffer* buf){
	buf->writeByte(bin->Lenght);
	buf->writeBytes(bin->Data,bin->Lenght);
}
void CByteBuffer::writeSBin(Bytes* bin, CByteBuffer* buf){
	CByteBuffer::writeInt16(bin->Lenght,buf);
	buf->writeBytes(bin->Data,bin->Lenght);
}
void CByteBuffer::writeBString(Bytes* str,CByteBuffer* buf){
}
void CByteBuffer::writeSString(Bytes* str,CByteBuffer* buf){
}
*/