#pragma once
#include <cstdint>
#include <cstddef>

// Bounded views of actual0963 server methods, shared by this menu cohort.
namespace menu_server {
struct Slots {
    const void* slots000_02c[12];
    void* (__fastcall* as_creature_030)(void*, void*);
    const void* slots034_090[24];
    std::uint32_t (__fastcall* dead_094)(void*, void*);
    std::uint16_t (__fastcall* max_hp_098)(void*, void*, std::uint32_t);
    std::uint32_t (__fastcall* current_hp_09c)(void*, void*, std::uint32_t);
};
static_assert(offsetof(Slots, as_creature_030) == 0x30, "server creature cast");
static_assert(offsetof(Slots, dead_094) == 0x94, "server dead");
static_assert(offsetof(Slots, max_hp_098) == 0x98, "server max hit points");
static_assert(offsetof(Slots, current_hp_09c) == 0x9c, "server current hit points");
}

// Original x86 thiscall entries use ECX and callee cleanup. EDX is unused.
// The U8 target classification is consumed through AL by 00689410.
extern "C" {
void __fastcall kotor_fastcall_00689d80(void* self, void* unused_edx);
void __fastcall kotor_fastcall_00619db0(void* self, void* unused_edx,
                                     std::uint32_t category, void* output_array);
void __fastcall kotor_fastcall_00685960(void* self, void* unused_edx,
                                     void* icon_ref, void* action,
                                     std::uint32_t dimmed, std::uint32_t unused);
void __fastcall kotor_fastcall_00689280(void* self, void* unused_edx,
                                     std::uint32_t empty, std::uint32_t multiple);
void __fastcall kotor_fastcall_00689410(void* self, void* unused_edx,
                                     void* creature, std::uint32_t disabled,
                                     void* target);

void __fastcall kotor_fastcall_00616230(void* self, void* unused_edx,
                                     void* output_array);
void __fastcall kotor_fastcall_006197d0(void* self, void* unused_edx,
                                     void* output_array, std::uint32_t mask);
std::uint8_t __fastcall kotor_fastcall_0060fc00(void* self, void* unused_edx,
                                             void* target);
void __fastcall kotor_fastcall_00619c20(void* self, void* unused_edx,
                                     void* target, std::uint32_t category,
                                     void* output_array);
void __fastcall kotor_fastcall_00685cb0(void* self, void* unused_edx, void* target);
void __fastcall kotor_fastcall_00686090(void* self, void* unused_edx,
                                     void* creature, void* target);
}
