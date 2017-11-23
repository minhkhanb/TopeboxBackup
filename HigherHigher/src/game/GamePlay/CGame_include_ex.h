/**
*	will be included in CGame.h
*	for include specific *.h
*/
//#include "some_classes.h"

#include "../GamePlay/GameObject/MenuSystem.h"
#include "../GamePlay/GameObject/SpriteList.h"
#include "../GamePlay/GameObject/WorldGame.h"
#include "../GamePlay/GameObject/Jumper.h"

#include "../Lib/PineHighScore.h"
#include "../Lib/DataType/BufferStream.h"
#include "../GamePlay/MainGame/HSGame_Define_Const.h"
#include "../GamePlay/GameObject/CLine.h"
#include "../GamePlay/GameObject/Pine_Date_Time.h"
//include game object
#include "../GamePlay/GameObject/CEffectTail.h"
#include "../GamePlay/GameObject/CObjectEnvir.h"
#include "../GamePlay/GameObject/CFood.h"
#include "../GamePlay/GameObject/CCharacter.h"
#include "../GamePlay/GameObject/CUser.h"
#include "../GamePlay/GameObject/CQuest.h"
#include "../GamePlay/GameObject/CDialog.h"
#include "../Lib/PineAdBox.h"
#include "../GamePlay/GameObject/Topebox_PromoGame.h"

#include "../GamePlay/GameObject/GameDesign.h"
#include "../GamePlay/MainGame/HSGame.h"

#if ANDROID_OS
#include "../GamePlay/GameObject/CAsyncTasks.h"
#include "../Lib/PineAdmobAsync.h"
#endif


