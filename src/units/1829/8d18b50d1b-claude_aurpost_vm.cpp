// Lane aurpost: CSWVirtualMachineCommands::ExecuteCommandAurPostString 005CB350 (routine 582 AurPostString),
// reconstructed from the original swkotor.exe (read-only).
//
// Registration: the original InitializeCommands 0054C960 ends with `jmp 005CD2C0` (a separately placed tail chunk);
// 005CD2C3 `mov dword ptr [eax+0x918], 0x005CB350` stores this handler in slot 582. It is NOT a null slot.
//
// Original body (thiscall(int routine, int argc), RET 8, /EH frame for the local CExoString):
//   005CB368  CExoString::CExoString 005B3190 (str at [esp+0C])
//   005CB37B  StackPopString(&str)   [007A3A00] 005D1080   -> 0: ~str, return -2001
//   005CB38D  StackPopInteger(&x)    005D1000 ([esp+00])   -> 0: ~str, return -2001
//   005CB3A1  StackPopInteger(&y)    005D1000 ([esp+04])   -> 0: ~str, return -2001
//   005CB3DC  StackPopFloat(&life)   005D1020 ([esp+08])   -> 0: ~str, return -2001
//   005CB417  CExoString::CStr 005E5670 (result discarded: the retail build compiled the client post out)
//   005CB428  ~CExoString 005E5C20; return 0
// So the shipping AurPostString pops its four arguments and has no visible effect; that is the faithful behaviour.
// Quirk kept: x, y and life are popped into locals that are never read.
#include "061872cb05-claude_vm_actions_common.hpp"

namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* string, void*); }   // CExoString() [0603]

extern "C" int __fastcall kotor_fastcall_005cb350(void* self, void*, int routine, unsigned argc) {
    (void)self; (void)routine; (void)argc;
    vmx::ExoString str;
    kotor_movie::string_element_construct_005b3190(&str, nullptr);          // 005CB36C
    I x, y; float life;
    if (!recovered_vm_outer_pop_string_005d1080(vmx::vm(), nullptr, &str) ||
        !recovered_vm_outer_pop_integer_005d1000(vmx::vm(), nullptr, &x) ||
        !recovered_vm_outer_pop_integer_005d1000(vmx::vm(), nullptr, &y)) {
        vmx::exo_destroy(&str);                                              // 005CB3C1
        return vmx::k_pop_failed;                                            // 0xFFFFF82F
    }
    if (!recovered_vm_outer_pop_float_005d1020(vmx::vm(), nullptr, &life)) {
        vmx::exo_destroy(&str);                                              // 005CB3FC
        return vmx::k_pop_failed;
    }
    (void)WindowCStr005e5670(&str, nullptr);                                 // 005CB417 (result unused in original)
    vmx::exo_destroy(&str);                                                  // 005CB428
    return 0;
}
