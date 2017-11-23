#if !defined(_H_PCL_RANKEDNATION_DTO)
#define _H_PCL_SAVE_DTO
#include "../Pine/Src/StreamBuffer.h"
#include "../Pine/Src/ServerStream/ServerStreamManager.h"
namespace Pine{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PCLRankedNationDto sealed 
	{
		public:
			
			property uint16 _countData;

			property Platform::Array<Platform::String^>^ ID;
			property Platform::Array<uint64>^ ChampionScore;
			property Platform::Array<int32>^ MembersCount;
			
			PCLRankedNationDto(){};
			void InitData(int countData);
			void setFromStream(StreamBuffer^ buf);
			StreamBuffer^ CreateStream();
			int getStreamSize();

			void setID(int i, Platform::String^ _ID)
			{
				ID[i] = _ID;
			};

			void setChampionScore(int i, uint64 _ChampionScore)
			{
				ChampionScore[i] = _ChampionScore;
			};

			void setMembersCount(int i, int32 _MembersCount)
			{
				MembersCount[i] = _MembersCount;
			};
	};
	
}
#endif//_H_PCL_RANKEDNATION_DTO