// lane-w3 unit w3-client-door-placeable: the client "door add" and "placeable add" paths of the GameObjectUpdate
// message (major 05h, Update_Add router 00656BD0, type bytes 10 / 9) and the callees they reach.
//   0064CF30 CSWCMessage::HandleServerToPlayerDoorUpdate_Add        0064D500 ..PlaceableUpdate_Add
//   00683BB0 CSWCDoor::CSWCDoor  006842E0 LoadModel  00683D40 SetState  00683D00 SetAnimationWithoutTrigger
//   00683CC0 CSWCDoor::SetIsAreaTransition                           00753960 CSWCDoor table (82 slots)
//   00682270 CSWCPlaceable::CSWCPlaceable  006823F0 LoadModel  00682E20 SetAppearance  006826E0 LoadLight
//   00683630 CSWCPlaceable::SetAnimation                             007537D0 CSWCPlaceable table (82 slots)
//   006E56A0 / 006E5710 / 006E57B0 / 006E59C0 / 006E5AC0 CSWCAnimBaseDoor ctor / LoadModel / SetAnimationInternal /
//            GetAnimationName / ServerToClientAnimation                00758A08 CSWCAnimBaseDoor table (62 slots)
//   006E4E50 / 006E4EC0 CSWCAnimBasePlaceable ctor / LoadModel        007588B0 CSWCAnimBasePlaceable table (62 slots)
//   0069D4F0 CSWCAnimBase::SetAnimation  0069D7B0 CSWCAnimBase::SetEnvMap(BOOL,BOOL,CResRef)  0069DC30 TurnOffShadows
//   0063C080 CSWCObject::GetModel  0063CB70 CSWCObject::SetEnvMap(BOOL,BOOL,CResRef)  0060B310 / 0060B320 TurnOn/OffShadows
//   005ED600 / 005F1310 GetDoorByGameObjectID   005ED5F0 / 005F12C0 GetPlaceableByGameObjectID
//   005ED750 / 005F6340 AddObjectToHitcheckIgnoreList
//   00605BF0 CSWCArea::GetPlaceableObjectLight  00605B50 RemovePlaceableObjectLight  006057B0 ParentCorpseExists
// Original: swkotor.exe GOG K1 (image base 0x400000), read-only static disassembly (lane-w3 tools/odis.py, sdis.py);
// nothing original was executed. Conventions: w3_client.h. The render core (NewCAurObject, Gob slots) and every
// runtime1115 owner listed in w3_client_door_placeable.h are only called, never re-implemented.
#include "203861aa21-w3_client_door_placeable.h"
#include "b4799296be-m15e_animbase.h"

using namespace kotor_w3;

namespace {
constexpr U8 kPlaceable_00746638 = 9, kDoor_00746639 = 10;             // .rdata 00746638 / 00746639 object types
constexpr U8 kGameObjectOk_0074666c = 0;                               // .rdata 0074666C
constexpr U kZero_0073d700 = 0x00000000u;                              // .rdata 0073D700 (0.0f)
constexpr U kOne_3f800000 = 0x3F800000u;                               // immediate 1.0f
constexpr U kOneAndHalf_3fc00000 = 0x3FC00000u;                        // immediate 1.5f
constexpr U kThree_40400000 = 0x40400000u;                             // immediate 3.0f
constexpr U kNineNineNineNine_461c3c00 = 0x461C3C00u;                       // immediate 9999.0f
const char kEmpty_0073d71c[] = "";                                     // .rdata 0073D71C
const char kDefault_0073e304[] = "DEFAULT";                            // .rdata 0073E304
const char kModel_007499a0[] = "Model";                                // .rdata 007499A0
const char kVisibleModel_00749990[] = "VisibleModel";                  // .rdata 00749990
const char kModelName_00749814[] = "ModelName";                        // .rdata 00749814
const char kLowGore_0074ef78[] = "LowGore";                            // .rdata 0074EF78
const char kDoor_00741e80[] = "door";                                  // .rdata 00741E80
const char kPlaceable_00741e68[] = "placeable";                        // .rdata 00741E68
const char kCannotLoadDoor_00758b00[] = "Cannot load door model '%s'.";   // .rdata 00758B00
const char kNoAnimName_00758b20[] = "CSWCAnimBaseDoor::GetAnimationName(): No name for server animation %d";   // 00758B20
const char kFailedToMap_007589a8[] =
    "CSWCAnimBasePlaceable::ServerToClientAnimation(): Failed to map server anim %i to client anim.";   // 007589A8
const char kHeadHit_00753918[] = "_head_hit";                          // .rdata 00753918
const char kHitCheck_00753944[] = "HitCheck";                          // .rdata 00753944
const char kNonCull_0075393c[] = "NonCull";                            // .rdata 0075393C
const char kTrans_00753aa8[] = "trans";                                // .rdata 00753AA8
const char kLightColor_007494bc[] = "LightColor";                      // .rdata 007494BC
const char kLightNode_00753924[] = "LightNode";                        // .rdata 00753924
const char kImpact_00743318[] = "impact";                              // .rdata 00743318
const char kPlaceableLight_0074ef68[] = "placeablelight";              // .rdata 0074EF68
const char kFxPlaceable01_0074ef58[] = "fx_placeable01";               // .rdata 0074EF58
const char kRed_0074ef54[] = "RED";                                    // .rdata 0074EF54
const char kGreen_0074ef4c[] = "GREEN";                                // .rdata 0074EF4C
const char kBlue_0074ef44[] = "BLUE";                                  // .rdata 0074EF44
const char kLightOffsetX_0074ef34[] = "LightOffsetX";                  // .rdata 0074EF34
const char kLightOffsetY_0074ef24[] = "LightOffsetY";                  // .rdata 0074EF24
const char kLightOffsetZ_0074ef14[] = "LightOffsetZ";                  // .rdata 0074EF14
const char kRoot_00740c00[] = "root";                                  // .rdata 00740C00

U8 read_byte(void* m) noexcept { return kotor_fastcall_004d6720(m, nullptr, 8); }
U read_bool(void* m) noexcept { return kotor_fastcall_004d66f0(m, nullptr); }
U read_word(void* m) noexcept { return kotor_fastcall_004d6750(m, nullptr, 0x10); }
void exo_init(void* s) noexcept(false) {                               // 005B3190 CExoString()
    kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(s), nullptr);
}
void exo_text(void* s, const char* text) noexcept(false) { frame_LogStringCtor_005e5a90(s, nullptr, text); }   // 005E5A90
void exo_free(void* s) noexcept(false) { frame_LogStringDtor_005e5c20(s, nullptr); }                             // 005E5C20
// [[007A3A28]+B8h] CTwoDimArrays: +24h genericdoors, +28h doortypes, +2Ch placeables, +3Ch animations, +48h lightcolor
void* two_da(unsigned offset) noexcept { return rp(rp(kotor_cell_rules_007a3a28, 0xB8), offset); }
void* server_app() noexcept { return rp(kotor_menu_window_cells::manager_007a39fc, 8); }   // [[007A39FC]+8] CServerExoApp*

using SlotLoadModel = U (__fastcall*)(void*, void*, void*, U, U);      // CSWCObject +78h LoadModel(CResRef*, BYTE, BYTE) RET 0Ch
using SlotSetId = void (__fastcall*)(void*, void*, U);                 // +04h SetId RET 4
using SlotPtr = U (__fastcall*)(void*, void*, void*);                  // +94h AttachToScene RET 4
using SlotSetArea = void (__fastcall*)(void*, void*, void*);           // +84h SetArea RET 4
using SlotGetModel = void* (__fastcall*)(void*, void*, U, U);          // +98h GetModel(BYTE, BOOL) RET 8
using AnimLoadModel = U (__fastcall*)(void*, void*, void*, U);         // anim +0Ch LoadModel(CResRef*, BYTE) RET 8
using AnimGetModel = void* (__fastcall*)(void*, void*, U);             // anim +08h GetModel(BYTE) RET 4
using GobU = void (__fastcall*)(void*, void*, U);                      // gob +7Ch / +90h (one dword) RET 4
using GobText = U (__fastcall*)(void*, void*, const char*);            // gob +48h / +B0h (char*) RET 4
}  // namespace

// ============================================================================================ virtual tables
// Generated from the original .rdata slot words (lane-w3 tools: odis.rd). Shared slots point at the existing
// runtime1115 bodies / D83 traps of the lane-m15-e CSWCObject (007516C8) and CSWCAnimBase (00754F60) tables, at the
// lane-w3 bodies (0063F570 SetArea, 00616FB0 ShowShadowBlob) or at this unit; class-specific slots not reconstructed
// here are explicit D83 traps (w3_traps_door_placeable.cpp).
extern "C" {
const void* const recovered_client_door_methods_00753960[82] = {
    reinterpret_cast<const void*>(&recovered_client_door_destructor_006843f0),                         // +000 006843F0 CSWCDoor::~CSWCDoor
    reinterpret_cast<const void*>(&cswc_object_set_id_00616fa0),                                       // +004 00616FA0 CSWCObject::SetId
    reinterpret_cast<const void*>(&kotor_m15e_slot008_void8_005b5e90),                                 // +008 005B5E90 void_return
    reinterpret_cast<const void*>(&kotor_m15e_slot00c_self_00641db0),                                  // +00C 00641DB0 self_return
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +010 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot00c_self_00641db0),                                  // +014 00641DB0 self_return
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +018 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +01C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +020 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +024 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +028 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot02c_zero_0063e7f0),                                  // +02C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +030 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot02c_zero_0063e7f0),                                  // +034 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +038 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +03C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +040 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +044 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +048 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +04C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +050 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +054 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +058 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +05C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +060 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +064 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +068 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +06C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&recovered_client_door_get_name_006823c0),                           // +070 006823C0 CSWCDoor::GetName
    reinterpret_cast<const void*>(&recovered_client_door_ai_update_00684cd0),                          // +074 00684CD0 CSWCDoor::AIUpdate
    reinterpret_cast<const void*>(&recovered_client_door_load_model_006842e0),                         // +078 006842E0 CSWCDoor::LoadModel
    reinterpret_cast<const void*>(&kotor_m15e_object_unload_model_00646650),                          // +07C 00646650 CSWCObject::UnloadModel
    reinterpret_cast<const void*>(&recovered_client_object_unload_model_006825f0),                     // +080 006825F0 CSWCObject::UnloadModel
    reinterpret_cast<const void*>(&recovered_client_object_set_area_0063f570),                         // +084 0063F570 CSWCObject::SetArea
    reinterpret_cast<const void*>(&kotor_m15e_object_set_orientation_0063c110),                        // +088 0063C110 CSWCObject::SetOrientation
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063f340),                                          // +08C 0063F340 CSWCObject::SetPosition
    reinterpret_cast<const void*>(&kotor_m15e_slot090_void0_004015a0),                                 // +090 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_object_attach_to_scene_0063d820),                        // +094 0063D820 CSWCObject::AttachToScene
    reinterpret_cast<const void*>(&recovered_client_object_get_model_0063c080),                        // +098 0063C080 CSWCObject::GetModel
    reinterpret_cast<const void*>(&kotor_m15e_object_set_animation_0063e930),                          // +09C 0063E930 CSWCObject::SetAnimation
    reinterpret_cast<const void*>(&kotor_m15e_object_add_fire_forget_animation_0063d8c0),              // +0A0 0063D8C0 CSWCObject::AddFireForgetAnimation
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063cb00),                                          // +0A4 0063CB00 CSWCObject::Hilite
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063cb30),                                          // +0A8 0063CB30 CSWCObject::UnHilite
    reinterpret_cast<const void*>(&kotor_m15e_object_set_env_map_0063ccc0),                            // +0AC 0063CCC0 CSWCObject::SetEnvMap
    reinterpret_cast<const void*>(&recovered_client_object_set_env_map_0063cb70),                      // +0B0 0063CB70 CSWCObject::SetEnvMap
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d970),                                          // +0B4 0063D970 CSWCObject::AddVisualEffect
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063f400),                                          // +0B8 0063F400 CSWCObject::DeleteVisualEffect
    reinterpret_cast<const void*>(&kotor_m15e_object_kill_all_visual_effects_0063dae0),                // +0BC 0063DAE0 CSWCObject::KillAllVisualEffects
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063db20),                                          // +0C0 0063DB20 CSWCObject::RefreshVisualEffects
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063f490),                                          // +0C4 0063F490 CSWCObject::UpdateVisualEffectsList
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063daa0),                                          // +0C8 0063DAA0 CSWCObject::StopAllVisualEffects
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063ddf0),                                          // +0CC 0063DDF0 CSWCObject::HasVisualEffectApplied
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e760),                                          // +0D0 0063E760 CSWCObject::GetCameraFollowPart
    reinterpret_cast<const void*>(&kotor_m15e_slot0d4_true_0063e7a0),                                  // +0D4 0063E7A0 return_true
    reinterpret_cast<const void*>(&kotor_m15e_trap_00616fe0),                                          // +0D8 00616FE0 return_1.5
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063db90),                                          // +0DC 0063DB90 CSWCObject::ActivateHardCodedVisualWithHighestPriority
    reinterpret_cast<const void*>(&kotor_m15e_object_detach_visual_effects_0063e450),                  // +0E0 0063E450 CSWCObject::DetachVisualEffects
    reinterpret_cast<const void*>(&kotor_m15e_object_reattach_visual_effects_0063e4b0),                // +0E4 0063E4B0 CSWCObject::ReAttachVisualEffects
    reinterpret_cast<const void*>(&kotor_m15e_slot090_void0_004015a0),                                 // +0E8 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063cfd0),                                          // +0EC 0063CFD0 CSWCObject::SetPortrait
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e770),                                          // +0F0 0063E770 CSWCObject::GetPortrait
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d140),                                          // +0F4 0063D140 CSWCObject::SetPortraitId
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e790),                                          // +0F8 0063E790 CSWCObject::GetPortraitId
    reinterpret_cast<const void*>(&kotor_m15e_slot0d4_true_0063e7a0),                                  // +0FC 0063E7A0 return_true
    reinterpret_cast<const void*>(&kotor_m15e_object_remove_shadow_blob_0063d070),                     // +100 0063D070 CSWCObject::RemoveShadowBlob
    reinterpret_cast<const void*>(&recovered_client_object_show_shadow_blob_00616fb0),                 // +104 00616FB0 CSWCObject::ShowShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_object_hide_shadow_blob_00616fd0),                       // +108 00616FD0 CSWCObject::HideShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d0a0),                                          // +10C 0063D0A0 CSWCObject::SetGroundNormal
    reinterpret_cast<const void*>(&recovered_client_object_turn_on_shadows_0060b310),                  // +110 0060B310 CSWCObject::TurnOnShadows
    reinterpret_cast<const void*>(&recovered_client_object_turn_off_shadows_0060b320),                 // +114 0060B320 CSWCObject::TurnOffShadows
    reinterpret_cast<const void*>(&kotor_m15e_slot090_void0_004015a0),                                 // +118 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +11C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e7b0),                                          // +120 0063E7B0 CSWCObject::BarkString
    reinterpret_cast<const void*>(&kotor_m15e_trap_0060f560),                                          // +124 0060F560 CSWCObject::BarkBubble
    reinterpret_cast<const void*>(&kotor_m15e_slot128_false4_0063e7c0),                                // +128 0063E7C0 return_false_4
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e7d0),                                          // +12C 0063E7D0 CSWCObject::ClientServerSyncHandler_Position
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e7e0),                                          // +130 0063E7E0 CSWCObject::ClientServerSyncHandler_Orientation
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d3f0),                                          // +134 0063D3F0 CSWCObject::ClientServerSyncHandler_Animation
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +138 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d560),                                          // +13C 0063D560 CSWCObject::GetReticlePoint
    reinterpret_cast<const void*>(&kotor_m15e_trap_00616fe0),                                          // +140 00616FE0 return_1.5
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d780),                                          // +144 0063D780 CSWCObject::GetStickyMouseThreshhold
};
const void* const recovered_client_placeable_methods_007537d0[82] = {
    reinterpret_cast<const void*>(&recovered_client_placeable_destructor_006836a0),                    // +000 006836A0 CSWCPlaceable::~CSWCPlaceable
    reinterpret_cast<const void*>(&cswc_object_set_id_00616fa0),                                       // +004 00616FA0 CSWCObject::SetId
    reinterpret_cast<const void*>(&kotor_m15e_slot008_void8_005b5e90),                                 // +008 005B5E90 void_return
    reinterpret_cast<const void*>(&kotor_m15e_slot00c_self_00641db0),                                  // +00C 00641DB0 self_return
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +010 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +014 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +018 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +01C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +020 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +024 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +028 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot02c_zero_0063e7f0),                                  // +02C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +030 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot02c_zero_0063e7f0),                                  // +034 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +038 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +03C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +040 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +044 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +048 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot00c_self_00641db0),                                  // +04C 00641DB0 self_return
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +050 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +054 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +058 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +05C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +060 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +064 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +068 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +06C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&recovered_client_door_get_name_006823c0),                           // +070 006823C0 CSWCDoor::GetName
    reinterpret_cast<const void*>(&recovered_client_placeable_ai_update_006836c0),                     // +074 006836C0 CSWCPlaceable::AIUpdate
    reinterpret_cast<const void*>(&recovered_client_placeable_load_model_006823f0),                    // +078 006823F0 CSWCPlaceable::LoadModel
    reinterpret_cast<const void*>(&kotor_m15e_object_unload_model_00646650),                          // +07C 00646650 CSWCObject::UnloadModel
    reinterpret_cast<const void*>(&recovered_client_object_unload_model_006825f0),                     // +080 006825F0 CSWCObject::UnloadModel
    reinterpret_cast<const void*>(&recovered_client_object_set_area_0063f570),                         // +084 0063F570 CSWCObject::SetArea
    reinterpret_cast<const void*>(&kotor_m15e_object_set_orientation_0063c110),                        // +088 0063C110 CSWCObject::SetOrientation
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063f340),                                          // +08C 0063F340 CSWCObject::SetPosition
    reinterpret_cast<const void*>(&kotor_m15e_slot090_void0_004015a0),                                 // +090 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_object_attach_to_scene_0063d820),                        // +094 0063D820 CSWCObject::AttachToScene
    reinterpret_cast<const void*>(&recovered_client_object_get_model_0063c080),                        // +098 0063C080 CSWCObject::GetModel
    reinterpret_cast<const void*>(&recovered_client_placeable_set_animation_00683630),                 // +09C 00683630 CSWCPlaceable::SetAnimation
    reinterpret_cast<const void*>(&kotor_m15e_object_add_fire_forget_animation_0063d8c0),              // +0A0 0063D8C0 CSWCObject::AddFireForgetAnimation
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063cb00),                                          // +0A4 0063CB00 CSWCObject::Hilite
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063cb30),                                          // +0A8 0063CB30 CSWCObject::UnHilite
    reinterpret_cast<const void*>(&kotor_m15e_object_set_env_map_0063ccc0),                            // +0AC 0063CCC0 CSWCObject::SetEnvMap
    reinterpret_cast<const void*>(&recovered_client_object_set_env_map_0063cb70),                      // +0B0 0063CB70 CSWCObject::SetEnvMap
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d970),                                          // +0B4 0063D970 CSWCObject::AddVisualEffect
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063f400),                                          // +0B8 0063F400 CSWCObject::DeleteVisualEffect
    reinterpret_cast<const void*>(&kotor_m15e_object_kill_all_visual_effects_0063dae0),                // +0BC 0063DAE0 CSWCObject::KillAllVisualEffects
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063db20),                                          // +0C0 0063DB20 CSWCObject::RefreshVisualEffects
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063f490),                                          // +0C4 0063F490 CSWCObject::UpdateVisualEffectsList
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063daa0),                                          // +0C8 0063DAA0 CSWCObject::StopAllVisualEffects
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063ddf0),                                          // +0CC 0063DDF0 CSWCObject::HasVisualEffectApplied
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e760),                                          // +0D0 0063E760 CSWCObject::GetCameraFollowPart
    reinterpret_cast<const void*>(&kotor_m15e_slot0d4_true_0063e7a0),                                  // +0D4 0063E7A0 return_true
    reinterpret_cast<const void*>(&kotor_m15e_trap_00616fe0),                                          // +0D8 00616FE0 return_1.5
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063db90),                                          // +0DC 0063DB90 CSWCObject::ActivateHardCodedVisualWithHighestPriority
    reinterpret_cast<const void*>(&kotor_m15e_object_detach_visual_effects_0063e450),                  // +0E0 0063E450 CSWCObject::DetachVisualEffects
    reinterpret_cast<const void*>(&kotor_m15e_object_reattach_visual_effects_0063e4b0),                // +0E4 0063E4B0 CSWCObject::ReAttachVisualEffects
    reinterpret_cast<const void*>(&kotor_m15e_slot090_void0_004015a0),                                 // +0E8 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063cfd0),                                          // +0EC 0063CFD0 CSWCObject::SetPortrait
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e770),                                          // +0F0 0063E770 CSWCObject::GetPortrait
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d140),                                          // +0F4 0063D140 CSWCObject::SetPortraitId
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e790),                                          // +0F8 0063E790 CSWCObject::GetPortraitId
    reinterpret_cast<const void*>(&recovered_client_placeable_apply_shadow_blob_00682c60),             // +0FC 00682C60 CSWCPlaceable::ApplyShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_object_remove_shadow_blob_0063d070),                     // +100 0063D070 CSWCObject::RemoveShadowBlob
    reinterpret_cast<const void*>(&recovered_client_object_show_shadow_blob_00616fb0),                 // +104 00616FB0 CSWCObject::ShowShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_object_hide_shadow_blob_00616fd0),                       // +108 00616FD0 CSWCObject::HideShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d0a0),                                          // +10C 0063D0A0 CSWCObject::SetGroundNormal
    reinterpret_cast<const void*>(&recovered_client_object_turn_on_shadows_0060b310),                  // +110 0060B310 CSWCObject::TurnOnShadows
    reinterpret_cast<const void*>(&recovered_client_object_turn_off_shadows_0060b320),                 // +114 0060B320 CSWCObject::TurnOffShadows
    reinterpret_cast<const void*>(&kotor_m15e_slot090_void0_004015a0),                                 // +118 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +11C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e7b0),                                          // +120 0063E7B0 CSWCObject::BarkString
    reinterpret_cast<const void*>(&kotor_m15e_trap_0060f560),                                          // +124 0060F560 CSWCObject::BarkBubble
    reinterpret_cast<const void*>(&kotor_m15e_slot128_false4_0063e7c0),                                // +128 0063E7C0 return_false_4
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e7d0),                                          // +12C 0063E7D0 CSWCObject::ClientServerSyncHandler_Position
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e7e0),                                          // +130 0063E7E0 CSWCObject::ClientServerSyncHandler_Orientation
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d3f0),                                          // +134 0063D3F0 CSWCObject::ClientServerSyncHandler_Animation
    reinterpret_cast<const void*>(&recovered_client_placeable_is_hostile_006831b0),                    // +138 006831B0 CSWCPlaceable::IsHostile
    reinterpret_cast<const void*>(&recovered_client_placeable_get_reticle_point_00682ff0),             // +13C 00682FF0 CSWCPlaceable::GetReticlePoint
    reinterpret_cast<const void*>(&recovered_client_placeable_get_can_see_height_00682f20),            // +140 00682F20 CSWCPlaceable::GetCanSeeHeight
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d780),                                          // +144 0063D780 CSWCObject::GetStickyMouseThreshhold
};
const void* const recovered_anim_base_door_methods_00758a08[62] = {
    reinterpret_cast<const void*>(&w3_anim_door_scalar_006e59a0),                      // +000 006E59A0 CSWCAnimBaseDoor::~CSWCAnimBaseDoor
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_attach_to_scene_0069cd80),                     // +004 0069CD80 CSWCAnimBase::AttachToScene
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_get_model_0069cda0),                           // +008 0069CDA0 CSWCAnimBase::GetModel
    reinterpret_cast<const void*>(&recovered_anim_base_door_load_model_006e5710),                      // +00C 006E5710 CSWCAnimBaseDoor::LoadModel
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069cfd0_s010),                                  // +010 0069CFD0 CSWCAnimBase::ReplaceTexture
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_replace_texture_0069cef0),                     // +014 0069CEF0 CSWCAnimBase::ReplaceTexture
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_replace_texture_0069cdc0),                     // +018 0069CDC0 CSWCAnimBase::ReplaceTexture
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_replace_texture_0069ce50),                     // +01C 0069CE50 CSWCAnimBase::ReplaceTexture
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_restore_texture_0069d100),                     // +020 0069D100 CSWCAnimBase::RestoreTexture
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d0a0_s024),                                  // +024 0069D0A0 CSWCAnimBase::RestoreTexture
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069dd90_s028),                                  // +028 0069DD90 CSWCAnimBase::EnableBumpedOutShell
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069ddf0_s02c),                                  // +02C 0069DDF0 CSWCAnimBase::DisableBumpedOutShell
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069de10_s030),                                  // +030 0069DE10 CSWCAnimBase::SetMotionBlur
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069de40_s034),                                  // +034 0069DE40 CSWCAnimBase::SetMaxMotionBlurLength
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_get_animation_length_0069d250),                // +038 0069D250 CSWCAnimBase::GetAnimationLength
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_get_animation_time_0069d180),                  // +03C 0069D180 CSWCAnimBase::GetAnimationTime
    reinterpret_cast<const void*>(&recovered_anim_base_door_get_animation_name_006e59c0),              // +040 006E59C0 CSWCAnimBaseDoor::GetAnimationName
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_006a03c0_s044),                                  // +044 006A03C0 CSWCAnimBase::SetOverlayAnimation
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d3e0_s048),                                  // +048 0069D3E0 CSWCAnimBase::GetOverlayAnimation
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_get_overlay_animation_playing_0069d320),       // +04C 0069D320 CSWCAnimBase::GetOverlayAnimationPlaying
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_animation_exists_0069d910),                    // +050 0069D910 CSWCAnimBase::AnimationExists
    reinterpret_cast<const void*>(&recovered_anim_base_set_animation_0069d4f0),                        // +054 0069D4F0 CSWCAnimBase::SetAnimation
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d520_s058),                                  // +058 0069D520 CSWCAnimBase::SetAnimationOnSubObject
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d540_s05c),                                  // +05C 0069D540 CSWCAnimBase::SetAnimationVisemes
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_orientation_0069d640),                     // +060 0069D640 CSWCAnimBase::SetOrientation
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_orientation_0069d5d0),                     // +064 0069D5D0 CSWCAnimBase::SetOrientation
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_position_0069d690),                        // +068 0069D690 CSWCAnimBase::SetPosition
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d6e0_s06c),                                  // +06C 0069D6E0 CSWCAnimBase::SetAlwaysRender
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d710_s070),                                  // +070 0069D710 CSWCAnimBase::SetInCutscene
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_unload_model_0069d770),                        // +074 0069D770 CSWCAnimBase::UnloadModel
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_unload_model_0069d740),                        // +078 0069D740 CSWCAnimBase::UnloadModel
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_00401590_s07c),                                  // +07C 00401590 return_zero_0x10
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_005b5e90_s080),                                  // +080 005B5E90 void_return
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_004015a0_s084),                                  // +084 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_004015a0_s088),                                  // +088 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_004015a0_s08c),                                  // +08C 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0060e760_s090),                                  // +090 0060E760 void_return
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_alpha_0069eb20),                           // +094 0069EB20 CSWCAnimBase::SetAlpha
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_env_map_0069d830),                         // +098 0069D830 CSWCAnimBase::SetEnvMap
    reinterpret_cast<const void*>(&recovered_anim_base_set_env_map_0069d7b0),                          // +09C 0069D7B0 CSWCAnimBase::SetEnvMap
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d8b0_s0a0),                                  // +0A0 0069D8B0 CSWCAnimBase::RestoreEnvMap
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_apply_shadow_blob_0069da00),                   // +0A4 0069DA00 CSWCAnimBase::ApplyShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_remove_shadow_blob_0069da70),                  // +0A8 0069DA70 CSWCAnimBase::RemoveShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_show_shadow_blob_0069dac0),                    // +0AC 0069DAC0 CSWCAnimBase::ShowShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_hide_shadow_blob_0069db10),                    // +0B0 0069DB10 CSWCAnimBase::HideShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_shadow_blob_orientation_0069db50),         // +0B4 0069DB50 CSWCAnimBase::SetShadowBlobOrientation
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_turn_on_shadows_0069dc10),                     // +0B8 0069DC10 CSWCAnimBase::TurnOnShadows
    reinterpret_cast<const void*>(&recovered_anim_base_turn_off_shadows_0069dc30),                     // +0BC 0069DC30 CSWCAnimBase::TurnOffShadows
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069dc50_s0c0),                                  // +0C0 0069DC50 CSWCAnimBase::GetOrientation
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0060e760_s0c4),                                  // +0C4 0060E760 void_return
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069a3a0_s0c8),                                  // +0C8 0069A3A0 CSWCAnimBase::AddAnimationFlag
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069a3c0_s0cc),                                  // +0CC 0069A3C0 CSWCAnimBase::RemoveAnimationFlag
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069dd30_s0d0),                                  // +0D0 0069DD30 CSWCAnimBase::SetLean
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069dc90_s0d4),                                  // +0D4 0069DC90 CSWCAnimBase::Lean
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069f630_s0d8),                                  // +0D8 0069F630 CSWCAnimBase::GetBoundingRect
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069f1e0_s0dc),                                  // +0DC 0069F1E0 CSWCAnimBase::GetCenterPoint
    reinterpret_cast<const void*>(&recovered_anim_base_door_server_to_client_animation_006e5ac0),      // +0E0 006E5AC0 CSWCAnimBaseDoor::ServerToClientAnimation
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_enable_distortion_0069de50),                   // +0E4 0069DE50 CSWCAnimBase::EnableDistortion
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_disable_distortion_0069de70),                  // +0E8 0069DE70 CSWCAnimBase::DisableDistortion
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0060e760_s0ec),                                  // +0EC 0060E760 void_return
    reinterpret_cast<const void*>(&recovered_anim_base_door_set_animation_internal_006e57b0),          // +0F0 006E57B0 CSWCAnimBaseDoor::SetAnimationInternal
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d5a0_s0f4),                                  // +0F4 0069D5A0 CSWCAnimBase::SetAnimationOnSubObjectInternal
};
const void* const recovered_anim_base_placeable_methods_007588b0[62] = {
    reinterpret_cast<const void*>(&w3_anim_placeable_scalar_006e5180),                 // +000 006E5180 CSWCAnimBasePlaceable::~CSWCAnimBasePlaceable
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_attach_to_scene_0069cd80),                     // +004 0069CD80 CSWCAnimBase::AttachToScene
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_get_model_0069cda0),                           // +008 0069CDA0 CSWCAnimBase::GetModel
    reinterpret_cast<const void*>(&recovered_anim_base_placeable_load_model_006e4ec0),                 // +00C 006E4EC0 CSWCAnimBasePlaceable::LoadModel
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069cfd0_s010),                                  // +010 0069CFD0 CSWCAnimBase::ReplaceTexture
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_replace_texture_0069cef0),                     // +014 0069CEF0 CSWCAnimBase::ReplaceTexture
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_replace_texture_0069cdc0),                     // +018 0069CDC0 CSWCAnimBase::ReplaceTexture
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_replace_texture_0069ce50),                     // +01C 0069CE50 CSWCAnimBase::ReplaceTexture
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_restore_texture_0069d100),                     // +020 0069D100 CSWCAnimBase::RestoreTexture
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d0a0_s024),                                  // +024 0069D0A0 CSWCAnimBase::RestoreTexture
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069dd90_s028),                                  // +028 0069DD90 CSWCAnimBase::EnableBumpedOutShell
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069ddf0_s02c),                                  // +02C 0069DDF0 CSWCAnimBase::DisableBumpedOutShell
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069de10_s030),                                  // +030 0069DE10 CSWCAnimBase::SetMotionBlur
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069de40_s034),                                  // +034 0069DE40 CSWCAnimBase::SetMaxMotionBlurLength
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_get_animation_length_0069d250),                // +038 0069D250 CSWCAnimBase::GetAnimationLength
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_get_animation_time_0069d180),                  // +03C 0069D180 CSWCAnimBase::GetAnimationTime
    reinterpret_cast<const void*>(&w3_anim_placeable_name_006e51a0),         // +040 006E51A0 CSWCAnimBasePlaceable::GetAnimationName
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_006a03c0_s044),                                  // +044 006A03C0 CSWCAnimBase::SetOverlayAnimation
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d3e0_s048),                                  // +048 0069D3E0 CSWCAnimBase::GetOverlayAnimation
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_get_overlay_animation_playing_0069d320),       // +04C 0069D320 CSWCAnimBase::GetOverlayAnimationPlaying
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_animation_exists_0069d910),                    // +050 0069D910 CSWCAnimBase::AnimationExists
    reinterpret_cast<const void*>(&w3_anim_placeable_set_006e4f30),              // +054 006E4F30 CSWCAnimBasePlaceable::SetAnimation
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d520_s058),                                  // +058 0069D520 CSWCAnimBase::SetAnimationOnSubObject
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d540_s05c),                                  // +05C 0069D540 CSWCAnimBase::SetAnimationVisemes
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_orientation_0069d640),                     // +060 0069D640 CSWCAnimBase::SetOrientation
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_orientation_0069d5d0),                     // +064 0069D5D0 CSWCAnimBase::SetOrientation
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_position_0069d690),                        // +068 0069D690 CSWCAnimBase::SetPosition
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d6e0_s06c),                                  // +06C 0069D6E0 CSWCAnimBase::SetAlwaysRender
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d710_s070),                                  // +070 0069D710 CSWCAnimBase::SetInCutscene
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_unload_model_0069d770),                        // +074 0069D770 CSWCAnimBase::UnloadModel
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_unload_model_0069d740),                        // +078 0069D740 CSWCAnimBase::UnloadModel
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_00401590_s07c),                                  // +07C 00401590 return_zero_0x10
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_005b5e90_s080),                                  // +080 005B5E90 void_return
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_004015a0_s084),                                  // +084 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_004015a0_s088),                                  // +088 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_004015a0_s08c),                                  // +08C 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0060e760_s090),                                  // +090 0060E760 void_return
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_alpha_0069eb20),                           // +094 0069EB20 CSWCAnimBase::SetAlpha
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_env_map_0069d830),                         // +098 0069D830 CSWCAnimBase::SetEnvMap
    reinterpret_cast<const void*>(&recovered_anim_base_set_env_map_0069d7b0),                          // +09C 0069D7B0 CSWCAnimBase::SetEnvMap
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d8b0_s0a0),                                  // +0A0 0069D8B0 CSWCAnimBase::RestoreEnvMap
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_apply_shadow_blob_0069da00),                   // +0A4 0069DA00 CSWCAnimBase::ApplyShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_remove_shadow_blob_0069da70),                  // +0A8 0069DA70 CSWCAnimBase::RemoveShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_show_shadow_blob_0069dac0),                    // +0AC 0069DAC0 CSWCAnimBase::ShowShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_hide_shadow_blob_0069db10),                    // +0B0 0069DB10 CSWCAnimBase::HideShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_shadow_blob_orientation_0069db50),         // +0B4 0069DB50 CSWCAnimBase::SetShadowBlobOrientation
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_turn_on_shadows_0069dc10),                     // +0B8 0069DC10 CSWCAnimBase::TurnOnShadows
    reinterpret_cast<const void*>(&recovered_anim_base_turn_off_shadows_0069dc30),                     // +0BC 0069DC30 CSWCAnimBase::TurnOffShadows
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069dc50_s0c0),                                  // +0C0 0069DC50 CSWCAnimBase::GetOrientation
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0060e760_s0c4),                                  // +0C4 0060E760 void_return
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069a3a0_s0c8),                                  // +0C8 0069A3A0 CSWCAnimBase::AddAnimationFlag
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069a3c0_s0cc),                                  // +0CC 0069A3C0 CSWCAnimBase::RemoveAnimationFlag
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069dd30_s0d0),                                  // +0D0 0069DD30 CSWCAnimBase::SetLean
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069dc90_s0d4),                                  // +0D4 0069DC90 CSWCAnimBase::Lean
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069f630_s0d8),                                  // +0D8 0069F630 CSWCAnimBase::GetBoundingRect
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069f1e0_s0dc),                                  // +0DC 0069F1E0 CSWCAnimBase::GetCenterPoint
    reinterpret_cast<const void*>(&w3_anim_placeable_convert_006e53f0), // +0E0 006E53F0 CSWCAnimBasePlaceable::ServerToClientAnimation
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_enable_distortion_0069de50),                   // +0E4 0069DE50 CSWCAnimBase::EnableDistortion
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_disable_distortion_0069de70),                  // +0E8 0069DE70 CSWCAnimBase::DisableDistortion
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0060e760_s0ec),                                  // +0EC 0060E760 void_return
    reinterpret_cast<const void*>(&recovered_anim_base_internal_0069e8d0),                             // +0F0 0069E8D0 CSWCAnimBase::SetAnimationInternal
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d5a0_s0f4),                                  // +0F4 0069D5A0 CSWCAnimBase::SetAnimationOnSubObjectInternal
};
}

extern "C" {
// ============================================================================================ client app leaves
// 005F1310 CClientExoAppInternal::GetDoorByGameObjectID(id) RET 4: object array [this+14h] lookup, type 10, slot +14h.
void* __fastcall recovered_client_internal_get_door_by_id_005f1310(void* internal, void*, U id) noexcept(false) {
    void* object = nullptr;                                            // 005F131E {E-0x4}
    if (kotor_fastcall_004d8230(rp(internal, 0x14), nullptr, id, &object) != kGameObjectOk_0074666c) return nullptr;   // 005F1311..005F1331
    if (object == nullptr || ld<U8>(object, 8) != kDoor_00746639) return nullptr;   // 005F1333..005F1344
    return vslot<void* (__fastcall*)(void*, void*)>(object, 0x14)(object, nullptr);   // 005F1346..005F1348 AsSWCDoor
}
// 005ED600 CClientExoApp::GetDoorByGameObjectID: ECX = [this+4]; jmp 005F1310.
void* __fastcall recovered_client_get_door_by_id_005ed600(void* app, void*, U id) noexcept(false) {
    return recovered_client_internal_get_door_by_id_005f1310(rp(app, 4), nullptr, id);   // 005ED600..005ED603
}
// 005F12C0 CClientExoAppInternal::GetPlaceableByGameObjectID(id) RET 4: type 9, slot +4Ch AsSWCPlaceable.
void* __fastcall recovered_client_internal_get_placeable_by_id_005f12c0(void* internal, void*, U id) noexcept(false) {
    void* object = nullptr;                                            // 005F12CE {E-0x4}
    if (kotor_fastcall_004d8230(rp(internal, 0x14), nullptr, id, &object) != kGameObjectOk_0074666c) return nullptr;   // 005F12C1..005F12E1
    if (object == nullptr || ld<U8>(object, 8) != kPlaceable_00746638) return nullptr;   // 005F12E3..005F12F4
    return vslot<void* (__fastcall*)(void*, void*)>(object, 0x4C)(object, nullptr);   // 005F12F6..005F12F8
}
// 005ED5F0 CClientExoApp::GetPlaceableByGameObjectID: ECX = [this+4]; jmp 005F12C0.
void* __fastcall recovered_client_get_placeable_by_id_005ed5f0(void* app, void*, U id) noexcept(false) {
    return recovered_client_internal_get_placeable_by_id_005f12c0(rp(app, 4), nullptr, id);   // 005ED5F0..005ED5F3
}
// 005F6340 CClientExoAppInternal::AddObjectToHitcheckIgnoreList(id) RET 4: list [this+190h] of 4-byte id cells.
void __fastcall recovered_client_internal_add_to_hitcheck_ignore_list_005f6340(void* internal, void*, U id) noexcept(false) {
    void* const list = rp(internal, 0x190);                            // 005F6344
    void* position = rp(list, 0);                                      // 005F634A..005F634C {E-0x4}
    void* cell = rp(list, 0);                                          // 005F6350
    if (cell != nullptr) cell = rp(cell, 8);                           // 005F6352..005F6357
    while (position != nullptr) {                                      // 005F635A..005F6360 / 005F637E..005F6380
        if (cell != nullptr && rd32(cell, 0) == id) return;            // 005F6362..005F6368 already listed
        cell = unresolved_list_next_005e9b90(rp(internal, 0x190), nullptr, &position);   // 005F636A..005F6375
    }
    void* const fresh = live_operator_new(4);                          // 005F6382..005F6389
    st<U>(fresh, 0, id);                                               // 005F638C (no null check, as in the original)
    resource_append_005e9dc0(rp(internal, 0x190), nullptr, fresh);     // 005F638E..005F6395 AddTail
}
// 005ED750 CClientExoApp::AddObjectToHitcheckIgnoreList: ECX = [this+4]; jmp 005F6340.
void __fastcall recovered_client_add_to_hitcheck_ignore_list_005ed750(void* app, void*, U id) noexcept(false) {
    recovered_client_internal_add_to_hitcheck_ignore_list_005f6340(rp(app, 4), nullptr, id);   // 005ED750..005ED753
}

// ============================================================================================ CSWCArea leaves
// 00605BF0 CSWCArea::GetPlaceableObjectLight(id) RET 4: array [this+218h] (count [this+21Ch]) of {id, light} cells.
void* __fastcall recovered_client_area_get_placeable_object_light_00605bf0(void* area, void*, U id) noexcept {
    const I32 count = ld<I32>(area, 0x21C);                            // 00605BF2
    for (I32 i = 0; i < count; ++i) {                                  // 00605BF8..00605C20
        void* const cell = rp(rp(area, 0x218), static_cast<U>(i) * 4); // 00605BFF..00605C10
        if (cell != nullptr && rd32(cell, 0) == id) return rp(rp(rp(area, 0x218), static_cast<U>(i) * 4), 4);   // 00605C12..00605C2D
    }
    return nullptr;                                                    // 00605C22..00605C27
}
// 00605B50 CSWCArea::RemovePlaceableObjectLight(id, BOOL destroy) RET 8: drop the cell (and optionally the light).
void __fastcall recovered_client_area_remove_placeable_object_light_00605b50(void* area, void*, U id, U destroy) noexcept(false) {
    const I32 count = ld<I32>(area, 0x21C);                            // 00605B54
    I32 i = 0;                                                         // 00605B5A
    for (; i < count; ++i) {                                           // 00605B5C..00605B82
        void* const cell = rp(rp(area, 0x218), static_cast<U>(i) * 4); // 00605B65..00605B72
        if (cell != nullptr && rd32(cell, 0) == id) break;             // 00605B74..00605B7A
    }
    if (i >= count) return;                                            // 00605B84..00605B88
    if (destroy != 0) {                                                // 00605B8B..00605B91
        void* const light = rp(rp(rp(area, 0x218), static_cast<U>(i) * 4), 4);   // 00605B93..00605B96
        if (light != nullptr) delete_object(light);                    // 00605B99..00605BA1 [vtbl+0](1)
    }
    live_free(rp(rp(area, 0x218), static_cast<U>(i) * 4));             // 00605BA3..00605BAD
    const I32 now = ld<I32>(area, 0x21C) - 1;                          // 00605BB2..00605BBB
    st<I32>(area, 0x21C, now);                                         // 00605BC2
    for (I32 k = i; k < ld<I32>(area, 0x21C); ++k)                     // 00605BBC..00605BE7
        st<void*>(rp(area, 0x218), static_cast<U>(k) * 4, rp(rp(area, 0x218), static_cast<U>(k + 1) * 4));   // 00605BD0..00605BDC
}
// 006057B0 CSWCArea::ParentCorpseExists(id) RET 4: any of the four creature ids [this+268h..274h] whose
// creature+3DCh (corpse placeable id) equals id.
U __fastcall recovered_client_area_parent_corpse_exists_006057b0(void* area, void*, U id) noexcept(false) {
    for (U8 i = 0; i < 4; ++i) {                                       // 006057B9 / 006057E4..006057E9 (bl)
        void* const creature = word_ptr(frame_R4_005ed5b0(client_app(), nullptr, rd32(area, 0x268 + static_cast<U>(i) * 4)));   // 006057C0..006057D3
        if (creature != nullptr && rd32(creature, 0x3DC) == id) return 1;   // 006057D8..006057E2 / 006057F3..006057FB
    }
    return 0;                                                          // 006057EB..006057F0
}

// ============================================================================================ CSWCObject leaves
// 0063C080 CSWCObject::GetModel(BYTE part, BOOL) RET 8: anim base [+68h] slot +08h GetModel(part), or 0.
void* __fastcall recovered_client_object_get_model_0063c080(void* self, void*, U part, U) noexcept(false) {
    void* const anim = rp(self, 0x68);                                 // 0063C080
    if (anim == nullptr) return nullptr;                               // 0063C083..0063C089
    return vslot<AnimGetModel>(anim, 8)(anim, nullptr, part);          // 0063C08C..0063C096
}
// 0060B310 / 0060B320 CSWCObject::TurnOnShadows / TurnOffShadows: tail jump through anim base +B8h / +BCh.
// Original anim-less path has no return value; call the exact model slot when present.
void __fastcall recovered_client_object_turn_on_shadows_0060b310(void* self, void*) noexcept(false) {
    void* const anim = rp(self, 0x68);
    if (anim == nullptr) return;
    const auto* methods = static_cast<const KotorM15eAnimBaseVtbl*>(rp(anim, 0));
    (void)methods->s0b8(anim, nullptr);
}
void __fastcall recovered_client_object_turn_off_shadows_0060b320(void* self, void*) noexcept(false) {
    void* const anim = rp(self, 0x68);
    if (anim == nullptr) return;
    const auto* methods = static_cast<const KotorM15eAnimBaseVtbl*>(rp(anim, 0));
    (void)methods->s0bc(anim, nullptr);
}
// 0063CB70 CSWCObject::SetEnvMap(BOOL a1, BOOL a2, const CResRef* env) (slot +B0h) RET 0Ch -> anim base +9Ch result.
// "DEFAULT" selects the current area's environment map [area+BCh].
U __fastcall recovered_client_object_set_env_map_0063cb70(void* self, void*, U a1, U a2, void* resref) noexcept(false) {
    ExoString text;                                                    // {E-0x24}
    exo_init(&text);                                                   // 0063CB8B..0063CB8F
    if (rp(self, 0x68) == nullptr) {                                   // 0063CB94..0063CBA1
        exo_free(&text);                                               // 0063CBA3..0063CBAF
        return 0;                                                      // 0063CBB4..0063CBC5
    }
    void* const module = kotor_fastcall_005ed530(client_app(), nullptr);   // 0063CBC8..0063CBD2 (esi)
    if (c2da_equal_text_004060d0(resref, nullptr, kDefault_0073e304) != 0) {   // 0063CBD7..0063CBEB
        if (module != nullptr && rp(module, 0x48) != nullptr) {        // 0063CBED..0063CBF6
            ResRef copy = ld<ResRef>(rp(module, 0x48), 0xBC);          // 0063CBF8..0063CC1E {E-0x1c}
            c2da_ref_to_string_00405f70(reinterpret_cast<menu_resources::Ref*>(&copy), nullptr, reinterpret_cast<menu_resources::String*>(&text));        // 0063CC11..0063CC22 CopyToString
        }
        if (async_string_length_005e5790(&text, nullptr) == 0) {       // 0063CC27..0063CC32
            exo_free(&text);                                           // 0063CC34..0063CC40
            return 0;                                                  // 0063CC45..0063CC58
        }
    } else {
        c2da_ref_to_string_00405f70(reinterpret_cast<menu_resources::Ref*>(resref), nullptr, reinterpret_cast<menu_resources::String*>(&text));           // 0063CC5B..0063CC62
    }
    ResRef map;                                                        // {E-0x1c}
    async_ref_construct_string_00406d60(&map, nullptr, &text);         // 0063CC67..0063CC70 CResRef(CExoString)
    void* const anim = rp(self, 0x68);                                 // 0063CC75
    const U result = vslot<U (__fastcall*)(void*, void*, U, U, void*)>(anim, 0x9C)(anim, nullptr, a1, a2, &map);   // 0063CC78..0063CC89
    exo_free(&text);                                                   // 0063CC8F..0063CC9D
    return result;                                                     // 0063CCA7
}

// ============================================================================================ CSWCAnimBase leaves
// 0069D4F0 CSWCAnimBase::SetAnimation(a1..a4) (slot +54h) RET 10h: slot +F0h SetAnimationInternal(model [+B8h], a1..a4).
U __fastcall recovered_anim_base_set_animation_0069d4f0(void* self, void*, U a1, U a2, U a3, U a4) noexcept(false) {
    return vslot<U (__fastcall*)(void*, void*, void*, U, U, U, U)>(self, 0xF0)(self, nullptr, rp(self, 0xB8), a1, a2, a3, a4);   // 0069D4F0..0069D513
}
// 0069D7B0 CSWCAnimBase::SetEnvMap(BOOL a1, BOOL keep, const CResRef* env) (slot +9Ch) RET 0Ch.
U __fastcall recovered_anim_base_set_env_map_0069d7b0(void* self, void*, U a1, U keep, void* resref) noexcept(false) {
    void* const gob = rp(self, 0xB8);                                  // 0069D7B6
    if (gob == nullptr) return 0;                                      // 0069D7BC..0069D7C6
    if (a1 != 0) {                                                     // 0069D7CE..0069D7D5
        char name[0x14];                                               // {E-0x14}
        gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(resref), nullptr, name);   // 0069D7D7..0069D7DE CopyToString(char*)
        vslot<GobText>(rp(self, 0xB8), 0x48)(rp(self, 0xB8), nullptr, name);   // 0069D7E3..0069D7F7
    } else {
        vslot<GobText>(gob, 0x48)(gob, nullptr, kEmpty_0073d71c);      // 0069D7F0..0069D7F7
    }
    if (keep != 0) {                                                   // 0069D7FA..0069D800
        ResRef tmp;                                                    // {E-0x24} hidden return
        c2da_copy_00406120(reinterpret_cast<menu_resources::Ref*>(at(self, 0x40)), nullptr, reinterpret_cast<menu_resources::Ref*>(&tmp), reinterpret_cast<menu_resources::Ref*>(resref));     // 0069D802..0069D80B
        st<U>(self, 0x50, a1);                                         // 0069D810
    }
    return 1;                                                          // 0069D815..0069D81E
}
// 0069DC30 CSWCAnimBase::TurnOffShadows (slot +BCh): tail jump through model [+B8h] slot +BCh.
void __fastcall recovered_anim_base_turn_off_shadows_0069dc30(void* self, void*) noexcept(false) {
    void* const gob = rp(self, 0xB8);                                  // 0069DC30
    if (gob == nullptr) return;                                      // 0069DC36..0069DC38 / 0069DC42 (EAX unspecified)
    vslot<U (__fastcall*)(void*, void*)>(gob, 0xBC)(gob, nullptr);   // 0069DC3A..0069DC3C
}

// ============================================================================================ CSWCAnimBaseDoor
// 006E56A0 CSWCAnimBaseDoor::CSWCAnimBaseDoor: CSWCAnimBase base (REAL 0069DFB0), own table, [+C4h] = 5.
void* __fastcall recovered_anim_base_door_construct_006e56a0(void* self, void*) noexcept(false) {
    kotor_m15e_anim_base_construct_0069dfb0(self, nullptr);            // 006E56A1..006E56A3
    st<const void*>(self, 0, recovered_anim_base_door_methods_00758a08);   // 006E56A8
    st<U8>(self, 0xC4, 5);                                             // 006E56AE
    return self;                                                       // 006E56B5
}
// 006E5710 CSWCAnimBaseDoor::LoadModel(const CResRef*, BYTE part) (slot +0Ch) RET 8: only part 0FFh; NewCAurObject
// (REAL render core) with gob class "door"; model stored at [+B8h], resref at [+2Ch].
U __fastcall recovered_anim_base_door_load_model_006e5710(void* self, void*, void* resref, U part) noexcept(false) {
    if (static_cast<U8>(part) != 0xFF) return 0;                       // 006E5710..006E572A
    char name[0x24];                                                   // {E-0xa4}
    gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(resref), nullptr, name);    // 006E572E..006E573C
    void* const gob = aurora_new_aur_object_00449cc0(name, kDoor_00741e80, nullptr, nullptr);   // 006E5741..006E5754
    st<void*>(self, 0xB8, gob);                                        // 006E5759
    if (gob == nullptr) {                                              // 006E5757..006E575F
        char message[0x80];                                            // {E-0x80} diagnostic text, never consumed
        format_006fadb0(message, kCannotLoadDoor_00758b00, name);      // 006E5761..006E5775 sprintf
        return 0;                                                      // 006E5778..006E5782
    }
    ResRef tmp;                                                        // {E-0x90} hidden return
    c2da_copy_00406120(reinterpret_cast<menu_resources::Ref*>(at(self, 0x2C)), nullptr, reinterpret_cast<menu_resources::Ref*>(&tmp), reinterpret_cast<menu_resources::Ref*>(resref));         // 006E5785..006E578E
    st<U>(self, 0x3C, 0);                                              // 006E5794
    return 1;                                                          // 006E579B..006E57A7
}
// 006E5AC0 CSWCAnimBaseDoor::ServerToClientAnimation(WORD) (slot +E0h) RET 4 -> AX client animation row.
// The upper EAX half is unspecified in the original (callers use the WORD); the result here is zero-extended.
U __fastcall recovered_anim_base_door_server_to_client_animation_006e5ac0(void* self, void*, U anim) noexcept(false) {
    const U value = anim & 0xFFFFu;                                    // 006E5AC7..006E5AD1 movzx
    if (value == 0x2758) return rd32(self, 0xD0) != 0 ? 0x14Au : 0x147u;   // 006E5AD3..006E5ADA / 006E5B2E..006E5B4B
    if (value < 0x2758) {                                              // 006E5ADC..006E5AF1 (table 006E5BC8 / 006E5BDC)
        if (value == 0x271E) return 0x148;                             // 006E5B68
        if (value == 0x2726) return 0x14D;                             // 006E5B1C
        if (value == 0x2742) return 0x14B;                             // 006E5AF8
        if (value == 0x2743) return 0x14C;                             // 006E5B0A
    } else {                                                           // 006E5B4E..006E5B61 (table 006E5C04 / 006E5C14)
        if (value == 0x275D || (value >= 0x27E5 && value <= 0x27E8)) return 0x148;   // 006E5B68
        if (value == 0x27A9) return 0x16E;                             // 006E5B8C
        if (value == 0x281D) return 0x158;                             // 006E5B7A
    }
    if (value > 0x270F) {                                              // 006E5B9E..006E5BA3
        char message[0x100];                                           // {E-0x100} diagnostic text, never consumed
        format_006fadb0(message, kFailedToMap_007589a8, value);        // 006E5BA5..006E5BB5 sprintf
    }
    return value;                                                      // 006E5BB8 mov ax, si
}
// 006E59C0 CSWCAnimBaseDoor::GetAnimationName(CExoString* out, WORD) (slot +40h) RET 8 -> out: animations.2da name
// column ([007A2174]) of the client row from slot +E0h; "" when absent (a diagnostic string is formatted and dropped).
void* __fastcall recovered_anim_base_door_get_animation_name_006e59c0(void* self, void*, void* out, U anim) noexcept(false) {
    const U client = vslot<std::uint16_t (__fastcall*)(void*, void*, U)>(self, 0xE0)(self, nullptr, anim);   // 006E59DA..006E59F3 (esi)
    ExoString name;                                                    // {E-0x8}
    exo_init(&name);                                                   // 006E59EF..006E59F5
    if (c2da_current_get_string_entry_00413190(reinterpret_cast<c2da_current::Table*>(two_da(0x3C)), nullptr, static_cast<I32>(client & 0xFFFFu), static_cast<I32>(kotor_rules_cell32_007a2174), reinterpret_cast<kotor_reconstruction::StartupString*>(&name)) == 0)   // 006E59FA..006E5A2A
        c2da_assign_text_005e5140(&name, nullptr, kEmpty_0073d71c);    // 006E5A2C..006E5A35
    if (static_cast<I32>(async_string_length_005e5790(&name, nullptr)) <= 0) {   // 006E5A3A..006E5A45 (jg)
        ExoString message;                                             // diagnostic, destroyed unused
        exo_init(&message);                                            // 006E5A47..006E5A4B
        kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(&message),
                                                    kNoAnimName_00758b20, anim & 0xFFFFu);   // 006E5A50..006E5A68
        exo_free(&message);                                            // 006E5A6B..006E5A74
    }
    r8_copy_construct_005e5b00(out, nullptr, &name);                   // 006E5A79..006E5A84
    exo_free(&name);                                                   // 006E5A89..006E5A9A
    return out;                                                        // 006E5AA4
}
// 006E57B0 CSWCAnimBaseDoor::SetAnimationInternal(Gob*, WORD anim, float speed, int, int a5) (slot +F0h) RET 14h.
// Plays the named animation on the model through Gob slot +18h with mode 2 (open/close states) or 3 (one-shot).
U __fastcall recovered_anim_base_door_set_animation_internal_006e57b0(void* self, void*, void* gob, U anim, U speed, U,
                                                                     U a5) noexcept(false) {
    if (gob == nullptr) return 0;                                      // 006E57CA..006E57D0 / 006E58E8..006E58FA
    ExoString name;                                                    // {E-0x14} hidden return
    vslot<void* (__fastcall*)(void*, void*, void*, U)>(self, 0x40)(self, nullptr, &name, anim);   // 006E57D6..006E57E2
    if (fbits(speed) == fbits(kZero_0073d700)) speed = kOne_3f800000; // 006E57E5..006E57FE fcomp; jp -> keep
    const U value = anim & 0xFFFFu;                                    // 006E5806 movzx eax, si
    U mode = 0;                                                        // 0 = 006E5845 default branch
    if (value > 0x2743) {                                              // 006E5809..006E580E
        const U index = value - 0x2758;                                // 006E58AC
        if (index <= 0x90) {                                           // 006E58B1..006E58BF (table 006E5900 / 006E590C)
            if (value == 0x2758 || value == 0x27A9) mode = 2;          // 006E588D
            else if (value == 0x275D || (value >= 0x27E5 && value <= 0x27E8)) mode = 3;   // 006E58C6
        }
    } else if (value >= 0x2742) {                                      // 006E5814..006E5819
        mode = 2;                                                      // 006E588D
    } else if (value > 0x271E) {                                       // 006E581B..006E5820 / 006E5886..006E588B
        if (value == 0x2726) mode = 2;
    } else if (value == 0x271E || value == 0x149 || (value > 0x14D && value <= 0x151)) {   // 006E5822..006E583F
        mode = 3;                                                      // 006E58C6
    }
    using Play = U (__fastcall*)(void*, void*, const char*, U, U, U);  // Gob +18h PlayAnimation RET 10h
    if (mode == 0)
        vslot<Play>(gob, 0x18)(gob, nullptr, kEmpty_0073d71c, kOne_3f800000, 0, a5);   // 006E5845..006E585A
    else
        vslot<Play>(gob, 0x18)(gob, nullptr, async_string_cstr_005e5670(&name, nullptr), speed, mode, a5);   // 006E588D..006E58A7 / 006E58C6..006E58E0
    exo_free(&name);                                                   // 006E585D..006E5869
    return 1;                                                          // 006E586E..006E5883
}

// ============================================================================================ CSWCAnimBasePlaceable
// 006E4E50 CSWCAnimBasePlaceable::CSWCAnimBasePlaceable: CSWCAnimBase base, own table, [+C4h] = 0Ah.
void* __fastcall recovered_anim_base_placeable_construct_006e4e50(void* self, void*) noexcept(false) {
    kotor_m15e_anim_base_construct_0069dfb0(self, nullptr);            // 006E4E51..006E4E53
    st<const void*>(self, 0, recovered_anim_base_placeable_methods_007588b0);   // 006E4E58
    st<U8>(self, 0xC4, 0x0A);                                          // 006E4E5E
    return self;                                                       // 006E4E65
}
// 006E4EC0 CSWCAnimBasePlaceable::LoadModel(const CResRef*, BYTE) (slot +0Ch) RET 8: NewCAurObject gob "placeable".
U __fastcall recovered_anim_base_placeable_load_model_006e4ec0(void* self, void*, void* resref, U) noexcept(false) {
    char name[0x14];                                                   // {E-0x14}
    gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(resref), nullptr, name);    // 006E4EC5..006E4ED2
    void* const gob = aurora_new_aur_object_00449cc0(name, kPlaceable_00741e68, nullptr, nullptr);   // 006E4ED7..006E4EEA
    st<void*>(self, 0xB8, gob);                                        // 006E4EEF
    if (gob == nullptr) return 0;                                      // 006E4EED..006E4EFC (EAX = gob = 0)
    ResRef tmp;                                                        // {E-0x24} hidden return
    c2da_copy_00406120(reinterpret_cast<menu_resources::Ref*>(at(self, 0x2C)), nullptr, reinterpret_cast<menu_resources::Ref*>(&tmp), reinterpret_cast<menu_resources::Ref*>(resref));         // 006E4EFF..006E4F08
    st<U>(self, 0x3C, 0);                                              // 006E4F0E
    return 1;                                                          // 006E4F15..006E4F1E
}

// ============================================================================================ CSWCDoor
// 00683BB0 CSWCDoor::CSWCDoor (0x144 bytes): CSWCObject(10) base (REAL 0063E530), table 00753960, three CExoStrings.
void* __fastcall recovered_client_door_construct_00683bb0(void* self, void*) noexcept(false) {
    kotor_m15a_object_construct_0063e530(self, nullptr, kDoor_00746639);   // 00683BC8..00683BD9
    st<const void*>(self, 0, recovered_client_door_methods_00753960);  // 00683BEA
    exo_init(at(self, 0xFC));                                          // 00683BE0..00683BF0
    exo_init(at(self, 0x130));                                         // 00683BF5..00683C02
    exo_init(at(self, 0x13C));                                         // 00683C07..00683C12
    st<U16>(self, 0x48, 0x2726);                                       // 00683C17 current animation
    U time[2];                                                         // {E-0x18} / {E-0x14}
    frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, &time[0], &time[1]);   // 00683C1D..00683C3C
    st<U>(self, 0x3C, time[0]);                                        // 00683C41..00683C4B
    st<U>(self, 0x40, time[1]);                                        // 00683C45..00683C55
    st<U>(self, 0x108, 0);                                             // 00683C58
    st<U>(self, 0x10C, 0);                                             // 00683C5E
    st<U>(self, 0x110, 0);                                             // 00683C64
    st<U>(self, 0x114, 0);                                             // 00683C6A
    st<U>(self, 0x104, 0);                                             // 00683C70
    st<U>(self, 0x120, 0);                                             // 00683C76
    st<U>(self, 0x12C, 0);                                             // 00683C7C
    st<U>(self, 0x118, 1);                                             // 00683C82
    st<U8>(self, 0x124, 0xFF);                                         // 00683C49 / 00683C8C
    st<U8>(self, 0x125, 0xFF);                                         // 00683C92
    c2da_assign_text_005e5140(at(self, 0x130), nullptr, kEmpty_0073d71c);   // 00683C4E..00683C98
    st<U>(self, 0x128, 0);                                             // 00683CA1
    st<U>(self, 0x138, 0);                                             // 00683CA7
    return self;                                                       // 00683CAE
}
// 006842E0 CSWCDoor::LoadModel(const CResRef*, BYTE part, BYTE) (slot +78h) RET 0Ch.
U __fastcall recovered_client_door_load_model_006842e0(void* self, void*, void* resref, U part, U) noexcept(false) {
    if (rp(self, 0x68) == nullptr) {                                   // 006842F9..006842FE
        void* const memory = live_operator_new(0xF0);                  // 00684300..0068430A
        void* const anim = memory != nullptr ? recovered_anim_base_door_construct_006e56a0(memory, nullptr) : nullptr;   // 00684311..00684326
        st<void*>(self, 0x68, anim);                                   // 0068432E
        // 00684328..00684331: [anim+D0h] = [this+118h]; the original stores before its null test (a failed
        // allocation faults there); the reconstruction skips the store for a null anim base.
        if (anim != nullptr) st<U>(anim, 0xD0, rd32(self, 0x118));
        if (rp(self, 0x68) == nullptr) return 0;                       // 00684337..00684357
    }
    void* anim = rp(self, 0x68);                                       // 006842F9 / 00684337 (ecx)
    if (vslot<AnimLoadModel>(anim, 0x0C)(anim, nullptr, resref, part) == 0) return 0;   // 0068435A..0068436D / 0068436F..00684380
    anim = rp(self, 0x68);                                             // 00684383
    void* const gob = vslot<AnimGetModel>(anim, 8)(anim, nullptr, part);   // 00684386..0068438C
    if (gob != nullptr) {                                              // 0068438E..00684390
        vslot<void (__fastcall*)(void*, void*, void*)>(gob, 0x74)(gob, nullptr, self);   // 00684392..00684397 client object
        vslot<GobU>(gob, 0x7C)(gob, nullptr, kDoor_00746639);          // 0068439A..006843A6 object type
        using Callback = void (__fastcall*)(void*, void*, const void*, const char*, void*, U, U);   // Gob +28h RET 14h
        vslot<Callback>(gob, 0x28)(gob, nullptr, reinterpret_cast<const void*>(&recovered_door_move_next_door_state_00683f00),
                                   kEmpty_0073d71c, self, kNineNineNineNine_461c3c00, 0);   // 006843A9..006843BF
    }
    c2da_ref_to_string_00405f70(reinterpret_cast<menu_resources::Ref*>(resref), nullptr, reinterpret_cast<menu_resources::String*>(at(self, 0x130)));     // 006843C2..006843CB CopyToString
    return 1;                                                          // 006843D6
}
// 00683D40 CSWCDoor::SetState(BYTE state) RET 4: [+11Ch] = state; model slot +90h(state != 0) for states 0..3.
void __fastcall recovered_client_door_set_state_00683d40(void* self, void*, U state) noexcept(false) {
    void* const model = vslot<SlotGetModel>(self, 0x98)(self, nullptr, 0xFF, 1);   // 00683D43..00683D4C
    const U8 value = static_cast<U8>(state);                           // 00683D56
    st<U8>(self, 0x11C, value);                                        // 00683D5A
    if (model == nullptr) return;                                      // 00683D54 / 00683D61
    if (value == 0) {                                                  // 00683D63..00683D65
        vslot<GobU>(model, 0x90)(model, nullptr, 0);                   // 00683D7D..00683D87 (tail jump, argument slot reused)
    } else if (value <= 3) {                                           // 00683D67..00683D6B
        vslot<GobU>(model, 0x90)(model, nullptr, 1);                   // 00683D6D..00683D77
    }
}
// 00683D00 CSWCDoor::SetAnimationWithoutTrigger(WORD anim) RET 4.
void __fastcall recovered_client_door_set_animation_without_trigger_00683d00(void* self, void*, U anim) noexcept(false) {
    st<U16>(self, 0x48, static_cast<U16>(anim));                       // 00683D09
    if (kotor_m15a_object_animation_looping_0063c300(self, nullptr, anim) != 0)   // 00683D08..00683D14
        kotor_m15a_object_set_looping_animation_0063d830(self, nullptr, anim);   // 00683D16..00683D19
    void* const anim_base = rp(self, 0x68);                            // 00683D1E
    vslot<U (__fastcall*)(void*, void*, U, U, U, U)>(anim_base, 0x54)(anim_base, nullptr, anim, 0, 0, 0);   // 00683D21..00683D2A
}
// 00683CC0 CSWCDoor::SetIsAreaTransition(BOOL) RET 4: a non-transition door model gets Gob +B0h("trans").
void __fastcall recovered_client_door_set_is_area_transition_00683cc0(void* self, void*, U flag) noexcept(false) {
    if (flag == 0) {                                                   // 00683CC6..00683CCA
        void* const model = vslot<SlotGetModel>(self, 0x98)(self, nullptr, 0xFF, 1);   // 00683CCC..00683CD5
        if (model != nullptr) vslot<GobText>(model, 0xB0)(model, nullptr, kTrans_00753aa8);   // 00683CDB..00683CE8
    }
    st<U>(self, 0x12C, flag);                                          // 00683CEE
}

// ============================================================================================ CSWCPlaceable
// 00682270 CSWCPlaceable::CSWCPlaceable (0x150 bytes): CSWCObject(9) base, table 007537D0, three CExoStrings.
void* __fastcall recovered_client_placeable_construct_00682270(void* self, void*) noexcept(false) {
    kotor_m15a_object_construct_0063e530(self, nullptr, kPlaceable_00746638);   // 00682289..0068229A
    st<const void*>(self, 0, recovered_client_placeable_methods_007537d0);   // 006822AD
    exo_init(at(self, 0xFC));                                          // 0068229F..006822B3
    exo_init(at(self, 0x104));                                         // 006822B8..006822C5
    exo_init(at(self, 0x144));                                         // 006822CA..006822D5
    st<U16>(self, 0x48, 0x275C);                                       // 006822DA current animation
    U time[2];                                                         // {E-0x18} / {E-0x14}
    frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, &time[0], &time[1]);   // 006822E0..006822FF
    st<U>(self, 0x3C, time[0]);                                        // 00682304..00682311
    st<U>(self, 0x40, time[1]);                                        // 00682308..0068231B
    st<U>(self, 0x11C, 0);                                             // 0068231E
    st<U>(self, 0x124, 0);                                             // 00682324
    st<U>(self, 0x110, 0);                                             // 0068232A
    st<U>(self, 0x114, 0);                                             // 00682330
    st<U>(self, 0x118, 0);                                             // 00682336
    st<U>(self, 0x128, 0);                                             // 0068233C
    st<U>(self, 0x12C, 1);                                             // 00682342
    st<U>(self, 0x130, 1);                                             // 00682348
    st<U>(self, 0x13C, 0);                                             // 0068234E
    st<U>(self, 0x120, 0);                                             // 00682354
    st<U8>(self, 0x10C, 0xFF);                                         // 0068235A appearance
    st<U>(self, 0x134, 0xFFFFFFFFu);                                   // 00682361
    st<U>(self, 0x138, 0);                                             // 0068236B
    c2da_assign_text_005e5140(at(self, 0xFC), nullptr, kEmpty_0073d71c);    // 00682314..00682371
    c2da_assign_text_005e5140(at(self, 0x104), nullptr, kEmpty_0073d71c);   // 00682376..0068237D
    c2da_assign_text_005e5140(at(self, 0x144), nullptr, kEmpty_0073d71c);   // 00682382..0068238D
    st<U>(self, 0x14C, 0);                                             // 00682396 light
    st<U>(self, 0x140, 0);                                             // 0068239C
    return self;                                                       // 006823A3
}
// 006823F0 CSWCPlaceable::LoadModel(const CResRef*, BYTE part, BYTE) (slot +78h) RET 0Ch. Also records the model
// name without its 4-character prefix at [+144h] and the height of node "<name>_head_hit" at [+A4h] (1.5 default).
U __fastcall recovered_client_placeable_load_model_006823f0(void* self, void*, void* resref, U part, U) noexcept(false) {
    if (rp(self, 0x68) == nullptr) {                                   // 0068240B..00682410
        void* const memory = live_operator_new(0xF0);                  // 00682412..0068241C
        void* const anim = memory != nullptr ? recovered_anim_base_placeable_construct_006e4e50(memory, nullptr) : nullptr;   // 00682423..00682438
        st<void*>(self, 0x68, anim);                                   // 00682444
        if (anim == nullptr) return 0;                                 // 0068243A..00682458
    }
    void* anim = rp(self, 0x68);                                       // 0068245B
    if (vslot<AnimLoadModel>(anim, 0x0C)(anim, nullptr, resref, part) == 0) return 0;   // 0068245E..00682471 / 00682473..00682484
    anim = rp(self, 0x68);                                             // 00682487
    void* const gob = vslot<AnimGetModel>(anim, 8)(anim, nullptr, part);   // 0068248A..0068248E (edi)
    if (gob != nullptr) {                                              // 00682491..00682495
        vslot<void (__fastcall*)(void*, void*, void*)>(gob, 0x74)(gob, nullptr, self);   // 00682497..0068249C
        vslot<GobU>(gob, 0x7C)(gob, nullptr, kPlaceable_00746638);     // 0068249F..006824AB
    }
    void* const model_name = at(self, 0x144);                          // 006824AE (esi)
    c2da_ref_to_string_00405f70(reinterpret_cast<menu_resources::Ref*>(resref), nullptr, reinterpret_cast<menu_resources::String*>(model_name));          // 006824B4..006824B7 CopyToString
    ExoString tail;                                                    // {E-0x38} hidden return
    startup_rand::SubString005e6270(static_cast<kotor_reconstruction::StartupString*>(model_name), nullptr,
                                    reinterpret_cast<kotor_reconstruction::StartupString*>(&tail), 4, -1);   // 006824BC..006824C7
    async_assign_string_005e5c50(model_name, nullptr, &tail);          // 006824CC..006824D7
    exo_free(&tail);                                                   // 006824DC..006824E8
    float quaternion[4] = {fbits(kOne_3f800000), 0.0f, 0.0f, 0.0f};    // 006824F1..00682509 {E-0x1c..E-0x10}
    float position[3] = {0.0f, 0.0f, 0.0f};                            // 00682511..00682521 {E-0x28..E-0x20}
    ExoString node;                                                    // {E-0x40}
    exo_init(&node);                                                   // 006824ED..00682529
    ExoString suffix;                                                  // {E-0x48}
    exo_text(&suffix, kHeadHit_00753918);                              // 0068252E..0068253F
    ExoString joined;                                                  // {E-0x30} hidden return of operator+
    GuiConcat005e5d10(static_cast<kotor_reconstruction::StartupString*>(model_name), nullptr,
                      reinterpret_cast<kotor_reconstruction::StartupString*>(&joined),
                      reinterpret_cast<const kotor_reconstruction::StartupString*>(&suffix));   // 00682544..00682556
    async_assign_string_005e5c50(&node, nullptr, &joined);             // 0068255B..00682565
    exo_free(&joined);                                                 // 0068256A..00682572
    exo_free(&suffix);                                                 // 00682577..00682580
    // 00682585..0068259D: Gob +A0h(node name, &position, &quaternion) -> node found; the original dereferences the
    // model unconditionally here (a model-less LoadModel faults), as reproduced.
    using NodeQuery = U (__fastcall*)(void*, void*, const char*, float*, float*);
    if (vslot<NodeQuery>(gob, 0xA0)(gob, nullptr, async_string_cstr_005e5670(&node, nullptr), position, quaternion) != 0)
        st<float>(self, 0xA4, position[2]);                            // 006825A3..006825AC (z, copied as bits)
    else
        st<U>(self, 0xA4, kOneAndHalf_3fc00000);                       // 006825B4
    exo_free(&node);                                                   // 006825BE..006825CA
    return 1;                                                          // 006825D5
}
// 00682E20 CSWCPlaceable::SetAppearance(BYTE) RET 4: [+10Ch] = appearance; placeables.2da HitCheck drives model
// Gob +90h; NonCull is read (result unused) when an anim base exists.
void __fastcall recovered_client_placeable_set_appearance_00682e20(void* self, void*, U appearance) noexcept(false) {
    const U8 row = static_cast<U8>(appearance);                        // 00682E39 bl
    st<U8>(self, 0x10C, row);                                          // 00682E4A
    U hit_check = appearance;                                          // {E+0x4}: the argument slot is the output cell
    ExoString column;                                                  // {E-0x1c}
    exo_text(&column, kHitCheck_00753944);                             // 00682E41..00682E58
    (void)c2da_current_get_int_00413660(two_da(0x2C), nullptr, row, &column, &hit_check);   // 00682E5D..00682E83
    exo_free(&column);                                                 // 00682E88..00682E93
    void* const model = vslot<SlotGetModel>(self, 0x98)(self, nullptr, 0xFF, 1);   // 00682E98..00682EA3
    if (model != nullptr)                                              // 00682EA9..00682EAB
        // 00682EAD..00682EBB: ECX = HitCheck; sete cl -> only the low byte becomes (HitCheck == 0)
        vslot<GobU>(model, 0x90)(model, nullptr, (hit_check & 0xFFFFFF00u) | (hit_check == 0 ? 1u : 0u));
    if (rp(self, 0x68) != nullptr) {                                   // 00682EC1..00682EC6
        U non_cull = 0;                                                // {E-0x20} (uninitialised in the original; never read)
        exo_text(&column, kNonCull_0075393c);                          // 00682EC8..00682ED1
        (void)c2da_current_get_int_00413660(two_da(0x2C), nullptr, row, &column, &non_cull);   // 00682ED6..00682EFA
        exo_free(&column);                                             // 00682EFF..00682F07
    }
}
// 006826E0 CSWCPlaceable::LoadLight RET -> 1 (no light / attached) or 0 (no model / light object).
// placeables.2da LightColor selects a lightcolor.2da row; the light model "fx_placeable01" (or the one the area
// pre-created for this id) is attached to the placeable model at "root".
U __fastcall recovered_client_placeable_load_light_006826e0(void* self, void*) noexcept(false) {
    float rgb[3] = {0.0f, 0.0f, 0.0f};                                 // 00682708..00682718 {E-0x30..E-0x28}
    ExoString node;                                                    // {E-0x64}
    exo_init(&node);                                                   // 00682704..00682720
    ExoString unused;                                                  // {E-0x5c} (never assigned)
    exo_init(&unused);                                                 // 00682725..00682731
    ExoString column;                                                  // {E-0x54}
    exo_text(&column, kLightColor_007494bc);                           // 00682736..00682744
    U light_color = 0;                                                 // {E-0x68} (uninitialised in the original; read only on success)
    const U8 missing = c2da_current_get_int_00413660(two_da(0x2C), nullptr, ld<U8>(self, 0x10C), &column, &light_color) == 0
                           ? 1 : 0;                                    // 00682749..00682782
    exo_free(&column);                                                 // 0068277E..00682789
    U result = 1;
    if (missing == 0) {                                                // 0068278E..00682790
        void* area = nullptr;                                          // 0068279F edi
        if (kotor_fastcall_005ed530(client_app(), nullptr) != nullptr) {   // 00682796..006827A8
            area = rp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48);   // 006827AA..006827B7
            if (area != nullptr)                                       // 006827BA..006827BC
                st<void*>(self, 0x14C, recovered_client_area_get_placeable_object_light_00605bf0(area, nullptr, rd32(self, 4)));   // 006827BE..006827C9
        }
        exo_text(&column, kLightNode_00753924);                        // 006827CF..006827D8
        const U8 no_node = c2da_current_get_string_00413270(two_da(0x2C), nullptr, ld<U8>(self, 0x10C), &column, &node) == 0
                               ? 1 : 0;                                // 006827DD..00682817
        exo_free(&column);                                             // 00682813..0068281E
        if (no_node != 0) c2da_assign_text_005e5140(&node, nullptr, kImpact_00743318);   // 00682823..00682830
        void* const model = vslot<SlotGetModel>(self, 0x98)(self, nullptr, 0xFF, 1);   // 00682835..00682846 (ebp)
        if (model == nullptr) {                                        // 00682848..0068284A
            result = 0;                                                // 006828CA..006828FD
        } else {
            if (rp(self, 0x14C) == nullptr) {                          // 0068284C..00682854
                float quaternion[4] = {fbits(kOne_3f800000), 0.0f, 0.0f, 0.0f};   // 00682868..00682880 {E-0x24..E-0x18}
                float position[3] = {0.0f, 0.0f, 0.0f};                // 00682888..00682898 {E-0x3c..E-0x34}
                float offset[3] = {0.0f, 0.0f, 0.0f};                  // 006828A0..006828B0 {E-0x48..E-0x40}
                void* const light = aurora_new_aur_object_00449cc0(kFxPlaceable01_0074ef58, kPlaceableLight_0074ef68,
                                                                   nullptr, nullptr);   // 0068285A..006828BD
                st<void*>(self, 0x14C, light);                         // 006828C2
                if (light == nullptr) {                                // 006828C0..006828C8
                    result = 0;                                        // -> 006828CA
                } else {
                    const char* const colours[3] = {kRed_0074ef54, kGreen_0074ef4c, kBlue_0074ef44};
                    for (unsigned k = 0; k < 3; ++k) {                 // 006828FE..006829D3 lightcolor.2da RED/GREEN/BLUE
                        exo_text(&column, colours[k]);
                        (void)c2da_current_get_float_00413430(two_da(0x48), nullptr, static_cast<int>(light_color), &column, &rgb[k]);
                        exo_free(&column);
                    }
                    const char* const offsets[3] = {kLightOffsetX_0074ef34, kLightOffsetY_0074ef24, kLightOffsetZ_0074ef14};
                    const U defaults[3] = {0, 0, kThree_40400000};     // 00682A30 / 00682A90 / 00682AF0
                    for (unsigned k = 0; k < 3; ++k) {                 // 006829D8..00682AF0 placeables.2da LightOffsetX/Y/Z
                        exo_text(&column, offsets[k]);
                        const U8 absent = c2da_current_get_float_00413430(two_da(0x2C), nullptr, ld<U8>(self, 0x10C), &column,
                                                                          &offset[k]) == 0 ? 1 : 0;
                        exo_free(&column);
                        if (absent != 0) offset[k] = fbits(defaults[k]);
                    }
                    using NodeQuery = U (__fastcall*)(void*, void*, const char*, float*, float*);
                    (void)vslot<NodeQuery>(light, 0xA0)(light, nullptr, kRoot_00740c00, position, quaternion);   // 00682AF8..00682B0F
                    // 00682B15..00682B5D: x87 sums of two floats, each rounded to float by fstp
                    const float x = static_cast<float>(static_cast<double>(position[0]) + static_cast<double>(offset[0]));
                    const float y = static_cast<float>(static_cast<double>(position[1]) + static_cast<double>(offset[1]));
                    const float z = static_cast<float>(static_cast<double>(position[2]) + static_cast<double>(offset[2]));
                    // 00682B35..00682B7F: Gob +9Ch(name, Vector by value, Quaternion by value) RET 20h
                    using SetNode = U (__fastcall*)(void*, void*, const char*, U, U, U, U, U, U, U);
                    void* const light_now = rp(self, 0x14C);           // 00682B25
                    (void)vslot<SetNode>(light_now, 0x9C)(light_now, nullptr, kRoot_00740c00, bitsf(x), bitsf(y), bitsf(z),
                                                          bitsf(quaternion[0]), bitsf(quaternion[1]), bitsf(quaternion[2]),
                                                          bitsf(quaternion[3]));
                }
            }
            if (result != 0) {
                void* const light = rp(self, 0x14C);                   // 00682B85
                using Attach = U (__fastcall*)(void*, void*, void*, const char*, U);   // Gob +50h RET 0Ch
                (void)vslot<Attach>(light, 0x50)(light, nullptr, model, kRoot_00740c00, 0);   // 00682B8B..00682B95
                if (area != nullptr)                                   // 00682B98..00682B9A
                    recovered_client_area_remove_placeable_object_light_00605b50(area, nullptr, rd32(self, 4), 0);   // 00682B9C..00682BA4
            }
        }
    }
    exo_free(&unused);                                                 // 00682BA9..00682BB2 / 006828CA..006828D3
    exo_free(&node);                                                   // 00682BB7..00682BC3 / 006828D8..006828E4
    return result;                                                     // 00682BCD / 006828E9
}
// 00683630 CSWCPlaceable::SetAnimation(WORD, int, int) (slot +9Ch) RET 0Ch: a server corpse placeable keeps its
// state; animations 132h / 2758h unload the light; then CSWCObject::SetAnimation (REAL 0063E930).
U __fastcall recovered_client_placeable_set_animation_00683630(void* self, void*, U anim, U a2, U a3) noexcept(false) {
    if (kotor_m15a_object_get_server_object_0063d4b0(self, nullptr) != nullptr) {   // 00683633..0068363A
        void* const server = kotor_m15a_object_get_server_object_0063d4b0(self, nullptr);   // 0068363C..0068363E
        void* const placeable = vslot<void* (__fastcall*)(void*, void*)>(server, 0x48)(server, nullptr);   // 00683643..00683647
        if (placeable != nullptr && rd32(placeable, 0x44C) != 0) return 1;   // 0068364A..0068365E
    }
    const U16 value = static_cast<U16>(anim);                          // 00683662 edi
    if (value == 0x132 || value == 0x2758)                             // 00683666..00683672
        recovered_client_placeable_unload_light_00682be0(self, nullptr);   // 00683674..00683676
    return kotor_m15e_object_set_animation_0063e930(self, nullptr, anim, a2, a3);   // 0068367B..00683688
}

// ============================================================================================ message handlers
// 0064CF30 CSWCMessage::HandleServerToPlayerDoorUpdate_Add. ECX this, RET. EAX = 1 / 0.
// Record: OBJECT_ID, BOOL hitcheck-ignore, BYTE generic type [, BYTE door type when generic == 0], CExoLocString
// name, BOOL (unused), WORD animation state, 4 BOOL, CExoLocString.
U __fastcall recovered_client_handle_door_add_0064cf30(void* self, void*) {
    ExoString model;                                                   // {E-0x3c}
    exo_init(&model);                                                  // 0064CF54..0064CF58
    ExoString name;                                                    // {E-0x2c}
    exo_init(&name);                                                   // 0064CF5D..0064CF69
    ExoString description;                                             // {E-0x34}
    exo_init(&description);                                            // 0064CF6E..0064CF77
    U visible_model = 1;                                               // 0064CF81 {E-0x68}
    void* const objects = kotor_fastcall_005ed570(client_app(), nullptr);   // 0064CF7C..0064CFA3 {E-0x64}
    void* const area = rp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48);   // 0064CF9A..0064CFAC (ebp)
    U id = kotor_fastcall_00692630(self, nullptr);   // 0064CFAF..0064CFB8 {E-0x6c}
    const U ignore_hitcheck = read_bool(self);                         // 0064CFB6..0064CFC5 {E-0x50}
    const U8 generic = read_byte(self);                                // 0064CFC1..0064CFD0 {E-0x6e}
    U8 door_type = 0;                                                  // {E-0x6d}: uninitialised in the original when generic != 0
    if (generic == 0) door_type = read_byte(self);                     // 0064CFD4..0064CFDF
    ResRef sound;                                                      // {E-0x24}
    kotor_fastcall_00405ed0(&sound, nullptr);                          // 0064CFE3..0064CFE9
    ExoString read;                                                    // {E-0x4c} hidden return
    kotor_fastcall_00692660(self, nullptr, &read, &sound, nullptr);   // 0064CFEE..0064CFF6
    async_assign_string_005e5c50(&name, nullptr, &read);               // 0064CFFB..0064D008
    exo_free(&read);                                                   // 0064D00D..0064D015
    (void)read_bool(self);                                             // 0064D01A..0064D01C (discarded)
    const U animation = read_word(self);                               // 0064D021..0064D02C (edi)
    const U field_108 = read_bool(self);                               // 0064D02E..0064D035 {E-0x60}
    const U field_10c = read_bool(self);                               // 0064D039..0064D040 {E-0x5c}
    const U field_104 = read_bool(self);                               // 0064D044..0064D04B {E-0x58}
    const U area_transition = read_bool(self);                         // 0064D04F..0064D05A {E-0x54}
    kotor_fastcall_00405ed0(&sound, nullptr);                          // 0064D054..0064D05E
    kotor_fastcall_00692660(self, nullptr, &read, &sound, nullptr);   // 0064D063..0064D06B
    async_assign_string_005e5c50(&description, nullptr, &read);        // 0064D070..0064D07D
    exo_free(&read);                                                   // 0064D082..0064D08A
    if (recovered_player_message::overrun_004d6230(self) != 0) {       // 0064D08F..0064D09A
        exo_free(&description);                                        // 0064D20A..0064D213
        exo_free(&name);                                               // 0064D218..0064D221
        exo_free(&model);                                              // 0064D226..0064D232
        return 0;                                                      // 0064D237..0064D24B
    }
    ExoString column;                                                  // {E-0x4c}
    if (generic != 0) {                                                // 0064D0A0..0064D0AA
        exo_text(&column, kModel_007499a0);                            // 0064D0AC..0064D0B1
        (void)c2da_current_get_string_00413270(two_da(0x24), nullptr, generic, &column, &model);   // 0064D0B6..0064D0DF
        exo_free(&column);                                             // 0064D0E4..0064D0EC
        exo_text(&column, kVisibleModel_00749990);                     // 0064D0F1..0064D0FA
        (void)c2da_current_get_int_00413660(two_da(0x24), nullptr, generic, &column, &visible_model);   // 0064D0FF..0064D123
    } else {
        exo_text(&column, kModelName_00749814);                        // 0064D12A..0064D12F
        (void)c2da_current_get_string_00413270(two_da(0x28), nullptr, door_type, &column, &model);   // 0064D134..0064D15C
    }
    exo_free(&column);                                                 // 0064D161..0064D169
    void* door = recovered_client_get_door_by_id_005ed600(client_app(), nullptr, id);   // 0064D16E..0064D17C (esi)
    if (door == nullptr) {                                             // 0064D181..0064D185
        void* const memory = live_operator_new(0x144);                 // 0064D18B..0064D195
        door = memory != nullptr ? recovered_client_door_construct_00683bb0(memory, nullptr) : nullptr;   // 0064D19C..0064D1B0
        // the original does not test the result: a failed allocation faults at the first slot call below
        ResRef ref;                                                    // {E-0x4c}
        async_ref_construct_string_00406d60(&ref, nullptr, &model);    // 0064D1B2..0064D1C2
        vslot<SlotLoadModel>(door, 0x78)(door, nullptr, &ref, 0xFF, 5);   // 0064D1C7..0064D1D7 LoadModel
        vslot<SlotSetId>(door, 0x04)(door, nullptr, id);               // 0064D1DA..0064D1E3 SetId
        vslot<SlotSetArea>(door, 0x84)(door, nullptr, area);               // 0064D1E6..0064D1EB SetArea
        if (recovered_object_array_add_external_004d7ff0(objects, nullptr, &id, door, 1) != kGameObjectOk_0074666c) {   // 0064D1F1..0064D208
            exo_free(&description);                                    // 0064D20A..0064D213 (the door is not released)
            exo_free(&name);                                           // 0064D218..0064D221
            exo_free(&model);                                          // 0064D226..0064D232
            return 0;                                                  // 0064D237..0064D24B
        }
        recovered_client_ai_master_add_object_00604a20(kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(), nullptr),
                                                       nullptr, door);   // 0064D24C..0064D25D
    } else {
        recovered_client_object_set_visibility_on_set_area_0063e350(door, nullptr, area);   // 0064D264..0064D267
    }
    vslot<SlotPtr>(door, 0x94)(door, nullptr, rp(area, 0x184));       // 0064D26C..0064D277 AttachToScene(scene)
    async_assign_string_005e5c50(at(door, 0xFC), nullptr, &name);      // 0064D27D..0064D288
    const U16 state = static_cast<U16>(animation);                     // 0064D28D di
    if (state == 0x2726) recovered_client_door_set_state_00683d40(door, nullptr, 0);         // 0064D28D..0064D296
    else if (state == 0x2742) recovered_client_door_set_state_00683d40(door, nullptr, 1);    // 0064D298..0064D2A1
    else if (state == 0x2743) recovered_client_door_set_state_00683d40(door, nullptr, 2);    // 0064D2A3..0064D2AB (ebx)
    else if (state == 0x2758) recovered_client_door_set_state_00683d40(door, nullptr, 3);    // 0064D2AD..0064D2B8
    recovered_client_door_set_animation_without_trigger_00683d00(door, nullptr, animation);  // 0064D2BD..0064D2C0
    st<U>(door, 0x108, field_108);                                     // 0064D2C5..0064D2D1
    st<U>(door, 0x10C, field_10c);                                     // 0064D2C9..0064D2DE
    st<U>(door, 0x104, field_104);                                     // 0064D2CD..0064D2E4
    recovered_client_door_set_is_area_transition_00683cc0(door, nullptr, area_transition);   // 0064D2D7..0064D2EA
    st<U8>(door, 0x124, generic);                                      // 0064D2F3..0064D2F7
    st<U8>(door, 0x125, door_type);                                    // 0064D2EF..0064D2FD
    if (visible_model == 0) {                                          // 0064D303..0064D309
        st<U>(door, 0x104, 1);                                         // 0064D30F
        recovered_client_door_set_state_00683d40(door, nullptr, 3);    // 0064D30B..0064D319
    }
    st<U>(door, 0x138, ignore_hitcheck);                               // 0064D31E..0064D324
    if (ignore_hitcheck != 0)                                          // 0064D322..0064D32A
        recovered_client_add_to_hitcheck_ignore_list_005ed750(client_app(), nullptr, rd32(door, 4));   // 0064D32C..0064D339
    async_assign_string_005e5c50(at(door, 0x13C), nullptr, &description);   // 0064D33E..0064D349
    exo_free(&description);                                            // 0064D34E..0064D357
    exo_free(&name);                                                   // 0064D35C..0064D365
    exo_free(&model);                                                  // 0064D36A..0064D376
    return 1;                                                          // 0064D380
}

// 0064D500 CSWCMessage::HandleServerToPlayerPlaceableUpdate_Add. ECX this, RET. EAX = 1 / 0.
// Record: OBJECT_ID, CExoLocString name, BOOL, BYTE appearance, WORD animation, 7 BOOL.
U __fastcall recovered_client_handle_placeable_add_0064d500(void* self, void*) {
    void* const objects = kotor_fastcall_005ed570(client_app(), nullptr);   // 0064D51E..0064D533 {E-0x5c}
    U id = kotor_fastcall_00692630(self, nullptr);   // 0064D531..0064D543 {E-0x74}
    ResRef sound;                                                      // {E-0x24}
    kotor_fastcall_00405ed0(&sound, nullptr);                          // 0064D53C..0064D547
    ExoString name;                                                    // {E-0x2c} hidden return
    kotor_fastcall_00692660(self, nullptr, &name, &sound, nullptr);   // 0064D54C..0064D554
    const U field_124 = read_bool(self);                               // 0064D559..0064D568 {E-0x3c}
    const U8 appearance = read_byte(self);                             // 0064D564..0064D577 (bl, {E-0x60})
    const U animation = read_word(self);                               // 0064D573..0064D582 {E-0x38}
    const U field_110 = read_bool(self);                               // 0064D586..0064D58D {E-0x58}
    const U field_128 = read_bool(self);                               // 0064D591..0064D598 {E-0x54}
    const U field_130 = read_bool(self);                               // 0064D59C..0064D5A3 {E-0x50}
    const U field_114 = read_bool(self);                               // 0064D5A7..0064D5AE {E-0x4c}
    const U field_118 = read_bool(self);                               // 0064D5B2..0064D5B9 {E-0x48}
    const U field_12c = read_bool(self);                               // 0064D5BD..0064D5C4 {E-0x44}
    const U field_120 = read_bool(self);                               // 0064D5C8..0064D5D1 {E-0x40}
    if (recovered_player_message::overrun_004d6230(self) != 0) {       // 0064D5CD..0064D5DC
        exo_free(&name);                                               // 0064D874..0064D880
        return 0;                                                      // 0064D885..0064D899
    }
    void* const area = rp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48);   // 0064D5E2..0064D5F0 (ebp)
    void* placeable = recovered_client_get_placeable_by_id_005ed5f0(client_app(), nullptr, id);   // 0064D5F3..0064D600 (esi)
    if (placeable == nullptr) {                                        // 0064D605..0064D609
        ExoString model;                                               // {E-0x34}
        exo_init(&model);                                              // 0064D60F..0064D613
        ExoString column;                                              // {E-0x70}
        if (ld<U8>(kotor_fastcall_005ed700(client_app(), nullptr), 0x72) == 0) {   // 0064D618..0064D633 options gore
            exo_text(&column, kLowGore_0074ef78);                      // 0064D635..0064D63A
            const U8 missing = c2da_current_get_string_00413270(two_da(0x2C), nullptr, appearance, &column, &model) == 0
                                   ? 1 : 0;                            // 0064D63F..0064D675
            exo_free(&column);                                         // 0064D671..0064D67C
            if (missing != 0) {                                        // 0064D681..0064D683
                exo_text(&column, kModelName_00749814);                // 0064D685..0064D68E
                (void)c2da_current_get_string_00413270(two_da(0x2C), nullptr, appearance, &column, &model);   // 0064D693..0064D6E4
                exo_free(&column);                                     // 0064D6E9..0064D6F2
            }
        } else {
            exo_text(&column, kModelName_00749814);                    // 0064D6B4..0064D6B9
            (void)c2da_current_get_string_00413270(two_da(0x2C), nullptr, appearance, &column, &model);   // 0064D6BE..0064D6E4
            exo_free(&column);                                         // 0064D6E9..0064D6F2
        }
        if (server_app() == nullptr) {                                 // 0064D6F7..0064D700
            exo_free(&model);                                          // 0064D866..0064D86F
            exo_free(&name);                                           // 0064D874..0064D880
            return 0;                                                  // 0064D885
        }
        void* const memory = live_operator_new(0x150);                 // 0064D706..0064D710
        placeable = memory != nullptr ? recovered_client_placeable_construct_00682270(memory, nullptr) : nullptr;   // 0064D713..0064D72B
        if (placeable == nullptr) {                                    // 0064D72D..0064D734
            exo_free(&model);                                          // 0064D866..0064D86F
            exo_free(&name);                                           // 0064D874..0064D880
            return 0;
        }
        void* const server = server_app();                             // 0064D73A..0064D743 (edi)
        void* const server_placeable =
            recovered_placeable_lookup_004ae7a0(server, nullptr, kotor_fastcall_004aea30(server, nullptr, id));   // 0064D746..0064D756 (ebx)
        bool load_model = true;
        if (server_placeable != nullptr && rd32(server_placeable, 0x44C) != 0) {   // 0064D758..0064D768 corpse placeable
            const U body = kotor_fastcall_005eda50(client_app(), nullptr, rd32(server_placeable, 0x448));   // 0064D76E..0064D77E
            void* const creature = word_ptr(frame_R4_005ed5b0(client_app(), nullptr, body));   // 0064D783..0064D792 (edi)
            if (creature != nullptr && gob_get_property_0x68_0063c070(creature, nullptr) != nullptr) {   // 0064D794..0064D7A1
                load_model = false;
                recovered_client_object_acquire_animation_base_0063d700(placeable, nullptr, creature);   // 0064D7A3..0064D7A6
                kotor_m15a_object_set_looping_animation_0063d830(
                    placeable, nullptr, kotor_m15a_object_get_looping_animation_0063c100(creature, nullptr));   // 0064D7AB..0064D7B5
                kotor_m15e_client_ai_list_remove_object_00604810(
                    at(kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(), nullptr), 4), nullptr, creature);   // 0064D7BA..0064D7CC
                void* removed = nullptr;                               // {E-0x70} (stale stack slot in the original)
                if (recovered_object_array_remove_004d81a0(kotor_fastcall_005ed570(client_app(), nullptr),
                                                           nullptr, rd32(creature, 4), &removed) == kGameObjectOk_0074666c &&
                    removed != nullptr)                                // 0064D7D1..0064D7FD
                    delete_object(creature);                           // 0064D7FF..0064D805 [vtbl+0](1)
            }
        }
        if (load_model) {                                              // 0064D809
            ResRef ref;                                                // {E-0x70}
            async_ref_construct_string_00406d60(&ref, nullptr, &model);   // 0064D809..0064D812
            vslot<SlotLoadModel>(placeable, 0x78)(placeable, nullptr, &ref, 0xFF, 0x0A);   // 0064D817..0064D827 LoadModel
        }
        // 0064D82A..0064D831: the original pushes the dword {E-0x60} whose low byte is the appearance (upper bytes
        // stale); SetAppearance only uses it as the HitCheck default, here zero-extended.
        recovered_client_placeable_set_appearance_00682e20(placeable, nullptr, appearance);
        vslot<SlotSetId>(placeable, 0x04)(placeable, nullptr, id);     // 0064D836..0064D83F SetId
        vslot<SlotSetArea>(placeable, 0x84)(placeable, nullptr, area);     // 0064D842..0064D847 SetArea
        if (recovered_object_array_add_external_004d7ff0(objects, nullptr, &id, placeable, 1) != kGameObjectOk_0074666c) {   // 0064D84D..0064D864
            exo_free(&model);                                          // 0064D866..0064D86F (the placeable is not released)
            exo_free(&name);                                           // 0064D874..0064D880
            return 0;
        }
        recovered_client_ai_master_add_object_00604a20(kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(), nullptr),
                                                       nullptr, placeable);   // 0064D89A..0064D8AB
        recovered_client_placeable_load_light_006826e0(placeable, nullptr);   // 0064D8B0..0064D8B2
        if (server_placeable != nullptr && rd32(server_placeable, 0x440) != 0 && rd32(server_placeable, 0x444) == 0 &&
            area != nullptr && recovered_client_area_parent_corpse_exists_006057b0(area, nullptr, rd32(placeable, 4)) != 0) {   // 0064D8B7..0064D8E0
            recovered_client_object_set_desired_fade_state_0063ce60(placeable, nullptr, 0, 0, 0, 0);   // 0064D8E2..0064D8EC
            recovered_client_object_set_fade_state_0063e220(placeable, nullptr, 0);   // 0064D8F1..0064D8F5
        }
        exo_free(&model);                                              // 0064D8FA..0064D903
    } else {
        st<U>(placeable, 0xE4, 0);                                     // 0064D911
        recovered_client_object_set_desired_fade_state_0063ce60(placeable, nullptr, 1, 0, 0, 0);   // 0064D90A..0064D917
        recovered_client_object_set_visibility_on_set_area_0063e350(placeable, nullptr, area);   // 0064D91C..0064D91F
    }
    async_assign_string_005e5c50(at(placeable, 0xFC), nullptr, &name); // 0064D924..0064D92F
    vslot<SlotPtr>(placeable, 0x94)(placeable, nullptr, rp(area, 0x184));   // 0064D934..0064D93F AttachToScene(scene)
    st<U>(placeable, 0x110, field_110);                                // 0064D951
    st<U>(placeable, 0x128, field_128);                                // 0064D95B
    st<U>(placeable, 0x114, field_114);                                // 0064D965
    st<U>(placeable, 0x130, field_130);                                // 0064D96F
    st<U>(placeable, 0x118, field_118);                                // 0064D979
    st<U>(placeable, 0x120, field_120);                                // 0064D985
    st<U>(placeable, 0x12C, field_12c);                                // 0064D991
    st<U>(placeable, 0x124, field_124);                                // 0064D999
    vslot<U (__fastcall*)(void*, void*, U, U, U)>(placeable, 0x9C)(placeable, nullptr, animation, 0, 0);   // 0064D983..0064D9A2 SetAnimation
    if (rd32(placeable, 0x128) != 0)                                   // 0064D9A8..0064D9B4
        kotor_m15e_client_remove_from_hitcheck_ignore_list_005ed760(client_app(), nullptr, rd32(placeable, 4));   // 0064D9B6..0064D9BF
    else
        recovered_client_add_to_hitcheck_ignore_list_005ed750(client_app(), nullptr, rd32(placeable, 4));   // 0064D9C6..0064D9CF
    ResRef env;                                                        // {E-0x70}
    reinterpret_cast<menu_resources::Ref*>(&env)->construct_00406d80(kDefault_0073e304);   // 0064D9D4..0064D9DD CResRef("DEFAULT")
    vslot<U (__fastcall*)(void*, void*, U, U, void*)>(placeable, 0xB0)(placeable, nullptr, 1, 1, &env);   // 0064D9E2..0064D9EF SetEnvMap
    exo_free(&name);                                                   // 0064D9F5..0064DA01
    return 1;                                                          // 0064DA0B
}
}  // extern "C"
