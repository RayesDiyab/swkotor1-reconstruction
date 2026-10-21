// Original 00640F70 — lazy CResRef resource load, own C++ EH.
// thiscall ECX=object, one stack CResRef*, RET 4.
// The state-0 unwind action destroys only the local CExoString. The guard's
// closing scope is the original state=-1 point (00640FEC); the explicit destroy
// follows.
#include "1a93eac6b5-dialog_gui_contracts.hpp"

namespace {

struct StringCleanup {
    kotor_menu_10_19::String* value;
    bool active;

    ~StringCleanup() noexcept {
        if (active) {
            value->destroy_005e5c20();
        }
    }
};

}  // namespace

extern "C" void __fastcall kotor_fastcall_00640f70(
    void* self, void*, void* ref) {
    if (dlgpub::lp(self, 0x12C) != nullptr) {
        return;
    }
    if (c2da_not_equal_text_00406060(ref, nullptr, "") == 0) {
        return;
    }

    kotor_menu_10_19::String value;
    value.default_005b3190();
    {
        // Constructed after the default ctor, before 00405F70; state 0 here.
        StringCleanup cleanup{&value, true};
        c2da_ref_to_string_00405f70(ref, nullptr, &value);
        const char* const text = value.text_005e5670();
        void* const resource =
            aurora_new_aur_object_00449cc0(text, "", 0, 0);
        dlgpub::lwset(self, 0x12C,
                      static_cast<dlgpub::U32>(
                          reinterpret_cast<std::uintptr_t>(resource)));
        cleanup.active = false;
    }  // original state=-1 at 00640FEC
    value.destroy_005e5c20();
}
