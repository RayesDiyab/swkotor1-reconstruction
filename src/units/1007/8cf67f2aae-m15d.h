#pragma once
// lane-m15-d (L15-D, Milestone 1.5): custom char-gen stat panels -- CSWGuiAbilitiesCharGen, CSWGuiSkillsCharGen,
// CSWGuiFeatsCharGen, the feat chart controls (CSWGuiSkillFlowChart / CSWGuiSkillFlow / CSWGuiFlowSkillStruct),
// CSWGuiSkillInfoBox::SetSkillList and CSWGuiInGameSkillEntry::SetSkill.
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only.
// Consumers (L15-C CSWGuiCustomPanel 006EF2D0 / 006EF360 / 006EF3F0) include this header (header root:
// claude-takeover\lane-m15-d\src) or copy the declarations verbatim. Every symbol below is DEFINED by an L15-D unit
// (see PACKET.json); consumers must not author these addresses.
//
// Conventions (DECISIONS D1/D21/D24, same as lane-m15-0 / -b / -c): every original __thiscall entry is an
// extern "C" __fastcall(Self*, void* unused_edx, <original stack words>) veneer with identical callee cleanup
// (RET N noted per entry); callers pass nullptr for EDX; no callee reads EDX. Object pointers are void* (C linkage:
// the mangled name is @name@<bytes>). Event-callback PMFs are in m15d_gui.h.
#include <cstdint>

extern "C" {
// ------------------------------------------------------------------ panel constructors (callers: CSWGuiCustomPanel)
// The caller allocates with operator new 006FA7E6, frees the block when the ctor throws (its own EH state), then
// stores the parent panel in [panel+68h] AFTER the ctor returns (006EF334 / 006EF3C2 / 006EF452) and AddPanel(p, 3, 1).
// 006F3D60 CSWGuiFeatsCharGen::CSWGuiFeatsCharGen(manager, creature, level_up, bonus_feats) RET 10h, EAX = this.
//          size 1A1Ch (006EF2FC push 1A1Ch); vtable 007598B0. NOTE: the ctor overwrites [this+18h] with
//          CClientExoApp::GetSWGuiManager (006F3F1B..29) and runs InitiateFeats 006F3460 before returning.
void* __fastcall kotor_m15d_feats_construct_006f3d60(void* self, void* unused_edx, void* manager, void* creature,
                                                      std::uint32_t level_up, std::uint32_t bonus_feats) noexcept(false);
// 006F51D0 CSWGuiSkillsCharGen::CSWGuiSkillsCharGen(manager, creature, level_up) RET 0Ch, EAX = this.
//          size 49D0h (006EF38C push 49D0h); vtable 00759990.
void* __fastcall kotor_m15d_skills_construct_006f51d0(void* self, void* unused_edx, void* manager, void* creature,
                                                       std::uint32_t level_up) noexcept(false);
// 006F7600 CSWGuiAbilitiesCharGen::CSWGuiAbilitiesCharGen(manager, creature, level_up) RET 0Ch, EAX = this.
//          size 3DF4h (006EF41C push 3DF4h); vtable 00759C68.
void* __fastcall kotor_m15d_abilities_construct_006f7600(void* self, void* unused_edx, void* manager, void* creature,
                                                          std::uint32_t level_up) noexcept(false);

// ------------------------------------------------------------------ .data colours owned here (values from the image)
// 007A23D8 {0.74, 0.11, 0.0} and 007A23E4 {0.28, 0.92, 0.11}: never written by the program (static .data words),
// read by CSWGuiSkillFlow::SetSkillStatus 006CCFD0 (and other GUI code outside M1.5).
struct KotorM15dColor { unsigned long words[3]; };
extern const KotorM15dColor kotor_m15d_color_007a23d8;
extern const KotorM15dColor kotor_m15d_color_007a23e4;
}
