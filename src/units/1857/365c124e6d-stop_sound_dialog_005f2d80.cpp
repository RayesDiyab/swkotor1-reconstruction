// Original 005F2D80 CClientExoAppInternal::StopSoundPlayingInDialog.
// thiscall(self), no stack args, RET 0, void.
// Missing 0060F540 is an exact unowned provider (D2a); no trap/stub is used.
#include "b70a081b37-dialog_callee_contracts.hpp"

extern "C" void __fastcall kotor_fastcall_005f2d80(void* self, void*) {
    using namespace dlgcallee;

    void* const gui = lp(self, 0x40);
    const U32 id = lw(gui, 0x180);

    void* object = nullptr;
    const U8 status =
        kotor_fastcall_004d8230(lp(self, 0x14), nullptr, id, &object);
    if (status != 0) {
        return;
    }
    if (object == nullptr) {
        return;
    }

    void* const as_object = vcall<void*>(object, 0x0C);
    if (as_object == nullptr) {
        // Original 005F2DBD jumps to B4b 005F2DEA: no +0x2C, no 0060F540,
        // and no sound stop.
        return;
    }

    void* const as_creature = vcall<void*>(as_object, 0x2C);
    if (as_creature != nullptr) {
        // The original calls +0x2C twice; preserve the second call/result and
        // pass it to 0060F540 without a null guard.
        void* const animbase = vcall<void*>(as_object, 0x2C);
        kotor_fastcall_0060f540(animbase, nullptr);
    }

    void* const sound = lp(lp(self, 0x40), 0xB40);
    if (sound != nullptr) {
        kotor_fastcall_005d5b80(sound, nullptr);
    }
}
