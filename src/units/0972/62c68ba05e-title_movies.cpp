// lane-m1-movies (Milestone 1, L2): Movies button -> CSWGuiTitleMovies panel -> Back.
//   0067B250 CSWGuiMainMenu::OnMoviesPicked (full body: authored guard + post-guard body; lifts TRAP #73 0067B28C)
//   006DD910 CSWGuiTitleMovies::CSWGuiTitleMovies      006DCFE0 / 006DDAF0 ~CSWGuiTitleMovies (+ deleting)
//   006DCE80 CSWGuiTitleMovies::HandleInputEvent       006DD100 CSWGuiTitleMovies::PopulateMovies
//   00758130 CSWGuiTitleMovies method table (27 slots, typed constant struct, D21)
//   006DCF10 CSWGuiTitleMovies::OnMoviePicked: full body (lane-m1-movies-play replacement; lifts trap 006DCF37)
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only; nothing executed.
// Bound existing M49 owners (declarations repeated exactly as their owners spell them): gui-load control/panel
// family, lane-m48-traps f-warp 0041A5A0/0041C1D0/005E6890, lowers-directory-list 005E6640/004059A0/005E8C60,
// CExoString family (StartupString, 005E5680, 005E5D10, 005E6270, 005E5B00), core47 C2DA::GetINTEntry 00414110,
// M40 CTlkTable::GetSimpleString 0041E8F0, r4-lowers PopModalPanel 0040BE00, lane-input PlayGuiSound 0040A140,
// live allocator 006FA7E6/006FA390, task-10 eh_vector_destructor_iterator 006FA852.
// L0-shared leaves (lane-m1-options/shared, single owner): 00624BB0, 005ED9B0, 005ED9E0.
#include "f9a4d28992-movies_m1.h"
#include "95913ca8e0-mainmenu.h"                  // gui_load::MainMenuHandlers, AddPanel, StartupFormat005e5680, Tlk cell
#include "dad7f4429e-startup_views.h"             // M40 kotor_menu_10_19::Tlk::simple_0041e8f0, base_007a39e0
#include "b976ed802d-m1_shared.h"                 // L0-shared (lane-m1-options): 00624BB0 thunk, 005ED9B0, 005ED9E0
#include "015caeaf6d-allocator_live.h"            // live_operator_new (006FA7E6) / live_free (006FA390)
#include <cstdint>
#include <cstring>
#include <new>

using namespace gui_load;
namespace kih = kotor_input_hover;
using kotor_reconstruction::StartupString;
using kotor_m1_movies::TitleMovies;

// ---------------------------------------------------------------- existing owners (declarations repeated exactly)
namespace f200_ports { void __fastcall Enumerate005e6640(void*, void*, void*, StartupString*, unsigned, unsigned, unsigned); }
namespace f200_ports { struct Enumeration; }
namespace directory_list {
void __fastcall Add004059a0(f200_ports::Enumeration* self, void*, StartupString value);
void __fastcall Insert005e8c60(f200_ports::Enumeration* self, void*, StartupString value, int index);
}
namespace startup_rand { StartupString* __fastcall SubString005e6270(StartupString* self, void*, StartupString* output,
                                                                      std::int32_t start, std::int32_t length); }
namespace kotor_movie { void __fastcall string_element_destroy_005e5c20(void*, void*); } // 005E5C20 as element dtor
StartupString* __fastcall GuiConcat005e5d10(StartupString*, void*, StartupString*, const StartupString*);   // ini-string-priority
extern "C" {
void __stdcall c2da_destroy_array_006fa852(void* first, std::uint32_t size, std::int32_t count, void (__fastcall* destroy)(void*, void*));
extern const unsigned char kotor_rdata_live_count_0073ecd2;                            // .rdata 0073ECD2 (lane-tlk)
extern void* kotor_cell_rules_007a3a28;                                                // .data 007A3A28 CSWRules* (lane-prepare)
gui_load::Control* __fastcall kotor_fastcall_0041a5a0(gui_lb::ListBox* self, void*, std::int32_t index);
void __fastcall kotor_fastcall_0041c1d0(gui_lb::ListBox* self, void*, kotor_reconstruction::ArrayMutations* controls,
                                        gui_load::Word suppress, gui_load::Word restore, gui_load::Word variable) noexcept(false);
StartupString* __fastcall kotor_fastcall_005e6890(void* alias_list, void*, StartupString* alias, std::int32_t length);
int __fastcall kotor_fastcall_00414110(void* table, void*, StartupString* row, StartupString* column, std::int32_t* out);
void* __fastcall kotor_fastcall_005ed700(void* client_app, void*);                    // CClientExoApp::GetClientOptions
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* string) noexcept(false); // CResRef(CExoString const&)
void* __fastcall kotor_fastcall_0040be00(void* gui, void*);                            // CSWGuiManager::PopModalPanel
void __fastcall gui_load_border_params_set_pulsing_00414d00(void* params, void*, gui_load::Word a, gui_load::Word b,
                                                            gui_load::Word c);          // CSWGuiBorderParams::SetPulsingAlpha
void __fastcall gui_load_button_initialize_00418d80(gui_load::Object*, void*, gui_load::Word, gui_load::Word);
void __fastcall gui_load_navigable_destruct_00419ce0(gui_load::Navigable*, void*);
void __fastcall gui_load_label_destruct_00419d10(gui_load::Label*, void*);
extern const struct GuiLoadButtonMethods gui_load_button_methods_0073e658;
extern const MenuColor borrowed_exit_color_007a23b4;                                   // .data 007A23B4 (input-cells)
extern const MenuColor borrowed_enter_color_007a23c0;                                  // .data 007A23C0 (input-cells)
// lane-m1-movies-play (OnMoviePicked 006DCF10 body):
std::int32_t __fastcall kotor_fastcall_005edb30(void* client_app, void* unused_edx, StartupString* name, std::int32_t* out,
                                               std::int32_t unused_3, std::int32_t unused_4) noexcept(false);   // RET 10h
void __fastcall kotor_fastcall_005ee1c0(void* client_app, void* unused_edx, void* callback12,
                                        StartupString* name) noexcept(false);                                  // RET 8
}

namespace kotor_m1_movies {
// Event callback of the movie list entries (registered by PopulateMovies 006DD361..37A / 006DD69D..6B6).
struct TitleMoviesEvents : kih::Panel {
    void __thiscall OnMoviePicked_006dcf10(kih::Control* control);
};
}

namespace {
template<class T> T& at(void* object, unsigned offset) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset); }
std::uint32_t word_at(const void* object, unsigned offset) {
    std::uint32_t value; std::memcpy(&value, static_cast<const unsigned char*>(object) + offset, sizeof value); return value;
}
template<class V> const V* vt(const void* object) { const void* p; std::memcpy(&p, object, 4); return static_cast<const V*>(p); }
gui_load::Word word_of(const void* p) { return static_cast<gui_load::Word>(reinterpret_cast<std::uintptr_t>(p)); }
kih::GuiObject* event_target(TitleMovies* self) { return reinterpret_cast<kih::GuiObject*>(self); }
f200_ports::Enumeration* names_view(TitleMovies* self) { return reinterpret_cast<f200_ports::Enumeration*>(&self->names); }
void* client_app() { return at<void*>(kotor_menu_window_cells::manager_007a39fc, 4); }   // [[007A39FC]+4]
// [[[007A3A28]+B8h]+11Ch]: CSWRules -> movies C2DA (006DD48B..497, 006DD5AC..5B7, 006DD6CC..6D8, 006DD732..73E).
void* movies_table() { return at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xB8), 0x11C); }
kotor_menu_10_19::Tlk* tlk() { return static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08); }
gui_load::Object* panel_manager(const void* panel) {                                   // [panel+18h]
    gui_load::Object* m; std::memcpy(&m, static_cast<const unsigned char*>(panel) + 0x18, 4); return m;
}

// Views of the ORIGINAL tables' called slots (types exactly as gui_vtables.cpp declares them).
struct ButtonCallView {            // 0073E658
    void* s00;
    void (__fastcall* set_extent)(gui_load::Object*, void*, const gui_load::Extent*);           // +04 00417A20
    void* s08_94[0x24];
    void (__fastcall* initialize98)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +98 00418D80
};
static_assert(offsetof(ButtonCallView, initialize98) == 0x98, "button +98");
struct ControlSlot58View {         // control-family tables (button 0073E658 +58 = 00641DB0 self_return)
    void* s00_54[0x16];
    gui_load::Object* (__fastcall* slot58)(gui_load::Object*, void*);                           // +58
};
static_assert(offsetof(ControlSlot58View, slot58) == 0x58, "control +58");

// Raw storage for a CExoString produced into an unconstructed return slot (005E5D10 / 005E6270 outputs).
struct Slot { alignas(StartupString) unsigned char bytes[sizeof(StartupString)]; StartupString* p() { return reinterpret_cast<StartupString*>(bytes); } };

// Local CExoArrayList<CExoString> {data,count,capacity} filled by GetDirectoryList; released like the original
// epilogue 006DD8A2..006DD8D3: eh vector destructor iterator(data, 8, [data-4], 005E5C20), free(data-4).
struct StringList {
    StartupString* data = nullptr; std::int32_t size = 0; std::int32_t capacity = 0;
    ~StringList() {
        if (data) {
            unsigned char* const block = reinterpret_cast<unsigned char*>(data) - 4;
            std::int32_t count; std::memcpy(&count, block, 4);
            c2da_destroy_array_006fa852(data, 8, count, &kotor_movie::string_element_destroy_005e5c20);
            live_free(block);
        }
        data = nullptr;                                                               // 006DD8DA
    }
};
// Local CExoArrayList<CSWGuiControl*> [10]/[14]/[18]; data freed at 006DD890..89D.
struct ControlList {
    gui_load::Control** data = nullptr; std::int32_t size = 0; std::int32_t capacity = 0;
    ~ControlList() { live_free(data); data = nullptr; }                               // 006DD894..8AB
    // Inlined CExoArrayList<T*> growth (006DD383..3DD, 006DD78F..7F4): capacity*2, or 10h when 0; copy; free old.
    void grow() {
        const std::int32_t grown = capacity != 0 ? capacity * 2 : 0x10;
        gui_load::Control** const previous = data;
        capacity = grown;
        data = static_cast<gui_load::Control**>(live_operator_new(static_cast<std::uint32_t>(grown) * 4u));
        for (std::int32_t k = 0; k < size; ++k) data[k] = previous[k];
        if (previous != nullptr) live_free(previous);
    }
};
static_assert(sizeof(StringList) == 12 && sizeof(ControlList) == 12, "CExoArrayList storage");

// Pinned PE literals.
const char titlemovie_007581d8[] = "titlemovie";   // .rdata 007581D8 (layout resref)
const char lb_movies_007581cc[] = "LB_MOVIES";      // .rdata 007581CC
const char lbl_title_00751a20[] = "LBL_TITLE";      // .rdata 00751A20
const char btn_back_007548ac[] = "BTN_BACK";        // .rdata 007548AC
const char movies_alias_007581c4[] = "MOVIES:";     // .rdata 007581C4
const char live_format_0074e9ac[] = "live%d";       // .rdata 0074E9AC
const char movies_suffix_007581bc[] = ":movies";    // .rdata 007581BC
const char always_show_007581b0[] = "AlwaysShow";   // .rdata 007581B0
const char strref_name_007581a4[] = "StrrefName";   // .rdata 007581A4
const char order_0075819c[] = "Order";              // .rdata 0075819C
const char credits_0074eddc[] = "credits";          // .rdata 0074EDDC
constexpr gui_load::Word kMovieResType = 0x80F;     // 006DD192 / 006DD281 (bik)
constexpr gui_load::Word kCreditsStrref = 0xBB2E;   // 006DD310

gui_load::EventFunc movie_picked() {
    return static_cast<gui_load::EventFunc>(&kotor_m1_movies::TitleMoviesEvents::OnMoviePicked_006dcf10);  // 6DCF10h
}

// CSWGuiMainMenu handler guard (identical in 0067AFCB.. / 0067B1B8.. / 0067B268.. / 0067B308..; transcribed from
// the reviewed trap unit m48t-main-traps-mainmenu): true when the handler returns without effect.
bool guard_returns(const void* menu, const void* control) {
    if ((word_at(menu, 0x44) & 0x600u) == 0x400u) return true;   // 0067B268..75 [esi+44h] & 600h == 400h -> je 0067B2DB
    if (word_at(menu, 0x140C) == 0) return true;                  // 0067B277..7F [esi+140Ch] == 0 -> je 0067B2DB
    return word_at(control, 0x4C) == 0;                           // 0067B281..8A [arg+4Ch] == 0 -> je 0067B2DB
}

// CSWGuiTitleMovies+648 CExoArrayList<CExoString> release (006DD052..006DD084): eh vector destructor iterator
// (data, 8, [data-4], 005E5C20), free(data-4); data word zeroed afterwards.
void release_names(TitleMovies* self) {
    if (StartupString* const data = self->names.data) {
        unsigned char* const block = reinterpret_cast<unsigned char*>(data) - 4;     // 006DD061..064
        std::int32_t count; std::memcpy(&count, block, 4);
        c2da_destroy_array_006fa852(data, 8, count, &kotor_movie::string_element_destroy_005e5c20);   // 006DD067..070
        live_free(block);                                                            // 006DD075..07B
    }
    self->names.data = nullptr;                                                      // 006DD084
}

// Constructed-member stages of 006DD910 (EH states 0..4 of its FuncInfo at 00736715).
enum Stage { kPanel = 0, kListBox, kLabel, kButton, kNames };
void unwind_title_movies(TitleMovies* self, int built) {
    if (built >= kNames) release_names(self);
    if (built >= kButton) gui_load_button_destruct_00419da0(&self->btn_back, nullptr);
    if (built >= kLabel) gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);
    if (built >= kListBox) gui_load_listbox_destruct_0041b060(&self->lb_movies, nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}

// new(1C4h) + inlined CSWGuiButton ctor (006DD515..577; same sequence as 0041C0F0: Navigable, vptr 0073E658,
// border +6C, hilight +E0, text +154), unwinding EH states 0Ch..0Fh (free memory on a throwing member ctor).
gui_load::Button* new_inline_button() {
    void* const memory = live_operator_new(0x1C4);                                   // 006DD515..51A
    if (memory == nullptr) return nullptr;                                           // 006DD528..532 -> 006DD579 esi = 0
    gui_load::Button* const button = static_cast<gui_load::Button*>(memory);
    try {
        gui_load_navigable_construct_0041b8b0(button, nullptr);                      // 006DD536
    } catch (...) { live_free(memory); throw; }
    button->vtbl = reinterpret_cast<const ObjectVtbl*>(&gui_load_button_methods_0073e658);  // 006DD546
    try {
        gui_load_border_construct_004167f0(&button->border, nullptr);                // 006DD54C (+6C)
        try {
            gui_load_border_construct_004167f0(&button->hilight, nullptr);           // 006DD55F (+E0)
            try {
                gui_load_text_construct_00417280(&button->text, nullptr);            // 006DD572 (+154)
            } catch (...) { gui_load_border_destruct_004151c0(&button->hilight, nullptr); throw; }
        } catch (...) { gui_load_border_destruct_004151c0(&button->border, nullptr); throw; }
    } catch (...) { gui_load_navigable_destruct_00419ce0(button, nullptr); live_free(memory); throw; }
    return button;
}

// SetText(button text params +16C, GetSimpleString(strref)): the fetched CExoString lives in a temporary
// that is destroyed after SetText (006DD310..345 credits).
void set_text_from_tlk(gui_load::Button* button, gui_load::Word strref) {
    alignas(4) unsigned char bytes[8];
    kotor_menu_10_19::String* const fetched =
        tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(bytes), strref);
    try {
        gui_load_text_params_set_text_00415e00(reinterpret_cast<unsigned char*>(button) + 0x16C, nullptr,
                                               reinterpret_cast<String*>(fetched));
    } catch (...) { reinterpret_cast<String*>(bytes)->~String(); throw; }
    reinterpret_cast<String*>(bytes)->~String();
}
}

// ================================================================ 0067B250 CSWGuiMainMenu::OnMoviesPicked(CSWGuiControl*) RET 4
// Replaces the M49 trap body (TRAPS #73 at 0067B28C) of unit m48t-main-traps-mainmenu; guard unchanged.
void __thiscall gui_load::MainMenuHandlers::OnMoviesPicked(kotor_input_hover::Control* control) {
    if (guard_returns(this, control)) return;                                        // 0067B268..8A -> 0067B2DB
    kotor_fastcall_005ee120(client_app(), nullptr);                                  // 0067B28C..95 BusyWaitMenuMusic
    void* const memory = live_operator_new(0x654);                                   // 0067B29A..A4
    TitleMovies* panel = nullptr;                                                    // 0067B2C4 xor eax,eax when NULL
    if (memory != nullptr) {                                                         // 0067B2AB..B5 (EH state 0: free on unwind)
        try {
            panel = kotor_fastcall_006dd910(static_cast<TitleMovies*>(memory), nullptr, panel_manager(this));  // 0067B2B7..BD
        } catch (...) { live_free(memory); throw; }
    }
    kotor_fastcall_0040bc70(panel_manager(this), nullptr, panel, 3, 1);              // 0067B2C6..D6 AddPanel(panel, 3, 1)
}

extern "C" {
// ================================================================ 006DD910 CSWGuiTitleMovies::CSWGuiTitleMovies(manager) RET 4
TitleMovies* __fastcall kotor_fastcall_006dd910(TitleMovies* self, void*, gui_load::Object* manager) noexcept(false) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                       // 006DD932..937
    int built = kPanel;
    try {
        self->vtbl = reinterpret_cast<const ObjectVtbl*>(&kotor_m1_title_movies_methods_00758130);  // 006DD949 vptr 00758130
        gui_load_listbox_construct_0041be40(&self->lb_movies, nullptr);              // 006DD93C..94F (+64)
        built = kListBox;
        gui_load_label_construct_0041acd0(&self->lbl_title, nullptr);                // 006DD954..961 (+344)
        built = kLabel;
        gui_load_button_construct_0041c0f0(&self->btn_back, nullptr);                // 006DD966..973 (+484)
        built = kButton;
        self->names.size = 0;                                                        // 006DD97A (+64C)
        self->names.allocated = 0;                                                   // 006DD980 (+650)
        self->names.data = nullptr;                                                  // 006DD986 (+648)
        built = kNames;
        {
            gui_load::ResRef layout;                                                 // [esp+14h], no destructor
            layout.construct_00406d80(titlemovie_007581d8);                          // 006DD98C..99A
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);            // 006DD99F..9A6
        }
        {
            String name(lb_movies_007581cc);                                         // 006DD9AB..9B4 (state 5)
            gui_load_panel_init_control_0040b930(self, nullptr, &self->lb_movies, &name, 1);  // 006DD9B9..9C8
        }                                                                            // 006DD9D6
        {
            String name(lbl_title_00751a20);                                         // 006DD9DB..9E4 (state 6)
            gui_load_panel_init_control_0040b930(self, nullptr, &self->lbl_title, &name, 1);  // 006DD9E9..9F8
        }                                                                            // 006DDA06
        {
            String name(btn_back_007548ac);                                          // 006DDA0B..A14 (state 7)
            gui_load_panel_init_control_0040b930(self, nullptr, &self->btn_back, &name, 1);   // 006DDA19..A28
        }                                                                            // 006DDA36
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                          // 006DDA3B..A3D
        // 006DDA42..4A: proto item = LB_MOVIES +2CC (panel +330), vslot +58 (no null check on the proto itself).
        gui_load::Control* const proto = self->lb_movies.proto;
        gui_load::Object* const item = vt<ControlSlot58View>(proto)->slot58(proto, nullptr);
        if (item != nullptr) {                                                       // 006DDA4D..4F
            unsigned char* const bytes = reinterpret_cast<unsigned char*>(item);
            std::memcpy(bytes + 0x90, borrowed_exit_color_007a23b4.words, 12);       // 006DDA51..6E border color (+6C+14+10)
            std::memcpy(bytes + 0x104, borrowed_enter_color_007a23c0.words, 12);     // 006DDA71..92 hilight color (+E0+14+10)
            gui_load_border_params_set_pulsing_00414d00(bytes + 0xF4, nullptr, 1, 1, 0);  // 006DDA8E..A9D hilight params
        }
        self->btn_back.flags &= 0xFFFFFFFBu;                                         // 006DDAA2..AE ([+484+44h] &= ~4)
        gui_load_control_add_event_0041ab20(&self->btn_back, nullptr, 0x27, event_target(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0)); // 006DDAA5..AB5 (624BB0h)
        kotor_fastcall_006dd100(self, nullptr);                                      // 006DDABA..ABC PopulateMovies
        gui_load_panel_set_active_control_0040a630(self, nullptr, &self->lb_movies, 0);  // 006DDAC1..AC6
    } catch (...) {
        unwind_title_movies(self, built);
        throw;
    }
    return self;                                                                     // 006DDAD0 eax = this; RET 4
}

// ================================================================ 006DCFE0 CSWGuiTitleMovies::~CSWGuiTitleMovies() RET
void __fastcall kotor_fastcall_006dcfe0(TitleMovies* self, void*) {
    self->vtbl = reinterpret_cast<const ObjectVtbl*>(&kotor_m1_title_movies_methods_00758130);  // 006DD001
    // 006DD007..04F: for each list item (count [+304] = list +2A0, re-read each pass): GetControl(i) vslot +58,
    // then the movie-name CExoString* at [item+58h]: ~CExoString, free (credits item holds 0).
    for (std::int32_t i = 0; i < self->lb_movies.items.size; ++i) {
        gui_load::Control* const control = kotor_fastcall_0041a5a0(&self->lb_movies, nullptr, i);   // 006DD020..023
        gui_load::Object* const item = vt<ControlSlot58View>(control)->slot58(control, nullptr);  // 006DD028..02C
        StartupString* name;
        std::memcpy(&name, reinterpret_cast<unsigned char*>(item) + 0x58, 4);       // 006DD02F
        if (name != nullptr) {                                                       // 006DD032..034
            name->~StartupString();                                                  // 006DD036..038
            live_free(name);                                                         // 006DD03D..043
        }
    }
    release_names(self);                                                             // 006DD052..084 (state 3)
    gui_load_button_destruct_00419da0(&self->btn_back, nullptr);                     // 006DD07E..093 (+484)
    // 006DD098..0CC inlines the 00419D10 sequence on +344: vptr 0073E5B8, ~Text(+D0), ~Border(+5C), ~Control.
    gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);
    gui_load_listbox_destruct_0041b060(&self->lb_movies, nullptr);                   // 006DD0D1..0D9 (+64)
    gui_load_panel_destruct_0040cf70(self, nullptr);                                 // 006DD0DE..0E8
}

// ================================================================ 006DDAF0 scalar deleting destructor (flags) RET 4
void* __fastcall kotor_fastcall_006ddaf0(TitleMovies* self, void*, gui_load::Word flags) {
    kotor_fastcall_006dcfe0(self, nullptr);                                          // 006DDAF1..AF3
    if (flags & 1u) live_free(self);                                                 // 006DDAF8..B05 (test byte [esp+8],1)
    return self;                                                                     // 006DDB08
}

// ================================================================ 006DCE80 CSWGuiTitleMovies::HandleInputEvent(event, value) RET 8
void __fastcall kotor_fastcall_006dce80(gui_load::Object* object, void*, gui_load::I32 event, gui_load::I32 value) {
    TitleMovies* const self = static_cast<TitleMovies*>(static_cast<gui_load::Panel*>(object));
    if (value != 0) {                                                                // 006DCE8B..8F
        if (event == 0x28 || event == 0x2E) {                                        // 006DCE91..9E (28h; 2Dh+1 = 2Eh)
            kotor_m1_client_set_start_menu_music_on_movie_end_005ed9e0(client_app(), nullptr, 1);       // 006DCEA0..AA
            kotor_m1_client_start_menu_music_005ed9b0(client_app(), nullptr, 1, 0x7F, 2);               // 006DCEAF..BE (re-read)
            reinterpret_cast<kih::Manager*>(panel_manager(self))->PlayGuiSound(0);   // 006DCEC3..C8
            kotor_fastcall_0040be00(panel_manager(self), nullptr);                   // 006DCECD..D0 PopModalPanel
            self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                      // 006DCED5..E4
        } else if (event == 0x2D) {                                                  // 006DCE98..9B
            reinterpret_cast<kih::Manager*>(panel_manager(self))->PlayGuiSound(0);   // 006DCEF6..FB
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, value);              // 006DCEE7..EB / 006DCF00..04
}

// ================================================================ 006DD100 CSWGuiTitleMovies::PopulateMovies() RET
void __fastcall kotor_fastcall_006dd100(TitleMovies* self, void*) noexcept(false) {
    StartupString name;                    // [1C] 006DD127..12F (EH state 0)
    StringList dirs;                       // [54]/[58]/[5C] zeroed 006DD13D..145 (state 1)
    ControlList controls;                  // [10]/[14]/[18] zeroed 006DD149..151 (state 2)
    gui_load::Extent extent;               // [6C..78] = list box inner extent (panel +2F0 = list +28C) 006DD155..187
    std::memcpy(&extent, &self->lb_movies.inner, sizeof extent);
    std::int32_t order_new = 0;            // [4C] 006DD183 (zeroed once; written by every GetINTEntry below)
    {
        StartupString alias(movies_alias_007581c4);                                  // [24] 006DD172..18B "MOVIES:"
        f200_ports::Enumerate005e6640(kotor_menu_10_19::dependencies::base_007a39e0, nullptr, &dirs, &alias,
                                      kMovieResType, 0, 0);                          // 006DD190..1AF
    }                                                                                // 006DD1C0
    StartupString path;                                                              // [30] 006DD1C5..1C9 (state 4)
    for (std::uint32_t live = 1; live < kotor_rdata_live_count_0073ecd2; ++live) {  // 006DD1CE..1E2 / 006DD29B..2A5 (byte)
        kotor_reconstruction::StartupFormat005e5680(&path, live_format_0074e9ac, live);  // 006DD1F0..1FB "live%d"
        void* const alias_list = at<void*>(kotor_menu_10_19::dependencies::base_007a39e0, 0x0C);  // 006DD200..206
        StartupString* const resolved = kotor_fastcall_005e6890(alias_list, nullptr, &path, 0);   // 006DD20C..212
        if (resolved->BorrowedLength005e5790() <= 0) continue;                       // 006DD217..220 (signed jle)
        {
            StartupString suffix(movies_suffix_007581bc);                             // [24] 006DD222..22B ":movies"
            Slot sum;                                                                // [38]
            StartupString* const joined = GuiConcat005e5d10(&path, nullptr, sum.p(), &suffix);  // 006DD230..246
            path.Assign005e5c50(joined);                                             // 006DD24B..258
            joined->~StartupString();                                                // 006DD269
        }                                                                            // 006DD27A
        f200_ports::Enumerate005e6640(kotor_menu_10_19::dependencies::base_007a39e0, nullptr, &dirs, &path,
                                      kMovieResType, 0, 0);                          // 006DD27F..296
    }
    const std::int32_t count = dirs.size;                                            // [48] 006DD2AB..2B5
    gui_load::Control* const proto = self->lb_movies.proto;                          // [7C] 006DD2AF..2BD (+330)
    StartupString display;                                                           // [64] 006DD2B9..2C1 (state 7)
    std::int32_t always_show = 0;                                                    // [2C] (holds the credits button's
                                                                                     // allocation 006DD2DB; every read is
                                                                                     // preceded by GetINTEntry's write)
    // ---- the "credits" entry (index 0; [item+58h] = 0 selects StartCreditSequence in OnMoviePicked)
    {
        void* const memory = live_operator_new(0x1C4);                               // 006DD2C6..2D3
        gui_load::Button* button = nullptr;                                          // 006DD2F6 esi = 0 when NULL (then [esi] read)
        if (memory != nullptr) {                                                     // 006DD2DF..2E9 (state 8)
            try {
                button = gui_load_button_construct_0041c0f0(static_cast<gui_load::Button*>(memory), nullptr);  // 006DD2EB..2ED
            } catch (...) { live_free(memory); throw; }
        }
        always_show = static_cast<std::int32_t>(word_of(memory));                    // 006DD2DB [2C] = new() result
        vt<ButtonCallView>(button)->initialize98(button, nullptr, word_of(&extent), word_of(proto));  // 006DD2F8..30A vslot +98
        set_text_from_tlk(button, kCreditsStrref);                                   // 006DD310..345 (strref BB2Eh)
        at<gui_load::Word>(button, 0x58) = 0;                                        // 006DD34A
        extent.height = 0x38;                                                        // 006DD351 [78] = 38h
        vt<ButtonCallView>(button)->set_extent(button, nullptr, &extent);            // 006DD359..35E vslot +04
        gui_load_control_add_event_0041ab20(button, nullptr, 0x27, event_target(self), movie_picked());  // 006DD361..36B
        gui_load_control_add_event_0041ab20(button, nullptr, 0x2D, event_target(self), movie_picked());  // 006DD370..37A
        if (controls.size == controls.capacity) controls.grow();                     // 006DD37F..3DD
        controls.data[controls.size] = button;                                       // 006DD3E0..3E8
        ++controls.size;                                                             // 006DD3EB..402
        directory_list::Add004059a0(names_view(self), nullptr, StartupString(credits_0074eddc));  // 006DD3EF..412 (arg built in place)
    }
    for (std::int32_t i = 0; i < count; ++i) {                                       // [60] 006DD412..41E / 006DD84C..857
        name.Assign005e5c50(&dirs.data[i]);                                          // 006DD424..430
        {
            const std::int32_t length = name.BorrowedLength005e5790() - 4;           // 006DD435..43E (drop ".bik")
            Slot cut;                                                                // [A0]
            StartupString* const r = startup_rand::SubString005e6270(&name, nullptr, cut.p(), 0, length);  // 006DD441..44F
            name.Assign005e5c50(r);                                                  // 006DD454..461
            r->~StartupString();                                                     // 006DD466..475
        }
        {
            StartupString column(always_show_007581b0);                               // [88] 006DD47A..486 "AlwaysShow"
            kotor_fastcall_00414110(movies_table(), nullptr, &name, &column, &always_show);  // 006DD48B..4B9
        }                                                                            // 006DD4CD
        if (always_show != 1) {                                                      // 006DD4D2..4DD
            menu_resources::Ref movie;                                               // [A8] (no destructor)
            async_ref_construct_string_00406d60(&movie, nullptr, &name);             // 006DD4DF..4EB CResRef(CExoString const&)
            void* const options = kotor_fastcall_005ed700(client_app(), nullptr);    // 006DD4F0..501 GetClientOptions
            if (kotor_fastcall_0061d3d0(options, nullptr, &movie) == 0) continue;    // 006DD506..50F -> 006DD84C
        }
        gui_load::Button* const button = new_inline_button();                        // 006DD515..579
        vt<ButtonCallView>(button)->initialize98(button, nullptr, word_of(&extent), word_of(proto));  // 006DD57B..591 vslot +98
        std::int32_t strref = 0;                                                     // [40] 006DD5A3
        {
            StartupString column(strref_name_007581a4);                               // [80] 006DD597..5A7 "StrrefName"
            kotor_fastcall_00414110(movies_table(), nullptr, &name, &column, &strref);   // 006DD5AC..5D9
        }                                                                            // 006DD5ED
        if (strref != 0) {                                                           // 006DD5F2..5F8
            alignas(4) unsigned char bytes[8];                                       // [98]
            kotor_menu_10_19::String* const fetched = tlk()->simple_0041e8f0(
                reinterpret_cast<kotor_menu_10_19::String*>(bytes), static_cast<gui_load::Word>(strref));  // 006DD5FA..609
            try { display.Assign005e5c50(reinterpret_cast<StartupString*>(fetched)); }   // 006DD60E..61B
            catch (...) { reinterpret_cast<StartupString*>(bytes)->~StartupString(); throw; }
            reinterpret_cast<StartupString*>(bytes)->~StartupString();               // 006DD620..62F
        } else {
            display.Assign005e5c50(&name);                                           // 006DD636..63F
        }
        gui_load_text_params_set_text_00415e00(reinterpret_cast<unsigned char*>(button) + 0x16C, nullptr, &display);  // 006DD644..64F
        {
            void* const memory = live_operator_new(8);                               // 006DD654..65B
            StartupString* copy = nullptr;                                           // 006DD67C eax = 0 when NULL
            if (memory != nullptr) {                                                 // 006DD662..66C (state 12h)
                try {
                    copy = WindowCopy005e5b00(static_cast<String*>(memory), nullptr, &name);  // 006DD66E..675 (005E5B00)
                } catch (...) { live_free(memory); throw; }
            }
            at<StartupString*>(button, 0x58) = copy;                                 // 006DD67E
        }
        extent.height = 0x38;                                                        // 006DD685 [78] = 38h
        vt<ButtonCallView>(button)->set_extent(button, nullptr, &extent);            // 006DD681..69A vslot +04
        gui_load_control_add_event_0041ab20(button, nullptr, 0x27, event_target(self), movie_picked());  // 006DD69D..6A7
        gui_load_control_add_event_0041ab20(button, nullptr, 0x2D, event_target(self), movie_picked());  // 006DD6AC..6B6
        {
            StartupString column(order_0075819c);                                     // [90] 006DD6BB..6C7 "Order"
            kotor_fastcall_00414110(movies_table(), nullptr, &name, &column, &order_new);  // 006DD6CC..6FA
        }                                                                            // 006DD70E
        // 006DD4D6 edi = 1; 006DD713..789: first index (from 1) whose existing Order > this Order, else the end.
        std::int32_t index = 1;
        if (controls.size > 1) {                                                     // 006DD713..71A (signed jle)
            for (;;) {
                std::int32_t order_existing = 0;                                     // [50] 006DD729
                {
                    StartupString column(order_0075819c);                             // [38] 006DD720..72D
                    kotor_fastcall_00414110(movies_table(), nullptr, &self->names.data[index], &column,
                                            &order_existing);                        // 006DD732..762 (row = names[edi])
                }                                                                    // 006DD773
                if (order_existing > order_new) break;                               // 006DD778..784 (signed jg)
                ++index;                                                             // 006DD786
                if (index >= controls.size) break;                                   // 006DD787..789
            }
        }
        // 006DD78B..833 inlined CExoArrayList<CSWGuiControl*>::Insert(button, index).
        const std::int32_t old_size = controls.size;                                 // ebp
        if (old_size == controls.capacity) controls.grow();                          // 006DD78F..7F4
        controls.data[old_size] = button;                                            // 006DD7F8..7FC
        controls.size = old_size + 1;                                                // 006DD7FF..806
        for (std::int32_t k = old_size; k > index; --k) controls.data[k] = controls.data[k - 1];  // 006DD80A..81E
        controls.data[index] = button;                                               // 006DD833
        directory_list::Insert005e8c60(names_view(self), nullptr, name, index);      // 006DD820..841 (005E5B00 copy in arg slot)
    }
    kotor_fastcall_0041c1d0(&self->lb_movies, nullptr,
        reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&controls), 1, 0, 0);   // 006DD85D..869 AddControls(list,1,0,0)
    // Destruction order (C++ reverse declaration) equals 006DD86E..006DD8E9: display [64], path [30], controls data
    // free, GetDirectoryList array, name [1C].
}

// ================================================================ 00758130 CSWGuiTitleMovies method table (27 slots)
// .rdata 00758130: +00 006DDAF0, +3C 006DCE80; all other slots equal CSWGuiPanel's 0073E010 (dumped from the PE).
struct KotorM1TitleMoviesMethods {
    void* (__fastcall* s00)(TitleMovies*, void*, gui_load::Word);  // +00 006DDAF0 CSWGuiTitleMovies::~CSWGuiTitleMovies
    void (__fastcall* s04)(gui_load::Object*, void*, const gui_load::Extent*);  // +04 0040A5A0 CSWGuiPanel::SetExtent
    void (__fastcall* s08)(gui_load::Object*, void*, gui_load::Object*, gui_load::Word);  // +08 0040A630 SetActiveControl
    void (__fastcall* s0c)(gui_load::Object*, void*, gui_load::Word);  // +0C 0060E760 void_return [leaf RET 4]
    gui_load::Word (__fastcall* s10)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +10 00409EB0 [RET 8]
    void (__fastcall* s14)(gui_load::Object*, void*);  // +14 004015A0 void_return
    void (__fastcall* s18)(gui_load::Object*, void*);  // +18 004015A0
    void (__fastcall* s1c)(gui_load::Object*, void*);  // +1C 004015A0
    void (__fastcall* s20)(gui_load::Object*, void*);  // +20 004015A0
    void (__fastcall* s24)(gui_load::Object*, void*);  // +24 004015A0
    gui_load::Object* (__fastcall* s28)(gui_load::Object*, void*);  // +28 00641DB0 self_return
    gui_load::Word (__fastcall* s2c)(gui_load::Object*, void*);  // +2C 0063E7F0 return_zero
    gui_load::Word (__fastcall* s30)(gui_load::Object*, void*);  // +30 0063E7F0 return_zero
    void (__fastcall* s34)(gui_load::Panel*, void*, gui_load::Word);  // +34 0040B760 CSWGuiPanel::Draw
    void (__fastcall* s38)(gui_load::Object*, void*, gui_load::Word);  // +38 0060E760 void_return [RET 4]
    void (__fastcall* s3c)(gui_load::Object*, void*, gui_load::I32, gui_load::I32);  // +3C 006DCE80 HandleInputEvent
    gui_load::Object* (__fastcall* s40)(gui_load::Object*, void*, gui_load::I32, gui_load::I32);  // +40 0040B690 HitCheckMouse
    gui_load::Object* (__fastcall* s44)(gui_load::Object*, void*);  // +44 0040B630 GetActiveControl
    void (__fastcall* s48)(gui_load::Panel*, void*);  // +48 0040B870 CSWGuiPanel::OnPanelAdded
    void (__fastcall* s4c)(gui_load::Panel*, void*);  // +4C 0040C170 CSWGuiPanel::OnPanelRemoved
    void (__fastcall* s50)(gui_load::Object*, void*, gui_load::Word);  // +50 0040B640 OnAButtonPressed
    void (__fastcall* s54)(gui_load::Object*, void*, gui_load::Word);  // +54 0040B650 OnBButtonPressed
    void (__fastcall* s58)(gui_load::Object*, void*, gui_load::Word);  // +58 0040B660 OnXButtonPressed
    void (__fastcall* s5c)(gui_load::Object*, void*, gui_load::Word);  // +5C 0040B670 OnYButtonPressed
    void (__fastcall* s60)(gui_load::Object*, void*, gui_load::Word);  // +60 0040B680 OnBlackButtonPressed
    gui_load::String* (__fastcall* s64)(gui_load::Panel*, void*, gui_load::String*);  // +64 0040A900 GetFullScreenBG
    void (__fastcall* s68)(gui_load::Panel*, void*);  // +68 0040A9C0 CSWGuiPanel::ResetFont
};
static_assert(sizeof(KotorM1TitleMoviesMethods) == 108, "00758130 slot count");
extern const KotorM1TitleMoviesMethods kotor_m1_title_movies_methods_00758130 = {
    &kotor_fastcall_006ddaf0,  // +00
    &gui_load_panel_set_extent_0040a5a0,  // +04
    &gui_load_panel_set_active_control_0040a630,  // +08
    &gui_load_slot_ret4_0060e760,  // +0C
    &gui_load_slot_zero8_00409eb0,  // +10
    &gui_load_slot_ret_004015a0,  // +14
    &gui_load_slot_ret_004015a0,  // +18
    &gui_load_slot_ret_004015a0,  // +1C
    &gui_load_slot_ret_004015a0,  // +20
    &gui_load_slot_ret_004015a0,  // +24
    &gui_load_slot_self_00641db0,  // +28
    &gui_load_slot_zero_0063e7f0,  // +2C
    &gui_load_slot_zero_0063e7f0,  // +30
    &kotor_panel_draw_0040B760,  // +34
    &gui_load_slot_ret4_0060e760,  // +38
    &kotor_fastcall_006dce80,  // +3C
    &gui_load_panel_hit_check_0040b690,  // +40
    &gui_load_panel_active_control_0040b630,  // +44
    &gui_load_panel_on_added_0040b870,  // +48
    &gui_load_panel_on_removed_0040c170,  // +4C
    &gui_load_panel_button_a_0040b640,  // +50
    &gui_load_panel_button_b_0040b650,  // +54
    &gui_load_panel_button_x_0040b660,  // +58
    &gui_load_panel_button_y_0040b670,  // +5C
    &gui_load_panel_button_black_0040b680,  // +60
    &gui_load_panel_full_screen_bg_0040a900,  // +64
    &gui_load_panel_reset_font_0040a9c0,  // +68
};
} // extern "C"

// ================================================================ 006DCF10 CSWGuiTitleMovies::OnMoviePicked(CSWGuiControl*) RET 4
// lane-m1-movies-play: full body (lifts the D83 branch trap 006DCF37 of lane-m1-movies TRAPS-DELTA).
//   named entry ([item+58h] = the entry's CExoString*, PopulateMovies 006DD60D..62C): CClientExoApp::PlayMovie
//     005EDB30 (-> 00602CF0 -> AddMovieToExoArrayList 005FBBF0 + PlayMoviesInExoArrayList 00602AF0, wait = 1);
//   credits entry ([item+58h] == 0, PopulateMovies 006DD2FF..30D): CExoString("credits") + StartCreditSequence
//     005EE1C0 (-> 005F4C60) with a zeroed 12-byte local.
void __thiscall kotor_m1_movies::TitleMoviesEvents::OnMoviePicked_006dcf10(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                         // 006DCF16..31 [arg+4Ch] == 0 -> 006DCFCA
    gui_load::Object* const source = reinterpret_cast<gui_load::Object*>(control);    // same object (event source button)
    gui_load::Object* const item = vt<ControlSlot58View>(source)->slot58(source, nullptr);   // 006DCF37..39 vslot +58
    StartupString* name;
    std::memcpy(&name, reinterpret_cast<unsigned char*>(item) + 0x58, 4);           // 006DCF3C [eax+58h]
    if (name != nullptr) {                                                           // 006DCF3F..41
        // 006DCF4D..52: the original passes the address of its own (callee-owned) argument slot [esp+2Ch] as the
        // int* out-parameter after storing 0 into it; a local word is the same storage role (never read afterwards).
        std::int32_t out = 0;                                                        // 006DCF52
        (void)kotor_fastcall_005edb30(client_app(), nullptr, name, &out, 0, 1);      // 006DCF43..5E push 1, 0, &out, name
        return;                                                                      // 006DCF63..71
    }
    StartupString credits(credits_0074eddc);                                         // 006DCF74..7D CExoString("credits") (EH state 0)
    std::uint32_t callback[3] = {0u, 0u, 0u};                                        // 006DCF87..97 [esp+8..10h] = 0
    kotor_fastcall_005ee1c0(client_app(), nullptr, callback, &credits);              // 006DCF82..B4 push &credits, &callback
}                                                                                    // 006DCFB9..C5 ~CExoString (state -1)
