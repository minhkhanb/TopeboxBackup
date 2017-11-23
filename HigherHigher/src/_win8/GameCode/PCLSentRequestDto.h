#if !defined(_H_PCL_SENDREQUEST_DTO)
#define _H_PCL_SENDREQUEST_DTO
#include "../Pine/Src/StreamBuffer.h"
#include "../Pine/Src/ServerStream/ServerStreamManager.h"
namespace Pine{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PCLSentRequestDto sealed 
	{
		public:
			
			property uint16 _countData;

			property Platform::Array<int64>^ ID;
			property Platform::Array<uint64>^ GameId;
			property Platform::Array<uint64>^ Sender;
			property Platform::Array<uint64>^ Receiver;
			property Platform::Array<int32>^ RequestType;
			
			PCLSentRequestDto(){};
			void InitData(int countData);
			void setFromStream(StreamBuffer^ buf);
			StreamBuffer^ CreateStream();
			int getStreamSize();

			void setID(int i, int64 _ID)
			{
				ID[i] = _ID;
			};

			void setGameId(int i, uint64 _GameId)
			{
				GameId[i] = _GameId;
			};

			void setSender(int i, uint64 _Sender)
			{
				Sender[i] = _Sender;
			};

			void setReceiver(int i, uint64 _Receiver)
			{
				Receiver[i] = _Receiver;
			};

			void setRequestType(int i, int32 _RequestType)
			{
				RequestType[i] = _RequestType;
			};
	};
	
}
#endif//_H_PCL_SENDREQUEST_DTO