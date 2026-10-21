// Original 0062ABB0 CGuiInGame::IsFadingInDialog.
// thiscall(gui), no stack args, RET 0. Returns 1 when the fade/letterbox object
// at +0x68 exists and IsFadeDone returns 0.
#include "e93cd573fa-dialog_camera_client_contracts.hpp"

extern "C" int __fastcall kotor_fastcall_0062abb0(void* self, void*) {
    void* const fade = dlgcam::lp(self, 0x68);
    if (fade == nullptr) {
        return 0;
    }
    return kotor_fastcall_006a6da0(fade, nullptr) == 0 ? 1 : 0;
}
