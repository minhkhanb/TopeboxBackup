#if	!defined(_H_JNI_GAME)
#define _H_JNI_GAME

#include <jni.h>
#include "jni_config_header.h"
#include "jni_environment.h"
#include "../../../game/GameCore/CGame.h"
#include "../../../game/Lib/Sound/OpenSLES/SoundOpenSLES.h"

#if defined(__cplusplus)
extern "C" {
#endif

void JNI_CORE_FUNCTION(Game_nativeInit) (JNIEnv* env,jobject obj, jint device_screen_w, jint device_screen_h);
void JNI_CORE_FUNCTION(Game_nativeSizeChange) (JNIEnv* env,jobject obj, jint device_screen_w, jint device_screen_h);
void JNI_CORE_FUNCTION(Game_nativePause) (JNIEnv* env);
void JNI_CORE_FUNCTION(Game_nativeResume) (JNIEnv* env);
void JNI_CORE_FUNCTION(Game_nativeUpdate) (JNIEnv* env);
void JNI_CORE_FUNCTION(Game_nativeRender) (JNIEnv* env);
void JNI_CORE_FUNCTION(Game_nativeTouchBegin)(JNIEnv* env,jobject obj,jint id, jint x, jint y);
void JNI_CORE_FUNCTION(Game_nativeTouchDrag)(JNIEnv* env,jobject obj,jint id, jint x, jint y);
void JNI_CORE_FUNCTION(Game_nativeTouchEnd)(JNIEnv* env,jobject obj,jint id, jint x, jint y);
void JNI_CORE_FUNCTION(Game_nativeChangeOrientation)(JNIEnv* env, jobject obj, jint ori);
jboolean JNI_CORE_FUNCTION(Game_nativeBackKeyPress) (JNIEnv* env);
#if defined(__cplusplus)
}
#endif
#endif //_H_JNI_GAME