// Original shared ACTION301/302/303 body00544C70..00544D66: thiscall RET8.
#include <cstdint>
#include <cstring>
extern "C" {
void* __cdecl live_operator_new(unsigned);
void __cdecl live_free(void*);
void* __fastcall recovered_talent_construct_004ca9e0(void*, void*);
unsigned __fastcall recovered_vm_outer_pop_integer_005d1000(void*, void*, void*);
unsigned __fastcall recovered_vm_outer_push_engine_structure_005d10b0(void*, void*, int, unsigned);
}
namespace frame_control { extern unsigned message_manager_007a3a00; }
extern "C" int __fastcall recovered_vm_talent_00544c70(void*, void*, int routine, unsigned) {
    void* allocation = live_operator_new(0x18);
    void* talent = allocation ? recovered_talent_construct_004ca9e0(allocation, nullptr) : nullptr;
    unsigned id;
    // Original leaks the constructed temporary on either pop or push failure.
    if (!recovered_vm_outer_pop_integer_005d1000(
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)), nullptr, &id)) return -2001;
    if (routine == 301 || routine == 302 || routine == 303) {
        const unsigned type = static_cast<unsigned>(routine - 301);
        std::memcpy(talent, &type, sizeof(type));
    }
    std::memcpy(static_cast<unsigned char*>(talent) + 4, &id, sizeof(id));
    if (!recovered_vm_outer_push_engine_structure_005d10b0(
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)), nullptr, 3,
            static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(talent)))) return -2000;
    // Original004015A0 destructor is empty; the pushed value owns a separate kind3 copy.
    live_free(talent);
    return 0;
}
static_assert(sizeof(void*) == 4, "original x86 ABI");
