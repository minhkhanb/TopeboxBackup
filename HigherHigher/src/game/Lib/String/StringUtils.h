#if !defined(_H_STRING_UTILS)
#define _H_STRING_UTILS
#include "../DataType/CType.h"
#include "../Math/CMath.h"
#define CTOI(c) (c-'0')
#define STR_TO_UINT(str,len,i) {int l=len;i=0; for(int j = 0; j< l;j++){i+=( CTOI(str[j])*CMath::POW(10,l-j-1));};}
#define STR_TO_INT(str,len,i) {i=0; int pos = 0; int l = len; bool minus = false;if(str[0]=='-'){minus = true;pos++;};for(int j = pos; j< l;j++){i+=( CTOI(str[j])*CMath::POW(10,l-j-1));};if(minus){i=0-i;};}

class StringUtils{
public:
	//these function share by internet and modyfied to use
	static char charFromHexChar(char ch);
	static char charToHexChar(char code);
	static Buffer* encodeUrl(const char *str, int inputlen);
	static Buffer* decodeUrl(const char *str, int inputlen);

	static Buffer* encodeData(const char* data, int len);
	static Buffer* decodeData(const char* data, int len);
	//these function write to work with char array
	static int getIndexOfString(const char* src, int src_len, const char* find, int find_len);
	static char* subString(const char* src, int src_len, int pos, int len = -1);
	static int getStringLen(const char* src);

	static Buffer* compressData(const char* str, int nof_length);
	static Buffer* extractData(const char* str, int nof_length);
	static int getNumFromString(int pos, const char* str, int length);
	static int getLengthStringCompress(const char* str, int nof_length);
	//static int stringToUInt(const char* str, int len);
	static char* cstringFromPineString(PineString& str);
};

#endif