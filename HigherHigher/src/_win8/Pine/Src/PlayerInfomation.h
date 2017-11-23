namespace Pine{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PlayerInfomation sealed{
	public:
		static property Platform::String^ FacebookId;
		static property Platform::String^ FacebookUserName;
		static property Platform::String^ UserId;
		static property Platform::String^ FacebookAvatarUrl;
		static property bool LoginedFacebook;
	};
}