// Lane gui-load / ListBox fork: CSWGuiScrollBar (0x188, vtable 0073E6F8, 40 slots
// 0073E6F8..0073E794; +A0 is float data 3DCCCCCD), the scroll bar of CSWGuiListBox (+104).
#include "4d74549b1b-listbox.h"

using namespace gui_lb;
namespace kih = kotor_input_hover;

struct GuiLoadScrollBarMethods {
    void* (__fastcall* deleting_destroy)(ScrollBar*, void*, Word);                    // +00 00419EC0
    void (__fastcall* set_extent)(ScrollBar*, void*, const Extent*);                  // +04 004193F0
    void (__fastcall* slot08)(Object*, void*, Word, Word);                            // +08 005B5E90
    void (kih::Control::*handle_mouse_over)(int);                                     // +0C 004189A0 (lane-input)
    Word (__fastcall* slot10)(Object*, void*, Word, Word);                            // +10 00409EB0
    void (__fastcall* slot14)(Object*, void*);                                        // +14 004015A0
    void (kih::Control::*handle_lmouse_down)();                                       // +18 0041AB90 (lane-input)
    void (kih::Control::*handle_lmouse_up)();                                         // +1C 0041ABD0 (lane-input)
    void (__fastcall* slot20)(Object*, void*);                                        // +20 004015A0
    void (__fastcall* handle_rmouse_up)(Object*, void*);                              // +24 0041AC70
    Word (__fastcall* slot28)(Object*, void*);                                        // +28 0063E7F0
    Word (__fastcall* slot2c)(Object*, void*);                                        // +2C 0063E7F0
    Object* (__fastcall* slot30)(Object*, void*);                                     // +30 00641DB0
    Word (__fastcall* slot34)(Object*, void*);                                        // +34 004064A0
    void (__fastcall* draw)(ScrollBar*, void*, Word);                                 // +38 00419660 (MISSING)
    void (kih::Control::*handle_input_event)(int, int);                               // +3C 00418750 (lane-input)
    void (kih::Control::*set_active)(int);                                            // +40 004181D0 (lane-input)
    kih::Control* (kih::Control::*hit_check_mouse)(int, int);                         // +44 004187C0 (lane-input)
    void (__fastcall* load)(ScrollBar*, void*, Gff*, Struct*);                        // +48 0041BCD0
    Word (__fastcall* slots4c_68[8])(Object*, void*);                                 // +4C..+68 0063E7F0
    Object* (__fastcall* slot6c)(Object*, void*);                                     // +6C 00641DB0
    Word (__fastcall* slots70_78[3])(Object*, void*);                                 // +70..+78 0063E7F0
    void (__fastcall* focus_change)(Object*, void*, Word);                            // +7C 00418960
    int (kih::Control::*is_selectable)();                                             // +80 004189D0 (lane-input)
    void (__fastcall* rmouse_down)(Object*, void*);                                   // +84 0041AC30
    void (__fastcall* set_enabled)(Object*, void*, Word);                             // +88 004176A0
    void (__fastcall* slot8c)(Object*, void*);                                        // +8C 004015A0
    Word (__fastcall* tooltip)(Object*, void*);                                       // +90 00418A90
    void (__fastcall* slot94)(Object*, void*, Word, Word);                            // +94 005B5E90
    void (__fastcall* initialize98)(ScrollBar*, void*, const Extent*, ScrollBar*);    // +98 00419790
    void (__fastcall* initialize9c)(ScrollBar*, void*, const Extent*, const void*,
                                    menu_resources::Ref*, menu_resources::Ref*);      // +9C 00418100
};
static_assert(sizeof(void (kih::Control::*)(int)) == 4, "single-inheritance PMF is one code word");
static_assert(sizeof(GuiLoadScrollBarMethods) == 0xA0 && offsetof(GuiLoadScrollBarMethods, load) == 0x48 &&
              offsetof(GuiLoadScrollBarMethods, focus_change) == 0x7C &&
              offsetof(GuiLoadScrollBarMethods, initialize9c) == 0x9C, "scrollbar vtable");

extern "C" unsigned __int64 __cdecl unresolved_ftol2_006fae8c();   // M40 world-timer-x87-conversion.obj

namespace {
// x87 FILD a / FIDIV b / FIMUL c / __ftol2 (truncation) as at 00419573..8C and 004195E3..FC.
// The quotient stays on ST0 and is converted by the original _ftol2 port 006FAE8C
// (FISTP qword, low dword), M40 world-timer-x87-conversion.obj (D30-FINAL).
I32 ratio_scale(I32 a, I32 b, I32 c) {
    I32 converted;
    __asm {
        fild dword ptr a
        fidiv dword ptr b
        fimul dword ptr c
        call unresolved_ftol2_006fae8c   // original _ftol2 006FAE8C: ST0 -> EDX:EAX (D30-FINAL)
        mov converted, eax
    }
    return converted;
}
void set_extent_virtual(ScrollBar* self) {
    vt<ControlCallVtbl>(self)->set_extent(self, nullptr, &self->extent);
}
}

extern "C" {

// 0041AFE0 CSWGuiScrollBar::CSWGuiScrollBar: Control ctor (0041AFFE); EH state 0 (0041B008);
// vptr 73E6F8 (0041B00C); Border +6C (0041B012) state 1; Image +E0 (0041B022) state 2;
// Image +134 (0041B032); +5C/+60/+64 = 0, +68 &= ~1 (0041B037..4A).
// Unwind (FuncInfo 007616B4): 2 -> ~Image(+E0) 00415AD0, 1 -> ~Border(+6C) 004151C0, 0 -> ~Control 00419A20.
ScrollBar* __fastcall gui_load_scrollbar_construct_0041afe0(ScrollBar* self, void*) noexcept(false) {
    gui_load_control_construct_0041aa80(self, nullptr);
    try {
        set_vptr(self, &gui_load_scrollbar_methods_0073e6f8);
        gui_load_border_construct_004167f0(&self->border, nullptr);
        try {
            gui_load_image_construct_00415a30(&self->thumb, nullptr);
            try {
                gui_load_image_construct_00415a30(&self->dir, nullptr);
            } catch (...) {
                gui_load_image_destruct_00415ad0(&self->thumb, nullptr);
                throw;
            }
        } catch (...) {
            gui_load_border_destruct_004151c0(&self->border, nullptr);
            throw;
        }
    } catch (...) {
        gui_load_control_destruct_00419a20(self, nullptr);
        throw;
    }
    const Word flags = self->sb_flags;
    self->max_value = 0;
    self->cur_value = 0;
    self->visible_value = 0;
    self->sb_flags = flags & 0xFFFFFFFEu;
    return self;
}

// 00419E40 ~CSWGuiScrollBar: vptr 73E6F8 (00419E5D); ~Image(+134) (00419E71); ~Image(+E0)
// (00419E81); ~Border(+6C) (00419E8E); ~Control (00419E9D).
void __fastcall gui_load_scrollbar_destruct_00419e40(ScrollBar* self, void*) {
    set_vptr(self, &gui_load_scrollbar_methods_0073e6f8);
    gui_load_image_destruct_00415ad0(&self->dir, nullptr);
    gui_load_image_destruct_00415ad0(&self->thumb, nullptr);
    gui_load_border_destruct_004151c0(&self->border, nullptr);
    gui_load_control_destruct_00419a20(self, nullptr);
}

// 00419EC0 scalar deleting destructor (00419EC3..DB).
void* __fastcall gui_load_scrollbar_deleting_destruct_00419ec0(ScrollBar* self, void*, Word flags) {
    gui_load_scrollbar_destruct_00419e40(self, nullptr);
    if (flags & 1u) live_free(self);
    return self;
}

// 00418070 setMaxValue(int, uchar) RET 8: only if changed and >= 0 (00418074..7B): max = v,
// flags bit1 = arg2 byte (00418083..94), vslot +04 SetExtent(&extent) (0041809A).
void __fastcall gui_load_scrollbar_set_max_00418070(ScrollBar* self, void*, I32 value, Word flag) {
    if (self->max_value == value || value < 0) return;
    const Word flags = self->sb_flags;
    self->max_value = value;
    self->sb_flags = flags ^ ((((flag & 0xFFu) << 1) ^ flags) & 2u);
    set_extent_virtual(self);
}

// 004180A0 setCurValue(int) RET 4: clamp to [0, max] (004180A4..B3); cur = v; tail vslot +04.
void __fastcall gui_load_scrollbar_set_cur_004180a0(ScrollBar* self, void*, I32 value) {
    if (value < 0) value = 0;
    else if (value > self->max_value) value = self->max_value;
    self->cur_value = value;
    set_extent_virtual(self);
}

// 004180D0 setVisibleValue(int) RET 4: if changed: v<0 -> 1; v>0 -> v; v==0 -> (max ? 1 : 0)
// (004180D4..EB); tail vslot +04 always (004180F2..FB).
void __fastcall gui_load_scrollbar_set_visible_004180d0(ScrollBar* self, void*, I32 value) {
    if (self->visible_value != value) {
        if (value < 0 || (value == 0 && self->max_value != 0)) self->visible_value = 1;
        else self->visible_value = value;
    }
    set_extent_virtual(self);
}

// 004193F0 CSWGuiScrollBar::SetExtent(CSWGuiExtent const&) RET 4 (per-step cites).
void __fastcall gui_load_scrollbar_set_extent_004193f0(ScrollBar* self, void*, const Extent* extent) {
    self->extent = *extent;                                               // 004193FE..15
    frame_RenderBoundary_0044ed90();                                     // 00419418 AurMakeCurrent
    Extent image;                                                         // [esp+20]
    gui_load_image_get_extent_00415b30(&self->dir, nullptr, &image);      // 0041942A
    const I32 height = self->extent.height;                               // 0041942F
    const I32 width = self->extent.width;                                 // 00419432
    Extent border = { 0, 0, 0, 0 };                                       // [esp+40] 00419439..45
    Extent thumb = { 0, 0, 0, 0 };                                        // [esp+10] 00419449..55
    if (height > width) {                                                 // 00419459 vertical
        if (image.height + image.height > height) image.height = height / 2;              // 0041945B..6D
        if (image.width < width) image.left = (width - image.width) / 2 + self->extent.left; // 00419471..87
        else { image.left = self->extent.left; image.width = width; }                     // 0041948D..94
        vt<ImageCallVtbl>(&self->dir)->set_extent(&self->dir, nullptr, &image);           // 004194A1
        border.left = self->extent.left;                                                  // 004194B0
        border.top = self->extent.top + image.height;                                     // 004194B9
        border.height = self->extent.height - (image.height + image.height);              // 004194C2
        border.width = self->extent.width;                                                // 004194C6
    } else {
        if (image.width + image.width > width) image.width = width / 2;                   // 004194CC..DE
        I32 top;
        if (image.height < height) top = (height - image.height) / 2 + self->extent.top;  // 004194E2..F6
        else { top = self->extent.top; image.height = height; }                           // 004194FA..FD
        image.top = top;                                                                  // 00419503
        vt<ImageCallVtbl>(&self->dir)->set_extent(&self->dir, nullptr, &image);           // 0041950E
        border.left = self->extent.left + image.width;                                    // 0041951D
        border.top = self->extent.top;                                                    // 00419526
        border.width = self->extent.width - (image.width + image.width);                  // 0041952F
        border.height = self->extent.height;                                              // 00419533
    }
    vt<BorderCallVtbl>(&self->border)->set_extent(&self->border, nullptr, &border);       // 00419544
    Extent inner;                                                                         // [esp+30]
    gui_load_border_inner_extent_00415240(&self->border, nullptr, &inner);                // 0041954E
    const I32 max = self->max_value;                                                      // 00419559
    if (self->extent.height > self->extent.width) {                                       // 00419553..5C
        thumb.width = inner.width;                                                        // 00419569
        thumb.left = inner.left;                                                          // 0041956D
        if (max > 1) {                                                                    // 0041955E/71
            I32 size = ratio_scale(self->visible_value, self->visible_value + max, inner.height);
            if (size <= 4) size = 4;                                                      // 00419593..A1
            thumb.height = size;
            thumb.top = (inner.height - size) * self->cur_value / (max - 1) + inner.top;  // 004195A5..B7
        } else {
            thumb.height = inner.height;                                                  // 004195BD..C9
            thumb.top = inner.top;
        }
    } else {
        thumb.height = inner.height;                                                      // 004195D9
        thumb.top = inner.top;                                                            // 004195DD
        if (max != 0) {                                                                   // 004195CF/E1
            I32 size = ratio_scale(self->visible_value, self->visible_value + max, inner.width);
            if (size <= 4) size = 4;                                                      // 00419603..11
            thumb.width = size;
            thumb.left = (inner.width - size) * self->cur_value / max + inner.left;       // 00419615..26
        } else {
            thumb.width = inner.width;                                                    // 0041962C..38
            thumb.left = inner.left;
        }
    }
    vt<ImageCallVtbl>(&self->thumb)->set_extent(&self->thumb, nullptr, &thumb);           // 0041964D
}

// 0041BCD0 CSWGuiScrollBar::Load(gff, struct) RET 8: border(+6C) "BORDER", dir(+134) "DIR",
// thumb(+E0) "THUMB" via vslot +10 (0041BD18/55/92); MAXVALUE int 0 -> setMaxValue(v,0)
// (0041BDB7/BF); CURVALUE -> setCurValue (0041BDD3/DB); VISIBLEVALUE -> setVisibleValue
// (0041BDEF/F7); DRAWMODE byte 0 -> flags bit0 (0041BE0B..1E); CSWGuiControl::Load (0041BE21).
void __fastcall gui_load_scrollbar_load_0041bcd0(ScrollBar* self, void*, Gff* gff, Struct* s) noexcept(false) {
    {
        String label("BORDER");
        vt<BorderCallVtbl>(&self->border)->load(&self->border, nullptr, gff, s, &label);
    }
    {
        String label("DIR");
        vt<ImageCallVtbl>(&self->dir)->load(&self->dir, nullptr, gff, s, &label);
    }
    {
        String label("THUMB");
        vt<ImageCallVtbl>(&self->thumb)->load(&self->thumb, nullptr, gff, s, &label);
    }
    Word success;
    gui_load_scrollbar_set_max_00418070(self, nullptr,
        gui_load_gff_int_00411c90(gff, nullptr, s, "MAXVALUE", &success, 0), 0);
    gui_load_scrollbar_set_cur_004180a0(self, nullptr,
        gui_load_gff_int_00411c90(gff, nullptr, s, "CURVALUE", &success, 0));
    gui_load_scrollbar_set_visible_004180d0(self, nullptr,
        gui_load_gff_int_00411c90(gff, nullptr, s, "VISIBLEVALUE", &success, 0));
    const Word mode = gui_load_gff_byte_00411a60(gff, nullptr, s, "DRAWMODE", &success, 0);
    const Word flags = self->sb_flags;
    self->sb_flags = flags ^ ((mode ^ flags) & 1u);
    gui_load_control_load_0041b8e0(self, nullptr, gff, s);
}

// 00419790 CSWGuiScrollBar::Initialize(extent, CSWGuiScrollBar const& other) RET 8:
// this vslot +9C(extent, &other.border.params (+80), &other.dir.params (+15C), &other.thumb.params (+108)).
void __fastcall gui_load_scrollbar_initialize_00419790(ScrollBar* self, void*, const Extent* extent,
                                                       ScrollBar* other) noexcept(false) {
    vt<ControlCallVtbl>(self)->initialize9c(self, nullptr, extent,
        other->border.params, &other->dir.params.image, &other->thumb.params.image);
}

// 00418100 CSWGuiScrollBar::Initialize(extent, BorderParams const&, CResRef const& dir,
// CResRef const& thumb) RET 10h: extent copy (0041810D..23); Border::Initialize(&extent,
// params) (0041812F); local ImageParams(default ref,0,0,0,12h,1.0,&(1,1,1)) (00418134..6C);
// SetImage(dir,0) (0041817C) -> Image::Initialize(+134) (0041818D); SetImage(thumb,0)
// (0041819D) -> Image::Initialize(+E0) (004181AE); vslot +04 SetExtent(&extent) (004181B8).
void __fastcall gui_load_scrollbar_initialize_00418100(ScrollBar* self, void*, const Extent* extent,
    const void* border_params, menu_resources::Ref* dir, menu_resources::Ref* thumb) noexcept(false) {
    self->extent = *extent;
    gui_load_border_initialize_00415190(&self->border, nullptr, &self->extent, border_params);
    const Vector white = { 1.0f, 1.0f, 1.0f };
    menu_resources::Ref none;
    c2da_default_ref_00405ed0(&none, nullptr);
    ImageParams local;
    const float one = 1.0f;
    Word one_bits;
    std::memcpy(&one_bits, &one, 4);
    gui_load_image_params_construct_00414d30(&local, nullptr, &none, 0, 0, 0, 0x12, one_bits, &white);
    gui_load_image_params_set_image_00415900(&local, nullptr, dir, 0);
    gui_load_image_initialize_00416e90(&self->dir, nullptr, &self->extent, &local);
    gui_load_image_params_set_image_00415900(&local, nullptr, thumb, 0);
    gui_load_image_initialize_00416e90(&self->thumb, nullptr, &self->extent, &local);
    set_extent_virtual(self);
}

const GuiLoadScrollBarMethods gui_load_scrollbar_methods_0073e6f8 = {
    &gui_load_scrollbar_deleting_destruct_00419ec0,
    &gui_load_scrollbar_set_extent_004193f0,
    &gui_load_slot_ret8_005b5e90,
    &kih::Control::HandleMouseOver,
    &gui_load_slot_zero8_00409eb0,
    &gui_load_slot_ret_004015a0,
    &kih::Control::HandleLMouseDown,
    &kih::Control::HandleLMouseUp,
    &gui_load_slot_ret_004015a0,
    &gui_load_control_handle_rmouse_up_0041ac70,
    &gui_load_slot_zero_0063e7f0,
    &gui_load_slot_zero_0063e7f0,
    &gui_load_slot_self_00641db0,
    &gui_load_slot_prop10_004064a0,
    &gui_load_scrollbar_draw_00419660,
    &kih::Control::HandleInputEvent,
    &kih::Control::SetActive,
    &kih::Control::HitCheckMouse,
    &gui_load_scrollbar_load_0041bcd0,
    { &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0,
      &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0 },
    &gui_load_slot_self_00641db0,
    { &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0 },
    &gui_load_control_focus_change_00418960,
    &kih::Control::GetIsSelectable,
    &gui_load_control_rmouse_down_0041ac30,
    &gui_load_control_set_enabled_004176a0,
    &gui_load_slot_ret_004015a0,
    &gui_load_control_tooltip_00418a90,
    &gui_load_slot_ret8_005b5e90,
    &gui_load_scrollbar_initialize_00419790,
    &gui_load_scrollbar_initialize_00418100,
};

} // extern "C"
