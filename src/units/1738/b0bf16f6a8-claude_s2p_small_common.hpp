#pragma once
// Lane worker "s2p-small": shared declarations for the CSWCMessage::HandleServerToPlayer* client handlers routed from
// the client dispatcher 0066A640 (CSWCMessage::HandleServerToPlayerMessage). Transcribed from the original
// swkotor.exe (read-only). Conventions as claude_dialogue_common.hpp:
//   * original thiscall members -> extern "C" __fastcall(self, unused_edx, stack args...) decorated @name@(8+4*args);
//   * every provider declared here is a real body in the c19 candidate map (none appears in the c19 trap census)
//     or a function defined in one of the claude_s2p_small*.cpp files;
//   * game heap only (live_operator_new / live_free); no original address is dereferenced: globals are bound by their
//     rebuilt symbol, .rdata constants are written as their proven literal values.
#include "6bd5403233-claude_dialogue_common.hpp"

namespace s2p {
using namespace dlg;
// CExoString temporary written by a by-value CExoString return (constructed by the callee, destroyed by the caller).
struct StrTemp { U a, b; };
// CSWCAmbientSound (area +1DCh) / CExoStreamingSoundSource layout used below.
}  // namespace s2p

namespace table29_required {   // 0674-sound-path.obj / 0666-sound-donor-streaming-state.obj (real bodies)
menu_resources::Ref* __fastcall gui_file_playing_005d6230(void*, void*, menu_resources::Ref*);  // GetFilePlaying RET 4
unsigned __fastcall gui_playing_005d5af0(void*, void*);                                         // IsPlaying RET 0
void __fastcall gui_volume_005d5b50(void*, void*, unsigned);                                    // SetVolume RET 4
}
namespace menu_tables_m41 { void __fastcall SetCameraMode_0061b6f0(void*, void*, unsigned); }   // CClientOptions::SetCameraMode
namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth {   // 0098-viewport-list-growth.obj: CExoArrayList<4-byte>::Allocate {data,size,capacity} RET 4
void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned);
}
namespace s2p {
inline void list_allocate(void* list, dlg::U capacity) {
    menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr, capacity);
}
}  // namespace s2p

extern "C" {
// w2_message_native.obj (CSWMessage / CSWCMessage readers)
dlg::U __fastcall kotor_fastcall_004d6230(void* msg, void*, dlg::U bits);                 // MessageReadOverflow RET 4
dlg::U __fastcall kotor_fastcall_004d6240(void* msg, void*, dlg::U bits);                 // MessageReadUnderflow RET 4
dlg::U8 __fastcall kotor_fastcall_004d6720(void* msg, void*, dlg::U bits);                // ReadBYTE RET 4
dlg::U16 __fastcall kotor_fastcall_004d6750(void* msg, void*, dlg::U bits);               // ReadWORD RET 4
dlg::U __fastcall kotor_fastcall_004d6780(void* msg, void*, dlg::U bits);                 // ReadDWORD RET 4
dlg::U __fastcall kotor_fastcall_004d66f0(void* msg, void*);                              // ReadBOOL RET 0
float __fastcall kotor_fastcall_004d67b0(void* msg, void*, float scale, dlg::U bits);     // ReadFLOAT RET 8 (ST0)
void* __fastcall kotor_fastcall_004d67e0(void* msg, void*, void* result, dlg::U bits);    // ReadCExoString RET 8
void* __fastcall kotor_fastcall_004d6200(void* msg, void*, dlg::U size);                  // ReadVOIDPtr RET 4
dlg::U __fastcall kotor_fastcall_00692630(void* msg, void*);                              // ReadOBJECTIDClient RET 0
void* __fastcall kotor_fastcall_00692660(void* msg, void*, void* result, void* sound, unsigned* length_out); // ReadCExoLocStringClient RET 0Ch
// client app
void* __fastcall kotor_fastcall_005ed580(void* client, void*, dlg::U id);                 // CClientExoApp::GetGameObject
dlg::U __fastcall frame_R4_005ed5b0(void* client, void*, dlg::U id);                      // GetCreatureByGameObjectID
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void* client, void*, dlg::U id); // GetItemByGameObjectID
void* __fastcall recovered_client_get_sound_object_005ed620(void* client, void*, dlg::U id); // GetSoundObjectByGameObjectID
void* __fastcall kotor_fastcall_005ed700(void* client, void*);                            // GetClientOptions
void* __fastcall kotor_fastcall_005ed570(void* client, void*);                            // GetGameObjectArray
// client objects
void __fastcall recovered_gui_in_game_set_level_up_mode_00628650(void* gui, void*, dlg::U mode); // CGuiInGame::SetLevelUpMode
void __fastcall recovered_client_sound_object_play_0066b050(void* sound, void*);         // CSWCSoundObject::Play
void __fastcall recovered_client_sound_object_stop_0066b130(void* sound, void*);         // CSWCSoundObject::Stop
void __fastcall kotor_m15_sound_source_set_position_005d59e0(void* source, void*, const void* v, dlg::U flag); // CExoSoundSource::SetPosition
void __fastcall kotor_m15_stream_source_set_position_005d62c0(void* source, void*, const void* v); // CExoStreamingSoundSource::SetPosition
dlg::U __fastcall recovered_client_creature_play_sound_set_sound_00611470(void* creature, void*, dlg::U16 entry, dlg::U flag); // CSWCCreature::PlaySoundSetSound RET 8
void* __fastcall recovered_client_item_get_active_property_006466a0(void* item, void*, dlg::I index); // CSWCItem::GetActiveProperty
void __fastcall recovered_client_item_set_property_uses_left_006469b0(void* item, void*, dlg::I index, dlg::U16 uses); // SetItemPropertyUsesLeft
void* __fastcall recovered_client_map_pin_construct_00692540(void* pin, void*);          // CSWCMapPin::CSWCMapPin
int __cdecl format_006fadb0(char* out, const char* format, ...);                           // sprintf (_format_006fadb0)

// ---- exports of the claude_s2p_small*.cpp files (thiscall(CSWCMessage*, BYTE minor) RET 4 handlers) ----
dlg::U __fastcall kotor_fastcall_006527e0(void* self, void*, dlg::U minor);   // LevelUp
dlg::U __fastcall kotor_fastcall_006637c0(void* self, void*, dlg::U minor);   // GuiQuickbar
dlg::U __fastcall kotor_fastcall_006514c0(void* self, void*, dlg::U minor);   // CombatRound
dlg::U __fastcall kotor_fastcall_00656ba0(void* self, void*, dlg::U minor);   // Sound
dlg::U __fastcall kotor_fastcall_00665510(void* self, void*, dlg::U minor);   // Ambient
dlg::U __fastcall kotor_fastcall_0064b8e0(void* self, void*, dlg::U minor);   // Camera
dlg::U __fastcall kotor_fastcall_00654230(void* self, void*, dlg::U minor);   // PopUpGUIPanel
dlg::U __fastcall kotor_fastcall_00665590(void* self, void*, dlg::U minor);   // GuiTimingEvent
dlg::U __fastcall kotor_fastcall_00654900(void* self, void*, dlg::U minor);   // SaveLoad
dlg::U __fastcall kotor_fastcall_00654970(void* self, void*, dlg::U minor);   // ShutDownServer (client side)
dlg::U __fastcall kotor_fastcall_006631e0(void* self, void*, dlg::U minor);   // GuiInfoPopup
dlg::U __fastcall kotor_fastcall_00651ba0(void* self, void*, dlg::U minor);   // VoiceChat
dlg::U __fastcall kotor_fastcall_00651810(void* self, void*, dlg::U minor);   // GuiContainerObject
dlg::U __fastcall kotor_fastcall_00651560(void* self, void*, dlg::U minor);   // ActiveItemPropertyUpdate
dlg::U __fastcall kotor_fastcall_00654aa0(void* self, void*, dlg::U minor);   // GUICharacterSheet
dlg::U __fastcall kotor_fastcall_00663220(void* self, void*, dlg::U minor);   // CheatMessage
dlg::U __fastcall kotor_fastcall_0064dea0(void* self, void*, dlg::U minor);   // GuiInventory
dlg::U __fastcall kotor_fastcall_00663810(void* self, void*, dlg::U minor);   // MapPin
dlg::U __fastcall kotor_fastcall_00654700(void* self, void*, dlg::U minor);   // CharacterDownload
}
