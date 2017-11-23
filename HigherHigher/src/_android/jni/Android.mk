LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := PineEngine
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../game/Lib/PineEngine/_android/libPineEngine.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := FreeType
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../game/Lib/FreeType/_android/libFreeType.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

LOCAL_MODULE := topebox_framework

LOCAL_C_INCLUDES += $(LOCAL_PATH) \
					$(STLPORT_BASE)/stlport \
					$(LOCAL_PATH)/../../game/Lib/PineEngine/includes \
					$(LOCAL_PATH)/../../game/Lib/FreeType/header \
					$(LOCAL_PATH)/../../game/GameCore

LOCAL_CFLAGS := -DANDROID_NDK \
				-DANDROID_OS \
				-std=c++11 \
				-w \
				-DOPENSSL_STATIC\
				-D_DEBUG \
				-DNDK_DEBUG=1
				
LOCAL_CPPFLAGS  += -std=c++11 -DANDROID_NDK -DANDROID_OS -D_DEBUG

LOCAL_CPP_FEATURES += exceptions

LOCAL_SRC_FILES := \
	core/jni_OsFunction.cpp \
	core/jni_game.cpp \
	core/jni_environment.cpp \
	core/jni_log.cpp \
	core/jni_service_manager.cpp \
	../../game/GameCore/Animation.cpp \
	../../game/GameCore/CAnimObject.cpp \
	../../game/GameCore/CGame_Core.cpp \
	../../game/GameCore/CTouch.cpp \
	../../game/GameCore/Device.cpp \
	../../game/GameCore/GameContext.cpp \
	../../game/GameCore/GameFlatform.cpp \
	../../game/GameCore/GameRect.cpp \
	../../game/GameCore/Object.cpp \
	../../game/GameCore/PineGame.cpp \
	../../game/GameCore/RenderObject.cpp \
	../../game/GamePlay/Android_Connector.cpp \
	../../game/GamePlay/GameObject/CCharacter.cpp \
	../../game/GamePlay/GameObject/CDialog.cpp \
	../../game/GamePlay/GameObject/CEffectTail.cpp \
	../../game/GamePlay/GameObject/CFood.cpp \
	../../game/GamePlay/GameObject/CLine.cpp \
	../../game/GamePlay/GameObject/CObjectEnvir.cpp \
	../../game/GamePlay/GameObject/CQuest.cpp \
	../../game/GamePlay/GameObject/CScrollBar.cpp \
	../../game/GamePlay/GameObject/CUser.cpp \
	../../game/GamePlay/GameObject/GameDesign.cpp \
	../../game/GamePlay/GameObject/Jumper.cpp \
	../../game/GamePlay/GameObject/MenuSystem.cpp \
	../../game/GamePlay/GameObject/Pine_Date_Time.cpp \
	../../game/GamePlay/GameObject/SpriteList.cpp \
	../../game/GamePlay/GameObject/WorldGame.cpp \
	../../game/GamePlay/GameObject/CAsyncTasks.cpp \
	../../game/GamePlay/GameObject/Topebox_PromoGame.cpp \
	../../game/GamePlay/GameState/CGame_State_Read_Design.cpp \
	../../game/GamePlay/GameState/CGame_State_Result.cpp \
	../../game/GamePlay/MainGame/HSGame.cpp \
	../../game/Lib/png/png.cpp \
	../../game/Lib/PineADVERT.cpp \
	../../game/Lib/String/StringUtils.cpp \
	../../game/Lib/DataType/BufferStream.cpp \
	../../game/Lib/DataType/ServiceStream.cpp \
	../../game/Lib/FreeType/PineFreeType.cpp \
	../../game/Lib/FreeType/PineFreeTypeCanvas.cpp \
	../../game/Lib/FreeType/PineFreeTypeChar.cpp \
	../../game/Lib/FreeType/PineFreeTypePen.cpp \
	../../game/Lib/FreeType/PineFreeTypeText.cpp \
	../../game/Lib/FreeType/PineFreeTypeTextChar.cpp \
	../../game/Lib/Graphic/GraphicUtil.cpp \
	../../game/Lib/Graphic/PineCanvas.cpp \
	../../game/Lib/Graphic/PineColor.cpp \
	../../game/Lib/Graphic/PineColorPalette.cpp \
	../../game/Lib/Graphic/PineTexture.cpp \
	../../game/Lib/HUD/ButtonAnimation.cpp \
	../../game/Lib/MapDesigner/MapDesigner.cpp \
	../../game/Lib/Math/PineMatrix4.cpp \
	../../game/Lib/Math/PinePolygon.cpp \
	../../game/Lib/Math/PineVector3.cpp \
	../../game/Lib/OpenGLES/GLESLib1.cpp \
	../../game/Lib/OpenGLES/Graphic.cpp \
	../../game/Lib/Sound/OpenSLES/Sound.cpp \
	../../game/Lib/Sound/OpenSLES/Music.cpp \
	../../game/Lib/Sound/OpenSLES/SoundOpenSLES.cpp \
	../../game/Lib/Sound/OpenSLES/WaveFormat.cpp \
	../../game/Lib/PineHighScore.cpp \
	../../game/Lib/PineSocial.cpp \
	../../game/Lib/PineStore.cpp \
	../../game/Lib/PineAdmobAsync.cpp \
	../../game/Lib/Sound/SoundEngine.cpp \
	../../game/Lib/Sprite/PineSprite.cpp \
	../../game/Lib/Sprite/PineSpriteFModule.cpp \
	../../game/GamePlay/CGame_State.cpp \
	../../game/GamePlay/GameState/CGame_State_Logo.cpp \
	../../game/GamePlay/GameState/CGame_State_ExFunction.cpp \
	../../game/GamePlay/GameState/CGame_State_GamePlay.cpp \
	../../game/Lib/ArrayList/ArrayList.cpp \
	../../game/Lib/Math/CMath.cpp \
	../../game/Lib/Sprite/CSprite.cpp \
	../../game/Lib/PineImage.cpp \
	../../game/Lib/DataInputStream/DataInputStream.cpp \
	../../game/Lib/png/lodepng.cpp 

LOCAL_SHARED_LIBRARIES := PineEngine FreeType
	
LOCAL_LDLIBS := -latomic -ldl -llog -lstdc++ -lc -lGLESv1_CM -lm -lOpenSLES -pthread -landroid
#-lGLESv1_CM  -lGLESv2
include $(BUILD_SHARED_LIBRARY)
