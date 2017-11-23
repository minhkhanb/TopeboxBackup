#if !defined(_H_PCL_SERVERINFO_DTO)
#define _H_PCL_SERVERINFO_DTO
#include "../Pine/Src/StreamBuffer.h"
#include "../Pine/Src/ServerStream/ServerStreamManager.h"
namespace Pine{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PCLServerInfoDto sealed 
	{
		public:
			
			property uint16 _countData;

			property Platform::Array<uint64>^ ID;
			property Platform::Array<Windows::Foundation::DateTime>^ PstNow;
			property Platform::Array<int32>^ PstYear;
			property Platform::Array<int32>^ PstMonth;
			property Platform::Array<int32>^ PstDay;
			property Platform::Array<int32>^ PstHour;
			property Platform::Array<int32>^ PstMinute;
			property Platform::Array<int32>^ PstSecond;
			property Platform::Array<int64>^ ResetRemaining;
			
			PCLServerInfoDto(){};
			void InitData(int countData);
			void setFromStream(StreamBuffer^ buf);
			StreamBuffer^ CreateStream();
			int getStreamSize();
			void setPstNow(int i, Windows::Foundation::DateTime _PstNow)
			{
				PstNow[i] = _PstNow;
			};
			void setPstYear(int i, int32 _PstYear)
			{
				PstYear[i] = _PstYear;
			};
			void setPstMonth(int i, int32 _PstMonth)
			{
				PstMonth[i] = _PstMonth;
			};
			void setPstDay(int i, int32 _PstDay)
			{
				PstDay[i] = _PstDay;
			};
			void setPstHour(int i, int32 _PstHour)
			{
				PstHour[i] = _PstHour;
			};
			void setPstMinute(int i, int32 _PstMinute)
			{
				PstMinute[i] = _PstMinute;
			};
			void setPstSecond(int i, int32 _PstSecond)
			{
				PstSecond[i] = _PstSecond;
			};
			void setResetRemaining(int i, int64 _ResetRemaining)
			{
				ResetRemaining[i] = _ResetRemaining;
			};

	};
	
}
#endif//_H_PCL_SERVERINFO_DTO