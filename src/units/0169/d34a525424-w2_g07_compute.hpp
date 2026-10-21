#pragma once
// W2 tier 4 (G07 object updates): CSWSMessage::ComputeUpdateRequired 0056B950 and
// CSWSMessage::ComputeAppearanceUpdateRequired 00567100, plus the 48-byte unowned callee
// CSWSFaction::GetCreatureInParty 005BE800, CompareVisualEffectLists 005672F0,
// CompareCreatureLists 0056B830 and CSWSCreature::UpdateAppearanceForEquippedItems 004ED620. Logic over original 32-bit address words; float
// values are IEEE-754 single bit patterns; x87 arithmetic is modelled with IEEE double
// (precision control 53 bits, the CRT default). Every last-update-object field read is listed
// in the .cpp with the reading instruction.
#include "1e16cfcf3d-server_mainloop.hpp"
#include <cstdint>
namespace w2::g07 {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;

struct ComputePorts {
    virtual ~ComputePorts() = default;
    virtual Word app_007a39fc() = 0;
    virtual Word vcall0(Word object, Word slot) = 0;                    // thiscall [[o]+slot](o), EAX
    virtual Word vcall1(Word object, Word slot, Word a) = 0;            // thiscall [[o]+slot](o, a), EAX
    virtual void compute_ai_state_004f1460(Word creature) = 0;
    virtual Word game_object_00561080(Word player) = 0;
    virtual Word ai_state_reputation_0057cd90(Word creature, Word id) = 0;      // AL
    virtual Word is_beam_004ca9d0(Word effect) = 0;                             // CLoopingVisualEffect::GetIsBeam (w2_g07_object)
    virtual Word item_in_slot_005a4c20(Word inventory, Word slot) = 0;          // @recovered_equipment_item_005a4c20@12
    virtual Word get_class_005a4e90(Word stats, Word index) = 0;                 // AL
    virtual Word get_class_level_005a5090(Word stats, Word index, Word flag) = 0; // AL
    virtual Word string_ne_005e5310(Word a, Word b) = 0;                         // CExoString::operator!=(this=a, b)
    virtual Word animation_stationary_004cae60(Word object, Word animation) = 0;
    virtual Word object_area_004cb120(Word object) = 0;
    virtual Word walk_rate_bits_004f1b20(Word creature) = 0;                     // ST0 (float) as bits
    virtual Word run_rate_bits_004f1be0(Word creature) = 0;                      // ST0 (float) as bits
    virtual Word faction_00513fc0(Word creature) = 0;
    virtual Word max_force_points_004fd490(Word creature) = 0;                   // AX
    virtual Word client_object_004ae910(Word server, Word id) = 0;
    virtual Word singleton_party_005bf690(Word faction) = 0;
    virtual Word leader_005bf5c0(Word faction) = 0;
    virtual Word module_004ae6b0(Word server) = 0;
    virtual Word array_count_004af240(Word list, Word value) = 0;
    virtual Word visible_element_00517a20(Word creature, Word id) = 0;
    virtual Word damage_level_004cb020(Word object) = 0;                         // AL
    virtual Word resref_ne_00406dc0(Word a, Word b) = 0;                         // CResRef::operator!=(this=a, b)
    virtual Word creature_by_id_004ae770(Word server, Word id) = 0;
};

// 0056B950 ComputeUpdateRequired(player, object, last, flag) RET 10h. scratch >= 0x10 (CResRef out of vtbl+C8h).
Word compute_update_required_0056b950(Memory&, ComputePorts&, Word self, Word player, Word object, Word last, Word flag, Word scratch);
// 00567100 ComputeAppearanceUpdateRequired(object, last) RET 8.
Word compute_appearance_update_required_00567100(Memory&, ComputePorts&, Word self, Word object, Word last);
// 005672F0 CompareVisualEffectLists(a, b) RET 8: 1 when every effect of each list has a match in the other.
Word compare_vfx_lists_005672f0(Memory&, ComputePorts&, Word self, Word a, Word b);
// 0056B830 CompareCreatureLists(a, b) RET 8: equal counts and element-wise equal ids.
Word compare_creature_lists_0056b830(Memory&, Word self, Word a, Word b);
// 004ED620 CSWSCreature::UpdateAppearanceForEquippedItems() RET 0: refresh creature+A50..A5C from slots 20h/10h/2/1.
void update_appearance_equipped_004ed620(Memory&, ComputePorts&, Word creature);
// 005BE800 CSWSFaction::GetCreatureInParty(id) RET 4: scan [faction][0..[faction+4]).
Word creature_in_party_005be800(Memory&, Word faction, Word id);
}
