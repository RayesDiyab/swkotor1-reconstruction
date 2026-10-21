// lane-m47-core rev2 ADAPTED COPY of resman-core/resman_objects.h (base sha256
// a5a7253df719bb09089222122c81abe20ed89c88125f001fa62a53c96d27620e): 00409B80 declared with its real EAX (D12).
#pragma once
// lane-resman: CExoResMan / CRes object-management boundary on the texture/GUI
// demand path. Bodies are reviewed Task10 donors (see PACKET.json), re-bound to
// the exact M40 provider names. No new Manager, resource class or global cell.
#include "15d7bcda8b-resources.h"   // M40 task-36 menu_resources::{Manager,Ref,String}
#include <cstdint>

namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction { struct Node; }

// M40 Manager list Internal is the canonical kotor_list::Internal used by
// ini_section::AddTail005e9dc0 (M40 unit ini-add-tail, startup-next).
namespace ini_section {
__declspec(noinline) base_ini_reconstruction::Node* __fastcall AddTail005e9dc0(
    kotor_list::Internal*, void*, void*);
}

extern "C" {
// ---- M40-defined lowers (exact existing symbols; see PACKET undefined_externals)
menu_resources::Manager* __cdecl c2da_manager_007a39e8();
void* __cdecl key_debug_root_007a39e0();
std::uint32_t __fastcall unresolved_key_entry_from_table_004071a0(
    menu_resources::Manager*, void*, void* list_member, menu_resources::Ref*,
    std::uint32_t type_word, std::uint32_t* output1, std::uint32_t* output2, std::uint32_t kind);
menu_resources::String* __fastcall c2da_assign_text_005e5140(menu_resources::String*, void*, const char*);
menu_resources::String* __fastcall async_assign_string_005e5c50(menu_resources::String*, void*, menu_resources::String*);
void __fastcall c2da_ref_to_string_00405f70(menu_resources::Ref*, void*, menu_resources::String*);
char* __fastcall WindowCStr005e5670(menu_resources::String*, void*);
menu_resources::String* __fastcall c2da_base_005e6660(void* receiver, void* unused_edx, std::uint32_t type_word);
int __fastcall c2da_equal_text_004060d0(menu_resources::Ref*, void*, const char*);
void __fastcall c2da_manager_cancel_004088f0(menu_resources::Manager*, void*, void*);
std::uint32_t __fastcall c2da_manager_demand_004089f0(menu_resources::Manager*, void*, void*);
void __fastcall c2da_manager_release_demand_00408c90(menu_resources::Manager*, void*, void*);
// rev2 (D12): the full-EAX body owner of 00408C90 (dependency-task10 manager_demand.cpp).
std::uint32_t __fastcall c2da_manager_release_demand_body_00408c90(menu_resources::Manager*, void*, void*);
void __fastcall c2da_resource_serviced_00409b40(void*, void*, std::uint32_t);

// ---- Lane-local literal data accessors (original .rdata 0073D8DC/E0/E4)
const char* __cdecl c2da_archive_label_0073d8dc();
const char* __cdecl c2da_archive_label_0073d8e0();
const char* __cdecl c2da_archive_label_0073d8e4();

// ---- Lane exports (names required by lane-tpc tpc.hpp and M40 binding.h)
void* __fastcall c2da_lookup_004074d0(menu_resources::Manager*, void*, menu_resources::Ref*, menu_resources::Word);
void __fastcall c2da_register_00407680(menu_resources::Manager*, void*, menu_resources::Ref*, menu_resources::Word, void*);
int __fastcall resource_request_00408620(void*, void*);
void __fastcall c2da_request_00408620(void*, void*);
void __fastcall resource_append_005e9dc0(kotor_list::Internal*, void*, void*);
void __fastcall c2da_cancel_00409b00(void*, void*);
std::uint32_t __fastcall c2da_current_resource_demand_00409b10(void*, void*);
// rev2 (D12): 00409B80 returns 00408C90's EAX (00409B87 call; 00409B8C ret with EAX unchanged).
std::uint32_t __fastcall c2da_release_demand_00409b80(void*, void*);
int __fastcall c2da_release_00409cf0(menu_resources::Manager*, void*, void*);
int __fastcall c2da_not_equal_text_00406060(menu_resources::Ref*, void*, const char*);
// 005E5B70 CExoString(char const*, int) RET8 (string_counted.cpp; D18 single provider).
menu_resources::String* __fastcall resman_string_counted_005e5b70(menu_resources::String*, void*, const char*, int);
}
