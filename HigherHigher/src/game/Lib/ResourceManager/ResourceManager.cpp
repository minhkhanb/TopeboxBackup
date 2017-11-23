#include "ResourceManager.h"
ResourceManager::ResourceManager(char** res_filename, int num_filename, int* res_idtofile, int num_idtofile, int* res_info, int num_info){
	_res_filename = res_filename;
	_res_idtofile = res_idtofile;
	_res_info = res_info;
	_num_filename = num_filename;
	_num_idtofile = num_idtofile;
	_num_info = num_info;
}
const char* ResourceManager::GetFileName(UINT32 res_id){
	return _res_filename[_res_idtofile[res_id]];
}
