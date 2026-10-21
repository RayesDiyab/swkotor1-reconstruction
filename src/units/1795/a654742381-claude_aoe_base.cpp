// claude_aoe_base.cpp (lane "aoe"): CSWSObject base pieces the AoE / store classes need, transcribed from the original
// swkotor.exe (read-only), plus shared helpers with no callable provider in the c31 candidate.
//
//   004CFCB0 CSWSObject::CSWSObject(type, id, flag)      RET 0Ch -> @kotor_fastcall_004cfcb0@20
//   004CC3D0 CExoLinkedList<CSWSObjectActionNode> ctor   RET 0   -> @kotor_fastcall_004cc3d0@8
//   00745F18 CSWSObject vtable (56 slots)                         -> _kotor_vtable_00745f18
//   inherited CSWSObject virtual slots (the 0963 bodies are file-local / Ports-only, so they are rebuilt here):
//   004CC390 ClearAction RET 8, 004CCF60 SetAnimation RET 4, 004D0170 GetDialogResref RET 4, 004CB190 GetGender RET 0,
//   004D0190 GetLastName RET 0, 004CB810 GetDead RET 0, 004D01A0 GetMaxHitPoints RET 4, 004CAEC0 GetCurrentHitPoints
//   RET 4, 004CAEE0 GetDamageImmunity RET 4, 004CAF70 GetDamageImmunityByFlags RET 4, 004D01B0
//   GetLockOrientationToObject RET 0, 004D01C0 GetPortrait RET 4, 004D09C0 CSWSObject scalar deleting dtor RET 4.
//   shared helpers: 004B0850 CServerAIMaster::AddObject RET 8,
//   0056D140 CSWSMessage::SendServerToPlayerArea_VisualEffect RET 14h.
//
// Conventions: thiscall -> extern "C" __fastcall(self, unused_edx, ...); game heap only (live_operator_new/live_free);
// no original address is dereferenced (rdata constants are written as their proven literal values).
// EH: the original ctor's unwind funclets only run when a callee throws; no provider bound here throws (operator new
// 006FA7E6 returns NULL on failure), so the unwind states are documented, not modelled.
#include "842d6b1618-w5_t3.hpp"

namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* string, void*); }   // CExoString() [0603]
namespace table29_required { void* __fastcall gui_game_object_004d7d60(void* self, void*, unsigned type, unsigned id); } // CGameObject [0712]
namespace recovered_server_create { void* construct_list_internal_005e9aa0(void* memory); }  // CExoLinkedListInternal() [0842]
namespace menu_resources { struct Ref { Ref* construct_00406d80(const char* text); }; }       // CResRef(char const*) [0030]
namespace frame_control { extern unsigned message_manager_007a3a00; }                        // g_pVirtualMachine [0456]

extern "C" {
using w5::U; using w5::I; using w5::U8; using w5::U16; using w5::Vector;
void* __fastcall exo_locstring_construct_005e9e10(void* self, void*);                          // CExoLocString() [0265]
void* __fastcall c2da_default_ref_00405ed0(void* self, void*);                                 // CResRef() [0263]
U __cdecl aurora_vector_equal_004aa980(const void* a, const void* b);                          // Vector operator== [0446]
void* __fastcall recovered_object_array_004aed70(void* server, void*);                         // GetObjectArray [w2_party_native]
U __fastcall recovered_area_update_00506fe0(void* area, void*, void* object);                 // CSWSArea::AddObjectToSubAreas? [0886]
U8 __fastcall area_vfx_register_existing_004d80e0(void* array, void*, U* id, void* object, U flag); // AddInternalObject RET 0Ch
unsigned char __fastcall kotor_fastcall_004d7e90(void* array, void*, U id, void* object);    // AddExternalObject [claude_0886_bodies_area]
void* __fastcall GuiAssign005e5140(void* string, void*, const char* text);                    // CExoString::operator=(char const*) [0093]
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                 // GetWorldTimer [0457]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* day, U* time);              // GetWorldTime [0457]
void __fastcall recovered_object_destroy_004d0220(void* self, void*);                          // CSWSObject::~CSWSObject [0963]
void* __fastcall frame_R4_005ed690(void* client, void*);                                       // CClientExoApp::GetInGameGui RET 0 [0457]
void __fastcall kotor_fastcall_0062ec60(void* gui, void*, U state);                            // CGuiInGame::SetGlobalDialogState RET 4 [dialogue_state]
U __fastcall recovered_ai_remove_004af3d0(void* ai, void*, void* object);                      // CServerAIMaster::RemoveObject [1070]
void __fastcall w1x_unique_append_006449d0(void* list, void*, U value);                        // CExoArrayList<ulong>::AddUnique [0886]
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b);              // CSWMessage::CreateWriteMessage [w2_message_native]
void __fastcall kotor_fastcall_004d6300(void* message, void*, U value, U bits);               // CSWMessage::WriteWORD
void __fastcall kotor_fastcall_004d6920(void* message, void*, U value_bits, U scale, U bits); // CSWMessage::WriteFLOAT
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);             // CSWMessage::GetWriteMessage
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage
// generic slot bodies (0832 / 0486 / 0209 / 0886 / 0963-export / effects lanes)
void __fastcall slot_void_return_4_0060e760_a1(void*, void*, U);                               // 0060E760 ret 4
void __fastcall kotor_m15e_slot008_void8_005b5e90(void*, void*, U, U);                         // 005B5E90 ret 8
U __fastcall kotor_m15e_slot010_zero_0063e7f0(void*, void*);                                   // 0063E7F0 return_zero
void* __fastcall kotor_m15e_slot00c_self_00641db0(void*, void*);                               // 00641DB0 self_return
U __fastcall kotor_m15e_slot0d4_true_0063e7a0(void*, void*);                                   // 0063E7A0 return_true
void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);                               // 004015A0 void_return
void __fastcall kotor_m15e_slot074_purecall_006fb8de(void*, void*);                            // 006FB8DE _purecall
void __fastcall kotor_m15e_slot078_purecall_006fb8de(void*, void*, U, U, U);                   // 006FB8DE _purecall
U __stdcall c2da_resource_constant_word_00406490();                                            // 00406490 return -1
void __fastcall kotor_fastcall_004ccf80(void* object, void*, I damage);                        // CSWSObject::DoDamage [effects g3]
int __fastcall kotor_fastcall_004cf160(void*, void*, void*, int, U, U, U);                     // DoDamageImmunity [0963 damage]
int __fastcall kotor_fastcall_004d09e0(void*, void*, void*, int, U, U, U);                     // DoDamageReduction
int __fastcall kotor_fastcall_004d0e40(void*, void*, void*, int, U, U, U, U);                  // DoDamageResistance
void __fastcall recovered_object_portrait_ref_004d01f0(void*, void*, void*);                    // SetPortrait [0886]
U __fastcall recovered_creature_portrait_004d0210(void*, void*);                                // GetPortraitId [creature_destructor_r2]
void __fastcall recovered_object_portrait_004cad20(void*, void*, U);                            // SetPortraitId [0886]
}

namespace {
using namespace w5;
constexpr U k_invalid_00746644 = 0x7f000000u;          // .rdata 00746644
constexpr U k_one_f = 0x3f800000u;                       // 1.0f
inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
}

extern "C" {
// ======================================================================= CSWSObject inherited virtual slots
// 004CC390 CSWSObject::ClearAction(CSWSObjectActionNode*, int): RET 8.
U __fastcall kotor_fastcall_004cc390(void* self, void*, void* node, U) {
    const U type = at<U>(node, 0);                                                               // 004CC390..94
    if (type == 0x18) {                                                                          // 004CC396..99
        void* gui = frame_R4_005ed690(client_app(), nullptr);                                    // 004CC39B..A1, A6
        kotor_fastcall_0062ec60(gui, nullptr, 0);                                                // 004CC3A4 push 0, 004CC3AB..AD
        return 1;                                                                                // 004CC3B2..B7
    }
    if (type == 0x20) at<U>(self, 0x50) = 0;                                                     // 004CC3BA..BF
    return 1;                                                                                    // 004CC3C6..CB
}
// 004CCF60 CSWSObject::SetAnimation(int): RET 4.
void __fastcall kotor_fastcall_004ccf60(void* self, void*, U animation) {
    at<U>(self, 0x1fc) |= 1;                                                                     // 004CCF60
    at<U>(self, 0xd4) = animation;                                                               // 004CCF67..6B
}
// 004D0170 CSWSObject::GetDialogResref(): hidden CResRef* out, RET 4 -> CResRef("").
void* __fastcall kotor_fastcall_004d0170(void*, void*, void* out) {
    static_cast<menu_resources::Ref*>(out)->construct_00406d80("");                              // 004D0175..7C ([0073D71C] "")
    return out;                                                                                  // 004D0181
}
// 004CB190 CSWSObject::GetGender(): AL = byte [0073ECD0] = 00. RET 0.
U8 __fastcall kotor_fastcall_004cb190(void*, void*) { return 0; }                                // 004CB190
// 004D0190 CSWSObject::GetLastName(): &this->+0C (CExoLocString). RET 0.
void* __fastcall kotor_fastcall_004d0190(void* self, void*) { return bytes(self) + 0xc; }       // 004D0190
// 004D01A0 CSWSObject::GetMaxHitPoints(int): AX = word +E0. RET 4.
U16 __fastcall kotor_fastcall_004d01a0(void* self, void*, U) { return at<U16>(self, 0xe0); }    // 004D01A0
// 004CAEC0 CSWSObject::GetCurrentHitPoints(int flag): flag==1 -> AX = word +DC, else +E4 + +DC (dword). RET 4.
U __fastcall kotor_fastcall_004caec0(void* self, void*, U flag) {
    if (flag == 1) return at<U16>(self, 0xdc);                                                   // 004CAEC0..CE (upper EAX unspecified)
    return at<U>(self, 0xe4) + at<U>(self, 0xdc);                                                // 004CAED1..DD
}
// 004CB810 CSWSObject::GetDead(): RET 0.
U __fastcall kotor_fastcall_004cb810(void* self, void*) {
    if (vcall<void*>(self, 0x30) != nullptr &&                                                   // 004CB811..1A
        at<U>(vcall<void*>(self, 0x30), 0x9d4) != 0) {                                           // 004CB81C..2B
        void* c = vcall<void*>(self, 0x30);                                                      // 004CB82D..31
        const short hp = static_cast<short>(vcall<U>(c, 0x9c, 0u));                              // 004CB834..3A
        return hp <= -10 ? 1u : 0u;                                                              // 004CB840..46 (cmp ax,FFF6; setle)
    }
    if (vcall<void*>(self, 0x30) != nullptr) {                                                   // 004CB84D..58
        void* c = vcall<void*>(self, 0x30);                                                      // 004CB85A..5C
        return static_cast<short>(vcall<U>(c, 0x9c, 0u)) <= 0 ? 1u : 0u;                         // 004CB85F..70
    }
    return static_cast<short>(vcall<U>(self, 0x9c, 0u)) <= 0 ? 1u : 0u;                          // 004CB877..86
}
// 004CAEE0 CSWSObject::GetDamageImmunity(byte type): clamp((char)+1AC[type], -100, 100). RET 4.
int __fastcall kotor_fastcall_004caee0(void* self, void*, U type) {
    const signed char v = static_cast<signed char>(at<U8*>(self, 0x1ac)[type & 0xff]);           // 004CAEE0..EB
    if (v >= 100) return 100;                                                                    // 004CAEEE..F0, 004CAF09
    if (v < -100) return -100;                                                                   // 004CAEF2..FA
    return v;                                                                                    // 004CAF02..07
}
// 004CAF70 CSWSObject::GetDamageImmunityByFlags(word flags): minimum over set bits 0..14 of vtbl+B0(bit), clamped.
// Quirk kept: when a smaller value is found the virtual is called a second time for the stored value. RET 4.
int __fastcall kotor_fastcall_004caf70(void* self, void*, U flags) {
    short best = 0;                                                                              // 004CAF78 (bx)
    for (U i = 0; i < 0xf; ++i) {                                                                // 004CAF7C..B8
        if (((static_cast<U16>(flags) >> i) & 1u) == 0) continue;                                // 004CAF80..8A
        if (best == 0) {                                                                         // 004CAF8C..92
            best = static_cast<signed char>(vcall<int>(self, 0xb0, i));                          // 004CAFA8..B0
        } else if (static_cast<short>(static_cast<signed char>(vcall<int>(self, 0xb0, i))) < best) { // 004CAF94..A3
            best = static_cast<signed char>(vcall<int>(self, 0xb0, i));                          // 004CAFA5..B0
        }
    }
    if (best >= 100) return 100;                                                                 // 004CAFBA..BE, D4
    if (static_cast<int>(best) < -100) return -100;                                              // 004CAFC0..CB
    return best;                                                                                 // 004CAFD9
}
// 004D01B0 CSWSObject::GetLockOrientationToObject(): 7F000000. RET 0.
U __fastcall kotor_fastcall_004d01b0(void*, void*) { return k_invalid_00746644; }               // 004D01B0
// 004D01C0 CSWSObject::GetPortrait(): hidden CResRef* out <- 16 bytes at +20. RET 4.
void* __fastcall kotor_fastcall_004d01c0(void* self, void*, void* out) {
    std::memcpy(out, bytes(self) + 0x20, 16);                                                    // 004D01C4..DD (four dword moves)
    return out;
}
// 004D09C0 CSWSObject scalar deleting destructor: RET 4.
void* __fastcall kotor_fastcall_004d09c0(void* self, void*, U flags) {
    recovered_object_destroy_004d0220(self, nullptr);                                            // 004D09C1..C3
    if (flags & 1) live_free(self);                                                              // 004D09C8..D5
    return self;                                                                                 // 004D09D8
}

// 00745F18 CSWSObject vtable (56 slots, original order).
extern const void* const kotor_vtable_00745f18[56];
const void* const kotor_vtable_00745f18[56] = {
    reinterpret_cast<const void*>(&kotor_fastcall_004d09c0),                   // +00 ~CSWSObject (deleting)
    reinterpret_cast<const void*>(&slot_void_return_4_0060e760_a1),            // +04
    reinterpret_cast<const void*>(&kotor_m15e_slot008_void8_005b5e90),         // +08
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +0C
    reinterpret_cast<const void*>(&kotor_m15e_slot00c_self_00641db0),          // +10 AsSWSObject
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +14
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +18
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +1C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +20
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +24
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +28
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +2C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +30 AsSWSCreature
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +34
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +38
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +3C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +40
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +44
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +48
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +4C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +50
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +54
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +58
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +5C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +60
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +64
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +68
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +6C
    reinterpret_cast<const void*>(&kotor_m15e_slot074_purecall_006fb8de),      // +70 AIUpdate (pure)
    reinterpret_cast<const void*>(&kotor_fastcall_004cc390),                   // +74 ClearAction
    reinterpret_cast<const void*>(&kotor_m15e_slot078_purecall_006fb8de),      // +78 EventHandler (pure)
    reinterpret_cast<const void*>(&kotor_fastcall_004ccf60),                   // +7C SetAnimation
    reinterpret_cast<const void*>(&kotor_fastcall_004d0170),                   // +80 GetDialogResref
    reinterpret_cast<const void*>(&kotor_m15e_slot0d4_true_0063e7a0),          // +84 return_true
    reinterpret_cast<const void*>(&kotor_fastcall_004cb190),                   // +88 GetGender
    reinterpret_cast<const void*>(&kotor_fastcall_004d0190),                   // +8C GetLastName
    reinterpret_cast<const void*>(&kotor_fastcall_004d0190),                   // +90 GetLastName
    reinterpret_cast<const void*>(&kotor_fastcall_004cb810),                   // +94 GetDead
    reinterpret_cast<const void*>(&kotor_fastcall_004d01a0),                   // +98 GetMaxHitPoints
    reinterpret_cast<const void*>(&kotor_fastcall_004caec0),                   // +9C GetCurrentHitPoints
    reinterpret_cast<const void*>(&kotor_fastcall_004ccf80),                   // +A0 DoDamage
    reinterpret_cast<const void*>(&kotor_fastcall_004d09e0),                   // +A4 DoDamageReduction
    reinterpret_cast<const void*>(&kotor_fastcall_004d0e40),                   // +A8 DoDamageResistance
    reinterpret_cast<const void*>(&kotor_fastcall_004cf160),                   // +AC DoDamageImmunity
    reinterpret_cast<const void*>(&kotor_fastcall_004caee0),                   // +B0 GetDamageImmunity
    reinterpret_cast<const void*>(&kotor_fastcall_004caf70),                   // +B4 GetDamageImmunityByFlags
    reinterpret_cast<const void*>(&c2da_resource_constant_word_00406490),      // +B8 GetEffectSpellId (-1)
    reinterpret_cast<const void*>(&slot_void_return_4_0060e760_a1),            // +BC SetEffectSpellId (void)
    reinterpret_cast<const void*>(&slot_void_return_4_0060e760_a1),            // +C0
    reinterpret_cast<const void*>(&kotor_fastcall_004d01b0),                   // +C4 GetLockOrientationToObject
    reinterpret_cast<const void*>(&kotor_fastcall_004d01c0),                   // +C8 GetPortrait
    reinterpret_cast<const void*>(&recovered_object_portrait_ref_004d01f0),    // +CC SetPortrait
    reinterpret_cast<const void*>(&recovered_creature_portrait_004d0210),      // +D0 GetPortraitId
    reinterpret_cast<const void*>(&recovered_object_portrait_004cad20),        // +D4 SetPortraitId
    reinterpret_cast<const void*>(&kotor_m15e_slot090_void0_004015a0),         // +D8
    reinterpret_cast<const void*>(&slot_void_return_4_0060e760_a1),            // +DC
};

// ======================================================================= CSWSObject construction
// 004CC3D0 CExoLinkedList<CSWSObjectActionNode>::CExoLinkedList(): [this] = new CExoLinkedListInternal or 0. RET 0.
void* __fastcall kotor_fastcall_004cc3d0(void* self, void*) {
    void* memory = live_operator_new(0xc);                                                       // 004CC3E7..EB
    at<void*>(self, 0) = memory != nullptr ? recovered_server_create::construct_list_internal_005e9aa0(memory) : nullptr; // 004CC3F7..0A, 1E..24
    return self;
}

// 004CFCB0 CSWSObject::CSWSObject(byte type, ulong id, int flag): RET 0Ch.
void* __fastcall kotor_fastcall_004cfcb0(void* self, void*, U type, U id, U flag) {
    unsigned char* const s = bytes(self);
    table29_required::gui_game_object_004d7d60(self, nullptr, type, id);                         // 004CFCD2..DE CGameObject(type,id)
    at<const void*>(self, 0) = kotor_vtable_00745f18;                                            // 004CFCEC
    exo_locstring_construct_005e9e10(s + 0x0c, nullptr);                                         // 004CFCE5..F2
    kotor_movie::string_element_construct_005b3190(s + 0x18, nullptr);                           // 004CFCF7..D01
    c2da_default_ref_00405ed0(s + 0x20, nullptr);                                                // 004CFD06..0E
    at<U>(self, 0x90) = 0; at<U>(self, 0x94) = 0; at<U>(self, 0x98) = 0;                         // 004CFD13..1E
    at<U>(self, 0x9c) = 0; at<U>(self, 0xa0) = 0; at<U>(self, 0xa4) = 0;                         // 004CFD27..33
    kotor_fastcall_004cc3d0(s + 0xfc, nullptr);                                                  // 004CFD21, 39
    at<U>(self, 0x104) = 0; at<U>(self, 0x108) = 0; at<U>(self, 0x100) = 0; at<U>(self, 0x10c) = 0; // 004CFD3E..50
    // 004CFD56..61 CSWSScriptVarTable::CSWSScriptVarTable 0059AFE0 (+110): five zero dwords (inline view of 0059AFE0)
    for (U o = 0x110; o <= 0x120; o += 4) at<U>(self, o) = 0;
    at<U>(self, 0x128) = 0; at<U>(self, 0x12c) = 0; at<U>(self, 0x124) = 0;                      // 004CFD66..72
    at<U>(self, 0x134) = 0; at<U>(self, 0x138) = 0; at<U>(self, 0x130) = 0;                      // 004CFD78..84
    at<U>(self, 0x140) = 0; at<U>(self, 0x144) = 0; at<U>(self, 0x13c) = 0;                      // 004CFD8A..96
    kotor_movie::string_element_construct_005b3190(s + 0x184, nullptr);                          // 004CFD9C..A7
    at<U>(self, 0x190) = 0; at<U>(self, 0x194) = 0; at<U>(self, 0x18c) = 0;                      // 004CFDAC..B8
    at<U>(self, 0x1a4) = 0; at<U>(self, 0x1a8) = 0; at<U>(self, 0x1a0) = 0;                      // 004CFDBE..CA
    U zero[3] = {0, 0, 0};                                                                       // 004CFDED..F5 [esp+14..1C]
    at<U>(self, 0x1b0) = 0; at<U>(self, 0x1b4) = 0; at<U>(self, 0x1b8) = 0;                      // 004CFDDB..E7
    if (aurora_vector_equal_004aa980(s + 0x90, zero) == 0) {                                     // 004CFDD0..03
        at<U>(self, 0x1fc) |= 2;                                                                 // 004CFE05
        void* found = nullptr;                                                                   // 004CFE11 [esp+30]
        (void)kotor_fastcall_004d8230(recovered_object_array_004aed70(server_app(), nullptr), nullptr,
                                      at<U>(self, 0x8c), &found);                                // 004CFE0C..2B (status unused)
        void* area = found != nullptr ? vcall<void*>(found, 0x28) : nullptr;                     // 004CFE30..43
        at<U>(self, 0x90) = zero[0]; at<U>(self, 0x94) = zero[1]; at<U>(self, 0x98) = zero[2];   // 004CFE47..5C
        if (vcall<void*>(self, 0x30) != nullptr) (void)vcall<void*>(self, 0x30);                 // 004CFE58..6A (result unused)
        if (area != nullptr) (void)recovered_area_update_00506fe0(area, nullptr, self);          // 004CFE6D..76
    }
    at<U>(self, 0x9c) = k_one_f; at<U>(self, 0xa0) = 0; at<U>(self, 0xa4) = 0;                   // 004CFE7B..AB
    at<U>(self, 0x1fc) |= 4;                                                                     // 004CFEB1..BE
    at<U>(self, 0xd4) = 10000;                                                                   // 004CFEC4
    at<U>(self, 0xbc) = 0; at<U>(self, 0xc0) = 0;                                                // 004CFECE..D4
    at<U>(self, 0xb8) = k_one_f;                                                                 // 004CFEDA
    at<U>(self, 0x88) = 0;                                                                       // 004CFEE4
    void* objects = recovered_object_array_004aed70(server_app(), nullptr);                      // 004CFEEA..F0, 004CFF06/19
    if (id == k_invalid_00746644) {                                                              // 004CFEF3..FA
        (void)area_vfx_register_existing_004d80e0(objects, nullptr, &at<U>(self, 4), self, flag); // 004CFEFC..0D AddInternalObject
    } else {
        (void)kotor_fastcall_004d7e90(objects, nullptr, at<U>(self, 4), self);                   // 004CFF14..20 AddExternalObject
    }
    at<U>(self, 0xdc) = 1; at<U>(self, 0xe0) = 1;                                                // 004CFF25..30
    at<U>(self, 0xe4) = 0; at<U>(self, 0x34) = 0;                                                // 004CFF36..3C
    at<U>(self, 0x54) = k_invalid_00746644;                                                      // 004CFF3F..45
    at<U>(self, 0x8c) = k_invalid_00746644;                                                      // 004CFF48..55
    at<U>(self, 0x50) = 0; at<U>(self, 0x58) = 10001; at<U>(self, 0x5c) = 0;                     // 004CFF5B..65
    at<U>(self, 0xe8) = 1; at<U>(self, 0x19c) = 0;                                               // 004CFF68..6E
    GuiAssign005e5140(s + 0x18, nullptr, "");                                                    // 004CFF4E..74 ([0073D71C])
    at<U8>(self, 0x1d5) = 0xff; at<U8>(self, 0x1d4) = 0xff;                                      // 004CFF79..81
    at<U>(self, 0x1b0) = 0; at<U>(self, 0x1b4) = 0;                                              // 004CFF8A..B4
    at<U>(self, 0xec) = 1; at<U>(self, 0xf0) = 0; at<U>(self, 0xf4) = 0;                         // 004CFFBC..CC
    at<U>(self, 0xd8) = k_one_f;                                                                 // 004CFFD2
    at<U>(self, 0x78) = 0xffffffffu;                                                             // 004CFFDC
    at<U>(self, 0x17c) = k_invalid_00746644; at<U>(self, 0x54) = k_invalid_00746644;            // 004CFFDF..E5
    at<U>(self, 0x158) = k_invalid_00746644; at<U>(self, 0x160) = k_invalid_00746644;           // 004CFFE8..EE
    at<U>(self, 0x15c) = k_invalid_00746644; at<U>(self, 0x154) = k_invalid_00746644;           // 004CFFF4..FA
    at<U>(self, 0x164) = k_invalid_00746644;                                                     // 004D0000
    at<U>(self, 0x1d0) = 0; at<U>(self, 0x1c0) = 0xffffffffu; at<U8>(self, 0x1c4) = 0;           // 004D0006..12
    at<U>(self, 0x1bc) = k_invalid_00746644; at<U>(self, 0x1b8) = 0; at<U>(self, 0x1cc) = 0;     // 004D0018..24
    at<U>(self, 0x1dc) = 0xffffffffu; at<U>(self, 0x1e0) = 0;                                    // 004D002A..30
    at<U>(self, 0x1d8) = k_invalid_00746644; at<U>(self, 0x174) = k_invalid_00746644;           // 004D0036..3C
    at<U>(self, 0x178) = 0; at<U>(self, 0x14c) = 0xffffffffu;                                    // 004D0042..48
    at<void*>(self, 0x1ac) = live_operator_new(0xf);                                             // 004CFFBA, 004D004E..56
    for (U i = 0; i < 0xf; ++i) at<U8*>(self, 0x1ac)[i] = 0;                                     // 004D005C..6D (null store kept)
    at<void*>(self, 0x168) = live_operator_new(0x3c);                                            // 004D006F..79
    for (U i = 0; i < 0x3c; i += 4) *reinterpret_cast<U*>(at<U8*>(self, 0x168) + i) = 0xffffffffu; // 004D007F..90
    at<U16>(self, 0x16c) = 0; at<U8>(self, 0x16e) = 0;                                           // 004D0092..99
    at<U>(self, 0x170) = k_invalid_00746644; at<U>(self, 0xf8) = 0;                              // 004D009F..A5
    at<U16>(self, 0x14) = 0; at<U16>(self, 0x16) = 0;                                            // 004D00AB..AF
    at<U>(self, 0x1e4) = 0; at<U16>(self, 0x1e8) = 0; at<U>(self, 0x1ec) = 0; at<U>(self, 0x148) = 0; // 004D00B3..C6
    at<U16>(self, 0x30) = 0xffff;                                                                // 004D00CC
    for (U o = 0x1f0; o <= 0x20c; o += 4) at<U>(self, o) = 0;                                    // 004D00D2..FC (+1FC flags cleared: original)
    at<U>(self, 0x74) = 0; at<U>(self, 0x214) = 0; at<U>(self, 0x40) = 0; at<U>(self, 0x44) = 0; // 004D0102..0E
    at<U>(self, 0x70) = 0; at<U>(self, 0x60) = 0; at<U>(self, 0x64) = 0; at<U>(self, 0x218) = 0; // 004D0111..1A
    at<U>(self, 0x21c) = 1; at<U>(self, 0x224) = 0; at<U>(self, 0x220) = 0;                      // 004D0120..30
    frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server_app(), nullptr), nullptr,
                                &at<U>(self, 0xc4), &at<U>(self, 0xc8));                         // 004D0136..53
    return self;                                                                                 // 004D015D
}

// ======================================================================= shared helpers (dedupe candidates)
// 004B0850 CServerAIMaster::AddObject(CSWSObject*, int level): RET 8, EAX 0/1.
U __fastcall kotor_fastcall_004b0850(void* ai, void*, void* object, I level) {
    if (level > 4 || level < 0) return 0;                                                        // 004B0857..60 (signed)
    if (object == nullptr) return 0;                                                             // 004B0862..68
    if (at<I>(object, 0x78) != -1) (void)recovered_ai_remove_004af3d0(ai, nullptr, object);     // 004B086A..71
    w1x_unique_append_006449d0(bytes(ai) + static_cast<U>(level) * 16 + 4, nullptr, at<U>(object, 4)); // 004B0876..83
    at<I>(object, 0x78) = level;                                                                 // 004B0888
    return 1;
}
// 004EF6E0 CSWSCreature::CalculateSpellSaveDC: imported from claude_vm_vmA_endar.cpp (same reading).
// 0056D140 CSWSMessage::SendServerToPlayerArea_VisualEffect(CSWSPlayer*, ushort vfx, Vector p): message 4/2.
// RET 14h, EAX 0 when GetWriteMessage fails, else the send result.
U __fastcall kotor_fastcall_0056d140(void* message, void*, void* player, U vfx, float x, float y, float z) {
    kotor_fastcall_004d68a0(message, nullptr, 0xe, 0xffffffffu, 1);                             // 0056D144..4C
    kotor_fastcall_004d6300(message, nullptr, vfx, 0x10);                                        // 0056D151..5A
    kotor_fastcall_004d6920(message, nullptr, bits(x), k_one_f, 0x20);                           // 0056D15F..6D
    kotor_fastcall_004d6920(message, nullptr, bits(y), k_one_f, 0x20);                           // 0056D172..80
    kotor_fastcall_004d6920(message, nullptr, bits(z), k_one_f, 0x20);                           // 0056D185..93
    void* data = nullptr; U size = 0;                                                            // [esp+8] / [esp+4]
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;                  // 0056D198..B1
    return kotor_fastcall_00567e60(message, nullptr, at<U>(player, 4), 4, 2, data, size);        // 0056D1B4..CC
}
}
