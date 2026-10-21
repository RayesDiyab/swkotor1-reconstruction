#pragma once
// lane-m15-f (Milestone 1.5): consumer declarations for the D83 trap bodies in src/m15_traps.cpp.
// Single owner of these 15 original addresses: lane-m15-f. Consumers (L15-0 SetTutorialReason 006AA900,
// L15-C Quick/Custom panels, L15-D Abilities/Skills/Feats panels) include this header and call/register
// these symbols; they must not author bodies for these addresses. Evidence: lane-m15-f/TRAPS-DELTA.json.
//
// Conventions (D1/D47/D107, same as m1/shared/m1_shared.h):
//  * __thiscall entries reached by a direct call/jmp are extern "C" __fastcall(Self*, void* unused_edx, ...)
//    veneers with the original stack arguments and callee cleanup (D1);
//  * the CSWGuiMessageBox callbacks (Perform*) follow L15-0's TutorialBoxHandlers contract (see below).
#include <cstdint>

// The ten CSWGuiTutorialBox::Perform* trap bodies (006AA260..006AA620) are defined by this lane (src/m15_traps.cpp)
// as members of kotor_m15_l0::TutorialBoxHandlers, the PMF contract declared by L15-0 in lane-m15-0/src/m15_l0_gui.h
// (SetTutorialReason 006AA900 registers them via kotor_m15_l0::event_func). Single owner of the bodies: lane-m15-f.

extern "C" {
// 006EB320 CSWGuiMainCharGen::BeginPlayGame (thiscall, RET; ends in JMP 006DBDF0). Milestone 1.5 Play boundary.
// Callers: CSWGuiCustomPanel::OnSelectPlayButton 006EF237, CSWGuiQuickPanel::OnSelectPlayButton 006EFD77.
void __fastcall kotor_fastcall_006eb320(void* main_chargen, void* unused_edx);

// CSWGuiLevelUpPanel parent notifications (in-game level-up parent of the shared Abilities/Skills/Feats panels).
// 006EE580 CancelledByChild (RET): jmp'd from 006F2CA9 / 006F4A72 / 006F6E1A (and PowersLevelUp 006F1120).
void __fastcall kotor_fastcall_006ee580(void* level_up_panel, void* unused_edx);
// 006EE5D0 SelectionCompleted (RET): 006F464D / 006F4E0D / 006F6A04 (and PowersLevelUp 006F11FC).
void __fastcall kotor_fastcall_006ee5d0(void* level_up_panel, void* unused_edx);
// 006EE5B0 AbilitySelectionCompleted (RET): jmp'd from 006F6F87.
void __fastcall kotor_fastcall_006ee5b0(void* level_up_panel, void* unused_edx);
// 006EE0D0 NewAbilityScores (thiscall, 7 stack words; MOV ECX,[ECX+68h]; JMP 006E8500 which RETs 1Ch): 006F6EB1.
void __fastcall kotor_fastcall_006ee0d0(void* level_up_panel, void* unused_edx,
    std::uint32_t a1, std::uint32_t a2, std::uint32_t a3, std::uint32_t a4,
    std::uint32_t a5, std::uint32_t a6, std::uint32_t a7);
}
