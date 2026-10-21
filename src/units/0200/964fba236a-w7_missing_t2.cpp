// W7 t2 named-trap fallbacks for imports owned by other lanes (selected by /alternatename in vm_actions_t2.cpp only
// when the owner's export is absent from the link). They never return; the address is the original function.
extern "C" __declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long address, const char* reason);
extern "C" int __fastcall w7_missing_stack_pop_command_005d10e0(void*, void*, void*) {
  kotor_unreachable_trap(0x005d10e0, "CVirtualMachine::StackPopCommand not linked (W6 tiers/t2 export @recovered_vm_stack_pop_command_005d10e0@12)");
}
extern "C" int __fastcall w7_missing_add_do_command_action_0057cb10(void*, void*, void*) {
  kotor_unreachable_trap(0x0057cb10, "CSWSObject::AddDoCommandAction not linked (server action queue owner; ends in CSWSObject::AddAction 004CEA20)");
}
