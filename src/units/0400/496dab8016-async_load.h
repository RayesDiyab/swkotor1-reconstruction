#pragma once
#include "868f97a4f5-resources.h"
namespace async_load {
using Manager=menu_resources::Manager;
using Ref=menu_resources::Ref;
using String=menu_resources::String;
using Word=menu_resources::Word;
}
extern "C" {
// RIM consumer boundary: ECX receiver/ignored EDX, zero stack purge, EAX ignored.
void __fastcall rim_load_004094a0(void*,void*);
// Reuse existing Task36 add/key-entry declarations from resources.h.
// New required lower transports only; no lower implementation in this packet.
async_load::Ref* __fastcall async_ref_assign_string_004061f0(async_load::Ref*,void*,async_load::Ref*,async_load::String*);
async_load::Ref* __fastcall async_ref_construct_string_00406d60(async_load::Ref*,void*,async_load::String*);
async_load::Word __fastcall async_remove_key_00407830(async_load::Manager*,void*,async_load::String*,async_load::Word);
void __fastcall rim_enter_005e9300(void*,void*);
void __fastcall rim_leave_005e9310(void*,void*);
}
