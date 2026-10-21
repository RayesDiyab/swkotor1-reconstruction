// W2 t6 native binding + export: CSWSCreature::SaveCreature 00500610 (@16, RET 8).
#if !defined(_M_IX86)
#error W2 native bindings require the Windows x86 route
#endif
#include "55014e1687-w2_save_creature.hpp"
#include <algorithm>
#include <cstdint>
#include <cstring>
extern "C" {
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
void __fastcall recovered_set_in_party_004fdb20(void*, void*, unsigned, unsigned);                    // W2 t2
void __fastcall recovered_save_stats_005b1b90(void*, void*, void*, void*);                            // W2 t6 (fork B)
void __fastcall recovered_gff_00412620(void*, void*, void*, unsigned, const char*);                    // 0873 WriteFieldBYTE
void __fastcall recovered_gff_004126c0(void*, void*, void*, unsigned, const char*);                    // 0873 WriteFieldWORD
void __fastcall recovered_gff_00412760(void*, void*, void*, unsigned, const char*);                    // 0873 WriteFieldDWORD
void __fastcall recovered_gff_004127b0(void*, void*, void*, unsigned, const char*);                    // 0873 WriteFieldINT
void __fastcall recovered_gff_00412870(void*, void*, void*, float, const char*);                       // W2 t6 (fork A) WriteFieldFLOAT
void __fastcall recovered_gff_004128c0(void*, void*, void*, void*, const char*);                       // 0873 WriteFieldCResRef
void* __fastcall async_ref_construct_string_00406d60(void*, void*, void*);                            // CResRef(CExoString const&)
void __fastcall recovered_gff_00412450(void*, void*, void*, void*, const char*);                       // 0873 AddList
void __fastcall recovered_gff_004124e0(void*, void*, void*, void*, unsigned);                          // 0873 AddListElement
void __fastcall recovered_gff_004125b0(void*, void*, void*, void*, const char*, unsigned);             // W2 t6 (fork A) AddStructToStruct
void* __fastcall recovered_equipment_item_005a4c20(void*, void*, unsigned);
void* __fastcall recovered_creature_inventory_004ef770(void*, void*, unsigned);
void* __fastcall w2x_inventory_item_005560b0(void*, void*, unsigned);
void __fastcall recovered_save_item_0055ccd0(void*, void*, void*, void*);                              // W2 t6 (fork C)
void __fastcall recovered_save_combat_round_004d3ec0(void*, void*, void*, void*);                      // W2 t6 (fork D)
void __fastcall recovered_save_listen_data_004cca50(void*, void*, void*, void*);                       // W2 t6 (fork D)
void __fastcall recovered_follow_info_save_004eaf70(void*, void*, void*, void*);                       // W2 t6 (fork D)
void __fastcall recovered_save_object_state_004cec50(void*, void*, void*, void*);                      // W2 t6 (fork E1)
}
namespace {
using w2::save::Word;
inline void* ptr(Word a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline Word word(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
struct Mem final : kotor::server_mainloop::Memory {
    Word read(Word a) override { Word v; std::memcpy(&v, ptr(a), 4); return v; }
    void write(Word a, Word v) override { std::memcpy(ptr(a), &v, 4); }
    Word read16(Word a) override { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
    void write16(Word a, Word v) override { auto x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
    Word read8(Word a) override { std::uint8_t v; std::memcpy(&v, ptr(a), 1); return v; }
};
float f32(Word b) { float v; std::memcpy(&v, &b, 4); return v; }
// 0873 writers copy the label into the GFF label table (16 bytes); pass a zero-padded copy like the chargen binding
struct Label { char text[17]; explicit Label(const char* l) { std::memset(text, 0, sizeof text); std::memcpy(text, l, std::min<std::size_t>(std::strlen(l), 16)); } };
struct Native final : w2::save::CreaturePorts {
    [[noreturn]] void trap(Word a, const char* w) override { kotor_unreachable_trap(a, w); }
    void set_in_party_004fdb20(Word c, Word a, Word b) override { recovered_set_in_party_004fdb20(ptr(c), nullptr, a, b); }
    void save_stats_005b1b90(Word s, Word g, Word st) override { recovered_save_stats_005b1b90(ptr(s), nullptr, ptr(g), ptr(st)); }
    void write_byte_00412620(Word g, Word st, Word v, const char* l) override { Label t(l); recovered_gff_00412620(ptr(g), nullptr, ptr(st), v & 0xffu, t.text); }
    void write_word_004126c0(Word g, Word st, Word v, const char* l) override { Label t(l); recovered_gff_004126c0(ptr(g), nullptr, ptr(st), v & 0xffffu, t.text); }
    void write_dword_00412760(Word g, Word st, Word v, const char* l) override { Label t(l); recovered_gff_00412760(ptr(g), nullptr, ptr(st), v, t.text); }
    void write_int_004127b0(Word g, Word st, Word v, const char* l) override { Label t(l); recovered_gff_004127b0(ptr(g), nullptr, ptr(st), v, t.text); }
    void write_float_00412870(Word g, Word st, Word b, const char* l) override { Label t(l); recovered_gff_00412870(ptr(g), nullptr, ptr(st), f32(b), t.text); }
    void write_resref_004128c0(Word g, Word st, Word r, const char* l) override { Label t(l); recovered_gff_004128c0(ptr(g), nullptr, ptr(st), ptr(r), t.text); }
    void ref_from_string_00406d60(Word r, Word s) override { (void)async_ref_construct_string_00406d60(ptr(r), nullptr, ptr(s)); }
    void add_list_00412450(Word g, Word out, Word parent, const char* l) override { Label t(l); recovered_gff_00412450(ptr(g), nullptr, ptr(out), ptr(parent), t.text); }
    void add_list_element_004124e0(Word g, Word out, Word list, Word ty) override { recovered_gff_004124e0(ptr(g), nullptr, ptr(out), ptr(list), ty); }
    void add_struct_to_struct_004125b0(Word g, Word out, Word parent, const char* l, Word ty) override { Label t(l); recovered_gff_004125b0(ptr(g), nullptr, ptr(out), ptr(parent), t.text, ty); }
    Word item_in_slot_005a4c20(Word inv, Word mask) override { return word(recovered_equipment_item_005a4c20(ptr(inv), nullptr, mask)); }
    Word item_repository_004ef770(Word c, Word f) override { return word(recovered_creature_inventory_004ef770(ptr(c), nullptr, f)); }
    Word repository_item_005560b0(Word r, Word i) override { return word(w2x_inventory_item_005560b0(ptr(r), nullptr, i)); }
    void save_item_0055ccd0(Word it, Word g, Word st) override { recovered_save_item_0055ccd0(ptr(it), nullptr, ptr(g), ptr(st)); }
    void save_combat_round_004d3ec0(Word r, Word g, Word st) override { recovered_save_combat_round_004d3ec0(ptr(r), nullptr, ptr(g), ptr(st)); }
    void save_listen_data_004cca50(Word o, Word g, Word st) override { recovered_save_listen_data_004cca50(ptr(o), nullptr, ptr(g), ptr(st)); }
    void follow_info_save_004eaf70(Word fi, Word g, Word st) override { recovered_follow_info_save_004eaf70(ptr(fi), nullptr, ptr(g), ptr(st)); }
    void save_object_state_004cec50(Word o, Word g, Word st) override { recovered_save_object_state_004cec50(ptr(o), nullptr, ptr(g), ptr(st)); }
};
}
extern "C" unsigned __fastcall recovered_save_creature_00500610(void* c, void*, void* gff, void* st) {
    Mem m; Native p; alignas(8) unsigned char s[0x60] = {};
    return w2::save::save_creature_00500610(m, p, word(c), word(gff), word(st), word(s));
}
