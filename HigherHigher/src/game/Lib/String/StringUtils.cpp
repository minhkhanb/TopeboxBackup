#include "pch.h"
#include "StringUtils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
//////////////////////////////////////////////////////////
//STRING UTILS
//////////////////////////////////////////////////////////
//////
// Converts a hex character to its integer value 
// this function share by internet and modyfied to use
char StringUtils::charFromHexChar(char ch) {
	return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

// Converts an integer value to its hex character
// this function share by internet and modyfied to use
char StringUtils::charToHexChar(char code) {
	static char hex[] = "0123456789abcdef";
	return hex[code & 15];
}


// Returns a url-encoded version of str
// this function share by internet and modyfied to use
Buffer* StringUtils::encodeUrl(const char *str, int inputlen) {
	char *pstr = (char*)str, *buf = new char[inputlen * 3 + 1], *pbuf = buf;
	for (int i = 0; i< inputlen; i++){
		//while (*pstr) {
		if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~')
			*pbuf++ = *pstr;
		else if (*pstr == ' ')
			*pbuf++ = '+';
		else
			*pbuf++ = 'x', *pbuf++ = charToHexChar(*pstr >> 4), *pbuf++ = charToHexChar(*pstr & 15);
		pstr++;
	}
	int len = pbuf - buf;
	*pbuf = '\0';
	Buffer* rs = new Buffer((BYTE*)buf, len);
	delete[] buf;
	return rs;
}

// Returns a url-decoded version of str
// this function share by internet and modyfied to use
Buffer* StringUtils::decodeUrl(const char *str, int inputlen) {
	char *pstr = (char*)str, *buf = new char[inputlen + 1], *pbuf = buf;
	while (*pstr) {
		if (*pstr == 'x') {
			if (pstr[1] && pstr[2]) {
				*pbuf++ = charFromHexChar(pstr[1]) << 4 | charFromHexChar(pstr[2]);
				pstr += 2;
			}
		}
		else if (*pstr == '+') {
			*pbuf++ = ' ';
		}
		else {
			*pbuf++ = *pstr;
		}
		pstr++;
	}
	int len = pbuf - buf;
	*pbuf = '\0';
	Buffer* rs = new Buffer((BYTE*)buf, len);
	delete[] buf;
	return rs;
}

///
/// Compression string
///

Buffer* StringUtils::compressData(const char *str, int nof_length){
	char *tmp = new char[nof_length];
	int nof_total = 0;
	int pos = 0;
	for (int i = 0; i < nof_length; i++){
		for (int j = i + 1; j <= nof_length; j++){
			if (str[i] == str[j]){
				nof_total++;
			}
			else{
				if (nof_total == 0){
					tmp[pos] = str[i]; pos++;
				}
				else if (nof_total > 0){
					int chr;
					nof_total = nof_total + 1;
					while (nof_total > 9){
						chr = nof_total / 10;
						tmp[pos] = (char)((chr)+48);	pos++;
						nof_total = nof_total % 10;
					}
					chr = nof_total % 10;
					tmp[pos] = (char)((chr)+48);	pos++;
					tmp[pos] = str[i]; pos++;
				}
				nof_total = 0; i = j - 1;
				break;
			}
		}
	}
	tmp[pos] = '\0';
	Buffer *sdata = new Buffer((BYTE*)tmp, pos);
	delete[]tmp;
	return sdata;
}

////
//// Extract Data
////

Buffer* StringUtils::extractData(const char* str, int length){
	int nof_length = StringUtils::getLengthStringCompress(str, length);
	int len_sub = 0;
	char l_num[255];
	char *tmp = new char[nof_length + 1];
	int pos = 0;
	for (int i = 0; i < length; i++)
	{
		if (str[i] >= '0' && str[i] <= '9'){
			int num = StringUtils::getNumFromString(i, str, length);
			sprintf(l_num, "%d", num);
			int next_pos = 0;
			next_pos = StringUtils::getStringLen(l_num);
			for (int j = 0; j < num; j++){
				tmp[pos] = str[i + next_pos]; pos++;
			}
			i += next_pos;
		}
		else{
			tmp[pos] = str[i]; pos++;
		}
	}

	tmp[pos] = '\0';
	Buffer *sdata = new Buffer((BYTE*)tmp, pos);
	return sdata;
}

///
/// get length data
///
int StringUtils::getLengthStringCompress(const char* str, int length){
	int num = 0;
	int nof_total = 0;
	int sum = 0;
	for (int i = 0; i < length; i++){
		if (str[i] >= '0' && str[i] <= '9'){
			for (int j = i; j < length; j++){
				if (str[j] >= '0' && str[j] <= '9'){
					nof_total++;
					if (nof_total > 1){
						num = num * 10;
						num += str[j] - '0';
					}
					else{
						num = str[j] - '0';
					}

				}
				else{
					i = j - 1;
					break;
				}
			}
		}
		else{
			if (nof_total > 0){
				sum += num;
			}
			else{
				sum += 1;
			}
			num = 0;
			nof_total = 0;
		}
	}
	return sum;
}

//Get Number from String

int StringUtils::getNumFromString(int pos, const char *str, int length){
	int num = 0;
	int nof_total = 0;
	for (int i = pos; i < length; i++){
		if (str[i] >= '0' && str[i] <= '9'){
			nof_total++;
			if (nof_total > 1){
				num = num * 10;
				num += str[i] - '0';
			}
			else{
				num = str[i] - '0';
			}
		}
		else{
			break;
		}
	}
	return num;
}

//find index of a string in another string
//-1 : not match
//>0 : begin pos of string
int StringUtils::getIndexOfString(const char* src, int src_len, const char* find, int find_len){
	//printf("getIndexOfString\n");
	if (find_len > src_len) return -1;
	for (int i = 0; i <= (src_len - find_len); i++){
		if (src[i] == find[0]){
			//printf("found at:%d\n",i);
			bool found = true;
			for (int j = 1; j< find_len; j++){
				//printf("%c %c\n",src[i+j],find[j]);
				if (src[i + j] != find[j]){

					found = false;
					break;
				}
			}
			if (found)return i;
		}
	}
	return -1;
}

//get sub string of a string.
//src : string to get substring from
//str_len: len of src
//pos : the position to begin get substring.
//len : the number of character to taking.
char* StringUtils::subString(const char* src, int src_len, int pos, int len){
	//printf("subString\n");
	if (len == -1) len = src_len - pos;
	if (pos < 0 || (pos + len) > src_len)return NULL;
	//printf("subString1:%d %d\n",pos, len);
	char* newstr = new char[len + 1];
	char* pointer = newstr;
	for (int i = pos; i<pos + len; i++){
		*newstr++ = *(src + i);
	}
	*newstr = '\0';
	//printf("urls:%s\n",pointer);
	return pointer;
}
int StringUtils::getStringLen(const char* src){
	int count = 0;
	char* tmp = (char*)src;
	while (*tmp++ != '\0'){
		count++;
	}
	return count;
}

char* StringUtils::cstringFromPineString(PineString& str)
{
	char* cstr = new char[str.Length() + 1];
	UNICHAR* pstr = str.Data();
	
	for(int i = 0; i< str.Length(); i++)
	{
		cstr[i] = pstr[i] & 0xFF;
	}
	cstr[str.Length()] = '\0';
	return cstr;
}