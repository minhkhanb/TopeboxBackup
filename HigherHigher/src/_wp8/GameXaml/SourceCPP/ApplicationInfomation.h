namespace PhoneDirect3DXamlAppComponent{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ApplicationInfomation sealed{
	public:
		static property Platform::String^ AppId;
		static property int DeltaTimeFromLastRun;
		static property bool HaveServerTime;
		static property int DeltaServerTimeWithNow;
		static void onDeactive();
		static void onActive();
		static void onLoginFacebook();
		static void onClosing();
		static void onPopupOn();
		static void onPopupOff();
#if defined(_DEBUG_SUPPORT)
		static void onSupport(int add_money, int add_gem, int add_exp);
		static void onNeedSupportUpload();
#endif
	};
}