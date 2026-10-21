#pragma once
#include "163d10ec4d-resources.h"
#include <cstddef>
namespace c2da_binding {
using Ref=menu_resources::Ref;using Word=menu_resources::Word;
// Consumed live prefix only, not a complete C2DA/CResHelper class declaration.
struct Helper {
    const void* methods;
    volatile Word requested;
    void* volatile resource;
    Ref reference;
    __declspec(noinline) void __thiscall SetResRef(Ref*,Word);
};
static_assert(sizeof(Helper)==0x1c && offsetof(Helper,requested)==4 && offsetof(Helper,resource)==8 && offsetof(Helper,reference)==12);
}
// Proposed genuine provider imports. None is defined in production by this packet.
// The access shim borrows the one canonical007A39E8 cell; it creates no manager.
extern "C" {
menu_resources::Manager* __cdecl c2da_manager_007a39e8();
const char* __cdecl c2da_empty_0073d71c();
int __fastcall c2da_equal_ref_004060b0(menu_resources::Ref*,void*,menu_resources::Ref*);
int __fastcall c2da_not_equal_text_00406060(menu_resources::Ref*,void*,const char*);
int __fastcall c2da_equal_text_004060d0(menu_resources::Ref*,void*,const char*);
void __fastcall c2da_cancel_00409b00(void*,void*);
int __fastcall c2da_release_00409cf0(menu_resources::Manager*,void*,void*);
void* __fastcall c2da_lookup_004074d0(menu_resources::Manager*,void*,menu_resources::Ref*,menu_resources::Word);
void* __fastcall c2da_resource_ctor_0041d730(void*,void*);
void __fastcall c2da_register_00407680(menu_resources::Manager*,void*,menu_resources::Ref*,menu_resources::Word,void*);
void __fastcall c2da_request_00408620(void*,void*);
menu_resources::Ref* __fastcall c2da_copy_00406120(menu_resources::Ref*,void*,menu_resources::Ref*,menu_resources::Ref*);
}
