#include "pch.h"
#include "Music.h"
#include <string.h>
Music::Music(int bufflen):
	_loop_num(0), _buffer(bufflen), _buffer_double(bufflen),
	_curr_buffer(NULL), _curr_offset(0), _revision(0),
	_is_attach_music(false), _is_loadding(false)
{
	_curr_buffer = &_buffer;
}
Music::~Music(){
}
void* Music::LoadBuffer(void* context){
	
	Music* music = (Music*)context;
	PDEBUG("begin load buffer\n");
	if(music->_curr_buffer == &music->_buffer){
		PDEBUG("load in buffer 1\n");
	}else{
		PDEBUG("load in buffer 2\n");
	}
	BYTE test[200000];
	if(music->_is_attach_music){
		music->_is_loadding = true;
		FILE* file = fopen(music->_res_item.Path, "rb");
		if(file != NULL){
			fseek(file, 0, SEEK_END);
			int fsize = ftell(file);
			if(fsize <= music->_curr_offset){
				music->_curr_offset = music->_res_item.Offset;
			}
			fseek(file, music->_curr_offset, SEEK_SET);
			int loaded = fread((BYTE*)music->_curr_buffer->Data(),1, music->_curr_buffer->Length(), file);
			//int loaded = fread(test,1, 10000, file);
			music->_curr_offset += loaded;
			music->_curr_buffer->AvailableLength = loaded;
			fclose(file);
			music->_revision ++;
		}else{
			PDEBUG("loaded fail\n");
		}
		music->_is_loadding = false;
	}
	PDEBUG("load finish\n");
}
UINT32 Music::GetRevision(){
	return _revision;
}
const MusicBuffer* Music::GetNextBuffer(){
	//swapbuffer for next load
	MusicBuffer* buff = _curr_buffer;
	if(_is_loadding){
		PDEBUG("waiting for complete\n");
		pthread_join(_curr_thread,NULL);
	}
	if(_curr_buffer == &_buffer){
		_curr_buffer = &_buffer_double;
	}else{
		_curr_buffer = &_buffer;
	}
	PDEBUG("begin run thread\n");
	pthread_create(&_curr_thread, NULL, &Music::LoadBuffer, this);
	//pthread_join(_curr_thread,NULL);
	if(buff == &_buffer){
		PDEBUG("return buffer 1\n");
	}else{
		PDEBUG("return buffer 2\n");
	}
	return buff;
}
void Music::LoadMusic(ResourceItem resitem){
	_res_item = resitem;
	_is_attach_music = true;
	_curr_offset = resitem.Offset;
	pthread_create(&_curr_thread, NULL, &Music::LoadBuffer, this);
	pthread_join(_curr_thread,NULL);
}