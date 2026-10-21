// Lane W4 GUI successor (worker "levelup", fork "lu-crules"): client-side level-up rules and the level-up message that
// had no directly callable provider in the c31 candidate map. Transcribed from the original swkotor.exe linear sweep
// (read-only); every statement carries its original PCs. All entries are original thiscall members exported as
// extern "C" __fastcall(self, unused EDX, stack words...) with callee cleanup (@N = 8 + 4 * stack words).
//   00649070 CSWCLevelUpStats::ComputeNumberKnownSpellsLeft()              RET 0   -> BYTE   @kotor_fastcall_00649070@8
//   005BCD30 CSWClass::GetSpellGain(BYTE level, BYTE spell_level)           RET 8   -> BYTE   @kotor_fastcall_005bcd30@16
//   006492C0 CSWCCreatureStats::HasSpell(BYTE list, DWORD spell)            RET 8   -> int    @kotor_fastcall_006492c0@16
//   006497A0 CSWCLevelUpStats::HasSpellPrereq(CSWSpell*, CExoArrayList<DWORD>*) RET 8 -> int  @kotor_fastcall_006497a0@16
//   00649B50 CSWCLevelUpStats::CanChooseSpell(DWORD, BYTE left, CExoArrayList<DWORD>*) RET 0Ch -> int @kotor_fastcall_00649b50@20
//   00649C30 CSWCLevelUpStats::AddDefaultPackageSpells(CExoArrayList<DWORD>*) RET 4 -> BYTE  @kotor_fastcall_00649c30@12
//   0064A1F0 CSWCLevelUpStats::SetClass(BYTE index, BYTE class)            RET 8             @kotor_fastcall_0064a1f0@16
//   0064A2A0 CSWCLevelUpStats::SetClassLevel(BYTE index, BYTE level)       RET 8             @kotor_fastcall_0064a2a0@16
//   00649580 CSWCCreatureStats::ClearKnownSpells(BYTE index)               RET 4             @kotor_fastcall_00649580@12
//   00678280 CSWCMessage::SendPlayerToServer_LevelUp(CSWCCreatureStats* old, CSWCLevelUpStats* new) RET 8 -> int
//                                                                                            @kotor_fastcall_00678280@16
// 005BCD30 is reconstructed here because its only body in the map (0986 ?creature_class_slots_005bcd30@server_mainloop)
// takes a Memory& port and is not directly callable.
#include "6030a9ee65-claude_gui_char.hpp"
#include <cmath>

namespace recovered_save_stats {
unsigned short __cdecl spell_count_006487d0(void* stats, unsigned cls, unsigned level);   // CSWCCreatureStats::GetNumberKnownSpells (0859)
}

extern "C" {
// ---- client creature stats (0819 m15b / claude_s2p_small_debug)
w4::U8 __fastcall kotor_m15b_creature_stats_get_class_00647730(void* stats, void*, w4::U32 index);      // GetClass RET 4
std::int32_t __fastcall kotor_m15b_levelup_stats_get_level_00648430(void* self, void*);                 // CSWCLevelUpStats::GetLevel (AL)
w4::U16 __fastcall kotor_m15b_creature_stats_get_feat_006486e0(void* stats, void*, w4::U32 index);       // GetFeat RET 4
w4::U32 __fastcall kotor_m15b_levelup_stats_feat_acquired_00649260(void* stats, void*, w4::U32 feat, void* list); // FeatAcquired RET 8
w4::U32 __fastcall kotor_fastcall_006495a0(void* stats, void*, w4::U32 cls, w4::U32 level, w4::U32 index); // GetKnownSpell RET 0Ch
void __fastcall kotor_m15b_class_info_clear_known_spells_006488b0(void* class_info, void*);             // ClassInfo::ClearKnownSpells
void __fastcall kotor_m15b_levelup_stats_set_fort_saving_throw_00648b30(void* self, void*);              // SetFortSavingThrow
void __fastcall kotor_m15b_levelup_stats_set_will_saving_throw_00648be0(void* self, void*);              // SetWillSavingThrow
void __fastcall kotor_m15b_levelup_stats_set_reflex_saving_throw_00648c90(void* self, void*);            // SetReflexSavingThrow
void __fastcall kotor_m15b_creature_stats_initialize_combat_information_00649d60(void* self, void*);     // InitializeCombatInformation
// ---- rules
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, w4::U32 id);            // CSWSpellArray::GetSpell RET 4
w4::U32 __fastcall kotor_fastcall_0059b650(void* spell, void*, w4::U32 cls);                             // CSWSpell::GetSpellLevel (AL) RET 4
w4::U32 __fastcall kotor_fastcall_005be6d0(void* cls, void*, w4::U32 alignment, w4::U32 index);          // CSWClass::GetRecommendedSpell RET 8 (fork lu-srules, srcw5)
// ---- messages (w2_message_native / net layer)
w4::U32 __fastcall kotor_fastcall_004d68a0(void* message, void*, w4::U32 size, w4::U32, w4::U32);        // CSWMessage::CreateWriteMessage RET 0Ch
void __fastcall kotor_fastcall_004d62c0(void* message, void*, w4::U32 value, w4::U32 bits);              // CSWMessage::WriteBYTE RET 8
void __fastcall kotor_fastcall_004d6300(void* message, void*, w4::U32 value, w4::U32 bits);              // CSWMessage::WriteWORD RET 8
void __fastcall kotor_fastcall_004d6340(void* message, void*, w4::U32 value, w4::U32 bits);              // CSWMessage::WriteDWORD RET 8
w4::U32 __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, w4::U32* size);           // CSWMessage::GetWriteMessage RET 8
int __fastcall net_layer_send_message_to_player_005d4cb0(void* net, void*, w4::U32 player, const char* data, w4::U32 size, w4::U32 flags); // RET 10h
// ---- defined here
w4::U8 __fastcall kotor_fastcall_005bcd30(void* cls, void*, w4::U32 level, w4::U32 spell_level);
w4::U8 __fastcall kotor_fastcall_00649070(void* self, void*);
w4::U32 __fastcall kotor_fastcall_006492c0(void* self, void*, w4::U32 list, w4::U32 spell);
w4::U32 __fastcall kotor_fastcall_006497a0(void* self, void*, void* spell, void* list);
w4::U32 __fastcall kotor_fastcall_00649b50(void* self, void*, w4::U32 spell, w4::U32 left, void* list);
w4::U8 __fastcall kotor_fastcall_00649c30(void* self, void*, void* list);
void __fastcall kotor_fastcall_0064a1f0(void* self, void*, w4::U32 index, w4::U32 cls);
void __fastcall kotor_fastcall_0064a2a0(void* self, void*, w4::U32 index, w4::U32 level);
void __fastcall kotor_fastcall_00649580(void* self, void*, w4::U32 index);
w4::U32 __fastcall kotor_fastcall_00678280(void* self, void*, void* old_stats, void* new_stats);
}

using namespace gchar;

namespace {
// .rdata 0074B31C / 0074B31D / 0074B31E: the three Jedi class ids (bytes 03, 04, 05).
constexpr U8 k_class_0074b31c = 0x03;
constexpr U8 k_class_0074b31d = 0x04;
constexpr U8 k_class_0074b31e = 0x05;
constexpr double k_half_0073f3d0 = 0.5;                                                     // .rdata 0073F3D0 (qword 0.5)
constexpr U32 k_server_player_0074c528 = 0xFFFFFFFDu;                                       // .rdata 0074C528 (immutable)

// Class row in CSWRules::m_pClasses ([rules+ACh] + class * 1A0h).
inline void* class_row(U8 cls) { return pt(lw(rules(), 0xAC) + static_cast<U32>(cls) * 0x1A0u); }
// Inline "last class" pattern: idx = [stats+32h]-1; idx < 2 ? [stats + idx*20h + F0h] : FFh.
inline U8 last_class(void* stats) {
    const U8 idx = static_cast<U8>(lb(stats, 0x32) - 1);
    return idx < 2 ? lb(stats, static_cast<U32>(idx) * 0x20u + 0xF0u) : 0xFFu;
}
inline U16 spell_count(void* stats, U8 idx) { return recovered_save_stats::spell_count_006487d0(stats, idx, 0); }
// Inlined CExoArrayList<DWORD>::Add of 00678280: grow to cap*2 (10h when 0) with operator new + copy + _free.
void add_inline(PtrList& list, U32 value) {
    if (list.size == list.capacity) {                                                       // 00678561..00678569
        const I32 cap = list.capacity != 0 ? list.capacity * 2 : 0x10;                     // 0067856B..00678573
        void** const old = list.data;                                                       // 00678578
        list.capacity = cap;                                                                // 00678584
        void** const fresh = static_cast<void**>(live_operator_new(static_cast<U32>(cap) * 4u)); // 0067857C..00678588
        list.data = fresh;                                                                  // 00678598
        for (I32 i = 0; i < list.size; ++i) fresh[i] = old[i];                              // 00678594..006785AB
        if (old != nullptr) live_free(old);                                                 // 006785B1..006785B6
    }
    list.data[list.size] = pt(value);                                                       // 006785C4..006785CC
    ++list.size;                                                                            // 006785CF..006785D0
}
// Spells of `from` (class idx, list 0) that `other` does not know, in order (00678504..006785EF / 00678610..006786FF).
void collect_new_spells(void* from, void* other, U8 idx, PtrList& out) {
    if (spell_count(from, idx) == 0) return;                                                // 006784F6..006784FE (test ax; jbe)
    U8 j = 0;                                                                               // [esp+54h] / [esp+54h] low byte
    do {
        const U32 spell = kotor_fastcall_006495a0(from, nullptr, idx, 0, j);               // 00678504..0067850E
        U32 found = 0; U8 k = 0;                                                            // 00678513..00678515
        for (;;) {
            if (!(static_cast<U16>(k) < spell_count(other, idx))) break;                   // 00678520..00678533 (jae)
            if (kotor_fastcall_006495a0(other, nullptr, idx, 0, k) == spell) found = 1;    // 00678535..0067854C
            ++k;                                                                            // 00678551
            if (found != 0) break;                                                          // 00678553..0067855B
        }
        if (found == 0) add_inline(out, spell);                                             // 0067855D..006785D0
        ++j;                                                                                // 006785D4..006785DA
    } while (static_cast<U16>(j) < spell_count(from, idx));                                 // 006785D8..006785EF (jb)
}
}  // namespace

extern "C" {

// 005BCD30 CSWClass::GetSpellGain(BYTE level, BYTE spell_level) RET 8: 1..14h and spell_level < [this+113h+level]
// -> [this+127h+level], else FFh.
w4::U8 __fastcall kotor_fastcall_005bcd30(void* cls, void*, w4::U32 level, w4::U32 spell_level) {
    const U8 l = static_cast<U8>(level);                                                    // 005BCD30
    if (l < 1 || l > 0x14) return 0xFF;                                                     // 005BCD34..005BCD3A -> 005BCD59
    if (!(static_cast<U8>(spell_level) < lb(cls, 0x113u + l))) return 0xFF;                 // 005BCD3C..005BCD4E (jae)
    return lb(cls, 0x127u + l);                                                             // 005BCD50..005BCD56
}

// 00649070 CSWCLevelUpStats::ComputeNumberKnownSpellsLeft() RET 0 -> AL.
w4::U8 __fastcall kotor_fastcall_00649070(void* self, void*) {
    const U8 idx = static_cast<U8>(lb(self, 0x32) - 1);                                     // 00649075..00649078
    const U8 cls = idx < 2 ? lb(self, static_cast<U32>(idx) * 0x20u + 0xF0u) : 0xFFu;      // 0064907A..0064908A
    void* const row = class_row(cls);                                                       // 00649091..006490A6
    if (row == nullptr) return 0;                                                           // 006490A8..006490B0
    if (cls != k_class_0074b31d && cls != k_class_0074b31c && cls != k_class_0074b31e) return 0; // 006490B1..006490C7
    const U8 level = idx < 2 ? lb(self, static_cast<U32>(idx) * 0x20u + 0xF1u) : 0;        // 006490C9..006490E1
    const U8 gain = kotor_fastcall_005bcd30(row, nullptr, level, 0);                       // 006490E5..006490EC
    return gain == 0xFF ? 0 : gain;                                                         // 006490F1..00649106 (sete/dec/and)
}

// 006492C0 CSWCCreatureStats::HasSpell(BYTE list, DWORD spell) RET 8: any class's known-spell list `list` holds spell.
w4::U32 __fastcall kotor_fastcall_006492c0(void* self, void*, w4::U32 list, w4::U32 spell) {
    U8 i = 0;                                                                               // 006492C8
    if (lb(self, 0x32) != 0) {                                                              // 006492C4..006492CD
        const U32 offset = static_cast<U32>(static_cast<U8>(list)) * 12u;                   // 006492CF..006492D7
        do {
            U8* const entry = static_cast<U8*>(self) + static_cast<U32>(i) * 0x20u + 0xD4u + offset; // 006492E0..006492E6
            U32 n = 0;                                                                      // 006492ED
            if (lh(entry, 4) != 0) {                                                        // 006492EF..006492F4 (jbe)
                const U8* p = static_cast<const U8*>(lp(entry, 0));                         // 006492F6
                do {
                    if (lw(p, 0) == spell) return 1;                                        // 00649300..00649306 -> 00649327
                    ++n; p += 4;                                                            // 0064930D..0064930E
                } while (static_cast<I32>(n) < static_cast<I32>(lh(entry, 4)));              // 00649308..00649313 (jl)
            }
            ++i;                                                                            // 00649318
        } while (i < lb(self, 0x32));                                                       // 00649315..0064931C
    }
    return 0;                                                                               // 0064931E..00649324
}

// 006497A0 CSWCLevelUpStats::HasSpellPrereq(CSWSpell* spell, CExoArrayList<DWORD>* chosen) RET 8.
w4::U32 __fastcall kotor_fastcall_006497a0(void* self, void*, void* spell, void* list) {
    // Both FFFFFFFFh-terminated prerequisite arrays ([spell+174h], then [spell+160h]) must be fully satisfied: known
    // (HasSpell(0, id)) or already chosen in `list`.
    const U32 arrays[2] = {0x174, 0x160};                                                   // 006497AA / 00649800
    for (U32 a = 0; a < 2; ++a) {
        const U8* entry = static_cast<const U8*>(spell) + arrays[a];
        for (U32 id = lw(entry, 0); id != 0xFFFFFFFFu; entry += 4, id = lw(entry, 0)) {    // 006497B0..006497B8 / 006497F1..006497FA
            U32 r = kotor_fastcall_006492c0(self, nullptr, 0, id);                         // 006497C0..006497C5
            if (r != 0) continue;                                                           // 006497CA..006497CC
            const I32 n = li(list, 4);                                                      // 006497CE
            if (n <= 0) return 0;                                                           // 006497D1..006497D3 -> 006497E3
            const U8* data = static_cast<const U8*>(lp(list, 0));                           // 006497D5
            for (;;) {                                                                      // eax counts from HasSpell's 0
                if (lw(data, r * 4u) == id) break;                                          // 006497D7..006497D9
                ++r;                                                                        // 006497DB
                if (!(static_cast<I32>(r) < n)) return 0;                                   // 006497DF..006497E1 -> 006497E3
            }
            if (r == 0xFFFFFFFFu) return 0;                                                 // 006497EC..006497EF (never true)
        }
    }
    const U8 cls = last_class(self);                                                        // 00649855..00649872 -> [esp+18h] low byte
    const U32 cls_word = (ad(list) & 0xFFFFFF00u) | cls;                                    // 00649876 dword = arg2 slot with low byte replaced
    const U8 spell_level = static_cast<U8>(kotor_fastcall_0059b650(spell, nullptr, cls_word)); // 0064987A..00649886
    const U8 level = static_cast<U8>(kotor_m15b_levelup_stats_get_level_00648430(self, nullptr)); // 00649884..00649888
    return level < spell_level ? 0u : 1u;                                                   // 0064988E..00649895 (cmp/sbb/inc)
}

// 00649B50 CSWCLevelUpStats::CanChooseSpell(DWORD spell, BYTE left, CExoArrayList<DWORD>* chosen) RET 0Ch.
w4::U32 __fastcall kotor_fastcall_00649b50(void* self, void*, w4::U32 spell_id, w4::U32 left, void* list) {
    const U8 idx = static_cast<U8>(lb(self, 0x32) - 1);                                     // 00649B60..00649B6A
    void* const spell = kotor_m15b_spell_array_get_spell_0059b6d0(lp(rules(), 0x8C), nullptr, spell_id); // 00649B51..00649B6C
    if (spell == nullptr) return 0;                                                         // 00649B71..00649B82
    const U8 user = lb(spell, 0x140);                                                       // 00649B85
    if (user != 1 && user != 5) return 0;                                                   // 00649B8B..00649B91
    if (static_cast<U8>(left) == 0) return 0;                                               // 00649B93..00649B99
    for (U32 i = 0;; ++i) {                                                                 // 00649B9B / 00649BD3
        if (!(idx < 2)) break;                                                              // 00649BA0..00649BA3 (jae)
        U8* const info = static_cast<U8*>(self) + static_cast<U32>(idx) * 0x20u;            // 00649BA5..00649BAB
        if (!(static_cast<U16>(i) < lh(info, 0xD8))) break;                                 // 00649BAD..00649BB4 (jae)
        const I32 k = static_cast<U8>(i);                                                   // 00649BB6 movzx edx, al
        const U32 known = k < li(info, 0xD8) ? lw(lp(info, 0xD4), static_cast<U32>(k) * 4u) : 0xFFFFFFFFu; // 00649BB9..00649BCC
        if (spell_id == known) return 0;                                                    // 00649BCF..00649BD1
    }
    const I32 n = li(list, 4);                                                              // 00649BD6..00649BDA
    if (n > 0) {                                                                            // 00649BDF..00649BE1
        const U8* data = static_cast<const U8*>(lp(list, 0));                               // 00649BE3
        U16 j = 0; U32 k = 0;                                                               // 00649BDD / 00649BE5
        do {
            if (spell_id == lw(data, k * 4u)) return 0;                                     // 00649BE7..00649BEE
            ++j; k = j;                                                                     // 00649BF0..00649BF1
        } while (static_cast<I32>(k) < n);                                                  // 00649BF4..00649BF6
    }
    if (kotor_fastcall_006497a0(self, nullptr, spell, list) == 0) return 0;                 // 00649BFC..00649C07
    const I32 remaining = static_cast<I32>(static_cast<U8>(left)) - li(list, 4);            // 00649C0D..00649C16
    return remaining > 0 ? 1u : 0u;                                                         // 00649C19..00649C1E (setg)
}

// 00649C30 CSWCLevelUpStats::AddDefaultPackageSpells(CExoArrayList<DWORD>* chosen) RET 4 -> AL (spells left).
w4::U8 __fastcall kotor_fastcall_00649c30(void* self, void*, void* list) {
    const U8 left = kotor_fastcall_00649070(self, nullptr);                                 // 00649C3C..00649C41 [esp+0Ch]
    const U8 cls = last_class(self);                                                        // 00649C45..00649C59
    const I32 spell_total = li(lp(rules(), 0x8C), 0);                                       // 00649C60..00649C6D [[rules+8Ch]] -> [esp+18h]
    void* const row = class_row(cls);                                                       // 00649C71..00649C87
    I32 added = 0;                                                                          // 00649C8D [esp+10h]
    I32 index = 1;                                                                          // 00649C91 ebx
    if (lw(row, 0x19C) == 0) return left;                                                   // 00649C80..00649C96
    const std::int16_t align = static_cast<std::int16_t>(lh(self, 0x80));                   // 00649C9C
    U8 band;
    if (align <= 0x28) band = 3;                                                            // 00649CA3..00649CAB (jg)
    else band = static_cast<U8>((align >= 0x3C ? 1 : 0) + 1);                               // 00649CAD..00649CB4
    U32 flag = 1;                                                                           // 00649CB8 [esp+14h]
    if (band == 3) flag = 0;                                                                // 00649CB6..00649CBE
    const I32 wanted = left;                                                                // 00649CC2..00649CC9 [esp+20h]
    if (wanted <= 0) return left;                                                           // 00649CC7..00649CCD
    do {
        if (index > spell_total) break;                                                     // 00649CD4..00649CD8 (jg)
        const U32 spell = static_cast<U16>(kotor_fastcall_005be6d0(row, nullptr, flag, static_cast<U32>(index))); // 00649CDA..00649CE7 movzx
        if (spell == 0xFFFFFFFFu) continue;                                                 // 00649CEA..00649CED -> 00649D3A (never: zero-extended)
        if (kotor_fastcall_00649b50(self, nullptr, spell, left, list) != 0) {              // 00649CEF..00649D01
            gchar::list_add(*static_cast<PtrList*>(list), pt(spell));                       // 00649D03..00649D32 (Allocate 005E03C0)
            ++added;                                                                        // 00649D2C..00649D35
        }
        ++index;                                                                            // 00649D39
    } while (added < wanted);                                                               // 00649D3A..00649D42
    return left;                                                                            // 00649D45..00649D4F
}

// 0064A1F0 CSWCLevelUpStats::SetClass(BYTE index, BYTE class) RET 8.
void __fastcall kotor_fastcall_0064a1f0(void* self, void*, w4::U32 index, w4::U32 cls) {
    const U8 idx = static_cast<U8>(index);                                                  // 0064A1F0
    if (idx >= 2) return;                                                                   // 0064A1F4..0064A1F9
    const U8 c = static_cast<U8>(cls);                                                      // 0064A203
    sb(self, static_cast<U32>(idx) * 0x20u + 0xF0u, c);                                     // 0064A1FF..0064A20A
    kotor_m15b_levelup_stats_set_reflex_saving_throw_00648c90(self, nullptr);               // 0064A211
    kotor_m15b_levelup_stats_set_will_saving_throw_00648be0(self, nullptr);                 // 0064A216..0064A218
    kotor_m15b_levelup_stats_set_fort_saving_throw_00648b30(self, nullptr);                 // 0064A21D..0064A21F
    if (lh(self, 0x48) == 0) {                                                              // 0064A224..0064A229
        const U16 hit_die = lb(class_row(c), 0x54);                                         // 0064A22B..0064A241 movzx di
        const I32 con = lb(self, 0x36);                                                     // 0064A247..0064A24B (into the arg slot)
        std::memcpy(static_cast<U8*>(self) + 0x48, &hit_die, 2);                            // 0064A252
        const double half = std::floor(static_cast<double>(con) * k_half_0073f3d0);        // 0064A256..0064A263 fild/fmul/floor
        const U8 t = static_cast<U8>(static_cast<I32>(half));                               // 0064A26B __ftol2 (exact: integral, in range)
        U16 hp = static_cast<U16>(static_cast<std::int16_t>(static_cast<std::int8_t>(static_cast<U8>(t - 5))) + hit_die); // 0064A270..0064A276
        if (static_cast<std::int16_t>(hp) < 1) hp = 1;                                      // 0064A278..0064A27F (jge)
        std::memcpy(static_cast<U8*>(self) + 0x4E, &hp, 2);                                 // 0064A284
        std::memcpy(static_cast<U8*>(self) + 0x4C, &hp, 2);                                 // 0064A288
    }
    kotor_m15b_creature_stats_initialize_combat_information_00649d60(self, nullptr);        // 0064A28C..0064A28E
}

// 0064A2A0 CSWCLevelUpStats::SetClassLevel(BYTE index, BYTE level) RET 8.
void __fastcall kotor_fastcall_0064a2a0(void* self, void*, w4::U32 index, w4::U32 level) {
    const U8 idx = static_cast<U8>(index);                                                  // 0064A2A0
    if (idx >= 2) return;                                                                   // 0064A2A4..0064A2A9
    sb(self, static_cast<U32>(idx) * 0x20u + 0xF1u, static_cast<U8>(level));                // 0064A2AB..0064A2B5
    kotor_m15b_levelup_stats_set_reflex_saving_throw_00648c90(self, nullptr);               // 0064A2BC..0064A2BE
    kotor_m15b_levelup_stats_set_will_saving_throw_00648be0(self, nullptr);                 // 0064A2C3..0064A2C5
    kotor_m15b_levelup_stats_set_fort_saving_throw_00648b30(self, nullptr);                 // 0064A2CA..0064A2CC
    kotor_m15b_creature_stats_initialize_combat_information_00649d60(self, nullptr);        // 0064A2D1..0064A2D3
}

// 00649580 CSWCCreatureStats::ClearKnownSpells(BYTE index) RET 4.
void __fastcall kotor_fastcall_00649580(void* self, void*, w4::U32 index) {
    const U8 idx = static_cast<U8>(index);                                                  // 00649580
    if (idx >= 2) return;                                                                   // 00649584..00649586
    kotor_m15b_class_info_clear_known_spells_006488b0(static_cast<U8*>(self) + static_cast<U32>(idx) * 0x20u + 0xD4u, nullptr); // 00649588..00649595
}

// 00678280 CSWCMessage::SendPlayerToServer_LevelUp(CSWCCreatureStats* old, CSWCLevelUpStats* stats) RET 8 -> EAX.
// Wire (after the 3-byte header 70h 1Dh 00h): BYTE class; [level%4==0 and an ability rose: BYTE ability index];
// DWORD changed-skill mask; per set bit BYTE rank delta; WORD skill points; BYTE feat-count delta; WORD per new feat;
// BYTE n + n DWORD added spells; BYTE m + m DWORD spells of `old` missing from `stats`.
w4::U32 __fastcall kotor_fastcall_00678280(void* self, void*, void* old_stats, void* stats) {
    void* const message = self;                                                             // 006782A0 ebx / [esp+20h]
    U32 size = 0x64;                                                                        // 006782A8 [esp+1Ch]
    void* data = nullptr;                                                                   // [esp+24h]
    kotor_fastcall_004d68a0(message, nullptr, 0x64, 0xFFFFFFFFu, 1);                        // 0067829C..006782B0
    {
        const U8 idx = static_cast<U8>(lb(stats, 0x32) - 1);                                // 006782B9..006782BE
        const U8 cls = kotor_m15b_creature_stats_get_class_00647730(stats, nullptr, idx);   // 006782C2..006782C5
        kotor_fastcall_004d62c0(message, nullptr, cls, 8);                                  // 006782CA..006782CD
    }
    const U8 level = static_cast<U8>(kotor_m15b_levelup_stats_get_level_00648430(stats, nullptr)); // 006782D2..006782D9
    if ((level & 3u) == 0) {                                                                // 006782DC..006782EC (and 80000003h; level >= 0)
        for (U32 a = 0; a < 6; ++a) {                                                       // 006782EE..0067833A STR..CHA bases +3Ah..+3Fh
            if (lb(stats, 0x3A + a) > lb(old_stats, 0x3A + a)) {                            // (jbe = not above)
                kotor_fastcall_004d62c0(message, nullptr, a, 8);                            // 006782F6..00678342
                break;
            }
        }
    }
    U32 mask = 0;                                                                           // 00678357 [esp+14h]
    {
        const U8 n = lb(rules(), 0xAB);                                                     // 00678347..0067835F [esp+13h]
        if (n != 0) {                                                                       // 00678355..00678363 (jbe)
            U8 i = 0;
            do {
                const U8 a = i < n ? lb(lp(stats, 0x88), i) : 0;                            // 00678365..00678385 [esp+54h]
                const U8 b = i < n ? lb(lp(old_stats, 0x88), i) : 0;                        // 00678389..006783A1
                if (a != b) mask |= 1u << (i & 31u);                                        // 006783A4..006783B7
                ++i;                                                                        // 006783BF
            } while (i < n);                                                                // 006783BB..006783C3
        }
    }
    kotor_fastcall_004d6340(message, nullptr, mask, 0x20);                                  // 006783C5..006783CE
    {
        U8 n = lb(rules(), 0xAB);                                                           // 006783D3..006783D9
        if (n != 0) {                                                                       // 006783E1..006783E7
            U8 i = 0;                                                                       // [esp+54h]
            do {
                if ((mask & (1u << (i & 31u))) != 0) {                                      // 006783F0..006783FD
                    const U8 a = i < n ? lb(lp(stats, 0x88), i) : 0;                        // 006783FF..00678419
                    const U8 b = i < n ? lb(lp(old_stats, 0x88), i) : 0;                    // 0067841C..00678432
                    kotor_fastcall_004d62c0(message, nullptr, static_cast<U8>(a - b), 8);   // 00678435..0067843C
                }
                n = lb(rules(), 0xAB);                                                      // 00678445..0067844A
                ++i;                                                                        // 00678450
            } while (i < n);                                                                // 00678452..00678458
        }
    }
    kotor_fastcall_004d6300(message, nullptr, lh(stats, 0xAE), 0x10);                       // 0067845A..00678468 skill points
    kotor_fastcall_004d62c0(message, nullptr, static_cast<U8>(lw(stats, 0xB4) - lw(old_stats, 0xB4)), 8); // 0067846D..00678480
    if (lh(stats, 0xB4) != 0) {                                                             // 00678485..0067848E (jbe)
        U16 i = 0;
        do {
            const U32 feat = kotor_m15b_creature_stats_get_feat_006486e0(stats, nullptr, i);    // 00678490..00678493 (zero-extended)
            if (kotor_m15b_levelup_stats_feat_acquired_00649260(old_stats, nullptr, feat, nullptr) == 0) // 00678498..006784A8
                kotor_fastcall_004d6300(message, nullptr, feat, 0x10);                     // 006784AA..006784AF
            ++i;                                                                            // 006784B4
        } while (i < lh(stats, 0xB4));                                                      // 006784B5..006784BC
    }
    {
        const U8 idx = static_cast<U8>(lb(stats, 0x32) - 1);                                // 006784BE..006784C3 [esp+18h] low byte
        PtrList added;                                                                      // 006784C9..006784D1 {[esp+34h],[esp+38h],[esp+3Ch]} (EH state 0)
        PtrList dropped;                                                                    // 006784D9..006784E1 {[esp+28h],[esp+2Ch],[esp+30h]} (EH state 1)
        collect_new_spells(stats, old_stats, idx, added);                                   // 006784E5..006785EF
        collect_new_spells(old_stats, stats, idx, dropped);                                 // 006785F5..006786FF
        const I32 na = added.size;                                                          // 00678705
        kotor_fastcall_004d62c0(message, nullptr, static_cast<U32>(na), 8);                 // 0067870D..00678712
        if (na > 0) {                                                                       // 00678719..0067871B
            U8 b = 0;
            do { kotor_fastcall_004d6340(message, nullptr, ad(added.data[b]), 0x20); ++b; } // 00678721..00678731
            while (static_cast<I32>(b) < na);                                               // 00678733..00678738
        }
        const I32 nd = dropped.size;                                                        // 0067873A
        kotor_fastcall_004d62c0(message, nullptr, static_cast<U32>(nd), 8);                 // 0067873E..00678743
        if (nd > 0) {                                                                       // 0067874E..00678750
            U8 b = 0;
            do { kotor_fastcall_004d6340(message, nullptr, ad(dropped.data[b]), 0x20); ++b; } // 00678752..00678762
            while (static_cast<I32>(b) < nd);                                               // 00678764..00678769
        }
    }                                                                                       // 0067876B..00678783 _free(dropped), _free(added)
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;             // 0067878B..0067879E -> 006787C4
    // Inlined CSWCMessage::SendPlayerToServerMessage(1Dh, 00h, data, size).
    U32 length = size;                                                                      // 006787A0..006787A6 edi / ebx
    const U32 owned = size;
    char* buffer;
    if (size != 0) {
        buffer = static_cast<char*>(data);                                                  // 006787AA
    } else {
        length = 3;                                                                         // 006787B0
        buffer = static_cast<char*>(live_operator_new(3));                                  // 006787B5..006787BE
    }
    if (buffer == nullptr) return 0;                                                        // 006787C0..006787C6
    buffer[0] = 0x70;                                                                       // 006787C8
    buffer[1] = 0x1D;                                                                       // 006787CB
    buffer[2] = 0x00;                                                                       // 006787CF
    void* const client = client_app();                                                      // 006787D3..006787D9 [[007A39FC]+4]
    void* const net = vcall<void*>(client, 0x10);                                           // 006787E1..006787E8
    const U32 result = static_cast<U32>(net_layer_send_message_to_player_005d4cb0(net, nullptr, k_server_player_0074c528,
                                                                                  buffer, length, 0)); // 006787DC..006787ED
    if (owned == 0) live_free(buffer);                                                      // 006787F2..006787FE
    return result;                                                                          // 00678801..00678815
}

}  // extern "C"
