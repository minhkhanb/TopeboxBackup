#include <PineEngine.h>
#include <Unity/IUnityInterface.h>
#include <Unity/IUnityGraphics.h>

static IUnityInterfaces* s_UnityInterfaces = NULL;
static IUnityGraphics* s_Graphics = NULL;
static UnityGfxRenderer s_RendererType = kUnityGfxRendererNull;

static void UNITY_INTERFACE_API
OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType)
{
	switch (eventType)
	{
	case kUnityGfxDeviceEventInitialize:
	{
		s_RendererType = s_Graphics->GetRenderer();
		//TODO: user initialization code
		break;
	}
	case kUnityGfxDeviceEventShutdown:
	{
		s_RendererType = kUnityGfxRendererNull;
		//TODO: user shutdown code
		break;
	}
	case kUnityGfxDeviceEventBeforeReset:
	{
		//TODO: user Direct3D 9 code
		break;
	}
	case kUnityGfxDeviceEventAfterReset:
	{
		//TODO: user Direct3D 9 code
		break;
	}
	};
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
UnityPluginLoad(IUnityInterfaces* unityInterfaces)
{
	s_UnityInterfaces = unityInterfaces;
	s_Graphics = unityInterfaces->Get<IUnityGraphics>();

	s_Graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);

	// Run OnGraphicsDeviceEvent(initialize) manually on plugin load
	// to not miss the event in case the graphics device is already initialized
	OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
UnityPluginUnload()
{
	s_Graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
}

// Plugin function to handle a specific rendering event
static void UNITY_INTERFACE_API OnRenderEvent(int eventID)
{
	//TODO: user rendering code
}

// Freely defined function to pass a callback to plugin-specific scripts
extern "C" UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
GetRenderEventFunc()
{
	return OnRenderEvent;
}


extern "C"
{ 
	static ACTION_CALLBACK_FUNC _action_cb;

	PE_EXPORT void regActionCallbackListenner(ACTION_CALLBACK_FUNC action_callback) {
		_action_cb = action_callback;
		_action_cb("hello plugin...", 5);
		
	}
}