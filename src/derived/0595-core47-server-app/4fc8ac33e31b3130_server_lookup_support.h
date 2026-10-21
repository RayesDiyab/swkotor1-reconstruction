#pragma once
#include <cstdint>
#include <cstring>
// Exact helper semantics retained from stage-m60/r4-lowers/r4_lowers.h.
// Narrow declarations avoid pulling unrelated client/menu header epochs into this unit.
namespace r4_lowers {
using U32=std::uint32_t;
inline U32 w(const void* p,U32 o){U32 v;std::memcpy(&v,static_cast<const unsigned char*>(p)+o,4);return v;}
inline unsigned char b8(const void* p,U32 o){return static_cast<const unsigned char*>(p)[o];}
inline void* at(U32 v){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(v));}
inline void* pw(const void* p,U32 o){return at(w(p,o));}
inline U32 vcall0(void* r,U32 slot){return reinterpret_cast<U32(__thiscall*)(void*)>(static_cast<std::uintptr_t>(w(pw(r,0),slot)))(r);}
}
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
extern "C" void* __fastcall kotor_fastcall_004ae750(void*,void*,std::uint32_t);
extern "C" unsigned char __fastcall kotor_fastcall_004d8230(void*,void*,std::uint32_t,void**);
