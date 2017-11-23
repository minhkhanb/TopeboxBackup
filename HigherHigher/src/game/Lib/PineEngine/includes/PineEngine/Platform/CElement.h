#if !defined(_H_PINE_ENGINE_PLATFORM_CELEMENT)
#define _H_PINE_ENGINE_PLATFORM_CELEMENT

#include "../common.h"
#include <vector>
#include <memory>
#include "EElementType.h"
#include "EElementDataType.h"

namespace Pine
{
	namespace Platform
	{
		class PE_EXPORT CElement
		{
			friend class CEffect;
			
		private:
			char _name[20];
			EElementType _type;
			EElementDataType _data_type;
			int _element_num; //number of member in a group of continuous element.
		public:

			CElement() : _type(EElementType::VERTEX), _data_type(EElementDataType::UNSIGNED_BYTE), _element_num(1){};
			CElement(const char* name, EElementType type, EElementDataType data_type,  int element_num);
			inline EElementType getType() { return _type; };
			inline EElementDataType getDataType() { return _data_type; };
			inline int getSize() { return ElementDataType::getSize(_data_type) * _element_num; };
			inline int getNumElements() { return _element_num; };
			inline const char* getName() { return _name; };
		};

		typedef std::shared_ptr<std::vector<CElement>> TVertexFormat;
	}
}

#endif//_H_PINE_ENGINE_PLATFORM_IELEMENT