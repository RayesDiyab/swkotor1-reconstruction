// Original 005EDC90 CClientExoApp::GetCanSendDialog.
// Body: mov ecx,[ecx+4]; jmp 005F3070. thiscall + 3 stack args, RET 0xC.
// The target 005F3070 is an exact-symbol missing provider (D1); no body, trap or
// numeric-VA bind is substituted. The naked tail transfer preserves the original
// single-call frame/cleanup; a normal call is stack-balanced but not a tail.
#include "e93cd573fa-dialog_camera_client_contracts.hpp"

extern "C" __declspec(naked) int __fastcall kotor_fastcall_005edc90(
    void*, void*, dlgcam::U32, dlgcam::U32, void*) {
    __asm {
        mov ecx, dword ptr [ecx + 4]
        jmp kotor_fastcall_005f3070
    }
}
