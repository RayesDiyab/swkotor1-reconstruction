#pragma once
#include "00a0946121-resources.h"
#include "3fb1417bda-resource_cell_contract.h"
#include <cstdint>
#include <cstring>
#include <initializer_list>
namespace menu_normal_retained_40_49 {
using U32=std::uint32_t;using U8=unsigned char;
template<unsigned N>struct Image {
 alignas(4) U8 bytes[N];
 U32 get(unsigned o)const{U32 v;std::memcpy(&v,bytes+o,4);return v;}
 void put(unsigned o,U32 v){std::memcpy(bytes+o,&v,4);}
 void put8(unsigned o,U8 v){bytes[o]=v;}
 void put16(unsigned o,std::uint16_t v){std::memcpy(bytes+o,&v,2);}
 void* ptr(unsigned o)const{return reinterpret_cast<void*>(get(o));}
};
struct MusicInternal:Image<0x30c>{__declspec(noinline) void __thiscall music_005f6480();};
struct Module:Image<0x138>{__declspec(noinline) Module* __thiscall construct_00643f40();};
using String=kotor_reconstruction::StartupString;using Ref=menu_resources::Ref;
struct RawStringStorage {alignas(String) unsigned char bytes[sizeof(String)];String* pointer(){return reinterpret_cast<String*>(bytes);}};
// D1: every original __thiscall slot is the ABI-identical __fastcall(receiver, unused EDX, stack words).
using Constructor=void*(__fastcall*)(void*,void*);using Destructor=void(__fastcall*)(void*,void*);
struct Providers {
 // receiver spelled void*: one provider serves both namespaces' MusicInternal views (same 0x30C object).
 void(__fastcall* fade_005f1b20)(void*,void*,void*,U32);
 Destructor sound_destroy_005d6200;
 void(__cdecl* free_006fa390)(void*);
 U32(__fastcall* not_equal_005e5390)(void*,void*,const char*);
 Constructor string_construct_005b3190;
 const char*(__fastcall* cstr_005e5670)(void*,void*);
 void(__cdecl* format_005e5680)(String*,const char*,...);
 void*(__fastcall* cache_004085f0)(void*,void*,String*,String*,U32);
 Destructor string_destroy_005e5c20;
 void*(__cdecl* allocate_006fa7e6)(U32);
 Constructor sound_construct_005d5a50;
 void(__fastcall* loop_005d5b40)(void*,void*,U32);
 void(__fastcall* priority_005d5ba0)(void*,void*,U32);
 void*(__fastcall* ref_string_00406d60)(Ref*,void*,void*);
 U32(__fastcall* play_005d5b10)(void*,void*,Ref*,U32,U32,U32,U32);
 void(__fastcall* sound_mode_005d5e80)(void*,void*,U32);
 U32(__fastcall* playing_005d5af0)(void*,void*);
 Destructor resume_005d5b30;
 void*(__fastcall* game_object_004d7d60)(void*,void*,U32,U32);
 Constructor ref_default_00405ed0;
 void*(__fastcall* assign_005e5140)(void*,void*,const char*);
 void* const volatile* sound_007a39ec;task10_resource_cells::ManagerCell resource_007a39e8;const U32* mode_0074c5f8;
 const U8* type_00746632;const U32* word_00746644;const U32* word_0074410c;
 const char* empty_0073d71c;const char* format_0074c304;
};
extern Providers providers;
// Normal nonthrowing source/provider paths only. Historical FS/EH records,
// cleanup/unwind, complete classes/vtables and real providers remain gated.
static_assert(sizeof(void*)==4 && sizeof(String)==8 && sizeof(Ref)==16,"original word sizes");
}
