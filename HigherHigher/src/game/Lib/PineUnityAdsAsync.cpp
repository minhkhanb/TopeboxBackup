#include "pch.h"
#include "PineUnityAdsAsync.h"

extern "C" void OS_AsynUnityAdsInterstitialPrepare(PineServiceHandle* hsvc);
extern "C" void OS_AsynUnityAdsInterstitialShow(PineServiceHandle* hsvc);

extern "C" void OS_AsynUnityAdsRewardedVideoPrepare(PineServiceHandle* hsvc);
extern "C" void OS_AsynUnityAdsRewardedVideoShow(PineServiceHandle* hsvc);

extern "C" void OS_AsynUnityAdsSkippableVideoPrepare(PineServiceHandle* hsvc);
extern "C" void OS_AsynUnityAdsSkippableVideoShow(PineServiceHandle* hsvc);

PineUnityAdsAsync::PineUnityAdsAsync()
{
	_svc_prepare_ads.reset();
	_svc_prepare_video.reset();
	_svc_prepare_skippable_video.reset();
}

PineUnityAdsAsync::enumUnityAdsUpdateResult PineUnityAdsAsync::update()
{
	if (_svc_prepare_ads.Status == PineServiceResult::WaitingResponse){
		OS_AsynUnityAdsInterstitialPrepare(&_svc_prepare_ads);
		if (_svc_prepare_ads.isFinish()){
		//	showInterstitial();
			PDEBUG("unity ad prepare finish:%d\n", _svc_prepare_ads.ResponseStatus);
			if(_svc_prepare_ads.ResponseStatus == 1)
			{
				prepareAds();
			}
		}
	}
	
	if(_svc_show_ads.Status == PineServiceResult::WaitingResponse)
	{
		OS_AsynUnityAdsInterstitialShow(&_svc_show_ads);
		if(_svc_show_ads.Status == PineServiceResult::Responsed)
		{
			prepareAds();
			_svc_show_ads.reset();
		}
	}
	
	if (_svc_prepare_video.Status == PineServiceResult::WaitingResponse){
		OS_AsynUnityAdsRewardedVideoPrepare(&_svc_prepare_video);
		if (_svc_prepare_video.isFinish()){
		//	showInterstitial();
			if(_svc_prepare_video.ResponseStatus == 0)
			{
				PDEBUG("unity video ad prepare finish successful!");
				
			}
			else
			{
				if (_svc_prepare_video.ResponseStatus == 1)
				{
					prepareVideo();
				}
			}
		}
	}
	
	if(_svc_show_video.Status == PineServiceResult::WaitingResponse)
	{
		OS_AsynUnityAdsRewardedVideoShow(&_svc_show_video);
		if(_svc_show_video.Status == PineServiceResult::Responsed)
		{
			prepareVideo();
			
			if (_svc_show_video.ResponseStatus == 0)
			{
				PDEBUG("Show unity video ads successful!");
				_svc_show_video.reset();
				return enumUnityAdsUpdateResult::SHOW_VIDEO_SUCCESS;	
			}
			else
			{
				PDEBUG("Show unity video ads fail!");
				_svc_show_video.reset();
				return enumUnityAdsUpdateResult::SHOW_VIDEO_FAIL;
			}
			
		}
	}

	if (_svc_prepare_skippable_video.Status == PineServiceResult::WaitingResponse) {
		OS_AsynUnityAdsSkippableVideoPrepare(&_svc_prepare_skippable_video);
		if (_svc_prepare_skippable_video.isFinish()) {
			if (_svc_prepare_skippable_video.ResponseStatus == 1)
			{
				prepareSkippableVideo();
			}
			PDEBUG("unity video skippable ads prepare finish:%d\n", _svc_prepare_skippable_video.ResponseStatus);
		}
	}

	if (_svc_show_skippable_video.Status == PineServiceResult::WaitingResponse)
	{
		OS_AsynUnityAdsSkippableVideoShow(&_svc_show_skippable_video);
		if (_svc_show_skippable_video.Status == PineServiceResult::Responsed)
		{
			prepareSkippableVideo();
			
			if (_svc_show_skippable_video.ResponseStatus == 0)
			{
				PDEBUG("Show unity video skippable ads successful!");
				_svc_show_skippable_video.reset();
				return enumUnityAdsUpdateResult::SHOW_VIDEO_SUCCESS;
			}
			else
			{
				PDEBUG("Show unity video skippable ads fail!");
				_svc_show_skippable_video.reset();
				return enumUnityAdsUpdateResult::SHOW_VIDEO_FAIL;
			}
			
		}
	}
	return enumUnityAdsUpdateResult::NOTHING;
}

bool PineUnityAdsAsync::isAdsReady()
{
	return _svc_prepare_ads.Status == PineServiceResult::Responsed && _svc_prepare_ads.ResponseStatus == 0;
}

void PineUnityAdsAsync::prepareAds()
{
	PDEBUG("call prepare unity ads\n");
	if (_svc_prepare_ads.Status != PineServiceResult::WaitingResponse){
		_svc_prepare_ads.reset();
		OS_AsynUnityAdsInterstitialPrepare(&_svc_prepare_ads);
		PDEBUG("unity ads prepare\n");
	}
}

void PineUnityAdsAsync::showAds()
{
	PDEBUG("call unity ads 0\n");
	if(isAdsReady())
	{
		PDEBUG("call unity ads\n");
		_svc_show_ads.reset();
		OS_AsynUnityAdsInterstitialShow(&_svc_show_ads);
	}
}
	
bool PineUnityAdsAsync::isVideoReady()
{
	return _svc_prepare_video.Status == PineServiceResult::Responsed && _svc_prepare_video.ResponseStatus == 0;
}

bool PineUnityAdsAsync::isVideoSkippalbeReady()
{
	return _svc_prepare_skippable_video.Status == PineServiceResult::Responsed && _svc_prepare_skippable_video.ResponseStatus == 0;
}

void PineUnityAdsAsync::prepareVideo()
{
	PDEBUG("call prepare unity ads video\n");
	if (_svc_prepare_video.Status != PineServiceResult::WaitingResponse){
		_svc_prepare_video.reset();
		OS_AsynUnityAdsRewardedVideoPrepare(&_svc_prepare_video);
		PDEBUG("unity video ads prepare\n");
	}
}

void PineUnityAdsAsync::prepareSkippableVideo()
{
	PDEBUG("call prepare unity ads video skippable\n");
	if (_svc_prepare_skippable_video.Status != PineServiceResult::WaitingResponse) {
		_svc_prepare_skippable_video.reset();
		OS_AsynUnityAdsSkippableVideoPrepare(&_svc_prepare_skippable_video);
		PDEBUG("unity video ads prepare skippable\n");
	}
}

void PineUnityAdsAsync::showVideo()
{
	PDEBUG("call unity ads video 0\n");
	if(isVideoReady())
	{
		PDEBUG("call unity ads video\n");
		_svc_show_video.reset();
		OS_AsynUnityAdsRewardedVideoShow(&_svc_show_video);
	}
}

void PineUnityAdsAsync::showSkippableVideo()
{
	PDEBUG("call unity ads video skippable\n");
	if (isVideoSkippalbeReady())
	{
		_svc_show_skippable_video.reset();
		OS_AsynUnityAdsSkippableVideoShow(&_svc_show_skippable_video);
	}
}