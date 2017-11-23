#include "pch.h"
#include "XAudio.h"
#include "XAudio\RandomAccessReader.h"
#include "XAudio\SoundFileReader.h"
/*
#include <xaudio2.h>
using namespace Windows::ApplicationModel;
namespace
{
	//
	// Handler for XAudio source voice callbacks to maintain playing state
	//
	class SoundCallbackHander : public IXAudio2VoiceCallback
	{
	public:
		SoundCallbackHander(bool* isPlayingHolder) :
			m_isPlayingHolder(isPlayingHolder)
		{
		}

		~SoundCallbackHander()
		{
			m_isPlayingHolder = nullptr;
		}

		//
		// Voice callbacks from IXAudio2VoiceCallback
		//
		STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 bytesRequired);
		STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS);
		STDMETHOD_(void, OnStreamEnd) (THIS);
		STDMETHOD_(void, OnBufferStart) (THIS_ void* bufferContext);
		STDMETHOD_(void, OnBufferEnd) (THIS_ void* bufferContext);
		STDMETHOD_(void, OnLoopEnd) (THIS_ void* bufferContext);
		STDMETHOD_(void, OnVoiceError) (THIS_ void* bufferContext, HRESULT error);

	private:
		bool* m_isPlayingHolder;
	};

	//
	// Callback handlers, only implement the buffer events for maintaining play state
	//
	void SoundCallbackHander::OnVoiceProcessingPassStart(UINT32 )
	{
	}
	void SoundCallbackHander::OnVoiceProcessingPassEnd()
	{
	}
	void SoundCallbackHander::OnStreamEnd()
	{
	}
	void SoundCallbackHander::OnBufferStart(void*)
	{
		*m_isPlayingHolder = true;
	}
	void SoundCallbackHander::OnBufferEnd(void* )
	{
		*m_isPlayingHolder = false;
	}
	void SoundCallbackHander::OnLoopEnd(void*)
	{
	}
	void SoundCallbackHander::OnVoiceError(void*, HRESULT)
	{
	}
}

namespace
{
	//
	// 4 Character Tags in the RIFF File of Interest (read backwards)
	//
	const uint32 FOURCC_RIFF_TAG = 'FFIR';
	const uint32 FOURCC_FORMAT_TAG = ' tmf';
	const uint32 FOURCC_DATA_TAG = 'atad';
	const uint32 FOURCC_WAVE_FILE_TAG = 'EVAW';

	//
	// The header of every 'chunk' of data in the RIFF file
	//
	struct ChunkHeader
	{
		uint32 tag;
		uint32 size;
	};

	//
	// Helper function to find a riff-chunk with-in the sent bounds.
	// It is assumed the bounds begin at the start of a chunk
	//
	uint64 FindChunk(
		RandomAccessReader^ file,
		uint32 tag,
		uint64 startLoc,
		uint64 endLoc
		)
	{
		// Move to start of search
		file->SeekAbsolute(startLoc);

		uint64 newLoc = startLoc;
		while (endLoc > (newLoc + sizeof(ChunkHeader)))
		{
			Platform::Array<byte>^ headerBytes = file->Read(sizeof(ChunkHeader));
			ChunkHeader* header = reinterpret_cast<ChunkHeader*>(headerBytes->Data);
			if (header->tag == tag)
			{
				// Found the requested tag
				return newLoc;
			}
			file->SeekRelative(static_cast<int64>(header->size));
			newLoc += header->size + sizeof(*header);
		}

		// Chunk with sent tag was not found
		throw ref new Platform::FailureException();
	}

	//
	// Read the riff chunk header at the send location
	//
	void ReadHeader(
		RandomAccessReader^ file,
		uint64 loc,
		ChunkHeader& header
		)
	{
		// Move to read location
		file->SeekAbsolute(loc);
		Platform::Array<byte>^ headerBytes = file->Read(sizeof(ChunkHeader));
		header = *reinterpret_cast<ChunkHeader*>(headerBytes->Data);
	}
}


///////////////////////////////////////////////////////////////////////////////
// MediaEngineNotify
//
// Implements the callback for Media Engine event notification.
///////////////////////////////////////////////////////////////////////////////

class MediaEngineNotify : public IMFMediaEngineNotify
{
	long m_cRef;
	MediaEngineNotifyCallback* m_pCB;

public:
	MediaEngineNotify() :
		m_cRef(1),
		m_pCB(nullptr)
	{
	}

	STDMETHODIMP QueryInterface(REFIID riid, void** ppv)
	{
		if (__uuidof(IMFMediaEngineNotify) == riid)
		{
			*ppv = static_cast<IMFMediaEngineNotify*>(this);
		}
		else
		{
			*ppv = nullptr;
			return E_NOINTERFACE;
		}
		AddRef();
		return S_OK;
	}

	STDMETHODIMP_(ULONG) AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}

	STDMETHODIMP_(ULONG) Release()
	{
		LONG cRef = InterlockedDecrement(&m_cRef);
		if (cRef == 0)
		{
			delete this;
		}
		return cRef;
	}

	void MediaEngineNotifyCallback(MediaEngineNotifyCallback* pCB)
	{
		m_pCB = pCB;
	}

	// EventNotify is called when the Media Engine sends an event.
	STDMETHODIMP EventNotify(DWORD meEvent, DWORD_PTR param1, DWORD param2)
	{
		if (meEvent == MF_MEDIA_ENGINE_EVENT_NOTIFYSTABLESTATE)
		{
			SetEvent(reinterpret_cast<HANDLE>(param1));
		}
		else
		{
			m_pCB->OnMediaEngineEvent(meEvent);
		}
		return S_OK;
	}
};




///////////////////////////////////////////////////////////////////////////////
// MEDIAENGINE PLAYER
///////////////////////////////////////////////////////////////////////////////

MediaEnginePlayer::MediaEnginePlayer() :
m_spMediaEngine(nullptr),
m_spEngineEx(nullptr),
m_isPlaying(false)
{
	memset(&m_bkgColor, 0, sizeof(MFARGB));
}




//-----------------------------------------------------------------------------
// Initialize
//
// Create DXGI Adapter required for Frame-Server mode use of
// IMFMediaEngine, and then create MediaEngine
//
//-----------------------------------------------------------------------------
void MediaEnginePlayer::ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		// Set a breakpoint on this line to catch Win32 API errors.
		throw Platform::Exception::CreateException(hr);
	}
}
void MediaEnginePlayer::Initialize(
	ComPtr<ID3D11Device> device,
	DXGI_FORMAT d3dFormat)
{
	ComPtr<IMFMediaEngineClassFactory> spFactory;
	ComPtr<IMFAttributes> spAttributes;
	ComPtr<MediaEngineNotify> spNotify;

	ThrowIfFailed(MFStartup(MF_VERSION));

	UINT resetToken;
	ComPtr<IMFDXGIDeviceManager> DXGIManager;
	ThrowIfFailed(MFCreateDXGIDeviceManager(&resetToken, &DXGIManager));
	ThrowIfFailed(DXGIManager->ResetDevice(device.Get(), resetToken));

	// Create our event callback object.
	spNotify = new MediaEngineNotify();
	if (spNotify == nullptr)
	{
		ThrowIfFailed(E_OUTOFMEMORY);
	}

	spNotify->MediaEngineNotifyCallback(this);

	// Set configuration attribiutes.
	ThrowIfFailed(MFCreateAttributes(&spAttributes, 1));
	ThrowIfFailed(spAttributes->SetUnknown(MF_MEDIA_ENGINE_DXGI_MANAGER, (IUnknown*)DXGIManager.Get()));
	ThrowIfFailed(spAttributes->SetUnknown(MF_MEDIA_ENGINE_CALLBACK, (IUnknown*)spNotify.Get()));
	ThrowIfFailed(spAttributes->SetUINT32(MF_MEDIA_ENGINE_VIDEO_OUTPUT_FORMAT, d3dFormat));

	// Create MediaEngine.
	ThrowIfFailed(CoCreateInstance(CLSID_MFMediaEngineClassFactory, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&spFactory)));
	ThrowIfFailed(spFactory->CreateInstance(0, spAttributes.Get(), &m_spMediaEngine));

	// Create MediaEngineEx
	ThrowIfFailed(m_spMediaEngine.Get()->QueryInterface(__uuidof(IMFMediaEngine), (void**)&m_spEngineEx));
	return;
}




//-----------------------------------------------------------------------------
// SetSource
//
// Set media source URI
//-----------------------------------------------------------------------------

void MediaEnginePlayer::SetSource(Platform::String^ szURL)
{
	BSTR bstrURL = nullptr;

	if (nullptr != bstrURL)
	{
		::CoTaskMemFree(bstrURL);
		bstrURL = nullptr;
	}

	size_t cchAllocationSize = 1 + ::wcslen(szURL->Data());
	bstrURL = (LPWSTR)::CoTaskMemAlloc(sizeof(WCHAR)*(cchAllocationSize));

	if (bstrURL == 0)
	{
		throw Platform::Exception::CreateException(E_OUTOFMEMORY);
	}

	StringCchCopyW(bstrURL, cchAllocationSize, szURL->Data());
	if (m_spMediaEngine != nullptr){
		m_spMediaEngine->SetSource(bstrURL);
	}
	return;
}

void MediaEnginePlayer::SetSource(const char* szURL)
{
	//return;
	BSTR bstrURL = nullptr;
	wchar_t  buf[255];
	swprintf(buf, 255, L"%hs", szURL);
	OutputDebugString(buf);
	if(nullptr != bstrURL)
	{
	::CoTaskMemFree(bstrURL);
	bstrURL = nullptr;
	}

	size_t cchAllocationSize = 1 + ::wcslen(buf);
	bstrURL = (LPWSTR)::CoTaskMemAlloc(sizeof(WCHAR)*(cchAllocationSize));

	if (bstrURL == 0)
	{
		throw Platform::Exception::CreateException(E_OUTOFMEMORY);
	}

	StringCchCopyW(bstrURL, cchAllocationSize,buf);
	if (m_spMediaEngine != nullptr){
		m_spMediaEngine->SetSource(bstrURL);
	}
	return;
}

//-----------------------------------------------------------------------------
// SetByteStream
//
// Provice a bytestream as media source
//-----------------------------------------------------------------------------

void MediaEnginePlayer::SetBytestream(IRandomAccessStream^ streamHandle, Platform::String^ szURL)
{
	ComPtr<IMFByteStream> spMFByteStream = nullptr;
	BSTR bstrURL = nullptr;

	if (nullptr != bstrURL)
	{
		::CoTaskMemFree(bstrURL);
		bstrURL = nullptr;
	}

	size_t cchAllocationSize = 1 + ::wcslen(szURL->Data());
	bstrURL = (LPWSTR)::CoTaskMemAlloc(sizeof(WCHAR)*(cchAllocationSize));

	if (bstrURL == 0)
	{
		throw Platform::Exception::CreateException(E_OUTOFMEMORY);
	}

	StringCchCopyW(bstrURL, cchAllocationSize, szURL->Data());

	HRESULT rs = MFCreateMFByteStreamOnStreamEx((IUnknown*)streamHandle, &spMFByteStream);
	if (FAILED(rs)){
		throw Platform::Exception::CreateException(rs);
	}

	rs =  m_spEngineEx->SetSourceFromByteStream(spMFByteStream.Get(), bstrURL);
	if (FAILED(rs)){
		throw Platform::Exception::CreateException(rs);
	}

	return;
}


//-----------------------------------------------------------------------------
// OnMediaEngineEvent
//
// All Media Engine Notificacation are channeled here
//-----------------------------------------------------------------------------

void MediaEnginePlayer::OnMediaEngineEvent(DWORD meEvent)
{
	switch (meEvent)
	{
	case MF_MEDIA_ENGINE_EVENT_LOADEDDATA:
		//Pause();
		break;
	case MF_MEDIA_ENGINE_EVENT_LOADEDMETADATA:
		//Pause();
		break;

	case MF_MEDIA_ENGINE_EVENT_CANPLAY:
		//Play(m_loop);             
		break;

	case MF_MEDIA_ENGINE_EVENT_PLAY:
		break;

	case MF_MEDIA_ENGINE_EVENT_PAUSE:
		break;

	case MF_MEDIA_ENGINE_EVENT_ENDED:
		if (m_loop == -1){
			Play(-1);
		}
		else if (m_loop > 0){
			Play(m_loop - 1);
		}
		break;

	case MF_MEDIA_ENGINE_EVENT_TIMEUPDATE:
		break;

	case MF_MEDIA_ENGINE_EVENT_ERROR:
		if (m_spMediaEngine)
		{
			//ComPtr<IMFMediaError> error;
			//m_spMediaEngine->GetError(&error);
			//USHORT errorCode = error->GetErrorCode();
		}
		break;
	}

	return;
}


//-----------------------------------------------------------------------------
// Play
//
// Play the media
//-----------------------------------------------------------------------------
void MediaEnginePlayer::Play(int loop)
{
	if (m_spMediaEngine)
	{
		HRESULT rs = m_spMediaEngine->Play();
		if (FAILED(rs)){
			throw Platform::Exception::CreateException(rs);
		}
		m_isPlaying = true;
		m_loop = loop;
	}
	return;
}


//-----------------------------------------------------------------------------
// Pause
//
// Pause the media
//-----------------------------------------------------------------------------
void MediaEnginePlayer::Pause()
{
	if (m_spMediaEngine)
	{
		HRESULT rs = m_spMediaEngine->Pause();
		if (FAILED(rs)){
			throw Platform::Exception::CreateException(rs);
		}
		m_isPlaying = false;
	}
	return;
}

//-----------------------------------------------------------------------------
// Mute
//
// Mute the media
//-----------------------------------------------------------------------------
void MediaEnginePlayer::SetMuted(bool muted)
{
	if (m_spMediaEngine)
	{
		HRESULT rs = m_spMediaEngine->SetMuted(muted);
		if (FAILED(rs)){
			throw Platform::Exception::CreateException(rs);
		}
	}
	return;
}

//-----------------------------------------------------------------------------
// GetNativeVideoSize
//
// Gets the size of the video frame, adjusted for aspect ratio.
//-----------------------------------------------------------------------------
void MediaEnginePlayer::GetNativeVideoSize(DWORD *cx, DWORD *cy)
{
	if (m_spMediaEngine)
	{
		m_spMediaEngine->GetNativeVideoSize(cx, cy);
	}
	return;
}

//-----------------------------------------------------------------------------
// IsPlaying
//
// Returns whether or not the video is currently playing
//-----------------------------------------------------------------------------
bool MediaEnginePlayer::IsPlaying()
{
	return m_isPlaying;
}

//-----------------------------------------------------------------------------
//
//  TransferFrame
//
//------------------------------------------------------------------------------
void MediaEnginePlayer::TransferFrame(ComPtr<ID3D11Texture2D> texture, MFVideoNormalizedRect rect, RECT rcTarget)
{
	if (m_spMediaEngine != nullptr && m_isPlaying)
	{
		LONGLONG pts;
		if (m_spMediaEngine->OnVideoStreamTick(&pts) == S_OK)
		{
			// new frame available at the media engine so get it 
			HRESULT rs = m_spMediaEngine->TransferVideoFrame(texture.Get(), &rect, &rcTarget, &m_bkgColor);
			if (FAILED(rs)){
				throw Platform::Exception::CreateException(rs);
			}
		}
	}

	return;
}

MediaEnginePlayer::~MediaEnginePlayer()
{
	Shutdown();
	MFShutdown();
}

//-----------------------------------------------------------------------------
// Shutdown
//
// Shut down the player and release all interface pointers.
//-----------------------------------------------------------------------------
void MediaEnginePlayer::Shutdown()
{
	if (m_spMediaEngine)
	{
		m_spMediaEngine->Shutdown();
	}
	return;
}



///////////////////////////////////////////////////////////////////////////////
//	RANDOM ACCESS READER
///////////////////////////////////////////////////////////////////////////////

RandomAccessReader::RandomAccessReader(
	_In_ Platform::String^ fileName
	)
{
	Platform::String^ fullPath = fileName;
	//fullPath += "\\";
	//fullPath += fileName;

	HANDLE fileHandle = ::CreateFile2(
		fullPath->Data(),
		GENERIC_READ,
		FILE_SHARE_READ,
		OPEN_EXISTING,
		nullptr
		);

	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		throw ref new Platform::FailureException();
	}

	m_file.Attach(fileHandle);
}

Platform::Array<byte>^ RandomAccessReader::Read(
	_In_ size_t bytesToRead
	)
{
	Platform::Array<byte>^ fileData = ref new Platform::Array<byte>(static_cast<UINT>(bytesToRead));

	if (!ReadFile(
		m_file.Get(),
		fileData->Data,
		fileData->Length,
		nullptr,
		nullptr
		))
	{
		throw ref new Platform::FailureException();
	}

	return fileData;
}

uint64 RandomAccessReader::GetFileSize()
{
	FILE_STANDARD_INFO fileStandardInfo = { 0 };
	BOOL result = ::GetFileInformationByHandleEx(
		m_file.Get(),
		FileStandardInfo,
		&fileStandardInfo,
		sizeof(fileStandardInfo)
		);

	if ((result == 0) || (fileStandardInfo.AllocationSize.QuadPart < 0))
	{
		throw ref new Platform::FailureException();
	}

	return fileStandardInfo.AllocationSize.QuadPart;
}

uint64 RandomAccessReader::GetPosition()
{
	LARGE_INTEGER position = { 0 };
	LARGE_INTEGER zero = { 0 };
	BOOL result = ::SetFilePointerEx(m_file.Get(), zero, &position, FILE_CURRENT);
	if ((result == 0) || (position.QuadPart < 0))
	{
		throw ref new Platform::FailureException();
	}

	return position.QuadPart;
}

void RandomAccessReader::SeekRelative(_In_ int64 offset)
{
	LARGE_INTEGER position;
	position.QuadPart = offset;
	BOOL result = ::SetFilePointerEx(m_file.Get(), position, nullptr, FILE_CURRENT);
	if (result == 0)
	{
		throw ref new Platform::FailureException();
	}
}

void RandomAccessReader::SeekAbsolute(_In_ int64 position)
{
	if (position < 0)
	{
		throw ref new Platform::FailureException();
	}

	LARGE_INTEGER pos;
	pos.QuadPart = position;
	BOOL result = ::SetFilePointerEx(m_file.Get(), pos, nullptr, FILE_BEGIN);
	if (result == 0)
	{
		throw ref new Platform::FailureException();
	}
}


void RandomAccessReader::SeekToStart()
{
	LARGE_INTEGER zero = { 0 };
	BOOL result = ::SetFilePointerEx(m_file.Get(), zero, nullptr, FILE_BEGIN);
	if (result == 0)
	{
		throw ref new Platform::FailureException();
	}
}

void RandomAccessReader::SeekToEnd()
{
	LARGE_INTEGER zero = { 0 };
	BOOL result = ::SetFilePointerEx(m_file.Get(), zero, nullptr, FILE_END);
	if (result == 0)
	{
		throw ref new Platform::FailureException();
	}
}

///////////////////////////////////////////////////////////////////////////////
//	SOUND FILE READER
// Name: SoundFileReader constructor
// Desc: Any failure to construct will throw.
//       If the constructor succeeds this is a fully usable object
///////////////////////////////////////////////////////////////////////////////
SoundFileReader::SoundFileReader(
	_In_ Platform::String^ fileName
	)
{
	//
	// Open the file for random read access
	//
	RandomAccessReader^ riffFile = ref new RandomAccessReader(fileName);
	uint64 fileSize = riffFile->GetFileSize();

	//
	// Locate, read and validate the RIFF chunk
	//
	uint64 riffLoc = FindChunk(riffFile, FOURCC_RIFF_TAG, 0, fileSize);

	// Read beyond the riff header.
	ChunkHeader chunkHeader;
	ReadHeader(riffFile, riffLoc, chunkHeader);

	uint32 tag = 0;
	Platform::Array<byte>^ riffData = riffFile->Read(sizeof(tag));
	tag = *reinterpret_cast<uint32*>(riffData->Data);
	if (tag != FOURCC_WAVE_FILE_TAG)
	{
		// Only support .wav files
		throw ref new Platform::FailureException();
	}
	uint64 riffChildrenStart = riffLoc + sizeof(chunkHeader)+sizeof(tag);
	uint64 riffChildrenEnd = riffLoc + sizeof(chunkHeader)+chunkHeader.size;

	//
	// Find, read and validate the format chunk (a child within the RIFF chunk)
	//
	uint64 formatLoc = FindChunk(riffFile, FOURCC_FORMAT_TAG, riffChildrenStart, riffChildrenEnd);
	ReadHeader(riffFile, formatLoc, chunkHeader);
	//  if (chunkHeader.size < sizeof(WAVEFORMATEX))
	//  {
	//char buf[255];
	//sprintf(buf,"chunkheader : %d %d\n", chunkHeader.size, sizeof(WAVEFORMATEX));
	//      // Format data of unsupported size; must be unsupported format
	//      throw ref new Platform::FailureException();
	//  }
	m_soundFormat = riffFile->Read(chunkHeader.size);
	WAVEFORMATEX format = *reinterpret_cast<WAVEFORMATEX*>(m_soundFormat->Data);
	if (format.wFormatTag != WAVE_FORMAT_PCM
		&& format.wFormatTag != WAVE_FORMAT_ADPCM)
	{
		// This is not PCM or ADPCM, which is all we support
		throw ref new Platform::FailureException();
	}

	//
	// Locate, the PCM data in the data chunk
	//
	uint64 dataChunkStart = FindChunk(riffFile, FOURCC_DATA_TAG, riffChildrenStart, riffChildrenEnd);
	ReadHeader(riffFile, dataChunkStart, chunkHeader);

	//
	// Now read the PCM data and setup the buffer
	//
	m_soundData = riffFile->Read(chunkHeader.size);
}
//--------------------------------------------------------------------------------------
// Name: SoundFileReader::GetSoundData
// Desc: Sound Data accessor
//--------------------------------------------------------------------------------------
Platform::Array<byte>^ SoundFileReader::GetSoundData() const
{
	return m_soundData;
}

//--------------------------------------------------------------------------------------
// Name: SoundFileReader::GetSoundFormat
// Desc: Sound Format Accessor
//--------------------------------------------------------------------------------------
WAVEFORMATEX* SoundFileReader::GetSoundFormat() const
{
	return reinterpret_cast<WAVEFORMATEX*>(m_soundFormat->Data);
}




///////////////////////////////////////////////////////////////////////////////
//	XAUDIO2_SOUND_PLAYER
///////////////////////////////////////////////////////////////////////////////
void XAudio2SoundPlayer::ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		// Set a breakpoint on this line to catch Win32 API errors.
		throw Platform::Exception::CreateException(hr);
	}
}

//
// Per sound data required to play a sound
//
struct XAudio2SoundPlayer::ImplData
{
	ImplData::ImplData(Platform::Array<byte>^ data) :
	sourceVoice(nullptr),
	playData(data),
	isPlaying(false),
	callbackHander(&isPlaying)
	{
	}

	~ImplData()
	{
		if (sourceVoice)
		{
			sourceVoice->DestroyVoice();
			sourceVoice = nullptr;
		}
	}

	IXAudio2SourceVoice*    sourceVoice;
	Platform::Array<byte>^  playData;
	bool                    isPlaying;
	SoundCallbackHander     callbackHander;
};


//--------------------------------------------------------------------------------------
// Name: XAudio2SoundPlayer constructor
// Desc: Setup common XAudio2 objects:
//          XAudio2Engine
//          Mastering Voice
//--------------------------------------------------------------------------------------
XAudio2SoundPlayer::XAudio2SoundPlayer(uint32 sampleRate) :
m_soundList()
{
	// Create the XAudio2 Engine
	UINT32 flags = 0;
	ThrowIfFailed(
		XAudio2Create(&m_audioEngine, flags)
		);

	// Create the mastering voice
	ThrowIfFailed(
		m_audioEngine->CreateMasteringVoice(
		&m_masteringVoice,
		XAUDIO2_DEFAULT_CHANNELS,
		sampleRate
		)
		);
}

//--------------------------------------------------------------------------------------
// Name: XAudio2SoundPlayer destructor
// Desc: Stop all playing sounds, cleanup per sound data, and destroy XAudio object.
//          XAudio2Engine
//          Mastering Voice
//--------------------------------------------------------------------------------------
XAudio2SoundPlayer::~XAudio2SoundPlayer()
{
	if (m_masteringVoice != nullptr)
	{
		m_masteringVoice->DestroyVoice();
		m_masteringVoice = nullptr;
	}

	if (m_audioEngine != nullptr)
	{
		m_audioEngine->Release();
		m_audioEngine = nullptr;
	}
}

//--------------------------------------------------------------------------------------
// Name: XAudio2SoundPlayer::AddSound
// Desc: Create data, create voice, store data
//--------------------------------------------------------------------------------------
size_t XAudio2SoundPlayer::AddSound(
	_In_ WAVEFORMATEX*  format,
	_In_ Platform::Array<byte>^   data
	)
{

	//
	// Allocate the implementation data
	//
	std::shared_ptr<ImplData> implData(new ImplData(data));
	//
	// Create the source voice
	//
	ThrowIfFailed(
		m_audioEngine->CreateSourceVoice(
		&implData->sourceVoice,
		format,
		0,
		XAUDIO2_DEFAULT_FREQ_RATIO,
		reinterpret_cast<IXAudio2VoiceCallback*>(&implData->callbackHander),
		nullptr,
		nullptr
		)
		);

	//
	// Add to the list and return its index
	//
	for (int i = 0; i < m_soundList.size(); i++)
	{
		if (m_soundList[i] == nullptr){
			m_soundList[i] = implData;
			return i;
		}
	}
	m_soundList.push_back(implData);
	return (m_soundList.size() - 1);
}
bool XAudio2SoundPlayer::RemoveSound(size_t index){
	std::shared_ptr<ImplData> soundData = m_soundList[index];
	soundData.reset();
	m_soundList[index] = NULL;
	return true;
}
//--------------------------------------------------------------------------------------
// Name: XAudio2SoundPlayer::PlaySound
// Desc: Stop if already playing, setup XAudio2 Sound buffer and play
//--------------------------------------------------------------------------------------
bool XAudio2SoundPlayer::PlaySound(size_t index)
{
	//
	// Setup buffer
	//
	if (m_soundList[index] == NULL)return false;
	XAUDIO2_BUFFER playBuffer = { 0 };
	std::shared_ptr<ImplData> soundData = m_soundList[index];
	playBuffer.AudioBytes = soundData->playData->Length;
	playBuffer.pAudioData = soundData->playData->Data;
	playBuffer.Flags = XAUDIO2_END_OF_STREAM;

	//
	// In case it is playing, stop it and flush the buffers.
	//
	HRESULT hr = soundData->sourceVoice->Stop();
	if (SUCCEEDED(hr))
	{
		hr = soundData->sourceVoice->FlushSourceBuffers();
	}

	//
	// Submit the sound buffer and (re)start (ignore any 'stop' failures)
	//
	hr = soundData->sourceVoice->SubmitSourceBuffer(&playBuffer);
	if (SUCCEEDED(hr))
	{
		hr = soundData->sourceVoice->Start(0, XAUDIO2_COMMIT_ALL);
	}
	return SUCCEEDED(hr);
}

//--------------------------------------------------------------------------------------
// Name: XAudio2SoundPlayer::StopSound
// Desc: Stop if playing
//--------------------------------------------------------------------------------------
bool XAudio2SoundPlayer::StopSound(size_t index)
{
	std::shared_ptr<ImplData> soundData = m_soundList[index];
	if (soundData == NULL)return true;
	HRESULT hr = soundData->sourceVoice->Stop();
	if (SUCCEEDED(hr))
	{
		hr = soundData->sourceVoice->FlushSourceBuffers();
	}

	return SUCCEEDED(hr);
}


//--------------------------------------------------------------------------------------
// Name: XAudio2SoundPlayer::IsSoundPlaying
// Desc: Returns the playing state of the sound at sent index
//--------------------------------------------------------------------------------------
bool XAudio2SoundPlayer::IsSoundPlaying(size_t index) const
{
	return m_soundList[index]->isPlaying;
}

//--------------------------------------------------------------------------------------
// Name: XAudio2SoundPlayer::GetSoundCount
// Desc: Returns the number of sounds in the sound list
//--------------------------------------------------------------------------------------
size_t XAudio2SoundPlayer::GetSoundCount() const
{
	return m_soundList.size();
}

//--------------------------------------------------------------------------------------
// Name: XAudio2SoundPlayer::Suspend
// Desc: Stops the XAudio2 Engine
//--------------------------------------------------------------------------------------
void XAudio2SoundPlayer::Suspend()
{
	m_audioEngine->StopEngine();
}

//--------------------------------------------------------------------------------------
// Name: XAudio2SoundPlayer::Resume
// Desc: Start the XAudio2 engine
//--------------------------------------------------------------------------------------
void XAudio2SoundPlayer::Resume()
{
	ThrowIfFailed(
		m_audioEngine->StartEngine()
		);
}
*/

///////////////////////////////////////////////////////////////////////////////
//	XAUDIO
///////////////////////////////////////////////////////////////////////////////
void XAudio::ChangeDevice(ID3D11Device1* device)
{
	if (device != NULL){
		try{
			_player->Shutdown();
			_player->Initialize(device, DXGI_FORMAT_B8G8R8A8_UNORM);
		}
		catch (exception ex){

		}
	}
}

void XAudio::Init()
{
	_player = new MediaEnginePlayer();
	_sound_player = new XAudio2SoundPlayer(48000);
}

void XAudio::Release()
{

}

void XAudio::LoadMusic(ResourceItem resitem)
{
	_player->SetSource(resitem.Path);
	_player->Pause();
}
void XAudio::PlayMusic(int num_loop)
{
	_player->Play(num_loop);
}
void XAudio::PauseMusic()
{
	_player->Pause();
}
void XAudio::StopMusic()
{
	_player->Pause();
}
void XAudio::ResumeMusic(){
	_player->Resume();
}
bool XAudio::IsPlayingMusic(){
	return _player->IsPlaying();
}
HSOUND XAudio::LoadSoundEffect(ResourceItem resitem)
{
	wchar_t  buf[255];
	swprintf(buf, 255, L"%hs", resitem.Path);
	Platform::String^ spath = ref new Platform::String(buf);
	
	SoundFileReader sound(spath);
	return _sound_player->AddSound(sound.GetSoundFormat(), sound.GetSoundData());
}
void XAudio::PlaySoundEffect(HSOUND handle)
{
	_sound_player->StopSound(handle);
	_sound_player->PlaySound(handle);
}
void XAudio::PauseAllSoundEffect()
{

}
void XAudio::ResumeAllSoundEffect()
{

}
void XAudio::StopSoundEffect(HSOUND handle)
{
	_sound_player->StopSound(handle);
}
void XAudio::StopAllSoundEffect()
{

}
void XAudio::UnloadSoundEffect(HSOUND handle){
	////_sound_player->RemoveSound(handle);
}