// W2 tier 6 (fork C) native binding: CSWSItem::SaveItem 0055CCD0, SaveContainerItems 0055CFA0,
// SaveItemProperties 00555790. GFF writers: 0873 @recovered_gff_* exports (FLOAT 00412870 from W2 t6 fork A).
#if !defined(_M_IX86)
#error W2 native bindings require the Windows x86 route
#endif
#include "73fb4cc128-w2_save_item.hpp"
#include <cstdint>
#include <cstring>
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
extern void* kotor_cell_rules_007a3a28;
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void*, void*, int);
void* __fastcall recovered_item_lookup_004ae760(void*, void*, unsigned);
unsigned __fastcall recovered_item_cost_00554000(void*, void*);                         // W2 t4
unsigned __fastcall recovered_gff_00412620(void*, void*, void*, unsigned, const char*);   // BYTE
unsigned __fastcall recovered_gff_004126c0(void*, void*, void*, unsigned, const char*);   // WORD
unsigned __fastcall recovered_gff_00412760(void*, void*, void*, unsigned, const char*);   // DWORD
unsigned __fastcall recovered_gff_004127b0(void*, void*, void*, unsigned, const char*);   // INT
unsigned __fastcall recovered_gff_00412870(void*, void*, void*, float, const char*);      // FLOAT (W2 t6 fork A)
unsigned __fastcall recovered_gff_00412970(void*, void*, void*, void*, const char*);      // CExoString
unsigned __fastcall recovered_gff_00412a10(void*, void*, void*, void*, const char*);      // CExoLocString
unsigned __fastcall recovered_gff_00412450(void*, void*, void*, void*, const char*);      // AddList(out list, parent struct, label)
unsigned __fastcall recovered_gff_004124e0(void*, void*, void*, void*, unsigned);         // AddListElement(out struct, list, type)
unsigned __fastcall recovered_save_item_0055ccd0(void*, void*, void*, void*);
}
namespace {
using w2::save_item::Word;
inline void* ptr(Word a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline Word word(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
struct Mem final : kotor::server_mainloop::Memory {
    Word read(Word a) override { if (a == 0x7a39fcu) return word(kotor_menu_window_cells::manager_007a39fc); Word v; std::memcpy(&v, ptr(a), 4); return v; }
    void write(Word a, Word v) override { std::memcpy(ptr(a), &v, 4); }
    Word read16(Word a) override { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
    void write16(Word a, Word v) override { auto x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
    Word read8(Word a) override { std::uint8_t v; std::memcpy(&v, ptr(a), 1); return v; }
};
// Original .rdata label -> text, copied into a zero-padded 16-byte label buffer (as the 0873 chargen binding does).
struct Label {
    char text[16] = {};
    explicit Label(Word a) {
        const char* t = nullptr;
        switch (a) {
        case 0x00749018u: t = "BaseItem"; break;       case 0x00747a28u: t = "Tag"; break;
        case 0x0074900cu: t = "Identified"; break;     case 0x007488c0u: t = "Description"; break;
        case 0x00748ffcu: t = "DescIdentified"; break; case 0x00748fecu: t = "LocalizedName"; break;
        case 0x00748fe0u: t = "StackSize"; break;      case 0x00748fd8u: t = "Stolen"; break;
        case 0x00748fccu: t = "Upgrades"; break;       case 0x0074721cu: t = "Dropable"; break;
        case 0x00748fbcu: t = "Pickpocketable"; break; case 0x00748facu: t = "ModelVariation"; break;
        case 0x00748f9cu: t = "BodyVariation"; break;  case 0x00748f90u: t = "TextureVar"; break;
        case 0x00748f88u: t = "Charges"; break;        case 0x00748f7cu: t = "MaxCharges"; break;
        case 0x00748f74u: t = "Cost"; break;           case 0x00748f6cu: t = "AddCost"; break;
        case 0x00748f64u: t = "Plot"; break;           case 0x00745d80u: t = "XPosition"; break;
        case 0x00745d74u: t = "YPosition"; break;      case 0x00745d68u: t = "ZPosition"; break;
        case 0x00745d38u: t = "XOrientation"; break;   case 0x00745d48u: t = "YOrientation"; break;
        case 0x00745d58u: t = "ZOrientation"; break;   case 0x00748f54u: t = "NonEquippable"; break;
        case 0x00748f4cu: t = "NewItem"; break;        case 0x00748f40u: t = "DELETING"; break;
        case 0x00747210u: t = "ItemList"; break;       case 0x00748e84u: t = "PropertiesList"; break;
        case 0x00748e74u: t = "PropertyName"; break;   case 0x00748e6cu: t = "Subtype"; break;
        case 0x00748e60u: t = "CostTable"; break;      case 0x00748e54u: t = "CostValue"; break;
        case 0x00748e4cu: t = "Param1"; break;         case 0x00748e40u: t = "Param1Value"; break;
        case 0x00748e30u: t = "ChanceAppear"; break;   case 0x00748bfcu: t = "UsesPerDay"; break;
        case 0x00748e28u: t = "Useable"; break;        case 0x00748db8u: t = "UpgradeType"; break;
        default: kotor_unreachable_trap(a, "W2 save_item label not transcribed");
        }
        const std::size_t n = std::strlen(t); std::memcpy(text, t, n < sizeof text ? n : sizeof text);
    }
};
float f32(Word b) { float v; std::memcpy(&v, &b, 4); return v; }
struct Native final : w2::save_item::Ports {
    [[noreturn]] void trap(Word a, const char* w) override { kotor_unreachable_trap(a, w); }
    Word app_007a39fc() override { return word(kotor_menu_window_cells::manager_007a39fc); }
    Word rules_007a3a28() override { return word(kotor_cell_rules_007a3a28); }
    Word base_item_005b31a0(Word a, Word t) override { return word(kotor_m15b_base_item_array_get_base_item_005b31a0(ptr(a), nullptr, static_cast<int>(t))); }
    Word item_by_id_004ae760(Word s, Word id) override { return word(recovered_item_lookup_004ae760(ptr(s), nullptr, id)); }
    Word item_cost_00554000(Word i) override { return recovered_item_cost_00554000(ptr(i), nullptr); }
    Word save_item_0055ccd0(Word i, Word g, Word st) override { return recovered_save_item_0055ccd0(ptr(i), nullptr, ptr(g), ptr(st)); }
    void write_byte_00412620(Word g, Word st, Word v, Word l) override { Label t(l); (void)recovered_gff_00412620(ptr(g), nullptr, ptr(st), v, t.text); }
    void write_word_004126c0(Word g, Word st, Word v, Word l) override { Label t(l); (void)recovered_gff_004126c0(ptr(g), nullptr, ptr(st), v, t.text); }
    void write_dword_00412760(Word g, Word st, Word v, Word l) override { Label t(l); (void)recovered_gff_00412760(ptr(g), nullptr, ptr(st), v, t.text); }
    void write_int_004127b0(Word g, Word st, Word v, Word l) override { Label t(l); (void)recovered_gff_004127b0(ptr(g), nullptr, ptr(st), v, t.text); }
    void write_float_00412870(Word g, Word st, Word b, Word l) override { Label t(l); (void)recovered_gff_00412870(ptr(g), nullptr, ptr(st), f32(b), t.text); }
    void write_string_00412970(Word g, Word st, Word s, Word l) override { Label t(l); (void)recovered_gff_00412970(ptr(g), nullptr, ptr(st), ptr(s), t.text); }
    void write_locstring_00412a10(Word g, Word st, Word s, Word l) override { Label t(l); (void)recovered_gff_00412a10(ptr(g), nullptr, ptr(st), ptr(s), t.text); }
    void add_list_00412450(Word g, Word out, Word parent, Word l) override { Label t(l); (void)recovered_gff_00412450(ptr(g), nullptr, ptr(out), ptr(parent), t.text); }
    void add_list_element_004124e0(Word g, Word out, Word list, Word type) override { (void)recovered_gff_004124e0(ptr(g), nullptr, ptr(out), ptr(list), type); }
};
}
namespace si = w2::save_item;
extern "C" {
// thiscall (item) RET 8 -> @16. Each frame owns its CResList/CResStruct scratch (the original's stack locals).
unsigned __fastcall recovered_save_item_0055ccd0(void* item, void*, void* gff, void* st) {
    Mem m; Native p; alignas(8) unsigned char s[0x28] = {}; return si::save_item_0055ccd0(m, p, word(item), word(gff), word(st), word(s));
}
unsigned __fastcall recovered_save_container_items_0055cfa0(void* item, void*, void* gff, void* parent) {
    Mem m; Native p; alignas(8) unsigned char s[0x28] = {}; return si::save_container_items_0055cfa0(m, p, word(item), word(gff), word(parent), word(s));
}
unsigned __fastcall recovered_save_item_properties_00555790(void* item, void*, void* gff, void* parent) {
    Mem m; Native p; alignas(8) unsigned char s[0x28] = {}; return si::save_item_properties_00555790(m, p, word(item), word(gff), word(parent), word(s));
}
}
