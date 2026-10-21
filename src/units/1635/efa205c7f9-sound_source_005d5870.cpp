// 005D5870 CExoSoundSource::CExoSoundSource() wrapper.
//
// Original body: 005D5870..005D58FF (0x90 bytes including its SEH prologue).
// It is a separate body from the accepted sibling 005D60E0, which has an extra
// CResRef parameter and a res_helper_set_ref_005d5e90 call. This wrapper does
// not call 00405ED0: the 16-byte zero at internal+0xC is inside the selected
// lower constructor 006DED60.
#include "67223098d0-sound_source_005d5870.h"

// Explicit global-scope using-declarations: a using-directive inside an unnamed
// namespace would not make these helpers visible in the global-scope wrapper.
using r4_sound_corpse::load_ptr;
using r4_sound_corpse::store_ptr;

namespace {

// Original EH cleanup funclet 00723FF0 calls 006FA390(live_free) on the saved
// new block at [ebp-0x10], then continues the exception. The accepted sibling
// 005D60E0 source uses the same Guard pattern; this is a separate wrapper body,
// not a copy of the sibling function.
struct LiveBlockGuard {
    void* block;

    ~LiveBlockGuard() noexcept {
        if (block != nullptr) live_free(block);
    }
};

} // namespace

extern "C" void* __fastcall kotor_sound_source_construct_005d5870(void* self, void*) noexcept(false) {
    // 005D5889: [this] = selected vtable before the Disable Sound branch.
    store_ptr(self, 0, &kotor_sound_rev3::vtable_0074c608[0]);

    // 005D588F..005D5896: nonzero Disable Sound -> null internal, no allocation,
    // no lower constructor.
    if (kotor_cell_disable_sound_00832894 != 0) {
        store_ptr(self, 4, nullptr);
        return self;
    }

    // 005D58B1..005D58C4: new(0xA0); the guard covers the raw block from here.
    void* const raw = live_operator_new(0xA0);
    LiveBlockGuard guard{raw};

    // 005D58C2..005D58E2: a null allocation stores a null internal and never
    // dereferences the 007A39EC cell.
    void* internal = nullptr;
    if (raw != nullptr) {
        // 005D58CE..005D58D6: cell value -> object -> first dword. The cell
        // address is not the system argument. No null guard, as in the original.
        void* const sound_object = kotor_cell_sound_007a39ec;
        void* const system = load_ptr(sound_object, 0);

        // 005D58D6..005D58DE: ECX=raw, EDX=system (unused dummy), stack=system.
        internal = kotor_sound_rev3::InternalConstruct005daf50(raw, system, system);
    }

    // 005D58E2..005D58E9: disarm the cleanup and store the returned internal.
    guard.block = nullptr;
    store_ptr(self, 4, internal);
    return self;
}
