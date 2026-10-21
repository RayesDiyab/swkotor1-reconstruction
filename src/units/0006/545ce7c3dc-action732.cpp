// Original 005462D0..00546337: thiscall RET8, transported as fastcall + dummy EDX.
// W6 ACTION732 only. Existing W5/VM/lookup implementations are called unchanged.
#include <cstdint>
#include <cstring>
extern "C" {
unsigned __fastcall recovered_vm_outer_pop_object_005d10c0(void*, void*, void*);
unsigned __fastcall recovered_vm_outer_push_integer_005d1010(void*, void*, unsigned);
void* __fastcall kotor_fastcall_004ae770(void*, void*, unsigned);
unsigned __fastcall kotor_fastcall_005b4880(void*, void*);
}
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace frame_control { extern unsigned message_manager_007a3a00; }
extern "C" int __fastcall recovered_vm_get_is_debilitated_005462d0(void*, void*, int, unsigned) {
    unsigned object_id = 0x7f000000u;
    if (!recovered_vm_outer_pop_object_005d10c0(
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)), nullptr, &object_id)) return -2001;
    void* server;
    std::memcpy(&server, static_cast<unsigned char*>(kotor_menu_window_cells::manager_007a39fc) + 8, sizeof(server));
    void* creature = kotor_fastcall_004ae770(server, nullptr, object_id);
    const unsigned result = creature ? kotor_fastcall_005b4880(creature, nullptr) : 0u;
    // Original re-reads the outer VM singleton after lookup/IsDebilitated.
    return recovered_vm_outer_push_integer_005d1010(
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)), nullptr, result) ? 0 : -2000;
}
static_assert(sizeof(void*) == 4, "original x86 ABI");
