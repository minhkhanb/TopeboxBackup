#pragma once
#ifndef _H_PINE_ENGINE_SERVICE_SHELL_CCLOUD_KEYVALUE_REQUEST
#define _H_PINE_ENGINE_SERVICE_SHELL_CCLOUD_KEYVALUE_REQUEST

#include "../common.h"
#include "../Io/Interface/IStorable.h"
#include "../Cloud/ECloudProvider.h"
#include "CRequestStream.h"
#include <string>


namespace Pine
{
	namespace ServiceShell
	{
		namespace Cloud 
		{
			namespace KeyValueService
			{
				using namespace Pine::Platform;
				using namespace Pine::Io::Interface;
				using namespace Pine::Io;
				using namespace Pine::Cloud;

				typedef void(*CallbackCloudKeyValueWritePtr)(int id, bool status);
				typedef void(*CallbackCloudKeyValueChangedPtr)(const char** data_buf, int data_len);
				typedef void(*CallbackCloudKeyValueReadPtr)(int id, const char** data_buf, int data_len);

				typedef std::function<void(int id, bool status)> CallbackCloudKeyValueWrite;
				typedef std::function<void(const char** data_buf, int data_len)>  CallbackCloudKeyValueChanged;
				typedef std::function<void(int id, const char** data_buff, int data_len)> CallbackCloudKeyValueRead;

				class PE_EXPORT CWriteRequest : virtual public IStorable
				{
				protected:
					CBytes _key;
					CBytes _data;
					ECloudProvider _provider;
					uint8_t _status;

				public:
					void setKey(std::string key);
					void setData(CBytes data);
					void setProvider(ECloudProvider provider);

					virtual size_t getBufferLength();
					virtual void writeToBuffer(CBuffer* buffer);
					virtual void readFromBuffer(CBuffer* buffer);
					virtual void updateFromBuffer(CBuffer* buffer);	
				};

				typedef CRequestStream<CWriteRequest, PINE_SS_ID_CLOUD_WRITE, CallbackCloudKeyValueWrite> CWriteRequestStream;

				class PE_EXPORT CReadRequest : virtual public IStorable
				{
				protected:
					ECloudProvider _provider;
					CBytes _key;
					uint8_t* _data_readed;
				public:
					CReadRequest();
					virtual ~CReadRequest();
					void setProvider(ECloudProvider provider);
					void setKey(std::string key);

					virtual size_t getBufferLength();
					virtual void writeToBuffer(CBuffer* buffer);
					virtual void readFromBuffer(CBuffer* buffer);
					virtual void updateFromBuffer(CBuffer* buffer);

				};
				typedef CRequestStream<CReadRequest, PINE_SS_ID_CLOUD_READ, CallbackCloudKeyValueRead> CReadRequestStream;
			}
		}
	}
}

#endif//_H_PINE_ENGINE_SERVICE_SHELL_CCLOUD_KEYVALUE_REQUEST