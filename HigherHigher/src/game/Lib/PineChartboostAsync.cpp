#include "pch.h"
#include "PineChartboostAsync.h"
#include "../GameCore/OS_Functions.h"

PineChartBoostAsync::PineChartBoostAsync()
{
	_svc_interstitial_prepare.reset();
	_svc_rewarded_video_prepare.reset();

}

void PineChartBoostAsync::prepareInterstitial()
{

	//OS_AsynCBInterstitialPrepare
	if(_svc_interstitial_prepare.Status != PineServiceResult::WaitingResponse)
	{
		_svc_interstitial_prepare.reset();
		PDEBUG("prepare chartboost");
		OS_AsynCBInterstitialPrepare(&_svc_interstitial_prepare);
	}
}

void PineChartBoostAsync::prepareRewardedVideo()
{

	if(_svc_rewarded_video_prepare.Status != PineServiceResult::WaitingResponse)
	{

		_svc_rewarded_video_prepare.reset();
		OS_AsynCBRewardedVideoPrepare(&_svc_rewarded_video_prepare);
	}
}

void PineChartBoostAsync::update()
{

	if (_svc_interstitial_prepare.Status == PineServiceResult::WaitingResponse)
	{
		OS_AsynCBInterstitialPrepare(&_svc_interstitial_prepare);
		
		if(_svc_interstitial_prepare.Status == PineServiceResult::Responsed)
		{
			PDEBUG("prepare chartboost finish");
		}
	}


	if (_svc_rewarded_video_prepare.Status == PineServiceResult::WaitingResponse)
	{
		OS_AsynCBRewardedVideoPrepare(&_svc_rewarded_video_prepare);
	}

	
	if (_svc_interstitial_show.Status == PineServiceResult::WaitingResponse)
	{
		OS_AsynCBInterstitialShow(&_svc_interstitial_show);
		if(_svc_interstitial_show.Status == PineServiceResult::Responsed)
		{
			prepareInterstitial();
		}
	}



	if (_svc_rewarded_video_show.Status == PineServiceResult::WaitingResponse)
	{
		OS_AsynCBRewardedVideoShow(&_svc_rewarded_video_show);
		if(_svc_rewarded_video_show.Status == PineServiceResult::Responsed)
		{
			prepareRewardedVideo();
		}
	}
	
}

bool PineChartBoostAsync::hasInterstitial()
{

	return (_svc_interstitial_prepare.Status == PineServiceResult::Responsed && _svc_interstitial_prepare.ResponseStatus == 0);
}

bool PineChartBoostAsync::hasRewardedVideo()
{
	return (_svc_rewarded_video_prepare.Status == PineServiceResult::Responsed && _svc_rewarded_video_prepare.ResponseStatus == 0);
}

void PineChartBoostAsync::showInterstitial()
{
	_svc_interstitial_show.reset();
	OS_AsynCBInterstitialShow(&_svc_interstitial_show);

}
void PineChartBoostAsync::showRewardedVideo()
{
	_svc_rewarded_video_show.reset();
	OS_AsynCBRewardedVideoShow(&_svc_rewarded_video_show);
}