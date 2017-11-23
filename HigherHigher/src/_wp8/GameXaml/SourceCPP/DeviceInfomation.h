namespace PhoneDirect3DXamlAppComponent{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DeviceInfomation sealed{
		Platform::Array<uint8>^ _data;
	public:
		static property bool IsNetworkAvailable;
		static property bool IsSafe;
		static property Platform::String^ RegionFormat;
		static property Platform::String^ RegionFormatShort;
	};
}