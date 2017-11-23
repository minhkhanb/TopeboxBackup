#if !defined(_H_SOUND_ENGINE)
#define _H_SOUND_ENGINE
#include "../DataType/CType.h"
typedef int HSOUND;
class SoundEngine{
public:
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void LoadMusic(ResourceItem resitem) = 0;
	virtual void PlayMusic(int num_loop) = 0;
	virtual void PauseMusic() = 0;
	virtual void ResumeMusic() = 0;
	virtual void StopMusic() = 0;
	virtual bool IsPlayingMusic() = 0;
	

	virtual HSOUND LoadSoundEffect(ResourceItem resitem) = 0;
	virtual void PlaySoundEffect(HSOUND handle) = 0;
	virtual void StopSoundEffect(HSOUND handle) = 0;
	virtual void StopAllSoundEffect() = 0;
    virtual void PauseAllSoundEffect() = 0;
    virtual void ResumeAllSoundEffect() = 0;
	virtual void UnloadSoundEffect(HSOUND handle) = 0;
	//Lam Add
	virtual void ClearAllSFX() = 0;
};
#endif