// Authored packet source — reviewed, unbuilt; provider/EH gates remain.
//
// 0059FB60 CSWSDialog::RemovePlayer — thiscall, 1 stack arg, ret 4.
#include "a4d5785d2e-dialog_dependencies_contracts.hpp"

extern "C" int __fastcall kotor_fastcall_0059fb60(void* self_bits, void*, U32 id) {
    auto* self = static_cast<CSWSDialog*>(self_bits);
    CExoLinkedListInternal* list = self->players;
    CExoLinkedListNode* head = list->head;
    CExoLinkedListNode* cursor = head;
    CSWSDialogPlayer* data = (head != nullptr)
                                 ? static_cast<CSWSDialogPlayer*>(head->data)
                                 : nullptr;
    bool first = true;
    while (data != nullptr) {
        if (data->id == id) {
            break;
        }
        first = false;
        data = static_cast<CSWSDialogPlayer*>(
            unresolved_list_next_005e9b90(list, nullptr, &cursor));
    }
    if (data == nullptr) {
        return list->head != nullptr ? 1 : 0;
    }
    if (list->count == 1 || (self->field29_0x8c != 0 && first)) {
        return 0;
    }
    (void)c2da_005e9c10(list, nullptr, cursor);
    if (data->id == self->player_object_id) {
        live_free(data);
        CExoLinkedListNode* const new_head = list->head;
        if (new_head == nullptr) {
            return 1;
        }
        data = static_cast<CSWSDialogPlayer*>(new_head->data);
        if (data == nullptr) {
            return 1;
        }
        self->player_object_id = data->id;
        self->field21_0x78 = data->byte8;
        return 1;
    }
    live_free(data);
    return 1;
}
