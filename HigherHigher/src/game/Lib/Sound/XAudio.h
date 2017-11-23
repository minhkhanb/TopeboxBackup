#if !defined(_H_XAUDIO)
#define _H_XAUDIO
#include "SoundEngine.h"
#pragma comment( lib, "xaudio2.lib")
#pragma comment( lib, "mfplat.lib")
#include "XAudio\MediaEnginePlayer.h"
#include "XAudio\XAudio2SoundPlayer.h"
/*
#include <wrl.h>
#include <mfmediaengine.h>
#include <strsafe.h>
#include <mfapi.h>
#include <agile.h>
#include <xaudio2.h>
#include "mmreg.h"
#include <vector>
#include <memory>


using namespace std;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

// MediaEngineNotifyCallback - Defines the callback method to process media engine events.
struct MediaEngineNotifyCallback abstract
{
	virtual void OnMediaEngineEvent(DWORD meEvent) = 0;
};

class MediaEnginePlayer : public MediaEngineNotifyCallback
{
	ComPtr<IMFMediaEngine>           m_spMediaEngine;
	ComPtr<IMFMediaEngineEx>         m_spEngineEx;

	MFARGB                           m_bkgColor;
	int								m_loop;
public:
	MediaEnginePlayer();
	~MediaEnginePlayer();

	void ThrowIfFailed(HRESULT hr);
	// Media Info
	void GetNativeVideoSize(DWORD *cx, DWORD *cy);
	bool IsPlaying();

	// Initialize/Shutdown
	void Initialize(ComPtr<ID3D11Device> device, DXGI_FORMAT d3dFormat);
	void Shutdown();

	// Media Engine related
	void OnMediaEngineEvent(DWORD meEvent);

	// Media Engine Actions
	void Play(int loop);//-1 loop, > 0 time, 
	void Pause();
	void SetMuted(bool muted);

	// Media Source
	void SetSource(Platform::String^ sourceUri);
	void SetSource(const char* sourceUri);
	void SetBytestream(IRandomAccessStream^ streamHandle, Platform::String^ szURL);

	// Transfer Video Frame
	void TransferFrame(ComPtr<ID3D11Texture2D> texture, MFVideoNormalizedRect rect, RECT rcTarget);

private:
	bool m_isPlaying;
};



class XAudio2SoundPlayer
{
	struct ImplData;

public:
	XAudio2SoundPlayer(uint32 sampleRate);
	~XAudio2SoundPlayer();

	size_t AddSound(_In_ WAVEFORMATEX* format, _In_ Platform::Array<byte>^ data);
	bool   PlaySound(size_t index);
	bool   StopSound(size_t index);
	bool   RemoveSound(size_t index);
	bool   IsSoundPlaying(size_t index) const;
	size_t GetSoundCount() const;

	void Suspend();
	void Resume();

private:
	void ThrowIfFailed(HRESULT hr);
	interface IXAudio2*                     m_audioEngine;
	interface IXAudio2MasteringVoice*       m_masteringVoice;
	std::vector<std::shared_ptr<ImplData>>  m_soundList;
};

class SoundFileReader
{
public:
	SoundFileReader(_In_ Platform::String^ soundFileName);

	Platform::Array<byte>^ GetSoundData() const;
	WAVEFORMATEX* GetSoundFormat() const;

private:
	Platform::Array<byte>^    m_soundData;
	Platform::Array<byte>^    m_soundFormat;
};


// A simple reader class that provides support for random access to files.
ref class RandomAccessReader
{
internal:
	RandomAccessReader(_In_ Platform::String^ fileName);

	Platform::Array<byte>^ Read(_In_ size_t bytesToRead);

	void SeekRelative(_In_ int64 offset);
	void SeekAbsolute(_In_ int64 position);
	void SeekToStart();
	void SeekToEnd();

	uint64 GetFileSize();
	uint64 GetPosition();

private:
	Wrappers::FileHandle    m_file;
};
*/

class XAudio : public SoundEngine{
private:
	MediaEnginePlayer* _player;
	XAudio2SoundPlayer* _sound_player;
public:
	///////////////////////////////////
	// IMPLEMENT SOUND ENGINE
	///////////////////////////////////
	virtual void Init();
	virtual void Release();
	virtual void LoadMusic(ResourceItem resitem);
	virtual void PlayMusic(int num_loop);
	virtual void PauseMusic();
	virtual void StopMusic();
	virtual void ResumeMusic();
	virtual bool IsPlayingMusic();

	virtual HSOUND LoadSoundEffect(ResourceItem resitem);
	virtual void PlaySoundEffect(HSOUND handle);
	virtual void StopSoundEffect(HSOUND handle);
	virtual void StopAllSoundEffect();
	virtual void PauseAllSoundEffect();
	virtual void ResumeAllSoundEffect();
	virtual void UnloadSoundEffect(HSOUND handle);
	//end implement sound engine

	void ChangeDevice(ID3D11Device1* device);
};
#endif