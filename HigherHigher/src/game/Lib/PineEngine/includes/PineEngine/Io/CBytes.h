#ifndef _H_PINE_ENGINE_PLATFORM_CBYTES
#define _H_PINE_ENGINE_PLATFORM_CBYTES
#include <cstdint>
#include <utility>
#include <cstring>
#include "../common.h"
#include "../Os/OS.h"

#ifdef PE_BUILD_ANDROID
#define MEMCPY(des,src, len) {uint8_t* tmp_src = (uint8_t*)(src); uint8_t* tmp_des = (uint8_t*)(des); for(int i=0;i<len;i++)tmp_des[i]=tmp_src[i];}
#else
#define MEMCPY std::memcpy
#endif

using namespace Pine::Os;

namespace Pine
{
    namespace Io
    {
        class PE_EXPORT CBytes
        {
		public:
            enum class PE_EXPORT Type 
			{
				UNKNOWN = 0,
				ABSTRACT = 1,
				LOCAL = 2
			};
        protected:
            uint8_t* _data;
            size_t _data_len;
            Type _type;
        public:
            inline uint8_t* data(){return _data;};
            inline size_t length(){return _data_len;};
            inline Type getType(){return _type;};
            
            CBytes(const uint8_t* data, size_t len) :_data(NULL),_data_len(0)
            {
                init(data, len);
            };

            CBytes(size_t len) :_data(NULL),_data_len(0)
            {
                init(len);
            };

            CBytes(const CBytes& buffer) :_data(NULL), _data_len(0)
            {
                init(buffer._data, buffer._data_len);
            };

            CBytes& operator=(const CBytes& buff)
            {
                if (this != &buff)
                {
                    if(buff._type == Type::ABSTRACT)
                    {
                        bind(buff._data, buff._data_len);
                    }
					else
					{
                        init(buff._data, buff._data_len);
                    }
                }
                return *this;
            };

            CBytes& operator=(CBytes&& buffer)
            {
                release();
                _data = std::move(buffer._data);
                _data_len = std::move(buffer._data_len);
                _type = std::move(buffer._type);
                return *this;
            };

            CBytes() :_data(NULL),_data_len(0), _type(Type::UNKNOWN)
            {
            };
            
            virtual void release()
            {
                if(_type != Type::ABSTRACT)
                {
                    if(_data != NULL) {delete[] _data; }
                }
                _type = Type::UNKNOWN;
                _data = NULL;
                _data_len = 0;
            };

            virtual void init(size_t len)
            {
                release();
				if (len > 0) 
				{
					_data = new uint8_t[len + 1];
					_data[len] = '\0';
					_data_len = len;
					_type = Type::LOCAL;
				}
            };

            virtual void init(const uint8_t* data, size_t len)
            {
				if (len > 0) 
				{
					init(len);
					MEMCPY(_data, data, len);
					_type = Type::LOCAL;
				}
				else 
				{
					release();
				}
            };

            virtual void bind(uint8_t* data, size_t len)
            {
                release();
                _type = Type::ABSTRACT;
                _data = data;
                _data_len = len;
            };

            virtual ~CBytes()
            {
                release(); 
            };

            void debug()
            {
                Pine::Os::OS::writeConsole("begin debug:");
                for(size_t i = 0; i< _data_len; i++)
                {
                    Pine::Os::OS::writeConsole("i:%d value:%d", i, _data[i]);
                }
                Pine::Os::OS::writeConsole("end --");
            };

            friend class CBuffer;
        };
   }
}

#endif//_H_PINE_ENGINE_PLATFORM_CBYTES