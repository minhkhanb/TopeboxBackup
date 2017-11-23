#include "pch.h"
#include "PCLSentRequestDto.h"
using namespace Pine;

void PCLSentRequestDto::setFromStream(StreamBuffer^ buf)
{
	_countData = buf->readInt16();
	InitData(_countData);
	for (int i = 0; i < _countData; i++)
	{
		ID[i] = buf->readInt64();
		GameId[i] = buf->readUInt64();
		Sender[i] = buf->readUInt64();
		Receiver[i] = buf->readUInt64();
		RequestType[i] = buf->readInt32();
	}
}

void PCLSentRequestDto::InitData(int countData)
{
	_countData = countData;

	ID = ref new Platform::Array<int64>(_countData);
	GameId = ref new Platform::Array<uint64>(_countData);
	Sender = ref new Platform::Array<uint64>(_countData);
	Receiver = ref new Platform::Array<uint64>(_countData);
	RequestType = ref new Platform::Array<int32>(_countData);
}

int PCLSentRequestDto::getStreamSize()
{
	//countData
	int size = 2;

	for (int i = 0; i < _countData; i++)
	{
		//ID
		size += 8;

		//GameId
		size += 8;

		//Sender
		size += 8;

		//Receiver
		size += 8;

		//RequestType
		size += 4;
	}

	return size;
}

StreamBuffer^ PCLSentRequestDto::CreateStream()
{
	StreamBuffer^ buf = ref new StreamBuffer(getStreamSize());

	buf->writeInt16(_countData);

	for (int i = 0; i < _countData; i++)
	{
		buf->writeInt64(ID[i]);
		buf->writeUInt64(GameId[i]);
		buf->writeUInt64(Sender[i]);
		buf->writeUInt64(Receiver[i]);
		buf->writeUInt32(RequestType[i]);
	}

	return buf;
}