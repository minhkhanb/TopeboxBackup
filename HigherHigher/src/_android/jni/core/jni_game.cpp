#include "jni_game.h"
#include "../../../game/Define/Defines.h"
#include "jni_service_manager.h"
#include "pch.h"
//////////////////////////////////////////////////
///INCLUDE LIBRARY
//////////////////////////////////////////////////
void JNI_CORE_FUNCTION(Game_nativeInit) (JNIEnv* env,jobject obj, jint device_screen_w, jint device_screen_h){
	PDEBUG("Game Native Init\n");
	while(Environment::GetInstance() == NULL);
	(Environment::GetInstance())->Init(device_screen_w,device_screen_h);
}

void JNI_CORE_FUNCTION(Game_nativeSizeChange) (JNIEnv* env,jobject obj, jint device_screen_w, jint device_screen_h){
	while(Environment::GetInstance() == NULL);
	(Environment::GetInstance())->SizeChange(device_screen_w,device_screen_h);
}


void JNI_CORE_FUNCTION(Game_nativePause) (JNIEnv* env)
{
	if(Environment::GetInstance() != NULL)
	{
		Environment::GetInstance()->Suppend();
	}
}

void JNI_CORE_FUNCTION(Game_nativeResume) (JNIEnv* env)
{
	if(Environment::GetInstance() != NULL)
	{
		Environment::GetInstance()->Resume();
	}
}

void JNI_CORE_FUNCTION(Game_nativeUpdate) (JNIEnv* env)
{
	ServiceStreamManager::update();
	if(Environment::GetInstance() != NULL)
	{
		Environment::GetInstance()->Update();
	}
}

void JNI_CORE_FUNCTION(Game_nativeRender) (JNIEnv* env)
{
	if(Environment::GetInstance() != NULL)
	{
		Environment::GetInstance()->Render();
	}
}

void JNI_CORE_FUNCTION(Game_nativeTouchBegin)(JNIEnv* env,jobject obj,jint id, jint x, jint y)
{
	if(Environment::GetInstance() != NULL)
	{
		Environment::GetInstance()->TouchBegin(id,x,y);
	}
}

void JNI_CORE_FUNCTION(Game_nativeTouchDrag)(JNIEnv* env,jobject obj,jint id, jint x, jint y){
	if(Environment::GetInstance() != NULL){
		Environment::GetInstance()->TouchDrag(id,x,y);
	}
}

void JNI_CORE_FUNCTION(Game_nativeTouchEnd)(JNIEnv* env,jobject obj,jint id, jint x, jint y){
	if(Environment::GetInstance() != NULL){
		Environment::GetInstance()->TouchEnd(id,x,y);
	}
}
void JNI_CORE_FUNCTION(Game_nativeChangeOrientation)(JNIEnv* env, jobject obj, jint ori){
	if(Environment::GetInstance() != NULL){
		Environment::GetInstance()->ChangeOrientation(ori);
	}
}
jboolean JNI_CORE_FUNCTION(Game_nativeBackKeyPress) (JNIEnv* env)
{
	if(Environment::GetInstance() != NULL)
	{
		jboolean used = Environment::GetInstance()->BackKeyPress()? JNI_TRUE: JNI_FALSE;
		return used;
	}
	return JNI_FALSE;
}