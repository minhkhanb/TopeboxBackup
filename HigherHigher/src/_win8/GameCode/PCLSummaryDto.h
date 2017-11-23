#if !defined(_H_PCL_SUMMARY_DTO)
#define _H_PCL_SAVE_DTO
#include "../Pine/Src/StreamBuffer.h"
#include "../Pine/Src/ServerStream/ServerStreamManager.h"
namespace Pine{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PCLSummaryDto sealed 
	{
		public:
			
			property uint16 _countData;

			property Platform::Array<uint64>^ ID;
			property Platform::Array<int16>^ LevelMain;
			property Platform::Array<int16>^ LevelCurrentWorld;
			property Platform::Array<int32>^ TournamentsPlayed;
			property Platform::Array<int32>^ TournamentsPushed;
			property Platform::Array<int32>^ LivesSent;
			property Platform::Array<int32>^ AbaddonProcessing;
			property Platform::Array<int32>^ XmasProcessing;
			property Platform::Array<int32>^ FriendsCount;
			property Platform::Array<int32>^ BagOfVacinePack;
			property Platform::Array<int32>^ CrateOfVacinePack;
			property Platform::Array<int32>^ StockpileOfVacinePack;
			property Platform::Array<int32>^ TruckLoadOfVacinePack;
			property Platform::Array<int32>^ VacinePackReserve01;
			property Platform::Array<int32>^ VacinePackReserve02;
			property Platform::Array<int32>^ BagOfFoodPack;
			property Platform::Array<int32>^ CrateOfFoodPack;
			property Platform::Array<int32>^ StockpileOfFoodPack;
			property Platform::Array<int32>^ TruckLoadOfFoodPack;
			property Platform::Array<int32>^ FoodPackReserve01;
			property Platform::Array<int32>^ FoodPackReserve02;

			PCLSummaryDto(){};
			void InitData(int countData);
			void setFromStream(StreamBuffer^ buf);
			StreamBuffer^ CreateStream();
			int getStreamSize();

			void setID(int i, uint64 _ID)
			{
				ID[i] = _ID;
			};

			void setLevelMain(int i, int16 _LevelMain)
			{
				LevelMain[i] = _LevelMain;
			};

			void setLevelCurrentWorld(int i, int16 _LevelCurrentWorld)
			{
				LevelCurrentWorld[i] = _LevelCurrentWorld;
			};

			void setTournamentsPlayed(int i, int32 _TournamentsPlayed)
			{
				TournamentsPlayed[i] = _TournamentsPlayed;
			};

			void setTournamentsPushed(int i, int32 _TournamentsPushed)
			{
				TournamentsPushed[i] = _TournamentsPushed;
			};

			void setLivesSent(int i, int32 _LivesSent)
			{
				LivesSent[i] = _LivesSent;
			};

			void setAbaddonProcessing(int i, int32 _AbaddonProcessing)
			{
				AbaddonProcessing[i] = _AbaddonProcessing;
			};

			void setXmasProcessing(int i, int32 _XmasProcessing)
			{
				XmasProcessing[i] = _XmasProcessing;
			};

			void setFriendsCount(int i, int32 _FriendsCount)
			{
				FriendsCount[i] = _FriendsCount;
			};

			void setBagOfVacinePack(int i, int32 _BagOfVacinePack)
			{
				BagOfVacinePack[i] = _BagOfVacinePack;
			};

			void setCrateOfVacinePack(int i, int32 _CrateOfVacinePack)
			{
				CrateOfVacinePack[i] = _CrateOfVacinePack;
			};

			void setStockpileOfVacinePack(int i, int32 _StockpileOfVacinePack)
			{
				StockpileOfVacinePack[i] = _StockpileOfVacinePack;
			};

			void setTruckLoadOfVacinePack(int i, int32 _TruckLoadOfVacinePack)
			{
				TruckLoadOfVacinePack[i] = _TruckLoadOfVacinePack;
			};

			void setVacinePackReserve01(int i, int32 _VacinePackReserve01)
			{
				VacinePackReserve01[i] = _VacinePackReserve01;
			};

			void setVacinePackReserve02(int i, int32 _VacinePackReserve02)
			{
				VacinePackReserve02[i] = _VacinePackReserve02;
			};

			void setBagOfFoodPack(int i, int32 _BagOfFoodPack)
			{
				BagOfFoodPack[i] = _BagOfFoodPack;
			};

			void setCrateOfFoodPack(int i, int32 _CrateOfFoodPack)
			{
				CrateOfFoodPack[i] = _CrateOfFoodPack;
			};

			void setStockpileOfFoodPack(int i, int32 _StockpileOfFoodPack)
			{
				StockpileOfFoodPack[i] = _StockpileOfFoodPack;
			};

			void setTruckLoadOfFoodPack(int i, int32 _TruckLoadOfFoodPack)
			{
				TruckLoadOfFoodPack[i] = _TruckLoadOfFoodPack;
			};

			void setFoodPackReserve01(int i, int32 _FoodPackReserve01)
			{
				FoodPackReserve01[i] = _FoodPackReserve01;
			};

			void setFoodPackReserve02(int i, int32 _FoodPackReserve02)
			{
				FoodPackReserve02[i] = _FoodPackReserve02;
			};
	};
	
}
#endif//_H_PCL_SUMMARY_DTO