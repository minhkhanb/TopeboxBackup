#if !defined(_H_PINE_ENGINE_OS)
#define _H_PINE_ENGINE_OS
#include "../common.h"
#include "../Base/CResult.h"
#include "../Io/EFileLocation.h"
#include "../Resource/CResourceSegment.h"
#include "../Platform/CDeviceContext.h"
#include "../Platform/CApplication.h"
#include "../Platform/CGraphicDevice.h"
#include "../Io/EFileAttribute.h"
#include <memory>
#include <stdarg.h>
#include <stdio.h>
#include "PineEngine/wrapper_helper.h"

namespace Pine
{
	namespace Os
	{
		class PE_EXPORT OS
		{
		public:
            static CResult getResourceSegment(std::wstring resource_identify, Io::EFileLocation location, Resource::CResourceSegment* res);
            static std::wstring findResourcePath(std::wstring resource_identify, Io::EFileLocation location);
            
            static FILE* openFile(std::wstring resource_identify, Io::EFileLocation location, std::string mode = "r");
            static FILE* openFile(std::wstring path, std::string mode = "r");
            static bool setFileAttribute(std::wstring resource_identify, Io::EFileLocation location, Io::EFileAttribute attr, bool is_on);
            
            static std::shared_ptr<Pine::Platform::CDeviceContext> getDevice(std::shared_ptr<Pine::Platform::CApplication>);
			static std::shared_ptr<Pine::Platform::CGraphicDevice> getGraphic();

#if defined(PE_BUILD_WINDOWS_DESKTOP)
			static std::shared_ptr<Pine::Platform::CApplication> getApplication(HINSTANCE inst, const wchar_t* win_class);
#endif


			static void setWriteConsoleHandleFunction(OsWriteConsoleFuncPtr funcptr);
			static void resetWriteConsoleHandleFunction();
            
            static void writeConsole(const char* format, ...);
            static void writeConsoleV(const char* format, va_list varg);
		};
	}
}

#endif //_H_PINE_ENGINE_OS
