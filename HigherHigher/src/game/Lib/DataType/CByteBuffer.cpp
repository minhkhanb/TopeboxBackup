/*
///written by Duy Nguyen
*/
#include "pch.h"
#include "CByteBuffer.h"
#include <stdio.h>
CByteBuffer::CByteBuffer(){
	_data = NULL;
}
CByteBuffer::~CByteBuffer(){
	if(_data != NULL){
		delete[] _data;
		_data = NULL;
	}
}
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