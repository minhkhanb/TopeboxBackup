//#ifndef MAC_OS
#include "pch.h"
#include "CGame.h"
#include "../GamePlay/ResourceManifest.cpp"
#include "Os_Functions.h"
#include "math.h"
#include "../Lib/DataType/BufferStream.h"
#include <thread>
#include <chrono>

#if !defined(_WINDOWS_PHONE8)
using namespace std;
PE_USING_ENGINE;
#endif

bool CGame::_is_portrail_exception = false;
extern "C" PineSize ResVersionSize[] = {
	{ 667, 375 },//0 - RESVER_667_375
	{ 800, 400 },//1 -	RESVER_800_400
	{ 800, 480 },//2 -	RESVER_800_480
	{ 800, 600 },//3 -	RESVER_800_600
	{ 854, 480 },//4 -	RESVER_854_480
	{ 960, 540 },//5 -	RESVER_960_540
	{ 960, 640 },//6 -	RESVER_960_640
	{ 1024, 600 },//7 - RESVER_1024_600
	{ 1024, 768 },//8 - RESVER_1024_768
	{ 1024, 800 },//9 - RESVER_1024_800
	{ 1136, 640 },//10 - RESVER_1136_640
	{ 1280, 768 },//11 - RESVER_1280_768
	{ 1280, 720 },//12 - RESVER_1280_720
	{ 1280, 800 },//13 - RESVER_1280_800
	{ 1366, 768 },//14 - RESVER_1366_768
	{ 1920, 1200 },//15 - RESVER_1920_1200
	{ 2560, 1600 },//16 - RESVER_2560_1600
	{ 2048, 1536 },//17 - RESVER_2048_1536

	//IOS 6
	{ 1334, 750 }, //18 - RESVER_1334_750

	//IOS 6 Plus
	{ 2208, 1242 }, //19 - RESVER_2208_1242
	{ 1000, 562 }, //20 - RESVER_1000_562
};

extern "C" int GetDeviceModel();
Device	CGame::_device;
int CGame::FPS_LIMIT = FPS_MAX;
UINT64 CGame::FPS_LIMIT_TIME = 1000000000 / FPS_MAX;

#ifdef MAC_OS
//extern "C" int GetPhoneOrientation();
extern "C" INT_TIME GetCurrentMilisecond();
extern "C" void OS_sleep(int iTime);
extern "C" void ForceExitGame();
#else
//int _sprite_id_will_free[TOTAL_SPRITE_INGAME];
//int _current_sprite_will_free;
 float  CGame::_windows_screen_scale = 1.0f;
#endif
void CGame::UpdateSpriteFree()
{
	if(_this->_GameSubState == k_LOOP && _current_sprite_will_free > 0)
	{
		UnloadSpriteVer(_sprite_id_will_free[_current_sprite_will_free-1]);
		_current_sprite_will_free--;
	}
}
void CGame::InitWillFree()
{
	_current_sprite_will_free  = 0;
}
void CGame::SpriteWillFree(int id)
{
	_sprite_id_will_free[_current_sprite_will_free] = id;
	_current_sprite_will_free++;
}

CGame*		CGame::_this = NULL;
UINT8*		CGame::_unzip_buff = NULL;
UINT32		CGame::_unzip_buff_length = 0;
CSprite**	CGame::_sprites = NULL;
UINT32		CGame::_touch_id_gen = 0;

CGame::CGame()
{
    
    _is_init = false;
	_this = this;
	_last_frame_time = 0;
	_run_time = 0;
	_game_millisecond = 0;
	_unzip_buff = NULL;
#ifdef MAC_OS
	_device.IsSupportAppleGameCenter = true;
#endif

	//Detect Graphic engine
#if ISGVER_GLES1
	_g = new GLESLib1();
#elif ISGVER_GLES2
#elif ISGVER_DIRECTX11
	_g = new GDirectX11();
#endif
	_sound_engine = NULL;
	
	_graphic_context.ComplexMode = PineGraphicComplexOrientation::ComplexNone;
	_reg_sprites_unload = NULL;
	_is_texture_scale = false;
	_GameState = 0;
	_GameSubState = k_INIT;
}

CGame::~CGame()
{
	
}

void CGame::Init()
{
#if !defined(_WINDOWS_PHONE8) && !defined (ANDROID_OS) && !defined(WIN32)
    _device_context = OS::getDevice(nullptr);
#endif
#if defined(MAC_OS)
	//_current_version = GetCurrentVersion();
#endif
	_step_force_loading = TOTAL_SPRITE_INGAME;
	_compete_backup_stock = false;
	for(int i = 0; i < TOTAL_SPRITE_INGAME; i++)
	{
		_sprite_loaded[i] = 0;
		_sprite_optimized_loaded[i] = 0;
		_sprite_antialias_loaded[i] = 0;
	}
	InitAnimStock();

	CMath::PRE_RANDOM();
	
	_isExit = false;
	_isPause = false;
	_output_buffer = NULL;
	_frame_counter = 0;
	
	//_input_stream = new ResourceInputStream();
	//_data_input_stream = new DataInputStream();
	_sprites = new CSprite*[NUM_ANIM_FILES];
	_reg_sprites_unload = new int[NUM_ANIM_FILES];
	_num_reg_sprite_unload = 0;
	for (int i = 0; i < NUM_ANIM_FILES; i++)
	{
		_sprites[i] = NULL;
	}
	
	/*#_texts = new UNICHAR**[NUM_TEXT_PACKAGES];
	_text_lens = new UINT16*[NUM_TEXT_PACKAGES];
	for (int i = 0; i < NUM_TEXT_PACKAGES; i++)
	{
		_texts[i] = NULL;
		_text_lens[i] = NULL;
	}*/
	
	_device_orientation = ORIENTATION_DEFAULT;
	
	
	_g->Init();

	//init touch
	_touches = new CTouch*[MAX_TOUCHES];
	for (int i = 0; i < MAX_TOUCHES; i++)
	{
		_touches[i] = NULL;
	}
	_num_touches = 0;
	_active_touch = 0;
    _typeOtimizeDrawModule = 0; // optimize default
	InitMathTable();
	 InitEx();
#ifdef MAC_OS
	 _sys_begin_second = GetCurrentMilisecond();
#else
	_sys_begin_second = time(NULL);
#endif
	_sync_sleep_time = 0;
	_last_frame_time = GetTime();
	_game_millisecond_begin = GetTime();
	//for enterbackground not call
	CGame::_this->_time_supen_begin = CGame::_this->GetTime();

	//hao.ly add
	
#if ADD_WITTER
	_twitterStatusCode = TWITTER_CONTROL_NORMAL_CODE;
#endif
	
	/////////////////////////////////////
	// MUSIC _SOUND
	////////////////////////////////////
	_soundEnable = true;
	_musicEnable = true;
	_music_effect_volume_state = 0;
	_current_music_vol = 0.0f;
	for (int s = 0; s < NUM_SOUND; s++){
		_sound_loaded[s] = false;
		_sound_handle[s] = 0;
	}

	_is_system_message_come = false;
	for (int i = 0; i < EVENT_STACK_MAX_SIZE; i++){
		_events[i].State = PINE_EVENT_STATE_UNAVAILABLE;
	}
    

#ifdef MAC_OS
#if ADD_ADVERT
#if !SERVER_ADS_START_IN_LOADING
    ServerADVERTStart();
#endif
#endif
#endif

	_isResumeMusicAfterPause = false;

	_is_init = true;
}
Orientation CGame::GetGameOrientation()
{
    /*
    if( (((GLESLib1*)_g)->GetCurrentGraphicContext())->ComplexMode == PineGraphicComplexOrientation::ComplexToRight)
    {
        return Orientation::Landscape_270;
    }
    else if((((GLESLib1*)_g)->GetCurrentGraphicContext())->ComplexMode== PineGraphicComplexOrientation::ComplexToRight)
    {
        return Orientation::Landscape_90;
    }
     */
    return _device.Device_Orientation;
}
void CGame::OnChangeOrientation(Orientation ori){
	PDEBUG("CHANGE ORIENTATION:%d", ori);
	_device.Device_Orientation = ori;
	if (_curr_resversion.VersionOrientation != Orientation::Portrait){
		if (_graphic_context.ComplexMode == PineGraphicComplexOrientation::ComplexToLeft && ori == Orientation::Landscape_90){
			_graphic_context.ComplexMode = PineGraphicComplexOrientation::ComplexToRight;
			if (_g != NULL)_g->ChangeContext(_graphic_context);
		}
		else if (_graphic_context.ComplexMode == PineGraphicComplexOrientation::ComplexToRight && ori == Orientation::Landscape_270){
			_graphic_context.ComplexMode = PineGraphicComplexOrientation::ComplexToLeft;
			if (_g != NULL)_g->ChangeContext(_graphic_context);
		}
	}
}
void CGame::InitDevice(Device dv){
	_device = dv;
	OnScreenChange(_device.ScreenSize);
}
void CGame::Release()
{
	ReleaseEx();

	SAFE_DEL(_unzip_buff);
	//SAFE_DEL(_input_stream);
	//SAFE_DEL(_data_input_stream);
	for (int i = 0; i < NUM_ANIM_FILES; i++)
	{
		if (_sprites[i] != NULL)
		{
			_sprites[i]->Unload(_g);
		}
		SAFE_DEL(_sprites[i]);
	}
	SAFE_DEL_ARRAY(_sprites);
	SAFE_DEL_ARRAY(_reg_sprites_unload);
	
	/*#for (int i = 0; i < NUM_TEXT_PACKAGES; i++)
	{
		SAFE_DEL_ARRAY(_texts[i]);
		SAFE_DEL_ARRAY(_text_lens[i]);
	}
	SAFE_DEL_ARRAY(_texts);
	SAFE_DEL_ARRAY(_text_lens);
	*/
	SAFE_DEL(_g);
	
	for (int i = 0; i < MAX_TOUCHES; i++)
	{
		SAFE_DEL(_touches[i]);
	}
	SAFE_DEL_ARRAY(_touches);
	
	//SAFE_DEL(pOnline);
	FreeOutputBuffer();
	delete (DataInputStream::_buffer);
	delete(CSprite::_rle_ind_buffer);
	delete(CSprite::_rle_alp_buffer);
	delete(CSprite::_ind_buffer);
	delete(CSprite::_alp_buffer);
	
}

///////////////////////////////////////////////
//	WORK WITH RES SOURCE VERSION
///////////////////////////////////////////////
PineSize CGame::GetResVersionSize(SupportVersion version)
{
	return ResVersionSize[(int)version];
}
Orientation CGame::GetOrientation(PineSize* size){
	return size->W > size->H ? Orientation::Landscape_90 : Orientation::Portrait;
}
void CGame::CorrectSize(PineSize* size, Orientation ori){
	PineSize tmp = *size;
	if (ori == Orientation::Landscape_90 || ori == Orientation::Landscape_270){
		size->W = tmp.W > tmp.H ? tmp.W : tmp.H;
		size->H = tmp.W < tmp.H ? tmp.W : tmp.H;
	}
	else if (ori == Orientation::Portrait){
		size->H = tmp.W > tmp.H ? tmp.W : tmp.H;
		size->W = tmp.W < tmp.H ? tmp.W : tmp.H;
	}
}
ResVersion CGame::GetAppropriateVersion(PineSize withsize, ResVersion* support_screen, int array_len, bool support_rotate)
{
	ENGINE_DEBUG("getAppropriateVersion: withsize[w:%0.2f h:%0.2f] numversion:%d\n", withsize.W, withsize.H,array_len);
	PineSize tmp_withsize = withsize;
	float width = support_rotate ? (tmp_withsize.W > tmp_withsize.H ? tmp_withsize.W : tmp_withsize.H) : tmp_withsize.W;
	float height = support_rotate ? (tmp_withsize.W < tmp_withsize.H ? tmp_withsize.W : tmp_withsize.H) : tmp_withsize.H;
	int choice = 0;
	PineSize current_size = GetResVersionSize(support_screen[0].Version);

#if defined (CHOOSE_VERSION_PREFERRED_SIZE)
	float choice_size = current_size.W*current_size.H;
	choice = 0;
	for (int r = 1; r < array_len; r++)
	{
		current_size = GetResVersionSize(support_screen[r].Version);
		
		if (width * height == current_size.W * current_size.H) {
			choice = r;
			break;
		}

		int temp_choice_size = current_size.W * current_size.H;
		if (CMath::ABSOLUTE_VALUE(temp_choice_size - width * height) < CMath::ABSOLUTE_VALUE(choice_size - width * height))
		{
			choice_size = current_size.W * current_size.H;
			choice = r;
		}
	}
#else
	if (!support_rotate)
	{
		CorrectSize(&current_size, support_screen[0].VersionOrientation);
	}
	else{
		CorrectSize(&tmp_withsize, support_screen[0].VersionOrientation);
	}
	float scalew = width > current_size.W ? (width / current_size.W) : current_size.W / width;
	float scaleh = height > current_size.H ? (height / current_size.H) : current_size.H / height;
	float choice_scale = scalew - scaleh;
	if (choice_scale < 0) choice_scale *= -1.0f;
	float choice_w = current_size.W;
	for (int r = 1; r < array_len; r++)
	{
		current_size = GetResVersionSize(support_screen[r].Version);
		if (!support_rotate)
		{
			CorrectSize(&current_size, support_screen[r].VersionOrientation);
		}
		else{
			CorrectSize(&tmp_withsize, support_screen[r].VersionOrientation);
			ENGINE_DEBUG("w:%0.2f h:%0.2f crrw:%0.2f crrh:%0.2f\n", width, height,current_size.W, current_size.H);
			if(width == current_size.W && height == current_size.H){
				ENGINE_DEBUG("here2\n");
				choice_scale = 1.0f;
				choice = r;
				break;
			}
		}
		scalew = width > current_size.W ? (width / current_size.W) : current_size.W / width;
		scaleh = height > current_size.H ? (height / current_size.H) : current_size.H / height;
		float tmp_scale = scalew - scaleh;
		if (tmp_scale < 0) tmp_scale *= -1.0f;
		if (tmp_scale < choice_scale)
		{
			//PDEBUG("choice scale:%0.2f r:%d\n", tmp_scale, r);
			choice_scale = tmp_scale;
			choice = r;
			choice_w = current_size.W;
		}
		else if (tmp_scale == choice_scale){
			if (CMath::ABSOLUTE_VALUE(current_size.W - tmp_withsize.W) < CMath::ABSOLUTE_VALUE(choice_w - tmp_withsize.W)){
				choice_scale = tmp_scale;
				choice = r;
				choice_w = current_size.W;
				//PDEBUG("choice scale:%0.2f r:%d\n", tmp_scale, r);
			}
		}

		//else{
		//	PDEBUG("not choice scale:%0.2f r:%d\n", tmp_scale, r);
		//}
	}
#endif
	ENGINE_DEBUG("choice:%d\n",choice);
	return support_screen[choice];
}

///////////////////////////////////////////////
//	WORK WITH GAME CONTEXT
///////////////////////////////////////////////
void CGame::AlignmentContext(Alignment align, PineSize withsize, ResVersion res_version, bool allow_scale,int scanline){
	_curr_resversion = res_version;
	_game_context.CurrentResourceVersion = res_version;
	ENGINE_DEBUG("with:w:%0.2f h:%0.2f\n",withsize.W, withsize.H);
	PineSize versize = GetResVersionSize(res_version.Version);

	Orientation withsize_ori = GetOrientation(&withsize);
	CorrectSize(&versize, res_version.VersionOrientation);
	ENGINE_DEBUG("versize: w:%0.2f h:%0.2f\n", versize.W, versize.H);
	if((versize.W < versize.H && withsize.W > withsize.H)
		|| (versize.W > versize.H && withsize.W < withsize.H)){
		ENGINE_DEBUG("USE COMPLEXMODE\n");
		//if (_device_orientation == ORIENTATION_LANDSCAPE_90)
		//{
		//	_complex_mode = PineGraphicComplexOrientation::ComplexToRight;
		//}
		//else
		//{
		//	_complex_mode = PineGraphicComplexOrientation::ComplexToLeft;
		//}
		if (_device.Device_Orientation == Orientation::Landscape_90){
			_graphic_context.ComplexMode = PineGraphicComplexOrientation::ComplexToRight;
		}
		else{
			_graphic_context.ComplexMode = PineGraphicComplexOrientation::ComplexToLeft;
		}
	}else{
		ENGINE_DEBUG("NOT USE COMPLEXMODE\n");
		_graphic_context.ComplexMode = PineGraphicComplexOrientation::ComplexNone;
	}
	
	
	CorrectSize(&withsize, res_version.VersionOrientation);
	_game_context.Size.W = versize.W;
	_game_context.Size.H = versize.H;
	float scale = 1.0f;
	if (allow_scale){
		float scalew = withsize.W /versize.W;
		float scaleh = withsize.H/ versize.H;
		if(scanline == _HCENTER)
		{
			scale = scalew;
#if defined (_WINDOWS_PHONE8)
			if (versize.H * scale > withsize.H && versize.W * scaleh > withsize.W)
#else
			if (versize.H * scale > withsize.H)
#endif
			{
				scale = scaleh;
			}
		}
		else
		{
			scale = scalew;
			/*
			 if (versize.H * scale > withsize.W){
			 scale = scalew;
			 }
			 */
		}
	}
	_game_context.ScaleX = _game_context.ScaleY = scale;
	versize.W *= scale; 
	versize.H *= scale;
	versize.W = ceil(versize.W);
	versize.H = ceil(versize.H);

	float soutw = versize.W / withsize.W;
	float south = versize.H / withsize.H;
	float sout = soutw < 1.0 ? soutw : south < 1.0 ? south : 1.0f;
	_game_context.ScaleOutX = 1.0f / soutw;
	_game_context.ScaleOutY = 1.0f / south;
	
	if (sout< 1.0f){
		_game_context.ScaleOut = 1.0f / sout;
	}

	_game_context.BorderTop = _game_context.BorderLeft = 0;
	float dw = withsize.W - versize.W;
	float dh = withsize.H - versize.H;
	if (align.Horizontal == HorizontalAlign::Center){
		_game_context.TranslateX = dw / 2;
		_game_context.BorderLeft = dw / 2;
	}
	if (align.Horizontal == HorizontalAlign::Right){
		_game_context.TranslateX = dw;
		_game_context.BorderLeft = dw;
	}
	if (align.Vertical == VerticalAlign::Middle){
		_game_context.TranslateY = dh / 2;
		_game_context.BorderTop = dh / 2;
	}
	if (align.Vertical == VerticalAlign::Bottom){
		_game_context.TranslateY = dh;
		_game_context.BorderTop = dh;
	}
	_game_context.BorderRight = _game_context.BorderLeft + versize.W;
	_game_context.BorderBottom = _game_context.BorderTop + versize.H;
	
	bool is_use_graphic_context = true;
	_graphic_context.TranslateX = 0;
	_graphic_context.TranslateY = 0;
	_graphic_context.ScaleX = 1.0f;
	_graphic_context.ScaleY = 1.0f;
		
	if(_device.IsEnableScaleView())
	{
		is_use_graphic_context = !_device.ScaleView(_game_context.ScaleX, _game_context.ScaleY);
	}
	
	if (_g != NULL && is_use_graphic_context){

		CorrectSize(&withsize, withsize_ori);
		_graphic_context.DeviceSize = withsize;
		_graphic_context.TranslateX = _game_context.TranslateX;
		_graphic_context.TranslateY = _game_context.TranslateY;
		_graphic_context.ScaleX = _game_context.ScaleX;
		_graphic_context.ScaleY = _game_context.ScaleY;
#if !defined(USE_GRAPHIC_CLIP_FULL_SCREEN)
		_graphic_context.ClipFixContext = true;
		if (_graphic_context.ComplexMode == PineGraphicComplexOrientation::ComplexNone){
			_graphic_context.ClipArea.X = _game_context.BorderLeft;
			_graphic_context.ClipArea.Y = _game_context.BorderTop;
			_graphic_context.ClipArea.W = versize.W;
			_graphic_context.ClipArea.H = versize.H;
		}
		else if (_graphic_context.ComplexMode == PineGraphicComplexOrientation::ComplexToLeft){
			_graphic_context.ClipArea.X = _game_context.BorderTop;
			_graphic_context.ClipArea.Y = withsize.H - _game_context.BorderLeft - versize.W;
			_graphic_context.ClipArea.W = versize.H;
			_graphic_context.ClipArea.H = versize.W;
		}
		else if (_graphic_context.ComplexMode == PineGraphicComplexOrientation::ComplexToRight){
			_graphic_context.ClipArea.X = withsize.W - _game_context.BorderTop - versize.H;
			_graphic_context.ClipArea.Y = _game_context.BorderLeft;
			_graphic_context.ClipArea.W = versize.H;
			_graphic_context.ClipArea.H = versize.W;
		}
#else
		_graphic_context.ClipFixContext = false;
		_graphic_context.ClipArea.X = 0;
		_graphic_context.ClipArea.Y = 0;
		_graphic_context.ClipArea.W = withsize.W;
		_graphic_context.ClipArea.H = withsize.H;
#endif
		PinePoint point_o = _game_context.MeasureAnchor(0,0);
		PinePoint point_bot_right = _game_context.MeasureAnchor(0, 0, ANCHOR_RIGHT | ANCHOR_BOTTOM);


		_game_context.PointTopLeft = point_o;
		_game_context.PointBottomRight = point_bot_right;

		_graphic_context.Viewport.X = point_o.X;
		_graphic_context.Viewport.Y = point_o.Y;
		_graphic_context.Viewport.W = point_bot_right.X - point_o.X;
		_graphic_context.Viewport.H = point_bot_right.Y - point_o.Y;
		printf("\n scale_x:%0.2f scale_y:%0.2f\n", _graphic_context.ScaleX, _graphic_context.ScaleY);
		
	}
	
	_g->ChangeContext(_graphic_context);
}
/////////////////////////////
//	OPTIMIZE
/////////////////////////////
void CGame::SetTextureScale(bool is_scale) {
	_is_texture_scale = is_scale;
}

/////////////////////////////////////
///EVENT SYSTEM
/////////////////////////////////////
void CGame::addEvent(EVENTID id, const char* data, UINT32 datalen){
	for (int i = 0; i < EVENT_STACK_MAX_SIZE; i++){
		if (_events[i].State == PINE_EVENT_STATE_UNAVAILABLE){
			_events[i].release();
			_events[i].ID = id;
			_events[i].init(data, datalen);
			_events[i].State = PINE_EVENT_STATE_AVAILABLE;
			if ((id >> 24) == (BYTE)PineEventClass::System) _is_system_message_come = true;
			break;
		}
	}
}
const PineEvent* CGame::NextEvent(EVENTID id, EVENT_POINTER* pointer){
	EVENT_POINTER pass = *pointer;
	if (pass < 0) pass = 0;
	for (int i = pass; i < EVENT_STACK_MAX_SIZE; i++){
		if (_events[i].ID == id && (_events[i].State & (PINE_EVENT_STATE_AVAILABLE | PINE_EVENT_STATE_TIMEOUT))){
			*pointer = i + 1;
			return &_events[i];
		}
	}
	return NULL;
}
const PineEvent* CGame::NextEventOnClass(PineEventClass class_fillter, EVENT_POINTER* pointer){
	EVENT_POINTER pass = *pointer;
	if (pass < 0) pass = 0;
	for (int i = pass; i < EVENT_STACK_MAX_SIZE; i++){
		if (((_events[i].ID >> 24) == (BYTE)class_fillter) && (_events[i].State & (PINE_EVENT_STATE_AVAILABLE | PINE_EVENT_STATE_TIMEOUT))){
			*pointer = i + 1;
			return &_events[i];
		}
	}
	return NULL;
}
void CGame::KeepEvent(const PineEvent* pevent){
	((PineEvent*)pevent)->State = PINE_EVENT_STATE_AVAILABLE;
}
void CGame::CleanEvent(const PineEvent* pevent){
	((PineEvent*)pevent)->State = PINE_EVENT_STATE_TIMEOUT;
}
void CGame::CleanEvents(EVENTID id){
	for (int i = 0; i < EVENT_STACK_MAX_SIZE; i++){
		if (_events[i].ID == id){
			_events[i].State = PINE_EVENT_STATE_UNAVAILABLE;
		}
	}
}
void CGame::FlushGraphics()
{
#if !defined(_WINDOWS_PHONE8) || !defined(_WIN8) || !defined(ANDROID_OS)
	_g->Flush();
#endif
}


void CGame::Run()
{
    UINT64 begin_frame_time = GetTime();
	//TEST_UPDATE();
	if (!_g->IsEngineReady() || _isPause)
	{
		return;
	}
	/////////////////////////////////////////
	///PROCESS SYSTEM MESSAGE
	/////////////////////////////////////////
	bool is_frame_break = false;
	if (_is_system_message_come){
		EVENT_POINTER evt_pointer = 0;
		const PineEvent* evt = NextEventOnClass(PineEventClass::System, &evt_pointer);
		while (evt != NULL){
			switch (evt->ID){
			case EVENT_FRAME_BREAK:
				is_frame_break = true;
				KeepEvent(evt);
				break;
			case EVENT_FRAME_CONTINUE:
				CleanEvents(EVENT_FRAME_BREAK);
				is_frame_break = false;
				CleanEvent(evt);
				break;
#if defined(MAC_OS)
            case EVENT_REQUEST_CREATE_TEXTURE:
                {
                    BufferStream bufferSteam;
                    bufferSteam.Init(evt->Data->Data(), evt->Data->Length());
                    int lengthOfPointer = bufferSteam.ReadByte();
                    BYTE* tmp_arr = new BYTE[lengthOfPointer];
                    for (int i = 0; i < lengthOfPointer; i++)
                    {
                        tmp_arr[i] = bufferSteam.ReadByte();
                        //printf("\n read tmp_arr[%d] = %d", i, tmp_arr[i]);
                    }
                    
                    BYTE** tmp_pointer = ((BYTE**)tmp_arr);
                    //printf("\n read pointer0:%d", ((BYTE**)tmp_arr));
                    
                    int width = bufferSteam.ReadInt32();
                    int height = bufferSteam.ReadInt32();
                    PineColorFormat colorFormat = (PineColorFormat)bufferSteam.ReadInt32();
                    BYTE *buffer = *tmp_pointer;
                    //printf("\n read buffer:%d", buffer);
                    HTEXTURE texName = G()->createTexture(buffer, width, height, colorFormat);
                    CleanEvent(evt);
                    
                    int lengthPointer = sizeof(int*);
                    int dataLength = 1 + lengthPointer + 4;
                    BufferStream bufferStreamResponse;
                    bufferStreamResponse.Init(dataLength);
                    
                    bufferStreamResponse.WriteByte(lengthPointer);
                    for (int i = 0; i < lengthOfPointer; i++)
                    {
                        bufferStreamResponse.WriteByte(bufferSteam.Data()[1 + i]);
                    }
                    bufferStreamResponse.WriteInt32(texName);
                    
                    addEvent(EVENT_RESPONSE_CREATE_TEXTURE, (const char*)bufferStreamResponse.Data(), dataLength);
                }
                break;
#endif
			}
			evt = NextEventOnClass(PineEventClass::System, &evt_pointer);
		}
        
	}
	//update and render game
	if (!(_isExit || _isPause || is_frame_break))
	{
		_g->BeginTransaction();
		if (_num_reg_sprite_unload > 0){
			for (int i = _num_reg_sprite_unload - 1; i >= 0; i--){
				UnloadSprite(_reg_sprites_unload[i]);
			}
			_num_reg_sprite_unload = 0;
		}

		UpdateAndRender();
#ifndef REMOVE_MUSIC_ADJUST
		if (_music_effect_volume_state == 1)
		{
			_current_music_vol += _delta_volume;
			if (_current_music_vol > 1.0f)
			{
				_current_music_vol = 1.0f;
				_music_effect_volume_state = 0;
			}
		}
		else if (_music_effect_volume_state == 2)
		{
			_current_music_vol -= _delta_volume;
			if (_current_music_vol < 0)
			{
				_current_music_vol = 0;
				_music_effect_volume_state = 0;
				////OS_StopCurrentMusic();
				StopCurrentMusic();
			}
		}
		////SetVolume(_current_music_vol);
#endif
		_frame_counter++;
		/////////////////////////////////////////
		///UPDATE EVENT LIFE CIRCLE
		/////////////////////////////////////////
		for (int i = 0; i< EVENT_STACK_MAX_SIZE; i++){
			if (!(_events[i].State & PINE_EVENT_STATE_UNAVAILABLE)) _events[i].State = _events[i].State << 1;
		}
	}
	else{
		return;
	}

#if !defined(_WINDOWS_PHONE8) && !defined(_WIN8) && !defined(ANDROID_OS)
	if (!(_isExit || _isPause))
	{
		FlushGraphics();
        
        UINT64 begin_sleep_time = GetTime();

		_frame_dt = (begin_sleep_time - begin_frame_time) + _sync_sleep_time;
        //printf("fdt:%llu begin_frm:%llu begin_sleep:%llu sync:%llu\n ", _frame_dt, begin_frame_time, begin_sleep_time, _syn_time_start);
        
		if (_frame_dt < FPS_LIMIT_TIME)
		{
			Sleep(FPS_LIMIT_TIME - _frame_dt);

			_sync_sleep_time = GetTime() - begin_sleep_time;

			if (_frame_dt + _sync_sleep_time > FPS_LIMIT_TIME)
			{
				_sync_sleep_time = (_frame_dt + _sync_sleep_time) - FPS_LIMIT_TIME;
			}
			else
			{
				_sync_sleep_time = 0;
			}
		}
		else
		{
			_sync_sleep_time = _frame_dt - FPS_LIMIT_TIME;
		}

		if(_sync_sleep_time >= FPS_LIMIT_TIME) _sync_sleep_time = 0;
	}
#else
	INT64 time_curr = GetTime();
	_frame_dt = time_curr - _last_frame_time;
	_last_frame_time = time_curr;
#endif
}

extern "C" void forceRender();
void CGame::Pause()
{

	_isPause = true;

	if(!_is_init)return;

	StopAllSFX();
	if(_sound_engine != NULL){
		if(_sound_engine->IsPlayingMusic()){
			_sound_engine->PauseMusic();
			_isResumeMusicAfterPause = true;
		}
	}	
	PauseEx();
}

void CGame::Resume()
{
	_isPause = false;
	
	if(!_is_init)return;

#if !defined (_WINDOWS_PHONE8)
	if (_g != NULL){
		_g->BeginTransaction();
	}
	if (_sound_engine != NULL && _musicEnable) {
		if (_isResumeMusicAfterPause) {
			_sound_engine->ResumeMusic();
		}
		//interup complete

	}
	_isResumeMusicAfterPause = false;
#endif	
	ResumeEx();
}

void CGame::Exit()
{
	_isExit = true;

}

void CGame::SetGameState(int iNewState, bool isCleaning)
{
   
	if (isCleaning)
	{
		_GameStateNew = iNewState;
		_GameSubState = k_DESTROY;
		//ClearTouchesEnd();
		//ClearAllTouch();
	}
	else
	{
		_GameStateOld = _GameState;
		_GameState = iNewState;
		_GameSubState = k_INIT;
		//ClearTouchesEnd();
		//ClearAllTouch();
	}
	_run_time = 0;
}

/*mark
void CGame::SetGameStateWithEffect(int iNewState)
{
	_GameStateNew = iNewState;
	_GameSubState = k_EFFECT_UNLOAD;
	ClearAllTouch();
	_run_time = 0;
}
*/

void CGame::SetSubState(int iNewSubState)
{
	_GameSubState = iNewSubState;
	//ClearTouchesEnd();
	//ClearAllTouch();
	_run_time = 0;
}
BYTE* CGame::GetResource(const char* sFile)
{
    FILE* f = NULL;
    f = fopen(sFile, "rb");
    fseek(f,0,SEEK_END);
    int size = ftell(f);
    fseek(f,0,SEEK_SET);
    
    INT32 iOffset = 0;
    INT32 iLen = size;
    BYTE* abResData = new BYTE[iLen];
    int iReadedBytes = fread(abResData, 1, iLen, f);
    fclose(f);
    f = NULL;
    sFile = NULL;
    //if read fail
    if (iReadedBytes != iLen)
    {
        return NULL;
    }
    return abResData;
}
BYTE* CGame::GetResource(INT32 iResID)
{
#if USE_COMPRESSOR_DATA_INGAME
    char* sFile = NULL;
    //Compressor_Exporter::_zipcompress_file[i*TOTAL__ZIPCOMPRESS_FILE+_ZIPCOMPRESS_FILE_PACK_SHORT_ZIP];
    int index = Device::GetPackCompressed(_ResourceFileName[_IDtoFileName[iResID]]);
    if(index >= 0)
    {
        bool exitsfile = OS_IsDocumentFileExist(Device::GetPackShortNameCompressed(index));
        bool isLastFile =  false;
        if(exitsfile)
        {
            char name[256];
            sprintf(name,"%s",Device::GetPackFullNameCoompressed(index));
            time_t t = Device::GetTimeModifyOfFile(OS_GetResourceFilePath(name));
            sprintf(name,"log_%ld_%s",t,Device::GetPackShortNameCompressed(index));
            //printf("\nLog 0: %s",name);
            if(Device::HasAppInstalled(name,GAME_VERSION))
            {
                isLastFile = true;
            }
            //printf("\n File: %s, time modify date:%ld",name,t);
        }
        if(!exitsfile || !isLastFile)
        {
            std::string str_archive(Device::GetPackFullNameCoompressed(index));
            std::wstring archive_file(str_archive.begin(), str_archive.end());
            std::wstring archive_path = OS::findResourcePath(archive_file, EFileLocation::APP_DIR);
            
            std::string str_output(Device::GetPackShortNameCompressed(index));
            std::wstring output_file(str_output.begin(), str_output.end());
            std::wstring output_path = OS::findResourcePath(output_file, EFileLocation::DOCUMENT_DIR);
            
            std::string str_identify(Device::GetPackShortNameCompressed(index));
            

            if(Device::PINEZ_Extractor(archive_path, str_identify, output_path))
            {
                char name[256];
                sprintf(name,"%s",Device::GetPackFullNameCoompressed(index));
                time_t t = Device::GetTimeModifyOfFile(OS_GetResourceFilePath(name));
                sprintf(name,"log_%ld_%s",t,Device::GetPackShortNameCompressed(index));
                //printf("\nLog 1: %s",name);
                Device::SetAppInstalled(name,GAME_VERSION);
                
                OS::setFileAttribute(output_path, EFileLocation::PATH, EFileAttribute::ARCHIVE, false);
                
            }
        }
        sFile = OS_GetDocumentFilePath(Device::GetPackShortNameCompressed(index));
    }
    else
    {
        sFile = OS_GetResourceFilePath(_ResourceFileName[_IDtoFileName[iResID]]);
    }
#else
	char* sFile = OS_GetResourceFilePath(_ResourceFileName[_IDtoFileName[iResID]]);
#endif
	FILE* f = NULL;

	f = fopen(sFile, "rb");
	INT32 iOffset = _ResourceInfo[2*iResID];
	INT32 iLen = _ResourceInfo[2*iResID + 1];
	BYTE* abResData = new BYTE[iLen];
	fseek(f, iOffset, SEEK_SET);
	int iReadedBytes = fread(abResData, 1, iLen, f);
	fclose(f);

	delete[] sFile;
	f = NULL;
	sFile = NULL;
	//if read fail
	if (iReadedBytes != iLen)
	{
		return NULL;
	}
	return abResData;
}

INT32 CGame::GetResourceSize(int iResID)
{
	return _ResourceInfo[2*iResID + 1];
}
DataInputStream* CGame::GetDataInputStream(BYTE* buff,int length)
{
	DataInputStream* _data_input_stream = new DataInputStream();
	bool result = _data_input_stream->Open(buff, length);
	
	if (result)
	{
		return _data_input_stream;
	}
	
	_data_input_stream->Close();
	SAFE_DEL(_data_input_stream);
	
	return NULL;
}
DataInputStream* CGame::GetDataInputStream(int iResID,const char* file_name)
{
	
	char filename[1024];
	sprintf(filename, "%s", _ResourceFileName[_IDtoFileName[iResID]]);
	//PDEBUG("get data input stream:%s %d %d\n", _ResourceFileName[_IDtoFileName[iResID]], _IDtoFileName[iResID], iResID);
	//ResourceItem res_item = OS_GetResourceItem(_ResourceFileName[_IDtoFileName[iResID]]);
	ResourceItem res_item = OS_GetResourceItem(_ResourceFileName[_IDtoFileName[iResID]]);
	//char* sFile = OS_GetResourceFilePath(_ResourceFileName[_IDtoFileName[iResID]]);
	//PDEBUG("make Data Input Strea:%s %d %d\n", res_item.Path, res_item.Offset, res_item.Length);
	INT32 iOffset = _ResourceInfo[2*iResID];
	DataInputStream* _data_input_stream = new DataInputStream();
	bool result = _data_input_stream->Open(res_item.Path, res_item.Offset + iOffset,GetResourceSize(iResID));
	//res_item.Offset += iOffset;
	//bool result = _data_input_stream->Open(res_item);
	
	if (result)
	{
		return _data_input_stream;
	}
	
	_data_input_stream->Close();
	SAFE_DEL(_data_input_stream);
	//delete[] sFile;
	return NULL;
}
//text resource
void CGame::LoadTextPack(INT32 iTextPackID)
{
	INT32 offset = 0;
	BYTE* text_pack = GetResource(iTextPackID);
	
	int iEncodeType = 0;
	iEncodeType = text_pack[offset++]&0xFF;
		
	//load text pack
	INT32 num_text = (text_pack[offset]&0xFF) | ((text_pack[offset+1]&0xFF)<<8);
	offset += 2;
	
	//#_texts[iTextPackID - FIRST_TEXT_PACKAGE] = new UNICHAR*[num_text];
	//#_text_lens[iTextPackID - FIRST_TEXT_PACKAGE] = new UINT16[num_text];
	/*#
	for (INT32 i = 0; i < num_text; i++)
	{
		UNICHAR* text_data = NULL;
		
		INT32 num_chars = (text_pack[offset]&0xFF) | ((text_pack[offset+1]&0xFF)<<8);
		offset += 2;
		
		text_data = new UNICHAR[num_chars];
		for (int j = 0; j < num_chars; j++)
		{
			if (iEncodeType == 1)
			{
				text_data[j] = (text_pack[offset]&0xFF) | ((text_pack[offset+1]&0xFF)<<8);
				offset += 2;
			}
			else
			{
				text_data[j] = text_pack[offset++]&0xFF;
			}
		}
		
		_texts[iTextPackID - FIRST_TEXT_PACKAGE][i] = text_data;
		_text_lens[iTextPackID - FIRST_TEXT_PACKAGE][i] = num_chars;
	}
	
	SAFE_DEL_ARRAY(text_pack);
	*/
}

void CGame::FreeOutputBuffer()
{
	SAFE_DEL_ARRAY(_output_buffer);
}

//gfx resource
void CGame::LoadSpriteFromRGBA(int sprite_id,BYTE* rgba,int width,int height, bool is_cache_texture, bool linear_texture, int pal , bool load_trueColor_texture,bool fullcache)
{
	UnloadSprite(sprite_id);
	if (_sprites[sprite_id] == NULL)
	{
		_sprites[sprite_id] = new CSprite();
	}
	if(fullcache)
	{
		DataInputStream* is = GetDataInputStream(sprite_id);
		_sprites[sprite_id]->LoadFromRGBA(_g, is,rgba,width,height, is_cache_texture, linear_texture, 0, load_trueColor_texture,pal);
		_num_sprites++;
		
		is->Close();
	}
	else
	{
	
		DataInputStream* is = GetDataInputStream(sprite_id);
		_sprites[sprite_id]->LoadFromRGBA(_g, is,rgba,width,height, is_cache_texture, linear_texture, 0, load_trueColor_texture,pal);
		_num_sprites++;
		
		is->Close();
	}
}

void CGame::LoadSprite(int sprite_id,BYTE* buffer,int length, bool load_trueColor_texture,bool linear_texture, int pal)
{
	if (_sprites[sprite_id] == NULL)
	{
		//PDEBUG("load sprite:%d\n", sprite_id);
		_sprites[sprite_id] = new CSprite();
		DataInputStream* is = GetDataInputStream(buffer, length);
		_sprites[sprite_id]->Load(_g, is, true, linear_texture, 0, load_trueColor_texture, pal);
		_num_sprites++;

		is->Close();
		delete is;
	}
	else
	{
		CSprite* sprite = _sprites[sprite_id];
		if(!sprite->_has_texture)
		{
			//PDEBUG("reload texture sprite:%d\n", sprite_id);
			DataInputStream* is = GetDataInputStream(sprite_id, "");
			_sprites[sprite_id]->reloadTexture(_g, is);
			is->Close();
			delete is;
		}
	}
}

void CGame::LoadSprite(int sprite_id, bool is_cache_texture, int linear_texture, int pal, bool load_trueColor_texture,bool fullcache,const char* name_file)
{
	if (_sprites[sprite_id] == NULL)
	{
		//PDEBUG("load sprite 2:%d\n", sprite_id);
		_sprites[sprite_id] = new CSprite();
		if (_is_texture_scale) {
			_sprites[sprite_id]->SetTextureScale(_g, _graphic_context.ScaleX);
		}
		if (fullcache)
		{
			//printf("%s", name_file);
			DataInputStream* is = GetDataInputStream(sprite_id,name_file);
			_sprites[sprite_id]->Load(_g,is, is_cache_texture, linear_texture, 0, load_trueColor_texture,pal, false);
			_num_sprites++;

			is->Close();

			delete is;
		}
	}
	else{
		CSprite* sprite = _sprites[sprite_id];
		if (!sprite->_has_texture)
		{
			//PDEBUG("reload texture 2 sprite:%d\n", sprite_id);
			DataInputStream* is = GetDataInputStream(sprite_id, name_file);
			_sprites[sprite_id]->reloadTexture(_g, is, false);
			is->Close();
			delete is;
		}
	}
}

void CGame::LoadSpriteInThread(int sprite_id, bool is_cache_texture, bool linear_texture, int pal, bool load_trueColor_texture,bool fullcache,const char* name_file)
{
    if (_sprites[sprite_id] == NULL)
    {
		//PDEBUG("load sprite thread:%d\n", sprite_id);
        _sprites[sprite_id] = new CSprite();
        if (_is_texture_scale) {
            _sprites[sprite_id]->SetTextureScale(_g, _graphic_context.ScaleX);
        }
        if (fullcache)
        {
            //printf("%s", name_file);
            DataInputStream* is = GetDataInputStream(sprite_id,name_file);
            _sprites[sprite_id]->Load(_g,is, is_cache_texture, linear_texture, 0, load_trueColor_texture,pal, true);
            _num_sprites++;
            
            is->Close();
            
            delete is;
        }
    }
    else{
		CSprite* sprite = _sprites[sprite_id];
		if (!sprite->_has_texture)
		{
			DataInputStream* is = GetDataInputStream(sprite_id, name_file);
			_sprites[sprite_id]->reloadTexture(_g, is, true);
			is->Close();
			delete is;
		}
    }
}

void CGame::ReloadSpriteTextureCurrentVer(int spriteID){
	int sid = _syn_sprite_index[spriteID*VERSION_NUMS + _current_version];
	if (_sprites[sid] != NULL){
		DataInputStream* is = GetDataInputStream(sid, "");
		_sprites[sid]->reloadTexture(_g, is);
		is->Close();
		delete is;
	}

}
void CGame::LoadSprite(int sprite_id, bool is_cache_texture, int pal,const char* name_file)
{
	LoadSprite( sprite_id, is_cache_texture, true, pal,true,1,name_file);
}

void CGame::UnloadSprite(int sprite_id)
{
	if (_sprites[sprite_id] != NULL)
	{
		//PDEBUG("unload sprite %d\n", sprite_id);
		for (int i = 0; i < _anim_stock_num; i++)
		{
			if (_anim_stock[i] != NULL && _sprites[sprite_id] == GET_SPRITE(_index_anim_stock[i]))
			{
				_anim_stock[i] = NULL;
			}
		}
		_sprites[sprite_id]->Unload(_g);
		SAFE_DEL(_sprites[sprite_id]);
		_num_sprites--;
	}
}
void CGame::RegUnloadSprite(int sprite_id){
	_reg_sprites_unload[_num_reg_sprite_unload] = sprite_id;
	_num_reg_sprite_unload++;
}
CSprite* CGame::GetSprite(int sprite_id)
{
	return _sprites[sprite_id];
}

UNICHAR* CGame::GetText(int text_id, int pack_id)
{
	return NULL;//# _texts[pack_id - FIRST_TEXT_PACKAGE][text_id];
}

UINT16 CGame::GetTextLength(int text_id, int pack_id)
{
	return 0;//# _text_lens[pack_id - FIRST_TEXT_PACKAGE][text_id];
}

void CGame::DrawText(CSprite* font, int text_id, int pack_id, float x, float y, int anchor)
{
	font->DrawText(	G(), 
					GetText(text_id, pack_id), 
					GetTextLength(text_id, pack_id), 
					x, y, anchor);
}

void CGame::DrawTextPage(CSprite* font, int text_id, int pack_id, float x, float y, int w, int anchor)
{
	font->DrawPage(	G(), 
					GetText(text_id, pack_id), 
					GetTextLength(text_id, pack_id), 
					x, y, w,anchor);
}

void CGame::DrawText(CSprite* font, const char* text, float x, float y, int anchor)
{
	font->DrawAnsiText(	G(),	x, y, anchor, text);
}

void CGame::NotifyTouchesBegan(int touch_id, float x, float y, int tap_count)
{
	
	/*if (_complex_mode == PineGraphicComplexOrientation::ComplexToRight){
	float tmp = x;
	x = y;
	y = _game_context.Size.H - tmp;
	}*/
	//PDEBUG("x:%0.2f y:%0.2f\n", x, y);
	if (_graphic_context.ComplexMode == PineGraphicComplexOrientation::ComplexToRight){
		float tmp = x;
		x = y;
		y = _device.ScreenSize.W - tmp;
	}
	else if (_graphic_context.ComplexMode == PineGraphicComplexOrientation::ComplexToLeft){
		float tmp = y;
		y = x;
		x = _device.ScreenSize.H - tmp;
	}
	//PDEBUG("x:%0.2f y:%0.2f\t", x, y);
	x -= _game_context.TranslateX;
 	y -= _game_context.TranslateY;
	x /= _game_context.ScaleX;
	y /= _game_context.ScaleY;
	//PDEBUG(" -scalex:%0.2f scaley:%0.2f tx:%0.2f ty:%0.2f x:%0.2f y:%0.2f\n", _game_context.ScaleX,_game_context.ScaleY,_game_context.TranslateX,_game_context.TranslateY,x, y);
	//PDEBUG("touch begin id %d \n", touch_id);
	if (_num_touches >= MAX_TOUCHES)
	{
		//find oldest touch end
		int index = -1;
		unsigned int time = 0;
		/*for (int i = 0; i < _num_touches; i++)
		{
			if (_touches[i]->_flag == false)
			{
				time = _touches[i]->_touch_time;
				index = i;
				break;
			}
		}
		
		//clear this touch
		if (index >= 0)
		{
			_touches[index]->Init(touch_id, x, y, TOUCH_DOWN, tap_count, _frame_counter);
		}*/
		
		for (int i = 0; i < _num_touches; i++)
		{
			if (_touches[i]->_state == TOUCH_UP)
			{
				if (index == -1 || time > _touches[i]->_touch_time)
				{
					time = _touches[i]->_touch_time;
					index = i;
				}
			}
		}
		
		//clear this touch
		if (index >= 0)
		{
			CTouch* tmp = NULL;
			tmp = _touches[index];
			_touches[index] = _touches[_num_touches-1];
			_touches[_num_touches-1] = tmp;
			_num_touches--;
		}
	}
	
	if (_num_touches < MAX_TOUCHES)
	{
		if (_touches[_num_touches] == NULL)
		{
			_touches[_num_touches] = new CTouch(touch_id, x, y, TOUCH_DOWN, tap_count, _frame_counter);
		}
		else
		{
			_touches[_num_touches]->Init(touch_id, x, y, TOUCH_DOWN, tap_count, _frame_counter);
		}
		_num_touches++;
	}
	_active_touch++;
	UpdateTouch(x, y, TOUCH_DOWN, touch_id, tap_count);
}

void CGame::NotifyTouchesMoved(int touch_id, float x, float y, int tap_count)
{
	/*if (_complex_mode == PineGraphicComplexOrientation::ComplexToRight){
	float tmp = x;
	x = y;
	y = _game_context.Size.H - tmp;
	}*/
	if (_graphic_context.ComplexMode == PineGraphicComplexOrientation::ComplexToRight){
		float tmp = x;
		x = y;
		y = _device.ScreenSize.W - tmp;
	}
	else if (_graphic_context.ComplexMode == PineGraphicComplexOrientation::ComplexToLeft){
		float tmp = y;
		y = x;
		x = _device.ScreenSize.H - tmp;
	}
	x -= _game_context.TranslateX;
	y -= _game_context.TranslateY;
	x /= _game_context.ScaleX;
	y /= _game_context.ScaleY;
	//PDEBUG("touch moved id %d \n", touch_id);
	for (int i = 0; i < _num_touches; i++)
	{
		if (_touches[i]->_id == touch_id)
		{
			if (_touches[i]->_flag){
				if (_touches[i]->_state != TOUCH_DOWN || _frame_counter != _touches[i]->_touch_time){
					_touches[i]->Set(x, y, TOUCH_DRAG, tap_count, _frame_counter);
				}
			}
		}
	}
	UpdateTouch(x, y, TOUCH_DRAG, touch_id, tap_count);
}

void CGame::NotifyTouchesEnd(int touch_id, float x, float y, int tap_count)
{
	/*if (_complex_mode == PineGraphicComplexOrientation::ComplexToRight){
		float tmp = x;
		x = y;
		y = _game_context.Size.H - tmp;
	}*/
	if (_graphic_context.ComplexMode == PineGraphicComplexOrientation::ComplexToRight){
		float tmp = x;
		x = y;
		y = _device.ScreenSize.W - tmp;
	}
	else if (_graphic_context.ComplexMode == PineGraphicComplexOrientation::ComplexToLeft){
		float tmp = y;
		y = x;
		x = _device.ScreenSize.H - tmp;
	}
	x -= _game_context.TranslateX;
	y -= _game_context.TranslateY;
	x /= _game_context.ScaleX;
	y /= _game_context.ScaleY;
	//PDEBUG("touch ended id %d \n", touch_id);
	//printf("touch end id %d\n", touch_id);
	for (int i = 0; i < _num_touches; i++)
	{
		if (_touches[i]->_id == touch_id)
		{
			_touches[i]->Set(x, y, TOUCH_UP, tap_count, _frame_counter);
			//PDEBUG("\nTOUCH END SET up AT %d",i);
		}
	}
	//note: don't clear touch end here.
	_active_touch --;
	
	UpdateTouch(x, y, TOUCH_UP, touch_id, tap_count);
}
void CGame::NotifyTouchesCancel(int touch_id)
{
	for (int i = 0; i < _num_touches; i++)
	{
		if (_touches[i]->_id == touch_id)
		{
			_touches[i]->_is_cancelled = true;
			break;
		}
	}
}

void CGame::ClearTouchesEnd()
{
	CTouch* tmp = NULL;
	for (int i = _num_touches-1; i >= 0; i--)
	{
		if (
			_touches[i]->_state == TOUCH_UP)
		{
			TOUCHS(i)->_flag = false;
		}
	}
}

void CGame::ClearAllTouch()
{
	/*CTouch* tmp = NULL;
	for (int i = _num_touches-1; i >= 0; i--)
	{
		//if (_touches[i] != NULL)// ||
		//	_touches[i]->_state == TOUCH_UP)
		{
			tmp = _touches[i];
			_touches[i] = _touches[_num_touches-1];
			_touches[_num_touches-1] = tmp;
			
			_num_touches--;
		}
	}*/
	for(int i = 0; i < _num_touches;i++)
	{
		TOUCHS(i)->_flag = false;
	}
	_num_touches = 0;
	_active_touch = 0;
}

//sfx and music
bool CGame::LoadMusic(int music_id)
{
//#if defined(MAC_OS)
//	OS_StopCurrentMusic();
//	OS_LoadMusic(_ResourceFileName[_IDtoFileName[music_id]]);
//#else
	if(_sound_engine != NULL){
		//char* real_path = OS_GetResourceFilePath(_ResourceFileName[_IDtoFileName[music_id]]);
		//PDEBUG("GAME CORE Load Music: id:%d %s\n", music_id, _ResourceFileName[_IDtoFileName[music_id]]);
		StopCurrentMusic();
		ResourceItem resitem = OS_GetResourceItem(_ResourceFileName[_IDtoFileName[music_id]]);
#if !defined (ANDROID_OS)
		resitem.Offset = _ResourceInfo[2*music_id + 0];
		resitem.Length = _ResourceInfo[2*music_id + 1];
#endif
		_sound_engine->LoadMusic(resitem);
	}
//#endif
	_current_music_vol = 0;
	_music_effect_volume_state = 1;
	_delta_volume = 0.01f;
	return true;
}
#if	USE_LIB_MUSIC
void CGame::PlayLibMusic(int num_loops)
{
	OS_PlayLibMusic(num_loops);
}

void CGame::StopLibMusic()
{
	OS_StopLibMusic();
}
void CGame::PauseLibMusic()
{
	OS_PauseLibMusic();
}
int CGame::GetCurrentLibMusicTime()
{
	return OS_GetCurrentLibMusicTime();
}
int CGame::GetLibSongTime()
{
	return OS_GetLibSongTime();
}
void CGame::UpdateSongList()
{
	OS_UpdateLibSongList();
	//char* title = OS_GetLibSongTitle(0);
	//printf("tile 0 = %s\n", title);
}

int	 CGame::GetLibSongNum()
{
	return OS_GetLibSongNum();
}

char* CGame::GetLibSongTitle(int index)
{
	return OS_GetLibSongTitle(index);
}
char* CGame::GetLibCurrentSongTitle()
{
	return OS_GetLibCurrentSongTitle();
}
void CGame::PreviousMusic(int numSecond)
{
	OS_PreviousMusic(numSecond);
}
void CGame::ForwardMusic(int numSecond)
{
	OS_ForwardMusic(numSecond);
}
double CGame::GetCurrentMusicTime()
{
	return OS_GetCurrentMusicTime();
}
double CGame::GetSongTime()
{
	return OS_GetSongTime();
}
#endif

void CGame::PlayCurrentMusic(int num_loops)
{
//#if defined(MAC_OS)
//	if(_musicEnable){
//		OS_PlayCurrentMusic(num_loops);
//	}
//#else
	if (_musicEnable && _sound_engine != NULL){
		_sound_engine->PlayMusic(num_loops);
	}
//#endif
}

void CGame::StopCurrentMusic(float delta_volume)
{
#if defined(MAC_OS)
	OS_StopCurrentMusic();
#else
	if (_sound_engine != NULL){
		_sound_engine->StopMusic();
		_music_effect_volume_state = 2;
		_delta_volume = delta_volume;
	}
#endif
}

void CGame::PauseCurrentMusic()
{
#if defined(MAC_OS)
	OS_PauseCurrentMusic();
#else
	if (_sound_engine != NULL){
		_sound_engine->PauseMusic();
	}
#endif
}

bool CGame::IsCurrentMusicPlaying()
{
#if defined(MAC_OS)
	return  OS_IsCurrentMusicPlaying();
#else
	if (_sound_engine != NULL){
		return _sound_engine->IsPlayingMusic();
	}
	return false;
#endif
}

void CGame::SetVolume(float vol)
{
#if defined(MAC_OS)
	OS_setMusicVolume(vol/100.0f);
#else
	////OS_setMusicVolume(vol); //may be not support
#endif
}

//sound effect
bool CGame::HasLoadSFX(int soundid)
{
    int id = soundid % NUM_SOUND;
    //printf("\n name %s",_ResourceFileName[_IDtoFileName[soundid]]);
    if (_sound_loaded[id] && _sound_engine != NULL)
    {
        return true;
    }
    return false;
    
}
bool CGame::LoadSFX(int sound_id)
{
	int id = sound_id % NUM_SOUND;
    //printf("\n name %s",_ResourceFileName[_IDtoFileName[sound_id]]);
	if (!_sound_loaded[id] && _sound_engine != NULL){
		ResourceItem resitem = OS_GetResourceItem(_ResourceFileName[_IDtoFileName[sound_id]]);
		
		resitem.Offset += _ResourceInfo[2*sound_id + 0];
		resitem.Length = _ResourceInfo[2*sound_id + 1];
		_sound_handle[id] = _sound_engine->LoadSoundEffect(resitem);
		_sound_loaded[id] = true;
        _sound_map_id[id] = sound_id;
		return true;
	}
	return false;
}

void CGame::PlaySFX(int sound_id)
{
    //printf("\nsound_id  =%d",sound_id);
	int id = sound_id % NUM_SOUND;
	if (_soundEnable && _sound_loaded[id] && _sound_engine != NULL){
		_sound_engine->PlaySoundEffect(_sound_handle[id]);
	}
}

void CGame::StopSFX(int sound_id){
	int id = sound_id % NUM_SOUND;
	if (_soundEnable && _sound_loaded[id] && _sound_engine != NULL){
		_sound_engine->StopSoundEffect(_sound_handle[id]);
	}
}

void CGame::StopAllSFX()
{
	if (_sound_engine != NULL){
		//PDEBUG("STOP ALL SFX");
		_sound_engine->StopAllSoundEffect();
	}
}

void CGame::ClearAllSFX()
{
	if (_sound_engine != NULL) {
		//PDEBUG("STOP ALL SFX");
		_sound_engine->ClearAllSFX();
	}
}

void CGame::FreeSFX(int soundId)
{
    //StopSFX(soundId);
	int id = soundId % NUM_SOUND;
	if (_sound_loaded[id] && _sound_engine != NULL){
		_sound_engine->UnloadSoundEffect(_sound_handle[id]);
		_sound_loaded[id] = false;
        _sound_map_id[id] = -1;
	}
}

void CGame::FreeAllSFX()
{
    StopAllSFX();
	if (_sound_engine != NULL){
		for (int i = 0; i < NUM_SOUND; i++){
			if (_sound_loaded[i]){
				_sound_engine->UnloadSoundEffect(_sound_handle[i]);
				_sound_loaded[i] = false;
			}
		}
	}
}

//write file
void CGame::ClearIOBuffer()
{
	_output_offset = 0;
	if (_output_buffer == NULL)
	{
		_output_buffer = new BYTE[IO_BUFFER_LENGTH];
	}
}

bool CGame::ClearFileContent(const char* filename)
{
	return OS_SaveAppData(filename, (char*)_output_buffer, 0, false);
}

bool CGame::RemoveFile(const char* filename)
{
	return OS_DeleteAppData(filename);
}

bool CGame::WriteInt8(const char* filename, int data)
{
	if (_output_offset >= IO_BUFFER_LENGTH)
	{
		FlushIOBuffer(filename);
	}
	
	_output_buffer[_output_offset] = data&0xFF;
	_output_offset++;
	
	return true;
}

bool CGame::WriteInt16(const char* filename, int data)
{
	if (_output_offset >= IO_BUFFER_LENGTH - 1)
	{
		FlushIOBuffer(filename);
	}
	
	_output_buffer[_output_offset] = data&0xFF;
	_output_offset++;
	_output_buffer[_output_offset] = (data>>8)&0xFF;
	_output_offset++;
	
	return true;
}

bool CGame::WriteInt32(const char* filename, int data)
{
	if (_output_offset >= IO_BUFFER_LENGTH - 3)
	{
		FlushIOBuffer(filename);
	}
	
	_output_buffer[_output_offset] = data&0xFF;
	_output_offset++;
	_output_buffer[_output_offset] = (data>>8)&0xFF;
	_output_offset++;
	_output_buffer[_output_offset] = (data>>16)&0xFF;
	_output_offset++;
	_output_buffer[_output_offset] = (data>>24)&0xFF;
	_output_offset++;
	
	return true;
}

bool CGame::FlushIOBuffer(const char* filename)
{
	if (_output_offset > 0)
	{
		bool result = OS_SaveAppData(filename, (char*)_output_buffer, _output_offset, true);
		_output_offset = 0;

		return result;
	}

	return false;
}
CGame* CGame::GetInstace()
{
	if (_this == NULL)
		_this = new CGame();
	return _this;
}

//Reload anim
void CGame::InitAnimStock()
{
	_anim_stock_capacity = ANIM_STOCK_MAX;
	_anim_stock_num = 0;
	_anim_stock =  new CAnimObject*[ANIM_STOCK_MAX];
	for(int i = 0; i < ANIM_STOCK_MAX;i++)
	{
		_anim_stock[i] = NULL;
	}
	_current_anim_stock = new int[ANIM_STOCK_MAX];
	_index_anim_stock = new int[ANIM_STOCK_MAX];
	_anim_loop_stock = new int[ANIM_STOCK_MAX];
	_anim_x_stock = new float[ANIM_STOCK_MAX];
	_anim_y_stock = new float[ANIM_STOCK_MAX];
	_anim_is_stop_stock = new int[ANIM_STOCK_MAX];
}

void CGame::AddAnimStock(CAnimObject *anim,int index)
{
	/*for(int i = 0; i < _anim_stock_num; i++)
	{
		if(anim == _anim_stock[i])
		{
			_index_anim_stock[i] = index;
			return;
		}
	}
	_anim_stock[_anim_stock_num] = anim;
	_index_anim_stock[_anim_stock_num] = index;
	PDEBUG("add anim:%d\n", index);
	_anim_stock_num++;
	*/
}

void CGame::BackupAnimStock()
{
	/*for(int i = 0; i <_anim_stock_num; i++)
	{
		if(NULL != GET_SPRITE(_index_anim_stock[i]))
		{
			
			
			_current_anim_stock[i] = _anim_stock[i]->_current_anim;
			_anim_loop_stock[i] = _anim_stock[i]->_is_loop;
			_anim_x_stock[i] = _anim_stock[i]->_x;
			_anim_y_stock[i] = _anim_stock[i]->_y;
			_anim_is_stop_stock[i] = _anim_stock[i]->IsCurrentAnimStop();
			PDEBUG("backup anim: %d %d", i, _anim_stock[i]);
		}
	}
	*/
}

void CGame::RestoreAnimStock()
{
	/*for(int i = 0; i <_anim_stock_num; i++)
	{
		PDEBUG("try to ")
		if (_anim_stock[i] != NULL)
		{
			if (NULL != GET_SPRITE(_index_anim_stock[i]))
			{
				PDEBUG("restore anim :%d\n", _index_anim_stock[i]);
				_anim_stock[i]->SetAnimSprite(GET_SPRITE(_index_anim_stock[i]), _index_anim_stock[i]);
				if (!_anim_is_stop_stock[i])
				{
					_anim_stock[i]->SetAnim(_current_anim_stock[i], _anim_loop_stock[i]);
					_anim_stock[i]->SetPos(_anim_x_stock[i], _anim_y_stock[i]);
				}
			}
		}
	}
	printf("abc");
	//ANIM().Clear();
	*/
}

int CGame::GetFixedTexture(int size)
{
	int ret = 1;
	while(ret < size)
	{
		ret=(ret<<1);
	}
	return ret;
}

void CGame::ClearTouchID(int ids)
{
    for(int i = 0; i < _num_touches; i++)
    {
        if(TOUCHS(i)->_flag && TOUCHS(i)->_id == ids)
        {
            TOUCHS(i)->_flag = false;
        }
    }
}

void CGame::ClearAnotherTouch(int touch)
{
	for(int i = 0; i < _num_touches; i++)
	{
		if(touch != i)
		{
			TOUCHS(i)->_flag = false;
		}
	}
}

void CGame::Sleep(UINT64 iTime)
{
	//printf("sheep %d ", iTime);
	
#ifdef MAC_OS
	//OS_sleep(iTime);
	std::this_thread::sleep_for(std::chrono::nanoseconds(iTime));
#elif defined(_WINDOWS_PHONE8)
	//Utils::Sleep(iTime);
#elif defined(_WIN8)
	
	
#elif defined(ANDROID_OS)
	
#else

	std::this_thread::sleep_for(std::chrono::nanoseconds(iTime));
#endif

}

unsigned long CGame::GetTime()
{
	return OS_GetTime();
}
const char* CGame::GetResourceFileName(UINT32 iResID){
	return _ResourceFileName[_IDtoFileName[iResID]];
}
#if TEXTURE_MANAGER_MAX
int CGame::_texture_manager[TEXTURE_MANAGER_MAX];
bool  CGame::_texture_ticket_used[TEXTURE_MANAGER_MAX];
void CGame::RegiterTexture()
{
	_current_ticket_texture = 0;
	glEnable(GL_TEXTURE_2D);
	for(int i = 0; i < TEXTURE_MANAGER_MAX; i++)
	{
		_texture_ticket_used[i] = false;
		UINT32 texname = -1;
		glGenTextures(1, &texname);
		_texture_manager[i] = texname;
		if(_texture_manager[i] < 0)
		{
			printf("\nError i = %d",i);
		}
	}
	glDisable(GL_TEXTURE_2D);
}

int CGame::GetTextureTicket()
{
	int ticket = -1;
	for(int i = 0; i < TEXTURE_MANAGER_MAX; i++)
	{
		int index = (_current_ticket_texture+i)%TEXTURE_MANAGER_MAX;
		if(!_texture_ticket_used[index])
		{
			_texture_ticket_used[index] = true;
			_current_ticket_texture = (index+1)%TEXTURE_MANAGER_MAX;
			return _texture_manager[index];
		}
	}
	return ticket;
}
void CGame::DeleteTicket(int ticket_id)
{
	int index = ticket_id - _texture_manager[0];
	if(index >= 0 && index < TEXTURE_MANAGER_MAX)
	{
		if(_texture_manager[index] == ticket_id)
		{
			DeleteTicketAt(index);
			return;
		}
	}
	for(int i = 0; i < TEXTURE_MANAGER_MAX; i++)
	{
		if(_texture_ticket_used[i] &&_texture_manager[i] == ticket_id )
		{
			DeleteTicketAt(i);
			return;
		}
	}
}
void CGame::DeleteTicketAt(int index)
{
	if(index >= 0 && index <TEXTURE_MANAGER_MAX)
	{
		_texture_ticket_used[index] = false;
	}
}
#endif

//hao.ly add
void CGame::setUseOptimizeDrawModule(BYTE typeOptimize)
{
	_typeOtimizeDrawModule = typeOptimize;
}
INT_TIME CGame::GetTimePasses()
{
	return (time(NULL)-_syn_time_start);
}
void CGame::SetTimeSynStart()
{
	_syn_time_start = time(NULL);
}
//////////////////////////////////////////
/////////////////////////////////////////SPRITE LOAD SYTEMMMMMMMMMMMMMMMMMMMMMMMMM:


int CGame::GetSpriteVerID(int spriteID)
{
	return _syn_sprite_index[spriteID*VERSION_NUMS + _current_version];
}
void CGame::UnloadSpriteVer(int spriteID,int version)
{
	_sprite_loaded[spriteID] = 0;
	UnloadSprite(_syn_sprite_index[spriteID*VERSION_NUMS + version]);
}
void CGame::UnloadSpriteVer(int spriteID)
{
	UnloadSpriteVer(spriteID,_current_version);
}
void CGame::UnloadSpriteTextureVer(int spriteID)
{
	CSprite* sprite = GetSpriteVer(spriteID);
	if(sprite != NULL){
		sprite->unloadTexture(G());
	}
}
void CGame::RegUnloadSpriteVer(int spriteID, int version){
	RegUnloadSprite(_syn_sprite_index[spriteID*VERSION_NUMS + version]);
}
void CGame::RegUnloadSpriteVer(int spriteID){
	RegUnloadSpriteVer(spriteID, _current_version);
}
void CGame::LoadSpriteVer(int spriteID,int version,int antialias,bool truecolor,int pal)
{
    
    
	printf("\nLoadSprite =%d",_syn_sprite_index[spriteID*VERSION_NUMS + version]);
	_sprite_loaded[spriteID] = 1;
	_sprite_optimized_loaded[spriteID] = truecolor;
	_sprite_antialias_loaded[spriteID] = antialias;
	_sprite_from_rbga[spriteID] = false;
	LoadSprite(_syn_sprite_index[spriteID*VERSION_NUMS + _current_version],true,antialias,pal,truecolor);
    _sprite_loaded[spriteID] = 2;
}
CSprite* CGame::GetSpriteVer(int spriteID, int version)
{
	//printf("\n_syn_sprite_index[spriteID*VERSION_NUMS + version] = %d",_syn_sprite_index[spriteID*VERSION_NUMS + version]);
	return GetSprite(_syn_sprite_index[spriteID*VERSION_NUMS + version]);
}
void CGame::LoadSpriteCurrentVerFromFile(int spriteID,const char* file_name,bool true_color)
{
	//LoadSpriteCurrentVer(spriteID,true,true_color,0,file_name);
	
	FILE * stream = NULL;
	stream = fopen(file_name, "rb");
	
	if (stream != NULL)
	{
		fseek(stream,0,SEEK_END);
		int size = ftell(stream);
		BYTE* buff = new BYTE[size];
		fseek(stream,0,SEEK_SET);
		fread((void *)buff,sizeof(BYTE),size,stream);
		fclose(stream);
		LoadSpriteCurrentVer(spriteID,buff,size,true,true_color);
		//printf("\n size size size = %d",size);
		delete[] buff;
	}
	
}
void CGame::LoadSpriteCurrentVerFromRGBA(int spriteID,BYTE* rgba,int width,int height,int antialias,bool truecolor,int pal)
{
	_sprite_loaded[spriteID] = 1;
	_sprite_optimized_loaded[spriteID] = truecolor;
	_sprite_antialias_loaded[spriteID] = antialias;
	_sprite_from_rbga[spriteID] = true;
	LoadSpriteFromRGBA(_syn_sprite_index[spriteID*VERSION_NUMS + _current_version],rgba,width,height,true,antialias,pal,truecolor);
    _sprite_loaded[spriteID] = 2;
}

void CGame::LoadSpriteCurrentVer(int spriteID,BYTE* buffer,int length,bool antialias,bool truecolor,int pal)
{
	_sprite_loaded[spriteID] = 1;
	_sprite_optimized_loaded[spriteID] = truecolor;
	_sprite_antialias_loaded[spriteID] = antialias;
	_sprite_from_rbga[spriteID] = false;
	LoadSprite(_syn_sprite_index[spriteID*VERSION_NUMS + _current_version],buffer,length,antialias,truecolor,pal);
    _sprite_loaded[spriteID] = 2;
}

void CGame::LoadSpriteCurrentVer(int spriteID,int antialias,bool truecolor,int pal,const char *file_name)
{
	
	//printf("\nLoadingggggggggggggggggggggg _syn_sprite_index[spriteID*VERSION_NUMS + _current_version] id = %d",_syn_sprite_index[spriteID*VERSION_NUMS + _current_version]);
	//printf("\n_current_version = %d",_current_version);
	_sprite_loaded[spriteID] = 1;
	_sprite_optimized_loaded[spriteID] = truecolor;
	_sprite_antialias_loaded[spriteID] = antialias;
	_sprite_from_rbga[spriteID] = false;
	LoadSprite(_syn_sprite_index[spriteID*VERSION_NUMS + _current_version],true,antialias,pal,truecolor,true,file_name);
    _sprite_loaded[spriteID] = 2;
}

void CGame::LoadSpriteCurrentVerFromThread(int spriteID,int antialias,bool truecolor,int pal,const char *file_name)
{
    _sprite_loaded[spriteID] = 1;
    _sprite_optimized_loaded[spriteID] = truecolor;
    _sprite_antialias_loaded[spriteID] = antialias;
    _sprite_from_rbga[spriteID] = false;
    LoadSpriteInThread(_syn_sprite_index[spriteID*VERSION_NUMS + _current_version],true,antialias,pal,truecolor,true,file_name);
    _sprite_loaded[spriteID] = 2;
}

CSprite* CGame::GetSpriteVer(int spriteID)
{
	// printf("\n_current_version = %d",_current_version);
	return GetSpriteVer(spriteID,_current_version);
}

float CGame::GetScreenWidth()
{
	return _game_context.Size.W;
}

float CGame::GetScreenHeight()
{
	return _game_context.Size.H;
}

float CGame::_sin_table[MAX_TABLE];
float CGame::_cos_table[MAX_TABLE];

void CGame::InitSinTable()
{
    
	for(int i = 0; i < MAX_TABLE; i++)
	{
        float angle = 0.1*i;
		_sin_table[i] = sin((float)(angle*PI/180));
	}
}
void CGame::InitCosTable()
{
    
	for(int i = 0; i < MAX_TABLE; i++)
	{
        float angle = 0.1*i;
        _cos_table[i] = cos((float)(angle*PI/180));
	}
}

void CGame::InitMathTable()
{
	InitSinTable();
	InitCosTable();
}

float CGame::Math_Cos(float angle)
{
	int offangle = angle - (int)angle;
	angle = ((int)angle%360) + offangle;
	while(angle < 0)
	{
		angle += 360;
	}
	while(angle >= 360)
	{
		angle -= 360;
	}
	return _cos_table[(int)(angle/0.1)];
}

float CGame::Math_Sin(float angle)
{
	int offangle = angle - (int)angle;
	angle = ((int)angle%360) + offangle;
	while(angle < 0)
	{
		angle += 360;
	}
	while(angle >= 360)
	{
		angle -= 360;
	}
	return _sin_table[(int)(angle/0.1)];
}

float CGame::Math_AngleAbs(float angle)
{
	float offset =(angle - (int)angle);
	angle = (int)(angle)%360 + offset;
	if(angle < 0 || angle >= 360)
	{
		while(angle < 0)
		{
			angle += 360;
		}
		while(angle >= 360)
		{
			angle -= 360;
		}
	}
	return angle;
}
void CGame::SetGlobalContext(float tx, float ty,float scalex, float scaley){
	_game_context.TranslateX = tx;
	_game_context.TranslateY = ty;
	_game_context.ScaleX = scalex;
	_game_context.ScaleY = scaley;
	float device_w = k_SCREEN_WIDTH > k_SCREEN_HEIGHT ? (_device.ScreenSize.W>_device.ScreenSize.H ? _device.ScreenSize.W : _device.ScreenSize.H) : (_device.ScreenSize.W>_device.ScreenSize.H ? _device.ScreenSize.H : _device.ScreenSize.W);
	float device_h = (device_w == _device.ScreenSize.W) ? _device.ScreenSize.H : _device.ScreenSize.W;

	float real_game_w = k_SCREEN_WIDTH * scalex;
	float real_game_h = k_SCREEN_HEIGHT * scaley;
	//printf("devicew:%0.2f deviceh:%0.2f real_game_w:%0.2f real_game_h:%0.2f\n", device_w, device_h,real_game_w,real_game_h);
	float dw = device_w - real_game_w;
	float dh = device_h - real_game_h;
	//printf("dw: %0.2f dh:%0.2f\n", dw, dh);
	float dx = dw/2;
	float dy = dh/2;
	_game_context.BorderLeft = dx>0?0:dx;
	_game_context.BorderTop = dy>0?0:dy;
	_game_context.BorderRight = _game_context.BorderLeft + dw + real_game_w;
	_game_context.BorderBottom = _game_context.BorderTop + dh + real_game_h;
	//printf("bound %0.2f %0.2f %0.2f %0.2f\n",_game_context.BorderLeft,  _game_context.BorderRight,_game_context.BorderTop,_game_context.BorderBottom );
	////_g->SetGlobalContext(tx, ty,scalex, scaley);
}

void CGame::SetByteAt(char* buffer,int offset, int value)
{
	BYTE data = value&0xff;
	if(value < 0)
	{
		return;
	}
	memcpy(buffer + offset,&data,1);
}

char CGame::GetByteAt(char* buffer,int offset)
{
	BYTE value = 0;
	memcpy(&value,buffer + offset,1);
	return value;
}

void CGame::SetCharAt(char* buffer,int offset, int value)
{
	char data = value&0xff;
	memcpy(buffer + offset,&data,1);
}

char CGame::GetCharAt(char* buffer,int offset)
{
	char value = 0;
	memcpy(&value,buffer + offset,1);
	return value;
}

void CGame::SetInt16At(char* buffer,int offset, int value)
{
	/*if(value < 0)
	{
		return;
	}*/
	short data = value&0xFFFF;
	memcpy(buffer + offset,&data,2);
}
int	 CGame::GetInt16At(char* buffer,int offset)
{
	short value = 0;
	memcpy(&value,buffer + offset,2);
	return value;
}
void CGame::SetInt32At(char* buffer,int offset, int value)
{
	/*if(value < 0)
	{
		return;
	}*/
	//printf("\noffset = %d",offset);
	int data = value&0xFFFFFFFF;
	memcpy(buffer + offset,&data,4);
}

int CGame::GetInt32At(char* buffer,int offset)
{
	//printf("\nGet offset = %d",offset);
	int value = 0;
	memcpy(&value,buffer + offset,4);
	return value;
}

void CGame::SetInt64At(char* buffer,int offset, INT_TIME value)
{
	/*if(value < 0)
	{
		return;
	}*/
	//printf("\noffset = %d",offset);
	INT_TIME data = value&0xFFFFFFFFFFFFFFFF;
	memcpy(buffer + offset,&data,8);
}
INT_TIME CGame::GetInt64At(char* buffer,int offset)
{
	INT_TIME value = 0;
	memcpy(&value,buffer + offset,8);
	return value;
}


int CGame::_key_1_state = 0;
int CGame::_key_2_state = 0;
int CGame::_key_3_state = 0;
void CGame::ResetVirtualKeys()
{
    _key_1_state = 0;
    _key_2_state = 0;
    _key_3_state = 0;
}

void CGame::Key1Press()
{
    _key_1_state = 1;
}
void CGame::Key2Press()
{
    _key_2_state = 1;
}
void CGame::Key3Press()
{
    _key_3_state = 1;
}
bool CGame::HasKey1Press()
{
    return  _key_1_state;
}
bool CGame::HasKey2Press()
{
    return  _key_2_state;
}
bool CGame::HasKey3Press()
{
    return  _key_3_state;
}
void CGame::ResetKey1()
{
    _key_1_state = 0;
}
void CGame::ResetKey2()
{
    _key_2_state = 0;
}
void CGame::ResetKey3()
{
    _key_3_state = 0;
}
