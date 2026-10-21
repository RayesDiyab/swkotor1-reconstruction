// lane-m50-vtables (W-4): typed CGameObject (007457A0), CSWCModule (00751848) and CServerExoApp (00744184) method
// tables with their slot bodies. Evidence: lane-m50-vtables/dis/*.asm (dumpbin of the pinned swkotor.exe) and
// tools/pe.py slot dumps; see NOTES.md / PACKET.json.
#include "b593572262-m50_vtables.h"
#include "2b590d51e1-string_priority.h"   // GuiAssign005e5140 (005E5140) + kotor_reconstruction::StartupString
#include "05a3c5e4fb-allocator_live.h"                       // live_free (006FA390, D76 direct provider)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstring>

extern "C" {
// D83 shared fatal trap (m48/t/include/kotor_unreachable_trap.h, identical declaration).
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long original_address, const char* symbol);
// Existing shared-slot bodies of exactly these types (reused, no new address credit):
void __fastcall light_manager_void_return_8_005b5e90(void* self, void*, std::uint32_t, std::uint32_t);   // 3d construct3d.h:265
void* __fastcall behavior_null_return_0063e7f0(void* self, void*);                                        // 3d construct3d.h:241
void* __fastcall behavior_self_return_00641db0(void* self, void*);                                        // 3d construct3d.h:242
void __fastcall kotor_fastcall_004015a0(void*, void*);                    // routes kotor_original_calls.h (004015A0 `ret`)
// 00445080 CExoArrayList<T*>::Remove(value), thiscall RET 4 (3d anim3d.h:239 spelling of the strict-link owner).
void __fastcall kotor_fastcall_00445080(void* list, void*, void* element);
// transition lane: 00609410 CSWCArea::UnloadArea (thiscall, RET) - live body in 1204-01-w3t2b_w3_client_area.obj.
void __fastcall recovered_client_area_unload_area_00609410(void* area, void*);

// Names defined by this unit.
void __fastcall game_object_void_return_4_0060e760(void* self, void*, std::uint32_t);
void* __fastcall cgameobject_scalar_deleting_destructor_004c3040(void* self, void*, std::uint32_t flags);
void __fastcall cswcmodule_destructor_00641cd0(void* self, void*);
void* __fastcall cswcmodule_scalar_deleting_destructor_00644150(void* self, void*, std::uint32_t flags);
}
namespace kotor_client_exo_app { Word __stdcall client_get_application_id_005ed990(); }   // startup46/client_exo_app.cpp

namespace {
using Word = std::uint32_t;
using String = kotor_reconstruction::StartupString;
Word word_at(const void* o, Word off) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(o) + off, 4); return v; }
void put_word(void* o, Word off, Word v) { std::memcpy(static_cast<unsigned char*>(o) + off, &v, 4); }
void* pointer_at(const void* o, Word off) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word_at(o, off))); }
String* string_at(void* o, Word off) { return reinterpret_cast<String*>(static_cast<unsigned char*>(o) + off); }
const char empty_0073d71c[] = "";                                               // .rdata 0073D71C
}

// ============================================================================================================
// Shared slot 0060E760 `ret 4` (Ghidra void_return): D24 typed own body for this slot type (no exact-type existing
// extern "C" body: gui_load_slot_ret4_0060e760 takes gui_load::Object*, behavior_void_return_4_0060e760 char*).
extern "C" void __fastcall game_object_void_return_4_0060e760(void*, void*, std::uint32_t) {}

// ============================================================================================================
// 004C3040..004C305E CGameObject::`scalar deleting destructor'(flags) RET 4:
//  [esi]=007457A0 (004C3048); if (flags & 1) free(this) (004C304E..004C3056, 006FA390); return this (004C3059).
extern "C" void* __fastcall cgameobject_scalar_deleting_destructor_004c3040(void* self, void*, std::uint32_t flags) {
    put_word(self, 0, kotor_vtable_word(&kotor_vtable_cgameobject_007457a0));   // 004C3048
    if (flags & 1u) live_free(self);                                             // 004C3040 test byte [esp+4],1 / 004C3051
    return self;                                                                 // 004C3059
}

// 00641CD0..00641DA8 CSWCModule::~CSWCModule (thiscall, RET).
//  00641CEF [esi]=00751848; 00641CF5..00641D07 String(+10) = "" (005E5140, push 0073D71C);
//  00641D0C..00641D27 if [+48] (area): CSWCArea::UnloadArea 00609410, then [+48]->slot0(1), [+48]=0;
//  00641D2A..00641D37 if [+40]: [+40]->slot0(1), [+40]=0;  00641D3A..00641D4A if [+12C]: [+12C]->slot0(1), [+12C]=0;
//  00641D50..00641D5B ~String(+130) (005E5C20);
//  00641D60..00641D74 if [+118]: CExoArrayList::Remove(list = [+118]+4, value = this+118) (00445080);
//  00641D79..00641D81 ~String(+38); 00641D86..00641D8C ~String(+10); 00641D96 [esi]=007457A0 (inlined ~CGameObject).
//  CResRef members +18/+28 have trivial destruction.
// transition lane (review F1): the three deletes are now ported exactly. The former D83 gate argument no longer holds:
// CSWCModule::LoadModule 0063F660 is linked (1204-06) and sets +48h (0063F6C3) and +40h (0063F6E0), so every client
// module unload (005F8290 -> vtbl[0] 00644150 -> here) reached the traps. Each delete is a virtual call of slot 0
// (scalar deleting destructor) with flags 1 through the object's own table (area dtor and camera dtor are live).
namespace {
using ScalarDeletingDtor = void* (__fastcall*)(void* self, void*, std::uint32_t flags);
void delete_via_slot0(void* object) {                                           // mov reg,[ecx]; push 1; call [reg]
    (*static_cast<ScalarDeletingDtor* const*>(object))[0](object, nullptr, 1u);
}
}
extern "C" void __fastcall cswcmodule_destructor_00641cd0(void* self, void*) {
    put_word(self, 0, kotor_vtable_word(&kotor_vtable_cswcmodule_00751848));   // 00641CEF
    GuiAssign005e5140(string_at(self, 0x10), nullptr, empty_0073d71c);          // 00641CF5..00641D07
    if (void* area = pointer_at(self, 0x48)) {                                  // 00641D0C..00641D13
        recovered_client_area_unload_area_00609410(area, nullptr);              // 00641D15
        if (void* again = pointer_at(self, 0x48))                               // 00641D1A..00641D1F (re-read)
            delete_via_slot0(again);                                            // 00641D21..00641D25
        put_word(self, 0x48, 0);                                                // 00641D27
    }
    if (void* object40 = pointer_at(self, 0x40)) {                              // 00641D2A..00641D2F
        delete_via_slot0(object40);                                             // 00641D31..00641D35
        put_word(self, 0x40, 0);                                                // 00641D37
    }
    if (void* object12c = pointer_at(self, 0x12c)) {                            // 00641D3A..00641D42
        delete_via_slot0(object12c);                                            // 00641D44..00641D48
        put_word(self, 0x12c, 0);                                               // 00641D4A
    }
    string_at(self, 0x130)->~String();                                          // 00641D50..00641D5B
    if (void* holder = pointer_at(self, 0x118))                                 // 00641D60..00641D6E
        kotor_fastcall_00445080(static_cast<unsigned char*>(holder) + 4, nullptr,
                                static_cast<unsigned char*>(self) + 0x118);     // 00641D68 lea / 00641D70..00641D74
    string_at(self, 0x38)->~String();                                           // 00641D79..00641D81
    string_at(self, 0x10)->~String();                                           // 00641D86..00641D8C
    put_word(self, 0, kotor_vtable_word(&kotor_vtable_cgameobject_007457a0));   // 00641D96
}

// 00644150..0064416B CSWCModule::`scalar deleting destructor'(flags) RET 4:
//  ~CSWCModule (00644153 call 00641CD0); if (flags & 1) free(this) (00644158..00644165); return this (00644168).
extern "C" void* __fastcall cswcmodule_scalar_deleting_destructor_00644150(void* self, void*, std::uint32_t flags) {
    cswcmodule_destructor_00641cd0(self, nullptr);                              // 00644153
    if (flags & 1u) live_free(self);                                            // 00644158 test byte [esp+8],1 / 00644160
    return self;                                                                // 00644168
}

// ---- typed tables ------------------------------------------------------------------------------------------
#define M50_Z &behavior_null_return_0063e7f0
// 007457A0 CGameObject_vtable: 004C3040 0060E760 005B5E90, then 0063E7F0 x25 (007457AC..0074580C).
extern "C" const kotor_game_object::Methods kotor_vtable_cgameobject_007457a0 = {
    &cgameobject_scalar_deleting_destructor_004c3040, &game_object_void_return_4_0060e760,
    &light_manager_void_return_8_005b5e90,
    {M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z,
     M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z}};
// 00751848 CSWCModule_vtable: 00644150 0060E760 005B5E90, 0063E7F0 x4 (+0C..+18), 00641DB0 (+1C),
// 0063E7F0 x20 (+20..+6C, 00751868..007518B4); the next word 007518B8 is double data.
extern "C" const kotor_game_object::Methods kotor_vtable_cswcmodule_00751848 = {
    &cswcmodule_scalar_deleting_destructor_00644150, &game_object_void_return_4_0060e760,
    &light_manager_void_return_8_005b5e90,
    {M50_Z, M50_Z, M50_Z, M50_Z, &behavior_self_return_00641db0,
     M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z,
     M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z, M50_Z}};
#undef M50_Z

namespace kotor_game_object {
const Methods* module_methods_of(const void* module) {
    if (word_at(module, 0) == kotor_vtable_word(&kotor_vtable_cswcmodule_00751848)) return &kotor_vtable_cswcmodule_00751848;
    TerminateProcess(GetCurrentProcess(), 86);   // D41 contract violation code (unknown receiver table)
    return nullptr;
}
}

// ============================================================================================================
// CServerExoApp_vtable 00744184 (ten slots): gfxapply 2026-10-20 - the single definition of
// _kotor_vtable_cserverexoapp_00744184 now lives in 0845 server_outer_native_packet (the ctor's Native::vtable and
// ~CServerExoApp 004AEC70 both install it), with every slot bound to its real body; this unit's former copy (four
// D83 trap stubs for 004AE870 / 004AE840 / 004AE880 / 004AECA0) is removed. m50_vtables.h keeps the declaration.
