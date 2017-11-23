#if !defined(_H_SOUND)
#define _H_SOUND
#include "../../DataType/CType.h"
#include "../../DataInputStream/DataInputStream.h"
#include "WaveFormat.h"
class SoundService;
class Sound{
public:
	Sound(ResourceItem resitem);
	~Sound();
	bool load();
	void unload();
	const char* getPath(){return _res_item.Path;};
private:
	friend class SoundOpenSLES;
private:
	ResourceItem _res_item;
	short* _buffer;
	unsigned long _buffer_length;
	bool _loaded;
	WaveFormat _info;
};
#endif