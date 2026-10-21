// Original 006412B0 CSWCModule::IsCameraAnimated.
// thiscall(module), no stack args, RET 0.
// Reads camera at +0x12C and its CExoString name at +0x130, then calls the
// selected camera vtable +0x1C (gob_get_animation) with (name,0,0).
#include "e93cd573fa-dialog_camera_client_contracts.hpp"

extern "C" int __fastcall kotor_fastcall_006412b0(void* self, void*) {
    using namespace dlgcam;

    void* const camera = lp(self, 0x12C);
    if (camera == nullptr) {
        return 0;
    }

    // Original reads [camera] before the CStr call.
    void** const vtable = *reinterpret_cast<void***>(camera);
    const char* const name =
        WindowCStr005e5670(atv(self, 0x130), nullptr);

    using F = void* (__thiscall*)(void*, const char*, int, int);
    // Original re-reads [module+0x12C] at 006412CF after CStr, before the call.
    void* const camera_for_call = lp(self, 0x12C);
    void* const result =
        reinterpret_cast<F>(vtable[0x1C / 4])(camera_for_call, name, 0, 0);

    return result != nullptr ? 1 : 0;
}
