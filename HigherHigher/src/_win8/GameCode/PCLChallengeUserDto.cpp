#include "pch.h"
#include "PCLChallengeUserDto.h"
using namespace Pine;

void PCLChallengeUserDto::setFromStream(StreamBuffer^ buf)
{
	_countData = buf->readInt16();
	InitData(_countData);
	for (int i = 0; i < _countData; i++)
	{
		ID[i] = buf->readUInt64();
		UserName[i] = buf->readSStr();
		FacebookId[i] = buf->readUInt64();
		AvatarUrl[i] = buf->readSStr();
		ChooseCharater[i] = buf->readInt32();
		NationCode[i] = buf->readSStr();
		ChampionScore[i] = buf->readInt32();
		RunHighScore[i] = buf->readUInt64();
		Rank[i] = buf->readInt32();
	}
}

void PCLChallengeUserDto::InitData(int countData)
{
	_countData = countData;

	ID = ref new Platform::Array<uint64>(_countData);
	UserName = ref new Platform::Array<Platform::String^>(_countData);
	FacebookId = ref new Platform::Array<uint64>(_countData);
	AvatarUrl = ref new Platform::Array<Platform::String^>(_countData);
	ChooseCharater = ref new Platform::Array<int32>(_countData);
	NationCode = ref new Platform::Array<Platform::String^>(_countData);
	ChampionScore = ref new Platform::Array<int32>(_countData);
	RunHighScore = ref new Platform::Array<uint64>(_countData);
	Rank = ref new Platform::Array<int32>(_countData);
}

int PCLChallengeUserDto::getStreamSize()
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

		//FacebookId
		size += 8;

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

		//RunHighScore
		size += 8;

		//Rank
		size += 4;
	}

	return size;
}

StreamBuffer^ PCLChallengeUserDto::CreateStream()
{
	StreamBuffer^ buf = ref new StreamBuffer(getStreamSize());

	buf->writeInt16(_countData);

	for (int i = 0; i < _countData; i++)
	{
		buf->writeUInt64(ID[i]);
		buf->writeSStr(UserName[i]);
		buf->writeUInt64(FacebookId[i]);
		buf->writeSStr(AvatarUrl[i]);
		buf->writeInt32(ChooseCharater[i]);
		buf->writeSStr(NationCode[i]);
		buf->writeInt32(ChampionScore[i]);
		buf->writeUInt64(RunHighScore[i]);
		buf->writeInt32(Rank[i]);
	}

	return buf;
}