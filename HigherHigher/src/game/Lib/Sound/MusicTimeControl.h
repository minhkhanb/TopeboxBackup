#if !defined(_H_MUSIC_TIME_CONTROL)
#define _H_MUSIC_TIME_CONTROL
#include "SoundEngine.h"

class MusicTimeControl{
protected:
	char _file_name[255];
	INT_TIME _last_time_update;
	UINT32 _res_music_id;
	UINT32 _play_time;
	bool _loop;
	UINT32 _curr_play_time;

	//simulator volume
	float _current_volume;
	bool _is_playing;
public:
	MusicTimeControl();
	void SetMusic(UINT32 resid, int time_sec, bool loop);
	void Play();
	void Stop();
	void Update();
	bool IsPlaying();
};
#endif