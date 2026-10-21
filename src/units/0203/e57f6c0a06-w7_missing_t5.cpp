// W7 t5 named-trap fallbacks (selected by /alternatename in vm_actions_t5.cpp only when the owner's export is absent).
extern "C" __declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long address, const char* reason);
extern "C" unsigned __fastcall w7_missing_get_use_range_004ee440(void*, void*, unsigned, void*, float*, unsigned) {
  kotor_unreachable_trap(0x004ee440, "CSWSCreature::GetUseRange not linked (W5 t3 export w5_creature_get_use_range_004ee440)");
}
extern "C" void __fastcall w7_missing_party_set_leader_00635480(void*, void*, int) {
  kotor_unreachable_trap(0x00635480, "CSWParty::SetLeader not linked (W4 t1 export kotor_fastcall_00635480)");
}
