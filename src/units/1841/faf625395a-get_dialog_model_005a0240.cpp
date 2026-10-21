// Original 005A0240 CSWSDialog::GetDialogModel.
// thiscall(self) + 2 stack args (signed index, U32* out), RET 8.
// Missing 004CC4B0/004CB5C0 are exact unowned providers; no trap/stub is used.
#include "0ca2333203-dialog_model_contracts.hpp"

extern "C" dlgmodel::U32 __fastcall kotor_fastcall_005a0240(
    void* self, void*, dlgmodel::I32 index, dlgmodel::U32* out) {
    using namespace dlgmodel;

    *out = 0;

    if (index < 0) {
        return 0x7F000000u;
    }
    const I32 count = static_cast<I32>(lw(self, 0x48));
    if (index >= count) {
        return 0x7F000000u;
    }

    void* const server =
        lp(kotor_menu_window_cells::manager_007a39fc, 8);
    void* const object =
        kotor_fastcall_004ae750(server, nullptr, lw(self, 0x80));
    if (object == nullptr) {
        return 0x7F000000u;
    }

    if (vcall<void*>(object, 0x10) == nullptr) {
        return 0x7F000000u;
    }
    void* const second = vcall<void*>(object, 0x10);

    void* const client =
        lp(kotor_menu_window_cells::manager_007a39fc, 4);
    auto model_at = [&]() -> void* {
        return reinterpret_cast<void*>(
            static_cast<U8*>(lp(self, 0x4C)) +
            static_cast<U32>(index) * 0x18u);
    };
    auto text_at = [&]() -> kotor_reconstruction::StartupString* {
        return reinterpret_cast<kotor_reconstruction::StartupString*>(
            model_at());
    };

    if (text_at()->BorrowedEquals005e5290("PLAYER") != 0) {
        return kotor_fastcall_005eda50(
            client, nullptr, kotor_fastcall_004cc4b0(self, nullptr));
    }
    if (text_at()->BorrowedEquals005e5290("OWNER") != 0 ||
        text_at()->BorrowedEquals005e5290("") != 0) {
        return kotor_fastcall_005eda50(client, nullptr, lw(self, 0x80));
    }

    const U32 nearest =
        kotor_fastcall_004cb5c0(second, nullptr, model_at(), 0.0f);
    const U32 mapped = kotor_fastcall_005eda50(client, nullptr, nearest);
    void* const gui = frame_R4_005ed690(client, nullptr);
    const U32 predicate = kotor_fastcall_0062f840(gui, nullptr, model_at());
    *out = predicate;
    return mapped;
}
