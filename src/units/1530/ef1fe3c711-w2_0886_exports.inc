// Lane W2 tier 2 integration request for the 0886 owner (server_mainloop_native_vm_canonical_cell.cpp).
// #include this file at the END of that TU (after the W1 w1_0886_exports.inc). It adds extern "C"
// fastcall-with-dummy-EDX wrappers over existing portable providers whose native glue is TU-local to
// 0886 (NativeMemory, NativeCreatureStats, native_object_common, native_creature_known_spells,
// native_creature_template_finish, NativeRun). No new behaviour: each wrapper instantiates exactly the
// glue 0886 already uses for the same original address.
//
// 005AFCE0 CSWSCreatureStats::ReadStatsFromGff(resource, struct, out) RET 0Ch (same glue as NativeCreatureTemplate::stats_header)
extern "C" unsigned __fastcall w2x_creature_stats_005afce0(void* stats, void*, void* resource, void* structure, void* out) {
  NativeMemory m; NativeCreatureStats p(m); alignas(8) unsigned char scratch[0x120];
  return kotor::server_mainloop::creature_stats_identity_005afce0(m, static_cast<kotor::server_mainloop::CreatureStatsScalarPorts&>(p), word(stats), word(resource), word(structure), word(out), word(scratch));
}
// 004D1CF0 CSWSObject::LoadObjectState(resource, struct) RET 8
extern "C" void __fastcall w2x_object_state_004d1cf0(void* object, void*, void* resource, void* structure) {
  native_object_common(word(object), word(resource), word(structure));
}
// 005AEB30 CSWSCreatureStats::ReadSpellsFromGff(resource, struct) RET 8
extern "C" void __fastcall w2x_known_spells_005aeb30(void* stats, void*, void* resource, void* structure) {
  native_creature_known_spells(word(stats), word(resource), word(structure));
}
// 004F1C40 CSWSCreature::PostProcess() RET
extern "C" void __fastcall w2x_template_finish_004f1c40(void* creature, void*) {
  native_creature_template_finish(word(creature));
}
// 004B6270 CServerExoAppInternal::RunModule() (same glue as NativeServerSystem::run_module; that caller
// passes [server+4], i.e. the internal object, which is what this wrapper receives)
extern "C" unsigned __fastcall w2x_run_module_004b6270(void* internal, void*) {
  NativeMemory m; NativeRun p(m); unsigned char scratch[0x10];
  return kotor::server_mainloop::server_run_004b6270(m, p, word(internal), word(scratch));
}
//
// ---- W2 tier 3 additions (same glue 0886 already instantiates for each address) ----
// 005638D0 / 00563950 CSWPartyTable::AddGameInProgress / RemoveGameInProgress() RET (NativeMount, as NativeLoadRequest)
extern "C" void __fastcall w2x_party_mount_005638d0(void* party, void*) {
  NativeMemory m; NativeMount p; unsigned scratch[2]; kotor::server_mainloop::party_mount_005638d0(m, p, word(party), word(scratch));
}
extern "C" void __fastcall w2x_party_unmount_00563950(void* party, void*) {
  NativeMemory m; NativeMount p; unsigned scratch[2]; kotor::server_mainloop::party_unmount_00563950(m, p, word(party), word(scratch));
}
// 004EDD60 CSWSCreature::GetGold() RET (NativeCreatureOwnerValues, as creature_stats_native_combatguard owner_value)
extern "C" unsigned __fastcall w2x_creature_gold_004edd60(void* creature, void*) {
  NativeMemory m; NativeCreatureOwnerValues p(m); return kotor::server_mainloop::creature_owner_value_004edd60(m, p, word(creature));
}
// 005560B0 CItemRepository::ItemListGetItem(index) RET 4 (NativeInventoryItem)
extern "C" void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, unsigned index) {
  NativeMemory m; NativeInventoryItem p; return ptr(kotor::server_mainloop::inventory_item_005560b0(m, p, word(repository), index));
}
// 005026D0 CSWSCreature::LoadFromTemplate(CResRef*, flag) RET 8 (NativeCreatureTemplate, as area_creatures_native_safe load_template)
extern "C" unsigned __fastcall w2x_creature_template_005026d0(void* creature, void*, void* ref, unsigned flag) {
  NativeMemory m; NativeCreatureTemplate p(m); alignas(8) unsigned char scratch[0x50];
  return kotor::server_mainloop::creature_template_005026d0(m, p, word(creature), word(ref), flag, word(scratch));
}
// 004BE5E0 CSWSArea::TestSafeLocationPoint(Vector*, ai) RET 8 (NativeAreaSafeValid, as area_safe_position_native_reaction valid)
extern "C" unsigned __fastcall w2x_area_safe_point_004be5e0(void* area, void*, void* position, unsigned ai) {
  NativeMemory m; NativeAreaSafeValid p(m); alignas(8) unsigned char scratch[0x5c];
  return kotor::server_mainloop::area_safe_valid_004be5e0(m, p, word(area), word(position), ai, word(scratch));
}
// 004BB600 CSWSArea::GetRoom(Vector*, int* out_room_index) RET 8 (NativeAreaFindRoom, as area_safe_position_native_reaction room);
// the original writes *out = -1 at 004BB651 (t4: parameter renamed/retyped per W5; ABI unchanged)
extern "C" unsigned __fastcall w2x_area_room_004bb600(void* area, void*, void* position, int* out_room_index) {
  NativeMemory m; NativeAreaFindRoom p; alignas(8) unsigned char scratch[0x48];
  return kotor::server_mainloop::area_find_room_004bb600(m, p, word(area), word(position), word(out_room_index), word(scratch));
}
//
// ---- W2 tier 4 addition ----
// 004F3FE0 CSWSCreature::GetTotalEffectBonus, ability case only: the same guard 0886 already uses
// (NativeCreatureAbility::effects traps every argument shape except case 4 / null target / ability < 6).
extern "C" unsigned __fastcall w2x_creature_effect_ability_004f3fe0(void* creature, void*, unsigned a1, unsigned a2, unsigned a3, unsigned a4,
                                                                     unsigned a5, unsigned a6, unsigned a7, unsigned a8, unsigned a9, unsigned a10) {
  NativeMemory m; NativeCreatureAbility p(m); return p.effects(word(creature), a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}
//
// ---- W2 tier 4b addition ----
// 004F3FE0 skill case (a1 = 5, null target, out a3, skill a8): 1122 creature_effect_skill_004f3fe0 through 0886
// NativeCreatureSkill (its non-empty effect list still traps at 004F5A8F). Shape checks live in the W2 entry.
extern "C" unsigned __fastcall w2x_creature_effect_skill_004f3fe0(void* creature, void*, unsigned* out, unsigned skill) {
  NativeMemory m; NativeCreatureSkill p(m); return kotor::server_mainloop::creature_effect_skill_004f3fe0(m, p, word(creature), word(out), skill);
}
//
// ---- W2 tier 5 additions ----
// 00553CF0 CSWSItem::CompareItem(other) RET 4 over the 1159 owner kotor::server_mainloop::item_stack_compatible_00553cf0
// (0886 NativeItemStack ports). Name matches W6's import alias target @recovered_compare_item_00553cf0@12.
extern "C" unsigned __fastcall recovered_compare_item_00553cf0(void* self, void*, void* other) {
  NativeMemory m; NativeItemStack p(m); return kotor::server_mainloop::item_stack_compatible_00553cf0(m, p, word(self), word(other));
}
// 005BFA70 CSWSFaction::AddMember(id, flag) RET 8 over 1038 faction_member_005bfa70 (0886 NativeFactionMember).
extern "C" void __fastcall w2x_faction_add_member_005bfa70(void* faction, void*, unsigned id, unsigned flag) {
  NativeMemory m; NativeFactionMember p(m); kotor::server_mainloop::faction_member_005bfa70(m, p, word(faction), id, flag);
}
// 005A5680 CSWSCreatureStats::SetMovementRate(rate) RET 4 over 1006 creature_movement_rate_005a5680 (0886 NativeCreatureMovement).
extern "C" void __fastcall w2x_movement_rate_005a5680(void* stats, void*, unsigned rate) {
  NativeMemory m; NativeCreatureMovement p; alignas(8) unsigned char scratch[0x38];
  kotor::server_mainloop::creature_movement_rate_005a5680(m, p, word(stats), rate, word(scratch));
}
