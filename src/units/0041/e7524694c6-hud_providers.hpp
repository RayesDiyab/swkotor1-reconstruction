#pragma once
// Lane W4 G15/G19-client: extra runtime1115 providers (exact map symbols) and HUD-fork cross references.
#include "93d5c409bf-w4_frame.hpp"

extern "C" {
// existing runtime1115 providers
void* __fastcall client_get_player_creature_005ed540(void* app, void*);                 // CClientExoApp::GetPlayerCreature
std::uint32_t __fastcall kotor_m15_client_get_player_creature_id_005ed550(void* app, void*); // GetPlayerCreatureId
void* __fastcall kotor_fastcall_005ed8b0(void* app, void*);                              // CClientExoApp::GetSWParty
std::uint32_t __fastcall kotor_fastcall_005edc10(void* app, void*);                      // CClientExoApp::GetPausedByCombat
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void* object, void*);      // CSWCObject::GetServerObject
void* __fastcall recovered_object_area_004cb120(void* server_object, void*);             // CSWSObject::GetArea
int __fastcall kotor_m15b_creature_stats_get_can_use_skill_006477e0(void* stats, void*, std::uint32_t skill); // GetCanUseSkill
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void* app, void*, std::uint32_t id); // GetItemByGameObjectID
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);              // CSWItem::GetBaseItem
std::uint32_t __fastcall gui_load_panel_hit_check_0040b690(void* panel, void*, int x, int y); // CSWGuiPanel::HitCheckMouse
// W1-owned cross-cutting leaf (pending, see integration/hud.json)
std::int32_t __fastcall recovered_party_index_006349c0(void* party, void*, std::uint32_t id);   // CSWParty::GetIndex
// HUD-fork bodies (defined in src/hud)
void __fastcall kotor_fastcall_00686ba0(void* self, void*, float delta);                 // CSWGuiMainInterface::Update
void __fastcall kotor_fastcall_006858e0(void* self, void*, float delta);                 // CSWGuiMainInterfaceAction::Update
void __fastcall kotor_fastcall_00686a20(void* self, void*, float delta);                 // CSWGuiMainInterfaceStatus::Update
std::uint32_t __fastcall kotor_fastcall_00686b70(void* self, void*, int x, int y);       // CSWGuiMainInterface::HitCheckMouse
std::uint32_t __fastcall kotor_fastcall_00685f90(void* self, void*, int x, int y);       // CSWGuiTargetActionMenu::HitCheckMouse
void __fastcall kotor_fastcall_006345e0(void* member, void*, std::uint32_t message);     // CSWPartyMemberData::SetCombatMessage
void __fastcall kotor_fastcall_0062b110(void* in_game, void*, std::uint32_t message);    // CGuiInGame::SetCombatMessage
int __fastcall kotor_fastcall_00610ac0(void* creature, void*);                           // CSWCCreature::IsStealthCapable
void __fastcall kotor_fastcall_0062eeb0(void* in_game, void*, std::uint8_t kind, std::uint32_t value); // CGuiInGame::UpdateStatus
}
namespace lane_config_required {
void* __fastcall GetPartyTable_004aee70(void* server, void*);                            // CServerExoApp::GetPartyTable
}
