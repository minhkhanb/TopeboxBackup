#include "pch.h"
#include "FPS_Utils.h"
#include <string>
void FPS_Utils::Begin(int log_index)
{
	_log_used[log_index] = true;
	_log_time[log_index] = clock();
}
void FPS_Utils::End(int log_index)
{
	_log_time[log_index] = clock()-_log_time[log_index];
}
void FPS_Utils::ClearAllLog()
{
	for(int i = 0; i < FPS_MAX_SLOT; i++)
	{
		_log_used[i] = false;
		
	}
}
const char* FPS_Utils::GetLogInfo()
{
	sprintf(_log_info,"");
	for(int i = 0; i < FPS_MAX_SLOT; i++)
	{
		if(_log_used[i])
		{
			sprintf(_log_info,"%s ,[%d,%d]  ",_log_info,i,_log_time[i]);
		}
		
	}
	return _log_info;
}