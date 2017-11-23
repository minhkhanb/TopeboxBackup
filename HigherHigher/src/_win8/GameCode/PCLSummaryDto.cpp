#include "pch.h"
#include "PCLSummaryDto.h"
using namespace Pine;

void PCLSummaryDto::setFromStream(StreamBuffer^ buf)
{
	_countData = buf->readInt16();
	InitData(_countData);
	for (int i = 0; i < _countData; i++)
	{
		ID[i] = buf->readUInt64();
		LevelMain[i] = buf->readInt16();
		LevelCurrentWorld[i] = buf->readInt16();
		TournamentsPlayed[i] = buf->readInt32();
		TournamentsPushed[i] = buf->readInt32();
		LivesSent[i] = buf->readInt32();
		AbaddonProcessing[i] = buf->readInt32();
		XmasProcessing[i] = buf->readInt32();
		FriendsCount[i] = buf->readInt32();
		BagOfVacinePack[i] = buf->readInt32();
		CrateOfVacinePack[i] = buf->readInt32();
		StockpileOfVacinePack[i] = buf->readInt32();
		TruckLoadOfVacinePack[i] = buf->readInt32();
		VacinePackReserve01[i] = buf->readInt32();
		VacinePackReserve02[i] = buf->readInt32();
		BagOfFoodPack[i] = buf->readInt32();
		CrateOfFoodPack[i] = buf->readInt32();
		StockpileOfFoodPack[i] = buf->readInt32();
		TruckLoadOfFoodPack[i] = buf->readInt32();
		FoodPackReserve01[i] = buf->readInt32();
		FoodPackReserve02[i] = buf->readInt32();
	}
}

void PCLSummaryDto::InitData(int countData)
{
	_countData = countData;

	ID = ref new Platform::Array<uint64>(_countData);
	LevelMain = ref new Platform::Array<int16>(_countData);
	LevelCurrentWorld = ref new Platform::Array<int16>(_countData);
	TournamentsPlayed = ref new Platform::Array<int32>(_countData);
	TournamentsPushed = ref new Platform::Array<int32>(_countData);
	LivesSent = ref new Platform::Array<int32>(_countData);
	AbaddonProcessing = ref new Platform::Array<int32>(_countData);
	XmasProcessing = ref new Platform::Array<int32>(_countData);
	FriendsCount = ref new Platform::Array<int32>(_countData);
	BagOfVacinePack = ref new Platform::Array<int32>(_countData);
	CrateOfVacinePack = ref new Platform::Array<int32>(_countData);
	StockpileOfVacinePack = ref new Platform::Array<int32>(_countData);
	TruckLoadOfVacinePack = ref new Platform::Array<int32>(_countData);
	VacinePackReserve01 = ref new Platform::Array<int32>(_countData);
	VacinePackReserve02 = ref new Platform::Array<int32>(_countData);
	BagOfFoodPack = ref new Platform::Array<int32>(_countData);
	CrateOfFoodPack = ref new Platform::Array<int32>(_countData);
	StockpileOfFoodPack = ref new Platform::Array<int32>(_countData);
	TruckLoadOfFoodPack = ref new Platform::Array<int32>(_countData);
	FoodPackReserve01 = ref new Platform::Array<int32>(_countData);
	FoodPackReserve02 = ref new Platform::Array<int32>(_countData);
}

int PCLSummaryDto::getStreamSize()
{
	//countData
	int size = 2;

	for (int i = 0; i < _countData; i++)
	{
		//ID
		size += 8;

		//LevelMain
		size += 2;

		//LevelCurrentWorld
		size += 2;

		//TournamentsPlayed
		size += 4;

		//TournamentsPushed
		size += 4;

		//LivesSent
		size += 4;

		//AbaddonProcessing
		size += 4;

		//XmasProcessing
		size += 4;

		//FriendsCount
		size += 4;

		//BagOfVacinePack
		size += 4;

		//CrateOfVacinePack
		size += 4;

		//StockpileOfVacinePack
		size += 4;

		//TruckLoadOfVacinePack
		size += 4;

		//VacinePackReserve01
		size += 4;

		//VacinePackReserve02
		size += 4;

		//BagOfFoodPack
		size += 4;

		//CrateOfFoodPack
		size += 4;

		//StockpileOfFoodPack
		size += 4;

		//TruckLoadOfFoodPack
		size += 4;

		//FoodPackReserve01
		size += 4;

		//FoodPackReserve02
		size += 4;
	}

	return size;
}

StreamBuffer^ PCLSummaryDto::CreateStream()
{
	StreamBuffer^ buf = ref new StreamBuffer(getStreamSize());

	buf->writeInt16(_countData);
	
	for (int i = 0; i < _countData; i++)
	{
		buf->writeUInt64(ID[i]);
		buf->writeInt16(LevelMain[i]);
		buf->writeInt16(LevelCurrentWorld[i]);
		buf->writeInt32(TournamentsPlayed[i]);
		buf->writeInt32(TournamentsPushed[i]);
		buf->writeInt32(LivesSent[i]);
		buf->writeInt32(AbaddonProcessing[i]);
		buf->writeInt32(XmasProcessing[i]);
		buf->writeInt32(FriendsCount[i]);
		buf->writeInt32(BagOfVacinePack[i]);
		buf->writeInt32(CrateOfVacinePack[i]);
		buf->writeInt32(StockpileOfVacinePack[i]);
		buf->writeInt32(TruckLoadOfVacinePack[i]);
		buf->writeInt32(VacinePackReserve01[i]);
		buf->writeInt32(VacinePackReserve02[i]);
		buf->writeInt32(BagOfFoodPack[i]);
		buf->writeInt32(CrateOfFoodPack[i]);
		buf->writeInt32(StockpileOfFoodPack[i]);
		buf->writeInt32(TruckLoadOfFoodPack[i]);
		buf->writeInt32(FoodPackReserve01[i]);
		buf->writeInt32(FoodPackReserve02[i]);
	}

	return buf;
}