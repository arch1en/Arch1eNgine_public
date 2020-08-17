#ifndef PRECOMPILEDHEADER_PCH
#define PRECOMPILEDHEADER_PCH

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using string = std::string;
using wstring = std::wstring;
using wchar = wchar_t;

template<typename T>
using vector = std::vector<T>;
template<typename K, typename V>
using map = std::map<K, V>;
template<typename K, typename V>
using hashtable = std::unordered_map<K, V>;

template<typename T>
using uptr = std::unique_ptr<T>;
template<typename T>
using sptr = std::shared_ptr<T>;
template<typename T>
using wptr = std::weak_ptr<T>;

#endif PRECOMPILEDHEADER_PCH