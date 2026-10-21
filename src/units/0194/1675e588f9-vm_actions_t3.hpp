#pragma once
// W7 tier t3 exports (ABI as in vm_actions_t1.hpp).
#include <cstdint>
extern "C" {
int __fastcall recovered_vm_get_global_value_00538dc0(void* self, void*, int routine, unsigned argc);              // 578, 580
int __fastcall recovered_vm_set_global_value_00542b60(void* self, void*, int routine, unsigned argc);              // 579, 581
int __fastcall recovered_vm_get_object_by_tag_0053c280(void* self, void*, int routine, unsigned argc);             // 200
int __fastcall recovered_vm_get_pc_speaker_0053c9c0(void* self, void*, int routine, unsigned argc);               // 238
int __fastcall recovered_vm_action_pause_conversation_0052d330(void* self, void*, int routine, unsigned argc);    // 205
int __fastcall recovered_vm_action_resume_conversation_0052d520(void* self, void*, int routine, unsigned argc);   // 206
void __fastcall recovered_global_set_bit_005290c0(void* table, void*, void* bits, int index, int value);
void __fastcall recovered_global_get_boolean_00529110(void* table, void*, void* name, int* out);
void __fastcall recovered_global_get_number_00529240(void* table, void*, void* name, signed char* out);
void __fastcall recovered_global_set_boolean_00529570(void* table, void*, void* name, int value);
void __fastcall recovered_global_set_number_00529680(void* table, void*, void* name, int value);
unsigned __fastcall recovered_module_find_object_by_tag_004c5fe0(void* module, void*, const void* tag, int nth);
}
