// 0963-gates (Claude): closure of the former required(0x...) vtable / port gates of the server object
// constructor unit. Included inside the unit's anonymous namespace, before the vtable tables.
// Definitions live in claude_0963_bodies.inc (included at the end of the TU); the three CSWSObject
// damage bodies and CSWCCMessageData::CopyTo live in srcw5/actions/claude_0963_bodies_damage.cpp.

// ---- existing real providers (C linkage, exact decorated arity checked against the c10 map) ----
extern "C" {
void* __cdecl live_operator_new(unsigned);                                                  // 006FA7E6 game heap
void __cdecl packed_purecall_006fb8de();                                                    // 006FB8DE _purecall (0268)
void __fastcall kotor_m15e_slot008_void8_005b5e90(void*,void*,unsigned,unsigned);           // 005B5E90 ret 8 (0832)
void __fastcall game_object_void_return_4_0060e760(void*,void*,unsigned);                   // 0060E760 ret 4 (0797 m50)
unsigned __fastcall recovered_creature_portrait_004d0210(void*,void*);                       // 004D0210 GetPortraitId
void* __fastcall frame_R4_005ed690(void*,void*);                                            // 005ED690 CClientExoApp::GetInGameGui
void __fastcall kotor_fastcall_0062ec60(void*,void*,unsigned);                              // 0062EC60 CGuiInGame::SetGlobalDialogState
void __fastcall recovered_creature_add_visible_0051b370(void*,void*,unsigned,unsigned,unsigned,unsigned,unsigned); // 0051B370
void __fastcall recovered_creature_remove_visible_00517960(void*,void*,unsigned);           // 00517960
void* __fastcall combat_reuse_faction_00513fc0(void*,void*);                                // 00513FC0 CSWSCreature::GetFaction
void* __fastcall kotor_fastcall_00561080(void*,void*);                                      // 00561080 CSWSPlayer::GetGameObject
void* __fastcall recovered_server_sws_message_004aedb0(void*,void*);                        // 004AEDB0 GetSWSMessage
unsigned __fastcall recovered_cc_message_00568570(void*,void*,unsigned,unsigned,void*,unsigned); // 00568570 SendServerToPlayerCCMessage
unsigned __fastcall recovered_ai_remove_004af3d0(void*,void*,void*);                         // 004AF3D0 CServerAIMaster::RemoveObject
unsigned __fastcall recovered_effect_integer_00503690(void*,void*,unsigned);                 // 00503690 CGameEffect::GetInteger
void __fastcall recovered_effect_integer_005036a0(void*,void*,unsigned,unsigned);            // 005036A0 CGameEffect::SetInteger
void __fastcall recovered_effect_creator_00503a00(void*,void*,unsigned);                     // 00503A00 CGameEffect::SetCreator
void* __fastcall recovered_effect_construct_00503e40(void*,void*,unsigned);                  // 00503E40 CGameEffect::CGameEffect
unsigned __fastcall recovered_object_remove_effect_004d06c0(void*,void*,unsigned,unsigned);  // 004D06C0 RemoveEffectById
void __fastcall recovered_effect_apply_004d14f0(void*,void*,void*,unsigned,unsigned);        // 004D14F0 ApplyEffect
void* __fastcall recovered_object_area_004cb120(void*,void*);                                // 004CB120 CSWSObject::GetArea
void* __fastcall server_get_module_004ae6b0(void*,void*);                                   // 004AE6B0 CServerExoApp::GetModule
void __fastcall recovered_module_remove_tag_004c5ef0(void*,void*,void*,unsigned);            // 004C5EF0 RemoveObjectFromLookupTable
void __fastcall kotor_fastcall_00555de0(void*,void*);                                       // 00555DE0 CItemRepository::~CItemRepository
void* __fastcall frame_R4_005ed5b0(void*,void*,unsigned);                                   // 005ED5B0 GetCreatureByGameObjectID
void* __fastcall recovered_server_ai_master_004aed80(void*,void*);                          // 004AED80 GetServerAIMaster
void __fastcall recovered_object_position_004cd1a0(void*,void*,void*,unsigned,unsigned,unsigned); // 004CD1A0 SetPosition
void __fastcall recovered_int_list_remove_0066dcc0(void*,void*,unsigned);                    // 0066DCC0 CExoArrayList<int>::Remove
void* __fastcall recovered_event_construct_004d7540(void*,void*);                           // 004D7540 CScriptEvent::CScriptEvent
void __fastcall recovered_event_enqueue_004b08d0(void*,void*,unsigned,unsigned,unsigned,unsigned,unsigned,void*); // 004B08D0 AddEventDeltaTime
void __fastcall recovered_ai_level_004b08a0(void*,void*,void*,unsigned);                     // 004B08A0 SetAILevel
unsigned __fastcall c2da_release_demand_00409b80(void*,void*);                               // 00409B80 CRes::Release
unsigned __fastcall c2da_release_00409cf0(void*,void*,void*);                                // 00409CF0 CExoResMan::ReleaseResObject
void __fastcall recovered_expression_list_delete_list_0059d920(void*,void*);                  // 0059D920 CSWSExpressionList::DeleteList (vm_actions_t1)
void __fastcall recovered_action_node_destroy_004cc4d0(void*,void*);                         // 004CC4D0 ~CSWSObjectActionNode (w1_native_b7)
// srcw5/actions/claude_0963_bodies_damage.cpp (new faithful bodies, compiled with cc_w5.py)
int __fastcall kotor_fastcall_004cf160(void*,void*,void*,int,unsigned,unsigned,unsigned);    // DoDamageImmunity  RET 14
int __fastcall kotor_fastcall_004d09e0(void*,void*,void*,int,unsigned,unsigned,unsigned);    // DoDamageReduction RET 14
int __fastcall kotor_fastcall_004d0e40(void*,void*,void*,int,unsigned,unsigned,unsigned,unsigned); // DoDamageResistance RET 18
}

// ---- file-local bodies defined later in this TU ----
unsigned __fastcall trigger_dead_004cb810(void*,void*);       // trigger_native.inc: 004CB810 CSWSObject::GetDead
void* __fastcall object_name_004d0190(void*,void*);           // sound_native.inc:   004D0190 CSWSObject::GetLastName
void __fastcall body_purecall_006fb8de(void*,void*);
int __fastcall body_004caee0(void*,void*,unsigned);
int __fastcall body_004caf70(void*,void*,unsigned);
unsigned char __fastcall body_004cb190(void*,void*);
void __fastcall body_004cb160(void*,void*,void*);
unsigned __fastcall body_004cc390(void*,void*,void*,unsigned);
void __fastcall body_004ccf80(void*,void*,int);
void* __fastcall body_004d0170(void*,void*,void*);
unsigned short __fastcall body_004d01a0(void*,void*,unsigned);
unsigned __fastcall body_004d01b0(void*,void*);
void* __fastcall body_004d01c0(void*,void*,void*);
void* __fastcall body_004d09c0(void*,void*,unsigned);
unsigned char __fastcall body_004d7e90(void*,void*,unsigned,void*);
void __fastcall body_0063d520(void*,void*,void*);
void __fastcall body_0063d530(void*,void*);
void __fastcall body_0059d9e0(void*,void*);
void __fastcall body_004eb160(void*,void*,unsigned);
void __fastcall body_004eb6b0(void*,void*,unsigned,unsigned);
void __fastcall body_004ec610(void*,void*,void*);
unsigned __fastcall body_005be780(void*,void*,void*);
void* __fastcall body_004f8290(void*,void*,void*);
unsigned __fastcall body_004f82e0(void*,void*);
unsigned char __fastcall body_004f82f0(void*,void*);
void* __fastcall body_004f8310(void*,void*);
void* __fastcall body_004f8330(void*,void*);
unsigned __fastcall body_004f8360(void*,void*,unsigned);
void* __fastcall body_0058a1b0(void*,void*,void*);
void* __fastcall body_0058a1e0(void*,void*);
unsigned __fastcall body_00588ab0(void*,void*);
void __fastcall body_00589190(void*,void*,int);
void* __fastcall body_00553200(void*,void*);
void* __fastcall body_005b4770(void*,void*,unsigned);
void __fastcall body_005c7f10(void*,void*,unsigned,unsigned,void*,unsigned,unsigned);
void* __fastcall body_0058f690(void*,void*,unsigned);
void* __fastcall body_005c8630(void*,void*,unsigned);
void* __fastcall body_005c9020(void*,void*,unsigned);
void* __fastcall body_00587a50(void*,void*,unsigned);
void* __fastcall body_0058c810(void*,void*,unsigned);
