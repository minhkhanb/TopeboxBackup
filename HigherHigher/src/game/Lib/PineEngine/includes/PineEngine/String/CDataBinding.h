#if !defined (_H_PINE_ENGINE_STRING_CDATA_BINDING)
#define _H_PINE_ENGINE_STRING_CDATA_BINDING

#include "../common.h"
namespace Pine {
	namespace String {
		template <typename T>
		class PE_EXPORT CDataBinding
		{
		public:
			CDataBinding();
			CDataBinding(T* data, int data_len);
			void bind(T* data, int data_len);
		protected:
			const T* _data;
			int _data_len;

			friend class CString;
		};

		typedef CDataBinding<BYTE>		CDataBindingAscii;
		typedef CDataBinding<UINT16>	CDataBindingUtf16;
		typedef CDataBinding<UINT32>	CDataBindingUcs;
	}
}

#endif//_H_PINE_ENGINE_STRING_CDATA_BINDING