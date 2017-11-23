#include "pch.h"
#include "PCLSaveGameDto.h"
using namespace Pine;

void PCLSaveGameDto::setFromStream(StreamBuffer^ buf)
{
	_countData = buf->readInt16();
	InitData(_countData);
	Data = buf->readIBin();
	for (int i = 0; i < _countData; i++)
	{
		ID[i] = buf->readUInt64();
		UserName[i] = buf->readSStr();
		FacebookId[i] = buf->readUInt64();
		AvatarUrl[i] = buf->readSStr();
		Token[i] = buf->readSStr();
		LevelMain[i] = buf->readInt16();
		LevelCurrentWorld[i] = buf->readInt16();
		ChooseCharater[i] = buf->readInt32();
		NationCode[i] = buf->readSStr();
		ChampionScore[i] = buf->readInt32();
		LastChampionScore[i] = buf->readInt32();
		RunHighScore[i] = buf->readUInt64();
		LastRunHighScore[i] = buf->readUInt64();
		Revision[i] = buf->readUInt64();
		/*Platform::Array<uint8>^ data = buf->readIBin();
		int offset = i*10000;
		for(int d = 0; d< 10000; d++){
			Data[offset + d] = data[d];
		}*/
		DataSchemaVersion[i] = buf->readUInt64();
		Rank[i] = buf->readInt32();
		LastRank[i] = buf->readInt32();
	}
}

void PCLSaveGameDto::InitData(int countData)
{
	_countData = countData;

	ID = ref new Platform::Array<uint64>(_countData);
	UserName = ref new Platform::Array<Platform::String^>(_countData);
	FacebookId = ref new Platform::Array<uint64>(_countData);
	AvatarUrl = ref new Platform::Array<Platform::String^>(_countData);
	Token = ref new Platform::Array<Platform::String^>(_countData);
	LevelMain = ref new Platform::Array<int16>(_countData);
	LevelCurrentWorld = ref new Platform::Array<int16>(_countData);
	ChooseCharater = ref new Platform::Array<int32>(_countData);
	NationCode = ref new Platform::Array<Platform::String^>(_countData);
	ChampionScore = ref new Platform::Array<int32>(_countData);
	LastChampionScore = ref new Platform::Array<int32>(_countData);
	RunHighScore = ref new Platform::Array<uint64>(_countData);
	LastRunHighScore = ref new Platform::Array<uint64>(_countData);
	Revision = ref new Platform::Array<uint64>(_countData);
	Data = ref new Platform::Array<uint8>(_countData*10000);
	DataSchemaVersion = ref new Platform::Array<uint64>(_countData);
	Rank = ref new Platform::Array<int32>(_countData);
	LastRank = ref new Platform::Array<int32>(_countData);
}

int PCLSaveGameDto::getStreamSize()
{
	//countData
	int size = 2;
	//Data
	size += 4;
	if(Data != nullptr)
	{
		size += Data->Length;
	}

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

		//Token
		size += 2;
		if(Token[i] != nullptr)
		{
			size += 2*Token[i]->Length();
		}

		//LevelMain
		size += 2;

		//LevelCurrentWorld
		size += 2;

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

		//LastChampionScore
		size += 4;

		//RunHighScore
		size += 8;

		//LastRunHighScore
		size += 8;

		//Revision
		size += 8;

		//DataSchemaVersion
		size += 8;

		//Rank
		size += 4;

		//LastRank
		size += 4;
	}

	return size;
}

StreamBuffer^ PCLSaveGameDto::CreateStream()
{
	StreamBuffer^ buf = ref new StreamBuffer(getStreamSize());

	buf->writeInt16(_countData);
	/*for (int i = 0; i < _countData; i++)
	{
		int size = 10000;
		Platform::Array<uint8>^ data = ref new Platform::Array<uint8>(size);
		int offset = i*size;
		for(int d = 0; d< size; d++){
			data[d] = Data[offset + d];
		}
	}*/
	if (_countData > 0)
	{
		buf->writeIBin(Data);
	}
	

	for (int i = 0; i < _countData; i++)
	{
		buf->writeUInt64(ID[i]);
		buf->writeSStr(UserName[i]);
		buf->writeUInt64(FacebookId[i]);
		buf->writeSStr(AvatarUrl[i]);
		buf->writeSStr(Token[i]);
		buf->writeInt16(LevelMain[i]);
		buf->writeInt16(LevelCurrentWorld[i]);
		buf->writeInt32(ChooseCharater[i]);
		buf->writeSStr(NationCode[i]);
		buf->writeInt32(ChampionScore[i]);
		buf->writeInt32(LastChampionScore[i]);
		buf->writeUInt64(RunHighScore[i]);
		buf->writeUInt64(LastRunHighScore[i]);
		buf->writeUInt64(Revision[i]);
		buf->writeUInt64(DataSchemaVersion[i]);
		buf->writeInt32(Rank[i]);
		buf->writeInt32(LastRank[i]);
	}

	return buf;
}