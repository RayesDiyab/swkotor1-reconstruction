// Private reconstruction of original 005A6610
// CSWSCreatureStats::GetExpNeededForLevelUp.
//
// Original ABI: __fastcall(stats, dummy), no stack arguments, plain RET 0.
// Reconstruction export: @kotor_fastcall_005a6610@8.
// No SEH, no heap, no indirect call.
//
// Original 005A6610:
//   push 0; call 005A5FD0(stats, 0) -> AL = level
//   ECX = [0x7A3A28] (rules pointer)
//   MOVZX EAX, AL; MOV EAX, [ECX + EAX*4 + 0x38]
//
// Selected providers:
//   005A5FD0  @recovered_stats_level_005a5fd0@12
//   007A3A28  _kotor_cell_rules_007a3a28 (32-bit rules pointer cell)
//
// Source-only private proposal; not built/run. No native or runtime acceptance.
#include "da79f00dee-w4_providers.hpp"

using w4::U32;

extern "C" {
unsigned __fastcall recovered_stats_level_005a5fd0(void*, void*, unsigned);
extern void* kotor_cell_rules_007a3a28;
}

extern "C" unsigned __fastcall kotor_fastcall_005a6610(void* stats, void*) {
    const unsigned char level =
        static_cast<unsigned char>(recovered_stats_level_005a5fd0(stats, nullptr, 0));
    void* const rules = kotor_cell_rules_007a3a28;

    // Original unchecked rules dereference: a null rules pointer faults at +0x38.
    return w4::lw(rules, 0x38u + static_cast<U32>(level) * 4u);
}
