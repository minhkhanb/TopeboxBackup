#include "pch.h"
#include "CGame.h"
#include "GameBase.h"
#include "Os_Functions.h"
#include "string.h"
#include <time.h>
#include <sys/stat.h>
extern "C" void OS_InitFireBase();
extern "C" void OS_CloseSessionFireBase();
extern "C" void OS_OpenSessionFireBase();
extern "C" void OS_AuthFacebookWithFireBase();
extern "C" void OS_SignOutFacebookFireBase();
extern "C" void OS_InitDatabaseFireBase();
extern "C" void OS_TestCase(int type);



void GameBase::TestCase(int type)
{
    OS_TestCase(type);
}


 void GameBase::InitGameBase()
{
    OS_InitFireBase();
    OS_InitDatabaseFireBase();
}


 void GameBase::OpenSession()
{
    OS_OpenSessionFireBase();
}



 void GameBase::CloseSession()
{
    OS_CloseSessionFireBase();
}

PineSocial  *GameBase::_fbSocial = NULL;

 void GameBase::AuthFacebook(PineSocial *fb)
{
    _fbSocial = fb;
    OS_AuthFacebookWithFireBase();
}

void GameBase::SignOutFacebook()
{
    if(_fbSocial)
    {
        ((PineFacebookController*)_fbSocial)->Logout();
    }
    OS_SignOutFacebookFireBase();
}
