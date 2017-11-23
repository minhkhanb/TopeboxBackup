//Pine-Entertainment 2014
//Map Designer
//@author : Duy Nguyen
//@last update: 11/2014

#include "pch.h"
#include "MapDesigner.h"
#include "../DataInputStream/DataInputStream.h"
MapDesigner::MapDesigner(){
	_res_item.Path[0] = '\0';
	_res_item.Length = -1;
	_res_item.Offset = 0;
	_buffer = NULL;
	_string_array = NULL;
	_layer_string = -1;
	_total_cells = 0;
}
MapDesigner::~MapDesigner(){
	release();
}
void MapDesigner::init(const char** string_array, const char* file_path){
	release();
	_string_array = string_array;
	sprintf(_res_item.Path, "%s", file_path);
	_res_item.Length = -1;
	_res_item.Offset = 0;
	loadDataToBuffer();
}
void MapDesigner::init(const char** string_array, ResourceItem res_item){
	release();
	_res_item = res_item;
	_string_array = string_array;
	loadDataToBuffer();

}
void MapDesigner::release(){
	if (_buffer != NULL){
		delete[] _buffer;
		_buffer = NULL;
	}
	_string_array = NULL;
	_layer_string = -1;
}
void MapDesigner::loadDataToBuffer(){
	if (_buffer == NULL){
		FILE* file = NULL;
		file = fopen(_res_item.Path, "rb");
		if (file != NULL){
			fseek(file,0, SEEK_END);
			int fsize = ftell(file);
			if (_res_item.Length > 0){
				fsize = fsize < _res_item.Offset + _res_item.Length ? fsize - _res_item.Offset: _res_item.Length;
			}
			fseek(file, _res_item.Offset, SEEK_SET);
			_buffer = new BYTE[fsize];
			fread(_buffer, 1, fsize, file);

			//analys
			_version = _buffer[0];
			_format_version = _buffer[1];
			//PDEBUG("map type:%d formatver:%d\n", _version, _format_version);
			int offset = 2;
			if (_version == 1){
				switch (_format_version)
				{
				case 2:
					_num_col = DataInputStream::ReadUInt32(_buffer, offset); offset += 4;
					_num_row = DataInputStream::ReadInt32(_buffer, offset); offset += 4;
					_total_cells = _num_col * _num_row;
					_num_layer = DataInputStream::ReadUInt16(_buffer, offset); offset += 2;
					_layer_string = DataInputStream::ReadInt16(_buffer, offset); offset += 2;
					_header_len = offset;
					//PDEBUG("col:%d row:%d layernum:%d\n", _num_col, _num_row, _num_layer);
					break;
				default:
					break;
				}
				//version 1

				//version 2
			}
			fclose(file);
		}
	}
}
UINT32 MapDesigner::getColNum(){
	return _num_col;
}
UINT32 MapDesigner::getRowNum(){
	return _num_row;
}
int MapDesigner::getLayerNum(){
	return _num_layer;
}
const char* MapDesigner::getStringAt(int col, int row){
	if (_string_array != NULL){
		UINT32 str_index = getObjectAt(_layer_string, col, row);
		return _string_array[str_index];
	}
	return NULL;
}
const char* MapDesigner::getStringAt(int cell_offset){
	if (_string_array != NULL){
		UINT32 str_index = getObjectAt(_layer_string, cell_offset);
		return _string_array[str_index];
	}
	return NULL;
}
UINT32 MapDesigner::getObjectAt(unsigned int layer, int col, int row){
	if (layer < _num_layer){
		int offset = _header_len + (layer * _num_col * _num_row * 4) + row * _num_col * 4 + col * 4 ;
		return DataInputStream::ReadUInt32(_buffer, offset);
	}
	return 0;
}
UINT32 MapDesigner::getObjectAt(unsigned int layer, int cell_offset){
	if (layer < _num_layer && cell_offset < _total_cells){
		int offset = _header_len + (layer * _num_col * _num_row * 4) + cell_offset * 4;
		return DataInputStream::ReadUInt32(_buffer, offset);
	}
	return 0;
}