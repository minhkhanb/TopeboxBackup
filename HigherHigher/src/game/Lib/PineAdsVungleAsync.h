#if !defined(_H_VUNGLE_MANAGER)
#define _H_VUNGLE_MANAGER
#include "DataType/CType.h"
#include "../GameCore/Os_Functions.h"

class PineAdsVungleAsync{
public:
	ENUM enumVungleAdsUpdateResult {
		NOTHING = 0,
		PREPARE_ADS_SUCCESS,
		PREPARE_ADS_FAIL,
		PREPARE_VIDEO_SUCCESS,
		PREPARE_VIDEO_FAIL,
		SHOW_ADS_SUCCESS,
		SHOW_ADS_FAIL,
		SHOW_VIDEO_SUCCESS,
		SHOW_VIDEO_FAIL,
	};
private:
	PineServiceHandle _svc_prepare, _svc_show;
	bool _is_request_prepare;
	char _curr_unit_id [255];
public :
	PineAdsVungleAsync();
	void setUnitId(const char* ad_unit_id);
	PineAdsVungleAsync::enumVungleAdsUpdateResult update();
	void prepare();
	void show();
	bool isReady();
};
#endif