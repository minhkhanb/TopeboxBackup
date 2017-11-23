#if !defined (_H_PINE_ENGINE_WRAPPER_SERVICE_SHELL)
#define _H_PINE_ENGINE_WRAPPER_SERVICE_SHELL

#include "common.h"
#include "wrapper_helper.h"
#include <string>
#include "ServiceShell/CCryptoRequest.h"
#include "ServiceShell/CHttpRequest.h"
#include "ServiceShell/CCloudKeyValueRequest.h"

using namespace Pine::ServiceShell;
using namespace Pine::ServiceShell::Cloud::KeyValueService;

extern "C"
{
	//shell security
	WrapperHandle PineShellBasicAuthorizeV1Open(CallbackCryptoPtr callback_ptr);
	int PineShellBasicAuthorizeV1GetPass(WrapperHandle handle, const char* user_id);
	int PineShellBasicAuthorizeV1SimpleEncrypt(WrapperHandle handle, const char* data);
	void PineShellBasicAuthorizeV1Close(WrapperHandle handle);

	//shell http request
	WrapperHandle PineShellHttpRequestOpen(const char* url, CallbackHttpRequestPtr callback_ptr);
	void PineShellHttpRequestSetHeader(WrapperHandle handle, const char* key, const char* value);
	void PineShellHttpRequestSetStringBody(WrapperHandle handle, const char* data);
	int PineShellHttpRequestSend(WrapperHandle handle);
	void PineShellHttpRequestClose(WrapperHandle handle);

	//shell cloud key value
	WrapperHandle PineShellCloudKeyValueRegService
	(
		int cloud_provider, 
		CallbackCloudKeyValueChangedPtr callback_changed_ptr, 
		CallbackCloudKeyValueWritePtr callback_write,
		CallbackCloudKeyValueReadPtr callback_read
	);

	int PineShellCloudKeyValueWrite(WrapperHandle handle, const char* key, const char* data_buf, int data_len);
	int PineShellCloudKeyValueRead(WrapperHandle handle, const char* key);
}

#endif//_H_PINE_ENGINE_WRAPPER_SERVICE_SHELL
