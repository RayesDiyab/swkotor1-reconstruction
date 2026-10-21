#pragma once
#include <cstdint>
#include <cstddef>
#include <new>
#include "e1805047c9b7563b_startup_string.h"
#include "9eca067a90332644_debug_constructor.h"
#include "a460430650bbc002_timers_constructor.h"
#include "6f01c2143fcea5be_alias_constructor.h"
#include "177a467f7ffe42b2_ini_comparison.h"
#include "c1a5cb021279949d_rand_constructor.h"
#include "d3bcdb58b43a10ee_constructors.h"
namespace base_ini_reconstruction {
using String=kotor_reconstruction::StartupString;
struct Node {Node* prev;Node* next;void* data;};
using List=kotor_list::Internal; // one actual list type, owned by task30
struct ListWrapper {List* internal;
 __declspec(noinline) ListWrapper* __thiscall construct_005ea4d0();
};
struct Section {
 alignas(String) unsigned char name_storage[8]; ListWrapper lines;
 String* name(){return std::launder(reinterpret_cast<String*>(name_storage));}
};
struct Base {
 void *ini,*timers,*debug,*aliases,*random,*internal;
 __declspec(noinline) Base* __thiscall construct_005e6500();
};
struct BaseInternal {
 volatile std::uint32_t extension_count;
 std::uint16_t* volatile extension_ids;
 String* volatile extension_strings; // each actual String lives behind the four-byte count cookie
 __declspec(noinline) void __thiscall load_aliases_005e7a90(String*);
};
struct Ini {
 ListWrapper sections; alignas(String) unsigned char filename_storage[8];
 String* filename(){return std::launder(reinterpret_cast<String*>(filename_storage));}
 __declspec(noinline) Ini* __thiscall construct_005ea540();
 __declspec(noinline) int __thiscall read_005ea6c0(String*,String*,String*,String*);
};
struct WritableIni;
struct PublicIni {
 WritableIni* volatile internal;
 __declspec(noinline) PublicIni* __thiscall construct_005e6750();
 __declspec(noinline) int __thiscall read_005e67c0(String*,String*,String*,String*);
 __declspec(noinline) int __thiscall write_005e67d0(String*,String*,String*,String*);
 __declspec(noinline) void __thiscall destroy_005e67e0();
};
static_assert(sizeof(void*)==4 && sizeof(Base)==24 && sizeof(Ini)==12 && sizeof(Section)==12 && sizeof(Node)==12 && sizeof(List)==12,"observed x86 layouts");
static_assert(offsetof(Base,debug)==8 && offsetof(Base,internal)==20 && offsetof(Ini,filename_storage)==4 && offsetof(Section,lines)==8 && offsetof(Node,data)==8,"observed field offsets");
using Ctor=void*(__thiscall*)(void*);
struct Bindings {
 void*(__cdecl *allocate_006fa7e6)(std::uint32_t);
 kotor_menu20::Debug*(__fastcall *debug_005e9370)(kotor_menu20::Debug*,void*);
 kotor_startup::Timers*(__fastcall *timers_005e91e0)(kotor_startup::Timers*,void*);
 alias_path::AliasListView*(__fastcall *aliases_005e6810)(alias_path::AliasListView*,void*);
 PublicIni*(PublicIni::*ini_005e6750)();
 startup_rand::Rand*(__fastcall *random_005ea420)(startup_rand::Rand*,void*);
 BaseInternal*(__fastcall *internal_005e8ba0)(BaseInternal*,void*);
 ListWrapper*(ListWrapper::*list_005ea4d0)();
 Section*(__fastcall *section_005ea600)(Section*,void*);
 char*(__fastcall *c_str_005e5670)(String*,void*);
 int(__cdecl *sprintf_006fadb0)(char*,const char*,...);
 std::int32_t(__fastcall *compare_nc_005e58c0)(String*,void*,const char*);
 void(__fastcall *destroy_005ea650)(Ini*,void*);
 String*(String::*assign_string_005e5c50)(String*);
 String*(__fastcall *assign_cstr_005e5140)(String*,void*,const char*);
 void*(__cdecl *open_006fad9d)(const char*,const char*);
 char*(__cdecl *gets_006fc469)(char*,int,void*);
 int(__cdecl *seek_006fbb19)(void*,std::int32_t,int);
 int(__cdecl *close_006facf0)(void*);
 Node*(__fastcall *add_tail_005e9dc0)(List*,void*,void*);
 void*(__fastcall *next_005e9b90)(List*,void*,Node**);
 char*(__cdecl *chr_006fb5e0)(const char*,int);
 String*(__fastcall *right_005e6170)(String*,void*,String*,int);
 String*(__fastcall *left_005e5f70)(String*,void*,String*,int);
 int(String::*equals_005e5290)(const char*);
 std::int32_t(String::*length_005e5790)();
 void(__cdecl *format_string_005e5680)(String*,const char*,...);
 void(__fastcall *add_alias_005e7760)(BaseInternal*,void*,String*,String*,String*,String*);
};
extern Bindings bindings;
// No production installation, globals or successful fallback providers.
// Valid nonthrowing bound providers and original object lifetimes are required.
}

static_assert(sizeof(base_ini_reconstruction::BaseInternal)==12 && offsetof(base_ini_reconstruction::BaseInternal,extension_count)==0 && offsetof(base_ini_reconstruction::BaseInternal,extension_ids)==4 && offsetof(base_ini_reconstruction::BaseInternal,extension_strings)==8,"exact twelve-byte canonical resource table receiver");
