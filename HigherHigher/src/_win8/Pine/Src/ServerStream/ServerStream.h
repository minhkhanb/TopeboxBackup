#if !defined(_H_SERVER_STREAM)
#define _H_SERVER_STREAM
namespace Pine{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ServerStream sealed{
	public:
		property int CurrentHandle;
		property bool AutoFinish;
		property uint32 RequestCode;
		property int Status;
		property int ErrorCode;
		property Platform::Array<uint8>^ PostData;
		property Platform::Array<uint8>^ ResponseData;
	};
}
#endif