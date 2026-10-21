// 0963-gates (Claude): CSWSObject damage-mitigation virtuals that the server object vtables of unit 0963
// (object/creature/door/placeable/trigger/waypoint/sound/item tables, slots +0xA4/+0xA8/+0xAC) bind to,
// Their only callee without a c10 provider, CSWCCMessageData::CopyTo 004D6EE0, is imported from the existing
// faithful body kotor_fastcall_004d6ee0 in claude_combat_attack_round.cpp (link that object too).
// Transcribed from the original swkotor.exe (read-only); addresses in comments are original instructions.
// ABI: original __thiscall members are exported as extern "C" __fastcall(self, unused_edx, stack args...):
//   004CF160 DoDamageImmunity  (CSWSCreature* attacker, int damage, ushort flags, int simulate, int feedback) RET 0x14
//   004D09E0 DoDamageReduction (CSWSCreature* attacker, int damage, uchar power, int simulate, int feedback)  RET 0x14
//   004D0E40 DoDamageResistance(CSWSCreature* attacker, int damage, ushort flags, int simulate, int feedback,
//                               int record_type) RET 0x18
// .rdata constants bound to their proven values: [007466AC] = 12 (EFFECT damage reduction),
// [00746684] = 2 (EFFECT damage resistance).  Allocation: live_operator_new / live_free only.
// EH: the originals register an unwind frame that only frees a CSWCCMessageData whose constructor throws;
// the bound constructor (004D69F0) only zero-fills and cannot throw, so that cleanup is not reproduced.
// Quirks kept: null allocation results are passed on unchecked; Reduction/Resistance overwrite their best
// amount with the effect's remaining pool when it is exhausted; the "else" feedback branch of
// Reduction/Resistance never sets message integer 0 (the id only goes to SendFeedbackMessage).
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
inline U rd16(U a) { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
inline U rd8(U a) { return *static_cast<const unsigned char*>(ptr(a)); }
inline void wr8(U a, U v) { *static_cast<unsigned char*>(ptr(a)) = static_cast<unsigned char>(v); }
}

extern "C" {
void* __cdecl live_operator_new(unsigned);                                                       // 006FA7E6
void __cdecl live_free(void*);                                                                   // 006FA390
void* __fastcall recovered_message_data_construct_004d69f0(void*, void*);                        // 004D69F0 CSWCCMessageData ctor
void __fastcall recovered_message_data_destroy_004d6a40(void*, void*);                          // 004D6A40 CSWCCMessageData dtor
void __fastcall recovered_event_integer_004d76b0(void*, void*, U, U);                            // 004D76B0 CSWCCMessageData::SetInteger
void __fastcall recovered_event_object_004d7780(void*, void*, U, U);                             // 004D7780 CSWCCMessageData::SetObjectID
void __fastcall recovered_creature_feedback_004ede10(void*, void*, U, void*);                    // 004EDE10 CSWSCreature::SendFeedbackMessage
void __fastcall aurora_list_add16_00671c00(void*, void*, void*);                                 // 00671C00 CExoArrayList<T*>::Add
void* __fastcall recovered_combat_round_attack_004d2b50(void*, void*, U);                        // 004D2B50 CSWSCombatRound::GetAttack
U __fastcall recovered_effect_integer_00503690(void*, void*, U);                                 // 00503690 CGameEffect::GetInteger
void __fastcall recovered_effect_integer_005036a0(void*, void*, U, U);                           // 005036A0 CGameEffect::SetInteger
U __fastcall recovered_object_remove_effect_004d06c0(void*, void*, U, U);                        // 004D06C0 CSWSObject::RemoveEffectById
U __fastcall recovered_stats_has_feat_005a6630(void*, void*, U);                                 // 005A6630 CSWSCreatureStats::HasFeat

void __fastcall kotor_fastcall_004d6ee0(void* self, void*, void* out);                            // 004D6EE0 CopyTo (claude_combat_attack_round.cpp)
int __fastcall kotor_fastcall_004cf160(void* self, void*, void* attacker, int damage, U flags, U simulate, U feedback);
int __fastcall kotor_fastcall_004d09e0(void* self, void*, void* attacker, int damage, U power, U simulate, U feedback);
int __fastcall kotor_fastcall_004d0e40(void* self, void*, void* attacker, int damage, U flags, U simulate, U feedback, U record);
}

namespace {
// Virtual slot +0x30 (AsSWSCreature) and +0xB4 (GetDamageImmunityByFlags) of a CSWSObject.
U as_creature(U object) { using F = void*(__thiscall*)(void*); return word(reinterpret_cast<F>(rd(rd(object) + 0x30))(ptr(object))); }
U immunity_by_flags(U object, U flags) { using F = U(__thiscall*)(void*, U); return reinterpret_cast<F>(rd(rd(object) + 0xb4))(ptr(object), flags); }
U new_message() {                                                                 // push 34; call 006FA7E6; test; ctor
    void* const memory = live_operator_new(0x34);
    return memory ? word(recovered_message_data_construct_004d69f0(memory, nullptr)) : 0;
}
void delete_message(U message) {                                                  // ~CSWCCMessageData; free
    recovered_message_data_destroy_004d6a40(ptr(message), nullptr);
    live_free(ptr(message));
}
void set_int(U message, U index, U value) { recovered_event_integer_004d76b0(ptr(message), nullptr, index, value); }
void attach_to_attack(U attacker, U message, U index) {                          // GetAttack(index)->messages.Add(msg)
    void* const attack = recovered_combat_round_attack_004d2b50(ptr(rd(attacker + 0x9c8)), nullptr, index);
    aurora_list_add16_00671c00(ptr(word(attack) + 0xa0), nullptr, ptr(message));
}
U current_attack_index(U attacker) { return rd8(rd(attacker + 0x9c8) + 0x96c); } // movzx byte [round+0x96C]
// Feedback tail shared by every "else" branch: own copy to self (if a creature), copy to the attacker.
void send_pair(U self, U attacker, U id, U mine, U theirs) {
    if (as_creature(self)) recovered_creature_feedback_004ede10(ptr(as_creature(self)), nullptr, id, ptr(mine));
    else if (mine) delete_message(mine);
    if (attacker) recovered_creature_feedback_004ede10(ptr(attacker), nullptr, id, ptr(theirs));
    else if (theirs) delete_message(theirs);
}
int min_int(int a, int b) { return a < b ? a : b; }
}

// 004CF160 CSWSObject::DoDamageImmunity: returns damage minus the immune share (>=1 when immunity > 0).
int __fastcall kotor_fastcall_004cf160(void* self, void*, void* attacker_ptr, int damage, U flags, U simulate, U feedback) {
    const U s = word(self), attacker = word(attacker_ptr);
    const int immunity = static_cast<signed char>(immunity_by_flags(s, flags));  // 004CF17A..CF18F (slot +0xB4)
    int share = static_cast<int>(static_cast<U>(immunity) * static_cast<U>(damage)) / 100; // 004CF192..CF1BD
    if (immunity > 0 && share < 1) share = 1;                                     // 004CF1A1..CF1B6
    const int result = damage - share;                                            // 004CF1C3..CF1C5
    if (simulate || immunity <= 0) return result;                                 // 004CF1C7..CF1D1
    const U type = flags & 0xffff;
    if (feedback == 1 && attacker) {                                              // 004CF1D7..CF1E8
        const U msg = new_message();                                              // 004CF1EE..CF20D
        set_int(msg, 0, 0x3e);                                                    // 004CF20F..CF21D
        set_int(msg, 1, static_cast<U>(damage - result));                         // 004CF222..CF229
        set_int(msg, 2, type);                                                    // 004CF22E..CF238
        recovered_event_object_004d7780(ptr(msg), nullptr, 0, rd(s + 4));         // 004CF23D..CF245
        attach_to_attack(attacker, msg, current_attack_index(attacker));          // 004CF24A..CF268
        return result;
    }
    const U mine = new_message();                                                 // 004CF272..CF295
    const U theirs = new_message();                                               // 004CF299..CF2CE
    set_int(mine, 0, 0x3e);                                                       // 004CF2D0..CF2DE
    set_int(mine, 1, static_cast<U>(damage - result));                            // 004CF2E3..CF2EA
    set_int(mine, 2, type);                                                       // 004CF2EF..CF2F9
    recovered_event_object_004d7780(ptr(mine), nullptr, 0, rd(s + 4));            // 004CF2FE..CF306
    kotor_fastcall_004d6ee0(ptr(mine), nullptr, ptr(theirs));                     // 004CF30B..CF312
    send_pair(s, attacker, 0x3e, mine, theirs);                                   // 004CF317..CF36E
    return result;                                                                // 004CF378
}

// 004D09E0 CSWSObject::DoDamageReduction: strongest type-12 effect whose power exceeds the attack power.
int __fastcall kotor_fastcall_004d09e0(void* self, void*, void* attacker_ptr, int damage, U power, U simulate, U feedback) {
    const U s = word(self), attacker = word(attacker_ptr);
    int best_power = 0, best = 0; U best_index = 0, found = 0;                   // 004D0A00..0A0A, 0A35
    U j = as_creature(s) ? rd16(rd(as_creature(s) + 0xa74) + 0x132) : 0;        // 004D0A0E..0A2B
    if (static_cast<int>(rd(s + 0x128)) > 0 && static_cast<int>(j) < static_cast<int>(rd(s + 0x128))) { // 004D0A2D..0A3D
        do {
            const U effect = rd(rd(s + 0x124) + 4 * j);                           // 004D0A40..0A46
            const U type = rd16(effect + 8);                                      // 004D0A49
            if (static_cast<int>(type) > 12) break;                               // 004D0A4D..0A54 [007466AC]=12
            if (type == 12) {                                                     // 004D0A56..0A5C
                const int amount = static_cast<int>(recovered_effect_integer_00503690(ptr(effect), nullptr, 0)); // 004D0A5E..0A62
                const int level = static_cast<int>(recovered_effect_integer_00503690(ptr(effect), nullptr, 1));  // 004D0A67..0A6D
                if (amount > best) { best_index = j; best = amount; best_power = level; found = 1; } // 004D0A72..0A84
            }
            ++j;                                                                  // 004D0A92
        } while (static_cast<int>(j) < static_cast<int>(rd(s + 0x128)));         // 004D0A8C..0A95
    }
    if (!found) {                                                                 // 004D0A97..0A9F
        if (!as_creature(s)) return damage;                                       // 004D0AA1..0AAE
        return damage >= 0 ? damage : 0;                                          // 004D0AB4..0ABC
    }
    const U effect = rd(rd(s + 0x124) + 4 * best_index);                          // 004D0AC3..0ACD
    const int remaining = static_cast<int>(recovered_effect_integer_00503690(ptr(effect), nullptr, 2)); // 004D0AD0..0AD2
    if (remaining) {                                                              // 004D0ADD
        const int left = remaining - damage;                                      // 004D0AE1
        if (left > 0) {
            if (simulate) goto finish;                                            // 004D0AE7..0AED
            recovered_effect_integer_005036a0(ptr(effect), nullptr, 2, static_cast<U>(left)); // 004D0AF3..0B03
        } else {
            best = remaining;                                                     // 004D0B10 (quirk: overwrites best)
            if (simulate) goto finish;                                            // 004D0B0A..0B14
            (void)recovered_object_remove_effect_004d06c0(ptr(s), nullptr, rd(effect), rd(effect + 4)); // 004D0B1A..0B30
        }
    } else if (simulate) goto finish;                                             // 004D0B37..0B3D
    if (static_cast<int>(power & 255) >= best_power) goto finish;                 // 004D0B43..0B4C
    if (feedback == 1 && attacker) {                                              // 004D0B52..0B63
        const U msg = new_message();                                              // 004D0B69..0B8E
        recovered_event_object_004d7780(ptr(msg), nullptr, 0, rd(s + 4));         // 004D0B90..0BA0
        const U index = current_attack_index(attacker);                           // 004D0BA7..0BB8
        if (remaining) {                                                          // 004D0BA5, 0BBE
            set_int(msg, 0, 0x43);                                                // 004D0BC0..0BC4
            if (remaining - damage > 0) {                                         // 004D0BC9..0BCF
                set_int(msg, 1, static_cast<U>(min_int(damage, best)));           // 004D0BD1..0BE2
                set_int(msg, 2, static_cast<U>(remaining - min_int(damage, best))); // 004D0BE7..0BF6
            } else {
                set_int(msg, 1, static_cast<U>(remaining));                       // 004D0C01..0C06
                set_int(msg, 2, 0);                                               // 004D0C0B..0C11
            }
        } else {
            set_int(msg, 0, 0x40);                                                // 004D0C39..0C3D
            set_int(msg, 1, static_cast<U>(min_int(damage, best)));               // 004D0C42..0C51
        }
        attach_to_attack(attacker, msg, index);                                   // 004D0C16..0C2F / 0C56..0C6F
        goto finish;
    }
    {
        const U mine = new_message();                                             // 004D0C79..0C9E
        const U theirs = new_message();                                           // 004D0CA0..0CCE
        recovered_event_object_004d7780(ptr(mine), nullptr, 0, rd(s + 4));        // 004D0CD6..0CE2
        if (remaining) {                                                          // 004D0CE7..0CE9
            if (remaining - damage > 0) {                                         // 004D0CEF..0CF9
                set_int(mine, 1, static_cast<U>(min_int(damage, best)));          // 004D0CFB..0D0A
                set_int(mine, 2, static_cast<U>(remaining - min_int(damage, best))); // 004D0D0F..0D2E
            } else {
                set_int(mine, 1, static_cast<U>(remaining));                      // 004D0D1E..0D23
                set_int(mine, 2, 0);                                              // 004D0D28..0D2E
            }
            kotor_fastcall_004d6ee0(ptr(mine), nullptr, ptr(theirs));             // 004D0D33..0D3A
            send_pair(s, attacker, 0x43, mine, theirs);                           // 004D0D3F..0DFC
        } else {
            set_int(mine, 1, static_cast<U>(min_int(damage, best)));              // 004D0D83..0D96
            kotor_fastcall_004d6ee0(ptr(mine), nullptr, ptr(theirs));             // 004D0D9B..0DA2
            send_pair(s, attacker, 0x40, mine, theirs);                           // 004D0DA7..0DFC
        }
    }
finish:                                                                           // 004D0E03
    if (static_cast<int>(power & 255) >= best_power) return damage;               // 004D0E03..0E0E
    const int reduced = damage - best;                                            // 004D0E10
    return reduced > 0 ? reduced : 0;                                             // 004D0E14..0E1C
}

// 004D0E40 CSWSObject::DoDamageResistance: per flagged damage bit, the strongest matching type-2 effect;
// the overall strongest one absorbs (vs_flag+1)*damage.  record_type != 0 records the last flagged bits.
int __fastcall kotor_fastcall_004d0e40(void* self, void*, void* attacker_ptr, int damage, U flags, U simulate, U feedback, U record) {
    const U s = word(self), attacker = word(attacker_ptr);
    int best = 0; U best_index = 0; U doubled = 0; U last_bits = 0;              // [esp+0x10], [+0x28] (unset), [+0x24], [+0x1c]
    const U mask = flags & 0xffff;
    if (static_cast<int>(rd(s + 0x128)) > 0) {                                    // 004D0E5C..0E77
        for (U bit = 0; bit < 15; ++bit) {                                        // 004D0E7D..0E80, 0F70..0F7C
            if (!((mask >> bit) & 1)) continue;                                   // 004D0E84..0E8D
            int current = 0; U current_index = 0;                                 // 004D0E97..0E9B
            U j = as_creature(s) ? rd16(rd(as_creature(s) + 0xa74) + 0x134) : 0; // 004D0E9F..0EBC
            if (static_cast<int>(j) < static_cast<int>(rd(s + 0x128))) {          // 004D0EBE..0EC4
                do {
                    const U effect = rd(rd(s + 0x124) + 4 * j);                   // 004D0ECC..0ED6
                    const U type = rd16(effect + 8);                              // 004D0ED9
                    if (static_cast<int>(type) > 2) break;                        // 004D0EDD..0EE4 [00746684]=2
                    if (type == 2) {                                              // 004D0EE6..0EEC
                        const U kinds = recovered_effect_integer_00503690(ptr(effect), nullptr, 0); // 004D0EEE..0EF2
                        const int amount = static_cast<int>(recovered_effect_integer_00503690(ptr(effect), nullptr, 1)); // 004D0EF7..0EFD
                        if ((kinds & mask) && amount > current) { current = amount; current_index = j; } // 004D0F02..0F15
                        if (recovered_effect_integer_00503690(ptr(effect), nullptr, 3) & mask) doubled = 1; // 004D0F19..0F26
                    }
                    ++j;                                                          // 004D0F38
                } while (static_cast<int>(j) < static_cast<int>(rd(s + 0x128)));  // 004D0F2E..0F3B
            }
            if (current > best) { best = current; best_index = current_index; }   // 004D0F45..0F53
            if (record) last_bits = mask >> bit;                                  // 004D0F57..0F6C
        }
        if (best) {                                                               // 004D0F82..0F86
            const U effect = rd(rd(s + 0x124) + 4 * best_index);                  // 004D0F8C..0F96
            const int remaining = static_cast<int>(recovered_effect_integer_00503690(ptr(effect), nullptr, 2)); // 004D0F99..0F9B
            int absorb = static_cast<int>((doubled + 1) * static_cast<U>(damage)); // 004D0FA0..0FA5
            if (remaining) {                                                      // 004D0FAC..0FAE
                const int left = remaining - absorb;                              // 004D0FB0
                if (left > 0) {
                    if (simulate) goto tail;                                      // 004D0FB6..0FBC
                    recovered_effect_integer_005036a0(ptr(effect), nullptr, 2, static_cast<U>(left)); // 004D0FC2..0FCE
                } else {
                    best = remaining;                                             // 004D0FDB (quirk: overwrites best)
                    if (simulate) goto tail;                                      // 004D0FD5..0FDF
                    (void)recovered_object_remove_effect_004d06c0(ptr(s), nullptr, rd(effect), rd(effect + 4)); // 004D0FE5..0FF7
                }
            } else if (simulate) goto tail;                                       // 004D0FFE..1004
            if (feedback == 1 && attacker) {                                      // 004D100A..1021
                const U msg = new_message();                                      // 004D1027..1046
                recovered_event_object_004d7780(ptr(msg), nullptr, 0, rd(s + 4)); // 004D1048..105C
                const U index = current_attack_index(attacker);                   // 004D1063..1070
                if (remaining) {                                                  // 004D1061, 1076
                    set_int(msg, 0, 0x42);                                        // 004D107C..1080
                    if (remaining - absorb > 0) {                                 // 004D1085..108B
                        set_int(msg, 1, static_cast<U>(min_int(absorb, best)));   // 004D108D..109E
                        absorb = min_int(absorb, best);                           // 004D10A3..10A7
                        set_int(msg, 2, static_cast<U>(remaining - absorb));      // 004D10A9..10B0
                    } else {
                        set_int(msg, 1, static_cast<U>(remaining));               // 004D10BB..10C0
                        set_int(msg, 2, 0);                                       // 004D10C5..10CB
                    }
                } else {
                    set_int(msg, 0, 0x3f);                                        // 004D10F3..10F7
                    absorb = min_int(absorb, best);                               // 004D10FC..1104
                    set_int(msg, 1, static_cast<U>(absorb));                      // 004D1106..110B
                }
                attach_to_attack(attacker, msg, index);                           // 004D10D0..10E9 / 1110..1129
            } else {
                const U mine = new_message();                                     // 004D1133..1154
                const U theirs = new_message();                                   // 004D1156..1184
                recovered_event_object_004d7780(ptr(mine), nullptr, 0, rd(s + 4)); // 004D1190..119C
                if (remaining) {                                                  // 004D11A1..11A3
                    if (remaining - absorb > 0) {                                 // 004D11A9..11AF
                        set_int(mine, 1, static_cast<U>(min_int(absorb, best)));  // 004D11B1..11C0
                        absorb = min_int(absorb, best);                           // 004D11C5..11CD
                        set_int(mine, 2, static_cast<U>(remaining - absorb));     // 004D11CF..11E4
                    } else {
                        set_int(mine, 1, static_cast<U>(remaining));              // 004D11D4..11D9
                        set_int(mine, 2, 0);                                      // 004D11DE..11E4
                    }
                    kotor_fastcall_004d6ee0(ptr(mine), nullptr, ptr(theirs));     // 004D11E9..11F0
                    send_pair(s, attacker, 0x42, mine, theirs);                   // 004D11F5..12B1
                } else {
                    absorb = min_int(absorb, best);                               // 004D123C..1244
                    set_int(mine, 1, static_cast<U>(absorb));                     // 004D1246..124B
                    kotor_fastcall_004d6ee0(ptr(mine), nullptr, ptr(theirs));     // 004D1250..1257
                    send_pair(s, attacker, 0x3f, mine, theirs);                   // 004D125C..12B1
                }
            }
        }
    }
tail:                                                                             // 004D12B9
    int result = damage;                                                          // 004D12E0 / 136B ebx
    if (as_creature(s)) {                                                         // 004D12B9..12C2
        const U creature = as_creature(s);                                        // 004D12C8..12CF
        if (recovered_stats_has_feat_005a6630(ptr(rd(creature + 0xa74)), nullptr, 0x7b)) { // 004D12D1..12E4
            result -= 2;                                                          // 004D12EA
            if (record) {                                                         // 004D12E6..12EF
                const U attack = word(recovered_combat_round_attack_004d2b50(ptr(rd(attacker + 0x9c8)), nullptr, current_attack_index(attacker))); // 004D12F1..1303
                wr8(attack + 0x107, static_cast<U>(result));                      // 004D130C
                wr8(attack + 0x104, 2);                                           // 004D1312
                wr8(attack + 0x106, last_bits);                                   // 004D1308, 1319
            }
        }
        if (recovered_stats_has_feat_005a6630(ptr(rd(creature + 0xa74)), nullptr, 0x5f)) { // 004D131F..132E
            result -= 2;                                                          // 004D1334
            if (record) {                                                         // 004D1330..1339
                const U attack = word(recovered_combat_round_attack_004d2b50(ptr(rd(attacker + 0x9c8)), nullptr, current_attack_index(attacker))); // 004D133B..134D
                wr8(attack + 0x108, static_cast<U>(result));                      // 004D1356
                wr8(attack + 0x105, 2);                                           // 004D135C
                wr8(attack + 0x106, last_bits);                                   // 004D1352, 1363
            }
        }
    }
    const int reduced = result - best;                                            // 004D136F..1375
    return reduced > 0 ? reduced : 0;                                             // 004D1379..138B
}
