// W2 tier 2 native binding + exports for 00561E30 LoadCharacterFromIFO and its
// unowned callees (004FDB20, 004F79E0, 004F0020, 004F17D0, 005633C0,
// 004F00A0, 0050EE30, 0050EE50, 004F6D70) and 0051B940. 004BF470/00516020 -> W5; 005C5A30 -> 0773.
// Providers: symbols verified in build/1121/windows-link/kotor-vm-cell1121.map;
// w2x_* come from w2_0886_exports.inc (compiled inside 0886).
#if !defined(_M_IX86)
#error W2 native bindings require the Windows x86 route
#endif
#include "056127edc0-w2_ifo.hpp"
#include "f0c71cbcb2-object_position.hpp"
#include <cstdint>
#include <cstring>
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace kotor_reconstruction { struct StartupString; }
namespace menu_resources { struct Manager { unsigned __thiscall directory_00408800(kotor_reconstruction::StartupString*); }; Manager* __cdecl BorrowCanonicalManager007a39e8(); }
namespace kotor_input_hover { struct DeviceBuffer { DeviceBuffer* Construct005c5a30(); }; }   // 0773 owner of 005C5A30
namespace kotor_q05 { struct StructHandle; struct CResGFF { unsigned __thiscall GetTopLevelStruct(StructHandle*); }; }
namespace kotor_runtime_reporter_dependencies { char* __cdecl string_copy_n_006fab80(char*, const char*, unsigned); }
namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned); }
extern "C" {
int __fastcall kotor_fastcall_004cb4f0(void*, void*);   // 004CB4F0 CSWSObject::StopDialog (D2 stop_dialog_004cb4f0.obj)
// closeA: CSWSJournal setters (thiscall(CExoString* tag, int value, int flag), RET 0Ch; claude_dialogue_server.obj)
void __fastcall kotor_fastcall_005c5a40(void*, void*, void*, unsigned, unsigned);   // CSWSJournal::SetState
void __fastcall kotor_fastcall_005c63f0(void*, void*, void*, unsigned, unsigned);   // CSWSJournal::SetDate
void __fastcall kotor_fastcall_005c6650(void*, void*, void*, unsigned, unsigned);   // CSWSJournal::SetTime
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
void __fastcall frame_LogStringCtor_005e5a90(void*, void*, const char*);
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);
void __fastcall async_assign_string_005e5c50(void*, void*, const void*);
void __fastcall kotor_fastcall_004088d0(void*, void*, void*);
void __fastcall unresolved_ref_assign_00406290(void*, void*, void*, const char*);
void* __fastcall c2da_lookup_004074d0(void*, void*, void*, unsigned);
void* __fastcall recovered_gff_construct_004105a0(void*, void*);
void __fastcall c2da_register_00407680(void*, void*, void*, unsigned, void*);
unsigned __fastcall c2da_current_resource_demand_00409b10(void*, void*);
unsigned __fastcall c2da_release_demand_00409b80(void*, void*);
int __fastcall c2da_release_00409cf0(void*, void*, void*);
unsigned __fastcall gff_list_004118c0(void*, void*, void*, void*, const char*);
unsigned __fastcall gff_list_count_00411940(void*, void*, void*);
unsigned __fastcall gff_list_element_00411990(void*, void*, void*, void*, unsigned);
unsigned __fastcall gui_load_gff_dword_00411c20(void*, void*, void*, const char*, unsigned*, unsigned);
unsigned char __fastcall gui_load_gff_byte_00411a60(void*, void*, void*, const char*, unsigned*, unsigned char);
int __fastcall gui_load_gff_int_00411c90(void*, void*, void*, const char*, unsigned*, int);
unsigned short __fastcall recovered_gff_word_00411b40(void*, void*, void*, const char*, unsigned*, unsigned short);
float __fastcall gui_load_gff_float_00411d00(void*, void*, void*, const char*, unsigned*, float);
void* __fastcall gui_load_gff_string_00411ec0(void*, void*, void*, void*, const char*, unsigned*, void*);
void* __fastcall recovered_server_creature_construct_004f7a10(void*, void*, unsigned, unsigned);
void __fastcall recovered_set_game_object_00561c40(void*, void*, void*);
void __fastcall recovered_creature_inventory_004ffda0(void*, void*, void*, void*, unsigned, unsigned, unsigned);
void __fastcall recovered_creature_scripts_004ebf20(void*, void*, void*, void*);
unsigned __fastcall recovered_ai_level_004b08a0(void*, void*, void*, unsigned);
unsigned __fastcall recovered_effect_integer_00503690(void*, void*, unsigned);
void* __fastcall recovered_effect_construct_00503e40(void*, void*, unsigned);
void __fastcall recovered_effect_creator_00503a00(void*, void*, unsigned);
void __fastcall recovered_effect_integer_005036a0(void*, void*, unsigned, unsigned);
void __fastcall recovered_effect_apply_004d14f0(void*, void*, void*, unsigned, unsigned);
unsigned char __fastcall kotor_fastcall_004d8230(void*, void*, unsigned, void*);
void* __fastcall kotor_fastcall_004ae750(void*, void*, unsigned);
void __fastcall gui_load_array_add_00671c00(void*, void*, unsigned);
void* __fastcall kotor_fastcall_0051b720(void*, void*, void*);
// W2 t3 bodies (w2_party_native.cpp), bound in t4
void* __fastcall recovered_aoe_by_id_004ae7d0(void*, void*, unsigned);
void __fastcall recovered_aoe_jump_to_point_00596360(void*, void*, void*, void*);
// W1 tier4 provider
unsigned __fastcall recovered_object_remove_effect_004d06c0(void*, void*, unsigned, unsigned);
// W5 providers (lane-w5 receipts/W5-INDEX.json; ruling t2 review: W5 owns 004BF470 / 00516020)
unsigned __fastcall w5_area_in_sub_areas_004bf470(void*, void*, void*, void*);
unsigned __fastcall w5_creature_handle_sub_area_state_changes_00516020(void*, void*, void*, void*, unsigned, unsigned);
// 0886 wrappers (w2_0886_exports.inc)
unsigned __fastcall w2x_creature_stats_005afce0(void*, void*, void*, void*, void*);
void __fastcall w2x_object_state_004d1cf0(void*, void*, void*, void*);
void __fastcall w2x_known_spells_005aeb30(void*, void*, void*, void*);
void __fastcall w2x_template_finish_004f1c40(void*, void*);
// W2-owned original data cells
unsigned recovered_journal_sort_order_00833a90 = 0;        // .data 00833A90, initial 0
unsigned recovered_subarea_sticky_007a1b54 = 1;            // .data 007A1B54, initial 1
}
namespace {
using w2::ifo::Word;
inline void* ptr(Word a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline Word word(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
[[noreturn]] void trap(Word a, const char* w) { kotor_unreachable_trap(a, w); }
const char* text(Word a) { return static_cast<const char*>(ptr(a)); }
struct Mem final : kotor::server_mainloop::Memory {
    Word read(Word a) override { if (a == 0x7a39fcu) return word(kotor_menu_window_cells::manager_007a39fc); Word v; std::memcpy(&v, ptr(a), 4); return v; }
    void write(Word a, Word v) override { std::memcpy(ptr(a), &v, 4); }
    Word read16(Word a) override { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
    void write16(Word a, Word v) override { auto x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
    Word read8(Word a) override { std::uint8_t v; std::memcpy(&v, ptr(a), 1); return v; }
};
struct Native final : w2::ifo::Ports {
    Mem& m; explicit Native(Mem& v) : m(v) {}
    Word literal(Word a) override {
        switch (a) {
        case 0x0073d71cu: return word("");
        case 0x00745310u: return word("TEMP:");
        case 0x00745318u: return word("pifo");
        case 0x007442a8u: return word("Module");
        case 0x00745330u: return word("IFO ");
        case 0x00745320u: return word("Mod_PlayerList");
        case 0x00744c24u: return word("ObjectId");
        case 0x00746d10u: return word("AreaId");
        case 0x007472dcu: return word("DetectMode");
        case 0x007472d0u: return word("StealthMode");
        case 0x007472c0u: return word("CreatureSize");
        case 0x00747260u: return word("PM_IsDisguised");
        case 0x00747250u: return word("PM_Appearance");
        case 0x00745d80u: return word("XPosition");
        case 0x00745d74u: return word("YPosition");
        case 0x00745d68u: return word("ZPosition");
        case 0x00745d38u: return word("XOrientation");
        case 0x00745d48u: return word("YOrientation");
        case 0x00745d58u: return word("ZOrientation");
        case 0x00746f90u: return word("JNL_Entries");
        case 0x00746f80u: return word("JNL_SortOrder");
        case 0x00746f74u: return word("JNL_PlotID");
        case 0x00746f68u: return word("JNL_State");
        case 0x00746f5cu: return word("JNL_Date");
        case 0x00746f50u: return word("JNL_Time");
        default: trap(a, "W2 ifo literal not transcribed");
        }
    }
    void write8(Word a, Word v) override { const auto x = static_cast<std::uint8_t>(v); std::memcpy(ptr(a), &x, 1); }
    Word app_007a39fc() override { return word(kotor_menu_window_cells::manager_007a39fc); }
    Word resman_007a39e8() override { return word(menu_resources::BorrowCanonicalManager007a39e8()); }
    Word allocate_006fa7e6(Word n) override { return word(live_operator_new(n)); }
    void free_006fa390(Word a) override { live_free(ptr(a)); }
    void str_literal_005e5a90(Word s, Word t) override { frame_LogStringCtor_005e5a90(ptr(s), nullptr, text(t)); }
    void str_destroy_005e5c20(Word s) override { frame_LogStringDtor_005e5c20(ptr(s), nullptr); }
    void str_empty_005b3190(Word s) override { m.write(s, 0); m.write(s + 4, 0); }
    void str_assign_005e5c50(Word d, Word s) override { async_assign_string_005e5c50(ptr(d), nullptr, ptr(s)); }
    void directory_add_00408800(Word mgr, Word s) override { (void)static_cast<menu_resources::Manager*>(ptr(mgr))->directory_00408800(static_cast<kotor_reconstruction::StartupString*>(ptr(s))); }
    void directory_remove_004088d0(Word mgr, Word s) override { kotor_fastcall_004088d0(ptr(mgr), nullptr, ptr(s)); }
    void ref_construct_00406d80(Word r, Word t) override { alignas(4) unsigned char tmp[16]; unresolved_ref_assign_00406290(ptr(r), nullptr, tmp, text(t)); }
    Word resource_lookup_004074d0(Word mgr, Word r, Word type) override { return word(c2da_lookup_004074d0(ptr(mgr), nullptr, ptr(r), type)); }
    Word gff_construct_004105a0(Word s) override { return word(recovered_gff_construct_004105a0(ptr(s), nullptr)); }
    void resource_register_00407680(Word mgr, Word r, Word type, Word res) override { c2da_register_00407680(ptr(mgr), nullptr, ptr(r), type, ptr(res)); }
    void strncpy_006fab80(Word d, Word s, Word n) override { (void)kotor_runtime_reporter_dependencies::string_copy_n_006fab80(static_cast<char*>(ptr(d)), text(s), n); }
    Word demand_00409b10(Word r) override { return c2da_current_resource_demand_00409b10(ptr(r), nullptr); }
    Word release_00409b80(Word r) override { return c2da_release_demand_00409b80(ptr(r), nullptr); }
    Word manager_release_00409cf0(Word mgr, Word r) override { return static_cast<Word>(c2da_release_00409cf0(ptr(mgr), nullptr, ptr(r))); }
    void top_00411240(Word r, Word out) override { (void)static_cast<kotor_q05::CResGFF*>(ptr(r))->GetTopLevelStruct(static_cast<kotor_q05::StructHandle*>(ptr(out))); }
    Word list_004118c0(Word r, Word out, Word parent, Word label) override { return gff_list_004118c0(ptr(r), nullptr, ptr(out), ptr(parent), text(label)); }
    Word list_count_00411940(Word r, Word l) override { return gff_list_count_00411940(ptr(r), nullptr, ptr(l)); }
    Word element_00411990(Word r, Word out, Word l, Word i) override { return gff_list_element_00411990(ptr(r), nullptr, ptr(out), ptr(l), i); }
    Word dword_00411c20(Word r, Word st, Word l, Word status, Word f) override { return gui_load_gff_dword_00411c20(ptr(r), nullptr, ptr(st), text(l), static_cast<unsigned*>(ptr(status)), f); }
    Word byte_00411a60(Word r, Word st, Word l, Word status, Word f) override { return gui_load_gff_byte_00411a60(ptr(r), nullptr, ptr(st), text(l), static_cast<unsigned*>(ptr(status)), static_cast<unsigned char>(f)); }
    Word int_00411c90(Word r, Word st, Word l, Word status, Word f) override { return static_cast<Word>(gui_load_gff_int_00411c90(ptr(r), nullptr, ptr(st), text(l), static_cast<unsigned*>(ptr(status)), static_cast<int>(f))); }
    Word word_00411b40(Word r, Word st, Word l, Word status, Word f) override { return recovered_gff_word_00411b40(ptr(r), nullptr, ptr(st), text(l), static_cast<unsigned*>(ptr(status)), static_cast<unsigned short>(f)); }
    Word float_bits_00411d00(Word r, Word st, Word l, Word status, Word fb) override {
        float f; std::memcpy(&f, &fb, 4);
        const float v = gui_load_gff_float_00411d00(ptr(r), nullptr, ptr(st), text(l), static_cast<unsigned*>(ptr(status)), f);
        Word w; std::memcpy(&w, &v, 4); return w;
    }
    Word string_00411ec0(Word r, Word res, Word st, Word l, Word status, Word f) override { return word(gui_load_gff_string_00411ec0(ptr(r), nullptr, ptr(res), ptr(st), text(l), static_cast<unsigned*>(ptr(status)), ptr(f))); }
    Word creature_construct_004f7a10(Word s, Word id, Word f) override { return word(recovered_server_creature_construct_004f7a10(ptr(s), nullptr, id, f)); }
    void set_game_object_00561c40(Word pl, Word c) override { recovered_set_game_object_00561c40(ptr(pl), nullptr, ptr(c)); }
    Word stats_read_005afce0(Word st, Word r, Word s, Word o) override { return w2x_creature_stats_005afce0(ptr(st), nullptr, ptr(r), ptr(s), ptr(o)); }
    void items_read_004ffda0(Word c, Word r, Word s, Word a, Word b, Word d) override { recovered_creature_inventory_004ffda0(ptr(c), nullptr, ptr(r), ptr(s), a, b, d); }
    void object_state_004d1cf0(Word o, Word r, Word s) override { w2x_object_state_004d1cf0(ptr(o), nullptr, ptr(r), ptr(s)); }
    void spells_read_005aeb30(Word st, Word r, Word s) override { w2x_known_spells_005aeb30(ptr(st), nullptr, ptr(r), ptr(s)); }
    void scripts_read_004ebf20(Word c, Word r, Word s) override { recovered_creature_scripts_004ebf20(ptr(c), nullptr, ptr(r), ptr(s)); }
    void set_orientation_004cd170(Word o, Word v) override { kotor::server_mainloop::object_orientation_004cd170(m, o, v); }
    void post_process_004f1c40(Word c) override { w2x_template_finish_004f1c40(ptr(c), nullptr); }
    void vcall1(Word o, Word slot, Word a) override { using F = void*(__thiscall*)(void*, unsigned); (void)reinterpret_cast<F>(m.read(m.read(o) + slot))(ptr(o), a); }
    Word vcall0(Word o, Word slot) override { using F = void*(__thiscall*)(void*); return word(reinterpret_cast<F>(m.read(m.read(o) + slot))(ptr(o))); }
    void set_ai_level_004b08a0(Word ai, Word c, Word level) override { (void)recovered_ai_level_004b08a0(ptr(ai), nullptr, ptr(c), level); }
    void journal_set_state_005c5a40(Word j, Word plot, Word state, Word flag) override { kotor_fastcall_005c5a40(ptr(j), nullptr, ptr(plot), state, flag); }
    void journal_set_date_005c63f0(Word j, Word plot, Word date, Word flag) override { kotor_fastcall_005c63f0(ptr(j), nullptr, ptr(plot), date, flag); }
    void journal_set_time_005c6650(Word j, Word plot, Word time, Word flag) override { kotor_fastcall_005c6650(ptr(j), nullptr, ptr(plot), time, flag); }
    Word journal_sort_order_cell_00833a90() override { return word(&recovered_journal_sort_order_00833a90); }
    Word array_list_construct_005c5a30(Word s) override { return word(static_cast<kotor_input_hover::DeviceBuffer*>(ptr(s))->Construct005c5a30()); }
    Word effect_integer_00503690(Word e, Word i) override { return recovered_effect_integer_00503690(ptr(e), nullptr, i); }
    void remove_effect_by_id_004d06c0(Word o, Word lo, Word hi) override { (void)recovered_object_remove_effect_004d06c0(ptr(o), nullptr, lo, hi); }
    Word effect_construct_00503e40(Word s, Word f) override { return word(recovered_effect_construct_00503e40(ptr(s), nullptr, f)); }
    void effect_creator_00503a00(Word e, Word id) override { recovered_effect_creator_00503a00(ptr(e), nullptr, id); }
    void effect_set_integer_005036a0(Word e, Word i, Word v) override { recovered_effect_integer_005036a0(ptr(e), nullptr, i, v); }
    void apply_effect_004d14f0(Word o, Word e, Word a, Word b) override { recovered_effect_apply_004d14f0(ptr(o), nullptr, ptr(e), a, b); }
    Word stop_dialog_004cb4f0(Word o) override { return static_cast<Word>(kotor_fastcall_004cb4f0(ptr(o), nullptr)); }   // real CSWSObject::StopDialog (stop_dialog_004cb4f0.obj)
    Word object_lookup_004d8230(Word a, Word id, Word out) override { return kotor_fastcall_004d8230(ptr(a), nullptr, id, ptr(out)); }
    Word game_object_by_id_004ae750(Word s, Word id) override { return word(kotor_fastcall_004ae750(ptr(s), nullptr, id)); }
    Word aoe_by_id_004ae7d0(Word s, Word id) override { return word(recovered_aoe_by_id_004ae7d0(ptr(s), nullptr, id)); }   // W2 t3 (t4 rebind)
    void aoe_jump_00596360(Word a, Word area, Word pos) override { recovered_aoe_jump_to_point_00596360(ptr(a), nullptr, ptr(area), ptr(pos)); }   // W2 t3 (t4 rebind)
    Word in_subareas_004bf470(Word a, Word pos, Word l) override { return w5_area_in_sub_areas_004bf470(ptr(a), nullptr, ptr(pos), ptr(l)); }
    void list_add_00671c00(Word l, Word v) override { gui_load_array_add_00671c00(ptr(l), nullptr, v); }
    void list_allocate_005e03c0(Word l, Word n) override { menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(ptr(l)), nullptr, n); }
    Word subarea_changes_00516020(Word c, Word l, Word a, Word b, Word f) override { return w5_creature_handle_sub_area_state_changes_00516020(ptr(c), nullptr, ptr(l), ptr(a), b, f); }  // (now, crossed=0, trigger_check=0, fire)
    void list_assign_0051b720(Word d, Word s) override { (void)kotor_fastcall_0051b720(ptr(d), nullptr, ptr(s)); }
    Word sticky_subarea_cell_007a1b54() override { return word(&recovered_subarea_sticky_007a1b54); }
};
}
namespace i = w2::ifo;
extern "C" {
// 00561E30 CSWSPlayer::LoadCharacterFromIFO(index, flag, post) RET 0Ch
unsigned __fastcall recovered_load_character_00561e30(void* player, void*, unsigned index, unsigned flag, unsigned post) {
    Mem m; Native p(m); alignas(8) unsigned char s[0xd0] = {};
    std::memcpy(s + 0x7c, &index, 4); std::memcpy(s + 0x80, &flag, 4); std::memcpy(s + 0x84, &post, 4);
    return i::load_character_00561e30(m, p, word(player), word(s));
}
void __fastcall recovered_set_in_party_004fdb20(void* c, void*, unsigned a, unsigned b) { Mem m; Native p(m); i::set_in_party_004fdb20(m, p, word(c), a, b); }
void* __fastcall recovered_follow_info_construct_004f79e0(void* self, void*) { Mem m; return ptr(i::follow_info_construct_004f79e0(m, word(self))); }
void __fastcall recovered_follow_info_clear_004f0020(void* self, void*) { Mem m; i::follow_info_clear_004f0020(m, word(self)); }
void __fastcall recovered_load_journal_004f17d0(void* c, void*, void* res, void* st) { Mem m; Native p(m); alignas(8) unsigned char s[0x40] = {}; i::load_journal_004f17d0(m, p, word(c), word(res), word(st), word(s)); }
void* __fastcall recovered_party_journal_005633c0(void* party, void*) { Mem m; Native p(m); return ptr(i::party_journal_005633c0(m, p, word(party))); }
void __fastcall recovered_set_activity_004f00a0(void* c, void*, unsigned mask, unsigned on, unsigned extra) { Mem m; Native p(m); i::set_activity_004f00a0(m, p, word(c), mask, on, extra); }
void __fastcall recovered_set_detect_mode_0050ee30(void* c, void*, unsigned mode) { Mem m; Native p(m); i::set_detect_mode_0050ee30(m, p, word(c), mode); }
void __fastcall recovered_set_stealth_mode_0050ee50(void* c, void*, unsigned mode, unsigned value) { Mem m; Native p(m); i::set_stealth_mode_0050ee50(m, p, word(c), mode, value); }
void __fastcall recovered_movement_rate_004f6d70(void* c, void*) { Mem m; Native p(m); i::movement_rate_004f6d70(m, p, word(c)); }
unsigned __fastcall recovered_update_subareas_0051b940(void* c, void*, void* pos, unsigned area_id, unsigned flag) {
    Mem m; Native p(m); alignas(8) unsigned char s[0x20] = {}; return i::update_subareas_0051b940(m, p, word(c), word(pos), area_id, flag, word(s));
}
}
