// lane-m48-traps / fork f-warp: CSWGuiMainMenu warp-button chain (swkotor.exe GOG K1, image base 0x400000).
//   0067C4B0 CSWGuiMainMenu::OnWarpButtonPressed (consumer: gui-load mainmenu.cpp event table, RET 4)
//   0067C3F0 CSWGuiMainMenu::OnLoadGamePicked    0067BC40 CSWGuiMainMenu::PopulateModules
//   0041A5A0 CSWGuiListBox::GetControl            0041C1D0 CSWGuiListBox::AddControls(CExoArrayList<CSWGuiControl*>&,...)
//   005E5500 CExoString::operator<=               005E6890 CExoAliasList::GetAliasPath -> 005EB650 CExoAliasListInternal::GetAliasPath
//   0067B9D0 CSWGuiMainMenu::OnModulePicked: D83 TRAP (module start; registered only by PopulateModules 0067C230/0067C23F).
// Bound existing composed-link owners: 005E6640 f200_ports::Enumerate005e6640 (-> 005E8CF0), 005E5680/005E5C50/005E5D10/
// 005E6270/005E6080/005E5200/005E5EB0/005E5790/005E5290/005E5900/005E5B00/005E5A90/005B3190/005E5C20 CExoString family,
// 004059A0 CExoArrayList<CExoString>::Add, 006FA852 eh vector destructor iterator, 006FA7E6/006FA390 live allocator,
// gui-load control family (0041B8B0, 004167F0, 00417280, 00415E00, 00414CD0, 0041AB20, 00419A90, 006C2A40, 00671C00,
// 0041B140, table 0073E658) and lane-m48-b 0041B860.
// Convention (D1): every original __thiscall entry is __fastcall(self, unused EDX, stack args...) with identical cleanup.
#include "e05ffa59c4-mainmenu.h"
#include "ed8f77ad6d-allocator_live.h"            // live_operator_new (006FA7E6) / live_free (006FA390)
#include "5907a9dfa7-kotor_unreachable_trap.h"    // D83 shared fatal trap (lane-m48-traps)
#include <cstdint>
#include <cstring>
#include <new>

using namespace gui_load;
namespace kih = kotor_input_hover;
using kotor_reconstruction::StartupString;

// ---------------------------------------------------------------- existing owners (declarations repeated exactly)
namespace f200_ports { void __fastcall Enumerate005e6640(void*, void*, void*, StartupString*, unsigned, unsigned, unsigned); }
namespace f200_ports { struct Enumeration; }
namespace directory_list { void __fastcall Add004059a0(f200_ports::Enumeration* self, void*, StartupString value); }
namespace startup_rand { StartupString* __fastcall SubString005e6270(StartupString* self, void*, StartupString* output,
                                                                      std::int32_t start, std::int32_t length); }
namespace directory_list { StartupString* __fastcall LowerCase005e6080(StartupString* self, void*, StartupString* output); }
namespace add_key_lower { int __fastcall Equal005e5200(StartupString*, void*, StartupString*); }
namespace options_video { struct String; }
namespace client_movies { std::int32_t __fastcall StringFind_005e5eb0(options_video::String* self, void* unused_edx,
                                                                        options_video::String* needle, std::int32_t start); }
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }   // CExoBase* cell 007A39E0
namespace kotor_movie { void __fastcall string_element_destroy_005e5c20(void*, void*); } // 005E5C20 as element dtor
extern "C" {
void __stdcall c2da_destroy_array_006fa852(void* first, std::uint32_t size, std::int32_t count, void (__fastcall* destroy)(void*, void*));
extern const unsigned char kotor_rdata_live_count_0073ecd2;                            // .rdata 0073ECD2 (07h)
void __fastcall gui_load_button_initialize_00418d80(gui_load::Object*, void*, gui_load::Word, gui_load::Word);
void __fastcall gui_m48_extent_list_add_0041b860(kotor_reconstruction::ArrayMutations*, void*, gui_load::Extent value) noexcept(false);
void __fastcall gui_load_navigable_destruct_00419ce0(gui_load::Navigable*, void*);
void __fastcall gui_load_border_destruct_004151c0(gui_load::Border*, void*);
extern const struct GuiLoadButtonMethods gui_load_button_methods_0073e658;
}

// ---------------------------------------------------------------- authored here (new names, NOTES.md convention)
extern "C" {
void __fastcall kotor_fastcall_0067c3f0(gui_load::MainMenu* self, void*) noexcept(false);
void __fastcall kotor_fastcall_0067bc40(gui_load::MainMenu* self, void*) noexcept(false);
gui_load::Control* __fastcall kotor_fastcall_0041a5a0(gui_lb::ListBox* self, void*, std::int32_t index);
void __fastcall kotor_fastcall_0041c1d0(gui_lb::ListBox* self, void*, kotor_reconstruction::ArrayMutations* controls,
                                        gui_load::Word suppress, gui_load::Word restore, gui_load::Word variable) noexcept(false);
int __fastcall kotor_fastcall_005e5500(StartupString* self, void*, StartupString* other);
StartupString* __fastcall kotor_fastcall_005e6890(void* alias_list, void*, StartupString* alias, std::int32_t length);
StartupString* __fastcall kotor_fastcall_005eb650(void* internal, void*, StartupString* alias, std::int32_t length);
}

namespace {
template<class T> T& at(void* object, unsigned offset) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset); }
gui_lb::ListBox* modules(MainMenu* menu) { return reinterpret_cast<gui_lb::ListBox*>(&menu->lb_modules); }  // +64 (0067C42B)
options_video::String* find_view(StartupString* s) { return reinterpret_cast<options_video::String*>(s); }
// Views of the ORIGINAL tables' called slots (types exactly as gui_vtables.cpp declares them).
struct ButtonCallView {            // 0073E658
    void* s00;
    void (__fastcall* set_extent)(gui_load::Object*, void*, const gui_load::Extent*);           // +04 00417A20
    void* s08_94[0x24];
    void (__fastcall* initialize98)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +98 00418D80
};
static_assert(offsetof(ButtonCallView, initialize98) == 0x98, "button +98");
struct TextCallView {              // 0073E3A4
    void* s00; void* s04;
    gui_load::Word (__fastcall* ideal_height)(gui_load::Text*, void*);                           // +08 00414EB0
};
struct ListBoxCallView {           // 0073E840
    void* s00; void* s04;
    void (__fastcall* set_active_control)(gui_lb::ListBox*, void*, gui_load::Control*, gui_load::Word);  // +08 0041C3A0
};
struct PanelCallView {             // 00752F70
    void* s00; void* s04;
    void (__fastcall* set_active_control)(gui_load::Object*, void*, gui_load::Object*, gui_load::Word);  // +08 0040A630
};
template<class V> const V* vt(const void* object) { const void* p; std::memcpy(&p, object, 4); return static_cast<const V*>(p); }

// Raw storage for a CExoString produced into an unconstructed return slot (005E5D10 / 005E6270 / 005E6080 outputs).
struct Slot { alignas(StartupString) unsigned char bytes[sizeof(StartupString)]; StartupString* p() { return reinterpret_cast<StartupString*>(bytes); } };

// Local CExoArrayList<CExoString> {data,count,capacity}; released like the original epilogue
// (0067C333..0067C364 / 0067C367..0067C395): eh vector destructor iterator(data, 8, [data-4], 005E5C20), free(data-4).
struct StringList {
    StartupString* data = nullptr; std::int32_t size = 0; std::int32_t capacity = 0;
    ~StringList() {
        if (data) {
            unsigned char* const block = reinterpret_cast<unsigned char*>(data) - 4;
            std::int32_t count; std::memcpy(&count, block, 4);
            c2da_destroy_array_006fa852(data, 8, count, &kotor_movie::string_element_destroy_005e5c20);
            live_free(block);
        }
        data = nullptr;
    }
};
// Local CExoArrayList<CSWGuiControl*> [1C]/[20]/[24]; freed at 0067C321..0067C32E.
struct ControlList {
    gui_load::Control** data = nullptr; std::int32_t size = 0; std::int32_t capacity = 0;
    ~ControlList() { live_free(data); data = nullptr; }
};
static_assert(sizeof(StringList) == 12 && sizeof(ControlList) == 12, "CExoArrayList storage");

const char rim_suffix_00752ff0[] = "_s.rim";         // .rdata 00752FF0
const char sentinel_00752ff8[] = "zzzzzzzz.zzz";     // .rdata 00752FF8
const char modules_suffix_00753008[] = ":modules";   // .rdata 00753008
const char modules_alias_0073d90c[] = "MODULES:";    // .rdata 0073D90C
const char live_format_0074e9ac[] = "live%d";        // .rdata 0074E9AC
const char empty_0073d71c[] = "";                    // .rdata 0073D71C

// Find(text, needle literal, 0) with the temporary literal CExoString destroyed right after (0067C0AB..0067C0E0).
std::int32_t find_literal(StartupString* text, const char* literal) {
    StartupString needle(literal);
    return client_movies::StringFind_005e5eb0(find_view(text), nullptr, find_view(&needle), 0);
}
}

// ================================================================ 0041A5A0 CSWGuiListBox::GetControl(int) RET 4
gui_load::Control* __fastcall kotor_fastcall_0041a5a0(gui_lb::ListBox* self, void*, std::int32_t index) {
    if (index < 0) return nullptr;                                                   // 0041A5A4..A6 jl
    if (index >= self->items.size) return nullptr;                                   // 0041A5A8..AE jge [+2A0]
    gui_load::Control* control;
    std::memcpy(&control, reinterpret_cast<const void*>(static_cast<std::uintptr_t>(self->items.data + static_cast<std::uint32_t>(index) * 4u)), 4); // 0041A5B0..B6
    return control;
}

// ================================================================ 0041C1D0 CSWGuiListBox::AddControls(list&, suppressCleanUp, restoreExtents, variableItemHeights) RET 10h
void __fastcall kotor_fastcall_0041c1d0(gui_lb::ListBox* self, void*, kotor_reconstruction::ArrayMutations* controls,
                                        gui_load::Word suppress, gui_load::Word restore, gui_load::Word variable) noexcept(false) {
    std::int32_t next_id = 0;                                                        // [esp+10h] (0041C1E2)
    const std::int32_t count = controls->size;                                       // [esp+14h] = [list+4] read BEFORE ClearItems (0041C1DA/E6)
    gui_load_listbox_clear_items_00419a90(self, nullptr);                            // 0041C1EA
    self->lb_flags = (self->lb_flags & 0xFFFFFFF9u) | ((((restore & 1u) << 1) | (suppress & 1u)) << 1); // 0041C1EF..0041C210
    for (std::int32_t i = 0; i < count; ++i) {                                       // 0041C20E/0041C27A..7D
        gui_load::Control* c;                                                        // list data re-read each pass (0041C220..26)
        std::memcpy(&c, reinterpret_cast<const void*>(static_cast<std::uintptr_t>(controls->data + static_cast<std::uint32_t>(i) * 4u)), 4);
        const gui_load::Word word = static_cast<gui_load::Word>(reinterpret_cast<std::uintptr_t>(c));
        if (gui_load_int_list_index_of_006c2a40(&self->items, nullptr, word) != -1) continue; // 0041C229..34
        c->id = next_id;                                                             // 0041C23A
        ++next_id;                                                                   // 0041C23D/0041C244
        c->owner = self;                                                             // 0041C241
        gui_load_array_add_00671c00(&self->items, nullptr, word);                    // 0041C23E..48
        gui_m48_extent_list_add_0041b860(&self->extents, nullptr, c->extent);        // 0041C24D..71 (extent +4 by value)
    }
    const gui_load::Word flags = self->lb_flags;                                     // 0041C27F
    self->lb_flags = ((((variable << 8) ^ flags) & 0x100u) ^ flags) | 8u;            // 0041C285..9C
    gui_load_listbox_organize_0041b140(self, nullptr);                               // 0041C2A2
}

// ================================================================ 005E5500 CExoString::operator<=(CExoString const&) RET 4
int __fastcall kotor_fastcall_005e5500(StartupString* self, void*, StartupString* other) {
    const unsigned char* a = reinterpret_cast<const unsigned char*>(self->data);     // 005E5500
    if (a == nullptr) return 1;                                                      // 005E5505 -> 005E5559..5576 (any right side)
    const unsigned char* b = reinterpret_cast<const unsigned char*>(other->data);    // 005E550B
    if (b == nullptr) return *a == 0 ? 1 : 0;                                        // 005E550F -> 005E5567..5576 / 5570
    std::int32_t r = 0;                                                              // 005E5512..5534 bytewise unsigned compare
    for (;;) {
        if (a[0] != b[0]) { r = a[0] < b[0] ? -1 : 1; break; }                       // 005E5518 / 005E5546..48 sbb
        if (a[0] == 0) break;                                                        // 005E551C
        if (a[1] != b[1]) { r = a[1] < b[1] ? -1 : 1; break; }                       // 005E5528
        if (a[1] == 0) break;                                                        // 005E5532
        a += 2; b += 2;                                                              // 005E552C..2F
    }
    return r <= 0 ? 1 : 0;                                                           // 005E553A..3C / 005E554D..4F setle
}

// ================================================================ 005E6890 CExoAliasList::GetAliasPath(CExoString const&, int) RET 8
StartupString* __fastcall kotor_fastcall_005e6890(void* alias_list, void*, StartupString* alias, std::int32_t length) {
    return kotor_fastcall_005eb650(at<void*>(alias_list, 0), nullptr, alias, length);  // 005E6890 mov ecx,[ecx]; jmp 005EB650
}
// 005EB650 CExoAliasListInternal::GetAliasPath: internal +0 default path CExoString, +8 first node;
// node {+0 alias CExoString, +8 path CExoString, +10 next} (same layout ~CExoAliasListInternal 005EC0E0 walks).
StartupString* __fastcall kotor_fastcall_005eb650(void* internal, void*, StartupString* alias, std::int32_t length) {
    if (alias->BorrowedEquals005e5290(empty_0073d71c) != 0)                          // 005EB658..66
        return static_cast<StartupString*>(internal);                                // 005EB668
    if (length == 0) length = alias->BorrowedLength005e5790();                        // 005EB671..80
    for (void* node = at<void*>(internal, 8); node != nullptr; node = at<void*>(node, 0x10)) { // 005EB682..87 / 005EB697
        if (static_cast<StartupString*>(node)->ComparePrefix005e5900(alias, length) == 1)      // 005EB689..95
            return reinterpret_cast<StartupString*>(static_cast<unsigned char*>(node) + 8);   // 005EB69D
    }
    return static_cast<StartupString*>(internal);                                    // 005EB6A8
}

// ================================================================ 0067B9D0 CSWGuiMainMenu::OnModulePicked(CSWGuiControl*) RET 4 — D83 TRAP
// Starts a game module (CAppManager::CreateServer 00401380, CNetLayer::StartConnectToSession 005D5040, load screen
// 005EDCF0/005EDA80, SetInputClass): milestone-2+ debt. Reachable only by user activation of a warp-list module button,
// whose list exists only after OnWarpButtonPressed on the hidden BTN_WARP (TRAPS: f-warp/FORK.json traps[0]).
namespace gui_load_warp {
struct MainMenuWarp : kih::MainMenu {
    void __thiscall OnModulePicked(kih::Control* control);                           // 0067B9D0
};
void __thiscall MainMenuWarp::OnModulePicked(kih::Control*) {
    kotor_unreachable_trap(0x0067B9D0u, "CSWGuiMainMenu::OnModulePicked");
}
}

// ================================================================ 0067BC40 CSWGuiMainMenu::PopulateModules() RET
void __fastcall kotor_fastcall_0067bc40(gui_load::MainMenu* self, void*) noexcept(false) {
    StartupString name;                    // [4C] 0067BC6B (EH state 0)
    StartupString s34;                     // [34] 0067BC7D
    StartupString s2c;                     // [2C] 0067BC8E
    StringList rims;                       // [60]/[64]/[68] zeroed 0067BC93..9B (type 0xBBA list)
    StringList mods;                       // [54]/[58]/[5C] zeroed 0067BC9F..A7 (type 0x7DB list)
    // [B8..C0] and [CC..D4] are zeroed (0067BCAB..CE) and never read or released: dead stores, not modeled.
    ControlList controls;                  // [1C]/[20]/[24] zeroed 0067BCD5..DD
    gui_load::Extent extent;               // [78..84] = list box inner extent +2F0 (= ListBox+28C) 0067BCE1..0067BD1A
    std::memcpy(&extent, &modules(self)->inner, sizeof extent);
    std::int32_t i = 0;                    // [28] 0067BD12 (rim index)
    std::int32_t j = 0;                    // edi / [3C] 0067BD01/16 (mod index)
    StartupString unused_b0;               // [B0] 0067BD21: constructed, only destroyed at 0067C30D
    {
        StartupString alias(modules_alias_0073d90c);                                  // [14] 0067BD26..37
        f200_ports::Enumerate005e6640(kotor_menu_10_19::dependencies::base_007a39e0, nullptr, &mods, &alias, 0x7DB, 0, 1); // 0067BD3C..5C
    }                                                                                // 0067BD6D
    {
        StartupString alias(modules_alias_0073d90c);                                  // 0067BD72..7B
        f200_ports::Enumerate005e6640(kotor_menu_10_19::dependencies::base_007a39e0, nullptr, &rims, &alias, 0xBBA, 0, 1); // 0067BD80..A0
    }                                                                                // 0067BDB1
    StartupString path;                                                              // [40] 0067BDB6
    for (std::uint32_t live = 1; live < kotor_rdata_live_count_0073ecd2; ++live) {  // 0067BDBF..D3 / 0067BEAC..B6 (byte compare)
        kotor_reconstruction::StartupFormat005e5680(&path, live_format_0074e9ac, live);   // 0067BDE0..EB "live%d"
        void* const alias_list = at<void*>(kotor_menu_10_19::dependencies::base_007a39e0, 0x0C);  // 0067BDF0..F5
        StartupString* const resolved = kotor_fastcall_005e6890(alias_list, nullptr, &path, 0);   // 0067BDFB..0067BE01
        if (resolved->BorrowedLength005e5790() <= 0) continue;                       // 0067BE06..0F
        {
            StartupString suffix(modules_suffix_00753008);                            // [14] 0067BE15..1E ":modules"
            Slot sum;                                                                // [6C]
            StartupString* const joined = GuiConcat005e5d10(&path, nullptr, sum.p(), &suffix); // 0067BE23..39
            path.Assign005e5c50(joined);                                             // 0067BE3E..4B
            joined->~StartupString();                                                // 0067BE5C
        }                                                                            // 0067BE6D
        f200_ports::Enumerate005e6640(kotor_menu_10_19::dependencies::base_007a39e0, nullptr, &mods, &path, 0x7DB, 0, 1);  // 0067BE72..8A
        f200_ports::Enumerate005e6640(kotor_menu_10_19::dependencies::base_007a39e0, nullptr, &rims, &path, 0xBBA, 0, 1);  // 0067BE8F..A7
    }
    void* const proto = at<void*>(self, 0x330);                                      // [6C] 0067BEBC/CA (ListBox+2CC proto item)
    const std::int32_t rim_count = rims.size;                                        // [74] 0067BEC2/CE
    const std::int32_t mod_count = mods.size;                                        // [48] 0067BEC6/D2

top:                                                                                 // 0067BEE0
    if (i < rim_count) {                                                             // 0067BEE4..E8 -> 0067C09A
        s34.Assign005e5c50(&rims.data[i]);                                           // 0067C09A..A6
        if (find_literal(&s34, rim_suffix_00752ff0) == -1) goto check_mod;           // 0067C0AB..E8 "_s.rim"
        ++i; goto top;                                                               // 0067C0EE..F2
    }
    if (j >= mod_count) goto done;                                                   // 0067BEEE..F2
    { StartupString sentinel(sentinel_00752ff8); s34.Assign005e5c50(&sentinel); }    // 0067BEF8..0067BF2A
check_mod:                                                                           // 0067BF2F
    if (j < mod_count) {                                                             // 0067BF2F..33 -> 0067C0F7
        s2c.Assign005e5c50(&mods.data[j]);                                           // 0067C0F7..0067C103
        if (find_literal(&s2c, rim_suffix_00752ff0) == -1) goto trim;                // 0067C108..45
        ++j; goto top;                                                               // 0067C14B..50
    }
    { StartupString sentinel(sentinel_00752ff8); s2c.Assign005e5c50(&sentinel); }    // 0067BF39..6B
trim:                                                                                // 0067BF70
    {
        Slot cut;                                                                    // [E0]
        StartupString* const r = startup_rand::SubString005e6270(&s34, nullptr, cut.p(), 0, s34.BorrowedLength005e5790() - 4); // 0067BF70..8A
        s34.Assign005e5c50(r);                                                       // 0067BF8F..9C
        r->~StartupString();                                                         // 0067BFB0
    }
    {
        Slot cut;                                                                    // [D8]
        StartupString* const r = startup_rand::SubString005e6270(&s2c, nullptr, cut.p(), 0, s2c.BorrowedLength005e5790() - 4); // 0067BFB5..CF
        s2c.Assign005e5c50(r);                                                       // 0067BFD4..E1
        r->~StartupString();                                                         // 0067BFF5
    }
    int rim_first;
    {
        Slot lower_mod;                                                              // [C4]
        StartupString* const m = directory_list::LowerCase005e6080(&s2c, nullptr, lower_mod.p());  // 0067BFFA..0067C006
        Slot lower_rim;                                                              // [A0]
        StartupString* const r = directory_list::LowerCase005e6080(&s34, nullptr, lower_rim.p());  // 0067C00D..21
        rim_first = kotor_fastcall_005e5500(r, nullptr, m);                          // 0067C026..31
        r->~StartupString();                                                         // 0067C047 ([A0] first)
        m->~StartupString();                                                         // 0067C05B ([C4])
    }
    if (rim_first == 0) {                                                            // 0067C060..62
        name.Assign005e5c50(&s2c);                                                   // 0067C155..5E
        ++j;                                                                         // 0067C163..64
    } else {
        name.Assign005e5c50(&s34);                                                   // 0067C068..71
        ++i;                                                                         // 0067C076..84
        if (add_key_lower::Equal005e5200(&s34, nullptr, &s2c) != 0) ++j;             // 0067C07A..95 -> 0067C163
    }
    {                                                                                // 0067C168: new CSWGuiButton (0x1C4, inlined ctor)
        void* const memory = live_operator_new(0x1C4);                               // 0067C168..6D
        gui_load::Button* button = nullptr;
        if (memory != nullptr) {                                                     // 0067C17B..85 (state 16h: delete on unwind)
            button = static_cast<gui_load::Button*>(memory);
            try {
                gui_load_navigable_construct_0041b8b0(button, nullptr);              // 0067C189
            } catch (...) { live_free(memory); throw; }
            button->vtbl = reinterpret_cast<const ObjectVtbl*>(&gui_load_button_methods_0073e658);  // 0067C199 vptr 0073E658
            try {                                                                    // state 17h
                gui_load_border_construct_004167f0(&button->border, nullptr);        // 0067C19F (+6C)
                try {                                                                // state 18h
                    gui_load_border_construct_004167f0(&button->hilight, nullptr);   // 0067C1B2 (+E0)
                    try {                                                            // state 19h
                        gui_load_text_construct_00417280(&button->text, nullptr);    // 0067C1C5 (+154)
                    } catch (...) { gui_load_border_destruct_004151c0(&button->hilight, nullptr); throw; }
                } catch (...) { gui_load_border_destruct_004151c0(&button->border, nullptr); throw; }
            } catch (...) { gui_load_navigable_destruct_00419ce0(button, nullptr); live_free(memory); throw; }
        }                                                                            // 0067C1CC: esi = 0 when allocation failed (then [esi] read: original fault)
        vt<ButtonCallView>(button)->initialize98(button, nullptr,                    // 0067C1CE..E4 vslot +98(&extent, proto)
            static_cast<gui_load::Word>(reinterpret_cast<std::uintptr_t>(&extent)),
            static_cast<gui_load::Word>(reinterpret_cast<std::uintptr_t>(proto)));
        gui_load_text_params_set_text_00415e00(reinterpret_cast<unsigned char*>(button) + 0x16C, nullptr, &name); // 0067C1EA..F5
        const std::int32_t dim = gui_load_border_params_dim_00414cd0(
            reinterpret_cast<const BorderParams*>(reinterpret_cast<unsigned char*>(button) + 0x80), nullptr);    // 0067C1FA..0067C206
        gui_load::Text* const text = &button->text;                                  // 0067C200 edi = +154
        const gui_load::Word ideal = vt<TextCallView>(text)->ideal_height(text, nullptr);   // 0067C20B..13 vslot +08
        extent.height = static_cast<std::int32_t>(ideal) + dim * 2;                  // 0067C216..1D [84] = eax + dim*2
        vt<ButtonCallView>(button)->set_extent(button, nullptr, &extent);            // 0067C224..2D vslot +04
        const EventFunc picked = static_cast<EventFunc>(&gui_load_warp::MainMenuWarp::OnModulePicked);  // 67B9D0h
        gui_load_control_add_event_0041ab20(button, nullptr, 0x27, reinterpret_cast<kih::GuiObject*>(self), picked); // 0067C230..3A
        gui_load_control_add_event_0041ab20(button, nullptr, 0x2D, reinterpret_cast<kih::GuiObject*>(self), picked); // 0067C23F..49
        if (controls.size == controls.capacity) {                                    // 0067C24E..56 inlined CExoArrayList<ptr>::Add
            const std::int32_t grown = controls.capacity != 0 ? controls.capacity * 2 : 0x10;  // 0067C258..65
            gui_load::Control** const previous = controls.data;                      // 0067C265
            controls.capacity = grown;                                               // 0067C271
            controls.data = static_cast<gui_load::Control**>(live_operator_new(static_cast<std::uint32_t>(grown) * 4u)); // 0067C269..7E
            for (std::int32_t k = 0; k < controls.size; ++k) controls.data[k] = previous[k];   // 0067C285..A1
            if (previous != nullptr) live_free(previous);                            // 0067C2A3..A8
        }
        controls.data[controls.size] = button;                                       // 0067C2B0..B8
        ++controls.size;                                                             // 0067C2BB..C3
        directory_list::Add004059a0(reinterpret_cast<f200_ports::Enumeration*>(&at<gui_load::Word>(self, 0x344)), nullptr, name); // 0067C2C0..DD (+344 CExoArrayList<CExoString>, copy 005E5B00)
    }
    goto top;                                                                        // 0067C2E2..E6
done:                                                                                // 0067C2EB
    kotor_fastcall_0041c1d0(modules(self), nullptr,
        reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&controls), 1, 0, 0);    // 0067C2EB..F7
    // Destruction order (C++ reverse declaration) equals 0067C2FC..0067C3CC: path [40], unused_b0 [B0], controls data
    // free, mods array, rims array, s2c [2C], s34 [34], name [4C].
}

// ================================================================ 0067C3F0 CSWGuiMainMenu::OnLoadGamePicked() RET
void __fastcall kotor_fastcall_0067c3f0(gui_load::MainMenu* self, void*) noexcept(false) {
    if ((self->flags & 0x600u) == 0x400u) return;                                    // 0067C3F3..0067C400
    if (self->u140c == 0) return;                                                    // 0067C406..0E (set 1 by Draw 0067AC28)
    gui_lb::ListBox* const list = modules(self);                                     // 0067C42B edi = +64
    if (list->items.size == 0) kotor_fastcall_0067bc40(self, nullptr);               // 0067C414..1E [+304] = ListBox+2A0
    at<gui_load::Word>(self, 0xA8) = at<gui_load::Word>(self, 0xA8) | 2u;            // 0067C423..31 list box flags (+64+44) |= 2
    static const unsigned hide[] = { 0x3A4, 0x5F8, 0x7BC, 0x434, 0x980, 0x10C8, 0x128C, 0xE48, 0xF88 };  // 0067C434..71 &= ~2
    for (unsigned offset : hide) at<gui_load::Word>(self, offset) = at<gui_load::Word>(self, offset) & 0xFFFFFFFDu;
    vt<PanelCallView>(self)->set_active_control(self, nullptr, reinterpret_cast<gui_load::Object*>(list), 0); // 0067C477..7E vslot +08
    const ListBoxCallView* const lv = vt<ListBoxCallView>(list);                     // 0067C481 ebp = [edi] before GetControl
    gui_load::Control* const first = kotor_fastcall_0041a5a0(list, nullptr, 0);      // 0067C483..89
    lv->set_active_control(list, nullptr, first, 0);                                 // 0067C48E..91 vslot +08(first, 0)
    self->u350 = 2;                                                                  // 0067C495
}

// ================================================================ 0067C4B0 CSWGuiMainMenu::OnWarpButtonPressed(CSWGuiControl*) RET 4
void __thiscall gui_load::MainMenuHandlers::OnWarpButtonPressed(kotor_input_hover::Control*) {
    kotor_fastcall_0067c3f0(reinterpret_cast<gui_load::MainMenu*>(this), nullptr);   // 0067C4B0 call 0067C3F0; RET 4
}
