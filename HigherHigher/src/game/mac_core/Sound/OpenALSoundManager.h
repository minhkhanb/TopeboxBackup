//
//  OpenALSoundManager.h
//  PineEntertainment


#ifndef PineEntertainment_OpenALSoundManager_h
#define PineEntertainment_OpenALSoundManager_h
#include "SoundEngine.h"
class OpenALSoundManager: public SoundEngine{
private:
    bool* _is_loaded_sound;
    int* _handle_sound;
    bool _is_playing_music;
public:
    OpenALSoundManager();
    ~OpenALSoundManager();
    virtual void Init();
	virtual void Release();
	virtual void LoadMusic(ResourceItem resitem);
	virtual void PlayMusic(int num_loop);
	virtual void PauseMusic();
	virtual void StopMusic();
	virtual void ResumeMusic();
	virtual bool IsPlayingMusic();
	
    
	virtual HSOUND LoadSoundEffect(ResourceItem resitem);
	virtual void PlaySoundEffect(HSOUND handle);
	virtual void StopSoundEffect(HSOUND handle);
	virtual void StopAllSoundEffect();
    virtual void PauseAllSoundEffect();
    virtual void ResumeAllSoundEffect();
	virtual void UnloadSoundEffect(HSOUND handle);
    //Lam Add
    virtual void ClearAllSFX();
    
    void endInterruption();
};


#endif
