#if !defined(_H_WAVEFORMAT)
#define _H_WAVEFORMAT
#include "../../DataType/CType.h"
struct Riff
{
	char _chunkId[5]; // "RIFF" (assuming char is 8 bits)
	unsigned long _chunkSize; // (assuming unsigned long is 32 bits)
	char _format[5]; // "WAVE"
};

struct Format
{
	char _chunkId[5]; // "fmt "
	unsigned long _chunkSize;
	short _format; // assuming short is 16 bits
	short _numChannels;
	unsigned long _sampleRate;
	unsigned long _byteRate;
	short _align;
	short _bitsPerSample;
};

struct Data
{
	char _chunkId[5]; // "data"
	unsigned long _chunkSize; // length of data
	char* _data;
};

class WaveFormat{
public:
	WaveFormat();
	~WaveFormat();
	int readWaveHeader(FILE* fp, int offset);
	void printInfo();
	Riff _riffHeader;
	Format _formatHeader;
	Data _dataHeader;
};
#endif