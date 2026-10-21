// Original0052F4A0..0052F4E9: ACTION9 ClearAllActions, thiscallRET8.
#include <cstring>
extern "C" {
void* __fastcall recovered_object_array_004aed70(void*, void*);
unsigned char __fastcall kotor_fastcall_004d8230(void*, void*, unsigned, void**);
void __fastcall w5_object_clear_all_actions_004ccd80(void*, void*, unsigned);
}
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" int __fastcall recovered_vm_clear_all_actions_0052f4a0(void* self, void*, int, unsigned) {
    unsigned enabled;
    std::memcpy(&enabled, static_cast<unsigned char*>(self) + 4, sizeof(enabled));
    void* object = nullptr;
    if (enabled != 0) {
        void* manager = kotor_menu_window_cells::manager_007a39fc;
        unsigned object_id;
        std::memcpy(&object_id, static_cast<unsigned char*>(self) + 8, sizeof(object_id));
        void* server;
        std::memcpy(&server, static_cast<unsigned char*>(manager) + 8, sizeof(server));
        void* array = recovered_object_array_004aed70(server, nullptr);
        // Original74666C byte is0; original calls ClearAllActions even if successful lookup wrote null.
        if (kotor_fastcall_004d8230(array, nullptr, object_id, &object) == 0)
            w5_object_clear_all_actions_004ccd80(object, nullptr, 1);
    }
    return 0;
}
static_assert(sizeof(void*) == 4, "original x86 ABI");
