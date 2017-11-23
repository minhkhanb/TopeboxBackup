#include "pch.h"
#include "../GameCore/CGame.h"
#include "../GameCore/Os_Functions.h"


extern "C" int	Android_GetIAPDesNum()
{
	return 3;//TOTAL_IAP_DES + 2;
}

extern "C" const char* Android_GetIAPDesList(int index)
{
	return "null";//CGame::_this->_design.getIAPDes(index, IAP_DES_GOOGLE_ID);
}

extern "C" void Android_OnBuyInAppItemComplete(int result_code, const char* item_id)
{
	PDEBUG("Android_OnBuyInAppItemComplete result_code = %d\n", result_code);
	if (result_code == 0)
	{
		PURCHASE_FUNC(item_id, enumIAPResponseStatus::IAP_RESPONSE_STATUS_SUCCESS);
	}
	else if (result_code == 2)
	{
		PURCHASE_FUNC(item_id, enumIAPResponseStatus::IAP_RESPONSE_STATUS_SUCCESS_RESTORE);
	}
	else
	{
		PURCHASE_FUNC(item_id, enumIAPResponseStatus::IAP_RESPONSE_STATUS_FAIL);
	}
}

extern "C" void Android_OnUpdateInAppItem(int index, const char* price, bool isOwned)
{
	if (isOwned)
	{
		PURCHASE_FUNC(Android_GetIAPDesList(index), enumIAPResponseStatus::IAP_RESPONSE_STATUS_SUCCESS);
		PDEBUG("Owned item: index = %d, id = %s, price = %s", index, Android_GetIAPDesList(index), price);
	}
	else
	{
		PDEBUG("Not yet owned item: index = %d, id = %s, price = %s", index, Android_GetIAPDesList(index), price);
	}
}