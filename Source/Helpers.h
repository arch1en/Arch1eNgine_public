////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Helpers
//  @author     : Artur Ostrowski
//  @usage      : Miscellaneous helper functions.
//  @version    : 1.0.0
//
////////////////////////////////////////
#pragma once

#include "stdafx.h"

#include <vector>

void Split(const char * InSentence, char InDelimiter, std::vector<std::string> & OutTokens);

template <class return_type, class... params>
class Delegate
{
	typedef return_type(*Type)(void* Callee, params...);
public:

	Delegate(void* InCallee, Type InFunctionCallback)
		: FunctionPointerCallee{ InCallee }
		, FunctionPointerCallbackFunction{ InFunctionCallback }
	{}

	template<class T, return_type(T::*TMethod)(params...)>
	static Delegate FromFunction(T* Callee)
	{
		Delegate d(Callee, &MethodCaller<T, TMethod>);
		return d;
	}

	return_type operator()(params... Parameters) const
	{
		return (*FunctionPointerCallbackFunction)(FunctionPointerCallee, Parameters);
	}

private:
	void* FunctionPointerCallee;
	Type FunctionPointerCallbackFunction;

	template <class T, return_type(T::*TMethod)(params...)>
	static return_type MethodCaller(void* Callee, params... Parameters)
	{
		T* p = static_cast<T*>(Callee);
		return(p->*TMethod)(Parameters...);
	}
};

std::string GetFileExtension(const char* aFilePath);
void TranslateInputKeyToSDLKeycode(const std::string& aKey, char& aOutChar);

