// Original 0062E020 — GUI dialog-model publication.
// thiscall ECX=gui, six stack DWORDs, RET 0x18, EAX 0/1.
// The first stack DWORD is never read. No EH, no allocation, no x87.
#include "1a93eac6b5-dialog_gui_contracts.hpp"

extern "C" int __fastcall kotor_fastcall_0062e020(
    void* self, void*, dlgpub::U32, dlgpub::U32 count,
    void* ids, void* refs, void* resref, void* arg6) {
    (void)kotor_fastcall_0062b850(self, nullptr, count);

    for (dlgpub::U32 index = 0; index < count; ++index) {
        if (index < dlgpub::lw(self, 0x18C)) {
            void* const dst = dlgpub::lp(self, 0x190);
            if (dst != nullptr) {
                void* const ref_dst = dlgpub::lp(self, 0x194);
                if (ref_dst != nullptr) {
                    dlgpub::lwset(dst, index * 4u,
                                  dlgpub::lw(ids, index * 4u));
                    dlgpub::U8 temp[16];
                    (void)c2da_copy_00406120(
                        static_cast<dlgpub::U8*>(ref_dst) + index * 16u,
                        nullptr, temp,
                        static_cast<dlgpub::U8*>(refs) + index * 16u);
                }
            }
        }
    }

    if (kotor_fastcall_0062c620(self, nullptr) == 0) {
        return 0;
    }
    if (gui_load_resref_is_valid_00406030(resref, nullptr) != 0 &&
        dlgpub::lw(self, 0xB28) == 0) {
        void* const client = kotor_fastcall_005ed530(
            dlgpub::lp(kotor_menu_window_cells::manager_007a39fc, 4),
            nullptr);
        if (client != nullptr) {
            kotor_fastcall_00640f70(client, nullptr, resref);
            dlgpub::lwset(self, 0xB28, 1);
        }
    }
    (void)kotor_fastcall_0062a8b0(self, nullptr, arg6);
    return 1;
}
