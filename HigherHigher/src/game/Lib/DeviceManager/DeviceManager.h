#pragma once
#include "stdio.h"
#include "../../Define/Defines.h"

//#define DEFAULT_SCR_H	320
//#define DEFAULT_SCR_W	480

extern "C" int GetDeviceModel();

class DeviceManager
{
public:	
	static int SCR_W;
	static int SCR_H;
	static int DIV_W;
	static int DIV_H;
	static DeviceManager* _this;
	
	static int _device_model;
	
	DeviceManager();
	~DeviceManager();

	static DeviceManager* GetInstace();
	void setScreenSize(int width, int height);
	int getDeviceModel();
};
