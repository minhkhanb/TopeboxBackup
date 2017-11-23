#include "pch.h"
#include "PineAdmobAsync.h"

extern "C" void OS_AsynAdmobInterstitialPrepare(PineServiceHandle* svc, bool auto_show, const char* ad_id);
extern "C" void OS_AsynAdmobInterstitialShow(PineServiceHandle* hsvc);

extern "C" void OS_AsynAdmobBannerPrepare(PineServiceHandle* svc, bool auto_show, const char* ad_id);
extern "C" void OS_AsynAdmobBannerShow(PineServiceHandle* hsvc);
extern "C" void OS_AsynAdmobBannerHide(PineServiceHandle* hsvc);

extern "C" void OS_AsynAdmobRewardedVideoPrepare(PineServiceHandle* svc);
extern "C" void OS_AsynAdmobRewardedVideoShow(PineServiceHandle* hsvc);

PineAdmobAsync::PineAdmobAsync(){
	_svc_interstitial_prepare.reset();

}

void PineAdmobAsync::setUnitId(const char* ad_unit_id){
	sprintf(_curr_unit_id,"%s",ad_unit_id); 
}

PineAdmobAsync::enumAdmobUpdateResult PineAdmobAsync::update(){
	if (_svc_interstitial_prepare.Status == PineServiceResult::WaitingResponse){
		OS_AsynAdmobInterstitialPrepare(&_svc_interstitial_prepare, false, _curr_unit_id);
		if(_svc_interstitial_prepare.Status == PineServiceResult::Responsed)
		{			
			if (_svc_interstitial_prepare.ResponseStatus == 0)
			{
				PDEBUG("prepare admob ads successful!");
			}
			else
			{
				PDEBUG("prepare admob ads fail!");
			}
			//_svc_interstitial_prepare.reset();
		}
	}
	
	if(_svc_interstitial_show.Status == PineServiceResult::WaitingResponse)
	{
		OS_AsynAdmobInterstitialShow(&_svc_interstitial_show);
		if(_svc_interstitial_show.Status == PineServiceResult::Responsed)
		{	
			prepareInterstitial();	
			if (_svc_interstitial_show.ResponseStatus == 0)
			{
				PDEBUG("Show admob ads successful!");
				_svc_interstitial_show.reset();
				return enumAdmobUpdateResult::SHOW_VIDEO_SUCCESS;	
			}
			else
			{
				PDEBUG("Show admob ads fail!");
				_svc_interstitial_show.reset();
				return enumAdmobUpdateResult::SHOW_VIDEO_FAIL;
			}
		}
	}

	if (_svc_banner_prepare.Status == PineServiceResult::WaitingResponse) {
		OS_AsynAdmobBannerPrepare(&_svc_banner_prepare, false, _curr_unit_id);
		if (_svc_banner_prepare.Status == PineServiceResult::Responsed)
		{
			if (_svc_banner_prepare.ResponseStatus == 0)
			{
				PDEBUG("prepare admob banner successful!");
			}
			else
			{
				PDEBUG("prepare admob banner fail!");
			}
			//_svc_interstitial_prepare.reset();
		}
	}

	if (_svc_banner_show.Status == PineServiceResult::WaitingResponse)
	{
		OS_AsynAdmobBannerShow(&_svc_banner_show);
		if (_svc_banner_show.Status == PineServiceResult::Responsed)
		{
			if (_svc_banner_show.ResponseStatus == 0)
			{
				PDEBUG("Show admob banner successful!");
				return enumAdmobUpdateResult::SHOW_VIDEO_SUCCESS;
			}
			else
			{
				PDEBUG("Show admob banner fail!");
				_svc_banner_show.reset();
				return enumAdmobUpdateResult::SHOW_VIDEO_FAIL;
			}
		}
	}
	if (_svc_banner_hide.Status == PineServiceResult::WaitingResponse)
	{
		OS_AsynAdmobBannerHide(&_svc_banner_hide);
		if (_svc_banner_hide.Status == PineServiceResult::Responsed)
		{
			if (_svc_banner_hide.ResponseStatus == 0)
			{
				PDEBUG("Show admob banner successful!");
				_svc_banner_show.reset();
				return enumAdmobUpdateResult::SHOW_VIDEO_SUCCESS;
			}
			else
			{
				PDEBUG("Show admob banner fail!");
				_svc_banner_hide.reset();
				return enumAdmobUpdateResult::SHOW_VIDEO_FAIL;
			}
		}
	}
	
	if (_svc_rewarded_video_prepare.Status == PineServiceResult::WaitingResponse) {
		OS_AsynAdmobRewardedVideoPrepare(&_svc_rewarded_video_prepare);
		if (_svc_rewarded_video_prepare.Status == PineServiceResult::Responsed)
		{
			if (_svc_rewarded_video_prepare.ResponseStatus == 0)
			{
				PDEBUG("prepare admob rewarded video successful!");
			}
			else
			{
				//PDEBUG("prepare admob rewarded video fail!");
				if (_svc_rewarded_video_prepare.ResponseStatus == 1)
				{
					prepareRewardedVideo();
				}
			}
		}
	}

	if (_svc_rewarded_video_show.Status == PineServiceResult::WaitingResponse)
	{
		OS_AsynAdmobRewardedVideoShow(&_svc_rewarded_video_show);
		if (_svc_rewarded_video_show.Status == PineServiceResult::Responsed)
		{
			prepareRewardedVideo();
			if (_svc_rewarded_video_show.ResponseStatus == 0)
			{
				PDEBUG("Show admob rewarded video successful!");
				return enumAdmobUpdateResult::SHOW_VIDEO_SUCCESS;
			}
			else
			{
				PDEBUG("Show admob rewarded video fail!");
				_svc_rewarded_video_show.reset();
				return enumAdmobUpdateResult::SHOW_VIDEO_FAIL;
			}
		}
	}
	return enumAdmobUpdateResult::NOTHING;
}
void PineAdmobAsync::prepareInterstitial(){
	if (_svc_interstitial_prepare.Status == PineServiceResult::Responsed && _svc_interstitial_prepare.ResponseStatus == 2) //Turn off in java code
	{
		return;
	}
	if (_svc_interstitial_prepare.Status != PineServiceResult::WaitingResponse){
		_svc_interstitial_prepare.reset();
		OS_AsynAdmobInterstitialPrepare(&_svc_interstitial_prepare, false, _curr_unit_id);
		PDEBUG("admod prepare\n");
	}
}

void PineAdmobAsync::showInterstitial()
{	
	if (_svc_interstitial_prepare.Status == PineServiceResult::Responsed && _svc_interstitial_prepare.ResponseStatus == 0){
		_svc_interstitial_show.reset();
		OS_AsynAdmobInterstitialShow(&_svc_interstitial_show);
		PDEBUG("CALL SHOW");
	}else{
		PDEBUG("Some BUG");
	}
}

bool PineAdmobAsync::isInterstitialReady() {
	if (_svc_interstitial_prepare.Status == PineServiceResult::Responsed && _svc_interstitial_prepare.ResponseStatus == 0)
	{
		return true;
	}
	else
	{
		prepareInterstitial();
		return false;
	}
}

void PineAdmobAsync::prepareBanner() {
	if (_svc_banner_prepare.Status == PineServiceResult::Responsed && _svc_banner_prepare.ResponseStatus == 2) //Turn off in java code
	{
		return;
	}
	if (_svc_banner_prepare.Status != PineServiceResult::WaitingResponse) {
		_svc_banner_prepare.reset();
		OS_AsynAdmobBannerPrepare(&_svc_banner_prepare, false, _curr_unit_id);
		PDEBUG("admod banner prepare\n");
	}
}

void PineAdmobAsync::showBanner()
{
	if (_svc_banner_prepare.Status == PineServiceResult::Responsed && _svc_banner_prepare.ResponseStatus == 0) {
		_svc_banner_show.reset();
		OS_AsynAdmobBannerShow(&_svc_banner_show);
		PDEBUG("CALL SHOW BANNER");
	}
	else {
		PDEBUG("SHOW BANNER ERROR");
	}
}

void PineAdmobAsync::hideBanner()
{
	if (_svc_banner_show.Status == PineServiceResult::Responsed && _svc_banner_show.ResponseStatus == 0) {
		PDEBUG("HIDE BANNER");
		_svc_banner_hide.reset();
		OS_AsynAdmobBannerHide(&_svc_banner_hide);
	}
	else {		
		PDEBUG("HIDE BANNER ERROR");
	}
}

bool PineAdmobAsync::isShowBanner() {
	if (_svc_banner_show.Status == PineServiceResult::Responsed && _svc_banner_show.ResponseStatus == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PineAdmobAsync::isBannerReady() {
	if (_svc_banner_prepare.Status == PineServiceResult::Responsed && _svc_banner_prepare.ResponseStatus == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PineAdmobAsync::prepareRewardedVideo() {
	if (_svc_rewarded_video_prepare.Status == PineServiceResult::Responsed && _svc_rewarded_video_prepare.ResponseStatus == 2) //Turn off in java code
	{
		return;
	}
	if (_svc_rewarded_video_prepare.Status != PineServiceResult::WaitingResponse) {
		_svc_rewarded_video_prepare.reset();
		OS_AsynAdmobRewardedVideoPrepare(&_svc_rewarded_video_prepare);
		//PDEBUG("admod rewarded video prepare\n");
	}
}

void PineAdmobAsync::showRewardedVideo()
{
	if (_svc_rewarded_video_prepare.Status == PineServiceResult::Responsed && _svc_rewarded_video_prepare.ResponseStatus == 0) {
		_svc_rewarded_video_show.reset();
		OS_AsynAdmobRewardedVideoShow(&_svc_rewarded_video_show);
		PDEBUG("CALL SHOW REWARDED VIDEO");
	}
	else {
		PDEBUG("SHOW REWARDED VIDEO ERROR");
	}
}

bool PineAdmobAsync::isRewardedVideoReady() {
	if (_svc_rewarded_video_prepare.Status == PineServiceResult::Responsed && _svc_rewarded_video_prepare.ResponseStatus == 0)
	{
		return true;
	}
	else
	{
		prepareRewardedVideo();
		return false;
	}
}



bool PineAdmobAsync::isTurnoff(){
	if (_svc_interstitial_prepare.Status == PineServiceResult::Responsed && _svc_interstitial_prepare.ResponseStatus == 2)
	{
		return true;
	}
	else
	{
		return false;
	}	
}
