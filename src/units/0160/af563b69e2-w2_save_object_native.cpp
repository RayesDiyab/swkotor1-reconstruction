// W2 tier 6 (fork E1) native binding + exports for w2_save_object (object state / effects / var tables).
// Providers: 0873 GFF writer exports; fork A GFF primitives (00412870 / 00412800 / 004125B0); W2 w2_entry var exports;
// 0078 CExoString copy / dtor; W6 vm_situation CScriptLocation ctor (1125); fork E2 SaveActionQueue.
#if !defined(_M_IX86)
#error W2 native bindings require the Windows x86 route
#endif
#include "cb64dd2119-w2_save_object.hpp"
#include <cstdint>
#include <cstring>
namespace kotor::vm_situation { kotor::server_mainloop::Word location_construct_004ca7a0(kotor::server_mainloop::Memory&, kotor::server_mainloop::Word self); }
extern "C" {
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
// 0873 GFF writer (thiscall on CResGFF): (struct*, value, label) RET 0Ch
unsigned __fastcall recovered_gff_00412620(void*, void*, void*, unsigned, const char*);   // WriteFieldBYTE
unsigned __fastcall recovered_gff_004126c0(void*, void*, void*, unsigned, const char*);   // WriteFieldWORD
unsigned __fastcall recovered_gff_00412760(void*, void*, void*, unsigned, const char*);   // WriteFieldDWORD
unsigned __fastcall recovered_gff_004127b0(void*, void*, void*, unsigned, const char*);   // WriteFieldINT
unsigned __fastcall recovered_gff_00412970(void*, void*, void*, void*, const char*);      // WriteFieldCExoString
unsigned __fastcall recovered_gff_00412450(void*, void*, void*, void*, const char*);      // AddList(out, parent, label)
unsigned __fastcall recovered_gff_004124e0(void*, void*, void*, void*, unsigned);         // AddListElement(out, list, type)
// fork A (W2 t6) GFF primitives
unsigned __fastcall recovered_gff_00412870(void*, void*, void*, unsigned, const char*);   // WriteFieldFLOAT (value as float bits)
unsigned __fastcall recovered_gff_00412800(void*, void*, void*, unsigned, unsigned, const char*);   // WriteFieldDWORD64 (lo, hi)
unsigned __fastcall recovered_gff_004125b0(void*, void*, void*, void*, const char*, unsigned);      // AddStructToStruct(out, parent, label, type)
// W2 (w2_entry_native.cpp) CSWSScriptVarTable leaves
void* __fastcall recovered_var_match_0059a390(void*, void*, void*, unsigned, unsigned);
void* __fastcall recovered_var_string_0059a590(void*, void*, void*, void*);
// strings (0078 window-native-strings / 0459 log strings)
void* __fastcall WindowCopy005e5b00(void*, void*, void*);
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);
// fork E2 (W2 t6)
unsigned __fastcall recovered_save_action_queue_004cc7e0(void*, void*, void*, void*);
}
namespace {
using w2::save_object::Word;
inline void* ptr(Word a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline Word word(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
struct Mem final : kotor::server_mainloop::Memory {
    Word read(Word a) override { Word v; std::memcpy(&v, ptr(a), 4); return v; }
    void write(Word a, Word v) override { std::memcpy(ptr(a), &v, 4); }
    Word read16(Word a) override { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
    void write16(Word a, Word v) override { auto x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
    Word read8(Word a) override { std::uint8_t v; std::memcpy(&v, ptr(a), 1); return v; }
};
const char* L(Word a) {
    const char* t = w2::save_object::save_object_label(a);
    if (t == nullptr) kotor_unreachable_trap(a, "W2 save_object label not transcribed");
    return t;
}
struct Native final : w2::save_object::Ports {
    Mem& m; explicit Native(Mem& v) : m(v) {}
    [[noreturn]] void trap(Word a, const char* w) override { kotor_unreachable_trap(a, w); }
    Word gff_byte_00412620(Word g, Word s, Word v, Word l) override { return recovered_gff_00412620(ptr(g), nullptr, ptr(s), v, L(l)); }
    Word gff_word_004126c0(Word g, Word s, Word v, Word l) override { return recovered_gff_004126c0(ptr(g), nullptr, ptr(s), v, L(l)); }
    Word gff_dword_00412760(Word g, Word s, Word v, Word l) override { return recovered_gff_00412760(ptr(g), nullptr, ptr(s), v, L(l)); }
    Word gff_int_004127b0(Word g, Word s, Word v, Word l) override { return recovered_gff_004127b0(ptr(g), nullptr, ptr(s), v, L(l)); }
    Word gff_float_00412870(Word g, Word s, Word b, Word l) override { return recovered_gff_00412870(ptr(g), nullptr, ptr(s), b, L(l)); }
    Word gff_dword64_00412800(Word g, Word s, Word lo, Word hi, Word l) override { return recovered_gff_00412800(ptr(g), nullptr, ptr(s), lo, hi, L(l)); }
    Word gff_string_00412970(Word g, Word s, Word str, Word l) override { return recovered_gff_00412970(ptr(g), nullptr, ptr(s), ptr(str), L(l)); }
    Word gff_add_list_00412450(Word g, Word out, Word parent, Word l) override { return recovered_gff_00412450(ptr(g), nullptr, ptr(out), ptr(parent), L(l)); }
    Word gff_add_list_element_004124e0(Word g, Word out, Word list, Word t) override { return recovered_gff_004124e0(ptr(g), nullptr, ptr(out), ptr(list), t); }
    Word gff_add_struct_to_struct_004125b0(Word g, Word out, Word parent, Word l, Word t) override { return recovered_gff_004125b0(ptr(g), nullptr, ptr(out), ptr(parent), L(l), t); }
    Word var_match_0059a390(Word t, Word n, Word ty, Word c) override { return word(recovered_var_match_0059a390(ptr(t), nullptr, ptr(n), ty, c)); }
    Word var_string_0059a590(Word t, Word out, Word n) override { return word(recovered_var_string_0059a590(ptr(t), nullptr, ptr(out), ptr(n))); }
    void str_copy_005e5b00(Word out, Word src) override { (void)WindowCopy005e5b00(ptr(out), nullptr, ptr(src)); }
    void str_destroy_005e5c20(Word s) override { frame_LogStringDtor_005e5c20(ptr(s), nullptr); }
    void location_construct_004ca7a0(Word l) override { (void)kotor::vm_situation::location_construct_004ca7a0(m, l); }
    Word save_action_queue_004cc7e0(Word o, Word g, Word s) override { return recovered_save_action_queue_004cc7e0(ptr(o), nullptr, ptr(g), ptr(s)); }
};
}
namespace so = w2::save_object;
extern "C" {
unsigned __fastcall recovered_save_object_state_004cec50(void* o, void*, void* gff, void* st) {
    Mem m; Native p(m); alignas(8) unsigned char s[0x80] = {}; return so::save_object_state_004cec50(m, p, word(o), word(gff), word(st), word(s));
}
unsigned __fastcall recovered_save_effect_list_004cc9d0(void* o, void*, void* gff, void* st) {
    Mem m; Native p(m); alignas(8) unsigned char s[0x60] = {}; return so::save_effect_list_004cc9d0(m, p, word(o), word(gff), word(st), word(s));
}
unsigned __fastcall recovered_save_game_effect_00503790(void* e, void*, void* gff, void* st) {
    Mem m; Native p(m); alignas(8) unsigned char s[0x28] = {}; return so::save_game_effect_00503790(m, p, word(e), word(gff), word(st), word(s));
}
void* __fastcall recovered_effect_copy_string_00503700(void* e, void*, void* out, unsigned index) {
    Mem m; Native p(m); return ptr(so::effect_copy_string_00503700(m, p, word(e), word(out), index));
}
unsigned __fastcall recovered_save_var_table_0059adb0(void* t, void*, void* gff, void* st) {
    Mem m; Native p(m); alignas(8) unsigned char s[0x78] = {}; return so::save_var_table_0059adb0(m, p, word(t), word(gff), word(st), word(s));
}
void* __fastcall recovered_var_get_location_0059a650(void* t, void*, void* out, void* name) {
    Mem m; Native p(m); alignas(8) unsigned char s[0x18] = {}; return ptr(so::var_get_location_0059a650(m, p, word(t), word(out), word(name), word(s)));
}
unsigned __fastcall recovered_save_location_004ca8e0(void* l, void*, void* gff, void* st) {
    Mem m; Native p(m); return so::save_location_004ca8e0(m, p, word(l), word(gff), word(st));
}
unsigned __fastcall recovered_sw_var_table_save_0059b250(void* t, void*, void* gff, void* st) {
    Mem m; Native p(m); alignas(8) unsigned char s[0x40] = {}; return so::sw_var_table_save_0059b250(m, p, word(t), word(gff), word(st), word(s));
}
}
