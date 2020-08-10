#ifndef UTILITIES_H
#define UTILITIES_H

#include "Debug/LogSystem.h"

#include <type_traits>

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

struct ErrorHandle
{
	const char* Msg;
	int16_t Code;

	ErrorHandle& operator=(const ErrorHandle& LHS) = delete;

	ErrorHandle() : Msg{ "OK." }, Code{0} {}
};

#define TypeSafeEnumBitmask(EnumType)	\
	inline std::underlying_type<EnumType>::type operator|(EnumType LHS, EnumType RHS) { return static_cast<std::underlying_type<EnumType>::type>(LHS) | static_cast<std::underlying_type<EnumType>::type>(RHS); } \
	inline std::underlying_type<EnumType>::type operator|(std::underlying_type<EnumType>::type LHS, EnumType RHS) { return LHS | static_cast<std::underlying_type<EnumType>::type>(RHS); }	\
	inline std::underlying_type<EnumType>::type operator|(EnumType LHS, std::underlying_type<EnumType>::type RHS) { return static_cast<std::underlying_type<EnumType>::type>(LHS) | RHS; }	\
	inline std::underlying_type<EnumType>::type operator&(EnumType LHS, EnumType RHS) { return static_cast<std::underlying_type<EnumType>::type>(LHS) & static_cast<std::underlying_type<EnumType>::type>(RHS); } \
	inline std::underlying_type<EnumType>::type operator&(std::underlying_type<EnumType>::type LHS, EnumType RHS) { return LHS & static_cast<std::underlying_type<EnumType>::type>(RHS); }	\
	inline std::underlying_type<EnumType>::type operator&(EnumType LHS, std::underlying_type<EnumType>::type RHS) { return static_cast<std::underlying_type<EnumType>::type>(LHS) & RHS; }

inline std::string operator/(const std::string& LHS, const std::string& RHS)
{
	return LHS + "/" + RHS;
}

inline std::string operator/(const std::string& LHS, const char* RHS)
{
	return LHS + "/" + RHS;
}

inline std::string operator/(const char* LHS, const std::string RHS)
{
	return std::string(LHS) + "/" + RHS;
}

#endif