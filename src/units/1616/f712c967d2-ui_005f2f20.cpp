// 005F2F20 — CClientExoAppInternal::HurryDeathSequence (53 bytes, 0x005F2F20..0x005F2F55).
//
// Original-byte reference: EVIDENCE/function-005F2F20.disasm.txt.
// Exact accepted 0769 import:
//   ?hurry_death_sequence_005f2f20@kotor_input_required@@YIXPAX0@Z
//
// ABI: __fastcall(ECX=CClientExoAppInternal*, EDX unused, 0 stack args), RET 0, void.
// Exact call: StartGlobalFade(gui, 0, 0, 0.5f, &zero12), ECX=gui, four stack dwords,
// RET 0x10.  The 12-byte zero block is zeroed before the call.
#include "1f393e8651-ui_save_state_types.h"

namespace kotor_input_required {

void __fastcall hurry_death_sequence_005f2f20(void* self, void*) {
    void* const gui = ui_load_at<void*>(self, 0x40);

    unsigned int zero12[3] = {0u, 0u, 0u};
    kotor_m15_ingame_start_global_fade_0062abf0(
        gui, nullptr, 0u, 0u, 0.5f, zero12);
}

}  // namespace kotor_input_required
