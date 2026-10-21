#pragma once
// W2 tier 4: CSWSCreature::GetTotalEffectBonus 004F3FE0, saving-throw specialization (case a1 = 3, null target).
// The ability (case 4) and skill (case 5) specializations are existing 1122 owners
// (kotor::server_mainloop::creature_effect_ability_004f3fe0 / creature_effect_skill_004f3fe0); this file adds only
// case 3. Original: thiscall, 10 stack arguments, RET 28h:
//   a1 BYTE kind, a2 target object*, a3 int* out, a4, a5, a6 BYTE save type, a7 BYTE save vs type, a8, a9, a10.
// Domain: a1 == 3 and a2 == 0. Jump table 004F5F54 (004F4106): 1 -> 004F410D, 2 -> 004F4491, 3 -> 004F5124,
// 4 -> 004F5639, 5 -> 004F5A78; kinds 0 and > 5 return 0 (004F4100 ja 004F5F38). Kinds 1/2/4/5 trap here at their
// case address (4 and 5 are routed elsewhere by the single native entry, see w2_g07_native.cpp); a non-null target traps
// at 004F405E (target race / alignment lookups through vtbl 30h are not reconstructed). a4, a5, a8, a9, a10 only
// feed the attack-type value at [esp+58h], which case 3 never reads.
#include "1e16cfcf3d-server_mainloop.hpp"
namespace w2::g07 {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;

struct EffectSavePorts {
    virtual ~EffectSavePorts() = default;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    virtual Word app_007a39fc() = 0;
    virtual Word rules_007a3a28() = 0;                                   // [007A3A28]
    virtual Word combat_attack_004d2b50(Word round, Word index) = 0;     // CSWSCombatRound::GetAttack (W1 tier4)
    virtual Word effect_integer_00503690(Word effect, Word index) = 0;   // CGameEffect::GetInteger
    virtual Word item_by_id_004ae760(Word server, Word id) = 0;          // CServerExoApp::GetItemByGameObjectID
};

// 004F3FE0 case 3 (saving-throw effects). Returns min(positive,20) - min(negative,20) as a signed dword.
Word effect_save_004f3fe0(Memory&, EffectSavePorts&, Word self, Word a1, Word target, Word out, Word a4, Word a5,
                          Word a6, Word a7, Word a8, Word a9, Word a10);
}
