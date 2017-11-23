/*duy.nguyen implement to use OpenSLES*/

#include "pch.h"
#include "SoundOpenSLES.h"
#include <string.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

extern "C" AAssetManager* getAssetManager();

void SoundOpenSLES::Init()
{
	start();
	_is_music_playing = false;
}
void SoundOpenSLES::Release()
{
	stop();
}

void SoundOpenSLES::LoadMusic(ResourceItem resitem)
{
	StopMusic();
	//PDEBUG("Begin load music: ResourcItem: %d, offset: %d, length: %d\n", resitem.FileDescription, resitem.Offset, resitem.Length);
	/*AAssetManager* asset_mng = getAssetManager();
	if(asset_mng == NULL)return;
	
    AAsset* asset = AAssetManager_open(asset_mng, resitem.FileName, AASSET_MODE_UNKNOWN);
	if(asset == NULL)return;
	
    // open asset as file descriptor
    off_t start, length;
    int fd = AAsset_openFileDescriptor(asset, &start, &length);
	AAsset_close(asset);
	
	if(fd == 0)return;*/
	
	if (resitem.FileDescription == 0) return;
    

	//off_t start = 0, length = resitem.Length;
	SLresult lRes;
	// Set-up sound audio source.
	//SLDataLocator_AndroidFD lDataLocatorIn = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
	SLDataLocator_AndroidFD lDataLocatorIn = {SL_DATALOCATOR_ANDROIDFD, resitem.FileDescription, resitem.Offset, resitem.Length};
	
	SLDataFormat_MIME lDataFormat;
	lDataFormat.formatType = SL_DATAFORMAT_MIME;
	lDataFormat.mimeType = NULL;
	lDataFormat.containerType = SL_CONTAINERTYPE_UNSPECIFIED;
	
	SLDataSource lDataSource;
	lDataSource.pLocator = &lDataLocatorIn;
	lDataSource.pFormat = &lDataFormat;
	
	SLDataLocator_OutputMix lDataLocatorOut;
	lDataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
	lDataLocatorOut.outputMix = mOutputMixObj;
	
	SLDataSink lDataSink;
	lDataSink.pLocator = &lDataLocatorOut;
	lDataSink.pFormat = NULL;
	
	const SLuint32 lBGMPlayerIIDCount = 2;
	const SLInterfaceID lBGMPlayerIIDs[] = { SL_IID_PLAY, SL_IID_SEEK};
	const SLboolean lBGMPlayerReqs[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
	
	if (mEngine == NULL) goto ERROR;
	
	lRes = (*mEngine)->CreateAudioPlayer(mEngine,&mBGMPlayerObj, &lDataSource, &lDataSink,lBGMPlayerIIDCount, lBGMPlayerIIDs, lBGMPlayerReqs);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	lRes = (*mBGMPlayerObj)->Realize(mBGMPlayerObj,SL_BOOLEAN_FALSE);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	lRes = (*mBGMPlayerObj)->GetInterface(mBGMPlayerObj,SL_IID_PLAY, &mBGMPlayer);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	lRes = (*mBGMPlayerObj)->GetInterface(mBGMPlayerObj,SL_IID_SEEK, &mBGMPlayerSeek);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	//lRes = (*mBGMPlayerObj)->GetInterface(mBGMPlayerObj,SL_IID_BUFFERQUEUE, &mMusicPlayerQueue);
	//if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	lRes = (*mBGMPlayerSeek)->SetLoop(mBGMPlayerSeek,SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	//lRes = (*mMusicPlayerQueue)->RegisterCallback(mMusicPlayerQueue, bqPlayerCallback, this);
    //if(lRes != SL_RESULT_SUCCESS)goto ERROR;
	
	return;
ERROR:
	PDEBUG("error when init bgmusic player\n");
	return;
}
void SoundOpenSLES::PlayMusic(int num_loop)
{
	if(mBGMPlayer != NULL){
		SLresult lRes;
		lRes = (*mBGMPlayer)->SetPlayState(mBGMPlayer,SL_PLAYSTATE_PLAYING);
		_is_music_playing = true;
		if (lRes != SL_RESULT_SUCCESS){
			PDEBUG("error when play music\n");
		}
	}
}
void SoundOpenSLES::PauseMusic()
{
	if(_is_music_playing){
		SLresult lRes;
		lRes = (*mBGMPlayer)->SetPlayState(mBGMPlayer,SL_PLAYSTATE_PAUSED);
		_is_music_playing = false;
		if (lRes != SL_RESULT_SUCCESS){
			PDEBUG("error when play music\n");
		}
	}
}
void SoundOpenSLES::StopMusic()
{
	_is_music_playing = false;
	if (mBGMPlayer != NULL) {
		SLuint32 lBGMPlayerState;
		(*mBGMPlayerObj)->GetState(mBGMPlayerObj,&lBGMPlayerState);
		if (lBGMPlayerState == SL_OBJECT_STATE_REALIZED) {
			(*mBGMPlayer)->SetPlayState(mBGMPlayer,SL_PLAYSTATE_PAUSED);
			(*mBGMPlayerObj)->Destroy(mBGMPlayerObj);
			mBGMPlayerObj = NULL;
			mBGMPlayer = NULL;
			//mBGMPlayerSeek = NULL;
		}
	}
}
bool SoundOpenSLES::IsPlayingMusic()
{
	return _is_music_playing;
}
	

HSOUND SoundOpenSLES::LoadSoundEffect(ResourceItem resitem)
{
	return registerSound(resitem);
}
void SoundOpenSLES::PlaySoundEffect(HSOUND handle)
{
	if(handle > -1){
		if (mSounds[handle]->_info._formatHeader._numChannels == 2 && _is_init_stereo_sound_player_success)
		{
			//PDEBUG("\n PLAY STEREO 1 handle: %d", handle);
			playSound(mSounds[handle]);
		}
		else
		{
			//PDEBUG("PLAY MONO");
			if (_is_init_mono_sound_player_success)
			{
				playSoundMono(mSounds[handle]);
			}
			else if (_is_init_stereo_sound_player_success)
			{
				//PDEBUG("\n PLAY STEREO 2 handle: %d", handle);
				playSound(mSounds[handle]);
			}
		}
	}
}

void SoundOpenSLES::ClearAllSFX()
{
	PDEBUG("\n<<<<<<<<<<<<<<<<<<<< ClearAllSFX>>>>>>>>>>>>>>>>>>>>>>>>>>");
	for (int i = 0; i < stereo_player_count; i++)
	{
		if (mPlayerObj[i] != NULL) {
			(*mPlayerObj[i])->Destroy(mPlayerObj[i]);
			mPlayerObj[i] = NULL; mPlayer[i] = NULL; mPlayerQueue[i] = NULL;
		}
	}
	startSoundPlayer();
	for (int i = 0; i < mono_player_count; i++)
	{
		if (mPlayerObj_mono[i] != NULL) {
			(*mPlayerObj_mono[i])->Destroy(mPlayerObj_mono[i]);
			mPlayerObj_mono[i] = NULL; mPlayer_mono[i] = NULL; mPlayerQueue_mono[i] = NULL;
		}
	}
	startSoundPlayerMono();
}

void SoundOpenSLES::StopSoundEffect(HSOUND handle)
{
}
void SoundOpenSLES::StopAllSoundEffect()
{
    for (int i = 0; i < stereo_player_count; i ++)
    {
        if (mPlayer[i] != NULL) {
            SLuint32 lPlayerState;
            (*mPlayerObj[i])->GetState(mPlayerObj[i],&lPlayerState);
            if (lPlayerState == SL_OBJECT_STATE_REALIZED) {
                (*mPlayer[i])->SetPlayState(mPlayer[i],SL_PLAYSTATE_PAUSED);
            }
        }
    }

    for (int i = 0; i < mono_player_count; i ++)
    {
        if (mPlayer_mono[i] != NULL) {
            SLuint32 lPlayerState;
            (*mPlayerObj_mono[i])->GetState(mPlayerObj_mono[i], &lPlayerState);
            if (lPlayerState == SL_OBJECT_STATE_REALIZED) {
                (*mPlayer_mono[i])->SetPlayState(mPlayer_mono[i], SL_PLAYSTATE_PAUSED);
            }
        }
    }
}
void SoundOpenSLES::UnloadSoundEffect(HSOUND handle)
{
}
SoundOpenSLES::~SoundOpenSLES() {
	for (int i = 0; i < mSoundCount; ++i) {
		delete mSounds[i];
	}
	mSoundCount = 0;
}
SoundOpenSLES* SoundOpenSLES::_this = NULL;
SoundOpenSLES::SoundOpenSLES():
	mEngineObj(NULL), mEngine(NULL),mOutputMixObj(NULL),
	mBGMPlayerObj(NULL), mBGMPlayer(NULL), mBGMPlayerSeek(NULL), //mMusicPlayerQueue(NULL),
	mSounds(), mSoundCount(0),
	_is_init_stereo_sound_player_success(false), _is_init_mono_sound_player_success(false)
{
    for (int i = 0; i < MAX_SOUND_PLAYER; i ++)
    {
        mPlayerObj[i] = NULL;
        mPlayer[i] = NULL;
        mPlayerQueue[i] = NULL;
        mPlayerObj_mono[i] = NULL;
        mPlayer_mono[i] = NULL;
        mPlayerQueue_mono[i] = NULL;
    }
}
void SoundOpenSLES::start()
{
	PDEBUG("sound open sles begin\n");
	SLresult lRes;
	
	const SLuint32 lEngineMixIIDCount = 1;
	const SLInterfaceID lEngineMixIIDs[]={SL_IID_ENGINE};
	const SLboolean lEngineMixReqs[]={SL_BOOLEAN_TRUE};
	
	const SLuint32 lOutputMixIIDCount=0;	
	const SLInterfaceID lOutputMixIIDs[0]={};
	const SLboolean lOutputMixReqs[0]={};

	lRes = slCreateEngine(&mEngineObj, 0, NULL, lEngineMixIIDCount, lEngineMixIIDs, lEngineMixReqs);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	lRes = (*mEngineObj)->Realize(mEngineObj,SL_BOOLEAN_FALSE);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	lRes = (*mEngineObj)->GetInterface(mEngineObj, SL_IID_ENGINE, &mEngine);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	lRes = (*mEngine)->CreateOutputMix(mEngine, &mOutputMixObj,lOutputMixIIDCount,lOutputMixIIDs, lOutputMixReqs);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	lRes = (*mOutputMixObj)->Realize(mOutputMixObj,SL_BOOLEAN_FALSE);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	if (!startSoundPlayer())
	{
		_is_init_stereo_sound_player_success = false;
		for (int i = 0; i < stereo_player_count; i ++)
        {
            if (mPlayerObj[i] != NULL) {
                (*mPlayerObj[i])->Destroy(mPlayerObj[i]);
                mPlayerObj[i] = NULL; mPlayer[i] = NULL; mPlayerQueue[i] = NULL;
            }
        }
	}
	else 
	{
		_is_init_stereo_sound_player_success = true;
	}
	
	if (!startSoundPlayerMono())
	{
		_is_init_mono_sound_player_success = false;
		for (int i = 0; i < mono_player_count; i ++)
        {
            if (mPlayerObj_mono[i] != NULL) {
                (*mPlayerObj_mono[i])->Destroy(mPlayerObj_mono[i]);
                mPlayerObj_mono[i] = NULL; mPlayer_mono[i] = NULL; mPlayerQueue_mono[i] = NULL;
            }
        }
	}
	else 
	{
		_is_init_mono_sound_player_success = true;
	}
	
	
	for (int32_t i = 0; i < mSoundCount; ++i) {
		if (!mSounds[i]->load()) goto ERROR;
	}
	return;
	
ERROR:
	PDEBUG("Error when init sound engine\n");
	stop();
	return;
}
void SoundOpenSLES::stop() 
{
	StopMusic();
	if (mOutputMixObj != NULL) 
	{
		(*mOutputMixObj)->Destroy(mOutputMixObj);
		mOutputMixObj = NULL;
	}
	if (mEngineObj != NULL) 
	{
		(*mEngineObj)->Destroy(mEngineObj);
		mEngineObj = NULL; mEngine = NULL;
	}
	for (int i = 0; i < stereo_player_count; i ++)
    {
        if (mPlayerObj[i] != NULL) {
            (*mPlayerObj[i])->Destroy(mPlayerObj[i]);
            mPlayerObj[i] = NULL; mPlayer[i] = NULL; mPlayerQueue[i] = NULL;
        }
    }
	_is_init_stereo_sound_player_success = false;
	for (int i = 0; i < mono_player_count; i ++)
    {
        if (mPlayerObj_mono[i] != NULL) {
            (*mPlayerObj_mono[i])->Destroy(mPlayerObj_mono[i]);
            mPlayerObj_mono[i] = NULL; mPlayer_mono[i] = NULL; mPlayerQueue_mono[i] = NULL;
        }
    }
	_is_init_stereo_sound_player_success = false;
	for (int32_t i = 0; i < mSoundCount; ++i) {
		mSounds[i]->unload();
	}
}
void SoundOpenSLES::ResumeMusic(){
	if(mBGMPlayer != NULL){
		SLresult lRes;
		lRes = (*mBGMPlayer)->SetPlayState(mBGMPlayer,SL_PLAYSTATE_PLAYING);
		_is_music_playing = true;
		if (lRes != SL_RESULT_SUCCESS){
			PDEBUG("error when play music\n");
		}
	}
}

HSOUND SoundOpenSLES::registerSound(ResourceItem resitem) {
	PDEBUG("\n ----------register sound mSoundCount: %d--------------", mSoundCount);
	for (int i = 0; i < mSoundCount; ++i) {
		if (strcmp(resitem.Path, mSounds[i]->getPath()) == 0 && resitem.Offset == mSounds[i]->_res_item.Offset) {
			//PDEBUG("find_sound:%d path:%s offset:%d path:%s offset:%d",i,resitem.Path,resitem.Offset, mSounds[i]->_res_item.Path, mSounds[i]->_res_item.Offset);
			return i;
		}
	}
	Sound* lSound = new Sound(resitem);
	//PDEBUG("\tbegin load");
	lSound->load();
	mSounds[mSoundCount++] = lSound;
	//PDEBUG("\tloaded at:%d path:%s offset:%d", mSoundCount-1, mSounds[ mSoundCount-1]->_res_item.Path, mSounds[ mSoundCount-1]->_res_item.Offset);
	//PDEBUG("end register sound");
	return mSoundCount-1;
}

bool SoundOpenSLES::startSoundPlayer() {
    stereo_player_count = 0;
    for (int i = 0; i < MAX_SOUND_PLAYER; i ++)
    {
        if (startSoundPlayer(i))
        {
            stereo_player_count ++;
        }
        else
        {
            break;
        }
    }
    PDEBUG("stereo_player_count = %d", stereo_player_count);
    return stereo_player_count > 0 ? true : false;
}

bool SoundOpenSLES::startSoundPlayer(int i) {
	SLresult lRes;
	// Set-up sound audio source.
	SLDataLocator_AndroidSimpleBufferQueue lDataLocatorIn;
	lDataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
	// At most one buffer in the queue.
	lDataLocatorIn.numBuffers = 1;
	
	SLDataFormat_PCM lDataFormat;
	lDataFormat.formatType = SL_DATAFORMAT_PCM;
	
	lDataFormat.samplesPerSec = SL_SAMPLINGRATE_44_1;
	lDataFormat.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
	lDataFormat.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
	//lDataFormat.numChannels = 1; // Mono sound.
	//lDataFormat.channelMask = SL_SPEAKER_FRONT_CENTER;
	lDataFormat.numChannels = 2;
	lDataFormat.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
	
	lDataFormat.endianness = SL_BYTEORDER_LITTLEENDIAN;
	
	SLDataSource lDataSource;
	lDataSource.pLocator = &lDataLocatorIn;
	lDataSource.pFormat = &lDataFormat;
	
	SLDataLocator_OutputMix lDataLocatorOut;
	lDataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
	lDataLocatorOut.outputMix = mOutputMixObj;
	
	SLDataSink lDataSink;
	lDataSink.pLocator = &lDataLocatorOut;
	lDataSink.pFormat = NULL;
	
	const SLuint32 lSoundPlayerIIDCount = 2;//3;
	const SLInterfaceID lSoundPlayerIIDs[] ={ SL_IID_PLAY, /*SL_IID_EFFECTSEND,*/ SL_IID_BUFFERQUEUE};
	const SLboolean lSoundPlayerReqs[] ={ SL_BOOLEAN_TRUE, /*SL_BOOLEAN_TRUE,*/ SL_BOOLEAN_TRUE};
	lRes = (*mEngine)->CreateAudioPlayer(mEngine, &mPlayerObj[i],&lDataSource, &lDataSink, lSoundPlayerIIDCount,lSoundPlayerIIDs, lSoundPlayerReqs);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	lRes = (*mPlayerObj[i])->Realize(mPlayerObj[i], SL_BOOLEAN_FALSE);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	lRes = (*mPlayerObj[i])->GetInterface(mPlayerObj[i], SL_IID_PLAY,&mPlayer[i]);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	//lRes = (*mPlayerObj[i])->GetInterface(mPlayerObj[i],SL_IID_EFFECTSEND, &mPlayerQueue[i]);
	//if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	
	lRes = (*mPlayerObj[i])->GetInterface(mPlayerObj[i],SL_IID_BUFFERQUEUE, &mPlayerQueue[i]);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	lRes = (*mPlayer[i])->SetPlayState(mPlayer[i],SL_PLAYSTATE_PLAYING);

	if (lRes != SL_RESULT_SUCCESS) goto ERROR;
	
	return true;
ERROR:
	PDEBUG("Error while starting SoundPlayer");
	return false;
}

bool SoundOpenSLES::startSoundPlayerMono() {
    mono_player_count = 0;
    for (int i = 0; i < MAX_SOUND_PLAYER; i ++)
    {
        if (startSoundPlayerMono(i))
        {
            mono_player_count ++;
        }
        else
        {
            break;
        }
    }
    PDEBUG("mono_player_count = %d", mono_player_count);
    return mono_player_count > 0 ? true : false;
}

bool SoundOpenSLES::startSoundPlayerMono(int i)
{
	SLresult lRes;
	// Set-up sound audio source.
	SLDataLocator_AndroidSimpleBufferQueue lDataLocatorIn;
	lDataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
	// At most one buffer in the queue.
	lDataLocatorIn.numBuffers = 1;

	SLDataFormat_PCM lDataFormat;
	lDataFormat.formatType = SL_DATAFORMAT_PCM;

	lDataFormat.samplesPerSec = SL_SAMPLINGRATE_44_1;
	lDataFormat.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
	lDataFormat.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
	lDataFormat.numChannels = 1; // Mono sound.
	lDataFormat.channelMask = SL_SPEAKER_FRONT_CENTER;

	lDataFormat.endianness = SL_BYTEORDER_LITTLEENDIAN;

	SLDataSource lDataSource;
	lDataSource.pLocator = &lDataLocatorIn;
	lDataSource.pFormat = &lDataFormat;

	SLDataLocator_OutputMix lDataLocatorOut;
	lDataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
	lDataLocatorOut.outputMix = mOutputMixObj;

	SLDataSink lDataSink;
	lDataSink.pLocator = &lDataLocatorOut;
	lDataSink.pFormat = NULL;

	const SLuint32 lSoundPlayerIIDCount = 2;//3;
	const SLInterfaceID lSoundPlayerIIDs[] = { SL_IID_PLAY, /*SL_IID_EFFECTSEND,*/ SL_IID_BUFFERQUEUE };
	const SLboolean lSoundPlayerReqs[] = { SL_BOOLEAN_TRUE, /*SL_BOOLEAN_TRUE,*/ SL_BOOLEAN_TRUE };
	lRes = (*mEngine)->CreateAudioPlayer(mEngine, &mPlayerObj_mono[i], &lDataSource, &lDataSink, lSoundPlayerIIDCount, lSoundPlayerIIDs, lSoundPlayerReqs);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;

	lRes = (*mPlayerObj_mono[i])->Realize(mPlayerObj_mono[i], SL_BOOLEAN_FALSE);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;

	lRes = (*mPlayerObj_mono[i])->GetInterface(mPlayerObj_mono[i], SL_IID_PLAY, &mPlayer_mono[i]);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;

	//lRes = (*mPlayerObj_mono[i])->GetInterface(mPlayerObj_mono[i], SL_IID_EFFECTSEND, &mPlayerQueue_mono[i]);
	//if (lRes != SL_RESULT_SUCCESS) goto ERROR;


	lRes = (*mPlayerObj_mono[i])->GetInterface(mPlayerObj_mono[i], SL_IID_BUFFERQUEUE, &mPlayerQueue_mono[i]);
	if (lRes != SL_RESULT_SUCCESS) goto ERROR;

	lRes = (*mPlayer_mono[i])->SetPlayState(mPlayer_mono[i], SL_PLAYSTATE_PLAYING);

	if (lRes != SL_RESULT_SUCCESS) goto ERROR;

	return true;
ERROR:
	PDEBUG("Error while starting SoundPlayerMono");
	return false;
}

void SoundOpenSLES::playSound(Sound* pSound) {
	SLresult lRes;
	SLuint32 lPlayerState;
	SLBufferQueueState state; 
	//PDEBUG("\n SoundOpenSLES::playSound -- stereo_player_count: %d", stereo_player_count);
	for (int i = 0; i < stereo_player_count; i ++)
	{
        if(mPlayerObj[i] != NULL){
            (*mPlayerObj[i])->GetState(mPlayerObj[i], &lPlayerState);
            if (lPlayerState == SL_OBJECT_STATE_REALIZED) {
                //(*mPlayer)->SetPlayState(mPlayer, SL_PLAYSTATE_STOPPED);
                // Removes any sound from the queue.
                //lRes = (*mPlayerQueue)->Clear(mPlayerQueue);
                //if (lRes != SL_RESULT_SUCCESS){
                    //PDEBUG("Error trying to play sound 1: %d",lRes);
                    //return;
                //};
				lRes = (*mPlayerQueue[i])->GetState(mPlayerQueue[i], &state);
				if (lRes != SL_RESULT_SUCCESS || state.count > 0)
				{
					//PDEBUG("\n Error trying to play sound 1: %d -- state.count: %d", lRes, state.count);
					continue;
				}
                // Plays the new sound.
                lRes = (*mPlayerQueue[i])->Enqueue(mPlayerQueue[i], pSound->_buffer, pSound->_buffer_length);
                if (lRes != SL_RESULT_SUCCESS){
                    //PDEBUG("\n Error trying to play sound 2: %d",lRes);
                    //return;
                    continue;
                };
                lRes = (*mPlayer[i])->SetPlayState(mPlayer[i],SL_PLAYSTATE_PLAYING);
                if (lRes != SL_RESULT_SUCCESS){
                    //PDEBUG("\n Error trying to play sound 3: %d",lRes);
                    //return;
                    continue;
                };
				//PDEBUG("\n\n ---- play successful: %d ----\n\n", lRes);
                break;
            }else{
                PDEBUG("Error trying to play sound 4: %d",lPlayerState);
                continue;
            }
        }
    }
    if (lRes != SL_RESULT_SUCCESS)
    {
         (*mPlayer[0])->SetPlayState(mPlayer[0], SL_PLAYSTATE_STOPPED);
         lRes = (*mPlayerQueue[0])->Clear(mPlayerQueue[0]);
         lRes = (*mPlayerQueue[0])->Enqueue(mPlayerQueue[0], pSound->_buffer, pSound->_buffer_length);
         if (lRes != SL_RESULT_SUCCESS){
            PDEBUG("Error trying to play sound 2: %d",lRes);
            return;
         };
         lRes = (*mPlayer[0])->SetPlayState(mPlayer[0],SL_PLAYSTATE_PLAYING);
         if (lRes != SL_RESULT_SUCCESS){
             PDEBUG("Error trying to play sound 3: %d",lRes);
             return;
         };
    }
}

void SoundOpenSLES::playSoundMono(Sound* pSound) {
	SLresult lRes;
	SLuint32 lPlayerState;
	SLBufferQueueState state; 
	for (int i = 0; i < mono_player_count; i ++)
	{
        if (mPlayerObj_mono[i] != NULL) {
            (*mPlayerObj_mono[i])->GetState(mPlayerObj_mono[i], &lPlayerState);
            if (lPlayerState == SL_OBJECT_STATE_REALIZED) {
                //(*mPlayer_mono)->SetPlayState(mPlayer_mono, SL_PLAYSTATE_STOPPED);
                // Removes any sound from the queue.
                //lRes = (*mPlayerQueue_mono)->Clear(mPlayerQueue_mono);
                //if (lRes != SL_RESULT_SUCCESS) {
                    //PDEBUG("Error trying to play sound 1: %d", lRes);
                    //return;
                //};
				lRes = (*mPlayerQueue_mono[i])->GetState(mPlayerQueue_mono[i], &state);
				if (lRes != SL_RESULT_SUCCESS || state.count > 0)
				{
					continue;
				}
                // Plays the new sound.
                lRes = (*mPlayerQueue_mono[i])->Enqueue(mPlayerQueue_mono[i], pSound->_buffer, pSound->_buffer_length);
                if (lRes != SL_RESULT_SUCCESS) {
                    //PDEBUG("Error trying to play Mono sound 2: %d", lRes);
                    //return;
                    continue;
                };
                lRes = (*mPlayer_mono[i])->SetPlayState(mPlayer_mono[i], SL_PLAYSTATE_PLAYING);
                if (lRes != SL_RESULT_SUCCESS) {
                    //PDEBUG("Error trying to play Mono sound 3: %d", lRes);
                    //return;
                    continue;
                };
                break;
            }
            else {
                //PDEBUG("Error trying to play Mono sound 4: %d", lPlayerState);
                continue;
            }
        }
    }
    if (lRes != SL_RESULT_SUCCESS)
    {
        (*mPlayer_mono[0])->SetPlayState(mPlayer_mono[0], SL_PLAYSTATE_STOPPED);
        lRes = (*mPlayerQueue_mono[0])->Clear(mPlayerQueue_mono[0]);
        lRes = (*mPlayerQueue_mono[0])->Enqueue(mPlayerQueue_mono[0], pSound->_buffer, pSound->_buffer_length);
        if (lRes != SL_RESULT_SUCCESS){
           PDEBUG("Error trying to play sound 2: %d",lRes);
           return;
        };
        lRes = (*mPlayer_mono[0])->SetPlayState(mPlayer_mono[0],SL_PLAYSTATE_PLAYING);
        if (lRes != SL_RESULT_SUCCESS){
            PDEBUG("Error trying to play sound 3: %d",lRes);
            return;
        };
    }
}

void SoundOpenSLES::PauseAllSoundEffect()
{
	
}

void SoundOpenSLES::ResumeAllSoundEffect()
{
	
}

