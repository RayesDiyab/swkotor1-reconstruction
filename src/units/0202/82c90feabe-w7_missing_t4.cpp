// W7 t4 named-trap fallback (selected by /alternatename in vm_actions_t4.cpp only when W5's export is absent).
extern "C" __declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long address, const char* reason);
extern "C" void __fastcall w7_missing_set_lock_orientation_004f34a0(void*, void*, unsigned, unsigned) {
  kotor_unreachable_trap(0x004f34a0, "CSWSCreature::SetLockOrientationToObject not linked (W5 t3 export w5_creature_set_lock_orientation_to_object_004f34a0)");
}
