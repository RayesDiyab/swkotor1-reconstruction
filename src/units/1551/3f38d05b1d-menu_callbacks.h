#pragma once
#include <cstdint>

extern "C" {
void __fastcall kotor_fastcall_0060f330(void* self, void* unused_edx,
                                     std::uint32_t action, void* creature);
void __fastcall kotor_fastcall_0060f590(void* self, void* unused_edx,
                                     std::uint32_t action, void* creature);
void __fastcall kotor_fastcall_00616200(void* self, void* unused_edx,
                                     std::uint32_t action, void* creature);
void __fastcall kotor_fastcall_00617dd0(void* self, void* unused_edx,
                                     std::uint32_t feat, void* creature);
void __fastcall kotor_fastcall_00617e40(void* self, void* unused_edx,
                                     std::uint32_t item_id, void* creature);
// Integer argument slots retain all original bits; individual fields are
// narrowed by the writer. Position remains an opaque pointer to three raw
// DWORD float representations, preserving write order and NaN payloads.
std::uint32_t __fastcall kotor_fastcall_006776a0(
    void* self, void* unused_edx, std::uint32_t action, std::uint32_t byte_a,
    std::uint32_t byte_b, std::uint32_t byte_c, std::uint32_t flag,
    const void* position, std::uint32_t object_id);
std::uint32_t __fastcall kotor_fastcall_00677bd0(
    void* self, void* unused_edx, std::uint32_t action, std::uint32_t byte_a,
    std::uint32_t byte_b, std::uint32_t object_id, const void* position);
std::uint32_t __fastcall kotor_fastcall_00677de0(
    void* self, void* unused_edx, std::uint32_t first, std::uint32_t second);
}
