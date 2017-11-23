#if !defined(_H_BASS_LIB)
#define _H_BASS_LIB
#include "bass.h"
#include <Windows.h>
#pragma comment( lib, "bass.lib")
#include "SoundEngine.h"
#include <Windows.h>
#define CORE_GAME_MAX_SOUND 50
class BassLib :public SoundEngine{
private:
	DWORD chan;
	DWORD chan_sound[CORE_GAME_MAX_SOUND];

	UINT8* _soundDATA[CORE_GAME_MAX_SOUND];
	int _soundSizeData[CORE_GAME_MAX_SOUND];
	int _soundHandle[CORE_GAME_MAX_SOUND];

	bool _sound_map[CORE_GAME_MAX_SOUND];
	int _sound_idgen;
public:
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

	void InitBass(HWND whandle);
};
#endif