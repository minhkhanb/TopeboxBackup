#define SAFE_DEL(a)					{if(a){delete (a);(a) = NULL;}}
#define SAFE_DEL_ARRAY(a)			{if(a){delete[] (a);(a) = NULL;}}
#define SAFE_DEL_ARRAY_TYPE(a, t)	{if((t)a){delete[] ((t)(a));(a) = NULL;}}
#define SAFE_DEL_ARRAY_OBJ(p, n)	{if (p) {for (int __i = 0; __i < (n); __i++) SAFE_DEL((p)[__i]); SAFE_DEL_ARRAY(p);}}
#define SAFE_DEL_ARRAY_ARRAY(p, n)	{if (p) {for (int __i = 0; __i < (n); __i++) SAFE_DEL_ARRAY((p)[__i]); SAFE_DEL_ARRAY(p);}}

#define URL_SERVER  "http://odata.pine-entertainment.com/api/youarmy/"
#define MY_SERCET_PASSWORD  @"pineEntertainment"

#define CHECK_POINT_IN_RECT(X1,Y1,X,Y,W,H) ((X1)>= (X) && (X1) <= (X)+(W) && (Y1)>=(Y) && (Y1) <= (Y)+(H))
#define CHECK_POINT_IN_CIRCLE(X, Y, CX, CY, RAD) ((X-CX)*(X-CX) + (Y-CY)*(Y-CY) <(RAD)*(RAD))
#define _DD_(X1, Y1, X2, Y2) (((X1)-(X2))*((X1)-(X2)) + ((Y1)-(Y2))*((Y1)-(Y2)))
#define _DD2_(vx,vy)		((vx)*(vx) + (vy)*(vy))

#define DT()			(CGame::_this->_frame_dt)
#ifdef _WINDOWS_PHONE8
#define G()	 ((GLESLib*)CGame::_this->_g)
#else
#define G()	 (CGame::_this->_g)
#endif
#define TOUCHS(id) (CGame::_this->_touches[id])
#define GET_SPRITE(id) (CGame::_this->GetSpriteVer(id))
#define SET_ASPRITE(anim,id)	(anim.SetAnimSprite(CGame::_this->GetSpriteVer(id)))
#define GAME()	(CGame::_this)
#define BG()	(CGame::_this->_bg)
#define ROBOT()	(CGame::_this->_main_character)
#define CAMX()	(CGame::_this->_camera_x)
#define CAMY()	(CGame::_this->_camera_y)
#define SET_CAMX(a) (CGame::_this->_camera_x = (a))
#define SET_CAMY(a) (CGame::_this->_camera_y = (a))
#define OBS(id)		(CGame::_this->_obstacles[id])
#define MSIN(a)	(CGame::_this->Math_Sin(a))
#define MCOS(a) (CGame::_this->Math_Cos(a))
#define COINS()	(CGame::_this->_coin)
#define SCORES()	(CGame::_this->_score)
#define LEVEL_POWER(i)	(CGame::_this->_main_character._power_up_level[i])
#define BULLETS(i)		(CGame::_this->_bullet[i])
#define ITEMS(i)		(CGame::_this->_item[i])

#define PLAY_SFX(id)	(CGame::_this->PlaySFX(id))
#define FRAME_COUNTER()	(CGame::_this->_frame_counter)

#define DEVICE_VERSION    (CGame::_this->_current_version)
#define SETSCALE_IPAD(c,d)  if(CGame::_this->_current_version == VERSION_IPAD){G()->SetScale(768/640.0f,1024/960.0f,(c),(d));}
#define IPAD_SCALEX     ((CGame::_this->_current_version == VERSION_IPAD)?(768/640.0f):1)
#define IPAD_SCALEY     ((CGame::_this->_current_version == VERSION_IPAD)?(1024/960.0f):1)

#define ADVERT()				(CGame::_this->_advert)
#define GET_PINE_HIGH_SCORE()	(GAME()->_pineHighScore)