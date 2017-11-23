#if !defined(_H_ANDROID_CONNECTOR)
#define _H_ANDROID_CONNECTOR

extern "C" int	Android_GetIAPDesNum();
extern "C" const char* Android_GetIAPDesList(int index);
extern "C" void Android_OnBuyInAppItemComplete(int result_code, const char* item_id);
extern "C" void Android_OnUpdateInAppItem(int index, const char* price, bool isOwned = false);

#endif//_H_ANDROID_CONNECTOR