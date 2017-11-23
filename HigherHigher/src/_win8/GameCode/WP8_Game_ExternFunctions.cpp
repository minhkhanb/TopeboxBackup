#include "pch.h"
#include <string>
#include "../../game/GameCore/CGame.h"
#include "../Pine/Src/ServerStream/ServerStreamManager.h"
#include "../../game/GameCore/Os_Functions.h"
#include "PCLSaveGameDto.h"
#include "PCLServerInfoDto.h"
#include "PCLUserDto.h"
#include "PCLChallengeUserDto.h"
#include "PCLRankedUserDto.h"
#include "PCLRankedNationDto.h"
#include "PCLSummaryDto.h"
#include "PCLSentRequestDto.h"

#if defined(BUILD_SUPPORT)
#include "../_wp8/GameXaml/GameXamlSource/PlayerInfomation.h"
#endif
using namespace Pine;
extern "C" void InitAndGetLocation(){
	GAME()->_loadNationCodeState = 0;
    //force to use region
	if (OS_GetRegionShort() == NULL)
	{
		sprintf(GAME()->_currentNationCode, "US");
	}
	else
	{
		sprintf(GAME()->_currentNationCode, "%s", OS_GetRegionShort());
	}
    
    GAME()->_loadNationCodeFrom = 1;
    
    GET_MAIN_CHARATER()._nationCodeIndex = GET_DESIGN_UTIL().getIndexForNationCode(GAME()->_currentNationCode);

    GAME()->_loadNationCodeState = 1;
}

#define SERVER_REQUEST_CODE_CHECK_SERVER				1001
#define SERVER_REQUEST_LOAD_UPDATE_VERSION				1002
#define SERVER_REQUEST_LOAD_PLAYER_HEADER				1003
#define SERVER_REQUEST_CREATE_NEW_USER					1004
#define SERVER_REQUEST_SYN_SAVE_FROM_SERVER				1005
#define SERVER_REQUEST_SYN_SAVE_TO_SERVER				1006
#define SERVER_REQUEST_GET_SERVER_TIME					1007
#define SERVER_REQUEST_GET_CHALLENGE_COUNTDOWN_TIME		1008
#define SERVER_REQUEST_GET_FOLLOWER_BUFFER				1009
#define SERVER_REQUEST_GET_CHALLENGE_BUFFER				1010
#define SERVER_REQUEST_LOADING_DETAIL_FROM_SERVER		1011
#define SERVER_REQUEST_GET_CHALLENGE_RANDOM_PLAYER		1012
#define SERVER_REQUEST_GET_TOURNAMENT_TOP_USER			1013
#define SERVER_REQUEST_GET_NATION_RECORD				1014
#define SERVER_REQUEST_SEND_USER_LOG					1015
#define SERVER_REQUEST_GET_REQUEST_LIST					1016
#define SERVER_REQUEST_SEND_A_REQUEST					1017

#define SERVER_REQUEST_AVATAR_SYN						1018
#define SERVER_REQUEST_AVATAR_DOWNLOAD					1019

#if defined(BUILD_SUPPORT)
#define SERVER_REQUEST_SUPPORT_OPEN_PAGE				1101
#endif

extern "C" void SVS_checkServerRealIsAble(SVHANDLE* handle){
	if(handle->Handle == -1 && handle->Status == SVRESULT::SV_DATA_FREE){
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_CHECK_SERVER,nullptr);
		handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}else if(handle->Handle > -1){
		if(ServerStreamManager::isStreamComplete(handle->Handle)){
			ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);
			int errorcode = stream->ErrorCode;
			/*
                * error code = 0 : use serverReal
                * error code = 1 : use serverDev
                * error code = 2 : not network
                */
			switch (errorcode)
			{
			case 2:
				handle->Status = SVRESULT::SV_DATA_ERROR_UNKNOW;
				break;
			default:
				handle->Status = SVRESULT::SV_DATA_SUCCESS;
				break;
			}

			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1;
		}else{
			handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}

extern "C" void SVS_getServerUpdateVersion(SVHANDLE* handle)
{
	if (handle->Handle == -1 && handle->Status == SVRESULT::SV_DATA_FREE)
	{
		Platform::String^ url = Utils::ConvertCharToString(SERVER_VERSION_UPDATE_URL);
		Platform::String^ fname = Utils::ConvertCharToString(SERVER_VERSION_UPDATE_FILE_NAME);
		int size = 2 + (url->Length() * 2) + 2 + (fname->Length() * 2);
		StreamBuffer^ buf = ref new StreamBuffer(size);
		buf->writeSStr(url);
		buf->writeSStr(fname);

		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_LOAD_UPDATE_VERSION,buf->Data);
		handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if(handle->Handle > -1)
	{
		if(ServerStreamManager::isStreamComplete(handle->Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);
			handle->Status = SVRESULT::SV_DATA_SUCCESS;
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1;
		}
		else
		{
			handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}

extern "C" int getHeaderUser(bool getRevision)
{
	int ret = SERVER_DATA_ERROR_NO_NETWORK;

	GAME()->_svhandle_load_header.reset();

	while (true)
	{
		if (GAME()->_svhandle_load_header.Handle == -1 && GAME()->_svhandle_load_header.Status == SVRESULT::SV_DATA_FREE)
		{
			PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
			saveDTO->InitData(1);
			saveDTO->ID[0] = GET_MAIN_CHARATER()._player_id;

			StreamBuffer^ buf = saveDTO->CreateStream();

			GAME()->_svhandle_load_header.Handle = ServerStreamManager::Request(SERVER_REQUEST_LOAD_PLAYER_HEADER, buf->Data);
			GAME()->_svhandle_load_header.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
		else if (GAME()->_svhandle_load_header.Handle > -1)
		{
			if(ServerStreamManager::isStreamComplete(GAME()->_svhandle_load_header.Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_load_header.Handle);
				
				if (stream->ErrorCode == 0)//complete
				{
					StreamBuffer^ buf = ref new StreamBuffer(stream->ResponseData);
					PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
					saveDTO->setFromStream(buf);
					if (getRevision)
					{
						GET_MAIN_CHARATER()._server_revision = saveDTO->Revision[0];
						GET_MAIN_CHARATER()._server_id = saveDTO->ID[0];
						char* avarurl = (char*)Utils::ConvertStringToCharArray(saveDTO->AvatarUrl[0]);
						sprintf(GET_MAIN_CHARATER()._avatar_url, "%s", avarurl);
						delete[] avarurl;
					}
					//GET_MAIN_CHARATER()._champion_score = saveDTO->ChampionScore[0];
					//GET_MAIN_CHARATER()._run_high_score = saveDTO->RunHighScore[0];
					
					if (GET_MAIN_CHARATER()._rank != saveDTO->Rank[0])
					{
						GAME()->_lastRankInServer = GET_MAIN_CHARATER()._rank;
						GET_MAIN_CHARATER()._rank = saveDTO->Rank[0];
					}
					
					GET_MAIN_CHARATER()._lastRank = saveDTO->LastRank[0];
					
					ret = SERVER_DATA_SUCCESS;
				}
				else if (stream->ErrorCode == 1) //no record
				{
					ret = SERVER_DATA_ERROR_NO_RECORD;
				}

				GAME()->_svhandle_load_header.Status = SVRESULT::SV_DATA_SUCCESS;
				ServerStreamManager::freeStream(GAME()->_svhandle_load_header.Handle);
				GAME()->_svhandle_load_header.Handle = -1;
			}
			else
			{
				GAME()->_svhandle_load_header.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
			}
		}

		if(GAME()->_svhandle_load_header.Status != SVRESULT::SV_DATA_WAITING_RESPONSE)
		{
			break;
		} 
		else
		{
			GAME()->Sleep(FPS_DELAY_TIME);
		}
	}
	
	return ret;
}
extern "C" void SVS_getHeaderUser(bool getRevision){
	if (GAME()->_svhandle_load_header.Handle == -1 && GAME()->_svhandle_load_header.Status == SVRESULT::SV_DATA_FREE)
	{
		PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
		saveDTO->InitData(1);
		saveDTO->ID[0] = GET_MAIN_CHARATER()._player_id;

		StreamBuffer^ buf = saveDTO->CreateStream();

		GAME()->_svhandle_load_header.Handle = ServerStreamManager::Request(SERVER_REQUEST_LOAD_PLAYER_HEADER, buf->Data);
		GAME()->_svhandle_load_header.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (GAME()->_svhandle_load_header.Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(GAME()->_svhandle_load_header.Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_load_header.Handle);

			if (stream->ErrorCode == 0)//complete
			{
				StreamBuffer^ buf = ref new StreamBuffer(stream->ResponseData);
				PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
				saveDTO->setFromStream(buf);
				if (getRevision)
				{
					GET_MAIN_CHARATER()._server_revision = saveDTO->Revision[0];
					GET_MAIN_CHARATER()._server_id = saveDTO->ID[0];
					char* avarurl = (char*)Utils::ConvertStringToCharArray(saveDTO->AvatarUrl[0]);
					sprintf(GET_MAIN_CHARATER()._avatar_url, "%s", avarurl);
					delete[] avarurl;
				}
				//GET_MAIN_CHARATER()._champion_score = saveDTO->ChampionScore[0];
				//GET_MAIN_CHARATER()._run_high_score = saveDTO->RunHighScore[0];

				if (GET_MAIN_CHARATER()._rank != saveDTO->Rank[0])
				{
					GAME()->_lastRankInServer = GET_MAIN_CHARATER()._rank;
					GET_MAIN_CHARATER()._rank = saveDTO->Rank[0];
				}

				GET_MAIN_CHARATER()._lastRank = saveDTO->LastRank[0];

				GAME()->_svhandle_load_header.Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else if (stream->ErrorCode == 1) //no record
			{
				GAME()->_svhandle_load_header.Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			else{
				GAME()->_svhandle_load_header.Status = SVRESULT::SV_DATA_ERROR_UNKNOW;
			}
			ServerStreamManager::freeStream(GAME()->_svhandle_load_header.Handle);
			GAME()->_svhandle_load_header.Handle = -1;
		}
		else
		{
			GAME()->_svhandle_load_header.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}
extern "C" int CreateNewUser()
{
	int ret = SERVER_DATA_ERROR_NO_NETWORK;

	GAME()->_svhandle_createNewUser.reset();
	
	while (true)
	{
		if (GAME()->_svhandle_createNewUser.Handle == -1 && GAME()->_svhandle_createNewUser.Status == SVRESULT::SV_DATA_FREE)
		{
			PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
			saveDTO->InitData(1);
			
			Platform::String^ userName = Utils::ConvertCharToString(GET_MAIN_CHARATER()._player_name);
			Platform::String^ token = nullptr; //Utils::ConvertCharToString(GET_MAIN_CHARATER()._token);
			Platform::String^ nationCode = Utils::ConvertCharToString( CGame_Design::NationCode_string[GET_MAIN_CHARATER()._nationCodeIndex * TOTAL_NATIONCODE_STRING_ATRIBUTE + NATIONCODE_STRING_ATRIUBTE_ISO_CODE_1] );
			
			saveDTO->setID(0, GET_MAIN_CHARATER()._player_id);
			saveDTO->setUserName(0, userName);
			saveDTO->setFacebookId(0, GET_MAIN_CHARATER()._player_id);
			saveDTO->setToken(0, token);
			saveDTO->setLevelMain(0, GET_MAIN_CHARATER()._main_camp._main_lvl);
			saveDTO->setLevelCurrentWorld(0, GET_MAIN_CHARATER()._current_unlock_map);
			saveDTO->setChooseCharater(0, GET_MAIN_CHARATER()._gender);
			saveDTO->setRunHighScore(0, GET_MAIN_CHARATER()._run_high_score);
			saveDTO->setRevision(0, GET_MAIN_CHARATER()._revision);
			saveDTO->setNationCode(0, nationCode);
			saveDTO->setChampionScore(0, GET_MAIN_CHARATER()._champion_score);
			
			/*int offset = 0;//i * MAX_SAVE_BUFFER_CACHE;
			for(int d = 0; d< MAX_SAVE_BUFFER_CACHE; d++){
				saveDTO->Data[offset + d] = GET_MAIN_CHARATER()._save_buffer[d];
			}*/

			Platform::Array<uint8>^ data = ref new Platform::Array<uint8>(10000);
			for (int k = 0; k < 10000; k++)
			{
				data[k] = GET_MAIN_CHARATER()._save_buffer[k];
			}
			saveDTO->setData(0, data); 

			/*saveDTO->Data[0] = ref new Platform::Array<uint8>(MAX_SAVE_BUFFER_CACHE);
			for(int k = 0; k < MAX_SAVE_BUFFER_CACHE; k++)
			{
				saveDTO->Data[0][k] = GET_MAIN_CHARATER()._save_buffer[k];
			}*/
			
			saveDTO->setLastChampionScore(0, 0);
			saveDTO->setLastRunHighScore(0, 0);
			saveDTO->setRank(0, 0);
			saveDTO->setLastRank(0, 0);
			saveDTO->setDataSchemaVersion(0, GET_MAIN_CHARATER()._version);
			
			StreamBuffer^ buf = saveDTO->CreateStream();

			GAME()->_svhandle_createNewUser.Handle = ServerStreamManager::Request(SERVER_REQUEST_CREATE_NEW_USER, buf->Data);
			GAME()->_svhandle_createNewUser.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
		else if (GAME()->_svhandle_createNewUser.Handle > -1)
		{
			if(ServerStreamManager::isStreamComplete(GAME()->_svhandle_createNewUser.Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_createNewUser.Handle);
				
				if (stream->ErrorCode == 0)//complete
				{
					GET_MAIN_CHARATER()._server_revision = GET_MAIN_CHARATER()._revision;
					ret = SERVER_DATA_SUCCESS;
				}
				else if (stream->ErrorCode == 1) //no record
				{
					ret = SERVER_DATA_ERROR_NO_RECORD;
				}

				GAME()->_svhandle_createNewUser.Status = SVRESULT::SV_DATA_SUCCESS;
				ServerStreamManager::freeStream(GAME()->_svhandle_createNewUser.Handle);
				GAME()->_svhandle_createNewUser.Handle = -1;
			}
			else
			{
				GAME()->_svhandle_createNewUser.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
			}
		}

		if(GAME()->_svhandle_createNewUser.Status != SVRESULT::SV_DATA_WAITING_RESPONSE)
		{
			break;
		} 
		else
		{
			GAME()->Sleep(FPS_DELAY_TIME);
		}
	}
	
	return ret;
}
extern "C" void SVS_CreateNewUser(){

	if (GAME()->_svhandle_createNewUser.Handle == -1 && GAME()->_svhandle_createNewUser.Status == SVRESULT::SV_DATA_FREE)
	{
		PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
		saveDTO->InitData(1);

		Platform::String^ userName = Utils::ConvertCharToString(GET_MAIN_CHARATER()._player_name);
		Platform::String^ token = nullptr; //Utils::ConvertCharToString(GET_MAIN_CHARATER()._token);
		Platform::String^ nationCode = Utils::ConvertCharToString(CGame_Design::NationCode_string[GET_MAIN_CHARATER()._nationCodeIndex * TOTAL_NATIONCODE_STRING_ATRIBUTE + NATIONCODE_STRING_ATRIUBTE_ISO_CODE_1]);

		saveDTO->setID(0, GET_MAIN_CHARATER()._player_id);
		saveDTO->setUserName(0, userName);
		saveDTO->setFacebookId(0, GET_MAIN_CHARATER()._player_id);
		saveDTO->setToken(0, token);
		saveDTO->setLevelMain(0, GET_MAIN_CHARATER()._main_camp._main_lvl);
		saveDTO->setLevelCurrentWorld(0, GET_MAIN_CHARATER()._current_unlock_map);
		saveDTO->setChooseCharater(0, GET_MAIN_CHARATER()._gender);
		saveDTO->setRunHighScore(0, GET_MAIN_CHARATER()._run_high_score);
		saveDTO->setRevision(0, GET_MAIN_CHARATER()._revision);
		saveDTO->setNationCode(0, nationCode);
		saveDTO->setChampionScore(0, GET_MAIN_CHARATER()._champion_score);

		/*int offset = 0;//i * MAX_SAVE_BUFFER_CACHE;
		for(int d = 0; d< MAX_SAVE_BUFFER_CACHE; d++){
		saveDTO->Data[offset + d] = GET_MAIN_CHARATER()._save_buffer[d];
		}*/

		Platform::Array<uint8>^ data = ref new Platform::Array<uint8>(10000);
		for (int k = 0; k < 10000; k++)
		{
			data[k] = GET_MAIN_CHARATER()._save_buffer[k];
		}
		saveDTO->setData(0, data);

		/*saveDTO->Data[0] = ref new Platform::Array<uint8>(MAX_SAVE_BUFFER_CACHE);
		for(int k = 0; k < MAX_SAVE_BUFFER_CACHE; k++)
		{
		saveDTO->Data[0][k] = GET_MAIN_CHARATER()._save_buffer[k];
		}*/

		saveDTO->setLastChampionScore(0, 0);
		saveDTO->setLastRunHighScore(0, 0);
		saveDTO->setRank(0, 0);
		saveDTO->setLastRank(0, 0);
		saveDTO->setDataSchemaVersion(0, GET_MAIN_CHARATER()._version);

		StreamBuffer^ buf = saveDTO->CreateStream();

		GAME()->_svhandle_createNewUser.Handle = ServerStreamManager::Request(SERVER_REQUEST_CREATE_NEW_USER, buf->Data);
		GAME()->_svhandle_createNewUser.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (GAME()->_svhandle_createNewUser.Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(GAME()->_svhandle_createNewUser.Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_createNewUser.Handle);

			if (stream->ErrorCode == 0)//complete
			{
				GET_MAIN_CHARATER()._server_revision = GET_MAIN_CHARATER()._revision;
				GAME()->_svhandle_createNewUser.Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else if (stream->ErrorCode == 1) //no record
			{
				GAME()->_svhandle_createNewUser.Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(GAME()->_svhandle_createNewUser.Handle);
			GAME()->_svhandle_createNewUser.Handle = -1;
		}
		else
		{
			GAME()->_svhandle_createNewUser.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}
extern "C" int synSaveFromServer()
{
	int ret = SERVER_DATA_ERROR_NO_NETWORK;


	GAME()->_svhandle_synSaveFromServer.reset();
	
	while (true)
	{
		if (GAME()->_svhandle_synSaveFromServer.Handle == -1 && GAME()->_svhandle_synSaveFromServer.Status == SVRESULT::SV_DATA_FREE)
		{
			PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
			saveDTO->InitData(1);
			saveDTO->setID(0, GET_MAIN_CHARATER()._player_id);
			
			StreamBuffer^ buf = saveDTO->CreateStream();

			GAME()->_svhandle_synSaveFromServer.Handle = ServerStreamManager::Request(SERVER_REQUEST_SYN_SAVE_FROM_SERVER, buf->Data);
			GAME()->_svhandle_synSaveFromServer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
		else if (GAME()->_svhandle_synSaveFromServer.Handle > -1)
		{
			if(ServerStreamManager::isStreamComplete(GAME()->_svhandle_synSaveFromServer.Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_synSaveFromServer.Handle);
				
				PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
				saveDTO->setFromStream(ref new StreamBuffer(stream->ResponseData));
				
				if (stream->ErrorCode == 0)//complete
				{
					char* username = (char*)Utils::ConvertStringToCharArray(saveDTO->UserName[0]);
					sprintf(GET_MAIN_CHARATER()._player_name, "%s", username);
					delete[] username;
					GET_DESIGN_UTIL().formatStringToAssci(GET_MAIN_CHARATER()._player_name);
					sprintf(GET_MAIN_CHARATER()._player_name, "%s", GET_DESIGN_UTIL()._string_util);
					
					GET_MAIN_CHARATER()._fb_id = saveDTO->FacebookId[0];
					char* avaurl = (char*)Utils::ConvertStringToCharArray(saveDTO->AvatarUrl[0]);
					sprintf(GET_MAIN_CHARATER()._avatar_url, "%s", avaurl);
					delete[] avaurl;
					//GET_MAIN_CHARATER()._run_high_score = saveDTO->RunHighScore[0];
					GET_MAIN_CHARATER()._player_id = saveDTO->ID[0];
					//GET_MAIN_CHARATER()._champion_score = saveDTO->ChampionScore[0];
					GET_MAIN_CHARATER()._lastRank = saveDTO->LastRank[0];
					
					int offset = 0; //i* MAX_SAVE_BUFFER_CACHE;
					for(int k = 0; k< MAX_SAVE_BUFFER_CACHE; k++){
						GET_MAIN_CHARATER()._save_buffer[k] = saveDTO->Data[offset + k];
					}
					/*for(int k = 0; k < MAX_SAVE_BUFFER_CACHE; k++)
					{
						GET_MAIN_CHARATER()._save_buffer[k] = saveDTO->Data[0][k];
					}*/
					
					ret = SERVER_DATA_SUCCESS;
				}
				else if (stream->ErrorCode == 1) //no record
				{
					ret = SERVER_DATA_ERROR_NO_RECORD;
				}

				GAME()->_svhandle_synSaveFromServer.Status = SVRESULT::SV_DATA_SUCCESS;
				ServerStreamManager::freeStream(GAME()->_svhandle_synSaveFromServer.Handle);
				GAME()->_svhandle_synSaveFromServer.Handle = -1;
			}
			else
			{
				GAME()->_svhandle_synSaveFromServer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
			}
		}

		if(GAME()->_svhandle_synSaveFromServer.Status != SVRESULT::SV_DATA_WAITING_RESPONSE)
		{
			break;
		} 
		else
		{
			GAME()->Sleep(FPS_DELAY_TIME);
		}
	}
	
	return ret;
}
extern "C" void SVS_synSaveFromServer(){


	if (GAME()->_svhandle_synSaveFromServer.Handle == -1 && GAME()->_svhandle_synSaveFromServer.Status == SVRESULT::SV_DATA_FREE)
	{
		PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
		saveDTO->InitData(1);
		saveDTO->setID(0, GET_MAIN_CHARATER()._player_id);

		StreamBuffer^ buf = saveDTO->CreateStream();

		GAME()->_svhandle_synSaveFromServer.Handle = ServerStreamManager::Request(SERVER_REQUEST_SYN_SAVE_FROM_SERVER, buf->Data);
		GAME()->_svhandle_synSaveFromServer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (GAME()->_svhandle_synSaveFromServer.Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(GAME()->_svhandle_synSaveFromServer.Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_synSaveFromServer.Handle);

			PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
			saveDTO->setFromStream(ref new StreamBuffer(stream->ResponseData));

			if (stream->ErrorCode == 0)//complete
			{
				char* name = (char*)Utils::ConvertStringToCharArray(saveDTO->UserName[0]);
				sprintf(GET_MAIN_CHARATER()._player_name, "%s", name);
				delete[] name;
				GET_DESIGN_UTIL().formatStringToAssci(GET_MAIN_CHARATER()._player_name);
				sprintf(GET_MAIN_CHARATER()._player_name, "%s", GET_DESIGN_UTIL()._string_util);

				GET_MAIN_CHARATER()._fb_id = saveDTO->FacebookId[0];
				char* avaurl = (char*)Utils::ConvertStringToCharArray(saveDTO->AvatarUrl[0]);
				sprintf(GET_MAIN_CHARATER()._avatar_url, "%s", avaurl);
				delete[] avaurl;
				//GET_MAIN_CHARATER()._run_high_score = saveDTO->RunHighScore[0];
				GET_MAIN_CHARATER()._player_id = saveDTO->ID[0];
				//GET_MAIN_CHARATER()._champion_score = saveDTO->ChampionScore[0];
				GET_MAIN_CHARATER()._lastRank = saveDTO->LastRank[0];

				int offset = 0; //i* MAX_SAVE_BUFFER_CACHE;
				for (int k = 0; k< MAX_SAVE_BUFFER_CACHE; k++){
					GET_MAIN_CHARATER()._save_buffer[k] = saveDTO->Data[offset + k];
				}
				/*for(int k = 0; k < MAX_SAVE_BUFFER_CACHE; k++)
				{
				GET_MAIN_CHARATER()._save_buffer[k] = saveDTO->Data[0][k];
				}*/

				GAME()->_svhandle_synSaveFromServer.Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else if (stream->ErrorCode == 1) //no record
			{
				GAME()->_svhandle_synSaveFromServer.Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(GAME()->_svhandle_synSaveFromServer.Handle);
			GAME()->_svhandle_synSaveFromServer.Handle = -1;
		}
		else
		{
			GAME()->_svhandle_synSaveFromServer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}
extern "C" int synSaveToServer()
{
	int ret = SERVER_DATA_ERROR_NO_NETWORK;


	GAME()->_svhandle_synSaveToServer.reset();
	
	while (true)
	{
		if (GAME()->_svhandle_synSaveToServer.Handle == -1 && GAME()->_svhandle_synSaveToServer.Status == SVRESULT::SV_DATA_FREE)
		{
			PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
			saveDTO->InitData(1);
			
			Platform::String^ userName = Utils::ConvertCharToString(GET_MAIN_CHARATER()._player_name);
			Platform::String^ token = nullptr; //Utils::ConvertCharToString(GET_MAIN_CHARATER()._token);
			Platform::String^ nationCode = Utils::ConvertCharToString( CGame_Design::NationCode_string[GET_MAIN_CHARATER()._nationCodeIndex * TOTAL_NATIONCODE_STRING_ATRIBUTE + NATIONCODE_STRING_ATRIUBTE_ISO_CODE_1] );
			
			saveDTO->setID(0, GET_MAIN_CHARATER()._player_id);
			saveDTO->setUserName(0, userName);
			saveDTO->setFacebookId(0, GET_MAIN_CHARATER()._player_id);
			saveDTO->setToken(0, token);
			saveDTO->setLevelMain(0, GET_MAIN_CHARATER()._main_camp._main_lvl);
			saveDTO->setLevelCurrentWorld(0, GET_MAIN_CHARATER()._current_unlock_map);
			saveDTO->setChooseCharater(0, GET_MAIN_CHARATER()._gender);
			saveDTO->setRunHighScore(0, GET_MAIN_CHARATER()._run_high_score);
			saveDTO->setRevision(0, GET_MAIN_CHARATER()._revision);
			saveDTO->setNationCode(0, nationCode);
			saveDTO->setChampionScore(0, GET_MAIN_CHARATER()._champion_score);
			
			Platform::Array<uint8>^ data = ref new Platform::Array<uint8>(10000);
			for (int k = 0; k < 10000; k++)
			{
				data[k] = GET_MAIN_CHARATER()._save_buffer[k];
			}
			saveDTO->setData(0, data); 

			/*int offset = 0;//i * MAX_SAVE_BUFFER_CACHE;
			for(int d = 0; d< MAX_SAVE_BUFFER_CACHE; d++){
				saveDTO->Data[offset + d] = GET_MAIN_CHARATER()._save_buffer[d];
			}*/
			
			saveDTO->setLastRunHighScore(0, 0);
			saveDTO->setRank(0, 0);
			saveDTO->setLastRank(0, 0);
			saveDTO->setDataSchemaVersion(0, GET_MAIN_CHARATER()._version);
			
			StreamBuffer^ buf = saveDTO->CreateStream();

			GAME()->_svhandle_synSaveToServer.Handle = ServerStreamManager::Request(SERVER_REQUEST_SYN_SAVE_TO_SERVER, buf->Data);
			GAME()->_svhandle_synSaveToServer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
		else if (GAME()->_svhandle_synSaveToServer.Handle > -1)
		{
			if(ServerStreamManager::isStreamComplete(GAME()->_svhandle_synSaveToServer.Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_synSaveToServer.Handle);
				
				if (stream->ErrorCode == 0)//complete
				{
					GET_MAIN_CHARATER()._last_syn_revision = GET_MAIN_CHARATER()._revision;
					ret = SERVER_DATA_SUCCESS;
				}
				else if (stream->ErrorCode == 1) //no record
				{
					ret = SERVER_DATA_ERROR_NO_RECORD;
				}

				GAME()->_svhandle_synSaveToServer.Status = SVRESULT::SV_DATA_SUCCESS;
				ServerStreamManager::freeStream(GAME()->_svhandle_synSaveToServer.Handle);
				GAME()->_svhandle_synSaveToServer.Handle = -1;
			}
			else
			{
				GAME()->_svhandle_synSaveToServer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
			}
		}

		if(GAME()->_svhandle_synSaveToServer.Status != SVRESULT::SV_DATA_WAITING_RESPONSE)
		{
			break;
		} 
		else
		{
			GAME()->Sleep(FPS_DELAY_TIME);
		}
	}
	
	return ret;
}
extern "C" void SVS_synSaveToServer()
{


#if defined(BUILD_SUPPORT)
	GAME()->_svhandle_synSaveToServer.Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
#endif
	if (GAME()->_svhandle_synSaveToServer.Handle == -1 && GAME()->_svhandle_synSaveToServer.Status == SVRESULT::SV_DATA_FREE)
	{
		PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
		saveDTO->InitData(1);

		Platform::String^ userName = Utils::ConvertCharToString(GET_MAIN_CHARATER()._player_name);
		Platform::String^ token = nullptr; //Utils::ConvertCharToString(GET_MAIN_CHARATER()._token);
		Platform::String^ nationCode = Utils::ConvertCharToString(CGame_Design::NationCode_string[GET_MAIN_CHARATER()._nationCodeIndex * TOTAL_NATIONCODE_STRING_ATRIBUTE + NATIONCODE_STRING_ATRIUBTE_ISO_CODE_1]);

		saveDTO->setID(0, GET_MAIN_CHARATER()._player_id);
		saveDTO->setUserName(0, userName);
		saveDTO->setFacebookId(0, GET_MAIN_CHARATER()._player_id);
		saveDTO->setToken(0, token);
		saveDTO->setLevelMain(0, GET_MAIN_CHARATER()._main_camp._main_lvl);
		saveDTO->setLevelCurrentWorld(0, GET_MAIN_CHARATER()._current_unlock_map);
		saveDTO->setChooseCharater(0, GET_MAIN_CHARATER()._gender);
		saveDTO->setRunHighScore(0, GET_MAIN_CHARATER()._run_high_score);
		saveDTO->setRevision(0, GET_MAIN_CHARATER()._revision);
		saveDTO->setNationCode(0, nationCode);
		saveDTO->setChampionScore(0, GET_MAIN_CHARATER()._champion_score);

		Platform::Array<uint8>^ data = ref new Platform::Array<uint8>(10000);
		for (int k = 0; k < 10000; k++)
		{
			data[k] = GET_MAIN_CHARATER()._save_buffer[k];
		}
		saveDTO->setData(0, data);

		/*int offset = 0;//i * MAX_SAVE_BUFFER_CACHE;
		for(int d = 0; d< MAX_SAVE_BUFFER_CACHE; d++){
		saveDTO->Data[offset + d] = GET_MAIN_CHARATER()._save_buffer[d];
		}*/

		saveDTO->setLastRunHighScore(0, 0);
		saveDTO->setRank(0, 0);
		saveDTO->setLastRank(0, 0);
		saveDTO->setDataSchemaVersion(0, GET_MAIN_CHARATER()._version);

		StreamBuffer^ buf = saveDTO->CreateStream();

		GAME()->_svhandle_synSaveToServer.Handle = ServerStreamManager::Request(SERVER_REQUEST_SYN_SAVE_TO_SERVER, buf->Data);
		GAME()->_svhandle_synSaveToServer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (GAME()->_svhandle_synSaveToServer.Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(GAME()->_svhandle_synSaveToServer.Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_synSaveToServer.Handle);

			if (stream->ErrorCode == 0)//complete
			{
				GET_MAIN_CHARATER()._last_syn_revision = GET_MAIN_CHARATER()._revision;
				GAME()->_svhandle_synSaveToServer.Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else if (stream->ErrorCode == 1) //no record
			{
				GAME()->_svhandle_synSaveToServer.Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(GAME()->_svhandle_synSaveToServer.Handle);
			GAME()->_svhandle_synSaveToServer.Handle = -1;
		}
		else
		{
			GAME()->_svhandle_synSaveToServer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}
extern "C" int getServerTime()
{
	int ret = SERVER_DATA_ERROR_NO_NETWORK;


	GAME()->_svhandle_getServerTime.reset();
	
	while (true)
	{
		if (GAME()->_svhandle_getServerTime.Handle == -1 && GAME()->_svhandle_getServerTime.Status == SVRESULT::SV_DATA_FREE)
		{
			PCLServerInfoDto^ serverDTO = ref new PCLServerInfoDto(); 
			
			StreamBuffer^ buf = serverDTO->CreateStream();

			GAME()->_svhandle_getServerTime.Handle = ServerStreamManager::Request(SERVER_REQUEST_GET_SERVER_TIME, buf->Data);
			GAME()->_svhandle_getServerTime.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
		else if (GAME()->_svhandle_getServerTime.Handle > -1)
		{
			if(ServerStreamManager::isStreamComplete(GAME()->_svhandle_getServerTime.Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_getServerTime.Handle);
				
				if (stream->ErrorCode == 0)//complete
				{
					StreamBuffer^ buf = ref new StreamBuffer(stream->ResponseData);
					PCLServerInfoDto^ serverInfoDTO = ref new PCLServerInfoDto();
					serverInfoDTO->setFromStream(buf);

					GAME()->_serverTime.setDateByRealDate(serverInfoDTO->PstYear[0], serverInfoDTO->PstMonth[0], serverInfoDTO->PstDay[0], serverInfoDTO->PstHour[0], serverInfoDTO->PstMinute[0], serverInfoDTO->PstSecond[0]); 

					ret = SERVER_DATA_SUCCESS;
				}
				else if (stream->ErrorCode == 1) //no record
				{
					ret = SERVER_DATA_ERROR_NO_RECORD;
				}

				GAME()->_svhandle_getServerTime.Status = SVRESULT::SV_DATA_SUCCESS;
				ServerStreamManager::freeStream(GAME()->_svhandle_getServerTime.Handle);
				GAME()->_svhandle_getServerTime.Handle = -1;
			}
			else
			{
				GAME()->_svhandle_getServerTime.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
			}
		}

		if(GAME()->_svhandle_getServerTime.Status != SVRESULT::SV_DATA_WAITING_RESPONSE)
		{
			break;
		} 
		else
		{
			GAME()->Sleep(FPS_DELAY_TIME);
		}
	}
	
	return ret;
}

extern "C" void SVS_getServerTime()
{


	if (GAME()->_svhandle_getServerTime.Handle == -1 && GAME()->_svhandle_getServerTime.Status == SVRESULT::SV_DATA_FREE)
	{
		PCLServerInfoDto^ serverDTO = ref new PCLServerInfoDto();

		StreamBuffer^ buf = serverDTO->CreateStream();

		GAME()->_svhandle_getServerTime.Handle = ServerStreamManager::Request(SERVER_REQUEST_GET_SERVER_TIME, buf->Data);
		GAME()->_svhandle_getServerTime.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (GAME()->_svhandle_getServerTime.Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(GAME()->_svhandle_getServerTime.Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_getServerTime.Handle);

			if (stream->ErrorCode == 0)//complete
			{
				StreamBuffer^ buf = ref new StreamBuffer(stream->ResponseData);
				PCLServerInfoDto^ serverInfoDTO = ref new PCLServerInfoDto();
				serverInfoDTO->setFromStream(buf);

				GAME()->_serverTime.setDateByRealDate(serverInfoDTO->PstYear[0], serverInfoDTO->PstMonth[0], serverInfoDTO->PstDay[0], serverInfoDTO->PstHour[0], serverInfoDTO->PstMinute[0], serverInfoDTO->PstSecond[0]);

				GAME()->_svhandle_getServerTime.Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else if (stream->ErrorCode == 1) //no record
			{
				GAME()->_svhandle_getServerTime.Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(GAME()->_svhandle_getServerTime.Handle);
			GAME()->_svhandle_getServerTime.Handle = -1;
		}
		else
		{
			GAME()->_svhandle_getServerTime.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}
extern "C" int getChallengeCountDownTime()
{
	int ret = SERVER_DATA_ERROR_NO_NETWORK;

	GAME()->_svhandle_getChallengeCountDownTime.reset();
	
	while (true)
	{
		if (GAME()->_svhandle_getChallengeCountDownTime.Handle == -1 && GAME()->_svhandle_getChallengeCountDownTime.Status == SVRESULT::SV_DATA_FREE)
		{
			PCLServerInfoDto^ serverDTO = ref new PCLServerInfoDto(); 
			
			StreamBuffer^ buf = serverDTO->CreateStream();

			GAME()->_svhandle_getChallengeCountDownTime.Handle = ServerStreamManager::Request(SERVER_REQUEST_GET_CHALLENGE_COUNTDOWN_TIME, buf->Data);
			GAME()->_svhandle_getChallengeCountDownTime.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
		else if (GAME()->_svhandle_getChallengeCountDownTime.Handle > -1)
		{
			if(ServerStreamManager::isStreamComplete(GAME()->_svhandle_getChallengeCountDownTime.Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_getChallengeCountDownTime.Handle);
				
				if (stream->ErrorCode == 0)//complete
				{
					StreamBuffer^ buf = ref new StreamBuffer(stream->ResponseData);
					PCLServerInfoDto^ serverInfoDTO = ref new PCLServerInfoDto();
					serverInfoDTO->setFromStream(buf);

					int64 minuteRemain = serverInfoDTO->ResetRemaining[0];
            
					if (minuteRemain <= 0)
					{
						int server_pending_minute = 60 + minuteRemain;

						GET_BATTLE_CHALLENGE_VAR()._is_open_toutnament = true;
						GET_BATTLE_CHALLENGE_VAR()._time_count_down.setDateByMinute(CMath::ABSOLUTE_VALUE(server_pending_minute));
					}
					else
					{
						GET_BATTLE_CHALLENGE_VAR()._is_open_toutnament = false;
						GET_BATTLE_CHALLENGE_VAR()._time_count_down.setDateByMinute(minuteRemain);
					}

					ret = SERVER_DATA_SUCCESS;
				}
				else if (stream->ErrorCode == 1) //no record
				{
					ret = SERVER_DATA_ERROR_NO_RECORD;
				}

				GAME()->_svhandle_getChallengeCountDownTime.Status = SVRESULT::SV_DATA_SUCCESS;
				ServerStreamManager::freeStream(GAME()->_svhandle_getChallengeCountDownTime.Handle);
				GAME()->_svhandle_getChallengeCountDownTime.Handle = -1;
			}
			else
			{
				GAME()->_svhandle_getChallengeCountDownTime.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
			}
		}

		if(GAME()->_svhandle_getChallengeCountDownTime.Status != SVRESULT::SV_DATA_WAITING_RESPONSE)
		{
			break;
		} 
		else
		{
			GAME()->Sleep(FPS_DELAY_TIME);
		}
	}
	
	return ret;
}

extern "C" void SVS_getChallengeCountDownTime()
{

	if (GAME()->_svhandle_getChallengeCountDownTime.Handle == -1 && GAME()->_svhandle_getChallengeCountDownTime.Status == SVRESULT::SV_DATA_FREE)
	{
		PCLServerInfoDto^ serverDTO = ref new PCLServerInfoDto();

		StreamBuffer^ buf = serverDTO->CreateStream();

		GAME()->_svhandle_getChallengeCountDownTime.Handle = ServerStreamManager::Request(SERVER_REQUEST_GET_CHALLENGE_COUNTDOWN_TIME, buf->Data);
		GAME()->_svhandle_getChallengeCountDownTime.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (GAME()->_svhandle_getChallengeCountDownTime.Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(GAME()->_svhandle_getChallengeCountDownTime.Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_getChallengeCountDownTime.Handle);

			if (stream->ErrorCode == 0)//complete
			{
				StreamBuffer^ buf = ref new StreamBuffer(stream->ResponseData);
				PCLServerInfoDto^ serverInfoDTO = ref new PCLServerInfoDto();
				serverInfoDTO->setFromStream(buf);

				int64 minuteRemain = serverInfoDTO->ResetRemaining[0];

				if (minuteRemain <= 0)
				{
					int server_pending_minute = 60 + minuteRemain;

					GET_BATTLE_CHALLENGE_VAR()._is_open_toutnament = true;
					GET_BATTLE_CHALLENGE_VAR()._time_count_down.setDateByMinute(CMath::ABSOLUTE_VALUE(server_pending_minute));
				}
				else
				{
					GET_BATTLE_CHALLENGE_VAR()._is_open_toutnament = false;
					GET_BATTLE_CHALLENGE_VAR()._time_count_down.setDateByMinute(minuteRemain);
				}

				GAME()->_svhandle_getChallengeCountDownTime.Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else if (stream->ErrorCode == 1) //no record
			{
				GAME()->_svhandle_getChallengeCountDownTime.Status = SVRESULT::SV_DATA_SUCCESS;
			}
			ServerStreamManager::freeStream(GAME()->_svhandle_getChallengeCountDownTime.Handle);
			GAME()->_svhandle_getChallengeCountDownTime.Handle = -1;
		}
		else
		{
			GAME()->_svhandle_getChallengeCountDownTime.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}
extern "C" int getFollowerBuffer()
{
    int ret = SERVER_DATA_SUCCESS;
    Pine_Date_Time localDay, timeAdd;
    localDay.setDateByLocalDate();
    timeAdd.setDateByMinute(FRIEND_CAN_RECRUIT_WITH_MINUTE);

	GAME()->_svhandle_getFollowerBuffer.reset();
	for (int i = 0; i < GET_FOLLOWER()._current_set; i++)
	{
		while (true)
		{
			if (GAME()->_svhandle_getFollowerBuffer.Handle == -1 && GAME()->_svhandle_getFollowerBuffer.Status == SVRESULT::SV_DATA_FREE)
			{
				PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
				saveDTO->InitData(1);
				saveDTO->setID(0, GET_FOLLOWER()._id_friend[i]);

				StreamBuffer^ buf = saveDTO->CreateStream();

				GAME()->_svhandle_getFollowerBuffer.Handle = ServerStreamManager::Request(SERVER_REQUEST_GET_FOLLOWER_BUFFER, buf->Data);
				GAME()->_svhandle_getFollowerBuffer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
			}
			else if (GAME()->_svhandle_getFollowerBuffer.Handle > -1)
			{
				if(ServerStreamManager::isStreamComplete(GAME()->_svhandle_getFollowerBuffer.Handle))
				{
					ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_getFollowerBuffer.Handle);
				
					if (stream->ErrorCode == 0)//complete
					{
						StreamBuffer^ buf = ref new StreamBuffer(stream->ResponseData);
						PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
						saveDTO->setFromStream(buf);

						int offset = 0;//i* MAX_SAVE_BUFFER_CACHE;
						for(int k = 0; k< MAX_SAVE_BUFFER_CACHE; k++)
						{
							GET_FOLLOWER()._user_friend[i]._save_buffer[k] = saveDTO->Data[offset + k];
						}
						GET_FOLLOWER()._user_friend[i].DataLoadFromBuffer();
						
						char* name = (char*)Utils::ConvertStringToCharArray(saveDTO->UserName[0]);
						sprintf(GET_FOLLOWER()._user_friend[i]._player_name, "%s", name);
						delete[] name;
                
						GET_DESIGN_UTIL().formatStringToAssci(GET_FOLLOWER()._user_friend[i]._player_name);
                
						sprintf(GET_FOLLOWER()._user_friend[i]._player_name, "%s", GET_DESIGN_UTIL()._string_util);
                
						GET_FOLLOWER()._user_friend[i]._fb_id = saveDTO->FacebookId[0];
						if (saveDTO->AvatarUrl[0]->Length() > 0)
						{
							char* avaurl = (char*)Utils::ConvertStringToCharArray(saveDTO->AvatarUrl[0]);
							sprintf(GET_FOLLOWER()._user_friend[i]._avatar_url, "%s", avaurl);
							delete[] avaurl;
						}
						else
						{
							sprintf(GET_FOLLOWER()._user_friend[i]._avatar_url, "");
						}
						//wait ssl sprintf(_to, "%s", GET_DESIGN_UTIL()._string_util);
						GET_FOLLOWER()._user_friend[i]._main_camp._main_lvl = saveDTO->LevelMain[0];
						GET_FOLLOWER()._user_friend[i]._current_unlock_map = saveDTO->LevelCurrentWorld[0];
						GET_FOLLOWER()._user_friend[i]._run_high_score = saveDTO->RunHighScore[0];
						GET_FOLLOWER()._user_friend[i]._revision = saveDTO->Revision[0];
						GET_FOLLOWER()._user_friend[i]._version = saveDTO->DataSchemaVersion[0];

						//modify time
						GET_FRIEND_MANAGER().AddOrModifyFriend(GET_FOLLOWER()._user_friend[i]._player_id, GET_FOLLOWER()._user_friend[i]._player_name, 
							GET_FOLLOWER()._user_friend[i]._main_camp._main_lvl, GET_FOLLOWER()._user_friend[i]._current_unlock_map, 
							GET_FOLLOWER()._user_friend[i]._avatar_url, GET_FOLLOWER()._user_friend[i]._gender, 
							GET_FOLLOWER()._user_friend[i]._run_high_score, GET_FOLLOWER()._user_friend[i]._champion_score, 
							GET_FOLLOWER()._user_friend[i]._rank, GET_FOLLOWER()._user_friend[i]._version, GET_FOLLOWER()._user_friend[i]._fb_id);
                
						GET_FRIEND_MANAGER()._friend[GET_FRIEND_MANAGER()._indexChoose[i]]._timeNextToTeam.setDateByDate(localDay + timeAdd);

						ret = SERVER_DATA_SUCCESS;
					}
					else if (stream->ErrorCode == 1) //no record
					{
						ret = SERVER_DATA_ERROR_NO_RECORD;
					}

					GAME()->_svhandle_getFollowerBuffer.Status = SVRESULT::SV_DATA_SUCCESS;
					ServerStreamManager::freeStream(GAME()->_svhandle_getFollowerBuffer.Handle);
					GAME()->_svhandle_getFollowerBuffer.Handle = -1;
				}
				else
				{
					GAME()->_svhandle_getFollowerBuffer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
				}
			}

			if(GAME()->_svhandle_getFollowerBuffer.Status != SVRESULT::SV_DATA_WAITING_RESPONSE)
			{
				GAME()->_svhandle_getFollowerBuffer.reset();
				break;
			} 
			else
			{
				GAME()->Sleep(FPS_DELAY_TIME);
			}
		}
	}

	return ret;
}

extern "C" void SVS_getFollowerBuffer(int foffset)
{

	if (GAME()->_svhandle_getFollowerBuffer.Handle == -1 && GAME()->_svhandle_getFollowerBuffer.Status == SVRESULT::SV_DATA_FREE)
	{
		PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
		saveDTO->InitData(1);
		saveDTO->setID(0, GET_FOLLOWER()._id_friend[foffset]);

		StreamBuffer^ buf = saveDTO->CreateStream();

		GAME()->_svhandle_getFollowerBuffer.Handle = ServerStreamManager::Request(SERVER_REQUEST_GET_FOLLOWER_BUFFER, buf->Data);
		GAME()->_svhandle_getFollowerBuffer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (GAME()->_svhandle_getFollowerBuffer.Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(GAME()->_svhandle_getFollowerBuffer.Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_getFollowerBuffer.Handle);

			if (stream->ErrorCode == 0)//complete
			{
				Pine_Date_Time localDay, timeAdd;
				localDay.setDateByLocalDate();
				timeAdd.setDateByMinute(FRIEND_CAN_RECRUIT_WITH_MINUTE);

				StreamBuffer^ buf = ref new StreamBuffer(stream->ResponseData);
				PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
				saveDTO->setFromStream(buf);

				int offset = 0;//i* MAX_SAVE_BUFFER_CACHE;
				for (int k = 0; k< MAX_SAVE_BUFFER_CACHE; k++)
				{
					GET_FOLLOWER()._user_friend[foffset]._save_buffer[k] = saveDTO->Data[offset + k];
				}
				GET_FOLLOWER()._user_friend[foffset].DataLoadFromBuffer();

				char* name = (char*)Utils::ConvertStringToCharArray(saveDTO->UserName[0]);
				sprintf(GET_FOLLOWER()._user_friend[foffset]._player_name, "%s", name);
				delete[] name;

				GET_DESIGN_UTIL().formatStringToAssci(GET_FOLLOWER()._user_friend[foffset]._player_name);

				sprintf(GET_FOLLOWER()._user_friend[foffset]._player_name, "%s", GET_DESIGN_UTIL()._string_util);

				GET_FOLLOWER()._user_friend[foffset]._fb_id = saveDTO->FacebookId[0];
				if (saveDTO->AvatarUrl[0]->Length() > 0)
				{
					char* avaurl = (char*)Utils::ConvertStringToCharArray(saveDTO->AvatarUrl[0]);
					sprintf(GET_FOLLOWER()._user_friend[foffset]._avatar_url, "%s", avaurl);
					delete[] avaurl;
				}
				else
				{
					sprintf(GET_FOLLOWER()._user_friend[foffset]._avatar_url, "");
				}
				//wait ssl sprintf(_to, "%s", GET_DESIGN_UTIL()._string_util);
				GET_FOLLOWER()._user_friend[foffset]._main_camp._main_lvl = saveDTO->LevelMain[0];
				GET_FOLLOWER()._user_friend[foffset]._current_unlock_map = saveDTO->LevelCurrentWorld[0];
				GET_FOLLOWER()._user_friend[foffset]._run_high_score = saveDTO->RunHighScore[0];
				GET_FOLLOWER()._user_friend[foffset]._revision = saveDTO->Revision[0];
				GET_FOLLOWER()._user_friend[foffset]._version = saveDTO->DataSchemaVersion[0];

				//modify time
				GET_FRIEND_MANAGER().AddOrModifyFriend(GET_FOLLOWER()._user_friend[foffset]._player_id, GET_FOLLOWER()._user_friend[foffset]._player_name,
					GET_FOLLOWER()._user_friend[foffset]._main_camp._main_lvl, GET_FOLLOWER()._user_friend[foffset]._current_unlock_map,
					GET_FOLLOWER()._user_friend[foffset]._avatar_url, GET_FOLLOWER()._user_friend[foffset]._gender,
					GET_FOLLOWER()._user_friend[foffset]._run_high_score, GET_FOLLOWER()._user_friend[foffset]._champion_score,
					GET_FOLLOWER()._user_friend[foffset]._rank, GET_FOLLOWER()._user_friend[foffset]._version, GET_FOLLOWER()._user_friend[foffset]._fb_id);

				GET_FRIEND_MANAGER()._friend[GET_FRIEND_MANAGER()._indexChoose[foffset]]._timeNextToTeam.setDateByDate(localDay + timeAdd);

				GAME()->_svhandle_getFollowerBuffer.Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else if (stream->ErrorCode == 1) //no record
			{
				GAME()->_svhandle_getFollowerBuffer.Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}

			ServerStreamManager::freeStream(GAME()->_svhandle_getFollowerBuffer.Handle);
			GAME()->_svhandle_getFollowerBuffer.Handle = -1;
		}
		else
		{
			GAME()->_svhandle_getFollowerBuffer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}
extern "C" int getChallengeCharater()
{

	int ret = SERVER_DATA_ERROR_NO_NETWORK;

	GAME()->_svhandle_getChallengeBuffer.reset();
	
	while (true)
	{
		if (GAME()->_svhandle_getChallengeBuffer.Handle == -1 && GAME()->_svhandle_getChallengeBuffer.Status == SVRESULT::SV_DATA_FREE)
		{
			PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
			saveDTO->InitData(1);
			saveDTO->setID(0, GET_FRIEND_CHARATER()._player_id);
			
			StreamBuffer^ buf = saveDTO->CreateStream();

			GAME()->_svhandle_getChallengeBuffer.Handle = ServerStreamManager::Request(SERVER_REQUEST_GET_CHALLENGE_BUFFER, buf->Data);
			GAME()->_svhandle_getChallengeBuffer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
		else if (GAME()->_svhandle_getChallengeBuffer.Handle > -1)
		{
			if(ServerStreamManager::isStreamComplete(GAME()->_svhandle_getChallengeBuffer.Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_getChallengeBuffer.Handle);
				
				PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
				saveDTO->setFromStream(ref new StreamBuffer(stream->ResponseData));
				
				if (stream->ErrorCode == 0)//complete
				{
					GET_FRIEND_CHARATER()._fb_id = saveDTO->FacebookId[0];
					if (saveDTO->AvatarUrl[0]->Length() > 0)
					{
						char* avaurl = (char*)Utils::ConvertStringToCharArray(saveDTO->AvatarUrl[0]);
						sprintf(GET_FRIEND_CHARATER()._avatar_url, "%s", avaurl);
						delete[] avaurl;
					}
					else
					{
						sprintf(GET_FRIEND_CHARATER()._avatar_url, "");
					}
					//wait ssl sprintf(_to, "%s", GET_DESIGN_UTIL()._string_util);
					if (GET_FRIEND_CHARATER()._player_id < 9000000000000000000) //real player
					{
						int offset = 0; //i* MAX_SAVE_BUFFER_CACHE;
						for(int k = 0; k< MAX_SAVE_BUFFER_CACHE; k++)
						{
							GET_FRIEND_CHARATER()._save_buffer[k] = saveDTO->Data[offset + k];
						}
                
						INT_TIME runHighScore = GET_FRIEND_CHARATER()._run_high_score;
						int championScore = GET_FRIEND_CHARATER()._champion_score;
                
						GET_FRIEND_CHARATER().DataLoadFromBuffer();
                
						GET_FRIEND_CHARATER()._run_high_score = runHighScore;
						GET_FRIEND_CHARATER()._champion_score = championScore;
					}
					else
					{
                
					}
					char* name = (char*)Utils::ConvertStringToCharArray(saveDTO->UserName[0]);
					sprintf(GET_FRIEND_CHARATER()._player_name, "%s", name);
					delete[] name;

					GET_DESIGN_UTIL().formatStringToAssci(GET_FRIEND_CHARATER()._player_name);
            
					sprintf(GET_FRIEND_CHARATER()._player_name, "%s", GET_DESIGN_UTIL()._string_util);
					
					ret = SERVER_DATA_SUCCESS;
				}
				else if (stream->ErrorCode == 1) //no record
				{
					ret = SERVER_DATA_ERROR_NO_RECORD;
				}

				GAME()->_svhandle_getChallengeBuffer.Status = SVRESULT::SV_DATA_SUCCESS;
				ServerStreamManager::freeStream(GAME()->_svhandle_getChallengeBuffer.Handle);
				GAME()->_svhandle_getChallengeBuffer.Handle = -1;
			}
			else
			{
				GAME()->_svhandle_getChallengeBuffer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
			}
		}

		if(GAME()->_svhandle_getChallengeBuffer.Status != SVRESULT::SV_DATA_WAITING_RESPONSE)
		{
			break;
		} 
		else
		{
			GAME()->Sleep(FPS_DELAY_TIME);
		}
	}
	
	return ret;
}
extern "C" void SVS_getChallengeCharater()
{

	if (GAME()->_svhandle_getChallengeBuffer.Handle == -1 && GAME()->_svhandle_getChallengeBuffer.Status == SVRESULT::SV_DATA_FREE)
	{
		PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
		saveDTO->InitData(1);
		printf("download:%lld\n", GET_FRIEND_CHARATER()._player_id);
		saveDTO->setID(0, GET_FRIEND_CHARATER()._player_id);

		StreamBuffer^ buf = saveDTO->CreateStream();

		GAME()->_svhandle_getChallengeBuffer.Handle = ServerStreamManager::Request(SERVER_REQUEST_GET_CHALLENGE_BUFFER, buf->Data);
		GAME()->_svhandle_getChallengeBuffer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (GAME()->_svhandle_getChallengeBuffer.Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(GAME()->_svhandle_getChallengeBuffer.Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_getChallengeBuffer.Handle);

			PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
			saveDTO->setFromStream(ref new StreamBuffer(stream->ResponseData));

			if (stream->ErrorCode == 0)//complete
			{
				GET_FRIEND_CHARATER()._fb_id = saveDTO->FacebookId[0];
				if (saveDTO->AvatarUrl[0]->Length() > 0)
				{
					char* avaurl = (char*)Utils::ConvertStringToCharArray(saveDTO->AvatarUrl[0]);
					sprintf(GET_FRIEND_CHARATER()._avatar_url, "%s", avaurl);
					delete[] avaurl;
				}
				else
				{
					sprintf(GET_FRIEND_CHARATER()._avatar_url, "");
				}
				printf("playerid:%lld\n", GET_FRIEND_CHARATER()._player_id);
				//wait ssl sprintf(_to, "%s", GET_DESIGN_UTIL()._string_util);
				if (GET_FRIEND_CHARATER()._player_id < 9000000000000000000) //real player
				{
					int offset = 0; //i* MAX_SAVE_BUFFER_CACHE;
					for (int k = 0; k< MAX_SAVE_BUFFER_CACHE; k++)
					{
						GET_FRIEND_CHARATER()._save_buffer[k] = saveDTO->Data[offset + k];
					}

					INT_TIME runHighScore = GET_FRIEND_CHARATER()._run_high_score;
					int championScore = GET_FRIEND_CHARATER()._champion_score;

					GET_FRIEND_CHARATER().DataLoadFromBuffer();

					GET_FRIEND_CHARATER()._run_high_score = runHighScore;
					GET_FRIEND_CHARATER()._champion_score = championScore;
				}
				else
				{

				}
				char* name = (char*)Utils::ConvertStringToCharArray(saveDTO->UserName[0]);
				sprintf(GET_FRIEND_CHARATER()._player_name, "%s", name);
				delete[] name;

				GET_DESIGN_UTIL().formatStringToAssci(GET_FRIEND_CHARATER()._player_name);

				sprintf(GET_FRIEND_CHARATER()._player_name, "%s", GET_DESIGN_UTIL()._string_util);

				GAME()->_svhandle_getChallengeBuffer.Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else if (stream->ErrorCode == 1) //no record
			{
				GAME()->_svhandle_getChallengeBuffer.Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}

			
			ServerStreamManager::freeStream(GAME()->_svhandle_getChallengeBuffer.Handle);
			GAME()->_svhandle_getChallengeBuffer.Handle = -1;
		}
		else
		{
			GAME()->_svhandle_getChallengeBuffer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}

extern "C" int loadingFriendDetaiFromServer()
{
	int ret = SERVER_DATA_ERROR_NO_NETWORK;

	int currentIndex = 0;
    int countList = 0;
	int startIndex = 0;
	while (currentIndex < GET_FRIEND_MANAGER()._currentListFriendFBCount)
	{
		currentIndex++;
		countList++;
		if (countList == 100 || currentIndex == GET_FRIEND_MANAGER()._currentListFriendFBCount)
		{
			GAME()->_svhandle_loadingDetailFromserver.reset();
			while (true)
			{
				if (GAME()->_svhandle_loadingDetailFromserver.Handle == -1 && GAME()->_svhandle_loadingDetailFromserver.Status == SVRESULT::SV_DATA_FREE)
				{
					PCLUserDto^ userDTO = ref new PCLUserDto();
			
					userDTO->InitData(currentIndex - startIndex);
					for (int i = startIndex; i < currentIndex; i++)
					{
						userDTO->setID(i - startIndex, GET_FRIEND_MANAGER()._listFriendFBID[i]);						
					}
			
					StreamBuffer^ buf = userDTO->CreateStream();

					GAME()->_svhandle_loadingDetailFromserver.Handle = ServerStreamManager::Request(SERVER_REQUEST_LOADING_DETAIL_FROM_SERVER, buf->Data);
					GAME()->_svhandle_loadingDetailFromserver.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
				}
				else if (GAME()->_svhandle_loadingDetailFromserver.Handle > -1)
				{
					if(ServerStreamManager::isStreamComplete(GAME()->_svhandle_loadingDetailFromserver.Handle))
					{
						ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_loadingDetailFromserver.Handle);
				
						if (stream->ErrorCode == 0)//complete
						{
							PCLUserDto^ userDTO = ref new PCLUserDto();
							userDTO->setFromStream(ref new StreamBuffer(stream->ResponseData));
							char name[64];
							char avartarURL[256];

							for (int i = 0; i < userDTO->_countData; i++)
							{
								char* namept = (char*)Utils::ConvertStringToCharArray(userDTO->UserName[i]);
								sprintf(name, "%s", namept);
								delete[] namept;
								GET_DESIGN_UTIL().formatStringToAssci(name);
								sprintf(name, "%s", GET_DESIGN_UTIL()._string_util);

								if (userDTO->AvatarUrl[i]->Length() > 0)
								{
									char* avaurl = (char*)Utils::ConvertStringToCharArray(userDTO->AvatarUrl[i]);
									sprintf(name, "%s", avaurl);
									delete[] avaurl;
								}
								else
								{
									sprintf(avartarURL, "");
								}

								GET_FRIEND_MANAGER().AddOrModifyFriend(userDTO->ID[i],
																   name,
																   userDTO->LevelMain[i],
																   userDTO->LevelCurrentWorld[i],
																   avartarURL,
																   userDTO->ChooseCharater[i],
																   userDTO->RunHighScore[i],
																   userDTO->ChampionScore[i],
																   userDTO->Rank[i],
																   userDTO->DataSchemaVersion[i],
																   userDTO->FacebookId[i]
																   );
							}                    
					
							ret = SERVER_DATA_SUCCESS;
						}
						else if (stream->ErrorCode == 1) //no record
						{
							ret = SERVER_DATA_ERROR_NO_RECORD;
						}

						GAME()->_svhandle_loadingDetailFromserver.Status = SVRESULT::SV_DATA_SUCCESS;
						ServerStreamManager::freeStream(GAME()->_svhandle_loadingDetailFromserver.Handle);
						GAME()->_svhandle_loadingDetailFromserver.Handle = -1;
					}
					else
					{
						GAME()->_svhandle_loadingDetailFromserver.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
					}
				}

				if(GAME()->_svhandle_loadingDetailFromserver.Status != SVRESULT::SV_DATA_WAITING_RESPONSE)
				{
					startIndex = currentIndex;
					countList = 0;
					GAME()->_svhandle_loadingDetailFromserver.reset();
					break;
				} 
				else
				{
					GAME()->Sleep(FPS_DELAY_TIME);
				}
			}
		}
	}
	return ret;
}
extern "C" void SVS_loadingFriendDetaiFromServer(int startIndex, int currentIndex)
{

	if (GAME()->_svhandle_loadingDetailFromserver.Handle == -1 && GAME()->_svhandle_loadingDetailFromserver.Status == SVRESULT::SV_DATA_FREE)
	{
		PCLUserDto^ userDTO = ref new PCLUserDto();
		userDTO->InitData(currentIndex - startIndex);
		for (int i = startIndex; i < currentIndex; i++)
		{
			userDTO->setID(i - startIndex, GET_FRIEND_MANAGER()._listFriendFBID[i]);
		}
		StreamBuffer^ buf = userDTO->CreateStream();
		GAME()->_svhandle_loadingDetailFromserver.Handle = ServerStreamManager::Request(SERVER_REQUEST_LOADING_DETAIL_FROM_SERVER, buf->Data);
		GAME()->_svhandle_loadingDetailFromserver.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (GAME()->_svhandle_loadingDetailFromserver.Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(GAME()->_svhandle_loadingDetailFromserver.Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_loadingDetailFromserver.Handle);

			if (stream->ErrorCode == 0)//complete
			{
				PCLUserDto^ userDTO = ref new PCLUserDto();
				userDTO->setFromStream(ref new StreamBuffer(stream->ResponseData));
				char name[64];
				char avartarURL[255];

				for (int i = 0; i < userDTO->_countData; i++)
				{
					char* namept = (char*)Utils::ConvertStringToCharArray(userDTO->UserName[i]);
					sprintf(name, "%s", namept);
					delete[] namept;
					GET_DESIGN_UTIL().formatStringToAssci(name);
					sprintf(name, "%s", GET_DESIGN_UTIL()._string_util);

					if (userDTO->AvatarUrl[i]->Length() > 0)
					{
						char* avaurl = (char*)Utils::ConvertStringToCharArray(userDTO->AvatarUrl[i]);
						sprintf(avartarURL, "%s", avaurl);
						delete[] avaurl;
					}
					else
					{
						sprintf(avartarURL, "");
					}

					GET_FRIEND_MANAGER().AddOrModifyFriend(userDTO->ID[i],
						name,
						userDTO->LevelMain[i],
						userDTO->LevelCurrentWorld[i],
						avartarURL,
						userDTO->ChooseCharater[i],
						userDTO->RunHighScore[i],
						userDTO->ChampionScore[i],
						userDTO->Rank[i],
						userDTO->DataSchemaVersion[i],
						userDTO->FacebookId[i]
						);
				}
				GAME()->_svhandle_loadingDetailFromserver.Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else if (stream->ErrorCode == 1) //no record
			{
				GAME()->_svhandle_loadingDetailFromserver.Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(GAME()->_svhandle_loadingDetailFromserver.Handle);
			GAME()->_svhandle_loadingDetailFromserver.Handle = -1;
		}
		else
		{
			GAME()->_svhandle_loadingDetailFromserver.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}

extern "C" int getChallengeRandomPlayer()
{
	int ret = SERVER_DATA_ERROR_NO_NETWORK;

	GAME()->_svhandle_getChallengeRandomPlayer.reset();
	
	while (true)
	{
		if (GAME()->_svhandle_getChallengeRandomPlayer.Handle == -1 && GAME()->_svhandle_getChallengeRandomPlayer.Status == SVRESULT::SV_DATA_FREE)
		{
			PCLChallengeUserDto^ challengeDTO = ref new PCLChallengeUserDto();
			challengeDTO->InitData(1);

			StreamBuffer^ buf = challengeDTO->CreateStream();

			GAME()->_svhandle_getChallengeRandomPlayer.Handle = ServerStreamManager::Request(SERVER_REQUEST_GET_CHALLENGE_RANDOM_PLAYER, buf->Data);
			GAME()->_svhandle_getChallengeRandomPlayer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
		else if (GAME()->_svhandle_getChallengeRandomPlayer.Handle > -1)
		{
			if(ServerStreamManager::isStreamComplete(GAME()->_svhandle_getChallengeRandomPlayer.Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_getChallengeRandomPlayer.Handle);
				
				if (stream->ErrorCode == 0)//complete
				{
					PCLChallengeUserDto^ challengeDTO = ref new PCLChallengeUserDto();
					challengeDTO->setFromStream(ref new StreamBuffer(stream->ResponseData));
				
					GET_BATTLE_CHALLENGE_VAR()._current_user = 0;

					char* name = NULL;
					for (int i = 0; i < challengeDTO->_countData; i++)
					{
						GET_BATTLE_CHALLENGE_VAR()._user[GET_BATTLE_CHALLENGE_VAR()._current_user]._id = challengeDTO->ID[i];
						name = (char*)Utils::ConvertStringToCharArray(challengeDTO->UserName[i]);
						sprintf(GET_BATTLE_CHALLENGE_VAR()._user[GET_BATTLE_CHALLENGE_VAR()._current_user]._name, "%s", name);
						delete[] name;

						GET_DESIGN_UTIL().formatStringToAssci(GET_BATTLE_CHALLENGE_VAR()._user[GET_BATTLE_CHALLENGE_VAR()._current_user]._name);
            
						sprintf(GET_BATTLE_CHALLENGE_VAR()._user[GET_BATTLE_CHALLENGE_VAR()._current_user]._name, "%s", GET_DESIGN_UTIL()._string_util);
            
						name = (char*)Utils::ConvertStringToCharArray(challengeDTO->AvatarUrl[i]);
						sprintf(GET_BATTLE_CHALLENGE_VAR()._user[GET_BATTLE_CHALLENGE_VAR()._current_user]._avartarURL, "%s", name);
						delete[] name;

						//GET_BATTLE_CHALLENGE_VAR()._user[GET_BATTLE_CHALLENGE_VAR()._current_user]._run_high_score = challengeDTO->RunHighScore[i];
						GET_BATTLE_CHALLENGE_VAR()._user[GET_BATTLE_CHALLENGE_VAR()._current_user]._run_high_score = GET_BATTLE_CHALLENGE_VAR().getMinRunHighScore(challengeDTO->RunHighScore[i]);
						GET_BATTLE_CHALLENGE_VAR()._user[GET_BATTLE_CHALLENGE_VAR()._current_user]._chooseGender = challengeDTO->ChooseCharater[i];
						GET_BATTLE_CHALLENGE_VAR()._user[GET_BATTLE_CHALLENGE_VAR()._current_user]._champion_score = challengeDTO->ChampionScore[i];
						GET_BATTLE_CHALLENGE_VAR()._user[GET_BATTLE_CHALLENGE_VAR()._current_user]._rank = challengeDTO->Rank[i];
						GET_BATTLE_CHALLENGE_VAR()._user[GET_BATTLE_CHALLENGE_VAR()._current_user]._champion_score = GET_BATTLE_CHALLENGE_VAR().getChampionMaxScore(GET_MAIN_CHARATER()._champion_score, GET_BATTLE_CHALLENGE_VAR()._user[GET_BATTLE_CHALLENGE_VAR()._current_user]._champion_score);
						
						name = (char*)Utils::ConvertStringToCharArray(challengeDTO->NationCode[i]);
						GET_BATTLE_CHALLENGE_VAR()._user[GET_BATTLE_CHALLENGE_VAR()._current_user]._flagCode = GET_DESIGN_UTIL().getIndexForNationCode(name);
						delete[] name;

						GET_BATTLE_CHALLENGE_VAR()._current_user++;
					}
					
					ret = SERVER_DATA_SUCCESS;
				}
				else if (stream->ErrorCode == 1) //no record
				{
					ret = SERVER_DATA_ERROR_NO_RECORD;
				}

				GAME()->_svhandle_getChallengeRandomPlayer.Status = SVRESULT::SV_DATA_SUCCESS;
				ServerStreamManager::freeStream(GAME()->_svhandle_getChallengeRandomPlayer.Handle);
				GAME()->_svhandle_getChallengeRandomPlayer.Handle = -1;
			}
			else
			{
				GAME()->_svhandle_getChallengeRandomPlayer.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
			}
		}

		if(GAME()->_svhandle_getChallengeRandomPlayer.Status != SVRESULT::SV_DATA_WAITING_RESPONSE)
		{
			break;
		} 
		else
		{
			GAME()->Sleep(FPS_DELAY_TIME);
		}
	}
	
	return ret;
}

extern "C" int getTounamentTopUser()
{
	int ret = SERVER_DATA_ERROR_NO_NETWORK;

	GAME()->_svhandle_getTounamentTopUser.reset();
	
	while (true)
	{
		if (GAME()->_svhandle_getTounamentTopUser.Handle == -1 && GAME()->_svhandle_getTounamentTopUser.Status == SVRESULT::SV_DATA_FREE)
		{
			PCLRankedUserDto^ rankedDTO = ref new PCLRankedUserDto();
			rankedDTO->InitData(1);

			StreamBuffer^ buf = rankedDTO->CreateStream();

			GAME()->_svhandle_getTounamentTopUser.Handle = ServerStreamManager::Request(SERVER_REQUEST_GET_TOURNAMENT_TOP_USER, buf->Data);
			GAME()->_svhandle_getTounamentTopUser.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
		else if (GAME()->_svhandle_getTounamentTopUser.Handle > -1)
		{
			if(ServerStreamManager::isStreamComplete(GAME()->_svhandle_getTounamentTopUser.Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_getTounamentTopUser.Handle);
				
				if (stream->ErrorCode == 0)//complete
				{
					PCLRankedUserDto^ rankedDTO = ref new PCLRankedUserDto();
					rankedDTO->setFromStream(ref new StreamBuffer(stream->ResponseData));
					GET_BATTLE_CHALLENGE_VAR()._topUserCount = rankedDTO->_countData;
					char* chbuf = NULL;
					for (int i = 0; i < rankedDTO->_countData; i++)
					{
						GET_BATTLE_CHALLENGE_VAR()._topUser[i]._id = rankedDTO->ID[i];
						chbuf = (char*)Utils::ConvertStringToCharArray(rankedDTO->UserName[i]);
						sprintf(GET_BATTLE_CHALLENGE_VAR()._topUser[i]._name, "%s", chbuf);
						delete[] chbuf;

						GET_DESIGN_UTIL().formatStringToAssci(GET_BATTLE_CHALLENGE_VAR()._topUser[i]._name);
						sprintf(GET_BATTLE_CHALLENGE_VAR()._topUser[i]._name, "%s", GET_DESIGN_UTIL()._string_util);
                
						chbuf = (char*)Utils::ConvertStringToCharArray(rankedDTO->AvatarUrl[i]);
						sprintf(GET_BATTLE_CHALLENGE_VAR()._topUser[i]._avartarURL, "%s", chbuf);
						delete[] chbuf;
                
						chbuf = (char*)Utils::ConvertStringToCharArray(rankedDTO->NationCode[i]);
						GET_BATTLE_CHALLENGE_VAR()._topUser[i]._flagCode = GET_DESIGN_UTIL().getIndexForNationCode(chbuf);
						delete[] chbuf;
                
						GET_BATTLE_CHALLENGE_VAR()._topUser[i]._champion_score = rankedDTO->ChampionScore[i];
						GET_BATTLE_CHALLENGE_VAR()._topUser[i]._rank = i+1;
						GET_BATTLE_CHALLENGE_VAR()._topUser[i]._chooseGender = rankedDTO->ChooseCharater[i];
					}
					
					ret = SERVER_DATA_SUCCESS;
				}
				else if (stream->ErrorCode == 1) //no record
				{
					ret = SERVER_DATA_ERROR_NO_RECORD;
				}

				GAME()->_svhandle_getTounamentTopUser.Status = SVRESULT::SV_DATA_SUCCESS;
				ServerStreamManager::freeStream(GAME()->_svhandle_getTounamentTopUser.Handle);
				GAME()->_svhandle_getTounamentTopUser.Handle = -1;
			}
			else
			{
				GAME()->_svhandle_getTounamentTopUser.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
			}
		}

		if(GAME()->_svhandle_getTounamentTopUser.Status != SVRESULT::SV_DATA_WAITING_RESPONSE)
		{
			break;
		} 
		else
		{
			GAME()->Sleep(FPS_DELAY_TIME);
		}
	}
	
	return ret;
}

extern "C" int getNationRecord()
{
	int ret = SERVER_DATA_ERROR_NO_NETWORK;

	GAME()->_svhandle_getNationRecord.reset();
	
	while (true)
	{
		if (GAME()->_svhandle_getNationRecord.Handle == -1 && GAME()->_svhandle_getNationRecord.Status == SVRESULT::SV_DATA_FREE)
		{
			PCLRankedNationDto^ rankedNationDTO = ref new PCLRankedNationDto();
			rankedNationDTO->InitData(1);

			StreamBuffer^ buf = rankedNationDTO->CreateStream();

			GAME()->_svhandle_getNationRecord.Handle = ServerStreamManager::Request(SERVER_REQUEST_GET_NATION_RECORD, buf->Data);
			GAME()->_svhandle_getNationRecord.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
		else if (GAME()->_svhandle_getNationRecord.Handle > -1)
		{
			if(ServerStreamManager::isStreamComplete(GAME()->_svhandle_getNationRecord.Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_getNationRecord.Handle);
				
				if (stream->ErrorCode == 0)//complete
				{
					PCLRankedNationDto^ rankedNationDTO = ref new PCLRankedNationDto();
					rankedNationDTO->setFromStream(ref new StreamBuffer(stream->ResponseData));

					char* chbuf = NULL;
					for (int i = 0; i < rankedNationDTO->_countData; i++)
					{
						chbuf = (char*)Utils::ConvertStringToCharArray(rankedNationDTO->ID[i]);
						GET_BATTLE_CHALLENGE_VAR()._nationRecord[i]._flagID = GET_DESIGN_UTIL().getIndexForNationCode(chbuf);
						delete[] chbuf;

						GET_BATTLE_CHALLENGE_VAR()._nationRecord[i]._memberCount = rankedNationDTO->MembersCount[i];
						GET_BATTLE_CHALLENGE_VAR()._nationRecord[i]._sum = rankedNationDTO->MembersCount[i];
                
						GET_DESIGN_UTIL().ConvertNumberToStringWithCommas(GET_BATTLE_CHALLENGE_VAR()._nationRecord[i]._memberCount, 4);
						sprintf(GET_BATTLE_CHALLENGE_VAR()._nationRecord[i]._countStr, "%s", GET_DESIGN_UTIL()._string_util);
                
						GET_DESIGN_UTIL().ConvertNumberToStringWithCommas(GET_BATTLE_CHALLENGE_VAR()._nationRecord[i]._sum, 5);
						sprintf(GET_BATTLE_CHALLENGE_VAR()._nationRecord[i]._sumStr, "%s", GET_DESIGN_UTIL()._string_util);
					}
					GET_BATTLE_CHALLENGE_VAR()._currentNationRecord = rankedNationDTO->_countData;
					ret = SERVER_DATA_SUCCESS;
				}
				else if (stream->ErrorCode == 1) //no record
				{
					ret = SERVER_DATA_ERROR_NO_RECORD;
				}

				GAME()->_svhandle_getNationRecord.Status = SVRESULT::SV_DATA_SUCCESS;
				ServerStreamManager::freeStream(GAME()->_svhandle_getNationRecord.Handle);
				GAME()->_svhandle_getNationRecord.Handle = -1;
			}
			else
			{
				GAME()->_svhandle_getNationRecord.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
			}
		}

		if(GAME()->_svhandle_getNationRecord.Status != SVRESULT::SV_DATA_WAITING_RESPONSE)
		{
			break;
		} 
		else
		{
			GAME()->Sleep(FPS_DELAY_TIME);
		}
	}
	
	return ret;
}

extern "C" int getRewarRank_Extern()
{
	int ret = SERVER_DATA_ERROR_NO_NETWORK;

	int currentIndex = 0;
    int countList = 0;
	int startIndex = 0;
	while (currentIndex < GET_FRIEND_MANAGER()._current_friend)
	{
		currentIndex++;
		countList++;
		if (countList == 100 || currentIndex == GET_FRIEND_MANAGER()._current_friend)
		{
			GAME()->_svhandle_loadingDetailFromserver.reset();
			while (true)
			{
				if (GAME()->_svhandle_loadingDetailFromserver.Handle == -1 && GAME()->_svhandle_loadingDetailFromserver.Status == SVRESULT::SV_DATA_FREE)
				{
					PCLUserDto^ userDTO = ref new PCLUserDto();
			
					userDTO->InitData(currentIndex - startIndex);
					for (int i = startIndex; i < currentIndex; i++)
					{
						userDTO->setID(i - startIndex, GET_FRIEND_MANAGER()._friend[i]._id);						
					}
			
					StreamBuffer^ buf = userDTO->CreateStream();

					GAME()->_svhandle_loadingDetailFromserver.Handle = ServerStreamManager::Request(SERVER_REQUEST_LOADING_DETAIL_FROM_SERVER, buf->Data);
					GAME()->_svhandle_loadingDetailFromserver.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
				}
				else if (GAME()->_svhandle_loadingDetailFromserver.Handle > -1)
				{
					if(ServerStreamManager::isStreamComplete(GAME()->_svhandle_loadingDetailFromserver.Handle))
					{
						ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_loadingDetailFromserver.Handle);
				
						if (stream->ErrorCode == 0)//complete
						{
							PCLUserDto^ userDTO = ref new PCLUserDto();
							userDTO->setFromStream(ref new StreamBuffer(stream->ResponseData));
							char name[64];
							char avartarURL[256];
							char* chbuf = NULL;
							for (int i = 0; i < userDTO->_countData; i++)
							{
								chbuf = (char*)Utils::ConvertStringToCharArray(userDTO->UserName[i]);
								sprintf(name, "%s", chbuf);
								delete[] chbuf;
								GET_DESIGN_UTIL().formatStringToAssci(name);
								sprintf(name, "%s", GET_DESIGN_UTIL()._string_util);

								if (userDTO->AvatarUrl[i]->Length() > 0)
								{
									chbuf = (char*)Utils::ConvertStringToCharArray(userDTO->AvatarUrl[i]);
									sprintf(avartarURL, "%s", chbuf);
									delete[] chbuf;
								}
								else
								{
									sprintf(avartarURL, "");
								}

								GET_FRIEND_MANAGER().AddOrModifyFriend(userDTO->ID[i],
																   name,
																   userDTO->LevelMain[i],
																   userDTO->LevelCurrentWorld[i],
																   avartarURL,
																   userDTO->ChooseCharater[i],
																   userDTO->RunHighScore[i],
																   userDTO->ChampionScore[i],
																   userDTO->Rank[i],
																   userDTO->DataSchemaVersion[i],
																   userDTO->FacebookId[i]
																   );
							}                    
					
							ret = SERVER_DATA_SUCCESS;
						}
						else if (stream->ErrorCode == 1) //no record
						{
							ret = SERVER_DATA_ERROR_NO_RECORD;
						}

						GAME()->_svhandle_loadingDetailFromserver.Status = SVRESULT::SV_DATA_SUCCESS;
						ServerStreamManager::freeStream(GAME()->_svhandle_loadingDetailFromserver.Handle);
						GAME()->_svhandle_loadingDetailFromserver.Handle = -1;
					}
					else
					{
						GAME()->_svhandle_loadingDetailFromserver.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
					}
				}

				if(GAME()->_svhandle_loadingDetailFromserver.Status != SVRESULT::SV_DATA_WAITING_RESPONSE)
				{
					startIndex = currentIndex;
					countList = 0;
					GAME()->_svhandle_loadingDetailFromserver.reset();
					break;
				} 
				else
				{
					GAME()->Sleep(FPS_DELAY_TIME);
				}
			}
		}
	}

	return ret;
}

extern "C" int SendUserLog()
{
	int ret = SERVER_DATA_ERROR_NO_NETWORK;

	GAME()->_svhandle_sendUserLog.reset();
	
	while (true)
	{
		if (GAME()->_svhandle_sendUserLog.Handle == -1 && GAME()->_svhandle_sendUserLog.Status == SVRESULT::SV_DATA_FREE)
		{
			PCLSummaryDto^ summaryDTO = ref new PCLSummaryDto();
			summaryDTO->InitData(1);
			summaryDTO->setID(0, GET_MAIN_CHARATER()._player_id);
			summaryDTO->setLevelMain(0, GET_MAIN_CHARATER()._log_currentBaseLevel);
			summaryDTO->setLevelCurrentWorld(0, GET_MAIN_CHARATER()._log_currentMaplocation);
			summaryDTO->setTournamentsPlayed(0, GET_MAIN_CHARATER()._log_tournament_played_counter);
			summaryDTO->setTournamentsPushed(0, GET_MAIN_CHARATER()._log_tournament_pushed_counter);
			summaryDTO->setLivesSent(0, GET_MAIN_CHARATER()._log_lives_sent);
			summaryDTO->setAbaddonProcessing(0, GET_MAIN_CHARATER()._log_abaddon_won_counter);
			summaryDTO->setXmasProcessing(0, GET_MAIN_CHARATER()._log_xmas_processing_counter);
			summaryDTO->setFriendsCount(0, GET_MAIN_CHARATER()._log_friend_counter);
			summaryDTO->setBagOfVacinePack(0, GET_MAIN_CHARATER()._log_packetIAP[0]);
			summaryDTO->setCrateOfVacinePack(0, GET_MAIN_CHARATER()._log_packetIAP[1]);
			summaryDTO->setStockpileOfVacinePack(0, GET_MAIN_CHARATER()._log_packetIAP[2]);
			summaryDTO->setTruckLoadOfVacinePack(0, GET_MAIN_CHARATER()._log_packetIAP[3]);
			summaryDTO->setVacinePackReserve01(0, GET_MAIN_CHARATER()._log_packetIAP[4]);
			summaryDTO->setVacinePackReserve02(0, GET_MAIN_CHARATER()._log_packetIAP[5]);
			summaryDTO->setBagOfFoodPack(0, GET_MAIN_CHARATER()._log_packetIAP[6]);
			summaryDTO->setCrateOfFoodPack(0, GET_MAIN_CHARATER()._log_packetIAP[7]);
			summaryDTO->setStockpileOfFoodPack(0, GET_MAIN_CHARATER()._log_packetIAP[8]);
			summaryDTO->setTruckLoadOfFoodPack(0, GET_MAIN_CHARATER()._log_packetIAP[9]);
			summaryDTO->setFoodPackReserve01(0, GET_MAIN_CHARATER()._log_packetIAP[10]);
			summaryDTO->setFoodPackReserve02(0, GET_MAIN_CHARATER()._log_packetIAP[11]);

			StreamBuffer^ buf = summaryDTO->CreateStream();

			GAME()->_svhandle_sendUserLog.Handle = ServerStreamManager::Request(SERVER_REQUEST_SEND_USER_LOG, buf->Data);
			GAME()->_svhandle_sendUserLog.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
		else if (GAME()->_svhandle_sendUserLog.Handle > -1)
		{
			if(ServerStreamManager::isStreamComplete(GAME()->_svhandle_sendUserLog.Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_sendUserLog.Handle);
				
				if (stream->ErrorCode == 0)//complete
				{
					ret = SERVER_DATA_SUCCESS;
				}
				else if (stream->ErrorCode == 1) //no record
				{
					ret = SERVER_DATA_ERROR_NO_RECORD;
				}

				GAME()->_svhandle_sendUserLog.Status = SVRESULT::SV_DATA_SUCCESS;
				ServerStreamManager::freeStream(GAME()->_svhandle_sendUserLog.Handle);
				GAME()->_svhandle_sendUserLog.Handle = -1;
			}
			else
			{
				GAME()->_svhandle_sendUserLog.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
			}
		}

		if(GAME()->_svhandle_sendUserLog.Status != SVRESULT::SV_DATA_WAITING_RESPONSE)
		{
			break;
		} 
		else
		{
			GAME()->Sleep(FPS_DELAY_TIME);
		}
	}
	
	return ret;
}
extern "C" void SVS_sendUserLog()
{

	if (GAME()->_svhandle_sendUserLog.Handle == -1 && GAME()->_svhandle_sendUserLog.Status == SVRESULT::SV_DATA_FREE)
	{
		PCLSummaryDto^ summaryDTO = ref new PCLSummaryDto();
		summaryDTO->InitData(1);
		summaryDTO->setID(0, GET_MAIN_CHARATER()._player_id);
		summaryDTO->setLevelMain(0, GET_MAIN_CHARATER()._log_currentBaseLevel);
		summaryDTO->setLevelCurrentWorld(0, GET_MAIN_CHARATER()._log_currentMaplocation);
		summaryDTO->setTournamentsPlayed(0, GET_MAIN_CHARATER()._log_tournament_played_counter);
		summaryDTO->setTournamentsPushed(0, GET_MAIN_CHARATER()._log_tournament_pushed_counter);
		summaryDTO->setLivesSent(0, GET_MAIN_CHARATER()._log_lives_sent);
		summaryDTO->setAbaddonProcessing(0, GET_MAIN_CHARATER()._log_abaddon_won_counter);
		summaryDTO->setXmasProcessing(0, GET_MAIN_CHARATER()._log_xmas_processing_counter);
		summaryDTO->setFriendsCount(0, GET_MAIN_CHARATER()._log_friend_counter);
		summaryDTO->setBagOfVacinePack(0, GET_MAIN_CHARATER()._log_packetIAP[0]);
		summaryDTO->setCrateOfVacinePack(0, GET_MAIN_CHARATER()._log_packetIAP[1]);
		summaryDTO->setStockpileOfVacinePack(0, GET_MAIN_CHARATER()._log_packetIAP[2]);
		summaryDTO->setTruckLoadOfVacinePack(0, GET_MAIN_CHARATER()._log_packetIAP[3]);
		summaryDTO->setVacinePackReserve01(0, GET_MAIN_CHARATER()._log_packetIAP[4]);
		summaryDTO->setVacinePackReserve02(0, GET_MAIN_CHARATER()._log_packetIAP[5]);
		summaryDTO->setBagOfFoodPack(0, GET_MAIN_CHARATER()._log_packetIAP[6]);
		summaryDTO->setCrateOfFoodPack(0, GET_MAIN_CHARATER()._log_packetIAP[7]);
		summaryDTO->setStockpileOfFoodPack(0, GET_MAIN_CHARATER()._log_packetIAP[8]);
		summaryDTO->setTruckLoadOfFoodPack(0, GET_MAIN_CHARATER()._log_packetIAP[9]);
		summaryDTO->setFoodPackReserve01(0, GET_MAIN_CHARATER()._log_packetIAP[10]);
		summaryDTO->setFoodPackReserve02(0, GET_MAIN_CHARATER()._log_packetIAP[11]);

		StreamBuffer^ buf = summaryDTO->CreateStream();

		GAME()->_svhandle_sendUserLog.Handle = ServerStreamManager::Request(SERVER_REQUEST_SEND_USER_LOG, buf->Data);
		GAME()->_svhandle_sendUserLog.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (GAME()->_svhandle_sendUserLog.Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(GAME()->_svhandle_sendUserLog.Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_sendUserLog.Handle);

			if (stream->ErrorCode == 0)//complete
			{
				GAME()->_svhandle_sendUserLog.Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else if (stream->ErrorCode == 1) //no record
			{
				GAME()->_svhandle_sendUserLog.Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}

			
			ServerStreamManager::freeStream(GAME()->_svhandle_sendUserLog.Handle);
			GAME()->_svhandle_sendUserLog.Handle = -1;
		}
		else
		{
			GAME()->_svhandle_sendUserLog.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}
extern "C" int getRequestList()
{
	int ret = SERVER_DATA_ERROR_NO_NETWORK;

	GAME()->_svhandle_getRequestList.reset();
	
	while (true)
	{
		if (GAME()->_svhandle_getRequestList.Handle == -1 && GAME()->_svhandle_getRequestList.Status == SVRESULT::SV_DATA_FREE)
		{
			PCLSentRequestDto^ sendRequestPCL = ref new PCLSentRequestDto();
			sendRequestPCL->InitData(1);
			sendRequestPCL->setID(0, GET_MAIN_CHARATER()._player_id);
			sendRequestPCL->setGameId(0, GET_REQUEST_MANAGER()._gameID);

			StreamBuffer^ buf = sendRequestPCL->CreateStream();

			GAME()->_svhandle_getRequestList.Handle = ServerStreamManager::Request(SERVER_REQUEST_GET_REQUEST_LIST, buf->Data);
			GAME()->_svhandle_getRequestList.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
		else if (GAME()->_svhandle_getRequestList.Handle > -1)
		{
			if(ServerStreamManager::isStreamComplete(GAME()->_svhandle_getRequestList.Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_getRequestList.Handle);
				
				if (stream->ErrorCode == 0)//complete
				{
					PCLSentRequestDto^ sendRequestPCL = ref new PCLSentRequestDto();
					sendRequestPCL->setFromStream(ref new StreamBuffer(stream->ResponseData));

					for (int i = 0; i < sendRequestPCL->_countData; i++)
					{
						GET_REQUEST_MANAGER().AddRequest(sendRequestPCL->Sender[i] , sendRequestPCL->RequestType[i]);
					}
					
					ret = SERVER_DATA_SUCCESS;
				}
				else if (stream->ErrorCode == 1) //no record
				{
					ret = SERVER_DATA_ERROR_NO_RECORD;
				}

				GAME()->_svhandle_getRequestList.Status = SVRESULT::SV_DATA_SUCCESS;
				ServerStreamManager::freeStream(GAME()->_svhandle_getRequestList.Handle);
				GAME()->_svhandle_getRequestList.Handle = -1;
			}
			else
			{
				GAME()->_svhandle_getRequestList.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
			}
		}

		if(GAME()->_svhandle_getRequestList.Status != SVRESULT::SV_DATA_WAITING_RESPONSE)
		{
			break;
		} 
		else
		{
			GAME()->Sleep(FPS_DELAY_TIME);
		}
	}

	return ret;
}

extern "C" void SVS_getRequestList()
{
	if (GAME()->_svhandle_getRequestList.Handle == -1 && GAME()->_svhandle_getRequestList.Status == SVRESULT::SV_DATA_FREE)
	{
		PCLSentRequestDto^ sendRequestPCL = ref new PCLSentRequestDto();
		sendRequestPCL->InitData(1);
		sendRequestPCL->setID(0, GET_MAIN_CHARATER()._player_id);
		sendRequestPCL->setGameId(0, GET_REQUEST_MANAGER()._gameID);

		StreamBuffer^ buf = sendRequestPCL->CreateStream();

		GAME()->_svhandle_getRequestList.Handle = ServerStreamManager::Request(SERVER_REQUEST_GET_REQUEST_LIST, buf->Data);
		GAME()->_svhandle_getRequestList.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (GAME()->_svhandle_getRequestList.Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(GAME()->_svhandle_getRequestList.Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_getRequestList.Handle);

			if (stream->ErrorCode == 0)//complete
			{
				PCLSentRequestDto^ sendRequestPCL = ref new PCLSentRequestDto();
				sendRequestPCL->setFromStream(ref new StreamBuffer(stream->ResponseData));

				for (int i = 0; i < sendRequestPCL->_countData; i++)
				{
					GET_REQUEST_MANAGER().AddRequest(sendRequestPCL->Sender[i], sendRequestPCL->RequestType[i]);
				}

				GAME()->_svhandle_getRequestList.Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else if (stream->ErrorCode == 1) //no record
			{
				GAME()->_svhandle_getRequestList.Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}

			GAME()->_svhandle_getRequestList.Status = SVRESULT::SV_DATA_SUCCESS;
			ServerStreamManager::freeStream(GAME()->_svhandle_getRequestList.Handle);
			GAME()->_svhandle_getRequestList.Handle = -1;
		}
		else
		{
			GAME()->_svhandle_getRequestList.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}

extern "C" int SendARequest(INT_TIME sender, INT_TIME receiver, int typeRequest)
{
	int ret = SERVER_DATA_ERROR_NO_NETWORK;
	GAME()->_svhandle_SendARequest.reset();
	
	while (true)
	{
		if (GAME()->_svhandle_SendARequest.Handle == -1 && GAME()->_svhandle_SendARequest.Status == SVRESULT::SV_DATA_FREE)
		{
			PCLSentRequestDto^ sendRequestDTO = ref new PCLSentRequestDto();
			sendRequestDTO->InitData(1);
			
			sendRequestDTO->setID(0, GET_MAIN_CHARATER()._player_id);
			sendRequestDTO->setGameId(0, GET_REQUEST_MANAGER()._gameID);
			sendRequestDTO->setSender(0, sender);
			sendRequestDTO->setReceiver(0, receiver);
			sendRequestDTO->setRequestType(0, typeRequest);

			StreamBuffer^ buf = sendRequestDTO->CreateStream();

			GAME()->_svhandle_SendARequest.Handle = ServerStreamManager::Request(SERVER_REQUEST_SEND_A_REQUEST, buf->Data);
			GAME()->_svhandle_SendARequest.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
		else if (GAME()->_svhandle_SendARequest.Handle > -1)
		{
			if(ServerStreamManager::isStreamComplete(GAME()->_svhandle_SendARequest.Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(GAME()->_svhandle_SendARequest.Handle);
				
				if (stream->ErrorCode == 0)//complete
				{
					ret = SERVER_DATA_SUCCESS;
				}
				else if (stream->ErrorCode == 1) //no record
				{
					ret = SERVER_DATA_ERROR_NO_RECORD;
				}

				GAME()->_svhandle_SendARequest.Status = SVRESULT::SV_DATA_SUCCESS;
				ServerStreamManager::freeStream(GAME()->_svhandle_SendARequest.Handle);
				GAME()->_svhandle_SendARequest.Handle = -1;
			}
			else
			{
				GAME()->_svhandle_SendARequest.Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
			}
		}

		if(GAME()->_svhandle_SendARequest.Status != SVRESULT::SV_DATA_WAITING_RESPONSE)
		{
			break;
		} 
		else
		{
			GAME()->Sleep(FPS_DELAY_TIME);
		}
	}
	
	return ret;
}
extern "C" void SVS_synAvatar(SVHANDLE* handle)
{
	//get image of me and upload it to facebook.
	if (handle->Handle == -1 && handle->Status == SVRESULT::SV_DATA_FREE)
	{
		handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_AVATAR_SYN, nullptr);
	}
	else if (handle->Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(handle->Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);

			if (stream->ErrorCode == 0)//complete
			{
				handle->Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else if (stream->ErrorCode == 1) //no record
			{
				handle->Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			else{
				handle->Status = SVRESULT::SV_DATA_ERROR_UNKNOW;
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1;
		}
		else
		{
			handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}
extern "C" void SVS_getImageFromUrl(SVHANDLE * handle){
	//get Image From Server and save to document
	if (handle->Handle == -1 && handle->Status == SVRESULT::SV_DATA_FREE)
	{
		int bufsize = 2;
		Platform::String^ url = Utils::ConvertCharToString(GET_MAIN_CHARATER()._avatar_url);
		if (url->Length() > 0){
			bufsize += url->Length();
		}
		StreamBuffer^ buf = ref new StreamBuffer(bufsize);
		buf->writeSStr(url);
		handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_AVATAR_DOWNLOAD, buf->Data);
	}
	else if (handle->Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(handle->Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);

			if (stream->ErrorCode == 0)//complete
			{
				handle->Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else if (stream->ErrorCode == 1) //no record
			{
				handle->Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1;
		}
		else
		{
			handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}

}

extern "C" void SVS_FBLoadFriend(SVHANDLE* handle){

	if (handle->Handle == -1)
	{
		handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_GET_FRIEND, nullptr);
		GET_FRIEND_MANAGER()._currentListFriendFBCount = 0;
	}
	else if (handle->Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(handle->Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);
			if (!ServerStreamManager::isError(handle->Handle))
			{
				StreamBuffer^ buf = ref new StreamBuffer(stream->ResponseData);

				GET_FRIEND_MANAGER()._currentListFriendFBCount = buf->readInt32();
				//printf("current _listfriend fb count:%d\n", GET_FRIEND_MANAGER()._currentListFriendFBCount);
				if (GET_FRIEND_MANAGER()._currentListFriendFBCount > 0)
				{
					GET_FRIEND_MANAGER()._listFriendFBID = new INT64[GET_FRIEND_MANAGER()._currentListFriendFBCount];
					for (int i = 0; i < GET_FRIEND_MANAGER()._currentListFriendFBCount; i++)
					{
						GET_FRIEND_MANAGER()._listFriendFBID[i] = 3000000000000000000 + buf->readInt64();
					}
				}
				handle->Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else
			{
				handle->Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1;
		}
		else
		{
			handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}

extern "C" LoadAvatar WP8_calcAvatarUrl(int friendIndex, int typeLoadAvatar)
{
	LoadAvatar ret;
	ret.FriendIndex = friendIndex;
	ret.TypeOfAvatar = typeLoadAvatar;

	char _fileAvatar[30];
	CBattleChallenge::ChallengeDetail *user = NULL;

	switch (typeLoadAvatar)
	{
	case FRIEND_SERVER_STATE_LOAD_BATTLE_RANDOM_AVATAR:
		user = &GET_BATTLE_CHALLENGE_VAR()._user[friendIndex];
		break;
	case FRIEND_SERVER_STATE_LOAD_TOP_USER_RANDOM_AVATAR:
		user = &GET_BATTLE_CHALLENGE_VAR()._topUser[friendIndex];
		break;
	}

	switch (typeLoadAvatar)
	{
	case CHALLENGE_BATTLE_TYPE_AVATAR_LOADING_NONE:
		sprintf(_fileAvatar, "A%lld", GET_FRIEND_MANAGER()._friend[friendIndex]._id);
		break;
	case FRIEND_SERVER_STATE_LOAD_BATTLE_RANDOM_AVATAR:
	case FRIEND_SERVER_STATE_LOAD_TOP_USER_RANDOM_AVATAR:
		sprintf(_fileAvatar, "A%lld", user->_id);
		break;
	}



	//printf("\n _fileAvatar = %s ", _fileAvatar);

	if (!GAME()->_bFlagNetworkAvailable)
	{
		//if (ExitsAvatar(_fileAvatar))
		if (true)
		{
			switch (typeLoadAvatar)
			{
			case CHALLENGE_BATTLE_TYPE_AVATAR_LOADING_NONE:
				user->_avartarState = FRIEND_AVARTAR_STATE_DOWNLOAD;
				break;
			case FRIEND_SERVER_STATE_LOAD_BATTLE_RANDOM_AVATAR:
			case FRIEND_SERVER_STATE_LOAD_TOP_USER_RANDOM_AVATAR:
				user->_avartarState = FRIEND_AVARTAR_STATE_DOWNLOAD;
				break;
			}
		}
		else
		{
			switch (typeLoadAvatar)
			{
			case CHALLENGE_BATTLE_TYPE_AVATAR_LOADING_NONE:
				GET_FRIEND_MANAGER()._friend[friendIndex]._avartarState = FRIEND_AVARTAR_STATE_NOT_ON_SERVER;
				break;
			case FRIEND_SERVER_STATE_LOAD_BATTLE_RANDOM_AVATAR:
			case FRIEND_SERVER_STATE_LOAD_TOP_USER_RANDOM_AVATAR:
				user->_avartarState = FRIEND_AVARTAR_STATE_NOT_ON_SERVER;
				break;
			}
		}
		ret.IsValid = 0;
		//no need load
		return ret;
	}
	
	switch (typeLoadAvatar)
	{
		case CHALLENGE_BATTLE_TYPE_AVATAR_LOADING_NONE:
		{
			if (GET_FRIEND_MANAGER()._friend[friendIndex]._id == GET_MAIN_CHARATER()._player_id)
			{
				GET_FRIEND_MANAGER()._friend[friendIndex]._avartarState = FRIEND_AVARTAR_STATE_DOWNLOAD;
			}
			else
			{
				sprintf(ret.AvatarUrl, "%s", GET_FRIEND_MANAGER()._friend[friendIndex]._avartarURL);
				sprintf(ret.FileName, "%s", _fileAvatar);
				ret.IsValid = 1;
				/*int statusCode = DownloadAvatar(GET_FRIEND_MANAGER()._friend[friendIndex]._avartarURL, _fileAvatar);
				if (statusCode == 200 || statusCode == 304)
					GET_FRIEND_MANAGER()._friend[friendIndex]._avartarState = FRIEND_AVARTAR_STATE_DOWNLOAD;
				else
					GET_FRIEND_MANAGER()._friend[friendIndex]._avartarState = FRIEND_AVARTAR_STATE_NOT_ON_SERVER;*/
			}
		}
		break;
		case FRIEND_SERVER_STATE_LOAD_BATTLE_RANDOM_AVATAR:
		case FRIEND_SERVER_STATE_LOAD_TOP_USER_RANDOM_AVATAR:
		{
			if (user->_id == GET_MAIN_CHARATER()._player_id)
			{
				user->_avartarState = FRIEND_AVARTAR_STATE_DOWNLOAD;
			}
			else
			{
				sprintf(ret.AvatarUrl, "%s", user->_avartarURL);
				sprintf(ret.FileName, "%s", _fileAvatar);
				ret.IsValid = 1;
				/*int statusCode = DownloadAvatar(user->_avartarURL, _fileAvatar);
				if (statusCode == 200 || statusCode == 304)
					user->_avartarState = FRIEND_AVARTAR_STATE_DOWNLOAD;
				else
					user->_avartarState = FRIEND_AVARTAR_STATE_NOT_ON_SERVER;*/
			}
		}
		break;
	}
	return ret;
}

extern "C" void SVS_downloadAvatar(SVHANDLE* handle, LoadAvatar info)
{

	if (handle->Handle == -1 && handle->Status == SVRESULT::SV_DATA_FREE)
	{
		Platform::String^ url = Utils::ConvertCharToString(info.AvatarUrl);
		Platform::String^ fname = Utils::ConvertCharToString(info.FileName);
		if (url != nullptr && fname != nullptr){
			int size = 2 + 2*url->Length() + 2 + 2*fname->Length();
			StreamBuffer^ buf = ref new StreamBuffer(size);
			buf->writeSStr(url);
			buf->writeSStr(fname);
			handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
			handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_AVATAR_DOWNLOAD, buf->Data);
		}
		else{
			handle->Status = SVRESULT::SV_DATA_ERROR_UNKNOW;
		}
	}
	else if (handle->Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(handle->Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);
			if (!ServerStreamManager::isError(handle->Handle))
			{
				handle->Status = SVRESULT::SV_DATA_SUCCESS;
				handle->StatusCode = stream->ErrorCode;
			}
			else
			{
				handle->Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1;
		}
		else{
			handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}
#if defined(BUILD_SUPPORT)
char* _support_userid = NULL;
extern "C" const char* GEX_getSupportUserId(){
	if (_support_userid != NULL){
		delete[] _support_userid;
		_support_userid = NULL;
	}
	if (PhoneDirect3DXamlAppComponent::PlayerInfomation::UserId != nullptr){
		//ConvertStringToCharArray return a non const char* 
		//we must delete after use.
		_support_userid = (char*)Utils::ConvertStringToCharArray(PhoneDirect3DXamlAppComponent::PlayerInfomation::UserId);
		return (const char*)_support_userid;
	}
	return NULL;
}
extern "C" void SVS_OpenSupportPage(SVHANDLE* handle){
	if (handle->Handle == -1 && handle->Status == SVRESULT::SV_DATA_FREE)
	{
			handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
			handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_SUPPORT_OPEN_PAGE, nullptr);
	}
	else if (handle->Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(handle->Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);
			if (!ServerStreamManager::isError(handle->Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);
				StreamBuffer^ buff = ref new StreamBuffer(stream->ResponseData);
				int addmoney = buff->readInt32();
				int addvacxin = buff->readInt32();
				int addchampion = buff->readInt32();
				GET_MAIN_CHARATER().AddFood(addmoney);
				GET_MAIN_CHARATER().AddVaccine(addvacxin);
				GET_MAIN_CHARATER()._champion_score += addchampion;
				handle->Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else
			{
				handle->Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1;
		}
		else{
			handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}
extern "C" void SVS_SupportSave(SVHANDLE* handle)
{
	if (handle->Handle == -1 && handle->Status == SVRESULT::SV_DATA_FREE)
	{
		PCLSaveGameDto^ saveDTO = ref new PCLSaveGameDto();
		saveDTO->InitData(1);

		Platform::String^ userName = Utils::ConvertCharToString(GET_MAIN_CHARATER()._player_name);
		Platform::String^ token = nullptr; //Utils::ConvertCharToString(GET_MAIN_CHARATER()._token);
		Platform::String^ nationCode = Utils::ConvertCharToString(CGame_Design::NationCode_string[GET_MAIN_CHARATER()._nationCodeIndex * TOTAL_NATIONCODE_STRING_ATRIBUTE + NATIONCODE_STRING_ATRIUBTE_ISO_CODE_1]);

		saveDTO->setID(0, GET_MAIN_CHARATER()._player_id);
		saveDTO->setUserName(0, userName);
		saveDTO->setFacebookId(0, GET_MAIN_CHARATER()._player_id);
		saveDTO->setToken(0, token);
		saveDTO->setLevelMain(0, GET_MAIN_CHARATER()._main_camp._main_lvl);
		saveDTO->setLevelCurrentWorld(0, GET_MAIN_CHARATER()._current_unlock_map);
		saveDTO->setChooseCharater(0, GET_MAIN_CHARATER()._gender);
		saveDTO->setRunHighScore(0, GET_MAIN_CHARATER()._run_high_score);
		saveDTO->setRevision(0, GET_MAIN_CHARATER()._revision);
		saveDTO->setNationCode(0, nationCode);
		saveDTO->setChampionScore(0, GET_MAIN_CHARATER()._champion_score);

		Platform::Array<uint8>^ data = ref new Platform::Array<uint8>(10000);
		for (int k = 0; k < 10000; k++)
		{
			data[k] = GET_MAIN_CHARATER()._save_buffer[k];
		}
		saveDTO->setData(0, data);

		/*int offset = 0;//i * MAX_SAVE_BUFFER_CACHE;
		for(int d = 0; d< MAX_SAVE_BUFFER_CACHE; d++){
		saveDTO->Data[offset + d] = GET_MAIN_CHARATER()._save_buffer[d];
		}*/

		saveDTO->setLastRunHighScore(0, 0);
		saveDTO->setRank(0, 0);
		saveDTO->setLastRank(0, 0);
		saveDTO->setDataSchemaVersion(0, GET_MAIN_CHARATER()._version);

		StreamBuffer^ buf = saveDTO->CreateStream();

		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_SYN_SAVE_TO_SERVER, buf->Data);
		handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (handle->Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(handle->Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);

			if (stream->ErrorCode == 0)//complete
			{
				GET_MAIN_CHARATER()._last_syn_revision = GET_MAIN_CHARATER()._revision;
				handle->Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else if (stream->ErrorCode == 1) //no record
			{
				handle->Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1;
		}
		else
		{
			handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}
#endif