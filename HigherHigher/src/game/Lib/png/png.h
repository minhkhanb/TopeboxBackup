#ifndef _F_PNG
#define _F_PNG
#include "../DataInputStream/DataInputStream.h"
#include "../DataType/CType.h"
/**
* Pngpic and some function to working with png file
* Copyright(c) Nguyen Duy 2011
* Tratstil@gmail.com
* Modify to work with pine core. 2013
*/
#define PNG_OK			1//(1<<0)
#define PNG_NOTPNG		2//(1<<1)
#define PNG_NOTFOUND	4//(1<<2)

//support chunk id
#define PNG_H_IHDR		0
#define PNG_H_IDAT		1
#define PNG_H_IEND		2
#define PNG_H_BKGD		3
#define PNG_H_PLTE		4
#define PNG_HNUM		5

//support output color type
#define PNG_COLOR_ARGB	0
#define PNG_COLOR_RGBA	1
#define PNG_COLOR_RGB	2

// png chunk ==============================================
class pngchunk{
public:
	unsigned char _type[4];
	unsigned int _length;
	unsigned char* _buffer;
	unsigned int _CRC;
	pngchunk(){
		_buffer = NULL; 
		_length = 0;
	};
	~pngchunk(){
		if (_buffer != NULL && _length > 0){
			delete[] _buffer; _buffer = NULL; _length = 0;
		}
	};
	void readChunk(DataInputStream* res){
		if(_buffer != NULL && _length > 0){
			delete[] _buffer; _buffer = NULL; _length = 0;
		}
		int len = res->ReadUInt32();
		_length = DataInputStream::RevertUInt32(len);

		res->ReadData(_type,4);
		if(_length > 0){
			_buffer = new unsigned char[_length];
			res->ReadData(_buffer,_length);
		}
		_CRC = res->ReadUInt32();
	};	
};
class PngPalette{
public:
	PngPalette();
	~PngPalette();
	int _num_color;
	unsigned char* _data;
};
class PNG_DATA;
class PNG_HANDLE{
public:
	int iWidth, iHeight;
	int iBitDepth, iColorType;
	int iCompressType, iFilterType;
	int iInterlaceType;
	int iBytePerSample, iSamplePerFixel;
	unsigned char* bpData;
	PNG_HANDLE();
	~PNG_HANDLE();
};

class Png{
private:
	PNG_DATA* _data;
public:
	static unsigned char _pngheader[8]; //png header [137,80,78,71,13,10,26,10]
	static unsigned char _pngchunkheader[PNG_HNUM*4];// header for each chunk
	Png();
	~Png();
	void clearData();
	unsigned int loadPng(const char* filename);
	unsigned int loadPng(const char* filename, unsigned char* buffer, int buffer_len, unsigned int out_colortype);
	unsigned int loadPng(ResourceItem resitem, unsigned char* buffer, int buffer_len, unsigned int out_colortype);
	static int getBytesPerSample(int bitdepth);
	static int getNumOfSample(int colortype);
	static int getColorSample(int bitdepth, int colortype);
	static int getTypeOfChunk(pngchunk* pngc);

	unsigned char* getImage(unsigned int out_colortype, unsigned char* buf);
	int getW();
	int getH();
	int getBitDepth();
	int getColorType();
	int getCompressType();
	int getFilterType();
	int getInterlaceType();
	int getBytePerSample();
	int getSamplePerPixel();
	int getBytePerPixel();
	int getBackgroundColor();
};
#endif