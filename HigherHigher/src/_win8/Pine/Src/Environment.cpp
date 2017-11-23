#include "pch.h"
#include "Environment.h"
#include "ServerStream\ServerStreamManager.h"
#include "../../../game/Lib/Sound/XAudio.h"
Environment* Environment::_this = NULL;
bool Environment::_on_init = false;
Environment::Environment() :_touch_count(0){
	if (!_on_init){
		_on_init = true;
		_game.Init();
		XAudio* sound_engine = new XAudio();
		sound_engine->Init();
		_game.SetSoundEngine(sound_engine);
		_on_init = false;
		for (int i = 0; i < 10; i++){
			_touch_id_mng[i] = -1;
		}
	}
}

Environment* Environment::GetInstance(){
	if (_this == NULL && (!_on_init)){
		_this = new Environment();
	}
	return _this;
}
void Environment::SetDevice(ID3D11Device1* device)
{
	if (_game._g != NULL){
		((GDirectX11*)_game._g)->ChangeDevice(device);
		((XAudio*)_game.getSoundEngine())->ChangeDevice(device);
	}
}
void Environment::SetContext(ID3D11DeviceContext1* context)
{
	if (_game._g != NULL){
		((GDirectX11*)_game._g)->ChangeContext(context);
	}
}
void Environment::SetDeviceScreen(float scr_width, float scr_height)
{
	_device.ScreenSize.W = scr_width;
	_device.ScreenSize.H = scr_height;
	_device.IsSupportAppleGameCenter = false;
	_device.IsSupportTwitter = false;
	_device.IsSupportFacebook = true;
	_game.InitDevice(_device);
}
void Environment::SetDeviceOrientaion(Orientation ori)
{
	_device.Device_Orientation = ori;
	_game._device.Device_Orientation = ori;
	if (_game._g != NULL){
		////_game._g->ChangeOrientation(ori);
	}
}
void Environment::Update(){
	Pine::ServerStreamManager::Update();
	_game.Run();
}
void Environment::Render(){
	//_game._g->DrawRect(0, 0, k_DEVICE_WIDTH, k_SCREEN_HEIGHT);
	_game._g->Flush();
}
void Environment::TouchBegin(int touch_id, float x, float y, int touch_tap)
{
	
	int handle = -1;
	for (int t = 0; t < 10; t++){
		if (_touch_id_mng[t] < 0){
			_touch_id_mng[t] = touch_id;
			handle = t;
			break;
		}
	}
	if (handle > -1){
		//PDEBUG("touch begin:%d\n", handle);
		_game.NotifyTouchesBegan(handle, x, y, touch_tap);
	}
}
void Environment::TouchDrag(int touch_id, float x, float y, int touch_tap)
{
	//PDEBUG("touch move:%d\n", touch_id);
	int handle = -1;
	for (int t = 0; t < 10; t++){
		if (_touch_id_mng[t] == touch_id){
			handle = t;
			break;
		}
	}
	if (handle > -1){
		//PDEBUG("touch move:%d\n", handle);
		_game.NotifyTouchesMoved(handle, x, y, touch_tap);
	}
}
void Environment::TouchEnd(int touch_id, float x, float y, int touch_tap)
{
	
	int handle = -1;
	for (int t = 0; t < 10; t++){
		if (_touch_id_mng[t] == touch_id){
			_touch_id_mng[t] = -1;
			handle = t;
			break;
		}
	}
	if (handle > -1){
		//PDEBUG("touch end:%d\n", handle);
		_game.NotifyTouchesEnd(handle, x, y, touch_tap);
	}
}