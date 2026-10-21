// 0886-gates (Claude): C-linkage wrappers over 0886-unit glue for the claude_0886_bodies_*.cpp bodies.
// No new behaviour; each wrapper instantiates the glue this unit already uses for the same address.
// 005A6D80 CSWSCreatureStats::AdjustSpellUsesPerDay (thiscall, RET 0), the same call NativeCreatureStats::recalculate_005a6d80 makes.
extern "C" void __fastcall w0886x_stats_adjust_spell_uses_005a6d80(void* stats, void*) {
  NativeMemory m; NativeCreatureAbility p(m); alignas(8) unsigned char scratch[0x10];
  creature_recalculate_005a6d80(m, p, word(stats), word(scratch));
}

// 00587170 CSWSPlaceable::AddToArea(CSWSArea*, Vector*, int) RET 0Ch -> 1165 port via the 0886 placement glue (effects-g3 OnRemoveDisappearAppear)
extern "C" void __fastcall kotor_fastcall_00587170(void* placeable, void*, void* area, void* position, unsigned flag){native_placeable_placement(word(placeable),word(area),word(position),flag);}
