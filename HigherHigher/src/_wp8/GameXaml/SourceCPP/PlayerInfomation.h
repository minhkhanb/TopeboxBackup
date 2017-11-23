namespace PhoneDirect3DXamlAppComponent{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PlayerInfomation sealed{
	public:
		static property Platform::String^ FacebookId;
		static property Platform::String^ FacebookUserName;
		static property Platform::String^ FacebookAvatarUrl;
		static property Platform::String^ UserId;
		static property bool LoginedFacebook;
		static property Platform::String^ TwitterId;
		static property Platform::String^ TwitterUserName;
		static property bool LoginedTwitter;
	};
}