#if !defined(_H_CHARTBOOST_)
#define _H_CHARTBOOST_
#include "DataType/CType.h"


class PineChartBoostAsync{
public:
	PineServiceHandle _svc_interstitial_prepare,_svc_rewarded_video_prepare,_svc_interstitial_show,_svc_rewarded_video_show;

	PineChartBoostAsync();
	void update();
	
	void prepareInterstitial();
	void prepareRewardedVideo();
	
	
	bool hasInterstitial();
	bool hasRewardedVideo();
	
	void showInterstitial();
	void showRewardedVideo();
};
#endif //_H_CHARTBOOST_