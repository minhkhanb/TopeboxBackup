#if !defined(_H_MUSIC)
#define _H_MUSIC
#include <pthread.h>
#include "../../DataType/CType.h"
class MusicBuffer: public Buffer{
public:
	MusicBuffer(int buff_size):Buffer(buff_size), AvailableLength(0){};
	int AvailableLength;
};
class Music{
private:
	int _loop_num;
	//control loop: if < 0 is loop forever
	
	MusicBuffer _buffer;
	MusicBuffer _buffer_double;
	//to hold data loaded
	
	MusicBuffer* _curr_buffer;
	//current buffer to load data
	
	int _curr_offset;
	//current position in file that readed to.
	
	bool _is_attach_music;
	//have music file attached to load?
	
	ResourceItem _res_item;
	//info about attached music file
	
	pthread_t  _curr_thread;
	//loadding buffer thread handler
	
	static void* LoadBuffer(void* context);
	//run function to loadding buffer thread
	
	bool _is_loadding;
	//true when loadding data to buffer;
	//false loaded data;
	
	UINT32 _revision;
public:
	void LoadMusic(ResourceItem resitem);
	Music(int bufflen);
	~Music();
	UINT32 GetRevision();
	const MusicBuffer* GetNextBuffer();
};
#endif