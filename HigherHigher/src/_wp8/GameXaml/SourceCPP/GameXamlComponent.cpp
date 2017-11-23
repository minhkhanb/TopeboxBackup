#include "pch.h"
#include "GameXamlComponent.h"
#include "Direct3DContentProvider.h"
////#include "../../../../game/WindowsPhoneCore/CGameCore.h"
////#include "../../../../game/WindowsPhoneCore/PineDevice.h"

using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Microsoft::WRL;
using namespace Windows::Phone::Graphics::Interop;
using namespace Windows::Phone::Input::Interop;

namespace PhoneDirect3DXamlAppComponent
{

Direct3DBackground::Direct3DBackground() :
	m_timer(ref new BasicTimer())
{
}

IDrawingSurfaceBackgroundContentProvider^ Direct3DBackground::CreateContentProvider()
{
	ComPtr<Direct3DContentProvider> provider = Make<Direct3DContentProvider>(this);
	return reinterpret_cast<IDrawingSurfaceBackgroundContentProvider^>(provider.Get());
}

// IDrawingSurfaceManipulationHandler
void Direct3DBackground::SetManipulationHost(DrawingSurfaceManipulationHost^ manipulationHost)
{
	manipulationHost->PointerPressed +=
		ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DBackground::OnPointerPressed);

	manipulationHost->PointerMoved +=
		ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DBackground::OnPointerMoved);

	manipulationHost->PointerReleased +=
		ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DBackground::OnPointerReleased);
}

// Event Handlers
void Direct3DBackground::OnPointerPressed(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
	if(m_renderer == nullptr)return;
	// Insert your code here.
	int x = (int)args->CurrentPoint->Position.X;
	int y = (int)args->CurrentPoint->Position.Y;
	m_renderer->NotifyTouchBegin(args->CurrentPoint->PointerId, x, y,0);
}

void Direct3DBackground::OnPointerMoved(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
	if(m_renderer == nullptr)return;
	// Insert your code here.
	int x = (int)args->CurrentPoint->Position.X;
	int y = (int)args->CurrentPoint->Position.Y;

	m_renderer->NotifyTouchMove(args->CurrentPoint->PointerId, x, y,0);
}

void Direct3DBackground::OnPointerReleased(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
	// Insert your code here.
	int x = (int)args->CurrentPoint->Position.X;
	int y = (int)args->CurrentPoint->Position.Y;
	m_renderer->NotifyTouchEnd(args->CurrentPoint->PointerId, x, y,0);
}
bool Direct3DBackground::OnBackKeyPress(){
	m_renderer->NotifyBackKey();
	return true;
}
// Interface With Direct3DContentProvider
HRESULT Direct3DBackground::Connect(_In_ IDrawingSurfaceRuntimeHostNative* host, _In_ ID3D11Device1* device)
{
	m_renderer = ref new PineRenderer();
	m_renderer->Initialize(device);
	m_renderer->OnRequestBaseService += ref new RenderRequestBaseService(this,&Direct3DBackground::CallBaseService);
	m_renderer->UpdateForWindowSizeChange(WindowBounds.Width, WindowBounds.Height);

	// Restart timer after renderer has finished initializing.
	m_timer->Reset();
	m_renderer->NotifyResume();
	return S_OK;
}

void Direct3DBackground::Disconnect()
{
	printf("pause\n");
	m_renderer->NotifyPause();
	m_renderer = nullptr;
}

HRESULT Direct3DBackground::PrepareResources(_In_ const LARGE_INTEGER* presentTargetTime, _Inout_ DrawingSurfaceSizeF* desiredRenderTargetSize)
{
	m_timer->Update();
	m_renderer->Update(m_timer->Total, m_timer->Delta);

	desiredRenderTargetSize->width = RenderResolution.Width;
	desiredRenderTargetSize->height = RenderResolution.Height;

	return S_OK;
}

HRESULT Direct3DBackground::Draw(_In_ ID3D11Device1* device, _In_ ID3D11DeviceContext1* context, _In_ ID3D11RenderTargetView* renderTargetView)
{
	m_renderer->UpdateDevice(device, context, renderTargetView);
	m_renderer->Render();

	RequestAdditionalFrame();

	return S_OK;
}
void Direct3DBackground::CallBaseService(UINT32 constructor_id, UINT32 data_len){
	OnRequestBaseService(constructor_id,  data_len);
}

void Direct3DBackground::OnOrientationChange(int ori){
	m_renderer->DetectOrientation(ori);
}
}