// combat-attack lane, part B "round": CSWSCombatRound / CSWSCombatAttackData / CSWSCombatSubAttack members on the
// AIActionPhysicalAttacks 005BBBF0 cascade (StartCombatRound 004D5F70, EndCombatRound 004D4620 and their callees).
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are original instructions.
// Conventions (W5 t1..t3): thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision ->
// double intermediates, float exactly where the original stores a dword; ST0 returns are double;
// live_operator_new / live_free only. Every callee binds to an existing selected provider of the c07b candidate map,
// to a sibling claude_combat_attack*.cpp export, or is reconstructed below. No traps are introduced by this file.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"

using namespace w5;

namespace kotor_reconstruction {
struct StartupString;
void __cdecl StartupFormat005e5680(StartupString*, const char*, ...);              // CExoString::Format [0125]
}
namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* s, void*); } // CExoString() [0603]
namespace frame_control { extern unsigned message_manager_007a3a00; }               // .data 007A3A00 g_pVirtualMachine [0456]

extern "C" {
// ---------------- imports: existing providers (c07b candidate map; object in brackets) ----------------
extern U recovered_effect_rank_scratch_008327e0;                                     // .bss 008327E0 fistp scratch [0886]
void* __fastcall recovered_equipment_item_005a4c20(void* inventory, void*, U slot);  // CSWInventory::GetItemInSlot [1025]
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, int id); // CSWBaseItemArray::GetBaseItem [0820]
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);          // CSWItem::GetBaseItem [0820]
std::uint8_t __fastcall texture_controller_return_true_005a4cf0(void* self, void*); // return_true (CSWSCreatureStats) [0317]
U __fastcall w5_combat_attack_clear_004d3010(void* data, void*);                     // CSWSCombatAttackData::ClearAttackData [W5 t3]
unsigned short __fastcall w1x_creature_max_fp_004fd490(void* creature, void*);       // CSWSCreature::GetMaxForcePoints
void* __fastcall kotor_fastcall_00635460(void* party, void*);                       // CSWParty::GetPlayerCharacter
unsigned __fastcall kotor_fastcall_004aea30(void* server, void*, unsigned id);      // CServerExoApp::ClientToServerObjectId
unsigned __fastcall kotor_fastcall_005edc10(void* client, void*);                   // CClientExoApp::GetPausedByCombat
void* __fastcall kotor_fastcall_005ed700(void* client, void*);                      // CClientExoApp::GetClientOptions
void __fastcall kotor_fastcall_005edee0(void* client, void*, unsigned a, unsigned b); // CClientExoApp::SetAutoPaused
void __fastcall kotor_fastcall_005edd70(void* client, void*, unsigned target, unsigned flag); // CClientExoApp::SetLastTarget
void __fastcall frame_R4_005d0fc0(void* vm, void*, unsigned* script, unsigned id, unsigned flag); // CVirtualMachine::RunScript [0457]
void* __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* pos);      // CExoLinkedListInternal::GetAtPos [0168]
void __fastcall w5_creature_set_combat_mode_0050ee80(void* creature, void*, U mode, U check); // SetCombatMode [W5 t3]
void __fastcall w5_combat_clear_all_special_attacks_004d4f60(void* round, void*);    // ClearAllSpecialAttacks [W5 t3]
void __fastcall kotor_fastcall_004d38b0(void* round, void*, U target, U a2, U a3, U a4, U a5); // AddAttackAction
unsigned __fastcall recovered_object_get_reputation_0057cb80(void* self, void*, unsigned target, unsigned a2); // GetReputation
std::uint32_t __fastcall kotor_fastcall_004f2de0(void* self, void*, float radius, U a2, U a3); // GetNearestEnemy [nearest_enemy]
void __fastcall frame_LogStringDtor_005e5c20(void* s, void*);                        // CExoString::~CExoString
void* __fastcall async_assign_string_005e5c50(void* dst, void*, void* src);          // CExoString::operator= [0199]
void __fastcall kotor_fastcall_00405080(void* list, void*, int size);               // CExoArrayList<CExoString>::Allocate [0603]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value); // CSWCCMessageData::SetInteger [1017]
void __fastcall kotor_m15e_slot090_void0_004015a0(void* self, void*);                // void_return (~CSWSCombatSubAttack) [0832]
void __stdcall crt_eh_vector_constructor_006fb55f(void* first, U size, I count, void* construct, void* destroy); // [d1]
void __stdcall c2da_destroy_array_006fa852(void* first, U size, I count, void* destroy); // eh_vector_destructor_iterator [0186]

// ---------------- sibling-part exports (claude_combat_attack.cpp) ----------------
double __fastcall kotor_fastcall_004ffad0(void* creature, void*, U a1, U a2, U a3);  // MaxCleaveRange, RET 0Ch, ST0
U __fastcall kotor_fastcall_005b48b0(void* creature, void*, U a1);                   // CancelAllCombatModes, RET 4
U __fastcall kotor_fastcall_004d3e50(void* round, void*, U target);                  // UpdateAttackTargetForAllAction, RET 4

// ---------------- own exports used before their definition ----------------
void __fastcall kotor_fastcall_005b8010(void* creature, void*);
void __fastcall kotor_fastcall_005b6980(void* self, void*, U died, U cleave, U next);
}

namespace {
// CSWSCombatSubAttack (0xD0 bytes), passed by value to CExoArrayList<CSWSCombatSubAttack>::Add (RET 0D0h)
struct SubAttack { unsigned char b[0xd0]; };
static_assert(sizeof(SubAttack) == 0xd0, "CSWSCombatSubAttack is 0xD0 bytes");

const float k_log_scale_00745ff8 = 3.321928024291992f;   // .rdata 00745FF8 (0x40549A78)

inline void* owner_of(void* round) { return at<void*>(round, 0x9b4); }            // CSWSCombatRound +9B4 creature
inline void* inventory_of(void* creature) { return at<void*>(creature, 0xa2c); }  // CSWSCreature +A2C CSWInventory
inline void* slot_item(void* round, U slot) { return recovered_equipment_item_005a4c20(inventory_of(owner_of(round)), nullptr, slot); }
inline void* base_item(U id) {
    return kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(kotor_cell_rules_007a3a28, 0x34), nullptr, static_cast<int>(id));
}

// 004D2084..20BD / 004D20D3..2105: damage-type index = frndint((float)(log10(type) * 3.3219...)) via the
// global fistp scratch 008327E0 (the original stores there and reads it back). Exact x87 sequence:
// fldlg2; fild; fyl2x; fmul dword; fstp dword (float rounding); fld; frndint; fistp dword [008327E0].
// Type 0 gives -inf -> integer indefinite 80000000h as in the original.
U damage_index(U type_word) {
    U value = type_word;
    float scaled;
    const float scale = k_log_scale_00745ff8;
    __asm {
        fldlg2
        fild dword ptr [value]
        fyl2x
        fmul dword ptr [scale]
        fstp dword ptr [scaled]
        fld dword ptr [scaled]
        frndint
        fistp dword ptr [recovered_effect_rank_scratch_008327e0]
    }
    return recovered_effect_rank_scratch_008327e0;
}
inline U16& damage_slot(void* data, U index) {                                     // word [data + index*2 + 1C]
    return *reinterpret_cast<U16*>(static_cast<unsigned char*>(data) + index * 2 + 0x1c);
}
// fld dword + fstp dword: an x87 round trip copies every float exactly except that a signaling NaN is quieted
inline U x87_float_copy(U f) {
    if ((f & 0x7f800000u) == 0x7f800000u && (f & 0x007fffffu) != 0) f |= 0x00400000u;
    return f;
}
}  // namespace

extern "C" {
// =========================================================================================================
// CSWSCombatAttackData (+1C..+37 fifteen damage words indexed by log2(type), +38 base damage word)
// ---------------------------------------------------------------------------------------------------------
// 004D1F80 CSWSCombatAttackData::GetDamage(flags): RET 4, EAX = movsx word (first matching bit in the original order)
U __fastcall kotor_fastcall_004d1f80(void* data, void*, U flags) {
    if (flags & 0x10) return static_cast<U>(static_cast<I>(at<std::int16_t>(data, 0x24)));     // 004D1F84
    if (flags & 0x20) return static_cast<U>(static_cast<I>(at<std::int16_t>(data, 0x26)));     // 004D1F8F
    if (flags & 0x40) return static_cast<U>(static_cast<I>(at<std::int16_t>(data, 0x28)));     // 004D1F9A
    if (flags & 0x80) return static_cast<U>(static_cast<I>(at<std::int16_t>(data, 0x2a)));     // 004D1FA5 (test al,al; jns)
    if (flags & 0x100) return static_cast<U>(static_cast<I>(at<std::int16_t>(data, 0x2c)));    // 004D1FB0
    if (flags & 0x8) return static_cast<U>(static_cast<I>(at<std::int16_t>(data, 0x22)));      // 004D1FBC
    if (flags & 0x200) return static_cast<U>(static_cast<I>(at<std::int16_t>(data, 0x2e)));    // 004D1FC7
    if (flags & 0x400) return static_cast<U>(static_cast<I>(at<std::int16_t>(data, 0x30)));    // 004D1FD3
    if (flags & 0x800) return static_cast<U>(static_cast<I>(at<std::int16_t>(data, 0x32)));    // 004D1FDF
    if (flags & 0x1000) return static_cast<U>(static_cast<I>(at<std::int16_t>(data, 0x34)));   // 004D1FEB
    if (flags & 0x1) return static_cast<U>(static_cast<I>(at<std::int16_t>(data, 0x1c)));      // 004D1FF7
    if (flags & 0x2) return static_cast<U>(static_cast<I>(at<std::int16_t>(data, 0x1e)));      // 004D2002
    if (flags & 0x4) return static_cast<U>(static_cast<I>(at<std::int16_t>(data, 0x20)));      // 004D200D
    if (flags & 0x2000) return static_cast<U>(static_cast<I>(at<std::int16_t>(data, 0x36)));   // 004D2018
    return 0;                                                                                   // 004D2024
}

// 004D2030 CSWSCombatAttackData::SetBaseDamage(value): RET 4. Positive base word accumulates, otherwise replaced.
void __fastcall kotor_fastcall_004d2030(void* data, void*, U value) {
    const U base = at<U16>(data, 0x38);                                             // 004D2030..32 (zero-extended)
    if (static_cast<std::int16_t>(base) > 0) {                                      // 004D2036..39 (jle: signed word)
        at<U16>(data, 0x38) = static_cast<U16>(base + value);                       // 004D203B..41
        return;
    }
    at<U16>(data, 0x38) = static_cast<U16>(value);                                  // 004D2048..4D
}

// 004D2070 CSWSCombatAttackData::SetDamage(type word, value): RET 8. Negative values are clamped to 0.
void __fastcall kotor_fastcall_004d2070(void* data, void*, U type, U value) {
    if (static_cast<I>(value) < 0) value = 0;                                       // 004D2073..7F (jge: signed)
    const U index = damage_index(type & 0xffff);                                    // 004D2087..B8
    damage_slot(data, index) = static_cast<U16>(value);                             // 004D20B3..BD
}

// 004D20D0 CSWSCombatAttackData::AddDamage(type word, value, add to base): RET 0Ch
void __fastcall kotor_fastcall_004d20d0(void* data, void*, U type, U value, U base) {
    const U index = damage_index(type & 0xffff);                                    // 004D20D3..FF
    const std::int16_t current = static_cast<std::int16_t>(damage_slot(data, index)); // 004D2105..09
    if (current > 0) {                                                              // 004D210E..11 (jle: signed word)
        I sum = static_cast<I>(current) + static_cast<I>(value);                    // 004D2114..1B
        if (sum < 1) sum = 1;                                                       // 004D211D..23 (jge: signed)
        damage_slot(data, index) = static_cast<U16>(sum);                           // 004D2128
    } else {
        kotor_fastcall_004d2070(data, nullptr, type, value);                        // 004D212F..39
    }
    if (base == 0) return;                                                          // 004D213E..44
    const U old = at<U16>(data, 0x38);                                              // 004D214A..4C (inline SetBaseDamage)
    if (static_cast<std::int16_t>(old) > 0) {                                       // 004D2150..53
        at<U16>(data, 0x38) = static_cast<U16>(old + value);                        // 004D2155..5B
        return;
    }
    at<U16>(data, 0x38) = static_cast<U16>(value);                                  // 004D2165..6A
}

// 004D2180 CSWSCombatAttackData::GetTotalDamage(unused): RET 4. Sum of the fourteen words +1C..+36 above -1.
U __fastcall kotor_fastcall_004d2180(void* data, void*, U) {
    I total = 0;                                                                    // 004D2181
    for (U offset = 0x1c; offset <= 0x36; offset += 2) {                            // 004D2183..FC (2 x 7 words)
        const std::int16_t v = at<std::int16_t>(data, offset);
        if (v > -1) total += v;                                                     // cmp dx,-1; jle (signed)
    }
    return static_cast<U>(total);
}

// =========================================================================================================
// CSWSCombatSubAttack / CSWSCombatSubAttackThreat
// ---------------------------------------------------------------------------------------------------------
// 004D2EC0 CSWSCombatSubAttackThreat::Clear: RET 0
void __fastcall kotor_fastcall_004d2ec0(void* threat, void*) {
    at<U8>(threat, 0) = 0; at<U>(threat, 4) = 0; at<U8>(threat, 8) = 0;           // 004D2EC0..C7
    at<U>(threat, 0xc) = 0; at<U8>(threat, 0x10) = 0; at<U8>(threat, 0x11) = 0;    // 004D2ECA..D0
}

// 004D32F0 CSWSCombatSubAttack::CSWSCombatSubAttack: RET 0, EAX = this. Fields not listed stay unwritten.
void* __fastcall kotor_fastcall_004d32f0(void* s, void*) {
    at<U>(s, 0x20) = 0; at<U>(s, 0x24) = 0; at<U>(s, 0x28) = 0;                    // 004D32F5..FB
    for (U o = 0x50; o <= 0x57; ++o) at<U8>(s, o) = 0;                              // 004D32FE..13
    at<U>(s, 0x58) = 0;                                                             // 004D3316
    for (U o = 0x5c; o <= 0x65; ++o) at<U8>(s, o) = 0;                              // 004D3319..34
    at<U>(s, 0x68) = 0; at<U>(s, 0x6c) = 0; at<U8>(s, 0x70) = 0;                    // 004D3337..3D
    at<U>(s, 0x74) = 0; at<U8>(s, 0x78) = 0; at<U>(s, 0x7c) = 0;                    // 004D3340..46
    at<U8>(s, 0x80) = 0; at<U8>(s, 0x81) = 0;                                       // 004D3349..4F
    for (U o = 0x84; o <= 0x8c; ++o) at<U8>(s, o) = 0;                              // 004D3355..85
    for (U o = 0x8e; o <= 0x99; ++o) at<U8>(s, o) = 0;                              // 004D338B..CD
    std::memset(static_cast<unsigned char*>(s) + 0x9a, 0xff, 7 * 4 + 2);            // 004D33D4..E4 (rep stosd x7 + stosw)
    for (U o = 0xbc; o <= 0xc3; ++o) at<U8>(s, o) = 0;                              // 004D33E7..11
    at<U>(s, 0xc8) = 0; at<U>(s, 0xcc) = 0;                                         // 004D3417..1D
    at<U>(s, 0xb8) = k_object_invalid;                                              // 004D3423
    at<U8>(s, 0xc4) = 0xff;                                                         // 004D342D
    return s;
}

// 004D43B0 CExoArrayList<CSWSCombatSubAttack>::Allocate(size): RET 4. Array cookie (count) precedes the data.
void __fastcall kotor_fastcall_004d43b0(void* list, void*, I size) {
    unsigned char* old = at<unsigned char*>(list, 0);                               // 004D43D6 (EBP)
    at<I>(list, 8) = size;                                                          // 004D43DD
    void* memory = live_operator_new(static_cast<U>(size) * 0xd0 + 4);              // 004D43C8..E0
    unsigned char* data = nullptr;                                                  // EDI = 0 (004D43EC)
    if (memory != nullptr) {                                                        // 004D43EE..F4
        at<I>(memory, 0) = size;                                                    // 004D440A
        data = static_cast<unsigned char*>(memory) + 4;                             // 004D4401
        crt_eh_vector_constructor_006fb55f(data, 0xd0, size, reinterpret_cast<void*>(&kotor_fastcall_004d32f0),
                                           reinterpret_cast<void*>(&kotor_m15e_slot090_void0_004015a0)); // 004D43F6..0C
    }
    at<unsigned char*>(list, 0) = data;                                             // 004D4420
    for (I i = 0; i < at<I>(list, 4); ++i)                                          // 004D4411..3F (count re-read)
        std::memcpy(at<unsigned char*>(list, 0) + i * 0xd0, old + i * 0xd0, 0xd0);  // rep movsd x34h
    if (old != nullptr) {                                                           // 004D4441..43
        c2da_destroy_array_006fa852(old, 0xd0, at<I>(old - 4, 0), reinterpret_cast<void*>(&kotor_m15e_slot090_void0_004015a0)); // 004D4445..57
        live_free(old - 4);                                                         // 004D445C..62
    }
}

// 004D57A0 CExoArrayList<CSWSCombatSubAttack>::Add(CSWSCombatSubAttack by value): RET 0D0h
void __fastcall kotor_fastcall_004d57a0(void* list, void*, SubAttack value) {
    const I alloc = at<I>(list, 8);                                                 // 004D57A3
    if (at<I>(list, 4) == alloc)                                                    // 004D57A6..AB
        kotor_fastcall_004d43b0(list, nullptr, alloc != 0 ? alloc * 2 : 0x10);      // 004D57AD..BD
    std::memcpy(at<unsigned char*>(list, 0) + at<I>(list, 4) * 0xd0, &value, 0xd0); // 004D57C2..D8
    at<I>(list, 4) = at<I>(list, 4) + 1;                                            // 004D57DA..E0
}
}  // extern "C"

namespace {
// shared tail of 004D5A50 / 004D5B60 (004D5A95..5B4D / 004D5BE5..5C9D): move the fifteen damage words into the
// sub-attack (+30..+4C) and reset them to FFFF, copy the attack data blocks, and append to +AC.
void finish_sub_attack(void* data, SubAttack& s) {
    unsigned char* d = static_cast<unsigned char*>(data);
    for (U i = 0; i < 0xf; ++i) {                                                   // 004D5AA2..C0 / 004D5BF2..10
        std::memcpy(s.b + 0x30 + i * 2, d + 0x1c + i * 2, 2);
        *reinterpret_cast<U16*>(d + 0x1c + i * 2) = 0xffff;
    }
    std::memcpy(s.b + 0x50, d + 0xc0, 0x20);                                        // 8 dwords +C0 -> +50
    std::memcpy(s.b + 0x70, d + 0xe0, 0x14);                                        // 5 dwords +E0 -> +70
    std::memcpy(s.b + 0x84, d + 0xf4, 8); s.b[0x8c] = d[0xfc];                      // +F4 dword, +F8 dword, +FC byte
    std::memcpy(s.b + 0x8e, d + 0xfe, 0x2a);                                        // 10 dwords + word +FE -> +8E
    std::memcpy(s.b + 0xb8, d + 0x128, 0x18);                                       // 6 dwords +128 -> +B8
    kotor_fastcall_004d57a0(d + 0xac, nullptr, s);                                  // 004D5B31..4D (by-value copy)
}
}  // namespace

extern "C" {
// 004D5A50 CSWSCombatAttackData::ResolveAttackToMeleeSubAttack(a1, a2): RET 8
void __fastcall kotor_fastcall_004d5a50(void* data, void*, U a1, U a2) {
    SubAttack s;                                                                    // [esp+0C]
    kotor_fastcall_004d32f0(&s, nullptr);                                           // 004D5A5B..5F
    std::memcpy(s.b + 0xc, &a2, 4);                                                 // 004D5A75
    const U16 w64 = at<U16>(data, 0x64); std::memcpy(s.b + 6, &w64, 2);             // 004D5A79..84
    std::memcpy(s.b + 0, &a1, 4);                                                   // 004D5A7D
    s.b[4] = at<U8>(data, 0x5c);                                                    // 004D5A72 / 8D
    s.b[8] = at<U8>(data, 0x3a);                                                    // 004D5A81 / 91
    const U zero = 0; std::memcpy(s.b + 0x18, &zero, 4);                            // 004D5A95
    finish_sub_attack(data, s);
}

// 004D5B60 CSWSCombatAttackData::ResolveAttackToRangedSubAttack(a1..a5): RET 14h
void __fastcall kotor_fastcall_004d5b60(void* data, void*, U a1, U a2, U a3, U a4, U a5) {
    SubAttack s;                                                                    // [esp+0C]
    kotor_fastcall_004d32f0(&s, nullptr);                                           // 004D5B6B..6F
    std::memcpy(s.b + 0, &a1, 4);                                                   // 004D5B89
    std::memcpy(s.b + 0x10, &a3, 4);                                                // 004D5B94
    std::memcpy(s.b + 0xc, &a2, 4);                                                 // 004D5B9B
    std::memcpy(s.b + 0x14, &a4, 4);                                                // 004D5BA6
    s.b[4] = at<U8>(data, 0x5c);                                                    // 004D5BAD
    std::memcpy(s.b + 0x2c, &a5, 4);                                                // 004D5BB1
    s.b[8] = at<U8>(data, 0x3a);                                                    // 004D5BB9
    std::memcpy(s.b + 0x20, static_cast<unsigned char*>(data) + 0x68, 0xc);         // 004D5BBD..DD (+68 vector)
    const U16 w64 = at<U16>(data, 0x64); std::memcpy(s.b + 6, &w64, 2);             // 004D5BC2
    const U w10 = at<U16>(data, 0x10); std::memcpy(s.b + 0x1c, &w10, 4);            // 004D5BD1..E1 (movzx)
    const U one = 1; std::memcpy(s.b + 0x18, &one, 4);                              // 004D5BE5
    finish_sub_attack(data, s);
}

// =========================================================================================================
// CSWSCombatRound (+934 round started, +938 cleave, +93C, +940, +944 round time, +94C round length, +950,
// +958 paused, +95C pausing id, +960 pause timer, +96C current attack, +970 off-hand flag, +990 on-hand attacks,
// +994 off-hand attacks, +9A0 / +9A4 extra attacks, +9B0 action list, +9B4 owner creature, +9B8 master,
// +9BC slave flag, +9C0 / +9C4 master/slave ids, +9CC round target)
// ---------------------------------------------------------------------------------------------------------
// 004D29F0 CSWSCombatRound::SetOffHandAttack(value): RET 4
void __fastcall kotor_fastcall_004d29f0(void* round, void*, U value) {
    at<U>(round, 0x970) = value;                                                    // 004D29F0..F4
}

// 004D2A00 CSWSCombatRound::GetOffHandAttack: RET 0
U __fastcall kotor_fastcall_004d2a00(void* round, void*) {
    return at<U>(round, 0x970);                                                     // 004D2A00
}

// 004D2EB0 CSWSCombatRound::GetTotalAttacks: RET 0
U __fastcall kotor_fastcall_004d2eb0(void* round, void*) {
    return at<U>(round, 0x994) + at<U>(round, 0x990);                               // 004D2EB0..B6
}

// 004D2970 CSWSCombatRound::CheckActionLengthAtTime(id, length, time): RET 0Ch, EAX 0/1
U __fastcall kotor_fastcall_004d2970(void* round, void*, U id, U length, U time) {
    if (at<U>(round, 0x958) == 1 && at<U>(round, 0x95c) != id) {                    // 004D2970..8F
        const I now = at<I>(round, 0x944);                                          // 004D2991
        const I end = at<I>(round, 0x960) + now;                                    // 004D2997..9D
        if (static_cast<I>(now + static_cast<I>(time)) < end) return 0;             // 004D299F..A9 (jge: signed)
    }
    if (at<U>(round, 0x934) != 1) return 1;                                         // 004D29AC..B2 (EAX still 1)
    const I left = at<I>(round, 0x94c) - at<I>(round, 0x950) - at<I>(round, 0x944) - static_cast<I>(time) + 0x3e8; // 004D29B4..D0
    return left >= static_cast<I>(length) ? 1u : 0u;                                // 004D29D5..DC (setge)
}

// 004D2A10 CSWSCombatRound::CalculateOnHandAttacks(weapon word): RET 4, EAX = +990
U __fastcall kotor_fastcall_004d2a10(void* round, void*, U weapon) {
    const U base = texture_controller_return_true_005a4cf0(at<void*>(owner_of(round), 0xa74), nullptr); // 004D2A13..1F (AL)
    U attacks = base + at<U>(round, 0x9a4);                                         // 004D2A24..2D
    at<U>(round, 0x990) = attacks;                                                  // 004D2A39
    const U16 w = static_cast<U16>(weapon);                                         // 004D2A30
    if (w == 0xb || w == 0x5b || w == 0x35 || w == 0x1e || w == 0x5c || w == 0x1a) { // 004D2A35..5D
        ++attacks;
        at<U>(round, 0x990) = attacks;                                              // 004D2A5F..60
    }
    return at<U>(round, 0x990);                                                     // 004D2A66
}

// 004D2A70 CSWSCombatRound::CalculateOffHandAttacks: RET 0, EAX = +994 (0/1)
U __fastcall kotor_fastcall_004d2a70(void* round, void*) {
    U attacks = 0;                                                                  // EBX
    void* item = slot_item(round, 0x10);                                            // 004D2A75..85
    if (item != nullptr) {                                                          // 004D2A8C..8E
        const I size = static_cast<I>(at<U8>(base_item(at<U>(item, 0xc)), 0x1b)) - at<I>(owner_of(round), 0x4f8); // 004D2A94..B0
        if (size > 0) {                                                             // 004D2AB6..B8 (jg: signed)
            if (at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 8) == 3) attacks = 1; // 004D2B0F..1C
        } else {
            void* left = recovered_equipment_item_005a4c20(inventory_of(owner_of(round)), nullptr, 0x20); // 004D2ABA..C2
            if (left != nullptr && at<U8>(base_item(at<U>(left, 0xc)), 9) != 0 &&   // 004D2AC9..E3
                at<U8>(base_item(at<U>(left, 0xc)), 8) != 0) {                      // 004D2AE5..FC
                attacks = 1;                                                        // 004D2AFE
            }
        }
    }
    at<U>(round, 0x994) = attacks;                                                  // 004D2B03 / 004D2B21
    return attacks;
}

// 004D2E20 CSWSCombatRound::HasCreatureWeapons: RET 0. 0 when a hand slot (10h / 20h) holds an item; otherwise 1
// when a creature weapon slot (4000h / 8000h / 10000h) holds one.
U __fastcall kotor_fastcall_004d2e20(void* round, void*) {
    if (slot_item(round, 0x10) != nullptr) return 0;                                // 004D2E23..38
    if (slot_item(round, 0x20) != nullptr) return 0;                                // 004D2E3A..4F
    if (slot_item(round, 0x4000) != nullptr) return 1;                              // 004D2E51..69
    if (slot_item(round, 0x8000) != nullptr) return 1;                              // 004D2E6B..83
    return slot_item(round, 0x10000) != nullptr ? 1u : 0u;                          // 004D2E85..A8
}

// 004D3DA0 CSWSCombatRound::GetWeaponAttackType: RET 0. 1 main hand, 2 off-hand, 3 creature, 6 two-weapon left,
// 7 unarmed, 8 extra (unarmed) attack.
U __fastcall kotor_fastcall_004d3da0(void* round, void*) {
    const U creature_weapons = kotor_fastcall_004d2e20(round, nullptr);             // 004D3DA3
    const U off_hand = at<U>(round, 0x970);                                         // 004D3DB0
    U extra;                                                                        // ECX
    if (at<U>(round, 0x9a0) == 0 && at<U>(round, 0x9a4) == 0) extra = 0;            // 004D3DA8..C4
    else extra = static_cast<I>(at<U8>(round, 0x96c) + 1) > at<I>(round, 0x990) ? 1u : 0u; // 004D3DC6..DE (setg)
    if (creature_weapons != 0) return slot_item(round, 0x4000) != nullptr ? 3u : 0u; // 004D3E30..4E
    if (off_hand != 0) return 2;                                                    // 004D3DE5..0C
    if (extra != 0) return slot_item(round, 0x20) != nullptr ? 6u : 8u;             // 004D3E13..2F
    return slot_item(round, 0x10) != nullptr ? 1u : 7u;                             // 004D3DEF..0B
}

// 004D4FF0 CSWSCombatRound::GetCurrentAttackWeapon(type, 0 = current): RET 4, EAX = CSWSItem* or 0
void* __fastcall kotor_fastcall_004d4ff0(void* round, void*, U type) {
    if (type == 0) type = kotor_fastcall_004d3da0(round, nullptr);                  // 004D4FF0..FC
    if (type == 1 || type == 6) return slot_item(round, 0x10);                      // 004D5001..0D -> 004D50F7
    if (type == 2) {                                                                // 004D5013
        void* right = slot_item(round, 0x10);                                       // 004D5018..26
        if (right != nullptr && at<U8>(base_item(at<U>(right, 0xc)), 8) == 3) return right; // 004D502D..4D
        return slot_item(round, 0x20);                                              // 004D5050..66 (tail jump)
    }
    if (type == 3) return slot_item(round, 0x4000);                                 // 004D506B..86
    if (type == 4) return slot_item(round, 0x8000);                                 // 004D508B..A6
    if (type == 5) return slot_item(round, 0x10000);                                // 004D50AB..C6
    if (type == 7 || type == 8) return slot_item(round, 8);                         // 004D50CB..F2
    return nullptr;                                                                 // 004D50D5..D9
}

// =========================================================================================================
// 004D4620 CSWSCombatRound::EndCombatRound(flag): RET 4, EAX 1
U __fastcall kotor_fastcall_004d4620(void* round, void*, U flag) {
    void* owner = owner_of(round);                                                  // 004D463E
    at<U>(round, 0x934) = 0; at<U>(round, 0x944) = 0; at<U>(round, 0x94c) = 0;     // 004D464C..58
    at<U>(round, 0x958) = 0; at<U>(round, 0x95c) = k_object_invalid;               // 004D465E..64
    at<U>(round, 0x960) = 0; at<U>(round, 0x9ac) = k_object_invalid;               // 004D466A..70
    at<U>(round, 0x9a0) = 0; at<U>(round, 0x968) = 0;                              // 004D4676..7C
    if (owner != nullptr && at<U>(owner, 0xa9c) != 0) {                             // 004D4644..8A
        void* stats = at<void*>(owner, 0xa74);                                      // 004D468C
        at<U16>(stats, 0x124) = w1x_creature_max_fp_004fd490(owner, nullptr);       // 004D4692..9A
    }
    U can_run = 1;                                                                  // [esp+18] (004D46AD..B2)
    if (at<U>(round, 0x9bc) != 0) {                                                 // 004D46A6..B6 (slave round)
        void* master = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(round, 0x9c4)); // 004D46BC..CB
        if (master != nullptr) {                                                    // 004D46D0..D2
            void* r = at<void*>(master, 0x9c8);                                     // 004D46D4
            const U mode = at<U>(r, 0x964);
            at<U>(r, 0x958) = 0; at<U>(r, 0x95c) = k_object_invalid;                // 004D46E0..E6 (inline SetRoundPaused(0))
            if (mode != 0) at<U>(r, 0x964) = 0;                                     // 004D46DA / EC..EE
            r = at<void*>(master, 0x9c8);
            at<U>(r, 0x964) = 0; at<U>(r, 0x960) = 0;                               // 004D46F4..00
            at<U>(at<void*>(master, 0x9c8), 0x9b8) = 0;                             // 004D4706..0C
            at<U>(at<void*>(master, 0x9c8), 0x9c0) = k_object_invalid;              // 004D4712..18
        } else {
            alignas(4) unsigned char text[8];                                       // CExoString [esp+20]
            kotor_movie::string_element_construct_005b3190(text, nullptr);          // 004D4720..24
            kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(text),
                "CSWSCombatRound::EndCombatRound - %x Combat Slave (%x) not found!", // .rdata 007463D0
                at<U>(owner, 4), at<U>(round, 0x9c4));                              // 004D4729..48
            frame_LogStringDtor_005e5c20(text, nullptr);                            // 004D4750..5C
        }
    }
    U target_died = 0;                                                              // [esp+1C]
    U next_target = k_object_invalid;                                               // [esp+14]
    if (at<U>(round, 0x9cc) != k_object_invalid) {                                  // 004D4761..71
        void* obj = kotor_fastcall_004ae750(server_app(), nullptr, at<U>(round, 0x9cc)); // 004D4777..81
        if (obj == nullptr) {                                                       // 004D4788..8A
            at<U>(round, 0x9cc) = k_object_invalid;                                 // 004D482B
        } else if (vcall<void*>(obj, 0x30) != nullptr) {                            // 004D4790..9B
            void* creature = vcall<void*>(obj, 0x30);                               // 004D479D..A2
            if (vcall<U>(creature, 0x94) != 0 || recovered_creature_incapacitated_004ef890(creature, nullptr) != 0) { // 004D47A4..BB
                at<U>(round, 0x9cc) = k_object_invalid;                             // 004D47C3
                target_died = 1;                                                    // 004D47CD
                next_target = at<U>(round, 0x938) != 0 ? at<U>(owner, 0x524) : at<U>(owner, 0x50c); // 004D47BD..F5
            }
        } else {
            bool clear = false;
            if (vcall<void*>(obj, 0x18) != nullptr && at<U8>(vcall<void*>(obj, 0x18), 0x2cc) != 0) clear = true; // 004D47F7..0F (door)
            else if (vcall<void*>(obj, 0x48) != nullptr && at<U>(vcall<void*>(obj, 0x48), 0x338) != 0) clear = true; // 004D4811..29 (placeable)
            if (clear) at<U>(round, 0x9cc) = k_object_invalid;                      // 004D482B
        }
    }
    vcall<void>(owner_of(round), 0x7c, static_cast<U>(0x2711));                     // 004D4835..42
    for (I i = 0; i < 5; ++i) {                                                     // 004D4845..62
        const I index = i < 0 ? 0 : i;                                              // 004D4847..4D (inline index clamp)
        w5_combat_attack_clear_004d3010(static_cast<unsigned char*>(round) + 4 + index * 0x14c, nullptr);
    }
    {
        void* o = owner_of(round);
        void* obj = kotor_fastcall_004ae750(server_app(), nullptr, at<U>(o, 0x50c));  // 004D4864..7A
        if (obj == nullptr) at<U>(owner_of(round), 0x50c) = k_object_invalid;        // 004D4881..90
        else if (vcall<void*>(obj, 0x10) != nullptr && vcall<U>(vcall<void*>(obj, 0x10), 0x94) != 0) // 004D4898..B6
            at<U>(owner_of(round), 0x50c) = k_object_invalid;                       // 004D48B8..BE
    }
    static const U k_offsets[4] = {0x524u, 0x504u, 0x528u, 0x520u};
    for (U offset : k_offsets) {                                                    // 004D48CD..4A01 (same shape)
        void* obj = kotor_fastcall_004ae750(server_app(), nullptr, at<U>(owner_of(round), offset));
        if (obj == nullptr || (vcall<void*>(obj, 0x10) != nullptr && vcall<U>(vcall<void*>(obj, 0x10), 0x94) != 0))
            at<U>(owner_of(round), offset) = k_object_invalid;
    }
    kotor_fastcall_005b8010(owner_of(round), nullptr);                              // 004D4A01..07
    U controlled = 0;                                                               // [esp+10]
    void* player = kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr); // 004D4A0C..25 (EBP)
    if (player != nullptr &&
        at<U>(owner_of(round), 4) == kotor_fastcall_004aea30(server_app(), nullptr, at<U>(player, 4))) { // 004D4A27..46
        controlled = 1;                                                             // 004D4A48
    } else {
        w5_creature_set_combat_mode_0050ee80(owner_of(round), nullptr, 0, 1);       // 004D4A52..5B
    }
    void* actions = at<void*>(owner_of(round), 0xfc);                              // 004D4A60..66
    void* head = at<void*>(actions, 0);                                             // 004D4A6C
    if (head != nullptr && flag != 0) {                                             // 004D4A6E..78
        if (at<U>(unresolved_list_at_pos_005e9b60(actions, nullptr, head), 0) == 1) can_run = 0; // 004D4A7E..8B
    }
    if (flag != 0 && can_run != 0 && at<U>(round, 0x93c) == 0 && at<U>(round, 0x940) == 0 && controlled == 0 && // 004D4A93..B9
        vcall<U>(owner_of(round), 0x94) == 0 &&                                     // 004D4ABB..CB
        recovered_creature_incapacitated_004ef890(owner_of(round), nullptr) == 0) { // 004D4ACD..DA
        void* o = owner_of(round);                                                  // 004D4ADC
        frame_R4_005d0fc0(reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)), nullptr,
                          reinterpret_cast<unsigned*>(static_cast<unsigned char*>(o) + 0x260), at<U>(o, 4), 1); // 004D4AE2..F4
    }
    void* client = client_app();                                                    // 004D4AF9..FF (EDI)
    if (client != nullptr && kotor_fastcall_005edc10(client, nullptr) == 0 && flag != 0 && controlled != 0 && // 004D4B02..1B
        (at<U>(kotor_fastcall_005ed700(client, nullptr), 0x14) & 0x800) != 0 &&     // 004D4B1D..2A
        (at<U8>(player, 0x440) & 1) != 0) {                                         // 004D4B2C..33
        kotor_fastcall_005edee0(client, nullptr, 1, 5);                             // 004D4B35..3B
    }
    if (at<U>(round, 0x940) == 0 && controlled != 0 && flag != 0)                   // 004D4B40..55
        kotor_fastcall_005b6980(owner_of(round), nullptr, target_died, at<U>(round, 0x938), next_target); // 004D4B57..6E
    void* o = owner_of(round);
    at<U>(o, 0x53c) = at<U>(o, 0x504);                                              // 004D4B73..7F
    o = owner_of(round);
    at<U>(o, 0x544) = at<U>(o, 0x540);                                              // 004D4B85..91
    if (at<U>(round, 0x938) != 0) {                                                 // 004D4B97..A1
        o = owner_of(round);
        at<U>(o, 0x54c) = at<U>(o, 0x548);                                          // 004D4BA3..AF
        at<U>(owner_of(round), 0x960) = 0;                                          // 004D4BB5..BB
        at<U>(round, 0x938) = 0;                                                    // 004D4BC1
    }
    o = owner_of(round);
    if (at<U>(o, 0x550) != 0) at<U>(o, 0x554) = at<U>(o, 0x550);                    // 004D4BC7..D7
    at<U>(round, 0x984) = 1; at<U>(round, 0x988) = 1;                               // 004D4BE1..ED
    return 1;
}

// 004D5F70 CSWSCombatRound::StartCombatRound(target, master, slave, weapon word, a5): RET 14h
void __fastcall kotor_fastcall_004d5f70(void* round, void*, U target, U master, U slave, U weapon, U a5) {
    at<U>(round, 0x9b8) = master;                                                   // 004D5F81
    at<U>(round, 0x9bc) = slave;                                                    // 004D5F87
    if (master != 0 && slave == 0) kotor_fastcall_004d4620(round, nullptr, at<U>(round, 0x934)); // 004D5F8D..9C
    if (at<U>(round, 0x9bc) != 0) {                                                 // 004D5FA1..AB
        at<U>(round, 0x9c0) = at<U>(owner_of(round), 4);                            // 004D5FAD..BC
        if (at<U>(round, 0x9b8) != 0) {                                             // 004D5FB3..C2
            at<U>(round, 0x9c4) = target;                                           // 004D5FC4
            void* other = kotor_fastcall_004ae770(server_app(), nullptr, target);   // 004D5FCA..D3
            if (other != nullptr) {                                                 // 004D5FD8..DA
                at<U>(at<void*>(other, 0x9c8), 0x9c0) = at<U>(round, 0x9c0);        // 004D5FDC..E8
                at<U>(at<void*>(other, 0x9c8), 0x9c4) = at<U>(round, 0x9c4);        // 004D5FEE..FA
                at<U>(at<void*>(other, 0x9c8), 0x944) = 0;                          // 004D6000..06
            }
        }
    } else {
        at<U>(round, 0x9c0) = target;                                               // 004D600E
    }
    at<U>(round, 0x934) = 1; at<U>(round, 0x944) = 0; at<U>(round, 0x958) = 0;     // 004D6019..29
    at<U>(round, 0x960) = 0; at<U>(round, 0x95c) = k_object_invalid;               // 004D602F..35
    at<U>(round, 0x938) = 0; at<U>(round, 0x93c) = 0; at<U>(round, 0x940) = a5;    // 004D603F..4B
    at<U>(round, 0x94c) = 0xbb8;                                                    // 004D6051
    for (I i = 0; i < 5; ++i) {                                                     // 004D605B..7B
        const I index = i < 0 ? 0 : i;                                              // 004D6060..66 (inline index clamp)
        w5_combat_attack_clear_004d3010(static_cast<unsigned char*>(round) + 4 + index * 0x14c, nullptr);
    }
    kotor_fastcall_004d2a10(round, nullptr, weapon);                                // 004D607D..84
    kotor_fastcall_004d2a70(round, nullptr);                                        // 004D6089..8B
    void* owner = owner_of(round);                                                  // 004D6090
    at<U>(round, 0x998) = 0; at<U>(round, 0x99c) = 0;                               // 004D6096..9C
    at<U>(round, 0x9cc) = at<U>(owner, 4) == target ? k_object_invalid : target;    // 004D60A2..B0
    at<U>(round, 0x9ac) = target;                                                   // 004D60BC
    at<U>(round, 0x978) = 1; at<U>(round, 0x97c) = 0;                               // 004D60C2..CC
    w5_creature_set_lock_orientation_to_object_004f34a0(owner, nullptr, target, 0); // 004D60BA..D2
    at<U>(owner_of(round), 0x540) = 3;                                              // 004D60D7..DF
}

// =========================================================================================================
// 005B8010 CSWSCreature::ClearAttackerList: RET 0. +A80 = list (+0 data), +A84 count.
void __fastcall kotor_fastcall_005b8010(void* creature, void*) {
    void* list = at<void*>(creature, 0xa80);                                        // 005B8014
    if (list == nullptr) return;                                                    // 005B801A..1C
    live_free(at<void*>(list, 0));                                                  // 005B801E..21
    at<void*>(list, 0) = nullptr;                                                   // 005B8027
    live_free(list);                                                                // 005B8026..2D
    at<U>(creature, 0xa80) = 0; at<U>(creature, 0xa84) = 0;                         // 005B8035..3F
}

// 005B6980 CSWSCreature::CreateNewAttackActions(target died, cleave, next target): RET 0Ch
void __fastcall kotor_fastcall_005b6980(void* self, void*, U died, U cleave, U next) {
    U add = 0;                                                                      // EBX
    U target = cleave != 0 ? at<U>(self, 0x524) : at<U>(at<void*>(self, 0x9c8), 0x9cc); // 005B6980..A1 (EBP)
    bool nearest = false;
    if (target != k_object_invalid) {                                               // 005B69A7..AD
        if (at<void*>(at<void*>(at<void*>(at<void*>(self, 0x9c8), 0x9b0), 0), 0) == nullptr) { // 005B69B3..C4
            add = 1;                                                                // 005B69D4
            void* obj = kotor_fastcall_004ae750(server_app(), nullptr, target);     // 005B69CA..D9
            if (obj == nullptr) {                                                   // 005B69E0..E2
                at<U>(at<void*>(self, 0x9c8), 0x9cc) = k_object_invalid;            // 005B6ABC..C2
                add = 0;                                                            // 005B6ACC
            } else if (vcall<void*>(obj, 0x30) != nullptr) {                        // 005B69E8..F1
                void* creature = vcall<void*>(obj, 0x30);                           // 005B69F7..FE
                if (vcall<U>(creature, 0x94) != 0 || recovered_creature_incapacitated_004ef890(creature, nullptr) != 0 || // 005B6A00..1B
                    vcall<U>(self, 0x94) != 0 || recovered_creature_incapacitated_004ef890(self, nullptr) != 0) { // 005B6A21..3C
                    add = 0;                                                        // 005B6ACC
                } else if (static_cast<I>(recovered_object_get_reputation_0057cb80(self, nullptr, target, 0)) > 10) { // 005B6A42..4E
                    add = 0;                                                        // 005B6A50
                } else if (cleave != 0) {                                           // 005B6A57..5D
                    Vector d;                                                       // [esp+10]
                    kotor_cdecl_00423bc0(&d, reinterpret_cast<const Vector*>(static_cast<unsigned char*>(creature) + 0x90),
                                         reinterpret_cast<const Vector*>(static_cast<unsigned char*>(self) + 0x90)); // 005B6A63..76
                    const double range = kotor_fastcall_004ffad0(self, nullptr, at<U>(creature, 4), 0, 1); // 005B6A7B..88
                    // 005B6A8D..B6: ((z*z + y*y) + x*x) against range*range; fcompp, test ah,5; jp keeps the action
                    const double d2 = (static_cast<double>(d.z) * d.z + static_cast<double>(d.y) * d.y) + static_cast<double>(d.x) * d.x;
                    if (range * range < d2) add = 0;                                // 005B6AB8
                }
            }
        }
    } else if (at<void*>(at<void*>(at<void*>(at<void*>(self, 0x9c8), 0x9b0), 0), 0) == nullptr) { // 005B6AD0..E1
        nearest = true;                                                             // 005B6AE1 je 005B6AEF
    }
    if (!nearest && died == 0) {                                                    // 005B6AE3..E9
        if (add == 0) return;                                                       // 005B6C07..09
    } else {
        U found = k_object_invalid;
        void* other = kotor_fastcall_004ae770(server_app(), nullptr, next);         // 005B6AEF..FD
        if (other != nullptr) {                                                     // 005B6B04..06
            const U id = at<U>(self, 4);
            const float range = static_cast<float>(kotor_fastcall_004ffad0(self, nullptr, id, 0, 1)); // 005B6B08..1C (fstp dword)
            found = kotor_fastcall_004f2de0(other, nullptr, range, id, id);         // 005B6B1F
        }
        if (found == k_object_invalid) {                                            // 005B6B26..2C
            const U id = at<U>(self, 4);
            const float range = static_cast<float>(kotor_fastcall_004ffad0(self, nullptr, id, 0, 1)); // 005B6B2E..46
            found = kotor_fastcall_004f2de0(self, nullptr, range, id, k_object_invalid); // 005B6B49
            if (found == k_object_invalid) {                                        // 005B6B50..56 -> 005B6BCD
                w5_creature_set_lock_orientation_to_object_004f34a0(self, nullptr, k_object_invalid, 0); // 005B6BCD..D6
                vcall<void>(self, 0x7c, static_cast<U>(0x2710));                    // 005B6BDB..E4
                w5_creature_set_combat_mode_0050ee80(self, nullptr, 0, 1);          // 005B6BE7..ED
                w5_combat_clear_all_special_attacks_004d4f60(at<void*>(self, 0x9c8), nullptr); // 005B6BF2..F8
                return;
            }
        }
        void* obj = kotor_fastcall_004ae750(server_app(), nullptr, found);          // 005B6B58..62 (no null check)
        if (vcall<void*>(obj, 0x30) == nullptr) {                                   // 005B6B67..72
            kotor_fastcall_005b48b0(self, nullptr, 1);                              // 005B6B74..76
            return;
        }
        w5_creature_set_lock_orientation_to_object_004f34a0(self, nullptr, found, 0); // 005B6B85..88
        at<U>(self, 0x84) = found; at<U>(self, 0x50c) = found;                      // 005B6B94..9A
        kotor_fastcall_004d3e50(at<void*>(self, 0x9c8), nullptr, found);            // 005B6B8D..A0
        at<U>(self, 0x4e8) = 1;                                                     // 005B6BA5
        void* client = client_app();                                                // 005B6BAF..B4 (EBX)
        const U client_id = kotor_fastcall_005eda50(client, nullptr, found);        // 005B6BB7..BE (the pushed 1 stays
        kotor_fastcall_005edd70(client, nullptr, client_id, 1);                     // 005B6BC3..C6  for SetLastTarget)
        target = found;                                                             // 005B6BBC
    }
    kotor_fastcall_004d38b0(at<void*>(self, 0x9c8), nullptr, target, 0, 0, 0x5dc, 1); // 005B6C0B..1D
    if (at<U>(self, 0x510) == target) at<U>(self, 0x510) = k_object_invalid;        // 005B6C22..2A
}

// =========================================================================================================
// 004D6EE0 CSWCCMessageData::CopyTo(CSWCCMessageData* dst): RET 4. +0 byte, +4/+8 integers (SetInteger),
// +10/+14/+18 floats, +1C/+20/+24 CExoStrings, +28/+2C/+30 object ids. The float / id / string copies are the
// inline "set element i, growing the array to exactly i+1" sequences of the original.
void __fastcall kotor_fastcall_004d6ee0(void* src, void*, void* dst) {
    if (dst == nullptr) return;                                                     // 004D6EEC..F4
    at<U>(dst, 8) = 0; at<U>(dst, 0x14) = 0; at<U>(dst, 0x2c) = 0; at<U>(dst, 0x20) = 0; // 004D6EFA..03
    at<U8>(dst, 0) = 0;                                                             // 004D6F06
    for (I i = 0; i < at<I>(src, 8); ++i)                                           // 004D6F09..28
        recovered_event_integer_004d76b0(dst, nullptr, static_cast<U>(i), at<U*>(src, 4)[i]);
    if (at<I>(src, 0x14) > 0) {                                                     // 004D6F2A..2D
        U offset = 0;                                                               // [esp+1C]
        for (I n = 1; n - 1 < at<I>(src, 0x14); ++n, offset += 4) {                 // 004D6F39..D2 (EDI = n)
            const U value = x87_float_copy(*reinterpret_cast<U*>(at<unsigned char*>(src, 0x10) + offset)); // 004D6F40..4C
            if (n - 1 >= at<I>(dst, 0x14)) {                                        // 004D6F49..52 (jl: signed)
                if (n == 0) {                                                       // 004D6F54..56 (dead: n >= 1)
                    if (at<void*>(dst, 0x10) != nullptr) {
                        live_free(at<void*>(dst, 0x10)); at<U>(dst, 0x10) = 0; at<U>(dst, 0x18) = 0; // 004D6F58..6B
                    }
                } else {
                    U* old = at<U*>(dst, 0x10);                                     // 004D6F70
                    at<I>(dst, 0x18) = n;                                           // 004D6F77
                    at<void*>(dst, 0x10) = live_operator_new(offset + 4);           // 004D6F73..82
                    for (I k = 0; k < at<I>(dst, 0x14); ++k) at<U*>(dst, 0x10)[k] = old[k]; // 004D6F88..A0
                    if (old != nullptr) live_free(old);                             // 004D6FA2..AC
                }
                at<I>(dst, 0x14) = n;                                               // 004D6FB5
            }
            *reinterpret_cast<U*>(at<unsigned char*>(dst, 0x10) + offset) = value;  // 004D6FB8..C0
        }
    }
    if (at<I>(src, 0x2c) > 0) {                                                     // 004D6FD8..DB
        U offset = 0;                                                               // EBP
        for (I n = 1; n - 1 < at<I>(src, 0x2c); ++n, offset += 4) {                 // 004D6FE1..71
            const U value = *reinterpret_cast<U*>(at<unsigned char*>(src, 0x28) + offset); // 004D6FE6..EC
            if (n - 1 >= at<I>(dst, 0x2c)) {                                        // 004D6FF0..F8
                if (n == 0) {                                                       // 004D6FFA..FC (dead)
                    if (at<void*>(dst, 0x28) != nullptr) {
                        live_free(at<void*>(dst, 0x28)); at<U>(dst, 0x28) = 0; at<U>(dst, 0x30) = 0; // 004D6FFE..13
                    }
                } else {
                    U* old = at<U*>(dst, 0x28);                                     // 004D7018
                    at<I>(dst, 0x30) = n;                                           // 004D701F
                    at<void*>(dst, 0x28) = live_operator_new(offset + 4);           // 004D701B..2A
                    for (I k = 0; k < at<I>(dst, 0x2c); ++k) at<U*>(dst, 0x28)[k] = old[k]; // 004D7030..45
                    if (old != nullptr) live_free(old);                             // 004D7047..51
                }
                at<I>(dst, 0x2c) = n;                                               // 004D7058
            }
            *reinterpret_cast<U*>(at<unsigned char*>(dst, 0x28) + offset) = value;  // 004D705B..63
        }
    }
    if (at<I>(src, 0x20) > 0) {                                                     // 004D7079..7C
        U offset = 0;                                                               // [esp+1C]
        void* list = static_cast<unsigned char*>(dst) + 0x1c;                       // EDI
        for (I n = 1; n - 1 < at<I>(src, 0x20); ++n, offset += 8) {                 // 004D7089..10C (EBP = n)
            void* from = at<unsigned char*>(src, 0x1c) + offset;                    // 004D7090..A1
            if (n - 1 >= at<I>(dst, 0x20)) {                                        // 004D7097..A5
                if (n == 0) {                                                       // 004D70A7..A9 (dead)
                    unsigned char* data = at<unsigned char*>(list, 0);
                    if (data != nullptr) {                                          // 004D70AB..AF
                        c2da_destroy_array_006fa852(data, 8, at<I>(data - 4, 0),
                                                    reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20)); // 004D70B1..C0
                        live_free(data - 4);                                        // 004D70C5..C6
                        at<U>(list, 0) = 0; at<U>(list, 8) = 0;                     // 004D70D2..D4
                    }
                } else {
                    kotor_fastcall_00405080(list, nullptr, n);                      // 004D70D9..DC
                }
                at<I>(list, 4) = n;                                                 // 004D70E1
            }
            async_assign_string_005e5c50(at<unsigned char*>(list, 0) + offset, nullptr, from); // 004D70E4..ED
        }
    }
    at<U8>(dst, 0) = at<U8>(src, 0);                                                // 004D710E..10
}
}  // extern "C"
