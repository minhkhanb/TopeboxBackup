#if !defined(_H_PINE_ENGINE_IO_C7Z)
#define _H_PINE_ENGINE_IO_C7Z

#include "../common.h"

#include <string>
#include <memory>

#include "EFileLocation.h"

namespace Pine
{
    namespace Io
    {
        class PE_EXPORT C7z
        {
        public:
            static bool extractFile(std::wstring archive_file, EFileLocation archive_location, std::wstring output_file, EFileLocation output_location, std::string file_identify);
        };
    }
}

#endif//_H_PINE_ENGINE_IO_C7Z