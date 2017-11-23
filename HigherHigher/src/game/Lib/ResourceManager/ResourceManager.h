#if !defined(_H_RESOURCE_MANAGER)
#define _H_RESOURCE_MANAGER
#include "../DataType/CType.h"
#include "../../GameCore/Os_Functions.h"
class ResourceManager{
protected:
	char** _res_filename;
	int* _res_idtofile;
	int* _res_info;
	int _num_filename;
	int _num_idtofile;
	int _num_info;
public:
	ResourceManager(char** res_filename, int num_filename, int* res_idtofile, int num_idtofile, int* res_info, int num_info);
	const char* GetFileName(UINT32 res_id);
};
#endif