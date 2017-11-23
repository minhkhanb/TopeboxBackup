#define DESIGN_PARAM_SPEED_START			0
#define DESIGN_PARAM_SPEED_INCREASE			1
#define DESIGN_PARAM_SPEED_MAX				2
#define DESIGN_PARAM_COMBO_TIME				3

#define GAME_DEBUG_UNDIE			0

#define TOUCH_TIME_COUNTDOWN        30
#define TOUCH_TIME_HOLD             5

#define SIZE_CHAR_WIDTH				38
#define SIZE_CHAR_HEIGHT			38

#define SIZE_FLATFORM_WIDTH				186
#define SIZE_FLATFORM_HEIGHT			450
#define SIZE_FLATFORM_HEADER_HEIGHT		5

#define OBJECT_ENVIR_TREES				10
#define OBJECT_ENVIR_NUM				5

#define COLOR_NUM						6

#define POSX(a)					()
#define OFF_PORT				(CMath::ABSOLUTE_VALUE(CGame::_this->_pointLeftTop.X))
#define SCALE_PORT				(CGame::_this->_game_context.ScaleOut)

#define TIME_DELAY_SHOW_TOPEBOX_ADS			8*60*60;
#define TW_PAGE_ENABLE						0
#define TIME_DELAY_SHOW_ADS					1.5f*60
#if defined (ANDROID_OS)
#define	ADD_GOOGLE_ANALYTICS		1
#endif
#define MENU_GGPlay_HEIGHT	(100)