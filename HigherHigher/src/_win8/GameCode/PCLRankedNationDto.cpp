#include "pch.h"
#include "PCLRankedNationDto.h"
using namespace Pine;

void PCLRankedNationDto::setFromStream(StreamBuffer^ buf)
{
	_countData = buf->readInt16();
	InitData(_countData);
	for (int i = 0; i < _countData; i++)
	{
		ID[i] = buf->readSStr();
		ChampionScore[i] = buf->readUInt64();
		MembersCount[i] = buf->readInt32();
	}
}

void PCLRankedNationDto::InitData(int countData)
{
	_countData = countData;

	ID = ref new Platform::Array<Platform::String^>(_countData);
	ChampionScore = ref new Platform::Array<uint64>(_countData);
	MembersCount = ref new Platform::Array<int32>(_countData);
}

int PCLRankedNationDto::getStreamSize()
{
	//countData
	int size = 2;

	for (int i = 0; i < _countData; i++)
	{
		//ID
		size += 2;
		if(ID[i] != nullptr)
		{
			size += 2*ID[i]->Length();
		}

		//ChampionScore
		size += 8;

		//MembersCount
		size += 4;
	}

	return size;
}

StreamBuffer^ PCLRankedNationDto::CreateStream()
{
	StreamBuffer^ buf = ref new StreamBuffer(getStreamSize());

	buf->writeInt16(_countData);
	
	for (int i = 0; i < _countData; i++)
	{
		buf->writeSStr(ID[i]);
		buf->writeUInt64(ChampionScore[i]);
		buf->writeInt32(MembersCount[i]);
	}

	return buf;
}