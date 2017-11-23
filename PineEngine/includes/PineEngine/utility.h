#pragma once
#ifndef _H_PINEENGINE_UTILITY
#define _H_PINEENGINE_UTILITY

#include "common.h"
#include <functional>
#include <sstream>
#include <stdlib.h>
/*
int atoi(const char *str)
{
	return((int)strtol(str, (char **)NULL, 10));
}*/
namespace std
{
	template < typename T > std::string to_string( const T& n )
	{
		std::stringstream stm ;
		stm << n ;
		return stm.str() ;
	};

	template <typename T>
	struct hex_to {
	    T value;
	    operator T() const {return value;}
	    friend std::istream& operator>>(std::istream& in, hex_to& out) {
	        in >> std::hex >> out.value;
	        return in;
	    }
	};
	
	#ifdef PE_BUILD_ANDROID

	
	
	float strtof(const char* str, char** endptr);
	long strtold(const char* str, char** endptr);
	long long strtolld(const char* str, char** endptr);
	/*

	 * Convert a string to an unsigned long long.

	 *

	 * Ignores `locale' stuff.  Assumes that the upper and lower case

	 * alphabets and digits are each contiguous.

	 */
	 //https://github.com/android/platform_bionic/blob/master/libc/upstream-openbsd/lib/libc/stdlib/strtoull.c

	unsigned long long strtoull(const char *nptr, char **endptr, int base);

	long long strtoll(const char* str, char** endptr, int wt);
	int stoi(string str);
	
	#endif
}

#endif //_H_PINEENGINE_UTILITY