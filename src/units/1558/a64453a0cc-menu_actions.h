#pragma once
#include <cstdint>

extern "C" {
void __fastcall kotor_fastcall_006191f0(void* self, void* unused_edx,
                                     void* target, void* output_list);
void __fastcall kotor_fastcall_00619b10(void* self, void* unused_edx,
                                     void* acting_creature, std::uint32_t category,
                                     void* output_list);
void __fastcall kotor_fastcall_006837d0(void* self, void* unused_edx,
                                     void* acting_creature, std::uint32_t category,
                                     void* output_list);
void __fastcall kotor_fastcall_00684410(void* self, void* unused_edx,
                                     void* acting_creature, std::uint32_t category,
                                     void* output_list);
void __fastcall kotor_fastcall_00691f00(void* self, void* unused_edx,
                                     void* acting_creature, std::uint32_t category,
                                     void* output_list);
void __fastcall kotor_fastcall_00618c20(void* self, void* unused_edx,
                                     void* output_list, std::uint32_t kind,
                                     std::uint32_t target_filter,
                                     std::uint32_t equipment_mask);
void __fastcall kotor_fastcall_00682dd0(void* self, void* unused_edx,
                                     std::uint32_t action, void* creature);
void __fastcall kotor_fastcall_00683e50(void* self, void* unused_edx,
                                     std::uint32_t action, void* creature);
}
