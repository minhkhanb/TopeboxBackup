#include "pch.h"
#include <stdio.h>
#include <string>
#include <locale>
#include "StreamBuffer.h"
#include "Utils.h"
using namespace Pine;
StreamBuffer::StreamBuffer(int size){
	Data = ref new Platform::Array<uint8>(size);
	CurPos = 0;
}

StreamBuffer::StreamBuffer(const Platform::Array<uint8>^ data){
	Data = data;
	CurPos = 0;
}
uint8 StreamBuffer::readByte(){
	if(CurPos < Data->Length){
		uint8 value = Data[CurPos];
		CurPos++;
		return value;
	}
	return 0;
}
int16 StreamBuffer::readInt16(){
	if(CurPos <= Data->Length - 2){
		int16 value;
		char data[2];
		data[1] = (Data[CurPos]);
		data[0] = (Data[CurPos + 1]);
		memcpy(&value, data,2);
		CurPos += 2;
		return value;
	}
	return 0;
}
uint16 StreamBuffer::readUInt16(){
	if(CurPos <= Data->Length - 2){
		uint16 value;
		char data[2];
		data[1] = (Data[CurPos]);
		data[0] = (Data[CurPos + 1]);
		memcpy(&value, data,2);
		CurPos += 2;
		return value;
	}
	return 0;
}
int32 StreamBuffer::readInt32(){
	if(CurPos <= Data->Length - 4){
		int32 value;
		char data[4];
		data[3] = (Data[CurPos]);
		data[2] = (Data[CurPos + 1]);
		data[1] = (Data[CurPos + 2]);
		data[0] = (Data[CurPos + 3]);
		memcpy(&value, data,4);
		CurPos += 4;
		return value;
	}
	return 0;
}
uint32 StreamBuffer::readUInt32(){
	if(CurPos <= Data->Length - 4){
		uint32 value;
		char data[4];
		data[3] = (Data[CurPos]);
		data[2] = (Data[CurPos + 1]);
		data[1] = (Data[CurPos + 2]);
		data[0] = (Data[CurPos + 3]);
		memcpy(&value, data,4);
		CurPos += 4;
		return value;
	}
	return 0;
}
int64 StreamBuffer::readInt64(){
	if(CurPos <= Data->Length - 8){
		uint64 value;
		char data[8];
		data[7] = (Data[CurPos]);
		data[6] = (Data[CurPos + 1]);
		data[5] = (Data[CurPos + 2]);
		data[4] = (Data[CurPos + 3]);
		data[3] = (Data[CurPos + 4]);
		data[2] = (Data[CurPos + 5]);
		data[1] = (Data[CurPos + 6]);
		data[0] = (Data[CurPos + 7]);
		memcpy(&value, data,8);
		CurPos += 8;
		return value;
	}
	return 0;
}
uint64 StreamBuffer::readUInt64(){
	if(CurPos <= Data->Length - 8){
		uint64 value;
		char data[8];
		data[7] = (Data[CurPos]);
		data[6] = (Data[CurPos + 1]);
		data[5] = (Data[CurPos + 2]);
		data[4] = (Data[CurPos + 3]);
		data[3] = (Data[CurPos + 4]);
		data[2] = (Data[CurPos + 5]);
		data[1] = (Data[CurPos + 6]);
		data[0] = (Data[CurPos + 7]);
		memcpy(&value, data,8);
		CurPos += 8;
		return value;
	}
	return 0;
}
Platform::Array<uint8>^ StreamBuffer::readSBin(){
	if(CurPos < Data->Length - 2){
		uint16 len = readUInt16();
		if( len > 0 && CurPos <= Data->Length - len){
			Platform::Array<uint8>^ data = ref new Platform::Array<uint8>(len);
			for(int i = 0; i< len; i++){
				data[i] = readByte();
			}
			return data;
		}
	}
	return nullptr;
}
Platform::Array<uint8>^ StreamBuffer::readIBin(){
	if(CurPos < Data->Length - 4){
		uint32 len = readUInt32();
		if( len > 0 && CurPos <= Data->Length - len){
			Platform::Array<uint8>^ data = ref new Platform::Array<uint8>(len);
			for(int i = 0; i< len; i++){
				data[i] = readByte();
			}
			return data;
		}
	}
	return nullptr;
}
Platform::String^ StreamBuffer::readSStr(){
	if(CurPos < Data->Length - 2){
		uint16 len = readUInt16();
		if( len > 0 && CurPos <= Data->Length - len){
			Platform::String^ string = ref new Platform::String();
			for(int i = 0; i< len; i++){
				uint16 ch= readUInt16();
				string += (wchar_t)ch;
			}
			return string;
		}
	}
	return nullptr;
}
Platform::String^ StreamBuffer::readIStr(){
	if(CurPos < Data->Length - 4){
		uint32 len = readUInt32();
		if( len > 0 && CurPos <= Data->Length - len){
			Platform::String^ string = ref new Platform::String();
			for(int i = 0; i< len; i++){
				uint16 ch= readUInt16();
				string += (wchar_t)ch;
			}
			return string;
		}
	}
	return nullptr;
}

void StreamBuffer::writeByte(uint8 value){
	if(CurPos < Data->Length){
		Data[CurPos] = (uint8)value;
		CurPos++;
	}
}
void StreamBuffer::writeInt16(int16 value){
	writeByte((byte)(value>>8));
	writeByte((byte)value);
}
void StreamBuffer::writeUInt16(uint16 value){
	writeByte((byte)(value>>8));
	writeByte((byte)value);
}
void StreamBuffer::writeInt32(int32 value){
	writeByte((byte)((value>>24)&0xFF));
	writeByte((byte)((value>>16)&0xFF));
	writeByte((byte)((value>>8)&0xFF));
	writeByte((byte)(value)&0xFF);
}
void StreamBuffer::writeUInt32(uint32 value){
	writeByte((byte)((value>>24)&0xFF));
	writeByte((byte)((value>>16)&0xFF));
	writeByte((byte)((value>>8)&0xFF));
	writeByte((byte)(value)&0xFF);
}
void StreamBuffer::writeInt64(int64 value){
	writeByte((byte)(value>>56));
	writeByte((byte)(value>>48));
	writeByte((byte)(value>>40));
	writeByte((byte)(value>>32));
	writeByte((byte)(value>>24));
	writeByte((byte)(value>>16));
	writeByte((byte)(value>>8));
	writeByte((byte)value);
}
void StreamBuffer::writeUInt64(uint64 value){		
	writeByte((byte)((value>>56)&0xFF));
	writeByte((byte)((value>>48)&0xFF));
	writeByte((byte)((value>>40)&0xFF));
	writeByte((byte)((value>>32)&0xFF));
	writeByte((byte)((value>>24)&0xFF));
	writeByte((byte)((value>>16)&0xFF));
	writeByte((byte)((value>>8)&0xFF));
	writeByte((byte)(value&0xFF));
}
void StreamBuffer::writeSBin(const Platform::Array<uint8>^ data){
	int len = data->Length;
	writeUInt16(len);
	for(int i = 0; i< len; i++){
		writeByte(data[i]);
	}
}
void StreamBuffer::writeIBin(const Platform::Array<uint8>^ data){
	if(data != nullptr){
		int len = data->Length;
		writeUInt32(len);
		for(int i = 0; i< len; i++){
			writeByte(data[i]);
		}
	}else{
		writeUInt32(0);
	}
}
void StreamBuffer::writeSStr(Platform::String^ str){
	int len = str->Length();
	writeUInt16(len);
	for(int i = 0; i< len; i++){
		writeUInt16((str->Data())[i]);
	}
}
void StreamBuffer::writeIStr(Platform::String^ str){
	int len = str->Length();
	writeUInt32(len);
	for(int i = 0; i< len; i++){
		writeUInt16((str->Data())[i]);
	}
}
