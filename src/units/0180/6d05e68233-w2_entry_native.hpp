#pragma once
// Native binding of w2::entry::Ports. Each callee is bound to an existing 1115
// owner (symbols verified in kotor-vm-cell1115.map), to a W2-owned body, to a
// pending owner's declared native entry (provided only as explicit trap stubs
// by w2_pending_owner_stubs.cpp until that owner lands), or to an explicit
// kotor_unreachable_trap with the original address.
#include "2346527c32-w2_native_ports.hpp"
#include "72239a78a7-w2_entry.hpp"
#include "f0c71cbcb2-object_position.hpp"
#include "1e4deaabc8-area_attachment.hpp"
#include <cstring>

namespace kotor_reconstruction { struct StartupString; void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }
namespace options_video { struct String; }
namespace client_movies { std::int32_t __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*); }
namespace recovered_creature_area_access { using U = unsigned; struct Ports { virtual ~Ports() = default; virtual U read(U) = 0; virtual void write(U, U) = 0; virtual U invalid() = 0; }; void set(U, U, Ports&); }
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }
namespace menu_continuation { struct ExoTimers { __int64 __thiscall GetHighResolutionTimer_005e9260(); }; }
extern "C" {
// W2 t4 G07 writers (w2_g07_native.cpp)
unsigned __fastcall recovered_test_object_visible_0056dc60(void*, void*, void*, void*);
void __fastcall recovered_delete_for_object_0056e190(void*, void*, void*, unsigned);
void __fastcall recovered_update_object_00574a10(void*, void*, void*, void*, void*, unsigned, unsigned);
void __fastcall recovered_test_update_differences_00577fa0(void*, void*, void*, void*, void*, void*, void*);
void __fastcall recovered_store_values_00576fc0(void*, void*, void*, void*, void*, unsigned, unsigned);
void __fastcall recovered_major_gui_panels_00577670(void*, void*, void*);
unsigned __fastcall recovered_party_ai_state_005779f0(void*, void*, void*);
unsigned __fastcall recovered_charsheet_required_00561260(void*, void*, void*);
void __fastcall recovered_write_charsheet_0056f490(void*, void*, void*, unsigned);
void __fastcall recovered_last_update_destroy_005ca6a0(void*, void*);
unsigned __fastcall recovered_module_get_waypoint_004c6e00(void*, void*, const void*);   // W7 t4 (link dependency)
void* __fastcall recovered_waypoint_by_id_004ae7e0(void*, void*, unsigned);          // W2 t4 (w2_party_native.cpp)
void __fastcall recovered_position_party_00565b00(void*, void*);   // W2 t3 (w2_party_native.cpp)
extern void* kotor_cell_rules_007a3a28;
void __fastcall c2da_005e9c10(void*, void*, void*);
void* __fastcall server_internal_get_module_004b14f0(void*, void*);
void* __fastcall module_get_area_004c30f0(void*, void*);
void* __fastcall kotor_fastcall_00561080(void*, void*);
void* __fastcall kotor_fastcall_004ae750(void*, void*, unsigned);
void* __fastcall recovered_object_area_004cb120(void*, void*);
void* __fastcall kotor_fastcall_004ae780(void*, void*, unsigned);
void* __fastcall kotor_fastcall_004ae770(void*, void*, unsigned);
void* __fastcall recovered_server_creature_construct_004f7a10(void*, void*, unsigned, unsigned);
void __fastcall GuiAssign005e5140(void*, void*, const char*);
char __fastcall c2da_current_string_index_005e5600(void*, void*, int);
int __fastcall c2da_current_get_string_00413270(void*, void*, int, void*, void*);
int __fastcall c2da_current_get_int_00413660(void*, void*, int, void*, int*);
int __fastcall exostring_compare_no_case_005e6450(void*, void*, const void*);
int __cdecl crt_rand_006fc447(void);
unsigned __fastcall frame_GetPauseState_004ae980(void*, void*, unsigned);
unsigned __fastcall kotor_fastcall_0056c9f0(void*, void*, unsigned, unsigned);
void* __fastcall recovered_event_construct_004d7540(void*, void*);
void __fastcall recovered_event_object_004d7780(void*, void*, unsigned, unsigned);
void __fastcall recovered_event_enqueue_004b08d0(void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned, void*);
unsigned __fastcall recovered_creature_incapacitated_004ef890(void*, void*);
void __fastcall recovered_creature_placement_004fa100(void*, void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned);
void __fastcall exo_locstring_construct_005e9e10(void*, void*);
void __fastcall exo_locstring_destroy_005ea000(void*, void*);
void* __fastcall recovered_localized_assign_005e9e70(void*, void*, const void*);
unsigned char __fastcall kotor_fastcall_004d8230(void*, void*, unsigned, void*);
void* __fastcall recovered_placeable_lookup_004ae7a0(void*, void*, unsigned);
unsigned __fastcall recovered_placeable_light_get_00585130(void*, void*);
unsigned __fastcall async_string_equal_005e5200(void*, void*, const void*);
// Pending owners: declared native entries; until the owner lands they resolve
// to the explicit trap stubs in w2_pending_owner_stubs.cpp.
void* __fastcall recovered_party_repository_00563340(void* party, void*);             // W2 tier 2 (w2_party_repository_native.cpp)
float __fastcall recovered_creature_spot_distance_004efc70(void*, void*);             // W1 tier3b (thiscall, ST0)
float __fastcall recovered_creature_listen_distance_004efd20(void*, void*);           // W1 tier3b (thiscall, ST0)
unsigned __fastcall w1x_area_safe_location_004be860(void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned); // W1 0886 wrapper
void __fastcall recovered_creature_update_visible_0051b050(void*, void*, unsigned);      // W1
unsigned __fastcall recovered_update_subareas_0051b940(void*, void*, void*, unsigned, unsigned); // W2 tier 2
unsigned __fastcall recovered_load_character_00561e30(void*, void*, unsigned, unsigned, unsigned); // W2 tier 2
unsigned __fastcall w2x_run_module_004b6270(void*, void*);                                // W2 0886 wrapper (w2_0886_exports.inc)
void __fastcall recovered_repository_parent_00556150(void* repository, void*, unsigned id); // W2 t2b (w2_party_repository_native.cpp)
void __fastcall recovered_last_update_clear_actions_005ca0d0(void*, void*);           // W2 t4 bodies (w2_g07_native.cpp)
void __fastcall recovered_last_update_clear_spells_005cab20(void*, void*);
void __fastcall recovered_last_update_clear_spell_uses_005ca060(void*, void*);
void __fastcall recovered_last_update_clear_spell_lists_005cac30(void*, void*);
void __fastcall recovered_last_update_clear_feats_005cab60(void*, void*);
// closeA: former trap ports, bound to their c35 providers
unsigned __fastcall kotor_fastcall_004b58a0(void* internal, void*, unsigned number, void* name, unsigned player, unsigned unused); // CServerExoAppInternal::SaveGame RET 10h (claude_closeB_save)
unsigned __fastcall kotor_fastcall_004ba640(void* internal, void*, unsigned id, void* name, void* module, unsigned player); // CServerExoAppInternal::LoadGame RET 10h (claude_loadgame)
void __fastcall kotor_fastcall_00507220(void* area, void*);                          // CSWSArea::IncreaseAILevelPriority (claude_stale_bodies_ports)
void __fastcall kotor_fastcall_00507340(void* area, void*);                          // CSWSArea::DecreaseAILevelPriority (claude_combat_spell_mine)
unsigned __fastcall kotor_fastcall_0056b400(void* message, void*, unsigned player, unsigned creature, unsigned flag); // SendServerToPlayerPlayModuleCharacterListResponse (claude_p2s_b_misc)
}
namespace w2::native {
// 0059A3EF..0059A4BD CSWSScriptVarTable::MatchIndex create arm (defined in w2_entry_native.cpp, closeA).
Word var_create_0059a3ef_native(Word table, Word name, Word type);
}

namespace w2::native {
struct AreaMemory final : kotor::character_load::Memory {
    Word read(Word a) const override { if (a == 0x7a39fcu) return word(kotor_menu_window_cells::manager_007a39fc); Word v; std::memcpy(&v, ptr(a), 4); return v; }
    void write(Word a, Word v) override { std::memcpy(ptr(a), &v, 4); }
    std::uint8_t read_byte(Word a) const override { std::uint8_t v; std::memcpy(&v, ptr(a), 1); return v; }
    void write_byte(Word a, std::uint8_t v) override { std::memcpy(ptr(a), &v, 1); }
    void write_short(Word a, std::uint16_t v) override { std::memcpy(ptr(a), &v, 2); }
};
struct AreaPorts final : kotor::area_attachment::Ports {
    std::uint8_t get_game_object_004d8230(Word a, Word id, Word& out) override { return kotor_fastcall_004d8230(ptr(a), nullptr, id, &out); }
    Word as_area_slot_28(Word f, Word o) override { using F = void*(__thiscall*)(void*); return word(reinterpret_cast<F>(f)(ptr(o))); }
    void increase_ai_priority_00507220(Word a) override { kotor_fastcall_00507220(ptr(a), nullptr); }
    void decrease_ai_priority_00507340(Word a) override { kotor_fastcall_00507340(ptr(a), nullptr); }
};
struct AreaAccess final : recovered_creature_area_access::Ports {
    unsigned read(unsigned a) override { Word v; std::memcpy(&v, ptr(a), 4); return v; }
    void write(unsigned a, unsigned v) override { std::memcpy(ptr(a), &v, 4); }
    unsigned invalid() override { return 0x7f000000u; }                   // [00746644]
};

struct EntryPorts : w2::entry::Ports {
    Mem& m; MessagePorts base;
    explicit EntryPorts(Mem& memory) : m(memory), base(memory) {}
    // ---- forward msg::LocPorts to the shared message binding ----
    Word allocate(Word n) override { return base.allocate(n); }
    void free(Word a) override { base.free(a); }
    void write_byte(Word a, Word v) override { base.write_byte(a, v); }
    Word network_getter(Word a) override { return base.network_getter(a); }
    Word send_005d4cb0(Word a, Word b, Word c, Word d, Word e) override { return base.send_005d4cb0(a, b, c, d, e); }
    void copy(Word d, Word s, Word n) override { base.copy(d, s, n); }
    Word str_length_005e5790(Word s) override { return base.str_length_005e5790(s); }
    Word str_cstr_005e5670(Word s) override { return base.str_cstr_005e5670(s); }
    void str_literal_005e5a90(Word s, Word t) override { base.str_literal_005e5a90(s, t); }
    void str_counted_005e5b70(Word s, Word t, Word n) override { base.str_counted_005e5b70(s, t, n); }
    void str_copy_005e5b00(Word d, Word s) override { base.str_copy_005e5b00(d, s); }
    void str_destroy_005e5c20(Word s) override { base.str_destroy_005e5c20(s); }
    void ref_assign_text_00406290(Word a, Word b, Word c) override { base.ref_assign_text_00406290(a, b, c); }
    void ref_assign_raw_00406170(Word a, Word b, Word c) override { base.ref_assign_raw_00406170(a, b, c); }
    Word empty_text_0073d71c() override { return base.empty_text_0073d71c(); }
    Word list_at_005e9b60(Word l, Word p) override { return base.list_at_005e9b60(l, p); }
    Word list_next_005e9b90(Word l, Word s) override { return base.list_next_005e9b90(l, s); }
    Word locinternal_get_005ec9c0(Word i, Word x, Word o) override { return base.locinternal_get_005ec9c0(i, x, o); }
    Word get_string_005e9eb0(Word a, Word b, Word c, Word d, Word e) override { return base.get_string_005e9eb0(a, b, c, d, e); }
    void str_empty_005b3190(Word s) override { base.str_empty_005b3190(s); }
    Word server() override { return base.server(); }
    Word tlk_fetch_0041e550(Word a, Word b, Word c, Word d) override { return base.tlk_fetch_0041e550(a, b, c, d); }
    Word tlk() override { return base.tlk(); }
    void ref_default_00405ed0(Word r) override { base.ref_default_00405ed0(r); }
    void ref_assign_lower_00406120(Word a, Word b, Word c) override { base.ref_assign_lower_00406120(a, b, c); }
    // ---- entry ports ----
    Word literal(Word a) override {
        switch (a) {
        case 0x0073d71cu: return word("");
        case 0x00749368u: return word("TileSet");
        case 0x00749360u: return word("TILESET");
        case 0x007454e4u: return word("NORMALHEAD");
        case 0x00747b3cu: return word("NW_TOTAL_MAP_PINS");
        case 0x00747b28u: return word("NW_MAP_PIN_NTRY_%i");
        case 0x00747b14u: return word("NW_MAP_PIN_AREA_%i");
        case 0x00747b00u: return word("NW_MAP_PIN_XPOS_%i");
        case 0x00747aecu: return word("NW_MAP_PIN_YPOS_%i");
        case 0x00747ad8u: return word("NW_MAP_PIN_ZPOS_%i");
        case 0x00748da4u: return word("MAXSINGLEITEMVALUE");   // GetMinEquipLevel 00554080 (itemlevel.2da column)
        default: trap(a, "W2 literal not transcribed");
        }
    }
    Word app_007a39fc() override { return word(kotor_menu_window_cells::manager_007a39fc); }
    Word rules_007a3a28() override { return word(kotor_cell_rules_007a3a28); }
    Word internal_module_004b14f0(Word a) override { return word(server_internal_get_module_004b14f0(ptr(a), nullptr)); }
    Word module_area_004c30f0(Word a) override { return word(module_get_area_004c30f0(ptr(a), nullptr)); }
    Word game_object_00561080(Word a) override { return word(kotor_fastcall_00561080(ptr(a), nullptr)); }
    Word game_object_by_id_004ae750(Word s, Word id) override { return word(kotor_fastcall_004ae750(ptr(s), nullptr, id)); }
    Word object_area_004cb120(Word a) override { return word(recovered_object_area_004cb120(ptr(a), nullptr)); }
    Word area_by_id_004ae780(Word s, Word id) override { return word(kotor_fastcall_004ae780(ptr(s), nullptr, id)); }
    Word creature_by_id_004ae770(Word s, Word id) override { return word(kotor_fastcall_004ae770(ptr(s), nullptr, id)); }
    Word vcall0(Word o, Word slot) override { using F = unsigned(__thiscall*)(void*); return reinterpret_cast<F>(m.read(m.read(o) + slot))(ptr(o)); }
    Word vcall1(Word o, Word slot, Word a) override { using F = unsigned(__thiscall*)(void*, unsigned); return reinterpret_cast<F>(m.read(m.read(o) + slot))(ptr(o), a); }
    Word creature_construct_004f7a10(Word mem, Word id, Word flag) override { return word(recovered_server_creature_construct_004f7a10(ptr(mem), nullptr, id, flag)); }
    void set_orientation_004cd170(Word o, Word v) override { kotor::server_mainloop::object_orientation_004cd170(m, o, v); }
    void set_area_004cb160(Word o, Word a) override { AreaAccess p; recovered_creature_area_access::set(o, a, p); }
    void str_assign_005e5c50(Word d, Word s) override { async_assign_string_005e5c50(ptr(d), nullptr, ptr(s)); }
    void str_assign_text_005e5140(Word d, Word t) override { GuiAssign005e5140(ptr(d), nullptr, static_cast<const char*>(ptr(t))); }
    Word str_char_005e5600(Word s, Word i) override { return static_cast<unsigned char>(c2da_current_string_index_005e5600(ptr(s), nullptr, static_cast<int>(i))); }
    Word c2da_string_00413270(Word t, Word r, Word c, Word o) override { return static_cast<Word>(c2da_current_get_string_00413270(ptr(t), nullptr, static_cast<int>(r), ptr(c), ptr(o))); }
    Word c2da_int_00413660(Word t, Word r, Word c, Word o) override { return static_cast<Word>(c2da_current_get_int_00413660(ptr(t), nullptr, static_cast<int>(r), ptr(c), static_cast<int*>(ptr(o)))); }
    Word compare_no_case_005e6450(Word a, Word b) override { return static_cast<Word>(exostring_compare_no_case_005e6450(ptr(a), nullptr, ptr(b))); }
    Word rand_006fc447() override { return static_cast<Word>(crt_rand_006fc447()); }
    Word atan2_bits(Word y, Word x) override {
        float fy, fx, r; std::memcpy(&fy, &y, 4); std::memcpy(&fx, &x, 4);
        __asm { fld fy
                fld fx
                fpatan
                fstp r }
        Word out; std::memcpy(&out, &r, 4); return out;
    }
    Word pack_area_0050aaa0(Word area, Word a, Word b, Word c, Word player) override {
        alignas(8) unsigned char scratch[0x100] = {};
        return w2::entry::pack_area_0050aaa0(m, *this, area, a, b, c, player, word(scratch));
    }
    Word str_chr_006fb5e0(Word t, Word ch) override { return word(std::strchr(static_cast<const char*>(ptr(t)), static_cast<int>(ch))); }
    Word pause_state_004ae980(Word s, Word k) override { return frame_GetPauseState_004ae980(ptr(s), nullptr, k); }
    Word set_pause_state_0056c9f0(Word msg, Word a, Word b) override { return kotor_fastcall_0056c9f0(ptr(msg), nullptr, a, b); }
    Word save_game_004b58a0(Word in, Word number, Word name, Word player, Word unused) override { return kotor_fastcall_004b58a0(ptr(in), nullptr, number, ptr(name), player, unused); }   // closeB body
    Word load_game_004ba640(Word in, Word id, Word name, Word module, Word player) override { return kotor_fastcall_004ba640(ptr(in), nullptr, id, ptr(name), ptr(module), player); }   // loadgame body
    Word run_module_004b6270(Word internal) override { return w2x_run_module_004b6270(ptr(internal), nullptr); }
    Word event_construct_004d7540(Word mem) override { return word(recovered_event_construct_004d7540(ptr(mem), nullptr)); }
    void event_object_004d7780(Word e, Word i, Word id) override { recovered_event_object_004d7780(ptr(e), nullptr, i, id); }
    void event_enqueue_004b08d0(Word ai, Word a, Word b, Word c, Word d, Word e, Word f) override { recovered_event_enqueue_004b08d0(ptr(ai), nullptr, a, b, c, d, e, ptr(f)); }
    Word dead_temp_004ef890(Word c) override { return recovered_creature_incapacitated_004ef890(ptr(c), nullptr); }
    void load_character_ifo_00561e30(Word player, Word index, Word a, Word b) override { (void)recovered_load_character_00561e30(ptr(player), nullptr, index, a, b); }
    void character_list_response_0056b400(Word msg, Word player, Word creature, Word flag) override { (void)kotor_fastcall_0056b400(ptr(msg), nullptr, player, creature, flag); }
    void enforce_no_pc_head_cloning_00508820(Word area, Word head) override { alignas(8) unsigned char sc[0x10] = {}; w2::entry::no_pc_head_cloning_00508820(m, *this, area, head, word(sc)); }
    Word party_repository_00563340(Word party) override { return word(recovered_party_repository_00563340(ptr(party), nullptr)); }
    void set_parent_id_00556150(Word r, Word id) override { recovered_repository_parent_00556150(ptr(r), nullptr, id); }
    void increment_players_00508c20(Word a) override { AreaMemory am; AreaPorts ap; kotor::area_attachment::increment_players_00508c20(am, ap, a); }
    void decrement_players_00508c40(Word a) override { AreaMemory am; AreaPorts ap; kotor::area_attachment::decrement_players_00508c40(am, ap, a); }
    void clear_action_queue_005ca0d0(Word l) override { recovered_last_update_clear_actions_005ca0d0(ptr(l), nullptr); }
    void clear_known_spells_005cab20(Word l) override { recovered_last_update_clear_spells_005cab20(ptr(l), nullptr); }
    void clear_known_spell_uses_005ca060(Word l) override { recovered_last_update_clear_spell_uses_005ca060(ptr(l), nullptr); }
    void clear_spell_add_delete_005cac30(Word l) override { recovered_last_update_clear_spell_lists_005cac30(ptr(l), nullptr); }
    void clear_feats_005cab60(Word l) override { recovered_last_update_clear_feats_005cab60(ptr(l), nullptr); }
    Word waypoint_004c6e00(Word mod, Word tag) override { return recovered_module_get_waypoint_004c6e00(ptr(mod), nullptr, ptr(tag)); }   // W7 t4
    Word waypoint_by_id_004ae7e0(Word s, Word id) override { return word(recovered_waypoint_by_id_004ae7e0(ptr(s), nullptr, id)); }   // W2 t4
    Word safe_location_004be860(Word area, Word pos, Word radius, Word a, Word b, Word out) override { return w1x_area_safe_location_004be860(ptr(area), nullptr, pos, radius, a, b, out); }
    void update_subareas_0051b940(Word c, Word pos, Word id, Word flag) override { (void)recovered_update_subareas_0051b940(ptr(c), nullptr, ptr(pos), id, flag); }
    void add_to_area_004fa100(Word c, Word a, Word x, Word y, Word z, Word f, Word g) override { recovered_creature_placement_004fa100(ptr(c), nullptr, ptr(a), x, y, z, f, g); }
    void update_visible_0051b050(Word c, Word flag) override { recovered_creature_update_visible_0051b050(ptr(c), nullptr, flag); }
    void position_party_00565b00(Word party) override { recovered_position_party_00565b00(ptr(party), nullptr); }   // W2 t3
    void loc_construct_005e9e10(Word l) override { exo_locstring_construct_005e9e10(ptr(l), nullptr); }
    void loc_destroy_005ea000(Word l) override { exo_locstring_destroy_005ea000(ptr(l), nullptr); }
    void loc_assign_005e9e70(Word d, Word s) override { (void)recovered_localized_assign_005e9e70(ptr(d), nullptr, ptr(s)); }
    Word overflow_004d6230(Word s) override { return recovered_player_message::overrun_004d6230(ptr(s)); }
    Word underflow_004d6240(Word s) override { return recovered_player_message::underrun_004d6240(ptr(s)); }
    void fault(Word pc, const char* what) override { trap(pc, what); }
    Word object_lookup_004d8230(Word a, Word id, Word out) override { return kotor_fastcall_004d8230(ptr(a), nullptr, id, ptr(out)); }
    Word placeable_by_id_004ae7a0(Word s, Word id) override { return word(recovered_placeable_lookup_004ae7a0(ptr(s), nullptr, id)); }
    Word placeable_light_00585130(Word pl) override { return recovered_placeable_light_get_00585130(ptr(pl), nullptr); }
    Word str_equal_005e5200(Word a, Word b) override { return async_string_equal_005e5200(ptr(a), nullptr, ptr(b)); }
    void str_format_int_005e5680(Word s, Word f, Word v) override { kotor_reconstruction::StartupFormat005e5680(static_cast<kotor_reconstruction::StartupString*>(ptr(s)), static_cast<const char*>(ptr(f)), v); }
    Word str_not_equal_text_005e5390(Word s, Word t) override { return static_cast<Word>(client_movies::StringNotEqual_005e5390(static_cast<options_video::String*>(ptr(s)), nullptr, static_cast<const char*>(ptr(t)))); }
    Word var_create_0059a3ef(Word table, Word name, Word type) override { return var_create_0059a3ef_native(table, name, type); }
    // ---- G07 ----
    std::uint64_t high_resolution_timer_005e9260() override {
        void* timers; std::memcpy(&timers, static_cast<unsigned char*>(kotor_menu_10_19::dependencies::base_007a39e0) + 4, 4);
        return static_cast<std::uint64_t>(static_cast<menu_continuation::ExoTimers*>(timers)->GetHighResolutionTimer_005e9260());
    }
    void list_remove_005e9c10(Word l, Word pos) override { c2da_005e9c10(ptr(l), nullptr, ptr(pos)); }
    void last_update_destroy_005ca6a0(Word l) override { recovered_last_update_destroy_005ca6a0(ptr(l), nullptr); }
    Word test_object_visible_0056dc60(Word self, Word o, Word obs) override { return recovered_test_object_visible_0056dc60(ptr(self), nullptr, ptr(o), ptr(obs)); }
    void test_update_differences_00577fa0(Word self, Word pl, Word o, Word last, Word a, Word b) override { recovered_test_update_differences_00577fa0(ptr(self), nullptr, ptr(pl), ptr(o), ptr(last), ptr(a), ptr(b)); }
    void update_object_00574a10(Word self, Word pl, Word o, Word last, Word a, Word b) override { recovered_update_object_00574a10(ptr(self), nullptr, ptr(pl), ptr(o), ptr(last), a, b); }
    void store_values_00576fc0(Word self, Word pl, Word last, Word o, Word a, Word b) override { recovered_store_values_00576fc0(ptr(self), nullptr, ptr(pl), ptr(last), ptr(o), a, b); }
    void delete_for_object_0056e190(Word self, Word pl, Word id) override { recovered_delete_for_object_0056e190(ptr(self), nullptr, ptr(pl), id); }
    void major_gui_panels_00577670(Word self, Word pl) override { recovered_major_gui_panels_00577670(ptr(self), nullptr, ptr(pl)); }
    void party_ai_state_005779f0(Word self, Word pl) override { (void)recovered_party_ai_state_005779f0(ptr(self), nullptr, ptr(pl)); }
    Word charsheet_required_00561260(Word gui, Word pl) override { return recovered_charsheet_required_00561260(ptr(gui), nullptr, ptr(pl)); }
    void write_charsheet_0056f490(Word self, Word pl, Word f) override { recovered_write_charsheet_0056f490(ptr(self), nullptr, ptr(pl), f); }
    Word spot_distance_004efc70(Word c) override { const float f = recovered_creature_spot_distance_004efc70(ptr(c), nullptr); Word w; std::memcpy(&w, &f, 4); return w; }
    Word listen_distance_004efd20(Word c) override { const float f = recovered_creature_listen_distance_004efd20(ptr(c), nullptr); Word w; std::memcpy(&w, &f, 4); return w; }
};
}
