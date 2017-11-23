#include "stdafx.h"
#include "DeviceManager.h"

DeviceManager* DeviceManager::_this = NULL;
int DeviceManager::SCR_W = DEFAULT_SCR_W;
int DeviceManager::SCR_H = DEFAULT_SCR_H;
int DeviceManager::DIV_W = DEFAULT_DIV_W;
int DeviceManager::DIV_H = DEFAULT_DIV_H;
int DeviceManager::_device_model = DEVICE_IPOD;

extern "C" int GetInterfaceVersion();

DeviceManager::DeviceManager()
{
	/*_device_model = GetDeviceModel();
	printf("_device_model %d\n", _device_model);
	if (_device_model == DEVICE_IPHONE_4G || _device_model == DEVICE_IPOD_4)
	{
		SCR_W = D4_SCR_W;
		SCR_H = D4_SCR_H;
		DIV_W = D4_DIV_W;
		DIV_H = D4_DIV_H;
	}
	else if (_device_model == DEVICE_IPAD)
	{
		SCR_W = IPAD_SCR_W;
		SCR_H = IPAD_SCR_H;
		DIV_W = IPAD_DIV_W;
		DIV_H = IPAD_DIV_H;
	}
	else
	{
		SCR_W = DEFAULT_SCR_W;
		SCR_H = DEFAULT_SCR_H;
		DIV_W = DEFAULT_DIV_W;
		DIV_H = DEFAULT_DIV_H;
	}*/
    int interface = GetInterfaceVersion();
    if (interface == 2)
    {
        
		SCR_W = IPAD_SCR_W;
		SCR_H = IPAD_SCR_H;
		DIV_W = IPAD_DIV_W;
		DIV_H = IPAD_DIV_H;
    }
    else
    {
        SCR_W = DEFAULT_SCR_W;
		SCR_H = DEFAULT_SCR_H;
		DIV_W = DEFAULT_DIV_W;
		DIV_H = DEFAULT_DIV_H;
    }
}

DeviceManager::~DeviceManager()
{
	delete _this;
}
DeviceManager* DeviceManager::GetInstace()
{
	if (_this == NULL)
		_this = new DeviceManager();
	return _this;
}

void DeviceManager::setScreenSize(int width, int height)
{
	SCR_W = width;
	SCR_H = height;
}

int DeviceManager::getDeviceModel()
{
	return _device_model;
}
