#pragma once
// W2 lane G08: player entry into area, module/area client messages and the
// server-side handlers of the client's Module/Area replies.
#include "913ae857a3-w2_message.hpp"
namespace w2::entry {
using w2::msg::Word;
using w2::msg::Memory;

struct Ports : w2::msg::LocPorts {
    // original read-only literals (0073D71C "", 00749368 "TileSet", 00749360 "TILESET", 007454E4 "NORMALHEAD")
    virtual Word literal(Word original_address) = 0;
    virtual Word app_007a39fc() = 0;
    virtual Word rules_007a3a28() = 0;
    // existing owners / callees (names carry the original address)
    virtual Word internal_module_004b14f0(Word internal) = 0;
    virtual Word module_area_004c30f0(Word module) = 0;
    virtual Word game_object_00561080(Word player) = 0;
    virtual Word game_object_by_id_004ae750(Word server, Word id) = 0;
    virtual Word object_area_004cb120(Word object) = 0;
    virtual Word area_by_id_004ae780(Word server, Word id) = 0;
    virtual Word creature_by_id_004ae770(Word server, Word id) = 0;
    virtual Word vcall0(Word object, Word slot) = 0;                    // thiscall [[object]+slot](object)
    virtual Word vcall1(Word object, Word slot, Word a) = 0;
    virtual Word creature_construct_004f7a10(Word memory, Word id, Word flag) = 0;
    virtual void set_orientation_004cd170(Word object, Word vector) = 0;
    virtual void set_area_004cb160(Word object, Word area) = 0;
    virtual void str_assign_005e5c50(Word dst, Word src) = 0;
    virtual void str_assign_text_005e5140(Word dst, Word text) = 0;
    virtual Word str_char_005e5600(Word str, Word index) = 0;
    virtual Word c2da_string_00413270(Word table, Word row, Word column, Word out) = 0;
    virtual Word c2da_int_00413660(Word table, Word row, Word column, Word out) = 0;
    virtual Word compare_no_case_005e6450(Word a, Word b) = 0;
    virtual Word rand_006fc447() = 0;
    virtual Word atan2_bits(Word y_bits, Word x_bits) = 0;             // fld y; fld x; fpatan; fstp m32
    virtual Word pack_area_0050aaa0(Word area, Word a, Word b, Word c, Word player) = 0;
    virtual Word str_chr_006fb5e0(Word text, Word ch) = 0;
    virtual Word pause_state_004ae980(Word server, Word kind) = 0;
    virtual Word set_pause_state_0056c9f0(Word message, Word a, Word b) = 0;
    virtual Word save_game_004b58a0(Word internal, Word id, Word name, Word player, Word flag) = 0;
    virtual Word load_game_004ba640(Word internal, Word id, Word a, Word b, Word player) = 0;
    virtual Word run_module_004b6270(Word internal) = 0;
    virtual Word event_construct_004d7540(Word memory) = 0;
    virtual void event_object_004d7780(Word event, Word index, Word id) = 0;
    virtual void event_enqueue_004b08d0(Word ai, Word a, Word b, Word c, Word d, Word e, Word f) = 0;
    virtual Word dead_temp_004ef890(Word creature) = 0;
    virtual void load_character_ifo_00561e30(Word player, Word index, Word a, Word b) = 0;
    virtual void character_list_response_0056b400(Word message, Word a, Word b, Word c) = 0;
    virtual void enforce_no_pc_head_cloning_00508820(Word area, Word value) = 0;
    virtual Word party_repository_00563340(Word party) = 0;
    virtual void set_parent_id_00556150(Word repository, Word id) = 0;
    virtual void increment_players_00508c20(Word area) = 0;
    virtual void decrement_players_00508c40(Word area) = 0;
    virtual void clear_action_queue_005ca0d0(Word last) = 0;
    virtual void clear_known_spells_005cab20(Word last) = 0;
    virtual void clear_known_spell_uses_005ca060(Word last) = 0;
    virtual void clear_spell_add_delete_005cac30(Word last) = 0;
    virtual void clear_feats_005cab60(Word last) = 0;
    virtual Word waypoint_004c6e00(Word module, Word tag) = 0;
    virtual Word waypoint_by_id_004ae7e0(Word server, Word id) = 0;
    virtual Word safe_location_004be860(Word area, Word position, Word radius_bits, Word a, Word b, Word out) = 0;
    virtual void update_subareas_0051b940(Word creature, Word position, Word area_id, Word flag) = 0;
    virtual void add_to_area_004fa100(Word creature, Word area, Word x, Word y, Word z, Word a, Word b) = 0;
    virtual void update_visible_0051b050(Word creature, Word flag) = 0;
    virtual void position_party_00565b00(Word party) = 0;
    virtual void loc_construct_005e9e10(Word loc) = 0;
    virtual void loc_destroy_005ea000(Word loc) = 0;
    virtual void loc_assign_005e9e70(Word dst, Word src) = 0;
    virtual Word overflow_004d6230(Word message) = 0;                  // existing 0917 owner
    virtual Word underflow_004d6240(Word message) = 0;                 // existing 0917 owner
    // Original hardware fault at `pc` (e.g. #DE). Never returns in native code.
    virtual void fault(Word pc, const char* what) = 0;
    // area pack (0050AAA0) callees
    virtual Word object_lookup_004d8230(Word array, Word id, Word out) = 0;  // status byte; 0 = success ([0074666C])
    virtual Word placeable_by_id_004ae7a0(Word server, Word id) = 0;
    virtual Word placeable_light_00585130(Word placeable) = 0;
    virtual Word str_equal_005e5200(Word a, Word b) = 0;
    virtual void str_format_int_005e5680(Word str, Word format, Word value) = 0;
    virtual Word str_not_equal_text_005e5390(Word str, Word text) = 0;
    // CSWSScriptVarTable::MatchIndex create path (CExoArrayList<CSWSScriptVar>::Add 0059A2A0, CScriptLocation ctor 004CA7A0)
    virtual Word var_create_0059a3ef(Word table, Word name, Word type) = 0;
    // ---- G07 per-frame object update callees ----
    virtual std::uint64_t high_resolution_timer_005e9260() = 0;          // [[007A39E0]+4]->GetHighResolutionTimer
    virtual void list_remove_005e9c10(Word list, Word position) = 0;
    virtual void last_update_destroy_005ca6a0(Word last_update) = 0;    // CLastUpdateObject::~CLastUpdateObject
    virtual Word test_object_visible_0056dc60(Word message, Word object, Word observer) = 0;
    virtual void test_update_differences_00577fa0(Word message, Word player, Word object, Word last_out, Word a_out, Word b_out) = 0;
    virtual void update_object_00574a10(Word message, Word player, Word object, Word last, Word a, Word b) = 0;
    virtual void store_values_00576fc0(Word message, Word player, Word last, Word object, Word a, Word b) = 0;
    virtual void delete_for_object_0056e190(Word message, Word player, Word id) = 0;
    virtual void major_gui_panels_00577670(Word message, Word player) = 0;
    virtual void party_ai_state_005779f0(Word message, Word player) = 0;
    virtual Word charsheet_required_00561260(Word charsheet, Word player) = 0;
    virtual void write_charsheet_0056f490(Word message, Word player, Word required) = 0;
    // W1-owned CSWSCreature::GetSpot/ListenCheckDistance (thiscall, float in ST0) -> IEEE single bits
    virtual Word spot_distance_004efc70(Word creature) = 0;
    virtual Word listen_distance_004efd20(Word creature) = 0;
};

// __ftol2 (006FAE8C) low dword for a single-precision input: truncation toward
// zero modulo 2^32; NaN/|x|>=2^63 give the x87 integer indefinite (low dword 0).
Word ftol2_low(Word float_bits);
inline Word server_of(Memory& m, Ports& p) { return m.read(p.app_007a39fc() + 8); }

// 00560CD0 CSWSPlayer::SetAreaTransitionBMP(int, CExoString*)          RET 8
void set_area_transition_bmp_00560cd0(Memory&, Ports&, Word player, Word value, Word str);
// 0056CD20 CSWSMessage::SendServerToPlayerArea_ClientArea(player, area, x, y, z, Vector* orientation, BOOL) RET 1Ch
// scratch: 0x40 bytes
Word area_client_area_0056cd20(Memory&, Ports&, Word self, Word player, Word area, Word x, Word y, Word z, Word orientation, Word flag, Word scratch);
// 0056D2D0 CSWSMessage::SendServerToPlayerSetCustomTokenList(recipient)  RET 4
Word custom_token_list_0056d2d0(Memory&, Ports&, Word self, Word recipient, Word scratch8);
// 004C3170 CSWSModule::PackModuleIntoMessage(unused)                      RET 4. scratch: 8
void pack_module_004c3170(Memory&, Ports&, Word module, Word scratch);
// 00567EF0 CSWSMessage::SendServerToPlayerModule_Info(recipient)           RET 4
Word module_info_00567ef0(Memory&, Ports&, Word self, Word recipient, Word scratch);
// 0056FDE0 CSWSMessage::SendServerToPlayerLogin_Confirm(recipient)        RET 4
Word login_confirm_0056fde0(Memory&, Ports&, Word self, Word recipient);
// 0056CC50 CSWSMessage::SendServerToPlayerModule_EndStartNewModule(BOOL) RET 4 (3/12 to all players)
Word end_start_new_module_0056cc50(Memory&, Ports&, Word self, Word flag, Word scratch8);
// 0056A5F0 CSWSMessage::SendServerToPlayerModuleUpdate_Time(player*, flags, dword10, byte8, byte4, byte2, kind, value) RET 20h
Word module_update_time_0056a5f0(Memory&, Ports&, Word self, Word player, Word flags, Word dword10, Word byte8, Word byte4, Word byte2, Word kind, Word value, Word scratch8);
// 0056D1E0 CSWSMessage::SendServerToPlayerArea_Weather(player*, weather byte, BOOL) RET 0Ch
Word area_weather_0056d1e0(Memory&, Ports&, Word self, Word player, Word weather, Word flag, Word scratch8);
// 0056CB10 CSWSMessage::SendServerToPlayerModule_Loading(player)           RET 4
Word module_loading_0056cb10(Memory&, Ports&, Word self, Word player);
// 00568030 / 0056FE40 player list (scratch: 0x80)
Word player_list_add_00568030(Memory&, Ports&, Word self, Word recipient, Word player, Word scratch);
Word player_list_all_0056fe40(Memory&, Ports&, Word self, Word player, Word scratch);
// 00561C40 CSWSPlayer::SetGameObject(object)                               RET 4
void set_game_object_00561c40(Memory&, Ports&, Word player, Word object);
// 004B3A90 CServerExoAppInternal::InitiateModuleForPlayer(player)         RET 4. scratch: 0x20
void initiate_module_004b3a90(Memory&, Ports&, Word internal, Word player, Word scratch);
// 004B3D10 CServerExoAppInternal::MovePlayerToArea(player)                 RET 4. scratch: 0x20
void move_player_to_area_004b3d10(Memory&, Ports&, Word internal, Word player, Word scratch);
// 004C5D90 CSWSModule::GetPrimaryPlayerIndex()                              RET
Word primary_player_index_004c5d90(Memory&, Ports&, Word module, Word scratch4);
// 004B5C50 CServerExoAppInternal::LoadCharacterFinish(player, flag)       RET 8. scratch: 0x20
Word load_character_finish_004b5c50(Memory&, Ports&, Word internal, Word player, Word flag, Word scratch);
// 00508820 CSWSArea::EnforceNoPCHeadCloning(head) RET 4. scratch >= 0x10 (+0 out value persists across iterations)
void no_pc_head_cloning_00508820(Memory&, Ports&, Word area, Word head, Word scratch);
// 004B5F50 CServerExoAppInternal::LoadPrimaryPlayer(player)               RET 4
Word load_primary_player_004b5f50(Memory&, Ports&, Word internal, Word player, Word scratch);
// 00524800 CSWSMessage::HandlePlayerToServerModuleMessage(player, minor)  RET 8. scratch: 0x40
Word module_message_00524800(Memory&, Ports&, Word self, Word player, Word minor, Word scratch);
// 004C4100 CSWSModule::GetTime(6 outputs)                                 RET 18h
void module_time_004c4100(Memory&, Ports&, Word module, Word a, Word b, Word c, Word d, Word byte_out, Word f);
// 0059A390 CSWSScriptVarTable::MatchIndex(CExoString* name, type, create)   RET 0Ch
Word var_match_0059a390(Memory&, Ports&, Word table, Word name, Word type, Word create);
Word var_int_0059a530(Memory&, Ports&, Word table, Word name);              // RET 4
Word var_float_bits_0059a560(Memory&, Ports&, Word table, Word name);       // RET 4 (ST0)
Word var_string_0059a590(Memory&, Ports&, Word table, Word result, Word name, Word scratch8); // RET 8
Word var_object_0059a620(Memory&, Ports&, Word table, Word name);           // RET 4
// 005C9780 CSWSAmbientSound::PackIntoMessage(message)                        RET 4
void ambient_pack_005c9780(Memory&, Ports&, Word ambient, Word message);
// 005C89C0 CSWSSoundObject::PackIntoMessage(message)                         RET 4
void sound_pack_005c89c0(Memory&, Ports&, Word sound, Word message, Word scratch16);
// 0050AAA0 CSWSArea::PackAreaIntoMessage(a, b, c, player)                     RET 10h. scratch: 0x100
Word pack_area_0050aaa0(Memory&, Ports&, Word area, Word a, Word b, Word c, Word player, Word scratch);
// ---- G07 ----
// 004B3EC0 CServerExoAppInternal::UpdateClientGameObjectsForPlayer(player, force, time lo, time hi) RET 10h
void update_client_objects_004b3ec0(Memory&, Ports&, Word internal, Word player, Word force, Word time_lo, Word time_hi);
// 00578520 / 00578200 CSWSMessage::SendServerToPlayerGameObjUpdate(player[, id]) RET 4 / RET 8. scratch: 0x20
Word game_object_update_00578520(Memory&, Ports&, Word self, Word player, Word scratch);
Word game_object_update_00578200(Memory&, Ports&, Word self, Word player, Word id, Word scratch);
// 0056F430 SendServerToPlayerGameObjUpdate_ObjControl(recipient, id)       RET 8
Word object_control_0056f430(Memory&, Ports&, Word self, Word recipient, Word id, Word scratch8);
// 00578060 ComputeGameObjectUpdateForObject(player, observer, array, id)    RET 10h
void compute_for_object_00578060(Memory&, Ports&, Word self, Word player, Word observer, Word array, Word id, Word scratch);
// 00578130 ComputeGameObjectUpdateForCategory(category, limit, player, observer, array, list, count) RET 1Ch
Word compute_for_category_00578130(Memory&, Ports&, Word self, Word category, Word limit, Word player, Word observer, Word array, Word list, Word count, Word scratch);
// 00567BA0 WriteGameObjUpdate_WorkRemaining(object, area, stage, total)      RET 10h
void work_remaining_00567ba0(Memory&, Ports&, Word self, Word object, Word area, Word stage, Word total);
// 00572990 WriteGameObjUpdate_MinorGUIPanels(player)                         RET 4
void minor_gui_panels_00572990(Memory&, Ports&, Word self, Word player);
// 0056DF60 DeleteLastUpdateObjectsInOtherAreas(player)                       RET 4. scratch: 0x10
void delete_other_areas_0056df60(Memory&, Ports&, Word self, Word player, Word scratch);
// 00567850 SortObjectsForGameObjectUpdate(player, observer, array, &count) -> entry array {id, float distance, category}
Word sort_objects_00567850(Memory&, Ports&, Word self, Word player, Word observer, Word array, Word count_out, Word scratch);
// 00567780 SelectCategoryForGameObject(object, observer)                      RET 8
Word select_category_00567780(Memory&, Ports&, Word object, Word observer);
// 005071B0 / 005071E0 CSWSArea::GetFirst/NextObjectInArea(Word* out)          RET 4
Word area_first_object_005071b0(Memory&, Word area, Word out);
Word area_next_object_005071e0(Memory&, Word area, Word out);
// 00561820 CSWSPlayer::GetLastUpdateObject(id)                                 RET 4
Word last_update_object_00561820(Memory&, Ports&, Word player, Word id, Word scratch4);
// 00524B80 CSWSMessage::HandlePlayerToServerAreaMessage(player, minor)    RET 8. scratch: 0x20
Word area_message_00524b80(Memory&, Ports&, Word self, Word player, Word minor, Word scratch);
}
