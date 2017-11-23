#if !defined(_H_PINE_ENGINE_IO_EFILE_LOCATION)
#define _H_PINE_ENGINE_IO_EFILE_LOCATION

#include "../common.h"

namespace Pine
{
	namespace Io
	{
		enum class EFileLocation
		{
			//application's folder
			APP_DIR,

			//data folder of application in document folder
			DOCUMENT_DIR,

			//data folder of application in sdcard
			//some device may not support this type
			//or user don't have a sdcard.
			SDCARD_DIR,

			//file identifier is a path
			PATH,
		};
	}
}

#endif//_H_PINE_ENGINE_IO_EFILE_LOCATION