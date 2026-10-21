// Original 006A6DA0 CSWGuiDialogLetterbox::IsFadeDone.
// thiscall(self), no stack args, RET 0, int 0/1.
// Exact x87 sequence/masks are preserved by the private helpers.
#include "b70a081b37-dialog_callee_contracts.hpp"

extern "C" int __fastcall kotor_fastcall_006a6da0(void* self, void*) {
    using namespace dlgcallee;

    float elapsed;
    std::memcpy(&elapsed, static_cast<const U8*>(self) + 0x9C, 4);

    const float zero = 0.0f;
    const U16 first = x87_cmp(&elapsed, &zero);
    if (((first >> 8) & 0x44) == 0x40) {
        // AH mask, C3=1/C2=0: ordered equality with +0.0/-0.0 only.
        return 1;
    }

    float length;
    float delay;
    std::memcpy(&length, static_cast<const U8*>(self) + 0x98, 4);
    std::memcpy(&delay, static_cast<const U8*>(self) + 0x94, 4);

    // Original re-reads [self+0x9C] for the second fcomp.
    float elapsed2;
    std::memcpy(&elapsed2, static_cast<const U8*>(self) + 0x9C, 4);
    const U16 second = x87_add_cmp(&length, &delay, &elapsed2);
    return (((second >> 8) & 0x05) == 0x01) ? 1 : 0;
}
