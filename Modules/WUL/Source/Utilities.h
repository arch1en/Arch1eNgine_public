#ifndef UTILITIES_H
#define UTILITIES_H

#include "LogSystem.h"

class TypeID
{
public:
	virtual bool IsTypeOf(const char* InTypeName) const
	{
		return strcmp(this->GetTypeName(), InTypeName) == 0;
	}

private:
	virtual inline const char* GetTypeName() const = 0 { Log(LogType::Error, 0, "Method must be overrided and return typeid(this).name()."); return ""; }
};

#endif