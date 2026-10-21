#pragma once
// lane-resource-lowers: CExoBase::GetDirectoryList closure (original 005E6640 ->
// 005E8CF0) and the CExoString / CExoArrayList lowers it needs that no M40
// object defines. All functions keep the original thiscall machine shape as
// fastcall: ECX receiver, ignored EDX, remaining words on the stack, callee pops.
#include "98ca969b25-f200.h"
#include <windows.h>
#include <cstdint>
namespace directory_list {
using W=std::uint32_t;using S=kotor_reconstruction::StartupString;
// Observed CExoArrayList<CExoString> storage {data,count,capacity}
// (004059B8/004059DD/004050A9); the same three words f200.h already measured.
using List=f200_ports::Enumeration;
// Observed CExoArrayList<FILETIME> storage of the 005E8CF0 local at [esp+38h]
// (data [38h], count [3Ch], capacity [40h]; 005E8E37-005E8E3F; 004C4F57/004C4F66).
struct TimeList {FILETIME* data;std::int32_t count;std::int32_t capacity;};
static_assert(sizeof(TimeList)==12&&sizeof(FILETIME)==8,"observed x86 storage");
// 005E6080 CExoString::LowerCase: hidden return storage (no live String), RET 4.
__declspec(noinline) S* __fastcall LowerCase005e6080(S* self,void*,S* output);
// 005E5600 CExoString::operator[]: AL result, RET 4.
__declspec(noinline) char __fastcall At005e5600(S* self,void*,std::int32_t index);
// 005E5410 CExoString::operator< and 005E5580 CExoString::operator>=: RET 4.
__declspec(noinline) std::int32_t __fastcall Less005e5410(S* self,void*,S* other);
__declspec(noinline) std::int32_t __fastcall GreaterEqual005e5580(S* self,void*,S* other);
// 00405080 CExoArrayList<CExoString>::Allocate (RET 4), 004059A0 ::Add (String by
// value, callee destroys, RET 8), 005E8C60 ::Insert (String by value + index, RET 0Ch).
__declspec(noinline) void __fastcall Allocate00405080(List* self,void*,std::int32_t capacity);
__declspec(noinline) void __fastcall Add004059a0(List* self,void*,S value);
__declspec(noinline) void __fastcall Insert005e8c60(List* self,void*,S value,std::int32_t index);
// 004C4F50 CExoArrayList<FILETIME>::Allocate (RET 4), 005E8BF0 ::Insert (RET 0Ch).
__declspec(noinline) void __fastcall AllocateTimes004c4f50(TimeList* self,void*,std::int32_t capacity);
__declspec(noinline) void __fastcall InsertTime005e8bf0(TimeList* self,void*,FILETIME value,std::int32_t index);
// 005E8CF0 CExoBaseInternal::GetDirectoryList: ECX never read (overwritten at
// 005E8D14), five stack words, RET 14h, EAX 1 listed / 0 FindFirstFileA failed.
__declspec(noinline) W __fastcall GetDirectoryList005e8cf0(void* internal,void*,List* list,S* directory,W type,W directories,W order);
}
