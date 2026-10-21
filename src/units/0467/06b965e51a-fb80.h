#pragma once
#include "48d4671e3a-f3c0.h"
#include "dc6e86509b-source_substring.h"
#include "cebae40286-string_priority.h"
namespace fb80_ports {
using W=menu_resources::Word;using S=menu_resources::String;
void* __cdecl AllocateBlob005e9350(W);
void __cdecl ReleaseBlob005e9360(void*);
std::int32_t __fastcall Find005e5f20(S*,void*,W,std::int32_t);
void* __fastcall Archive0040ee80(void*,void*);
W __fastcall Probe0040d200(void*,void*,S*);
}
namespace fb80_private {
// One persistent machine slot, supplied by wrapper or defined-input fixture.
// No constructor, default initializer, production seed, or external class claim.
struct alignas(4) TypeFrame {unsigned char bytes[4];};
static_assert(sizeof(TypeFrame)==4,"one persistent type DWORD");
__declspec(noinline) menu_resources::Word __cdecl LoadRawType(const TypeFrame*);
__declspec(noinline) menu_resources::Word __fastcall Core(void*,void*,menu_resources::Word,TypeFrame*);
}
