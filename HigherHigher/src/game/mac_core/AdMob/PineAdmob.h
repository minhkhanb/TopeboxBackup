//
//  PineChartBoost.h
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 5/6/14.
//
//

//////////////////////////
//// README
/////////////////////////



extern "C" void DownloadFile(const char* url, const char* file_name,int time_out);
extern "C" int GetSizeFile(const char *file_name);
extern "C" int ReadAppDataBinary(const char* filename, BYTE* readed_data);
//_pine_view_admob
#ifndef PineEntertainment_Admob
#define PineEntertainment_Admob
-(bool) AdmobAvailable
{
    return (_admob_app_id != nil && _admob_app_syn != nil);
}
-(bool) Admob_HasAdToShow
{
    return [_pine_view_admob admob_hasadstoshow];
}
-(void) ShowAdMob
{
    [_pine_view_admob AdMobShow];
}

extern "C" bool HasAdmobToShow()
{
    return [[PineGameDelegate sharedInstance] Admob_HasAdToShow];
    
}
extern "C" void ShowAdmobFullScreen()
{
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(ShowAdMob) withObject:nil waitUntilDone:NO];
    //[[PineGameDelegate sharedInstance]->_pine_view_admob AdMobShow ];
  
}
extern "C" bool OS_AdmobAvailable()
{
    return  [[PineGameDelegate sharedInstance] AdmobAvailable];
}

#endif
