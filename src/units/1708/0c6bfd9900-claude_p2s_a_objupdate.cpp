// Lane p2s-a: 005228D0 CSWSMessage::HandlePlayerToServerGameObjectUpdate (player-to-server major 5) and the
// callees that have no C-linkage implementation in the candidate (c16 map): 00503730 CGameEffect::SetString,
// 004ED6E0 CSWSCreature::UpdatePersonalSpace. Transcribed from the original swkotor.exe (read-only); addresses
// in comments are the original instructions.
// Handler ABI: original BOOL __thiscall CSWSMessage::HandlePlayerToServerGameObjectUpdate(CSWSPlayer* player,
// BYTE minor), RET 8 (sole ret 8 at 005231CE). Exported as the 0918 dispatcher calls handlers:
//   extern "C" unsigned __fastcall recovered_player_to_server_game_object_update_005228d0(void* message, void*,
//                                                                                        void* player, unsigned minor)
// EH: the original registers an SEH frame (handler 0071C4A1) whose states own the new CSWSItem (0), the
// CExoString locals (1, 3) and the new CGameEffect (2, 4); the unwind cleanups only run when a callee throws
// (none of the bound callees throws in normal operation) and are not reproduced.
// Conventions as W5: thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision;
// live_operator_new / live_free only.
#include "58f963adf7-w5_types.hpp"

using namespace w5;

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }       // [007A39FC] CAppManager*
namespace menu_resources {
struct Ref;
struct Manager { unsigned exists_00408bc0(Ref* ref, unsigned type, unsigned* table); }; // CExoResMan::Exists [0031]
}
namespace kotor_resource_demand { extern menu_resources::Manager* volatile resource_manager_007a39e8; } // [007A39E8]

extern "C" {
extern void* kotor_cell_rules_007a3a28;                 // [007A3A28] CSWRules*
extern unsigned kotor_rules_cell32_007a2118;            // .data 007A2118 appearance.2da column index [0431]
extern unsigned kotor_rules_cell32_007a20d0;            // .data 007A20D0 appearance.2da column index [0431]
extern unsigned kotor_rules_cell32_007a20cc;            // .data 007A20CC
extern unsigned kotor_rules_cell32_007a2108;            // .data 007A2108
extern unsigned kotor_rules_cell32_007a2104;            // .data 007A2104

// ---------------- imports: existing providers in the c16 candidate (exact names) ----------------
unsigned __fastcall kotor_fastcall_004d6230(void* message, void*, unsigned);              // MessageReadOverflow (-> 0917)
unsigned __fastcall kotor_fastcall_004d6240(void* message, void*, unsigned);              // MessageReadUnderflow (-> 0917)
unsigned char __fastcall kotor_fastcall_004d6720(void* message, void*, unsigned bits);   // ReadBYTE
unsigned short __fastcall kotor_fastcall_004d6750(void* message, void*, unsigned bits);  // ReadWORD
float __fastcall kotor_fastcall_004d67b0(void* message, void*, unsigned scale_bits, unsigned bits); // ReadFLOAT (ST0)
void* __fastcall kotor_fastcall_004d67e0(void* message, void*, void* result, unsigned bits); // ReadCExoString (by value)
void* __fastcall kotor_fastcall_004d6180(void* message, void*, void* result, unsigned size); // ReadCResRef (by value)
void __fastcall kotor_fastcall_004d6170(void* message, void*);                            // ClearReadMessage
unsigned __fastcall kotor_fastcall_00578540(void* message, void*);                        // ReadOBJECTIDServer
void* __fastcall ref_assign_copy_00406120(void* ref, void*, void* out, const void* other); // CResRef::operator= [0278]
void* __fastcall server_get_module_004ae6b0(void* server, void*);                         // CServerExoApp::GetModule
void* __fastcall recovered_object_array_004aed70(void* server, void*);                    // CServerExoApp::GetObjectArray
void* __fastcall module_get_area_004c30f0(void* module, void*);                           // CSWSModule::GetArea
unsigned __fastcall kotor_fastcall_004d8230(void* array, void*, unsigned id, void** out); // CGameObjectArray::GetGameObject
unsigned char __fastcall recovered_object_array_remove_004d81a0(void* array, void*, unsigned id, void* out); // ::Delete
void* __fastcall kotor_fastcall_00561080(void* player, void*);                            // CSWSPlayer::GetGameObject
void* __fastcall recovered_item_construct_005530a0(void* self, void*, unsigned id);       // CSWSItem::CSWSItem
U __fastcall recovered_item_template_005608b0(void* item, void*, void* resref);           // CSWSItem::LoadFromTemplate
void __fastcall recovered_object_position_004cd1a0(void* object, void*, const Vector* p, U a, U b, U c); // SetPosition
void __fastcall recovered_item_add_to_area_00555720(void* item, void*, void* area, float x, float y, float z, U flag);
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);        // CGameEffect::CGameEffect
void __fastcall recovered_effect_set_object_id_005036f0(void* effect, void*, U index, U id); // SetObjectID
void __fastcall kotor_fastcall_005036d0(void* effect, void*, U index, U value_bits);      // SetFloat
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b); // CSWSObject::ApplyEffect
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);                         // ~CExoString [0459]
void* __fastcall async_assign_string_005e5c50(void* self, void*, const void* other);      // CExoString::operator= [0199]
void __fastcall w2x_movement_rate_005a5680(void* stats, void*, U rate);                   // CSWSCreatureStats::SetMovementRate
I __fastcall kotor_m15b_c2da_get_float_entry_00413350(void* table, void*, int row, U column, float* out); // C2DA::GetFLOATEntry
void __fastcall kotor_fastcall_005cec90(void* pf, void*);                                 // ComputeStepTolerance

// ---------------- exports ----------------
void __fastcall kotor_fastcall_00503730(void* effect, void*, U index, const void* string);
void __fastcall kotor_fastcall_004ed6e0(void* creature, void*);
U __fastcall recovered_player_to_server_game_object_update_005228d0(void* message, void*, void* player, U minor);
}  // extern "C"

namespace {
constexpr U k_object_invalid = 0x7f000000u;
constexpr U k_one_0x3f800000 = 0x3f800000u;       // 1.0 ReadFLOAT scale immediate
constexpr U16 k_effect_type_00746694 = 6;         // .rdata 00746694 (word)
constexpr U16 k_effect_type_007467dc = 0x59;      // .rdata 007467DC (word)
constexpr U16 k_subtype_007473b8 = 0;             // .rdata 007473B8 (word)
constexpr U k_res_uti_7e9 = 0x7e9;
constexpr U k_item_size_29c = 0x29c;
constexpr U k_effect_size_8c = 0x8c;
constexpr U k_personal_space_default = 0x3f19999au; // 0.6f immediate (004ED6E9 / 004ED726)
constexpr U k_half_0x3f000000 = 0x3f000000u;      // 0.5f immediate
constexpr U k_one_float = 0x3f800000u;            // 1.0f immediate

template<class R, class... A> inline R vcall(void* object, U offset, A... args) {
    using F = R(__fastcall*)(void*, void*, A...);
    return (*reinterpret_cast<F*>(*static_cast<unsigned char**>(object) + offset))(object, nullptr, args...);
}
inline void* server_app() { return at<void*>(kotor_menu_window_cells::manager_007a39fc, 8); }
inline void* appearance_table() { return at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0xc); } // [[rules+B8]+C]
inline float read_float(void* m) { return kotor_fastcall_004d67b0(m, nullptr, k_one_0x3f800000, 0x20); }
inline bool read_failed(void* m) {
    return kotor_fastcall_004d6230(m, nullptr, 1) != 0 || kotor_fastcall_004d6240(m, nullptr, 1) != 0;
}

// Cases 'C' (00522ECA) and 'M' (0052303F): ID, three FLOATs, CExoString; build a CGameEffect of the given type
// on the player's game object. Identical shape apart from the type word and the string slot.
U apply_effect_case(void* m, void* player, U16 type) {
    const U id = kotor_fastcall_00578540(m, nullptr);                                // 00522ECA..CC / 0052303F..41
    const float x = read_float(m);                                                   // fstp [esp+1C]
    const float y = read_float(m);                                                   // fstp [esp+20]
    const float z = read_float(m);                                                   // fstp [esp+14]
    alignas(4) unsigned char text[8];                                                // CExoString [esp+28] / [esp+30]
    kotor_fastcall_004d67e0(m, nullptr, text, 0x20);                                 // 00522F09..12 / 0052307E..87
    if (read_failed(m)) {                                                            // 00522F17..3E / 0052308C..B4
        frame_LogStringDtor_005e5c20(text, nullptr);                                 // 00523026..35 / 00523197..A6
        return 0;
    }
    if (kotor_fastcall_00561080(player, nullptr) != nullptr)                         // 00522F44..54 (result unused)
        (void)kotor_fastcall_00561080(player, nullptr);                              // 00522F56..58 (result unused)
    void* memory = live_operator_new(k_effect_size_8c);                              // 00522F5D..67
    void* effect = memory != nullptr ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr; // 00522F78..87
    at<U16>(effect, 8) = type;                                                       // 00522F89 / 005230FF
    at<U16>(effect, 0xa) = static_cast<U16>((at<U16>(effect, 0xa) & 0xfff8u) | k_subtype_007473b8); // 00522F90..B4
    recovered_effect_set_object_id_005036f0(effect, nullptr, 0, id);                 // 00522F9A..B8
    kotor_fastcall_005036d0(effect, nullptr, 0, bits(x));                            // 00522FBD..C6
    kotor_fastcall_005036d0(effect, nullptr, 1, bits(y));                            // 00522FCB..D4
    kotor_fastcall_005036d0(effect, nullptr, 2, bits(z));                            // 00522FD9..E2
    kotor_fastcall_00503730(effect, nullptr, 0, text);                               // 00522FE7..F0
    recovered_effect_apply_004d14f0(kotor_fastcall_00561080(player, nullptr), nullptr, effect, 0, 0); // 00522FF5..3003
    frame_LogStringDtor_005e5c20(text, nullptr);                                     // 00523008..17
    return 1;
}

// Cases 'G' / 'H' / 'T': ID + BYTE stored into one CSWSCreature byte.
U creature_byte_case(void* m, void* array, U offset) {
    const U id = kotor_fastcall_00578540(m, nullptr);
    const unsigned char value = kotor_fastcall_004d6720(m, nullptr, 8);
    if (read_failed(m)) return 0;
    void* object = nullptr;
    (void)kotor_fastcall_004d8230(array, nullptr, id, &object);                       // status ignored; NULL on miss
    void* creature = vcall<void*>(object, 0x30);                                     // AsSWSCreature (object unchecked)
    if (creature == nullptr) return 0;
    at<unsigned char>(creature, offset) = value;
    return 1;
}
}  // namespace

extern "C" {
// ---------------------------------------------------------------------------------------------------------
// 00503730 CGameEffect::SetString(index, CExoString const& value): string slot index*8 + 48h. RET 8.
void __fastcall kotor_fastcall_00503730(void* effect, void*, U index, const void* string) {
    (void)async_assign_string_005e5c50(static_cast<unsigned char*>(effect) + index * 8 + 0x48, nullptr, string); // 00503730..3D
}

// 004ED6E0 CSWSCreature::UpdatePersonalSpace(): appearance.2da (row = appearance +A60) columns into the
// CPathfindInformation (+340) personal-space fields. RET 0. The float temp ([esp+4]) starts at 0.6 and keeps the
// last value a lookup wrote; the +8 fallback stores that temp (both branches identical in the original).
void __fastcall kotor_fastcall_004ed6e0(void* creature, void*) {
    float value = flt(k_personal_space_default);                                     // 004ED6E9
    I got = kotor_m15b_c2da_get_float_entry_00413350(appearance_table(), nullptr, at<U16>(creature, 0xa60),
                                                     kotor_rules_cell32_007a2118, &value); // 004ED6E1..0D
    void* pf = at<void*>(creature, 0x340);                                           // 004ED715
    if (got == 1) at<float>(pf, 4) = value;                                          // 004ED712..21
    else at<U>(pf, 4) = k_personal_space_default;                                    // 004ED726
    kotor_fastcall_005cec90(pf, nullptr);                                            // 004ED72D
    got = kotor_m15b_c2da_get_float_entry_00413350(appearance_table(), nullptr, at<U16>(creature, 0xa60),
                                                   kotor_rules_cell32_007a20d0, &value); // 004ED732..55
    if (got == 1) at<float>(at<void*>(creature, 0x340), 8) = value;                  // 004ED75A..69
    else at<float>(at<void*>(creature, 0x340), 8) = value;                           // 004ED76E..78 (same store)
    got = kotor_m15b_c2da_get_float_entry_00413350(appearance_table(), nullptr, at<U16>(creature, 0xa60),
                                                   kotor_rules_cell32_007a20cc, &value); // 004ED77B..9D
    pf = at<void*>(creature, 0x340);                                                 // 004ED7A5
    if (got == 1) at<float>(pf, 0x10) = value;                                       // 004ED7A2..B1
    else at<float>(pf, 0x10) = at<float>(pf, 8);                                     // 004ED7B6..B9 (fld/fstp dword)
    got = kotor_m15b_c2da_get_float_entry_00413350(appearance_table(), nullptr, at<U16>(creature, 0xa60),
                                                   kotor_rules_cell32_007a2108, &value); // 004ED7BC..DE
    if (got == 1) at<float>(at<void*>(creature, 0x340), 0x18) = value;               // 004ED7E3..F2
    else at<U>(at<void*>(creature, 0x340), 0x18) = k_half_0x3f000000;                // 004ED7F7..FD
    got = kotor_m15b_c2da_get_float_entry_00413350(appearance_table(), nullptr, at<U16>(creature, 0xa60),
                                                   kotor_rules_cell32_007a2104, &value); // 004ED804..25
    if (got == 1) at<float>(at<void*>(creature, 0x340), 0x14) = value;               // 004ED82A..39
    else at<U>(at<void*>(creature, 0x340), 0x14) = k_one_float;                      // 004ED83F..45
}

// ---------------------------------------------------------------------------------------------------------
// 005228D0 CSWSMessage::HandlePlayerToServerGameObjectUpdate(CSWSPlayer* player, BYTE minor). RET 8.
// Minor 1 only. Subtype BYTE 'A'..'T' through byte table 00523200 -> jump table 005231D4:
//   A item create, C effect 6, D object delete, G +A63, H +A68, L +A64..A67 by selector, M effect 59h,
//   P full appearance update, R appearance word + movement, T +A62; every other letter returns 0.
U __fastcall recovered_player_to_server_game_object_update_005228d0(void* message, void*, void* player, U minor) {
    void* const m = message;
    if (kotor_fastcall_004d6230(m, nullptr, 1) != 0) return 0;                       // 005228EC..FB
    if (static_cast<unsigned char>(minor) != 1) return 0;                            // 00522900..0B -> 005231B4
    alignas(4) unsigned char resref[16];                                             // CResRef [esp+44]
    std::memset(resref, 0, sizeof resref);                                           // 00522911..15 (CResRef ctor 00405ED0)
    void* const module = server_get_module_004ae6b0(server_app(), nullptr);          // 0052291A..22
    void* const array = recovered_object_array_004aed70(server_app(), nullptr);      // 00522927..32
    void* const net = vcall<void*>(server_app(), 0x10);                              // 00522937..44 GetNetLayer
    if (module == nullptr || array == nullptr || net == nullptr) {                   // 00522947..59
        kotor_fastcall_004d6170(m, nullptr);                                         // 005231AD..AF
        return 0;
    }
    const unsigned char subtype = kotor_fastcall_004d6720(m, nullptr, 8);            // 0052295F..63
    if (kotor_fastcall_004d6230(m, nullptr, 1) != 0) return 0;                       // 00522968..75
    const U index = static_cast<U>(static_cast<I>(static_cast<signed char>(subtype)) - 0x41); // 0052297B..7E (movsx)
    if (index > 0x13) return 0;                                                      // 00522981..84
    static const unsigned char k_case_00523200[20] = {0, 10, 1, 2, 10, 10, 3, 4, 10, 10, 10, 5, 6, 10, 10, 7, 10, 8, 10, 9};
    switch (k_case_00523200[index]) {                                                // 0052298A..91
    case 0: {                                                                        // 'A' 00522998: create item
        (void)kotor_fastcall_00578540(m, nullptr);                                   // 00522998..9A (id unused)
        const float x = read_float(m);                                               // 005229A8 fstp [esp+1C]
        const float y = read_float(m);                                               // 005229BA fstp [esp+20]
        const float z = read_float(m);                                               // 005229CC fstp [esp+14]
        const unsigned char stack = kotor_fastcall_004d6720(m, nullptr, 8);          // 005229D5..E2
        (void)kotor_fastcall_004d6720(m, nullptr, 8);                                // 005229E4 (discarded)
        (void)kotor_fastcall_004d6720(m, nullptr, 8);                                // 005229ED (discarded)
        alignas(4) unsigned char read_ref[16];                                       // [esp+88]
        alignas(4) unsigned char assign_ret[16];                                     // [esp+78]
        void* const got = kotor_fastcall_004d6180(m, nullptr, read_ref, 0x10);       // 005229F2..FE
        (void)ref_assign_copy_00406120(resref, nullptr, assign_ret, got);            // 00522A03..0D
        if (read_failed(m)) return 0;                                                // 00522A12..2E
        if (kotor_resource_demand::resource_manager_007a39e8->exists_00408bc0(
                reinterpret_cast<menu_resources::Ref*>(resref), k_res_uti_7e9, nullptr) == 0) // 00522A34..4C
            return 1;                                                                // 00522AF0
        void* memory = live_operator_new(k_item_size_29c);                           // 00522A52..5C
        void* item = memory != nullptr ? recovered_item_construct_005530a0(memory, nullptr, k_object_invalid) : nullptr; // 00522A63..82
        (void)recovered_item_template_005608b0(item, nullptr, resref);               // 00522A84..96
        if (stack != 0) at<unsigned char>(item, 0x264) = stack;                      // 00522A9B..9F
        Vector p; p.x = x; p.y = y; p.z = z;                                         // 00522AA5..C7 ([esp+38])
        recovered_object_position_004cd1a0(static_cast<unsigned char*>(item) + 0x10, nullptr, &p, 1, 1, 0); // 00522AB1..CB
        recovered_item_add_to_area_00555720(item, nullptr, module_get_area_004c30f0(module, nullptr), x, y, z, 1); // 00522AD0..EB
        return 1;                                                                    // 00522AF0
    }
    case 1:                                                                          // 'C' 00522ECA
        return apply_effect_case(m, player, k_effect_type_00746694);
    case 2: {                                                                        // 'D' 00522AFA: delete object
        const U id = kotor_fastcall_00578540(m, nullptr);                            // 00522AFA..05
        if (read_failed(m)) return 0;                                                // 00522B01..1F
        // [esp+10]: Delete writes it only on a hit; the original slot is otherwise uninitialized (NULL here).
        void* object = nullptr;
        (void)recovered_object_array_remove_004d81a0(array, nullptr, id, &object);   // 00522B25..2D (status ignored)
        if (object == nullptr) return 0;                                             // 00522B32..38
        void* const item = vcall<void*>(object, 0x38);                               // 00522B3E..40 AsSWSItem
        if (item != nullptr) (void)vcall<void*>(item, 0, 1u);                        // 00522B43..4D deleting dtor(1)
        return 1;                                                                    // 00522B4F
    }
    case 3:                                                                          // 'G' 00522C0B
        return creature_byte_case(m, array, 0xa63);
    case 4:                                                                          // 'H' 00522E66
        return creature_byte_case(m, array, 0xa68);
    case 5: {                                                                        // 'L' 00522B59
        const U id = kotor_fastcall_00578540(m, nullptr);                            // 00522B59..64
        const unsigned char selector = kotor_fastcall_004d6720(m, nullptr, 8);       // 00522B60..6F ([esp+1B])
        const unsigned char value = kotor_fastcall_004d6720(m, nullptr, 8);          // 00522B6B..7C
        if (read_failed(m)) return 0;                                                // 00522B78..96
        void* object = nullptr;
        (void)kotor_fastcall_004d8230(array, nullptr, id, &object);                   // 00522B9C..A4 (status ignored)
        void* const creature = vcall<void*>(object, 0x30);                           // 00522BA9..AF (object unchecked)
        if (creature == nullptr) return 0;                                           // 00522BB2..B4
        switch (selector) {                                                          // 00522BBA..C4 table 00523214
        case 0: at<unsigned char>(creature, 0xa64) = value; break;                   // 00522BCB
        case 1: at<unsigned char>(creature, 0xa65) = value; break;                   // 00522BDB
        case 6: at<unsigned char>(creature, 0xa66) = value; break;                   // 00522BEB
        case 7: at<unsigned char>(creature, 0xa67) = value; break;                   // 00522BFB
        default: break;                                                              // 2..5, >7 -> 00522C01
        }
        return 1;
    }
    case 6:                                                                          // 'M' 0052303F
        return apply_effect_case(m, player, k_effect_type_007467dc);
    case 7: {                                                                        // 'P' 00522D90: full update
        const U id = kotor_fastcall_00578540(m, nullptr);                            // 00522D90..9B
        if (kotor_fastcall_004d6230(m, nullptr, 1) != 0) return 0;                   // 00522D97..A4 (no underflow test)
        void* object = nullptr;
        (void)kotor_fastcall_004d8230(array, nullptr, id, &object);                   // 00522DAA..B2
        void* const creature = vcall<void*>(object, 0x30);                           // 00522DB7..BD (object unchecked)
        if (creature == nullptr) {                                                   // 00522DC0..C6
            kotor_fastcall_004d6170(m, nullptr);                                     // 005231AF
            return 0;
        }
        const U16 appearance = kotor_fastcall_004d6750(m, nullptr, 0x10);            // 00522DCC..D7
        const unsigned char b1 = kotor_fastcall_004d6720(m, nullptr, 8);             // 00522DD3..E3
        const unsigned char b2 = kotor_fastcall_004d6720(m, nullptr, 8);             // 00522DDF..EE [esp+67]
        const unsigned char b3 = kotor_fastcall_004d6720(m, nullptr, 8);             // 00522DEA..FB [esp+68]
        const unsigned char b4 = kotor_fastcall_004d6720(m, nullptr, 8);             // 00522DF7..08 [esp+69]
        if (read_failed(m)) return 0;                                                // 00522E04..24
        at<unsigned char>(creature, 0xa64) = b3;                                     // 00522E36
        at<U16>(creature, 0xa60) = appearance;                                       // 00522E3E
        at<unsigned char>(creature, 0xa62) = b1;                                     // 00522E45
        at<unsigned char>(creature, 0xa63) = b2;                                     // 00522E4B
        at<unsigned char>(creature, 0xa65) = b4;                                     // 00522E51
        kotor_fastcall_004ed6e0(creature, nullptr);                                  // 00522E57
        return 1;
    }
    case 8: {                                                                        // 'R' 00522C6F: appearance + rate
        const U id = kotor_fastcall_00578540(m, nullptr);                            // 00522C6F..7A
        const U16 appearance = kotor_fastcall_004d6750(m, nullptr, 0x10);            // 00522C76..85
        if (read_failed(m)) return 0;                                                // 00522C81..9F
        void* object = nullptr;
        (void)kotor_fastcall_004d8230(array, nullptr, id, &object);                   // 00522CA5..AD
        void* const creature = vcall<void*>(object, 0x30);                           // 00522CB2..B8 (object unchecked)
        if (creature == nullptr) return 0;                                           // 00522CBB..BF
        at<U16>(creature, 0xa60) = appearance;                                       // 00522CC5
        at<U16>(at<void*>(creature, 0xa74), 0x186) = appearance;                     // 00522CCC..D2
        w2x_movement_rate_005a5680(at<void*>(creature, 0xa74), nullptr, 7);          // 00522CD9..E1
        float value;                                                                 // [esp+14] (written on a hit)
        if (kotor_m15b_c2da_get_float_entry_00413350(appearance_table(), nullptr, appearance,
                                                     kotor_rules_cell32_007a2118, &value) != 0) { // 00522CE6..0C
            void* const pf = at<void*>(creature, 0x340);                             // 00522D0E
            at<float>(pf, 4) = value;                                                // 00522D14..18
            kotor_fastcall_005cec90(pf, nullptr);                                    // 00522D1B..1D
        }
        return 1;                                                                    // 00522D22
    }
    case 9:                                                                          // 'T' 00522D2C
        return creature_byte_case(m, array, 0xa62);
    default:                                                                         // 10 -> 005231B4
        return 0;
    }
}
}  // extern "C"
