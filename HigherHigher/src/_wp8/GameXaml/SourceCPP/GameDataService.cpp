#include "pch.h"
#include "GameDataService.h"
using namespace PhoneDirect3DXamlAppComponent;

int GameDataService::UploadData(Platform::WriteOnlyArray<uint8>^ outarray, int offset){
	for(int i = 0; i< outarray->Length; i++){
		outarray[i] = 1;
	}
	return outarray->Length - 1;
}
void GameDataService::NotifyDownLoadData(const Platform::Array<uint8>^ inarray){
}