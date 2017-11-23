#include "pch.h"
#include "WaveFormat.h"

WaveFormat::WaveFormat(){
	_riffHeader._chunkId[0] = 0; _riffHeader._chunkId[1] = 0; _riffHeader._chunkId[2] = 0; _riffHeader._chunkId[3] = 0; _riffHeader._chunkId[4] = '\0';
	_riffHeader._chunkSize = 0;
	_riffHeader._format[0] = 0;	_riffHeader._format[1] = 0;	_riffHeader._format[2] = 0;	_riffHeader._format[3] = 0; _riffHeader._format[4] = '\0';

	_formatHeader._chunkId[0] = 0; _formatHeader._chunkId[1] = 0; _formatHeader._chunkId[2] = 0; _formatHeader._chunkId[3] = 0; _formatHeader._chunkId[4] = '\0';
	_formatHeader._chunkSize = 0;
	_formatHeader._format = 0;
	_formatHeader._numChannels = 0;
	_formatHeader._sampleRate = 0;
	_formatHeader._byteRate = 0;
	_formatHeader._align = 0;
	_formatHeader._bitsPerSample = 0;

	_dataHeader._chunkId[0] = 0; _dataHeader._chunkId[1] = 0; _dataHeader._chunkId[2] = 0; _dataHeader._chunkId[3] = 0; _dataHeader._chunkId[4] = '\0';
	_dataHeader._chunkSize = 0;
}
WaveFormat::~WaveFormat(){
	delete[] _dataHeader._data;
	_dataHeader._data = NULL; _dataHeader._chunkSize = 0;
}
int WaveFormat::readWaveHeader(FILE* fp, int offset){
	int totalByteRead = 0;
	fseek(fp, offset, SEEK_SET);
	// First read the RIFF header
	totalByteRead += fread(&_riffHeader._chunkId, sizeof(char), 4, fp);
	totalByteRead += fread(&_riffHeader._chunkSize, sizeof(unsigned long), 1, fp);
	totalByteRead += fread(&_riffHeader._format, sizeof(char), 4, fp);

	// Now read the FORMAT header
	totalByteRead += fread(&_formatHeader._chunkId, sizeof(char), 4, fp);
	totalByteRead += fread(&_formatHeader._chunkSize, sizeof(unsigned long), 1, fp);
	totalByteRead += fread(&_formatHeader._format, sizeof(short), 1, fp);
	totalByteRead += fread(&_formatHeader._numChannels, sizeof(short), 1, fp);
	totalByteRead += fread(&_formatHeader._sampleRate, sizeof(unsigned long), 1, fp);
	totalByteRead += fread(&_formatHeader._byteRate, sizeof(unsigned long), 1, fp);
	totalByteRead += fread(&_formatHeader._align, sizeof(short), 1, fp);
	totalByteRead += fread(&_formatHeader._bitsPerSample, sizeof(short), 1, fp);

	// WAVE files are just a special type of RIFF file, so it's possible
	// there are other chunks, like "fact" chunks. We'll skip over these
	// extra chunks until we find a "data" chunk
	char chunkId = 0;
	unsigned long chunkSize = 0;
	int readed = 0;	
	while (readed = fread(&chunkId, sizeof(char), 1, fp) &&
		(chunkId != 'd'))
	{
		totalByteRead += readed;
		//totalByteRead += fread(&chunkSize, sizeof(unsigned long), 1, fp); // Read the chunk's size
		//fseek(fp, offset + totalByteRead + chunkSize, SEEK_SET); // Skip the chunk
	}
	// We've found the DATA chunk and header
	_dataHeader._chunkId[0] = chunkId;
	totalByteRead += fread(&_dataHeader._chunkId[1], sizeof(char), 1, fp);
	totalByteRead += fread(&_dataHeader._chunkId[2], sizeof(char), 1, fp);
	totalByteRead += fread(&_dataHeader._chunkId[3], sizeof(char), 1, fp);
	totalByteRead += fread(&_dataHeader._chunkSize, sizeof(unsigned long), 1, fp);
	//printInfo();
	return totalByteRead;
	// The actual data goes in wave.dataHeader.data, so you can allocate it
	// and then read direclty into it now
}

void WaveFormat::printInfo()
{
	PDEBUG("\n_riffHeader._chunkId = %s\n", _riffHeader._chunkId);
	PDEBUG("_riffHeader._chunkSize = %lu\n", _riffHeader._chunkSize);
	PDEBUG("_riffHeader._format = %s\n",	_riffHeader._format);

	PDEBUG("_formatHeader._chunkId = %s\n", _formatHeader._chunkId);
	PDEBUG("_formatHeader._chunkSize = %lu\n", _formatHeader._chunkSize);
	PDEBUG("_formatHeader._format = %d\n", _formatHeader._format);
	PDEBUG("_formatHeader._numChannels = %d\n", _formatHeader._numChannels);
	PDEBUG("_formatHeader._sampleRate = %lu\n", _formatHeader._sampleRate);
	PDEBUG("_formatHeader._byteRate = %lu\n", _formatHeader._byteRate);
	PDEBUG("_formatHeader._align = %d\n", _formatHeader._align);
	PDEBUG("_formatHeader._bitsPerSample = %d\n", _formatHeader._bitsPerSample);

	PDEBUG("_dataHeader._chunkId = %s\n", _dataHeader._chunkId);
	PDEBUG("_dataHeader._chunkSize = %lu\n", _dataHeader._chunkSize);
}