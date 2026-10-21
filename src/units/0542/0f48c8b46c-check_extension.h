#pragma once
#include <cstdint>
namespace kotor_renderer_init {
using U32=std::uint32_t;using I32=std::int32_t;using U8=std::uint8_t;
extern const char* const token_delimiter_0073d5d8;
extern U32 __cdecl StringCSpan_006fcb40(const char*,const char*);
extern I32 __cdecl StringNCompare_006faff0(const char*,const char*,U32);
U8 __cdecl CheckExtension_0045f980(const char*,const char*);
}
