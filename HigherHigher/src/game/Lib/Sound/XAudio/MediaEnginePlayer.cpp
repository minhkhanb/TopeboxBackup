/* 
    Copyright (c) 2012 Microsoft Corporation.  All rights reserved.
    Use of this sample source code is subject to the terms of the Microsoft license 
    agreement under which you licensed this sample source code and is provided AS-IS.
    If you did not accept the terms of the license agreement, you are not authorized 
    to use this sample source code.  For the terms of the license, please see the 
    license agreement between you and Microsoft.
  
    To see all Code Samples for Windows Phone, visit http://go.microsoft.com/fwlink/?LinkID=219604 
  
*/
#include "pch.h"
#include "MediaEnginePlayer.h"


//-----------------------------------------------------------------------------
// MediaEngineNotify
//
// Implements the callback for Media Engine event notification.
//-----------------------------------------------------------------------------


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
        if(__uuidof(IMFMediaEngineNotify) == riid)
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


//-----------------------------------------------------------------------------
// MediaEngine Player
//-----------------------------------------------------------------------------


MediaEnginePlayer::MediaEnginePlayer() :
    m_spMediaEngine(nullptr), 
    m_spEngineEx(nullptr),
    m_isPlaying(false),
	m_bstrURL(nullptr)
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
    //BSTR bstrURL = nullptr;

    if(nullptr != m_bstrURL)
    {
        ::CoTaskMemFree(m_bstrURL);
		m_bstrURL = nullptr;
    }

    size_t cchAllocationSize = 1 + ::wcslen(szURL->Data());
	m_bstrURL = (LPWSTR)::CoTaskMemAlloc(sizeof(WCHAR)*(cchAllocationSize));

    if (m_bstrURL == 0)
    {
        ThrowIfFailed(E_OUTOFMEMORY);
    }

    StringCchCopyW(m_bstrURL, cchAllocationSize, szURL->Data());
	if(m_spMediaEngine != nullptr){
		m_spMediaEngine->SetSource(m_bstrURL);
		m_spMediaEngine->SetAutoPlay(true);
	}
    return;
}
void MediaEnginePlayer::SetSource(const char* szURL){
	//return;
    //BSTR bstrURL = nullptr;
	wchar_t  buf[255];
	swprintf(buf, 255, L"%hs", szURL);
	OutputDebugString(buf);
    if(nullptr != m_bstrURL)
    {
        ::CoTaskMemFree(m_bstrURL);
		m_bstrURL = nullptr;
    }

	size_t cchAllocationSize = 1 + ::wcslen(buf);
	m_bstrURL = (LPWSTR)::CoTaskMemAlloc(sizeof(WCHAR)*(cchAllocationSize));

    if (m_bstrURL == 0)
    {
        ThrowIfFailed(E_OUTOFMEMORY);
    }

	StringCchCopyW(m_bstrURL, cchAllocationSize,buf);
	if (m_spMediaEngine != nullptr){
		m_spMediaEngine->SetSource(m_bstrURL);
		m_spMediaEngine->SetAutoPlay(true);
	}
	//Platform::String^ url = Utils::ConvertCharToString(szURL);
    //SetSource(url);

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

    if(nullptr != bstrURL)
    {
        ::CoTaskMemFree(bstrURL);
        bstrURL = nullptr;
    }

    size_t cchAllocationSize = 1 + ::wcslen(szURL->Data());
    bstrURL = (LPWSTR)::CoTaskMemAlloc(sizeof(WCHAR)*(cchAllocationSize));  

    if (bstrURL == 0)
    {
        ThrowIfFailed(E_OUTOFMEMORY);
    }

    StringCchCopyW(bstrURL, cchAllocationSize, szURL->Data());

	ThrowIfFailed(MFCreateMFByteStreamOnStreamEx((IUnknown*)streamHandle, &spMFByteStream));
    ThrowIfFailed(m_spEngineEx->SetSourceFromByteStream(spMFByteStream.Get(), bstrURL));  

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
			m_spMediaEngine->Play();
			m_isPlaying = true;
			m_loop = -1;
            break;        

        case MF_MEDIA_ENGINE_EVENT_PLAY:   
            break;       

        case MF_MEDIA_ENGINE_EVENT_PAUSE:     
            break;

        case MF_MEDIA_ENGINE_EVENT_ENDED:
			if(m_loop == -1){
				Play(-1);
			}else if(m_loop > 0){
				Play(m_loop - 1);
			}
            break;

        case MF_MEDIA_ENGINE_EVENT_TIMEUPDATE:    
            break;

        case MF_MEDIA_ENGINE_EVENT_ERROR:   
            if(m_spMediaEngine)
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
    if (m_spMediaEngine && m_spMediaEngine->HasAudio())
    {
        ThrowIfFailed(m_spMediaEngine->Play());
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
    if (m_spMediaEngine && m_spMediaEngine->HasAudio() && !m_spMediaEngine->IsPaused())
    {
        ThrowIfFailed(m_spMediaEngine->Pause());
        m_isPlaying = false;
    }
    return;
}

void MediaEnginePlayer::Resume(){
	//ThrowIfFailed(m_spMediaEngine->Play());
	//m_isPlaying = true;
	if (m_spMediaEngine != nullptr) {
		m_spMediaEngine->SetSource(m_bstrURL);
		m_spMediaEngine->SetAutoPlay(true);
	}
}

//-----------------------------------------------------------------------------
// Mute
//
// Mute the media
//-----------------------------------------------------------------------------
void MediaEnginePlayer::SetMuted(bool muted)
{    
    if (m_spMediaEngine && m_spMediaEngine->HasAudio())
    {
        ThrowIfFailed(m_spMediaEngine->SetMuted(muted));
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
void MediaEnginePlayer::TransferFrame(ComPtr<ID3D11Texture2D> texture, MFVideoNormalizedRect rect ,RECT rcTarget)
{
    if (m_spMediaEngine != nullptr && m_isPlaying)
    {
        LONGLONG pts;
        if (m_spMediaEngine->OnVideoStreamTick(&pts) == S_OK)
        {
            // new frame available at the media engine so get it 
            ThrowIfFailed(
                m_spMediaEngine->TransferVideoFrame(texture.Get(), &rect, &rcTarget, &m_bkgColor)
                );
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
    if (m_spMediaEngine.Get() != NULL)
    {
        m_spMediaEngine->Shutdown();
    }
    return;
}
