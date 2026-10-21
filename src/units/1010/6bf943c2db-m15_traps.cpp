// lane-m15-f (Milestone 1.5, D134): D83 trap bodies for the character-generation boundary and gates.
// Static evidence only (original swkotor.exe GOG K1, image base 0x400000; nothing executed). Reachability
// evidence per address: lane-m15-f/TRAPS-DELTA.json. Traps are milestone-2+ debt, never reconstructed behaviour:
// each body logs its ORIGINAL function address through the shared D83 trap (m48/t/include) and terminates.
//
// 1. Play boundary (D134: "Play button remains trap boundary"):
//    006EB320 CSWGuiMainCharGen::BeginPlayGame = push esi; mov esi,ecx; call 006EA830 RemoveSubPanels;
//    mov ecx,[esi+68h]; pop esi; jmp 006DBDF0 CSWGuiClassSelection::BeginPlayGame (-> CreateServer 006DBE93,
//    SaveCharGenCreature 006DBE9B, StartConnectToSession 006DBEDE, module load screen). Only callers:
//    CSWGuiCustomPanel::OnSelectPlayButton 006EF237 and CSWGuiQuickPanel::OnSelectPlayButton 006EFD77 (event
//    callbacks registered at 006EFA2B/006EFA9D and 006F062D, i.e. activation of a Play button only).
// 2. Gameplay-only tutorial callbacks: CSWGuiTutorialBox::Perform* are stored only by SetTutorialReason 006AA900
//    for reasons {00..04h, 1Dh..22h, 27h}; the char-gen panels pass reasons 10h/11h/12h (006F6DC9 / 006F49B9 /
//    006F2F55), which the index table 006AAA9C maps to entry 0Bh = MessageBoxUnpause 006AA230.
// 3. In-game level-up parent: CSWGuiLevelUpPanel callbacks are reached from the shared Abilities/Skills/Feats
//    panels only when their level-up flag is set (Abilities +3DF0h, Skills +49C8h bit0, Feats +19F4h bit0), which
//    their ctors copy from a ctor argument that CSWGuiCustomPanel passes as 0 (006EF31F/006EF321, 006EF3AF,
//    006EF43F) and only CSWGuiLevelUpPanel passes as 1 (006EE4C6 etc.; created by CSWGuiLevelUpCharGen 006E987D
//    <- CSWGuiInGameCharacter::ShowLevelUpGUI 006B0C40, in-game only).
#include "3f6e6cc5bd-m15_traps.h"
#include "20c6d806d8-m15_l0_gui.h"   // kotor_m15_l0::TutorialBoxHandlers (L15-0 PMF contract; header root lane-m15-0/src)
#include "c272944ff0-kotor_unreachable_trap.h"

namespace kotor_m15_l0 {
// 006AA260 PerformCombatFeat (RET 4): GetCreatureByGameObjectID([this+984h]) -> CSWCCreature::PerformCombatFeat 006AA2B2.
// PerformCombatFeat_006aa260: faithful body in srcw5/actions/claude_stale_bodies_client.cpp (stale-stubs G5).
// 006AA2C0 PerformForcePower (RET 4): -> CSWCCreature::PerformCastSpell 006AA341.
// PerformForcePower_006aa2c0: faithful body in srcw5/actions/claude_stale_bodies_client.cpp (stale-stubs G5).
// 006AA350 PerformUseGrenade (RET 4): -> CSWCCreature::PerformUseHostileItem 006AA3B6.
// PerformUseGrenade_006aa350: faithful body in srcw5/actions/claude_stale_bodies_client.cpp (stale-stubs G5).
// 006AA3D0 PerformSetMine (RET 4): -> CSWCCreature::PerformUseItem 006AA429.
// PerformSetMine_006aa3d0: faithful body in srcw5/actions/claude_stale_bodies_client.cpp (stale-stubs G5).
// 006AA440 PerformPlayerFlourishWeapons (RET 4).
// PerformPlayerFlourishWeapons_006aa440: faithful body in srcw5/actions/claude_stale_bodies_client.cpp (stale-stubs G5).
// 006AA480 PerformBash (RET 4 at 006AA4F4 / 006AA514).
// PerformBash_006aa480: faithful body in srcw5/actions/claude_stale_bodies_client.cpp (stale-stubs G5).
// 006AA520 PerformAttack (RET 4): -> CSWCCreature::ActionMenuAttack 006AA583.
// PerformAttack_006aa520: faithful body in srcw5/actions/claude_stale_bodies_client.cpp (stale-stubs G5).
// 006AA590 PerformNonCombatXButton (RET 4).
// PerformNonCombatXButton_006aa590: faithful body in srcw5/actions/claude_stale_bodies_client.cpp (stale-stubs G5).
// 006AA5D0 PerformCombatYButton (RET 4).
// PerformCombatYButton_006aa5d0: faithful body in srcw5/actions/claude_stale_bodies_client.cpp (stale-stubs G5).
// 006AA620 PerformReturnToBase (RET 4).
// PerformReturnToBase_006aa620: faithful body in srcw4/src/gui/claude_closeC_gui.cpp (lane closeC); trap removed.
}

// CSWGuiLevelUpPanel parent notifications 006EE580 CancelledByChild / 006EE5D0 SelectionCompleted /
// 006EE5B0 AbilitySelectionCompleted / 006EE0D0 NewAbilityScores: faithful bodies in
// srcw4/src/gui/claude_gui_levelup_panel.cpp (worker "levelup"); trap definitions removed here.
