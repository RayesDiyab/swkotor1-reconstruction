// W7 t6 named-trap fallbacks (selected by /alternatename in vm_actions_t6.cpp only when the owner's export is absent).
#include <cstdint>
extern "C" __declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long address, const char* reason);
extern "C" {
void __fastcall w7_missing_reset_drive_acceleration_005edba0(void*, void*) {
  kotor_unreachable_trap(0x005edba0, "CClientExoApp::ResetDriveAcceleration not linked (W4 export kotor_fastcall_005edba0)");
}
void __fastcall w7_missing_update_status_0062eeb0(void*, void*, std::uint8_t, std::uint32_t) {
  kotor_unreachable_trap(0x0062eeb0, "CGuiInGame::UpdateStatus not linked (W4 t1 export kotor_fastcall_0062eeb0)");
}
void __fastcall w7_missing_clear_all_actions_004ccd80(void*, void*, unsigned) {
  kotor_unreachable_trap(0x004ccd80, "CSWSObject::ClearAllActions not linked (W5 export w5_object_clear_all_actions_004ccd80)");
}
void __fastcall w7_missing_client_clear_all_actions_0063d470(void*, void*) {
  kotor_unreachable_trap(0x0063d470, "CSWCObject::ClearAllActions not linked (W4 export kotor_fastcall_0063d470)");
}
void __fastcall w7_missing_set_global_dialog_state_0062ec60(void*, void*, unsigned) {
  kotor_unreachable_trap(0x0062ec60, "CGuiInGame::SetGlobalDialogState not linked (W4 export kotor_fastcall_0062ec60)");
}
void __fastcall w7_missing_add_to_dialog_list_00632300(void*, void*, void*) {
  kotor_unreachable_trap(0x00632300, "CGuiInGame::AddToDialogNonDependantList not linked (W4 export kotor_fastcall_00632300)");
}
unsigned __fastcall w7_missing_can_use_skill_005af880(void*, void*, unsigned) {
  kotor_unreachable_trap(0x005af880, "CSWSCreatureStats::GetCanUseSkill not linked (W2 t6 export recovered_stats_can_use_skill_005af880)");
}
void __fastcall w7_missing_distribute_experience_005653a0(void*, void*, unsigned, unsigned) {
  kotor_unreachable_trap(0x005653a0, "CSWPartyTable::DistributeExperience not linked (W2 t6 export recovered_party_distribute_experience_005653a0)");
}
}
