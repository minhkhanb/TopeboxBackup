#include "pch.h"
#include "PineUnityAds.h"

extern "C" void OS_AsynUnityCheckVideo(PineServiceHandle* hsvc);
extern "C" void OS_AsynUnityCheckRewardVideo(PineServiceHandle* hsvc);

PineUnityAdsAsync::PineUnityAdsAsync()
{
	this->_is_video_cached = false;
	this->_is_videoreward_cached = false;

	this->_svc_check_video_.reset();
	this->_svc_check_rewardvideo_.reset();
}

void PineUnityAdsAsync::update()
{
	if (!_is_video_cached && !_is_videoreward_cached)
	{
		// request check 
		if (_svc_check_video_.Status != PineServiceResult::Responsed)
		{
			OS_AsynUnityCheckVideo(&_svc_check_video_);
		}
		else
		{
			if (_svc_check_video_.ResponseStatus == 0)
			{
				_is_video_cached = true;
				_svc_check_video_.reset();
			}
			else
			{
				_is_video_cached = false;
				_svc_check_video_.reset();
			}
		}

		if (_svc_check_rewardvideo_.Status != PineServiceResult::Responsed)
		{
			OS_AsynUnityCheckRewardVideo(&_svc_check_rewardvideo_);
		}
		else
		{
			if (_svc_check_rewardvideo_.ResponseStatus == 0)
			{
				_is_videoreward_cached = true;
				_svc_check_rewardvideo_.reset();
			}
			else
			{
				_is_videoreward_cached = false;
				_svc_check_rewardvideo_.reset();
			}
		}
	}
}