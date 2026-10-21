// Original 005F3070 CClientExoAppInternal::GetCanSendDialog.
// thiscall(self) + 3 stack args, RET 0xC, int 0/1.
// Missing 005A0240/005A0080 are exact unowned providers; no trap/stub is used.
#include "b70a081b37-dialog_callee_contracts.hpp"

extern "C" int __fastcall kotor_fastcall_005f3070(
    void* self, void*, dlgcallee::U32 arg1, dlgcallee::U32 arg2, void* dialog) {
    using namespace dlgcallee;

    // B0: clear activity for arg2 (first), then B1 for arg1.
    {
        void* const server = lp(kotor_menu_window_cells::manager_007a39fc, 8);
        void* const object =
            kotor_fastcall_004ae750(server, nullptr, arg2);
        if (object != nullptr) {
            void* const first = vcall<void*>(object, 0x30);
            if (first != nullptr) {
                void* const activity = vcall<void*>(object, 0x30);
                recovered_set_activity_004f00a0(activity, nullptr, 1, 0, 1);
            }
        }
    }
    {
        void* const server = lp(kotor_menu_window_cells::manager_007a39fc, 8);
        void* const object =
            kotor_fastcall_004ae750(server, nullptr, arg1);
        if (object != nullptr) {
            void* const first = vcall<void*>(object, 0x30);
            if (first != nullptr) {
                void* const activity = vcall<void*>(object, 0x30);
                recovered_set_activity_004f00a0(activity, nullptr, 1, 0, 1);
            }
        }
    }

    // B2: pause state (server null skips).
    {
        void* const server = lp(kotor_menu_window_cells::manager_007a39fc, 8);
        if (server != nullptr) {
            if (frame_GetPauseState_004ae980(server, nullptr, 2) != 0) {
                return 0;
            }
        }
    }

    // B3: dialog-disable bit.
    if ((lb(self, 0x384) & 1) != 0) {
        return 0;
    }

    // B4: early success latch, no write.
    if (lw(self, 0x230) != 0) {
        return 1;
    }

    // B5: timer state must be initialized.
    if (lw(self, 0x228) == 0) {
        return 0;
    }
    if (lw(self, 0x22C) == 0) {
        return 0;
    }

    // B7: 64-bit snapshot time, divide by 1000, keep low dword.
    {
        void* const client_app =
            lp(kotor_menu_window_cells::manager_007a39fc, 4);
        void* const client_internal = lp(client_app, 4);
        void* const timer = lp(client_internal, 0x2C);
        const I64 now = kotor_fastcall_004adc70(timer, nullptr);
        const I64 quotient = now / 1000;  // signed 64-bit division: the compiler emits the CRT __alldiv helper (stdcall-style, callee-clean) exactly as the original
        const U32 low = static_cast<U32>(quotient);
        const U32 previous = lw(self, 0x22C);
        if (static_cast<U32>(low - previous) <= 1000u) {
            return 0;
        }
        const U32 next = low - 2000u;
        const bool skip_arg1 = (arg1 == 0x7F000000u);
        lwset(self, 0x22C, next);
        if (!skip_arg1) {
            void* const lookup_app =
                lp(kotor_menu_window_cells::manager_007a39fc, 4);
            const U32 mapped =
                kotor_fastcall_005eda50(lookup_app, nullptr, arg1);
            void* const lookup_app2 =
                lp(kotor_menu_window_cells::manager_007a39fc, 4);
            void* const object =
                kotor_fastcall_005ed580(lookup_app2, nullptr, mapped);
            if (object == nullptr) {
                return 0;
            }
            if (vcall<void*>(object, 0x0C) == nullptr) {
                return 0;
            }
        }
    }

    // B9: same client validity check for arg2.
    if (arg2 != 0x7F000000u) {
        void* const client_app =
            lp(kotor_menu_window_cells::manager_007a39fc, 4);
        const U32 mapped =
            kotor_fastcall_005eda50(client_app, nullptr, arg2);
        void* const client_app2 =
            lp(kotor_menu_window_cells::manager_007a39fc, 4);
        void* const object =
            kotor_fastcall_005ed580(client_app2, nullptr, mapped);
        if (object == nullptr) {
            return 0;
        }
        if (vcall<void*>(object, 0x0C) == nullptr) {
            return 0;
        }
    }

    // B10: conversation-active shortcut.
    if (lw(self, 0x234) != 0) {
        lwset(self, 0x230, 1);
        return 1;
    }

    // B11: dialog model loop. Out aliases the incoming arg2 stack home.
    I32 count = static_cast<I32>(lw(dialog, 0x48));
    if (count > 0) {
        I32 i = 0;
        for (;;) {
            void* out = reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(arg2));
            const U32 id =
                kotor_fastcall_005a0240(dialog, nullptr, i, &out);
            if (out == nullptr) {
                void* const client_app =
                    lp(kotor_menu_window_cells::manager_007a39fc, 4);
                void* const object =
                    kotor_fastcall_005ed580(client_app, nullptr, id);
                if (object == nullptr) {
                    return 0;
                }
                if (vcall<void*>(object, 0x0C) == nullptr) {
                    return 0;
                }
            }

            count = static_cast<I32>(lw(dialog, 0x48));
            ++i;
            if (i >= count) {
                break;
            }
        }
    }

    // B12: final player/dialog-owner send path.
    {
        void* server = lp(kotor_menu_window_cells::manager_007a39fc, 8);
        const U32 player =
            kotor_fastcall_004aea40(server, nullptr);
        server = lp(kotor_menu_window_cells::manager_007a39fc, 8);
        void* const owner =
            kotor_fastcall_004ae910(server, nullptr, player);
        server = lp(kotor_menu_window_cells::manager_007a39fc, 8);
        const U32 dialog_id = lw(dialog, 0x80);
        void* const object =
            kotor_fastcall_004ae750(server, nullptr, dialog_id);
        if (object == nullptr) {
            return 0;
        }
        if (vcall<void*>(object, 0x10) == nullptr) {
            return 0;
        }
        void* const z = vcall<void*>(object, 0x10);
        kotor_fastcall_005a0080(dialog, nullptr, z, lw(owner, 4));
    }

    if (lw(self, 0x234) == 0) {
        return 0;
    }
    lwset(self, 0x230, 1);
    return 1;
}
