#include "pch.h"
#include "../../GameCore/CGame.h"
#include <string.h>
int CGame::_current_version = SIMULATOR_VERSION;
extern "C" float GetAccelX();
extern "C" float GetAccelY();
extern "C" float GetAccelZ();
extern "C" int  GetMotionAnchor();
extern "C" void SetMotionAnchor(int anchor);
//extern "C" float _sensor_x;

extern "C" int OS_ReadAppData(const char* filename, char* readed_data);
extern "C" bool OS_SaveAppData(const char* filename, const char* data_to_save, int data_length, bool is_append);
/////////////////////////////////SAVE RECORD STORE SYYYYYYYYYYYYYYYYYYYTEMMMMMMMMMMMMMMM




void CGame::GameDataLoad()
{
	int saved =0;
#ifdef MAC_OS
	saved = OS_ReadAppData(GAME_FILE, _buffer_save);
#else
	FILE * stream = NULL;
	stream = fopen(GAME_FILE, "rb");
	if (stream != NULL)
	{
		fseek(stream,0,SEEK_END);
		int size = ftell(stream);
		fseek(stream,0,SEEK_SET);
		fread((void *)_buffer_save,GAME_DATA_LENGTH,size,stream);
		fclose(stream);
		saved = size;
	}	
#endif
	
	if(!saved)
	{
		GameDataDefault();
		GameDataSave();
	}
	else
	{
		GameLoadDataFromBuffer();
	}

}
void CGame::GameDataSave()
{
	GameDataToBuffer();
#ifdef MAC_OS
	OS_SaveAppData(GAME_FILE,_buffer_save, GAME_DATA_LENGTH, false);
#else

	FILE * stream = NULL;

	stream = fopen(GAME_FILE, "wb");

	if (stream != NULL)
	{
		fwrite((void *)_buffer_save, sizeof(BYTE), GAME_DATA_LENGTH, stream);
	}

	if (stream != NULL)
	{
		fclose(stream);
	}
#endif
}
void CGame::GameDataDefault()
{
	_game_version = GAME_VERSION;
    
}

void CGame::GameDataToBuffer()
{
	int value;
	//SetInt32At(_buffer_save,GAME_DATA_VERSION,_game_version);
	
}
void CGame::GameLoadDataFromBuffer()
{
	int value;
	_game_version =  GetInt32At(_buffer_save,GAME_DATA_VERSION);
	if(GAME_VERSION != _game_version)
	{
		_game_version = GAME_VERSION;
	}
	
}


