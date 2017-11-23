#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <memory>
#include <functional>

using std::string;
using std::cout;
using std::endl;
using std::function;

using namespace std;
using namespace Pine::Platform;
using namespace Pine::Io::Interface;
using namespace Pine::Io;
using namespace Pine::ServiceShell;

TopeBox_PromoGame::TopeBox_PromoGame()
{
	
}

TopeBox_PromoGame::~TopeBox_PromoGame()
{

}

#pragma region GamePromoFunction
void TopeBox_PromoGame::ResetGamePromoInfo()
{
	_is_ready = false;
	_renderx = 0;
	_rendery = 0;
	_touch_id = -1;
	_has_download = false;
	sprintf(_device_id, "%s" ,"deviceID");
	sprintf(_game_id,"%s", "gameID");
	sprintf(_link_store,"%s", "linkStore");
	_is_need_reload = false;
	_icon_scale_state = 0;
	_icon_scale_time = 1.5f*FPS_MAX;
	_icon_scaleEff_speed = 0.02f;
	_icon_scaleEff = 0;
	_isRedirect = false;
	_reGetTopeboxInfo = false;
}
void TopeBox_PromoGame::InitGamePromo(float x, float y, int w, int h)
{
	_is_ready = false;
	_renderx = 0;
	_rendery = 0;
	_touch_id = -1;
	_icon_w = w;
	_icon_h = h;
	_has_download = false;
	sprintf(_device_id, "%s", "deviceID");
	sprintf(_game_id, "%s", "gameID");
	sprintf(_link_store, "%s", "linkStore");
	_is_need_reload = false;
	_icon_scale_state = 0;
	_icon_scale_time = 1.5f*FPS_MAX;
	_icon_scaleEff_speed = 0.02f;
	_icon_scaleEff = 0;
	_isRedirect = false;
	_reGetTopeboxInfo = false;
}
void TopeBox_PromoGame::RenderGamePromo(float x, float y)
{
	HSGame *game = (HSGame*)GAME()->_current_game;
	if (_is_need_reload)
	{
		SaveImageToLocal();
		_is_need_reload = false;
	}

	if (_reGetTopeboxInfo)
	{
		_reGetTopeboxInfo = false;
		GetTopeboxPromoGameInfo(game->_user._game_promo_revision, game->_user._game_promo_order);
	}

	if (!_is_ready)
	{
		return;
	}
	//TDGame* game = (TDGame*)(GAME()->_main_game);
	_renderx = x;
	_rendery = y;
	
	float scaleEff = 1.0f;
	if (_touch_id != -1)
	{
		scaleEff = 1.1f;
	}
	else
	{
		switch (_icon_scale_state)
		{
		case 0://normal
			_icon_scale_time--;
			if (_icon_scale_time <= 0)
			{
				_icon_scale_state = 1;
				_icon_scaleEff_speed = 0.02f;
			}
			break;
		case 1://zoom in
			_icon_scaleEff += _icon_scaleEff_speed;
			if (_icon_scaleEff >= 0.2f)
			{
				_icon_scaleEff = 0.2f;
				_icon_scale_state = 2;
				_icon_scaleEff_speed = 0.0075f;
			}
			break;
		case 2:// zoom out
			_icon_scaleEff += -_icon_scaleEff_speed;
			_icon_scaleEff_speed *= 1.1f;
			if (_icon_scaleEff <= 0)
			{
				_icon_scaleEff = 0;
				_icon_scale_state = 0;
				_icon_scale_time = 2.0f * FPS_MAX;
			}
			break;
		default:
			break;
		}
	}
	G()->SetScale((scaleEff + _icon_scaleEff), (scaleEff + _icon_scaleEff), _renderx + _icon_w / 2, _rendery + _icon_h / 2);
	_game_icon.DrawImageWithRect(G(), _renderx , _rendery );
	G()->ClearScale();
	//rem sprite
	GET_SPRITE(SPRITEID_FONT_TITLE_01)->_char_spacing = 2;
	//G()->SetScale(0.7f, 0.7f, _renderx - 5, _rendery + _icon_h -5);
	GET_SPRITE(SPRITEID_FONT_TITLE_01)->DrawAnsiTextFormat(G(), _renderx -8, _rendery + _icon_h*1.2f, _LEFT|_VCENTER, "%s", _des);
	//G()->ClearScale();
}
bool TopeBox_PromoGame::UpdateTouchGamePromo()
{
	bool ret = false;
	if (!_is_ready)
	{
		return ret;
	}

	for (int i = GAME()->_num_touches-1; i >= 0; i--)
	{
		if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, _renderx, _rendery, _icon_w, _icon_h))
		{
			if (TOUCHS(i)->_state == TOUCH_DOWN)
			{
				if (_touch_id == -1 && !_has_download)
				{
					_touch_id = TOUCHS(i)->_id;
					PLAY_SFX(SFX_BUTTON);
				}
			}
			else if (TOUCHS(i)->_state == TOUCH_DRAG)
			{

			}
			else if (TOUCHS(i)->_state == TOUCH_UP)
			{
				if (_touch_id == TOUCHS(i)->_id)
				{
					_touch_id = -1;
					ret = true;
					_has_download = true;
					
				}
			}
		}
		else
		{
			if (_touch_id == TOUCHS(i)->_id)
			{
				if (TOUCHS(i)->_state == TOUCH_UP)
				{
					_touch_id = -1;
					ret = true;
				}
			}
		}
		
	}
	if (_touch_id != -1)
	{
		ret = true;
	}
	return ret;
}
bool TopeBox_PromoGame::GamePromoIsReady()
{
	return _is_ready;
}

bool TopeBox_PromoGame::GamePromoHasTouchDown()
{
	return _has_download;
}
#pragma endregion

void function_cb(int requestID, int status, CBytes data)
{
	HSGame *game = (HSGame*)GAME()->_current_game;
	PDEBUG("\n ##### status: %d", status);
	PDEBUG("\n ##### requestID: %d", requestID);
	PDEBUG("\n ##### datalength: %d", data.length());
	PDEBUG("\n ##### data: %s", data.data());
	if (status == 200)
	{
		if (data.length() > 0)
		{
			char dataContent[1024];
			int lenght = sprintf(dataContent, "%s", data.data());
			game->_topebox_game.SetTopeBoxPromoGameInfo((string)dataContent);
		}
	}
	else if (status == 302)
	{
		string link("href=\"http:");
		char linkredirect[128];
		char dataContent[1024];
		int lenght = sprintf(dataContent, "%s", data.data());
		game->_topebox_game.GetSubString(dataContent, link, '/', linkredirect);
		sprintf(game->_topebox_game._linkRedirect, "http://%s/ads/v2/delivery", linkredirect);
		sprintf(game->_topebox_game._responseRedirect, "http://%s/ads/v1/response", linkredirect);
		
		PDEBUG("\n ##### link redirect: %s", game->_topebox_game._linkRedirect);
		game->_topebox_game._isRedirect = true;
		game->_topebox_game._reGetTopeboxInfo = true;
	}
}

void TopeBox_PromoGame::SetTopeBoxPromoGameInfo(string dataContent)
{
	HSGame *game = (HSGame*)GAME()->_current_game;
	char sv_order[64];
	string title("title"), linkstore("link"), des("description"), iconlink("icon"), gameid("id"), revision("revision"),total_order("capacity");
	GetSubString(dataContent, title, '"', _game_name);
	GetSubString(dataContent, linkstore, '"', _link_store);
	GetSubString(dataContent, iconlink, '"', _link_icon);
	GetSubString(dataContent, des, '"', _des);
	GetSubString(dataContent, gameid, '"', _game_id);
	GetSubString(dataContent, revision, '"', _revision);
	GetSubString(dataContent, total_order, '"', sv_order);

	PDEBUG("\n ##### _game_name: %s", _game_name);
	PDEBUG("\n ##### _link_store: %s", _link_store);
	PDEBUG("\n ##### _link_icon: %s", _link_icon);
	PDEBUG("\n ##### _game_id: %s", _game_id);
	PDEBUG("\n ##### _revision: %s", _revision);
	PDEBUG("\n ##### sv_order: %s", sv_order);
	//DownloadImageURL();
	int order = atoi(sv_order);
	int revi = atoi(_revision);
	if (revi != game->_user._game_promo_revision 
		|| strcmp(game->_user._game_promo_icon_id, _link_icon) != 0
		|| game->_user._game_promo_order >= order)
	{
		PDEBUG("\n ##### update GameInfo: %d", revi);
		game->_user._game_promo_revision = revi;
		game->_user._game_promo_has_show = true;
		game->_user._game_promo_order = 0;
		sprintf(game->_user._game_promo_icon_id, "%s", _link_icon);
		game->_user.SaveToBuffer();
		DownloadImageURL();
	}
	else
	{
		PDEBUG("\n ##### Load data cache");
		_is_need_reload = true;
	}
}


void TopeBox_PromoGame::GetSubString(string str, string substr, char c_cut, char*str_output)
{
	int c_index = str.find(substr) + substr.length() + 2;
	int count = 0;
	for (int i = c_index; i < str.length(); i++)
	{
		if (str[i] == '"')
		{
			continue;
		}
		if (str[i] != c_cut && str[i] != '}' && str[i] != ',')
		{
			str_output[count] = str[i];
			count++;
		}
		else
		{
			break;
		}
	}
	str_output[count] = '\0';
}

void function_get_img(int requestID, int status, CBytes data)
{
	HSGame *game = (HSGame*)GAME()->_current_game;
	PDEBUG("\n ##### img status: %d", status);
	PDEBUG("\n ##### img requestID: %d", requestID);
	PDEBUG("\n ##### img datalength: %d", data.length());
	PDEBUG("\n ##### img data: %s", data.data());
	if (status == 200)
	{
		if (data.length() > 0)
		{
			//TDGame* game = (TDGame*)(GAME()->_main_game);
			OS_DeleteAppData(GAME_ICON_FILE_NAME);
#if defined(MAC_OS) || defined(_WINDOWS_PHONE8 )|| defined(ANDROID_OS)
			
			OS_SaveAppData(GAME_ICON_FILE_NAME, (const char*)data.data(), data.length(), false);
			game->_topebox_game._is_need_reload = true;
#else
			FILE * stream = NULL;
			stream = fopen(GAME_ICON_FILE_NAME, "wb");
			if (stream != NULL)
			{
				size_t written = 0;// fwrite((void*)data.data(), sizeof(uint8_t), data.length(), stream);
				while (written < data.length())
				{
					written += fwrite((void*)(data.data() + written), sizeof(uint8_t), data.length(), stream);
				}
				fclose(stream);
				game->_topebox_game._is_need_reload = true;
			}
#endif
		}
	}
}

void TopeBox_PromoGame::DownloadImageURL()
{
	CallbackHttpRequest callback(function_get_img);

	shared_ptr<CHttpRequestStream> httpRequest = shared_ptr<CHttpRequestStream>(
		new CHttpRequestStream(callback)
		);
	PDEBUG("\n ##### _link_icon: %s", _link_icon);

	std::string icon_url(_link_icon);
	size_t pos = icon_url.find("https://");
	if (pos != std::string::npos)
	{
		icon_url = "http://" + icon_url.substr(8);
	}

	httpRequest->setUrl(icon_url);
	httpRequest->setMethod(Pine::ServiceShell::CHttpRequest::EHttpMethod::GET);
	CServiceShell::request(httpRequest);

}

void TopeBox_PromoGame::SaveImageToLocal()
{
	_game_icon.FreeTexture(G());
	//_game_icon.SetTextureMaxSize(_icon_w, _icon_h);
	if (_game_icon.CreateImageFromPNGinDocument(G(), GAME_ICON_FILE_NAME) > 0)
	{
		_icon_w = _game_icon.GetWidth();
		_icon_h = _game_icon.GetHeight();
		_is_ready = true;
		PDEBUG("\n ##### create IMG success \n");
	}
	else
	{
		DownloadImageURL();
		PDEBUG("\n ##### dowload icon \n");
	}
}


void function_response(int requestID, int status, CBytes data)
{
	PDEBUG("\n ##### response status: %d", status);
	PDEBUG("\n ##### response requestID: %d", requestID);
	PDEBUG("\n ##### response datalength: %d", data.length());
	PDEBUG("\n ##### response data: %s", data.data());
}

void TopeBox_PromoGame::ResponseRequest()
{
	HSGame *game = (HSGame*)GAME()->_current_game;
#if !defined (_WINDOWS)
	if (!Device::IsNetworkAvailable())
	{
		return;
	}
#endif

	GenerateSaltString();
	char inputString[256];
	if (strcmp(_device_id, "deviceID") == 0)
	{
		GenerateDeviceIDString();
	}
	sprintf(inputString, "device-id:%s.provider-id:%s.ads_id:%s.%s.%s", _device_id, PROVIDER_ID,  _game_id, _salt_str, PROVIDER_SECRET_KEY);
	string sha256 = CreateSHA256(inputString);
	for (int i = 0; i < sha256.length(); i++)
	{
		sha256[i] = toupper(sha256[i]);
	}
	char sign_str[128];
	sprintf(sign_str, "%s", sha256.c_str());
	sign_str[sha256.length()] = '\0';
	PDEBUG("\n ##### Get Game Info --- inputString:%s", inputString);
	CallbackHttpRequest callback(function_response);

	shared_ptr<CHttpRequestStream> httpRequest = shared_ptr<CHttpRequestStream>(
		new CHttpRequestStream(callback)
		);
	char dataContent[1024];

	sprintf(dataContent, "device-id=%s&provider-id=%s&salt=%s&sign=%s", _device_id, PROVIDER_ID, _salt_str, sign_str);
	PDEBUG("\n ##### dataContent: %s", dataContent);
	//httpRequest->setUrl(SERVER_TOPEBOX_URL_RESPONSE);
	if (!_isRedirect)
	{
		httpRequest->setUrl(SERVER_TOPEBOX_URL_RESPONSE);
	}
	else
	{
		httpRequest->setUrl(game->_topebox_game._responseRedirect);
	}
	httpRequest->setMethod(Pine::ServiceShell::CHttpRequest::EHttpMethod::POST);
	httpRequest->setHeader("Content-Type", "application/x-www-form-urlencoded");
	std::string data(dataContent);
	CBytes bytes((const uint8_t*)data.c_str(), data.length());

	httpRequest->setData(bytes);
	CServiceShell::request(httpRequest);
}

void TopeBox_PromoGame::CheckServerTopeboxPromo(int revision, int order)
{

}

void TopeBox_PromoGame::GetTopeboxPromoGameInfo(int revision, int order)
{
	HSGame *game = (HSGame*)GAME()->_current_game;
#if !defined (_WINDOWS)
	if (!Device::IsNetworkAvailable())
	{
		return;
	}
#endif
	GenerateSaltString();
	char inputString[256];
	if (strcmp(_device_id, "deviceID") == 0)
	{
		GenerateDeviceIDString();
	}
	//sprintf(_salt_str, "22c06e33-1ed8-41aa-b1ce-f6ee3d633697");
	sprintf(inputString, "provider-id:%s.device-id:%s.%s.%s", PROVIDER_ID, _device_id, _salt_str, PROVIDER_SECRET_KEY);
	string sha256 = CreateSHA256(inputString);
	for (int i = 0; i < sha256.length(); i++)
	{
		sha256[i] = toupper(sha256[i]);
	}
	char sign_str[128];
	sprintf(sign_str, "%s", sha256.c_str());
	sign_str[sha256.length()] = '\0';
	PDEBUG("\n ##### Get Game Info");
	CallbackHttpRequest callback (function_cb);

	shared_ptr<CHttpRequestStream> httpRequest = shared_ptr<CHttpRequestStream>( 
		new CHttpRequestStream(callback)
		);
	char dataContent[1024];
	
	sprintf(dataContent, "provider-id=%s&device-id=%s&salt=%s&sign=%s&revision=%d&order=%d", PROVIDER_ID, _device_id, _salt_str, sign_str,revision,order);
	PDEBUG("\n ##### dataContent: %s", dataContent);
	if (!_isRedirect)
	{
		httpRequest->setUrl(SERVER_TOPEBOX_URL);
	}
	else
	{
		httpRequest->setUrl(game->_topebox_game._linkRedirect);
	}
	
	httpRequest->setMethod(Pine::ServiceShell::CHttpRequest::EHttpMethod::POST);
	httpRequest->setHeader("Content-Type", "application/x-www-form-urlencoded");
	std::string data(dataContent);
	CBytes bytes((const uint8_t*)data.c_str(),data.length());

	httpRequest->setData(bytes);

	CServiceShell::request(httpRequest);
	
}

static const char alphanum[] =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

void TopeBox_PromoGame::GenerateSaltString()
{
	int count = 0;
	sprintf(_salt_str, "");
	while (count < 24)
	{
		int index = CMath::RANDOM(0, sizeof(alphanum) - 1);
		_salt_str[count] = alphanum[index];
		count++;
	}
	_salt_str[count] = '\0';
}

void TopeBox_PromoGame::GenerateDeviceIDString()
{
	HSGame *game = (HSGame*)GAME()->_current_game;
	int count = 0;
	sprintf(_device_id, "");
	while (count < 24)
	{
		int index = CMath::RANDOM(0, sizeof(alphanum) - 1);
		_device_id[count] = alphanum[index];
		count++;
	}
	_device_id[count] = '\0';
	sprintf(game->_user._game_promo_deviceID, "%s", _device_id);
}
void TopeBox_PromoGame::SetDeviceID(char* strID)
{
	sprintf(_device_id, "%s", strID);
}


const unsigned int TopeBox_PromoGame::SHA256::sha256_k[64] = //UL = uint32
{ 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

void TopeBox_PromoGame::SHA256::transform(const unsigned char *message, unsigned int block_nb)
{

	uint32 w[64];
	uint32 wv[8];
	uint32 t1, t2;
	const unsigned char *sub_block;
	int i;
	int j;
	for (i = 0; i < (int)block_nb; i++) {
		sub_block = message + (i << 6);
		for (j = 0; j < 16; j++) {
			SHA2_PACK32(&sub_block[j << 2], &w[j]);
		}
		for (j = 16; j < 64; j++) {
			w[j] = SHA256_F4(w[j - 2]) + w[j - 7] + SHA256_F3(w[j - 15]) + w[j - 16];
		}
		for (j = 0; j < 8; j++) {
			wv[j] = m_h[j];
		}
		for (j = 0; j < 64; j++) {
			t1 = wv[7] + SHA256_F2(wv[4]) + SHA2_CH(wv[4], wv[5], wv[6])
				+ sha256_k[j] + w[j];
			t2 = SHA256_F1(wv[0]) + SHA2_MAJ(wv[0], wv[1], wv[2]);
			wv[7] = wv[6];
			wv[6] = wv[5];
			wv[5] = wv[4];
			wv[4] = wv[3] + t1;
			wv[3] = wv[2];
			wv[2] = wv[1];
			wv[1] = wv[0];
			wv[0] = t1 + t2;
		}
		for (j = 0; j < 8; j++) {
			m_h[j] += wv[j];
		}
	}
}

void TopeBox_PromoGame::SHA256::init()
{
	m_h[0] = 0x6a09e667;
	m_h[1] = 0xbb67ae85;
	m_h[2] = 0x3c6ef372;
	m_h[3] = 0xa54ff53a;
	m_h[4] = 0x510e527f;
	m_h[5] = 0x9b05688c;
	m_h[6] = 0x1f83d9ab;
	m_h[7] = 0x5be0cd19;
	m_len = 0;
	m_tot_len = 0;
}

void TopeBox_PromoGame::SHA256::update(const unsigned char *message, unsigned int len)
{
	unsigned int block_nb;
	unsigned int new_len, rem_len, tmp_len;
	const unsigned char *shifted_message;
	tmp_len = SHA224_256_BLOCK_SIZE - m_len;
	rem_len = len < tmp_len ? len : tmp_len;
	memcpy(&m_block[m_len], message, rem_len);
	if (m_len + len < SHA224_256_BLOCK_SIZE) {
		m_len += len;
		return;
	}
	new_len = len - rem_len;
	block_nb = new_len / SHA224_256_BLOCK_SIZE;
	shifted_message = message + rem_len;
	transform(m_block, 1);
	transform(shifted_message, block_nb);
	rem_len = new_len % SHA224_256_BLOCK_SIZE;
	memcpy(m_block, &shifted_message[block_nb << 6], rem_len);
	m_len = rem_len;
	m_tot_len += (block_nb + 1) << 6;
}

void TopeBox_PromoGame::SHA256::final(unsigned char *digest)
{
	unsigned int block_nb;
	unsigned int pm_len;
	unsigned int len_b;
	int i;
	block_nb = (1 + ((SHA224_256_BLOCK_SIZE - 9)
		< (m_len % SHA224_256_BLOCK_SIZE)));
	len_b = (m_tot_len + m_len) << 3;
	pm_len = block_nb << 6;
	memset(m_block + m_len, 0, pm_len - m_len);
	m_block[m_len] = 0x80;
	SHA2_UNPACK32(len_b, m_block + pm_len - 4);
	transform(m_block, block_nb);
	for (i = 0; i < 8; i++) {
		SHA2_UNPACK32(m_h[i], &digest[i << 2]);
	}
}

std::string TopeBox_PromoGame::CreateSHA256(std::string input)
{
	unsigned char digest[SHA256::DIGEST_SIZE];
	memset(digest, 0, SHA256::DIGEST_SIZE);

	SHA256 ctx = SHA256();
	ctx.init();
	ctx.update((unsigned char*)input.c_str(), input.length());
	ctx.final(digest);

	char buf[2 * SHA256::DIGEST_SIZE + 1];
	buf[2 * SHA256::DIGEST_SIZE] = 0;
	for (int i = 0; i < SHA256::DIGEST_SIZE; i++)
		sprintf(buf + i * 2, "%02x", digest[i]);
	return std::string(buf);
}

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string TopeBox_PromoGame::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i <4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';

	}

	return ret;

}
std::string TopeBox_PromoGame::base64_decode(std::string const& encoded_string) {
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}