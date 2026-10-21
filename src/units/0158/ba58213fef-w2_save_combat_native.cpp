// W2 tier 6 (fork D) native binding: 004D3EC0 / 004D2210 / 004D1DD0 / 004CCA50 / 004EAF70.
// Providers: 0873 GFF writer exports (in 1122/1125), W2 t6 fork A GFF primitives (FLOAT 00412870, Vector 00412D30),
// 0168 linked-list accessors. Exports: thiscall as __fastcall(self, edx, gff, struct) -> @16 (original RET 8).
#if !defined(_M_IX86)
#error W2 native bindings require the Windows x86 route
#endif
#include "cdab552184-w2_save_combat.hpp"
#include <cstdint>
#include <cstring>
extern "C" {
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
#define W2_GFF_SCALAR(a) unsigned __fastcall recovered_gff_##a(void*, void*, void*, unsigned, const char*);
W2_GFF_SCALAR(00412620) W2_GFF_SCALAR(00412670) W2_GFF_SCALAR(004126c0) W2_GFF_SCALAR(00412710) W2_GFF_SCALAR(00412760) W2_GFF_SCALAR(004127b0)
W2_GFF_SCALAR(00412870)                                                                   // W2 t6 fork A: WriteFieldFLOAT (float bits)
#undef W2_GFF_SCALAR
unsigned __fastcall recovered_gff_00412970(void*, void*, void*, void*, const char*);       // WriteFieldCExoString
unsigned __fastcall recovered_gff_00412d30(void*, void*, void*, void*, const char*);       // W2 t6 fork A: WriteFieldVector
unsigned __fastcall recovered_gff_00412450(void*, void*, void*, void*, const char*);       // AddList
unsigned __fastcall recovered_gff_004124e0(void*, void*, void*, void*, unsigned);          // AddListElement
void* __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
void* __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
}
namespace {
using w2::save_combat::Word;
inline void* ptr(Word a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline Word word(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
struct Mem final : kotor::server_mainloop::Memory {
    Word read(Word a) override { Word v; std::memcpy(&v, ptr(a), 4); return v; }
    void write(Word a, Word v) override { std::memcpy(ptr(a), &v, 4); }
    Word read16(Word a) override { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
    void write16(Word a, Word v) override { auto x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
    Word read8(Word a) override { std::uint8_t v; std::memcpy(&v, ptr(a), 1); return v; }
};
struct Native final : w2::save_combat::Ports {
    [[noreturn]] void trap(Word a, const char* w) override { kotor_unreachable_trap(a, w); }
    Word write_field(Word writer, Word g, Word st, Word v, const char* l) override {
        switch (writer) {
        case w2::save_combat::kByte: return recovered_gff_00412620(ptr(g), nullptr, ptr(st), v, l);
        case w2::save_combat::kChar: return recovered_gff_00412670(ptr(g), nullptr, ptr(st), v, l);
        case w2::save_combat::kWord: return recovered_gff_004126c0(ptr(g), nullptr, ptr(st), v, l);
        case w2::save_combat::kShort: return recovered_gff_00412710(ptr(g), nullptr, ptr(st), v, l);
        case w2::save_combat::kDword: return recovered_gff_00412760(ptr(g), nullptr, ptr(st), v, l);
        case w2::save_combat::kInt: return recovered_gff_004127b0(ptr(g), nullptr, ptr(st), v, l);
        case w2::save_combat::kFloat: return recovered_gff_00412870(ptr(g), nullptr, ptr(st), v, l);
        case w2::save_combat::kString: return recovered_gff_00412970(ptr(g), nullptr, ptr(st), ptr(v), l);
        case w2::save_combat::kVector: return recovered_gff_00412d30(ptr(g), nullptr, ptr(st), ptr(v), l);
        default: kotor_unreachable_trap(writer, "W2 save_combat: unknown GFF writer");
        }
    }
    Word add_list(Word g, Word out, Word parent, const char* l) override { return recovered_gff_00412450(ptr(g), nullptr, ptr(out), ptr(parent), l); }
    Word add_list_element(Word g, Word out, Word list, Word type) override { return recovered_gff_004124e0(ptr(g), nullptr, ptr(out), ptr(list), type); }
    Word list_at_pos_005e9b60(Word l, Word pos) override { return word(unresolved_list_at_pos_005e9b60(ptr(l), nullptr, ptr(pos))); }
    void list_next_005e9b90(Word l, Word slot) override { (void)unresolved_list_next_005e9b90(ptr(l), nullptr, static_cast<void**>(ptr(slot))); }
};
namespace sc = w2::save_combat;
}
extern "C" {
unsigned __fastcall recovered_save_combat_round_004d3ec0(void* round, void*, void* gff, void* st) {
    Mem m; Native p; alignas(8) unsigned char s[0x50] = {}; return sc::save_combat_round_004d3ec0(m, p, word(round), word(gff), word(st), word(s));
}
unsigned __fastcall recovered_attack_data_save_004d2210(void* attack, void*, void* gff, void* st) {
    Mem m; Native p; alignas(8) unsigned char s[0x30] = {}; return sc::attack_data_save_004d2210(m, p, word(attack), word(gff), word(st), word(s));
}
unsigned __fastcall recovered_round_action_save_004d1dd0(void* action, void*, void* gff, void* st) {
    Mem m; Native p; return sc::round_action_save_004d1dd0(m, p, word(action), word(gff), word(st));
}
unsigned __fastcall recovered_save_listen_data_004cca50(void* object, void*, void* gff, void* parent) {
    Mem m; Native p; alignas(8) unsigned char s[0x30] = {}; return sc::save_listen_data_004cca50(m, p, word(object), word(gff), word(parent), word(s));
}
unsigned __fastcall recovered_follow_info_save_004eaf70(void* info, void*, void* gff, void* st) {
    Mem m; Native p; return sc::follow_info_save_004eaf70(m, p, word(info), word(gff), word(st));
}
}
