#include "pch.h"
#include "PCLServerInfoDto.h"
using namespace Pine;

void PCLServerInfoDto::setFromStream(StreamBuffer^ buf)
{
	_countData = buf->readInt16();
	InitData(_countData);
	for (int i = 0; i < _countData; i++)
	{
		ID[i] = buf->readUInt64();
		PstNow[i].UniversalTime = buf->readUInt64();
		PstYear[i] = buf->readInt32();
		PstMonth[i] = buf->readInt32();
		PstDay[i] = buf->readInt32();
		PstHour[i] = buf->readInt32();
		PstMinute[i] = buf->readInt32();
		PstSecond[i] = buf->readInt32();
		ResetRemaining[i] = buf->readInt64();
	}
}

void PCLServerInfoDto::InitData(int countData)
{
	_countData = countData;

	ID = ref new Platform::Array<uint64>(_countData);
	PstNow = ref new Platform::Array<Windows::Foundation::DateTime>(_countData);
	PstYear = ref new Platform::Array<int32>(_countData);
	PstMonth = ref new Platform::Array<int32>(_countData);
	PstDay = ref new Platform::Array<int32>(_countData);
	PstHour = ref new Platform::Array<int32>(_countData);
	PstMinute = ref new Platform::Array<int32>(_countData);
	PstSecond = ref new Platform::Array<int32>(_countData);
	ResetRemaining = ref new Platform::Array<int64>(_countData);
}

int PCLServerInfoDto::getStreamSize()
{
	//countData
	int size = 2;

	for (int i = 0; i < _countData; i++)
	{
		//ID
		size += 8;

		//PstNow
		size += 8;

		//PstYear
		size += 4;

		//PSTMonth
		size += 4;

		//PSTDay
		size += 4;

		//PSTHour
		size += 4;

		//PSTMinute
		size += 4;

		//PSTSecond
		size += 4;

		//ResetRemaining
		size += 8;
	}

	return size;
}

StreamBuffer^ PCLServerInfoDto::CreateStream()
{
	StreamBuffer^ buf = ref new StreamBuffer(getStreamSize());

	buf->writeInt16(_countData);
	
	for (int i = 0; i < _countData; i++)
	{
		buf->writeUInt64(ID[i]);
		buf->writeUInt64(PstNow[i].UniversalTime);
		buf->writeInt32(PstYear[i]);
		buf->writeInt32(PstMonth[i]);
		buf->writeInt32(PstDay[i]);
		buf->writeInt32(PstHour[i]);
		buf->writeInt32(PstMinute[i]);
		buf->writeInt32(PstSecond[i]);
		buf->writeInt64(ResetRemaining[i]);
	}

	return buf;
}