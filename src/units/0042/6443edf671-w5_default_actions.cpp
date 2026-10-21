// W5 t4: CClientExoAppInternal::GetDefaultActions 00620620 (the action list for the object under the cursor, reached
// from HandleMouseOver 006222F0 on normal in-game frames), the CSWGuiInterfaceAction list helpers it uses, and the
// cursor choice 0061FAA0. Transcribed from the original (read-only).
//
// CSWGuiInterfaceAction (38h bytes): +0 CExoString label, +8 action id, +C member-function pointer (16 bytes:
// function, 0, 0, 0), +1C target id, +20 CResRef icon, +30 flags, +34 dword.
// The menu-action handlers stored in +C (ActionInitiateDialog 0060F620, UsePlaceable 00682660, BashPlaceable 006826A0,
// ToggleDoorState 00683D90, MenuActionBash 00683E90, ActionMenuDisableMine 00691900, ActionMenuRecoverMine 00691950,
// ActionMenuAttack 00616800) have no implementation in any lane yet: their symbols resolve to explicit traps
// (w5_t4_foreign_traps.cpp) and are only CALLED when the player clicks the action.
#include "2c75bb10c4-w5_t3.hpp"
#include "6bff443bba-array_iterator.h"
#include <cstdint>
#include <cstring>

using namespace w5;

namespace kotor_menu20 { struct String; String* __fastcall AssignText005e5140(String*, void*, const char*); }   // 0262
namespace menu_resources { struct Ref; }
namespace resource_ref_assignment { menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*, void*, menu_resources::Ref*, const menu_resources::Ref*); } // 0278
namespace menu_tables_m41 {
using Constructor = void* (__fastcall*)(void*, void*);
using Destructor = void (__fastcall*)(void*, void*);
void __stdcall VectorConstruct_006fb55f(void* destination, unsigned size, unsigned count, Constructor ctor, Destructor dtor); // 0366
}
namespace kotor_input_cells_required { extern volatile int word_008338f4; }                                          // [008338F4]
namespace kotor_input_required { void* __fastcall get_in_game_gui_005ed690(void* app, void*); }

extern "C" {
// ---- existing providers (build/1122 map) ----
void __fastcall c2da_current_string_destroy_005e5c20(void* self, void*);                   // ~CExoString (0377)
void* __fastcall kotor_fastcall_005ed580(void* app, void*, U id);                          // CClientExoApp::GetGameObject (0701)
void* __fastcall kotor_fastcall_00635460(void* party, void*);                              // CSWParty::GetPlayerCharacter
void* __fastcall kotor_fastcall_005edeb0(void* app, void*, void* out, U id);               // CClientExoApp::GetGUIString (0581)
void __fastcall kotor_fastcall_0059ea30(void* self, void*);                                // element dtor (jmp ~CExoString) (0366)
// Generic array iterator declaration comes from array_iterator.h.
void* __fastcall unresolved_ref_assign_00406290(void* self, void*, void* out, const char* text);  // CResRef::operator=(char*) (0081)
void* __fastcall frame_GetServerArea_00605640(void* client_area, void*);                    // CSWCArea::GetServerArea
U __fastcall kotor_m15b_creature_stats_get_can_use_skill_006477e0(void* stats, void*, U skill); // CSWCCreatureStats::GetCanUseSkill
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                 // CServerExoApp::GetPlayerCreatureId
void* __fastcall kotor_fastcall_00683ee0(void* door, void*);                               // CSWCDoor::GetServerDoor (W4 t1)
void* __fastcall kotor_fastcall_00682e00(void* placeable, void*);                          // CSWCPlaceable::GetServerPlaceable (W4 t1)
U __fastcall kotor_fastcall_0061f790(void* door, void*);                                   // CSWCDoor::GetIsOpen (W5 t3)
// ---- menu-action handlers (addresses stored, not called here; trap fallbacks in w5_t4_foreign_traps.cpp) ----
void __fastcall kotor_fastcall_0060f620(void*, void*, U, void*);   // CSWCCreature::ActionInitiateDialog
void __fastcall kotor_fastcall_00682660(void*, void*, U, void*);   // CSWCPlaceable::UsePlaceable
void __fastcall kotor_fastcall_006826a0(void*, void*, U, void*);   // CSWCPlaceable::BashPlaceable
void __fastcall kotor_fastcall_00683d90(void*, void*, U, void*);   // CSWCDoor::ToggleDoorState
void __fastcall kotor_fastcall_00683e90(void*, void*, U, void*);   // CSWCDoor::MenuActionBash
void __fastcall kotor_fastcall_00691900(void*, void*, U, void*);   // CSWCTrigger::ActionMenuDisableMine
void __fastcall kotor_fastcall_00691950(void*, void*, U, void*);   // CSWCTrigger::ActionMenuRecoverMine
void __fastcall kotor_fastcall_00616800(void*, void*, U, void*);   // CSWCCreature::ActionMenuAttack

// ---- W5 t4 exports ----
void __fastcall kotor_fastcall_0063c000(void* action, void*);                              // CSWGuiInterfaceAction::Clear
void* __fastcall kotor_fastcall_004eae30(void* action, void*);                             // CSWGuiInterfaceAction ctor
void* __fastcall kotor_fastcall_004effa0(void* action, void*, const void* other);          // CSWGuiInterfaceAction::operator=
void __fastcall kotor_fastcall_004f7920(void* list, void*, I capacity);                    // CExoArrayList<CSWGuiInterfaceAction>::Allocate
void __fastcall kotor_fastcall_004fddd0(void* list, void*, I size);                        // CExoArrayList<CSWGuiInterfaceAction>::SetSize
U __fastcall kotor_fastcall_005ee0f0(void* app, void*);                                    // CClientExoApp::GetTargetChanging
U __fastcall kotor_fastcall_0060fcc0(void* creature, void*, void* object);                 // CSWCCreature::GetInterfaceTargetType
void __fastcall kotor_fastcall_00620620(void* ci, void*);                                  // CClientExoAppInternal::GetDefaultActions
U __fastcall kotor_fastcall_0061faa0(void* ci, void*, U action, U multiple);               // action id -> mouse cursor
}

namespace {
constexpr U k_none = 0x7f000000u;
constexpr U k_action_size = 0x38;
const char k_empty_0073d71c[] = "";
inline void* app() { return client_app(); }
inline unsigned char* actions(void* ci) { return at<unsigned char*>(ci, 0x4c8); }
// Original EH states release the constructed CExoString after the next call throws.
struct StringCleanup {
    void* slot;
    ~StringCleanup() noexcept(false) { c2da_current_string_destroy_005e5c20(slot, nullptr); }
};
inline U addr_of(void (__fastcall* f)(void*, void*, U, void*)) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(f)); }
// one menu entry: label (GUI string), id, handler {f,0,0,0}, target, icon
void fill(void* ci, U index, U gui_string, U id, U handler, U target, const char* icon) {
    {
    alignas(4) unsigned char temp[8];                                             // CExoString temporary
    void* s = kotor_fastcall_005edeb0(app(), nullptr, temp, gui_string);          // GetGUIString
    unsigned char* a = actions(ci) + index * k_action_size; // original reload after GetGUIString
    StringCleanup cleanup{temp}; // GetDefaultActions states 0/1/3..6/8..12; armed after return-slot construction
    async_assign_string_005e5c50(a, nullptr, s);                                  // CExoString::operator=
    }
    unsigned char* a = actions(ci) + index * k_action_size;
    at<U>(a, 8) = id;
    at<U>(a, 0xc) = handler; at<U>(a, 0x10) = 0; at<U>(a, 0x14) = 0; at<U>(a, 0x18) = 0;
    at<U>(a, 0x1c) = target;
    alignas(4) unsigned char ref_temp[16];
    unresolved_ref_assign_00406290(a + 0x20, nullptr, ref_temp, icon);            // CResRef::operator=(char*)
}
// party member that the player controls may not use / bash while a dead party member lies on it:
// 006209A3..A5F / 00620B2D..C26
void dead_member_flags(void* ci, void* pc, void* object, U index) {
    (void)ci;
    void* server = at<void*>(manager(), 8);
    const U pid = kotor_fastcall_004aea40(server, nullptr);
    void* client_obj = kotor_fastcall_004ae910(server, nullptr, pid);
    const U id = kotor_fastcall_005eda50(app(), nullptr, at<U>(client_obj, 0x38));
    void* g = kotor_fastcall_005ed580(app(), nullptr, id);
    if (g == nullptr || vcall<void*>(g, 0x2c) == nullptr) return;
    void* cr = vcall<void*>(g, 0x2c);
    if (at<U>(pc, 4) == id) return;
    if (at<U>(cr, 0x3a4) == 0) return;
    if (kotor_m15a_creature_get_server_creature_0060fb20(cr, nullptr) == nullptr) return;
    if (recovered_creature_incapacitated_004ef890(kotor_m15a_creature_get_server_creature_0060fb20(cr, nullptr), nullptr) == 0) return;
    if (kotor_fastcall_00682e00(vcall<void*>(object, 0x4c), nullptr) == nullptr) return;
    if (at<U>(kotor_fastcall_00682e00(vcall<void*>(object, 0x4c), nullptr), 0x218) != 0) return;
    unsigned char* a = actions(ci) + index * k_action_size;
    at<U>(a, 0x30) &= 0xfffffffeu;
    at<U>(a, 0x30) = (at<U>(a, 0x30) & 0xffffffedu) | 0xc;
}
}  // namespace

extern "C" {

// 0063C000 CSWGuiInterfaceAction::Clear
void __fastcall kotor_fastcall_0063c000(void* a, void*) {
    kotor_menu20::AssignText005e5140(static_cast<kotor_menu20::String*>(a), nullptr, k_empty_0073d71c);  // 0063C006..0D
    at<U>(a, 0xc) = 0; at<U>(a, 0x10) = 0; at<U>(a, 0x14) = 0;                    // 0063C012..2C
    at<U>(a, 8) = 0xffffffffu; at<U>(a, 0x18) = 0xffffffffu;                      // 0063C033..36
    at<U>(a, 0x1c) = k_none;                                                       // 0063C039
    alignas(4) unsigned char ref_temp[16];
    unresolved_ref_assign_00406290(static_cast<unsigned char*>(a) + 0x20, nullptr, ref_temp, k_empty_0073d71c);   // 0063C040
    at<U>(a, 0x30) = (at<U>(a, 0x30) & 0xffffffe1u) | 1;                            // 0063C045..4F
    at<U>(a, 0x34) = 1;                                                             // 0063C052
}
// 004EAE30 CSWGuiInterfaceAction::CSWGuiInterfaceAction
void* __fastcall kotor_fastcall_004eae30(void* a, void*) {
    kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(a), nullptr);  // 004EAE4D
    try {
    c2da_default_ref_00405ed0(static_cast<unsigned char*>(a) + 0x20, nullptr);   // 004EAE5D
    kotor_fastcall_0063c000(a, nullptr);                                            // 004EAE64
    } catch (...) { c2da_current_string_destroy_005e5c20(a, nullptr); throw; } // 0071AF40
    return a;
}
// 004EFFA0 CSWGuiInterfaceAction::operator=
void* __fastcall kotor_fastcall_004effa0(void* a, void*, const void* o) {
    async_assign_string_005e5c50(a, nullptr, const_cast<void*>(o));                  // 004EFFAC
    at<U>(a, 8) = at<const U>(const_cast<void*>(o), 8);                              // 004EFFB1..B4
    std::memcpy(static_cast<unsigned char*>(a) + 0xc, static_cast<const unsigned char*>(o) + 0xc, 16);  // 004EFFB7..D0
    at<U>(a, 0x1c) = at<const U>(const_cast<void*>(o), 0x1c);                         // 004EFFD3..E2
    alignas(4) unsigned char ref_temp[16];
    resource_ref_assignment::assign_00406120(reinterpret_cast<menu_resources::Ref*>(static_cast<unsigned char*>(a) + 0x20), nullptr,
        reinterpret_cast<menu_resources::Ref*>(ref_temp),
        reinterpret_cast<const menu_resources::Ref*>(static_cast<const unsigned char*>(o) + 0x20));   // 004EFFD6..E5
    U f = at<U>(a, 0x30);
    const U of = at<const U>(const_cast<void*>(o), 0x30);
    f ^= (f ^ of) & 1;                                                              // 004EFFEA..FA
    f ^= (of ^ f) & 0x1e;                                                           // 004EFFFD..07
    at<U>(a, 0x30) = f;
    at<U>(a, 0x34) = at<const U>(const_cast<void*>(o), 0x34);                         // 004F000A..0E
    return a;
}
// 004F7920 CExoArrayList<CSWGuiInterfaceAction>::Allocate(capacity): new[] (count word before the elements)
void __fastcall kotor_fastcall_004f7920(void* list, void*, I capacity) {
    unsigned char* old = at<unsigned char*>(list, 0);                               // 004F7944 (EBP)
    at<I>(list, 8) = capacity;                                                      // 004F794A
    unsigned char* block = static_cast<unsigned char*>(live_operator_new(static_cast<U>(capacity) * k_action_size + 4)); // 004F793D..4D
    unsigned char* data = nullptr;
    if (block != nullptr) {                                                          // 004F795B..61
        at<I>(block, 0) = capacity;                                                  // 004F7974
        data = block + 4;
        try {
        menu_tables_m41::VectorConstruct_006fb55f(data, k_action_size, static_cast<unsigned>(capacity),
            reinterpret_cast<menu_tables_m41::Constructor>(&kotor_fastcall_004eae30), &kotor_fastcall_0059ea30);
        } catch (...) { live_free(block); throw; } // 0071B270: iterator owns constructed-prefix cleanup   // 004F7963..76
    }
    at<unsigned char*>(list, 0) = data;                                              // 004F797E
    for (I i = 0; i < at<I>(list, 4); ++i)                                           // 004F797B..A6
        kotor_fastcall_004effa0(data + i * k_action_size, nullptr, old + i * k_action_size);
    if (old != nullptr) {                                                            // 004F79A8..C6
        c2da_destroy_array_006fa852(old, k_action_size, at<U>(old - 4, 0), &kotor_fastcall_0059ea30);
        live_free(old - 4);
    }
}
// 004FDDD0 CExoArrayList<CSWGuiInterfaceAction>::SetSize(size)
void __fastcall kotor_fastcall_004fddd0(void* list, void*, I size) {
    if (size == 0) {                                                                 // 004FDDD5..DA
        unsigned char* data = at<unsigned char*>(list, 0);
        if (data == nullptr) { at<I>(list, 4) = 0; return; }                         // 004FDDDC..E0 -> 004FDE16
        c2da_destroy_array_006fa852(data, k_action_size, at<U>(data - 4, 0), &kotor_fastcall_0059ea30); // 004FDDE2..F2
        live_free(data - 4);                                                         // 004FDDF7
        at<void*>(list, 0) = nullptr; at<I>(list, 8) = 0; at<I>(list, 4) = 0;         // 004FDE01..06
        return;
    }
    kotor_fastcall_004f7920(list, nullptr, size);                                    // 004FDE0E..11
    at<I>(list, 4) = size;                                                           // 004FDE16
}
// 005EE0F0 CClientExoApp::GetTargetChanging: internal +378 > 0
U __fastcall kotor_fastcall_005ee0f0(void* a, void*) {
    void* internal = at<void*>(a, 4);
    const float zero = 0.0f; // original literal0073D700, preserved x87 memory comparison
    unsigned short status;
    __asm {
        mov ecx, internal
        fld dword ptr [ecx + 378h]
        fcomp zero
        fnstsw status
    }
    return (static_cast<U>(status) & 0x4100u) == 0 ? 1u : 0u; // original TEST AH,41h / JNE
}
// 0060FCC0 CSWCCreature::GetInterfaceTargetType(object): 0 none, 1 door, 2 trigger, 3 placeable / other, 4 hostile
U __fastcall kotor_fastcall_0060fcc0(void*, void*, void* o) {
    if (o == nullptr) return 0;                                                      // 0060FCC8..CA
    if (vcall<void*>(o, 0x2c) != nullptr) {                                          // 0060FCD0..DD creature
        if (vcall<U>(vcall<void*>(o, 0x2c), 0x138) != 0) return 4;                   // 0060FCDF..EE
        return kotor_fastcall_005ee0f0(app(), nullptr) != 0 ? 4u : 3u;               // 0060FCF0..FF
    }
    if (vcall<void*>(o, 0x14) != nullptr) return 1;                                  // 0060FD0B..19 door
    if (vcall<void*>(o, 0x4c) != nullptr) {                                          // 0060FD1C..27 placeable
        void* sp = kotor_fastcall_00682e00(vcall<void*>(o, 0x4c), nullptr);          // 0060FD29..34
        if (sp != nullptr && at<U>(sp, 0x324) != 0) return 1;                        // 0060FD39..45
        return 3;
    }
    if (vcall<void*>(o, 0x3c) != nullptr) return 2;                                  // 0060FD51..5B trigger
    return 0;
}
// 0061FAA0 action id (low 28 bits) -> cursor; the second argument is not read. RET 8.
U __fastcall kotor_fastcall_0061faa0(void*, void*, U action, U) {
    const U v = action & 0x0fffffffu;                                                // 0061FAA4
    if (v > 0x3f2) {                                                                 // 0061FAA9..AE
        const U k = v - 0x3f4;
        if (k > 0xe) return 5;                                                       // 0061FAEE..F6
        static const unsigned char index_0061fb3c[15] = {0, 1, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3};
        static const U value_0061fb28[5] = {0x21, 7, 0x19, 0x25, 5};
        return value_0061fb28[index_0061fb3c[k]];                                    // 0061FAF8..FF
    }
    if (v == 0x3f2) return 0x17;                                                     // 0061FAE6
    if (v > 0x3ea) return v == 0x3eb ? 0x33u : 5u;                                   // 0061FAD7..DE
    if (v == 0x3ea) return 0xb;                                                      // 0061FACF
    if (v == 0x3e9) return 0x19;                                                     // 0061FAC0..C7
    return 5;                                                                        // 0061FB1E (the -1 test can never match)
}

// 00620620 CClientExoAppInternal::GetDefaultActions: rebuilds +4C8 (data/+4CC count/+4D0 alloc) for the in-game GUI's
// target (main interface +64).
void __fastcall kotor_fastcall_00620620(void* ci, void*) {
    void* pc = kotor_fastcall_00635460(kotor_fastcall_005ed8b0(app(), nullptr), nullptr);   // 00620651..58 [esp+18]
    void* pcs = pc ? kotor_m15a_creature_get_server_creature_0060fb20(pc, nullptr) : nullptr;   // [esp+10]
    void* main_if = at<void*>(kotor_input_required::get_in_game_gui_005ed690(app(), nullptr), 0x90);
    void* target = kotor_fastcall_005ed580(app(), nullptr, at<U>(main_if, 0x64));   // 00620678..99
    if (pc == nullptr) return;                                                       // 0062069E..A2
    if (pcs != nullptr && vcall<U>(pcs, 0x94) == 0) (void)recovered_creature_incapacitated_004ef890(pcs, nullptr); // 006206A8..C0 (unused)
    if (target == nullptr) return;
    void* o = vcall<void*>(target, 0x0c);                                            // 006206CD..D4
    if (o == nullptr) return;
    const U type = kotor_fastcall_0060fcc0(pc, nullptr, o);                          // 006206DE..E6 [esp+14]
    for (I i = 0; i < at<I>(ci, 0x4cc); ++i) kotor_fastcall_0063c000(actions(ci) + i * k_action_size, nullptr);  // 006206EA..23
    void* list = static_cast<unsigned char*>(ci) + 0x4c8;
    if (unsigned char* data = actions(ci)) {                                         // 00620725..58
        c2da_destroy_array_006fa852(data, k_action_size, at<U>(data - 4, 0), &kotor_fastcall_0059ea30);
        live_free(data - 4);
        at<void*>(ci, 0x4c8) = nullptr; at<I>(ci, 0x4d0) = 0;
    }
    at<I>(ci, 0x4cc) = 0;                                                            // 00620766
    switch (type) {                                                                  // table 006211F0
    case 0: {                                                                        // 0062077A no target
        kotor_fastcall_004f7920(list, nullptr, 1);
        at<I>(ci, 0x4cc) = 1;
        fill(ci, 0, 0x7dec, 0x404, 0, k_none, "i_noaction");
        at<U>(actions(ci), 0x18) = 0xffffffffu;                                      // 006207DB (EDI = -1)
        return;
    }
    case 1: case 3: {                                                                // 00620801
        if (vcall<void*>(o, 0x2c) != nullptr) {                                      // creature: dialog
            kotor_fastcall_004fddd0(list, nullptr, 1);
            fill(ci, 0, 0x173, 0x3ea, addr_of(&kotor_fastcall_0060f620), at<U>(o, 4), "i_dialog");
        }
        if (vcall<void*>(o, 0x4c) == nullptr) {                                      // 006208A5 -> 00620DBE door
            if (vcall<void*>(o, 0x14) == nullptr) return;
            (void)kotor_fastcall_00683ee0(vcall<void*>(o, 0x14), nullptr);           // 00620DD4 (unused)
            const U hostile_area = at<U>(frame_GetServerArea_00605640(at<void*>(o, 0x20), nullptr), 0x2b0);   // [esp+14]
            alignas(4) unsigned char local[8];
            kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(local), nullptr);
            StringCleanup local_cleanup{local}; // GetDefaultActions state7
            void* door = vcall<void*>(o, 0x14);
            if (kotor_fastcall_0061f790(door, nullptr) == 0 && at<U>(vcall<void*>(o, 0x14), 0x138) == 0) {   // 00620E04..25
                kotor_fastcall_004fddd0(list, nullptr, 1);
                fill(ci, 0, 0x16d, 0x3f2, addr_of(&kotor_fastcall_00683d90), at<U>(o, 4), "i_opendoor");
                if (at<U>(vcall<void*>(o, 0x14), 0x104) == 0 && hostile_area == 0 && at<U>(vcall<void*>(o, 0x14), 0x108) != 0) {
                    kotor_fastcall_004fddd0(list, nullptr, 2);
                    fill(ci, 1, 0x170, 0x3f5, addr_of(&kotor_fastcall_00683e90), at<U>(o, 4), "i_attack");
                }
            }
            return;
        }
        void* sp = kotor_fastcall_00682e00(vcall<void*>(o, 0x4c), nullptr);          // 006208AB..BB (EBX)
        const U hostile_area = at<U>(frame_GetServerArea_00605640(at<void*>(vcall<void*>(o, 0x4c), 0x20), nullptr), 0x2b0); // [esp+10]
        alignas(4) unsigned char local[8];
        kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(local), nullptr);
        StringCleanup local_cleanup{local}; // GetDefaultActions state2
        U n = 0;                                                                      // EBP
        if (sp != nullptr && at<U>(sp, 0x324) != 0) {                                // 006208DB..F6
            if (at<U>(vcall<void*>(o, 0x4c), 0x128) != 0) {                          // 006208FC..0B
                kotor_fastcall_004fddd0(list, nullptr, 1);
                fill(ci, 0, 0x16d, 0x3f7, addr_of(&kotor_fastcall_00682660), at<U>(o, 4), "i_openplace");
                dead_member_flags(ci, pc, o, 0);
                n = 1;
            }
            if (at<U>(vcall<void*>(o, 0x4c), 0x110) == 0 && hostile_area == 0 && at<U>(vcall<void*>(o, 0x4c), 0x118) != 0) { // 00620A67..97
                kotor_fastcall_004fddd0(list, nullptr, static_cast<I>(n + 1));
                fill(ci, n, 0x170, 0x3f5, addr_of(&kotor_fastcall_006826a0), at<U>(o, 4), "i_attack");
                dead_member_flags(ci, pc, o, n);
            }
        } else {                                                                     // 00620C2E
            if (at<U>(vcall<void*>(o, 0x4c), 0x128) != 0 && sp != nullptr && at<U>(sp, 0x44c) == 0) {
                kotor_fastcall_004fddd0(list, nullptr, 1);
                fill(ci, 0, 0x16e, 0x3f7, addr_of(&kotor_fastcall_00682660), at<U>(o, 4), "i_useplace");
                n = 1;
            }
            if (at<U>(vcall<void*>(o, 0x4c), 0x110) == 0 && hostile_area == 0) {     // 00620CE3..FE
                kotor_fastcall_004fddd0(list, nullptr, static_cast<I>(n + 1));
                fill(ci, n, 0x170, 0x3f5, addr_of(&kotor_fastcall_006826a0), at<U>(o, 4), "i_attack");
            }
        }
        return;
    }
    case 2: {                                                                        // 00620F97 trigger (mine)
        if (vcall<void*>(o, 0x3c) == nullptr) return;
        U n = 0;
        if (kotor_m15b_creature_stats_get_can_use_skill_006477e0(at<void*>(pc, 0x2f8), nullptr, 1) != 0 &&
            vcall<U>(vcall<void*>(o, 0x3c), 0x138) != 0) {                           // 00620FA6..D2
            kotor_fastcall_004fddd0(list, nullptr, 1);
            fill(ci, 0, 0x172, 0x3f4, addr_of(&kotor_fastcall_00691900), at<U>(o, 4), "i_disablemine");
            n = 1;
        }
        if (kotor_m15b_creature_stats_get_can_use_skill_006477e0(at<void*>(pc, 0x2f8), nullptr, 1) == 0) return;   // 00621067..7A
        kotor_fastcall_004fddd0(list, nullptr, static_cast<I>(n + 1));
        fill(ci, n, 0x5fb, 0x402, addr_of(&kotor_fastcall_00691950), at<U>(o, 4), "i_recovermine");
        return;
    }
    case 4: {                                                                        // 00621114 hostile creature
        if (vcall<void*>(o, 0x2c) == nullptr) return;
        void* sc = kotor_m15a_creature_get_server_creature_0060fb20(vcall<void*>(o, 0x2c), nullptr);
        if (sc == nullptr) return;
        if (at<U>(recovered_object_area_004cb120(sc, nullptr), 0x2b0) != 0) return;   // 00621139..48
        kotor_fastcall_004fddd0(list, nullptr, 1);
        fill(ci, 0, 0x177, 0x3eb, addr_of(&kotor_fastcall_00616800), at<U>(o, 4), "i_attack");
        return;
    }
    default:
        return;                                                                      // 0062076D (type > 4)
    }
}
}  // extern "C"
