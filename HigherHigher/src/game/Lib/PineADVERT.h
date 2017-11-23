#if !_PINE_ADVERT_H
#define _PINE_ADVERT_H 1

#include "../GamePlay/GameObject/MenuSystem.h"
#include "../GamePlay/GameObject/CScrollBar.h"
#include "PineImage.h"
#define ADS_LIMIT_NUM (200)

#define PINEADS_IDLE_STATE              (0)
#define PINEADS_DOWNLOAD_CONFIG         (1)
#define PINEADS_ERROR_STATE             (2)
#define PINEADS_LOAD_PLAN_STATE         (3)
#define PINEADS_DOWNLOAD_ASSET_STATE    (4)
#define PINEADS_UNZIP_ASSET_STATE       (5)
#define PINEADS_READY_STATE             (6)
#define PINEADS_NOPLAN_STATE            (7)
#define PINEADS_DOWNLOADED_STATE        (8)

#define PINEADS_INSTALL_STATE_NONE              (0)
#define PINEADS_INSTALL_STATE_WILL_DOWNLOAD     (1)
#define PINEADS_INSTALL_STATE_DOWNLOADING       (2)
#define PINEADS_INSTALL_STATE_DOWNLOAD_DONE     (3)
#define PINEADS_INSTALL_STATE_DOWNLOAD_CANCEL   (4)


#define PINEADS_POPUP_STATE_NONE    (0)
#define PINEADS_POPUP_STATE_INTRO   (1)
#define PINEADS_POPUP_STATE_ACTIVE  (2)
#define PINEADS_POPUP_STATE_OUTTRO  (3)
#define PINEADS_POPUP_STATE_WILL_CLOSE (4)
#define PINEADS_POPUP_STATE_CLOSE   (0)
class PineADVERT
{
    int _state;
    int _error;
    int _count;
    int _apple_id_list[ADS_LIMIT_NUM];
    int _app_version[ADS_LIMIT_NUM];
    bool _wifi[ADS_LIMIT_NUM];
    bool _asset[ADS_LIMIT_NUM];
    int _app_plan_index;
    int _app_install_state;
    bool _need_resume;
    char _decs[ADS_LIMIT_NUM][1024];
    INT_TIME _tStart;
    int _offset_y;
    
public:
    int GetAppInstallState();
    void SetAppInstallState(int state);
#define BUTTON_ADS_NUMS (2)
    Button _ads_button[BUTTON_ADS_NUMS];
    int GetState();
    void SetState(int state);
    PineADVERT();
    void ServerStart();
    void ServerUpdate();
    void ServerStop();
    bool CreateAPlan();
    void SetInstallAd();
    void SetBecomeActive();
    void ResignActive();
    void SetDownloadCompete();
    
    bool IsAvailable();
    void LoadSprite();
    void UnloadSprite();
    void SetScale(float x, float y);
    void SetTranslate(float x, float y);
    float _ads_scale_x;
    float _ads_scale_y;
    float _ads_translate_x;
    float _ads_translate_y;
    
    CAnimObject _anim;
    int _popup_state;
    int _opacity;
    bool HasPopup();
    void Init();
    void Close();
    void Update();
    void Render();
    bool UpdateTouch();
    
    void SaveLastDayIntallApp();
    int GetLastLastIntallApp();
};
#endif