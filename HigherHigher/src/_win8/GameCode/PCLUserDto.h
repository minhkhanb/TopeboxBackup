#if !defined(_H_PCL_USER_DTO)
#define _H_PCL_USER_DTO
#include "../Pine/Src/StreamBuffer.h"
#include "../Pine/Src/ServerStream/ServerStreamManager.h"
namespace Pine{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PCLUserDto sealed 
	{
		public:
			
			property uint16 _countData;

			property Platform::Array<uint64>^ ID;
			property Platform::Array<Platform::String^>^ UserName;
			property Platform::Array<uint64>^ FacebookId;
			property Platform::Array<Platform::String^>^ AvatarUrl;
			property Platform::Array<int16>^ LevelMain;
			property Platform::Array<int16>^ LevelCurrentWorld;
			property Platform::Array<int32>^ ChooseCharater;
			property Platform::Array<Platform::String^>^ NationCode;
			property Platform::Array<int32>^ ChampionScore;
			property Platform::Array<uint64>^ RunHighScore;
			property Platform::Array<uint64>^ Revision;
			property Platform::Array<int32>^ Rank;
			property Platform::Array<uint64>^ DataSchemaVersion; 

			PCLUserDto(){};
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

			void setFacebookId(int i, uint64 _FacebookId)
			{
				FacebookId[i] = _FacebookId;
			};

			void setAvatarUrl(int i, Platform::String^ _AvartarUrl)
			{
				AvatarUrl[i] = _AvartarUrl;
			};

			void setLevelMain(int i, int16 _LevelMain)
			{
				LevelMain[i] = _LevelMain;
			};

			void setLevelCurrentWorld(int i, int16 _LevelCurrentWorld)
			{
				LevelCurrentWorld[i] = _LevelCurrentWorld;
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

			void setRunHighScore(int i, uint64 _RunHighScore)
			{
				RunHighScore[i] = _RunHighScore;
			};

			void setRevision(int i, uint64 _Revision)
			{
				Revision[i] = _Revision;
			};

			void setRank(int i, int32 _Rank)
			{
				Rank[i] = _Rank;
			};

			void setDataSchemaVersion(int i, uint64 _DataSchemaVersion)
			{
				DataSchemaVersion[i] = _DataSchemaVersion;
			};

			
	};
	
}
#endif//_H_PCL_USER_DTO