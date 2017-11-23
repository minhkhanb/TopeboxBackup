#if !defined(_H_GAMEBASE)
#define _H_GAMEBASE
#include "../Lib/DataType/CType.h"
#include <string>

#define CHILD_NODE_MAX 512

struct BaseNode
{

};

class GameBase{

    public:
    
    static PineSocial  *_fbSocial;
    static void InitGameBase();
    static void OpenSession();
    static void CloseSession();
    static void AuthFacebook(PineSocial *fb);
    static void SignOutFacebook();
    static void TestCase(int type);
};
#endif//_H_GAMEBASE
