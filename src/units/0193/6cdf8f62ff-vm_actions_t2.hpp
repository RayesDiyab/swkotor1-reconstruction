#pragma once
// W7 tier t2 exports (ABI as in vm_actions_t1.hpp).
#include <cstdint>
extern "C" {
int __fastcall recovered_vm_execute_script_00535b70(void* self, void*, int routine, unsigned argc);       // 8 ExecuteScript
int __fastcall recovered_vm_get_run_script_var_0053d020(void* self, void*, int routine, unsigned argc);   // 565 GetRunScriptVar
int __fastcall recovered_vm_assign_command_0052e720(void* self, void*, int routine, unsigned argc);       // 6 AssignCommand
int __fastcall recovered_vm_delay_command_0052fe30(void* self, void*, int routine, unsigned argc);        // 7 DelayCommand
int __fastcall recovered_vm_action_do_command_0052c740(void* self, void*, int routine, unsigned argc);    // 294 ActionDoCommand
unsigned __fastcall recovered_vm_outer_pop_float_005d1020(void* vm, void*, void* out);                   // 005D1020
extern std::int32_t recovered_run_script_var_00832828;                                                   // .bss 00832828
}
