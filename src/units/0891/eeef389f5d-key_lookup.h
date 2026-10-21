#pragma once
#include <stdint.h>
namespace menu_resources { struct Manager; struct Ref; }
// Exact x86 carrier: ECX=receiver, EDX unused, four stack DWORDs, RET16.
// The @24 decoration counts both register carriers; it does not mean RET24.
extern "C" uint32_t __fastcall unresolved_key_entry_00407230(
    menu_resources::Manager*, void*, menu_resources::Ref*, uint32_t,
    uint32_t* output1, uint32_t* output2);
// Required genuine lower provider. This packet does not implement004071A0.
// Six stack DWORDs, RET24; list_member is the address of a receiver member.
extern "C" uint32_t __fastcall unresolved_key_entry_from_table_004071a0(
    menu_resources::Manager*, void*, void* list_member, menu_resources::Ref*,
    uint32_t type_word, uint32_t* output1, uint32_t* output2, uint32_t kind);
