#include "pch.h"
#include "../../game/Lib/DataType/CType.h"


extern "C" int getCharaterBuffer(){
	return -1;
}
extern "C" int getImageFromUrl(){
	return -1;
}
extern "C" int UpLoadGCAvatar(){
	return -1;
}

extern "C" void FBRequestToId(INT_TIME fbId){}

extern "C" bool needUpdateNewAvatar(int friendIndex, int typeLoadAvartar){
	return false;
}


//loading friend server

extern "C" void StartLoadFriendThread(){

}
extern "C" bool checkServerRealIsAble(){
	return false;
}