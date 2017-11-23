#include "pch.h"
#include "PCLRankedUserDto.h"
using namespace Pine;

void PCLRankedUserDto::setFromStream(StreamBuffer^ buf)
{
	_countData = buf->readInt16();
	InitData(_countData);
	for (int i = 0; i < _countData; i++)
	{
		ID[i] = buf->readUInt64();
		UserName[i] = buf->readSStr();
		AvatarUrl[i] = buf->readSStr();
		ChooseCharater[i] = buf->readInt32();
		NationCode[i] = buf->readSStr();
		ChampionScore[i] = buf->readInt32();
	}
}

void PCLRankedUserDto::InitData(int countData)
{
	_countData = countData;

	ID = ref new Platform::Array<uint64>(_countData);
	UserName = ref new Platform::Array<Platform::String^>(_countData);
	AvatarUrl = ref new Platform::Array<Platform::String^>(_countData);
	ChooseCharater = ref new Platform::Array<int32>(_countData);
	NationCode = ref new Platform::Array<Platform::String^>(_countData);
	ChampionScore = ref new Platform::Array<int32>(_countData);
}

int PCLRankedUserDto::getStreamSize()
{
	//countData
	int size = 2;

	for (int i = 0; i < _countData; i++)
	{
		//ID
		size += 8;

		//UserName
		size += 2;
		if(UserName[i] != nullptr)
		{
			size += 2*UserName[i]->Length();
		}

		//AvatarUrl
		size += 2;
		if(AvatarUrl[i] != nullptr)
		{
			size += 2*AvatarUrl[i]->Length();
		}

		//ChooseCharater
		size += 4;

		//NationCode
		size += 2;
		if(NationCode[i] != nullptr)
		{
			size += 2*NationCode[i]->Length();
		}

		//ChampionScore
		size += 4;
	}

	return size;
}

StreamBuffer^ PCLRankedUserDto::CreateStream()
{
	StreamBuffer^ buf = ref new StreamBuffer(getStreamSize());	
	buf->writeInt16(_countData);
	for (int i = 0; i < _countData; i++)
	{
		buf->writeUInt64(ID[i]);
		buf->writeSStr(UserName[i]);
		buf->writeSStr(AvatarUrl[i]);
		buf->writeInt32(ChooseCharater[i]);
		buf->writeSStr(NationCode[i]);
		buf->writeInt32(ChampionScore[i]);
	}

	return buf;
}