#pragma once
#if !defined(_H_PINE_ENGINE_PLATFORM_INTERFACE_NAMESPACE)
#define _H_PINE_ENGINE_PLATFORM_INTERFACE_NAMESPACE

#include "../../common.h"
#include <string>
namespace Pine
{
	namespace Platform
	{
		namespace Interface
		{

			//this interface makes the object enable identifing.

			class PE_EXPORT INamespace
			{
			public:
				//is object name
				virtual bool isName(std::string name) = 0;

				//set name to the object
				virtual void setName(std::string name) = 0;

				//get the object's name
				virtual std::string getName() = 0;

				//find if object has the child with name.
				virtual void* findChildName(std::string name) = 0;
			};
		}
	}
}
#endif//_H_PINE_ENGINE_PLATFORM_INTERFACE_NAMESPACE