// 005F4B50 — CClientExoAppInternal::DoQuickSave (136 bytes, 0x005F4B50..0x005F4BD8).
//
// Original-byte reference: EVIDENCE/function-005F4B50.disasm.txt.
// Exact accepted 0769 import:
//   ?do_quick_save_005f4b50@kotor_input_required@@YIXPAX0@Z
//
// ABI: __fastcall(ECX unused by the body, EDX unused, 0 stack args), RET 0, void.
// The body never reads ECX (preserved).  Two separate "QUICKSAVE" allocations; the
// two CExoString locals are destroyed B then A in reverse construction order.
#include "1f393e8651-ui_save_state_types.h"

using kotor_reconstruction::StartupString;

namespace kotor_input_required {

void __fastcall do_quick_save_005f4b50(void*, void*) {
    StartupString strA("QUICKSAVE");
    StartupString strB("QUICKSAVE");

    void* const app = kotor_menu_window_cells::manager_007a39fc;
    void* const internal = ui_load_at<void*>(app, 4);

    // Exact original stack: [esp]=0, [esp+4]=&B, [esp+8]=&A; RET 0xC.
    kotor_fastcall_005ed670(internal, nullptr, 0u, &strB, &strA);
}

}  // namespace kotor_input_required
