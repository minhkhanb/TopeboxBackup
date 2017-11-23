 #include "pch.h"
#include "../GameCore/CGame.h"
#include "../GameCore/Os_Functions.h"

#if !defined(_WINDOWS_PHONE8)
#include <PineEngine.h>
PE_USING_ENGINE;
#endif

#if ADD_ADVERT
#include "PineADVERT.h"

#include "../Lib/DataInputStream/DataInputStream.h"

//extern "C" void OpenAppStore(int appleID);
//extern "C" void DownloadFile(const char* url, const char* file_name);
//extern "C" char* OS_GetResourceFilePath(const char* filename);
//extern "C" const char* GetDocumentFilePath(const char* filename);
#if USE_ADBOX
extern "C" void OS_AdBoxDownloadFile();
#endif

extern "C" bool	OS_IsDocumentFileExist(const char* filename);

extern "C" void OpenAppStore(INT64 appleID);
void PineADVERT::SaveLastDayIntallApp()
{
    char buffer[128];
    char file_path[1024];
    sprintf(file_path,OS_GetDocumentFilePath("pinelastdownload.dat"));
    FILE* f= fopen(file_path,"w");
    if(f)
    {
        int day = Device::GetDayOfYear();
        CGame::_this->SetInt32At(buffer, 0, day);
        fwrite(buffer,sizeof(char),128,f);
        fclose(f);
    }
}
int PineADVERT::GetLastLastIntallApp()
{
    char file_path[1024];
    sprintf(file_path,OS_GetDocumentFilePath("pinelastdownload.dat"));
    //printf("\nFile checked = %s",file_path);
    FILE* f =  fopen(file_path,"r");
    if(f)
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
                return version;
               
            }
            else
            {
                fclose(f);
                return 0;
            }
            
    
        fclose(f);
        return 0;
    }
    //return OS_HasAppInstalled(boudble_id);
    return 0;
}
PineADVERT::PineADVERT()
{
    _app_install_state = PINEADS_INSTALL_STATE_NONE;
    _popup_state = PINEADS_POPUP_STATE_NONE;
}
void PineADVERT::ServerStop()
{
	
    _state = -1;
}
void PineADVERT::ServerStart()
{
	_state = 0;
	ServerUpdate();
}


void PineADVERT::ServerUpdate()
{
#ifdef MAC_OS
    int delay = 10000;
#if USE_ADBOX
    while(delay > 0)
    {
        delay -= 40;
        CGame::_this->Sleep(40);
    }
    OS_AdBoxDownloadFile();
#endif
#if ADD_PINE_ADVERT
    if(Device::IsNetworkAvailable())
    {
        _state = PINEADS_DOWNLOAD_CONFIG;
    }
    else
    {
        _state =  PINEADS_ERROR_STATE;;
    }
#if DISABLE_IPAD_ADS
    if(SupportVersion::RESVER_1024_768 == CGame::_this->_device._version || Device::IsIpadDevice())
    {
        _state =  PINEADS_ERROR_STATE;
    }
#endif
    bool interupted;
    _need_resume = false;
	while(_state >= 0 && CGame::_this != NULL && !CGame::_this->_isExit)
	{
        if(CGame::_this->_isPause)
        {
            CGame::_this->Sleep(100);
            interupted = true;
            _need_resume = true;
            continue;
        }
        switch(_state)
        {
            case PINEADS_DOWNLOAD_CONFIG:
            {
                
                char url[512];
                sprintf(url,"https://datancus.blob.core.windows.net/pinecrossingads/%d/crossingads%s.config", APPLE_ID,ADS_VERSION);
                if(!Device::DownloadFile(url, "crossingads.config",8))
                {
                    _state = PINEADS_ERROR_STATE;
                }
                else
                {
                    _state = PINEADS_LOAD_PLAN_STATE;
                }
            }
                break;
            case PINEADS_LOAD_PLAN_STATE:
            {
                
                if(CreateAPlan())
                {
                     _state = PINEADS_DOWNLOAD_ASSET_STATE;
                }
                else
                {
                    _state = PINEADS_NOPLAN_STATE;
                }
            }
                break;
            case PINEADS_DOWNLOAD_ASSET_STATE:
            {
               // _app_plan_id = APPLE_ID;
                int apple_id = _apple_id_list[_app_plan_index];
                int apple_ver= _app_version[_app_plan_index];
                char url[512];
                char strFile[512];
                sprintf(strFile,"%d_%d_%s",apple_id,apple_ver,"asset.pinez");
                
                if(OS_IsDocumentFileExist(strFile))
                {
                    std::string str_archive(strFile);
                    std::wstring archive_file(str_archive.begin(), str_archive.end());
                    std::wstring archive_path = OS::findResourcePath(archive_file, EFileLocation::DOCUMENT_DIR);
                    
                    std::string str_output("checksum.config");
                    std::wstring output_file(str_output.begin(), str_output.end());
                    std::wstring output_path = OS::findResourcePath(output_file, EFileLocation::DOCUMENT_DIR);
                    
                    std::string str_identify("checksum.config");
      
                    
                    if(Device::PINEZ_Extractor(archive_path, str_identify, output_path))
                    {
                        printf("\nCheck sum okie");
                        _state = PINEADS_UNZIP_ASSET_STATE;
                        OS::setFileAttribute(output_path, EFileLocation::PATH, EFileAttribute::ARCHIVE, false);
                        break;
                    }
                }
                bool is_landScape = !(CGame::_this->_device_orientation == ORIENTATION_PORTRAIT);
#if USE_PINEADS_PORTRAIL_ONLY
                is_landScape = false;
#endif
#if defined (MAC_OS)
                if(Device::IsIpadDevice())
#else	
				if (true)
#endif
                {
#if USE_MASTER_FOR_IPAD
                    if(is_landScape)
                    {
                        
                        sprintf(url,"https://datancus.blob.core.windows.net/pinecrossingads/%d/asset_%s_landscape.pinez", apple_id,DEFINE_MASTER_IOS);
                    }
                    else
                    {
                        sprintf(url,"https://datancus.blob.core.windows.net/pinecrossingads/%d/asset_%s.pinez", apple_id,DEFINE_MASTER_IOS);
                    }
#else
                    if(is_landScape)
                    {
                        sprintf(url,"https://datancus.blob.core.windows.net/pinecrossingads/%d/asset_ipad_landscape.pinez", apple_id);
                    }
                    else
                    {
                        sprintf(url,"https://datancus.blob.core.windows.net/pinecrossingads/%d/asset_ipad.pinez", apple_id);
                    }
#endif
                }
                else
                {
                    if(is_landScape)
                    {
                        
                        sprintf(url,"https://datancus.blob.core.windows.net/pinecrossingads/%d/asset_%s_landscape.pinez", apple_id,DEFINE_MASTER_IOS);
                    }
                    else
                    {
                        sprintf(url,"https://datancus.blob.core.windows.net/pinecrossingads/%d/asset_%s.pinez", apple_id,DEFINE_MASTER_IOS);
                    }
                }
                printf("\nurl = %s",url);
                printf("\nCGame::_this->_current_version  = %d",CGame::_this->_current_version );
                if(!Device::DownloadFile(url, strFile,30*60))
                {
                    _state = PINEADS_ERROR_STATE;
                }
                else
                {
                    _state = PINEADS_UNZIP_ASSET_STATE;
                }
            }
                break;
            case PINEADS_UNZIP_ASSET_STATE:
            {
                int apple_id = _apple_id_list[_app_plan_index];
                int apple_ver= _app_version[_app_plan_index];
                char file_zip[1024];
                char file_out[1024];
                sprintf(file_zip,"%d_%d_%s",apple_id,apple_ver,"asset.pinez");
                
                
                std::string str_archive(file_zip);
                std::wstring archive_file(str_archive.begin(), str_archive.end());
                std::wstring archive_path = OS::findResourcePath(archive_file, EFileLocation::DOCUMENT_DIR);
                
                std::string str_output("font.bpine");
                std::wstring output_file(str_output.begin(), str_output.end());
                std::wstring output_path = OS::findResourcePath(output_file, EFileLocation::DOCUMENT_DIR);
                
                std::string str_identify("font.bpine");
                
  
                /*
                if(!Device::PINEZ_Extractor(archive_path, str_identify, output_path))
                {
                     _state = PINEADS_ERROR_STATE;
                    
                    break;
                }else{
                    OS::setFileAttribute(output_path, EFileLocation::PATH, EFileAttribute::ARCHIVE, false);
                }
                */
                
                str_output = std::string("banner.bpine");
                
                output_file = std::wstring(str_output.begin(), str_output.end());
                output_path = OS::findResourcePath(output_file, EFileLocation::DOCUMENT_DIR);
                
                str_identify = std::string("banner.bpine");

                if(!Device::PINEZ_Extractor(archive_path, str_identify, output_path))
                {
                    _state = PINEADS_ERROR_STATE;
                    break;
                }else {
                    OS::setFileAttribute(output_path, EFileLocation::PATH, EFileAttribute::ARCHIVE, false);
                }
                

                //icon.png
                _state = PINEADS_READY_STATE;
                
            }
                break;
            case PINEADS_READY_STATE:
                
                //printf("\n Ready load ads");
                break;
            case PINEADS_ERROR_STATE:
                if(_need_resume)
                {
                    _state = PINEADS_DOWNLOAD_CONFIG;
                    _need_resume = false;
                }
                break;
            case PINEADS_NOPLAN_STATE:
                if(_need_resume)
                {
                    _state = PINEADS_DOWNLOAD_CONFIG;
                    _need_resume = false;
                }
                break;
            case PINEADS_DOWNLOADED_STATE:
                if(_need_resume)
                {
                    _state = PINEADS_DOWNLOAD_CONFIG;
                    _need_resume = false;
                }
                break;
            default:
                break;
        }
		 CGame::_this->Sleep(120);
	}
#endif
#endif
   
}
bool PineADVERT::CreateAPlan()
{
    char buffer[10240];
    char file_path[1024];
    sprintf(file_path,"%s", OS_GetDocumentFilePath("crossingads.config"));
    printf("\nFile checked = %s",file_path);
    FILE* f =  fopen(file_path,"r");
    if(f)
    {
        fseek (f, 0, SEEK_END);   // non-portable
        long size=ftell (f);
        fseek(f,0,SEEK_SET);
        if(size > 0)
        {
            fread(buffer,1,size,f);
             buffer[size]='\0';
            if(strstr(buffer,"[ADS]") != NULL && strstr(buffer,"[/ADS]") != NULL)
            {
                int list_count = 0;
                for(int i = 0; i < size;)
                {
                    
                    if(buffer[i] == '+')
                    {
                        i++;
                        char app_id[128];
                        char app_ver[128];
                        char network[128];
                        char asset[128];
                        char decs[1024];
                        int count = 0;
                        //find apple id;
                        for(int j = i; j < size; j++)
                        {
                            char ch = buffer[j];
                            if(ch == '#')
                            {
                                break;
                            }
                            else
                            {
                                app_id[count] = ch;
                                count++;
                            }
                        }
                        app_id[count] = '\0';
                        i = i + (count+1);
                        count = 0;
                        //find version
                        for(int j = i; j< size; j++)
                        {
                             char ch = buffer[j];
                            if(ch == '#')
                            {
                                break;
                            }
                            else
                            {
                                app_ver[count] = ch;
                                count++;
                            }
                        }
                        app_ver[count] = '\0';
                        i = i + (count+1);
                        count = 0;
                        
                        //find network
                        for(int j = i; j< size; j++)
                        {
                            char ch = buffer[j];
                            if(ch == '#')
                            {
                                break;
                            }
                            else
                            {
                                network[count] = ch;
                                count++;
                            }
                        }
                        network[count] = '\0';
                        i = i + (count+1);
                        count = 0;
                        
                        //find network
                        for(int j = i; j< size; j++)
                        {
                            char ch = buffer[j];
                            if(ch == '-' || ch == '#')
                            {
                                break;
                            }
                            else
                            {
                                asset[count] = ch;
                                count++;
                            }
                        }
                        asset[count] = '\0';
                        i = i + (count+1);
                        count = 0;
                        
                        //find decs
                        for(int j = i; j< size; j++)
                        {
                            char ch = buffer[j];
                            if(ch == '-')
                            {
                                break;
                            }
                            else
                            {
                                decs[count] = ch;
                                count++;
                            }
                        }
                        decs[count] = '\0';
                        i = i + (count+1);
                        count = 0;
                        
                        //sumup
                        if(strlen(app_id) > 0 && strlen(app_ver) > 0)
                        {
                            _apple_id_list[list_count] =  atoi(app_id);
                            _app_version[list_count] = atoi(app_ver);
                            _wifi[list_count] = (strcmp(network,"WIFI") == 0);
                            _asset[list_count] = (strcmp(asset,"ASSET") == 0);
                            sprintf(_decs[list_count],"%s",decs);
                            printf("\n app id = %d, app ver = %d, network = %d, asset = %d, decs = %s",_apple_id_list[list_count],_app_version[list_count],_wifi[list_count],_asset[list_count],_decs[list_count]);
                            list_count++;
                            
                        }

                        
                    }
                    else
                    {
                        
                        i++;
                    }
                    
                }
                if(list_count > 0)
                {
                    printf("\nbuffer = %s",buffer);
                    int network_type = Device::GetNetworkSupport();
                    _app_plan_index = -1;
                    switch(network_type)
                    {
                        case NETWORK_TYPE_3G:
                            for(int i = 0; i < list_count; i++)
                            {
                                char app[128];
                                sprintf(app,"%d",_apple_id_list[i]);
                                if(_app_plan_index < 0 && !_wifi[i] && !Device::HasAppInstalled(app,_app_version[i]))
                                {
                                    _app_plan_index = i;
                                    break;
                                }
                            }
                            break;
                        case NETWORK_TYPE_WIFI:
                            for(int i = 0; i < list_count; i++)
                            {
                                char app[128];
                                sprintf(app,"%d",_apple_id_list[i]);
                                if(_app_plan_index < 0 && _wifi[i] && !Device::HasAppInstalled(app,_app_version[i]))
                                {
                                    _app_plan_index = i;
                                    break;
                                }
                                
                            }
                            if(_app_plan_index < 0)
                            {
                                for(int i = 0; i < list_count; i++)
                                {
                                    char app[128];
                                    sprintf(app,"%d",_apple_id_list[i]);
                                    if(_app_plan_index < 0 && !Device::HasAppInstalled(app,_app_version[i]))
                                    {
                                        _app_plan_index = i;
                                        break;
                                    }
                                
                                }
                            }

                            break;
                        case NETWORK_TYPE_UNKOWN:
                            break;
                        
                    }
                    //HasAppInstalled
                    //network plan
                    fclose(f);
                    if(_app_plan_index >= 0)
                    {
                        return true;
                    }
                    return false;
                }
                else
                {
                    fclose(f);
                    return false;
                }
               
                
            }
            
        }
        fclose(f);
    }
    return false;
}

int PineADVERT::GetState()
{
    return _state;
}
void PineADVERT::SetState(int state)
{
    _state = state;
}

void PineADVERT::LoadSprite()
{
   // CGame::_this->LoadSpriteCurrentVerFromFile(SPRITEID_FONT_ADVERT,OS_GetDocumentFilePath("font.bpine"));
    UnloadSprite();
    CGame::_this->LoadSpriteCurrentVerFromFile(SPRITEID_PINE_ADVERT,OS_GetDocumentFilePath("banner.bpine"));
    for(int i = 0; i < BUTTON_ADS_NUMS; i++)
    {
        _ads_button[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER);
        _ads_button[i].SetAnim(SPRITEID_PINE_ADVERT,4 * i);
        //_ads_button[i]._soundID = SFX_BUTTON;
    }
}
void PineADVERT::UnloadSprite()
{
    //CGame::_this->UnloadSpriteVer(SPRITEID_FONT_ADVERT);
    if(GET_SPRITE(SPRITEID_PINE_ADVERT))
    {
        CGame::_this->UnloadSpriteVer(SPRITEID_PINE_ADVERT);
    }
}
bool PineADVERT::IsAvailable()
{
    return (_state == PINEADS_READY_STATE);
}

bool PineADVERT::HasPopup()
{
    return !(_popup_state == PINEADS_POPUP_STATE_NONE);
}
void PineADVERT::Init()
{
    _popup_state = PINEADS_POPUP_STATE_INTRO;
    _opacity = 0;
    LoadSprite();
    _anim.SetAnimSprite(GET_SPRITE(SPRITEID_PINE_ADVERT),SPRITEID_PINE_ADVERT);
    _anim.SetAnim(8,false);
    _ads_scale_x = 1.0f;
    _ads_scale_y = 1.0f;
    SetTranslate(0,0);
    _offset_y = -k_SCREEN_HEIGHT;
}
void PineADVERT::Close()
{
    _anim.SetAnim(10,false);
    _popup_state = PINEADS_POPUP_STATE_OUTTRO;
}
void PineADVERT::Update()
{
    switch(_popup_state)
    {
        case PINEADS_POPUP_STATE_INTRO:
            _opacity += 25;
            if(_opacity >= (255))
            {
                _opacity = (255);
            }
            
            _offset_y += (k_SCREEN_HEIGHT/25);
            if(_offset_y >= 0)
            {
                _offset_y = 0;
                _anim.SetAnim(9,false);
                _popup_state = PINEADS_POPUP_STATE_ACTIVE;
                
            }
            //_anim.SetPos(0,_offset_y);
            _anim.UpdateAnimObject();
            if(_anim.IsCurrentAnimStop())
            {
                _anim.SetAnim(8,false);
            }
            CGame::_this->ClearAllTouch();
            break;
        case PINEADS_POPUP_STATE_ACTIVE:
            _opacity += 25;
            if(_opacity >= (255))
            {
                _opacity = (255);
            }

            UpdateTouch();
            _anim.UpdateAnimObject();
            if(_anim.IsCurrentAnimStop())
            {
                _anim.SetAnim(9,false);
            }
            break;
        case PINEADS_POPUP_STATE_OUTTRO:
            _opacity -= 25;
            if(_opacity <= 0)
            {
                _opacity = 0;
            }
            _anim.UpdateAnimObject();
            if(_opacity <= 0 && _anim.IsCurrentAnimStop())
            {
                _popup_state = PINEADS_POPUP_STATE_WILL_CLOSE;
            }
            break;
        case PINEADS_POPUP_STATE_WILL_CLOSE:
            _popup_state = PINEADS_POPUP_STATE_NONE;
            break;
        default:
            break;
    }
}
bool PineADVERT::UpdateTouch()
{
    
  
    bool pressed = false;
    for (int i = GAME()->_num_touches - 1; i >= 0; i--)
    {
        if (TOUCHS(i)->_flag)
        {
            for (int touch = 0; touch < BUTTON_ADS_NUMS; touch++)
            {
                int x = _ads_button[touch].TZ_GetX() - 10;
                int y = _ads_button[touch].TZ_GetY() - 10;
                int w = _ads_button[touch].TZ_GetWidth() + 20;
                int h = _ads_button[touch].TZ_GetHeight() + 20;
                Button* button = &_ads_button[touch];
                if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
                {
                    //pressed = true;
                    if (_popup_state == PINEADS_POPUP_STATE_ACTIVE)
                    {
                        switch(touch)
                        {
                            case 0:
                                _app_install_state = PINEADS_INSTALL_STATE_DOWNLOAD_CANCEL;
                                Close();
                                break;
                            case 1:
                                SetInstallAd();
                                Close();
                                break;
                        }
                        CGame::_this->ClearAllTouch();
                        
                    }
                   
                }
            }
        }
    }
    return pressed;
}
void PineADVERT::Render()
{
    if(!IsAvailable())
    {
        return;
    }
   
    float offsetx = _ads_translate_x;
    float offsety = _ads_translate_y;
    G()->SetScale(_ads_scale_x,_ads_scale_y,0,0);
    G()->SetColor((_opacity*70/100)<<24);
    G()->FillFullScreen(true);
    //_anim.SetPos(offsetx,offsety+_offset_y);
    //_anim.DrawAnimObject(G());
    int opa = _opacity;
    if(opa > 100)
    {
        opa = 100;
    }
    //printf("\n opa = %d _opacity = %d",opa,_opacity);
    G()->SetOpacity(opa);
    if(_popup_state == PINEADS_POPUP_STATE_ACTIVE)
    {
        _ads_button[0].SetPos(offsetx,offsety);
        _ads_button[0].Update();
        _ads_button[0].Render(G());
    
        _ads_button[1].SetPos(offsetx,offsety);
        _ads_button[1].Update();
        _ads_button[1].Render(G());
    }
    
    GET_SPRITE(SPRITEID_PINE_ADVERT)->DrawFrame(G(),1,offsetx,offsety+_offset_y);
    //float centerX = Button::GetFrameX(GET_SPRITE(SPRITEID_PINE_ADVERT), 1) + 15 + offsetx;
    //float centerY = Button::GetFrameY(GET_SPRITE(SPRITEID_PINE_ADVERT), 1) + 45 + offsety;
    //printf("\n_decs[_app_plan_index] = %s",_decs[_app_plan_index]);
    //float xx = 0;
    //float yy = 0;
    //G()->SetScale(_ads_scale_x,_ads_scale_y,xx,yy);
    /*
    int space = GET_SPRITE(SPRITEID_FONT_ADVERT)->_char_spacing;
    int line = GET_SPRITE(SPRITEID_FONT_ADVERT)->_line_spacing;
    GET_SPRITE(SPRITEID_FONT_ADVERT)->_line_spacing = line+3;
    GET_SPRITE(SPRITEID_FONT_ADVERT)->_char_spacing = space+1;
    GET_SPRITE(SPRITEID_FONT_ADVERT)->DrawAnsiTextPageFormatCustomColor(G(),centerX,centerY, GET_SPRITE(SPRITEID_PINE_ADVERT)->GetFrameWidth(1)-25, 0,"%s",_decs[_app_plan_index]);
    GET_SPRITE(SPRITEID_FONT_ADVERT)->_char_spacing = space;
    GET_SPRITE(SPRITEID_FONT_ADVERT)->_line_spacing = line;
    G()->ClearScale();
     */
    //G()->FillRect(0,0,375.000000, 667.000000);
}


void PineADVERT::SetDownloadCompete()
{
    if(_app_plan_index >= 0)
    {
        char apple_id[128];
        sprintf(apple_id,"%d",_apple_id_list[_app_plan_index]);
        int version = _app_version[_app_plan_index];
        _state = PINEADS_DOWNLOADED_STATE;
        Device::SetAppInstalled(apple_id,version);
        _app_install_state = PINEADS_INSTALL_STATE_NONE;
    }
}
void PineADVERT::SetInstallAd()
{
    if(_app_plan_index >= 0)
    {
        _tStart = time(NULL);
        _app_install_state = PINEADS_INSTALL_STATE_WILL_DOWNLOAD;
        int apple_id = _apple_id_list[_app_plan_index];
        OpenAppStore(apple_id);
      
    }
}
void PineADVERT::SetBecomeActive()
{
    if(_app_install_state == PINEADS_INSTALL_STATE_DOWNLOADING)
    {
        INT_TIME timeout = time(NULL) - _tStart;
        printf("\n timeouttimeouttimeout = %lld",timeout);
        if(timeout >= 4)
        {
            printf("\nDownload Complete");
            _app_install_state = PINEADS_INSTALL_STATE_DOWNLOAD_DONE;
        }
        else
        {
            printf("\nDownload Cancel");
            _app_install_state = PINEADS_INSTALL_STATE_DOWNLOAD_CANCEL;
        }
    }
    _need_resume = true;
    
}
void PineADVERT::ResignActive()
{
    if(_app_install_state == PINEADS_INSTALL_STATE_WILL_DOWNLOAD)
    {
        //_tStart = time(NULL);
        _app_install_state = PINEADS_INSTALL_STATE_DOWNLOADING;
    }
    
}

int PineADVERT::GetAppInstallState()
{
    return _app_install_state;
}
void PineADVERT::SetAppInstallState(int state)
{
    _app_install_state = state;
}

void PineADVERT::SetScale(float x, float y)
{
    _ads_scale_x = x;
    _ads_scale_y = y;
}

void PineADVERT::SetTranslate(float x, float y)
{
    _ads_translate_x = x;
    _ads_translate_y = y;
}

#endif
