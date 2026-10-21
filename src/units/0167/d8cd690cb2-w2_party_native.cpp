// W2 tier 3 native binding + exports: CreateParty 00565760, PositionParty 00565B00 and callees, AOE /
// encounter sub-area predicates. Providers: symbols present in build/1122/windows-link/kotor-vm-cell1122.map,
// W1/W5 lane exports, or w2x_* wrappers from w2_0886_exports.inc. Unreconstructed callees trap explicitly.
#if !defined(_M_IX86)
#error W2 native bindings require the Windows x86 route
#endif
#include "669c8a7e66-w2_party.hpp"
#include "f0c71cbcb2-object_position.hpp"
#include "1291820eb6-faction_member_helpers.hpp"
#include <cstdint>
#include <cstring>
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace kotor_scene3d { struct Vector; }
namespace kotor_world_los { extern kotor_scene3d::Vector up_007a19b0; }   // [007A19B0] (0,0,1), never written by the original
namespace kotor_q05 { struct StructHandle; struct CResGFF { unsigned __thiscall GetTopLevelStruct(StructHandle*); }; }
namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned); }
struct W2V3 { float x, y, z; };
struct W2Q4 { float x, y, z, w; };
extern "C" {
void __fastcall kotor_fastcall_005b27e0(void*, void*, unsigned);   // CSWSCreatureStats::AutoLevelup thiscall(int) RET 4 (srcw5 claude_levelup_rules.cpp)
// closeA providers (c35 map): former trap ports
void __fastcall w5_party_follow_process_point_00636a30(void*, void*, const void*, float, unsigned);   // W5 t2 w5_precalc.cpp (RET 0Ch)
void __fastcall recovered_creature_receive_experience_004ef930(void*, void*, unsigned);               // W2 w2_experience_native (RET 4)
void __fastcall kotor_fastcall_00595b60(void*, void*, unsigned);                                      // CSWSAreaOfEffectObject::RemoveFromSubAreas (claude_w1_route_bodies)
void __fastcall kotor_fastcall_00595640(void*, void*, const void*);                                   // CSWSAreaOfEffectObject::UpdateSubAreas (claude_aoe)
unsigned __fastcall recovered_party_list_00567f90(void*, void*, unsigned, unsigned, const void*, unsigned, unsigned); // SendServerToPlayerParty_List (party_list)
void* __fastcall kotor_fastcall_004ae780(void*, void*, unsigned);                                     // CServerExoApp::GetAreaByGameObjectID (0530)
void __fastcall vector_scale_assign_004a97a0(void*, void*, float);                                    // Vector::operator*= (RET 4)
void __fastcall vector_add_assign_004a9740(void*, void*, const void*);                                // Vector::operator+= (RET 4)
extern void* kotor_cell_rules_007a3a28;
void* __cdecl live_operator_new(std::uint32_t);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
// existing providers (1122 map)
unsigned __fastcall kotor_fastcall_004aea40(void*, void*);
void* __fastcall kotor_fastcall_004ae770(void*, void*, unsigned);
void* __fastcall recovered_item_lookup_004ae760(void*, void*, unsigned);
void* __fastcall kotor_fastcall_004ae910(void*, void*, unsigned);
unsigned __fastcall kotor_fastcall_004aea30(void*, void*, unsigned);
unsigned char __fastcall kotor_fastcall_004d8230(void*, void*, unsigned, void*);
void* __fastcall server_internal_get_area_by_object_id_004b17e0(void*, void*, unsigned);
void* __fastcall server_get_module_004ae6b0(void*, void*);
void* __fastcall module_get_area_004c30f0(void*, void*);
void* __fastcall kotor_fastcall_005ed8b0(void*, void*);
unsigned __fastcall kotor_fastcall_005eda50(void*, void*, unsigned);
void* __fastcall frame_R4_005ed5b0(void*, void*, unsigned);
unsigned __fastcall kotor_m15_client_is_player_creature_005eda30(void*, void*, void*);
void* __fastcall kotor_fastcall_005ed700(void*, void*);
void __fastcall kotor_fastcall_00634b60(void*, void*, void*, float);
void* __fastcall recovered_creature_inventory_004ef770(void*, void*, unsigned);
void __fastcall recovered_creature_equip_004feb90(void*, void*, unsigned, void*, unsigned, unsigned, unsigned);
void* __fastcall recovered_server_creature_construct_004f7a10(void*, void*, unsigned, unsigned);
void __fastcall recovered_creature_placement_004fa100(void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned);
void __fastcall recovered_object_position_004cd1a0(void*, void*, void*, unsigned, unsigned, unsigned);
void* __fastcall recovered_effect_construct_00503e40(void*, void*, unsigned);
void __fastcall recovered_effect_apply_004d14f0(void*, void*, void*, unsigned, unsigned);
void* __fastcall recovered_object_area_004cb120(void*, void*);
void* __fastcall recovered_item_construct_005530a0(void*, void*, unsigned);
unsigned __fastcall recovered_item_template_005608b0(void*, void*, void*);
void __fastcall recovered_item_fields_0055fcd0(void*, void*, void*, void*);
void __fastcall recovered_item_owner_00553210(void*, void*, unsigned, unsigned, unsigned, unsigned);
unsigned __fastcall recovered_inventory_insert_0055d330(void*, void*, void**, unsigned, unsigned, unsigned);
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void*, void*, int);
int __fastcall async_string_equal_005e5200(void*, void*, void*);
void __fastcall frame_LogStringCtor_005e5a90(void*, void*, const char*);
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);
void __fastcall unresolved_ref_assign_00406290(void*, void*, void*, const char*);
void* __fastcall async_ref_construct_string_00406d60(void*, void*, void*);
void* __fastcall gui_load_gff_construct_00410630(void*, void*, unsigned, const char*, const void*);
unsigned __fastcall gui_load_slot_prop10_004064a0(void*, void*);
unsigned __fastcall gff_list_004118c0(void*, void*, void*, void*, const char*);
unsigned __fastcall gff_list_count_00411940(void*, void*, void*);
unsigned __fastcall gff_list_element_00411990(void*, void*, void*, void*, unsigned);
void* __fastcall gui_load_gff_resref_00411e10(void*, void*, void*, void*, const char*, unsigned*, void*);
unsigned char __fastcall gui_load_gff_byte_00411a60(void*, void*, void*, const char*, unsigned*, unsigned char);
int __fastcall c2da_current_get_int_00413660(void*, void*, unsigned, void*, unsigned*);
float __cdecl kotor_m15a_yaw_004aa0f0(const void*);
float __cdecl deg_to_rad_004a96d0(float);
W2Q4* __fastcall quaternion_construct_004ac960(W2Q4*, void*, W2V3, float);
W2V3* __cdecl quaternion_rotate_004a9bb0(W2V3*, const W2Q4*, const W2V3*);
W2V3* __cdecl vector_cross_004ab210(W2V3*, const W2V3*, const W2V3*);
unsigned __fastcall recovered_can_level_up_005a6810(void*, void*);   // W2 t4 (w2_g07_native.cpp)
// W4 providers (client party; ruling t3 review)
unsigned __fastcall kotor_fastcall_0060ef50(void*, void*, unsigned);
void* __fastcall kotor_fastcall_00634cc0(void*, void*, int);
// W1 / W5 / W2-t2 providers
void __fastcall recovered_int_list_remove_0066dcc0(void*, void*, unsigned);   // W1 tier4
unsigned __fastcall w1x_area_safe_location_004be860(void*, void*, void*, float, unsigned, unsigned, void*);
void __fastcall w5_object_clear_all_actions_004ccd80(void*, void*, unsigned);
unsigned __fastcall w5_area_intersect_line_segments_0058f840(void*, void*, float, float, float, float, float, float, float, float, float*, float*);
void __fastcall recovered_set_in_party_004fdb20(void*, void*, unsigned, unsigned);
void __fastcall recovered_set_activity_004f00a0(void*, void*, unsigned, unsigned, unsigned);
void* __fastcall recovered_inventory_construct_0055d290(void*, void*, unsigned, unsigned);
// 0886 wrappers (w2_0886_exports.inc)
void __fastcall w2x_party_mount_005638d0(void*, void*);
void __fastcall w2x_party_unmount_00563950(void*, void*);
unsigned __fastcall w2x_creature_gold_004edd60(void*, void*);
void* __fastcall w2x_inventory_item_005560b0(void*, void*, unsigned);
unsigned __fastcall w2x_creature_template_005026d0(void*, void*, void*, unsigned);
unsigned __fastcall w2x_area_safe_point_004be5e0(void*, void*, void*, unsigned);
unsigned __fastcall w2x_area_room_004bb600(void*, void*, void*, int*);
}
namespace {
using w2::party::Word;
inline void* ptr(Word a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline Word word(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
inline float f32(Word b) { float v; std::memcpy(&v, &b, 4); return v; }
inline Word bits(float v) { Word b; std::memcpy(&b, &v, 4); return b; }
const char* text(Word a) { return static_cast<const char*>(ptr(a)); }
struct Mem final : kotor::server_mainloop::Memory {
    Word read(Word a) override { if (a == 0x7a39fcu) return word(kotor_menu_window_cells::manager_007a39fc); Word v; std::memcpy(&v, ptr(a), 4); return v; }
    void write(Word a, Word v) override { std::memcpy(ptr(a), &v, 4); }
    Word read16(Word a) override { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
    void write16(Word a, Word v) override { auto x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
    Word read8(Word a) override { std::uint8_t v; std::memcpy(&v, ptr(a), 1); return v; }
};
struct Grow final : kotor::server_mainloop::FactionMemberHelperPorts {
    void grow(Word l, Word n) override { menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(ptr(l)), nullptr, n); }
    // 006449D0 never calls the three ports below (only faction_notify_005be9c0 does); bound to their real providers
    // exactly as runtime1115 faction_member_native.inc: [[server+4]+10010h] CSWSMessage, 004AE910, 00567F90.
    Word message(Word server) override { Mem mm; return mm.read(mm.read(server + 4) + 0x10010); }
    Word player(Word server, Word id) override { return word(kotor_fastcall_004ae910(ptr(server), nullptr, id)); }
    void send(Word message, Word player, Word count, Word list, Word subtype, Word excluded) override {
        (void)recovered_party_list_00567f90(ptr(message), nullptr, player, count, ptr(list), subtype, excluded);
    }
};
struct Native final : w2::party::Ports {
    Mem& m; explicit Native(Mem& v) : m(v) {}
    Word app_007a39fc() override { return word(kotor_menu_window_cells::manager_007a39fc); }
    Word rules_007a3a28() override { return word(kotor_cell_rules_007a3a28); }
    Word literal(Word a) override {
        switch (a) {
        case 0x0073d71cu: return word("");
        case 0x00747228u: return word("EquippedRes");
        case 0x00747200u: return word("InventoryRes");
        case 0x0074721cu: return word("Dropable");
        case 0x00748fbcu: return word("Pickpocketable");
        case 0x00747324u: return word("g_a_clothes01");
        case 0x00749320u: return word("INVENTORY");
        case 0x007490dcu: return word("INV ");
        case 0x00747210u: return word("ItemList");
        case 0x00746f08u: return word("PercentXP");
        case 0x00745338u: return word("V2.0");
        case 0x00747368u: return word("UTC ");
        default: kotor_unreachable_trap(a, "W2 party literal not transcribed");
        }
    }
    void write8(Word a, Word v) override { const auto x = static_cast<std::uint8_t>(v); std::memcpy(ptr(a), &x, 1); }
    [[noreturn]] void trap(Word a, const char* w) override { kotor_unreachable_trap(a, w); }
    Word allocate_006fa7e6(Word n) override { return word(live_operator_new(n)); }
    void vdelete(Word o) override { using F = void*(__thiscall*)(void*, unsigned); (void)reinterpret_cast<F>(m.read(m.read(o)))(ptr(o), 1); }
    Word vcall0(Word o, Word slot) override { using F = void*(__thiscall*)(void*); return word(reinterpret_cast<F>(m.read(m.read(o) + slot))(ptr(o))); }
    Word vcall1_short(Word o, Word slot, Word a) override { using F = short(__thiscall*)(void*, unsigned); return static_cast<Word>(static_cast<std::int32_t>(reinterpret_cast<F>(m.read(m.read(o) + slot))(ptr(o), a))); }
    Word player_creature_id_004aea40(Word s) override { return kotor_fastcall_004aea40(ptr(s), nullptr); }
    Word creature_by_id_004ae770(Word s, Word id) override { return word(kotor_fastcall_004ae770(ptr(s), nullptr, id)); }
    Word item_by_id_004ae760(Word s, Word id) override { return word(recovered_item_lookup_004ae760(ptr(s), nullptr, id)); }
    Word client_object_by_id_004ae910(Word s, Word id) override { return word(kotor_fastcall_004ae910(ptr(s), nullptr, id)); }
    Word client_to_server_id_004aea30(Word s, Word id) override { return kotor_fastcall_004aea30(ptr(s), nullptr, id); }
    Word object_lookup_004d8230(Word a, Word id, Word out) override { return kotor_fastcall_004d8230(ptr(a), nullptr, id, ptr(out)); }
    Word area_by_id_internal_004b17e0(Word in, Word id) override { return word(server_internal_get_area_by_object_id_004b17e0(ptr(in), nullptr, id)); }
    Word module_004ae6b0(Word s) override { return word(server_get_module_004ae6b0(ptr(s), nullptr)); }
    Word module_area_004c30f0(Word mod) override { return word(module_get_area_004c30f0(ptr(mod), nullptr)); }
    Word sw_party_005ed8b0(Word c) override { return word(kotor_fastcall_005ed8b0(ptr(c), nullptr)); }
    Word server_to_client_id_005eda50(Word c, Word id) override { return kotor_fastcall_005eda50(ptr(c), nullptr, id); }
    Word client_creature_005ed5b0(Word c, Word id) override { return word(frame_R4_005ed5b0(ptr(c), nullptr, id)); }
    Word is_player_creature_005eda30(Word c, Word cr) override { return kotor_m15_client_is_player_creature_005eda30(ptr(c), nullptr, ptr(cr)); }
    Word client_options_005ed700(Word c) override { return word(kotor_fastcall_005ed700(ptr(c), nullptr)); }
    void add_game_in_progress_005638d0(Word p) override { w2x_party_mount_005638d0(ptr(p), nullptr); }
    void remove_game_in_progress_00563950(Word p) override { w2x_party_unmount_00563950(ptr(p), nullptr); }
    Word client_set_in_party_0060ef50(Word c, Word f) override { return kotor_fastcall_0060ef50(ptr(c), nullptr, f); }
    Word path_data_00634cc0(Word a, Word i) override { return word(kotor_fastcall_00634cc0(ptr(a), nullptr, static_cast<int>(i))); }
    void follow_array_reset_00634b60(Word a, Word pos, Word yaw) override { kotor_fastcall_00634b60(ptr(a), nullptr, ptr(pos), f32(yaw)); }
    void follow_process_point_00636a30(Word a, Word point, Word yaw, Word weight) override { w5_party_follow_process_point_00636a30(ptr(a), nullptr, ptr(point), f32(yaw), weight); }
    void set_in_party_004fdb20(Word c, Word a, Word b) override { recovered_set_in_party_004fdb20(ptr(c), nullptr, a, b); }
    void set_activity_004f00a0(Word c, Word a, Word b, Word e) override { recovered_set_activity_004f00a0(ptr(c), nullptr, a, b, e); }
    Word gold_004edd60(Word c) override { return w2x_creature_gold_004edd60(ptr(c), nullptr); }
    Word item_repository_004ef770(Word c, Word f) override { return word(recovered_creature_inventory_004ef770(ptr(c), nullptr, f)); }
    void equip_item_004feb90(Word c, Word s, Word it, Word a, Word b, Word d) override { recovered_creature_equip_004feb90(ptr(c), nullptr, s, ptr(it), a, b, d); }
    Word creature_construct_004f7a10(Word st, Word id, Word f) override { return word(recovered_server_creature_construct_004f7a10(ptr(st), nullptr, id, f)); }
    Word creature_template_005026d0(Word c, Word ref, Word f) override { return w2x_creature_template_005026d0(ptr(c), nullptr, ptr(ref), f); }
    void clear_all_actions_004ccd80(Word o, Word f) override { w5_object_clear_all_actions_004ccd80(ptr(o), nullptr, f); }
    void add_to_area_004fa100(Word c, Word a, Word x, Word y, Word z, Word f, Word g) override { recovered_creature_placement_004fa100(ptr(c), nullptr, a, x, y, z, f, g); }
    void set_position_004cd1a0(Word o, Word v, Word a, Word b, Word c) override { recovered_object_position_004cd1a0(ptr(o), nullptr, ptr(v), a, b, c); }
    void set_orientation_004cd170(Word o, Word v) override { kotor::server_mainloop::object_orientation_004cd170(m, o, v); }
    Word effect_construct_00503e40(Word st, Word f) override { return word(recovered_effect_construct_00503e40(ptr(st), nullptr, f)); }
    void apply_effect_004d14f0(Word o, Word e, Word a, Word b) override { recovered_effect_apply_004d14f0(ptr(o), nullptr, ptr(e), a, b); }
    void receive_experience_004ef930(Word c, Word xp) override { recovered_creature_receive_experience_004ef930(ptr(c), nullptr, xp); }
    Word can_level_up_005a6810(Word st) override { return recovered_can_level_up_005a6810(ptr(st), nullptr); }   // W2 t4 body
    void auto_levelup_005b27e0(Word s, Word flag) override { kotor_fastcall_005b27e0(ptr(s), nullptr, flag); }   // levelup worker: was a trap
    Word object_area_004cb120(Word o) override { return word(recovered_object_area_004cb120(ptr(o), nullptr)); }
    Word item_construct_005530a0(Word st, Word id) override { return word(recovered_item_construct_005530a0(ptr(st), nullptr, id)); }
    Word item_template_005608b0(Word it, Word ref) override { return recovered_item_template_005608b0(ptr(it), nullptr, ptr(ref)); }
    void item_fields_0055fcd0(Word it, Word r, Word s) override { recovered_item_fields_0055fcd0(ptr(it), nullptr, ptr(r), ptr(s)); }
    void set_possessor_00553210(Word it, Word o, Word a, Word b, Word c) override { recovered_item_owner_00553210(ptr(it), nullptr, o, a, b, c); }
    Word repository_add_0055d330(Word r, Word slot, Word a, Word b, Word c) override { return recovered_inventory_insert_0055d330(ptr(r), nullptr, static_cast<void**>(ptr(slot)), a, b, c); }
    Word repository_construct_0055d290(Word st, Word o, Word s) override { return word(recovered_inventory_construct_0055d290(ptr(st), nullptr, o, s)); }
    Word repository_item_005560b0(Word r, Word i) override { return word(w2x_inventory_item_005560b0(ptr(r), nullptr, i)); }
    Word base_item_005b31a0(Word a, Word t) override { return word(kotor_m15b_base_item_array_get_base_item_005b31a0(ptr(a), nullptr, static_cast<int>(t))); }
    Word string_equal_005e5200(Word a, Word b) override { return static_cast<Word>(async_string_equal_005e5200(ptr(a), nullptr, ptr(b))); }
    void str_literal_005e5a90(Word s, Word t) override { frame_LogStringCtor_005e5a90(ptr(s), nullptr, text(t)); }
    void str_destroy_005e5c20(Word s) override { frame_LogStringDtor_005e5c20(ptr(s), nullptr); }
    void ref_construct_00406d80(Word r, Word t) override { alignas(4) unsigned char tmp[16]; unresolved_ref_assign_00406290(ptr(r), nullptr, tmp, text(t)); }
    void ref_from_string_00406d60(Word r, Word s) override { (void)async_ref_construct_string_00406d60(ptr(r), nullptr, ptr(s)); }
    Word gff_construct_00410630(Word st, Word a, Word t, Word ref) override { return word(gui_load_gff_construct_00410630(ptr(st), nullptr, a, text(t), ptr(ref))); }
    Word gff_loaded_004064a0(Word g) override { return gui_load_slot_prop10_004064a0(ptr(g), nullptr); }
    void gff_top_00411240(Word r, Word out) override { (void)static_cast<kotor_q05::CResGFF*>(ptr(r))->GetTopLevelStruct(static_cast<kotor_q05::StructHandle*>(ptr(out))); }
    Word gff_list_004118c0(Word r, Word out, Word parent, Word label) override { return ::gff_list_004118c0(ptr(r), nullptr, ptr(out), ptr(parent), text(label)); }
    Word gff_list_count_00411940(Word r, Word l) override { return ::gff_list_count_00411940(ptr(r), nullptr, ptr(l)); }
    Word gff_element_00411990(Word r, Word out, Word l, Word i) override { return gff_list_element_00411990(ptr(r), nullptr, ptr(out), ptr(l), i); }
    Word gff_resref_00411e10(Word r, Word out, Word st, Word l, Word status, Word f) override { return word(gui_load_gff_resref_00411e10(ptr(r), nullptr, ptr(out), ptr(st), text(l), static_cast<unsigned*>(ptr(status)), ptr(f))); }
    Word gff_byte_00411a60(Word r, Word st, Word l, Word status, Word f) override { return gui_load_gff_byte_00411a60(ptr(r), nullptr, ptr(st), text(l), static_cast<unsigned*>(ptr(status)), static_cast<unsigned char>(f)); }
    Word c2da_int_00413660(Word t, Word row, Word label, Word out) override { return static_cast<Word>(c2da_current_get_int_00413660(ptr(t), nullptr, row, ptr(label), static_cast<unsigned*>(ptr(out)))); }
    Word yaw_bits_004aa0f0(Word v) override { return bits(kotor_m15a_yaw_004aa0f0(ptr(v))); }
    Word deg_to_rad_bits_004a96d0(Word d) override { return bits(deg_to_rad_004a96d0(f32(d))); }
    void rotate_about_axis(Word out, Word axis, Word angle, Word in) override {
        W2V3 a; std::memcpy(&a, ptr(axis), sizeof a);
        W2Q4 q; (void)quaternion_construct_004ac960(&q, nullptr, a, f32(angle));
        (void)quaternion_rotate_004a9bb0(static_cast<W2V3*>(ptr(out)), &q, static_cast<const W2V3*>(ptr(in)));
    }
    Word area_safe_point_004be5e0(Word a, Word pos, Word ai) override { return w2x_area_safe_point_004be5e0(ptr(a), nullptr, ptr(pos), ai); }
    Word area_safe_location_004be860(Word a, Word pos, Word r, Word ai, Word f, Word out) override { return w1x_area_safe_location_004be860(ptr(a), nullptr, ptr(pos), f32(r), ai, f, ptr(out)); }
    Word area_room_004bb600(Word a, Word pos, Word out) override { return w2x_area_room_004bb600(ptr(a), nullptr, ptr(pos), static_cast<int*>(ptr(out))); }   // out: int* room index (may be null)
    void vector_cross_004ab210(Word out, Word a, Word b) override { (void)::vector_cross_004ab210(static_cast<W2V3*>(ptr(out)), static_cast<const W2V3*>(ptr(a)), static_cast<const W2V3*>(ptr(b))); }
    Word intersect_segments_0058f840(Word area, const Word (&v)[8], Word oa, Word ob) override {
        return w5_area_intersect_line_segments_0058f840(ptr(area), nullptr, f32(v[0]), f32(v[1]), f32(v[2]), f32(v[3]), f32(v[4]), f32(v[5]), f32(v[6]), f32(v[7]), static_cast<float*>(ptr(oa)), static_cast<float*>(ptr(ob)));
    }
    void aoe_remove_from_subareas_00595b60(Word aoe, Word flag) override { kotor_fastcall_00595b60(ptr(aoe), nullptr, flag); }
    void aoe_update_subareas_00595640(Word aoe, Word previous) override { kotor_fastcall_00595640(ptr(aoe), nullptr, ptr(previous)); }
    void int_list_remove_0066dcc0(Word l, Word v) override { recovered_int_list_remove_0066dcc0(ptr(l), nullptr, v); }
    void list_add_unique_006449d0(Word l, Word v) override { Grow g; kotor::server_mainloop::unique_append_006449d0(m, g, l, v); }
    // 0063765E..0063787B CSWParty::Reset(pos, yaw, flag) flag != 0 arm (closeA). cparty = EBP, position = ESI,
    // [esp+10h] = cparty+2DCh follow array. Formation offset: -(Quaternion(up, yaw*[007415F8]) * (0,1,0)) * 1.5;
    // member 1 slot gets the safe point at offset*1 (+D4h, +E0h := 0), member 2 at offset*2 (+15Ch, +168h := 0);
    // the out vector [esp+20h] is not reset between the two ComputeSafeLocation calls. Every arm ends with
    // ProcessPoint(position, yaw, 0.5f).
    void party_reset_formation_0063765e(Word cp, Word position, Word yaw_bits) override {
        void* const follow = ptr(cp + 0x2dc);                                              // [esp+10h]
        const float yaw = f32(yaw_bits);
        W2V3 axis; std::memcpy(&axis, &kotor_world_los::up_007a19b0, sizeof axis);          // 00637662..00637687 by value
        const float angle = static_cast<float>(static_cast<double>(yaw) * static_cast<double>(f32(0x3c8efa35u))); // 0063765E..00637678 fmul [007415F8], fstp dword
        W2Q4 q; (void)quaternion_construct_004ac960(&q, nullptr, axis, angle);             // 0063768A..0063768E
        const W2V3 forward = {0.0f, 1.0f, 0.0f};                                           // 006376A2..006376B2 [esp+14h]
        W2V3 rotated;
        const W2V3* r = quaternion_rotate_004a9bb0(&rotated, &q, &forward);                // 00637693..006376BA
        W2V3 offset = {-r->x, -r->y, -r->z};                                               // 006376BF..006376F9 fchs
        vector_scale_assign_004a97a0(&offset, nullptr, 1.5f);                              // 006376CA..006376FD
        W2V3 out = {0.0f, 0.0f, 0.0f};                                                     // 00637707..00637717 [esp+20h]
        const Word server = m.read(app_007a39fc() + 8);                                    // 00637702..0063771F
        const Word c = word(kotor_fastcall_004ae770(ptr(server), nullptr,
                                                    kotor_fastcall_004aea30(ptr(server), nullptr, m.read(cp + 0x24)))); // 00637722..00637735
        auto at_offset = [&]() {
            W2V3 p; std::memcpy(&p, ptr(position), sizeof p);                              // fadd [esi], [esi+4], [esi+8]
            return W2V3{static_cast<float>(static_cast<double>(offset.x) + p.x),
                        static_cast<float>(static_cast<double>(offset.y) + p.y),
                        static_cast<float>(static_cast<double>(offset.z) + p.z)};
        };
        if (c != 0) {                                                                      // 00637737..00637739
            W2V3 target = at_offset();                                                     // 0063773F..0063777C [esp+2Ch]
            void* area = kotor_fastcall_004ae780(ptr(server), nullptr, m.read(c + 0x8c));   // 0063774B / 00637780
            if (w1x_area_safe_location_004be860(area, nullptr, &target, 2.0f, m.read(c + 0x340), 1, &out) != 0) { // 00637787..0063778E
                std::memcpy(ptr(cp + 0xd4), &out, sizeof out);                             // 00637790..006377B0
                m.write(cp + 0xe0, 0);                                                     // 006377BD
                w5_party_follow_process_point_00636a30(follow, nullptr, &out, yaw, 0x3f000000u); // 006377A7..006377C7 (0.5f)
            }
            vector_add_assign_004a9740(&offset, nullptr, &offset);                         // 006377CC..006377D5
            target = at_offset();                                                          // 006377DA..00637817
            area = kotor_fastcall_004ae780(ptr(server), nullptr, m.read(c + 0x8c));         // 006377E6 / 0063781B
            if (w1x_area_safe_location_004be860(area, nullptr, &target, 2.0f, m.read(c + 0x340), 1, &out) != 0) { // 00637822..00637829
                std::memcpy(ptr(cp + 0x15c), &out, sizeof out);                            // 0063782B..0063784B
                m.write(cp + 0x168, 0);                                                    // 00637858
                w5_party_follow_process_point_00636a30(follow, nullptr, &out, yaw, 0x3f000000u); // 00637842..00637862
            }
        }
        w5_party_follow_process_point_00636a30(follow, nullptr, ptr(position), yaw, 0x3f000000u); // 00637867..00637876
    }
};
}
namespace q = w2::party;
extern "C" {
void __fastcall recovered_create_party_00565760(void* party, void*) { Mem m; Native p(m); alignas(8) unsigned char s[0x140] = {}; q::create_party_00565760(m, p, word(party), word(s)); }
unsigned __fastcall recovered_spawn_npc_00565130(void* party, void*, unsigned index, unsigned use_given, void* pos, void* ori, unsigned flag) {
    Mem m; Native p(m); alignas(8) unsigned char s[0xb0] = {}; return q::spawn_npc_00565130(m, p, word(party), index, use_given, word(pos), word(ori), flag, word(s));
}
unsigned __fastcall recovered_npc_object_00564700(void* party, void*, unsigned index, unsigned create, unsigned apply) {
    Mem m; Native p(m); alignas(8) unsigned char s[0x50] = {}; return q::npc_object_00564700(m, p, word(party), index, create, apply, word(s));
}
void* __fastcall recovered_party_filename_00563620(void* party, void*, void* out, unsigned index) {
    Mem m; Native p(m); alignas(8) unsigned char s[0x18] = {}; return ptr(q::filename_00563620(m, p, word(party), word(out), index, word(s)));
}
void __fastcall recovered_position_party_00565b00(void* party, void*) { Mem m; Native p(m); alignas(8) unsigned char s[0x100] = {}; q::position_party_00565b00(m, p, word(party), word(s)); }
void __fastcall recovered_transfer_inventory_005641e0(void* party, void*, void* c) { Mem m; Native p(m); Word slot = 0; q::transfer_inventory_005641e0(m, p, word(party), word(c), word(&slot)); }
void __fastcall recovered_unstealth_party_00563c60(void* party, void*, unsigned extra) { Mem m; Native p(m); q::unstealth_party_00563c60(m, p, word(party), extra); }
void __fastcall recovered_force_equip_clothing_00501c40(void* c, void*) { Mem m; Native p(m); alignas(8) unsigned char s[0x30] = {}; q::force_equip_clothing_00501c40(m, p, word(c), word(s)); }
unsigned __fastcall recovered_load_item_00560970(void* item, void*, void* gff, void* st) { Mem m; Native p(m); alignas(8) unsigned char s[0x50] = {}; return q::load_item_00560970(m, p, word(item), word(gff), word(st), word(s)); }
unsigned __fastcall recovered_find_item_with_tag_00555ed0(void* r, void*, void* tag) { Mem m; Native p(m); return q::find_item_with_tag_00555ed0(m, p, word(r), word(tag)); }
unsigned __fastcall recovered_item_in_repository_00555f40(void* r, void*, void* item, unsigned recurse) { Mem m; Native p(m); return q::item_in_repository_00555f40(m, p, word(r), word(item), recurse); }
unsigned __fastcall recovered_remove_item_00555fd0(void* r, void*, void* item) { Mem m; return q::remove_item_00555fd0(m, word(r), word(item)); }
void __fastcall recovered_clear_items_0055f330(void* r, void*) { Mem m; q::clear_items_0055f330(m, word(r)); }
unsigned __fastcall recovered_party_is_member_00563790(void* party, void*, unsigned id) { Mem m; return q::party_is_member_00563790(m, word(party), id); }
unsigned __fastcall recovered_load_from_save_game_004af050(void* s, void*) { Mem m; return q::load_from_save_game_004af050(m, word(s)); }
void* __fastcall recovered_object_array_004aed70(void* s, void*) { Mem m; return ptr(q::object_array_004aed70(m, word(s))); }
unsigned __fastcall recovered_test_safe_location_004b4d70(void* in, void*, unsigned id, void* pos) { Mem m; Native p(m); Word slot = 0; return q::test_safe_location_004b4d70(m, p, word(in), id, word(pos), word(&slot)); }
unsigned __fastcall recovered_test_safe_location_004aeb50(void* s, void*, unsigned id, void* pos) { return recovered_test_safe_location_004b4d70(*static_cast<void**>(static_cast<void*>(static_cast<char*>(s) + 4)), nullptr, id, pos); }
unsigned __fastcall recovered_compute_safe_location_004b4df0(void* in, void*, void* pos, float radius, unsigned id, unsigned flag, void* out) {
    Mem m; Native p(m); Word slot = 0; return q::compute_safe_location_004b4df0(m, p, word(in), word(pos), bits(radius), id, flag, word(out), word(&slot));
}
unsigned __fastcall recovered_compute_safe_location_004aeb60(void* s, void*, void* pos, float radius, unsigned id, unsigned flag, void* out) {
    return recovered_compute_safe_location_004b4df0(*static_cast<void**>(static_cast<void*>(static_cast<char*>(s) + 4)), nullptr, pos, radius, id, flag, out);
}
void* __fastcall recovered_aoe_by_id_004b1970(void* in, void*, unsigned id) { Mem m; Native p(m); Word slot = 0; return ptr(q::aoe_by_id_004b1970(m, p, word(in), id, word(&slot))); }
void* __fastcall recovered_waypoint_by_id_004b19c0(void* in, void*, unsigned id) { Mem m; Native p(m); Word slot = 0; return ptr(q::waypoint_by_id_004b19c0(m, p, word(in), id, word(&slot))); }
void* __fastcall recovered_waypoint_by_id_004ae7e0(void* s, void*, unsigned id) { return recovered_waypoint_by_id_004b19c0(*static_cast<void**>(static_cast<void*>(static_cast<char*>(s) + 4)), nullptr, id); }
unsigned __fastcall recovered_party_leader_slot_00563a40(void* party, void*, unsigned id) { Mem m; return q::party_leader_slot_00563a40(m, word(party), id); }
void* __fastcall recovered_aoe_by_id_004ae7d0(void* s, void*, unsigned id) { return recovered_aoe_by_id_004b1970(*static_cast<void**>(static_cast<void*>(static_cast<char*>(s) + 4)), nullptr, id); }
// closeA (review A-3): JumpToPoint is owned by the aoe lane (claude_aoe.cpp, complete body); this W2 export forwards.
void __fastcall kotor_fastcall_00596360(void*, void*, void*, void*);   // CSWSAreaOfEffectObject::JumpToPoint (RET 8)
void __fastcall recovered_aoe_jump_to_point_00596360(void* aoe, void*, void* area, void* pos) { kotor_fastcall_00596360(aoe, nullptr, area, pos); }
void __fastcall recovered_reset_all_follow_data_00637890(void* cp, void*, void* pos, float yaw, unsigned flag) {
    Mem m; Native p(m); alignas(8) unsigned char s[0x14] = {}; q::reset_all_follow_data_00637890(m, p, word(cp), word(pos), bits(yaw), flag, word(s));
}
void __fastcall recovered_party_reset_00637630(void* cp, void*, void* pos, float yaw, unsigned flag) { Mem m; Native p(m); q::party_reset_00637630(m, p, word(cp), word(pos), bits(yaw), flag); }
void* __fastcall recovered_aoe_position_005946e0(void* aoe, void*, void* out) { Mem m; Native p(m); Word slot = 0; return ptr(q::aoe_position_005946e0(m, p, word(aoe), word(out), word(&slot))); }
unsigned __fastcall recovered_aoe_in_area_of_effect_00595450(void* aoe, void*, float x, float y, float z) {
    Mem m; Native p(m); alignas(8) unsigned char s[0x40] = {}; return q::in_area_of_effect_00595450(m, p, word(aoe), bits(x), bits(y), bits(z), word(s));
}
unsigned __fastcall recovered_aoe_segment_intersect_005950c0(void* aoe, void*, float ax, float ay, float az, float bx, float by, float bz) {
    Mem m; Native p(m); alignas(8) unsigned char s[0x60] = {};
    const Word a[3] = {bits(ax), bits(ay), bits(az)}, b[3] = {bits(bx), bits(by), bits(bz)};
    return q::aoe_segment_005950c0(m, p, word(aoe), a, b, word(s));
}
unsigned __fastcall recovered_encounter_in_activate_area_00590090(void* e, void*, void* pos) { Mem m; return q::encounter_in_area_00590090(m, word(e), word(pos)); }
unsigned __fastcall recovered_encounter_segment_intersect_0058ffd0(void* e, void*, void* a, void* b) { Mem m; Native p(m); alignas(8) unsigned char s[8] = {}; return q::encounter_segment_0058ffd0(m, p, word(e), word(a), word(b), word(s)); }
}

// ===================================================================== W2 tier 5 (w2_npc_items)
#include "02d13fb33c-w2_npc_items.hpp"
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }
namespace kotor_reconstruction { struct StartupString; void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }
extern "C" {
unsigned char __fastcall recovered_stats_level_005a5fd0(void*, void*, unsigned);                 // W1 (in 1122)
void __fastcall w2x_faction_add_member_005bfa70(void*, void*, unsigned, unsigned);              // W2 0886 wrapper (t5)
void __fastcall w2x_movement_rate_005a5680(void*, void*, unsigned);                              // W2 0886 wrapper (t5)
char* __fastcall async_string_cstr_005e5670(void*, void*);
void* __fastcall recovered_gff_construct_004105a0(void*, void*);
unsigned __fastcall recovered_gff_00411260(void*, void*, void*, void*, void*);
unsigned __fastcall recovered_gff_00413030(void*, void*, void*, unsigned);
unsigned __fastcall recovered_inventory_has_item_005a4950(void*, void*, void*);                 // W1 tier5 (in 1125)
unsigned __fastcall recovered_save_creature_00500610(void*, void*, void*, void*);              // W2 t6
unsigned __fastcall recovered_creature_run_unequip_005023a0(void*, void*, unsigned, unsigned, unsigned, void**, unsigned);   // W1 tier7
void* __fastcall kotor_fastcall_004ae750(void*, void*, unsigned);
unsigned __fastcall recovered_item_cost_00554000(void*, void*);                                 // W2 t4 export (w2_g07_native.cpp)
}
namespace {
struct NativeT5 final : w2::npc_items::Ports {
    [[noreturn]] void trap(Word a, const char* w) override { kotor_unreachable_trap(a, w); }
    Word stats_level_005a5fd0(Word s, Word f) override { return recovered_stats_level_005a5fd0(ptr(s), nullptr, f); }
    Word creature_faction_00513fc0(Word c) override { Mem m; return kotor::server_mainloop::creature_faction_00513fc0(m, c); }
    void faction_add_member_005bfa70(Word f, Word id, Word flag) override { w2x_faction_add_member_005bfa70(ptr(f), nullptr, id, flag); }
    void stats_movement_rate_005a5680(Word s, Word r) override { w2x_movement_rate_005a5680(ptr(s), nullptr, r); }
    void str_default_005b3190(Word s) override { (void)kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(ptr(s)), nullptr); }
    void str_format_game_in_progress_005e5680(Word out, Word str) override {
        Mem m;   // CExoString passed by value: (text, length) as the original pushes [eax], [eax+4]
        kotor_reconstruction::StartupFormat005e5680(static_cast<kotor_reconstruction::StartupString*>(ptr(out)), "GAMEINPROGRESS:%s", ptr(m.read(str)), m.read(str + 4));
    }
    Word str_cstr_005e5670(Word s) override { return word(async_string_cstr_005e5670(ptr(s), nullptr)); }
    Word gff_construct_004105a0(Word mem) override { return word(recovered_gff_construct_004105a0(ptr(mem), nullptr)); }
    Word gff_create_00411260(Word g, Word top, Word type, Word ver) override { return recovered_gff_00411260(ptr(g), nullptr, ptr(top), ptr(type), ptr(ver)); }
    Word save_creature_00500610(Word c, Word g, Word top) override { return recovered_save_creature_00500610(ptr(c), nullptr, ptr(g), ptr(top)); }   // W2 t6
    Word gff_write_00413030(Word g, Word name, Word type) override { return recovered_gff_00413030(ptr(g), nullptr, ptr(name), type); }
    Word inventory_has_item_005a4950(Word inv, Word item) override { return recovered_inventory_has_item_005a4950(ptr(inv), nullptr, ptr(item)); }
    void run_unequip_005023a0(Word c, Word id, Word target, Word a2, Word out, Word flag) override { (void)recovered_creature_run_unequip_005023a0(ptr(c), nullptr, id, target, a2, static_cast<void**>(ptr(out)), flag); }   // W1 tier7 (ruling)
    Word game_object_004ae750(Word s, Word id) override { return word(kotor_fastcall_004ae750(ptr(s), nullptr, id)); }
    Word item_cost_00554000(Word item) override { return recovered_item_cost_00554000(ptr(item), nullptr); }
    void array_grow_005e03c0(Word l, Word n) override { menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(ptr(l)), nullptr, n); }
};
}
namespace t5 = w2::npc_items;
extern "C" {
unsigned __fastcall recovered_party_npc_available_005636b0(void* party, void*, unsigned npc) { Mem m; return t5::npc_available_005636b0(m, word(party), npc); }
unsigned __fastcall recovered_party_npc_selectable_005637c0(void* party, void*, unsigned npc) { Mem m; return t5::npc_selectable_005637c0(m, word(party), npc); }
unsigned __fastcall recovered_party_add_npc_creature_00564300(void* party, void*, unsigned npc, void* c) {
    Mem m; Native p(m); NativeT5 x; alignas(8) unsigned char s[0x60] = {}; return t5::add_npc_00564300(m, p, x, word(party), npc, word(c), word(s));
}
unsigned __fastcall recovered_party_add_npc_005645f0(void* party, void*, unsigned npc, void* template_str) {
    Mem m; Native p(m); NativeT5 x; alignas(8) unsigned char s[0xa0] = {}; return t5::add_npc_template_005645f0(m, p, x, word(party), npc, word(template_str), word(s));
}
void __fastcall recovered_item_set_identified_00554170(void* item, void*, unsigned flag) { Mem m; t5::set_identified_00554170(m, word(item), flag); }
unsigned __fastcall recovered_creature_remove_item_00510ef0(void* c, void*, void* item, unsigned a2, unsigned a3, void* out) {
    Mem m; Native p(m); NativeT5 x; return t5::creature_remove_item_00510ef0(m, p, x, word(c), word(item), a2, a3, word(out));
}
unsigned __fastcall recovered_placeable_remove_item_00584ac0(void* pl, void*, void* item) { Mem m; Native p(m); return t5::placeable_remove_item_00584ac0(m, p, word(pl), word(item)); }
void __fastcall recovered_repository_insert_item_0055d2c0(void* r, void*, void* item, unsigned pos) { Mem m; NativeT5 x; t5::repository_insert_item_0055d2c0(m, x, word(r), word(item), pos); }
unsigned __fastcall recovered_store_add_item_005c70c0(void* store, void*, void* cell) { Mem m; Native p(m); NativeT5 x; return t5::store_add_item_005c70c0(m, p, x, word(store), word(cell)); }
}
