#pragma once
#include "0d6cdf91cb-resources.h"
#include "10c58d5fbb-key_entry_from_table.h"
#include "f9e919733c-ini_addhead.h"
#include "91c3f40cf9-key_constructor.h"
#include "d11fc18ea9-warning_runtime.h"
#include "d2c5c47a35-allocator_live.h"
#include <type_traits>

// Required owner-reviewed lower joins only. No definitions, provider table,
// original numeric callable addresses or alternate class/storage owners.
// The key receiver is the same physical allocation returned by the selected
// key_table_construct_0040d030 owner. Its full nominal lifetime remains owned
// by that constructor/build/destructor cohort, not by this consumer.
namespace add_key_lower {
using Word=menu_resources::Word;
using String=menu_resources::String;
using List=base_ini_reconstruction::List;
__declspec(noinline) int __fastcall Equal005e5200(String*,void*,String*);
__declspec(noinline) void __fastcall Rebuild00410260(void*,void*);
__declspec(noinline) Word __fastcall Build00410190(
    void*,void*,Word,String*,Word,Word);
// Remove's original EAX is discarded here; only its consumed void contract
// is requested. This declaration supplies no original return-value credit.
__declspec(noinline) void __fastcall Remove005e9c10(List*,void*,void*);
__declspec(noinline) void __fastcall Destroy0040d0d0(void*,void*);
extern const char rebuild_message_0073d8a8[];
}

using AddKeyTransport=menu_resources::Word(__fastcall*)(
    menu_resources::Manager*,void*,menu_resources::String*,
    menu_resources::Word,menu_resources::Word);
static_assert(std::is_same<decltype(&unresolved_add_key_00406e20),
                          AddKeyTransport>::value,"existing fastcall20 contract");
static_assert(std::is_same<menu_resources::String,
                          kotor_reconstruction::StartupString>::value,
              "one genuine nominal String");
static_assert(std::is_same<base_ini_reconstruction::List,
                          kotor_list::Internal>::value,"one genuine nominal list");
static_assert(sizeof(menu_resources::Manager)==96 &&
              alignof(menu_resources::Manager)==4 &&
              sizeof(menu_resources::String)==8 &&
              sizeof(base_ini_reconstruction::ListWrapper)==4 &&
              sizeof(base_ini_reconstruction::List)==12 &&
              sizeof(base_ini_reconstruction::Node)==12,
              "actual current consumer storage contracts");
