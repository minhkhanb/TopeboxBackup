#if !defined(_H_WIN8_UTILS)
#define _H_WIN8_UTILS
#if defined(_DEBUG)
extern "C" void win8_log(const char* format, ...);
#endif
class Utils{
public:
	static char* ConvertStringToCharArray(Platform::String^ string);
	static Platform::String^ ConvertCharToString(const char* string);
};
#endif