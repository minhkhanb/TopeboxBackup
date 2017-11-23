#include "pch.h"
#include "MusicTimeControl.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"

MusicTimeControl::MusicTimeControl(){

}
void MusicTimeControl::SetMusic(UINT32 resid, int time_sec, bool loop){
	_play_time = time_sec;
	_loop = loop;
	if (GAME() != NULL){
		GAME()->LoadMusic(resid);
	}
}
void MusicTimeControl::Play(){
	_last_time_update = OS_GetTime();
	_curr_play_time = _play_time;
	_current_volume = 100.0f;
	if (GAME() != NULL){
		if (_loop){
			GAME()->PlayCurrentMusic(-1);
		}
		else{
			GAME()->PlayCurrentMusic(0);
		}
	}
	_is_playing = true;
}
bool MusicTimeControl::IsPlaying(){
	return _is_playing;
}
void MusicTimeControl::Update(){
	INT_TIME currtime = OS_GetTime();
	INT_TIME dt = currtime - _last_time_update;
	_curr_play_time -= dt;
	int durace = _play_time - _curr_play_time;
	if (_curr_play_time < 3000)
	{
		_current_volume -= 0.5f;
		if (_current_volume < 0)
		{
			_current_volume = 0;
			if (!_loop){
				if (GAME() != NULL){
					GAME()->StopCurrentMusic();
					_is_playing = false;
				}
			}
			else{
				_last_time_update = OS_GetTime();
				_curr_play_time = _play_time;
				_current_volume = 100.0f;
			}
		}
		GAME()->SetVolume(_current_volume);
	}
	else{
		if (durace < 3000)
		{
			_current_volume += 1.0f;
			if (_current_volume > 100)
			{
				_current_volume = 100;
			}
			GAME()->SetVolume(_current_volume);
		}
		else
		{
			if (_current_volume != 100)
			{
				_current_volume = 100;
				GAME()->SetVolume(_current_volume);
			}
		}
	}
}