// lane-m48-b fork GUI: CSWGuiImage / CSWGuiListBox / CSWGuiScrollBar / control-family bodies that the M47
// gui-load units reference but no staged unit defines. Authored from the original swkotor.exe (GOG, sha256
// 9c10e045...91435); every behaviour cites its instruction addresses. Conventions as gui-load listbox.h:
// D1 __fastcall(Self*, void* unused_edx, ...) veneers for original __thiscall entries (same stack layout and
// callee cleanup); virtual calls read the receiver's vptr at each call site through typed slot views (D24 object
// view of the table, no function-pointer casts); raw binary32 words are carried as Word where the original only
// moves them.
#include "041b9ba49e-listbox.h"
#include "76b86a691c-gui_objects.h"
#include "412cc7cc49-mainmenu.h"
#include "064c4faf5d-m1_shared.h"     // lane-m1-options L0-shared: 00624BA0 single owner (M50 composer: stage-relative, unit header_roots unchanged)
#include <initializer_list>

using namespace gui_lb;
namespace kih = kotor_input_hover;

namespace kotor_renderer_cells { extern std::uint32_t pulse_0078d3d4; }   // gui-cells owner (0078D3D4 pulsing alpha)

// user32 import of the original (IAT 0073D3DC PostQuitMessage); declared as in <winuser.h>.
extern "C" __declspec(dllimport) void __stdcall PostQuitMessage(int);
extern "C" {
// core47-gui-manager-leaves: CSWGuiManager::ReleaseMouse 0040A200 (thiscall, 1 stack word, RET 4).
void __fastcall kotor_fastcall_0040a200(void* gui, void*, void* control);
}

namespace {
using Slot0 = I32 (__fastcall*)(Control*, void*);                 // no-argument control slots returning EAX
const Slot0* slots(const Control* c) { return vt<Slot0>(c); }      // table view; slot index = byte offset / 4
Control* item_at(const ListBox* self, I32 index) {
    return *reinterpret_cast<Control* const volatile*>(self->items.data + static_cast<Word>(index) * 4u);
}
const ControlCallVtbl* cv(const Control* object) { return vt<ControlCallVtbl>(object); }
void* gui_manager() { return kotor_input_cells::gui_manager_007a39f4; }
Word word_at(const void* p, unsigned off) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
void put_word(void* p, unsigned off, Word v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }

// CAurGUIImageInternal slot +18 Draw 00459920 (gui_render.h ImageDrawSlot: 8 stack DWORDs, RET 20h).
using ImageDrawSlot = void (__fastcall*)(AurObject*, void*, I32 x, I32 y, I32 w, I32 h, Word flags, Word angle_bits,
                                         const Vector* color, Word alpha_bits);
struct AurImageDrawView { void* s00_14[6]; ImageDrawSlot draw; };   // +18
static_assert(offsetof(AurImageDrawView, draw) == 0x18, "aur image draw slot");

// FLD m32 / FSTP m32 round trip of a binary32 word through x87 (00415CC7 / 00415CDB).
Word x87_round_trip(const Word* bits) {
    Word result;
    __asm {
        mov eax, bits
        fld dword ptr [eax]
        fstp dword ptr result
    }
    return result;
}
}

extern "C" {

// ---- 00415C90 CSWGuiImage::Draw(float) (thiscall, RET 4; the delta argument is not read) ----
void __fastcall gui_load_image_draw_00415c90(Image* self, void*, Word) {
    AurObject* image = self->gui_image;                                            // 00415C93
    if (!image || self->extent.width == 0 || self->extent.height == 0) return;     // 00415C96..A6 (+0C/+10)
    Word alpha = self->params.u14;                                                 // 00415CA8..AB ([+3C])
    const Word flags = self->params.flags;                                         // 00415CAF ([+4C])
    if (flags & 1u) alpha = kotor_renderer_cells::pulse_0078d3d4;                  // 00415CB2..BF
    const Word angle = x87_round_trip(&self->params.u10);                          // 00415CC7 / 00415CDB ([+38])
    const Word draw_flags = (flags >> 2) & 0xFFFFFF0Fu;                            // 00415CD2..D5
    const AurImageDrawView* t = vt<AurImageDrawView>(image);                       // 00415CCA
    t->draw(image, nullptr, self->draw.left, self->draw.top, self->draw.width, self->draw.height,
            draw_flags, angle, &self->params.color, alpha);                        // 00415CCC..EF (args +14,+18,+1C,+20, +40)
}

// ---- 0041A700 CSWGuiListBox::HandleLMouseUp (thiscall, RET) ----
void __fastcall gui_load_listbox_lmouse_up_0041a700(ListBox* self, void*) {
    I32 index;                                                                     // [esp+8]
    self->u88 = 0;                                                                 // 0041A70A
    Control* hit = gui_load_listbox_hit_local_0041a5e0(self, nullptr, &index);     // 0041A703..14
    if (hit && (self->flags & 8u)) {                                               // 0041A71B..23
        if (self->u6c != -1) {                                                     // 0041A725..2B
            if (self->u6c == index)                                                // 0041A72D..31
                cv(self)->handle_input_event(self, nullptr, 0x27, 1);              // 0041A733..3B
        } else {
            bool go = (self->lb_flags & 1u) != 0;                                  // 0041A740..47
            if (!go && self->items.size > 0) {                                     // 0041A749..51
                Control* first = item_at(self, 0);                                 // 0041A753..59
                go = slots(first)[0x60 / 4](first, nullptr) != 0;                  // 0041A75B..62
            }
            if (go && (static_cast<const unsigned char*>(gui_manager())[0x1C] & 1u)) {   // 0041A764..6D
                if (slots(hit)[0x6C / 4](hit, nullptr) == 0)                       // 0041A76F..78
                    cv(self)->handle_input_event(self, nullptr, 0x27, 1);          // 0041A77A..82
            }
        }
    }
    self->u6c = -1;                                                                // 0041A785
    kotor_fastcall_0040a200(gui_manager(), nullptr, self);                         // 0041A78C..93 ReleaseMouse(this)
    put_word(self->u70, 0, 0);                                                     // 0041A799 ([esi+70h] = 0)
}

// ---- 0041C400 CSWGuiListBox::HandleMouseMove (thiscall, RET) ----
// Joined to lane-input's consumer spelling ?HandleMouseMove@ListBox@kotor_input_hover@@QAEXXZ (m48_gui_joins.cpp).
void __fastcall gui_load_listbox_handle_mouse_move_0041c400(ListBox* self, void*) {
    if (!(self->flags & 8u)) return;                                               // 0041C404..08
    I32 index;                                                                     // [esp+8]
    Control* hit = gui_load_listbox_hit_local_0041a5e0(self, nullptr, &index);     // 0041C40F..14
    Control* last = self->u2dc;                                                    // 0041C419
    if (hit != last) {                                                             // 0041C421..23
        if (last) reinterpret_cast<kih::Control*>(last)->RemoveToolTip();          // 0041C425..29 (004176C0)
        self->u2dc = hit;                                                          // 0041C430
        if (!hit) return;                                                          // 0041C436
        static_cast<kih::Manager*>(gui_manager())->ResetToolTipTimer();            // 0041C438..3E (0040B4D0)
    }
    if (!hit) return;                                                              // 0041C443..45
    if (slots(hit)[0x6C / 4](hit, nullptr) != 0) return;                           // 0041C447..50
    if (self->lb_flags & 1u) return;                                               // 0041C452..59
    if (self->items.size > 0) {                                                    // 0041C45B..63
        Control* first = item_at(self, 0);                                         // 0041C465..6B
        if (slots(first)[0x60 / 4](first, nullptr) != 0) return;                   // 0041C46D..74
    }
    if (static_cast<I32>(self->selected) == index) return;                         // 0041C476..83 (MOVSX +2C6)
    gui_load_listbox_set_selected_0041c040(self, nullptr, index, 1);               // 0041C485..8A
}

}

// ======================================================================== CClientExoApp::ExitProgram family
extern "C" {
// ---- 005EEA10 CClientExoAppInternal::ExitProgram (thiscall, ECX unused): push 0; call [0073D3DC] PostQuitMessage; ret.
void __fastcall gui_client_internal_exit_program_005eea10(void*, void*) {
    PostQuitMessage(0);                                                            // 005EEA10..12
}
// ---- 005ED450 CClientExoApp::ExitProgram: ECX = [ECX+4] (internal); JMP 005EEA10.
void __fastcall gui_client_exit_program_005ed450(void* app, void*) {
    gui_client_internal_exit_program_005eea10(reinterpret_cast<void*>(word_at(app, 4)), nullptr);   // 005ED450..53
}
}

// ======================================================================== CSWGuiMainMenu input / quit
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }    // CAppManager* cell 007A39FC (M40 owner)
extern "C" {
// ---- 0067B380 CSWGuiMainMenu::HandleInputEvent(int event, int pressed) (thiscall, RET 8) ----
// Cross-checked against the task-4 menu-registry-r2 replay donor (menu_input.cpp; its binding table not reused, D76).
void __fastcall gui_load_mainmenu_handle_input_0067b380(gui_load::Object* self, void*, gui_load::I32 event,
                                                        gui_load::I32 pressed) {
    unsigned char* const m = reinterpret_cast<unsigned char*>(self);
    if (pressed != 0) {                                                            // 0067B38B..8F
        if (event == 0x28 || event == 0x2E) {                                      // 0067B395..A6
            if (word_at(m, 0x350) == 2u) {                                         // 0067B3A8..B5
                put_word(m, 0xA8, word_at(m, 0xA8) & 0xFFFFFFFDu);                 // 0067B3B7
                for (unsigned off : {0x3A4u, 0x5F8u, 0x7BCu, 0x434u, 0x980u, 0x10C8u, 0x128Cu, 0xE48u, 0xF88u})
                    put_word(m, off, word_at(m, off) | 2u);                        // 0067B3BE..0E (same order)
                self->vtbl->set_active_control(self, nullptr,
                    reinterpret_cast<gui_load::Object*>(m + 0x3F0), 0);            // 0067B40C..19 (vslot +08, btn_newgame)
                put_word(m, 0x350, 1u);                                            // 0067B41C
            }
        } else if (event == 0x2D) {                                                // 0067B39C..9F
            reinterpret_cast<kih::Manager*>(word_at(m, 0x18))->PlayGuiSound(0);    // 0067B435..3A (0040A140)
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);           // 0067B426..2A / 0067B43F..43
}
void __fastcall gui_client_exit_program_005ed450(void* app, void*);
}
// ---- 0067B4A0 CSWGuiMainMenu::OnQuitButtonPressed(CSWGuiControl*) (thiscall, RET 4; argument unused) ----
void gui_load::MainMenuHandlers::OnQuitButtonPressed(kih::Control*) {
    const unsigned char* const m = reinterpret_cast<const unsigned char*>(this);
    if (word_at(m, 0x140C) == 0) return;                                           // 0067B4A0..A8
    if (word_at(m, 0x350) == 2u) {                                                 // 0067B4AA..B1
        const gui_load::Object* self = reinterpret_cast<const gui_load::Object*>(this);
        self->vtbl->handle_input_event(const_cast<gui_load::Object*>(self), nullptr, 0x2E, 1);   // 0067B4B3..B9 (vslot +3C)
        return;
    }
    gui_client_exit_program_005ed450(reinterpret_cast<void*>(word_at(kotor_menu_window_cells::manager_007a39fc, 4)),
                                     nullptr);                                     // 0067B4BF..C8 ([[007A39FC]+4])
}
// ---- 0067B490 CSWGuiMainMenu::LaunchDashboard(CSWGuiControl*) (RET 4; ECX and argument unused) ----
void gui_load::MainMenuHandlers::LaunchDashboard(kih::Control*) {
    gui_client_exit_program_005ed450(reinterpret_cast<void*>(word_at(kotor_menu_window_cells::manager_007a39fc, 4)),
                                     nullptr);                                     // 0067B490..98
}

// ======================================================================== CSWGuiProgressBar (0x150) / CSWGuiLoadScreen
namespace gui_lb {
// CSWGuiProgressBar: CSWGuiControl (0x5C) + fields cited at ctor 0041AED0 / SetExtent 00419300.
struct ProgressBar : Control {
    I32 max_value;             // +5C = 0 (0041AF2E); SetMaxValue (00417FA0)
    I32 cur_value;             // +60 = 0 (0041AF35); FILD (00419378)
    Word pb_flags;             // +64 |= 1 (0041AF18..21); bit0 START FROM LEFT (00419390, 00417FE0)
    Border border;             // +68 (0041AF03) "BORDER" (0041BBDD)
    Border progress;           // +DC (0041AF13) "PROGRESS" (0041BC1C)
};
static_assert(offsetof(ProgressBar, border) == 0x68 && offsetof(ProgressBar, progress) == 0xDC &&
              sizeof(ProgressBar) == 0x150, "progress bar");
}
// .bss Vector at 00833910 (beyond the .data raw size 0x17000; no store anywhere in the image - only the pushes
// 0067AB1B / 0068AC71 / 006AEB02 and reads 006A8996..A9): loader-zero black viewport colour. No other owner exists;
// owned here (lane-m48-b GUI fork), zero-initialized storage, never written.
namespace kotor_gui_m48_cells { gui_load::Vector viewport_color_00833910{}; }
namespace kotor_original { void __fastcall kotor_fastcall_00417fb0(void* bar, void* unused_edx, std::int32_t value); } // startup46 r4_gui_leaves.cpp
extern "C" unsigned __int64 __cdecl unresolved_ftol2_006fae8c();   // original _ftol2 port 006FAE8C (D30-FINAL)
extern "C" {
extern const struct GuiM48ProgressBarMethods gui_m48_progress_methods_0073e7a0;
void* __fastcall gui_m48_progress_deleting_destruct_0041af50(ProgressBar*, void*, Word);
void __fastcall gui_m48_progress_set_extent_00419300(ProgressBar*, void*, const Extent*);
void __fastcall gui_m48_progress_draw_00417f60(ProgressBar*, void*, Word);
void __fastcall gui_m48_progress_load_0041bbc0(ProgressBar*, void*, Gff*, Struct*) noexcept(false);
void __fastcall gui_m48_progress_initialize_004192d0(ProgressBar*, void*, const Extent*, ProgressBar*);
void __fastcall gui_m48_progress_initialize_00417f10(ProgressBar*, void*, const Extent*, const void*, const void*);
void __fastcall gui_m48_progress_set_max_00417f90(ProgressBar*, void*, I32);
void __fastcall gui_m48_progress_set_start_from_left_00417fe0(ProgressBar*, void*, Word);
void* __fastcall gui_m48_load_screen_deleting_destruct_0067ab60(gui_load::Panel*, void*, Word);
void __fastcall gui_m48_load_screen_destruct_0067ab80(gui_load::Panel*, void*);
void __fastcall gui_m48_load_screen_draw_0067ab00(gui_load::Panel*, void*, Word);
// existing owners
void __fastcall gui_load_panel_destruct_0040cf70(gui_load::Panel*, void*);
void __fastcall gui_load_label_destruct_00419d10(gui_load::Label*, void*);
void __stdcall kotor_AurGUIStartLayer_004591B0();
void __stdcall kotor_AurGUIStopLayer_004592B0();
std::uint32_t __cdecl kotor_AurGUISetupViewport_004592F0(std::uint32_t x, std::uint32_t y, std::uint32_t width,
    std::uint32_t height, void* color, std::uint32_t depth_test_zero, std::uint32_t alpha_bits);
void __stdcall kotor_AurGUICloseViewport_00459580();
}
namespace {
using DeltaSlot = void (__fastcall*)(void* self, void* unused_edx, Word delta_bits);   // gui_render.h DeltaSlot
struct DrawableSlots { void* s00; void* s04; void* s08; DeltaSlot draw; };          // border/text +0C Draw
struct ControlDrawSlots { void* s00_34[14]; DeltaSlot draw; };                       // control/panel +38 Draw
static_assert(offsetof(ControlDrawSlots, draw) == 0x38, "control draw slot");
const gui_load::ObjectVtbl* ov(const void* object) { return vt<gui_load::ObjectVtbl>(static_cast<const Object*>(object)); }
// x87 sequence of 00419378..8B / 004193A8..AC: FILD cur; FIDIV max; FIMUL dim; CALL _ftol2 (006FAE8C).
I32 progress_scaled(const I32* cur, const I32* max, const I32* dim) {
    I32 converted;
    __asm {
        mov eax, cur
        mov ecx, max
        mov edx, dim
        fild dword ptr [eax]
        fidiv dword ptr [ecx]
        fimul dword ptr [edx]
        call unresolved_ftol2_006fae8c
        mov converted, eax
    }
    return converted;
}
}
namespace kotor_original {
// ---- 0041AED0 CSWGuiProgressBar::CSWGuiProgressBar (thiscall, RET; EH states 0..1) ----
void* __fastcall kotor_fastcall_0041aed0(void* bar, void*) {
    ProgressBar* self = static_cast<ProgressBar*>(bar);
    gui_load_control_construct_0041aa80(self, nullptr);                            // 0041AEED
    try {                                                                          // state 0: ~CSWGuiControl
        set_vptr(self, &gui_m48_progress_methods_0073e7a0);                       // 0041AEFD (73E7A0)
        gui_load_border_construct_004167f0(&self->border, nullptr);                // 0041AEF2..03
        try {                                                                      // state 1: ~CSWGuiBorder(+68)
            gui_load_border_construct_004167f0(&self->progress, nullptr);          // 0041AF08..13
        } catch (...) { gui_load_border_destruct_004151c0(&self->border, nullptr); throw; }
    } catch (...) { gui_load_control_destruct_00419a20(self, nullptr); throw; }
    const Word control_flags = self->flags;                                        // 0041AF1B
    self->pb_flags = self->pb_flags | 1u;                                          // 0041AF18..21
    self->flags = control_flags & 0xFFFFFFFBu;                                     // 0041AF28..2B
    self->max_value = 0;                                                           // 0041AF2E
    self->cur_value = 0;                                                           // 0041AF35
    return self;                                                                   // 0041AF3C
}
// ---- 0041AF70 CSWGuiProgressBar::~CSWGuiProgressBar (thiscall, RET) ----
void __fastcall kotor_fastcall_0041af70(void* bar, void*) {
    ProgressBar* self = static_cast<ProgressBar*>(bar);
    set_vptr(self, &gui_m48_progress_methods_0073e7a0);                           // 0041AF8D
    gui_load_border_destruct_004151c0(&self->progress, nullptr);                   // 0041AF93..A1
    gui_load_border_destruct_004151c0(&self->border, nullptr);                     // 0041AFA6..AE
    gui_load_control_destruct_00419a20(self, nullptr);                             // 0041AFB3..BD
}
}
extern "C" {
// ---- 0041AF50 scalar deleting destructor (RET 4): ~CSWGuiProgressBar; flags bit0 -> free 006FA390.
void* __fastcall gui_m48_progress_deleting_destruct_0041af50(ProgressBar* self, void*, Word flags) {
    kotor_original::kotor_fastcall_0041af70(self, nullptr);                        // 0041AF53
    if (flags & 1u) live_free(self);                                               // 0041AF58..60
    return self;                                                                   // 0041AF68
}
// ---- 00419300 CSWGuiProgressBar::SetExtent(CSWGuiExtent const*) (RET 4) ----
void __fastcall gui_m48_progress_set_extent_00419300(ProgressBar* self, void*, const Extent* extent) {
    self->extent = *extent;                                                        // 0041930D..27
    vt<BorderCallVtbl>(&self->border)->set_extent(&self->border, nullptr, &self->extent);   // 0041932A..43 (vslot +04)
    Extent got{0, 0, 0, 0};                                                        // 00419333..3F (zeroed locals)
    const Extent inner_copy = *gui_load_border_inner_extent_00415240(&self->border, nullptr, &got);   // 00419346..6E
    Extent inner = inner_copy;
    const I32 max = self->max_value;                                               // 00419369..72
    if (max > 0) {                                                                 // 0041936C..76
        if (self->extent.height > self->extent.width) {                            // 0041937B..85 (+10 vs +0C)
            const I32 v = progress_scaled(&self->cur_value, &max, &inner.height);  // 00419378..8B
            if (!(self->pb_flags & 1u)) inner.top += inner.height - v;             // 00419390..9E
            inner.height = v;                                                      // 004193A2
        } else {
            const I32 v = progress_scaled(&self->cur_value, &max, &inner.width);   // 004193A8..AC
            if (!(self->pb_flags & 1u)) inner.left += inner.width - v;             // 004193B1..BF
            inner.width = v;                                                       // 004193C3
        }
    }
    vt<BorderCallVtbl>(&self->progress)->set_extent(&self->progress, nullptr, &inner);      // 004193C7..D8 (vslot +04)
}
// ---- 00417F60 CSWGuiProgressBar::Draw(float) (RET 4): border +68 vslot +0C, progress +DC vslot +0C.
void __fastcall gui_m48_progress_draw_00417f60(ProgressBar* self, void*, Word delta) {
    vt<DrawableSlots>(&self->border)->draw(&self->border, nullptr, delta);         // 00417F68..6F
    vt<DrawableSlots>(&self->progress)->draw(&self->progress, nullptr, delta);     // 00417F72..7F
}
// ---- 00417F90 CSWGuiProgressBar::SetMaxValue(int) (RET 4): if changed and >= 0: max = v; tail vslot +04(&extent).
void __fastcall gui_m48_progress_set_max_00417f90(ProgressBar* self, void*, I32 value) {
    if (self->max_value == value || value < 0) return;                             // 00417F94..9B
    self->max_value = value;                                                       // 00417FA0
    ov(self)->set_extent(self, nullptr, &self->extent);                            // 00417F9D..A9 (JMP [eax+4])
}
// ---- 00417FE0 CSWGuiProgressBar::SetStartFromLeft(ulong) (RET 4): if bit0 != arg: bit0 = arg bit0; tail vslot +04.
void __fastcall gui_m48_progress_set_start_from_left_00417fe0(ProgressBar* self, void*, Word value) {
    const Word flags = self->pb_flags;                                             // 00417FE0
    if ((flags & 1u) == value) return;                                             // 00417FE8..EF (full-word compare)
    self->pb_flags = ((flags ^ value) & 1u) ^ flags;                               // 00417FF1..FC
    ov(self)->set_extent(self, nullptr, &self->extent);                            // 00417FFF..07 (JMP [eax+4])
}
// ---- 0041BBC0 CSWGuiProgressBar::Load(CResGFF*, CResStruct*) (RET 8; EH states for the two label strings) ----
void __fastcall gui_m48_progress_load_0041bbc0(ProgressBar* self, void*, Gff* gff, Struct* s) noexcept(false) {
    {
        String label("BORDER");                                                    // 0041BBDD..E6 (0073DFD4)
        vt<BorderCallVtbl>(&self->border)->load(&self->border, nullptr, gff, s, &label);       // 0041BBF3..08
    }                                                                              // 0041BC17
    {
        String label("PROGRESS");                                                  // 0041BC1C..25 (0073EA98)
        vt<BorderCallVtbl>(&self->progress)->load(&self->progress, nullptr, gff, s, &label);   // 0041BC2A..45
    }                                                                              // 0041BC54
    // The original passes the address of the destroyed first label's slot as the success out word
    // (0041BC5B/0041BC77/0041BC93); the written value is never read. A dedicated local word is used here.
    Word success = 0;
    gui_m48_progress_set_max_00417f90(self, nullptr,
        gui_load_gff_int_00411c90(gff, nullptr, s, "MAXVALUE", &success, 0));      // 0041BC59..70 (0073EA8C)
    kotor_original::kotor_fastcall_00417fb0(self, nullptr,
        gui_load_gff_int_00411c90(gff, nullptr, s, "CURVALUE", &success, 0));      // 0041BC75..8C (0073EA80)
    gui_m48_progress_set_start_from_left_00417fe0(self, nullptr,
        gui_load_gff_byte_00411a60(gff, nullptr, s, "STARTFROMLEFT", &success, 1));  // 0041BC91..AB (0073EA70; MOVZX)
    gui_load_control_load_0041b8e0(self, nullptr, gff, s);                         // 0041BCB0..B4
}
// ---- 004192D0 CSWGuiProgressBar::Initialize(extent, CSWGuiProgressBar* source) (RET 8):
// vslot +9C(extent, &source->border.params (+7C), &source->progress.params (+F0)).
void __fastcall gui_m48_progress_initialize_004192d0(ProgressBar* self, void*, const Extent* extent, ProgressBar* source) {
    struct Init9c { void* s[0x27]; void (__fastcall* initialize)(ProgressBar*, void*, const Extent*, const void*, const void*); };
    vt<Init9c>(self)->initialize(self, nullptr, extent, source->border.params, source->progress.params);  // 004192D0..E7
}
// ---- 00417F10 CSWGuiProgressBar::Initialize(extent, border params, progress params) (RET 0Ch) ----
void __fastcall gui_m48_progress_initialize_00417f10(ProgressBar* self, void*, const Extent* extent,
                                                     const void* border_params, const void* progress_params) {
    self->extent = *extent;                                                        // 00417F10..30
    gui_load_border_initialize_00415190(&self->border, nullptr, &self->extent, border_params);     // 00417F33..3C
    gui_load_border_initialize_00415190(&self->progress, nullptr, &self->extent, progress_params); // 00417F41..4D
    ov(self)->set_extent(self, nullptr, &self->extent);                            // 00417F52..57 (vslot +04)
}

// ---- 0067AB80 CSWGuiLoadScreen::~CSWGuiLoadScreen (RET; EH states 4..0) ----
void __fastcall gui_m48_load_screen_destruct_0067ab80(gui_load::Panel* self, void*) {
    unsigned char* const p = reinterpret_cast<unsigned char*>(self);
    gui_load_label_destruct_00419d10(reinterpret_cast<gui_load::Label*>(p + 0x578), nullptr);  // 0067AB9D..AB
    gui_load_label_destruct_00419d10(reinterpret_cast<gui_load::Label*>(p + 0x434), nullptr);  // 0067ABB0..BB
    gui_load_label_destruct_00419d10(reinterpret_cast<gui_load::Label*>(p + 0x2F4), nullptr);  // 0067ABC0..CB
    gui_load_label_destruct_00419d10(reinterpret_cast<gui_load::Label*>(p + 0x1B4), nullptr);  // 0067ABD0..DB
    kotor_original::kotor_fastcall_0041af70(p + 0x64, nullptr);                    // 0067ABE0..E8
    gui_load_panel_destruct_0040cf70(self, nullptr);                               // 0067ABED..F7
}
// ---- 0067AB60 scalar deleting destructor (RET 4).
void* __fastcall gui_m48_load_screen_deleting_destruct_0067ab60(gui_load::Panel* self, void*, Word flags) {
    gui_m48_load_screen_destruct_0067ab80(self, nullptr);                          // 0067AB63
    if (flags & 1u) live_free(self);                                               // 0067AB68..70
    return self;                                                                   // 0067AB78
}
// ---- 0067AB00 CSWGuiLoadScreen::Draw(float) (RET 4) ----
void __fastcall gui_m48_load_screen_draw_0067ab00(gui_load::Panel* self, void*, Word delta) {
    unsigned char* const p = reinterpret_cast<unsigned char*>(self);
    kotor_AurGUIStartLayer_004591B0();                                             // 0067AB04
    const unsigned char* manager = reinterpret_cast<const unsigned char*>(word_at(p, 0x18));   // 0067AB09
    std::int16_t h, w;
    std::memcpy(&h, manager + 0x6E, 2);                                            // 0067AB0C (MOVSX)
    std::memcpy(&w, manager + 0x6C, 2);                                            // 0067AB10 (MOVSX)
    const std::uint32_t ok = kotor_AurGUISetupViewport_004592F0(0, 0,
        static_cast<std::uint32_t>(static_cast<I32>(w)), static_cast<std::uint32_t>(static_cast<I32>(h)),
        &kotor_gui_m48_cells::viewport_color_00833910, 0, 0x3F800000u);           // 0067AB14..26
    if (ok) {                                                                      // 0067AB32..34
        Control* label = reinterpret_cast<Control*>(p + 0x578);
        vt<ControlDrawSlots>(label)->draw(label, nullptr, delta);                  // 0067AB36..43 (vslot +38)
        kotor_AurGUICloseViewport_00459580();                                      // 0067AB46
    }
    kotor_AurGUIStopLayer_004592B0();                                              // 0067AB4B
    kotor_panel_draw_0040B760(self, nullptr, delta);                               // 0067AB50..53
}
}
// gui_m48_progress_methods_0073e7a0: original table 0073E7A0 (40 slots)
struct GuiM48ProgressBarMethods {
    void* (__fastcall* s00)(gui_lb::ProgressBar*, void*, gui_load::Word);  // +00 0041AF50 CSWGuiProgressBar::~CSWGuiProgressBar
    void (__fastcall* s04)(gui_lb::ProgressBar*, void*, const gui_load::Extent*);  // +04 00419300 CSWGuiProgressBar::SetExtent
    void (__fastcall* s08)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +08 005B5E90 void_return
    void (kih::Control::* s0c)(int);  // +0C 004189A0 CSWGuiControl::HandleMouseOver
    gui_load::Word (__fastcall* s10)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +10 00409EB0 return_zero_0x8
    void (__fastcall* s14)(gui_load::Object*, void*);  // +14 004015A0 void_return
    void (kih::Control::* s18)();  // +18 0041AB90 CSWGuiControl::HandleLMouseDown
    void (kih::Control::* s1c)();  // +1C 0041ABD0 CSWGuiControl::HandleLMouseUp
    void (__fastcall* s20)(gui_load::Object*, void*);  // +20 004015A0 void_return
    void (__fastcall* s24)(gui_load::Object*, void*);  // +24 0041AC70 CSWGuiControl::HandleRMouseUp
    gui_load::Word (__fastcall* s28)(gui_load::Object*, void*);  // +28 0063E7F0 return_zero
    gui_load::Word (__fastcall* s2c)(gui_load::Object*, void*);  // +2C 0063E7F0 return_zero
    gui_load::Object* (__fastcall* s30)(gui_load::Object*, void*);  // +30 00641DB0 self_return
    gui_load::Word (__fastcall* s34)(gui_load::Object*, void*);  // +34 004064A0 GetProperty0x10
    void (__fastcall* s38)(gui_lb::ProgressBar*, void*, gui_load::Word);  // +38 00417F60 CSWGuiProgressBar::Draw
    void (kih::Control::* s3c)(int, int);  // +3C 00418750 CSWGuiControl::HandleInputEvent
    void (kih::Control::* s40)(int);  // +40 004181D0 CSWGuiControl::SetActive
    kih::Control* (kih::Control::* s44)(int, int);  // +44 004187C0 CSWGuiControl::HitCheckMouse
    void (__fastcall* s48)(gui_lb::ProgressBar*, void*, gui_load::Gff*, gui_load::Struct*);  // +48 0041BBC0 CSWGuiProgressBar::Load
    gui_load::Word (__fastcall* s4c)(gui_load::Object*, void*);  // +4C 0063E7F0 return_zero
    gui_load::Word (__fastcall* s50)(gui_load::Object*, void*);  // +50 0063E7F0 return_zero
    gui_load::Word (__fastcall* s54)(gui_load::Object*, void*);  // +54 0063E7F0 return_zero
    gui_load::Word (__fastcall* s58)(gui_load::Object*, void*);  // +58 0063E7F0 return_zero
    gui_load::Word (__fastcall* s5c)(gui_load::Object*, void*);  // +5C 0063E7F0 return_zero
    gui_load::Word (__fastcall* s60)(gui_load::Object*, void*);  // +60 0063E7F0 return_zero
    gui_load::Word (__fastcall* s64)(gui_load::Object*, void*);  // +64 0063E7F0 return_zero
    gui_load::Object* (__fastcall* s68)(gui_load::Object*, void*);  // +68 00641DB0 self_return
    gui_load::Word (__fastcall* s6c)(gui_load::Object*, void*);  // +6C 0063E7F0 return_zero
    gui_load::Word (__fastcall* s70)(gui_load::Object*, void*);  // +70 0063E7F0 return_zero
    gui_load::Word (__fastcall* s74)(gui_load::Object*, void*);  // +74 0063E7F0 return_zero
    gui_load::Word (__fastcall* s78)(gui_load::Object*, void*);  // +78 0063E7F0 return_zero
    void (__fastcall* s7c)(gui_load::Object*, void*, gui_load::Word);  // +7C 00418960 CSWGuiControl::HandleFocusChange
    int (kih::Control::* s80)();  // +80 004189D0 CSWGuiControl::GetIsSelectable
    void (__fastcall* s84)(gui_load::Object*, void*);  // +84 0041AC30 CSWGuiControl::HandleRMouseDown
    void (__fastcall* s88)(gui_load::Object*, void*, gui_load::Word);  // +88 004176A0 CSWGuiControl::SetEnabled
    void (__fastcall* s8c)(gui_load::Object*, void*);  // +8C 004015A0 void_return
    gui_load::Word (__fastcall* s90)(gui_load::Object*, void*);  // +90 00418A90 CSWGuiControl::DisplayToolTip
    void (__fastcall* s94)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +94 005B5E90 void_return
    void (__fastcall* s98)(gui_lb::ProgressBar*, void*, const gui_load::Extent*, gui_lb::ProgressBar*);  // +98 004192D0 CSWGuiProgressBar::Initialize
    void (__fastcall* s9c)(gui_lb::ProgressBar*, void*, const gui_load::Extent*, const void*, const void*);  // +9C 00417F10 CSWGuiProgressBar::Initialize
};
static_assert(sizeof(GuiM48ProgressBarMethods) == 160, "73E7A0 slot count");
extern "C" const GuiM48ProgressBarMethods gui_m48_progress_methods_0073e7a0 = {
    &gui_m48_progress_deleting_destruct_0041af50,  // +00
    &gui_m48_progress_set_extent_00419300,  // +04
    &gui_load_slot_ret8_005b5e90,  // +08
    &kih::Control::HandleMouseOver,  // +0C
    &gui_load_slot_zero8_00409eb0,  // +10
    &gui_load_slot_ret_004015a0,  // +14
    &kih::Control::HandleLMouseDown,  // +18
    &kih::Control::HandleLMouseUp,  // +1C
    &gui_load_slot_ret_004015a0,  // +20
    &gui_load_control_handle_rmouse_up_0041ac70,  // +24
    &gui_load_slot_zero_0063e7f0,  // +28
    &gui_load_slot_zero_0063e7f0,  // +2C
    &gui_load_slot_self_00641db0,  // +30
    &gui_load_slot_prop10_004064a0,  // +34
    &gui_m48_progress_draw_00417f60,  // +38
    &kih::Control::HandleInputEvent,  // +3C
    &kih::Control::SetActive,  // +40
    &kih::Control::HitCheckMouse,  // +44
    &gui_m48_progress_load_0041bbc0,  // +48
    &gui_load_slot_zero_0063e7f0,  // +4C
    &gui_load_slot_zero_0063e7f0,  // +50
    &gui_load_slot_zero_0063e7f0,  // +54
    &gui_load_slot_zero_0063e7f0,  // +58
    &gui_load_slot_zero_0063e7f0,  // +5C
    &gui_load_slot_zero_0063e7f0,  // +60
    &gui_load_slot_zero_0063e7f0,  // +64
    &gui_load_slot_self_00641db0,  // +68
    &gui_load_slot_zero_0063e7f0,  // +6C
    &gui_load_slot_zero_0063e7f0,  // +70
    &gui_load_slot_zero_0063e7f0,  // +74
    &gui_load_slot_zero_0063e7f0,  // +78
    &gui_load_control_focus_change_00418960,  // +7C
    &kih::Control::GetIsSelectable,  // +80
    &gui_load_control_rmouse_down_0041ac30,  // +84
    &gui_load_control_set_enabled_004176a0,  // +88
    &gui_load_slot_ret_004015a0,  // +8C
    &gui_load_control_tooltip_00418a90,  // +90
    &gui_load_slot_ret8_005b5e90,  // +94
    &gui_m48_progress_initialize_004192d0,  // +98
    &gui_m48_progress_initialize_00417f10,  // +9C
};

// kotor_vtable_00752dc0: original table 00752DC0 (27 slots)
struct GuiM48LoadScreenMethods {
    void* (__fastcall* s00)(gui_load::Panel*, void*, gui_load::Word);  // +00 0067AB60 CSWGuiLoadScreen::~CSWGuiLoadScreen
    void (__fastcall* s04)(gui_load::Object*, void*, const gui_load::Extent*);  // +04 0040A5A0 CSWGuiPanel::SetExtent
    void (__fastcall* s08)(gui_load::Object*, void*, gui_load::Object*, gui_load::Word);  // +08 0040A630 CSWGuiPanel::SetActiveControl
    void (__fastcall* s0c)(gui_load::Object*, void*, gui_load::Word);  // +0C 0060E760 void_return
    gui_load::Word (__fastcall* s10)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +10 00409EB0 return_zero_0x8
    void (__fastcall* s14)(gui_load::Object*, void*);  // +14 004015A0 void_return
    void (__fastcall* s18)(gui_load::Object*, void*);  // +18 004015A0 void_return
    void (__fastcall* s1c)(gui_load::Object*, void*);  // +1C 004015A0 void_return
    void (__fastcall* s20)(gui_load::Object*, void*);  // +20 004015A0 void_return
    void (__fastcall* s24)(gui_load::Object*, void*);  // +24 004015A0 void_return
    gui_load::Object* (__fastcall* s28)(gui_load::Object*, void*);  // +28 00641DB0 self_return
    gui_load::Word (__fastcall* s2c)(gui_load::Object*, void*);  // +2C 0063E7F0 return_zero
    gui_load::Word (__fastcall* s30)(gui_load::Object*, void*);  // +30 0063E7F0 return_zero
    void (__fastcall* s34)(gui_load::Panel*, void*, gui_load::Word);  // +34 0067AB00 CSWGuiLoadScreen::Draw
    void (__fastcall* s38)(gui_load::Object*, void*, gui_load::Word);  // +38 0060E760 void_return
    void (__fastcall* s3c)(gui_load::Object*, void*, gui_load::I32, gui_load::I32);  // +3C 00409E60 CSWGuiPanel::HandleInputEvent
    gui_load::Object* (__fastcall* s40)(gui_load::Object*, void*, gui_load::I32, gui_load::I32);  // +40 0040B690 CSWGuiPanel::HitCheckMouse
    gui_load::Object* (__fastcall* s44)(gui_load::Object*, void*);  // +44 0040B630 CSWGuiPanel::GetActiveControl
    void (__fastcall* s48)(gui_load::Panel*, void*);  // +48 0040B870 CSWGuiPanel::OnPanelAdded
    void (__fastcall* s4c)(gui_load::Panel*, void*);  // +4C 0040C170 CSWGuiPanel::OnPanelRemoved
    void (__fastcall* s50)(gui_load::Object*, void*, gui_load::Word);  // +50 0040B640 CSWGuiPanel::OnAButtonPressed
    void (__fastcall* s54)(gui_load::Object*, void*, gui_load::Word);  // +54 0040B650 CSWGuiPanel::OnBButtonPressed
    void (__fastcall* s58)(gui_load::Object*, void*, gui_load::Word);  // +58 0040B660 CSWGuiPanel::OnXButtonPressed
    void (__fastcall* s5c)(gui_load::Object*, void*, gui_load::Word);  // +5C 0040B670 CSWGuiPanel::OnYButtonPressed
    void (__fastcall* s60)(gui_load::Object*, void*, gui_load::Word);  // +60 0040B680 CSWGuiPanel::OnBlackButtonPressed
    gui_load::String* (__fastcall* s64)(gui_load::Panel*, void*, gui_load::String*);  // +64 0040A900 CSWGuiPanel::GetFullScreenBG
    void (__fastcall* s68)(gui_load::Panel*, void*);  // +68 0040A9C0 CSWGuiPanel::ResetFont
};
static_assert(sizeof(GuiM48LoadScreenMethods) == 108, "752DC0 slot count");
extern "C" const GuiM48LoadScreenMethods kotor_vtable_00752dc0 = {
    &gui_m48_load_screen_deleting_destruct_0067ab60,  // +00
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
    &gui_m48_load_screen_draw_0067ab00,  // +34
    &gui_load_slot_ret4_0060e760,  // +38
    &gui_load_panel_handle_input_00409e60,  // +3C
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

// ======================================================================== CSWGuiListBox::Draw / CSWGuiScrollBar::Draw family
namespace r4_lowers_cells { extern volatile float arrow_pulse_alpha_0078d3e4; }   // r4-lowers-r4-cells owner (0078D3E4)
extern "C" {
I32 __fastcall gui_m48_scrollbar_can_scroll_up_00418010(ScrollBar*, void*);
I32 __fastcall gui_m48_scrollbar_can_scroll_down_00418030(ScrollBar*, void*);
I32 __fastcall gui_m48_scrollbar_hit_check_004197c0(ScrollBar*, void*, I32 x, I32 y);
}
namespace {
float float_of(Word bits) { float f; std::memcpy(&f, &bits, 4); return f; }
float float_at(const void* p, unsigned off) { float f; std::memcpy(&f, static_cast<const unsigned char*>(p) + off, 4); return f; }
void put_float(void* p, unsigned off, float f) { std::memcpy(static_cast<unsigned char*>(p) + off, &f, 4); }
struct ImageSlots { void* s00; void (__fastcall* set_extent)(Image*, void*, const Extent*); void* s08;
                    void (__fastcall* draw)(Image*, void*, Word); };              // 73E390 +04 SetExtent, +0C Draw
}
extern "C" {
// ---- 00418010 CSWGuiScrollBar::CanScrollUp (thiscall leaf; ECX preserved) ----
I32 __fastcall gui_m48_scrollbar_can_scroll_up_00418010(ScrollBar* self, void*) {
    if (self->visible_value >= self->max_value) return 0;                         // 00418010..16
    return self->cur_value >= 1 ? 1 : 0;                                           // 00418018..24
}
// ---- 00418030 CSWGuiScrollBar::CanScrollDown (thiscall leaf) ----
I32 __fastcall gui_m48_scrollbar_can_scroll_down_00418030(ScrollBar* self, void*) {
    const I32 max = self->max_value;                                               // 00418034
    I32 limit = max;                                                               // 00418038
    if (!(self->sb_flags & 2u) && !(self->visible_value > 1) && self->cur_value != max) ++limit;   // 00418030..47
    if (self->visible_value >= max) return 0;                                      // 00418048..4D
    return (self->cur_value + self->visible_value < limit) ? 1 : 0;                // 0041804F..5F
}
// ---- 004197C0 CSWGuiScrollBar::HitCheckScrollbar(int x, int y) (RET 8): 0 none, 1 up, 2 down, 3 thumb,
// 4 track before thumb, 5 track after thumb. Comparisons transcribed operand-for-operand (including the horizontal
// up-arrow bound DIR.width + extent.width + extent.left at 00419950..56, as compiled).
I32 __fastcall gui_m48_scrollbar_hit_check_004197c0(ScrollBar* self, void*, I32 x, I32 y) {
    const Extent d = self->dir.extent;                                             // 004197C8..F1 (+138)
    const Extent e = self->extent;                                                 // 004197E1..F5 (+04)
    const Extent t = self->thumb.extent;                                           // 004197F8..21 (+E4)
    Extent got;
    const Extent in = *gui_load_border_inner_extent_00415240(&self->border, nullptr, &got);   // 00419815..40 (+6C)
    if (self->extent.height > self->extent.width) {                                // 0041983D..4B
        if (y >= t.top && y <= t.height + t.top && x >= t.left && x <= t.width + t.left) return 3;   // 00419851..85
        if (y > e.top && y <= d.height + e.top && x > e.left && x < e.left + e.width) return 1;      // 00419888..AE
        const I32 down_top = e.height - d.height + e.top;                          // 004198B1..BB
        if (y >= down_top && y < e.height + e.top && x > e.left && x < e.width + e.left) return 2;   // 004198BF..E1
        if (y > d.height + e.top && y < t.top && x > in.left && x < in.width + in.left) return 4;    // 004198E8..12
        if (y > t.height + t.top && y < down_top && x > in.left && x < in.width + in.left) return 5;  // 00419919..0A
        return 0;                                                                  // 00419A0D
    }
    if (x > e.left && x <= d.width + e.width + e.left && y > e.top && y < e.height + e.top) return 1; // 00419948..68
    const I32 right_left = e.width - d.width + e.left;                             // 0041996E..78
    if (x >= right_left && x < e.width + e.left && y > e.top && y < e.height + e.top) return 2;      // 0041997C..90
    if (x >= t.left && x <= t.width + t.left && y >= t.top && y <= t.height + t.top) return 3;       // 00419996..B8
    if (x > d.width + e.left && x < t.left && y > in.top && y < in.height + in.top) return 4;        // 004199C2..DC
    if (x > t.width + t.left && x < right_left && y > in.top && y < in.height + in.top) return 5;    // 004199E2..0A
    return 0;                                                                      // 00419A0D
}
// ---- 00419660 CSWGuiScrollBar::Draw(float) (RET 4) ----
void __fastcall gui_load_scrollbar_draw_00419660(ScrollBar* self, void*, Word delta) {
    I32 up = 0, down = 0;                                                          // 00419670..75
    const Word mode = self->sb_flags & 1u;                                         // 0041966C..72
    if (mode == 0) {                                                               // 00419677..79
        struct DrawSlot { void* s00; void* s04; void* s08; void (__fastcall* draw)(void*, void*, Word); };
        vt<DrawSlot>(&self->border)->draw(&self->border, nullptr, delta);          // 0041968E..95 (+6C vslot +0C)
        vt<DrawSlot>(&self->thumb)->draw(&self->thumb, nullptr, delta);            // 00419698..A5 (+E0 vslot +0C)
        up = 1; down = 1;                                                          // 004196A8..AD
    } else if (mode == 1) {                                                        // 0041967B..7C
        up = gui_m48_scrollbar_can_scroll_up_00418010(self, nullptr);              // 0041967E..83
        down = gui_m48_scrollbar_can_scroll_down_00418030(self, nullptr);          // 00419685..8A
    }
    Extent r = self->dir.extent;                                                   // 004196B1..CE (+138)
    if (up) {                                                                      // 004196AF..D2
        if (self->extent.height > self->extent.width) r.top = self->extent.top;   // 004196D4..DF
        else r.left = self->extent.left;                                           // 004196E5..E8
        self->dir.params.flags &= 0xFFFFFFC3u;                                     // 004196EC (+180)
        vt<ImageSlots>(&self->dir)->set_extent(&self->dir, nullptr, &r);           // 004196F3..06
        vt<ImageSlots>(&self->dir)->draw(&self->dir, nullptr, delta);              // 00419709..0E
    }
    if (down) {                                                                    // 00419711..13
        if (self->extent.height > self->extent.width) {                            // 00419715..1B
            r.top = self->extent.height + self->extent.top - r.height;             // 0041971D..28
            self->dir.params.flags = (self->dir.params.flags & 0xFFFFFFD3u) | 0x10u;   // 0041972A..3A
        } else {
            r.left = self->extent.width - r.width + self->extent.left;             // 00419742..54
            self->dir.params.flags = (self->dir.params.flags & 0xFFFFFFE3u) | 0x20u;   // 00419749..60
        }
        vt<ImageSlots>(&self->dir)->set_extent(&self->dir, nullptr, &r);           // 00419766..79
        vt<ImageSlots>(&self->dir)->draw(&self->dir, nullptr, delta);              // 0041977C..81
    }
}
// ---- 0041A3E0 CSWGuiListBox::Draw(float) (RET 4) ----
void __fastcall gui_load_listbox_draw_0041a3e0(ListBox* self, void*, Word delta) {
    struct DrawSlot38 { void* s[14]; void (__fastcall* draw)(void*, void*, Word); };   // control +38 Draw
    struct DrawSlot0c { void* s00; void* s04; void* s08; void (__fastcall* draw)(void*, void*, Word); };
    const I32 count = self->items.size;                                            // 0041A3F4..FA (saved)
    const Word flags = self->lb_flags;                                             // 0041A3EB
    if (flags & 0x20u) {                                                           // 0041A3F1..FE
        if (flags & 0x800u) {                                                      // 0041A400..03
            const float pulse = r4_lowers_cells::arrow_pulse_alpha_0078d3e4;       // 0041A405
            std::memcpy(&self->scroll.dir.params.u14, &pulse, 4);                  // 0041A40A (+274)
        }
        vt<DrawSlot38>(&self->scroll)->draw(&self->scroll, nullptr, delta);        // 0041A410..1D
    }
    vt<DrawSlot0c>(&self->border)->draw(&self->border, nullptr, delta);            // 0041A420..2D
    const std::uint32_t ok = kotor_AurGUISetupViewport_004592F0(
        static_cast<std::uint32_t>(self->inner.left), static_cast<std::uint32_t>(self->inner.top),
        static_cast<std::uint32_t>(self->inner.width), static_cast<std::uint32_t>(self->inner.height),
        &self->color, 0, 0x3F800000u);                                             // 0041A430..5A
    if (ok) {                                                                      // 0041A462..64
        if (static_cast<I32>(self->padding) + self->item_height > self->inner.height &&
            self->items.size > 0 && !(self->lb_flags & 0x100u)) {                  // 0041A46A..96
            Control* first = item_at(self, 0);                                     // 0041A498..9E
            vt<DrawSlot38>(first)->draw(first, nullptr, delta);                    // 0041A4A0..A3
        } else {
            for (I32 i = 0; i < count; ++i) {                                      // 0041A4A9..EF
                Control* c = item_at(self, i);                                     // 0041A4B0..B6
                const I32 top = c->extent.top;                                     // 0041A4B9 (+08)
                const I32 bottom = c->extent.height + top;                         // 0041A4BC..C1 (+10)
                if (bottom <= 0) continue;                                         // 0041A4C3..C5
                const I32 inner_height = self->inner.height;                       // 0041A4C7
                if (top > inner_height) continue;                                  // 0041A4CD..CF
                if (reinterpret_cast<const unsigned char*>(self)[0x2BD] & 4u) {    // 0041A4D1..D8
                    if (bottom >= inner_height || top < 0) continue;               // 0041A4DA..E0
                }
                vt<DrawSlot38>(c)->draw(c, nullptr, delta);                        // 0041A4E2..E5
            }
        }
        kotor_AurGUICloseViewport_00459580();                                      // 0041A4F2
    }
    const unsigned char* manager = static_cast<const unsigned char*>(gui_manager());   // 0041A4F7
    if (word_at(manager, 0x10) != static_cast<Word>(reinterpret_cast<std::uintptr_t>(self))) return;   // 0041A4FC..FF
    if (self->u88 == 0) return;                                                    // 0041A505..0D
    I32 first_out = 0, second_out = 0;                                             // [esp+10] / [esp+0C]
    gui_load_panel_local_mouse_0040ba20(self->owner, nullptr, &first_out, &second_out);   // 0041A513..20
    const I32 hit = gui_m48_scrollbar_hit_check_004197c0(&self->scroll, nullptr, first_out, second_out);   // 0041A525..35
    if (static_cast<Word>(hit) != word_at(self->u70, 0)) return;                   // 0041A53A..3D ([esi+70h])
    const double remaining = static_cast<double>(float_at(self, 0x8C)) - static_cast<double>(float_of(delta));  // 0041A53F..45
    put_float(self, 0x8C, static_cast<float>(remaining));                          // 0041A549 (FST)
    if (!(remaining < 0.0)) return;                                                // 0041A54F..5A (C0/C2 parity; NaN exits)
    do {
        const double next = static_cast<double>(float_at(self, 0x8C)) + static_cast<double>(0.1f);   // 0041A560..6C ([0073E798])
        const Word event = self->u88;                                              // 0041A566
        put_float(self, 0x8C, static_cast<float>(next));                           // 0041A579 (FSTP)
        cv(self)->handle_input_event(self, nullptr, static_cast<I32>(event), 1);   // 0041A572..7F (vslot +3C)
    } while (static_cast<double>(float_at(self, 0x8C)) < 0.0);                     // 0041A582..93
}
}

// ======================================================================== CSWGuiListBox mouse input family + AurPrintf
#include <cstdarg>
#include <cstdio>
#include <cstring>
// Cells read only by AurPrintf 0044D4F0 (no other reference in the image: neither code nor data words point at them):
//   007B90F8 enable word (.bss, loader zero, never written -> AurPrintf returns at 0044D512 on this image)
//   007B90F4 column offset (.bss zero), 007B90E8 row counter (.bss zero; written 0044D59E), 0078E364 align byte (.data = 1).
namespace kotor_gui_m48_cells {
volatile std::int32_t aur_printf_enabled_007b90f8 = 0;
volatile std::int32_t aur_printf_column_007b90f4 = 0;
volatile std::int32_t aur_printf_row_007b90e8 = 0;
volatile unsigned char aur_printf_right_align_0078e364 = 1;
}
extern "C" {
// sibling fork CLIENT (lane-m48-b/client/src/client_leaves_a.cpp): PostedString::PostedString 0044D320, RET 10h.
void* __fastcall kotor_fastcall_0044d320(void* posted, void*, const char* text, std::uint32_t y, std::uint32_t x,
                                         std::uint32_t life_bits);
void* __cdecl live_operator_new(std::uint32_t);                                   // 006FA7E6 owner (allocator_live)
void __cdecl gui_m48_aur_printf_0044d4f0(const char* format, ...);
I32 __fastcall gui_m48_listbox_should_scroll_0041a290(ListBox*, void*);
I32 __fastcall gui_m48_listbox_pages_to_scroll_004182b0(ListBox*, void*);
}
namespace {
// x87 sequences ending in the original _ftol2 port 006FAE8C (D30-FINAL; wide intermediates kept on ST).
I32 pages_value(const I32* pages, const I32* dy, const I32* range, const Word* base) {   // 0041B71D..34
    static const float half = 0.5f;                                                 // [0073E9AC]
    I32 converted;
    __asm {
        mov eax, pages
        mov ecx, dy
        mov edx, range
        fild dword ptr [eax]
        fild dword ptr [ecx]
        fidiv dword ptr [edx]
        fmulp st(1), st
        mov eax, base
        fiadd dword ptr [eax]
        fadd dword ptr half
        call unresolved_ftol2_006fae8c
        mov converted, eax
    }
    return converted;
}
I32 rows_value(const I32* dy, const I32* range, const I32* rows, const Word* base) {   // 0041B7B4..D6
    static const float half = 0.5f;                                                 // [0073E9AC]
    I32 converted;
    __asm {
        mov eax, dy
        mov ecx, range
        mov edx, rows
        fild dword ptr [eax]
        fidiv dword ptr [ecx]
        fimul dword ptr [edx]
        mov eax, base
        fiadd dword ptr [eax]
        fadd dword ptr half
        call unresolved_ftol2_006fae8c
        mov converted, eax
    }
    return converted;
}
I32 pages_raw(const I32* item_height, const float* scale, const I32* inner_height) {   // 004182C2..DE
    static const float bias = 0.9998999834060669f;                                 // [0073E468]
    I32 converted;
    __asm {
        mov eax, item_height
        mov ecx, scale
        mov edx, inner_height
        fild dword ptr [eax]
        fmul dword ptr [ecx]
        fidiv dword ptr [edx]
        fadd dword ptr bias
        call unresolved_ftol2_006fae8c
        mov converted, eax
    }
    return converted;
}
I32 ftol_float(const float* value) {                                               // 004182DE..E6
    I32 converted;
    __asm {
        mov eax, value
        fld dword ptr [eax]
        call unresolved_ftol2_006fae8c
        mov converted, eax
    }
    return converted;
}
using PtrSlot = Control* (__fastcall*)(Control*, void*);                           // +6C "as scroll bar" (EAX pointer)
const PtrSlot* pslots(const Control* c) { return vt<PtrSlot>(c); }
}
extern "C" {
// ---- 0044D4F0 AurPrintf(char const* format, ...) (cdecl, EH state 0 around the PostedString ctor) ----
// Gate [007B90F8] is never written in this image, so every call returns at 0044D512; the body is reproduced for
// completeness. _vsprintf 006FC4EF is served by the linked UCRT vsprintf (same "%d"/"%s" formatting; only reached
// behind the never-set gate - equivalence note). Allocation failure: the original stores the colour words through the
// NULL result (0044D5AD..F6, access violation); here the stores are skipped on NULL (D2-style deviation, unreachable).
void __cdecl gui_m48_aur_printf_0044d4f0(const char* format, ...) {
    if (kotor_gui_m48_cells::aur_printf_enabled_007b90f8 == 0) return;             // 0044D4FE..12
    char text[0x200];                                                              // [esp+0Ch] 0x200 bytes
    va_list args;
    va_start(args, format);
    std::vsprintf(text, format, args);                                             // 0044D518..2D
    va_end(args);
    void* block = live_operator_new(0x434);                                        // 0044D532..37
    void* posted = nullptr;
    if (block) {                                                                   // 0044D545..52
        std::int32_t column;
        if (kotor_gui_m48_cells::aur_printf_right_align_0078e364) {                // 0044D554..5D
            column = 0x50 - static_cast<std::int32_t>(std::strlen(text)) -
                     kotor_gui_m48_cells::aur_printf_column_007b90f4;               // 0044D55F..76
        } else {
            column = kotor_gui_m48_cells::aur_printf_column_007b90f4;              // 0044D57E
        }
        const std::int32_t row = (kotor_gui_m48_cells::aur_printf_row_007b90e8 + 1) % 0x28;   // 0044D584..90 (CDQ/IDIV)
        kotor_gui_m48_cells::aur_printf_row_007b90e8 = row;                        // 0044D59E
        posted = kotor_fastcall_0044d320(block, nullptr, text, static_cast<std::uint32_t>(column),
                                         static_cast<std::uint32_t>(row), 0x41200000u);   // 0044D592..A4 (life 10.0f)
    }
    if (posted) {
        const Word colors[8] = {0x3F6B851Fu, 0x3F68F5C3u, 0x3F666666u, 0x3F400000u,
                                0x3EB33333u, 0x3E99999Au, 0x3ECCCCCDu, 0x3E99999Au};  // 0044D5AF..F6
        for (unsigned k = 0; k < 8; ++k) put_word(posted, 0x40C + 4 * k, colors[k]);
    }
}
// ---- 0041A290 CSWGuiListBox::shouldScroll (thiscall leaf) ----
I32 __fastcall gui_m48_listbox_should_scroll_0041a290(ListBox* self, void*) {
    if (static_cast<I32>(self->padding) + self->item_height <= self->inner.height) return 0;   // 0041A290..A3
    if (self->items.size <= 0) return 0;                                           // 0041A2A5..AD
    return (self->lb_flags & 0x100u) ? 0 : 1;                                      // 0041A2AF..C2
}
// ---- 004182B0 CSWGuiListBox::getPagesToScroll (thiscall) ----
I32 __fastcall gui_m48_listbox_pages_to_scroll_004182b0(ListBox* self, void*) {
    const I32 inner_height = self->inner.height;                                   // 004182B4..BC
    if (inner_height == 0) return 1;                                               // 004182BA..C0 / 004182F4
    const I32 upper = pages_raw(&self->item_height, &self->scale, &inner_height); // 004182C2..D9
    const I32 whole = ftol_float(&self->scale);                                    // 004182DE..E6
    return upper - whole + 1;                                                      // 004182EB..EE
}
// ---- 0041B670 CSWGuiListBox::HandleMouseCapturedMovement(int, int) (RET 8; arguments unused, EAX = 1) ----
Word __fastcall gui_load_listbox_captured_movement_0041b670(ListBox* self, void*, Word, Word) {
    const unsigned char* manager = static_cast<const unsigned char*>(gui_manager());   // 0041B670
    if (!(manager[0x0C] & 1u)) return 1;                                           // 0041B67D..82
    if (word_at(self->u70, 0) != 3u) return 1;                                     // 0041B688..8C ([esi+70h])
    I32 y = 0, x = 0;                                                              // [esp+10] / [esp+20]
    gui_load_panel_local_mouse_0040ba20(self->owner, nullptr, &x, &y);             // 0041B692..9F
    if (self->scroll.extent.height <= self->scroll.extent.width) return 1;         // 0041B6A4..B8 (+114 vs +110)
    const I32 dy = y - static_cast<I32>(word_at(self->u70, 8));                    // 0041B6BE..C5 ([esi+78h])
    Extent inner_buffer;
    const Extent* inner = gui_load_border_inner_extent_00415240(&self->scroll.border, nullptr, &inner_buffer);   // 0041B6E0..F3 (+170)
    const I32 range = inner->height - self->scroll.thumb.extent.height;            // 0041B6F8..FE (I.height - [+1F4])
    if (gui_m48_listbox_should_scroll_0041a290(self, nullptr)) {                   // 0041B702..0B
        const I32 pages = gui_m48_listbox_pages_to_scroll_004182b0(self, nullptr) - 1;   // 0041B711..19
        const I32 value = pages_value(&pages, &dy, &range, &self->u7c);            // 0041B71D..34
        std::int16_t page = static_cast<std::int16_t>(value);                      // 0041B739..3D (AX)
        self->u2c2 = page;
        const I32 clamped = page < 1 ? 1 : static_cast<I32>(page);                 // 0041B744..4D
        self->u2c2 = static_cast<I16>(clamped);                                    // 0041B752
        const I32 limit = gui_m48_listbox_pages_to_scroll_004182b0(self, nullptr); // 0041B75C
        const I32 chosen = (limit < static_cast<I16>(clamped)) ? gui_m48_listbox_pages_to_scroll_004182b0(self, nullptr)
                                                               : static_cast<I16>(clamped);   // 0041B761..6E
        self->u2c2 = static_cast<I16>(chosen);                                     // 0041B772
        gui_load_listbox_organize_oversized_0041a2d0(self, nullptr);               // 0041B779
        const Word anchor = word_at(self->u70, 8);                                 // 0041B77E ([esi+78h])
        const I32 total = gui_m48_listbox_pages_to_scroll_004182b0(self, nullptr); // 0041B78A
        gui_m48_aur_printf_0044d4f0("pos %d/%d, dY %d (%d-%d)", static_cast<I32>(self->u2c2), total, dy, y,
                                    static_cast<I32>(anchor));                     // 0041B78F..9D (0073E990)
        return 1;                                                                  // 0041B7A8
    }
    const I32 rows = self->scroll.max_value - 1;                                   // 0041B7B8..C1 (+160)
    I32 value = rows_value(&dy, &range, &rows, &self->u7c);                        // 0041B7B4..D6
    value &= -static_cast<I32>(value >= 0);                                        // 0041B7DB..E3 (SETL/DEC/AND)
    if (rows < value) value = rows;                                                // 0041B7E5..E9
    self->lb_flags &= 0xFFFFEFFFu;                                                 // 0041B7EB..FA
    self->first = static_cast<I16>(value);                                         // 0041B800 (+2C8)
    gui_load_scrollbar_set_cur_004180a0(&self->scroll, nullptr, value);            // 0041B7F7..07
    gui_load_listbox_organize_0041b140(self, nullptr);                             // 0041B80C..0E
    return 1;                                                                      // 0041B816
}
// ---- 0041C4A0 CSWGuiListBox::HandleLMouseDown (thiscall, RET) ----
void __fastcall gui_load_listbox_lmouse_down_0041c4a0(ListBox* self, void*) {
    kih::Manager* const manager = static_cast<kih::Manager*>(gui_manager());
    I32 index;                                                                     // [esp+10]
    Control* hit = gui_load_listbox_hit_local_0041a5e0(self, nullptr, &index);     // 0041C4A5..AC
    if (!hit) return;                                                              // 0041C4B3..B5
    if (slots(hit)[0x6C / 4](hit, nullptr) != 0) {                                // 0041C4BB..C4
        I32 first_out = 0, second_out = 0;                                         // [esp+8] / [esp+0C]
        gui_load_panel_local_mouse_0040ba20(self->owner, nullptr, &first_out, &second_out);   // 0041C4CA..D7
        ScrollBar* bar = static_cast<ScrollBar*>(pslots(hit)[0x6C / 4](hit, nullptr));      // 0041C4E5..EA
        const I32 part = gui_m48_scrollbar_hit_check_004197c0(bar, nullptr, first_out, second_out);   // 0041C4DC..EF
        put_word(self->u70, 0, static_cast<Word>(part));                           // 0041C4F4 ([esi+70h])
        self->u88 = 0;                                                             // 0041C4FB
        self->u8c = 0x3F000000u;                                                   // 0041C505 (0.5f)
        switch (part) {                                                            // 0041C4F7..15 (table 0041C69C)
        case 1:                                                                    // 0041C51C
            self->u88 = 0x1FB;
            cv(self)->handle_input_event(self, nullptr, 0x1FB, 1);
            break;
        case 2:                                                                    // 0041C55E
            self->u88 = 0x1FC;
            cv(self)->handle_input_event(self, nullptr, 0x1FC, 1);
            break;
        case 3:                                                                    // 0041C5A0
            put_word(self->u70, 8, static_cast<Word>(second_out));                 // 0041C5A8 ([esi+78h])
            put_word(self->u70, 4, static_cast<Word>(first_out));                  // 0041C5AD ([esi+74h])
            if (gui_m48_listbox_should_scroll_0041a290(self, nullptr))             // 0041C5B0..B7
                self->u7c = static_cast<Word>(static_cast<I32>(self->u2c2));       // 0041C5B9..C2 (MOVSX)
            else
                self->u7c = static_cast<Word>(self->scroll.cur_value);             // 0041C5D7..DF (+164)
            break;
        case 4:                                                                    // 0041C548 -> 0041C67E
            self->u88 = 0x1FD;
            cv(self)->handle_input_event(self, nullptr, 0x1FD, 1);
            break;
        case 5:                                                                    // 0041C58A -> 0041C67E
            self->u88 = 0x1FE;
            cv(self)->handle_input_event(self, nullptr, 0x1FE, 1);
            break;
        default: break;                                                            // 0041C50F -> 0041C685
        }
        manager->CaptureMouse(reinterpret_cast<kih::Control*>(self), 1);           // 0041C534..3D / 0041C685..8E
        return;
    }
    if (self->flags & 8u) {                                                        // 0041C5F4..F8
        const unsigned char* m = reinterpret_cast<const unsigned char*>(manager);
        kih::Control* focus = reinterpret_cast<kih::Control*>(word_at(m, 8));      // 0041C5FE..04
        kih::Control* me = reinterpret_cast<kih::Control*>(self);
        if (focus && (focus == me || focus->GetSelectableParent() == me) &&
            (reinterpret_cast<const unsigned char*>(gui_manager())[0x1C] & 1u))    // 0041C607..21
            cv(self)->handle_input_event(self, nullptr, 0x1F9, 1);                 // 0041C623..2E
        bool select = (self->lb_flags & 1u) != 0;                                  // 0041C631..38
        if (!select && self->items.size > 0) {                                     // 0041C63A..42
            Control* first = item_at(self, 0);                                     // 0041C644..4A
            select = slots(first)[0x60 / 4](first, nullptr) != 0;                  // 0041C64C..53
        }
        if (select) {
            if (static_cast<I32>(self->selected) != index)                         // 0041C65E..6B
                gui_load_listbox_set_selected_0041c040(self, nullptr, index, 1);   // 0041C66D..72
        } else {
            self->u6c = index;                                                     // 0041C655..59
        }
        cv(self)->handle_input_event(self, nullptr, 0x1F8, 1);                     // 0041C677..82
    }
    static_cast<kih::Manager*>(gui_manager())->CaptureMouse(reinterpret_cast<kih::Control*>(self), 1);   // 0041C685..8E
}
}

// ======================================================================== CSWGuiListBox::HandleInputEvent + CExoInput::ClearEventRepeat
namespace kotor_menu_window_cells { extern void* volatile input_007a39e4; }     // CExoInput* cell 007A39E4 (window-shared-cells)
extern "C" {
std::uint32_t __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);      // resource-list-accessor-exports
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*, void*, void**);       // resource-list-accessor-exports
void __fastcall gui_m48_input_internal_clear_event_repeat_005e05f0(void* internal, void*, Word index);
void __fastcall gui_m48_input_clear_event_repeat_005df4a0(void* input, void*, Word index);

// ---- 005E05F0 CExoInputInternal::ClearEventRepeat(ulong) (RET 4) ----
void __fastcall gui_m48_input_internal_clear_event_repeat_005e05f0(void* internal, void*, Word index) {
    const Word descs = word_at(internal, 0x128);                                   // 005E05F8
    const void* desc = reinterpret_cast<const void*>(word_at(reinterpret_cast<const void*>(descs), index * 4));   // 005E05FE
    if (!desc || word_at(desc, 0x14) != 1u) return;                                // 005E0604..0C
    void* list = reinterpret_cast<void*>(word_at(internal, 0x138));                // 005E060E
    void* pos = reinterpret_cast<void*>(word_at(list, 0));                         // 005E0614..18 (head)
    while (pos) {                                                                  // 005E061C / 005E0648
        list = reinterpret_cast<void*>(word_at(internal, 0x138));                  // 005E0620 (reloaded)
        void* data = reinterpret_cast<void*>(static_cast<std::uintptr_t>(unresolved_list_at_pos_005e9b60(list, nullptr, pos)));   // 005E0626..27
        if (data && word_at(data, 0) == index) {                                   // 005E062C..32
            put_word(data, 0x0C, 0);                                               // 005E0652
            put_word(data, 0x18, 0);                                               // 005E0655
            put_word(data, 0x10, 0);                                               // 005E0658
            put_word(data, 0x14, 0);                                               // 005E065B
            return;
        }
        list = reinterpret_cast<void*>(word_at(internal, 0x138));                  // 005E0634
        unresolved_list_next_005e9b90(list, nullptr, &pos);                        // 005E063A..3F
    }
}
// ---- 005DF4A0 CExoInput::ClearEventRepeat(ulong): ECX = [ECX+4]; JMP 005E05F0.
void __fastcall gui_m48_input_clear_event_repeat_005df4a0(void* input, void*, Word index) {
    gui_m48_input_internal_clear_event_repeat_005e05f0(reinterpret_cast<void*>(word_at(input, 4)), nullptr, index);   // 005DF4A0..A3
}
}
namespace {
// Owner panel's manager word [[owner vslot +28 ()]+18h] (0041CEFB..19 / 0041D006..1B).
const void* owner_manager(ListBox* self) {
    gui_load::Panel* panel = self->owner->vtbl->as_panel(self->owner, nullptr);
    return reinterpret_cast<const void*>(word_at(panel, 0x18));
}
// 0041CEE3: optional GUI sound of the owner panel's manager.
void list_sound(ListBox* self) {
    const unsigned char sound = reinterpret_cast<const unsigned char*>(self)[0x2C1];   // 0041CEE3 (+2C1)
    if (sound == 0xFF) return;                                                     // 0041CEEA
    if (!self->owner) return;                                                      // 0041CEF0..F5
    if (!self->owner->vtbl->as_panel(self->owner, nullptr)) return;                // 0041CEFB..02
    const void* manager = owner_manager(self);                                     // 0041CF08..19
    reinterpret_cast<kih::Manager*>(const_cast<void*>(manager))->PlayGuiSound(static_cast<char>(sound));   // 0041CF10..1C
}
// 0041CFFB..2D / 0041D11F..4C: wrap-around clears the repeat of the owner manager's current input event.
void clear_repeat(ListBox* self) {
    if (!self->owner) return;
    if (!self->owner->vtbl->as_panel(self->owner, nullptr)) return;
    const void* manager = owner_manager(self);
    gui_m48_input_clear_event_repeat_005df4a0(kotor_menu_window_cells::input_007a39e4, nullptr, word_at(manager, 0x68));
}
void scroll_to_first(ListBox* self, I16 first) {                                   // 0041D39A..C4 / 0041D287..A9
    self->first = first;
    self->lb_flags &= 0xFFFFEFFFu;
    gui_load_scrollbar_set_cur_004180a0(&self->scroll, nullptr, static_cast<I32>(first));
    gui_load_listbox_organize_0041b140(self, nullptr);
}
I32 clamp_pages(ListBox* self, I32 wanted) {                                       // 0041D181..A1 / 0041D20A..15
    const I32 pages = gui_m48_listbox_pages_to_scroll_004182b0(self, nullptr);
    return pages >= wanted ? wanted : gui_m48_listbox_pages_to_scroll_004182b0(self, nullptr);
}
}
extern "C" {
// ---- 0041CE20 CSWGuiListBox::HandleInputEvent(GuiEvents event, int value) (RET 8) ----
void __fastcall gui_load_listbox_handle_input_0041ce20(ListBox* self, void*, I32 event, I32 value) {
    if (value != 0) {                                                              // 0041CE20..30
        int kind = 0;                                                              // 1 next, 2 prev, 3 page-up, 4 page-down, 5 page-prev, 6 page-next
        if (event > 0x1F5) {                                                       // 0041CE36..3C / 0041D156..65 (table 0041D3D0)
            switch (event) { case 0x1FB: kind = 3; break; case 0x1FC: kind = 4; break;
                             case 0x1FD: kind = 5; break; case 0x1FE: kind = 6; break; default: break; }
        } else if (event == 0x1F5) kind = 4;                                       // 0041CE42
        else if (event > 0x3D) { if (event == 0x3E) kind = 1; else if (event == 0x1F4) kind = 3; }   // 0041D032..41
        else if (event == 0x3D || event == 0x31) kind = 2;                         // 0041CE51 / 0041CE5C
        else if (event == 0x32) kind = 1;                                          // 0041CE62..63
        const I16 selected = self->selected;                                       // 0041CE69 / 0041CF26 (DI)
        switch (kind) {
        case 1:                                                                    // 0041CE69
            if (selected == -1) {
                if (self->lb_flags & 0x200u) {                                     // 0041CE7A..83
                    const I32 n = self->items.size - static_cast<I32>(self->visible_count);   // 0041CE89..98
                    const I32 m = n > 0 ? n : 0;                                   // 0041CEA3..A9 (SETLE/DEC/AND)
                    const I32 first = static_cast<I32>(self->first);               // 0041CE9A
                    const bool changed = first != m;                               // 0041CEAD..B2
                    const I32 next = (first + 1 < m) ? first + 1 : m;              // 0041CEBE..CB
                    self->first = static_cast<I16>(next);                          // 0041CECD
                    gui_load_listbox_organize_0041b140(self, nullptr);             // 0041CED4..D6
                    if (changed) list_sound(self);                                 // 0041CEDB..DD
                } else if (self->items.size != 0) {                                // 0041D07B..83
                    gui_load_listbox_set_selected_0041c040(self, nullptr, 0, 1);   // 0041D089..8F
                    list_sound(self);                                              // 0041D094
                }
            } else if (gui_m48_listbox_should_scroll_0041a290(self, nullptr)) {    // 0041D099..A2
                const I32 page = static_cast<I32>(self->u2c2);                     // 0041D0A4
                const bool changed = page != gui_m48_listbox_pages_to_scroll_004182b0(self, nullptr);   // 0041D0AB..B6
                self->u2c2 = static_cast<I16>(clamp_pages(self, page + 1));        // 0041D0B9..D4
                gui_load_listbox_organize_oversized_0041a2d0(self, nullptr);       // 0041D0DB
                if (changed) list_sound(self);                                     // 0041D0E4 -> 0041CEDB
            } else if (static_cast<I32>(selected) < self->items.size - 1) {        // 0041D0E9..F5
                gui_load_listbox_set_selected_0041c040(self, nullptr, static_cast<I32>(selected) + 1, 1);   // 0041D0F7..FD
                list_sound(self);
            } else if (self->lb_flags & 0x40u) {                                   // 0041D107..0E
                gui_load_listbox_set_selected_0041c040(self, nullptr, 0, 1);       // 0041D114..1A
                clear_repeat(self);                                                // 0041D11F..4C
                list_sound(self);
            }
            break;
        case 2:                                                                    // 0041CF26
            if (selected == -1) {
                if (self->lb_flags & 0x200u) {                                     // 0041CF33..3C
                    const I16 first = self->first;                                 // 0041CF3E
                    const bool changed = first != 0;                               // 0041CF45..4A
                    const I32 prev = static_cast<I32>(first) - 1;                  // 0041CF4D..50
                    self->first = static_cast<I16>(prev > 0 ? prev : 0);           // 0041CF51..5D
                    gui_load_listbox_organize_0041b140(self, nullptr);             // 0041CED4
                    if (changed) list_sound(self);                                 // 0041CEDB
                } else if (self->items.size != 0) {                                // 0041CF69..71
                    gui_load_listbox_set_selected_0041c040(self, nullptr, 0, 0);   // 0041CF77..7D
                    list_sound(self);                                              // 0041CF82
                }
            } else if (gui_m48_listbox_should_scroll_0041a290(self, nullptr)) {    // 0041CF87..90
                const I16 page = self->u2c2;                                       // 0041CF92
                const bool changed = page != 1;                                    // 0041CF9B..A2
                const I32 prev = static_cast<I32>(page) - 1;                       // 0041CFA2..A5
                self->u2c2 = static_cast<I16>(prev < 1 ? 1 : prev);                // 0041CFA6..B4
                gui_load_listbox_organize_oversized_0041a2d0(self, nullptr);       // 0041CFBB
                if (changed) list_sound(self);                                     // 0041CFC0 -> 0041CEDB
            } else if (selected > 0) {                                             // 0041CFC5..C8
                gui_load_listbox_set_selected_0041c040(self, nullptr, static_cast<I32>(selected) - 1, 1);   // 0041CFCA..D3
                list_sound(self);
            } else if (self->lb_flags & 0x40u) {                                   // 0041CFDD..E4
                gui_load_listbox_set_selected_0041c040(self, nullptr, self->items.size - 1, 1);   // 0041CFEA..F6
                clear_repeat(self);                                                // 0041CFFB..28
                list_sound(self);
            }
            break;
        case 3:                                                                    // 0041D047 (0x1F4 / 0x1FB)
            if (gui_m48_listbox_should_scroll_0041a290(self, nullptr)) {
                const I32 prev = static_cast<I32>(self->u2c2) - 1;                 // 0041D056..5D
                self->u2c2 = static_cast<I16>(prev < 1 ? 1 : prev);                // 0041D05E..6A
                gui_load_listbox_organize_oversized_0041a2d0(self, nullptr);       // 0041D071
            } else if (self->first > 0) {                                          // 0041D2EA..F6
                scroll_to_first(self, static_cast<I16>(self->first - 1));          // 0041D2F8..C4
            }
            break;
        case 4:                                                                    // 0041D16C (0x1F5 / 0x1FC)
            if (gui_m48_listbox_should_scroll_0041a290(self, nullptr)) {
                self->u2c2 = static_cast<I16>(clamp_pages(self, static_cast<I32>(self->u2c2) + 1));   // 0041D177..9C
                gui_load_listbox_organize_oversized_0041a2d0(self, nullptr);
            } else {
                const I32 rows = self->scroll.max_value;                           // 0041D1AD (+160)
                const I16 first = self->first;                                     // 0041D1A6
                if (static_cast<I32>(first) < rows - 1 &&                          // 0041D1B3..BB
                    static_cast<I32>(self->last) < static_cast<I32>(self->visible_count) + rows - 1)   // 0041D1C1..D5
                    scroll_to_first(self, static_cast<I16>(first + 1));            // 0041D1DB -> 0041D39A
            }
            break;
        case 5:                                                                    // 0041D31B (0x1FD)
            if (gui_m48_listbox_should_scroll_0041a290(self, nullptr)) {
                self->u2c2 = static_cast<I16>(self->u2c2 - static_cast<I16>(ftol_float(&self->scale)));   // 0041D326..31
                if (self->u2c2 < 1) self->u2c2 = 1;                                // 0041D338..4C
                gui_load_listbox_organize_oversized_0041a2d0(self, nullptr);       // 0041D353 / 0041D369
            } else if (self->first > 0) {                                          // 0041D373..7D
                const I16 moved = static_cast<I16>(self->first - static_cast<I16>(self->scroll.visible_value));   // 0041D383..8A (word SUB)
                self->first = moved;
                scroll_to_first(self, moved < 0 ? static_cast<I16>(0) : moved);    // 0041D391..97 (JNS on the word result)
            }
            break;
        case 6:                                                                    // 0041D1E1 (0x1FE)
            if (gui_m48_listbox_should_scroll_0041a290(self, nullptr)) {
                self->u2c2 = static_cast<I16>(self->u2c2 + static_cast<I16>(ftol_float(&self->scale)));   // 0041D1EC..F7 (word ADD)
                self->u2c2 = static_cast<I16>(clamp_pages(self, static_cast<I32>(self->u2c2)));   // 0041D1FE..05 -> 0041D181
                gui_load_listbox_organize_oversized_0041a2d0(self, nullptr);
            } else {
                const I32 rows = self->scroll.max_value;                           // 0041D226
                const I32 last_row = rows - 1;                                     // 0041D22F (EBP)
                const I16 first = self->first;                                     // 0041D21F
                if (static_cast<I32>(first) >= last_row) break;                    // 0041D22C..34
                const I16 last = self->last;                                       // 0041D236
                if (static_cast<I32>(last) >= static_cast<I32>(self->visible_count) + rows - 1) break;   // 0041D23D..4D
                const I32 remaining = rows - static_cast<I32>(last) - 1;           // 0041D24F..59
                const I32 page = self->scroll.visible_value;                       // 0041D253 (+168)
                if (remaining >= page) self->first = static_cast<I16>(first + page);              // 0041D271..73 (word)
                else if (remaining > 0) self->first = static_cast<I16>(first - last + rows - 1);  // 0041D25E..68 (word)
                I32 target = static_cast<I32>(self->first);                        // 0041D27A
                if (last_row < target) target = last_row;                          // 0041D281..85
                scroll_to_first(self, static_cast<I16>(target));                   // 0041D287..A9
            }
            break;
        default: break;
        }
    }
    if (self->selected != -1) {                                                    // 0041D2B2..BD
        Control* item = item_at(self, static_cast<I32>(self->selected));          // 0041D2BF..C8
        cv(item)->handle_input_event(item, nullptr, event, value);                 // 0041D2CB..D3 (vslot +3C)
    }
    reinterpret_cast<kih::Control*>(self)->HandleInputEvent(event, value);         // 0041D2D6..DE (00418750)
}
}

// ======================================================================== tooltip family
#include "53d2c083d1-startup_views.h"      // M40 canonical kotor_menu_10_19 views (D44 single definition): Tlk::simple_0041e8f0
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" void* kotor_cell_tlk_007a3a08;                                          // lane-tlk (D23)
extern "C" void* kotor_cell_rules_007a3a28;                                        // CSWRules* cell (game-prepare owner)
extern "C" {
std::int32_t __fastcall c2da_current_get_int_00413660(void* table, void*, std::int32_t row, void* column,
                                                       std::int32_t* value);     // startup-misc-c2da (D38 G8)
void* __fastcall kotor_fastcall_005ed700(void* app, void*);                        // GetClientOptions (M41 owner, alias)
Word __fastcall gui_m48_client_get_input_class_005eda70(void* app, void*);         // joined to lane-input 005EDA70
void* __fastcall gui_m48_input_get_event_005df4c0(void* input, void*, Word index, I32 input_class);   // input_m48.cpp
void __fastcall gui_m48_manager_display_tooltip_0040b490(void* manager, void*, String* text) noexcept(false);
void __fastcall gui_m48_tooltip_panel_set_text_00624ae0(void* panel, void*, String* text) noexcept(false);
void __fastcall gui_m48_tooltip_panel_position_00624af0(void* panel, void*);
Extent* __fastcall gui_m48_text_ideal_width_height_00414f10(void* text, void*, Extent* out);
}
namespace {
// CAurGUIString slots used by CSWGuiText::GetIdealWidthAndHeight: +48 font height (no args), +50 line count for a width.
struct AurStringSlots { void* s00_44[18]; I32 (__fastcall* font_height)(void*, void*); void* s4c;
                        I32 (__fastcall* lines_for_width)(void*, void*, I32 width); };
static_assert(offsetof(AurStringSlots, font_height) == 0x48 && offsetof(AurStringSlots, lines_for_width) == 0x50, "aur string slots");
const AurStringSlots* aur_string(const void* object) { return vt<AurStringSlots>(static_cast<const AurObject*>(object)); }
kotor_menu_10_19::Tlk* tlk() { return static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08); }
kotor_menu_10_19::String* as_tlk_string(String* s) { return reinterpret_cast<kotor_menu_10_19::String*>(s); }
}
extern "C" {
// ---- 00414F10 CSWGuiText::GetIdealWidthAndHeight(CSWGuiExtent* out) (RET 4) ----
Extent* __fastcall gui_m48_text_ideal_width_height_00414f10(void* text, void*, Extent* out) {
    void* aur = reinterpret_cast<void*>(word_at(text, 0x14));                      // 00414F15
    const I32 lines = aur_string(aur)->lines_for_width(aur, nullptr, static_cast<I32>(word_at(text, 0x6C)));   // 00414F18..21
    I32 lines_out = lines;                                                         // EBX
    I32 width = static_cast<I32>(word_at(text, 0x6C));                             // 00414F24
    aur = reinterpret_cast<void*>(word_at(text, 0x14));                            // 00414F27
    if (lines == aur_string(aur)->lines_for_width(aur, nullptr, width)) {          // 00414F2C..34
        for (;;) {
            width -= 10;                                                           // 00414F36
            if (width <= 0) { width = static_cast<I32>(word_at(text, 0x6C)); break; }   // 00414F39..3B / 00414F4C
            aur = reinterpret_cast<void*>(word_at(text, 0x14));                    // 00414F3D
            if (lines != aur_string(aur)->lines_for_width(aur, nullptr, width)) break;  // 00414F40..48
        }
    }
    width += 10;                                                                   // 00414F54
    aur = reinterpret_cast<void*>(word_at(text, 0x14));                            // 00414F4F
    if (aur_string(aur)->font_height(aur, nullptr) < 0x10) ++lines_out;            // 00414F52..5F
    out->left = 0;                                                                 // 00414F68
    out->top = 0;                                                                  // 00414F6A (EBP = 0)
    out->width = width;                                                            // 00414F6D
    out->height = lines_out;                                                       // 00414F73
    return out;                                                                    // EAX = out (00414F60)
}
// ---- 00624AE0 CSWGuiToolTipPanel::SetToolTipText(CExoString*): ECX += 14Ch; JMP 00415E00 CSWGuiTextParams::SetText.
void __fastcall gui_m48_tooltip_panel_set_text_00624ae0(void* panel, void*, String* text) noexcept(false) {
    gui_load_text_params_set_text_00415e00(static_cast<unsigned char*>(panel) + 0x14C, nullptr, text);   // 00624AE0..E6
}
// ---- 00624AF0 CSWGuiToolTipPanel::PositionToolTip (RET) ----
void __fastcall gui_m48_tooltip_panel_position_00624af0(void* panel, void*) {
    Extent r{0, 0, 0, 0};                                                          // 00624AF3..0C
    void* text = reinterpret_cast<void*>(word_at(panel, 0x19C));                   // 00624AF8
    if (!text) return;                                                             // 00624AFE..10
    Extent ideal;
    const Extent got = *gui_m48_text_ideal_width_height_00414f10(text, nullptr, &ideal);   // 00624B19..1E
    const I32 screen_w = static_cast<I32>(kotor_renderer_gl_cells::width_0078d1d4);  // 00624B25
    r.left = got.left;                                                             // 00624B2B
    r.top = got.top;                                                               // 00624B32
    const I32 w = got.width + 8;                                                   // 00624B36 / 00624B46
    const I32 h = got.height + 8;                                                  // 00624B43 / 00624B49
    r.height = h;                                                                  // 00624B4C
    r.width = w;                                                                   // 00624B50
    const unsigned char* manager = static_cast<const unsigned char*>(gui_manager());   // 00624B39
    I32 x = static_cast<I32>(word_at(manager, 0)) + 0x0F;                          // 00624B54..59
    I32 y = static_cast<I32>(word_at(manager, 4)) + 0x0F;                          // 00624B56..5F
    if (x + w >= screen_w) x = screen_w - w - 2;                                   // 00624B5C..6B
    const I32 screen_h = static_cast<I32>(kotor_menu_window_cells::height_0078d1d8);   // 00624B6D
    if (y + h >= screen_h) y = screen_h - h - 2;                                   // 00624B73..7F
    r.top = y;                                                                     // 00624B84
    r.left = x;                                                                    // 00624B88
    Object* child = reinterpret_cast<Object*>(static_cast<unsigned char*>(panel) + 0x64);
    child->vtbl->set_extent(child, nullptr, &r);                                   // 00624B81..94 (vslot +04)
}
// ---- 0040B490 CSWGuiManager::DisplayToolTip(CExoString*) (RET 4) ----
void __fastcall gui_m48_manager_display_tooltip_0040b490(void* manager, void*, String* text) noexcept(false) {
    if (word_at(manager, 0x4C) != 0) return;                                       // 0040B493..98
    void* app = reinterpret_cast<void*>(word_at(kotor_menu_window_cells::manager_007a39fc, 4));   // 0040B49A..9F
    const void* options = kotor_fastcall_005ed700(app, nullptr);                   // 0040B4A2
    if (!(word_at(options, 0x14) & 0x400u)) return;                                // 0040B4A7..AD
    put_word(manager, 0x1C, word_at(manager, 0x1C) | 8u);                          // 0040B4AF
    gui_m48_tooltip_panel_set_text_00624ae0(reinterpret_cast<void*>(word_at(manager, 0x3C)), nullptr, text);   // 0040B4B3..BB
    gui_m48_tooltip_panel_position_00624af0(reinterpret_cast<void*>(word_at(manager, 0x3C)), nullptr);        // 0040B4C0..C3
}
// ---- 00418A90 CSWGuiControl::DisplayToolTip (thiscall, EH states 0..6) ----
Word __fastcall gui_load_control_tooltip_00418a90(gui_load::Object* object, void*) {
    Control* self = reinterpret_cast<Control*>(object);
    String* own_text = reinterpret_cast<String*>(self->text28);                    // +28 CExoString
    if (self->u24 == 0 && own_text->BorrowedEquals005e5290("")) {                  // 00418AB8..CF (0073D71C)
        Control* parent = self->parent;                                            // 00418AD1
        if (!parent) return 0;                                                     // 00418AD4..F2
        return cv(parent)->display_tooltip(parent, nullptr);                       // 00418AD8..DA (vslot +90)
    }
    String tip;                                                                    // 00418B06..0A (005B3190)
    if (self->u24 == 0) {                                                          // 00418B0F..1C
        tip.Assign005e5c50(own_text);                                              // 00418B1E / 00418B42..47
    } else {
        String fetched;                                                            // [esp+20] (state 1)
        tlk()->simple_0041e8f0(as_tlk_string(&fetched), self->u24);               // 00418B23..2F (0041E8F0)
        tip.Assign005e5c50(&fetched);                                              // 00418B42..47
    }                                                                              // 00418B59..5D (EBX bit0 -> ~CExoString)
    if (self->u30 != 0) {                                                          // 00418B62..67
        void* app = reinterpret_cast<void*>(word_at(kotor_menu_window_cells::manager_007a39fc, 4));   // 00418B6D..73
        const Word input_class = gui_m48_client_get_input_class_005eda70(app, nullptr);   // 00418B76
        const void* event = gui_m48_input_get_event_005df4c0(kotor_menu_window_cells::input_007a39e4, nullptr,
                                                             self->u30, static_cast<I32>(input_class));   // 00418B7B..86
        if (event) {                                                               // 00418B8D..8F
            std::int32_t strref = 0;                                               // [esp+14] (written by GetINTEntry)
            std::int32_t found;
            {
                String column("KeyNameStrRef");                                    // 00418B95..9E (0073E478)
                const void* rules = kotor_cell_rules_007a3a28;                     // 00418BA6
                void* table = reinterpret_cast<void*>(word_at(reinterpret_cast<const void*>(word_at(rules, 0xB8)), 0x6C));   // 00418BB2..B8
                // .rdata word [0074D3D8] = 6 (key-id base; read-only constant of the image, read at 00418BAC).
                const Word key_base_0074d3d8 = 6u;
                found = c2da_current_get_int_00413660(table, nullptr,
                    static_cast<std::int32_t>(word_at(event, 0x1C) - key_base_0074d3d8), &column, &strref);   // 00418BA3..CD
            }                                                                      // 00418BDD
            if (found) {                                                           // 00418BE2..E4
                String separator(" : ");                                           // 00418BEA..F3 (0073E474) state 3
                String name;                                                       // [esp+30]
                tlk()->simple_0041e8f0(as_tlk_string(&name), static_cast<Word>(strref));   // 00418BF8..0D (state 3->4)
                alignas(String) unsigned char joined_storage[sizeof(String)];      // [esp+28] hidden return of operator+
                String* joined = GuiConcat005e5d10(&tip, nullptr, reinterpret_cast<String*>(joined_storage), &separator);   // 00418C14..27
                // EH states 5/6 (funclets of 00715137) are not emitted: the consumer declarations (listbox.h:220,
                // gui_objects.h:295) are extern "C" (assumed nothrow under /EHsc) and no callee throws on this path
                // (original operator new 006FA7E6 returns NULL; D30-FINAL unwind rule) - recorded open gate.
                alignas(String) unsigned char full_storage[sizeof(String)];        // [esp+20] hidden return of operator+
                String* full = GuiConcat005e5d10(joined, nullptr, reinterpret_cast<String*>(full_storage), &name);   // 00418C2C..3A
                tip.Assign005e5c50(full);                                          // 00418C3F..49
                full->~String();                                                   // 00418C4E..56
                joined->~String();                                                 // 00418C5B..64
            }                                                                      // 00418C69..80 (~name, ~separator)
        }
    }
    gui_m48_manager_display_tooltip_0040b490(gui_manager(), nullptr, &tip);        // 00418C85..90
    self->flags |= 0x40u;                                                          // 00418C95..9F
    return 1;                                                                      // 00418CA2..B5 (~tip)
}
}

// ======================================================================== ListBox proto item classes:
// CSWGuiLabelHilight (0x1B4, table 73E8E8), CSWGuiSlider (0x1B8, table 73E9D0), CSWGuiButtonToggle (0x2B4, table 73EB88)
namespace gui_lb {
struct LabelHilight : gui_load::Label {
    Border hilight;            // +140 (0041B5E2..F6) "HILIGHT" (0041BA3D)
};
static_assert(sizeof(LabelHilight) == 0x1B4 && offsetof(LabelHilight, hilight) == 0x140, "label hilight");
struct Slider : Navigable {
    I32 hit;                   // +6C HitCheckSlider result (004191F9)
    I32 max_value;             // +70 = 0 (0041BAFC)
    I32 cur_value;             // +74 = 0 (0041BB03)
    unsigned char sound;       // +78 = 1 (0041BB0A); PlayGuiSound id (0041AEAD)
    unsigned char pad79[3];
    Border border;             // +7C (0041BAC2) "BORDER"
    Border hilight;            // +F0 (0041BAD8) "HILIGHT"
    Image thumb;               // +164 (0041BAE8) "THUMB"
};
static_assert(offsetof(Slider, border) == 0x7C && offsetof(Slider, hilight) == 0xF0 && offsetof(Slider, thumb) == 0x164 &&
              sizeof(Slider) == 0x1B8, "slider");
struct ButtonToggle : gui_load::Button {
    Word event_id;             // +1C4 = 27h (0041CA8E)
    Word toggle_flags;         // +1C8 bit0 selected (0041CA7B..88)
    Border selected;           // +1CC (0041CA52) "SELECTED"
    Border hilight_selected;   // +240 (0041CA6B) "HILIGHTSELECTED"
};
static_assert(offsetof(ButtonToggle, selected) == 0x1CC && offsetof(ButtonToggle, hilight_selected) == 0x240 &&
              sizeof(ButtonToggle) == 0x2B4, "button toggle");
}
extern "C" {
extern const struct GuiM48LabelHilightMethods gui_m48_label_hilight_methods_0073e8e8;
extern const struct GuiM48SliderMethods gui_m48_slider_methods_0073e9d0;
extern const struct GuiM48ButtonToggleMethods gui_m48_button_toggle_methods_0073eb88;
void __stdcall c2da_destroy_array_006fa852(void* first, std::uint32_t size, std::int32_t count, void (__fastcall* destroy)(void*, void*));
void __fastcall gui_m48_label_hilight_destruct_0041b610(void*, void*);
void* __fastcall gui_m48_label_hilight_deleting_destruct_006267c0(LabelHilight*, void*, Word);
void __fastcall gui_m48_label_hilight_set_extent_004178e0(LabelHilight*, void*, const Extent*);
void __fastcall gui_m48_label_hilight_draw_00417880(LabelHilight*, void*, Word);
void __fastcall gui_m48_label_hilight_load_0041ba20(LabelHilight*, void*, Gff*, Struct*) noexcept(false);
void __fastcall gui_m48_label_hilight_initialize_00418d50(LabelHilight*, void*, const Extent*, LabelHilight*);
void __fastcall gui_m48_label_hilight_initialize_004177e0(LabelHilight*, void*, const Extent*, const void*, const void*, const void*);
void __fastcall gui_m48_slider_destruct_0041bb40(Slider*, void*);
void* __fastcall gui_m48_slider_deleting_destruct_0041bb20(Slider*, void*, Word);
void __fastcall gui_m48_slider_set_extent_00418f20(Slider*, void*, const Extent*);
Word __fastcall gui_m48_slider_captured_movement_00419250(Slider*, void*, Word, Word);
void __fastcall gui_m48_slider_lmouse_down_004191d0(Slider*, void*);
void __fastcall gui_m48_slider_draw_00417e60(Slider*, void*, Word);
void __fastcall gui_m48_slider_handle_input_0041adf0(Slider*, void*, I32, I32);
void __fastcall gui_m48_slider_load_0041cc30(Slider*, void*, Gff*, Struct*) noexcept(false);
void __fastcall gui_m48_slider_initialize_00418ef0(Slider*, void*, const Extent*, Slider*);
void __fastcall gui_m48_slider_initialize_00417dc0(Slider*, void*, const Extent*, const void*, const void*, menu_resources::Ref*) noexcept(false);
I32 __fastcall gui_m48_slider_hit_check_00419060(Slider*, void*, I32, I32);
void __fastcall gui_m48_slider_set_cur_00417ee0(Slider*, void*, I32);
void __fastcall gui_m48_slider_set_max_00417ec0(Slider*, void*, I32);
void __fastcall gui_m48_button_toggle_destruct_0041cad0(ButtonToggle*, void*);
void* __fastcall gui_m48_button_toggle_deleting_destruct_0041cab0(ButtonToggle*, void*, Word);
void __fastcall gui_m48_button_toggle_set_extent_00417ba0(ButtonToggle*, void*, const Extent*);
void __fastcall gui_m48_button_toggle_draw_00417b10(ButtonToggle*, void*, Word);
void __fastcall gui_m48_button_toggle_handle_input_0041adb0(ButtonToggle*, void*, I32, I32);
void __fastcall gui_m48_button_toggle_load_0041cb40(ButtonToggle*, void*, Gff*, Struct*) noexcept(false);
void __fastcall gui_m48_button_toggle_initialize_00418e40(ButtonToggle*, void*, const Extent*, ButtonToggle*);
void __fastcall gui_m48_button_toggle_initialize_00417ca0(ButtonToggle*, void*, const Extent*, const void*, const void*,
                                                          const void*, const void*, const void*) noexcept(false);
void __fastcall gui_load_button_destruct_00419da0(gui_load::Button*, void*);
void __fastcall kotor_fastcall_00417310(void* text, void*, const void* extent, void* text_params, Word scale_bits) noexcept(false);
}
namespace {
template<class T> void draw0c(T* drawable, Word delta) {                           // border/text/image vslot +0C Draw
    struct V { void* s00; void* s04; void* s08; void (__fastcall* draw)(void*, void*, Word); };
    vt<V>(drawable)->draw(drawable, nullptr, delta);
}
template<class T> void extent04(T* drawable, const Extent* e) {                    // border/text/image vslot +04 SetExtent
    struct V { void* s00; void (__fastcall* set_extent)(void*, void*, const Extent*); };
    vt<V>(drawable)->set_extent(drawable, nullptr, e);
}
template<class T> void load10(T* drawable, Gff* gff, Struct* s, const char* label) {   // border/image vslot +10 Load(gff, struct, label)
    struct V { void* s00; void* s04; void* s08; void* s0c; void (__fastcall* load)(void*, void*, Gff*, Struct*, String*); };
    String text(label);
    vt<V>(drawable)->load(drawable, nullptr, gff, s, &text);
}
Extent inner_of(Border* b) { Extent buf; return *gui_load_border_inner_extent_00415240(b, nullptr, &buf); }
// owner panel manager for the slider sound (0041AE71..A4)
void* slider_sound_manager(Slider* self) {
    Object* owner = self->owner;
    if (!owner) return nullptr;
    if (owner->vtbl->as_panel(owner, nullptr))                                     // 0041AE78..82
        return reinterpret_cast<void*>(word_at(owner->vtbl->as_panel(owner, nullptr), 0x18));   // 0041AE84..A4
    if (!owner->vtbl->as_control(owner, nullptr)) return nullptr;                  // 0041AE8B..92
    Object* control = reinterpret_cast<Object*>(static_cast<std::uintptr_t>(owner->vtbl->as_control(owner, nullptr)));   // 0041AE94..99
    Object* parent_owner = reinterpret_cast<Object*>(word_at(control, 0x34));      // 0041AE9C
    return reinterpret_cast<void*>(word_at(parent_owner->vtbl->as_panel(parent_owner, nullptr), 0x18));   // 0041AE9F..A4
}
}
extern "C" {
// ======== CSWGuiLabelHilight
// ---- 0041B5C0 ctor (EH state 0: ~CSWGuiLabel) ----
gui_load::Control* __fastcall gui_load_label_hilight_construct_0041b5c0(void* memory, void*) noexcept(false) {
    LabelHilight* self = static_cast<LabelHilight*>(memory);
    gui_load_label_construct_0041acd0(self, nullptr);                              // 0041B5DD
    try {
        set_vptr(self, &gui_m48_label_hilight_methods_0073e8e8);                  // 0041B5F0 (73E8E8)
        gui_load_border_construct_004167f0(&self->hilight, nullptr);               // 0041B5E2..F6
    } catch (...) { gui_load_label_destruct_00419d10(self, nullptr); throw; }
    return self;                                                                   // 0041B5FF
}
// ---- 0041B610 dtor ----
void __fastcall gui_m48_label_hilight_destruct_0041b610(void* object, void*) {
    LabelHilight* self = static_cast<LabelHilight*>(object);
    set_vptr(self, &gui_m48_label_hilight_methods_0073e8e8);                      // 0041B62D
    gui_load_border_destruct_004151c0(&self->hilight, nullptr);                    // 0041B633..41
    gui_load_label_destruct_00419d10(self, nullptr);                               // 0041B646..50
}
// ---- 006267C0 vector deleting destructor (RET 4): flags bit1 -> eh_vector_destructor_iterator over [this-4] count.
void* __fastcall gui_m48_label_hilight_deleting_destruct_006267c0(LabelHilight* self, void*, Word flags) {
    if (flags & 2u) {                                                              // 006267C5..CB
        unsigned char* const block = reinterpret_cast<unsigned char*>(self) - 4;   // 006267D6
        const std::int32_t count = static_cast<std::int32_t>(word_at(block, 0));   // 006267CD
        c2da_destroy_array_006fa852(self, 0x1B4, count, &gui_m48_label_hilight_destruct_0041b610);   // 006267D1..E0
        if (flags & 1u) live_free(block);                                          // 006267E5..EB
        return block;                                                              // 006267F3
    }
    gui_m48_label_hilight_destruct_0041b610(self, nullptr);                        // 006267FB..FD
    if (flags & 1u) live_free(self);                                               // 00626802..08
    return self;                                                                   // 00626810
}
// ---- 004178E0 SetExtent (RET 4) ----
void __fastcall gui_m48_label_hilight_set_extent_004178e0(LabelHilight* self, void*, const Extent* extent) {
    self->extent = *extent;                                                        // 004178E4..0B
    extent04(&self->border, &self->extent);                                        // 00417905..11
    extent04(&self->hilight, &self->extent);                                       // 00417914..23
    const Extent a = inner_of(&self->hilight);                                     // 00417926..2D
    const Extent b = inner_of(&self->border);                                      // 00417933..3A
    Extent r;
    gui_load_extent_intersection_0040a4a0(&r, &b, &a);                             // 0041793F..45
    extent04(&self->text, &r);                                                     // 0041794A..5E (+D0)
}
// ---- 00417880 Draw (RET 4) ----
void __fastcall gui_m48_label_hilight_draw_00417880(LabelHilight* self, void*, Word delta) {
    if (self->flags & 1u) draw0c(&self->hilight, delta);                           // 00417883..9B
    else draw0c(&self->border, delta);                                             // 004178B3..B9
    draw0c(&self->text, delta);                                                    // 0041789E..AB / 004178BC..C9
}
// ---- 0041BA20 Load (RET 8) ----
void __fastcall gui_m48_label_hilight_load_0041ba20(LabelHilight* self, void*, Gff* gff, Struct* s) noexcept(false) {
    load10(&self->hilight, gff, s, "HILIGHT");                                     // 0041BA3D..7D (0073E9C4)
    gui_load_label_load_0041b960(self, nullptr, gff, s);                           // 0041BA82..86
}
// ---- 00418D50 Initialize(extent, CSWGuiLabelHilight* src) (RET 8): vslot +A4(extent, &src->text params (+E8),
// &src->border params (+70), &src->hilight params (+154)).
void __fastcall gui_m48_label_hilight_initialize_00418d50(LabelHilight* self, void*, const Extent* extent, LabelHilight* src) {
    struct V { void* s[0x29]; void (__fastcall* init)(LabelHilight*, void*, const Extent*, const void*, const void*, const void*); };
    unsigned char* const p = reinterpret_cast<unsigned char*>(src);
    vt<V>(self)->init(self, nullptr, extent, p + 0xE8, p + 0x70, p + 0x154);       // 00418D50..6D
}
// ---- 004177E0 Initialize(extent, text params, border params, hilight params) (RET 10h) ----
void __fastcall gui_m48_label_hilight_initialize_004177e0(LabelHilight* self, void*, const Extent* extent,
    const void* text_params, const void* border_params, const void* hilight_params) {
    self->extent = *extent;                                                        // 004177E4..05
    gui_load_border_initialize_00415190(&self->border, nullptr, &self->extent, border_params);     // 00417808..13
    gui_load_border_initialize_00415190(&self->hilight, nullptr, &self->extent, hilight_params);   // 00417818..26
    const Extent a = inner_of(&self->hilight);                                     // 0041782B..32
    const Extent b = inner_of(&self->border);                                      // 00417837..3F
    Extent r;
    gui_load_extent_intersection_0040a4a0(&r, &b, &a);                             // 00417844..4A
    kotor_fastcall_00417310(&self->text, nullptr, &r, const_cast<void*>(text_params), 0x3F800000u);   // 0041784F..67 (1.0f)
}

// ======== CSWGuiSlider
// ---- 0041BAA0 ctor (EH states 0..2) ----
gui_load::Control* __fastcall gui_load_slider_construct_0041baa0(void* memory, void*) noexcept(false) {
    Slider* self = static_cast<Slider*>(memory);
    gui_load_navigable_construct_0041b8b0(self, nullptr);                          // 0041BABD
    try {
        set_vptr(self, &gui_m48_slider_methods_0073e9d0);                         // 0041BACD (73E9D0)
        gui_load_border_construct_004167f0(&self->border, nullptr);                // 0041BAC2..D3
        try {
            gui_load_border_construct_004167f0(&self->hilight, nullptr);           // 0041BAD8..E3
            try { gui_load_image_construct_00415a30(&self->thumb, nullptr); }      // 0041BAE8..F3
            catch (...) { gui_load_border_destruct_004151c0(&self->hilight, nullptr); throw; }
        } catch (...) { gui_load_border_destruct_004151c0(&self->border, nullptr); throw; }
    } catch (...) { gui_load_navigable_destruct_00419ce0(self, nullptr); throw; }
    self->max_value = 0;                                                           // 0041BAFC
    self->cur_value = 0;                                                           // 0041BB03
    self->sound = 1;                                                               // 0041BB0A
    return self;
}
// ---- 0041BB40 dtor: own table, members, then CSWGuiNavigable table 73E520 and ~CSWGuiControl (as compiled).
void __fastcall gui_m48_slider_destruct_0041bb40(Slider* self, void*) {
    set_vptr(self, &gui_m48_slider_methods_0073e9d0);                             // 0041BB5D
    gui_load_image_destruct_00415ad0(&self->thumb, nullptr);                       // 0041BB63..71
    gui_load_border_destruct_004151c0(&self->hilight, nullptr);                    // 0041BB76..81
    gui_load_border_destruct_004151c0(&self->border, nullptr);                     // 0041BB86..8E
    set_vptr(self, &gui_load_navigable_methods_0073e520);                         // 0041BB9D
    gui_load_control_destruct_00419a20(self, nullptr);                             // 0041BB93..A3
}
// ---- 0041BB20 scalar deleting destructor (RET 4).
void* __fastcall gui_m48_slider_deleting_destruct_0041bb20(Slider* self, void*, Word flags) {
    gui_m48_slider_destruct_0041bb40(self, nullptr);                               // 0041BB23
    if (flags & 1u) live_free(self);                                               // 0041BB28..30
    return self;
}
// ---- 00417EC0 SetMaxValue(int) (RET 4): if changed and >= 0: max = v; tail vslot +04(&extent).
void __fastcall gui_m48_slider_set_max_00417ec0(Slider* self, void*, I32 value) {
    if (self->max_value == value || value < 0) return;                             // 00417EC4..CB
    self->max_value = value;                                                       // 00417ED0
    self->vtbl->set_extent(self, nullptr, &self->extent);                          // 00417ECD..D9
}
// ---- 00417EE0 SetCurValue(int) (RET 4): clamp to [0, max]; tail vslot +04(&extent).
void __fastcall gui_m48_slider_set_cur_00417ee0(Slider* self, void*, I32 value) {
    if (value < 0) value = 0;                                                      // 00417EE4..EA
    else if (value > self->max_value) value = self->max_value;                     // 00417EEC..F3
    self->cur_value = value;                                                       // 00417EF8
    self->vtbl->set_extent(self, nullptr, &self->extent);                          // 00417EF5..01
}
// ---- 00418F20 SetExtent (RET 4) ----
void __fastcall gui_m48_slider_set_extent_00418f20(Slider* self, void*, const Extent* extent) {
    self->extent = *extent;                                                        // 00418F3A..5A
    extent04(&self->border, &self->extent);                                        // 00418F57..61
    extent04(&self->hilight, &self->extent);                                       // 00418F64..71
    Extent buf;
    const Extent img = *gui_load_image_get_extent_00415b30(&self->thumb, nullptr, &buf);   // 00418F74..A4
    Extent r = img;                                                                // [esp+10..1C]
    const I32 max = self->max_value;                                               // 00418F91
    const I32 width = self->extent.width, height = self->extent.height;            // +0C / +10
    if (max > 0) {                                                                 // 00418F94..A8
        if (height > width) {                                                      // 00418FAA..B2
            r.width = width;                                                       // 00418FB6
            r.left = self->extent.left;                                            // 00418FBA..BC
            if (height > img.height) r.top = (height - img.height) * self->cur_value / max + self->extent.top;   // 00418FC0..D0
            else { r.top = 0; r.height = height; }                                 // 00418FD6..DE
        } else {
            r.height = height;                                                     // 00418FE6
            r.top = self->extent.top;                                              // 00418FEA..ED
            if (width > img.width) r.left = (width - img.width) * self->cur_value / max + self->extent.left;   // 00418FF1..FE
            else { r.left = 0; r.width = width; }                                  // 00419000..08
        }
    } else {
        if (height > width) {                                                      // 0041900E..16
            r.width = width;                                                       // 0041901A
            if (img.height >= height) r.height = height;                           // 00419018..20
        } else {
            r.height = height;                                                     // 00419028
            if (img.width >= width) r.width = width;                               // 00419026..2E
        }
        r.top = self->extent.top;                                                  // 00419032..37
        r.left = self->extent.left;                                                // 00419035 / 0041903B
    }
    extent04(&self->thumb, &r);                                                    // 0041903F..4A
}
// ---- 00419060 HitCheckSlider(int x, int y) (RET 8): 0 none, 1 thumb, 2 before thumb, 3 after thumb.
I32 __fastcall gui_m48_slider_hit_check_00419060(Slider* self, void*, I32 x, I32 y) {
    const Extent t = self->thumb.extent;                                           // 00419069..7E (+168)
    const Extent in = inner_of(&self->border);                                     // 0041907A..A8
    if (self->extent.height > self->extent.width) {                                // 0041909E..AC
        if (y >= t.top && y <= t.height + t.top && x >= t.left && x <= t.width + t.left) return 1;   // 004190B2..DE
        if (y > in.top && y < t.top && x > in.left && x < in.width + in.left) return 2;              // 004190E1..0B
        if (y > t.height + t.top && y < in.height + in.top && x > in.left && x < in.width + in.left) return 3;   // 00419112..B6
        return 0;
    }
    if (x >= t.left && x <= t.left + t.width && y >= t.top && y <= t.height + t.top) return 1;       // 00419138..57
    if (x > in.left && x < t.left && y > in.top && y < in.height + in.top) return 2;                  // 0041915D..83
    if (x > t.width + t.left && x < in.width + in.left && y > in.top && y < in.height + in.top) return 3;   // 00419190..B6
    return 0;                                                                      // 004191B9
}
// ---- 00419250 HandleMouseCapturedMovement(int, int) (RET 8; EAX = 1) ----
Word __fastcall gui_m48_slider_captured_movement_00419250(Slider* self, void*, Word, Word) {
    const unsigned char* manager = static_cast<const unsigned char*>(gui_manager());
    if (!(manager[0x0C] & 1u)) return 1;                                           // 00419250..5F
    if (self->hit != 1) return 1;                                                  // 00419261..65
    I32 x = 0, y = 0;                                                              // [esp+10] / [esp+0C]
    gui_load_panel_local_mouse_0040ba20(self->owner, nullptr, &x, &y);             // 00419269..76
    const I32 height = self->extent.height, width = self->extent.width;            // 0041927B..7E
    I32 value;
    if (height > width) {                                                          // 00419281..83
        const I32 d = y - self->extent.top;                                        // 00419285..89
        if (d < 0) value = 0;                                                      // 0041928C..90
        else if (d > height) value = self->max_value;                              // 00419292..97
        else value = self->max_value * d / height;                                 // 00419299..9D
    } else {
        const I32 d = x - self->extent.left;                                       // 004192A1..A5
        if (d < 0) value = 0;                                                      // 004192A8
        else if (d > width) value = self->max_value;                               // 004192AA..AF
        else value = self->max_value * d / width;                                  // 004192B1..B5
    }
    gui_m48_slider_set_cur_00417ee0(self, nullptr, value);                         // 004192B7..BA
    return 1;
}
// ---- 004191D0 HandleLMouseDown (RET) ----
void __fastcall gui_m48_slider_lmouse_down_004191d0(Slider* self, void*) {
    I32 first_out = 0, second_out = 0;                                             // [esp+8] / [esp+4]
    gui_load_panel_local_mouse_0040ba20(self->owner, nullptr, &first_out, &second_out);   // 004191D6..E3
    const I32 part = gui_m48_slider_hit_check_00419060(self, nullptr, first_out, second_out);   // 004191E8..F4
    self->hit = part;                                                              // 004191F9
    if (part == 3) cv(self)->handle_input_event(self, nullptr, 0x1F4, 1);          // 00419201..0F
    else if (part == 2) cv(self)->handle_input_event(self, nullptr, 0x1F5, 1);     // 00419225..30
    static_cast<kih::Manager*>(gui_manager())->CaptureMouse(reinterpret_cast<kih::Control*>(self), 1);   // 00419212..1B / 00419233..3C
}
// ---- 00417E60 Draw (RET 4) ----
void __fastcall gui_m48_slider_draw_00417e60(Slider* self, void*, Word delta) {
    if (self->flags & 1u) draw0c(&self->hilight, delta);                           // 00417E63..7B
    else draw0c(&self->border, delta);                                             // 00417E93..99
    draw0c(&self->thumb, delta);                                                   // 00417E7E..8B / 00417E9C..A9
}
// ---- 0041ADF0 HandleInputEvent(int event, int value) (RET 8) ----
void __fastcall gui_m48_slider_handle_input_0041adf0(Slider* self, void*, I32 event, I32 value) {
    if (value != 0) {                                                              // 0041ADFA..FF
        int step = 0;                                                              // -1 decrease, +1 increase
        if (self->extent.height > self->extent.width) {                            // 0041AE05..0C
            if (event == 0x3E || event == 0x32 || event == 0x1F4) step = 1;        // 0041AE0E..1D / 0041AE3A..41
            else if (event == 0x31 || event == 0x3D || event == 0x1F5) step = -1;  // 0041AE15..22 / 0041AE43..44
        } else {
            if (event == 0x40 || event == 0x30 || event == 0x1F4) step = 1;        // 0041AE24..33
            else if (event == 0x2F || event == 0x3F || event == 0x1F5) step = -1;  // 0041AE2B..38
        }
        if (step != 0) {
            const I32 cur = self->cur_value;                                       // 0041AE46 / 0041AE55
            const bool changed = step < 0 ? cur != 0 : cur != self->max_value;     // 0041AE49..4D / 0041AE58..5F
            gui_m48_slider_set_cur_00417ee0(self, nullptr, cur + step);            // 0041AE50 / 0041AE62..68
            if (changed) {                                                         // 0041AE6D..6F
                if (void* manager = slider_sound_manager(self))                    // 0041AE71..A9
                    static_cast<kih::Manager*>(manager)->PlayGuiSound(static_cast<char>(self->sound));   // 0041AEAB..B1
            }
        }
    }
    reinterpret_cast<kih::Navigable*>(self)->HandleInputEvent(event, value);       // 0041AEB7..BB (0041A9D0)
}
// ---- 0041CC30 Load (RET 8) ----
void __fastcall gui_m48_slider_load_0041cc30(Slider* self, void*, Gff* gff, Struct* s) noexcept(false) {
    load10(&self->border, gff, s, "BORDER");                                       // 0041CC4D..87 (0073DFD4)
    load10(&self->hilight, gff, s, "HILIGHT");                                     // 0041CC8C..C4 (0073E9C4)
    load10(&self->thumb, gff, s, "THUMB");                                         // 0041CCC9..01 (0073EAC0)
    Word success = 0;                                                              // [esp+10] slot reused by the original
    gui_m48_slider_set_max_00417ec0(self, nullptr, gui_load_gff_int_00411c90(gff, nullptr, s, "MAXVALUE", &success, 0));   // 0041CD06..1D
    gui_m48_slider_set_cur_00417ee0(self, nullptr, gui_load_gff_int_00411c90(gff, nullptr, s, "CURVALUE", &success, 0));   // 0041CD22..39
    gui_load_navigable_load_0041c890(self, nullptr, gff, s);                       // 0041CD3E..42
}
// ---- 00418EF0 Initialize(extent, CSWGuiSlider* src) (RET 8): vslot +9C(extent, &src border params (+90),
// &src hilight params (+104), &src thumb image ResRef (+18C)).
void __fastcall gui_m48_slider_initialize_00418ef0(Slider* self, void*, const Extent* extent, Slider* src) {
    struct V { void* s[0x27]; void (__fastcall* init)(Slider*, void*, const Extent*, const void*, const void*, menu_resources::Ref*); };
    unsigned char* const p = reinterpret_cast<unsigned char*>(src);
    vt<V>(self)->init(self, nullptr, extent, p + 0x90, p + 0x104, reinterpret_cast<menu_resources::Ref*>(p + 0x18C));   // 00418EF0..10
}
// ---- 00417DC0 Initialize(extent, border params, hilight params, CResRef const& thumb) (RET 10h) ----
void __fastcall gui_m48_slider_initialize_00417dc0(Slider* self, void*, const Extent* extent, const void* border_params,
                                                   const void* hilight_params, menu_resources::Ref* image) noexcept(false) {
    self->extent = *extent;                                                        // 00417DC3..E3
    gui_load_border_initialize_00415190(&self->border, nullptr, &self->extent, border_params);     // 00417DE6..EF
    gui_load_border_initialize_00415190(&self->hilight, nullptr, &self->extent, hilight_params);   // 00417DF4..00
    const gui_load::Vector white{1.0f, 1.0f, 1.0f};                                // 00417E20..30
    alignas(ImageParams) unsigned char params_storage[sizeof(ImageParams)];        // [esp+14] (constructed in place)
    ImageParams* params = gui_load_image_params_construct_00414d30(reinterpret_cast<ImageParams*>(params_storage), nullptr,
        image, 0, 0, 0, 0x12, 0x3F800000u, &white);                                // 00417E05..38 (ref, 0, 0, 0, 12h, 1.0f, &white)
    gui_load_image_initialize_00416e90(&self->thumb, nullptr, &self->extent, params);   // 00417E3D..49 (no destructor call in the original)
    self->vtbl->set_extent(self, nullptr, &self->extent);                          // 00417E4E..53
}

// ======== CSWGuiButtonToggle
// ---- 0041CA30 ctor (EH states 0..1) ----
gui_load::Control* __fastcall gui_load_button_toggle_construct_0041ca30(void* memory, void*) noexcept(false) {
    ButtonToggle* self = static_cast<ButtonToggle*>(memory);
    gui_load_button_construct_0041c0f0(self, nullptr);                             // 0041CA4D
    try {
        set_vptr(self, &gui_m48_button_toggle_methods_0073eb88);                  // 0041CA60 (73EB88)
        gui_load_border_construct_004167f0(&self->selected, nullptr);              // 0041CA52..66
        try { gui_load_border_construct_004167f0(&self->hilight_selected, nullptr); }   // 0041CA6B..76
        catch (...) { gui_load_border_destruct_004151c0(&self->selected, nullptr); throw; }
    } catch (...) { gui_load_button_destruct_00419da0(self, nullptr); throw; }
    self->toggle_flags &= 0xFFFFFFFEu;                                             // 0041CA7B..88
    self->event_id = 0x27;                                                         // 0041CA8E
    self->b54 = 3;                                                                 // 0041CA98
    return self;
}
// ---- 0041CAD0 dtor ----
void __fastcall gui_m48_button_toggle_destruct_0041cad0(ButtonToggle* self, void*) {
    set_vptr(self, &gui_m48_button_toggle_methods_0073eb88);                      // 0041CAED
    gui_load_border_destruct_004151c0(&self->hilight_selected, nullptr);           // 0041CAF3..01
    gui_load_border_destruct_004151c0(&self->selected, nullptr);                   // 0041CB06..11
    gui_load_button_destruct_00419da0(self, nullptr);                              // 0041CB16..20
}
// ---- 0041CAB0 scalar deleting destructor (RET 4).
void* __fastcall gui_m48_button_toggle_deleting_destruct_0041cab0(ButtonToggle* self, void*, Word flags) {
    gui_m48_button_toggle_destruct_0041cad0(self, nullptr);                        // 0041CAB3
    if (flags & 1u) live_free(self);                                               // 0041CAB8..C0
    return self;
}
// ---- 00417BA0 SetExtent (RET 4). The nested intersections of the four inner extents are computed and their
// result is not used (as compiled: 00417C41..7B); the text receives the incoming extent (00417C8A).
void __fastcall gui_m48_button_toggle_set_extent_00417ba0(ButtonToggle* self, void*, const Extent* extent) {
    self->extent = *extent;                                                        // 00417BA9..CE
    extent04(&self->border, extent);                                               // 00417BC8..D4 (+6C)
    extent04(&self->hilight, extent);                                              // 00417BD7..E6 (+E0)
    extent04(&self->selected, extent);                                             // 00417BE9..F6 (+1CC)
    extent04(&self->hilight_selected, extent);                                     // 00417BF9..06 (+240)
    const Extent b0 = inner_of(&self->border);                                     // 00417C09..10
    const Extent b1 = inner_of(&self->hilight);                                    // 00417C15..1C
    const Extent b2 = inner_of(&self->selected);                                   // 00417C21..2C
    const Extent b3 = inner_of(&self->hilight_selected);                           // 00417C31..3C
    Extent r1, r2, r3;
    gui_load_extent_intersection_0040a4a0(&r1, &b2, &b3);                          // 00417C41..50
    gui_load_extent_intersection_0040a4a0(&r2, &b0, &b1);                          // 00417C58..68
    gui_load_extent_intersection_0040a4a0(&r3, &r2, &r1);                          // 00417C70..76
    extent04(&self->text, extent);                                                 // 00417C7B..8B (+154)
}
// ---- 00417B10 Draw (RET 4) ----
void __fastcall gui_m48_button_toggle_draw_00417b10(ButtonToggle* self, void*, Word delta) {
    const bool selected = (self->toggle_flags & 1u) != 0;                          // 00417B13..20 (byte test)
    const bool active = (self->flags & 1u) != 0;                                   // 00417B22 (byte test)
    if (selected) {
        if (active) draw0c(&self->hilight_selected, delta);                        // 00417B28..38
        else draw0c(&self->selected, delta);                                       // 00417B50 -> 00417B83
    } else {
        if (active) draw0c(&self->hilight, delta);                                 // 00417B58..68
        else draw0c(&self->border, delta);                                         // 00417B80..85
    }
    draw0c(&self->text, delta);                                                    // +154 vslot +0C
}
// ---- 0041ADB0 HandleInputEvent(int, int) (RET 8) ----
void __fastcall gui_m48_button_toggle_handle_input_0041adb0(ButtonToggle* self, void*, I32 event, I32 value) {
    if (value != 0 && static_cast<Word>(event) == self->event_id)                  // 0041ADB4..C3
        self->toggle_flags ^= 1u;                                                  // 0041ADC5..D7
    reinterpret_cast<kih::Button*>(self)->HandleInputEvent(event, value);          // 0041ADDE..E0 (0041AD40)
}
// ---- 0041CB40 Load (RET 8) ----
void __fastcall gui_m48_button_toggle_load_0041cb40(ButtonToggle* self, void*, Gff* gff, Struct* s) noexcept(false) {
    load10(&self->selected, gff, s, "SELECTED");                                   // 0041CB5D..9D (0073EC4C)
    load10(&self->hilight_selected, gff, s, "HILIGHTSELECTED");                    // 0041CBA2..DA (0073EC3C)
    Word success = 0;                                                              // [esp+10] slot reused by the original
    const unsigned char selected = gui_load_gff_byte_00411a60(gff, nullptr, s, "ISSELECTED", &success, 0);   // 0041CBDF..EE (0073EC30)
    const Word flags = self->toggle_flags;                                         // 0041CBF3
    self->toggle_flags = ((selected ^ flags) & 1u) ^ flags;                        // 0041CBFD..04 (EAX = AL with upper bits of the return)
    gui_load_button_load_0041c930(self, nullptr, gff, s);                          // 0041CBF9..0A
}
// ---- 00418E40 Initialize(extent, CSWGuiButtonToggle* src) (RET 8): vslot +A4(extent, &src text params (+16C),
// &src border params (+80), &src hilight params (+F4), &src selected params (+1E0), &src hilight-selected params (+254)).
void __fastcall gui_m48_button_toggle_initialize_00418e40(ButtonToggle* self, void*, const Extent* extent, ButtonToggle* src) {
    struct V { void* s[0x29]; void (__fastcall* init)(ButtonToggle*, void*, const Extent*, const void*, const void*,
                                                       const void*, const void*, const void*); };
    unsigned char* const p = reinterpret_cast<unsigned char*>(src);
    vt<V>(self)->init(self, nullptr, extent, p + 0x16C, p + 0x80, p + 0xF4, p + 0x1E0, p + 0x254);   // 00418E40..6E
}
// ---- 00417CA0 Initialize(extent, text params, four border params) (RET 18h) ----
void __fastcall gui_m48_button_toggle_initialize_00417ca0(ButtonToggle* self, void*, const Extent* extent, const void* text_params,
    const void* border_params, const void* hilight_params, const void* selected_params, const void* hilight_selected_params) noexcept(false) {
    self->extent = *extent;                                                        // 00417CA4..C5
    gui_load_border_initialize_00415190(&self->border, nullptr, &self->extent, border_params);              // 00417CC8..D6
    gui_load_border_initialize_00415190(&self->hilight, nullptr, &self->extent, hilight_params);            // 00417CDB..EC
    gui_load_border_initialize_00415190(&self->selected, nullptr, &self->extent, selected_params);          // 00417CF1..00
    gui_load_border_initialize_00415190(&self->hilight_selected, nullptr, &self->extent, hilight_selected_params);   // 00417D05..14
    const Extent b0 = inner_of(&self->border);                                     // 00417D19..20
    const Extent b1 = inner_of(&self->hilight);                                    // 00417D25..2C
    const Extent b2 = inner_of(&self->selected);                                   // 00417D31..3C
    const Extent b3 = inner_of(&self->hilight_selected);                           // 00417D41..4C
    Extent r1, r2, r3;
    gui_load_extent_intersection_0040a4a0(&r1, &b2, &b3);                          // 00417D51..60
    gui_load_extent_intersection_0040a4a0(&r2, &b0, &b1);                          // 00417D68..78
    gui_load_extent_intersection_0040a4a0(&r3, &r2, &r1);                          // 00417D80..86
    kotor_fastcall_00417310(&self->text, nullptr, &r3, const_cast<void*>(text_params), 0x3F800000u);   // 00417D8B..A6
}
}
// gui_m48_label_hilight_methods_0073e8e8: original table 0073E8E8 (42 slots)
struct GuiM48LabelHilightMethods {
    void* (__fastcall* s00)(gui_lb::LabelHilight*, void*, gui_load::Word);  // +00 006267C0 CSWGuiLabelHilight::~CSWGuiLabelHilight
    void (__fastcall* s04)(gui_lb::LabelHilight*, void*, const gui_load::Extent*);  // +04 004178E0 CSWGuiLabelHilight::SetExtent
    void (__fastcall* s08)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +08 005B5E90 void_return
    void (kih::Control::* s0c)(int);  // +0C 004189A0 CSWGuiControl::HandleMouseOver
    gui_load::Word (__fastcall* s10)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +10 00409EB0 return_zero_0x8
    void (__fastcall* s14)(gui_load::Object*, void*);  // +14 004015A0 void_return
    void (kih::Control::* s18)();  // +18 0041AB90 CSWGuiControl::HandleLMouseDown
    void (kih::Control::* s1c)();  // +1C 0041ABD0 CSWGuiControl::HandleLMouseUp
    void (__fastcall* s20)(gui_load::Object*, void*);  // +20 004015A0 void_return
    void (__fastcall* s24)(gui_load::Object*, void*);  // +24 0041AC70 CSWGuiControl::HandleRMouseUp
    gui_load::Word (__fastcall* s28)(gui_load::Object*, void*);  // +28 0063E7F0 return_zero
    gui_load::Word (__fastcall* s2c)(gui_load::Object*, void*);  // +2C 0063E7F0 return_zero
    gui_load::Object* (__fastcall* s30)(gui_load::Object*, void*);  // +30 00641DB0 self_return
    gui_load::Word (__fastcall* s34)(gui_load::Object*, void*);  // +34 004064A0 GetProperty0x10
    void (__fastcall* s38)(gui_lb::LabelHilight*, void*, gui_load::Word);  // +38 00417880 CSWGuiLabelHilight::Draw
    void (kih::Control::* s3c)(int, int);  // +3C 00418750 CSWGuiControl::HandleInputEvent
    void (kih::Control::* s40)(int);  // +40 004181D0 CSWGuiControl::SetActive
    kih::Control* (kih::Control::* s44)(int, int);  // +44 004187C0 CSWGuiControl::HitCheckMouse
    void (__fastcall* s48)(gui_lb::LabelHilight*, void*, gui_load::Gff*, gui_load::Struct*);  // +48 0041BA20 CSWGuiLabelHilight::Load
    gui_load::Word (__fastcall* s4c)(gui_load::Object*, void*);  // +4C 0063E7F0 return_zero
    gui_load::Object* (__fastcall* s50)(gui_load::Object*, void*);  // +50 00641DB0 self_return
    gui_load::Object* (__fastcall* s54)(gui_load::Object*, void*);  // +54 00641DB0 self_return
    gui_load::Word (__fastcall* s58)(gui_load::Object*, void*);  // +58 0063E7F0 return_zero
    gui_load::Word (__fastcall* s5c)(gui_load::Object*, void*);  // +5C 0063E7F0 return_zero
    gui_load::Word (__fastcall* s60)(gui_load::Object*, void*);  // +60 0063E7F0 return_zero
    gui_load::Word (__fastcall* s64)(gui_load::Object*, void*);  // +64 0063E7F0 return_zero
    gui_load::Word (__fastcall* s68)(gui_load::Object*, void*);  // +68 0063E7F0 return_zero
    gui_load::Word (__fastcall* s6c)(gui_load::Object*, void*);  // +6C 0063E7F0 return_zero
    gui_load::Word (__fastcall* s70)(gui_load::Object*, void*);  // +70 0063E7F0 return_zero
    gui_load::Word (__fastcall* s74)(gui_load::Object*, void*);  // +74 0063E7F0 return_zero
    gui_load::Word (__fastcall* s78)(gui_load::Object*, void*);  // +78 0063E7F0 return_zero
    void (__fastcall* s7c)(gui_load::Object*, void*, gui_load::Word);  // +7C 00418960 CSWGuiControl::HandleFocusChange
    int (kih::Control::* s80)();  // +80 004189D0 CSWGuiControl::GetIsSelectable
    void (__fastcall* s84)(gui_load::Object*, void*);  // +84 0041AC30 CSWGuiControl::HandleRMouseDown
    void (__fastcall* s88)(gui_load::Object*, void*, gui_load::Word);  // +88 00418D00 CSWGuiLabel::SetEnabled
    void (__fastcall* s8c)(gui_load::Object*, void*);  // +8C 00418D40 CSWGuiLabel::ReSetFont
    gui_load::Word (__fastcall* s90)(gui_load::Object*, void*);  // +90 00418A90 CSWGuiControl::DisplayToolTip
    void (__fastcall* s94)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +94 005B5E90 void_return
    void (__fastcall* s98)(gui_load::Object*, void*, gui_load::Word, gui_load::Word, gui_load::Word);  // +98 00418CD0 CSWGuiLabel::Initialize
    void (__fastcall* s9c)(gui_load::Object*, void*, gui_load::Word, gui_load::Word, gui_load::Word, gui_load::Word);  // +9C 004176E0 CSWGuiLabel::Initialize
    void (__fastcall* sa0)(gui_lb::LabelHilight*, void*, const gui_load::Extent*, gui_lb::LabelHilight*);  // +A0 00418D50 CSWGuiLabelHilight::Initialize
    void (__fastcall* sa4)(gui_lb::LabelHilight*, void*, const gui_load::Extent*, const void*, const void*, const void*);  // +A4 004177E0 CSWGuiLabelHilight::Initialize
};
static_assert(sizeof(GuiM48LabelHilightMethods) == 168, "73E8E8 slot count");
extern "C" const GuiM48LabelHilightMethods gui_m48_label_hilight_methods_0073e8e8 = {
    &gui_m48_label_hilight_deleting_destruct_006267c0,  // +00
    &gui_m48_label_hilight_set_extent_004178e0,  // +04
    &gui_load_slot_ret8_005b5e90,  // +08
    &kih::Control::HandleMouseOver,  // +0C
    &gui_load_slot_zero8_00409eb0,  // +10
    &gui_load_slot_ret_004015a0,  // +14
    &kih::Control::HandleLMouseDown,  // +18
    &kih::Control::HandleLMouseUp,  // +1C
    &gui_load_slot_ret_004015a0,  // +20
    &gui_load_control_handle_rmouse_up_0041ac70,  // +24
    &gui_load_slot_zero_0063e7f0,  // +28
    &gui_load_slot_zero_0063e7f0,  // +2C
    &gui_load_slot_self_00641db0,  // +30
    &gui_load_slot_prop10_004064a0,  // +34
    &gui_m48_label_hilight_draw_00417880,  // +38
    &kih::Control::HandleInputEvent,  // +3C
    &kih::Control::SetActive,  // +40
    &kih::Control::HitCheckMouse,  // +44
    &gui_m48_label_hilight_load_0041ba20,  // +48
    &gui_load_slot_zero_0063e7f0,  // +4C
    &gui_load_slot_self_00641db0,  // +50
    &gui_load_slot_self_00641db0,  // +54
    &gui_load_slot_zero_0063e7f0,  // +58
    &gui_load_slot_zero_0063e7f0,  // +5C
    &gui_load_slot_zero_0063e7f0,  // +60
    &gui_load_slot_zero_0063e7f0,  // +64
    &gui_load_slot_zero_0063e7f0,  // +68
    &gui_load_slot_zero_0063e7f0,  // +6C
    &gui_load_slot_zero_0063e7f0,  // +70
    &gui_load_slot_zero_0063e7f0,  // +74
    &gui_load_slot_zero_0063e7f0,  // +78
    &gui_load_control_focus_change_00418960,  // +7C
    &kih::Control::GetIsSelectable,  // +80
    &gui_load_control_rmouse_down_0041ac30,  // +84
    &gui_load_label_set_enabled_00418d00,  // +88
    &gui_load_label_reset_font_00418d40,  // +8C
    &gui_load_control_tooltip_00418a90,  // +90
    &gui_load_slot_ret8_005b5e90,  // +94
    &gui_load_label_initialize_00418cd0,  // +98
    &gui_load_label_initialize_004176e0,  // +9C
    &gui_m48_label_hilight_initialize_00418d50,  // +A0
    &gui_m48_label_hilight_initialize_004177e0,  // +A4
};

// gui_m48_slider_methods_0073e9d0: original table 0073E9D0 (40 slots)
struct GuiM48SliderMethods {
    void* (__fastcall* s00)(gui_lb::Slider*, void*, gui_load::Word);  // +00 0041BB20 CSWGuiSlider::~CSWGuiSlider
    void (__fastcall* s04)(gui_lb::Slider*, void*, const gui_load::Extent*);  // +04 00418F20 CSWGuiSlider::SetExtent
    void (__fastcall* s08)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +08 005B5E90 void_return
    void (kih::Navigable::* s0c)(int);  // +0C 0041A980 CSWGuiNavigable::HandleMouseOver
    gui_load::Word (__fastcall* s10)(gui_lb::Slider*, void*, gui_load::Word, gui_load::Word);  // +10 00419250 CSWGuiSlider::HandleMouseCapturedMovement
    void (__fastcall* s14)(gui_load::Object*, void*);  // +14 004015A0 void_return
    void (__fastcall* s18)(gui_lb::Slider*, void*);  // +18 004191D0 CSWGuiSlider::HandleLMouseDown
    void (kih::Control::* s1c)();  // +1C 0041ABD0 CSWGuiControl::HandleLMouseUp
    void (__fastcall* s20)(gui_load::Object*, void*);  // +20 004015A0 void_return
    void (__fastcall* s24)(gui_load::Object*, void*);  // +24 0041AC70 CSWGuiControl::HandleRMouseUp
    gui_load::Word (__fastcall* s28)(gui_load::Object*, void*);  // +28 0063E7F0 return_zero
    gui_load::Word (__fastcall* s2c)(gui_load::Object*, void*);  // +2C 0063E7F0 return_zero
    gui_load::Object* (__fastcall* s30)(gui_load::Object*, void*);  // +30 00641DB0 self_return
    gui_load::Word (__fastcall* s34)(gui_load::Object*, void*);  // +34 004064A0 GetProperty0x10
    void (__fastcall* s38)(gui_lb::Slider*, void*, gui_load::Word);  // +38 00417E60 CSWGuiSlider::Draw
    void (__fastcall* s3c)(gui_lb::Slider*, void*, gui_lb::I32, gui_lb::I32);  // +3C 0041ADF0 CSWGuiSlider::HandleInputEvent
    void (kih::Control::* s40)(int);  // +40 004181D0 CSWGuiControl::SetActive
    kih::Control* (kih::Control::* s44)(int, int);  // +44 004187C0 CSWGuiControl::HitCheckMouse
    void (__fastcall* s48)(gui_lb::Slider*, void*, gui_load::Gff*, gui_load::Struct*);  // +48 0041CC30 CSWGuiSlider::Load
    gui_load::Object* (__fastcall* s4c)(gui_load::Object*, void*);  // +4C 00641DB0 self_return
    gui_load::Word (__fastcall* s50)(gui_load::Object*, void*);  // +50 0063E7F0 return_zero
    gui_load::Word (__fastcall* s54)(gui_load::Object*, void*);  // +54 0063E7F0 return_zero
    gui_load::Word (__fastcall* s58)(gui_load::Object*, void*);  // +58 0063E7F0 return_zero
    gui_load::Word (__fastcall* s5c)(gui_load::Object*, void*);  // +5C 0063E7F0 return_zero
    gui_load::Word (__fastcall* s60)(gui_load::Object*, void*);  // +60 0063E7F0 return_zero
    gui_load::Object* (__fastcall* s64)(gui_load::Object*, void*);  // +64 00641DB0 self_return
    gui_load::Word (__fastcall* s68)(gui_load::Object*, void*);  // +68 0063E7F0 return_zero
    gui_load::Word (__fastcall* s6c)(gui_load::Object*, void*);  // +6C 0063E7F0 return_zero
    gui_load::Word (__fastcall* s70)(gui_load::Object*, void*);  // +70 0063E7F0 return_zero
    gui_load::Word (__fastcall* s74)(gui_load::Object*, void*);  // +74 0063E7F0 return_zero
    gui_load::Word (__fastcall* s78)(gui_load::Object*, void*);  // +78 0063E7F0 return_zero
    void (__fastcall* s7c)(gui_load::Object*, void*, gui_load::Word);  // +7C 00418960 CSWGuiControl::HandleFocusChange
    int (kih::Control::* s80)();  // +80 004189D0 CSWGuiControl::GetIsSelectable
    void (__fastcall* s84)(gui_load::Object*, void*);  // +84 0041AC30 CSWGuiControl::HandleRMouseDown
    void (__fastcall* s88)(gui_load::Object*, void*, gui_load::Word);  // +88 004176A0 CSWGuiControl::SetEnabled
    void (__fastcall* s8c)(gui_load::Object*, void*);  // +8C 004015A0 void_return
    gui_load::Word (__fastcall* s90)(gui_load::Object*, void*);  // +90 00418A90 CSWGuiControl::DisplayToolTip
    void (__fastcall* s94)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +94 005B5E90 void_return
    void (__fastcall* s98)(gui_lb::Slider*, void*, const gui_load::Extent*, gui_lb::Slider*);  // +98 00418EF0 CSWGuiSlider::Initialize
    void (__fastcall* s9c)(gui_lb::Slider*, void*, const gui_load::Extent*, const void*, const void*, menu_resources::Ref*);  // +9C 00417DC0 CSWGuiSlider::Initialize
};
static_assert(sizeof(GuiM48SliderMethods) == 160, "73E9D0 slot count");
extern "C" const GuiM48SliderMethods gui_m48_slider_methods_0073e9d0 = {
    &gui_m48_slider_deleting_destruct_0041bb20,  // +00
    &gui_m48_slider_set_extent_00418f20,  // +04
    &gui_load_slot_ret8_005b5e90,  // +08
    &kih::Navigable::HandleMouseOver,  // +0C
    &gui_m48_slider_captured_movement_00419250,  // +10
    &gui_load_slot_ret_004015a0,  // +14
    &gui_m48_slider_lmouse_down_004191d0,  // +18
    &kih::Control::HandleLMouseUp,  // +1C
    &gui_load_slot_ret_004015a0,  // +20
    &gui_load_control_handle_rmouse_up_0041ac70,  // +24
    &gui_load_slot_zero_0063e7f0,  // +28
    &gui_load_slot_zero_0063e7f0,  // +2C
    &gui_load_slot_self_00641db0,  // +30
    &gui_load_slot_prop10_004064a0,  // +34
    &gui_m48_slider_draw_00417e60,  // +38
    &gui_m48_slider_handle_input_0041adf0,  // +3C
    &kih::Control::SetActive,  // +40
    &kih::Control::HitCheckMouse,  // +44
    &gui_m48_slider_load_0041cc30,  // +48
    &gui_load_slot_self_00641db0,  // +4C
    &gui_load_slot_zero_0063e7f0,  // +50
    &gui_load_slot_zero_0063e7f0,  // +54
    &gui_load_slot_zero_0063e7f0,  // +58
    &gui_load_slot_zero_0063e7f0,  // +5C
    &gui_load_slot_zero_0063e7f0,  // +60
    &gui_load_slot_self_00641db0,  // +64
    &gui_load_slot_zero_0063e7f0,  // +68
    &gui_load_slot_zero_0063e7f0,  // +6C
    &gui_load_slot_zero_0063e7f0,  // +70
    &gui_load_slot_zero_0063e7f0,  // +74
    &gui_load_slot_zero_0063e7f0,  // +78
    &gui_load_control_focus_change_00418960,  // +7C
    &kih::Control::GetIsSelectable,  // +80
    &gui_load_control_rmouse_down_0041ac30,  // +84
    &gui_load_control_set_enabled_004176a0,  // +88
    &gui_load_slot_ret_004015a0,  // +8C
    &gui_load_control_tooltip_00418a90,  // +90
    &gui_load_slot_ret8_005b5e90,  // +94
    &gui_m48_slider_initialize_00418ef0,  // +98
    &gui_m48_slider_initialize_00417dc0,  // +9C
};

// gui_m48_button_toggle_methods_0073eb88: original table 0073EB88 (42 slots)
struct GuiM48ButtonToggleMethods {
    void* (__fastcall* s00)(gui_lb::ButtonToggle*, void*, gui_load::Word);  // +00 0041CAB0 CSWGuiButtonToggle::~CSWGuiButtonToggle
    void (__fastcall* s04)(gui_lb::ButtonToggle*, void*, const gui_load::Extent*);  // +04 00417BA0 CSWGuiButtonToggle::SetExtent
    void (__fastcall* s08)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +08 005B5E90 void_return
    void (kih::Navigable::* s0c)(int);  // +0C 0041A980 CSWGuiNavigable::HandleMouseOver
    gui_load::Word (__fastcall* s10)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +10 00409EB0 return_zero_0x8
    void (__fastcall* s14)(gui_load::Object*, void*);  // +14 004015A0 void_return
    void (kih::Control::* s18)();  // +18 0041AB90 CSWGuiControl::HandleLMouseDown
    void (kih::Control::* s1c)();  // +1C 0041ABD0 CSWGuiControl::HandleLMouseUp
    void (__fastcall* s20)(gui_load::Object*, void*);  // +20 004015A0 void_return
    void (__fastcall* s24)(gui_load::Object*, void*);  // +24 0041AC70 CSWGuiControl::HandleRMouseUp
    gui_load::Word (__fastcall* s28)(gui_load::Object*, void*);  // +28 0063E7F0 return_zero
    gui_load::Word (__fastcall* s2c)(gui_load::Object*, void*);  // +2C 0063E7F0 return_zero
    gui_load::Object* (__fastcall* s30)(gui_load::Object*, void*);  // +30 00641DB0 self_return
    gui_load::Word (__fastcall* s34)(gui_load::Object*, void*);  // +34 004064A0 GetProperty0x10
    void (__fastcall* s38)(gui_lb::ButtonToggle*, void*, gui_load::Word);  // +38 00417B10 CSWGuiButtonToggle::Draw
    void (__fastcall* s3c)(gui_lb::ButtonToggle*, void*, gui_lb::I32, gui_lb::I32);  // +3C 0041ADB0 CSWGuiButtonToggle::HandleInputEvent
    void (kih::Button::* s40)(int);  // +40 00418E00 CSWGuiButton::SetActive
    kih::Control* (kih::Control::* s44)(int, int);  // +44 004187C0 CSWGuiControl::HitCheckMouse
    void (__fastcall* s48)(gui_lb::ButtonToggle*, void*, gui_load::Gff*, gui_load::Struct*);  // +48 0041CB40 CSWGuiButtonToggle::Load
    gui_load::Object* (__fastcall* s4c)(gui_load::Object*, void*);  // +4C 00641DB0 self_return
    gui_load::Word (__fastcall* s50)(gui_load::Object*, void*);  // +50 0063E7F0 return_zero
    gui_load::Word (__fastcall* s54)(gui_load::Object*, void*);  // +54 0063E7F0 return_zero
    gui_load::Object* (__fastcall* s58)(gui_load::Object*, void*);  // +58 00641DB0 self_return
    gui_load::Object* (__fastcall* s5c)(gui_load::Object*, void*);  // +5C 00641DB0 self_return
    gui_load::Word (__fastcall* s60)(gui_load::Object*, void*);  // +60 0063E7F0 return_zero
    gui_load::Word (__fastcall* s64)(gui_load::Object*, void*);  // +64 0063E7F0 return_zero
    gui_load::Word (__fastcall* s68)(gui_load::Object*, void*);  // +68 0063E7F0 return_zero
    gui_load::Word (__fastcall* s6c)(gui_load::Object*, void*);  // +6C 0063E7F0 return_zero
    gui_load::Word (__fastcall* s70)(gui_load::Object*, void*);  // +70 0063E7F0 return_zero
    gui_load::Word (__fastcall* s74)(gui_load::Object*, void*);  // +74 0063E7F0 return_zero
    gui_load::Word (__fastcall* s78)(gui_load::Object*, void*);  // +78 0063E7F0 return_zero
    void (__fastcall* s7c)(gui_load::Object*, void*, gui_load::Word);  // +7C 00418960 CSWGuiControl::HandleFocusChange
    int (kih::Control::* s80)();  // +80 004189D0 CSWGuiControl::GetIsSelectable
    void (__fastcall* s84)(gui_load::Object*, void*);  // +84 0041AC30 CSWGuiControl::HandleRMouseDown
    void (__fastcall* s88)(gui_load::Object*, void*, gui_load::Word);  // +88 00418DB0 CSWGuiButton::SetEnabled
    void (__fastcall* s8c)(gui_load::Object*, void*);  // +8C 00418DF0 CSWGuiButton::ReSetFont
    gui_load::Word (__fastcall* s90)(gui_load::Object*, void*);  // +90 00418A90 CSWGuiControl::DisplayToolTip
    void (__fastcall* s94)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +94 005B5E90 void_return
    void (__fastcall* s98)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +98 00418D80 CSWGuiButton::Initialize
    void (__fastcall* s9c)(gui_load::Object*, void*, gui_load::Word, gui_load::Word, gui_load::Word, gui_load::Word);  // +9C 00417970 CSWGuiButton::Initialize
    void (__fastcall* sa0)(gui_lb::ButtonToggle*, void*, const gui_load::Extent*, gui_lb::ButtonToggle*);  // +A0 00418E40 CSWGuiButtonToggle::Initialize
    void (__fastcall* sa4)(gui_lb::ButtonToggle*, void*, const gui_load::Extent*, const void*, const void*, const void*, const void*, const void*);  // +A4 00417CA0 CSWGuiButtonToggle::Initialize
};
static_assert(sizeof(GuiM48ButtonToggleMethods) == 168, "73EB88 slot count");
extern "C" const GuiM48ButtonToggleMethods gui_m48_button_toggle_methods_0073eb88 = {
    &gui_m48_button_toggle_deleting_destruct_0041cab0,  // +00
    &gui_m48_button_toggle_set_extent_00417ba0,  // +04
    &gui_load_slot_ret8_005b5e90,  // +08
    &kih::Navigable::HandleMouseOver,  // +0C
    &gui_load_slot_zero8_00409eb0,  // +10
    &gui_load_slot_ret_004015a0,  // +14
    &kih::Control::HandleLMouseDown,  // +18
    &kih::Control::HandleLMouseUp,  // +1C
    &gui_load_slot_ret_004015a0,  // +20
    &gui_load_control_handle_rmouse_up_0041ac70,  // +24
    &gui_load_slot_zero_0063e7f0,  // +28
    &gui_load_slot_zero_0063e7f0,  // +2C
    &gui_load_slot_self_00641db0,  // +30
    &gui_load_slot_prop10_004064a0,  // +34
    &gui_m48_button_toggle_draw_00417b10,  // +38
    &gui_m48_button_toggle_handle_input_0041adb0,  // +3C
    &kih::Button::SetActive,  // +40
    &kih::Control::HitCheckMouse,  // +44
    &gui_m48_button_toggle_load_0041cb40,  // +48
    &gui_load_slot_self_00641db0,  // +4C
    &gui_load_slot_zero_0063e7f0,  // +50
    &gui_load_slot_zero_0063e7f0,  // +54
    &gui_load_slot_self_00641db0,  // +58
    &gui_load_slot_self_00641db0,  // +5C
    &gui_load_slot_zero_0063e7f0,  // +60
    &gui_load_slot_zero_0063e7f0,  // +64
    &gui_load_slot_zero_0063e7f0,  // +68
    &gui_load_slot_zero_0063e7f0,  // +6C
    &gui_load_slot_zero_0063e7f0,  // +70
    &gui_load_slot_zero_0063e7f0,  // +74
    &gui_load_slot_zero_0063e7f0,  // +78
    &gui_load_control_focus_change_00418960,  // +7C
    &kih::Control::GetIsSelectable,  // +80
    &gui_load_control_rmouse_down_0041ac30,  // +84
    &gui_load_button_set_enabled_00418db0,  // +88
    &gui_load_button_reset_font_00418df0,  // +8C
    &gui_load_control_tooltip_00418a90,  // +90
    &gui_load_slot_ret8_005b5e90,  // +94
    &gui_load_button_initialize_00418d80,  // +98
    &gui_load_button_initialize_00417970,  // +9C
    &gui_m48_button_toggle_initialize_00418e40,  // +A0
    &gui_m48_button_toggle_initialize_00417ca0,  // +A4
};

// ======================================================================== CSWGuiManager::DrawSafeRegion 0040BEC0 + CSWGuiPanel::AddControl 0040B9A0
extern "C" {
void __fastcall gui_m48_panel_add_control_0040b9a0(gui_load::Panel* panel, void*, Control* control) noexcept(false);
void __fastcall frame_LogStringDtor_005e5c20(void* self, void*);                   // lane-r4-lowers r4_log_joins.cpp (005E5C20)
void __fastcall kotor_fastcall_0059ea30(void* self, void*);                         // lane-menu-tables (D59): jmp 005E5C20
}
namespace {
struct SafeFreeOnUnwind { void* block; ~SafeFreeOnUnwind() { if (block) live_free(block); } };
struct SafeTextParamsOnUnwind { void* params; ~SafeTextParamsOnUnwind() { if (params) kotor_fastcall_0059ea30(params, nullptr); } };
struct alignas(4) SafeParamsStorage { unsigned char bytes[0x58]; };
}
extern "C" {
// ---- 0040B9A0 CSWGuiPanel::AddControl(CSWGuiControl*) (RET 4) ----
void __fastcall gui_m48_panel_add_control_0040b9a0(gui_load::Panel* panel, void*, Control* control) noexcept(false) {
    const Word word = static_cast<Word>(reinterpret_cast<std::uintptr_t>(control));
    if (gui_load_int_list_index_of_006c2a40(&panel->controls, nullptr, word) != -1) return;   // 0040B9A9..B7
    control->owner = panel;                                                        // 0040B9BC ([control+34h] = this)
    gui_load_array_add_00671c00(&panel->controls, nullptr, word);                  // 0040B9B9..BF
}
// ---- 0040BEC0 CSWGuiManager::DrawSafeRegion(float) (RET 4; EH states 0..2) ----
void __fastcall frame_DrawTail_0040bec0(void* manager, void*, float delta) noexcept(false) {
    unsigned char* const m = static_cast<unsigned char*>(manager);
    Word delta_bits; std::memcpy(&delta_bits, &delta, 4);
    if (word_at(m, 0x80) == 0) {                                                   // 0040BEDE..E9
        void* memory = c2da_directory_new_006fa7e6(0x64);                          // 0040BEEF..F6
        void* panel = nullptr;
        if (memory) {                                                              // 0040BEFD
            SafeFreeOnUnwind guard{memory};                                        // state 0
            panel = gui_load_panel_construct_0040b570(static_cast<gui_load::Panel*>(memory), nullptr,
                                                      static_cast<gui_load::Object*>(manager));   // 0040BF08..0B
            guard.block = nullptr;
        }
        put_word(m, 0x80, static_cast<Word>(reinterpret_cast<std::uintptr_t>(panel)));   // 0040BF25
        memory = c2da_directory_new_006fa7e6(0x140);                               // 0040BF19..2B
        void* label = nullptr;
        if (memory) {                                                              // 0040BF37
            SafeFreeOnUnwind guard{memory};                                        // state 1
            label = gui_load_label_construct_0041acd0(static_cast<gui_load::Label*>(memory), nullptr);   // 0040BF46..48
            guard.block = nullptr;
        }
        put_word(m, 0x84, static_cast<Word>(reinterpret_cast<std::uintptr_t>(label)));   // 0040BF5F
        gui_m48_panel_add_control_0040b9a0(reinterpret_cast<gui_load::Panel*>(word_at(m, 0x80)), nullptr,
                                           static_cast<Control*>(label));          // 0040BF51..65
        unsigned char* const p = reinterpret_cast<unsigned char*>(word_at(m, 0x80));
        put_word(p, 0x50, 0xBF800000u);                                            // 0040BF6A..7F (panel colour = -1.0f x3)
        put_word(p, 0x54, 0xBF800000u);                                            // 0040BF81..99
        put_word(p, 0x58, 0xBF800000u);                                            // 0040BF8D..9C
        alignas(4) menu_resources::Ref edge;                                       // [S+60h]
        edge.construct_00406d80("border1b");                                       // 0040BF9F..A8 (0073E088)
        alignas(4) menu_resources::Ref corner;                                     // [S+50h]
        corner.construct_00406d80("border2b");                                     // 0040BFAD..B6 (0073E07C)
        SafeParamsStorage text_params;                                             // [S+80h]
        gui_load::TextParams* text = gui_load_text_params_construct_00417050(
            reinterpret_cast<gui_load::TextParams*>(&text_params), nullptr);      // 0040BFBB..C7
        SafeTextParamsOnUnwind text_guard{&text_params};                           // state 2
        const gui_load::Vector white{1.0f, 1.0f, 1.0f};                            // 0040BFE0..F0 ([S+10h])
        const Extent zero{0, 0, 0, 0};                                             // 0040BFF8..04 ([S+40h])
        struct V { void* s[0x27]; void (__fastcall* init)(void*, void*, const Extent*, void*, const void*, Word); };
        void* const label_now = reinterpret_cast<void*>(word_at(m, 0x84));         // 0040BFC9 / 0040C008 (vtbl read)
        alignas(4) menu_resources::Ref fill;                                       // [S+70h]
        void* const fill_ref = c2da_default_ref_00405ed0(&fill, nullptr);          // 0040C00B..1D
        SafeParamsStorage border_params;                                           // [S+D4h]
        gui_load::BorderParams* border = gui_load_border_params_construct_00415000(
            reinterpret_cast<gui_load::BorderParams*>(&border_params), nullptr, &corner, &edge,
            static_cast<const gui_load::Ref*>(fill_ref), 0, 0, 1.0f, &white, 0);    // 0040C022..34
        vt<V>(static_cast<Control*>(label_now))->init(reinterpret_cast<void*>(word_at(m, 0x84)), nullptr, &zero,
            text, border, 0x3F800000u);                                            // 0040BFCF / 0040C039..46 (vslot +9C, 1.0f)
        text_guard.params = nullptr;
        frame_LogStringDtor_005e5c20(&text_params, nullptr);                       // 0040C04C..5E
    }
    Extent whole{0, 0, static_cast<I32>(static_cast<I16>(word_at(m, 0x6C) & 0xFFFFu)),
                 static_cast<I32>(static_cast<I16>(word_at(m, 0x6E) & 0xFFFFu))};  // 0040C065..83 (MOVSX)
    gui_load::Object* panel = reinterpret_cast<gui_load::Object*>(word_at(m, 0x80));
    panel->vtbl->set_extent(panel, nullptr, &whole);                               // 0040C071..8A (vslot +04)
    unsigned char* const label = reinterpret_cast<unsigned char*>(word_at(m, 0x84));
    const I32 left = 0x30 - gui_load_border_params_dim_00414cd0(reinterpret_cast<const gui_load::BorderParams*>(label + 0x70), nullptr);        // 0040C08D..A9
    const I32 top = 0x24 - gui_load_border_params_dim_00414cd0(reinterpret_cast<const gui_load::BorderParams*>(reinterpret_cast<unsigned char*>(word_at(m, 0x84)) + 0x70), nullptr);   // 0040C09B..B9
    Extent inner;
    inner.left = left;                                                             // 0040C0DB
    inner.top = top;                                                               // 0040C0BB
    inner.width = static_cast<I32>(static_cast<I16>(word_at(m, 0x6C) & 0xFFFFu)) - 2 * left;    // 0040C0B0..C2
    inner.height = static_cast<I32>(static_cast<I16>(word_at(m, 0x6E) & 0xFFFFu)) - 2 * top;    // 0040C0C4..CD
    gui_load::Object* label_object = reinterpret_cast<gui_load::Object*>(word_at(m, 0x84));
    label_object->vtbl->set_extent(label_object, nullptr, &inner);                 // 0040C0D1..E6 (vslot +04)
    struct DrawSlot34 { void* s[13]; void (__fastcall* draw)(void*, void*, Word); };
    void* const panel_now = reinterpret_cast<void*>(word_at(m, 0x80));
    vt<DrawSlot34>(static_cast<const Object*>(panel_now))->draw(panel_now, nullptr, delta_bits);   // 0040C0E9..F9 (vslot +34)
}
}


// ======================================================================== CSWGuiMessageBox family (constructed by CGuiInGame::InitializeGlobal
// 00632720: message box 0x984 (table 74FDB0 / derived controller-loss table 7513F8), skill-info box 0x24E8 (table 757940),
// tutorial box 0x998 (table 755CD0)).
namespace gui_lb {
using gui_load::EventFunc;
struct GuiMessageBox : gui_load::Panel {
    Word mb_flags;                       // +64 (00626EA4..AD) bit0 result, bit1 allow cancel, bit2 cancel shown, bit3 hidden, bit4 icon
    EventFunc callback;                  // +68 (00624A4F; called 006251D8..E3)
    kih::GuiObject* callback_object;     // +6C (00624A48)
    Word callback_argument;              // +70 (00624A52)
    gui_load::Label message;             // +74 "LBL_MESSAGE" (00626E2B)
    gui_load::Label icon;                // +1B4 (00626E3D)
    gui_load::Button ok;                 // +2F4 "BTN_OK" (00626E4F)
    gui_load::Button cancel;             // +4B8 "BTN_CANCEL" (00626E5F)
    ListBox list;                        // +67C "LB_MESSAGE" (00626E6F)
    Extent panel_extent;                 // +95C (00626FF8..0627012)
    Extent list_extent;                  // +96C (00627018..0062703E)
    I32 saved_input_class;               // +97C (0062705C / 00625912)
    Word u980;                           // +980 (0062705E; 00625975)
};
static_assert(offsetof(GuiMessageBox, message) == 0x74 && offsetof(GuiMessageBox, icon) == 0x1B4 && offsetof(GuiMessageBox, ok) == 0x2F4 &&
              offsetof(GuiMessageBox, cancel) == 0x4B8 && offsetof(GuiMessageBox, list) == 0x67C && offsetof(GuiMessageBox, panel_extent) == 0x95C &&
              offsetof(GuiMessageBox, saved_input_class) == 0x97C && sizeof(GuiMessageBox) == 0x984, "message box");
struct TutorialBox : GuiMessageBox {
    Word u984, u988;                     // = 7F000000h (006AA130..36)
    Word u98c;                           // = 0 (006AA14F)
    Word paused_by_combat;               // +990 (006AA155; 006AA877)
    unsigned char row;                   // +994 = FFh (006AA15B)
    unsigned char message_index;         // +995 = 0 (006AA162)
    unsigned char pad996[2];
};
static_assert(sizeof(TutorialBox) == 0x998 && offsetof(TutorialBox, row) == 0x994, "tutorial box");
struct SkillEntry : LabelHilight {
    Border b1b4, b228, b29c;             // +1B4 / +228 / +29C (006ACC72..A6)
};
static_assert(sizeof(SkillEntry) == 0x310 && offsetof(SkillEntry, b228) == 0x228, "skill entry");
struct SkillInfoBox : gui_load::Panel {
    ListBox list;                        // +64 "LB_SKILLS"
    gui_load::Label message;             // +344 "LBL_MESSAGE"
    gui_load::Button ok;                 // +484 "BTN_OK"
    SkillEntry entries[10];              // +648 (eh vector 0x310 x 10, 006CE826..43)
};
static_assert(offsetof(SkillInfoBox, message) == 0x344 && offsetof(SkillInfoBox, ok) == 0x484 &&
              offsetof(SkillInfoBox, entries) == 0x648 && sizeof(SkillInfoBox) == 0x24E8, "skill info box");
}
namespace kotor_menu20 { struct Input { void __thiscall set_mouse_005df5d0(int, int); }; }   // M41 service-menu20-4 (005DF5D0)
extern "C" {
extern const struct GuiM48MessageBoxMethods gui_m48_message_box_methods_0074fdb0;
extern const struct GuiM48TutorialBoxMethods gui_m48_tutorial_box_methods_00755cd0;
extern const struct GuiM48SkillInfoBoxMethods gui_m48_skill_info_box_methods_00757940;
extern const struct GuiM48SkillEntryMethods gui_m48_skill_entry_methods_00755ed0;
void __fastcall light_manager_void_return_16_0063cfc0(void* self, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);   // 3d-construct (0063CFC0 RET 10h leaf)
void* __fastcall kotor_fastcall_0040be00(void* gui, void*);                       // r4-lowers PopModalPanel
void __fastcall kotor_fastcall_00627130(void* box, void*, Word allow);              // core47 SetAllowCancel
void* __fastcall frame_R4_005ed690(void* app, void*);                               // r4-lowers GetInGameGui
void __fastcall kotor_fastcall_005df540(void* input, void*, std::uint32_t active);  // CExoInput::SetActive (game-prepare)
void __fastcall kotor_fastcall_005edc20(void* client, void*, Word paused, Word reason, Word force);   // sibling CLIENT client_pause.cpp
unsigned __fastcall kotor_fastcall_005edc10(void* client, void*);                   // sibling CLIENT client_pause.cpp
void __fastcall kotor_fastcall_006200e0(void* internal, void*, std::uint32_t input_class, std::uint32_t show) noexcept(false);
void __fastcall gui_load_panel_on_added_0040b870(gui_load::Panel*, void*);
void __fastcall gui_load_panel_on_removed_0040c170(gui_load::Panel*, void*);
void __fastcall gui_load_border_params_set_pulsing_00414d00(void* params, void*, Word a, Word b, Word c);
void* __fastcall kotor_fastcall_004171e0(void* self, void*, const void* source) noexcept(false);   // gui-render TextParams::operator=
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* string) noexcept(false);
std::int32_t __fastcall c2da_current_get_string_00413270(void* table, void*, std::int32_t row, void* column, void* out);
void __stdcall crt_eh_vector_constructor_006fb55f(void* first, std::uint32_t size, std::uint32_t count,
    void* (__fastcall* construct)(void*, void*), void (__fastcall* destroy)(void*, void*));
void __fastcall gui_m48_client_set_input_class_005eda60(void* app, void*, std::uint32_t input_class, std::uint32_t show) noexcept(false);
void __fastcall gui_m48_client_internal_restore_mouse_005f4ea0(void* internal, void*, I32 x, I32 y);
void __fastcall gui_m48_manager_store_mouse_0040a3d0(void* manager, void*);
void __fastcall gui_m48_manager_move_mouse_0040c790(void* manager, void*, I32 x, I32 y);
void __fastcall gui_m48_manager_restore_mouse_0040c7d0(void* manager, void*);
void __fastcall gui_m48_panel_center_0040a600(gui_load::Panel*, void*);
void __fastcall gui_m48_panel_set_to_destroy_00624a00(gui_load::Panel*, void*, Word);
void __fastcall gui_m48_ingame_end_controller_pause_0062f7e0(void* in_game, void*);
void __fastcall gui_m48_listbox_add_controls_0041c2c0(ListBox*, void*, Control** controls, I32 count, Word suppress, Word restore, Word variable) noexcept(false);
void __fastcall gui_m48_extent_list_add_0041b860(kotor_reconstruction::ArrayMutations*, void*, Extent value) noexcept(false);
void __fastcall gui_m48_text_params_set_font_rename_00415df0(void* params, void*) noexcept(false);
void* __fastcall gui_m48_message_box_deleting_destruct_00627080(GuiMessageBox*, void*, Word);
void __fastcall gui_m48_message_box_destruct_006270a0(GuiMessageBox*, void*);
void __fastcall gui_m48_message_box_handle_input_006250f0(GuiMessageBox*, void*, I32, I32) noexcept(false);
void __fastcall gui_m48_message_box_on_added_006258f0(GuiMessageBox*, void*) noexcept(false);
void __fastcall gui_m48_message_box_on_removed_00625a00(GuiMessageBox*, void*) noexcept(false);
void __fastcall gui_m48_message_box_set_message_strref_006249d0(GuiMessageBox*, void*, Word) noexcept(false);
struct GuiM48StringBits { Word data, capacity; };                                  // CExoString passed by value (8 bytes)
void __fastcall gui_m48_message_box_set_message_006271a0(GuiMessageBox*, void*, GuiM48StringBits) noexcept(false);
void __fastcall gui_m48_message_box_set_ok_text_00625260(GuiMessageBox*, void*, Word) noexcept(false);
void __fastcall gui_m48_message_box_set_cancel_text_00625270(GuiMessageBox*, void*, Word) noexcept(false);
void __fastcall gui_m48_message_box_hide_buttons_00625280(GuiMessageBox*, void*);
void __fastcall gui_m48_message_box_reset_buttons_006252a0(GuiMessageBox*, void*) noexcept(false);
void __fastcall gui_m48_message_box_update_controls_00625200(GuiMessageBox*, void*);
void __fastcall gui_m48_message_box_set_icon_00627160(GuiMessageBox*, void*, gui_load::Ref*) noexcept(false);
void __fastcall gui_m48_message_box_add_message_label_006252f0(GuiMessageBox*, void*) noexcept(false);
void __fastcall gui_m48_message_box_fix_message_label_006253a0(GuiMessageBox*, void*) noexcept(false);
void __fastcall gui_m48_controller_loss_on_added_00627220(GuiMessageBox*, void*) noexcept(false);
void* __fastcall gui_m48_tutorial_box_deleting_destruct_006aa830(TutorialBox*, void*, Word);
void __fastcall gui_m48_tutorial_box_destruct_006aa850(TutorialBox*, void*);
void __fastcall gui_m48_tutorial_box_handle_input_006aa8a0(TutorialBox*, void*, I32, I32) noexcept(false);
void __fastcall gui_m48_tutorial_box_on_added_006aa860(TutorialBox*, void*) noexcept(false);
void __fastcall gui_m48_tutorial_box_set_message_006aa040(TutorialBox*, void*, Word) noexcept(false);
I32 __fastcall gui_m48_tutorial_box_set_next_message_006aa670(TutorialBox*, void*) noexcept(false);
void* __fastcall gui_m48_skill_info_deleting_destruct_006cdfa0(SkillInfoBox*, void*, Word);
void __fastcall gui_m48_skill_info_destruct_006cd320(SkillInfoBox*, void*);
void __fastcall gui_m48_skill_info_handle_input_006cd3c0(SkillInfoBox*, void*, I32, I32) noexcept(false);
void* __fastcall gui_m48_skill_entry_construct_006acc50(void*, void*) noexcept(false);
void __fastcall gui_m48_skill_entry_destruct_006aba80(void*, void*);
void* __fastcall gui_m48_skill_entry_deleting_destruct_006accc0(SkillEntry*, void*, Word);
void __fastcall gui_m48_skill_entry_set_extent_006ab8e0(SkillEntry*, void*, const Extent*);
void __fastcall gui_m48_skill_entry_draw_006ab9f0(SkillEntry*, void*, Word);
void __fastcall gui_m48_skill_entry_initialize_006ac930(SkillEntry*, void*, gui_load::Text*, Border*, Border*, I32) noexcept(false);
}
namespace {
// Event callbacks registered with CSWGuiControl::AddEvent / stored as CSWGuiMessageBox callbacks: thiscall members
// (RET 4) on a single-inheritance view of kih::GuiObject, converted to the base PMF by static_cast (D47).
struct MessageBoxHandlers : kih::GuiObject {
    void __thiscall OnOkButtonPressed(kih::Control*);       // 00625A80
    void __thiscall OnCancelButtonPressed(kih::Control*);   // 00625AA0
    void __thiscall OnAccept(kih::Control*);                // 00625A40 (controller-loss box)
};
GuiMessageBox* box_of(MessageBoxHandlers* h) { return reinterpret_cast<GuiMessageBox*>(h); }
EventFunc event_func_of(void (__thiscall MessageBoxHandlers::*f)(kih::Control*)) { return static_cast<EventFunc>(f); }
kih::GuiObject* event_target_of(void* object) { return reinterpret_cast<kih::GuiObject*>(object); }
void* app_client() { return reinterpret_cast<void*>(word_at(kotor_menu_window_cells::manager_007a39fc, 4)); }
I32 half(I32 v) { return v / 2; }                                                  // CDQ/SUB/SAR 1 (truncate toward 0)
using TextSlot08 = I32 (__fastcall*)(gui_load::Text*, void*);                      // CSWGuiText +08 GetIdealHeight
I32 ideal_height(gui_load::Text* t) { struct V { void* s0; void* s4; TextSlot08 ideal; }; return vt<V>(t)->ideal(t, nullptr); }
using PanelSlot = void (__fastcall*)(void*, void*, Word);
void panel_slot(void* panel, unsigned offset, Word arg) {                          // vslot call with one stack word
    const PanelSlot* table = vt<PanelSlot>(static_cast<const Object*>(panel));
    table[offset / 4](panel, nullptr, arg);
}
using PanelSlotNoArg = void (__fastcall*)(void*, void*);                           // slot with no stack words (plain RET)
void panel_slot_noarg(void* panel, unsigned offset) {                              // vslot call pushing nothing
    const PanelSlotNoArg* table = vt<PanelSlotNoArg>(static_cast<const Object*>(panel));
    table[offset / 4](panel, nullptr);
}
}
// ---- callbacks
void __thiscall MessageBoxHandlers::OnOkButtonPressed(kih::Control*) {             // 00625A80: vslot +3C(1F6h, [this+340h])
    GuiMessageBox* self = box_of(this);
    self->vtbl->handle_input_event(self, nullptr, 0x1F6, static_cast<I32>(word_at(self, 0x340)));   // 00625A80..8E
}
void __thiscall MessageBoxHandlers::OnCancelButtonPressed(kih::Control*) {         // 00625AA0: vslot +3C(1F7h, [this+504h])
    GuiMessageBox* self = box_of(this);
    self->vtbl->handle_input_event(self, nullptr, 0x1F7, static_cast<I32>(word_at(self, 0x504)));   // 00625AA0..AE
}
void __thiscall MessageBoxHandlers::OnAccept(kih::Control*) {                      // 00625A40
    gui_m48_ingame_end_controller_pause_0062f7e0(frame_R4_005ed690(app_client(), nullptr), nullptr);   // 00625A40..4F
}
// 00624BA0 AcceptButtonCallback: single owner lane-m1-options L0-shared (kotor_m1_shared::PanelEventThunks).
extern "C" {
// ======== small manager / client helpers
// ---- 005EDA60 CClientExoApp::SetInputClass(int, int): ECX = [ECX+4]; JMP 006200E0.
// M48 composer: 005EDA60 body dropped (single owner lane-m48-b client_status_summary.cpp @kotor_fastcall_005eda60@16; joined).
// ---- 005F4EA0 CClientExoAppInternal::RestoreMousePositionTo(int, int) (RET 8): +3C8 = x, +3CC = y.
void __fastcall gui_m48_client_internal_restore_mouse_005f4ea0(void* internal, void*, I32 x, I32 y) {
    put_word(internal, 0x3C8, static_cast<Word>(x));                               // 005F4EA8
    put_word(internal, 0x3CC, static_cast<Word>(y));                               // 005F4EAE
}
// ---- 0040A3D0 CSWGuiManager::StoreCurrentMousePosition: +58 = [+0], +5C = [+4].
void __fastcall gui_m48_manager_store_mouse_0040a3d0(void* manager, void*) {
    const Word x = word_at(manager, 0), y = word_at(manager, 4);                   // 0040A3D0..D2
    put_word(manager, 0x58, x);                                                    // 0040A3D5
    put_word(manager, 0x5C, y);                                                    // 0040A3D8
}
// ---- 0040C790 CSWGuiManager::MoveMouseToPosition(int x, int y) (RET 8) ----
void __fastcall gui_m48_manager_move_mouse_0040c790(void* manager, void*, I32 x, I32 y) {
    put_word(manager, 0, static_cast<Word>(x));                                    // 0040C79D
    put_word(manager, 4, static_cast<Word>(y));                                    // 0040C79F
    const I32 flipped = static_cast<I32>(kotor_menu_window_cells::height_0078d1d8) - y - 1;   // 0040C7A2..AF
    static_cast<kotor_menu20::Input*>(kotor_menu_window_cells::input_007a39e4)->set_mouse_005df5d0(x, flipped);   // 0040C7A7..B2
    static_cast<kih::Manager*>(manager)->HandleMouseMove(x, y);                     // 0040C7B7..BB (0040C1E0)
}
// ---- 0040C7D0 CSWGuiManager::RestoreOldMousePosition (RET) ----
void __fastcall gui_m48_manager_restore_mouse_0040c7d0(void* manager, void*) {
    const I32 x = static_cast<I32>(word_at(manager, 0x58));                        // 0040C7D3
    if (x < 0) return;                                                             // 0040C7D6..D8
    const I32 y = static_cast<I32>(word_at(manager, 0x5C));                        // 0040C7DA
    put_word(manager, 0, static_cast<Word>(x));                                    // 0040C7DD
    put_word(manager, 4, static_cast<Word>(y));                                    // 0040C7DF
    static_cast<kotor_menu20::Input*>(kotor_menu_window_cells::input_007a39e4)->set_mouse_005df5d0(
        x, static_cast<I32>(kotor_menu_window_cells::height_0078d1d8) - y - 1);   // 0040C7E2..F3
    gui_m48_client_internal_restore_mouse_005f4ea0(reinterpret_cast<void*>(word_at(app_client(), 4)), nullptr,
        static_cast<I32>(word_at(manager, 0)), static_cast<I32>(word_at(manager, 4)));   // 0040C7F8..0B
    const I32 cx = static_cast<I32>(word_at(manager, 0)), cy = static_cast<I32>(word_at(manager, 4));   // 0040C810..13
    put_word(manager, 0x58, 0xFFFFFFFFu);                                          // 0040C81C
    put_word(manager, 0x5C, 0xFFFFFFFFu);                                          // 0040C81F
    static_cast<kih::Manager*>(manager)->HandleMouseMove(cx, cy);                   // 0040C815..22
}
// ---- 0040A600 CSWGuiPanel::CenterPanel (RET) ----
void __fastcall gui_m48_panel_center_0040a600(gui_load::Panel* panel, void*) {
    const unsigned char* manager = reinterpret_cast<const unsigned char*>(panel->manager);   // 0040A604
    std::int16_t w, h; std::memcpy(&w, manager + 0x6C, 2); std::memcpy(&h, manager + 0x6E, 2);
    panel->extent.left = half(static_cast<I32>(w) - panel->extent.width);          // 0040A600..15
    panel->extent.top = half(static_cast<I32>(h) - panel->extent.height);          // 0040A610..23
    panel->flags |= 1u;                                                            // 0040A626
}
// ---- 00624A00 CSWGuiPanel::setToDestroy(int) (RET 4): bit8 = arg; if (flags & 600h) != 400h: flags = flags & ~400h | 200h.
// M48 composer: 00624A00 body dropped (single owner core47 gui_ingame_leaves.cpp @kotor_fastcall_00624a00@12; joined).
// ---- 0062F7E0 CGuiInGame::EndControllerPauseState (RET) ----
void __fastcall gui_m48_ingame_end_controller_pause_0062f7e0(void* in_game, void*) {
    const bool active = word_at(in_game, 0xC08) == 1u;                             // 0062F7E3
    put_word(in_game, 0xDC, 0);                                                    // 0062F7EA
    kotor_fastcall_005df540(kotor_menu_window_cells::input_007a39e4, nullptr, active ? 1u : 0u);   // 0062F7F4..02
    void* const app = kotor_menu_window_cells::manager_007a39fc;                   // 0062F807
    if (!app || !word_at(app, 8) || word_at(in_game, 0xE0) != 0) return;           // 0062F80C..1F
    kotor_fastcall_005edc20(reinterpret_cast<void*>(word_at(app, 4)), nullptr, 0, 0, 1);   // 0062F821..2A
}
// ---- 00415DF0 CSWGuiTextParams::SetFontRename: JMP 00415D60 (thunk; gui-load body).
void __fastcall gui_m48_text_params_set_font_rename_00415df0(void* params, void*) noexcept(false) {
    gui_load_text_params_set_font_rename_00415d60(static_cast<gui_load::TextParams*>(params), nullptr);   // 00415DF0
}
// ---- 0041B860 CExoArrayList<CSWGuiExtent>::Add(CSWGuiExtent) (RET 10h) ----
void __fastcall gui_m48_extent_list_add_0041b860(kotor_reconstruction::ArrayMutations* list, void*, Extent value) noexcept(false) {
    if (list->size == list->capacity)                                              // 0041B863..69
        gui_load_extent_list_allocate_0041a8f0(list, nullptr, list->capacity ? static_cast<Word>(list->capacity * 2) : 0x10u);   // 0041B86B..7B
    std::memcpy(reinterpret_cast<void*>(static_cast<std::uintptr_t>(list->data + static_cast<Word>(list->size) * 16u)), &value, 16);   // 0041B880..A2
    list->size = list->size + 1;                                                   // 0041B8A5
}
// ---- 0041C2C0 CSWGuiListBox::AddControls(controls, count, suppressCleanUp, restoreExtents, variableItemHeights) (RET 14h) ----
void __fastcall gui_m48_listbox_add_controls_0041c2c0(ListBox* self, void*, Control** controls, I32 count, Word suppress,
                                                      Word restore, Word variable) noexcept(false) {
    I32 next_id = 0;                                                               // [esp+10]
    gui_load_listbox_clear_items_00419a90(self, nullptr);                          // 0041C2CD
    self->lb_flags = (self->lb_flags & 0xFFFFFFF9u) | ((((restore & 1u) << 1) | (suppress & 1u)) << 1);   // 0041C2D2..F1
    for (I32 i = 0; i < count; ++i) {                                              // 0041C2F7..5E
        Control* c = controls[i];                                                  // 0041C303..07
        const Word word = static_cast<Word>(reinterpret_cast<std::uintptr_t>(c));
        if (gui_load_int_list_index_of_006c2a40(&self->items, nullptr, word) != -1) continue;   // 0041C30A..15
        c->id = next_id;                                                           // 0041C31B
        ++next_id;
        c->owner = self;                                                           // 0041C322
        gui_load_array_add_00671c00(&self->items, nullptr, word);                  // 0041C31F..29
        gui_m48_extent_list_add_0041b860(&self->extents, nullptr, c->extent);      // 0041C32E..52
    }
    const Word flags = self->lb_flags;                                             // 0041C360
    self->lb_flags = ((((variable << 8) ^ flags) & 0x100u) ^ flags) | 8u;          // 0041C366..7C
    gui_load_listbox_organize_0041b140(self, nullptr);                             // 0041C382..84
}

// ======== CSWGuiMessageBox
// ---- 00626DF0 ctor (EH states 0..8) ----
}
namespace kotor_original {
void* __fastcall kotor_fastcall_00626df0(void* memory, void*, void* gui_manager) {
    GuiMessageBox* self = static_cast<GuiMessageBox*>(memory);
    gui_load_panel_construct_0040b570(self, nullptr, static_cast<gui_load::Object*>(gui_manager));   // 00626E12..17
    try {
        set_vptr(self, &gui_m48_message_box_methods_0074fdb0);                    // 00626E25 (74FDB0)
        gui_load_label_construct_0041acd0(&self->message, nullptr);                // 00626E1E..2B
        try {
            gui_load_label_construct_0041acd0(&self->icon, nullptr);               // 00626E30..3D
            try {
                gui_load_button_construct_0041c0f0(&self->ok, nullptr);            // 00626E42..4F
                try {
                    gui_load_button_construct_0041c0f0(&self->cancel, nullptr);    // 00626E54..5F
                    try { gui_load_listbox_construct_0041be40(&self->list, nullptr); }   // 00626E64..6F
                    catch (...) { gui_load_button_destruct_00419da0(&self->cancel, nullptr); throw; }
                } catch (...) { gui_load_button_destruct_00419da0(&self->ok, nullptr); throw; }
            } catch (...) { gui_load_label_destruct_00419d10(&self->icon, nullptr); throw; }
        } catch (...) { gui_load_label_destruct_00419d10(&self->message, nullptr); throw; }
    } catch (...) { gui_load_panel_destruct_0040cf70(self, nullptr); throw; }
    try {                                                                          // state 5 (members constructed)
        self->list_extent = Extent{0, 0, 0, 0};                                    // 00626E74..9E (+968..+95C, +978..+96C)
        self->panel_extent = Extent{0, 0, 0, 0};
        self->mb_flags = (self->mb_flags & 0xFFFFFFE7u) | 6u;                      // 00626EA4..AD
        menu_resources::Ref layout;
        layout.construct_00406d80("confirm");                                      // 00626EB0..BE (0074FDA4)
        gui_load_panel_start_layout_0040a680(self, nullptr, &layout);              // 00626EC3..CA
        { String name("BTN_OK"); gui_load_panel_init_control_0040b930(self, nullptr, &self->ok, &name, 1); }        // 00626ECF..FA
        { String name("BTN_CANCEL"); gui_load_panel_init_control_0040b930(self, nullptr, &self->cancel, &name, 1); }  // 00626EFF..30
        { String name("LB_MESSAGE"); gui_load_panel_init_control_0040b930(self, nullptr, &self->list, &name, 1); }    // 00626F35..66
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                        // 00626F6B..6D
        gui_load_control_add_event_0041ab20(&self->ok, nullptr, 0x27, event_target_of(self),
            event_func_of(&MessageBoxHandlers::OnOkButtonPressed));                // 00626F72..7C (00625A80)
        gui_load_control_add_event_0041ab20(&self->cancel, nullptr, 0x27, event_target_of(self),
            event_func_of(&MessageBoxHandlers::OnCancelButtonPressed));            // 00626F81..8F (00625AA0)
        Extent icon_rect{0, 0x0A, 0x20, 0x20};                                     // 00626F94..B5 ([esp+14] 0, 0Ah, 20h, 20h)
        reinterpret_cast<unsigned char*>(self)[0x93D] = 1;                         // 00626FAA (list sound byte)
        self->icon.vtbl->set_extent(&self->icon, nullptr, &icon_rect);             // 00626FA5..BD (vslot +04)
        self->icon.flags &= 0xFFFFFFFDu;                                           // 00626FC0..C9
        gui_m48_panel_add_control_0040b9a0(self, nullptr, &self->icon);            // 00626FC6..CC
        if (Control* proto = self->list.proto) {                                   // 00626FD1..D9 (+948)
            struct V { void* s[0x26]; void (__fastcall* init98)(void*, void*, const Extent*, Control*, Word); };
            vt<V>(&self->message)->init98(&self->message, nullptr, &proto->extent, proto, 0x3F800000u);   // 00626FDB..EB
        }
        gui_m48_panel_center_0040a600(self, nullptr);                              // 00626FF1..F3
        self->panel_extent = self->extent;                                         // 00626FF8..0627012
        self->list_extent = self->list.extent;                                     // 00627018..0627064
        self->mb_flags &= 0xFFFFFFFEu;                                             // 0062703B..4B
        self->callback = nullptr;                                                  // 0062704F
        self->callback_object = nullptr;                                           // 00627052
        self->callback_argument = 0;                                               // 00627055
        self->saved_input_class = 0;                                               // 00627058
        self->u980 = 0;                                                            // 0062705E
        self->b60 = 0x0F;                                                          // 0062706A
    } catch (...) { gui_m48_message_box_destruct_006270a0(self, nullptr); throw; }
    return self;                                                                   // 0062706E
}
}
extern "C" {
// ---- 006270A0 dtor (EH states 4..0) ----
void __fastcall gui_m48_message_box_destruct_006270a0(GuiMessageBox* self, void*) {
    gui_load_listbox_destruct_0041b060(&self->list, nullptr);                      // 006270BD..CB
    gui_load_button_destruct_00419da0(&self->cancel, nullptr);                     // 006270D0..DB
    gui_load_button_destruct_00419da0(&self->ok, nullptr);                         // 006270E0..EB
    gui_load_label_destruct_00419d10(&self->icon, nullptr);                        // 006270F0..FB
    gui_load_label_destruct_00419d10(&self->message, nullptr);                     // 00627100..08
    gui_load_panel_destruct_0040cf70(self, nullptr);                               // 0062710D..17
}
// ---- 00627080 scalar deleting destructor.
void* __fastcall gui_m48_message_box_deleting_destruct_00627080(GuiMessageBox* self, void*, Word flags) {
    gui_m48_message_box_destruct_006270a0(self, nullptr);                          // 00627083
    if (flags & 1u) live_free(self);                                               // 00627088..90
    return self;
}
// ---- 006250F0 HandleInputEvent(int event, int value) (RET 8) ----
void __fastcall gui_m48_message_box_handle_input_006250f0(GuiMessageBox* self, void*, I32 event, I32 value) noexcept(false) {
    if (value != 0) {                                                              // 006250F5..FF
        if (event == 0x39 || event == 0x3A) {                                      // 00625116 / 0062510A
            cv(&self->list)->handle_input_event(&self->list, nullptr, event == 0x39 ? 0x31 : 0x32, 1);   // 0062511F..2F / 00625141..51
            gui_load_panel_handle_input_00409e60(self, nullptr, event, value);    // 00625132..36 / 00625154..58
            return;
        }
        bool finish = false; Word flags = self->mb_flags;
        if (event == 0x1F6) { flags |= 1u; finish = true; }                        // 0062517B..7E
        else if (event == 0x1F7 || event == 0x28 || event == 0x2E) {               // 0062516C / 0062510C..14
            flags = (flags & 2u) ? (flags & 0xFFFFFFFEu) : (flags | 1u);            // 0062516F..7E
            finish = true;
        }
        if (finish) {
            self->mb_flags = flags;                                                // 00625183
            self->mb_flags = (flags & 0xFFFFFFEFu) | 6u;                           // 00625186..8E
            panel_slot_noarg(self, 0x80);                                          // 00625181..91 (vslot +80 ResetButtonMessage 006252A0, RET; no push)
            Word pf = self->flags & 0xFFFFFEFFu;                                   // 00625197..9A
            self->flags = pf;                                                      // 006251AD
            if ((pf & 0x600u) != 0x400u) self->flags = (pf & 0xFFFFFBFFu) | 0x200u;   // 006251A1..BC
            kotor_fastcall_0040be00(self->manager, nullptr);                       // 006251BF..C2 (PopModalPanel)
            reinterpret_cast<kih::Manager*>(self->manager)->PlayGuiSound(0);       // 006251C7..CC
            if (self->callback_object && self->callback)                           // 006251D1..DD
                (self->callback_object->*(self->callback))(reinterpret_cast<kih::Control*>(
                    static_cast<std::uintptr_t>(self->callback_argument)));        // 006251DF..E3
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, value);            // 006251E5..E9
}
// ---- 006258F0 OnPanelAdded (RET) ----
void __fastcall gui_m48_message_box_on_added_006258f0(GuiMessageBox* self, void*) noexcept(false) {
    const I32 input_class = static_cast<I32>(gui_m48_client_get_input_class_005eda70(app_client(), nullptr));   // 006258F0..01
    gui_load_panel_on_added_0040b870(self, nullptr);                               // 00625906..0A
    self->saved_input_class = -1;                                                  // 00625912
    if (input_class != 2) {                                                        // 0062590F..1C
        self->saved_input_class = input_class;                                     // 0062591E
        gui_m48_client_set_input_class_005eda60(app_client(), nullptr, 2, 0);      // 00625924..31
    }
    self->vtbl->set_active_control(self, nullptr, nullptr, 0);                     // 00625936..3E (vslot +08)
    self->vtbl->set_active_control(self, nullptr, &self->ok, 0);                   // 00625941..4E
    const Extent panel = self->extent;                                             // 00625951..6F
    const Extent target = ((self->mb_flags & 2u) && !self->u980) ? self->cancel.extent : self->ok.extent;   // 00625964..B9
    void* const manager = gui_manager();
    gui_m48_manager_store_mouse_0040a3d0(manager, nullptr);                        // 006259BC..C2
    gui_m48_manager_move_mouse_0040c790(gui_manager(), nullptr,
        half(target.width) + panel.left + target.left,                             // 006259E1..EC
        half(target.height) + target.top + panel.top);                             // 006259CD..E0
}
// ---- 00625A00 OnPanelRemoved (RET) ----
void __fastcall gui_m48_message_box_on_removed_00625a00(GuiMessageBox* self, void*) noexcept(false) {
    gui_load_panel_on_removed_0040c170(self, nullptr);                             // 00625A03
    if (self->saved_input_class >= 0)                                              // 00625A08..10
        gui_m48_client_set_input_class_005eda60(app_client(), nullptr, static_cast<std::uint32_t>(self->saved_input_class), 1);   // 00625A12..1D
    gui_m48_manager_restore_mouse_0040c7d0(gui_manager(), nullptr);                // 00625A22..28
    self->u980 = 0;                                                                // 00625A2D
}
// ---- 006249D0 SetMessage(ulong strref) (RET 4): CExoString built in the outgoing argument slot (TLK GetSimpleString),
// passed by value to vslot +70 which owns and destroys it.
void __fastcall gui_m48_message_box_set_message_strref_006249d0(GuiMessageBox* self, void*, Word strref) noexcept(false) {
    GuiM48StringBits argument;                                                     // [esp] argument slot
    tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(&argument), strref);   // 006249D7..E9
    struct V { void* s[0x1C]; void (__fastcall* set_text)(GuiMessageBox*, void*, GuiM48StringBits); };
    vt<V>(static_cast<const Object*>(self))->set_text(self, nullptr, argument);   // 006249EE..F2 (vslot +70)
}
// ---- 006271A0 SetMessage(CExoString) (RET 8; argument owned) ----
void __fastcall gui_m48_message_box_set_message_006271a0(GuiMessageBox* self, void*, GuiM48StringBits text) noexcept(false) {
    String* const s = reinterpret_cast<String*>(&text);
    tlk_core::ParseStr0041dd30(tlk(), nullptr, s);                                 // 006271B9..CC
    unsigned char* const params = reinterpret_cast<unsigned char*>(self) + 0x15C;  // message text params
    gui_load_text_params_set_text_00415e00(params, nullptr, s);                    // 006271D1..DE
    gui_m48_text_params_set_font_rename_00415df0(params, nullptr);                 // 006271E3..E5
    gui_m48_message_box_fix_message_label_006253a0(self, nullptr);                 // 006271EA..EC
    frame_LogStringDtor_005e5c20(s, nullptr);                                      // 006271F1..FD (argument destroyed by callee)
}
// ---- 00625260 / 00625270 SetOk/CancelButtonMessage(ulong): ECX += 460h / 624h; JMP 00415E50 SetStrRef.
void __fastcall gui_m48_message_box_set_ok_text_00625260(GuiMessageBox* self, void*, Word strref) noexcept(false) {
    gui_load_text_params_set_strref_00415e50(reinterpret_cast<unsigned char*>(self) + 0x460, nullptr, strref);
}
void __fastcall gui_m48_message_box_set_cancel_text_00625270(GuiMessageBox* self, void*, Word strref) noexcept(false) {
    gui_load_text_params_set_strref_00415e50(reinterpret_cast<unsigned char*>(self) + 0x624, nullptr, strref);
}
// ---- 00625280 HideButtons ----
void __fastcall gui_m48_message_box_hide_buttons_00625280(GuiMessageBox* self, void*) {
    self->mb_flags |= 8u;                                                          // 00625280
    self->ok.flags &= 0xFFFFFFFDu;                                                 // 00625284..91 (+338)
    self->cancel.flags &= 0xFFFFFFFDu;                                             // 00625297 (+4FC)
}
// ---- 006252A0 ResetButtonMessage ----
void __fastcall gui_m48_message_box_reset_buttons_006252a0(GuiMessageBox* self, void*) noexcept(false) {
    self->ok.flags |= 2u;                                                          // 006252A3..B0
    self->cancel.flags |= 2u;                                                      // 006252B6
    self->mb_flags &= 0xFFFFFFF7u;                                                 // 006252BC..CD
    gui_load_text_params_set_strref_00415e50(reinterpret_cast<unsigned char*>(self) + 0x460, nullptr, 0x62C);   // 006252C2..D0
    gui_load_text_params_set_strref_00415e50(reinterpret_cast<unsigned char*>(self) + 0x624, nullptr, 0x62D);   // 006252D5..E0
}
// ---- 00625200 UpdateControls ----
// M48 composer: 00625200 body dropped (single owner core47 gui_ingame_leaves.cpp @kotor_fastcall_00625200@8; joined).
// ---- 00627160 SetIcon(CResRef const&) (RET 4) ----
void __fastcall gui_m48_message_box_set_icon_00627160(GuiMessageBox* self, void*, gui_load::Ref* icon) noexcept(false) {
    if (!gui_load_resref_is_valid_00406030(icon, nullptr)) return;                 // 00627166..71
    self->mb_flags |= 0x10u;                                                       // 00627173..82
    gui_load_border_params_set_fill_00414c00(reinterpret_cast<unsigned char*>(self) + 0x224, nullptr, icon, 0);   // 00627176..85
    gui_m48_message_box_update_controls_00625200(self, nullptr);                   // 0062718A..8C
}
// ---- 006252F0 AddMessageLabel (RET) ----
void __fastcall gui_m48_message_box_add_message_label_006252f0(GuiMessageBox* self, void*) noexcept(false) {
    Extent r = self->message.extent;                                               // 006252F8..30 (+78)
    r.width = self->list.inner.width - 2 * static_cast<I32>(self->list.padding);   // 0062530E..36 (+93C byte)
    self->message.vtbl->set_extent(&self->message, nullptr, &r);                   // 00625334..3D
    String* const text = reinterpret_cast<String*>(reinterpret_cast<unsigned char*>(self) + 0x15C);
    gui_load::Text* const label_text = &self->message.text;                        // +144
    r.height = text->BorrowedLength005e5790() > 0 ? ideal_height(label_text)       // 00625340..57
                                                  : gui_load_text_font_height_00414ee0(label_text, nullptr);   // 0062535C
    self->message.vtbl->set_extent(&self->message, nullptr, &r);                   // 00625361..6E
    Control* controls[1] = {&self->message};                                       // [esp+10] = label (00625380)
    gui_m48_listbox_add_controls_0041c2c0(&self->list, nullptr, controls, 1, 0, 0, 0);   // 00625371..84
    gui_load_listbox_set_active_control_0041c160(&self->list, nullptr, &self->message, 0);   // 00625389..8E
}
}
namespace {
bool list_scrolls(ListBox* l) { return gui_m48_listbox_should_scroll_0041a290(l, nullptr) != 0; }   // inline copies 00625654.. / 006256A0..
// 0062546B..0062552F / 00625533..006255F9 shared button fit: R = button extent with width 100, grow width by 10 until the
// text fits on one line, add twice the border dimension word, then widen the panel to at least R.width - 2.
void fit_button(gui_load::Button* button, Word border_dim, I32* panel_width) {
    Extent r = button->extent;                                                     // +2F8 / +4BC
    r.width = 0x64;                                                                // 006254AC / 00625574
    button->vtbl->set_extent(button, nullptr, &r);                                 // 006254A5..B4
    gui_load::Text* const text = &button->text;                                    // +448 / +60C
    while (ideal_height(text) != gui_load_text_font_height_00414ee0(text, nullptr)) {   // 006254B7..D3 / 006254EC..FF
        r.width += 0x0A;                                                           // 006254D5..E5
        button->vtbl->set_extent(button, nullptr, &r);                             // 006254D9..E9
    }
    r.width = r.width + static_cast<I32>(border_dim) * 2;                          // 00625501..17
    button->vtbl->set_extent(button, nullptr, &r);                                 // 0062550E..1B
    const I32 min_width = r.width - 2;                                             // 0062551E..26
    if (*panel_width <= min_width) *panel_width = min_width;                       // 00625529..2F
}
}
extern "C" {
// ---- 006253A0 FixMessageLabel (RET) ----
void __fastcall gui_m48_message_box_fix_message_label_006253a0(GuiMessageBox* self, void*) noexcept(false) {
    Extent e = self->panel_extent;                                                 // 006253A8..C1 (+95C)
    Extent l = self->list_extent;                                                  // 006253C5..FE (+96C)
    const Word flags = self->mb_flags;                                             // 006253F4
    if (flags & 0x10u) { e.height += 0x20; l.top += 0x20; e.top -= 0x10; }         // 006253F7..24
    if ((flags & 4u) && !(flags & 8u)) {                                           // 00625428..2E
        if (!(flags & 2u)) {                                                       // 00625430..32
            const I32 h = self->cancel.extent.height;                              // 00625434 (+4C8)
            e.height = e.height + (-2 - h);                                        // 0062543A..45
            e.top += half(h + 2);                                                  // 00625447..61
        }
    } else {
        const I32 h = self->ok.extent.height;                                      // 0062544C (+304)
        e.height -= h;                                                             // 00625452..56
        e.top += half(h);                                                          // 00625458..61
    }
    if (self->ok.flags & 2u) fit_button(&self->ok, word_at(self, 0x378), &e.width);           // 0062546B..2F
    if (self->cancel.flags & 2u) fit_button(&self->cancel, word_at(self, 0x53C), &e.width);   // 00625533..F9
    l.width += e.width - self->panel_extent.width;                                 // 006255FD..0D (+964)
    self->vtbl->set_extent(self, nullptr, &e);                                     // 00625609..18
    self->list.vtbl->set_extent(&self->list, nullptr, &l);                         // 0062561B..2E
    gui_load_listbox_scrollbar_extent_004181f0(&self->list, nullptr, 0x10, 1);     // 00625631..37
    gui_m48_message_box_add_message_label_006252f0(self, nullptr);                 // 0062563C..3E
    const I32 scroll_width = self->list.scroll.extent.width;                       // 00625643 (+78C)
    gui_load_listbox_scrollbar_extent_004181f0(&self->list, nullptr, 0, 1);        // 00625649..4F
    if (list_scrolls(&self->list)) {                                               // 00625654..88
        const I32 line = gui_load_text_font_height_00414ee0(&self->message.text, nullptr);   // 0062568E..99
        for (;;) {                                                                 // 0062569B / 006256A0
            if (!list_scrolls(&self->list)) break;                                 // 006256A0..D0
            if (e.width >= 0x1B8 && e.height >= 0x118) break;                      // 006256D6..E7
            if (e.height > 0xA0 && e.width < 0x1B8) {                              // 006256ED..FA
                l.width += 0x28; e.width += 0x28; e.left -= 0x14;                  // 006256FC..13
                self->list.vtbl->set_extent(&self->list, nullptr, &l);             // 00625717..1C
                gui_m48_message_box_add_message_label_006252f0(self, nullptr);     // 0062571F..21
                if (!list_scrolls(&self->list)) break;                             // 00625726..52
            }
            if (e.height >= 0x118) continue;                                       // 00625758..5D
            l.height += line;                                                      // 00625763..74
            e.height += line;                                                      // 00625767..69
            e.top += -half(line);                                                  // 0062576D..84
            self->list.vtbl->set_extent(&self->list, nullptr, &l);                 // 00625788..8F
            gui_m48_message_box_add_message_label_006252f0(self, nullptr);         // 00625792..94
        }
        self->vtbl->set_extent(self, nullptr, &e);                                 // 0062579E..A7
    }
    if (list_scrolls(&self->list)) {                                               // 006257AA..D6
        gui_load_listbox_scrollbar_extent_004181f0(&self->list, nullptr, scroll_width, 1);   // 006257D8..DD
        gui_m48_message_box_add_message_label_006252f0(self, nullptr);             // 006257E2..E4
    }
    const I32 center = half(self->extent.width);                                   // 006257E9..1B
    Extent r = self->icon.extent;                                                  // 006257F1..17 (+1B8)
    r.left = center - half(r.width);                                               // 0062580E..23
    self->icon.vtbl->set_extent(&self->icon, nullptr, &r);                         // 00625827..32
    r = self->ok.extent;                                                           // 00625835..64 (+2F8)
    r.top = self->list.extent.height + self->list.extent.top + 4;                  // 00625850..68 (+68C, +684)
    r.left = center - half(r.width);                                               // 0062586C..7B
    self->ok.vtbl->set_extent(&self->ok, nullptr, &r);                             // 0062587F..8A
    r = self->cancel.extent;                                                       // 0062588D..A9 (+4BC)
    r.top = self->ok.extent.height + self->ok.extent.top + 2;                      // 006258A6..B8
    r.left = center - half(r.width);                                               // 006258BC..D4
    self->cancel.vtbl->set_extent(&self->cancel, nullptr, &r);                     // 006258C3..D8
    gui_m48_panel_center_0040a600(self, nullptr);                                  // 006258DB..DD
}
// ---- 00627220 CSWGuiControllerLossBox::OnPanelAdded (table 7513F8 +48) ----
void __fastcall gui_m48_controller_loss_on_added_00627220(GuiMessageBox* self, void*) noexcept(false) {
    self->mb_flags &= 0xFFFFFFFDu;                                                 // 00627223
    self->b60 = 2;                                                                 // 00627227
    gui_m48_message_box_update_controls_00625200(self, nullptr);                   // 0062722B
    panel_slot(self, 0x6C, 0xBD0Eu);                                               // 00627230..37 (SetMessage strref)
    self->callback_object = event_target_of(self);                                 // 0062723A
    self->callback = event_func_of(&MessageBoxHandlers::OnAccept);                 // 0062723D (00625A40)
    self->callback_argument = 0;                                                   // 00627244
    gui_m48_message_box_on_added_006258f0(self, nullptr);                          // 0062724E (JMP)
}

// ======== CSWGuiTutorialBox
}
namespace kotor_original {
// ---- 006AA100 ctor (EH state 0: ~CSWGuiMessageBox) ----
void* __fastcall kotor_fastcall_006aa100(void* memory, void*, void* gui_manager) {
    TutorialBox* self = static_cast<TutorialBox*>(kotor_fastcall_00626df0(memory, nullptr, gui_manager));   // 006AA11F..24
    try {
        self->u984 = 0x7F000000u;                                                  // 006AA130
        self->u988 = 0x7F000000u;                                                  // 006AA136
        set_vptr(self, &gui_m48_tutorial_box_methods_00755cd0);                   // 006AA149 (755CD0)
        self->u98c = 0;                                                            // 006AA14F
        self->paused_by_combat = 0;                                                // 006AA155
        self->row = 0xFF;                                                          // 006AA15B
        self->message_index = 0;                                                   // 006AA162
        menu_resources::Ref font;
        font.construct_00406d80("fnt_d16x16");                                     // 006AA13C..68 (00753DA0)
        gui_load_text_params_set_base_font_00415dd0(reinterpret_cast<unsigned char*>(self) + 0x15C, nullptr, &font);   // 006AA16D..78
        self->b60 = 0x0F;                                                          // 006AA181
    } catch (...) { gui_m48_message_box_destruct_006270a0(self, nullptr); throw; }
    return self;
}
}
extern "C" {
// ---- 006AA850 dtor: JMP 006270A0.  006AA830 scalar deleting destructor.
void __fastcall gui_m48_tutorial_box_destruct_006aa850(TutorialBox* self, void*) { gui_m48_message_box_destruct_006270a0(self, nullptr); }
void* __fastcall gui_m48_tutorial_box_deleting_destruct_006aa830(TutorialBox* self, void*, Word flags) {
    gui_m48_tutorial_box_destruct_006aa850(self, nullptr);                         // 006AA833
    if (flags & 1u) live_free(self);                                               // 006AA838..40
    return self;
}
// ---- 006AA8A0 HandleInputEvent (RET 8) ----
void __fastcall gui_m48_tutorial_box_handle_input_006aa8a0(TutorialBox* self, void*, I32 event, I32 value) noexcept(false) {
    // 006AA8A5..D8: with a nonzero value, events 27h/28h, 2Dh/2Eh and 1F6h try SetNextMessage and stop when it returns a
    // nonzero strref; 29h..2Ch call CSWGuiMessageBox::HandleInputEvent directly (006AA8C5, same call as the fall-through).
    if (value != 0 && (event == 0x27 || event == 0x28 || event == 0x2D || event == 0x2E || event == 0x1F6) &&
        gui_m48_tutorial_box_set_next_message_006aa670(self, nullptr) != 0) return;   // 006AA8DA..E3
    gui_m48_message_box_handle_input_006250f0(self, nullptr, event, value);        // 006AA8E5..E9 / 006AA8C5..C7
}
// ---- 006AA860 OnPanelAdded (RET) ----
void __fastcall gui_m48_tutorial_box_on_added_006aa860(TutorialBox* self, void*) noexcept(false) {
    gui_m48_message_box_on_added_006258f0(self, nullptr);                          // 006AA863
    const unsigned paused = kotor_fastcall_005edc10(app_client(), nullptr);        // 006AA868..70
    self->paused_by_combat = paused;                                               // 006AA877
    if (paused == 0) kotor_fastcall_005edc20(app_client(), nullptr, 1, 2, 0);      // 006AA87E..8E (EAX = 0 pushed)
}
// ---- 006AA040 SetMessage(ulong strref) (RET 4) ----
void __fastcall gui_m48_tutorial_box_set_message_006aa040(TutorialBox* self, void*, Word strref) noexcept(false) {
    String text;                                                                   // [esp+4] (returned by GetSimpleString)
    tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(&text), strref);   // 006AA05F..6B
    tlk_core::ParseStr0041dd30(tlk(), nullptr, &text);                             // 006AA070..83
    GuiM48StringBits argument;                                                     // outgoing argument slot
    WindowCopy005e5b00(reinterpret_cast<String*>(&argument), nullptr, &text);      // 006AA088..96
    gui_m48_message_box_set_message_006271a0(self, nullptr, argument);             // 006AA09B..9D (direct call)
}                                                                                  // 006AA0A2..AE ~text
// ---- 006AA670 SetNextMessage (RET; EAX = message strref or 0) ----
I32 __fastcall gui_m48_tutorial_box_set_next_message_006aa670(TutorialBox* self, void*) noexcept(false) {
    String column;                                                                 // [esp+0C] (005B3190)
    String icon_name;                                                              // [esp+14] (005B3190)
    std::int32_t message = 0;                                                      // [esp+04]
    std::int32_t next = 0;                                                         // [esp+08]
    kotor_reconstruction::StartupFormat005e5680(&column, "Message%i", static_cast<int>(self->message_index));   // 006AA6A5..C4 (00755D60)
    ++self->message_index;                                                         // 006AA6C9..D4
    auto table = [] { return reinterpret_cast<void*>(word_at(reinterpret_cast<const void*>(word_at(kotor_cell_rules_007a3a28, 0xB8)), 0x118)); };
    if (!c2da_current_get_int_00413660(table(), nullptr, self->row, &column, &message)) return 0;   // 006AA6DA..07
    kotor_reconstruction::StartupFormat005e5680(&column, "Message%i", static_cast<int>(self->message_index));   // 006AA70D..1F
    const std::int32_t has_next = c2da_current_get_int_00413660(table(), nullptr, self->row, &column, &next);   // 006AA724..4D
    panel_slot(self, 0x74, has_next ? 0x96DFu : 0x62Cu);                           // 006AA752..6B (vslot +74 SetOkButtonMessage)
    {
        String icon_column("Icon");                                                // 006AA76F..78 (007488B8)
        const std::int32_t found = c2da_current_get_string_00413270(table(), nullptr, self->row, &icon_column, &icon_name);   // 006AA77D..A7
        if (found) {                                                               // 006AA7BC..BF (after ~icon_column)
            alignas(4) menu_resources::Ref ref;
            async_ref_construct_string_00406d60(&ref, nullptr, &icon_name);        // 006AA7C1..CA
            gui_m48_message_box_set_icon_00627160(self, nullptr, &ref);            // 006AA7CF..D6
        }
    }
    kotor_fastcall_00627130(self, nullptr, 0);                                     // 006AA7DB..DF (SetAllowCancel 0)
    panel_slot(self, 0x6C, static_cast<Word>(message));                            // 006AA7E4..ED (vslot +6C SetMessage)
    return message;                                                                // 006AA7F0 (EAX)
}

// ======== CSWGuiInGameSkillEntry (0x310, table 755ED0)
// ---- 006ACC50 ctor (EH states 0..2) ----
void* __fastcall gui_m48_skill_entry_construct_006acc50(void* memory, void*) noexcept(false) {
    SkillEntry* self = static_cast<SkillEntry*>(memory);
    gui_load_label_hilight_construct_0041b5c0(self, nullptr);                      // 006ACC6D
    try {
        set_vptr(self, &gui_m48_skill_entry_methods_00755ed0);                    // 006ACC80 (755ED0)
        gui_load_border_construct_004167f0(&self->b1b4, nullptr);                  // 006ACC72..86
        try {
            gui_load_border_construct_004167f0(&self->b228, nullptr);              // 006ACC8B..96
            try { gui_load_border_construct_004167f0(&self->b29c, nullptr); }      // 006ACC9B..A6
            catch (...) { gui_load_border_destruct_004151c0(&self->b228, nullptr); throw; }
        } catch (...) { gui_load_border_destruct_004151c0(&self->b1b4, nullptr); throw; }
    } catch (...) { gui_m48_label_hilight_destruct_0041b610(self, nullptr); throw; }
    return self;
}
// ---- 006ABA80 dtor (no vptr store; EH states 2..0) ----
void __fastcall gui_m48_skill_entry_destruct_006aba80(void* object, void*) {
    SkillEntry* self = static_cast<SkillEntry*>(object);
    gui_load_border_destruct_004151c0(&self->b29c, nullptr);                       // 006ABA9D..AB
    gui_load_border_destruct_004151c0(&self->b228, nullptr);                       // 006ABAB0..BB
    gui_load_border_destruct_004151c0(&self->b1b4, nullptr);                       // 006ABAC0..CB
    gui_m48_label_hilight_destruct_0041b610(self, nullptr);                        // 006ABAD0..DA
}
// ---- 006ACCC0 scalar deleting destructor.
void* __fastcall gui_m48_skill_entry_deleting_destruct_006accc0(SkillEntry* self, void*, Word flags) {
    gui_m48_skill_entry_destruct_006aba80(self, nullptr);                          // 006ACCC3
    if (flags & 1u) live_free(self);                                               // 006ACCC8..D0
    return self;
}
// ---- 006AB8E0 SetExtent (RET 4): only when width > 2Ah. ----
void __fastcall gui_m48_skill_entry_set_extent_006ab8e0(SkillEntry* self, void*, const Extent* extent) {
    if (extent->width <= 0x2A) return;                                             // 006AB8EB..F5
    self->extent = *extent;                                                        // 006AB8FC..14
    Extent r{self->extent.left, self->extent.top, 0x2A, 0x2A};                     // 006AB917..3A
    extent04(&self->b1b4, &r);                                                     // 006AB91C..3E
    extent04(&self->b228, &r);                                                     // 006AB941..52
    extent04(&self->b29c, &r);                                                     // 006AB955..66
    r.width = self->extent.width - 0x2A;                                           // 006AB969..77
    r.left += 0x2A;                                                                // 006AB96C..85
    extent04(&self->border, &r);                                                   // 006AB970..89 (+5C)
    extent04(&self->hilight, &r);                                                  // 006AB98C..9F (+140)
    const Extent a = inner_of(&self->hilight);                                     // 006AB9A8..B5
    const Extent b = inner_of(&self->border);                                      // 006AB9BA..C2
    Extent i;
    extent04(&self->text, gui_load_extent_intersection_0040a4a0(&i, &b, &a));      // 006AB9C7..D8 (+D0)
}
// ---- 006AB9F0 Draw (RET 4) ----
void __fastcall gui_m48_skill_entry_draw_006ab9f0(SkillEntry* self, void*, Word delta) {
    if (self->flags & 1u) { draw0c(&self->hilight, delta); draw0c(&self->b228, delta); }   // 006AB9F3..14 / 006ABA25..28
    else { draw0c(&self->border, delta); draw0c(&self->b1b4, delta); }                     // 006ABA16..28
    draw0c(&self->b29c, delta);                                                    // 006ABA2B..38
    draw0c(&self->text, delta);                                                    // 006ABA3B..48
}
// ---- 006AC930 Initialize(CSWGuiText* text, CSWGuiBorder* border, CSWGuiBorder* hilight, int width) (RET 10h) ----
void __fastcall gui_m48_skill_entry_initialize_006ac930(SkillEntry* self, void*, gui_load::Text* text, Border* border,
                                                        Border* hilight, I32 width) noexcept(false) {
    unsigned char* const p = reinterpret_cast<unsigned char*>(self);
    kotor_fastcall_004171e0(p + 0xE8, nullptr, reinterpret_cast<unsigned char*>(text) + 0x18);   // 006AC936..4B
    gui_load_border_params_assign_004150c0(reinterpret_cast<gui_load::BorderParams*>(p + 0x70), nullptr,
        reinterpret_cast<const gui_load::BorderParams*>(reinterpret_cast<unsigned char*>(border) + 0x14));   // 006AC950..60
    std::memcpy(p + 0x80, &borrowed_exit_color_007a23b4, 12);                       // 006AC965..85 (colour words 007A23B4..BC)
    gui_load_border_params_assign_004150c0(reinterpret_cast<gui_load::BorderParams*>(p + 0x154), nullptr,
        reinterpret_cast<const gui_load::BorderParams*>(reinterpret_cast<unsigned char*>(hilight) + 0x14));  // 006AC975..94
    std::memcpy(p + 0x164, &borrowed_enter_color_007a23c0, 12);                     // 006AC999..B7 (007A23C0..C8)
    gui_load_border_params_set_pulsing_00414d00(p + 0x154, nullptr, 1, 1, 0);      // 006AC9B3..BE
    const gui_load::Vector white{1.0f, 1.0f, 1.0f};
    for (unsigned target : {0x1C8u, 0x23Cu}) {                                     // 006AC9C3..ACF3
        alignas(4) menu_resources::Ref fill, edge, corner;
        fill.construct_00406d80("lbl_hex_3");                                      // 006AC9C3..D2 / 006ACA57..66 (00755EBC)
        edge.construct_00406d80("");                                               // 006AC9D7..E0 (0073D71C)
        corner.construct_00406d80("");                                             // 006AC9E5..EE
        SafeParamsStorage params;
        gui_load::BorderParams* made = gui_load_border_params_construct_00415000(
            reinterpret_cast<gui_load::BorderParams*>(&params), nullptr, &corner, &edge, &fill, 0, 0, 1.0f, &white, 0);   // 006AC9F3..ACA2E
        gui_load_border_params_assign_004150c0(reinterpret_cast<gui_load::BorderParams*>(p + target), nullptr, made);   // 006ACA33..36
        if (target == 0x1C8u) std::memcpy(p + 0x1D8, &borrowed_exit_color_007a23b4, 12);    // 006ACA3B..54
        else {
            std::memcpy(p + 0x24C, &borrowed_enter_color_007a23c0, 12);             // 006ACACF..EC
            gui_load_border_params_set_pulsing_00414d00(p + 0x23C, nullptr, 1, 0, 1);   // 006ACAE8..F3
        }
    }
    const Extent r{0, 0, width, 0x2A};                                             // 006ACAF8..1C
    self->vtbl->set_extent(self, nullptr, &r);                                     // 006ACAFF..24 (vslot +04)
}

// ======== CSWGuiSkillInfoBox
}
namespace kotor_original {
// ---- 006CE7C0 ctor (EH states 0..7) ----
void* __fastcall kotor_fastcall_006ce7c0(void* memory, void*, void* gui_manager) {
    SkillInfoBox* self = static_cast<SkillInfoBox*>(memory);
    gui_load_panel_construct_0040b570(self, nullptr, static_cast<gui_load::Object*>(gui_manager));   // 006CE7E2..E7
    try {
        set_vptr(self, &gui_m48_skill_info_box_methods_00757940);                 // 006CE7F9 (757940)
        gui_load_listbox_construct_0041be40(&self->list, nullptr);                 // 006CE7EC..FF
        try {
            gui_load_label_construct_0041acd0(&self->message, nullptr);            // 006CE804..11
            try {
                gui_load_button_construct_0041c0f0(&self->ok, nullptr);            // 006CE816..21
                try {
                    crt_eh_vector_constructor_006fb55f(self->entries, 0x310, 10, &gui_m48_skill_entry_construct_006acc50,
                                                      &gui_m48_skill_entry_destruct_006aba80);   // 006CE826..43
                } catch (...) { gui_load_button_destruct_00419da0(&self->ok, nullptr); throw; }
            } catch (...) { gui_load_label_destruct_00419d10(&self->message, nullptr); throw; }
        } catch (...) { gui_load_listbox_destruct_0041b060(&self->list, nullptr); throw; }
    } catch (...) { gui_load_panel_destruct_0040cf70(self, nullptr); throw; }
    try {
        menu_resources::Ref layout;
        layout.construct_00406d80("skillinfo");                                    // 006CE848..56 (007579B8)
        gui_load_panel_start_layout_0040a680(self, nullptr, &layout);              // 006CE85B..62
        { String name("LB_SKILLS"); gui_load_panel_init_control_0040b930(self, nullptr, &self->list, &name, 1); }      // 006CE867..92
        { String name("LBL_MESSAGE"); gui_load_panel_init_control_0040b930(self, nullptr, &self->message, &name, 1); } // 006CE897..C2
        { String name("BTN_OK"); gui_load_panel_init_control_0040b930(self, nullptr, &self->ok, &name, 1); }          // 006CE8C7..F8
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                        // 006CE8FD..FF
        self->list.lb_flags |= 0x200u;                                             // 006CE904
        reinterpret_cast<unsigned char*>(self)[0x325] = 1;                         // 006CE914 (list sound byte)
        panel_slot(&self->list, 0x88, 0);                                          // 006CE90E..1B (vslot +88 SetEnabled(0))
        Control* const proto = self->list.proto;                                   // 006CE921 (+330)
        struct V54 { void* s[0x15]; LabelHilight* (__fastcall* as_label_hilight)(Control*, void*); };
        LabelHilight* const source = vt<V54>(proto)->as_label_hilight(proto, nullptr);   // 006CE927..29 (vslot +54)
        const I32 width = self->list.inner.width - 2 * static_cast<I32>(self->list.padding);   // 006CE92C..3B
        for (int i = 0; i < 10; ++i) {                                             // 006CE959..81
            SkillEntry* e = &self->entries[i];
            struct VA8 { void* s[0x2A]; void (__fastcall* init)(SkillEntry*, void*, gui_load::Text*, Border*, Border*, I32); };
            vt<VA8>(static_cast<Control*>(e))->init(e, nullptr, &source->text, &source->border, &source->hilight, width);   // 006CE960..74
        }
        self->ok.flags &= 0xFFFFFFFBu;                                             // 006CE983..9A
        gui_load_control_add_event_0041ab20(&self->ok, nullptr, 0x27, event_target_of(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::AcceptButtonCallback_00624ba0));   // 006CE98F..9D (00624BA0)
        gui_m48_panel_center_0040a600(self, nullptr);                              // 006CE9A2..A4
    } catch (...) { gui_m48_skill_info_destruct_006cd320(self, nullptr); throw; }
    return self;
}
}
extern "C" {
// ---- 006CD320 dtor (EH states 4..0) ----
void __fastcall gui_m48_skill_info_destruct_006cd320(SkillInfoBox* self, void*) {
    set_vptr(self, &gui_m48_skill_info_box_methods_00757940);                     // 006CD33E
    gui_load_listbox_clear_items_00419a90(&self->list, nullptr);                   // 006CD344..51
    c2da_destroy_array_006fa852(self->entries, 0x310, 10, &gui_m48_skill_entry_destruct_006aba80);   // 006CD356..6E
    gui_load_button_destruct_00419da0(&self->ok, nullptr);                         // 006CD373..7E
    gui_load_label_destruct_00419d10(&self->message, nullptr);                     // 006CD383..8E
    gui_load_listbox_destruct_0041b060(&self->list, nullptr);                      // 006CD393..9A
    gui_load_panel_destruct_0040cf70(self, nullptr);                               // 006CD39F..A9
}
// ---- 006CDFA0 scalar deleting destructor.
void* __fastcall gui_m48_skill_info_deleting_destruct_006cdfa0(SkillInfoBox* self, void*, Word flags) {
    gui_m48_skill_info_destruct_006cd320(self, nullptr);                           // 006CDFA3
    if (flags & 1u) live_free(self);                                               // 006CDFA8..B0
    return self;
}
// ---- 006CD3C0 HandleInputEvent (RET 8): events 27h/28h/2Dh/2Eh (byte table 006CD414) close the panel.
void __fastcall gui_m48_skill_info_handle_input_006cd3c0(SkillInfoBox* self, void*, I32 event, I32 value) noexcept(false) {
    if (value != 0 && (event == 0x27 || event == 0x28 || event == 0x2D || event == 0x2E)) {   // 006CD3CB..E0
        reinterpret_cast<kih::Manager*>(self->manager)->PlayGuiSound(0);           // 006CD3E7..EC
        gui_m48_panel_set_to_destroy_00624a00(self, nullptr, 0);                   // 006CD3F1..F5
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, value);            // 006CD3FA..FE
}
}
// gui_m48_message_box_methods_0074fdb0: original table 0074FDB0 (33 slots)
struct GuiM48MessageBoxMethods {
    void* (__fastcall* s00)(gui_lb::GuiMessageBox*, void*, gui_load::Word);  // +00 00627080 CSWGuiMessageBox::~CSWGuiMessageBox
    void (__fastcall* s04)(gui_load::Object*, void*, const gui_load::Extent*);  // +04 0040A5A0 CSWGuiPanel::SetExtent
    void (__fastcall* s08)(gui_load::Object*, void*, gui_load::Object*, gui_load::Word);  // +08 0040A630 CSWGuiPanel::SetActiveControl
    void (__fastcall* s0c)(gui_load::Object*, void*, gui_load::Word);  // +0C 0060E760 void_return
    gui_load::Word (__fastcall* s10)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +10 00409EB0 return_zero_0x8
    void (__fastcall* s14)(gui_load::Object*, void*);  // +14 004015A0 void_return
    void (__fastcall* s18)(gui_load::Object*, void*);  // +18 004015A0 void_return
    void (__fastcall* s1c)(gui_load::Object*, void*);  // +1C 004015A0 void_return
    void (__fastcall* s20)(gui_load::Object*, void*);  // +20 004015A0 void_return
    void (__fastcall* s24)(gui_load::Object*, void*);  // +24 004015A0 void_return
    gui_load::Object* (__fastcall* s28)(gui_load::Object*, void*);  // +28 00641DB0 self_return
    gui_load::Word (__fastcall* s2c)(gui_load::Object*, void*);  // +2C 0063E7F0 return_zero
    gui_load::Word (__fastcall* s30)(gui_load::Object*, void*);  // +30 0063E7F0 return_zero
    void (__fastcall* s34)(gui_load::Panel*, void*, gui_load::Word);  // +34 0040B760 CSWGuiPanel::Draw
    void (__fastcall* s38)(gui_load::Object*, void*, gui_load::Word);  // +38 0060E760 void_return
    void (__fastcall* s3c)(gui_lb::GuiMessageBox*, void*, gui_lb::I32, gui_lb::I32);  // +3C 006250F0 CSWGuiMessageBox::HandleInputEvent
    gui_load::Object* (__fastcall* s40)(gui_load::Object*, void*, gui_load::I32, gui_load::I32);  // +40 0040B690 CSWGuiPanel::HitCheckMouse
    gui_load::Object* (__fastcall* s44)(gui_load::Object*, void*);  // +44 0040B630 CSWGuiPanel::GetActiveControl
    void (__fastcall* s48)(gui_lb::GuiMessageBox*, void*);  // +48 006258F0 CSWGuiMessageBox::OnPanelAdded
    void (__fastcall* s4c)(gui_lb::GuiMessageBox*, void*);  // +4C 00625A00 CSWGuiMessageBox::OnPanelRemoved
    void (__fastcall* s50)(gui_load::Object*, void*, gui_load::Word);  // +50 0040B640 CSWGuiPanel::OnAButtonPressed
    void (__fastcall* s54)(gui_load::Object*, void*, gui_load::Word);  // +54 0040B650 CSWGuiPanel::OnBButtonPressed
    void (__fastcall* s58)(gui_load::Object*, void*, gui_load::Word);  // +58 0040B660 CSWGuiPanel::OnXButtonPressed
    void (__fastcall* s5c)(gui_load::Object*, void*, gui_load::Word);  // +5C 0040B670 CSWGuiPanel::OnYButtonPressed
    void (__fastcall* s60)(gui_load::Object*, void*, gui_load::Word);  // +60 0040B680 CSWGuiPanel::OnBlackButtonPressed
    gui_load::String* (__fastcall* s64)(gui_load::Panel*, void*, gui_load::String*);  // +64 0040A900 CSWGuiPanel::GetFullScreenBG
    void (__fastcall* s68)(gui_load::Panel*, void*);  // +68 0040A9C0 CSWGuiPanel::ResetFont
    void (__fastcall* s6c)(gui_lb::GuiMessageBox*, void*, gui_load::Word);  // +6C 006249D0 CSWGuiMessageBox::SetMessage
    void (__fastcall* s70)(gui_lb::GuiMessageBox*, void*, GuiM48StringBits);  // +70 006271A0 CSWGuiMessageBox::SetMessage
    void (__fastcall* s74)(gui_lb::GuiMessageBox*, void*, gui_load::Word);  // +74 00625260 CSWGuiMessageBox::SetOkButtonMessage
    void (__fastcall* s78)(gui_lb::GuiMessageBox*, void*, gui_load::Word);  // +78 00625270 CSWGuiMessageBox::SetCancelButtonMessage
    void (__fastcall* s7c)(gui_lb::GuiMessageBox*, void*);  // +7C 00625280 CSWGuiMessageBox::HideButtons
    void (__fastcall* s80)(gui_lb::GuiMessageBox*, void*);  // +80 006252A0 CSWGuiMessageBox::ResetButtonMessage
};
static_assert(sizeof(GuiM48MessageBoxMethods) == 132, "74FDB0 slot count");
extern "C" const GuiM48MessageBoxMethods gui_m48_message_box_methods_0074fdb0 = {
    &gui_m48_message_box_deleting_destruct_00627080,  // +00
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
    &gui_m48_message_box_handle_input_006250f0,  // +3C
    &gui_load_panel_hit_check_0040b690,  // +40
    &gui_load_panel_active_control_0040b630,  // +44
    &gui_m48_message_box_on_added_006258f0,  // +48
    &gui_m48_message_box_on_removed_00625a00,  // +4C
    &gui_load_panel_button_a_0040b640,  // +50
    &gui_load_panel_button_b_0040b650,  // +54
    &gui_load_panel_button_x_0040b660,  // +58
    &gui_load_panel_button_y_0040b670,  // +5C
    &gui_load_panel_button_black_0040b680,  // +60
    &gui_load_panel_full_screen_bg_0040a900,  // +64
    &gui_load_panel_reset_font_0040a9c0,  // +68
    &gui_m48_message_box_set_message_strref_006249d0,  // +6C
    &gui_m48_message_box_set_message_006271a0,  // +70
    &gui_m48_message_box_set_ok_text_00625260,  // +74
    &gui_m48_message_box_set_cancel_text_00625270,  // +78
    &gui_m48_message_box_hide_buttons_00625280,  // +7C
    &gui_m48_message_box_reset_buttons_006252a0,  // +80
};

// kotor_vtable_007513f8: original table 007513F8 (33 slots)
struct GuiM48ControllerLossBoxMethods {
    void* (__fastcall* s00)(gui_lb::TutorialBox*, void*, gui_load::Word);  // +00 006AA830 CSWGuiMessageBox::~CSWGuiMessageBox
    void (__fastcall* s04)(gui_load::Object*, void*, const gui_load::Extent*);  // +04 0040A5A0 CSWGuiPanel::SetExtent
    void (__fastcall* s08)(gui_load::Object*, void*, gui_load::Object*, gui_load::Word);  // +08 0040A630 CSWGuiPanel::SetActiveControl
    void (__fastcall* s0c)(gui_load::Object*, void*, gui_load::Word);  // +0C 0060E760 void_return
    gui_load::Word (__fastcall* s10)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +10 00409EB0 return_zero_0x8
    void (__fastcall* s14)(gui_load::Object*, void*);  // +14 004015A0 void_return
    void (__fastcall* s18)(gui_load::Object*, void*);  // +18 004015A0 void_return
    void (__fastcall* s1c)(gui_load::Object*, void*);  // +1C 004015A0 void_return
    void (__fastcall* s20)(gui_load::Object*, void*);  // +20 004015A0 void_return
    void (__fastcall* s24)(gui_load::Object*, void*);  // +24 004015A0 void_return
    gui_load::Object* (__fastcall* s28)(gui_load::Object*, void*);  // +28 00641DB0 self_return
    gui_load::Word (__fastcall* s2c)(gui_load::Object*, void*);  // +2C 0063E7F0 return_zero
    gui_load::Word (__fastcall* s30)(gui_load::Object*, void*);  // +30 0063E7F0 return_zero
    void (__fastcall* s34)(gui_load::Panel*, void*, gui_load::Word);  // +34 0040B760 CSWGuiPanel::Draw
    void (__fastcall* s38)(gui_load::Object*, void*, gui_load::Word);  // +38 0060E760 void_return
    void (__fastcall* s3c)(gui_lb::GuiMessageBox*, void*, gui_lb::I32, gui_lb::I32);  // +3C 006250F0 CSWGuiMessageBox::HandleInputEvent
    gui_load::Object* (__fastcall* s40)(gui_load::Object*, void*, gui_load::I32, gui_load::I32);  // +40 0040B690 CSWGuiPanel::HitCheckMouse
    gui_load::Object* (__fastcall* s44)(gui_load::Object*, void*);  // +44 0040B630 CSWGuiPanel::GetActiveControl
    void (__fastcall* s48)(gui_lb::GuiMessageBox*, void*);  // +48 00627220 CSWGuiControllerLossBox::OnPanelAdded
    void (__fastcall* s4c)(gui_lb::GuiMessageBox*, void*);  // +4C 00625A00 CSWGuiMessageBox::OnPanelRemoved
    void (__fastcall* s50)(gui_load::Object*, void*, gui_load::Word);  // +50 0040B640 CSWGuiPanel::OnAButtonPressed
    void (__fastcall* s54)(gui_load::Object*, void*, gui_load::Word);  // +54 0040B650 CSWGuiPanel::OnBButtonPressed
    void (__fastcall* s58)(gui_load::Object*, void*, gui_load::Word);  // +58 0040B660 CSWGuiPanel::OnXButtonPressed
    void (__fastcall* s5c)(gui_load::Object*, void*, gui_load::Word);  // +5C 0040B670 CSWGuiPanel::OnYButtonPressed
    void (__fastcall* s60)(gui_load::Object*, void*, gui_load::Word);  // +60 0040B680 CSWGuiPanel::OnBlackButtonPressed
    gui_load::String* (__fastcall* s64)(gui_load::Panel*, void*, gui_load::String*);  // +64 0040A900 CSWGuiPanel::GetFullScreenBG
    void (__fastcall* s68)(gui_load::Panel*, void*);  // +68 0040A9C0 CSWGuiPanel::ResetFont
    void (__fastcall* s6c)(gui_lb::GuiMessageBox*, void*, gui_load::Word);  // +6C 006249D0 CSWGuiMessageBox::SetMessage
    void (__fastcall* s70)(gui_lb::GuiMessageBox*, void*, GuiM48StringBits);  // +70 006271A0 CSWGuiMessageBox::SetMessage
    void (__fastcall* s74)(gui_lb::GuiMessageBox*, void*, gui_load::Word);  // +74 00625260 CSWGuiMessageBox::SetOkButtonMessage
    void (__fastcall* s78)(gui_lb::GuiMessageBox*, void*, gui_load::Word);  // +78 00625270 CSWGuiMessageBox::SetCancelButtonMessage
    void (__fastcall* s7c)(gui_lb::GuiMessageBox*, void*);  // +7C 00625280 CSWGuiMessageBox::HideButtons
    void (__fastcall* s80)(gui_lb::GuiMessageBox*, void*);  // +80 006252A0 CSWGuiMessageBox::ResetButtonMessage
};
static_assert(sizeof(GuiM48ControllerLossBoxMethods) == 132, "7513F8 slot count");
extern "C" const GuiM48ControllerLossBoxMethods kotor_vtable_007513f8 = {
    &gui_m48_tutorial_box_deleting_destruct_006aa830,  // +00
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
    &gui_m48_message_box_handle_input_006250f0,  // +3C
    &gui_load_panel_hit_check_0040b690,  // +40
    &gui_load_panel_active_control_0040b630,  // +44
    &gui_m48_controller_loss_on_added_00627220,  // +48
    &gui_m48_message_box_on_removed_00625a00,  // +4C
    &gui_load_panel_button_a_0040b640,  // +50
    &gui_load_panel_button_b_0040b650,  // +54
    &gui_load_panel_button_x_0040b660,  // +58
    &gui_load_panel_button_y_0040b670,  // +5C
    &gui_load_panel_button_black_0040b680,  // +60
    &gui_load_panel_full_screen_bg_0040a900,  // +64
    &gui_load_panel_reset_font_0040a9c0,  // +68
    &gui_m48_message_box_set_message_strref_006249d0,  // +6C
    &gui_m48_message_box_set_message_006271a0,  // +70
    &gui_m48_message_box_set_ok_text_00625260,  // +74
    &gui_m48_message_box_set_cancel_text_00625270,  // +78
    &gui_m48_message_box_hide_buttons_00625280,  // +7C
    &gui_m48_message_box_reset_buttons_006252a0,  // +80
};

// gui_m48_tutorial_box_methods_00755cd0: original table 00755CD0 (33 slots)
struct GuiM48TutorialBoxMethods {
    void* (__fastcall* s00)(gui_lb::TutorialBox*, void*, gui_load::Word);  // +00 006AA830 CSWGuiMessageBox::~CSWGuiMessageBox
    void (__fastcall* s04)(gui_load::Object*, void*, const gui_load::Extent*);  // +04 0040A5A0 CSWGuiPanel::SetExtent
    void (__fastcall* s08)(gui_load::Object*, void*, gui_load::Object*, gui_load::Word);  // +08 0040A630 CSWGuiPanel::SetActiveControl
    void (__fastcall* s0c)(gui_load::Object*, void*, gui_load::Word);  // +0C 0060E760 void_return
    gui_load::Word (__fastcall* s10)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +10 00409EB0 return_zero_0x8
    void (__fastcall* s14)(gui_load::Object*, void*);  // +14 004015A0 void_return
    void (__fastcall* s18)(gui_load::Object*, void*);  // +18 004015A0 void_return
    void (__fastcall* s1c)(gui_load::Object*, void*);  // +1C 004015A0 void_return
    void (__fastcall* s20)(gui_load::Object*, void*);  // +20 004015A0 void_return
    void (__fastcall* s24)(gui_load::Object*, void*);  // +24 004015A0 void_return
    gui_load::Object* (__fastcall* s28)(gui_load::Object*, void*);  // +28 00641DB0 self_return
    gui_load::Word (__fastcall* s2c)(gui_load::Object*, void*);  // +2C 0063E7F0 return_zero
    gui_load::Word (__fastcall* s30)(gui_load::Object*, void*);  // +30 0063E7F0 return_zero
    void (__fastcall* s34)(gui_load::Panel*, void*, gui_load::Word);  // +34 0040B760 CSWGuiPanel::Draw
    void (__fastcall* s38)(gui_load::Object*, void*, gui_load::Word);  // +38 0060E760 void_return
    void (__fastcall* s3c)(gui_lb::TutorialBox*, void*, gui_lb::I32, gui_lb::I32);  // +3C 006AA8A0 CSWGuiTutorialBox::HandleInputEvent
    gui_load::Object* (__fastcall* s40)(gui_load::Object*, void*, gui_load::I32, gui_load::I32);  // +40 0040B690 CSWGuiPanel::HitCheckMouse
    gui_load::Object* (__fastcall* s44)(gui_load::Object*, void*);  // +44 0040B630 CSWGuiPanel::GetActiveControl
    void (__fastcall* s48)(gui_lb::TutorialBox*, void*);  // +48 006AA860 CSWGuiTutorialBox::OnPanelAdded
    void (__fastcall* s4c)(gui_lb::GuiMessageBox*, void*);  // +4C 00625A00 CSWGuiMessageBox::OnPanelRemoved
    void (__fastcall* s50)(gui_load::Object*, void*, gui_load::Word);  // +50 0040B640 CSWGuiPanel::OnAButtonPressed
    void (__fastcall* s54)(gui_load::Object*, void*, gui_load::Word);  // +54 0040B650 CSWGuiPanel::OnBButtonPressed
    void (__fastcall* s58)(gui_load::Object*, void*, gui_load::Word);  // +58 0040B660 CSWGuiPanel::OnXButtonPressed
    void (__fastcall* s5c)(gui_load::Object*, void*, gui_load::Word);  // +5C 0040B670 CSWGuiPanel::OnYButtonPressed
    void (__fastcall* s60)(gui_load::Object*, void*, gui_load::Word);  // +60 0040B680 CSWGuiPanel::OnBlackButtonPressed
    gui_load::String* (__fastcall* s64)(gui_load::Panel*, void*, gui_load::String*);  // +64 0040A900 CSWGuiPanel::GetFullScreenBG
    void (__fastcall* s68)(gui_load::Panel*, void*);  // +68 0040A9C0 CSWGuiPanel::ResetFont
    void (__fastcall* s6c)(gui_lb::TutorialBox*, void*, gui_load::Word);  // +6C 006AA040 CSWGuiTutorialBox::SetMessage
    void (__fastcall* s70)(gui_lb::GuiMessageBox*, void*, GuiM48StringBits);  // +70 006271A0 CSWGuiMessageBox::SetMessage
    void (__fastcall* s74)(gui_lb::GuiMessageBox*, void*, gui_load::Word);  // +74 00625260 CSWGuiMessageBox::SetOkButtonMessage
    void (__fastcall* s78)(gui_lb::GuiMessageBox*, void*, gui_load::Word);  // +78 00625270 CSWGuiMessageBox::SetCancelButtonMessage
    void (__fastcall* s7c)(gui_lb::GuiMessageBox*, void*);  // +7C 00625280 CSWGuiMessageBox::HideButtons
    void (__fastcall* s80)(gui_lb::GuiMessageBox*, void*);  // +80 006252A0 CSWGuiMessageBox::ResetButtonMessage
};
static_assert(sizeof(GuiM48TutorialBoxMethods) == 132, "755CD0 slot count");
extern "C" const GuiM48TutorialBoxMethods gui_m48_tutorial_box_methods_00755cd0 = {
    &gui_m48_tutorial_box_deleting_destruct_006aa830,  // +00
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
    &gui_m48_tutorial_box_handle_input_006aa8a0,  // +3C
    &gui_load_panel_hit_check_0040b690,  // +40
    &gui_load_panel_active_control_0040b630,  // +44
    &gui_m48_tutorial_box_on_added_006aa860,  // +48
    &gui_m48_message_box_on_removed_00625a00,  // +4C
    &gui_load_panel_button_a_0040b640,  // +50
    &gui_load_panel_button_b_0040b650,  // +54
    &gui_load_panel_button_x_0040b660,  // +58
    &gui_load_panel_button_y_0040b670,  // +5C
    &gui_load_panel_button_black_0040b680,  // +60
    &gui_load_panel_full_screen_bg_0040a900,  // +64
    &gui_load_panel_reset_font_0040a9c0,  // +68
    &gui_m48_tutorial_box_set_message_006aa040,  // +6C
    &gui_m48_message_box_set_message_006271a0,  // +70
    &gui_m48_message_box_set_ok_text_00625260,  // +74
    &gui_m48_message_box_set_cancel_text_00625270,  // +78
    &gui_m48_message_box_hide_buttons_00625280,  // +7C
    &gui_m48_message_box_reset_buttons_006252a0,  // +80
};

// gui_m48_skill_info_box_methods_00757940: original table 00757940 (27 slots)
struct GuiM48SkillInfoBoxMethods {
    void* (__fastcall* s00)(gui_lb::SkillInfoBox*, void*, gui_load::Word);  // +00 006CDFA0 CSWGuiSkillInfoBox::~CSWGuiSkillInfoBox
    void (__fastcall* s04)(gui_load::Object*, void*, const gui_load::Extent*);  // +04 0040A5A0 CSWGuiPanel::SetExtent
    void (__fastcall* s08)(gui_load::Object*, void*, gui_load::Object*, gui_load::Word);  // +08 0040A630 CSWGuiPanel::SetActiveControl
    void (__fastcall* s0c)(gui_load::Object*, void*, gui_load::Word);  // +0C 0060E760 void_return
    gui_load::Word (__fastcall* s10)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +10 00409EB0 return_zero_0x8
    void (__fastcall* s14)(gui_load::Object*, void*);  // +14 004015A0 void_return
    void (__fastcall* s18)(gui_load::Object*, void*);  // +18 004015A0 void_return
    void (__fastcall* s1c)(gui_load::Object*, void*);  // +1C 004015A0 void_return
    void (__fastcall* s20)(gui_load::Object*, void*);  // +20 004015A0 void_return
    void (__fastcall* s24)(gui_load::Object*, void*);  // +24 004015A0 void_return
    gui_load::Object* (__fastcall* s28)(gui_load::Object*, void*);  // +28 00641DB0 self_return
    gui_load::Word (__fastcall* s2c)(gui_load::Object*, void*);  // +2C 0063E7F0 return_zero
    gui_load::Word (__fastcall* s30)(gui_load::Object*, void*);  // +30 0063E7F0 return_zero
    void (__fastcall* s34)(gui_load::Panel*, void*, gui_load::Word);  // +34 0040B760 CSWGuiPanel::Draw
    void (__fastcall* s38)(gui_load::Object*, void*, gui_load::Word);  // +38 0060E760 void_return
    void (__fastcall* s3c)(gui_lb::SkillInfoBox*, void*, gui_lb::I32, gui_lb::I32);  // +3C 006CD3C0 CSWGuiSkillInfoBox::HandleInputEvent
    gui_load::Object* (__fastcall* s40)(gui_load::Object*, void*, gui_load::I32, gui_load::I32);  // +40 0040B690 CSWGuiPanel::HitCheckMouse
    gui_load::Object* (__fastcall* s44)(gui_load::Object*, void*);  // +44 0040B630 CSWGuiPanel::GetActiveControl
    void (__fastcall* s48)(gui_load::Panel*, void*);  // +48 0040B870 CSWGuiPanel::OnPanelAdded
    void (__fastcall* s4c)(gui_load::Panel*, void*);  // +4C 0040C170 CSWGuiPanel::OnPanelRemoved
    void (__fastcall* s50)(gui_load::Object*, void*, gui_load::Word);  // +50 0040B640 CSWGuiPanel::OnAButtonPressed
    void (__fastcall* s54)(gui_load::Object*, void*, gui_load::Word);  // +54 0040B650 CSWGuiPanel::OnBButtonPressed
    void (__fastcall* s58)(gui_load::Object*, void*, gui_load::Word);  // +58 0040B660 CSWGuiPanel::OnXButtonPressed
    void (__fastcall* s5c)(gui_load::Object*, void*, gui_load::Word);  // +5C 0040B670 CSWGuiPanel::OnYButtonPressed
    void (__fastcall* s60)(gui_load::Object*, void*, gui_load::Word);  // +60 0040B680 CSWGuiPanel::OnBlackButtonPressed
    gui_load::String* (__fastcall* s64)(gui_load::Panel*, void*, gui_load::String*);  // +64 0040A900 CSWGuiPanel::GetFullScreenBG
    void (__fastcall* s68)(gui_load::Panel*, void*);  // +68 0040A9C0 CSWGuiPanel::ResetFont
};
static_assert(sizeof(GuiM48SkillInfoBoxMethods) == 108, "757940 slot count");
extern "C" const GuiM48SkillInfoBoxMethods gui_m48_skill_info_box_methods_00757940 = {
    &gui_m48_skill_info_deleting_destruct_006cdfa0,  // +00
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
    &gui_m48_skill_info_handle_input_006cd3c0,  // +3C
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

// gui_m48_skill_entry_methods_00755ed0: original table 00755ED0 (43 slots)
struct GuiM48SkillEntryMethods {
    void* (__fastcall* s00)(gui_lb::SkillEntry*, void*, gui_load::Word);  // +00 006ACCC0 CSWGuiInGameSkillEntry::~CSWGuiInGameSkillEntry
    void (__fastcall* s04)(gui_lb::SkillEntry*, void*, const gui_load::Extent*);  // +04 006AB8E0 CSWGuiInGameSkillEntry::SetExtent
    void (__fastcall* s08)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +08 005B5E90 void_return
    void (kih::Control::* s0c)(int);  // +0C 004189A0 CSWGuiControl::HandleMouseOver
    gui_load::Word (__fastcall* s10)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +10 00409EB0 return_zero_0x8
    void (__fastcall* s14)(gui_load::Object*, void*);  // +14 004015A0 void_return
    void (kih::Control::* s18)();  // +18 0041AB90 CSWGuiControl::HandleLMouseDown
    void (kih::Control::* s1c)();  // +1C 0041ABD0 CSWGuiControl::HandleLMouseUp
    void (__fastcall* s20)(gui_load::Object*, void*);  // +20 004015A0 void_return
    void (__fastcall* s24)(gui_load::Object*, void*);  // +24 0041AC70 CSWGuiControl::HandleRMouseUp
    gui_load::Word (__fastcall* s28)(gui_load::Object*, void*);  // +28 0063E7F0 return_zero
    gui_load::Word (__fastcall* s2c)(gui_load::Object*, void*);  // +2C 0063E7F0 return_zero
    gui_load::Object* (__fastcall* s30)(gui_load::Object*, void*);  // +30 00641DB0 self_return
    gui_load::Word (__fastcall* s34)(gui_load::Object*, void*);  // +34 004064A0 GetProperty0x10
    void (__fastcall* s38)(gui_lb::SkillEntry*, void*, gui_load::Word);  // +38 006AB9F0 CSWGuiInGameSkillEntry::Draw
    void (kih::Control::* s3c)(int, int);  // +3C 00418750 CSWGuiControl::HandleInputEvent
    void (kih::Control::* s40)(int);  // +40 004181D0 CSWGuiControl::SetActive
    kih::Control* (kih::Control::* s44)(int, int);  // +44 004187C0 CSWGuiControl::HitCheckMouse
    void (__fastcall* s48)(gui_lb::LabelHilight*, void*, gui_load::Gff*, gui_load::Struct*);  // +48 0041BA20 CSWGuiLabelHilight::Load
    gui_load::Word (__fastcall* s4c)(gui_load::Object*, void*);  // +4C 0063E7F0 return_zero
    gui_load::Object* (__fastcall* s50)(gui_load::Object*, void*);  // +50 00641DB0 self_return
    gui_load::Object* (__fastcall* s54)(gui_load::Object*, void*);  // +54 00641DB0 self_return
    gui_load::Word (__fastcall* s58)(gui_load::Object*, void*);  // +58 0063E7F0 return_zero
    gui_load::Word (__fastcall* s5c)(gui_load::Object*, void*);  // +5C 0063E7F0 return_zero
    gui_load::Word (__fastcall* s60)(gui_load::Object*, void*);  // +60 0063E7F0 return_zero
    gui_load::Word (__fastcall* s64)(gui_load::Object*, void*);  // +64 0063E7F0 return_zero
    gui_load::Word (__fastcall* s68)(gui_load::Object*, void*);  // +68 0063E7F0 return_zero
    gui_load::Word (__fastcall* s6c)(gui_load::Object*, void*);  // +6C 0063E7F0 return_zero
    gui_load::Word (__fastcall* s70)(gui_load::Object*, void*);  // +70 0063E7F0 return_zero
    gui_load::Word (__fastcall* s74)(gui_load::Object*, void*);  // +74 0063E7F0 return_zero
    gui_load::Word (__fastcall* s78)(gui_load::Object*, void*);  // +78 0063E7F0 return_zero
    void (__fastcall* s7c)(gui_load::Object*, void*, gui_load::Word);  // +7C 00418960 CSWGuiControl::HandleFocusChange
    int (kih::Control::* s80)();  // +80 004189D0 CSWGuiControl::GetIsSelectable
    void (__fastcall* s84)(gui_load::Object*, void*);  // +84 0041AC30 CSWGuiControl::HandleRMouseDown
    void (__fastcall* s88)(gui_load::Object*, void*, gui_load::Word);  // +88 00418D00 CSWGuiLabel::SetEnabled
    void (__fastcall* s8c)(gui_load::Object*, void*);  // +8C 00418D40 CSWGuiLabel::ReSetFont
    gui_load::Word (__fastcall* s90)(gui_load::Object*, void*);  // +90 00418A90 CSWGuiControl::DisplayToolTip
    void (__fastcall* s94)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +94 005B5E90 void_return
    void (__fastcall* s98)(gui_load::Object*, void*, gui_load::Word, gui_load::Word, gui_load::Word);  // +98 00418CD0 CSWGuiLabel::Initialize
    void (__fastcall* s9c)(gui_load::Object*, void*, gui_load::Word, gui_load::Word, gui_load::Word, gui_load::Word);  // +9C 004176E0 CSWGuiLabel::Initialize
    void (__fastcall* sa0)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);  // +A0 005B5E90 void_return
    void (__fastcall* sa4)(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);  // +A4 0063CFC0 void_return_16
    void (__fastcall* sa8)(gui_lb::SkillEntry*, void*, gui_load::Text*, gui_lb::Border*, gui_lb::Border*, gui_lb::I32);  // +A8 006AC930 CSWGuiInGameSkillEntry::Initialize
};
static_assert(sizeof(GuiM48SkillEntryMethods) == 172, "755ED0 slot count");
extern "C" const GuiM48SkillEntryMethods gui_m48_skill_entry_methods_00755ed0 = {
    &gui_m48_skill_entry_deleting_destruct_006accc0,  // +00
    &gui_m48_skill_entry_set_extent_006ab8e0,  // +04
    &gui_load_slot_ret8_005b5e90,  // +08
    &kih::Control::HandleMouseOver,  // +0C
    &gui_load_slot_zero8_00409eb0,  // +10
    &gui_load_slot_ret_004015a0,  // +14
    &kih::Control::HandleLMouseDown,  // +18
    &kih::Control::HandleLMouseUp,  // +1C
    &gui_load_slot_ret_004015a0,  // +20
    &gui_load_control_handle_rmouse_up_0041ac70,  // +24
    &gui_load_slot_zero_0063e7f0,  // +28
    &gui_load_slot_zero_0063e7f0,  // +2C
    &gui_load_slot_self_00641db0,  // +30
    &gui_load_slot_prop10_004064a0,  // +34
    &gui_m48_skill_entry_draw_006ab9f0,  // +38
    &kih::Control::HandleInputEvent,  // +3C
    &kih::Control::SetActive,  // +40
    &kih::Control::HitCheckMouse,  // +44
    &gui_m48_label_hilight_load_0041ba20,  // +48
    &gui_load_slot_zero_0063e7f0,  // +4C
    &gui_load_slot_self_00641db0,  // +50
    &gui_load_slot_self_00641db0,  // +54
    &gui_load_slot_zero_0063e7f0,  // +58
    &gui_load_slot_zero_0063e7f0,  // +5C
    &gui_load_slot_zero_0063e7f0,  // +60
    &gui_load_slot_zero_0063e7f0,  // +64
    &gui_load_slot_zero_0063e7f0,  // +68
    &gui_load_slot_zero_0063e7f0,  // +6C
    &gui_load_slot_zero_0063e7f0,  // +70
    &gui_load_slot_zero_0063e7f0,  // +74
    &gui_load_slot_zero_0063e7f0,  // +78
    &gui_load_control_focus_change_00418960,  // +7C
    &kih::Control::GetIsSelectable,  // +80
    &gui_load_control_rmouse_down_0041ac30,  // +84
    &gui_load_label_set_enabled_00418d00,  // +88
    &gui_load_label_reset_font_00418d40,  // +8C
    &gui_load_control_tooltip_00418a90,  // +90
    &gui_load_slot_ret8_005b5e90,  // +94
    &gui_load_label_initialize_00418cd0,  // +98
    &gui_load_label_initialize_004176e0,  // +9C
    &gui_load_slot_ret8_005b5e90,  // +A0
    &light_manager_void_return_16_0063cfc0,  // +A4
    &gui_m48_skill_entry_initialize_006ac930,  // +A8
};
