#if !defined(_H_PINE_ENGINE_STRING_CSTRING)
#define _H_PINE_ENGINE_STRING_CSTRING
#include "../common.h"
#include "CChar.h"
#include <string.h>
#include <vector>
#include "CDataBinding.h"
#include "EEncoding.h"

namespace Pine {
	namespace String {

		class PE_EXPORT CString
		{
		public:
			CString();
			CString(EEncoding encoding);

			inline const CChar* getCharArray() { return _str.size() == 0? NULL : &_str[0]; };
			inline size_t size() { return _str.size(); };
			inline void clear() { _str.clear(); };

			CString& operator<<(const char* str_in);

			CString& operator<<(CDataBindingAscii& str_in);
			CString& operator<<(CDataBindingUtf16& str_in);
			CString& operator<<(CDataBindingUcs& str_in);
            

		protected:
			std::vector<CChar> _str;
			EEncoding _encoding;
		};
	}
}
#endif//_H_PINE_ENGINE_STRING_CSTRING
