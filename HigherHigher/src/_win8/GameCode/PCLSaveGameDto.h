#if !defined(_H_PCL_SAVEGAME_DTO)
#define _H_PCL_SAVE_DTO
#include "../Pine/Src/StreamBuffer.h"
#include "../Pine/Src/ServerStream/ServerStreamManager.h"
namespace Pine{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PCLSaveGameDto sealed 
	{
		public:
			
			property uint16 _countData;

			property Platform::Array<uint64>^ ID;
			property Platform::Array<Platform::String^>^ UserName;
			property Platform::Array<uint64>^ FacebookId;
			property Platform::Array<Platform::String^>^ AvatarUrl;
			property Platform::Array<Platform::String^>^ Token;
			property Platform::Array<int16>^ LevelMain;
			property Platform::Array<int16>^ LevelCurrentWorld;
			property Platform::Array<int32>^ ChooseCharater;
			property Platform::Array<Platform::String^>^ NationCode;
			property Platform::Array<int32>^ ChampionScore;
			property Platform::Array<int32>^ LastChampionScore;
			property Platform::Array<uint64>^ RunHighScore;
			property Platform::Array<uint64>^ LastRunHighScore;
			property Platform::Array<uint64>^ Revision;
			property Platform::Array<uint8>^ Data;
			property Platform::Array<uint64>^ DataSchemaVersion; 
			property Platform::Array<int32>^ Rank;
			property Platform::Array<int32>^ LastRank;
			
			PCLSaveGameDto(){};
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

			void setToken(int i, Platform::String^ _Token)
			{
				Token[i] = _Token;
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

			void setLastChampionScore(int i, int32 _LastChampionScore)
			{
				LastChampionScore[i] = _LastChampionScore;
			};

			void setRunHighScore(int i, uint64 _RunHighScore)
			{
				RunHighScore[i] = _RunHighScore;
			};

			void setLastRunHighScore(int i, uint64 _LastRunHighScore)
			{
				LastRunHighScore[i] = _LastRunHighScore;
			};

			void setRevision(int i, uint64 _Revision)
			{
				Revision[i] = _Revision;
			};

			void setData(int i, const Platform::Array<uint8>^ _Data)
			{
				for (int k = 0; k < 10000; k++)
				{
					Data[i*10000+k] = _Data[k];
				}
			};

			void setDataSchemaVersion(int i, uint64 _DataSchemaVersion)
			{
				DataSchemaVersion[i] = _DataSchemaVersion;
			};

			void setRank(int i, int32 _Rank)
			{
				Rank[i] = _Rank;
			};

			void setLastRank(int i, int32 _LastRank)
			{
				LastRank[i] = _LastRank;
			};
	};
	
}
#endif//_H_PCL_SAVE_DTO