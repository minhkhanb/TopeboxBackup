/**
* Png and some function to working with png file
* Copyright(c) Nguyen Duy 2011
* Tratstil@gmail.com
* Modify to work with pine core. 2013
*/
#include "pch.h"
#include <stdio.h>
#include <assert.h>
#include "png.h"
//#if isBuildOs(OS_WINDOWS)
//#include "../../lib/zlib_1.2.5/win32/zconf.h"
//#elif defined(__GNUC__)
//#include "../../lib/zlib_1.2.5/gcc/zconf.h"
//#endif
//#include "../../lib/zlib_1.2.5/zlib.h"
#include <cstdlib>
#include <math.h>
#define MINIZ_NO_STDIO
#define MINIZ_NO_ARCHIVE_APIS
#define MINIZ_NO_TIME
#define MINIZ_NO_ZLIB_APIS
#define MINIZ_NO_MALLOC
#include "miniz.c"
//#include <Images\RawImages.h>
// define =================================================
/* current not use
#ifndef PNG_CHUNK
#define PNG_CHUNK		32768
//#define	CHUNK		16384
#endif
*/
PngPalette::PngPalette(){
	_data = NULL;
}
PngPalette::~PngPalette(){
	if(_data != NULL){
		delete[] _data;
		_data = NULL;
	}
}
// png data ===============================================
#define IN_BUF_SIZE (32768)
#define OUT_BUF_SIZE (32768)
#define STATIC_BUF_SIZE (3000*3000*4)
#define BUFFER_STACK_SIZE	10
//typedef unsigned char byte;
class PNG_DATA{
public:
	//management buffer
	static unsigned char* _buffer_stack[BUFFER_STACK_SIZE];
	static unsigned int  _buffer_len_stack[BUFFER_STACK_SIZE];
	

	static int _buffer_stack_count;
	static int _buffer_user_counter;
	static unsigned char *_static_buffer;
	static unsigned int _static_buffer_len;

	unsigned int _w,_h,_bitdepth,_colortype,_compresstype,_filtertype,_interlacetype,_bufoffset,_buflength,_bytepersample,_numofsample,_bgcolor;
	unsigned char* _raw_buffer;
	PngPalette* _palette;
	int _IDAT_length;
	bool _ready_for_read;

	PNG_DATA(){
		_raw_buffer = NULL;
		_w = _h = _bitdepth = _colortype = _compresstype = _filtertype = _interlacetype = _bufoffset = -1;
		_buflength = _IDAT_length = 0; _ready_for_read = false;
		_palette = NULL;
	};
	~PNG_DATA(){
		if(_raw_buffer != NULL){
			delete[] _raw_buffer;
			_raw_buffer = NULL;
		}
		if(_palette != NULL){
			delete _palette;
			_palette = NULL;
		}
	};
	void initBuffer(){
		if (_buffer_user_counter == 0){
			_static_buffer = new unsigned char[STATIC_BUF_SIZE];
		}
		_buffer_user_counter++;
	};
	void releaseBuffer(){
		_buffer_user_counter--;
		if (_buffer_user_counter <= 0 && _static_buffer != NULL){
			delete[] _static_buffer;
		}
	};
	bool popBuffer(unsigned char* buffer, int buffer_len){
		if (_buffer_stack_count < BUFFER_STACK_SIZE){
			if (_static_buffer != NULL){
				_buffer_stack[_buffer_stack_count] = _static_buffer;
				_buffer_len_stack[_buffer_stack_count] = _static_buffer_len;
				_buffer_stack_count++;
			}
			_static_buffer = buffer;
			_static_buffer_len = buffer_len;
			return true;
		}
		return false;
	};
	unsigned char* pushBuffer(){
		unsigned char* tmp = NULL;
		unsigned int tmp_len = 0;
		if (_static_buffer != NULL){
			tmp = _static_buffer;
			tmp_len = _static_buffer_len;
		}
		if (_buffer_stack_count > 0){
			_static_buffer = _buffer_stack[_buffer_stack_count - 1];
			_static_buffer_len = _buffer_len_stack[_buffer_stack_count - 1];
			_buffer_stack_count--;
		}
		return tmp;
	};
	bool unZipBuffer(){
		unsigned char s_inbuf[IN_BUF_SIZE];
		unsigned char s_outbuf[OUT_BUF_SIZE];
		unsigned int infile_remaining = _IDAT_length;
		const void *next_in = s_inbuf;
		//const void *next_in = _raw_buffer;
		size_t avail_in = 0;
		void *next_out = s_outbuf;
		//void *next_out = _static_buffer;
		size_t avail_out = OUT_BUF_SIZE;
		size_t total_in = 0, total_out = 0;
		tinfl_decompressor inflator;
		tinfl_init(&inflator);
		int buffer_offset = 0;
		for ( ; ; ){
			size_t in_bytes, out_bytes;
			tinfl_status status;
			if (!avail_in){
				// Input buffer is empty, so read more bytes from input file.
				unsigned int n = IN_BUF_SIZE< infile_remaining?IN_BUF_SIZE:infile_remaining;
				//memcpy(s_inbuf,_raw_buffer + total_in,n);
				for(int in = 0; in < n; in++){
					s_inbuf[in] = _raw_buffer[total_in + in];
				}

				next_in = s_inbuf;
				avail_in = n;
				infile_remaining -= n;
			}

			in_bytes = avail_in;
			out_bytes = avail_out;
			
			status = tinfl_decompress(&inflator, (const mz_uint8 *)next_in, &in_bytes, s_outbuf, (mz_uint8 *)next_out, &out_bytes, (infile_remaining ? TINFL_FLAG_HAS_MORE_INPUT : 0) | TINFL_FLAG_PARSE_ZLIB_HEADER);
			//status = tinfl_decompress(&inflator, (const mz_uint8 *)_raw_buffer, &in_bytes, _static_buffer, (mz_uint8 *)next_out, &out_bytes, /*(infile_remaining ? TINFL_FLAG_HAS_MORE_INPUT : 0) |*/ TINFL_FLAG_PARSE_ZLIB_HEADER);
			
			avail_in -= in_bytes;
			next_in = (const mz_uint8 *)next_in + in_bytes;
			total_in += in_bytes;

			avail_out -= out_bytes;
			next_out = (mz_uint8 *)next_out + out_bytes;
			total_out += out_bytes;

			if ((status <= TINFL_STATUS_DONE) || (!avail_out)){
				// Output buffer is full, or decompression is done, so write buffer to output file.
				unsigned int n = OUT_BUF_SIZE - (unsigned int)avail_out;
				//memcpy(_buffer + total_out, s_outbuf , n);
				for(int out = 0; out < n; out++){
					if(buffer_offset < _buflength){
						_static_buffer[buffer_offset] = s_outbuf[out];
					}else{
						printf("PNG : buffer not enought\n");
					}
					buffer_offset++;
				}
				next_out = s_outbuf;
				avail_out = OUT_BUF_SIZE;
			}

			// If status is <= TINFL_STATUS_DONE then either decompression is done or something went wrong.
			if (status <= TINFL_STATUS_DONE){
				if (status == TINFL_STATUS_DONE){
					// Decompression completed successfully.
					return true;
					break;
				}
				else{
					// Decompression failed.
					printf("tinfl_decompress() failed with status %i!\n", status);
					return false;
				}
			}
		}
		return false;
	};
	
	int PaethPredictor(int a, int b, int c){
		// a = left, b = above, c = upper left
		int p = a + b - c; // initial estimate
		int pa = abs(p - a); // distances to a, b, c
		int pb = abs(p - b);
		int pc = abs(p - c);
		// return nearest of a,b,c,
		// breaking ties in order a,b,c.
		if (pa <= pb && pa <= pc) return a;
		else if(pb <= pc) return b;
		else return c;
	};
	bool unfilterBuffer(){
		int byteperfixel = _bytepersample * _numofsample;
		int eachscanlinebyte = (_w *byteperfixel) +1;
		int workingoffset = 0;
		int begin_offset = 0;
		for(int i = 0; i< _h; i++){
			int workingroot = workingoffset; // begin offset of current output line
			workingoffset++;
			int filtertype = _static_buffer[i*eachscanlinebyte];
			//printf("[%d %d %d %d] |",i,filtertype,workingroot,eachscanlinebyte);
			switch(filtertype){
			case 0:{//none
					workingoffset+= eachscanlinebyte;
				}break;
			case 1:{//sub
					for(int j = 0; j < eachscanlinebyte -1; j++){
						int sub = _static_buffer[workingoffset];
						int poslast = j - byteperfixel;
						int lastvalue = ((poslast<0)?0:_static_buffer[ workingroot + 1 + poslast]);
						_static_buffer[workingoffset] = (lastvalue + sub)%256;
						workingoffset++;
					}
				}break;
			case 2:{//up
					for(int j = 0; j < eachscanlinebyte -1; j++){
						int up = _static_buffer[workingoffset];
						int poslast = workingoffset - eachscanlinebyte;
						int lastvalue = ((poslast<0)?0:_static_buffer[poslast]);
						_static_buffer[workingoffset] = (up + lastvalue)%256;
						workingoffset++;
					}
				}break;
			case 3:{//average Average(x) = Raw(x) - floor((Raw(x-bpp)+Prior(x))/2) => Raw(x) = Average(x) + floor((Raw(x-bpp)+Prior(x))/2)
					for(int j = 0; j < eachscanlinebyte -1; j++){
						int ave = _static_buffer[workingoffset];
						int sublast = j - byteperfixel;
						int subvalue = ((sublast<0)?0:_static_buffer[workingroot + 1 + sublast]);

						int uplast = workingoffset - eachscanlinebyte;
						int upvalue = ((uplast<0)?0:_static_buffer[uplast]);
						_static_buffer[workingoffset] = int(float(ave) + int(floor(float(subvalue+upvalue)/2)))%256;
						workingoffset++;
					}
				}break;
			case 4:{//Paeth: Paeth(x) = Raw(x) - PaethPredictor(Raw(x-bpp), Prior(x), Prior(x-bpp)) => Raw(x) = Paeth(x) + PaethPredictor(Raw(x-bpp), Prior(x), Prior(x-bpp))
					for(int j = 0; j < eachscanlinebyte -1; j++){
						//PaethPredictor(int a, int b, int c);// a = left, b = above, c = upper left
						int Paeth = _static_buffer[workingoffset];

						int sub = j - byteperfixel;
						int subvalue = ((sub<0)?0:_static_buffer[workingroot + 1 + sub]);

						int up = workingoffset - eachscanlinebyte;
						int upvalue = ((up<0)?0:_static_buffer[up]);

						int upsub = workingroot + 1 + sub - eachscanlinebyte;
						int upsubvalue = ((upsub<0)?0:((sub<0)?0:_static_buffer[upsub]));
						_static_buffer[workingoffset] = (Paeth + PaethPredictor(subvalue,upvalue,upsubvalue))%256;
						workingoffset++;
					}
				}break;
			}
		}
		return true;
	};
	int getNumOfSample(int colortype){
		switch(colortype){
		case 0:return 1;//Each pixel is a grayscale sample.
		case 2:return 3;//Each pixel is an R,G,B triple.
		case 3:return 1;//Each pixel is a palette index; a PLTE chunk must appear.
		case 4:return 2;//Each pixel is a grayscale sample, followed by an alpha sample.
		case 6:return 4;//Each pixel is an R,G,B triple, followed by an alpha sample.
		}
		return -1;
	};
	void forceChunk(pngchunk* pngc){
		int ctype = Png::getTypeOfChunk(pngc);
		//PDEBUG("chunck name:");
		for(int i = 0; i< 4; i++){
		//	PDEBUG("%c ",pngc->_type[i]);
		}
		//PDEBUG("\n");
		if(ctype == -1) return;
		switch(ctype){
		case PNG_H_IHDR:{
				unsigned int offset = 0;
				_w = DataInputStream::ReadUInt32(pngc->_buffer,offset); offset+=sizeof(unsigned int);
				_h = DataInputStream::ReadUInt32(pngc->_buffer,offset); offset+=sizeof(unsigned int);
				_bitdepth = pngc->_buffer[offset]; offset+=sizeof(unsigned char);
				_colortype = pngc->_buffer[offset]; offset+=sizeof(unsigned char);
				_compresstype = pngc->_buffer[offset]; offset+= sizeof(unsigned char);
				_filtertype = pngc->_buffer[offset]; offset+= sizeof(unsigned char);
				_interlacetype = pngc->_buffer[offset];
				_bytepersample = Png::getBytesPerSample(_bitdepth);
				_numofsample = getNumOfSample(_colortype);
				int eachscanlinebyte = (_w *_bytepersample * _numofsample) +1;
				_buflength = eachscanlinebyte*_h; _bufoffset = 0;
				_raw_buffer = NULL;//new unsigned char[_buflength];
				//_buffer = new unsigned char[_buflength];
				_IDAT_length = 0;
				PDEBUG("png bitdepth:%d colortype:%d bytepersample:%d samplenum:%d _filtertype:%d _buflength:%d\n", _bitdepth, _colortype, _bytepersample, _numofsample,_filtertype,_buflength);
				PDEBUG("png w:%d h:%d compresstype:%d\n", _w, _h, _compresstype);
			}break;
		case PNG_H_PLTE:{
				PDEBUG("need palette\n");
				int numofcolor = pngc->_length/3;
				PDEBUG("palette color num:%d\n", numofcolor);
				_palette = new PngPalette();
				_palette->_data = new unsigned char[pngc->_length];
				_palette->_num_color = numofcolor;
				for(int i = 0;i< numofcolor * 3; i++){
					_palette->_data[i] = pngc->_buffer[i];
				}
			}break;
		case PNG_H_IDAT:{
				appendIDAT(pngc);
			}break;
		case PNG_H_IEND:{
				if(_IDAT_length > 0){
					_raw_buffer = new unsigned char[_IDAT_length];
					for(int i = 0; i< _IDAT_length; i++){
						_raw_buffer[i] = _static_buffer[i];
					}
				}else{
					_raw_buffer = NULL;
				}
				_ready_for_read = true;
				/*if(unZipBuffer()){
					if(unfilterBuffer()){
						_ready_for_read = true;
						if(_buffer != NULL){
							delete[] _buffer;
							_buffer = NULL;
						}
					}
				}*/
			}break;
		case PNG_H_BKGD:{
				switch(_colortype){
				case 0:{//Each pixel is a grayscale sample.
						//_bgcolor = 
					}break;
				case 2:{//Each pixel is an R,G,B triple.
						//0x00|color>>16,0x00|color>>8,0x00|color,0x00|color>>24
						
					}break;
				case 3:{//Each pixel is a palette index; a PLTE chunk must appear.
					}break;
				case 4:{//Each pixel is a grayscale sample, followed by an alpha sample.
					}break;
				case 6:{//Each pixel is an R,G,B triple, followed by an alpha sample.
					}break;
				}
			}break;
		}
	};
	void appendIDAT(pngchunk* pngc){
		for (int i = 0; i < pngc->_length; i++){
			_static_buffer[_IDAT_length] = pngc->_buffer[i];
			_IDAT_length++;
		}
	};
};

int PNG_DATA::_buffer_user_counter = 0;
unsigned char *PNG_DATA::_static_buffer = NULL;
unsigned char* PNG_DATA::_buffer_stack[BUFFER_STACK_SIZE] = { NULL };
unsigned int PNG_DATA::_buffer_len_stack[BUFFER_STACK_SIZE] = { 0 };
int PNG_DATA::_buffer_stack_count = 0;
unsigned int PNG_DATA::_static_buffer_len = 0;
/*
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif
*/
/*u16 readu16(byte* buffer,u32 offset);
u32 readu32(byte* buffer,u32 offset);
void readBuffer(byte* des,byte* src,u32 offset,u32 length);
*/
unsigned char Png::_pngheader[8] = {137,80,78,71,13,10,26,10};
unsigned char Png::_pngchunkheader[PNG_HNUM*4] = {
	/*IHDR*/73,72,68,82,	
	/*IDAT*/73,68,65,84,	
	/*IEND*/73,69,78,68,
	/*BKGD*/66,75,71,68,
	/*PLTE*/'P','L','T','E',
};
Png::Png(){
	_data = new PNG_DATA();
}
Png::~Png(){
	clearData();
	delete _data;
	_data = NULL;
}
unsigned int Png::loadPng(const char* filename){
	_data->_ready_for_read = false;
	clearData();
	DataInputStream* res = new DataInputStream();
	if(res->Open(filename,0,-1)){
		//DynaResource* res = new DynaResource((char*)filename,0,-1);
		unsigned char header[8];
		res->ReadData(header,8);
		for(int i = 0; i< 8; i++){
			if(header[i] != _pngheader[i]){
				return PNG_NOTPNG;
			}
		}
		pngchunk pngc;
		do{
			pngc.readChunk(res);
			_data->forceChunk(&pngc);
		}while(getTypeOfChunk(&pngc)!=PNG_H_IEND);
		res->Close();
		delete res;
		return PNG_OK;
	}
	delete res;
	return PNG_NOTFOUND;
}
unsigned int Png::loadPng(const char* filename, unsigned char* buffer, int buffer_len, unsigned int out_colortype){
	_data->_ready_for_read = false;
	clearData();
	_data->popBuffer(buffer, buffer_len);
	unsigned int rs = PNG_NOTPNG;
	DataInputStream* res = new DataInputStream();
	if (res->Open(filename, 0, -1)){
		unsigned char header[8];
		res->ReadData(header, 8);
		for (int i = 0; i< 8; i++){
			if (header[i] != _pngheader[i]){
				_data->pushBuffer();
				return PNG_NOTPNG;
			}
		}
		pngchunk pngc;
		do{
			pngc.readChunk(res);
			_data->forceChunk(&pngc);
		} while (getTypeOfChunk(&pngc) != PNG_H_IEND);
		res->Close();
		rs = PNG_OK;
	}
	delete res;
	if (rs == PNG_OK){
		if (!_data->_ready_for_read || !_data->unZipBuffer() || !_data->unfilterBuffer()){
			rs = PNG_NOTFOUND;
		}
		else{
			switch (out_colortype){
			case PNG_COLOR_RGB:{
								   if (_data->_colortype == 3){//palette

									   int byteperfixel = _data->_bytepersample * _data->_numofsample;
									   int output_size = 3 * _data->_w * _data->_h;
									   int output_linebyte = 3 * _data->_w;
									   unsigned char save[3];
									   save[0] = PNG_DATA::_static_buffer[1];
									   save[1] = PNG_DATA::_static_buffer[2];
									   save[2] = PNG_DATA::_static_buffer[3];
									   if (output_size <= buffer_len){
											   int eachscanlinebyte = (_data->_w *byteperfixel);
											   for (int i = _data->_h - 1; i >= 0; i--){
												   int out_offset = output_linebyte - 1;
												   for (int j = eachscanlinebyte - 1; j > 0; j --){
													   int palette_index = PNG_DATA::_static_buffer[i* eachscanlinebyte + j];
													   buffer[i * output_linebyte + out_offset] = _data->_palette->_data[palette_index * 3 + 3];
													   buffer[i * output_linebyte + out_offset - 1] = _data->_palette->_data[palette_index * 3 + 2];
													   buffer[i * output_linebyte + out_offset - 2] = _data->_palette->_data[palette_index * 3 + 1];
													   out_offset -= 3;
												   }
											   }
											   //buffer[0] = save[0];
											  // buffer[1] = save[1];
											  // buffer[2] = save[2];

									   }
									   else{
										   rs = PNG_NOTFOUND;
									   }
								   }
								   else if (_data->_colortype == 2){//RGB : bitdepth[8 or 16]
									   int byteperfixel = _data->_bytepersample * _data->_numofsample;
									   int eachscanlinebyte = (_data->_w *byteperfixel) + 1;
									   int offset = 0; int count = 0;
									   for (int i = 0; i < _data->_h; i++){
										   offset++;
										   for (int j = 0; j < eachscanlinebyte - 1; j++){
											   buffer[count] = PNG_DATA::_static_buffer[offset];
											   offset++;
											   count++;
										   }
									   }
								   }
								   else if (_data->_colortype == 6){//RGBA
									   int byteperfixel = _data->_bytepersample * _data->_numofsample;
									   if (byteperfixel == 4){
										   int eachscanlinebyte = (_data->_w *byteperfixel);
										   int offset = 0; int count = 0;
										   for (int i = 0; i<_data->_h; i++){
											   //the output buffer and data buffer is same but data buffer large than output size
											   //so we don't need caring about losing data.
											   offset++;//each line have a filter represent byte
											   for (int iline = 0; iline < eachscanlinebyte; iline+=4){
												   buffer[count * 3] = PNG_DATA::_static_buffer[offset];
												   buffer[count * 3 + 1] = PNG_DATA::_static_buffer[offset + 1];
												   buffer[count * 3 + 2] = PNG_DATA::_static_buffer[offset + 2];
												   offset += 4;
												   count++;
											   }
										   }
									   }
								   }

			}break;
			case PNG_COLOR_RGBA:{
									if (_data->_colortype == 3){//palette

										int byteperfixel = _data->_bytepersample * _data->_numofsample;
										int output_size = 4 * _data->_w * _data->_h;
										int output_linebyte = 4 * _data->_w;
										unsigned char save[3];
										save[0] = PNG_DATA::_static_buffer[1];
										save[1] = PNG_DATA::_static_buffer[2];
										save[2] = PNG_DATA::_static_buffer[3];
										if (output_size <= buffer_len){
											int eachscanlinebyte = (_data->_w *byteperfixel) + 1;
											for (int i = _data->_h - 1; i >= 0; i--){
												int out_offset = output_linebyte - 1;
												for (int j = eachscanlinebyte - 1; j > 0; j--){
													int palette_index = PNG_DATA::_static_buffer[i* eachscanlinebyte + j];
													buffer[i * output_linebyte + out_offset] = 0xFF;
													buffer[i * output_linebyte + out_offset - 1] = _data->_palette->_data[palette_index * 3 + 2];
													buffer[i * output_linebyte + out_offset - 2] = _data->_palette->_data[palette_index * 3 + 1];
													buffer[i * output_linebyte + out_offset - 3] = _data->_palette->_data[palette_index * 3];
													//buffer[i * output_linebyte + out_offset - 3] = 0xFF;
													out_offset -= 4;
												}
											}
											//buffer[0] = save[0];
											// buffer[1] = save[1];
											// buffer[2] = save[2];

										}
										else{
											rs = PNG_NOTFOUND;
										}
									}
									else if (_data->_colortype == 2){//RGB : bitdepth[8 or 16]
										int byteperfixel = _data->_bytepersample * _data->_numofsample;
										int output_size = 4 * _data->_w * _data->_h;
										int output_linebyte = 4 * _data->_w;
										unsigned char save[3];
										save[0] = PNG_DATA::_static_buffer[1];
										save[1] = PNG_DATA::_static_buffer[2];
										save[2] = PNG_DATA::_static_buffer[3];
										if (output_size <= buffer_len){
											if (byteperfixel == 3){
												int eachscanlinebyte = (_data->_w *byteperfixel);
												for (int i = _data->_h - 1; i >= 0; i--){
													int out_offset = output_linebyte - 1;
													for (int j = eachscanlinebyte - 1; j > 0; j -= 3){
														buffer[i * output_linebyte + out_offset] = 0xFF;
														buffer[i * output_linebyte + out_offset - 1] = PNG_DATA::_static_buffer[i* eachscanlinebyte + j];
														buffer[i * output_linebyte + out_offset - 2] = PNG_DATA::_static_buffer[i* eachscanlinebyte + j - 1];
														buffer[i * output_linebyte + out_offset - 3] = PNG_DATA::_static_buffer[i* eachscanlinebyte + j - 2];
														out_offset -= 4;
													}
												}
												buffer[0] = save[0];
												buffer[1] = save[1];
												buffer[2] = save[2];
											}
											else{
												rs = PNG_NOTFOUND;
											}
										}
										else{
											rs = PNG_NOTFOUND;
										}

										/*
										int byteperfixel = _data->_bytepersample * _data->_numofsample;
										if (byteperfixel == 3){
											int eachscanlinebyte = (_data->_w *byteperfixel);
											int offset = 0; int count = 0;
											for (int i = 0; i<_data->_h; i++){
							
												//becouse output buffer and data buffer is same then we must calc to save data.
												offset++;//each line have a filter represent byte
												for (int j = 0; j < eachscanlinebyte; j += 3){
													buffer[count * 4] = PNG_DATA::_static_buffer[offset];
													buffer[count * 4 + 1] = PNG_DATA::_static_buffer[offset + 1];
													buffer[count * 4 + 2] = PNG_DATA::_static_buffer[offset + 2];
													buffer[count * 4 + 3] = 0xFF;
													offset += 3;
													count++;
												}
											}
										}
										rs = PNG_NOTFOUND;
										*/
									}
									else if (_data->_colortype == 6){
										int byteperfixel = _data->_bytepersample * _data->_numofsample;
										if (byteperfixel == 4){
											int eachscanlinebyte = (_data->_w *byteperfixel);
											int offset = 0; int count = 0;
											for (int i = 0; i<_data->_h; i++){

												//the output buffer and data buffer is same but data buffer large than output size
												//so we don't need caring about losing data.
												offset++;//each line have a filter represent byte
												for (int iline = 0; iline < eachscanlinebyte; iline++){
													buffer[count * 4 + iline] = PNG_DATA::_static_buffer[offset + iline];
												}
												offset += eachscanlinebyte;
												count += _data->_w;
											}
										}
									}
			}break;
			default:
									rs = PNG_NOTFOUND;
				break;
			}
		}
	}
	_data->pushBuffer();
	return rs;
}

unsigned int Png::loadPng(ResourceItem resitem, unsigned char* buffer, int buffer_len, unsigned int out_colortype){
	_data->_ready_for_read = false;
	clearData();
	_data->popBuffer(buffer, buffer_len);
	unsigned int rs = PNG_NOTPNG;
	DataInputStream* res = new DataInputStream();
	if (res->Open(resitem.Path, resitem.Offset, resitem.Length)){
		unsigned char header[8];
		res->ReadData(header, 8);
		for (int i = 0; i< 8; i++){
			if (header[i] != _pngheader[i]){
				_data->pushBuffer();
				return PNG_NOTPNG;
			}
		}
		pngchunk pngc;
		do{
			pngc.readChunk(res);
			_data->forceChunk(&pngc);
		} while (getTypeOfChunk(&pngc) != PNG_H_IEND);
		res->Close();
		rs = PNG_OK;
	}
	delete res;
	if (rs == PNG_OK){
		if (!_data->_ready_for_read || !_data->unZipBuffer() || !_data->unfilterBuffer()){
			rs = PNG_NOTFOUND;
		}
		else{
			switch (out_colortype){
			case PNG_COLOR_RGB:{
								   if (_data->_colortype == 3){//palette

									   int byteperfixel = _data->_bytepersample * _data->_numofsample;
									   int output_size = 3 * _data->_w * _data->_h;
									   int output_linebyte = 3 * _data->_w;
									   unsigned char save[3];
									   save[0] = PNG_DATA::_static_buffer[1];
									   save[1] = PNG_DATA::_static_buffer[2];
									   save[2] = PNG_DATA::_static_buffer[3];
									   if (output_size <= buffer_len){
											   int eachscanlinebyte = (_data->_w *byteperfixel);
											   for (int i = _data->_h - 1; i >= 0; i--){
												   int out_offset = output_linebyte - 1;
												   for (int j = eachscanlinebyte - 1; j > 0; j --){
													   int palette_index = PNG_DATA::_static_buffer[i* eachscanlinebyte + j];
													   buffer[i * output_linebyte + out_offset] = _data->_palette->_data[palette_index * 3 + 3];
													   buffer[i * output_linebyte + out_offset - 1] = _data->_palette->_data[palette_index * 3 + 2];
													   buffer[i * output_linebyte + out_offset - 2] = _data->_palette->_data[palette_index * 3 + 1];
													   out_offset -= 3;
												   }
											   }
											   //buffer[0] = save[0];
											  // buffer[1] = save[1];
											  // buffer[2] = save[2];

									   }
									   else{
										   rs = PNG_NOTFOUND;
									   }
								   }
								   else if (_data->_colortype == 2){//RGB : bitdepth[8 or 16]
									   int byteperfixel = _data->_bytepersample * _data->_numofsample;
									   int eachscanlinebyte = (_data->_w *byteperfixel) + 1;
									   int offset = 0; int count = 0;
									   for (int i = 0; i < _data->_h; i++){
										   offset++;
										   for (int j = 0; j < eachscanlinebyte - 1; j++){
											   buffer[count] = PNG_DATA::_static_buffer[offset];
											   offset++;
											   count++;
										   }
									   }
								   }
								   else if (_data->_colortype == 6){//RGBA
									   int byteperfixel = _data->_bytepersample * _data->_numofsample;
									   if (byteperfixel == 4){
										   int eachscanlinebyte = (_data->_w *byteperfixel);
										   int offset = 0; int count = 0;
										   for (int i = 0; i<_data->_h; i++){
											   //the output buffer and data buffer is same but data buffer large than output size
											   //so we don't need caring about losing data.
											   offset++;//each line have a filter represent byte
											   for (int iline = 0; iline < eachscanlinebyte; iline+=4){
												   buffer[count * 3] = PNG_DATA::_static_buffer[offset];
												   buffer[count * 3 + 1] = PNG_DATA::_static_buffer[offset + 1];
												   buffer[count * 3 + 2] = PNG_DATA::_static_buffer[offset + 2];
												   offset += 4;
												   count++;
											   }
										   }
									   }
								   }

			}break;
			case PNG_COLOR_RGBA:{
									if (_data->_colortype == 3){//palette

										int byteperfixel = _data->_bytepersample * _data->_numofsample;
										int output_size = 4 * _data->_w * _data->_h;
										int output_linebyte = 4 * _data->_w;
										unsigned char save[3];
										save[0] = PNG_DATA::_static_buffer[1];
										save[1] = PNG_DATA::_static_buffer[2];
										save[2] = PNG_DATA::_static_buffer[3];
										if (output_size <= buffer_len){
											int eachscanlinebyte = (_data->_w *byteperfixel) + 1;
											for (int i = _data->_h - 1; i >= 0; i--){
												int out_offset = output_linebyte - 1;
												for (int j = eachscanlinebyte - 1; j > 0; j--){
													int palette_index = PNG_DATA::_static_buffer[i* eachscanlinebyte + j];
													buffer[i * output_linebyte + out_offset] = 0xFF;
													buffer[i * output_linebyte + out_offset - 1] = _data->_palette->_data[palette_index * 3 + 2];
													buffer[i * output_linebyte + out_offset - 2] = _data->_palette->_data[palette_index * 3 + 1];
													buffer[i * output_linebyte + out_offset - 3] = _data->_palette->_data[palette_index * 3];
													//buffer[i * output_linebyte + out_offset - 3] = 0xFF;
													out_offset -= 4;
												}
											}
											//buffer[0] = save[0];
											// buffer[1] = save[1];
											// buffer[2] = save[2];

										}
										else{
											rs = PNG_NOTFOUND;
										}
									}
									else if (_data->_colortype == 2){//RGB : bitdepth[8 or 16]
										int byteperfixel = _data->_bytepersample * _data->_numofsample;
										int output_size = 4 * _data->_w * _data->_h;
										int output_linebyte = 4 * _data->_w;
										unsigned char save[3];
										save[0] = PNG_DATA::_static_buffer[1];
										save[1] = PNG_DATA::_static_buffer[2];
										save[2] = PNG_DATA::_static_buffer[3];
										if (output_size <= buffer_len){
											if (byteperfixel == 3){
												int eachscanlinebyte = (_data->_w *byteperfixel);
												for (int i = _data->_h - 1; i >= 0; i--){
													int out_offset = output_linebyte - 1;
													for (int j = eachscanlinebyte - 1; j > 0; j -= 3){
														buffer[i * output_linebyte + out_offset] = 0xFF;
														buffer[i * output_linebyte + out_offset - 1] = PNG_DATA::_static_buffer[i* eachscanlinebyte + j];
														buffer[i * output_linebyte + out_offset - 2] = PNG_DATA::_static_buffer[i* eachscanlinebyte + j - 1];
														buffer[i * output_linebyte + out_offset - 3] = PNG_DATA::_static_buffer[i* eachscanlinebyte + j - 2];
														out_offset -= 4;
													}
												}
												buffer[0] = save[0];
												buffer[1] = save[1];
												buffer[2] = save[2];
											}
											else{
												rs = PNG_NOTFOUND;
											}
										}
										else{
											rs = PNG_NOTFOUND;
										}

										/*
										int byteperfixel = _data->_bytepersample * _data->_numofsample;
										if (byteperfixel == 3){
											int eachscanlinebyte = (_data->_w *byteperfixel);
											int offset = 0; int count = 0;
											for (int i = 0; i<_data->_h; i++){
							
												//becouse output buffer and data buffer is same then we must calc to save data.
												offset++;//each line have a filter represent byte
												for (int j = 0; j < eachscanlinebyte; j += 3){
													buffer[count * 4] = PNG_DATA::_static_buffer[offset];
													buffer[count * 4 + 1] = PNG_DATA::_static_buffer[offset + 1];
													buffer[count * 4 + 2] = PNG_DATA::_static_buffer[offset + 2];
													buffer[count * 4 + 3] = 0xFF;
													offset += 3;
													count++;
												}
											}
										}
										rs = PNG_NOTFOUND;
										*/
									}
									else if (_data->_colortype == 6){
										int byteperfixel = _data->_bytepersample * _data->_numofsample;
										if (byteperfixel == 4){
											int eachscanlinebyte = (_data->_w *byteperfixel);
											int offset = 0; int count = 0;
											for (int i = 0; i<_data->_h; i++){

												//the output buffer and data buffer is same but data buffer large than output size
												//so we don't need caring about losing data.
												offset++;//each line have a filter represent byte
												for (int iline = 0; iline < eachscanlinebyte; iline++){
													buffer[count * 4 + iline] = PNG_DATA::_static_buffer[offset + iline];
												}
												offset += eachscanlinebyte;
												count += _data->_w;
											}
										}
									}
			}break;
			default:
									rs = PNG_NOTFOUND;
				break;
			}
		}
	}
	_data->pushBuffer();
	return rs;
}

int Png::getBytesPerSample(int bitdepth){
	/*switch(bitdepth){
	case 2:
	}*/
	return bitdepth/8 + ((bitdepth%8>0)?1:0);
}

int Png::getTypeOfChunk(pngchunk* pngc){
	for(unsigned int i = 0; i< PNG_HNUM; i++){
		bool is = true;
		for(int j = 0; j < 4; j++){
			if(pngc->_type[j] != _pngchunkheader[i*4 + j] && pngc->_type[j] != _pngchunkheader[i*4 + j] - 62){
				is = false; break;
			}
		}
		if(is) return i;
	}
	return -1;
}
void Png::clearData(){
	//if(_buffer != NULL) delete _buffer;
}


unsigned char* Png::getImage(unsigned int out_colortype, unsigned char* buffer){
	if(!_data->_ready_for_read || !_data->unZipBuffer() || !_data->unfilterBuffer()){
		return NULL;
	}
	switch(out_colortype){
	case PNG_COLOR_RGB:{
			if(_data->_colortype == 3){//palette
				int byteperfixel = _data->_bytepersample * _data->_numofsample;
				int eachscanlinebyte = (_data->_w *byteperfixel) +1;
				//byte* buffer = new byte[_data->_w*_data->_h*byteperfixel];
				int offset = 0; int count = 0;
				for(int i = 0; i<_data->_h; i++){
					offset++;
					for(int j = 0; j < eachscanlinebyte -1; j++){
						int palette_index = PNG_DATA::_static_buffer[offset];
						//memcpy(buffer + (count*3), _data->_palette + (palette_index * 3), 3);
						//buffer[count*3] = _data->_raw_buffer[offset];
						offset++;
						count++;
					}
				}
			}else{
				int byteperfixel = _data->_bytepersample * _data->_numofsample;
				int eachscanlinebyte = (_data->_w *byteperfixel) +1;
				int offset = 0; int count = 0;
				for(int i = 0; i<_data->_h; i++){
					offset++;
					for(int j = 0; j < eachscanlinebyte -1; j++){
						buffer[count] = PNG_DATA::_static_buffer[offset];
						offset++;
						count++;
					}
				}
			}
		}break;
	case PNG_COLOR_RGBA:{
			
			if(_data->_colortype == 3){//palette
				int byteperfixel = _data->_bytepersample * _data->_numofsample;
				int eachscanlinebyte = (_data->_w *byteperfixel);
				//byte* buffer = new byte[_data->_w*_data->_h*byteperfixel];
				int offset = 0; int count = 0;
				for(int i = 0; i<_data->_h; i++){
					offset++;//each line have a filter represent byte
					for(int j = 0; j < eachscanlinebyte; j++){
						int palette_index = PNG_DATA::_static_buffer[offset];
						buffer[count*4] = _data->_palette->_data[palette_index * 3];
						buffer[count*4+1] = _data->_palette->_data[palette_index * 3 + 1];
						buffer[count*4+2] = _data->_palette->_data[palette_index * 3 + 2];
						buffer[count*4+3] = 0xFF;
						//buffer[count*3] = _data->_raw_buffer[offset];
						offset++;
						count++;
					}
				}
			}else if(_data->_colortype == 2){//RGB : bitdepth[8 or 16]
				int byteperfixel = _data->_bytepersample * _data->_numofsample;
				if(byteperfixel == 3){
					int eachscanlinebyte = (_data->_w *byteperfixel);
					int offset = 0; int count = 0;
					for(int i = 0; i<_data->_h; i++){
						offset++;//each line have a filter represent byte
						for(int j = 0; j < eachscanlinebyte; j+=3){
							//memcpy(buffer + count*4,_data->_raw_buffer + offset,3);
							buffer[count*4] = PNG_DATA::_static_buffer[offset];
							buffer[count*4+1] = PNG_DATA::_static_buffer[offset+1];
							buffer[count*4+2] = PNG_DATA::_static_buffer[offset+2];
							buffer[count*4+3] = 0xFF;
							offset+=3;
							count++;
						}
					}
				}
			}else if(_data->_colortype == 6){
				int byteperfixel = _data->_bytepersample * _data->_numofsample;
				if(byteperfixel == 4){
					int eachscanlinebyte = (_data->_w *byteperfixel);
					int offset = 0; int count = 0;
					for(int i = 0; i<_data->_h; i++){
						offset++;//each line have a filter represent byte
						//memcpy(buffer + (count * 4), _data->_raw_buffer + offset, eachscanlinebyte);
						for(int iline = 0; iline < eachscanlinebyte; iline++){
							buffer[count * 4 + iline] = PNG_DATA::_static_buffer[offset + iline];
						}
						offset += eachscanlinebyte;
						count += _data->_w;
					}
				}
			}
		}break;
	default:{
		int byteperfixel = _data->_bytepersample * _data->_numofsample;
			int eachscanlinebyte = (_data->_w *byteperfixel) +1;
			int offset = 0; int count = 0;
			for(int i = 0; i<_data->_h; i++){
				offset++;
				for(int j = 0; j < eachscanlinebyte -1; j++){
					buffer[count] = PNG_DATA::_static_buffer[offset];
					offset++;
					count++;
				}
			}
		}break;
	}
	return buffer;
}
int Png::getW(){
	return _data->_w;
}
int Png::getH(){
	return _data->_h;
}
int Png::getBitDepth(){
	return _data->_bitdepth;
}
int Png::getColorType(){
	return _data->_colortype;
}
int Png::getCompressType(){
	return _data->_compresstype;
}
int Png::getFilterType(){
	return _data->_filtertype;
}
int Png::getInterlaceType(){
	return _data->_interlacetype;
}
int Png::getBytePerSample(){
	return _data->_bytepersample;
}
int Png::getSamplePerPixel(){
	return _data->_numofsample;
}
int Png::getBytePerPixel(){
	return (_data->_bytepersample * _data->_numofsample);
}
int Png::getBackgroundColor(){
	return _data->_bgcolor;
}

