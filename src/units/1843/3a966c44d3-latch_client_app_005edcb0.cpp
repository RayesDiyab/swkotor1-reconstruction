// Original 005EDCB0 — client-app latch.
// thiscall ECX=app, one stack DWORD, RET 4. Writes [ [app+4] + 0x234 ].
// No guard, no normalization, no null check, no branch, no EH, no x87.
#include "1a93eac6b5-dialog_gui_contracts.hpp"

extern "C" void __fastcall kotor_fastcall_005edcb0(
    void* app, void*, dlgpub::U32 value) {
    void* const target = dlgpub::lp(app, 4);
    dlgpub::lwset(target, 0x234, value);
}
