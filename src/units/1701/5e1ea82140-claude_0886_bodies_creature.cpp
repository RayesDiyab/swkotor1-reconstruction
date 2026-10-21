// 0886-gates (Claude): faithful bodies for former required(0x...) gates of the 0886 cohort unit on the creature
// stats load paths (creature_skill_native_combatguard.inc NativeCreatureTail ports).
// Transcribed from the original swkotor.exe (read-only); addresses in comments are original instructions.
// ABI: original __thiscall members are exported as extern "C" __fastcall(self, unused_edx, stack args...):
//   0056B710 CExoArrayList<CSWSStats_SpellLikeAbility>::Allocate(int capacity)   RET 4
// Allocation: live_operator_new / live_free only (game heap).
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
}

extern "C" {
void* __cdecl live_operator_new(unsigned);                                         // 006FA7E6
void __cdecl live_free(void*);                                                     // 006FA390
void* __fastcall recovered_sla_construct_006475c0(void* self, void*) noexcept;     // 006475C0 CSWCStats_SpellLikeAbility ctor (w2 g07)

void __fastcall kotor_fastcall_0056b710(void* list, void*, I capacity);
}

// ===================================================================================== 0056B710
// Elements are 0Ch bytes. The new block is a plain operator new (no array cookie); every slot is constructed,
// the first `size` (+4, re-read) elements are copied dword-wise, the old block is freed when non-null.
// Quirk kept: a failed allocation leaves data = 0 and the copy loop then writes through it.
void __fastcall kotor_fastcall_0056b710(void* list, void*, I capacity) {
    const U self = word(list);
    const U old_data = rd(self);                                                   // 0056B730 / 0056B739
    wr(self + 8, static_cast<U>(capacity));                                        // 0056B73D
    void* const block = live_operator_new(static_cast<U>(capacity) * 12u);         // 0056B732..0056B740
    U data = 0;
    if (block != nullptr) {                                                        // 0056B74E..0056B758
        U cursor = word(block);
        for (I left = capacity; left > 0; --left, cursor += 12)                    // 0056B75A..0056B76D
            (void)recovered_sla_construct_006475c0(ptr(cursor), nullptr);          // 0056B762..0056B764
        data = word(block);
    }
    wr(self, data);                                                                // 0056B786
    for (I i = 0; i < static_cast<I>(rd(self + 4)); ++i) {                         // 0056B777..0056B7B0 (size re-read)
        const U from = old_data + 12u * static_cast<U>(i);
        const U to = rd(self) + 12u * static_cast<U>(i);
        wr(to, rd(from));                                                          // 0056B795..0056B799
        wr(to + 4, rd(from + 4));                                                  // 0056B79B..0056B79E
        wr(to + 8, rd(from + 8));                                                  // 0056B7A1..0056B7A4
    }
    if (old_data != 0) live_free(ptr(old_data));                                   // 0056B7B2..0056B7B7
}

// =====================================================================================================
//   005A5460 CSWSCreatureStats::CalcStatModifier(uchar stat)            RET 4  (stat-11 or stat-10, /2 toward 0)
//   004FFAF0 CSWSCreature::UpdateAttributesOnEffect(CGameEffect* e)     RET 4
// .rdata bound to proven values: [0074670C] = 24h, [00746710] = 25h (ability increase / decrease effect types),
//   [007466C8] = 13h (effect type of the hit-point event effect), [007473B8] = 0 (duration type), [007441C8] = 5
//   (AI event type).
// EH: 004FFAF0 registers an unwind frame that only frees the new CGameEffect when its constructor throws; the
// bound constructor (00503E40) does not throw in normal operation, so that cleanup is not reproduced.
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }    // [007A39FC] CAppManager*
extern "C" {
void* __cdecl live_operator_new(unsigned);                                       // 006FA7E6
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);    // 00503690 CGameEffect::GetInteger (W1 b7)
U __fastcall recovered_creature_stats_str_005a6190(void* stats, void*);          // 005A6190 GetSTRStat (w2_g07)
U __fastcall recovered_creature_stats_dex_005a6550(void* stats, void*);          // 005A6550 GetDEXStat
U __fastcall recovered_creature_stats_con_005a6250(void* stats, void*);          // 005A6250 GetCONStat
U __fastcall recovered_creature_stats_int_005a6310(void* stats, void*);          // 005A6310 GetINTStat
U __fastcall recovered_creature_stats_wis_005a63d0(void* stats, void*);          // 005A63D0 GetWISStat
U __fastcall recovered_creature_stats_cha_005a6490(void* stats, void*);          // 005A6490 GetCHAStat
U __fastcall kotor_m15e_slot010_zero_0063e7f0(void* self, void*);                // 0063E7F0 return_zero (0832)
U __fastcall recovered_creature_incapacitated_004ef890(void* creature, void*);   // 004EF890 GetDeadTemp (0963)
void __fastcall w1x_object_set_hp_004cc280(void* object, void*, U hp);           // 004CC280 SetCurrentHitPoints (0886 unit)
U __fastcall w1x_creature_max_fp_004fd490(void* creature, void*);               // 004FD490 GetMaxForcePoints (AX only, 0886 unit)
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag); // 00503E40 CGameEffect::CGameEffect (0886 unit)
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);    // 00503A00 CGameEffect::SetCreator
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value); // 005036A0 CGameEffect::SetInteger
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);       // 004AED80 GetServerAIMaster
U __fastcall recovered_event_enqueue_004b08d0(void* ai, void*, U day, U ms, U source, U target, U kind, void* data); // 004B08D0
void __fastcall w0886x_stats_adjust_spell_uses_005a6d80(void* stats, void*);     // 005A6D80 AdjustSpellUsesPerDay (0886 unit export)

U __fastcall kotor_fastcall_005a5460(void* stats, void*, U stat);
void __fastcall kotor_fastcall_004ffaf0(void* creature, void*, void* effect);
}

U __fastcall kotor_fastcall_005a5460(void*, void*, U stat) {
    const I value = static_cast<I>(stat & 0xff);                                 // 005A5460..005A5466 movzx
    return static_cast<U>((value < 10 ? value - 11 : value - 10) / 2);           // 005A5469..005A547E (cdq; sub; sar)
}

namespace {
template<class R, class... A> R vcall(void* self, U slot, A... args) {
    using F = R(__thiscall*)(void*, A...);
    return reinterpret_cast<F>(rd(rd(word(self)) + slot))(self, args...);
}
inline void wr8(U a, U v) { *static_cast<unsigned char*>(ptr(a)) = static_cast<unsigned char>(v); }
inline U rd16(U a) { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
inline void wr16(U a, U v) { const std::uint16_t x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
inline I sx16(U v) { return static_cast<I>(static_cast<std::int16_t>(v & 0xffff)); }
}

void __fastcall kotor_fastcall_004ffaf0(void* creature, void*, void* effect) {
    if (effect == nullptr) return;                                               // 004FFB09..004FFB0F
    const U self = word(creature);
    const U type = rd16(word(effect) + 8);                                       // 004FFB15..004FFB1B
    if (type != 0x24 && type != 0x25) return;                                    // 004FFB1F..004FFB32 ([0074670C]/[00746710])
    const U ability = recovered_effect_integer_00503690(effect, nullptr, 0);     // 004FFB38..004FFB3A
    void* const stats = ptr(rd(self + 0xa74));
    switch (ability) {
    case 0:                                                                      // 004FFB43..004FFB68 STR
        wr8(word(stats) + 0xea, kotor_fastcall_005a5460(stats, nullptr, recovered_creature_stats_str_005a6190(stats, nullptr)));
        wr(self + 0x344, 1);
        return;
    case 1:                                                                      // 004FFB72..004FFB97 DEX
        wr8(word(stats) + 0xec, kotor_fastcall_005a5460(stats, nullptr, recovered_creature_stats_dex_005a6550(stats, nullptr)));
        wr(self + 0x344, 1);
        return;
    case 2: {                                                                    // 004FFBA5..004FFCDE CON
        (void)kotor_m15e_slot010_zero_0063e7f0(creature, nullptr);              // 004FFBA5..004FFBA7 (result unused)
        const U old_current = vcall<U>(creature, 0x94);                          // 004FFBAC..004FFBB8 ebx
        const U dead_before = recovered_creature_incapacitated_004ef890(creature, nullptr); // 004FFBB6..004FFBBF
        const U old_max = vcall<U, U>(creature, 0x98, 1u);                       // 004FFBC3..004FFBD7 ebp
        void* const con_stats = ptr(rd(self + 0xa74));                           // 004FFBCF
        wr8(word(con_stats) + 0xee, kotor_fastcall_005a5460(con_stats, nullptr,
            recovered_creature_stats_con_005a6250(con_stats, nullptr)));         // 004FFBD5..004FFBE6
        const I base = sx16(vcall<U, U>(creature, 0x9c, 1u)) - sx16(old_max);   // 004FFBEC..004FFC04
        const I hp = base + sx16(vcall<U, U>(creature, 0x98, 1u));               // 004FFC00..004FFC0F
        w1x_object_set_hp_004cc280(creature, nullptr, static_cast<U>(hp));       // 004FFC11..004FFC14
        bool raise = false;
        if (old_current == 0 && vcall<U>(creature, 0x94) != 0) raise = true;     // 004FFC19..004FFC29
        else if (dead_before == 0 && recovered_creature_incapacitated_004ef890(creature, nullptr) != 0) raise = true; // 004FFC2B..004FFC40
        if (!raise) return;
        void* const memory = live_operator_new(0x8c);                            // 004FFC46..004FFC4B
        const U made = memory != nullptr ? word(recovered_effect_construct_00503e40(memory, nullptr, 1)) : 0; // 004FFC57..004FFC70
        wr16(made + 8, 0x13);                                                    // 004FFC72..004FFC82 ([007466C8])
        wr16(made + 0xa, (rd16(made + 0xa) & 0xfff8) | 0);                       // 004FFC7C..004FFC9B ([007473B8])
        recovered_effect_creator_00503a00(ptr(made), nullptr, rd(word(effect) + 0x18)); // 004FFC9F..004FFCA5
        recovered_effect_integer_005036a0(ptr(made), nullptr, 1, 1);             // 004FFCAA..004FFCB0
        void* const server = *reinterpret_cast<void* const*>(
            static_cast<unsigned char*>(kotor_menu_window_cells::manager_007a39fc) + 8); // 004FFCB5..004FFCBB
        (void)recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server, nullptr), nullptr, 0, 0,
            rd(made + 0x18), rd(self + 4), 5, ptr(made));                        // 004FFCBE..004FFCD9 ([007441C8])
        return;
    }
    case 3:                                                                      // 004FFCE8..004FFD03 INT
        wr8(word(stats) + 0xf0, kotor_fastcall_005a5460(stats, nullptr, recovered_creature_stats_int_005a6310(stats, nullptr)));
        break;
    case 4:                                                                      // 004FFD0A..004FFD2F WIS
    case 5: {                                                                    // 004FFD36..004FFD55 CHA
        const I old_max = sx16(w1x_creature_max_fp_004fd490(creature, nullptr));  // 004FFD0A..004FFD19 / 004FFD36..004FFD45
        void* const fp_stats = ptr(rd(self + 0xa74));
        if (ability == 4)
            wr8(word(fp_stats) + 0xf2, kotor_fastcall_005a5460(fp_stats, nullptr, recovered_creature_stats_wis_005a63d0(fp_stats, nullptr)));
        else
            wr8(word(fp_stats) + 0xf4, kotor_fastcall_005a5460(fp_stats, nullptr, recovered_creature_stats_cha_005a6490(fp_stats, nullptr)));
        const U holder = rd(self + 0xa74);                                       // 004FFD5B
        const I current = sx16(rd16(holder + 0x124));                            // 004FFD61
        const U new_max = w1x_creature_max_fp_004fd490(creature, nullptr);       // 004FFD68..004FFD6A
        wr16(holder + 0x124, (current - old_max) + static_cast<I>(new_max));     // 004FFD6F..004FFD73 (word store)
        break;
    }
    default:
        return;                                                                  // 004FFD31..004FFD34
    }
    w0886x_stats_adjust_spell_uses_005a6d80(ptr(rd(self + 0xa74)), nullptr);     // 004FFD7A..004FFD80
}
