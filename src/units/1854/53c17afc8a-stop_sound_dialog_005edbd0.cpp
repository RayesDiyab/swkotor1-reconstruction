// Original 005EDBD0 CClientExoApp::StopSoundPlayingInDialog.
// Body: mov ecx,[ecx+4]; jmp 005F2D80. thiscall, no stack args, RET 0.
// The exact target 005F2D80 is a missing provider (D2); its numeric VA is an
// unrelated rebuilt symbol, so only the exact extern is called.
#include "e93cd573fa-dialog_camera_client_contracts.hpp"

extern "C" void __fastcall kotor_fastcall_005edbd0(void* self, void*) {
    kotor_fastcall_005f2d80(dlgcam::lp(self, 4), nullptr);
}
