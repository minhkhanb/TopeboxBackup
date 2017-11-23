#if !defined(_H_PCL_RANKEDUSER_DTO)
#define _H_PCL_RANKEDUSER_DTO
#include "../Pine/Src/StreamBuffer.h"
#include "../Pine/Src/ServerStream/ServerStreamManager.h"
namespace Pine{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PCLRankedUserDto sealed 
	{
		public:
			
			property uint16 _countData;

			property Platform::Array<uint64>^ ID;
			property Platform::Array<Platform::String^>^ UserName;
			property Platform::Array<Platform::String^>^ AvatarUrl;
			property Platform::Array<int32>^ ChooseCharater;
			property Platform::Array<Platform::String^>^ NationCode;
			property Platform::Array<int32>^ ChampionScore;
			
			PCLRankedUserDto(){};
			void InitData(int countData);
			void setFromStream(StreamBuffer^ buf);
			StreamBuffer^ CreateStream();
			int getStreamSize();

			void setID(int i, uint64 _ID)
			{
				ID[i] = _ID;
			};

			void setUserName(int i, Platform::String^ _UserName)
			{
				UserName[i] = _UserName;
			};

			void setAvatarUrl(int i, Platform::String^ _AvartarUrl)
			{
				AvatarUrl[i] = _AvartarUrl;
			};

			void setChooseCharater(int i, int32 _ChooseCharater)
			{
				ChooseCharater[i] = _ChooseCharater;
			};

			void setNationCode(int i, Platform::String^ _NationCode)
			{
				NationCode[i] = _NationCode;
			};

			void setChampionScore(int i, int32 _ChampionScore)
			{
				ChampionScore[i] = _ChampionScore;
			};

	};
	
}
#endif//_H_PCL_RANKEDUSER_DTO