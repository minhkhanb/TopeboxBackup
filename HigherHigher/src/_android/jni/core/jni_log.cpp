#include "jni_log.h"
#include <stdarg.h>
#include <android/log.h>


void JniLog::info(const char* pmessage, ...) {
	va_list lVarArgs;
	va_start(lVarArgs, pmessage);
	__android_log_vprint(ANDROID_LOG_INFO, "INFO", pmessage,lVarArgs);
	__android_log_print(ANDROID_LOG_INFO, "INFO", "");
	va_end(lVarArgs);
}
void JniLog::debug(const char* pmessage, ...) {
	va_list lVarArgs;
	va_start(lVarArgs, pmessage);
	__android_log_vprint(ANDROID_LOG_INFO, "DEBUG", pmessage,lVarArgs);
	__android_log_print(ANDROID_LOG_INFO, "DEBUG", "");
	va_end(lVarArgs);
}
void JniLog::warn(const char* pmessage, ...) {
	va_list lVarArgs;
	va_start(lVarArgs, pmessage);
	__android_log_vprint(ANDROID_LOG_INFO, "WARNING", pmessage,lVarArgs);
	__android_log_print(ANDROID_LOG_INFO, "WARNING", "");
	va_end(lVarArgs);
}
void JniLog::error(const char* pmessage, ...) {
	va_list lVarArgs;
	va_start(lVarArgs, pmessage);
	__android_log_vprint(ANDROID_LOG_INFO, "ERROR", pmessage,lVarArgs);
	__android_log_print(ANDROID_LOG_INFO, "ERROR", "");
	va_end(lVarArgs);
}

