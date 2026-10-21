// Original 0062B850 — GUI model-array replacement, own C++ EH.
// thiscall ECX=gui, one stack DWORD count, RET 4, EAX 0/1.
// The state-0 unwind action frees only the refs pointer; it does not destroy
// constructed elements and does not free the ID array. The guard's closing
// scope is the original state=-1 point (0062B902); the active flag keeps the
// normal block-exit destructor from freeing refs.
#include "1a93eac6b5-dialog_gui_contracts.hpp"

namespace {

struct RefsCleanup {
    void* refs;
    bool active;

    ~RefsCleanup() noexcept {
        if (active) {
            live_free(refs);
        }
    }
};

}  // namespace

extern "C" int __fastcall kotor_fastcall_0062b850(
    void* self, void*, dlgpub::U32 count) {
    if (count == 0) {
        return 0;
    }

    void* const old_ids = dlgpub::lp(self, 0x190);
    if (old_ids != nullptr) {
        live_free(old_ids);
        dlgpub::lwset(self, 0x190, 0);
    }
    void* const old_refs = dlgpub::lp(self, 0x194);
    if (old_refs != nullptr) {
        live_free(old_refs);
        dlgpub::lwset(self, 0x194, 0);
    }

    dlgpub::lwset(self, 0x18C, count);
    void* const new_ids = live_operator_new(count * 4u);
    const dlgpub::U32 refs_count = dlgpub::lw(self, 0x18C);
    dlgpub::lwset(self, 0x190,
                  static_cast<dlgpub::U32>(
                      reinterpret_cast<std::uintptr_t>(new_ids)));
    void* const new_refs = live_operator_new(refs_count * 16u);

    void* ids_now = nullptr;
    {
        // Constructed after the refs pointer exists (the original E0+4 EH-slot
        // store at 0062B8DC), before __ehvec_ctor; state 0 is published here.
        RefsCleanup cleanup{new_refs, true};
        if (new_refs != nullptr) {
            vector_constructor_iterator_00401130(
                new_refs, 0x10u, refs_count,
                &c2da_default_ref_00405ed0);
        }
        ids_now = dlgpub::lp(self, 0x190);
        cleanup.active = false;
    }  // original state=-1 at 0062B902

    dlgpub::lwset(self, 0x194,
                  static_cast<dlgpub::U32>(
                      reinterpret_cast<std::uintptr_t>(new_refs)));
    if (ids_now != nullptr && new_refs != nullptr) {
        return 1;
    }

    live_free(ids_now);
    void* const refs_now = dlgpub::lp(self, 0x194);
    dlgpub::lwset(self, 0x190, 0);
    live_free(refs_now);
    dlgpub::lwset(self, 0x194, 0);
    dlgpub::lwset(self, 0x18C, 0);
    return 0;
}
