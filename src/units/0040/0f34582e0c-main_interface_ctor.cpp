// Lane W4 G15: CSWGuiMainInterface (in-game HUD) construction: 0068C100 ctor, 0068B170/0068D3B0 dtors and the child
// classes it builds (CSWGuiTargetActionMenu, CSWGuiMainInterfaceAction/Char/Status, CSWGuiCharButton) with the
// Initialize / layout helpers the constructor executes. Transcribed from dis/ann_0068C100.asm,
// dis/ann_mi_children1.asm, dis/ann_mi_init.asm and the cited listings.
#include "a950a9dfd8-gui_panels.hpp"
using namespace w4gui;

namespace kotor_renderer_gl_cells { extern unsigned width_0078d1d4; }
namespace kotor_menu_window_cells { extern volatile unsigned height_0078d1d8; }
namespace kotor_menu_10_19 {
struct String;
struct Tlk { String* __thiscall simple_0041e8f0(String* out, unsigned strref); };       // CTlkTable::GetSimpleString
}
namespace kotor_original {
void* __fastcall kotor_fastcall_0041aed0(void*, void*);                                // CSWGuiProgressBar::CSWGuiProgressBar
void __fastcall kotor_fastcall_0041af70(void*, void*);                                 // CSWGuiProgressBar::~CSWGuiProgressBar
}
void* __fastcall exo_array_list_construct_005a76d0(void*, void*);                       // CExoArrayList<>::CExoArrayList (global ns)
extern "C" {
void* __fastcall gui_load_label_hilight_construct_0041b5c0(void*, void*) noexcept(false);  // CSWGuiLabelHilight
void __fastcall gui_m48_label_hilight_destruct_0041b610(void*, void*);
void* __fastcall gui_load_button_toggle_construct_0041ca30(void*, void*) noexcept(false); // CSWGuiButtonToggle
void __fastcall gui_m48_button_toggle_destruct_0041cad0(void*, void*);
void __fastcall gui_load_control_add_child_0041acc0(void*, void*, void* child);         // CSWGuiControl::AddChildControl
void __fastcall gui_load_border_params_set_pulsing_00414d00(void*, void*, U32, U32, U32); // CSWGuiBorderParams::SetPulsingAlpha
void* __cdecl kotor_cdecl_0045a260(const char* name);                                    // NewCAurGUIImage
void* __fastcall kotor_fastcall_005edeb0(void* app, void*, void* out, U32 id);           // CClientExoApp::GetGUIString
void __fastcall kotor_fastcall_0059ea30(void* self, void*);                              // element dtor (jmp ~CExoString)
void __fastcall gui_load_slot_ret4_0060e760(void*, void*, U32);
void __fastcall gui_load_panel_reset_font_0040a9c0(void*, void*);                        // CSWGuiPanel::ResetFont
}
namespace w4gui { const void* vt_main_interface(); const void* vt_char_button(); }
namespace w4 { void* main_interface_00833bb4 = nullptr; }   // [00833BB4] W4-owned: written by the ctor, read by FloatyText

namespace {
void init_formatted(void* panel, void* control, void* scratch, std::int32_t flag) {
    gui_load_panel_init_control_0040b930(panel, nullptr, control, scratch, flag);
}
}

extern "C" {
// ---------------------------------------------------------------- event handlers referenced by address only
// (original slot/PMF targets that run on input; faithful bodies: srcw5/actions/claude_stale_bodies_client.cpp).
void __fastcall kotor_fastcall_006883c0(void*, void*, void*);   // CSWGuiMainInterface::OnMenuButtonPressed: body in claude_stale_bodies_client.cpp (stale-stubs G5)
void __fastcall kotor_fastcall_0068b050(void*, void*, void*);   // CSWGuiMainInterface::OnClearOneButtonPressed: body in claude_stale_bodies_client.cpp (stale-stubs G5)
void __fastcall kotor_fastcall_0068b0a0(void*, void*, void*);   // CSWGuiMainInterface::OnClearAllButtonPressed: body in claude_stale_bodies_client.cpp (stale-stubs G5)
void __fastcall kotor_fastcall_00688590(void*, void*, void*);   // CSWGuiMainInterface::OnPauseButtonPressed: body in claude_stale_bodies_client.cpp (stale-stubs G5)
void __fastcall kotor_fastcall_00688610(void*, void*, void*);   // CSWGuiMainInterface::OnSoloButtonPressed: body in claude_stale_bodies_client.cpp (stale-stubs G5)
void __fastcall kotor_fastcall_00688640(void*, void*, void*);   // CSWGuiMainInterface::OnStealthButtonPressed: body in claude_stale_bodies_client.cpp (stale-stubs G5)
void __fastcall kotor_fastcall_00688690(void*, void*, void*);   // CSWGuiMainInterface::OnCharacterClicked: body in claude_stale_bodies_client.cpp (stale-stubs G5)
void __fastcall kotor_fastcall_0068b970(void*, void*, void*);
void __fastcall kotor_fastcall_006884b0(void*, void*, void*);   // CSWGuiMainInterface::OnTargetUpArrowPressed: body in claude_stale_bodies_client.cpp (stale-stubs G5)
void __fastcall kotor_fastcall_00688520(void*, void*, void*);   // CSWGuiMainInterface::OnTargetDownArrowPressed: body in claude_stale_bodies_client.cpp (stale-stubs G5)
void __fastcall kotor_fastcall_00688490(void*, void*, void*);
void __fastcall kotor_fastcall_0068af70(void*, void*, void*);   // CSWGuiMainInterface::OnActionUpArrowPressed: body in claude_stale_bodies_client.cpp (stale-stubs G5)
void __fastcall kotor_fastcall_0068afe0(void*, void*, void*);   // CSWGuiMainInterface::OnActionDownArrowPressed: body in claude_stale_bodies_client.cpp (stale-stubs G5)
void __fastcall kotor_fastcall_006856c0(void*, void*, void*);

// ---------------------------------------------------------------- CExoArrayList<CSWGuiInterfaceAction> dtor 005FBFB0
void __fastcall kotor_fastcall_005fbfb0(void* list, void*) {
    if (void* data = lp(list, 0)) {                                                     // 005FBFB3..005FBFB7
        const U32 count = lw(static_cast<U8*>(data) - 4, 0);                               // 005FBFB9 [eax-4]
        vector_destruct(data, 0x38, count, &kotor_fastcall_0059ea30);                   // 005FBFC0..005FBFC9
        live_free(static_cast<U8*>(data) - 4);                                          // 005FBFCE..005FBFCF
    }
    zero(list, 0);                                                                      // 005FBFD8
}

// ---------------------------------------------------------------- CSWGuiMainInterfaceAction (0x71C)
void* __fastcall kotor_fastcall_00689200(void* self, void*) {                           // 00689200
    Unwind u;
    button(self, 0, u); button(self, 0x1C4, u); button(self, 0x388, u); button(self, 0x54C, u);   // 0068921D..00689250
    u.dismiss();
    sb(self, 0x714, 0); zero(self, 0x718);                                              // 00689259..00689260
    return self;
}
void __fastcall kotor_fastcall_006893a0(void* self, void*) {                            // 006893A0
    for (U32 off : {0x54Cu, 0x388u, 0x1C4u, 0u}) gui_load_button_destruct_00419da0(at(self, off), nullptr);
}
// 0068B9D0 CSWGuiMainInterfaceAction::Initialize(CSWGuiPanel* panel, int index, int target) (thiscall, RET 0Ch).
void __fastcall kotor_fastcall_0068b9d0(void* self, void*, void* panel, I32 index, U32 target) {
    U32 name[2];                                                                        // [esp+20h]
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(name), nullptr);  // 0068B9F1..0068B9F5
    Unwind u; u.push(&frame_LogStringDtor_005e5c20, name);
    unsigned char scratch_label[0x140];                                                 // [esp+30h] (constructed, unused)
    gui_load_label_construct_0041acd0(scratch_label, nullptr);                          // 0068B9FA..0068BA09
    u.push(&gui_load_label_destruct_00419d10, scratch_label);
    sw(self, 0x718, target);                                                            // 0068BA1F
    {
    ExoString kind(target != 0 ? "TARGET" : "ACTION");                                  // 0068BA25..0068BA36 (00753FF8 / 00753FF0)
    const std::int32_t first = target == 0 ? 1 : 0;                                     // 0068BA67..0068BA79 (sete)
    frame_LogFormat_005e5680(name, "BTN_%s%d", frame_LogCStr_005e5670(kind.storage, nullptr), index);   // 0068BA3B..0068BA5F
    init_formatted(panel, self, name, first);                                           // 0068BA75..0068BA84
    frame_LogFormat_005e5680(name, "LBL_%s%d", frame_LogCStr_005e5670(kind.storage, nullptr), index);   // 0068BA89..0068BA9E
    void* lbl = at(self, 0x1C4);
    init_formatted(panel, lbl, name, first);                                            // 0068BAA6..0068BAB9
    frame_LogFormat_005e5680(name, "BTN_%sUP%d", frame_LogCStr_005e5670(kind.storage, nullptr), index); // 0068BABE..0068BAD3
    void* up = at(self, 0x388);
    init_formatted(panel, up, name, first);                                             // 0068BADF..0068BAEE
    frame_LogFormat_005e5680(name, "BTN_%sDOWN%d", frame_LogCStr_005e5670(kind.storage, nullptr), index); // 0068BAF3..0068BB08
    void* down = at(self, 0x54C);
    init_formatted(panel, down, name, first);                                           // 0068BB14..0068BB23
    if (lw(self, 0x718) != 0) {                                                         // 0068BB28..0068BB30
        gui_load_control_add_child_0041acc0(self, nullptr, lbl); sw(self, 0x1D8, ad(self));   // 0068BB32..0068BB41
        gui_load_control_add_child_0041acc0(self, nullptr, up); sw(self, 0x39C, ad(self));    // 0068BB3E..0068BB4F
        gui_load_control_add_child_0041acc0(self, nullptr, down); sw(self, 0x560, ad(self));  // 0068BB4C..0068BB64
        index += 4;                                                                     // 0068BB5A..0068BB6A
    }
    const void* up_fn = target ? fn(&kotor_fastcall_006884b0) : fn(&kotor_fastcall_0068af70);     // 0068BB71..0068BBA6
    const void* down_fn = target ? fn(&kotor_fastcall_00688520) : fn(&kotor_fastcall_0068afe0);
    const void* focus_fn = target ? fn(&kotor_fastcall_00688490) : fn(&kotor_fastcall_006856c0);
    sw(lbl, 0x44, lw(lbl, 0x44) & 0xFFFFFFFBu);                                         // 0068BBAE..0068BBC0
    gui_load_control_add_event_0041ab20(lbl, nullptr, 0x27, panel, fn(&kotor_fastcall_0068b970));  // 0068BBB5..0068BBC3
    gui_load_control_add_event_0041ab20(lbl, nullptr, 0, panel, focus_fn);              // 0068BBC8..0068BBD4
    gui_load_control_add_event_0041ab20(lbl, nullptr, 0x1F4, panel, up_fn);             // 0068BBD9..0068BBE8
    gui_load_control_add_event_0041ab20(lbl, nullptr, 0x1F5, panel, down_fn);           // 0068BBED..0068BBFC
    sw(self, 0x21C, static_cast<U32>(index));                                           // 0068BC0D
    sw(self, 0x44, lw(self, 0x44) & 0xFFFFFFFBu);                                       // 0068BC13..0068BC1E
    gui_load_control_add_event_0041ab20(self, nullptr, 0, panel, focus_fn);             // 0068BC21
    gui_load_control_add_event_0041ab20(self, nullptr, 0x1F4, panel, up_fn);            // 0068BC33
    gui_load_control_add_event_0041ab20(self, nullptr, 0x1F5, panel, down_fn);          // 0068BC45
    sw(self, 0x58, static_cast<U32>(index));                                            // 0068BC55
    sw(up, 0x44, lw(up, 0x44) & 0xFFFFFFFBu);                                           // 0068BC58..0068BC60
    gui_load_control_add_event_0041ab20(up, nullptr, 0x27, panel, up_fn);               // 0068BC67
    gui_load_control_add_event_0041ab20(up, nullptr, 0, panel, focus_fn);               // 0068BC76
    gui_load_control_add_event_0041ab20(up, nullptr, 0x1F4, panel, up_fn);              // 0068BC88
    gui_load_control_add_event_0041ab20(up, nullptr, 0x1F5, panel, down_fn);            // 0068BC9A
    sw(self, 0x3E0, static_cast<U32>(index));                                           // 0068BCA6
    sw(down, 0x44, lw(down, 0x44) & 0xFFFFFFFBu);                                       // 0068BCAC
    gui_load_control_add_event_0041ab20(down, nullptr, 0x27, panel, down_fn);           // 0068BCBA
    gui_load_control_add_event_0041ab20(down, nullptr, 0, panel, focus_fn);             // 0068BCC9
    gui_load_control_add_event_0041ab20(down, nullptr, 0x1F4, panel, up_fn);            // 0068BCDB
    gui_load_control_add_event_0041ab20(down, nullptr, 0x1F5, panel, down_fn);          // 0068BCED
    sw(self, 0x5D4, 0x43340000u); sw(self, 0x648, 0x43340000u);                         // 0068BCF2..0068BCFD (180.0f)
    sw(self, 0x5A4, static_cast<U32>(index));                                           // 0068BD0D
    sw(up, 0x24, 0xBD51); sw(up, 0x30, 0xF8);                                           // 0068BD14..0068BD1B
    sw(down, 0x24, 0xBD52); sw(down, 0x30, 0xF9);                                       // 0068BD24..0068BD2B
    static const U32 strref[7] = {0xBD66, 0xBCA3, 0xBCAB, 0xBCA7, 0xBCAF, 0xBCB3, 0xBCB7}; // table 0068BE18
    static const U32 icon[7] = {0xE8, 0xEA, 0xEE, 0xEC, 0xE2, 0xE4, 0xE6};
    if (static_cast<U32>(index) <= 6) { sw(lbl, 0x24, strref[index]); sw(lbl, 0x30, icon[index]); }   // 0068BD0A..0068BDC3 (unsigned ja)
    }                                                                                   // 0068BDCA..0068BDD6 ~kind
    u.dismiss();
    gui_load_label_destruct_00419d10(scratch_label, nullptr);
    frame_LogStringDtor_005e5c20(name, nullptr);
}

// ---------------------------------------------------------------- CSWGuiCharButton (button + CExoString, table 00753BD0)
void* __fastcall kotor_fastcall_00686ab0(void* self, void*) {
    gui_load_button_construct_0041c0f0(self, nullptr);                                  // 00686ACD
    Unwind u; u.push(&gui_load_button_destruct_00419da0, self);
    set_vptr(self, vt_char_button());                                                   // 00686AE0 (00753BD0)
    kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(at(self, 0x1C8)), nullptr); // 00686AD2..00686AE6
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_00686b00(void* self, void*) {
    frame_LogStringDtor_005e5c20(at(self, 0x1C8), nullptr);                             // 00686B1D..00686B2B
    gui_load_button_destruct_00419da0(self, nullptr);                                   // 00686B30..00686B3A
}
void* __fastcall kotor_fastcall_00686b50(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_00686b00, flags); }

// ---------------------------------------------------------------- CSWGuiMainInterfaceChar (0xEA8)
void* __fastcall kotor_fastcall_00689c00(void* self, void*) {
    Unwind u;
    kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(at(self, 0x18)), nullptr); u.push(&frame_LogStringDtor_005e5c20, at(self, 0x18));
    kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(at(self, 0x20)), nullptr); u.push(&frame_LogStringDtor_005e5c20, at(self, 0x20));
    vector_member(at(self, 0x28), 0x140, 6, &gui_load_label_construct_0041acd0, &gui_load_label_destruct_00419d10, u);   // 00689C35..00689C4F
    vector_member(at(self, 0x7A8), 0x150, 2, &kotor_original::kotor_fastcall_0041aed0, &kotor_original::kotor_fastcall_0041af70, u); // 00689C54..00689C71
    label(self, 0xA48, u); label(self, 0xB88, u);                                        // 00689C76..00689C91
    kotor_fastcall_00686ab0(at(self, 0xCC8), nullptr);                                  // 00689C96..00689CA1
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_00689cc0(void* self, void*) {
    kotor_fastcall_00686b00(at(self, 0xCC8), nullptr);                                  // 00689CDD..00689CEB
    gui_load_label_destruct_00419d10(at(self, 0xB88), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0xA48), nullptr);
    vector_destruct(at(self, 0x7A8), 0x150, 2, &kotor_original::kotor_fastcall_0041af70);
    vector_destruct(at(self, 0x28), 0x140, 6, &gui_load_label_destruct_00419d10);
    frame_LogStringDtor_005e5c20(at(self, 0x20), nullptr);
    frame_LogStringDtor_005e5c20(at(self, 0x18), nullptr);
}
// 006898A0 CSWGuiMainInterfaceChar::Initialize(CSWGuiPanel* panel, int number) (thiscall, RET 8).
void __fastcall kotor_fastcall_006898a0(void* self, void*, void* panel, I32 number) {
    U32 name[2];
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(name), nullptr);   // 006898C3..006898C7
    struct Entry { const char* fmt; U32 off; std::int32_t flag; };
    static const Entry entries[] = {
        {"LBL_CHAR%d", 0x668, 1}, {"PB_VIT%d", 0x7A8, 1}, {"PB_FORCE%d", 0x8F8, 1}, {"LBL_LEVELUP%d", 0x3E8, 1},
        {"LBL_LVLUPBG%d", 0x528, 1}, {"LBL_BACK%d", 0x28, 1}, {"LBL_DEBILATATED%d", 0x168, 1}, {"LBL_DISABLED%d", 0x2A8, 1},
        {"LBL_CMBTEFCTINC%d", 0xA48, 0}, {"LBL_CMBTEFCTRED%d", 0xB88, 0}, {"BTN_CHAR%d", 0xCC8, 1}};   // 006898CC..00689A8A
    for (const Entry& e : entries) {
        frame_LogFormat_005e5680(name, e.fmt, number);
        gui_load_panel_init_control_0040b930(panel, nullptr, at(self, e.off), name, e.flag);
    }
    gui_load_border_params_set_pulsing_00414d00(at(self, 0x458), nullptr, 1, 0, 1);     // 00689A8F..00689A9B
    U32 base[4]; copy_extent(base, at(self, 0x668));                                     // 00689AA0..00689ABD
    U32 e[4];
    copy_extent(e, at(self, 0xA48)); e[0] -= base[0]; e[1] -= base[1]; set_extent(at(self, 0xA48), e);   // 00689AC1..00689B07
    copy_extent(e, at(self, 0xB88)); e[0] -= base[0]; e[1] -= base[1]; set_extent(at(self, 0xB88), e);   // 00689B0A..00689B46
    copy_extent(e, at(self, 0x7A8));                                                     // 00689B49..00689B66
    sw(self, 0xD20, static_cast<U32>(number));                                           // 00689B60
    {
        const std::uint16_t y = static_cast<std::uint16_t>(e[1]), x = static_cast<std::uint16_t>(e[0]);
        std::memcpy(at(self, 0xE98), &y, 2);                                             // 00689B80
        std::memcpy(at(self, 0xE9A), &x, 2);                                             // 00689B6F
    }
    void* button = at(self, 0xCC8);
    sw(button, 0x44, lw(button, 0x44) & 0xFFFFFFFBu);                                    // 00689B87..00689B9B
    gui_load_control_add_event_0041ab20(button, nullptr, 0x27, panel, fn(&kotor_fastcall_00688690)); // 00689B8A..00689B9E
    gui_load_control_add_event_0041ab20(button, nullptr, 0x44, panel, fn(&gui_load_slot_ret4_0060e760)); // 00689BA3..00689BAD
    sw(self, 0xE8C, ad(self));                                                           // 00689BB4
    for (U32 off = 0; off < 0x18; off += 4) zero(self, off);                             // 00689BBA..00689BCC
    zero(self, 0xEA4);                                                                   // 00689BCF
    frame_LogStringDtor_005e5c20(name, nullptr);                                         // 00689BC5..00689BDD
}

// ---------------------------------------------------------------- CSWGuiMainInterfaceStatus (label, 0x14C)
void* __fastcall kotor_fastcall_00686aa0(void* self, void*) { return gui_load_label_construct_0041acd0(self, nullptr); }  // 00686AA0
void __fastcall kotor_fastcall_0068ad10(void* self, void*) { gui_load_label_destruct_00419d10(self, nullptr); }           // 0068AD10 jmp
// 00686920 CSWGuiMainInterfaceStatus::Initialize(CSWGuiPanel* panel, BYTE kind) (thiscall, RET 8).
void __fastcall kotor_fastcall_00686920(void* self, void*, void* panel, U32 kind_word) {
    U32 name[2];
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(name), nullptr);   // 0068693B..0068693F
    const U8 kind = static_cast<U8>(kind_word);
    sb(self, 0x148, kind);                                                               // 00686944..00686948
    static const char* const names[9] = {"LBL_JOURNAL", "LBL_CASH", "LBL_PLOTXP", "LBL_STEALTHXP", "LBL_DARKSHIFT",
                                         "LBL_LIGHTSHIFT", nullptr, "LBL_ITEMRCVD", "LBL_ITEMLOST"};   // table 006869F8
    if (kind == 6) {                                                                     // 0068698F
        sw(self, 0x44, lw(self, 0x44) & 0xFFFFFFFDu);
    } else {
        if (kind <= 8) GuiAssign005e5140(name, nullptr, names[kind]);                    // 00686965..006869A5
        gui_load_panel_init_control_0040b930(panel, nullptr, self, name, 1);             // 006869AA..006869B6
        sw(self, 0x44, lw(self, 0x44) & 0xFFFFFFFDu);                                    // 006869BB..006869CA
        gui_load_border_params_set_pulsing_00414d00(at(self, 0x70), nullptr, 1, 0, 1);   // 006869BE..006869CD
    }
    frame_LogStringDtor_005e5c20(name, nullptr);                                         // 006869D2..006869DE
}

// ---------------------------------------------------------------- CSWGuiTargetActionMenu (embedded at +BC, 0x1AF0)
void* __fastcall kotor_fastcall_0068be40(void* self, void*) {
    Unwind u;
    vector_member(self, 0xC, 3, &exo_array_list_construct_005a76d0, &kotor_fastcall_005fbfb0, u);              // 0068BE59..0068BE6E
    vector_member(at(self, 0x54), 0x71C, 3, &kotor_fastcall_00689200, &kotor_fastcall_006893a0, u);          // 0068BE73..0068BE8E
    for (U32 off : {0x15B8u, 0x15B4u, 0x15B0u, 0x15ACu, 0x15C8u, 0x15C4u, 0x15C0u, 0x15BCu}) zero(self, off);    // 0068BE93..0068BEC8
    label(self, 0x15CC, u); label(self, 0x170C, u); label(self, 0x184C, u);                                       // 0068BECE..0068BEEE
    kotor_original::kotor_fastcall_0041aed0(at(self, 0x198C), nullptr); u.push(&kotor_original::kotor_fastcall_0041af70, at(self, 0x198C)); // 0068BEF3..0068BEFE
    for (U32 off = 0x24; off < 0x54; off += 4) sw(self, off, 0xFFFFFFFFu);                                       // 0068BF03..0068BF0E rep stosd x12
    sb(self, 0x1AEA, 0xFF);                                                                                       // 0068BF14
    sw(self, 0x1AEC, lw(self, 0x1AEC) & 0xFFFFFFFCu);                                                             // 0068BF1A
    sw(self, 0x1ADC, 0xBF800000u); sw(self, 0x1AE0, 0xBF800000u);                                                 // 0068BF26..0068BF2C
    sb(self, 0x1AEB, 0);                                                                                          // 0068BF32
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_0068b0c0(void* self, void*) {                             // ~CSWGuiTargetActionMenu
    kotor_original::kotor_fastcall_0041af70(at(self, 0x198C), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0x184C), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0x170C), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0x15CC), nullptr);
    vector_destruct(at(self, 0x54), 0x71C, 3, &kotor_fastcall_006893a0);
    vector_destruct(self, 0xC, 3, &kotor_fastcall_005fbfb0);
}
// 00685AF0 CSWGuiTargetActionMenu::SetNameLabel(CExoString*) (thiscall, RET 4).
void __fastcall kotor_fastcall_00685af0(void* self, void*, void* text) {
    gui_load_text_params_set_text_00415e00(at(self, 0x16B4), nullptr, text);             // 00685AFC..00685B03
    U32 e[4]; copy_extent(e, at(self, 0x15CC));                                          // 00685B08..00685B2D
    e[3] = vcall<U32>(at(self, 0x169C), 0x08);                                           // 00685B21..00685B3E text height
    set_extent(at(self, 0x15CC), e);                                                     // 00685B44..00685B45
    set_extent(at(self, 0x170C), e);                                                     // 00685B48..00685B59
    const I32 below = static_cast<I32>(e[1]) + static_cast<I32>(e[3]);                   // 00685B5C..00685B64 ebx
    U32 p[4]; copy_extent(p, at(self, 0x198C));                                          // 00685B67..00685B89
    p[1] = static_cast<U32>(static_cast<I32>(lb(self, 0x1AE8)) + below);                 // 00685B80..00685B8D
    set_extent(at(self, 0x198C), p);                                                     // 00685B91..00685BA2
    set_extent(at(self, 0x184C), p);                                                     // 00685BA5..00685BB6
    const I32 shift = static_cast<I32>(lb(self, 0x1AE9)) - li(self, 0x5C) + below;      // 00685BB9..00685BC3
    for (U32 k = 0; k < 3; ++k) {                                                        // 00685BC5..00685C9E
        void* action = at(self, 0x54 + k * 0x71C);
        for (U32 off : {0x1C4u, 0u, 0x388u, 0x54Cu}) {
            U32 c[4]; copy_extent(c, at(action, off)); c[1] = static_cast<U32>(static_cast<I32>(c[1]) + shift);
            set_extent(at(action, off), c);
        }
    }
}
// 0068BF50 CSWGuiTargetActionMenu::Initialize(CSWGuiPanel* panel) (thiscall, RET 4).
void __fastcall kotor_fastcall_0068bf50(void* self, void*, void* panel) {
    sw(self, 0x15A8, ad(panel));                                                         // 0068BF79
    for (I32 i = 0; i < 3; ++i) kotor_fastcall_0068b9d0(at(self, 0x54 + static_cast<U32>(i) * 0x71C), nullptr, panel, i, 1); // 0068BF85..0068BFA4
    { ExoString s("LBL_NAME");     gui_load_panel_init_control_0040b930(panel, nullptr, at(self, 0x15CC), s.storage, 0); } // 0068BFA6..0068BFDC
    { ExoString s("LBL_NAMEBG");   gui_load_panel_init_control_0040b930(panel, nullptr, at(self, 0x170C), s.storage, 0); } // 0068BFE1..0068C014
    { ExoString s("LBL_HEALTHBG"); gui_load_panel_init_control_0040b930(panel, nullptr, at(self, 0x184C), s.storage, 0); } // 0068C019..0068C04C
    { ExoString s("PB_HEALTH");    gui_load_panel_init_control_0040b930(panel, nullptr, at(self, 0x198C), s.storage, 0); } // 0068C051..0068C084
    sw(self, 0x15B4, lw(self, 0x15D8));                                                  // 0068C089..0068C092
    const U8 sum = static_cast<U8>(lb(self, 0x15DC) + lb(self, 0x15D4));                 // 0068C098..0068C0A4
    sb(self, 0x1AE8, static_cast<U8>(lb(self, 0x1994) - sum));                           // 0068C0A6..0068C0AE
    sb(self, 0x1AE9, static_cast<U8>(lb(self, 0x5C) - sum));                             // 0068C08F, 0068C0B4..0068C0BF
    ExoString empty("");                                                                 // 0068C0B6..0068C0C5
    kotor_fastcall_00685af0(self, nullptr, empty.storage);                               // 0068C0CA..0068C0D9
}

// ---------------------------------------------------------------- CSWGuiMainInterface layout helpers
// 00685760 CSWGuiMainInterface::ResetFont() (thiscall, RET 0; vtable slot +68h).
void __fastcall kotor_fastcall_00685760(void* self, void*) {
    gui_load_panel_reset_font_0040a9c0(self, nullptr);                                   // 00685763 CSWGuiPanel::ResetFont
    vcall<void>(at(self, 0x1688), 0x8C);                                                 // 00685768..00685774 ReSetFont
    vcall<void>(at(self, 0x17C8), 0x8C);                                                 // 0068577A..00685786
    vcall<void>(at(self, 0x1908), 0x8C);                                                 // 0068578C..00685799 (tail jmp)
}
// 00685560 CSWGuiMainInterface::SetActionDescription(CExoString*) (thiscall, RET 4).
void __fastcall kotor_fastcall_00685560(void* self, void*, void* text) {
    gui_load_text_params_set_text_00415e00(at(self, 0xA2BC), nullptr, text);             // 0068556A..00685571
    U32 e[4]; copy_extent(e, at(self, 0xA1D4));                                          // 00685576..0068559B
    const U32 h = vcall<U32>(at(self, 0xA2A4), 0x08);                                    // 0068558F..0068559F
    e[1] = lw(self, 0xA454) - h;                                                         // 006855A2..006855B4
    e[3] = h;                                                                            // 006855B0
    set_extent(at(self, 0xA1D4), e);                                                     // 006855B8..006855C3
    set_extent(at(self, 0xA314), e);                                                     // 006855C6..006855D7
}
// 006874C0 CSWGuiMainInterface::ShowActionQueue(int show) (thiscall, RET 4).
void __fastcall kotor_fastcall_006874c0(void* self, void*, U32 show) {
    const U32 flags = lw(self, 0xA45C);
    if (show == ((flags >> 1) & 1u)) return;                                             // 006874CA..006874D9
    const U32 bit = (show & 1u) * 2u;                                                    // 006874E5..006874E9
    sw(self, 0xA45C, (flags & 0xFFFFFFFDu) | bit);                                       // 006874EB..006874F3
    U32 move = 1;                                                                        // ebx
    if (show == 0 && (lb(self, 0x7260) & 2) == 0) move = 0;                              // 006874F0..00687504
    for (U32 off : {0x67A0u, 0x68E0u, 0x6A20u, 0x7260u, 0x6D14u, 0x6ED8u, 0x709Cu})      // 00687506..00687577
        sw(self, off, (lw(self, off) & 0xFFFFFFFDu) | bit);
    if (lw(self, 0x771C) == 0)                                                           // 0068757D..00687585
        for (U32 off : {0x73A0u, 0x74E0u}) sw(self, off, (lw(self, off) & 0xFFFFFFFDu) | bit);
    for (U32 off : {0x62A0u, 0x63E0u, 0x6520u, 0x6660u}) sw(self, off, (lw(self, off) & 0xFFFFFFFDu) | bit); // 006875A9..006875E9
    I32 dy = static_cast<I32>(lb(self, 0xA458));                                         // 006875EF
    if (show == 0) { dy = -dy; sw(self, 0x7724, 0xBF800000u); }                          // 006875F6..006875FA
    if (move == 0) return;                                                               // 00687604..00687606
    for (U32 off : {0x5CC0u, 0x5E00u, 0x6098u}) {                                        // 0068760C..0068769D
        U32 e[4]; copy_extent(e, at(self, off)); e[1] = static_cast<U32>(static_cast<I32>(e[1]) + dy); set_extent(at(self, off), e);
    }
    sw(self, 0x6084, static_cast<U32>(li(self, 0x6084) + dy));                           // 006876A0
    for (U32 i = 0; i < 9; ++i) {                                                        // 006876A6..006876E6
        void* status = at(self, 0xA460 + i * 0x14C);
        U32 e[4]; copy_extent(e, status); e[1] = static_cast<U32>(static_cast<I32>(e[1]) + dy); set_extent(status, e);
    }
}
// 00686E20 CSWGuiMainInterface::UpdateActionDescription() (thiscall, RET 0).
void __fastcall kotor_fastcall_00686e20(void* self, void*) {
    U32 text[2];
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(text), nullptr);   // 00686E3C..00686E40
    U32 alpha = 0x3F800000u;                                                             // 00686E54
    const double timer = lf(self, 0x6C);
    if (!jcc_jne(timer, 0.0, 0x41)) {                                                    // 00686E45..00686E61 (timer > 0)
        const double v = timer * static_cast<double>(bits_f(0x3E4CCCCDu));              // 00686E63..00686E66 (0.2f)
        if (!jcc_jp(v, static_cast<double>(bits_f(0x3F000000u)), 0x05))                  // 00686E6C..00686E77 (v < 0.5f)
            alpha = f_bits(static_cast<float>(v + v));                                   // 00686E79..00686E7B
        U32 gui_text[2];
        kotor_fastcall_005edeb0(client_app(), nullptr, gui_text, lw(self, 0x70));        // 00686E83..00686E94
        async_assign_string_005e5c50(text, nullptr, gui_text);                           // 00686E99..00686EA3
        frame_LogStringDtor_005e5c20(gui_text, nullptr);                                 // 00686EA8..00686EB1
    } else {
        const I32 list = li(self, 0x1BC4);                                               // 00686EBB
        U8* action = static_cast<U8*>(at(self, 0x772C + static_cast<U32>(list) * 0x71C)); // 00686EC1..00686ED3
        const bool any = (lb(self, 0x7934 + static_cast<U32>(list) * 0x71C) & 1) || (action[0x3CC] & 1) ||
                         (action[0x590] & 1) || (action[0x44] & 1);                      // 00686EC9..00686EF2
        if (any && list >= 0) {                                                          // 00686EF4..00686EF6
            const I32 count = li(self, 0x78 + static_cast<U32>(list) * 0xC);             // 00686EF8..00686EFD
            if (count > 0) {                                                             // 00686F00..00686F02
                U8* found = nullptr;
                const I32 selected = li(self, 0x1BAC + static_cast<U32>(list) * 4);       // 00686F05
                if (selected != -1) {                                                    // 00686F0C..00686F0F
                    U8* element = static_cast<U8*>(lp(self, 0x74 + static_cast<U32>(list) * 0xC));
                    for (I32 i = 0; i < count; ++i, element += 0x38)                     // 00686F17..00686F2D
                        if (li(element, 8) == selected) found = element;
                }
                if (found == nullptr) found = static_cast<U8*>(lp(self, 0x74 + static_cast<U32>(list) * 0xC)); // 00686F33..00686F36
                async_assign_string_005e5c50(text, nullptr, found);                      // 00686F3A..00686F3F
                const I32 uses = li(found, 0x34);                                        // 00686F44
                if (uses > 1)                                                            // 00686F47..00686F4B
                    frame_LogFormat_005e5680(text, "%s (%d)", frame_LogCStr_005e5670(text, nullptr), uses); // 00686F4D..00686F62
            }
        }
    }
    kotor_fastcall_00685560(self, nullptr, text);                                        // 00686F6B..00686F72
    sw(self, 0xA2E4, alpha);                                                             // 00686F77..00686F7F
    frame_LogStringDtor_005e5c20(text, nullptr);                                         // 00686F7B..00686F8D
}

// ---------------------------------------------------------------- 0068C100 CSWGuiMainInterface::CSWGuiMainInterface
void* __fastcall kotor_fastcall_0068c100(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                           // 0068C127..0068C12E
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_main_interface());                                                 // 0068C14E (00753F50)
    vector_member(at(self, 0x74), 0xC, 6, &exo_array_list_construct_005a76d0, &kotor_fastcall_005fbfb0, u);   // 0068C133..0068C154
    kotor_fastcall_0068be40(at(self, 0xBC), nullptr); u.push(&kotor_fastcall_0068b0c0, at(self, 0xBC));          // 0068C159..0068C167
    vector_member(at(self, 0x1BC8), 0x140, 3, &gui_load_label_construct_0041acd0, &gui_load_label_destruct_00419d10, u); // 0068C16C..0068C18C
    vector_member(at(self, 0x1F88), 0xEA8, 3, &kotor_fastcall_00689c00, &kotor_fastcall_00689cc0, u);         // 0068C191..0068C1B1
    kotor_fastcall_00689c00(at(self, 0x4B80), nullptr); u.push(&kotor_fastcall_00689cc0, at(self, 0x4B80));      // 0068C1B6..0068C1C4
    label(self, 0x5A2C, u); label(self, 0x5B6C, u);                                      // 0068C1C9..0068C1EC
    zero(self, 0x5CB8); zero(self, 0x5CBC); zero(self, 0x5CB4);                          // 0068C1F1..0068C1FD
    label(self, 0x5CC0, u); label(self, 0x5E00, u); label(self, 0x5F40, u);             // 0068C203..0068C237
    zero(self, 0x608C); zero(self, 0x6088); zero(self, 0x6084); zero(self, 0x6080);      // 0068C24A..0068C25C
    button(self, 0x6098, u);                                                             // 0068C23C..0068C262
    vector_member(at(self, 0x625C), 0x140, 4, &gui_load_label_construct_0041acd0, &gui_load_label_destruct_00419d10, u); // 0068C26F..0068C287
    vector_member(at(self, 0x675C), 0x140, 3, &gui_load_label_construct_0041acd0, &gui_load_label_destruct_00419d10, u); // 0068C28C..0068C2AC
    gui_load_label_hilight_construct_0041b5c0(at(self, 0x6B1C), nullptr); u.push(&gui_m48_label_hilight_destruct_0041b610, at(self, 0x6B1C)); // 0068C2B1..0068C2BF
    button(self, 0x6CD0, u); button(self, 0x6E94, u); button(self, 0x7058, u);           // 0068C2C4..0068C2F8
    label(self, 0x721C, u); label(self, 0x735C, u); label(self, 0x749C, u); label(self, 0x75DC, u);   // 0068C2FD..0068C344
    vector_member(at(self, 0x772C), 0x71C, 6, &kotor_fastcall_00689200, &kotor_fastcall_006893a0, u);        // 0068C349..0068C369
    label(self, 0xA1D4, u); label(self, 0xA314, u);                                      // 0068C36E..0068C38F
    vector_member(at(self, 0xA460), 0x14C, 9, &kotor_fastcall_00686aa0, &kotor_fastcall_0068ad10, u);         // 0068C394..0068C3B4
    for (U32 off : {0xB00Cu, 0xB1D0u, 0xB394u, 0xB558u, 0xB71Cu, 0xB8E0u, 0xBAA4u, 0xBC68u}) button(self, off, u); // 0068C3B9..0068C44C
    label(self, 0xBE2C, u); label(self, 0xBF6C, u);                                      // 0068C451..0068C472
    for (U32 off : {0xC0ACu, 0xC360u, 0xC614u}) {                                        // 0068C477..0068C4AB
        gui_load_button_toggle_construct_0041ca30(at(self, off), nullptr); u.push(&gui_m48_button_toggle_destruct_0041cad0, at(self, off));
    }
    U32 scratch[2];                                                                      // [esp+24h]
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(scratch), nullptr);   // 0068C4B0..0068C4BC
    u.push(&frame_LogStringDtor_005e5c20, scratch);
    unsigned char map_view[0x140];                                                       // [esp+88h] (local CSWGuiLabel)
    gui_load_label_construct_0041acd0(map_view, nullptr);                                // 0068C4C1..0068C4D0
    u.push(&gui_load_label_destruct_00419d10, map_view);
    main_interface_00833bb4 = self;                                                      // 0068C4D8
    {
        const I32 w = static_cast<std::int16_t>(lh(manager, 0x6C));                     // 0068C4D5..0068C4DE
        const char* layout = nullptr;
        if (w == 0x400) layout = "mipc210x7";                                            // 0068C563 (00754150)
        else if (w == 0x500) {
            const I32 h = static_cast<std::int16_t>(lh(manager, 0x6E));                 // 0068C529
            if (h == 0x3C0) layout = "mipc212x9";                                        // 0068C54E (0075415C)
            else if (h == 0x400) layout = "mipc212x10";                                  // 0068C539 (00754168)
        } else if (w == 0x640) layout = "mipc216x12";                                    // 0068C515 (00754174)
        else layout = "mipc28x6";                                                        // 0068C500 (00754180)
        if (layout != nullptr) start_layout(self, layout);                               // 0068C576..0068C578 (skipped for 1280 x other)
    }
    sw(self, 0x44, lw(self, 0x44) | 1u);                                                 // 0068C57D
    for (I32 i = 0; i < 6; ++i) kotor_fastcall_0068b9d0(at(self, 0x772C + static_cast<U32>(i) * 0x71C), nullptr, self, i, 0); // 0068C581..0068C5BE
    for (I32 i = 1; i <= 3; ++i) kotor_fastcall_006898a0(at(self, 0x1F88 + static_cast<U32>(i - 1) * 0xEA8), nullptr, self, i); // 0068C5C0..0068C5F5
    kotor_fastcall_006898a0(at(self, 0x4B80), nullptr, self, 4);                          // 0068C5F7..0068C600
    for (I32 i = 0; i < 4; ++i) {                                                        // 0068C605..0068C65B
        frame_LogFormat_005e5680(scratch, "LBL_QUEUE%d", i);
        gui_load_panel_init_control_0040b930(self, nullptr, at(self, 0x625C + static_cast<U32>(i) * 0x140), scratch, 1);
    }
    for (U32 i = 0; i < 9; ++i) kotor_fastcall_00686920(at(self, 0xA460 + i * 0x14C), nullptr, self, i);        // 0068C65D..0068C699
    kotor_fastcall_0068bf50(at(self, 0xBC), nullptr, self);                              // 0068C69B..0068C6A2
    for (I32 i = 1; i <= 3; ++i) {                                                       // 0068C6A7..0068C6F3
        frame_LogFormat_005e5680(scratch, "LBL_MOULDING%d", i);
        gui_load_panel_init_control_0040b930(self, nullptr, at(self, 0x1BC8 + static_cast<U32>(i - 1) * 0x140), scratch, 1);
    }
    for (I32 i = 1; i <= 3; ++i) {                                                       // 0068C6F5..0068C741
        frame_LogFormat_005e5680(scratch, "LBL_COMBATBG%d", i);
        gui_load_panel_init_control_0040b930(self, nullptr, at(self, 0x675C + static_cast<U32>(i - 1) * 0x140), scratch, 1);
    }
    init_control(self, 0x6CD0, "BTN_CLEARONE");                                          // 0068C743..0068C779 (00754114)
    init_control(self, 0x6E94, "BTN_CLEARONE2");                                         // 0068C77E..0068C7B4 (00754104)
    init_control(self, 0x7058, "BTN_CLEARALL");                                          // 0068C7B9..0068C7EF (007540F4)
    init_control(self, 0xA1D4, "LBL_ACTIONDESC");                                        // 0068C7F4..0068C82A (007540E4)
    init_control(self, 0xA314, "LBL_ACTIONDESCBG");                                      // 0068C82F..0068C865 (007540D0)
    init_control(self, 0x5CC0, "LBL_MAPBORDER");                                         // 0068C86A..0068C8A0 (007540C0)
    init_control(self, 0x5F40, "LBL_ARROW", 0);                                          // 0068C8A5..0068C8DB (007540B4)
    init_control(self, 0x5E00, "LBL_MAP", 0);                                            // 0068C8E0..0068C916 (007540AC)
    { ExoString s("LBL_MAPVIEW"); gui_load_panel_init_control_0040b930(self, nullptr, map_view, s.storage, 0); }   // 0068C91B..0068C952 (007540A0)
    init_control(self, 0x6098, "BTN_MINIMAP");                                           // 0068C957..0068C98D (00754094)
    init_control(self, 0x735C, "LBL_CMBTMODEMSG");                                       // 0068C992..0068C9C8 (00754084)
    vcall<void, const void*, void*, float>(at(self, 0x75DC), 0x98, at(self, 0x7360), at(self, 0x735C), 1.0f);   // 0068C9CD..0068C9EC
    init_control(self, 0x749C, "LBL_CMBTMSGBG");                                         // 0068C9F2..0068CA28 (00754074)
    sw(self, 0x73A0, lw(self, 0x73A0) & 0xFFFFFFFDu);                                    // 0068CA2D..0068CA3A
    sw(self, 0x74E0, lw(self, 0x74E0) & 0xFFFFFFFDu);                                    // 0068CA40
    sw(self, 0x7620, lw(self, 0x7620) & 0xFFFFFFFDu);                                    // 0068CA46
    {
        U32 combat[2];                                                                   // [esp+10h]
        static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08)->simple_0041e8f0(
            reinterpret_cast<kotor_menu_10_19::String*>(combat), 0xBD4A);                // 0068CA4C..0068CA5C
        gui_load_text_params_set_text_00415e00(at(self, 0x76C4), nullptr, combat);       // 0068CA61..0068CA70
        frame_LogStringDtor_005e5c20(combat, nullptr);                                   // 0068CA75..0068CA80
    }
    init_control(self, 0xB00C, "BTN_EQU");                                               // 0068CA85 (007500D8)
    init_control(self, 0xB1D0, "BTN_INV");                                               // 0068CAC0 (007500D0)
    init_control(self, 0xB394, "BTN_CHAR");                                              // 0068CAFB (007500C4)
    init_control(self, 0xB558, "BTN_ABI");                                               // 0068CB36 (007500B4)
    init_control(self, 0xB71C, "BTN_MSG");                                               // 0068CB71 (0075009C)
    init_control(self, 0xB8E0, "BTN_JOU");                                               // 0068CBAC (007500AC)
    init_control(self, 0xBAA4, "BTN_MAP");                                               // 0068CBE7 (007500BC)
    init_control(self, 0xBC68, "BTN_OPT");                                               // 0068CC22 (007500A4)
    init_control(self, 0xBE2C, "LBL_MENUBG");                                            // 0068CC5D (00752EC8)
    init_control(self, 0xBF6C, "LBL_ARROW_MARGIN");                                      // 0068CC98 (00754060)
    init_control(self, 0xC0AC, "TB_PAUSE");                                              // 0068CCD3 (00754054)
    init_control(self, 0xC360, "TB_SOLO");                                               // 0068CD0E (0075404C)
    init_control(self, 0xC614, "TB_STEALTH");                                            // 0068CD49 (00754040)
    stop_layout(self);                                                                   // 0068CD84..0068CD86
    {
        const I32 bottom = li(self, 0xBE34) + li(self, 0xBE3C);                          // 0068CD8B..0068CD9D
        const I32 x = li(self, 0xBF70);                                                  // 0068CD97
        const I32 width = static_cast<std::int16_t>(lh(lp(self, 0x18), 0x6C)) - 2 * x;  // 0068CDAB..0068CDB4
        sw(self, 0x1678, static_cast<U32>(x));                                           // 0068CDC2..0068CDC8
        sw(self, 0x167C, static_cast<U32>(bottom));                                      // 0068CDCE
        sw(self, 0x1680, static_cast<U32>(width));                                       // 0068CDD5
        sw(self, 0x1684, static_cast<U32>(li(self, 0xC0B4) - bottom));                   // 0068CD9F..0068CDDE
    }
    gui_m48_panel_add_control_0040b9a0(self, nullptr, at(self, 0x75DC));                  // 0068CDE1..0068CDE4
    if (static_cast<std::int16_t>(lh(manager, 0x6C)) >= 0x400) {                         // 0068CDE9..0068CDF2
        U32 e[4]; std::memcpy(e, at(self, 4), 16);                                       // 0068CDF4..0068CE19
        e[2] = kotor_renderer_gl_cells::width_0078d1d4;                                  // 0068CE13..0068CE21
        e[3] = kotor_menu_window_cells::height_0078d1d8;                                 // 0068CE0A..0068CE28
        gui_load_panel_set_extent_0040a5a0(self, nullptr, e);                            // 0068CE25..0068CE2C
    }
    {
        I32 margin_a = 0, margin_b = 0;                                                  // [esp+3Ch], [esp+44h]
        kotor_fastcall_00409ee0(lp(self, 0x18), nullptr, &margin_a, &margin_b);          // 0068CE31..0068CE3E (results unused)
    }
    sw(self, 0x6094, lw(self, 0x5E10));                                                  // 0068CE43..0068CE56
    sw(self, 0x6080, lw(map_view, 4)); sw(self, 0x6084, lw(map_view, 8));                // 0068CE4F..0068CE70
    sw(self, 0x6090, lw(self, 0x5E0C));                                                  // 0068CE76
    sw(self, 0x6088, lw(map_view, 0xC)); sw(self, 0x608C, lw(map_view, 0x10));           // 0068CE7C..0068CE89
    {
        U32 e[4]; copy_extent(e, at(self, 0x5F40));                                      // 0068CE8F..0068CEA5
        e[0] = static_cast<U32>((li(self, 0x6088) - static_cast<I32>(e[2])) / 2);        // 0068CEA8..0068CEB9
        e[1] = static_cast<U32>((li(self, 0x608C) - static_cast<I32>(e[3])) / 2);        // 0068CEBD..0068CED8
        sb(self, 0xA458, 0);                                                             // 0068CEDF
        sw(self, 0x7724, 0xBF800000u);                                                   // 0068CEE6
        set_extent(at(self, 0x5F40), e);                                                 // 0068CEDC..0068CEF0
    }
    sw(self, 0x5AB8, (lw(self, 0x5AB8) & 0xFFFFFFFEu) | 2u);                             // 0068CEF3..0068CEFF
    sw(self, 0x5A70, lw(self, 0x5A70) | 0x20u);                                          // 0068CF05..0068CF14
    gui_m48_panel_add_control_0040b9a0(self, nullptr, at(self, 0x5A2C));                  // 0068CF17..0068CF1A
    { const U32 e[4] = {0, 0, 0x10, 0x10}; set_extent(at(self, 0x5B6C), e); }             // 0068CF1F..0068CF3F
    sw(self, 0x5BF8, (lw(self, 0x5BF8) & 0xFFFFFFFEu) | 2u);                             // 0068CF42..0068CF4E
    sw(self, 0x5BB0, lw(self, 0x5BB0) | 0x20u);                                          // 0068CF54..0068CF5F
    sw(self, 0x5BB0, lw(self, 0x5BB0) & 0xFFFFFFFDu);                                    // 0068CF62
    gui_m48_panel_add_control_0040b9a0(self, nullptr, at(self, 0x5B6C));                  // 0068CF65..0068CF68
    sw(self, 0x5CAC, 0x7F000000u); sw(self, 0x64, 0x7F000000u); sw(self, 0x68, 0x7F000000u);   // 0068CF6D..0068CF7B
    sw(self, 0x5CB0, 0xBF800000u); sw(self, 0x6C, 0xBF800000u);                          // 0068CF7E..0068CF89
    for (U32 off = 0x1BAC; off < 0x1BC4; off += 4) sw(self, off, 0xFFFFFFFFu);           // 0068CF8C..0068CF9A rep stosd x6
    sw(self, 0xA45C, lw(self, 0xA45C) | 2u);                                             // 0068CF9C..0068CFAA
    zero(self, 0x5A28);                                                                  // 0068CFB0
    kotor_fastcall_006874c0(self, nullptr, 0);                                           // 0068CFA7..0068CFB6 ShowActionQueue(0)
    sw(self, 0x7728, 0xBC50); zero(self, 0x771C);                                        // 0068CFC9..0068CFD3
    for (U32 off : {0xB00Cu, 0xB1D0u, 0xB394u, 0xB558u, 0xB8E0u, 0xB71Cu, 0xBAA4u, 0xBC68u, 0x6098u})   // 0068CFBB..0068D071
        add_event(self, off, 0x27, self, fn(&kotor_fastcall_006883c0));
    for (U32 off : {0xB050u, 0xB214u, 0xB3D8u, 0xB59Cu, 0xB924u, 0xB760u, 0xBAE8u, 0xBCACu})              // 0068D076..0068D0AD
        sw(self, off, lw(self, off) & 0xFFFFFFFBu);
    sw(self, 0x60DC, lw(self, 0x60DC) & 0xFFFFFFFBu);                                    // 0068D0B3..0068D0C9
    add_event(self, 0x6CD0, 0x27, self, fn(&kotor_fastcall_0068b050));                   // 0068D0B9..0068D0CF
    add_event(self, 0x6E94, 0x27, self, fn(&kotor_fastcall_0068b050));                   // 0068D0D4..0068D0E2
    add_event(self, 0x7058, 0x27, self, fn(&kotor_fastcall_0068b0a0));                   // 0068D0E7..0068D0F5
    for (U32 off : {0x6D14u, 0x6ED8u, 0x709Cu}) sw(self, off, lw(self, off) & 0xFFFFFFFBu);   // 0068D0FA..0068D106
    sw(self, 0xBFB0, lw(self, 0xBFB0) & 0xFFFFFFFDu);                                    // 0068D10C..0068D11C
    sw(self, 0xBFB0, lw(self, 0xBFB0) & 0xFFFFFFFBu);                                    // 0068D123..0068D12D
    add_event(self, 0xC0AC, 0x27, self, fn(&kotor_fastcall_00688590));                   // 0068D115..0068D133
    add_event(self, 0xC360, 0x27, self, fn(&kotor_fastcall_00688610));                   // 0068D138..0068D146
    add_event(self, 0xC614, 0x27, self, fn(&kotor_fastcall_00688640));                   // 0068D14B..0068D159
    for (U32 off : {0xC0F0u, 0xC3A4u, 0xC658u}) sw(self, off, lw(self, off) & 0xFFFFFFFBu);   // 0068D15E..0068D177
    sw(self, 0xC8C8, ad(kotor_cdecl_0045a260("blackdot")));                              // 0068D172..0068D18E (00754034)
    zero(self, 0x1BC4);                                                                  // 0068D194
    { ExoString empty(""); gui_load_text_params_set_text_00415e00(at(self, 0x7444), nullptr, empty.storage); }   // 0068D185..0068D1C6
    sw(self, 0xA454, static_cast<U32>(li(self, 0xA1E4) + li(self, 0xA1DC)));             // 0068D1CB..0068D1E0
    { ExoString empty(""); kotor_fastcall_00685560(self, nullptr, empty.storage); }      // 0068D1D7..0068D20A
    kotor_fastcall_00686e20(self, nullptr);                                              // 0068D20F..0068D211
    static const U32 strings[][2] = {
        {0xB030, 0xBC5B}, {0xB03C, 0xD1}, {0xB1F4, 0xBC5C}, {0xB200, 0xD2}, {0xB3B8, 0xBC61}, {0xB3C4, 0xD3},
        {0xBAC8, 0xBC5D}, {0xBAD4, 0xD7}, {0x60BC, 0xBC5D}, {0x60C8, 0xD7}, {0xB57C, 0xBC60}, {0xB588, 0xD4},
        {0xB904, 0xBC5A}, {0xB910, 0xD6}, {0xBC8C, 0xBC5E}, {0xBC98, 0xD8}, {0xB740, 0xBC5F}, {0xB74C, 0xD5},
        {0xC0D0, 0xBB93}, {0xC0DC, 0xF1}, {0xC384, 0xBBA3}, {0xC390, 0xCF}, {0xC638, 0xF7}, {0xC644, 0x108},
        {0x6CF4, 0xBD48}, {0x6D00, 0xF5}, {0x6EB8, 0xBD48}, {0x6EC4, 0xF5}, {0x707C, 0xBD86}, {0x7088, 0xF0}};   // 0068D216..0068D32C
    for (const auto& s : strings) sw(self, s[0], s[1]);
    u.dismiss();
    gui_load_label_destruct_00419d10(map_view, nullptr);                                 // 0068D336..0068D378 (inlined ~CSWGuiLabel)
    frame_LogStringDtor_005e5c20(scratch, nullptr);                                      // 0068D37D..0068D389
    return self;
}
// 0068B170 CSWGuiMainInterface::~CSWGuiMainInterface (complete); 0068D3B0 scalar deleting.
void __fastcall kotor_fastcall_0068b170(void* self, void*) {
    set_vptr(self, vt_main_interface());                                                 // 0068B18D
    if (void* image = lp(self, 0xC8C8)) vcall<void*, U32>(image, 0x04, 1);               // 0068B193..0068B1A9 release
    for (U32 off : {0xC614u, 0xC360u, 0xC0ACu}) gui_m48_button_toggle_destruct_0041cad0(at(self, off), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0xBF6C), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0xBE2C), nullptr);
    for (U32 off : {0xBC68u, 0xBAA4u, 0xB8E0u, 0xB71Cu, 0xB558u, 0xB394u, 0xB1D0u, 0xB00Cu}) gui_load_button_destruct_00419da0(at(self, off), nullptr);
    vector_destruct(at(self, 0xA460), 0x14C, 9, &kotor_fastcall_0068ad10);
    gui_load_label_destruct_00419d10(at(self, 0xA314), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0xA1D4), nullptr);
    vector_destruct(at(self, 0x772C), 0x71C, 6, &kotor_fastcall_006893a0);
    for (U32 off : {0x75DCu, 0x749Cu, 0x735Cu, 0x721Cu}) gui_load_label_destruct_00419d10(at(self, off), nullptr);
    for (U32 off : {0x7058u, 0x6E94u, 0x6CD0u}) gui_load_button_destruct_00419da0(at(self, off), nullptr);
    gui_m48_label_hilight_destruct_0041b610(at(self, 0x6B1C), nullptr);
    vector_destruct(at(self, 0x675C), 0x140, 3, &gui_load_label_destruct_00419d10);
    vector_destruct(at(self, 0x625C), 0x140, 4, &gui_load_label_destruct_00419d10);
    gui_load_button_destruct_00419da0(at(self, 0x6098), nullptr);
    for (U32 off : {0x5F40u, 0x5E00u, 0x5CC0u}) gui_load_label_destruct_00419d10(at(self, off), nullptr);
    live_free(lp(self, 0x5CB4));                                                         // 0068B3D0..0068B3DC
    gui_load_label_destruct_00419d10(at(self, 0x5B6C), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0x5A2C), nullptr);
    kotor_fastcall_00689cc0(at(self, 0x4B80), nullptr);
    vector_destruct(at(self, 0x1F88), 0xEA8, 3, &kotor_fastcall_00689cc0);
    vector_destruct(at(self, 0x1BC8), 0x140, 3, &gui_load_label_destruct_00419d10);
    kotor_fastcall_0068b0c0(at(self, 0xBC), nullptr);
    vector_destruct(at(self, 0x74), 0xC, 6, &kotor_fastcall_005fbfb0);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall kotor_fastcall_0068d3b0(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_0068b170, flags); }
}  // extern "C"
