#define k_SCREEN_WIDTH				(CGame::_this->GetScreenWidth())
#define k_SCREEN_HEIGHT				(CGame::_this->GetScreenHeight())
#define k_DEVICE_WIDTH				(CGame::_this->GetScreenWidth())
#define k_DEVICE_HEIGHT				(CGame::_this->GetScreenHeight())

#define ORIENTATION_INVALID				(-1)
#define ORIENTATION_PORTRAIT			(1 << 0)
#define ORIENTATION_LANDSCAPE_90		(1 << 1)
#define ORIENTATION_LANDSCAPE_270		(1 << 2)

#define ORIENTATION_DEFAULT				ORIENTATION_PORTRAIT


#define	GET_SCALE()						(CGame::_this->GetScale())
#define GET_SCALEX()					(CGame::_this->GetScaleX())
#define GET_SCALEY()					(CGame::_this->GetScaleY())
