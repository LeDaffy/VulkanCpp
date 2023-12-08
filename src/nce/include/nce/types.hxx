#pragma once
#include <cstdint>
#include <cstring>

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using i8  = std::int8_t;
using i16 = std::int16_t; 
using i32 = std::int32_t; 
using i64 = std::int64_t; 

using f32 = float;
using f64 = double;

using CString = const char*;

template<typename SizeType=std::size_t>
struct CStr {
    const char* str;
    SizeType m_size;
    
    constexpr CStr(const char* str) : str(str), m_size(strlen(str)) {}
    constexpr CStr& operator=(const char* o) { this->str = o; m_size = strlen(str); return *this; };
    constexpr operator const char*() const {return str;}
    constexpr SizeType size() { return m_size; }
};
