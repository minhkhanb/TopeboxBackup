#if !defined(_H_ENVIRONMENT)
#define _H_ENVIRONMENT
//using namespace DirectX;
#include "../game/GameCore/CGame.h"
////#include "WindowsPhoneCore\CGameCore.h"
#define CONNECTOR_GAME_DATA_UPDATE		7
#define CONNECTOR_FB_GET_APP_USER		9
#define CONNECTOR_GAME_DATA_UPLOAD		8
#define CONNECTOR_FB_FEED_MESSAGE		11
#define CONNECTOR_TW_FEED_MESSAGE		12
#define CONNECTOR_GAME_USER_RANDOM		13
#define CONNECTOR_FB_GOTO_FANPAGE		14
#define CONNECTOR_TW_GOTO_FANPAGE		15
#define CONNECTOR_MAIL_SEND				16
#define CONNECTOR_SMS_SEND				17
#define CONNECTOR_NOTIFICATION_RUN		19
#define CONNECTOR_NOTIFICATION_STOP		20
#define CONNECTOR_GAME_EXIT				21
#define CONNECTOR_SERVER_CHECK_AND_RUN	22
#define CONNECTOR_SERVER_LOAD_IMAGE		23
class Environment{
private:
	static Environment* _this;
	static bool _on_init;
	Device _device;
	Environment();
public:
	bool _need_capture_screen;
	ID3D11Texture2D* _capture_texture;
	static Environment* GetInstace();
	void SetDevice(ID3D11Device1* device);
	void SetContext(ID3D11DeviceContext1* context);
	void SetDeviceScreen(float scr_width, float scr_height);
	void SetDeviceOrientaion(Orientation ori);


	const XMMATRIX GetWorldMatrix(){ 
#if !defined(DEBUG_REMOVE_GAME)
		return ((GDirectX11*)_game._g)->GetWorldMatrix(); 
#else
		return XMMatrixIdentity();
#endif
	};


#if !defined(DEBUG_REMOVE_GAME)
	CGame _game;
#endif


	void Init();
	void Release();
	void Update();
	void Render();
	void TouchBegin(int touch_id, float x, float y, int touch_tap);
	void TouchDrag(int touch_id, float x, float y, int touch_tap);
	void TouchEnd(int touch_id, float x, float y, int touch_tap);

	bool CaptureScreen();
	void AsynCaptureScreen(PineServiceHandle* svc);
	void AsynCaptureTexture(PineServiceHandle* svc, ID3D11Texture2D* texture, const char* fname);
	bool _is_pause;
};
#endif//_H_ENVIRONMENT