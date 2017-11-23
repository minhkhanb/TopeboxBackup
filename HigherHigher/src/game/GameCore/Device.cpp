#include "pch.h"
#include "CGame.h"
#include "Device.h"
#include "Os_Functions.h"
#include "string.h"
#include <time.h>
#include <sys/stat.h>
#if USE_COMPRESSOR_DATA_INGAME
#include "../GamePlay/GameObject/Compressor_Exporter.h"
#endif
#if USE_PINEGAMEPLAYREPLAY
 int Device::ReplayGamePlayState = RECORD_GAMEPLAY_IDLE_STATE;
#endif

#if !defined(_WINDOWS_PHONE8)
#include <PineEngine.h>
PE_USING_ENGINE;
#endif

extern "C" Orientation GetPhoneOrientation();
extern "C" Orientation GetViewOrientation();
int Device::_camera_state = CAMERA_FREE_STATE;
char* Device::_keyboard_character = new char[KEYBOARD_BUFFER_LENGTH+1];
extern "C" void OS_DisableReturnKey(bool disable);
extern "C" void OS_ClearTextBuffer();
bool Device::disableReturnKey = false;;

double Device::_device_motionX = 0;
double Device::_device_motionY = 0;
double Device::_device_motionZ = 0;

double Device::_device_root_motionX = 0;
double Device::_device_root_motionY = 0;
double Device::_device_root_motionZ = 0;

double Device::_motion_view2D_x = 640/2;
double Device::_motion_view2D_y = 480/2;
double Device::_motion_view2D_z = 0;
double Device::_motion_view2D_w = 640;
double Device::_motion_view2D_h = 480;


double Device::_motion_balance_angle_x = 0;
double Device::_motion_balance_angle_y = 0;
double Device::_motion_balance_angle_z = 0;

double Device::_motion_balance_x = 0;
double Device::_motion_balance_y = 0;
double Device::_motion_balance_z = 0;

double Device::_angle_moving = 0;


double Device::_logic_motion_speed = 0;
double Device::_logic_scroll_x = 0;
double Device::_logic_scroll_y = 0;
double Device::_use_physic_view = true;
bool Device::_safe_to_shake = false;

//sensitivity
#ifndef SENSITIVITY_ROUND
#define SENSITIVITY_ROUND 0.1f
#define SENSITIVITY_MULTIX 1.0f
#define SENSITIVITY_MULTIY (SENSITIVITY_MULTIX)
#define SENSITIVITY_SPEED_BASE (1000)
#define SENSITIVITY_SPEED_MIN (3.0)
#define SENSITIVITY_ACCELL (0.1)
#endif
extern "C" bool OS_DownloadFile(const char* url, const char* file_name,int time_out);
extern "C" int OS_GetNetworkSupport();
extern "C" bool LZMA_extractor(const char* input,const char* file_name_inzip,const char *output);
extern "C" void OS_LinkToRatingApp(unsigned long apple_id);
extern "C" void iOS_ShowRatingBasic();
void Device::OS_ShowRatingBasic()
{
#ifdef MAC_OS
    iOS_ShowRatingBasic();
#endif
}
void Device::LinkToRatingApp(unsigned long apple_id)
{
#ifdef MAC_OS
    OS_LinkToRatingApp(apple_id);
#endif
}
int Device::GetHourOfDay()
{
    time_t t = time(NULL);
    struct tm *tm_struct = localtime(&t);
    return tm_struct->tm_hour;
}
int Device::GetDayOfYear()
{
    time_t t = time(NULL);
    struct tm *tm_struct = localtime(&t);
    return tm_struct->tm_yday;
}
bool Device::DownloadFile(const char* url, const char* file_name,int time_out)
{
#ifdef MAC_OS
    return OS_DownloadFile(url,file_name,time_out);
#else
    return false;
#endif
}
/*bool Device::PINEZ_Extractor(const char* input,const char* file_name_inzip,const char *output)
{
#ifdef MAC_OS
    return LZMA_extractor(input,file_name_inzip,output);
#else
    return true;
#endif
}*/
bool Device::PINEZ_Extractor(std::wstring input,std::string file_name_inzip,std::wstring output)
{
    bool result = false;
    try{
        
#if !defined(_WINDOWS_PHONE8) && !defined (ANDROID_OS) && !defined(WIN32)
        result =  C7z::extractFile(input, EFileLocation::PATH, output, EFileLocation::PATH, file_name_inzip);
#endif
        
        
    }catch(const char* ex)
    {
        PDEBUG("extract error:%s\n", ex);
    }
    return result;
}
 int Device::GetNetworkSupport()
{
#ifdef MAC_OS
    return OS_GetNetworkSupport();
#else
    return NETWORK_TYPE_UNKOWN;
#endif
}
 void Device::SetAppInstalled(const char* apple_id, int with_version)
{
    char buffer[128];
    char file_path[1024];
    sprintf(file_path,"%s.pineads", OS_GetDocumentFilePath(apple_id));
    FILE* f= fopen(file_path,"wb");
    if(f)
    {
        CGame::_this->SetInt32At(buffer, 0, with_version);
        fwrite(buffer,sizeof(char),128,f);
        fclose(f);
    }
}
bool Device::HasAppInstalled(const char* apple_id, int with_version)
{
    char file_path[1024];
    sprintf(file_path,"%s.pineads", OS_GetDocumentFilePath(apple_id));
    //printf("\nFile checked = %s",file_path);
    FILE* f =  fopen(file_path,"r");
    if(f)
    {
        if(with_version > 0)
        {
            fseek (f, 0, SEEK_END);   // non-portable
            long size=ftell (f);
            fseek(f,0,SEEK_SET);
            if(size > 0)
            {
                char buff[128];
                long len = size>=128?size:128;
                fread(buff,1,len,f);
                int version = CGame::_this->GetInt32At(buff, 0);
                //printf("\n Version app_install = %d",version);
                if(version != with_version)
                {
                    fclose(f);
                    return false;
                }
            }
            else
            {
                fclose(f);
                return false;
            }
                
        }
        fclose(f);
        return true;
    }
    //return OS_HasAppInstalled(boudble_id);
    return false;
}

void  Device::ClearTextBuffer()
{
    sprintf(_keyboard_character,"");
    OS_ClearTextBuffer();
}
void Device::DisableReturnKey(bool disable)
{
	OS_DisableReturnKey(disable);
	disableReturnKey = disable;
}
extern "C" int OS_SetTextMax(int max);
extern "C" bool OS_IsSystemForceClose();
bool Device::HasForceCloseKeyboard()
{
#ifdef MAC_OS
    return OS_IsSystemForceClose();
#else
    return false;
#endif
}
void Device::SetTextBufferMax(int len_max)
{
#ifdef MAC_OS
    OS_SetTextMax(len_max);
#endif
}
void Device::ShowKeyboard(int x, float y, float w, float h)
{
	OS_TextInputStart(_keyboard_character,KEYBOARD_BUFFER_LENGTH,x,y,w,h);
}
void Device::CloseKeyboard()
{
	OS_TextInputClose();
}
void Device::SwitchCamera()
{
	OS_SwitchCamera();
}
void Device::CaptureCamera()
{
	_camera_state = CAMERA_TAKING_STATE;
   OS_CaptureCamera();
}
void Device::DisableCamera()
{
	OS_DisableCamera();
}
void Device::ShowCamera()
{
	OS_ShowCamera();
}
void Device::ShowPictureLibrary()
{
	_camera_state = CAMERA_PICKING_STATE;
	OS_ShowPicLibrary();
}
Device::Device() :
	_is_safe(true)
{
	_user_agent = NULL;
	_model = NULL;
	DisplayProperties.ColorDepth = PineDisplayColorDepth::ColorDepthUnknown;
	 _camera_state = CAMERA_FREE_STATE;
}


Device::Device(const Device& other):
	_is_safe(true)
{
	_user_agent = NULL;
	if (other._user_agent != NULL){
		int len = strlen(other._user_agent);
		_user_agent = new char[len + 1];
		sprintf(_user_agent, "%s", other._user_agent);
		_user_agent[len] = '\0';
	}
	_model = NULL;
	if (other._model != NULL){
		int len = strlen(other._model);
		_model = new char[len + 1];
		sprintf(_model, "%s", other._model);
		_model[len] = '\0';
	}
	DeviceScreenSize = other.DeviceScreenSize;
	ScreenSize = other.ScreenSize;
	IsSupportAppleGameCenter = other.IsSupportAppleGameCenter;
	IsSupportTwitter = other.IsSupportTwitter;
	IsSupportFacebook = other.IsSupportFacebook;
	Device_Orientation = other.Device_Orientation;
	DisplayProperties = other.DisplayProperties;
	_camera_state = CAMERA_FREE_STATE;
}


Device::~Device(){
	if(_user_agent != NULL)
	{
		delete[] _user_agent;
		_user_agent = NULL;
	}
	if (_model != NULL)
	{
		delete[] _model;
		_model = NULL;
	}
}

bool Device::IsNetworkAvailable(){
	return ::OS_IsNetworkAvailable();
}

const char* Device::getDeviceAgent(){
	return _user_agent;
}

void Device::setDeviceAgent(const char* agent){
	if (_user_agent != NULL)delete[] _user_agent;
	_user_agent = NULL;
	if (agent != NULL){
		int len = strlen(agent);
		_user_agent = new char[len + 1];
		sprintf(_user_agent, "%s", agent);
		_user_agent[len] = '\0';
	}
}

const char* Device::getDeviceModel(){
	return _model;
}

void Device::setDeviceModel(const char* model){
	if (_model != NULL) delete[] _model;
	_model = NULL;
	if (model != NULL){
		int len = strlen(model);
		_model = new char[len + 1];
		sprintf(_model, "%s", model);
		_model[len] = '\0';
	}
}

void Device::operator=(const Device other){
	if(_user_agent != NULL)delete[] _user_agent;
	_user_agent = NULL;
	if(other._user_agent != NULL){
		int len = strlen(other._user_agent);
		_user_agent = new char[len+1];
		sprintf(_user_agent,"%s", other._user_agent);
		_user_agent[len] = '\0';
	}
	if (_model != NULL) delete[] _model;
	_model = NULL;
	if (other._model != NULL){
		int len = strlen(other._model);
		_model = new char[len + 1];
		sprintf(_model, "%s", other._model);
		_model[len] = '\0';
	}
	DeviceScreenSize = other.DeviceScreenSize;
	ScreenSize = other.ScreenSize;
	IsSupportAppleGameCenter = other.IsSupportAppleGameCenter;
	IsSupportTwitter = other.IsSupportTwitter;
	IsSupportFacebook = other.IsSupportFacebook;
	Device_Orientation = other.Device_Orientation;
	DisplayProperties = other.DisplayProperties;
	return;
}

bool Device::IsSafe(){
	return _is_safe;
}

void Device::NoticeUnSafe(){
	_is_safe = false;
}



void Device::InitDeviceMotion(double viewX,double viewY, double viewW, double viewH)
{
    
     Device::_motion_view2D_x = viewX;
     Device::_motion_view2D_y = viewY;
     Device::_motion_view2D_z = 0;
     Device::_motion_view2D_w = viewW;
     Device::_motion_view2D_h = viewH;
    
    _device_motionX = OS_GetAccelX();
    _device_motionY = OS_GetAccelY();
    _device_motionZ = OS_GetAccelZ();
    SetRootOfMotion(_device_motionX,_device_motionY,_device_motionZ);
    _logic_scroll_x = viewX;
    _logic_scroll_y = viewY;
    _safe_to_shake = true;
}
double Device::CaculateMotionSpeedX()
{
    double xx = GetCursorPhysicX();
    return CMath::DISTANCE(xx,0,_motion_view2D_x,0)/CMath::DISTANCE(0,0,_motion_view2D_x,0);
}
double Device::CaculateMotionSpeedY()
{
   
    double yy = GetCursorPhysicY();
    
    return CMath::DISTANCE(0,yy,0,_motion_view2D_y)/CMath::DISTANCE(0,0,0,_motion_view2D_y);
}
double Device::CaculateMotionSpeedZ()
{
    float zz = GetDeviceMotionTransformZ();
    if(zz > 0)
    {
        return zz;
    }
    return -zz;
    
}
double Device::CaculateMotionXDivY()
{
    return _motion_view2D_w/_motion_view2D_h;
}
double Device::CaculateMotionYDivX()
{
    return _motion_view2D_h/_motion_view2D_w;
}
double Device::CaculateMotionSpeed()
{
    double xx = GetCursorPhysicX();
    double yy = GetCursorPhysicY();
    
    return CMath::DISTANCE(xx,yy,_motion_view2D_x,_motion_view2D_y)/CMath::DISTANCE(0,0,_motion_view2D_x,_motion_view2D_y);
    
}
void Device::SetRootOfMotion(double x, double y, double z)
{
    _device_root_motionX = x;
    _device_root_motionY = y;
    _device_root_motionZ = z;
}


double Device::GetDeviceMotionTransformX()
{
    float xx = GetDeviceMotionX();
    float angle = asin(xx)*180.0/PI;
    angle = angle -_motion_balance_angle_x;
    xx = CMath::SIN(angle);
    
    
   
    return xx;
}

double Device::GetDeviceMotionTransformY()
{
    float yy = GetDeviceMotionY();
    float angle = asin(yy)*180.0/PI;
    
    angle = angle -_motion_balance_angle_y;
    yy = CMath::SIN(angle);
    
    
    return yy;
}
 double Device::GetDeviceMotionTransformZ()
{
    float zz = GetDeviceMotionZ();
    float angle = asin(zz)*180.0/PI - _motion_balance_angle_z;
    zz = -CMath::SIN(angle);
    
    return zz;
}


double Device::GetDeviceMotionX()
{
    
    _device_motionX = OS_GetAccelX();
    
    if(GetViewOrientation() == Orientation::Landscape_270 || GetViewOrientation() == Orientation::Landscape_90)
    {
        if(OS_GetAccelZ() < 0)
        {
       
            _device_motionX = -OS_GetAccelY();
        }
        else
        {
            _device_motionX = OS_GetAccelY();
            
        }
        
    }

   
  
    
    return _device_motionX;
}
double Device::GetDeviceMotionY()
{
    _device_motionY = OS_GetAccelY();

    
    if(GetViewOrientation() == Orientation::Landscape_270 || GetViewOrientation() == Orientation::Landscape_90)
    {
        if(OS_GetAccelZ() < 0)
        {
            _device_motionY = -OS_GetAccelX();
        }
        else
        {
            _device_motionY = OS_GetAccelX();
        }
       
    }
    
   
    return _device_motionY;
}

double Device::GetDeviceMotionZ()
{
    _device_motionZ = OS_GetAccelZ();
    return _device_motionZ;
}

double Device::GetCursorPhysicX()
{
    //printf("\nGetDeviceMotionTransformX() = %f",GetDeviceMotionTransformX());
    float transform = GetDeviceMotionTransformX()*SENSITIVITY_MULTIX;
    //(((int)(GetDeviceMotionTransformX()*SENSITIVITY_ROUND))*1.0f/SENSITIVITY_ROUND)*SENSITIVITY_MULTIX;
    //printf("\ntransform = %f",transform);
    float xx = (_motion_view2D_x + transform*_motion_view2D_w/2);
    
    
    return xx;
}
double Device::GetCursorPhysicY()
{
    float transform = GetDeviceMotionTransformY()*SENSITIVITY_MULTIY;
    //(((int)(GetDeviceMotionTransformY()*SENSITIVITY_ROUND))*1.0f/SENSITIVITY_ROUND)*SENSITIVITY_MULTIY;
    float yy = (_motion_view2D_y + transform*_motion_view2D_h/2);
   
    return yy;
}
double Device::GetCursorViewX()
{
    //return GetCursorPhysicX();
    return _logic_scroll_x;
}
double Device::GetCursorViewY()
{
    //return GetCursorPhysicY();
    return _logic_scroll_y;
}


double Device::SetCurrentBalance()
{
    _motion_balance_angle_z = 0;
    _motion_balance_z = GetDeviceMotionZ();
    _motion_balance_angle_z = (asin(_motion_balance_z)*180.0/PI);//CMath::ARCSIN(GetDeviceMotionZ());
    
    _motion_balance_angle_x = 0;
    _motion_balance_x = GetDeviceMotionX();
    _motion_balance_angle_x = (asin(_motion_balance_x)*180.0/PI);//CMath::ARCSIN(GetDeviceMotionX());
    
    _motion_balance_angle_y = 0;
    _motion_balance_y = GetDeviceMotionY();
    _motion_balance_angle_y = (asin(_motion_balance_y)*180.0/PI);//CMath::ARCSIN(GetDeviceMotionY());
    
    

    
    //printf("\n_motion_balance_x = %f, _motion_balance_y = %f, _motion_balance_z = %f",_motion_balance_x,_motion_balance_y,_motion_balance_z);
    
    return 0;
}
void Device::DeviceUpdateMotion()
{
    if(_use_physic_view)
    {
        //printf("\nGetDeviceMotionZ  = %f",GetDeviceMotionZ());
//printf("\nOS_GetAccelX  = %f, OS_GetAccelY = %f,OS_GetAccelZ = %f",OS_GetAccelX(),OS_GetAccelY(),OS_GetAccelZ());
         //printf("\n_motion_balance_x = %f, _motion_balance_y = %f, _motion_balance_z = %f",_motion_balance_x,_motion_balance_y,_motion_balance_z);
        _logic_scroll_x = GetCursorPhysicX()*SENSITIVITY_ACCELL + _logic_scroll_x*(1.0-SENSITIVITY_ACCELL);
        _logic_scroll_y = GetCursorPhysicY()*SENSITIVITY_ACCELL + _logic_scroll_y*(1.0-SENSITIVITY_ACCELL);

    }
    else
    {
        double xx = GetCursorPhysicX();
        double yy = GetCursorPhysicY();
    
        double sx = CaculateMotionSpeedX();
        double sy = CaculateMotionSpeedY();
    
   
    
        double angle = CMath::ANGLE(xx, yy,_motion_view2D_x,_motion_view2D_y);

        if(sx > 0.5 || sy > 0.5)
        {
            _logic_scroll_x += (-CMath::COS(angle)*SENSITIVITY_SPEED_MIN);
            _logic_scroll_y += (-CMath::SIN(angle)*SENSITIVITY_SPEED_MIN);
        }
    
        _logic_scroll_x += (-CMath::COS(angle)*sx*SENSITIVITY_SPEED_BASE);
        _logic_scroll_y += (-CMath::SIN(angle)*sy*SENSITIVITY_SPEED_BASE);
    
    }
    if(_logic_scroll_x < 0)
    {
        _logic_scroll_x = 0;
    }
    if(_logic_scroll_x > _motion_view2D_w)
    {
        _logic_scroll_x = _motion_view2D_w;
    }
    
    if(_logic_scroll_y < 0)
    {
        _logic_scroll_y = 0;
    }
    if(_logic_scroll_y > _motion_view2D_h)
    {
        _logic_scroll_y = _motion_view2D_h;
    }
    
    //printf("\nUpdate Movetion loop ZZ =%f",GetDeviceMotionTransformZ());
}


bool Device::IsEnableScaleView()
{
	return OS_EnableScaleView();
}
bool Device::ScaleView(float scalex, float scaley)
{
	return OS_ScaleView(scalex, scaley);
}
#if defined (MAC_OS)
extern "C" bool  OS_IsIpadDevice();
bool Device::IsIpadDevice()
{
    return OS_IsIpadDevice();
}
#endif
#if USE_PINEGAMEPLAYREPLAY
extern "C" void OS_ForceStopGameplayRecord();
extern "C" void OS_StartGameplayRecord();
extern "C" void OS_StopGameplayRecord();
extern "C" void OS_ShareRecordGamePlay();
bool Device::IsChangingSetting = 0;
bool Device::CurrentReplaySetting = 0;
int Device::CurrentReplayType = 0;
int Device::FrameReplayToStop = 0;
extern "C" bool OS_SupportReplay();
void Device::LoadReplaySetting()
{
    char file_path[1024];
     sprintf(file_path,"%s", OS_GetDocumentFilePath("replay.setting"));
    FILE* f =  fopen(file_path,"rb");
    if(f)
    {
        char buff[128];
        fread(buff,1,128,f);
        int offset = 0;
        Device::CurrentReplaySetting = CGame::_this->GetByteAt(buff, offset);
        offset++;
        Device::CurrentReplayType = CGame::_this->GetByteAt(buff, offset);
        offset++;
    }
    else
    {
        SaveReplaySetting();
    }
    
}
void Device::SaveReplaySetting()
{
    char buffer[128];
    char file_path[1024];
    sprintf(file_path,"%s", OS_GetDocumentFilePath("replay.setting"));
    FILE* f= fopen(file_path,"wb");
    if(f)
    {
        
        int offset = 0;
        CGame::_this->SetByteAt(buffer, offset, Device::CurrentReplaySetting);
        offset++;
        CGame::_this->SetByteAt(buffer, offset, Device::CurrentReplayType);
        offset++;
        fwrite(buffer,1,128,f);
        fclose(f);
    }
}
void Device::OnOffReplay()
{
    if(Device::CurrentReplaySetting)
    {
        Device::CurrentReplaySetting = false;
        Device::CurrentReplayType = !(Device::CurrentReplayType);
    }
    else
    {
        Device::IsChangingSetting = true;
        StartRecordGameplay();
        
    }
}

bool Device::HasDeviceSupportReplay()
{
#ifdef MAC_OS
    return OS_SupportReplay();
#else
    return false;
#endif
    
}
void Device::ShareRecordGameplay()
{
#ifdef MAC_OS
    OS_ShareRecordGamePlay();
#endif
}
void Device::StartRecordGameplay()
{
#ifdef MAC_OS
    OS_StartGameplayRecord();
#endif
}
void Device::ForceStopRecordGameplay()
{
#ifdef MAC_OS
    OS_ForceStopGameplayRecord();
#endif
    
    
}
void Device::StopRecordGameplay()
{
#ifdef MAC_OS
    OS_StopGameplayRecord();
#endif
}
#endif


#ifndef MAX_CHANNEL
#define MAX_CHANNEL 3
#endif
//play sfx queue
int Device::_queue_sound_top = 0;
int Device::_queue_sound_rear = 0;
int Device::_queue_sound_ids[MAX_QUEUE];
int Device::_queue_sound_skip[MAX_QUEUE];
long Device::_queue_sound_frame[MAX_QUEUE];
void Device::PlayQueueSFX(int soundid, int frameToSkip)
{
    int next = (_queue_sound_rear+1)%MAX_QUEUE;
    if(next != _queue_sound_top)
    {
        //check exist;
        int top = _queue_sound_top;
        int rear = _queue_sound_rear;
        while(top != rear)
        {
            if(_queue_sound_ids[rear] == soundid && _queue_sound_frame[rear] == CGame::_this->_frame_counter)
            {
                return;
            }
        }
        _queue_sound_ids[_queue_sound_rear] = soundid;
        _queue_sound_skip[_queue_sound_rear] = frameToSkip;
        _queue_sound_frame[_queue_sound_rear] = CGame::_this->_frame_counter;
        _queue_sound_top = next;
    }
    
}
void Device::UpdateAndPlayQueueSFX()
{
    int count = 0;
    while(_queue_sound_rear != _queue_sound_top)
    {
        int index = _queue_sound_rear;
        _queue_sound_rear = (_queue_sound_rear+1)%MAX_QUEUE;
        if(_queue_sound_skip[index] <= 0 || (_queue_sound_skip[index] >= 0 && CGame::_this->_frame_counter - _queue_sound_frame[index] < _queue_sound_skip[index]))
        {
            CGame::_this->PlaySFX(_queue_sound_ids[index]);
        }
        count++;
        if(count >= MAX_CHANNEL)
        {
            break;
        }
    }
}

extern "C" bool Os_IsDeviceWeak();
bool Device::IsDeviceWeak()
{
#ifdef MAC_OS
    return Os_IsDeviceWeak();
#else
    return false;
#endif
}




#if USE_PINE_DATE_SYSTEM
#define PST (-8)
bool Device::_time_be_interupted = false;
long Device::_pine_tsecond = 0;
long Device::_pine_tminute = 0;
long Device::_pine_thour = 0;
long Device::_pine_tday = 0;
long Device::_pine_tmonth;
long Device::_pine_tyear = 0;

double Device::TimeDiff(time_t t1, time_t t2)
{
    return difftime(t1,t2);
}
void Device::PrintTime(time_t time)
{
    struct tm * timeinfo;
    char buffer [80];
    timeinfo = gmtime(&time);
    strftime (buffer,80,"Now it's %y/%m/%d.",timeinfo);
    printf("\nbuffer = %s",buffer);
}
time_t Device::MakeTime(long sec,long min, long hour, long day, long month, long year)
{
    time_t t = time(NULL);
    struct tm *tm_struct = gmtime(&t);
    tm_struct->tm_year = year;
    tm_struct->tm_mon = month;
    tm_struct->tm_mday = day;
    tm_struct->tm_hour = hour;
    tm_struct->tm_min = min;
    tm_struct->tm_sec = sec;
    return mktime(tm_struct);
}
bool Device::AddSecond()
{
    
    return false;
    
    
}
time_t Device::GetPTTime()
{
    time_t t;
    time(&t);
    time_t gap = (PST)*60*60;
    return t+gap;
}
void Device::RefeshTimer()
{
    time_t tchange = GetPTTime();
    //ASSERT(tchange != (time_t)-1);
    struct tm *tm_change = gmtime(&tchange);
    //printf("\n tm_change->tm_hour = %d",tm_change->tm_hour);
    _pine_tsecond = tm_change->tm_sec;
    _pine_tminute = tm_change->tm_min;
    _pine_thour = tm_change->tm_hour;
    _pine_tday = tm_change->tm_mday;
    _pine_tmonth = tm_change->tm_mon;
    _pine_tyear = tm_change->tm_year;
    //printf("\nREFESH Time: %ld:%ld:%ld Date: %ld/%ld/%ld",_pine_tsecond,_pine_tminute,_pine_thour,_pine_tday,_pine_tmonth,_pine_tyear);
    
}
void Device::UpdateTimer()
{
    //CLOCKS_PER_SEC
    if(CGame::_this->_frame_counter%60 == 0)
    {
        RefeshTimer();
        printf("\n GetTimeZone = %d",GetTimeZone());
    }
    
    
}
void Device::InitTimer()
{
    RefeshTimer();
}

int Device::GetTimeZone()
{
    time_t t = time(NULL);
    struct tm *tm = gmtime(&t);
    int hour1 = tm->tm_hour;
    tm = localtime(&t);
    int hour2 = tm->tm_hour;
    if(hour2 - hour1 < -12)
    {
        hour2 += 24;
    }
    
    return hour2-hour1;
}
#endif

#if  NOTIFICATION_MAX

int Device::_current_num_notification = 0;
char Device::_notification_string[NOTIFICATION_MAX][1024];
long long Device::_notification_time[NOTIFICATION_MAX];
extern "C" void OS_DeviceClearAllPushNotify();
extern "C" void OS_DeviceAskPushNotify();
extern "C" void OS_DevicePushNotify();
void Device::AskShowNotification()
{
    OS_DeviceAskPushNotify();
}
void Device::AddNotification(const char* notification,long long time, bool replay_if_exits)
{
    if(replay_if_exits)
    {
        for(int i = 0; i < _current_num_notification; i++)
        {
            if(strcmp(notification,_notification_string[i]) == 0)
            {
                _notification_time[i] = time;
                return;
            }
        }
    }
    if(_current_num_notification < NOTIFICATION_MAX)
    {
        sprintf(_notification_string[_current_num_notification],"%s",notification);
        _notification_time[_current_num_notification] = time;
        _current_num_notification++;
    }
    
}
void Device::PushNotifyResignActive()
{
    OS_DevicePushNotify();
}
void Device::PushNotifyBecameActive()
{
    OS_DeviceClearAllPushNotify();
    
}
#endif



#if USE_COMPRESSOR_DATA_INGAME
bool Device::HasUnzipPack(int index)
{
    bool exitsfile = OS_IsDocumentFileExist(Device::GetPackShortNameCompressed(index));
    if(exitsfile)
    {
        char name[256];
        sprintf(name,"%s",Device::GetPackFullNameCoompressed(index));
        time_t t = Device::GetTimeModifyOfFile(OS_GetResourceFilePath(name));
        sprintf(name,"log_%ld_%s",t,Device::GetPackShortNameCompressed(index));
        //printf("\nLog 0: %s",name);
        if(Device::HasAppInstalled(name,GAME_VERSION))
        {
            return true;
        }
       
    }
    return false;
}
bool Device::HasUnzipAll()
{
    for(int i = 0; i < TOTAL__ZIPCOMPRESS_FILE_ROW; i++)
    {
        if(!HasUnzipPack(i))
        {
            return false;
        }
    }
    return true;
}
bool Device::UnzipPack(int index)
{
    bool isLastFile =  false;
    if(index >= 0)
    {
        
        bool exitsfile = OS_IsDocumentFileExist(Device::GetPackShortNameCompressed(index));
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
            /*if(Device::PINEZ_Extractor(OS_GetResourceFilePath(Device::GetPackFullNameCoompressed(index)),
                                       Device::GetPackShortNameCompressed(index),
                                       OS_GetDocumentFilePath(Device::GetPackShortNameCompressed(index))))
            {*/
                char name[256];
                sprintf(name,"%s",Device::GetPackFullNameCoompressed(index));
                time_t t = Device::GetTimeModifyOfFile(OS_GetResourceFilePath(name));
                sprintf(name,"log_%ld_%s",t,Device::GetPackShortNameCompressed(index));
                //printf("\nLog 1: %s",name);
                Device::SetAppInstalled(name,GAME_VERSION);
                isLastFile = true;
                
                OS::setFileAttribute(output_path, EFileLocation::PATH, EFileAttribute::ARCHIVE, false);
            }else
            {
                PDEBUG("extract fail\n");
            }
        }
    }
    return isLastFile;
}
const char* Device::GetPackShortNameCompressed(int index)
{
    return Compressor_Exporter::_zipcompress_file[index*TOTAL__ZIPCOMPRESS_FILE+_ZIPCOMPRESS_FILE_PACK_SHORT_NAME];
}
const char* Device::GetPackFullNameCoompressed(int index)
{
     return Compressor_Exporter::_zipcompress_file[index*TOTAL__ZIPCOMPRESS_FILE+_ZIPCOMPRESS_FILE_PACK_SHORT_ZIP];
}
int Device::GetPackCompressed(const char* pack_name)
{
    for(int i = 0; i < TOTAL__ZIPCOMPRESS_FILE_ROW; i++)
    {
        if(strcmp(Compressor_Exporter::_zipcompress_file[i*TOTAL__ZIPCOMPRESS_FILE+_ZIPCOMPRESS_FILE_PACK_NAME],pack_name) == 0)
        {
            //printf("pack_name = %s compare with = %s",pack_name,Compressor_Exporter::_zipcompress_file[i*TOTAL__ZIPCOMPRESS_FILE+_ZIPCOMPRESS_FILE_PACK_NAME]);
            return i;
        }
    }
    return -1;
}

#endif


#if USE_AUDIO_RECORDER

extern "C" void OS_StopBlowDevice();
extern "C" double OS_GetMicroAudioPass();
extern "C" void OS_BeginListenBlowDevice();
extern "C" void OS_BlowSetCurrentPass(double v);
 int Device::AudioRecordState = 0;
extern "C" float OS_GetVolume();
float Device::GetVolumeSpeaker()
{
#ifdef MAC_OS
    return OS_GetVolume();
#endif
    return 0;
}
void Device::StartDetectBlow()
{
#ifdef MAC_OS
    Device::AudioRecordState = AUDIO_RECORD_WAIT_AUTHENTICAION_STATE;
    OS_BeginListenBlowDevice();
#endif
}
void Device::StopDetectBlow()
{
#ifdef MAC_OS
    if(Device::AudioRecordState == AUDIO_RECORD_AUTHENTICATION_PROCESSING)
    {
        Device::AudioRecordState = AUDIO_RECORD_AUTHENTICATION_STOPPED;
        OS_StopBlowDevice();
    }
#endif
}
double Device::GetBlowAudioPassVolume()
{
#ifdef MAC_OS
    return OS_GetMicroAudioPass();
#else
    return 0;
#endif
}
void Device::SetBlowPassCurrentVolume(double v)
{
#ifdef MAC_OS
    OS_BlowSetCurrentPass(v);
#endif
}
#endif


time_t Device::GetTimeModifyOfFile(const char* file)
{
#ifdef MAC_OS
    struct stat t_stat;
    stat(file, &t_stat);
    time_t rawtime_s =  t_stat.st_mtimespec.tv_sec;
    return rawtime_s;
#endif
    return 0;
}
time_t Device::GetTimeCreateOfFile(const char* file)
{
#ifdef MAC_OS
    struct stat t_stat;
    stat("file_name", &t_stat);
    time_t rawtime_s =  t_stat.st_birthtimespec.tv_sec;
    return rawtime_s;
#endif
    return 0;
    
}


#if ADD_GOOGLE_ANALYTICS
#if defined (ANDROID_OS)
void Device::GA_Init()
{
}
void Device::GA_LogPurchase(const char *TransactionID, double price)
{
	char str_price[32];
	sprintf(str_price, "%0.0f", price);
	OS_GoogleAnalyticsLogEvent("purchase_refund", TransactionID, "", str_price);
}
void Device::GA_LogBuyItem(const char *CurrencyName, const char* ItemName, double price)
{
	char str_price[32];
	sprintf(str_price, "%0.0f", price);
	OS_GoogleAnalyticsLogEvent("spend_virtual_currency", ItemName, "", str_price);
}
void Device::GA_LogAchievements(const char *achiID, const char* achiName)
{
	OS_GoogleAnalyticsLogEvent("unlock_achievement", achiName);
}
void Device::GA_LogContents(const char *title, const char* decs, const char* type)
{
	OS_GoogleAnalyticsLogEvent(title, decs, type);
}

void Device::GA_LogLevelup(double level_up)
{
	char str_level[32];
	sprintf(str_level, "%0.0f", level_up);
	//OS_GoogleAnalyticsLogEvent("level_up", "level_up", "", str_level);
}

#else
extern "C" void OS_InitGoogleAnalytics();
extern "C" void OS_GoogleAnalyticsLogPurchase(const char *TransactionID,double price);
extern "C" void OS_GoogleAnalyticsLogSpendVirtualCurrency(const char *CurrencyName,const char* ItemName,double price);
extern "C" void OS_GoogleAnalyticsLogAchievements(const char *achiID,const char* achiName);
extern "C" void OS_GoogleAnalyticsLogContents(const char *title,const char* decs,const char*type);
extern "C" void OS_GoogleAnalyticsLogContentsWithType(const char *title,const char* decs,const char*type);
extern "C" void OS_GoogleAnalyticsLogLevelUp(double level);
extern "C" void OS_GoogleAnalyticsSetUserLevelProperty(double level);

void Device::GA_SetUserLevelProperty(double level)
{
    OS_GoogleAnalyticsSetUserLevelProperty(level);
}
void Device::GA_Init()
{
    OS_InitGoogleAnalytics();
}
void Device::GA_LogPurchase(const char *TransactionID,double price)
{
    OS_GoogleAnalyticsLogPurchase(TransactionID,price);
}
void Device::GA_LogBuyItem(const char *CurrencyName,const char* ItemName,double price)
{
    OS_GoogleAnalyticsLogSpendVirtualCurrency(CurrencyName,ItemName,price);
}
void Device::GA_LogAchievements(const char *achiID,const char* achiName)
{
    OS_GoogleAnalyticsLogAchievements(achiID,achiName);
}
void Device::GA_LogContents(const char *title,const char* decs, const char* type)
{
    //OS_GoogleAnalyticsLogContents(title,decs,type);
    OS_GoogleAnalyticsLogContentsWithType(title,decs,type);
}

void Device::GA_LogLevelup(double level_up)
{
    OS_GoogleAnalyticsLogLevelUp(level_up);
}
#if ADD_REMOTE_CONFIG
extern "C" const char* OS_RemoteConfigGetString(const char* strID, const char* default_value);
extern "C" const bool OS_RemoteConfigGetBoolean(const char*  strID,bool default_value);
extern "C" const long OS_RemoteConfigGetInteger(const char*  strID,long default_value);
const char* Device::RemoteConfigGetString(const char*  strID,const char* default_value)
{
    return OS_RemoteConfigGetString(strID,default_value);
}

const long Device::RemoteConfigGetInteger(const char*  strID, long default_value)
{
    return OS_RemoteConfigGetInteger(strID,default_value);
}
const bool Device::RemoteConfigGetBoolean(const char*  strID, bool default_value)
{
    return OS_RemoteConfigGetBoolean(strID, default_value);
}
#endif
#endif
#endif
