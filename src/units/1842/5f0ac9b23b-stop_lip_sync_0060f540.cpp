// Original 0060F540 CSWCCreature::StopLipSync.
// thiscall(self), no stack args, RET 0, void.
// Selected anim-base +0x5C target is real 0069D540 @00531EE0 (or @00535A20
// naked forwarder); no new 0069D540 body is authored.
#include "2fa3712460-dialog_model_contracts.hpp"

extern "C" void __fastcall kotor_fastcall_0060f540(void* self, void*) {
    using namespace dlgmodel;

    void* const animbase = lp(self, 0x68);
    void** const vtable = *reinterpret_cast<void***>(animbase);

    using F = void(__thiscall*)(void*, int, int, int, int);
    reinterpret_cast<F>(vtable[0x5C / 4])(animbase, 0, 0, 0, 0);
}
