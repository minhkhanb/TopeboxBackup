#include "pch.h"
#include "../GameCore/CGame.h"
#include "../GameCore/Os_Functions.h"
#include <stdio.h>
extern "C" void ServerADVERTStart();
extern "C" void PurchaseProduct(const char *indentify);
extern "C" bool isChartboostAvailable();
void CGame::OnScreenChange(PineSize size)
{
	///////////////////////////////////////////
	// INIT CONTEXT
	///////////////////////////////////////////

	//define support version
#if defined(_WINDOWS_PHONE8)
	ResVersion support_version[] = {
		{ SupportVersion::RESVER_1280_768, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_800_480, Orientation::Landscape_90 }
	};
	int support_version_num = 2;
#elif defined(_WIN8)
	ResVersion support_version[] = {
		{ SupportVersion::RESVER_1366_768, Orientation::Landscape_90 }
	};
	int support_version_num = 1;
#elif defined(MAC_OS)
	ResVersion support_version[] = {
		/*{ SupportVersion::RESVER_960_640, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_1024_768, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_1136_640, Orientation::Landscape_90 },*/
		{ SupportVersion::RESVER_1334_750, Orientation::Portrait }
	};
	int support_version_num = 1;
#elif defined(ANDROID_OS)
	ResVersion support_version[] = {
		/*{ SupportVersion::RESVER_960_640, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_1024_768, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_1136_640, Orientation::Landscape_90 },*/
		{ SupportVersion::RESVER_1334_750, Orientation::Portrait }
	};
	int support_version_num = 1;
#else //windows
	ResVersion support_version[] = {
		//{ SupportVersion::RESVER_1280_768, Orientation::Landscape_90 },
		//{ SupportVersion::RESVER_800_480, Orientation::Landscape_90 },
		//{ SupportVersion::RESVER_1366_768, Orientation::Landscape_90 },
		//{ SupportVersion::RESVER_960_640, Orientation::Landscape_90 },
		//{ SupportVersion::RESVER_1024_768, Orientation::Portrait },
		//{ SupportVersion::RESVER_1136_640, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_1334_750, Orientation::Portrait }
	};
	int support_version_num = 1;
#endif
	//detect device version
	if (support_version_num > 0)
	{
		PineSize dvsize = { _device.ScreenSize.W, _device.ScreenSize.H };
		ResVersion curr_ver = GetAppropriateVersion(dvsize, support_version, support_version_num);
		Alignment game_align = { HorizontalAlign::Center, VerticalAlign::Middle };
		AlignmentContext(game_align, dvsize, curr_ver, true, _HCENTER);
		_current_version = VERSION_750x1334;
	}
#if defined(MAC_OS)
	{
		ResVersion device_support_version[] = {
			{ SupportVersion::RESVER_960_640, Orientation::Portrait },
			{ SupportVersion::RESVER_1024_768, Orientation::Portrait },
			{ SupportVersion::RESVER_1136_640, Orientation::Portrait },
			{ SupportVersion::RESVER_800_480, Orientation::Portrait },
			{ SupportVersion::RESVER_1280_768, Orientation::Portrait },
		};
		PineSize dvsize = { _device.ScreenSize.W, _device.ScreenSize.H };

		_device._version = (GetAppropriateVersion(dvsize, device_support_version, 5)).Version;
	}
#endif
	//port game
	_pointLeftTop = _game_context.MeasureAnchor(0, 0, ANCHOR_LEFT | ANCHOR_TOP);
	//printf("\n _pointLeftTop.X: %f, _pointLeftTop.Y: %f", _pointLeftTop.X, _pointLeftTop.Y);
	//printf("\n ScaleOutX: %f, ScaleOutY: %f, scaleX: %f, scaleY: %f, ScaleOut: %f, TranslateX: %f, TranslateY: %f", GAME()->_game_context.ScaleOutX, GAME()->_game_context.ScaleOutY, GAME()->_game_context.ScaleX, GAME()->_game_context.ScaleY, GAME()->_game_context.ScaleOut, GAME()->_game_context.TranslateX, GAME()->_game_context.TranslateY);

	PinePoint poin_x = _game_context.MeasureAnchor(k_SCREEN_WIDTH, 0);
	//printf("\n poin_x.X: %f, poin_x.Y: %f", poin_x.X, poin_x.Y);
}
void CGame::InitEx()
{
	InitBuff(UNZIP_BUFFER_LENGTH_HIGH);
	_main_game = new HSGame();
	_main_game->Init();
	_main_game->SetState(k_GS_LOGO);
	_main_game->_game_id = APPLE_ID;
	_current_game = _main_game;
	HSGame*game = (HSGame*)(GAME()->_current_game);
	_icloudStatus = 0;
	_gcSocial = NULL;
#ifdef MAC_OS
#if ADD_ADVERT
	ServerADVERTStart();
#endif
#endif
#if !defined (MAC_OS)
	CGame::_this->_isBackkeyPress = false;
#endif
	GAME()->InitAds();
	_banner_ads_scrolly = 0;
}

void CGame::ReleaseEx()
{

}

void CGame::PauseEx()
{
	HSGame *game = (HSGame*)(GAME()->_current_game);
	game->PushNotify();
	game->SetPause();
}
void CGame::ResumeEx()
{
	HSGame *game = (HSGame*)(GAME()->_current_game);
	if (game->_show_ads_interrupt && game->_current_state != k_GS_LOGO)
	{
		game->_count_frame_show_ads = 30;
		game->_show_ads_interrupt = false;
	}
	if (!game->_show_ads_interrupt)
	{
		GAME()->isClosedInterstitialAd = false;
		game->_show_ads_interrupt = true;
	}
	game->SetResume();
}
bool CGame::OnBackKeyPress() {
#if !defined (MAC_OS)
	CGame::_this->_isBackkeyPress = true;
	CGame::_this->_waitForProcessBackkeyCount = 0;
	return true;
#else
	return false;
#endif
}
void CGame::UpdateAndRender()
{
	G()->ClearClip();
	G()->SetOpacity(100);
	if (!LoadGameForceground())
	{
		return;
	}
	_main_game->UpdateAndRender();
	UpdateAds();
#if defined (ANDROID_OS)
	_async_task_manager.Run();
#endif
}
void CGame::UpdateTouch(int xx, int yy, int touch_state, int touch_id, int tap_count)
{
	/*switch (_GameState)
	{

	}*/
	GAME()->isClosedInterstitialAd = true;
}



void CGame::PurchaseIAP(const char* strID, enumIAPResponseStatus sucessed)
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
	if (sucessed == enumIAPResponseStatus::IAP_RESPONSE_STATUS_SUCCESS
		|| sucessed == enumIAPResponseStatus::IAP_RESPONSE_STATUS_SUCCESS_RESTORE)
	{
		if (strcmp(strID, "HH_NoAds") == 0)
		{
			game->_user._hadRemovedAds = true;
			game->_user.UserDataSave();
			for (int i = 0; i < CHAR_SPRITEID_NUM; i++)
			{
				game->_char_select[i]._is_lock = false;
			}
		}
#if ANDROID_OS
		if (strcmp(strID, "hh_no_ads_1_99") == 0)
		{
			game->_user._hadRemovedAds = true;
			game->_user.UserDataSave();
			for (int i = 0; i < CHAR_SPRITEID_NUM; i++)
			{
				game->_char_select[i]._is_lock = false;
			}
		}
#endif
	}

	game->_isWaiting = false;
}

#if defined(_WINDOWS)
void CGame::ProcessCommandLine(const char* cmd)
{

}
#endif
void CGame::ProcessingTag(int tag, int indexButton)
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
	switch (tag)
	{
	case ALERT_TAG_CONFRIM_SYNC_ICOUND:
		if (indexButton == 1)//touch ok
		{
			game->_user._isUseIcloud = CUserData::enumUseIcloud::ICLOUD_USE;
			game->_user.UserDataSave();
		}
		else if (indexButton == 0)//cancel
		{
			game->_user._isUseIcloud = CUserData::enumUseIcloud::ICLOUD_DONT_USE;
			game->_user.UserDataSave();
		}
		break;

	case ALERT_TAG_CONFRIM_ICOUND_LOAD_SAVE:
		if (indexButton == 1)//touch ok
		{
			game->_user._isUseIcloud = CUserData::enumUseIcloud::ICLOUD_USE;
			game->_user.LoadFromIcloud();
			game->_user.UserDataSave();
			if (game->_current_state > k_GS_LOGO)
			{
				game->SetState(k_GS_LOGO);
			}
		}
		break;

	case ALERT_TAG_CONFRIM_ICOUND_SYNC_COMPLETE:
		if (indexButton == 0)//touch ok
		{
			//_user.LoadFromICloud();
			//_user.DataSave();
		}
		break;
	}
	game->_isWaiting = false;
}


bool CGame::InitAds()
{
#if defined (ANDROID_OS)
	PDEBUG("INIT ADS");
	_admob_manager.setUnitId("DONT_NEED");
	_admob_manager.prepareInterstitial();
	_admob_manager.prepareBanner();
	//_admob_manager.prepareRewardedVideo();
	//_unityads_manager.prepareAds();
	//_unityads_manager.prepareVideo();
	//_adsVungle_manager.prepare();
	//_unityads_manager.prepareSkippableVideo();
	_on_show_video_ads = false;
	_admob_play_count = 0;
	_time_count_for_show_ads_banner = 0;
	return true;
#endif
	return false;
}
void CGame::ShowAds()
{
	HSGame* game = (HSGame*)(GAME()->_current_game);
#if defined (ANDROID_OS)
	PDEBUG("\n call ShowAds");
	//_on_show_video_ads = true;
	//_is_waiting_for_reward = true;
	//_on_video_ads_reward = false;
	/*if (_unityads_manager.isVideoReady())
	{
	PDEBUG("SHOW UNITY VIDEO");
	_unityads_manager.showVideo();
	_admob_play_count = 0;
	}
	else if (_adsVungle_manager.isReady())
	{
	PDEBUG("SHOW VUNGLE ADS");
	_admob_play_count = 0;
	_adsVungle_manager.show();
	}
	else */if (_admob_manager.isInterstitialReady())
	{
		PDEBUG("SHOW _admob showInterstitial");
		_admob_manager.showInterstitial();
	}
#endif
	
}

void CGame::ShowBannerAds()
{
#if defined (ANDROID_OS)
	PDEBUG(" call _admob banner: %d %d", _admob_manager.isBannerReady(), _admob_manager.isShowBanner());
	if (_admob_manager.isBannerReady() && !_admob_manager.isShowBanner())
	{
		PDEBUG("SHOW _admob banner");
		_admob_manager.showBanner();
		_has_show_ads_banner = true;
	}
	else {
		_time_count_for_show_ads_banner = 2 * FPS_MAX;
	}
#endif
}
void CGame::HideBannerAds()
{
#if defined (ANDROID_OS)
	_has_show_ads_banner = false;
	_admob_manager.hideBanner();
#endif
}
void CGame::UpdateAds()
{
#if defined (ANDROID_OS)
	if (OS_IsNetworkAvailable())
	{
		//Admob
		PineAdmobAsync::enumAdmobUpdateResult result = _admob_manager.update();
		//PDEBUG("\n enumAdmobUpdateResult: %d", result);
		switch (result)
		{
		case PineAdmobAsync::enumAdmobUpdateResult::SHOW_VIDEO_SUCCESS:
			break;
		case PineAdmobAsync::enumAdmobUpdateResult::SHOW_VIDEO_FAIL:
			break;
		default:
			break;
		}
	}
#endif
}
void CGame::ResetStateBannerAds() {
	if (!_has_show_ads_banner)
	{
		if (_banner_ads_scrolly > 0)
		{
			_banner_ads_scrolly += -5;
			if (_banner_ads_scrolly <= 0)
			{
				_banner_ads_scrolly = 0;
			}
		}
	}
	else {
		//PDEBUG("\nGAME()->reduceY: %f", GAME()->reduceY);
		if (_banner_ads_scrolly < GAME()->reduceY)
		{
			_banner_ads_scrolly += 5;
			if (_banner_ads_scrolly >= GAME()->reduceY)
			{
				_banner_ads_scrolly = GAME()->reduceY;
			}
		}
	}
}

