#if !defined(_H_UNITY_ADS_)
#define _H_UNITY_ADS_
#include "DataType/CType.h"


class PineUnityAdsAsync{
public:
	PineServiceHandle _svc_check_video_, _svc_check_rewardvideo_;
	bool _is_videoreward_cached;
	bool _is_video_cached;


	PineUnityAdsAsync();
	void cacheRewardVideo();
	void update();
};
#endif //_H_UNITY_ADS_