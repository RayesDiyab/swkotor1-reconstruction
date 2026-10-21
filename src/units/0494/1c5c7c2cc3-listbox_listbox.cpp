// Lane gui-load / ListBox fork: CSWGuiListBox (0x2E0, vtable 0073E840, 42 slots)
// constructor/destructor, Load path 0041D5B0, organization, small slot bodies,
// CExoArrayList<CSWGuiExtent>, CExoArrayList<int>::indexOf, CSWGuiText::GetFontHeight.
#include "4d74549b1b-listbox.h"

using namespace gui_lb;
namespace kih = kotor_input_hover;

struct GuiLoadListBoxMethods {
    void* (__fastcall* deleting_destroy)(ListBox*, void*, Word);                      // +00 0041BF60
    void (__fastcall* set_extent)(ListBox*, void*, const Extent*);                    // +04 0041BF80
    void (__fastcall* set_active_control)(ListBox*, void*, Control*, Word);           // +08 0041C3A0
    void (kih::Navigable::*handle_mouse_over)(int);                                   // +0C 0041A980 (lane-input)
    Word (__fastcall* captured_movement)(ListBox*, void*, Word, Word);                // +10 0041B670 (MISSING)
    void (__fastcall* slot14)(Object*, void*);                                        // +14 004015A0
    void (__fastcall* lmouse_down)(ListBox*, void*);                                  // +18 0041C4A0 (MISSING)
    void (__fastcall* lmouse_up)(ListBox*, void*);                                    // +1C 0041A700 (MISSING)
    void (__fastcall* slot20)(Object*, void*);                                        // +20 004015A0
    void (__fastcall* handle_rmouse_up)(Object*, void*);                              // +24 0041AC70
    Word (__fastcall* slot28)(Object*, void*);                                        // +28 0063E7F0
    Object* (__fastcall* slot2c)(Object*, void*);                                     // +2C 00641DB0
    Object* (__fastcall* slot30)(Object*, void*);                                     // +30 00641DB0
    Word (__fastcall* slot34)(Object*, void*);                                        // +34 004064A0
    void (__fastcall* draw)(ListBox*, void*, Word);                                   // +38 0041A3E0 (MISSING)
    void (__fastcall* handle_input_event)(ListBox*, void*, I32, I32);                 // +3C 0041CE20 (MISSING)
    void (__fastcall* set_active)(ListBox*, void*, Word);                             // +40 0041C3B0
    kih::Control* (kih::Control::*hit_check_mouse)(int, int);                         // +44 004187C0 (lane-input)
    void (__fastcall* load)(ListBox*, void*, Gff*, Struct*);                          // +48 0041D5B0
    Object* (__fastcall* slot4c)(Object*, void*);                                     // +4C 00641DB0
    Word (__fastcall* slots50_6c[8])(Object*, void*);                                 // +50..+6C 0063E7F0
    Object* (__fastcall* slot70)(Object*, void*);                                     // +70 00641DB0
    Word (__fastcall* slots74_78[2])(Object*, void*);                                 // +74..+78 0063E7F0
    void (__fastcall* focus_change)(Object*, void*, Word);                            // +7C 00418960
    Word (__fastcall* is_selectable)(ListBox*, void*);                                // +80 0041A6C0
    void (__fastcall* rmouse_down)(Object*, void*);                                   // +84 0041AC30
    void (__fastcall* set_enabled)(Object*, void*, Word);                             // +88 004176A0
    void (__fastcall* reset_font)(ListBox*, void*);                                   // +8C 0041A7B0
    Word (__fastcall* tooltip)(ListBox*, void*);                                      // +90 0041A800
    void (__fastcall* slot94)(Object*, void*, Word, Word);                            // +94 005B5E90
    void (__fastcall* initialize98)(ListBox*, void*, const Extent*, ListBox*);        // +98 00419B80
    void (__fastcall* initialize9c)(ListBox*, void*, const Extent*, const void*, Word); // +9C 0041CD60
    void (__fastcall* select_left_a0)(ListBox*, void*, Extent*, Word, Word, Word, Word); // +A0 00419BD0
    void (__fastcall* select_left_a4)(ListBox*, void*, Word);                         // +A4 0041B120
};
static_assert(sizeof(GuiLoadListBoxMethods) == 0xA8 && offsetof(GuiLoadListBoxMethods, load) == 0x48 &&
              offsetof(GuiLoadListBoxMethods, focus_change) == 0x7C &&
              offsetof(GuiLoadListBoxMethods, select_left_a4) == 0xA4, "listbox vtable");

extern "C" unsigned __int64 __cdecl unresolved_ftol2_006fae8c();   // M40 world-timer-x87-conversion.obj

namespace {
Control* item_at(const ListBox* self, I32 index) {
    return *reinterpret_cast<Control* const volatile*>(self->items.data + static_cast<Word>(index) * 4u);
}
void set_item(ListBox* self, I32 index, Control* value) {
    *reinterpret_cast<Control* volatile*>(self->items.data + static_cast<Word>(index) * 4u) = value;
}
Extent* extent_at(ListBox* self, I32 index) {
    return reinterpret_cast<Extent*>(self->extents.data + static_cast<Word>(index) * 16u);
}
const ControlCallVtbl* cv(const Control* object) { return vt<ControlCallVtbl>(object); }
Word scroll_flag(const ListBox* self) { return (self->lb_flags >> 9) & 0xFFFFFF01u; }   // SHR 9 / AND FFFFFF01 (0041B286..89)

// __ftol2 equivalents for the x87 sequences in 0041A30F..37A / 0041A395..B0 / 0041B378..82.
// The value stays on ST0 and goes through the original _ftol2 port 006FAE8C (FISTP qword, low dword:
// wraps for [2^31,2^63), 0 low dword for the indefinite), M40 world-timer-x87-conversion.obj.
I32 oversized_upper(I32 item_height, const float* scale, I32 inner_height) {
    static const float bias = 0.9998999834060669f;           // dword 0073E468
    I32 converted;
    __asm {
        mov eax, scale
        fild dword ptr item_height
        fmul dword ptr [eax]
        fidiv dword ptr inner_height
        fadd dword ptr bias
        call unresolved_ftol2_006fae8c   // original _ftol2 006FAE8C: ST0 -> EDX:EAX (D30-FINAL)
        mov converted, eax
    }
    return converted;
}
I32 float_to_int(const float* value) {
    I32 converted;
    __asm {
        mov eax, value
        fld dword ptr [eax]
        call unresolved_ftol2_006fae8c   // original _ftol2 006FAE8C: ST0 -> EDX:EAX (D30-FINAL)
        mov converted, eax
    }
    return converted;
}
I32 int_div_float(I32 numerator, const float* denominator) {
    I32 converted;
    __asm {
        mov eax, denominator
        fild dword ptr numerator
        fdiv dword ptr [eax]
        call unresolved_ftol2_006fae8c   // original _ftol2 006FAE8C: ST0 -> EDX:EAX (D30-FINAL)
        mov converted, eax
    }
    return converted;
}
I32 ratio_times(I32 a, I32 b, I32 c) {
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
void int_ratio_to_float(float* target, I32 numerator, I32 denominator) {
    __asm {
        mov eax, target
        fild dword ptr numerator
        fidiv dword ptr denominator
        fstp dword ptr [eax]
    }
}
}

extern "C" {

// ------------------------------------------------------------------ small support bodies
// 0041B830 CExoArrayList<CSWGuiExtent>(int) RET 4: size/capacity/data = 0 (0041B83B..41);
// Allocate(n) if n != 0 (0041B843..48); EAX = this.
Array* __fastcall gui_load_extent_list_construct_0041b830(Array* self, void*, Word count) noexcept(false) {
    self->size = 0;
    self->capacity = 0;
    self->data = 0;
    if (count) gui_load_extent_list_allocate_0041a8f0(self, nullptr, count);
    return self;
}

// 0041A8F0 CExoArrayList<CSWGuiExtent>::Allocate(int) RET 4: old = data (0041A8FC);
// capacity = n (0041A902); operator new(n*16) (0041A905); zero n entries, each +C,+8,+4,+0
// (0041A913..2F); data = new (0041A935); copy live size entries (size re-read, 0041A937..68);
// free(old) if non-null (0041A96C..71).
void __fastcall gui_load_extent_list_allocate_0041a8f0(Array* self, void*, Word count) noexcept(false) {
    const Word old = self->data;
    self->capacity = static_cast<I32>(count);
    Word* fresh = static_cast<Word*>(c2da_directory_new_006fa7e6(count << 4));
    if (fresh) {
        I32 remaining = static_cast<I32>(count) - 1;
        if (remaining >= 0) {
            volatile Word* entry = fresh + 2;
            for (I32 k = remaining + 1; k != 0; --k) {
                entry[1] = 0; entry[0] = 0; entry[-1] = 0; entry[-2] = 0;
                entry += 4;
            }
        }
    }
    self->data = reinterpret_cast<Word>(fresh);
    for (I32 i = 0; i < self->size; ++i) {
        const volatile Word* from = reinterpret_cast<const volatile Word*>(old + static_cast<Word>(i) * 16u);
        volatile Word* to = reinterpret_cast<volatile Word*>(self->data + static_cast<Word>(i) * 16u);
        to[0] = from[0]; to[1] = from[1]; to[2] = from[2]; to[3] = from[3];
    }
    if (old) live_free(reinterpret_cast<void*>(old));
}

// 006C2A40 CExoArrayList<int>::indexOf(int) RET 4: linear scan of size entries (006C2A40..5A); -1 if absent.
I32 __fastcall gui_load_int_list_index_of_006c2a40(Array* self, void*, Word value) {
    const I32 count = self->size;
    I32 index = 0;
    if (count > 0) {
        const Word* entry = reinterpret_cast<const Word*>(self->data);
        do {
            if (*entry == value) return index;
            ++index;
            ++entry;
        } while (index < count);
    }
    return -1;
}

// 00414EE0 CSWGuiText::GetFontHeight: string object at +14 (00414EE3); 0 if none;
// h = vslot +48 (00414EEC); h < 16 -> (reloaded) vslot +48 + 1 (00414EF7..FC), else tail vslot +48.
I32 __fastcall gui_load_text_font_height_00414ee0(Text* self, void*) {
    AurObject* string;
    std::memcpy(&string, reinterpret_cast<unsigned char*>(self) + 0x14, sizeof(string));
    if (!string) return 0;
    const I32 height = string->vtbl->font_height(string, nullptr);
    AurObject* again;
    std::memcpy(&again, reinterpret_cast<unsigned char*>(self) + 0x14, sizeof(again));
    if (height < 0x10) return again->vtbl->font_height(again, nullptr) + 1;
    return again->vtbl->font_height(again, nullptr);
}

// ------------------------------------------------------------------ lifetime
// 0041BE40 CSWGuiListBox::CSWGuiListBox (cites inline). Unwind (FuncInfo 00761894):
// 3 -> ~CExoArrayList(+29C) 0056B6F0, 2 -> ~ScrollBar(+104) 00419E40, 1 -> ~Border(+90)
// 004151C0, 0 -> ~CSWGuiNavigable 00419CE0.
ListBox* __fastcall gui_load_listbox_construct_0041be40(ListBox* self, void*) noexcept(false) {
    gui_load_navigable_construct_0041b8b0(self, nullptr);                         // 0041BE5E
    try {
        set_vptr(self, &gui_load_listbox_methods_0073e840);                       // 0041BE6F
        gui_load_border_construct_004167f0(&self->border, nullptr);               // 0041BE75
        try {
            gui_load_scrollbar_construct_0041afe0(&self->scroll, nullptr);        // 0041BE85
            try {
                self->inner.height = 0;                                           // 0041BE96..A8
                self->inner.width = 0;
                self->inner.top = 0;
                self->inner.left = 0;
                exo_array_list_construct_00416770(&self->items, nullptr, 0);      // 0041BEAE
                try {
                    gui_load_extent_list_construct_0041b830(&self->extents, nullptr, 0);   // 0041BEBF
                } catch (...) {
                    exo_array_list_destruct_0056b6f0(&self->items, nullptr);
                    throw;
                }
            } catch (...) {
                gui_load_scrollbar_destruct_00419e40(&self->scroll, nullptr);
                throw;
            }
        } catch (...) {
            gui_load_border_destruct_004151c0(&self->border, nullptr);
            throw;
        }
    } catch (...) {
        gui_load_navigable_destruct_00419ce0(self, nullptr);
        throw;
    }
    std::memset(&self->color, 0, sizeof(self->color));                            // 0041BEC4..D0
    const Word flags = (self->lb_flags & 0xFFFFF8BAu) | 0x183Au;                  // 0041BED6..F2
    self->selected = -1;                                                          // 0041BEE5
    self->u2c1 = 0xFF;                                                            // 0041BEEC
    self->u6c = -1;                                                               // 0041BEF8
    self->padding = 0;                                                            // 0041BEFB
    self->proto = nullptr;                                                        // 0041BF01
    self->lb_flags = flags;                                                       // 0041BF07
    self->first = 0;                                                              // 0041BF11
    self->last = 0;                                                               // 0041BF18
    self->u88 = 0;                                                                // 0041BF1F
    self->u8c = 0;                                                                // 0041BF25
    self->u7c = 0;                                                                // 0041BF2B
    self->u2dc = nullptr;                                                         // 0041BF2E
    self->u2c2 = 1;                                                               // 0041BF34
    self->scale = 1.0f;                                                           // 0041BF3D
    return self;
}

// 00419A90 CSWGuiListBox::ClearItems (cites inline).
void __fastcall gui_load_listbox_clear_items_00419a90(ListBox* self, void*) {
    if (self->lb_flags & 2u) {                                                    // 00419A9C
        if (Control* held = self->u2dc) {                                         // 00419AA1..A9
            if (!cv(held)->slot6c(held, nullptr)) self->u2dc = nullptr;           // 00419AAD..B4
        }
        for (I32 i = 0; i < self->items.size; ++i) {                              // 00419ABA..F5
            Control* item = item_at(self, i);
            if (item) cv(item)->deleting_destroy(item, nullptr, 1);               // 00419AE1
            set_item(self, i, nullptr);                                           // 00419AE9
        }
    } else {
        for (I32 i = 0; i < self->items.size; ++i) {                              // 00419AF9..48
            if (self->lb_flags & 4u) {                                            // 00419B10
                Control* item = item_at(self, i);
                cv(item)->set_extent(item, nullptr, extent_at(self, i));          // 00419B2D
            }
            item_at(self, i)->owner = nullptr;                                    // 00419B39
        }
    }
    self->u2dc = nullptr;                                                         // 00419B4C
    self->items.size = 0;                                                         // 00419B52
    self->extents.size = 0;                                                       // 00419B58
    self->selected = -1;                                                          // 00419B5E
    self->u2c2 = 1;                                                               // 00419B67
}

// 0041B060 CSWGuiListBox::~CSWGuiListBox: vptr 73E840 (0041B07D); proto vslot +00(1),
// clear (0041B083..9B); ClearItems (0041B0A7); ~extents (0041B0B7); ~items (0041B0C7);
// ~ScrollBar (0041B0D7); ~Border (0041B0E7); vptr 73E520 + ~CSWGuiControl (0041B0F6..FC)
// == body of CSWGuiNavigable::~CSWGuiNavigable 00419CE0.
void __fastcall gui_load_listbox_destruct_0041b060(ListBox* self, void*) {
    set_vptr(self, &gui_load_listbox_methods_0073e840);
    if (Control* proto = self->proto) {
        cv(proto)->deleting_destroy(proto, nullptr, 1);
        self->proto = nullptr;
    }
    gui_load_listbox_clear_items_00419a90(self, nullptr);
    exo_array_list_destruct_0056b6f0(&self->extents, nullptr);
    exo_array_list_destruct_0056b6f0(&self->items, nullptr);
    gui_load_scrollbar_destruct_00419e40(&self->scroll, nullptr);
    gui_load_border_destruct_004151c0(&self->border, nullptr);
    gui_load_navigable_destruct_00419ce0(self, nullptr);
}

// 0041BF60 scalar deleting destructor (0041BF63..7B).
void* __fastcall gui_load_listbox_deleting_destruct_0041bf60(ListBox* self, void*, Word flags) {
    gui_load_listbox_destruct_0041b060(self, nullptr);
    if (flags & 1u) live_free(self);
    return self;
}

// ------------------------------------------------------------------ layout helpers
// 004181F0 SetScrollBarExtent(int width, int update) RET 8 (cites inline).
void __fastcall gui_load_listbox_scrollbar_extent_004181f0(ListBox* self, void*, I32 width, Word update) {
    const I32 own_width = self->extent.width;                                     // 004181F6
    if (!own_width) return;                                                       // 004181FB
    if (width > own_width) width = own_width;                                     // 00418201..05
    Extent bar;
    bar.top = self->extent.top;                                                   // 00418207..0A
    bar.height = self->extent.height;                                             // 0041820E..11
    bar.width = width;                                                            // 0041821D
    if (self->lb_flags & 0x10u) bar.left = self->extent.left;                     // 00418215/26
    else bar.left = self->extent.left - width + own_width;                        // 0041822E..34
    cv(&self->scroll)->set_extent(&self->scroll, nullptr, &bar);                  // 00418249
    if (update) cv(self)->set_extent(self, nullptr, &self->extent);               // 00418254..59
}

// 00418270 SetScrollBarOnLeft(int) RET 4: compares the full argument with flags bit4
// (0041827B..83); if different set bit4 (00418288..92) and SetScrollBarExtent(scroll width, 1).
void __fastcall gui_load_listbox_scrollbar_left_00418270(ListBox* self, void*, Word left) {
    const Word flags = self->lb_flags;
    if (((flags >> 4) & 1u) == left) return;
    self->lb_flags = (((left << 4) ^ flags) & 0x10u) ^ flags;
    gui_load_listbox_scrollbar_extent_004181f0(self, nullptr, self->scroll.extent.width, 1);
}

// 0041C190 SetPadding(uchar) RET 4: compares the CURRENT padding with inner width/2 and
// height/2 (0041C190..B4); if not larger, store new padding (0041C1BA), flags |= 8,
// OrganizeControls (0041C1C0..C7).
void __fastcall gui_load_listbox_set_padding_0041c190(ListBox* self, void*, Word padding) {
    const I32 current = self->padding;
    if (current > self->inner.width / 2) return;
    if (current > self->inner.height / 2) return;
    self->padding = static_cast<unsigned char>(padding);
    self->lb_flags |= 8u;
    gui_load_listbox_organize_0041b140(self, nullptr);
}

// 0041A2D0 OrganizeOversized (cites inline).
void __fastcall gui_load_listbox_organize_oversized_0041a2d0(ListBox* self, void*) {
    const I32 inner_height = self->inner.height;                                  // 0041A2DE
    const I32 item_height = self->item_height;                                    // 0041A2E4
    const I32 padding = self->padding;                                            // 0041A2EB
    Extent slot;
    slot.left = padding;                                                          // 0041A2F9
    slot.width = self->inner.width - (padding + padding);                         // 0041A2F5/FD
    slot.height = item_height;                                                    // 0041A305
    I32 count;
    if (inner_height != 0) {                                                      // 0041A30D
        const I32 upper = oversized_upper(item_height, &self->scale, inner_height);   // 0041A30F..23
        count = upper - float_to_int(&self->scale) + 1;                           // 0041A328..3D
    } else {
        count = 1;                                                                // 0041A344
    }
    I32 current;
    if (static_cast<I32>(self->u2c2) == count) {                                  // 0041A350..59
        slot.top = inner_height - item_height - padding;                          // 0041A35B..5F
        current = item_height;                                                    // 0041A363
    } else {
        const I32 page = static_cast<I32>(self->u2c2) - 1;                        // 0041A366
        slot.top = padding - int_div_float(page * inner_height, &self->scale);    // 0041A36B..8B
        if (self->u2c2 == 1) current = 0;                                         // 0041A383..91
        else current = ratio_times(page, count - 1, item_height - inner_height);   // 0041A395..B0
    }
    gui_load_scrollbar_set_cur_004180a0(&self->scroll, nullptr, current);         // 0041A3BC
    Control* first = item_at(self, 0);
    cv(first)->set_extent(first, nullptr, &slot);                                 // 0041A3D0
}

// 00419EE0 OrganizeUnequal (cites inline). The local CExoString (00419F0C) is only
// constructed and destroyed (00419F0C / 0041A26D; EH state 0 in FuncInfo 007615C8).
void __fastcall gui_load_listbox_organize_unequal_00419ee0(ListBox* self, void*) {
    const I32 count = self->items.size;                                          // 00419EFD
    String unused;
    if (self->lb_flags & 8u) {                                                    // 00419F19
        if (count == 1) {
            Control* only = item_at(self, 0);
            self->item_height = cv(only)->property10(only, nullptr);              // 00419F2A..37
        } else {
            self->item_height = 0;                                                // 00419F44
            for (I32 i = 0; i < count - 1; ++i) {                                 // 00419F3F..6F (last item excluded)
                Control* item = item_at(self, i);
                const I32 h = cv(item)->property10(item, nullptr);
                if (h > self->item_height) self->item_height = h;
            }
        }
        const Word flags = self->lb_flags | 0x80u;                                // 00419F71..7D
        self->first = 0;                                                          // 00419F88
        self->last = -1;                                                          // 00419F8F
        self->lb_flags = flags;                                                   // 00419F96
        if (self->item_height == 0) return;                                       // 00419F9E..A4
        self->lb_flags = flags & 0xFFFFFFF7u;                                     // 00419FA9..AC
    }
    {
        Word flags = self->lb_flags;                                              // 00419FB7
        if ((flags & 0x1000u) && self->selected != -1) {                          // 00419FBD..CC
            const I16 selected = self->selected;
            const I16 first = self->first;
            if (first != -1 && selected < first) {                                // 00419FD5..DD
                self->first = selected;
                self->last = -1;
                self->lb_flags = flags | 0x80u;                                   // 00419FED..18
            } else {
                const I16 last = self->last;                                      // 00419FF4
                if (last != -1 && selected > last) {                              // 00419FFB..03
                    self->first = -1;
                    self->last = selected;
                    self->lb_flags = flags & 0xFFFFFF7Fu;                         // 0041A013..18
                }
            }
        }
    }
    const I32 padding = self->padding;                                            // 0041A01E
    Extent slot;
    slot.width = self->inner.width - (padding + padding);                         // 0041A025..32
    slot.left = padding;                                                          // 0041A03E
    slot.top = 0;                                                                 // 0041A042
    slot.height = 0;                                                              // 0041A046
    I32 anchor;
    I32 y;
    if (self->lb_flags & 0x80u) {                                                 // 0041A036..4A
        anchor = self->first;                                                     // 0041A04C
        y = padding;                                                              // 0041A053
    } else {
        anchor = self->last;                                                      // 0041A057
        y = self->inner.height - item_at(self, anchor)->extent.height - padding;  // 0041A05E..70
    }
    slot.top = y;                                                                 // 0041A078
    slot.height = item_at(self, anchor)->extent.height;                           // 0041A07C..85
    {
        Control* item = item_at(self, anchor);
        cv(item)->set_extent(item, nullptr, &slot);                               // 0041A094
    }
    for (I32 k = anchor - 1; k >= 0; --k) {                                       // 0041A097..D1
        Control* item = item_at(self, k);
        const I32 h = item->extent.height;
        slot.height = h;
        y -= self->padding + h;
        slot.top = y;
        cv(item)->set_extent(item, nullptr, &slot);
    }
    {
        Control* item = item_at(self, anchor);                                    // 0041A0D3..F5
        y = item->extent.height + item->extent.top + self->padding;
    }
    for (I32 k = anchor + 1; k < count; ++k) {                                    // 0041A0EE..33
        Control* item = item_at(self, k);
        const I32 h = item->extent.height;
        slot.height = h;
        slot.top = y;
        y += self->padding + h;
        cv(item)->set_extent(item, nullptr, &slot);
    }
    const Word flags = self->lb_flags;                                            // 0041A135
    if (flags & 0x80u) {                                                          // 0041A13D
        I32 index = self->first + 1;                                              // 0041A13F..46
        Word all_fit = 1;                                                         // 0041A149..4E
        if (index < count) {
            for (; index < count; ++index) {                                      // 0041A163..78
                Control* item = item_at(self, index);
                if (item->extent.height + item->extent.top > self->inner.height) {   // 0041A165..70
                    self->last = static_cast<I16>(index - 1);                     // 0041A180..81
                    all_fit = 0;                                                  // 0041A188
                    break;
                }
            }
        }
        if (self->last == -1 || all_fit) self->last = static_cast<I16>(count - 1);   // 0041A18A..9B
    } else {
        for (I32 index = self->last - 1; index >= 0; --index) {                   // 0041A1A4..C6
            if (item_at(self, index)->extent.top < 0) {                           // 0041A1B9..BE
                self->first = static_cast<I16>(index + 1);                        // 0041A1CA..CB
                break;
            }
        }
        if (self->first == -1) self->first = 0;                                   // 0041A1D2..DC
        self->lb_flags = flags | 0x80u;                                           // 0041A1E5..EB
    }
    const Word span = static_cast<Word>(static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(self->last) - static_cast<std::uint16_t>(self->first))) + 1u;   // 0041A1F1..01
    self->visible_count = static_cast<I16>(span);                                 // 0041A207
    I32 maximum = count - static_cast<I32>(static_cast<I16>(span)) + 1;           // 0041A202..0E
    if (maximum <= 1) maximum = 1;                                                // 0041A211..16
    gui_load_scrollbar_set_max_00418070(&self->scroll, nullptr, maximum, scroll_flag(self));   // 0041A21B..34
    gui_load_scrollbar_set_visible_004180d0(&self->scroll, nullptr,
        static_cast<I32>(self->last) - static_cast<I32>(self->first) + 1);       // 0041A239..4D
    gui_load_scrollbar_set_cur_004180a0(&self->scroll, nullptr, self->first);     // 0041A252..5C
}

// 0041B140 CSWGuiListBox::OrganizeControls (cites inline).
void __fastcall gui_load_listbox_organize_0041b140(ListBox* self, void*) {
    const I32 count = self->items.size;                                           // 0041B147..52
    if (count == 0) {
        if (!(self->lb_flags & 8u)) return;                                       // 0041B158
        gui_load_scrollbar_set_max_00418070(&self->scroll, nullptr, 1, 0);        // 0041B165..70
        gui_load_scrollbar_set_cur_004180a0(&self->scroll, nullptr, 1);           // 0041B179
        gui_load_scrollbar_set_visible_004180d0(&self->scroll, nullptr, 1);       // 0041B182
        self->lb_flags &= 0xFFFFFFF7u;                                            // 0041B187..91
        return;
    }
    if (self->lb_flags & 0x100u) {                                                // 0041B19D..A6
        gui_load_listbox_organize_unequal_00419ee0(self, nullptr);
        return;
    }
    I32 step = self->padding + self->item_height;                                 // 0041B1B6..C8
    if (self->lb_flags & 8u) {                                                    // 0041B1C6
        self->item_height = 0;                                                    // 0041B1D6
        for (I32 i = 0; i < count; ++i) {                                         // 0041B1E0..1D
            Control* item = item_at(self, i);
            cv(item)->set_extent(item, nullptr, extent_at(self, i));              // 0041B1F4
            Control* again = item_at(self, i);
            const I32 h = cv(again)->property10(again, nullptr);                  // 0041B202
            if (h > self->item_height) self->item_height = h;                     // 0041B205..0D
        }
        self->first = 0;                                                          // 0041B227
        if (self->item_height == 0) {                                             // 0041B21F..30
            gui_load_scrollbar_set_max_00418070(&self->scroll, nullptr, 1, 0);    // 0041B232..3D
            gui_load_scrollbar_set_cur_004180a0(&self->scroll, nullptr, 1);       // 0041B246
            gui_load_scrollbar_set_visible_004180d0(&self->scroll, nullptr, 1);   // 0041B24F
            return;
        }
        const I32 inner_height = self->inner.height;                              // 0041B25C
        step = self->padding + self->item_height;                                 // 0041B262..70
        const I32 visible = inner_height / step;                                  // 0041B26D
        self->visible_count = static_cast<I16>(visible);                          // 0041B277
        I32 visible_arg;
        if (static_cast<I16>(visible) == 0) {                                     // 0041B274..7E
            gui_load_scrollbar_set_max_00418070(&self->scroll, nullptr,
                self->item_height - inner_height + 1, scroll_flag(self));         // 0041B280..9B
            visible_arg = self->inner.height;                                     // 0041B2A0
        } else if ((self->lb_flags >> 9) & 1u) {                                  // 0041B2A9..B5
            I32 maximum = count - static_cast<I32>(static_cast<I16>(visible)) + 1; // 0041B2B7..C1
            if (maximum <= 1) maximum = 1;
            gui_load_scrollbar_set_max_00418070(&self->scroll, nullptr, maximum, scroll_flag(self));   // 0041B2DB
            visible_arg = 1;                                                      // 0041B2E0
        } else {
            I32 maximum = count - static_cast<I32>(static_cast<I16>(visible)) + 1; // 0041B2E4..F2
            if (maximum <= 1) maximum = 1;
            gui_load_scrollbar_set_max_00418070(&self->scroll, nullptr, maximum, scroll_flag(self));   // 0041B30C
            visible_arg = self->visible_count;                                    // 0041B311
            if (visible_arg >= count) visible_arg = count;                        // 0041B318..1C
        }
        gui_load_scrollbar_set_visible_004180d0(&self->scroll, nullptr, visible_arg);   // 0041B321
        if (self->padding + self->item_height > self->inner.height && self->items.size > 0 &&
            !(self->lb_flags & 0x100u)) {                                         // 0041B326..52
            Control* item = item_at(self, 0);
            gui_load::Label* label = cv(item)->as_label(item, nullptr);           // 0041B35E
            if (label) {
                const I32 font = gui_load_text_font_height_00414ee0(&label->text, nullptr);   // 0041B36B
                if (font) int_ratio_to_float(&self->scale, self->inner.height, font);       // 0041B378..82
            }
        }
        self->lb_flags &= 0xFFFFFFF7u;                                            // 0041B388..95
    }
    if (self->padding + self->item_height > self->inner.height && self->items.size > 0 &&
        !(self->lb_flags & 0x100u)) {                                             // 0041B39B..C7
        gui_load_listbox_organize_oversized_0041a2d0(self, nullptr);
        return;
    }
    if ((self->lb_flags & 0x1000u) && self->selected != -1) {                     // 0041B3D8..EE
        const I16 selected = self->selected;
        const I16 first = self->first;
        if (selected < first) {
            self->first = selected;                                               // 0041B3F7 -> 0041B42B
        } else if (static_cast<I32>(selected) >= static_cast<I32>(self->visible_count) + first) {   // 0041B3FC..0D
            I32 top = static_cast<I32>(selected) - static_cast<I32>(self->visible_count) + 1;      // 0041B414..20
            if (top < 0) top = 0;                                                 // 0041B421..29
            self->first = static_cast<I16>(top);
        }
    }
    gui_load_scrollbar_set_cur_004180a0(&self->scroll, nullptr, self->first);     // 0041B432..40
    const I32 visible = self->visible_count;                                      // 0041B445
    const I32 padding = self->padding;                                            // 0041B44C
    I32 remainder = self->inner.height - visible * step - padding;                // 0041B453..60
    const I32 extra = remainder / visible;                                        // 0041B462..65 (IDIV; visible==0 faults as in original)
    Extent slot;
    slot.left = padding;                                                          // 0041B46D
    slot.top = 0;                                                                 // 0041B471
    slot.height = self->item_height;                                              // 0041B479
    remainder -= visible * extra;                                                 // 0041B47D..80
    slot.width = self->inner.width - (padding + padding);                         // 0041B482..98
    I32 y = padding - self->first * step;                                         // 0041B491..A1
    for (I32 i = 0; i < self->first; ++i) {                                       // 0041B4A3..D7
        slot.top = y;
        Control* item = item_at(self, i);
        cv(item)->set_extent(item, nullptr, &slot);
        y += step;
    }
    I32 end = self->first + self->visible_count;                                  // 0041B4D9..EF
    if (end >= count) end = count;                                                // 0041B4F3..F5
    const I32 grown = self->item_height + extra;                                  // 0041B4F9..07
    for (I32 i = self->first; i < end; ++i) {                                     // 0041B509..62
        slot.height = grown;
        slot.top = y;
        if (remainder) {                                                          // 0041B520..34
            slot.height = grown + 1;
            ++y;
            --remainder;
        }
        Control* item = item_at(self, i);
        cv(item)->set_extent(item, nullptr, &slot);                               // 0041B545
        y += self->padding + grown;                                               // 0041B548..55
    }
    slot.height = self->item_height;                                              // 0041B576..80
    for (I32 i = self->first + self->visible_count; i < count; ++i) {             // 0041B564..B0
        slot.top = y;
        Control* item = item_at(self, i);
        cv(item)->set_extent(item, nullptr, &slot);
        y += step;
    }
}

// ------------------------------------------------------------------ Load path
// 0041D3E0 CSWGuiListBox::LoadProtoItem(gff, struct) RET 8 (cites inline). New-expression
// unwind states 0..4 free the allocation (FuncInfo 00761A8C -> 006FA390).
void __fastcall gui_load_listbox_load_proto_0041d3e0(ListBox* self, void*, Gff* gff, Struct* s) noexcept(false) {
    Struct proto;
    if (!gui_load_gff_struct_00411a10(gff, nullptr, &proto, s, "PROTOITEM")) return;   // 0041D412
    Word success;
    const Word type_value = static_cast<Word>(gui_load_gff_int_00411c90(gff, nullptr, &proto, "CONTROLTYPE", &success, 0));   // 0041D433
    const Word flags = self->lb_flags ^ (((type_value << 13) ^ self->lb_flags) & 0x1E000u);   // 0041D43E..4A
    self->lb_flags = flags;                                                       // 0041D45D
    const Word kind = ((flags >> 13) & 0xFu) - 4u;                                // 0041D44C..54
    if (kind <= 4u) {                                                             // 0041D45A..63 (jump table 0041D598)
        static const Word sizes[5] = { 0x140u, 0x1B4u, 0x1C4u, 0x2B4u, 0x1B8u };  // 0041D470/9D/C3/E9/50F
        void* allocation = c2da_directory_new_006fa7e6(sizes[kind]);
        Control* made = nullptr;
        if (allocation) {
            try {
                switch (kind) {
                    case 0: made = gui_load_label_construct_0041acd0(static_cast<gui_load::Label*>(allocation), nullptr); break;
                    case 1: made = gui_load_label_hilight_construct_0041b5c0(allocation, nullptr); break;
                    case 2: made = gui_load_button_construct_0041c0f0(static_cast<gui_load::Button*>(allocation), nullptr); break;
                    case 3: made = gui_load_button_toggle_construct_0041ca30(allocation, nullptr); break;
                    default: made = gui_load_slider_construct_0041baa0(allocation, nullptr); break;
                }
            } catch (...) {
                live_free(allocation);
                throw;
            }
        }
        self->proto = made;                                                       // 0041D53B (previous proto not released)
    }
    if (Control* item = self->proto) {                                            // 0041D541..49
        String label("PROTOITEM");
        gui_load_control_load_label_00418800(item, nullptr, self, gff, s, &label);   // 0041D56F (listbox struct, not PROTOITEM)
    }
}

// 0041D5B0 CSWGuiListBox::Load(gff, struct) RET 8: border(+90) "BORDER" vslot +10
// (0041D5FE); scroll(+104) CSWGuiControl::Load(this, gff, s, "SCROLLBAR") (0041D636);
// LEFTSCROLLBAR byte default 1 -> SetScrollBarOnLeft (0041D65B/66); PADDING int 0 ->
// SetPadding (0041D67A/82); COLOR default (0,0,0) -> +2D0 (0041D6B6..CE); LOOPING byte 0 ->
// flags bit6 (0041D6E0..F9); CSWGuiNavigable::Load (0041D702); LoadProtoItem (0041D70B).
void __fastcall gui_load_listbox_load_0041d5b0(ListBox* self, void*, Gff* gff, Struct* s) noexcept(false) {
    {
        String label("BORDER");
        vt<BorderCallVtbl>(&self->border)->load(&self->border, nullptr, gff, s, &label);
    }
    {
        String label("SCROLLBAR");
        gui_load_control_load_label_00418800(&self->scroll, nullptr, self, gff, s, &label);
    }
    Word success;
    gui_load_listbox_scrollbar_left_00418270(self, nullptr,
        gui_load_gff_byte_00411a60(gff, nullptr, s, "LEFTSCROLLBAR", &success, 1));
    gui_load_listbox_set_padding_0041c190(self, nullptr,
        static_cast<Word>(gui_load_gff_int_00411c90(gff, nullptr, s, "PADDING", &success, 0)));
    const Vector black = { 0.0f, 0.0f, 0.0f };
    Vector color;
    const Vector* read = gui_load_gff_vector_004122a0(gff, nullptr, &color, s, "COLOR", &success, &black);
    std::memcpy(&self->color, read, sizeof(self->color));
    const Word looping = gui_load_gff_byte_00411a60(gff, nullptr, s, "LOOPING", &success, 0);
    const Word flags = self->lb_flags;
    self->lb_flags = flags ^ (((looping << 6) ^ flags) & 0x40u);
    gui_load_navigable_load_0041c890(self, nullptr, gff, s);
    gui_load_listbox_load_proto_0041d3e0(self, nullptr, gff, s);
}

// ------------------------------------------------------------------ slot bodies
// 0041BF80 CSWGuiListBox::SetExtent(CSWGuiExtent const&) RET 4 (cites inline).
void __fastcall gui_load_listbox_set_extent_0041bf80(ListBox* self, void*, const Extent* extent) {
    const I32 bar = self->scroll.extent.width;                                    // 0041BF8B
    self->extent = *extent;                                                       // 0041BF94..AA
    Extent area = { extent->left, extent->top, extent->width - bar, extent->height };   // 0041BFAD..E3
    if (self->lb_flags & 0x10u) area.left += bar;                                 // 0041BFC0..CD
    vt<BorderCallVtbl>(&self->border)->set_extent(&self->border, nullptr, &area); // 0041BFEA
    Extent inner;
    const Extent* got = gui_load_border_inner_extent_00415240(&self->border, nullptr, &inner);   // 0041BFF4
    self->inner = *got;                                                           // 0041BFF9..17
    gui_load_listbox_scrollbar_extent_004181f0(self, nullptr, bar, 0);            // 0041C01A
    self->lb_flags |= 8u;                                                         // 0041C01F..2A
    gui_load_listbox_organize_0041b140(self, nullptr);                            // 0041C030
}

// 0041C040 SetSelectedControl(int index, int sound) RET 8 (cites inline).
void __fastcall gui_load_listbox_set_selected_0041c040(ListBox* self, void*, I32 index, Word sound) {
    if (self->selected != -1) {                                                   // 0041C043..4E
        Control* old = item_at(self, self->selected);
        cv(old)->handle_input_event(old, nullptr, 1, 1);                          // 0041C05E..62
    }
    if (index >= 0 && index < self->items.size) {                                 // 0041C065..73
        self->selected = static_cast<I16>(index);                                 // 0041C07B
        Control* item = item_at(self, self->selected);
        cv(item)->handle_input_event(item, nullptr, 0, 1);                        // 0041C08A..8E
        if (sound) {                                                              // 0041C095
            Control* chosen = item_at(self, self->selected);
            void* manager;
            std::memcpy(&manager, reinterpret_cast<unsigned char*>(self->owner) + 0x18, sizeof(manager));   // 0041C0A9..AC
            static_cast<kih::Manager*>(manager)->PlayGuiSound(static_cast<char>(chosen->b55));             // 0041C0AF..B5
        }
    } else {
        self->selected = -1;                                                      // 0041C0BC
    }
    self->lb_flags |= 0x1000u;                                                    // 0041C0C5
    gui_load_listbox_organize_0041b140(self, nullptr);                            // 0041C0D1
    gui_load_scrollbar_set_cur_004180a0(&self->scroll, nullptr, self->first);     // 0041C0D6..E4
}

// 0041C160 SetActiveControl(CSWGuiControl*, int) RET 8: indexOf (0041C173, RET 4 leaves the
// second pushed word as SetSelectedControl's second argument) -> SetSelectedControl (0041C17B).
void __fastcall gui_load_listbox_set_active_control_0041c160(ListBox* self, void*, Control* control, Word sound) {
    const I32 index = gui_load_int_list_index_of_006c2a40(&self->items, nullptr,
        static_cast<Word>(reinterpret_cast<std::uintptr_t>(control)));
    gui_load_listbox_set_selected_0041c040(self, nullptr, index, sound);
}

// 0041C3A0: JMP 0041C160 (slot +08 thunk).
void __fastcall gui_load_listbox_set_active_control_0041c3a0(ListBox* self, void*, Control* control, Word sound) {
    gui_load_listbox_set_active_control_0041c160(self, nullptr, control, sound);
}

// 0041A5E0 HitCheckMouseLocal(int* out_index) RET 4 (cites inline).
Control* __fastcall gui_load_listbox_hit_local_0041a5e0(ListBox* self, void*, I32* out_index) {
    I32 x, y;
    gui_load_panel_local_mouse_0040ba20(self->owner, nullptr, &x, &y);           // 0041A5E9..F6
    const Extent bar = self->scroll.extent;                                       // 0041A5FF..0F
    if (x >= bar.left && x <= bar.left + bar.width && y >= bar.top && y <= bar.top + bar.height) {   // 0041A616..28
        if (out_index) *out_index = -1;                                           // 0041A62A..32
        return &self->scroll;                                                     // 0041A639
    }
    const I32 local_x = x - self->inner.left;                                     // 0041A648..56
    const I32 local_y = y - self->inner.top;                                      // 0041A65E
    self->local_x = local_x;                                                      // 0041A662
    self->local_y = local_y;                                                      // 0041A668
    for (I32 i = 0; i < self->items.size; ++i) {                                  // 0041A66E..9D
        Control* item = item_at(self, i);
        Control* hit = cv(item)->hit_check_mouse(item, nullptr, local_x, local_y);   // 0041A68B..8D
        if (hit) {
            if (out_index) *out_index = i;                                        // 0041A6AB..B3
            return hit;
        }
    }
    return nullptr;
}

// 0041A6C0 GetIsSelectable: 0 unless control flags bit3 (0041A6C6); p = vslot +2C (0041A6D0);
// c = HitCheckMouseLocal on p (0041A6DB); 1 iff c and c vslot +80 (0041A6E8).
Word __fastcall gui_load_listbox_is_selectable_0041a6c0(ListBox* self, void*) {
    if (!(self->flags & 8u)) return 0;
    Control* receiver = cv(self)->as_control(self, nullptr);
    if (!receiver) return 0;
    Control* hit = gui_load_listbox_hit_local_0041a5e0(static_cast<ListBox*>(receiver), nullptr, nullptr);
    if (!hit) return 0;
    return cv(hit)->is_selectable(hit, nullptr) ? 1u : 0u;
}

// 0041A7B0 ReSetFont: each item vslot +8C (size re-read, 0041A7C0..DA); proto tail vslot +8C (0041A7DC..EA).
void __fastcall gui_load_listbox_reset_font_0041a7b0(ListBox* self, void*) {
    for (I32 i = 0; i < self->items.size; ++i) {
        Control* item = item_at(self, i);
        cv(item)->reset_font(item, nullptr);
    }
    if (Control* proto = self->proto) cv(proto)->reset_font(proto, nullptr);
}

// 0041A800 DisplayToolTip: hit = HitCheckMouseLocal(0) (0041A802); hit ? tail vslot +90 : 0.
Word __fastcall gui_load_listbox_tooltip_0041a800(ListBox* self, void*) {
    Control* hit = gui_load_listbox_hit_local_0041a5e0(self, nullptr, nullptr);
    if (!hit) return 0;
    return cv(hit)->display_tooltip(hit, nullptr);
}

// 0041C3B0 SetActive(int) RET 4: control flags bit0 = arg bit0 (0041C3B0..C3); if now set:
// (selected == -1 && !(flags & 200h)) -> SetSelectedControl(0,0) (0041C3C8..E1), else OrganizeControls (0041C3E9).
void __fastcall gui_load_listbox_set_active_0041c3b0(ListBox* self, void*, Word active) {
    const Word flags = self->flags ^ ((active ^ self->flags) & 1u);
    self->flags = flags;
    if (!(flags & 1u)) return;
    if (self->selected == -1 && !(self->lb_flags & 0x200u)) {
        gui_load_listbox_set_selected_0041c040(self, nullptr, 0, 0);
        return;
    }
    gui_load_listbox_organize_0041b140(self, nullptr);
}

// 00419B80 Initialize(extent, CSWGuiListBox const& other) RET 8: this vslot +9C(extent,
// &other.border.params (+A4), other.padding) (00419B88..A0); then tail scroll(+104) vslot +98
// (&other.scroll.extent, &other.scroll) (00419BA6..C5).
void __fastcall gui_load_listbox_initialize_00419b80(ListBox* self, void*, const Extent* extent, ListBox* other) noexcept(false) {
    vt<ListBoxCallVtbl>(self)->initialize9c(self, nullptr, extent, other->border.params, other->padding);
    vt<ScrollBarCallVtbl>(&self->scroll)->initialize98(&self->scroll, nullptr, &other->scroll.extent, &other->scroll);
}

// 0041CD60 Initialize(extent, BorderParams const&, uchar padding) RET 0Ch: extent copy
// (0041CD6D..83); area = extent minus scroll width, shifted when bar on left (0041CD86..CD);
// Border::Initialize(&area, params) (0041CDDF); inner = GetInnerExtent (0041CDEB..09); SetPadding (0041CE13).
void __fastcall gui_load_listbox_initialize_0041cd60(ListBox* self, void*, const Extent* extent,
                                                     const void* border_params, Word padding) noexcept(false) {
    self->extent = *extent;
    const I32 bar = self->scroll.extent.width;
    Extent area = { extent->left, extent->top, extent->width - bar, extent->height };
    if (self->lb_flags & 0x10u) area.left += bar;
    gui_load_border_initialize_00415190(&self->border, nullptr, &area, border_params);
    Extent inner;
    const Extent* got = gui_load_border_inner_extent_00415240(&self->border, nullptr, &inner);
    self->inner = *got;
    gui_load_listbox_set_padding_0041c190(self, nullptr, padding);
}

// 00419BD0 SetSelectOnLeftClick(CSWGuiExtent&, a2, a3, a4, int left) RET 14h (cites inline).
void __fastcall gui_load_listbox_select_left_00419bd0(ListBox* self, void*, Extent* bar, Word a2, Word a3,
                                                      Word a4, Word left) {
    self->lb_flags = self->lb_flags ^ (((left << 4) ^ self->lb_flags) & 0x10u);   // 00419BD0..F3
    if (bar->width > self->extent.width) bar->width = self->extent.width;          // 00419BF9..FE
    bar->top = self->extent.top;                                                   // 00419C01..04
    bar->height = self->extent.height;                                             // 00419C07..0A
    if (self->lb_flags & 0x10u) bar->left = self->extent.left;                     // 00419C0D..1B
    else bar->left = self->extent.left - bar->width + self->extent.width;          // 00419C1F..2B
    vt<ScrollBarCallVtbl>(&self->scroll)->initialize9c(&self->scroll, nullptr, bar,
        reinterpret_cast<const void*>(static_cast<std::uintptr_t>(a2)),
        reinterpret_cast<menu_resources::Ref*>(static_cast<std::uintptr_t>(a3)),
        reinterpret_cast<menu_resources::Ref*>(static_cast<std::uintptr_t>(a4)));  // 00419C2D..4B
    const I32 visible = self->visible_count < 1 ? 1 : static_cast<I32>(self->visible_count);   // 00419C51..63
    I32 maximum = self->items.size - visible + 1;                                  // 00419C66..70
    if (maximum <= 1) maximum = 1;                                                 // 00419C71..8E
    gui_load_scrollbar_set_max_00418070(&self->scroll, nullptr, maximum, scroll_flag(self));   // 00419C93..A6
    cv(self)->set_extent(self, nullptr, &self->extent);                            // 00419CAB..B0
}

// 0041B120 SetSelectOnLeftClick(int) RET 4: flags bit0 = arg bit0 (0041B120..35).
void __fastcall gui_load_listbox_select_left_0041b120(ListBox* self, void*, Word value) {
    self->lb_flags = self->lb_flags ^ ((self->lb_flags ^ value) & 1u);
}

// 0073E840 method table (42 slots, 0073E840..0073E8E4).
const GuiLoadListBoxMethods gui_load_listbox_methods_0073e840 = {
    &gui_load_listbox_deleting_destruct_0041bf60,
    &gui_load_listbox_set_extent_0041bf80,
    &gui_load_listbox_set_active_control_0041c3a0,
    &kih::Navigable::HandleMouseOver,
    &gui_load_listbox_captured_movement_0041b670,
    &gui_load_slot_ret_004015a0,
    &gui_load_listbox_lmouse_down_0041c4a0,
    &gui_load_listbox_lmouse_up_0041a700,
    &gui_load_slot_ret_004015a0,
    &gui_load_control_handle_rmouse_up_0041ac70,
    &gui_load_slot_zero_0063e7f0,
    &gui_load_slot_self_00641db0,
    &gui_load_slot_self_00641db0,
    &gui_load_slot_prop10_004064a0,
    &gui_load_listbox_draw_0041a3e0,
    &gui_load_listbox_handle_input_0041ce20,
    &gui_load_listbox_set_active_0041c3b0,
    &kih::Control::HitCheckMouse,
    &gui_load_listbox_load_0041d5b0,
    &gui_load_slot_self_00641db0,
    { &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0,
      &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0 },
    &gui_load_slot_self_00641db0,
    { &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0 },
    &gui_load_control_focus_change_00418960,
    &gui_load_listbox_is_selectable_0041a6c0,
    &gui_load_control_rmouse_down_0041ac30,
    &gui_load_control_set_enabled_004176a0,
    &gui_load_listbox_reset_font_0041a7b0,
    &gui_load_listbox_tooltip_0041a800,
    &gui_load_slot_ret8_005b5e90,
    &gui_load_listbox_initialize_00419b80,
    &gui_load_listbox_initialize_0041cd60,
    &gui_load_listbox_select_left_00419bd0,
    &gui_load_listbox_select_left_0041b120,
};

} // extern "C"
