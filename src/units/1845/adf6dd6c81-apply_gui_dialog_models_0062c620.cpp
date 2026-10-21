// Original 0062C620 — GUI dialog-model application.
// thiscall ECX=gui, no stack args, plain RET, EAX 0/1.
// Distinct from original 00405ED0 (selected at rebuilt 0062C620 in 0263).
// Dynamic slots: object->vt[0x0C], client_object->vt[0x98](0xFF/0xFE,0),
// returned Gob->vt[0x20](char* ref text). The selected bodies are @N fastcall
// facades with a dead dummy EDX; vcall_fast passes EDX=nullptr and the original
// ECX/stack arguments. No EH, no allocation, no x87.
#include "1a93eac6b5-dialog_gui_contracts.hpp"

extern "C" int __fastcall kotor_fastcall_0062c620(void* self, void*) {
    dlgpub::U32 count = dlgpub::lw(self, 0x18C);
    if (count == 0) {
        return 1;
    }

    dlgpub::U32 index = 0;
    dlgpub::U32 offset = 0;
    do {
        void* const app = kotor_menu_window_cells::manager_007a39fc;
        void* const ids = dlgpub::lp(self, 0x190);
        void* const object = kotor_fastcall_005ed580(
            dlgpub::lp(app, 4), nullptr, dlgpub::lw(ids, index * 4u));
        if (object != nullptr) {
            void* const client_object =
                dlgpub::vcall_fast<void*>(object, 0x0C);
            if (client_object != nullptr) {
                const std::uintptr_t ref_address =
                    reinterpret_cast<std::uintptr_t>(
                        dlgpub::lp(self, 0x194)) + offset;
                void* const ref = reinterpret_cast<void*>(ref_address);
                dlgpub::U8 temp[0x14];
                gui_load_resref_copy_to_string_00405fb0(
                    ref, nullptr, temp);
                if (temp[0] != 0) {
                    void* const model = dlgpub::vcall_fast<
                        void*, dlgpub::U32, dlgpub::U32>(
                        client_object, 0x98, 0xFFu, 0u);
                    if (model == nullptr) {
                        return 0;
                    }
                    (void)dlgpub::vcall_fast<dlgpub::U32, char*>(
                        model, 0x20, reinterpret_cast<char*>(temp));
                    void* const model2 = dlgpub::vcall_fast<
                        void*, dlgpub::U32, dlgpub::U32>(
                        client_object, 0x98, 0xFEu, 0u);
                    if (model2 != nullptr) {
                        (void)dlgpub::vcall_fast<dlgpub::U32, char*>(
                            model2, 0x20, reinterpret_cast<char*>(temp));
                    }
                }
            }
        }
        ++index;
        offset += 0x10u;
        count = dlgpub::lw(self, 0x18C);
    } while (index < count);
    return 1;
}
