/*duy.nguyen implement to use OpenSLES*/

#ifndef _H_OPEN_SLSE
#define _H_OPEN_SLSE
#define MAX_SOUND_PLAYER    8
#include "../../DataType/CType.h"
#if defined(ANDROID_OS)
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>
#endif
#include "../SoundEngine.h"
#include "Sound.h"
#include "Music.h"
class SoundOpenSLES:public SoundEngine {
public:
	virtual void Init();
	virtual void Release();
	virtual void LoadMusic(const ResourceItem resitem);
	virtual void PlayMusic(int num_loop);
	virtual void PauseMusic();
	virtual void StopMusic();
	virtual void ResumeMusic();
	virtual bool IsPlayingMusic();
	//Lam Add
	virtual void ClearAllSFX();
	

	virtual HSOUND LoadSoundEffect(ResourceItem resitem);
	virtual void PlaySoundEffect(HSOUND handle);
	virtual void StopSoundEffect(HSOUND handle);
	virtual void StopAllSoundEffect();
	virtual void UnloadSoundEffect(HSOUND handle);
	
	virtual void PauseAllSoundEffect();
    virtual void ResumeAllSoundEffect();
public:
	SoundOpenSLES();
	~SoundOpenSLES();
	void start();
	void stop();
private:
	HSOUND registerSound(ResourceItem resitem);
	void playSound(Sound* pSound);
	void playSoundMono(Sound* pSound);
	
	bool startSoundPlayer();
	bool startSoundPlayer(int i);
	bool startSoundPlayerMono();
    bool startSoundPlayerMono(int i);
	
	SLObjectItf mEngineObj; SLEngineItf mEngine;
	SLObjectItf mOutputMixObj;
	
	SLObjectItf mBGMPlayerObj; SLPlayItf mBGMPlayer;
	SLSeekItf mBGMPlayerSeek;
	
	SLObjectItf mPlayerObj[MAX_SOUND_PLAYER];
	SLPlayItf mPlayer[MAX_SOUND_PLAYER];
	SLBufferQueueItf mPlayerQueue[MAX_SOUND_PLAYER];
	bool _is_init_stereo_sound_player_success;
	int stereo_player_count;

	SLObjectItf mPlayerObj_mono[MAX_SOUND_PLAYER];
	SLPlayItf mPlayer_mono[MAX_SOUND_PLAYER];
	SLBufferQueueItf mPlayerQueue_mono[MAX_SOUND_PLAYER];
	bool _is_init_mono_sound_player_success;
	int mono_player_count;

	Sound* mSounds[100]; int32_t mSoundCount;
	
	//Music _music;
	
	static SoundOpenSLES* _this;
	bool _is_music_playing;
public:
	//SLAndroidSimpleBufferQueueItf mMusicPlayerQueue;
	//UINT32 _curr_music_revision;
	SoundOpenSLES* GetInstance(){return _this;};
	//Music* GetMusic(){return &_music;}
	
};
#endif