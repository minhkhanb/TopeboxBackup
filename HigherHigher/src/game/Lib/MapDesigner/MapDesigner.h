//Pine-Entertainment 2014
//Map Designer
//@author : Duy Nguyen
//@last update: 11/2014

#if !defined(_H_MAP_DESIGNER)
#define _H_MAP_DESIGNER
#include "../DataType/CType.h"
class MapDesigner{
protected:
	const char** _string_array;
	ResourceItem _res_item;
	BYTE* _buffer;
	int _version;
	int _format_version;
	int _header_len;
	//
	int _num_layer;
	UINT32 _layer_string;
	UINT32 _layer_0;
	UINT32 _num_row;
	UINT32 _num_col;

	//
	UINT32 _total_cells;
	//
	void loadDataToBuffer();
public:
	MapDesigner();
	~MapDesigner();
	void init(const char** string_array, const char* file_path);
	void init(const char** string_array, ResourceItem res_item);
	void release();
	UINT32 getColNum();
	UINT32 getRowNum();
	int getLayerNum();
	const char* getStringAt(int col, int row);
	const char* getStringAt(int cell_offset);
	UINT32 getObjectAt(unsigned int layer, int col, int row);
	UINT32 getObjectAt(unsigned int layer, int cell_offset);
};
#endif