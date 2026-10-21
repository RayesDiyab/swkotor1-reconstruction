// lane-m47-core rev2: CSWGuiToolTipPanel (0x1A4 = CSWGuiPanel 0x64 + CSWGuiLabel 0x140 at +64).
//   006277C0 CSWGuiToolTipPanel::CSWGuiToolTipPanel(CSWGuiManager*) RET 4   [callers 0040BBE5 (GUI manager ctor), 0040A396]
//   00627930 CSWGuiToolTipPanel::~CSWGuiToolTipPanel                         [EH state 0 = label, then panel base]
//   00627910 scalar deleting destructor (table slot 0)
//   00750030 method table (27 slots; identical to CSWGuiPanel 0073E010 except slot 0 = 00627910; tools/rd.py)
// FIRST FRAME: the GUI manager constructor 0040BAD0 (StartServices 005F93D7) allocates 0x1A4 and calls 006277C0 at 0040BBE5.
// Evidence: lane dis/006277c0.asm, dis/00627910.asm, dis/00627930.asm; .rdata strings 0075001C/00750010/00750000/
// 0074FFF4/0074FFE8/0074FFDC/0074FFD4 (tools/rd.py) reproduced as literals of identical bytes.
// All callees are the existing gui-load D1 providers (gui_objects.h / gui_control_load.h).
// Note (faithful): at width 1600 the original loads "tooltip16X12", re-reads the width and falls into the generic chain,
// so a second StartLoadFromLayout("tooltip6X4") follows (00627813..0062782D, then 00627832..006278B5).
#include "3a413dff74-gui_objects.h"
#include "5b18ef4ab5-mainmenu.h"
#include <cstddef>

namespace kotor_renderer_gl_cells { extern std::uint32_t width_0078d1d4; }
extern "C" void __cdecl live_free(void*);                        // 006FA390 (sbh-slice2 live allocator)

namespace gui_tooltip {
using namespace gui_load;
struct ToolTipPanel : Panel { Label label; };                     // +64 label (006277EA lea edi,[esi+64h])
static_assert(sizeof(ToolTipPanel) == 0x1A4 && offsetof(ToolTipPanel, label) == 0x64, "0040BBD7 push 1A4h");
}

extern "C" {
void* __fastcall gui_tooltip_panel_deleting_destruct_00627910(gui_load::Panel*, void*, gui_load::Word);

// 00750030: typed constant table (D21), slot types identical to gui-load's 0073E010 table.
struct GuiToolTipPanelMethods {
    void* (__fastcall* s00)(gui_load::Panel*, void*, gui_load::Word);
    void (__fastcall* s04)(gui_load::Object*, void*, const gui_load::Extent*);
    void (__fastcall* s08)(gui_load::Object*, void*, gui_load::Object*, gui_load::Word);
    void (__fastcall* s0c)(gui_load::Object*, void*, gui_load::Word);
    gui_load::Word (__fastcall* s10)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);
    void (__fastcall* s14)(gui_load::Object*, void*);
    void (__fastcall* s18)(gui_load::Object*, void*);
    void (__fastcall* s1c)(gui_load::Object*, void*);
    void (__fastcall* s20)(gui_load::Object*, void*);
    void (__fastcall* s24)(gui_load::Object*, void*);
    gui_load::Object* (__fastcall* s28)(gui_load::Object*, void*);
    gui_load::Word (__fastcall* s2c)(gui_load::Object*, void*);
    gui_load::Word (__fastcall* s30)(gui_load::Object*, void*);
    void (__fastcall* s34)(gui_load::Panel*, void*, gui_load::Word);
    void (__fastcall* s38)(gui_load::Object*, void*, gui_load::Word);
    void (__fastcall* s3c)(gui_load::Object*, void*, gui_load::I32, gui_load::I32);
    gui_load::Object* (__fastcall* s40)(gui_load::Object*, void*, gui_load::I32, gui_load::I32);
    gui_load::Object* (__fastcall* s44)(gui_load::Object*, void*);
    void (__fastcall* s48)(gui_load::Panel*, void*);
    void (__fastcall* s4c)(gui_load::Panel*, void*);
    void (__fastcall* s50)(gui_load::Object*, void*, gui_load::Word);
    void (__fastcall* s54)(gui_load::Object*, void*, gui_load::Word);
    void (__fastcall* s58)(gui_load::Object*, void*, gui_load::Word);
    void (__fastcall* s5c)(gui_load::Object*, void*, gui_load::Word);
    void (__fastcall* s60)(gui_load::Object*, void*, gui_load::Word);
    gui_load::String* (__fastcall* s64)(gui_load::Panel*, void*, gui_load::String*);
    void (__fastcall* s68)(gui_load::Panel*, void*);
};
static_assert(sizeof(GuiToolTipPanelMethods) == 108, "750030 slot count (next word 5F4E5442 = string bytes)");
extern const GuiToolTipPanelMethods gui_tooltip_panel_methods_00750030;
const GuiToolTipPanelMethods gui_tooltip_panel_methods_00750030 = {
    &gui_tooltip_panel_deleting_destruct_00627910,  // +00 00627910
    &gui_load_panel_set_extent_0040a5a0,            // +04 0040A5A0
    &gui_load_panel_set_active_control_0040a630,    // +08 0040A630
    &gui_load_slot_ret4_0060e760,                   // +0C 0060E760
    &gui_load_slot_zero8_00409eb0,                  // +10 00409EB0
    &gui_load_slot_ret_004015a0,                    // +14 004015A0
    &gui_load_slot_ret_004015a0,                    // +18
    &gui_load_slot_ret_004015a0,                    // +1C
    &gui_load_slot_ret_004015a0,                    // +20
    &gui_load_slot_ret_004015a0,                    // +24
    &gui_load_slot_self_00641db0,                   // +28 00641DB0
    &gui_load_slot_zero_0063e7f0,                   // +2C 0063E7F0
    &gui_load_slot_zero_0063e7f0,                   // +30
    &kotor_panel_draw_0040B760,                     // +34 0040B760
    &gui_load_slot_ret4_0060e760,                   // +38 0060E760
    &gui_load_panel_handle_input_00409e60,          // +3C 00409E60
    &gui_load_panel_hit_check_0040b690,             // +40 0040B690
    &gui_load_panel_active_control_0040b630,        // +44 0040B630
    &gui_load_panel_on_added_0040b870,              // +48 0040B870
    &gui_load_panel_on_removed_0040c170,            // +4C 0040C170
    &gui_load_panel_button_a_0040b640,              // +50
    &gui_load_panel_button_b_0040b650,              // +54
    &gui_load_panel_button_x_0040b660,              // +58
    &gui_load_panel_button_y_0040b670,              // +5C
    &gui_load_panel_button_black_0040b680,          // +60
    &gui_load_panel_full_screen_bg_0040a900,        // +64 0040A900
    &gui_load_panel_reset_font_0040a9c0,            // +68 0040A9C0
};

// 00627930 ~CSWGuiToolTipPanel: label (+64) then panel base (which restores its own table).
void __fastcall gui_tooltip_panel_destruct_00627930(gui_load::Panel* self, void*) {
    gui_load_label_destruct_00419d10(&static_cast<gui_tooltip::ToolTipPanel*>(self)->label, nullptr); // 00627958
    gui_load_panel_destruct_0040cf70(self, nullptr);                                               // 00627967
}
// 00627910 scalar deleting destructor RET 4.
void* __fastcall gui_tooltip_panel_deleting_destruct_00627910(gui_load::Panel* self, void*, gui_load::Word flags) {
    gui_tooltip_panel_destruct_00627930(self, nullptr);                                            // 00627913
    if ((flags & 1u) != 0u) live_free(self);                                                       // 00627918..20
    return self;
}

// 006277C0 CSWGuiToolTipPanel::CSWGuiToolTipPanel(manager) RET 4.
gui_load::Panel* __fastcall gui_tooltip_panel_construct_006277c0(gui_load::Panel* self_panel, void*,
                                                                 gui_load::Object* manager) noexcept(false) {
    using namespace gui_load;
    gui_tooltip::ToolTipPanel* const self = static_cast<gui_tooltip::ToolTipPanel*>(self_panel);
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                    // 006277E5
    try {                                                                                         // EH state 0
        self->vtbl = reinterpret_cast<const ObjectVtbl*>(&gui_tooltip_panel_methods_00750030);    // 006277F7
        gui_load_label_construct_0041acd0(&self->label, nullptr);                                 // 006277FD
        try {                                                                                     // EH state 1
            Ref layout;                                                                           // [esp+0Ch] (one slot)
            const char* name = nullptr;
            std::uint32_t width = kotor_renderer_gl_cells::width_0078d1d4;                        // 00627802
            if (width == 0x640u) {                                                                // 00627807..11
                layout.construct_00406d80("tooltip16X12");                                       // 00627813..1C
                gui_load_panel_start_layout_0040a680(self, nullptr, &layout);                     // 00627821..28
                width = kotor_renderer_gl_cells::width_0078d1d4;                                  // 0062782D (reloaded)
            }
            if (width == 0x500u) {                                                                // 00627832..37
                const std::uint32_t height = kotor_menu_window_cells::height_0078d1d8;            // 00627839
                if (height == 0x3C0u) name = "tooltip12X9";                                       // 0062783E..45
                else if (height == 0x400u) name = "tooltip12x10";                                 // 0062785A..61
            } else if (width == 0x400u) name = "tooltip10X8";                                     // 00627868..6F
            else if (width == 0x320u) name = "tooltip8X6";                                        // 00627884..8B
            else name = "tooltip6X4";                                                             // 006278A0
            if (name != nullptr) {
                layout.construct_00406d80(name);                                                  // 006278A5..A9
                gui_load_panel_start_layout_0040a680(self, nullptr, &layout);                     // 006278B3..B5
            }
            self->flags |= 1u;                                                                    // 006278BA..C9
            {
                String tag("tooltip");                                                            // 006278C0..CC (state 2)
                gui_load_panel_init_control_0040b930(self, nullptr, &self->label, &tag, 1);       // 006278D1..E0
            }                                                                                     // 006278EE ~tag
            gui_load_panel_stop_layout_0040b8f0(self, nullptr);                                   // 006278F3..F5
        } catch (...) {
            gui_load_label_destruct_00419d10(&self->label, nullptr);
            throw;
        }
    } catch (...) {
        gui_load_panel_destruct_0040cf70(self, nullptr);
        throw;
    }
    return self;
}
}
