#pragma once
// lane-m15-c (L15-C, Milestone 1.5): character-generation frame and flow panels.
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only.
// Consumers (L15-A ClassSelection::OnSelectButton 006DB9B0; L15-D Abilities/Skills/Feats panels) include this
// header (header root: claude-takeover\lane-m15-c\src) or copy the declarations verbatim. Every function below is
// DEFINED by an L15-C unit (see PACKET.json); consumers must not author these addresses.
//
// Conventions (DECISIONS D1/D21/D24, same as lane-m15-0 / lane-m15-b): every original __thiscall entry is an
// extern "C" __fastcall(Self*, void* unused_edx, <original stack words>) veneer with identical callee cleanup
// (RET N noted per entry); callers pass nullptr for EDX; no callee reads EDX. Object pointers are void* (C linkage:
// the mangled name is @name@<bytes>). Event callbacks (PMFs) registered by L15-C ctors are internal to L15-C.
#include <cstdint>

extern "C" {
// ------------------------------------------------------------------ CSWGuiMainCharGen (0x22DC; vtable 007592A8)
// 006EB420 CSWGuiMainCharGen::CSWGuiMainCharGen(CSWGuiManager* manager, CSWCCreature* creature,
//          CSWGuiClassSelection* class_selection) RET 0Ch, EAX = this. Caller allocates 0x22DC with operator new
//          006FA7E6 (006DBA14) and frees it on a throwing ctor (EH state of the caller).
void* __fastcall kotor_m15c_main_chargen_construct_006eb420(void* self, void* unused_edx, void* manager,
                                                            void* creature, void* class_selection) noexcept(false);

// ------------------------------------------------------------------ CSWGuiCustomPanel parent notifications
// Called by the L15-D panels on their CSWGuiCustomPanel parent ([panel+68h]); all thiscall, no stack words (RET).
// 006EF480 CancelledByChild: step button text colour 007A23C0 + flag 40h (callers 006F2C70/006F49D0/006F6DE0).
void __fastcall kotor_m15c_custom_cancelled_by_child_006ef480(void* custom_panel, void* unused_edx) noexcept(false);
// 006EF500 AbilityCreationCompleted: MainCharGen::AbilitiesChanged 006EAC30, SetState(2) (caller 006F6F8C).
void __fastcall kotor_m15c_custom_ability_creation_completed_006ef500(void* custom_panel, void* unused_edx) noexcept(false);
// 006EF520 SkillCreationCompleted: SetState(3) (callers 006F4DC0 / 006F6980).
void __fastcall kotor_m15c_custom_skill_creation_completed_006ef520(void* custom_panel, void* unused_edx) noexcept(false);
// 006EF530 FeatCreationCompleted: SetState(4) (caller 006F44C0).
void __fastcall kotor_m15c_custom_feat_creation_completed_006ef530(void* custom_panel, void* unused_edx) noexcept(false);
}
