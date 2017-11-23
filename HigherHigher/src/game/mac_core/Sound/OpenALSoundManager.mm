//
//  OpenALSoundManager.cpp
//  PineEntertainment
//
//  Created by duy.nguyen on 8/29/14.
//
//
#include "CType.h"
#include "OpenALSoundManager.h"
#include "CGame.h"
#include "oalPlayback.h"
#include "MyOpenALSupport.h"
#if !defined(NUM_OF_SOUND)
#   if defined(NUM_SOUND)
#       define NUM_OF_SOUND NUM_SOUND
#   else
#       define NUM_OF_SOUND 50
#   endif
#endif
OpenALSoundManager::OpenALSoundManager():_handle_sound(NULL), _is_loaded_sound(NULL){
    Release();
    _is_loaded_sound = new bool[NUM_OF_SOUND];
    _handle_sound = new int[NUM_OF_SOUND];
    for(int s = 0; s< NUM_OF_SOUND;s++){
        _is_loaded_sound[s] = false;
        _handle_sound[s] = -1;
    }
	_is_playing_music = false;
}
OpenALSoundManager::~OpenALSoundManager(){
    Release();
}
void OpenALSoundManager::Init(){
    
}
void OpenALSoundManager::Release(){
    if(_handle_sound != NULL){
        delete [] _handle_sound;
        _handle_sound = NULL;
    }
}
void OpenALSoundManager::LoadMusic(ResourceItem resitem){
    FILE* file = NULL;
    file = fopen(resitem.Path, "rb");
    Buffer* buff = NULL;
    if(file != NULL){
        fseek(file, resitem.Offset, SEEK_SET);
        buff = new Buffer(resitem.Length);
        fread(buff->Data(), sizeof(char), resitem.Length, file);
        fclose(file);
        [[oalPlayback getSharedInstance] loadMusicWithData:(const char*)buff->Data(): buff->Length()];
        delete buff;
    }
}
void OpenALSoundManager::PlayMusic(int num_loop){
    _is_playing_music = true;
    [[oalPlayback getSharedInstance] startMusic: num_loop];
}
void OpenALSoundManager::PauseMusic(){
    [[oalPlayback getSharedInstance] stopMusic];
}
void OpenALSoundManager::StopMusic(){
    _is_playing_music = false;
    [[oalPlayback getSharedInstance] stopMusic];
}
void OpenALSoundManager::ResumeMusic(){
	[[oalPlayback getSharedInstance] resumeMusic];
}
bool OpenALSoundManager::IsPlayingMusic(){
    if([oalPlayback getSharedInstance] == nil)
    {
        return false;
    }
	return _is_playing_music && [[oalPlayback getSharedInstance] isMusicPlaying] ;
}


HSOUND OpenALSoundManager::LoadSoundEffect(ResourceItem resitem){
    FILE* file = NULL;
    file = fopen(resitem.Path, "rb");
    Buffer* buff = NULL;
    if(file != NULL){
        fseek(file, resitem.Offset, SEEK_SET);
        buff = new Buffer(resitem.Length);
        fread(buff->Data(), sizeof(char), resitem.Length, file);
        fclose(file);
        HSOUND hsound = -1;
        for(int i = 0; i< NUM_OF_SOUND; i++){
            if(!_is_loaded_sound[i]){
                hsound = i;
                break;
            }
        }
        if(hsound  > -1 ){
            _is_loaded_sound[hsound]= true;
            [[oalPlayback getSharedInstance] loadSoundWidthData:(const char*)buff->Data() :buff->Length() :hsound];
        }
        SAFE_DEL(buff);
        return hsound;
    }
    return -1;
}
void OpenALSoundManager::PlaySoundEffect(HSOUND handle){
    if(handle > -1 && handle < NUM_OF_SOUND){
        [[oalPlayback getSharedInstance] startSound: handle];
    }
}
void OpenALSoundManager::StopSoundEffect(HSOUND handle){
    if(handle > -1 && handle < NUM_OF_SOUND){
        [[oalPlayback getSharedInstance] stopSound: handle];
    }
}

void OpenALSoundManager::StopAllSoundEffect(){
    [[oalPlayback getSharedInstance] stopAllSound];
}
void OpenALSoundManager::PauseAllSoundEffect() {
    [[oalPlayback getSharedInstance] pauseAllSound];
}
void OpenALSoundManager::ResumeAllSoundEffect() {
    [[oalPlayback getSharedInstance] resumeAllSound];
    
}
void OpenALSoundManager::UnloadSoundEffect(HSOUND handle){
    if(handle > -1 && handle < NUM_OF_SOUND){
        [[oalPlayback getSharedInstance] stopSound: handle];
        [[oalPlayback getSharedInstance] unloadSound:handle];
    }
}
void OpenALSoundManager::endInterruption()
{
    [[oalPlayback getSharedInstance]endInterruption];
}
//Lam Add
void OpenALSoundManager::ClearAllSFX()
{
    
}
