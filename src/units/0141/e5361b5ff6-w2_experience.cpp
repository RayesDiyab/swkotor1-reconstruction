#include "e657249d9e-w2_experience.hpp"
#include "28478d5d50-w2_ftol.hpp"
#include <cmath>
#include <cstdint>
#include <cstring>
namespace w2::experience {
namespace {
using I = std::int32_t;
// (kInvalid removed in t6b: only GetNPCID used it, which is now W1s)
constexpr float kPercent = 0.0099999998f;      // [0073DFEC]
Word server(Memory& m, Ports& p) { return m.read(p.app_007a39fc() + 8); }
float f32(Word b) { float v; std::memcpy(&v, &b, 4); return v; }
Word bits(float v) { Word b; std::memcpy(&b, &v, 4); return b; }
// inlined CSWSCreatureStats::GetClassLevel / GetClass: class entries 28h apart, id at +A7h, level at +A8h
Word level(Memory& m, Word s, Word i, Word count) { return i < count ? m.read8(s + 0xa8 + 0x28 * i) : 0u; }
Word class_id(Memory& m, Word s, Word i, Word count) { return i < count ? m.read8(s + 0xa7 + 0x28 * i) : 0xffu; }
}

// 005638A0 CSWPartyTable::GetNPCID is W1's (t6b ruling): Ports::party_npc_id_005638a0.

// 005AF6A0: single class -> plain add. Multiclass: favored class = race +1Fh, or (FFh) the first highest-level class;
// each non-favored class 2+ levels below the highest non-favored level costs 20 %. xp is read as UNSIGNED when
// negative (FILD + 2^32). factor and product in x87 double; __ftol2 low dword.
void add_experience_005af6a0(Memory& m, Ports& p, Word s, Word xp) {
    const Word count = m.read8(s + 0x89);
    if (count <= 1) { p.stats_set_experience_005af480(s, m.read(s + 0x68) + xp, 1); return; }   // 005AF85C
    const Word race = m.read16(s + 0xdc);
    Word favored = m.read8(m.read(p.rules_007a3a28() + 0xb0) + race * 0x34 + 0x1f);
    if (favored == 0xffu) {                                                   // 005AF6EF
        Word best = 0;
        Word lv = level(m, s, 0, count);
        if (count > 1 && lv < m.read8(s + 0xd0)) best = 1;
        lv = level(m, s, best, count);
        if (count > 2 && lv < m.read8(s + 0xf8)) best = 2;
        favored = best < count ? class_id(m, s, best, count) : 0xffu;
    }
    Word highest = 0;                                                         // [esp+0F]
    for (Word i = 0; i < count; ++i) {
        if (favored == class_id(m, s, i, count)) continue;
        const Word lv = level(m, s, i, count);
        if (!(highest > lv)) highest = lv;
    }
    double penalties = 0.0;                                                   // [0073EDF8]
    for (Word i = 0; i < count; ++i) {
        if (favored == class_id(m, s, i, count)) continue;
        if (static_cast<I>(highest) - static_cast<I>(level(m, s, i, count)) >= 2) penalties += 1.0;
    }
    const double factor = 1.0 - penalties * 0.2;                              // fmul [0074AD98]; fsubr [0073EDE8]
    double amount = static_cast<double>(static_cast<I>(xp));
    if (static_cast<I>(xp) < 0) amount += 4294967296.0;                       // 005AF84B
    const Word gained = w2::ftol2_006fae8c(amount * factor);
    p.stats_set_experience_005af480(s, m.read(s + 0x68) + gained, 1);
}

// 004EF930. Locals: multiplier float (orig [esp+08h], 1.0f); 2DA outs start at 100 (orig +10h / +14h).
void receive_experience_004ef930(Memory& m, Ports& p, Word c, Word xp, Word s) {
    const Word str1 = s, out1 = s + 8, out2 = s + 0x0c, str2 = s + 0x10;
    m.write(out1, 100); m.write(out2, 100);
    Word mult = bits(1.0f);
    const Word pt = p.party_table_004aee70(server(m, p));
    const Word table = m.read(m.read(p.rules_007a3a28() + 0xb8) + 0xf0);
    if (pt != 0) {
        const Word npc = p.party_npc_id_005638a0(pt, m.read(c + 4));   // W1 @recovered_party_table_npc_id_005638a0@12
        if (npc != 0xffffffffu && table != 0) {                                // 004EF996 / 004EF99D
            p.str_literal_005e5a90(str1, "PercentXP");
            (void)p.c2da_int_00413660(table, npc, str1, out1);
            p.str_destroy_005e5c20(str1);
            (void)p.str_cstr_005e5670(c + 0x18);
            mult = bits(static_cast<float>(static_cast<double>(static_cast<I>(m.read(out1))) * static_cast<double>(kPercent)));
        }
        if (table == 0) p.trap(0x004efa12, "W2: ReceiveExperience with no PartyTable 2DA (original calls GetINTEntry on null)");
        p.str_literal_005e5a90(str2, "PercentXP");
        (void)p.c2da_int_00413660(table, 9, str2, out2);                         // row 9: the player character
        p.str_destroy_005e5c20(str2);
        mult = bits(static_cast<float>(static_cast<double>(static_cast<I>(m.read(out2))) * static_cast<double>(kPercent)
                                       * static_cast<double>(f32(mult))));
    }
    (void)p.str_cstr_005e5670(c + 0x18);                                       // 004EFA3E (no effect)
    const double total = std::ceil(static_cast<double>(static_cast<I>(xp)) * static_cast<double>(f32(mult)));
    add_experience_005af6a0(m, p, m.read(c + 0xa74), w2::ftol2_006fae8c(total));
}

// 005653A0
void distribute_experience_005653a0(Memory& m, Ports& p, Word party, Word xp, Word feedback, Word s) {
    if (static_cast<I>(xp) <= 0) return;                                     // 005653C1 JLE
    m.write(party + 0xf8, m.read(party + 0xf8) + xp);
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(party)); ++i) {
        const Word slot = m.read(party + 4 + 4 * i);
        const Word c = p.creature_by_id_004ae770(server(m, p), m.read(party + 0x0c + 4 * slot));
        if (c == 0) continue;
        (void)p.str_cstr_005e5670(c + 0x18);                                   // 005653FE (no effect)
        receive_experience_004ef930(m, p, c, xp, s);
        if ((m.read8(p.client_options_005ed700(m.read(p.app_007a39fc() + 4)) + 8) & 1u) == 0) continue;
        if (p.can_level_up_005a6810(m.read(c + 0xa74)) == 0) continue;
        p.auto_levelup_005b27e0(m.read(c + 0xa74), 1);
    }
    const Word head = m.read(m.read(p.player_list_004aed90(server(m, p))));
    if (head == 0) p.trap(0x00565467, "W2: DistributeExperience with an empty player list (original calls through a null player)");
    const Word player = m.read(head + 8);
    if (player == 0) p.trap(0x00565467, "W2: DistributeExperience first player node holds null (original calls through it)");
    const Word c = p.creature_by_id_004ae770(server(m, p), m.read(p.vcall0(player, 4) + 0x38));
    if (c == 0) return;
    receive_experience_004ef930(m, p, c, xp, s);
    if (feedback == 0) return;
    const Word mem = p.allocate_006fa7e6(0x34);
    const Word data = mem != 0 ? p.message_data_construct_004d69f0(mem) : 0;
    if (data == 0) p.trap(0x005654ca, "W2: DistributeExperience feedback allocation failed (original calls SetInteger on null)");
    p.message_data_integer_004d76b0(data, 0, xp);
    p.send_feedback_004ede10(c, 0x8f, data);
}
}
