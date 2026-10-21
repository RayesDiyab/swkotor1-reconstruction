#pragma once
// lane-m50-vtables (W-4): typed original method tables that replace raw original vtable words stored into objects.
// Original swkotor.exe (GOG K1, sha 9c10e045...1435, image base 0x400000; never executed). Slot words were read from
// the pinned image (.rdata) by lane-m50-vtables/tools/pe.py; table extents from the Ghidra export
// ("Address table[28] created" at 007457A0 / 00751848; CServerExoApp_vtable 00744184 ends at the zero word 007441AC).
// D1: every original __thiscall slot is the ABI-identical __fastcall(receiver, unused EDX, stack words).
// D21/D24/D41: typed constant arrays of function pointers; no Word -> function-pointer conversion anywhere.
// Storing a table's address into an object's first word is an object-pointer -> integer conversion (D24 accepted).
#include <cstdint>

// ---- CBaseExoApp slot layout: verbatim view of startup46/client_exo_app.cpp (lane-m46-startup), the same token
// sequence show_fade_panel.cpp (lane-m48-fade) uses. The CServerExoApp table 00744184 has the same ten slots.
namespace kotor_client_exo_app {
using Word = std::uint32_t;
struct Methods {
    void (__fastcall* slot00_void_return_0c)(void*, void*, Word, Word, Word);                               // 005C5630 `ret 0Ch`
    void (__fastcall* handle_message)(void*, void*, Word, unsigned char*, Word, Word);                         // 005ED790
    float (__fastcall* get_fps)(void*, void*);                                                                  // 005EE350 (ST0)
    Word (__fastcall* continue_message_processing)(void*, void*);                                               // 005ED7A0
    void* (__fastcall* get_net_layer)(void*, void*);                                                            // 005ED6E0
    void (__fastcall* shut_down_to_main_menu)(void*, void*);                                                    // 005ED890
    void* (__fastcall* get_connection_lib)(void*, void*);                                                       // 005ED520
    Word (__fastcall* slot1c_return_zero)(void*, void*);                                                        // 0063E7F0 `xor eax,eax; ret`
    Word (__stdcall* get_application_id)();                                                                     // 005ED990 (stdcall, ECX unused)
    void (__fastcall* set_application_ids_match)(void*, void*, Word);                                           // 005ED8A0 RET 4
};
}

// ---- CGameObject_vtable 007457A0 / CSWCModule_vtable 00751848: 28 slots each (0x70 bytes).
namespace kotor_game_object {
using Word = std::uint32_t;
struct Methods {
    void* (__fastcall* scalar_deleting_destructor)(void* self, void*, Word flags);   // +00 004C3040 / 00644150, RET 4
    void (__fastcall* slot04_void_return_4)(void* self, void*, Word);                // +04 0060E760 `ret 4`
    void (__fastcall* slot08_void_return_8)(void* self, void*, Word, Word);          // +08 005B5E90 `ret 8`
    void* (__fastcall* slot0c_to_6c[25])(void* self, void*);                         // +0C..+6C 0063E7F0 `xor eax,eax; ret`
                                                                                      //   (CSWCModule +1C: 00641DB0 `mov eax,ecx; ret`)
};
static_assert(sizeof(Methods) == 0x70, "28 observed slots 007457A0..0074580C / 00751848..007518B4");
// Typed dispatch for a live CSWCModule receiver. Only the typed table address is accepted (every writer of a
// CSWCModule first word now stores it); anything else is the D41 contract-violation stop TerminateProcess(86).
const Methods* module_methods_of(const void* module);
}

extern "C" {
// startup46/client_exo_app.cpp (lane-m46-startup) - existing definition.
extern const kotor_client_exo_app::Methods kotor_vtable_cclientexoapp_0074e6d8;
// lane-m50-vtables m50_vtables.cpp.
extern const kotor_client_exo_app::Methods kotor_vtable_cserverexoapp_00744184;
extern const kotor_game_object::Methods kotor_vtable_cgameobject_007457a0;
extern const kotor_game_object::Methods kotor_vtable_cswcmodule_00751848;
// lane-m50-vtables m50_tlk_table_addresses.cpp: constant-initialized addresses of lane-tlk's typed tables
// (tlk_core.h kotor_vtable_ctlktable_0073ecdc / kotor_vtable_cswtlktable_0074e700) for units whose header roots do
// not reach tlk_core.h. Object pointers only.
extern const void* const kotor_vtable_address_ctlktable_0073ecdc;
extern const void* const kotor_vtable_address_cswtlktable_0074e700;
}

// The word stored into an object's vptr slot: the typed table's own (relocated) address.
inline std::uint32_t kotor_vtable_word(const void* table) noexcept {
    return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(table));
}
