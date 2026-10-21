#pragma once
#include <cstdint>
#include "ba3b8e7083-menu_action_record.h"

namespace menu_resources { struct Ref; }

extern "C" {
void __fastcall kotor_fastcall_006193a0(
    void* self, void* unused_edx, void* output_list, void* item,
    std::uint32_t equipped_mode, std::uint32_t filter_mask);
void __fastcall kotor_fastcall_006198e0(
    void* self, void* unused_edx, void* output_list, void* creature);
void __fastcall kotor_fastcall_00619950(
    void* self, void* unused_edx, void* output_list, void* creature);
std::uint32_t __fastcall kotor_fastcall_00616140(
    void* self, void* unused_edx, std::uint32_t feat1,
    std::uint32_t feat2, std::uint32_t feat3);
std::uint32_t __fastcall kotor_fastcall_00616520(
    void* self, void* unused_edx, void* item, std::uint32_t unused_word,
    std::uint32_t flags);
void __fastcall kotor_fastcall_006196a0(
    void* self, void* unused_edx, void* output_list, void* item, void* creature);
menu_resources::Ref* __fastcall kotor_fastcall_005556b0(
    void* self, void* unused_edx, menu_resources::Ref* output);
std::uint32_t __fastcall kotor_fastcall_00555a60(
    void* self, void* unused_edx, std::uint8_t property_index);
std::uint32_t __fastcall kotor_fastcall_0059b6a0(
    void* self, void* unused_edx, std::uint8_t index);
}
