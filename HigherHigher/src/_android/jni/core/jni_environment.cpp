#include "pch.h"
#include "jni_environment.h"

Environment* Environment::_this = NULL;

Environment::Environment(){
	IsInit = false;
	_is_pause = false;
}

void Environment::Init(int device_width, int device_height){
	Device device;
	device.ScreenSize.W = device_width;
	device.ScreenSize.H= device_height;
	_sound_engine.Init();
	_game.SetSoundEngine(&_sound_engine);
	_game.InitDevice(device);
	_game.Init();
	IsInit = true;
	ScreenWidth = device_width;
	ScreenHeight = device_height;
}

void Environment::SizeChange(int device_width, int device_height){
	PDEBUG("CHANGE SCREEN SIZE:%d %d", device_width, device_height);
	Device device;
	device.ScreenSize.W = device_width;
	device.ScreenSize.H = device_height;
	_game.InitDevice(device);
}

Environment* Environment::GetInstance(){
	if(_this == NULL){
		_this = new Environment();
	}
	return _this;
}

void Environment::Update(){
	_game.Run();
}

void Environment::Render(){
	_game._g->Flush();
}

void Environment::TouchBegin(int id, int x, int y){
    if (IsInit && !_is_pause)
    {
	    _game.NotifyTouchesBegan(id, x, y, 1);
	}
}

void Environment::TouchDrag(int id, int x, int y){
    if (IsInit && !_is_pause)
    {
	    _game.NotifyTouchesMoved(id, x, y, 1);
	}
}

void Environment::TouchEnd(int id, int x, int y){
    if (IsInit && !_is_pause)
    {
	    _game.NotifyTouchesEnd(id, x, y,1);
	}
}

void Environment::ChangeOrientation(int ori){
	PDEBUG("Change ORIENTATION:%d",ori);
	if(ori == 0){
		//_game.OnChangeOrientation(Orientation::Portrait);
		
	}else if(ori == 1){
		//_game.OnChangeOrientation(Orientation::Landscape_90);
	}
}
void Environment::Suppend(){
	PDEBUG("CALL SUPPEND\n");
	if(IsInit && !_is_pause){
		_game.Pause();
		_is_pause = true;
	}
}
void Environment::Resume(){
	PDEBUG("CALL RESUME\n");
	if(_is_pause){
		_game.Resume();
		_is_pause = false;
	}
}

bool Environment::BackKeyPress()
{
	return _game.OnBackKeyPress();
}