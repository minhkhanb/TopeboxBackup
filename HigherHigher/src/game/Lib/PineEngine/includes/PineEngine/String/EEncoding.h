#if !defined(_H_PINE_ENGINE_STRING_EENCODING)
#define _H_PINE_ENGINE_STRING_EENCODING

namespace Pine{
	namespace String {
		enum class PE_EXPORT EEncoding{
			ASCII,
			UTF_8,
			UTF_16,
			ACS,//unicode 32
		};
	}
}
#endif//_H_PINE_ENGINE_STRING_EENCODING