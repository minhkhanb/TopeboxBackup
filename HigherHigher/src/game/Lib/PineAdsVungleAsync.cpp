#include "pch.h"
#include "PineAdsVungleAsync.h"

PineAdsVungleAsync::PineAdsVungleAsync(){
	_svc_prepare.reset();

}
void PineAdsVungleAsync::setUnitId(const char* ad_unit_id){
	sprintf(_curr_unit_id,"%s",ad_unit_id); 
}
PineAdsVungleAsync::enumVungleAdsUpdateResult PineAdsVungleAsync::update(){
	if (_svc_prepare.Status == PineServiceResult::WaitingResponse){
		OS_AsynVungleAdsPrepare(&_svc_prepare/*, false, _curr_unit_id*/);
		if (_svc_prepare.isFinish()){
		//	showInterstitial();
			PDEBUG("vungle ads prepare finish. _svc_prepare.ResponseStatus = %d\n", _svc_prepare.ResponseStatus);
		}
	}
	
	if(_svc_show.Status == PineServiceResult::WaitingResponse)
	{
		OS_AsynVungleAdsShow(&_svc_show);
		if(_svc_show.Status == PineServiceResult::Responsed)
		{
			prepare();
			if (_svc_show.ResponseStatus == 0)
			{
				PDEBUG("Show vungle video ads successful!");
				_svc_show.reset();
				return enumVungleAdsUpdateResult::SHOW_VIDEO_SUCCESS;	
			}
			else
			{
				PDEBUG("Show vungle video ads fail!");
				_svc_show.reset();
				return enumVungleAdsUpdateResult::SHOW_VIDEO_FAIL;
			}
			//prepare();
			//_svc_show.reset();
		}
	}
	return enumVungleAdsUpdateResult::NOTHING;
}
void PineAdsVungleAsync::prepare(){
	
	if (_svc_prepare.Status != PineServiceResult::WaitingResponse){
		_svc_prepare.reset();
		OS_AsynVungleAdsPrepare(&_svc_prepare/*, false, _curr_unit_id*/);
		PDEBUG("vungle ads prepare\n");
	}
}

void PineAdsVungleAsync::show()
{	
	if (_svc_prepare.Status == PineServiceResult::Responsed && _svc_prepare.ResponseStatus == 0){
		_svc_show.reset();
		OS_AsynVungleAdsShow(&_svc_show);
		PDEBUG("CALL VUNGLE ADS SHOW");
	}else{
		PDEBUG("Some BUG");
	}
}
bool PineAdsVungleAsync::isReady(){
	bool isReady = _svc_prepare.Status == PineServiceResult::Responsed && _svc_prepare.ResponseStatus == 0;
	if (!isReady && _svc_prepare.Status == PineServiceResult::Responsed)
	{
		prepare();
	}
	return isReady;
}