// Authored packet source — reviewed, unbuilt; provider/EH gates remain.
//
// 004CD660 CSWSObject::RemoveObjectFromDialog — thiscall, 1 stack arg, ret 4.
#include "a4d5785d2e-dialog_dependencies_contracts.hpp"

extern "C" int __fastcall kotor_fastcall_004cd660(void* self_bits, void*, U32 id) {
    auto* self = static_cast<CSWSObject*>(self_bits);
    CSWSDialog* dialog = self->dialog;
    if (dialog == nullptr || self->id == id) {
        return 0;
    }
    CExoLinkedListInternal* const list = dialog->players;
    if (list->head != nullptr) {
        if (kotor_fastcall_0059fb60(dialog, nullptr, id) != 0) {
            return 1;
        }
    } else {
        // Reachable null-head path: the original's only candidate id is the
        // invalid-object constant, so any other id returns 1.
        if (0x7F000000u != id) {
            return 1;
        }
    }
    return kotor_fastcall_004cb4f0(self_bits, nullptr);
}
