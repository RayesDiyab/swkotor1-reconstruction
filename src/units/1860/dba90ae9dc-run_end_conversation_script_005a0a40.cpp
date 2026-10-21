// Authored packet source — reviewed, unbuilt; provider/EH gates remain.
//
// 005A0A40 CSWSDialog::RunEndConversationScript — thiscall, 2 stack args, ret 8.
#include "a4d5785d2e-dialog_dependencies_contracts.hpp"

using namespace dialog_dep;

extern "C" void __fastcall kotor_fastcall_005a0a40(void* self_bits, void*,
                                                    void* object_bits, I32 flag) {
    auto* self = static_cast<CSWSDialog*>(self_bits);
    auto* object = static_cast<CSWSObject*>(object_bits);

    if (self->field18_0x6c == 0 || self->field58_0x10c != 0) {
        (void)kotor_fastcall_0059ed70(self, nullptr, object, &self->script_b);
        return;
    }

    self->field58_0x10c = 1;
    (void)kotor_fastcall_0059ed70(self, nullptr, object, &self->script_a);

    void* const server = load_server();
    void* const array = recovered_object_array_004aed70(server, nullptr);
    CSWSObject* looked_up = nullptr;
    (void)kotor_fastcall_004d8230(array, nullptr, self->owner_object_id, &looked_up);
    if (looked_up == nullptr) {
        return;
    }

    CSWSObject* const sub = static_cast<CSWSObject*>(vcall0(looked_up, 0x10));
    if (sub == nullptr) {
        return;
    }
    auto* const area = static_cast<CSWSArea*>(recovered_object_area_004cb120(sub, nullptr));

    if (flag == 0) {
        const I32 count = static_cast<I32>(area->game_object_count);
        for (I32 i = 0; i < count; ++i) {
            CSWSObject* current = nullptr;
            (void)kotor_fastcall_004d8230(array, nullptr, area->game_objects[i], &current);
            if (current == nullptr) {
                continue;
            }
            void* const creature = vcall0(current, 0x30);
            if (creature == nullptr) {
                void* const placeable = vcall0(current, 0x48);
                if (placeable != nullptr) {
                    kotor_fastcall_00585460(placeable, nullptr, sub);
                }
            } else {
                kotor_fastcall_004ef910(creature, nullptr, sub);
            }
        }
        return;
    }

    void* const creature = vcall0(sub, 0x30);
    if (creature != nullptr) {
        kotor_fastcall_004ef910(creature, nullptr, sub);
        return;
    }
    void* const placeable = vcall0(sub, 0x48);
    if (placeable != nullptr) {
        kotor_fastcall_00585460(placeable, nullptr, sub);
    }
}
