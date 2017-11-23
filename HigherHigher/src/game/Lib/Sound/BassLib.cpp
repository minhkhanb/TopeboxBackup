#include "pch.h"
#include "BassLib.h"
#include <stdio.h>
void BassLib::InitBass(HWND whandle)
{
	BASS_Init(-1, 44100, 0, whandle, NULL);
	Init();
}
void BassLib::Init()
{
	for (int i = 0; i < CORE_GAME_MAX_SOUND; i++)
	{
		_soundDATA[i] = NULL;
		_sound_map[i] = false;
	}
	_sound_idgen = 0;
}
void BassLib::Release()
{
	for (int i = 0; i < CORE_GAME_MAX_SOUND; i++)
	{
		if (_soundDATA[i] != NULL){
			delete _soundDATA[i];
			_soundDATA[i] = NULL;
		}
		_sound_map[i] = false;
	}
}
void BassLib::LoadMusic(ResourceItem resitem)
{
	if (!(chan = BASS_StreamCreateFile(FALSE, resitem.Path, resitem.Offset, resitem.Length, BASS_SAMPLE_LOOP))
		&& !(chan = BASS_MusicLoad(FALSE, resitem.Path, resitem.Offset, resitem.Length, BASS_MUSIC_RAMP | BASS_SAMPLE_LOOP, 0))) {
		printf("Can't load file music, error code \n", BASS_ErrorGetCode());
	}
}
void BassLib::PlayMusic(int num_loop)
{
	if (num_loop == -1)
	{
		BASS_ChannelPlay(chan, true);
	}
	else
	{
		BASS_ChannelPlay(chan, false);
	}
}
void BassLib::PauseMusic()
{
	BASS_ChannelPause(chan);
}
void BassLib::ResumeMusic(){
	BASS_ChannelPlay(chan, true);
}
void BassLib::StopMusic()
{
	BASS_ChannelStop(chan);
}
bool BassLib::IsPlayingMusic()
{
	if (BASS_ChannelIsActive(chan) != BASS_ACTIVE_PLAYING)
	{
		return false;
	}
	else
	{
		return true;
	}
}

HSOUND BassLib::LoadSoundEffect(ResourceItem resitem)
{
	HSOUND hsound = _sound_idgen%CORE_GAME_MAX_SOUND;
	if (_sound_map[hsound]){
		if (_soundDATA[hsound] != NULL){
			delete[] _soundDATA[hsound];
			_soundDATA[hsound] = NULL;
		}
		//release old sound
	}

	FILE * stream = NULL;
	stream = fopen(resitem.Path, "rb");
	if (stream == NULL)
	{
		return NULL;
	}
	fseek(stream, 0, SEEK_END);
	int size = ftell(stream);
	size = size < resitem.Offset + resitem.Length ? size - resitem.Offset : resitem.Length;
	fseek(stream, resitem.Offset, SEEK_SET);

	_soundDATA[hsound] = new UINT8[size];
	fread(_soundDATA[hsound], sizeof(BYTE), size, stream);
	fclose(stream);
	_soundSizeData[hsound] = size;
	_sound_idgen++;
	return hsound;
}
void BassLib::PlaySoundEffect(HSOUND handle)
{
	for (int i = 0; i < CORE_GAME_MAX_SOUND; i++)
	{
		if (BASS_ChannelIsActive(chan_sound[i]) != BASS_ACTIVE_PLAYING)
		{
			chan_sound[i] = BASS_StreamCreateFile(true, _soundDATA[handle], 0, _soundSizeData[handle], BASS_STREAM_AUTOFREE);
			BASS_ChannelPlay(chan_sound[i], true);
			_soundHandle[i] = handle;
			break;
		}
	}
}
void BassLib::StopSoundEffect(HSOUND handle)
{
	for (int i = 0; i < CORE_GAME_MAX_SOUND; i++){
		if (_soundHandle[i] == handle){
			if (BASS_ChannelIsActive(chan_sound[i]) == BASS_ACTIVE_PLAYING){
				BASS_ChannelStop(chan_sound[i]);
			}
		}
	}
}
void BassLib::StopAllSoundEffect()
{
	for (int i = 0; i < CORE_GAME_MAX_SOUND; i++)
	{
		BASS_ChannelStop(chan_sound[i]);
	}
}
void BassLib::PauseAllSoundEffect()
{
}
void BassLib::ResumeAllSoundEffect()
{
}
void BassLib::UnloadSoundEffect(HSOUND handle){

}

void BassLib::ClearAllSFX()
{

}