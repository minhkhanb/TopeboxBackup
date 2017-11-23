#include "pch.h"
#include "Sound.h"

Sound::Sound(ResourceItem resitem){
	_buffer = NULL;
	_loaded = false;
	sprintf(_res_item.Path, "%s", resitem.Path);
	_res_item.Offset = resitem.Offset;
	_res_item.Length = resitem.Length;
	//PDEBUG("Create sound path:%s offset:%d", _res_item.Path, _res_item.Offset);
	//_res_item = resitem;
};
Sound::~Sound(){
}
bool Sound::load(){
	PDEBUG("Sound Load:%s offset:%d length:%d",_res_item.Path, _res_item.Offset,_res_item.Length);
	if(_loaded){
		PDEBUG("loaded");
		return false;
	}
	if(_buffer != NULL){
		delete[] _buffer;
		_buffer = NULL;
	}
	_buffer_length = 0;
	FILE* fp = fopen(_res_item.Path, "rb");
	if (fp) {
		_info.readWaveHeader(fp, _res_item.Offset);
		_buffer_length = _info._dataHeader._chunkSize;
		unsigned long data_size = _info._dataHeader._chunkSize/ sizeof(short);
		
		PDEBUG("_buffer_length = %lu \n", _buffer_length);
        _buffer = new short[data_size];
        fread(_buffer, sizeof(short), data_size, fp);

		_loaded = true;
		fclose(fp);
    }else{
		PDEBUG("loaded fail\n");
	}
	return _loaded;
}

void Sound::unload() {
	delete[] _buffer;
	_buffer = NULL; _buffer_length = 0;
	_loaded = false;
}