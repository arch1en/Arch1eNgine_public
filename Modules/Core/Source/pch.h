#ifndef PRECOMPILEDHEADER_PCH
#define PRECOMPILEDHEADER_PCH

#include <string>
#include <vector>
#include <memory>
#include <map>

using int8 = char;
using int16 = short int;
using int32 = int;
using int64 = long int;

using uint8 = unsigned char;
using uint16 = unsigned short int;
using uint32 = unsigned int;
using uint64 = unsigned long int;

using string = std::string;
using wstring = std::wstring;
using wchar = wchar_t;

template<typename T>
using vector = std::vector<T>;
template<typename K, typename V>
using map = std::map<K, V>;

template<typename T>
using uptr = std::unique_ptr<T>;
template<typename T>
using sptr = std::shared_ptr<T>;
template<typename T>
using wptr = std::weak_ptr<T>;

#endif PRECOMPILEDHEADER_PCH