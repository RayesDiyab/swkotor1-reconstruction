// Original00536900..00536942: ACTION489, thiscallRET8 transported asfastcall16.
#include <cstdint>
#include <cstring>
extern "C" {
void* __fastcall kotor_fastcall_004ae770(void*, void*, unsigned);
unsigned __fastcall recovered_vm_outer_push_object_005d10d0(void*, void*, unsigned);
}
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace frame_control { extern unsigned message_manager_007a3a00; }
extern "C" int __fastcall recovered_vm_get_attempted_movement_target_00536900(void* self, void*, int, unsigned) {
    void* manager = kotor_menu_window_cells::manager_007a39fc;
    unsigned object_id;
    std::memcpy(&object_id, static_cast<unsigned char*>(self) + 8, sizeof(object_id));
    void* server;
    std::memcpy(&server, static_cast<unsigned char*>(manager) + 8, sizeof(server));
    void* creature = kotor_fastcall_004ae770(server, nullptr, object_id);
    unsigned result = 0x7f000000u;
    if (creature) std::memcpy(&result, static_cast<unsigned char*>(creature) + 0x508, sizeof(result));
    return recovered_vm_outer_push_object_005d10d0(
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)), nullptr, result) ? 0 : -2000;
}
static_assert(sizeof(void*) == 4, "original x86 ABI");
