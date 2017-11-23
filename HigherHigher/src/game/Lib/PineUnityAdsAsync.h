#if !defined(_H_UNITY_ADS_)
#define _H_UNITY_ADS_
#include "DataType/CType.h"


class PineUnityAdsAsync{
public:
	PineServiceHandle _svc_prepare_ads, _svc_prepare_video, _svc_prepare_skippable_video;
	PineServiceHandle _svc_show_ads, _svc_show_video, _svc_show_skippable_video;

	ENUM enumUnityAdsUpdateResult {
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

	PineUnityAdsAsync();
	
	bool isAdsReady();
	void prepareAds();
	void showAds();
	
	bool isVideoReady();
	bool isVideoSkippalbeReady();
	void prepareVideo();
	void prepareSkippableVideo();
	void showVideo();
	void showSkippableVideo();
	
	enumUnityAdsUpdateResult update();
};
#endif //_H_UNITY_ADS_