#pragma once
// W2 tier 3: CSWPartyTable::CreateParty (00565760), CSWPartyTable::PositionParty (00565B00), their
// unowned callees, and the area-of-effect / encounter sub-area predicates (00595450, 005950C0,
// 00590090, 0058FFD0, 005946E0). Logic over original 32-bit address words; float values travel as
// IEEE-754 single bit patterns. Literal ids are the original .rdata addresses. x87 arithmetic is
// modelled with IEEE double (precision control 53 bits, the CRT default) and rounded to float exactly
// where the original stores to a 32-bit slot (FST/FSTP m32).
#include "1e16cfcf3d-server_mainloop.hpp"
#include <cstdint>
namespace w2::party {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;

struct Ports {
    virtual ~Ports() = default;
    virtual Word app_007a39fc() = 0;
    virtual Word rules_007a3a28() = 0;
    virtual Word literal(Word original_address) = 0;                       // original string literal -> live text
    virtual void write8(Word address, Word value) = 0;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;  // explicit trap (never a silent no-op)
    virtual Word allocate_006fa7e6(Word bytes) = 0;
    virtual void vdelete(Word object) = 0;                                  // [[object]+0](object, 1)
    virtual Word vcall0(Word object, Word slot) = 0;                        // thiscall [[object]+slot](object)
    virtual Word vcall1_short(Word object, Word slot, Word a) = 0;          // returns AX sign-extended
    // server / client app
    virtual Word player_creature_id_004aea40(Word server) = 0;
    virtual Word creature_by_id_004ae770(Word server, Word id) = 0;
    virtual Word item_by_id_004ae760(Word server, Word id) = 0;
    virtual Word client_object_by_id_004ae910(Word server, Word id) = 0;
    virtual Word client_to_server_id_004aea30(Word server, Word id) = 0;
    virtual Word object_lookup_004d8230(Word array, Word id, Word out) = 0;   // returns AL status
    virtual Word area_by_id_internal_004b17e0(Word internal, Word id) = 0;
    virtual Word module_004ae6b0(Word server) = 0;
    virtual Word module_area_004c30f0(Word module) = 0;
    virtual Word sw_party_005ed8b0(Word client) = 0;
    virtual Word server_to_client_id_005eda50(Word client, Word id) = 0;
    virtual Word client_creature_005ed5b0(Word client, Word id) = 0;
    virtual Word is_player_creature_005eda30(Word client, Word id) = 0;
    virtual Word client_options_005ed700(Word client) = 0;
    // party table game-in-progress (existing Memory-model owner module_mount)
    virtual void add_game_in_progress_005638d0(Word party) = 0;
    virtual void remove_game_in_progress_00563950(Word party) = 0;
    // client party (W4 owner): CSWCCreature::SetInParty, CSWPartyFollowDataArray::GetPathData
    virtual Word client_set_in_party_0060ef50(Word creature, Word flag) = 0;
    virtual Word path_data_00634cc0(Word array, Word index) = 0;
    virtual void follow_array_reset_00634b60(Word array, Word position, Word yaw_bits) = 0;
    virtual void follow_process_point_00636a30(Word array, Word point, Word yaw_bits, Word weight_bits) = 0;
    // creatures / objects
    virtual void set_in_party_004fdb20(Word creature, Word in_party, Word update) = 0;   // W2 t2
    virtual void set_activity_004f00a0(Word creature, Word mask, Word on, Word extra) = 0;  // W2 t2
    virtual Word gold_004edd60(Word creature) = 0;
    virtual Word item_repository_004ef770(Word creature, Word flag) = 0;
    virtual void equip_item_004feb90(Word creature, Word slot, Word item, Word a, Word b, Word c) = 0;
    virtual Word creature_construct_004f7a10(Word storage, Word id, Word flag) = 0;
    virtual Word creature_template_005026d0(Word creature, Word ref, Word flag) = 0;
    virtual void clear_all_actions_004ccd80(Word object, Word flag) = 0;
    virtual void add_to_area_004fa100(Word creature, Word area, Word x, Word y, Word z, Word a, Word b) = 0;
    virtual void set_position_004cd1a0(Word object, Word vector, Word a, Word b, Word c) = 0;
    virtual void set_orientation_004cd170(Word object, Word vector) = 0;
    virtual Word effect_construct_00503e40(Word storage, Word flag) = 0;
    virtual void apply_effect_004d14f0(Word object, Word effect, Word a, Word b) = 0;
    virtual void receive_experience_004ef930(Word creature, Word amount) = 0;
    virtual Word can_level_up_005a6810(Word stats) = 0;
    virtual void auto_levelup_005b27e0(Word stats, Word flag) = 0;
    virtual Word object_area_004cb120(Word object) = 0;
    // items / repositories
    virtual Word item_construct_005530a0(Word storage, Word id) = 0;
    virtual Word item_template_005608b0(Word item, Word ref) = 0;
    virtual void item_fields_0055fcd0(Word item, Word resource, Word structure) = 0;
    virtual void set_possessor_00553210(Word item, Word owner, Word a, Word b, Word c) = 0;
    virtual Word repository_add_0055d330(Word repository, Word item_slot, Word a, Word b, Word c) = 0;
    virtual Word repository_construct_0055d290(Word storage, Word owner, Word shared) = 0;
    virtual Word repository_item_005560b0(Word repository, Word index) = 0;
    virtual Word base_item_005b31a0(Word array, Word type) = 0;
    virtual Word string_equal_005e5200(Word a, Word b) = 0;
    // strings / resrefs / GFF
    virtual void str_literal_005e5a90(Word str, Word text) = 0;
    virtual void str_destroy_005e5c20(Word str) = 0;
    virtual void ref_construct_00406d80(Word ref, Word text) = 0;
    virtual void ref_from_string_00406d60(Word ref, Word str) = 0;
    virtual Word gff_construct_00410630(Word storage, Word a, Word type_text, Word ref) = 0;
    virtual Word gff_loaded_004064a0(Word gff) = 0;
    virtual void gff_top_00411240(Word gff, Word out) = 0;
    virtual Word gff_list_004118c0(Word gff, Word out, Word parent, Word label) = 0;
    virtual Word gff_list_count_00411940(Word gff, Word list) = 0;
    virtual Word gff_element_00411990(Word gff, Word out, Word list, Word index) = 0;
    virtual Word gff_resref_00411e10(Word gff, Word out, Word structure, Word label, Word status, Word fallback) = 0;
    virtual Word gff_byte_00411a60(Word gff, Word structure, Word label, Word status, Word fallback) = 0;
    virtual Word c2da_int_00413660(Word table, Word row, Word label, Word out) = 0;
    // geometry
    virtual Word yaw_bits_004aa0f0(Word vector) = 0;                        // cdecl, ST0 -> m32
    virtual Word deg_to_rad_bits_004a96d0(Word degrees_bits) = 0;           // cdecl, ST0 -> m32
    virtual void rotate_about_axis(Word out, Word axis, Word angle_bits, Word in) = 0;  // 004AC960 Quaternion(axis, angle) then 004A9BB0 operator*
    virtual Word area_safe_point_004be5e0(Word area, Word position, Word ai) = 0;
    virtual Word area_safe_location_004be860(Word area, Word position, Word radius_bits, Word ai, Word flag, Word out) = 0;
    virtual Word area_room_004bb600(Word area, Word position, Word flag) = 0;
    virtual void vector_cross_004ab210(Word out, Word a, Word b) = 0;      // cdecl vectorCrossProduct(out, a, b)
    virtual Word intersect_segments_0058f840(Word area, const Word (&xy)[8], Word out_a, Word out_b) = 0;  // W5
    // AOE sub-area maintenance (not reconstructed here)
    virtual void aoe_remove_from_subareas_00595b60(Word aoe, Word flag) = 0;
    virtual void aoe_update_subareas_00595640(Word aoe, Word flag) = 0;
    virtual void list_add_unique_006449d0(Word list, Word value) = 0;
    virtual void int_list_remove_0066dcc0(Word list, Word value) = 0;      // W1 owner (lane-w1 tier4)
    // closeA: 0063765E..0063787B, the flag != 0 arm of CSWParty::Reset 00637630 (formation points + ProcessPoint).
    // Appended last so every earlier slot keeps its offset for objects built against the previous header.
    virtual void party_reset_formation_0063765e(Word cparty, Word position, Word yaw_bits) = 0;
};

inline Word server_of(Memory& m, Ports& p) { return m.read(p.app_007a39fc() + 8); }
inline Word client_of(Memory& m, Ports& p) { return m.read(p.app_007a39fc() + 4); }

// ---- party table (CSWPartyTable)
// 00565760 CreateParty() RET. scratch >= 0x140 (0x00..0x5F original ESP offsets, then callee scratch)
void create_party_00565760(Memory&, Ports&, Word party, Word scratch);
// 00565130 SpawnNPC(index, use_given, position*, orientation*, flag) RET 14h. scratch >= 0xB0
Word spawn_npc_00565130(Memory&, Ports&, Word party, Word index, Word use_given, Word position, Word orientation, Word flag, Word scratch);
// 00564700 GetNPCObject(index, create, apply) RET 0Ch. scratch >= 0x50
Word npc_object_00564700(Memory&, Ports&, Word party, Word index, Word create, Word apply, Word scratch);
// 00563620 GetFilename(out CExoString*, index) RET 8 -> out. scratch >= 0x18 (text buffer)
Word filename_00563620(Memory&, Ports&, Word party, Word out, Word index, Word scratch);
// 00565B00 PositionParty() RET. scratch >= 0x100
void position_party_00565b00(Memory&, Ports&, Word party, Word scratch);
// 005641E0 TransferInventory(creature) RET 4. scratch >= 4 (item slot)
void transfer_inventory_005641e0(Memory&, Ports&, Word party, Word creature, Word scratch);
// 00563A40 GetIsLeader-labelled leader-slot setter (id) RET 4
Word party_leader_slot_00563a40(Memory&, Word party, Word id);
// 00563790 GetIsAvailable(id) RET 4
Word party_is_member_00563790(Memory&, Word party, Word id);
// 00563C60 UnstealthParty(extra) RET 4
void unstealth_party_00563c60(Memory&, Ports&, Word party, Word extra);
// ---- creature / items
// 00501C40 CSWSCreature::ForceEquipClothing() RET. scratch >= 0x30
void force_equip_clothing_00501c40(Memory&, Ports&, Word creature, Word scratch);
// 00560970 CSWSItem::LoadItem(resource, structure) RET 8. scratch >= 0x50
Word load_item_00560970(Memory&, Ports&, Word item, Word resource, Word structure, Word scratch);
// 00555ED0 FindItemWithTag(CExoString*) RET 4 / 00555F40 GetItemInRepository(item, recurse) RET 8
// 00555FD0 RemoveItem(item) RET 4 / 0055F330 ClearItems() RET
Word find_item_with_tag_00555ed0(Memory&, Ports&, Word repository, Word tag);
Word item_in_repository_00555f40(Memory&, Ports&, Word repository, Word item, Word recurse);
Word remove_item_00555fd0(Memory&, Word repository, Word item);
void clear_items_0055f330(Memory&, Word repository);
// ---- server app thunks
Word load_from_save_game_004af050(Memory&, Word server);                         // RET
Word test_safe_location_004b4d70(Memory&, Ports&, Word internal, Word id, Word position, Word slot4);  // RET 8 (004AEB50 thunk)
Word compute_safe_location_004b4df0(Memory&, Ports&, Word internal, Word position, Word radius_bits, Word id, Word flag, Word out, Word slot4);  // RET 14h (004AEB60)
Word aoe_by_id_004b1970(Memory&, Ports&, Word internal, Word id, Word slot4);    // RET 4 (004AE7D0 thunk)
// 004B19C0 GetWaypointByGameObjectID(id) RET 4 (004AE7E0 thunk): type byte 0Ch, vtbl 58h
Word waypoint_by_id_004b19c0(Memory&, Ports&, Word internal, Word id, Word slot4);
// 004AED70 CServerExoApp::GetObjectArray() = [[server+4]+1005Ch]
Word object_array_004aed70(Memory&, Word server);
// 00596360 CSWSAreaOfEffectObject::JumpToPoint(area, position*) RET 8. scratch >= 4
void aoe_jump_to_point_00596360(Memory&, Ports&, Word aoe, Word area, Word position, Word scratch);
// ---- client party (CSWParty / CSWCCreature)
// 00637890 CSWParty::ResetAllFollowData(position*, yaw, flag) RET 0Ch. scratch >= 0x14
void reset_all_follow_data_00637890(Memory&, Ports&, Word cparty, Word position, Word yaw_bits, Word flag, Word scratch);
// 00637630 CSWParty::Reset(position*, yaw, flag) RET 0Ch (flag != 0 path traps: not reconstructed)
void party_reset_00637630(Memory&, Ports&, Word cparty, Word position, Word yaw_bits, Word flag);
// ---- sub-area predicates
// 005946E0 CSWSAreaOfEffectObject::GetPosition(out*) RET 4 -> out
Word aoe_position_005946e0(Memory&, Ports&, Word aoe, Word out, Word slot4);
// 00595450 InAreaOfEffect(Vector) RET 0Ch. scratch >= 0x40
Word in_area_of_effect_00595450(Memory&, Ports&, Word aoe, Word x, Word y, Word z, Word scratch);
// 005950C0 LineSegmentIntersectAreaOfEffect(Vector a, Vector b) RET 18h. scratch >= 0x60
Word aoe_segment_005950c0(Memory&, Ports&, Word aoe, const Word (&a)[3], const Word (&b)[3], Word scratch);
// 00590090 CSWSEncounter::GetInActivateArea(Vector*) RET 4
Word encounter_in_area_00590090(Memory&, Word encounter, Word position);
// 0058FFD0 CSWSEncounter::LineSegmentIntersectActivateArea(Vector*, Vector*) RET 8. scratch >= 8
Word encounter_segment_0058ffd0(Memory&, Ports&, Word encounter, Word a, Word b, Word scratch);
}
