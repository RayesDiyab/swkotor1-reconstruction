// W2 tier 6 (fork B) native binding + exports: SaveStats 005B1B90, SaveClassInfo 005AEC90, GetSkillRankChange 005CB0F0,
// CCombatInformation::SaveData 00550F30. Providers: 0873 GFF writers (@recovered_gff_*), fork A GFF primitives
// (@recovered_gff_00412870 FLOAT, @recovered_gff_004125b0 AddStructToStruct), W2 t4 charsheet exports, W1, 1122 objects.
#if !defined(_M_IX86)
#error W2 native bindings require the Windows x86 route
#endif
#include "301b028a69-w2_save_stats.hpp"
#include <cstdint>
#include <cstring>
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }
namespace kotor_reconstruction { struct StartupString; void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }
namespace kotor::server_mainloop { Word creature_force_class_005be4a0(Word); }
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void*, void*); }
extern "C" {
extern void* kotor_cell_rules_007a3a28;
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
// 0873 GFF writer exports (CResGFF*, edx, ...)
unsigned __fastcall recovered_gff_00412620(void*, void*, void*, unsigned, const char*);
unsigned __fastcall recovered_gff_00412670(void*, void*, void*, unsigned, const char*);
unsigned __fastcall recovered_gff_004126c0(void*, void*, void*, unsigned, const char*);
unsigned __fastcall recovered_gff_00412710(void*, void*, void*, unsigned, const char*);
unsigned __fastcall recovered_gff_00412760(void*, void*, void*, unsigned, const char*);
unsigned __fastcall recovered_gff_004127b0(void*, void*, void*, unsigned, const char*);
unsigned __fastcall recovered_gff_004128c0(void*, void*, void*, void*, const char*);
unsigned __fastcall recovered_gff_00412970(void*, void*, void*, void*, const char*);
unsigned __fastcall recovered_gff_00412a10(void*, void*, void*, void*, const char*);
unsigned __fastcall recovered_gff_00412450(void*, void*, void*, void*, const char*);
unsigned __fastcall recovered_gff_004124e0(void*, void*, void*, void*, unsigned);
// fork A (W2 t6) GFF primitives: WriteFieldFLOAT(CResStruct*, float, char*) RET 0Ch; AddStructToStruct(out, parent, label, type) RET 10h
unsigned __fastcall recovered_gff_00412870(void*, void*, void*, unsigned, const char*);
unsigned __fastcall recovered_gff_004125b0(void*, void*, void*, void*, const char*, unsigned);
// strings
char* __fastcall async_string_cstr_005e5670(void*, void*);
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);
unsigned __fastcall exo_locstring_get_005ea130(void*, void*, unsigned, void*, unsigned);
// creature / stats
unsigned __fastcall w2x_creature_gold_004edd60(void*, void*);                          // W2 0886 wrapper
unsigned __fastcall recovered_stats_reflex_save_005ab8f0(void*, void*, unsigned);        // W2 t6 export (w2_g07_native.cpp)
unsigned __fastcall recovered_stats_base_will_save_005aa2f0(void*, void*);               // W2 t6 export (w2_g07_native.cpp)
unsigned __fastcall recovered_stats_base_fort_save_005aa1b0(void*, void*);               // W2 t6 export (w2_g07_native.cpp)
unsigned __fastcall recovered_creature_total_effect_bonus_004f3fe0(void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned);
unsigned __fastcall recovered_creature_armor_class_004ed1d0(void*, void*);               // W2 t4
unsigned __fastcall w1x_creature_max_fp_004fd490(void*, void*);                         // W1 0886 wrapper
unsigned char __fastcall recovered_stats_level_005a5fd0(void*, void*, unsigned);
void* __fastcall recovered_item_lookup_004ae760(void*, void*, unsigned);
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void*, void*, int);
}
namespace {
using w2::save_stats::Word;
inline void* ptr(Word a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline Word word(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
struct Mem final : kotor::server_mainloop::Memory {
    Word read(Word a) override { Word v; std::memcpy(&v, ptr(a), 4); return v; }
    void write(Word a, Word v) override { std::memcpy(ptr(a), &v, 4); }
    Word read16(Word a) override { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
    void write16(Word a, Word v) override { auto x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
    Word read8(Word a) override { std::uint8_t v; std::memcpy(&v, ptr(a), 1); return v; }
};
// CResGFF labels are at most 16 characters; the writer copies from a 16-byte window, so hand it a zero-padded copy.
struct Label { char t[20]; explicit Label(Word a) { std::memset(t, 0, sizeof t); const char* s = static_cast<const char*>(ptr(a)); for (int i = 0; i < 16 && s[i] != 0; ++i) t[i] = s[i]; } };
struct Native final : w2::save_stats::Ports {
    [[noreturn]] void trap(Word a, const char* w) override { kotor_unreachable_trap(a, w); }
    Word text(const char* l) override { return word(l); }
    Word app_007a39fc() override { return word(kotor_menu_window_cells::manager_007a39fc); }
    Word rules_007a3a28() override { return word(kotor_cell_rules_007a3a28); }
    void write_byte_00412620(Word g, Word st, Word v, Word l) override { Label x(l); (void)recovered_gff_00412620(ptr(g), nullptr, ptr(st), v, x.t); }
    void write_char_00412670(Word g, Word st, Word v, Word l) override { Label x(l); (void)recovered_gff_00412670(ptr(g), nullptr, ptr(st), v, x.t); }
    void write_word_004126c0(Word g, Word st, Word v, Word l) override { Label x(l); (void)recovered_gff_004126c0(ptr(g), nullptr, ptr(st), v, x.t); }
    void write_short_00412710(Word g, Word st, Word v, Word l) override { Label x(l); (void)recovered_gff_00412710(ptr(g), nullptr, ptr(st), v, x.t); }
    void write_dword_00412760(Word g, Word st, Word v, Word l) override { Label x(l); (void)recovered_gff_00412760(ptr(g), nullptr, ptr(st), v, x.t); }
    void write_int_004127b0(Word g, Word st, Word v, Word l) override { Label x(l); (void)recovered_gff_004127b0(ptr(g), nullptr, ptr(st), v, x.t); }
    void write_float_00412870(Word g, Word st, Word v, Word l) override { Label x(l); (void)recovered_gff_00412870(ptr(g), nullptr, ptr(st), v, x.t); }
    void write_resref_004128c0(Word g, Word st, Word r, Word l) override { Label x(l); (void)recovered_gff_004128c0(ptr(g), nullptr, ptr(st), ptr(r), x.t); }
    void write_string_00412970(Word g, Word st, Word s, Word l) override { Label x(l); (void)recovered_gff_00412970(ptr(g), nullptr, ptr(st), ptr(s), x.t); }
    void write_locstring_00412a10(Word g, Word st, Word s, Word l) override { Label x(l); (void)recovered_gff_00412a10(ptr(g), nullptr, ptr(st), ptr(s), x.t); }
    void add_list_00412450(Word g, Word out, Word parent, Word l) override { Label x(l); (void)recovered_gff_00412450(ptr(g), nullptr, ptr(out), ptr(parent), x.t); }
    void add_list_element_004124e0(Word g, Word out, Word list, Word type) override { (void)recovered_gff_004124e0(ptr(g), nullptr, ptr(out), ptr(list), type); }
    void add_struct_to_struct_004125b0(Word g, Word out, Word parent, Word l, Word type) override { Label x(l); (void)recovered_gff_004125b0(ptr(g), nullptr, ptr(out), ptr(parent), x.t, type); }
    void str_default_005b3190(Word s) override { (void)kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(ptr(s)), nullptr); }
    void str_format_s_d_005e5680(Word s, Word prefix, Word n) override {
        kotor_reconstruction::StartupFormat005e5680(static_cast<kotor_reconstruction::StartupString*>(ptr(s)), "%s%d", static_cast<const char*>(ptr(prefix)), static_cast<int>(n));
    }
    Word str_cstr_005e5670(Word s) override { return word(async_string_cstr_005e5670(ptr(s), nullptr)); }
    void str_destroy_005e5c20(Word s) override { frame_LogStringDtor_005e5c20(ptr(s), nullptr); }
    void locstring_get_005ea130(Word loc, Word lang, Word out, Word gender) override { (void)exo_locstring_get_005ea130(ptr(loc), nullptr, lang, ptr(out), gender); }
    Word gold_004edd60(Word c) override { return w2x_creature_gold_004edd60(ptr(c), nullptr); }
    Word reflex_save_005ab8f0(Word s, Word f) override { return recovered_stats_reflex_save_005ab8f0(ptr(s), nullptr, f); }
    Word base_will_save_005aa2f0(Word s) override { return recovered_stats_base_will_save_005aa2f0(ptr(s), nullptr); }
    Word base_fort_save_005aa1b0(Word s) override { return recovered_stats_base_fort_save_005aa1b0(ptr(s), nullptr); }
    Word effect_bonus_004f3fe0(Word c, Word a1, Word a2, Word a3, Word a4, Word a5, Word a6, Word a7, Word a8, Word a9, Word a10) override {
        return recovered_creature_total_effect_bonus_004f3fe0(ptr(c), nullptr, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
    }
    Word armor_class_004ed1d0(Word c) override { return recovered_creature_armor_class_004ed1d0(ptr(c), nullptr); }
    Word max_force_points_004fd490(Word c) override { return w1x_creature_max_fp_004fd490(ptr(c), nullptr); }
    Word vcall0(Word o, Word slot) override { Mem m; using F = Word(__thiscall*)(void*); return reinterpret_cast<F>(m.read(m.read(o) + slot))(ptr(o)); }
    Word vcall1(Word o, Word slot, Word a) override { Mem m; using F = Word(__thiscall*)(void*, Word); return reinterpret_cast<F>(m.read(m.read(o) + slot))(ptr(o), a); }
    Word stats_level_005a5fd0(Word s, Word f) override { return recovered_stats_level_005a5fd0(ptr(s), nullptr, f); }
    Word is_jedi_005be4a0(Word c) override { return kotor::server_mainloop::creature_force_class_005be4a0(c); }
    Word party_table_004aee70(Word s) override { return word(lane_config_required::GetPartyTable_004aee70(ptr(s), nullptr)); }
    Word item_by_id_004ae760(Word s, Word id) override { return word(recovered_item_lookup_004ae760(ptr(s), nullptr, id)); }
    Word base_item_005b31a0(Word a, Word t) override { return word(kotor_m15b_base_item_array_get_base_item_005b31a0(ptr(a), nullptr, static_cast<int>(t))); }
};
}
namespace ss = w2::save_stats;
extern "C" {
void __fastcall recovered_save_stats_005b1b90(void* stats, void*, void* gff, void* parent) {
    Mem m; Native p; alignas(8) unsigned char s[0x60] = {}; alignas(8) unsigned char s2[0xf0] = {};
    ss::save_stats_005b1b90(m, p, word(stats), word(gff), word(parent), word(s), word(s2));
}
void __fastcall recovered_save_class_info_005aec90(void* stats, void*, void* gff, void* parent) {
    Mem m; Native p; alignas(8) unsigned char s[0xf0] = {}; ss::save_class_info_005aec90(m, p, word(stats), word(gff), word(parent), word(s));
}
unsigned __fastcall recovered_skill_rank_change_005cb0f0(void* ls, void*, unsigned skill) { Mem m; Native p; return ss::skill_rank_change_005cb0f0(m, p, word(ls), skill); }
unsigned __fastcall recovered_combat_info_save_00550f30(void* ci, void*, void* gff, void* parent) {
    Mem m; Native p; alignas(8) unsigned char s[0x40] = {}; return ss::combat_info_save_00550f30(m, p, word(ci), word(gff), word(parent), word(s));
}
}
