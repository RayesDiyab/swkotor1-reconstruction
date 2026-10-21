// Original 005A0080 CSWSDialog::SendDialogModels.
// thiscall(self) + 2 stack args, RET 8, EAX=1 on normal paths.
// Real C++ unwind cleanup is active only during the 00405ED0 constructor loop
// and frees the first allocation; later exceptions leak. Missing providers are
// exact unresolved externs (004CB5C0, 0062E020, 005EDCB0).
// 00405ED0 is provided by the selected real 0263 implementation through the
// exact @c2da_default_ref_00405ed0@8 typed symbol; original 0062C620 is a
// different GUI function.
#include "0ca2333203-dialog_model_contracts.hpp"

namespace {

struct ModelsCleanup {
    void* models;
    bool active;

    ~ModelsCleanup() {
        if (active) {
            live_free(models);
        }
    }
};

}  // namespace

extern "C" int __fastcall kotor_fastcall_005a0080(
    void* self, void*, void* arg1, dlgmodel::U32 arg2) {
    using namespace dlgmodel;

    void* const app =
        lp(kotor_menu_window_cells::manager_007a39fc, 4);
    const U32 count = lw(self, 0x48);
    void* const models = live_operator_new(count << 4);
    ModelsCleanup cleanup{models, true};

    if (models != nullptr) {
        U32 remaining = count - 1u;
        if (static_cast<I32>(remaining) >= 0) {
            ++remaining;
            U8* element = static_cast<U8*>(models);
            do {
                c2da_default_ref_00405ed0(element, nullptr);
                element += 0x10;
            } while (--remaining != 0);
        }
    }

    cleanup.active = false;

    const U32 best_count = lw(self, 0x48);
    void* const best = live_operator_new(best_count << 2);

    U32 loop_count = lw(self, 0x48);
    if (loop_count != 0) {
        U32 index = 0;
        U32 model_offset = 0;
        U8* cursor = static_cast<U8*>(models);
        auto model_at = [&]() -> void* {
            return reinterpret_cast<void*>(
                static_cast<U8*>(lp(self, 0x4C)) + model_offset);
        };
        auto text_at = [&]() -> kotor_reconstruction::StartupString* {
            return reinterpret_cast<kotor_reconstruction::StartupString*>(
                model_at());
        };
        for (;;) {
            lwset(best, index * 4, 0x7F000000u);

            U32 color;
            if (text_at()->BorrowedEquals005e5290("PLAYER") != 0) {
                void* const player = lp(self, 0x64);
                color = (lw(player, 0) != 0) ? lw(self, 0x7C)
                                             : 0x7F000000u;
            } else if (text_at()->BorrowedEquals005e5290("OWNER") != 0) {
                color = lw(self, 0x80);
            } else if (text_at()->BorrowedEquals005e5290("") != 0) {
                color = lw(self, 0x80);
            } else {
                color = kotor_fastcall_004cb5c0(
                    arg1, nullptr, model_at(), 0.0f);
            }

            const U32 mapped =
                kotor_fastcall_005eda50(app, nullptr, color);
            lwset(best, index * 4, mapped);

            U8 temp[16];
            c2da_copy_00406120(
                cursor, nullptr, temp,
                static_cast<U8*>(model_at()) + 8);

            ++index;
            model_offset += 0x18;
            cursor += 0x10;
            loop_count = lw(self, 0x48);
            if (index >= loop_count) {
                break;
            }
        }
    }

    const U32 send_arg1 = arg2;
    const U32 send_arg2 = lw(self, 0x48);
    void* const send_arg3 = best;
    void* const send_arg4 = models;
    void* const send_arg5 = atv(self, 0x50);
    void* const send_arg6 = atv(self, 0xFC);

    void* const gui = frame_R4_005ed690(app, nullptr);
    const U32 sent = kotor_fastcall_0062e020(
        gui, nullptr, send_arg1, send_arg2, send_arg3, send_arg4,
        send_arg5, send_arg6);
    if (sent != 0) {
        void* const fresh_app =
            lp(kotor_menu_window_cells::manager_007a39fc, 4);
        kotor_fastcall_005edcb0(fresh_app, nullptr, 1);
    }

    live_free(best);
    live_free(models);
    return 1;
}
