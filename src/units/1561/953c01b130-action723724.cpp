// Originals 0053AED0..0053AF36 and 0053ACD0..0053AD36: thiscall RET8, fastcall transport.
// ACTION723 GetLastForcePowerUsed and724 GetLastCombatFeatUsed. Existing real providers reused.
#include <cstdint>
#include <cstring>
extern "C" {
unsigned __fastcall recovered_vm_outer_pop_object_005d10c0(void*, void*, void*);
unsigned __fastcall recovered_vm_outer_push_integer_005d1010(void*, void*, unsigned);
void* __fastcall kotor_fastcall_004ae770(void*, void*, unsigned);
}
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace frame_control { extern unsigned message_manager_007a3a00; }
extern "C" int __fastcall recovered_vm_get_last_force_power_used_0053aed0(void*, void*, int, unsigned) {
    unsigned object_id = 0x7f000000u;
    if (!recovered_vm_outer_pop_object_005d10c0(
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)), nullptr, &object_id)) return -2001;
    void* server;
    std::memcpy(&server, static_cast<unsigned char*>(kotor_menu_window_cells::manager_007a39fc) + 8, sizeof(server));
    void* creature = kotor_fastcall_004ae770(server, nullptr, object_id);
    unsigned result = 0;
    if (creature) std::memcpy(&result, static_cast<unsigned char*>(creature) + 0x54c, sizeof(result));
    // The original re-reads the outer VM singleton after server lookup.
    return recovered_vm_outer_push_integer_005d1010(
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)), nullptr, result) ? 0 : -2000;
}
extern "C" int __fastcall recovered_vm_get_last_combat_feat_used_0053acd0(void*, void*, int, unsigned) {
    unsigned object_id = 0x7f000000u;
    if (!recovered_vm_outer_pop_object_005d10c0(
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)), nullptr, &object_id)) return -2001;
    void* server;
    std::memcpy(&server, static_cast<unsigned char*>(kotor_menu_window_cells::manager_007a39fc) + 8, sizeof(server));
    void* creature = kotor_fastcall_004ae770(server, nullptr, object_id);
    unsigned result = 0;
    if (creature) std::memcpy(&result, static_cast<unsigned char*>(creature) + 0x554, sizeof(result));
    // The original re-reads the outer VM singleton after server lookup.
    return recovered_vm_outer_push_integer_005d1010(
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)), nullptr, result) ? 0 : -2000;
}
static_assert(sizeof(void*) == 4, "original x86 ABI");
