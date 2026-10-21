// Original 004CC4B0 CSWSDialog::GetPlayerObjectId.
// thiscall ECX=dialog, no stack args, RET 0, EAX u32.
// Returns [this+0x7C] when [[this+0x64]] != 0, else 0x7F000000.
// The unguarded [this+0x64] dereference is preserved (original fault boundary).
#include "65b8eb6bac-dialog_target_lookup_contracts.hpp"

static_assert(sizeof(void*) == 4, "004CC4B0 requires the original x86 pointer ABI");

extern "C" dtl::U32 __fastcall kotor_fastcall_004cc4b0(void* self, void*) {
    using namespace dtl;

    void* const p = lp(self, 0x64);
    if (*reinterpret_cast<const volatile U32*>(p) != 0) {
        return lw(self, 0x7C);
    }
    return 0x7F000000u;
}
