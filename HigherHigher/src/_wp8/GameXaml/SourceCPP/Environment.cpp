#include "pch.h"
#include "Environment.h"
#include "../game/Lib/Sound/XAudio.h"
#include "ServerStream\ServerStreamManager.h"
#include "DeviceInfomation.h"
Environment* Environment::_this = NULL;
bool Environment::_on_init = false;
Environment::Environment()
{
#if !defined(DEBUG_REMOVE_GAME)
	if (!_on_init)
	{
		_on_init = true;
		_game.Init();
		XAudio* sound_engine = new XAudio();
		sound_engine->Init();
		_game.SetSoundEngine(sound_engine);
		_need_capture_screen = false;
		_capture_texture = NULL;
		_on_init = false;
		_is_pause = false;
	}
#endif
}
Environment* Environment::GetInstace()
{
	if(_this == NULL && (!_on_init))
	{
		_this = new Environment();
	}
	return _this;
}
void Environment::SetDevice(ID3D11Device1* device)
{
#if !defined(DEBUG_REMOVE_GAME)
	if (_game._g != NULL)
	{
		((GDirectX11*)_game._g)->ChangeDevice(device);
		((XAudio*)_game.getSoundEngine())->ChangeDevice(device);
	}
#endif
}
void Environment::SetContext(ID3D11DeviceContext1* context)
{
#if !defined(DEBUG_REMOVE_GAME)
	if (_game._g != NULL){
		((GDirectX11*)_game._g)->ChangeContext(context);
	}
#endif
}
void Environment::SetDeviceScreen(float scr_width, float scr_height)
{

	_device.ScreenSize.W= scr_width;
	_device.ScreenSize.H = scr_height;
	_device.DeviceScreenSize.W = scr_width;
	_device.DeviceScreenSize.H = scr_height;
	_device.DisplayProperties.ColorDepth = PineDisplayColorDepth::ColorDepth24Bit;
	if (PhoneDirect3DXamlAppComponent::DeviceInfomation::IsSafe){
		_device.NoticeUnSafe();
	}
#if !defined(DEBUG_REMOVE_GAME)
	_game.InitDevice(_device);
#endif
}
void Environment::SetDeviceOrientaion(Orientation ori)
{
	_device.Device_Orientation = ori;
#if !defined(DEBUG_REMOVE_GAME)
	_game._device.Device_Orientation = ori;
	_game.OnChangeOrientation(ori);
#endif
}
void Environment::Update()
{
	PhoneDirect3DXamlAppComponent::ServerStreamManager::Update();
#if !defined(DEBUG_REMOVE_GAME)
	_game.Run();
#endif
}
void Environment::Render()
{
#if !defined(DEBUG_REMOVE_GAME)
	_game._g->Flush();
	_game.frameRateSyn();
#endif
}
void Environment::TouchBegin(int touch_id, float x, float y, int touch_tap)
{
#if !defined(DEBUG_REMOVE_GAME)
	_game.NotifyTouchesBegan(touch_id, x, y, touch_tap);
#endif
}
void Environment::TouchDrag(int touch_id, float x, float y, int touch_tap)
{
#if !defined(DEBUG_REMOVE_GAME)
	_game.NotifyTouchesMoved(touch_id, x, y, touch_tap);
#endif
}
void Environment::TouchEnd(int touch_id, float x, float y, int touch_tap)
{
#if !defined(DEBUG_REMOVE_GAME)
	_game.NotifyTouchesEnd(touch_id, x, y, touch_tap);
#endif
}
bool Environment::CaptureScreen(){
	return (PineRenderer::getInstance() != nullptr && PineRenderer::getInstance()->CaptureScreen());
}
void Environment::AsynCaptureScreen(PineServiceHandle* svc){
	if (PineRenderer::getInstance() != nullptr){
		//PineRenderer::getInstance()->AsynCaptureScreen(handle);
		svc->ResponseStatus = (PineRenderer::getInstance() != nullptr && PineRenderer::getInstance()->CaptureScreen())? 0: 1;
		svc->Status = PineServiceResult::Responsed;
	}
	else{
		svc->ResponseStatus = 1;
		svc->Status = PineServiceResult::Responsed;
	}
}
void Environment::AsynCaptureTexture(PineServiceHandle* svc, ID3D11Texture2D* texture, const char* fname){
	svc->ResponseStatus = 1;
	svc->Status = PineServiceResult::Responsed;
}