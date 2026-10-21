// Original0053EF70..0053EFA0: ACTION701, thiscallRET8 transported asfastcall16.
#include <cstdint>
#include <cstring>
extern "C" {
unsigned __fastcall frame_R4_005ed690(void*, void*);
unsigned __fastcall recovered_vm_outer_push_integer_005d1010(void*, void*, unsigned);
}
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace frame_control { extern unsigned message_manager_007a3a00; }
extern "C" int __fastcall recovered_vm_get_is_conversation_active_0053ef70(void*, void*, int, unsigned) {
    void* client;
    std::memcpy(&client, static_cast<unsigned char*>(kotor_menu_window_cells::manager_007a39fc) + 4, sizeof(client));
    const unsigned gui = frame_R4_005ed690(client, nullptr);
    unsigned result;
    std::memcpy(&result, reinterpret_cast<const void*>(static_cast<std::uintptr_t>(gui) + 0xb4u), sizeof(result));
    return recovered_vm_outer_push_integer_005d1010(
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)), nullptr, result) ? 0 : -2000;
}
static_assert(sizeof(void*) == 4, "original x86 ABI");
