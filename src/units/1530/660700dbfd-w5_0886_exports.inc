// Lane W5 integration include for the 0886 TU (server_mainloop_native_vm_canonical_cell.cpp).
// #include this file at the END of that TU, after W1's w1_0886_exports.inc (and W2's w2_0886_exports.inc).
// Same pattern as those files: extern "C" fastcall-with-dummy-EDX wrappers over EXISTING portable providers, built
// with the native glue the TU already uses for the same address. No new behaviour; no address credit.
// Scratch sizes are the ones the TU already uses for the same bodies.
// This file supersedes source/w5_server_mainloop_adapters.inc (t1) and w5_t2_server_mainloop_adapters.inc (t2).
// 004BB600 GetRoom and 004BE5E0 TestSafeLocationPoint are W2's (w2x_area_room_004bb600 / w2x_area_safe_point_004be5e0,
// lane-w2 tiers t3b/t4 w2_0886_exports.inc; t2b ruling). Include this file after W2's.

// 004BC590 CSWSArea::NoCreaturesOnLine(a*, b*, creature, p4..p7, hit*) RET 20h (NativeAreaCreatureCollision)
extern "C" int __fastcall w5x_area_no_creatures_on_line_004bc590(void* area, void*, void* a, void* b, void* creature,
    int p4, int p5, int p6, int p7, void* walk_hit) {
    NativeMemory m; NativeAreaCreatureCollision p; alignas(8) unsigned char scratch[0xb4];
    return static_cast<int>(kotor::server_mainloop::area_creature_collision_004bc590(m, p, word(area), word(a), word(b),
        word(creature), static_cast<Word>(p4), static_cast<Word>(p5), static_cast<Word>(p6), static_cast<Word>(p7),
        word(walk_hit), word(scratch)));
}
// 005B4880 CSWSCreature::IsDebilitated() RET 0 (NativeCreatureSkill, as creature_forcepoints_native_combatguard)
extern "C" unsigned __fastcall w5x_creature_is_debilitated_005b4880(void* creature, void*) {
    NativeMemory m; NativeCreatureSkill p(m);
    return kotor::server_mainloop::creature_dex_restricted_005b4880(m, p, word(creature));
}
// Same binding under the name W4's callers import (W4 drops its pending stub @kotor_fastcall_005b4880@8).
extern "C" unsigned __fastcall kotor_fastcall_005b4880(void* creature, void* edx) {
    return w5x_creature_is_debilitated_005b4880(creature, edx);
}
