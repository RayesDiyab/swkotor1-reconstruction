// W5 foreign imports (w5-foreign closure of 1127 w5c/source/w5_foreign_traps.cpp, sha 5b4accc5...).
// Every import name below is defined by a real linked provider in the c11 composition, so the /alternatename trap
// fallbacks and the trap bodies are removed: callers bind to the provider directly. /include makes the linker
// REQUIRE each provider, so dropping one becomes a link error instead of a silent trap. Original RET verified for
// each against swkotor.exe; the provider decorations match (@N = 8 + RET imm).
#include "1a370ea669-w5_foreign.hpp"
#define W5_REQUIRE(sym) __pragma(comment(linker, "/include:" sym))
W5_REQUIRE("@kotor_fastcall_004d67b0@16")                         // w2_message_native.obj  CSWMessage::ReadFLOAT RET 8 (ST0 float)
W5_REQUIRE("@kotor_fastcall_004d6750@12")                         // w2_message_native.obj  CSWMessage::ReadWORD RET 4
W5_REQUIRE("@kotor_fastcall_004d6720@12")                         // w2_message_native.obj  CSWMessage::ReadBYTE RET 4
W5_REQUIRE("@recovered_last_update_object_00561820@12")           // w2_update_native.obj   CSWSPlayer::GetLastUpdateObject RET 4
W5_REQUIRE("@recovered_set_activity_004f00a0@20")                 // w2_ifo_native.obj      CSWSCreature::SetActivity RET 0Ch
W5_REQUIRE("@kotor_fastcall_00578540@8")                          // w2_message_native.obj  CSWSMessage::ReadOBJECTIDServer RET 0
W5_REQUIRE("@recovered_aoe_in_area_of_effect_00595450@20")        // w2_party_native.obj    InAreaOfEffect(Vector) RET 0Ch
W5_REQUIRE("@recovered_aoe_segment_intersect_005950c0@32")        // w2_party_native.obj    LineSegmentIntersectAreaOfEffect RET 18h
W5_REQUIRE("@recovered_encounter_in_activate_area_00590090@12")   // w2_party_native.obj    CSWSEncounter::GetInActivateArea RET 4
W5_REQUIRE("@recovered_encounter_segment_intersect_0058ffd0@16")  // w2_party_native.obj    LineSegmentIntersectActivateArea RET 8
W5_REQUIRE("@w5_object_add_open_door_action_004cfac0@16")         // 1206-14-w5t3_w5_door.obj AddOpenDoorAction RET 8
W5_REQUIRE("@w5_object_add_close_door_action_004cfb20@16")        // 1206-14-w5t3_w5_door.obj AddCloseDoorAction RET 8
W5_REQUIRE("@w5_object_add_use_object_action_0057c810@12")        // 1206-14-w5t3_w5_door.obj AddUseObjectAction RET 4
W5_REQUIRE("@w5_creature_bump_friends_004f6390@20")               // 1206-11-w5t3_w5_precalc.obj BumpFriends RET 0Ch
W5_REQUIRE("@w5_import_game_effect_destroy_00503590@8")           // 1207-00 glue -> w1_native_b7 recovered_effect_destroy RET 0
W5_REQUIRE("@w5_import_vm_delete_script_situation_005d1110@12")   // 1194s.obj -> recovered_vm_delete_script_situation
