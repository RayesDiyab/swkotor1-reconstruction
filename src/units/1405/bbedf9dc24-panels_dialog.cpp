// Lane W4 G15/G19-client: dialogue panel family constructed by CGuiInGame::Initialize -- CSWGuiDialog (base),
// CSWGuiDialogTop, CSWGuiDialogCinematic (the conversation panel used for the opening Trask dialogue),
// CSWGuiDialogComputer(+Info), CSWGuiDialogComputerCamera -- plus the leaves their constructors execute.
// Transcribed from the annotated listings (build/hudtools/ann.py) of the cited original addresses.
#include "37dfeb87ca-gui_panels.hpp"
using namespace w4gui;

namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }   // CExoString()
namespace kotor_renderer_gl_cells { extern unsigned width_0078d1d4; }                                    // [0078D1D4]
namespace kotor_menu_window_cells { extern volatile unsigned height_0078d1d8; }                          // [0078D1D8]
namespace kotor_sound_rev3 { void* __fastcall SourceConstruct005d60e0(void*, void*, const void*); }      // CExoSoundSource(CResRef)
namespace kotor_sound_path_required {
void __fastcall source_set_volume_005db800(void*, void*, unsigned, unsigned, unsigned);                   // CExoSoundSourceInternal::SetVolume
}
extern "C" {
extern const unsigned char borrowed_exit_color_007a23b4[12];                                             // [007A23B4] input-cells
extern const unsigned char borrowed_enter_color_007a23c0[12];                                            // [007A23C0] input-cells
extern void* volatile kotor_cell_rules_007a3a28;                                                         // [007A3A28] CSWRules*
void* __fastcall c2da_default_ref_00405ed0(void* self, void*);                                           // CResRef()
void* __fastcall async_ref_construct_string_00406d60(void* self, void*, const void* source);             // CResRef(const CResRef&/char[16])
void* __fastcall async_ref_assign_string_004061f0(void* self, void*, void* result, void* string);        // CResRef::operator=(CExoString)
void __fastcall gui_load_text_params_set_base_font_00415dd0(void* params, void*, const void* ref);       // CSWGuiTextParams::SetBaseFont
void* __fastcall gui_load_border_params_construct_00415000(void* self, void*, const void* corner, const void* edge,
    const void* fill, U32 fill_style, I32 dimension, float f0c, const void* color, I32 inner_offset) noexcept(false); // CSWGuiBorderParams
void __fastcall gui_load_border_params_set_fill_00414c00(void* params, void*, const void* ref, I32);      // CSWGuiBorderParams::SetFillImage
void __fastcall gui_load_listbox_clear_items_00419a90(void* listbox, void*);                              // CSWGuiListBox::ClearItems
void __fastcall kotor_m15_sound_source_set_looping_005d59d0(void* source, void*, U32);                   // CExoSoundSource::SetLooping
}

namespace w4gui {
const void* vt_dialog();
const void* vt_dialog_top();
const void* vt_dialog_cinematic();
const void* vt_dialog_computer();
const void* vt_dialog_computer_camera();
const void* vt_blackened_label();
// [007A239C] (-1,-1,-1): .data, read by CSWGuiDialogTop only, never written in the original image. W4-owned.
extern const float dialog_top_color_007a239c[3] = {-1.0f, -1.0f, -1.0f};
}

namespace {
// Label vtable slot +98h (CSWGuiLabel::Initialize 00418CD0: Extent*, CSWGuiControl* proto, float alpha; RET 0Ch).
void label_initialize(void* label, const void* extent, void* proto, float alpha) {
    vcall<void, const void*, void*, float>(label, 0x98, extent, proto, alpha);
}
}

extern "C" {
// 00409EE0 CSWGuiManager::GetSafeMargin(int* x, int* y) (thiscall, RET 8).
void __fastcall kotor_fastcall_00409ee0(void*, void*, std::int32_t* x, std::int32_t* y) {
    *x = 0x30;                                                                           // 00409EE8
    *y = 0x24;                                                                           // 00409EEE
}

// 00417120 CSWGuiTextParams::CSWGuiTextParams(CExoString text, CResRef* font, Vector* color, int align, float alpha)
// (thiscall, RET 14h; `text` is a CExoString* argument here).
void* __fastcall kotor_fastcall_00417120(void* self, void*, void* text, const void* font, const void* color, U32 align, U32 alpha_bits) {
    kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(self), nullptr);   // 0041713F
    c2da_default_ref_00405ed0(at(self, 0x0C), nullptr);                                  // 00417146..0041714D
    zero(self, 0x1C); zero(self, 0x20); zero(self, 0x24);                                // 00417156..0041715C
    zero(self, 0x2C); zero(self, 0x30); zero(self, 0x34);                                // 0041715F..00417167
    zero(self, 0x50);                                                                    // 0041716D
    async_assign_string_005e5c50(self, nullptr, text);                                   // 0041716A..00417170
    std::memcpy(at(self, 0x2C), color, 12);                                              // 00417175..00417188
    gui_load_text_params_set_base_font_00415dd0(self, nullptr, font);                    // 0041718B..00417192
    std::memcpy(at(self, 0x1C), at(self, 0x2C), 12);                                     // 00417197..004171AC
    sw(self, 0x28, alpha_bits);                                                          // 004171B9
    sw(self, 0x38, (lw(self, 0x38) & 0xFFFFFF80u) | (align & 0x3Fu));                    // 004171A9..004171BE
    sw(self, 0x08, 0xFFFFFFFFu);                                                         // 004171C6
    return self;
}

// 005D5950 CExoSoundSource::SetVolume(BYTE) (thiscall, RET 4).
void __fastcall kotor_fastcall_005d5950(void* source, void*, U32 volume_word) {
    U32 volume = volume_word;
    if (static_cast<U8>(volume) > 0x7F) volume = (volume & 0xFFFFFF00u) | 0x7Fu;         // 005D5950..005D595A (byte store)
    void* internal = lp(source, 4);                                                      // 005D595E
    if (internal == nullptr) return;                                                     // 005D5961..005D5963
    kotor_sound_path_required::source_set_volume_005db800(internal, nullptr, volume, 0, 0x3F800000u); // 005D5965..005D5971
}

// ---------------------------------------------------------------------- CSWGuiDialog (base, table 007559E0)
// 006A7160 CSWGuiDialog::Reset() (thiscall, RET 0).
void __fastcall kotor_fastcall_006a7160(void* self, void*) {
    sw(self, 0x1A08, lw(self, 0x1A08) & 0xFFFFFFFDu);                                    // 006A7182..006A718F
    sw(self, 0x1CE8, lw(self, 0x1CE8) & 0xFFFFFFFDu);                                    // 006A7195
    sw(self, 0x68, 0xFFFFFFFFu);                                                         // 006A71A7
    zero(self, 0x6C);                                                                    // 006A71AE
    GuiAssign005e5140(at(self, 0x1DF0), nullptr, "");                                    // 006A719C..006A71B5 (0073D71C "")
    for (U32 i = 0; i < 20; ++i) {                                                       // 006A71C0..006A7212
        void* params = at(self, 0x158 + i * 0x140);
        gui_load_text_params_set_color_00414e10(params, nullptr, at(self, 0x1DE4));      // 006A71D0..006A71D3
        ExoString empty("");                                                             // 006A71D8..006A71E1
        gui_load_text_params_set_text_00415e00(params, nullptr, empty.storage);          // 006A71E6..006A71F5
    }
}
// 006A85B0 CSWGuiDialog::CSWGuiDialog(CSWGuiManager*) (thiscall, RET 4).
void* __fastcall kotor_fastcall_006a85b0(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                           // 006A85CE..006A85D3
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_dialog());                                                         // 006A85F3 (007559E0)
    vector_construct(at(self, 0x70), 0x140, 20, &gui_load_label_construct_0041acd0, &gui_load_label_destruct_00419d10); // 006A85D8..006A85F9
    for (U32 i = 20; i-- > 0;) u.push(&gui_load_label_destruct_00419d10, at(self, 0x70 + i * 0x140));
    listbox(self, 0x19C4, u);                                                            // 006A85FE..006A8609
    label(self, 0x1CA4, u);                                                              // 006A860E..006A8619
    zero(self, 0x1DE4); zero(self, 0x1DE8); zero(self, 0x1DEC);                          // 006A8629..006A8635
    kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(at(self, 0x1DF0)), nullptr); // 006A861E..006A863B
    u.push(&frame_LogStringDtor_005e5c20, at(self, 0x1DF0));
    sw(self, 0x19C0, 0x7F000000u);                                                       // 006A8653
    sw(self, 0x1DF8, (lw(self, 0x1DF8) & 0xFFFFFFFDu) | 5u);                             // 006A8640..006A865D
    kotor_fastcall_006a7160(self, nullptr);                                              // 006A864C..006A8663 Reset (direct)
    u.dismiss();
    return self;
}
// 006A6F30 CSWGuiDialog::~CSWGuiDialog (complete).
void __fastcall kotor_fastcall_006a6f30(void* self, void*) {
    frame_LogStringDtor_005e5c20(at(self, 0x1DF0), nullptr);                             // 006A6F4D..006A6F5B
    gui_load_label_destruct_00419d10(at(self, 0x1CA4), nullptr);                         // 006A6F60..006A6F6B
    gui_load_listbox_destruct_0041b060(at(self, 0x19C4), nullptr);                       // 006A6F70..006A6F7B
    vector_destruct(at(self, 0x70), 0x140, 20, &gui_load_label_destruct_00419d10);       // 006A6F80..006A6F95
    gui_load_panel_destruct_0040cf70(self, nullptr);                                     // 006A6F9A..006A6FA4
}
void* __fastcall kotor_fastcall_006a8680(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006a6f30, flags); }
// 006A6FC0 CSWGuiDialog::SetReplyActive(CSWGuiControl*) (event PMF, RET 4): reply text colour = [007A23C0].
void __fastcall kotor_fastcall_006a6fc0(void*, void*, void* control) {
    void* label = vcall<void*>(control, 0x50);                                           // 006A6FC0..006A6FC6
    gui_load_text_params_set_color_00414e10(at(label, 0xE8), nullptr, borrowed_enter_color_007a23c0); // 006A6FC9..006A6FD7
}
// 006A6FE0 CSWGuiDialog::SetReplyInactive(CSWGuiControl*) (event PMF, RET 4): colour = this+1DE4.
void __fastcall kotor_fastcall_006a6fe0(void* self, void*, void* control) {
    void* label = vcall<void*>(control, 0x50);                                           // 006A6FE3..006A6FE9
    gui_load_text_params_set_color_00414e10(at(label, 0xE8), nullptr, at(self, 0x1DE4)); // 006A6FEC..006A6FF9
}

// ---------------------------------------------------------------------- CSWGuiDialogTop (table 00755790, 0x1A4)
// 006A78C0 CSWGuiDialogTop::CSWGuiDialogTop(CSWGuiManager*, CResRef* font, Vector* color) (thiscall, RET 0Ch).
void* __fastcall kotor_fastcall_006a78c0(void* self, void*, void* manager, const void* font, const void* color) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                           // 006A78E6..006A78ED
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_dialog_top());                                                     // 006A7902 (00755790)
    void* text_label = label(self, 0x64, u);                                             // 006A78F2..006A7908
    I32 margin_x = 0, margin_y = 0;                                                      // [esp+1Ch], [esp+18h]
    kotor_fastcall_00409ee0(lp(self, 0x18), nullptr, &margin_x, &margin_y);              // 006A790D..006A7922
    void* gm = lp(self, 0x18);                                                           // 006A792F (ebp)
    sw(self, 4, static_cast<U32>(margin_x));                                             // 006A7932
    sw(self, 8, static_cast<U32>(margin_y));                                             // 006A7935
    sw(self, 0xC, static_cast<U32>(static_cast<I32>(static_cast<std::int16_t>(lh(gm, 0x6C))) - 2 * margin_x)); // 006A7938..006A7940
    const I32 t = static_cast<I32>(static_cast<double>(static_cast<std::int16_t>(lh(gm, 0x6C))) * bits_f(0x3EDB6DB9u)); // 006A7943..006A7955 [00755788]
    const I32 diff = static_cast<I32>(static_cast<std::int16_t>(lh(gm, 0x6E))) - t;     // 006A795A..006A7960
    sw(self, 0x10, static_cast<U32>(diff / 2 - margin_y - 5));                           // 006A7962..006A796C (cdq; sub; sar = trunc /2)
    {
        U32 ext[4]; std::memcpy(ext, at(self, 0x68), 16);                                // 006A796F..006A7990
        ext[2] = lw(self, 0xC);                                                          // 006A7999
        vcall<void, const void*>(text_label, 0x04, ext);                                 // 006A7994..006A799D SetExtent
    }
    std::memcpy(at(self, 0x50), dialog_top_color_007a239c, 12);                          // 006A79A0..006A79C1
    {
        ExoString empty("");                                                             // 006A79B8..006A79C4 (0073D71C)
        unsigned char text_params[0x58];                                                 // [esp+80h]
        kotor_fastcall_00417120(text_params, nullptr, empty.storage, font, color, 0x22, 0x3F800000u); // 006A79C9..006A79F4
        const float white[3] = {1.0f, 1.0f, 1.0f};                                       // [esp+30h]
        const U32 extent[4] = {0, 0, lw(self, 0xC), lw(self, 0x10)};                     // [esp+3Ch]
        unsigned char ref_fill[16], ref_edge[16], ref_corner[16], border[0x54];          // [esp+50h] [esp+70h] [esp+60h] [esp+D4h]
        c2da_default_ref_00405ed0(ref_fill, nullptr);                                    // 006A7A4B
        c2da_default_ref_00405ed0(ref_edge, nullptr);                                    // 006A7A58
        c2da_default_ref_00405ed0(ref_corner, nullptr);                                  // 006A7A65
        gui_load_border_params_construct_00415000(border, nullptr, ref_corner, ref_edge, ref_fill, 0, 0, 1.0f, white, 0); // 006A7A6B..006A7A72
        vcall<void, const void*, void*, void*, float>(text_label, 0x9C, extent, text_params, border, 1.0f);       // 006A7A77..006A7A80 Initialize
        frame_LogStringDtor_005e5c20(text_params, nullptr);                              // 006A7A86..006A7A95 (TextParams CExoString)
    }                                                                                    // 006A7A9A..006A7AA6 ~empty
    gui_m48_panel_add_control_0040b9a0(self, nullptr, text_label);                       // 006A7AAB..006A7AAE
    sw(self, 0x44, lw(self, 0x44) | 1u);                                                 // 006A7AB3..006A7AC0
    u.dismiss();
    return self;
}
// 006A8AF0 CSWGuiDialogTop::~CSWGuiDialogTop (complete); 006A8AD0 scalar deleting.
void __fastcall kotor_fastcall_006a8af0(void* self, void*) {
    gui_load_label_destruct_00419d10(at(self, 0x64), nullptr);                           // 006A8B0D..006A8B18
    gui_load_panel_destruct_0040cf70(self, nullptr);                                     // 006A8B1D..006A8B27
}
void* __fastcall kotor_fastcall_006a8ad0(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006a8af0, flags); }

// ---------------------------------------------------------------------- CSWGuiDialogCinematic (table 00755800, 0x1E00)
// 006A7CA0 CSWGuiDialogCinematic::SetExtent(CSWGuiExtent*) (thiscall, RET 4).
void __fastcall kotor_fastcall_006a7ca0(void* self, void*, const void* extent) {
    gui_load_panel_set_extent_0040a5a0(self, nullptr, extent);                           // 006A7CA3..006A7CAB
    U32 e[4];
    std::memcpy(e, at(self, 0x19C8), 16); e[2] = lw(self, 0xC);                          // 006A7CB0..006A7CDA
    vcall<void, const void*>(at(self, 0x19C4), 0x04, e);                                 // 006A7CDE..006A7CE9 (reply list)
    std::memcpy(e, at(self, 0x1CA8), 16); e[2] = lw(self, 0xC);                          // 006A7CEC..006A7D16
    vcall<void, const void*>(at(self, 0x1CA4), 0x04, e);                                 // 006A7D1A..006A7D25 (message label)
}
void* __fastcall kotor_fastcall_006a8b40(void* self, void*, void* manager) {
    kotor_fastcall_006a85b0(self, nullptr, manager);                                     // 006A8B60..006A8B67
    Unwind u; u.push(&kotor_fastcall_006a6f30, self);
    set_vptr(self, vt_dialog_cinematic());                                               // 006A8B6C (00755800)
    std::memcpy(at(self, 0x1DE4), borrowed_exit_color_007a23b4, 12);                     // 006A8B72..006A8B8F
    start_layout(self, "dialog");                                                        // 006A8B94..006A8BAD (00755B00)
    init_control(self, 0x19C4, "LB_REPLIES");                                            // 006A8BB2..006A8BE2 (00755AF4)
    init_control(self, 0x1CA4, "LBL_MESSAGE");                                           // 006A8BE7..006A8C17 (00755AE8)
    stop_layout(self);                                                                   // 006A8C1C..006A8C1E
    {
        I32 mx = 0, my = 0;                                                              // [esp+64h], [esp+20h]
        kotor_fastcall_00409ee0(lp(self, 0x18), nullptr, &mx, &my);                      // 006A8C23..006A8C30
        void* gm = lp(self, 0x18);                                                       // 006A8C39 (edi)
        const I32 w = static_cast<std::int16_t>(lh(gm, 0x6C)), h = static_cast<std::int16_t>(lh(gm, 0x6E));
        const I32 t = static_cast<I32>(static_cast<double>(w) * bits_f(0xBEDB6DB9u));    // 006A8C4C..006A8C5E [00755AE4]
        const U32 extent[4] = {static_cast<U32>(mx), static_cast<U32>((h - t) / 2),       // 006A8C35..006A8C74
                               static_cast<U32>(w - 2 * mx), 0x64u};                      // 006A8C78..006A8C8C
        kotor_fastcall_006a7ca0(self, nullptr, extent);                                  // 006A8C89..006A8C94 (direct)
    }
    void* proto = lp(self, 0x1C90);                                                      // 006A8C99..006A8C9F
    for (U32 i = 0; i < 20; ++i) {                                                       // 006A8CA3..006A8D1A
        void* reply = at(self, 0x70 + i * 0x140);
        const I32 width = li(self, 0x19C4 + 0x294) - 2 * static_cast<I32>(lb(self, 0x19C4 + 0x2C0)); // 006A8CB0..006A8CC3
        const U32 extent[4] = {0, 0, static_cast<U32>(width), 0x14u};                    // 006A8CC3..006A8CE0
        label_initialize(reply, extent, proto, 1.0f);                                    // 006A8CCB..006A8CE8 (vslot 98h)
        add_event(self, 0x70 + i * 0x140, 0, self, fn(&kotor_fastcall_006a6fc0));         // 006A8CEE..006A8CF7 SetReplyActive
        add_event(self, 0x70 + i * 0x140, 1, self, fn(&kotor_fastcall_006a6fe0));         // 006A8CFC..006A8D06 SetReplyInactive
    }
    gui_load_text_params_set_color_00414e10(at(self, 0x1D8C), nullptr, at(self, 0x1DE4)); // 006A8D1C..006A8D29
    unsigned char font[16];                                                              // [esp+44h]
    c2da_default_ref_00405ed0(font, nullptr);                                            // 006A8D2E..006A8D32
    {
        ExoString base_font(reinterpret_cast<const char*>(at(self, 0x1DC8)));            // 006A8D37..006A8D42 (label text params base font)
        unsigned char result[16];                                                        // [esp+34h]
        async_ref_assign_string_004061f0(font, nullptr, result, base_font.storage);      // 006A8D47..006A8D5A
    }                                                                                    // 006A8D5F..006A8D67
    void* top = nullptr;
    if (void* memory = live_operator_new(0x1A4)) {                                       // 006A8D6C..006A8D84
        { auto w4_guard_0 = ::w4gui::on_throw([&]() { live_free(memory); });  top = kotor_fastcall_006a78c0(memory, nullptr, lp(self, 0x18), font, at(self, 0x1DE4)); w4_guard_0.armed = false; }
    }
    sw(self, 0x1DFC, ad(top));                                                           // 006A8D9F
    sw(self, 0x44, lw(self, 0x44) | 1u);                                                 // 006A8DA5
    u.dismiss();
    return self;
}
// 006A7C20 CSWGuiDialogCinematic::~CSWGuiDialogCinematic (complete); 006A8DC0 scalar deleting.
void __fastcall kotor_fastcall_006a7c20(void* self, void*) {
    set_vptr(self, vt_dialog_cinematic());                                               // 006A7C3D
    if (void* top = lp(self, 0x1DFC)) {                                                  // 006A7C43..006A7C53
        vcall<void*, U32>(top, 0x00, 1);                                                 // 006A7C55..006A7C59 delete
        zero(self, 0x1DFC);                                                              // 006A7C5B
    }
    kotor_fastcall_006a6f30(self, nullptr);                                              // 006A7C65..006A7C6F
}
void* __fastcall kotor_fastcall_006a8dc0(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006a7c20, flags); }

// ---------------------------------------------------------------------- CSWGuiDialogComputerInfo (3 labels, 0x3C0)
void* __fastcall kotor_fastcall_006a8050(void* self, void*) {                             // 006A8050
    Unwind u;
    label(self, 0, u); label(self, 0x140, u); label(self, 0x280, u);                      // 006A806D..006A8090
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_006a80b0(void* self, void*) {                             // 006A80B0
    gui_load_label_destruct_00419d10(at(self, 0x280), nullptr);
    gui_load_label_destruct_00419d10(at(self, 0x140), nullptr);
    gui_load_label_destruct_00419d10(self, nullptr);
}

// ---------------------------------------------------------------------- CSWGuiDialogComputer (table 00755888, 0x3620)
void* __fastcall kotor_fastcall_006a8eb0(void* self, void*, void* manager) {
    kotor_fastcall_006a85b0(self, nullptr, manager);                                     // 006A8ED4..006A8EDB
    Unwind u; u.push(&kotor_fastcall_006a6f30, self);
    set_vptr(self, vt_dialog_computer());                                                // 006A8F00 (00755888)
    vector_construct(at(self, 0x1DFC), 0x3C0, 4, &kotor_fastcall_006a8050, &kotor_fastcall_006a80b0); // 006A8EE0..006A8F06
    for (U32 i = 4; i-- > 0;) u.push(&kotor_fastcall_006a80b0, at(self, 0x1DFC + i * 0x3C0));
    listbox(self, 0x2CFC, u);                                                            // 006A8F0B..006A8F18
    vector_construct(at(self, 0x2FDC), 0x140, 4, &gui_load_label_construct_0041acd0, &gui_load_label_destruct_00419d10); // 006A8F1D..006A8F3A
    for (U32 i = 4; i-- > 0;) u.push(&gui_load_label_destruct_00419d10, at(self, 0x2FDC + i * 0x140));
    label(self, 0x34DC, u);                                                              // 006A8F3F..006A8F4A
    U32 scratch[2];                                                                      // [esp+20h] CExoString
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(scratch), nullptr); // 006A8F4F..006A8F58
    u.push(&frame_LogStringDtor_005e5c20, scratch);
    sw(self, 0x1DE4, 0x3F2B851Fu); sw(self, 0x1DE8, 0x3F0F5C29u); sw(self, 0x1DEC, 0x3F5EB852u); // 006A8F5D..006A8F94
    void* source = nullptr;
    if (void* memory = live_operator_new(8)) {                                           // 006A8F8E..006A8FAA
        { auto w4_guard_1 = ::w4gui::on_throw([&]() { live_free(memory); }); 
            menu_resources::Ref ref; ref.construct_00406d80("computer_lp_04");           // 006A8FAC..006A8FB5 (00755C14)
            source = kotor_sound_rev3::SourceConstruct005d60e0(memory, nullptr, &ref);   // 006A8FBA..006A8FC3
        w4_guard_1.armed = false; }
    }
    sw(self, 0x361C, ad(source));                                                        // 006A8FD2
    if (source != nullptr) {                                                             // 006A8FCC..006A8FD8
        kotor_m15_sound_source_set_looping_005d59d0(source, nullptr, 1);                 // 006A8FDA..006A8FDE
        kotor_fastcall_005d5950(lp(self, 0x361C), nullptr, 0x1E);                        // 006A8FE3..006A8FEB
    }
    start_layout(self, "computer");                                                      // 006A8FF0..006A9005 (00755C08)
    init_control(self, 0x2CFC, "LB_MESSAGE");                                            // 006A900A..006A9034 (0074FD84)
    init_control(self, 0x19C4, "LB_REPLIES");                                            // 006A9039..006A9069 (00755AF4)
    init_control(self, 0x34DC, "LBL_OBSCURE");                                           // 006A906E..006A909E (00755BFC)
    for (I32 i = 1; i <= 4; ++i) {                                                       // 006A90A3..006A90ED
        frame_LogFormat_005e5680(scratch, "LBL_STATIC%d", i);                            // 006A90B0..006A90C5 (00755BEC)
        void* static_label = at(self, 0x2FDC + static_cast<U32>(i - 1) * 0x140);
        gui_load_panel_init_control_0040b930(self, nullptr, static_label, scratch, 1);   // 006A90C8..006A90D5
        sw(static_label, 0x7C, 0x3ECCCCCDu);                                             // 006A90DE (0.4f)
    }
    init_control(self, 0x1F3C, "LBL_COMP_SKILL");                                        // 006A90EF..006A911F (00755BDC)
    init_control(self, 0x1DFC, "LBL_COMP_SKILL_ICON");                                   // 006A9124..006A9154 (00755BC8)
    init_control(self, 0x207C, "LBL_COMP_SKILL_VAL");                                    // 006A9159..006A9189 (00755BB4)
    init_control(self, 0x22FC, "LBL_REP_SKILL");                                         // 006A918E..006A91BE (00755BA4)
    init_control(self, 0x21BC, "LBL_REP_SKILL_ICON");                                    // 006A91C3..006A91F3 (00755B90)
    init_control(self, 0x243C, "LBL_REP_SKILL_VAL");                                     // 006A91F8..006A9228 (00755B7C)
    init_control(self, 0x26BC, "LBL_COMP_SPIKES");                                       // 006A922D..006A925D (00755B6C)
    init_control(self, 0x257C, "LBL_COMP_SPIKES_ICON");                                  // 006A9262..006A9292 (00755B54)
    init_control(self, 0x27FC, "LBL_COMP_SPIKES_VAL");                                   // 006A9297..006A92C7 (00755B40)
    init_control(self, 0x2A7C, "LBL_REP_UNITS");                                         // 006A92CC..006A92FC (00755B30)
    init_control(self, 0x293C, "LBL_REP_UNITS_ICON");                                    // 006A9301..006A9331 (00755B1C)
    init_control(self, 0x2BBC, "LBL_REP_UNITS_VAL");                                     // 006A9336..006A9366 (00755B08)
    stop_layout(self);                                                                   // 006A936B..006A936D
    for (U32 k = 0; k < 2; ++k) {                                                        // 006A9372..006A93E9
        U8* skill = static_cast<U8*>(lp(kotor_cell_rules_007a3a28, 0xB4)) + k * 0xA0;
        gui_load_text_params_set_strref_00415e50(at(self, k ? 0x23E4 : 0x2024), nullptr, lw(skill, 0)); // 006A937E / 006A93B7
        unsigned char ref[16];                                                           // [esp+28h]
        async_ref_construct_string_00406d60(ref, nullptr, skill + 0x10);                 // 006A938C..006A9394 / 006A93CF..006A93D7
        gui_load_border_params_set_fill_00414c00(at(self, k ? 0x222C : 0x1E6C), nullptr, ref, 0); // 006A9399..006A93A6 / 006A93DC..006A93E9
    }
    void* proto = lp(self, 0x1C90);                                                      // 006A93EE..006A93F4
    for (U32 i = 0; i < 20; ++i) {                                                       // 006A93F8..006A9465
        const I32 width = li(self, 0x1C58) - 2 * static_cast<I32>(lb(self, 0x1C84));     // 006A9400..006A9411
        const U32 extent[4] = {0, 0, static_cast<U32>(width), 0x14u};
        label_initialize(at(self, 0x70 + i * 0x140), extent, proto, 1.0f);               // 006A9419..006A943A
        add_event(self, 0x70 + i * 0x140, 0, self, fn(&kotor_fastcall_006a6fc0));         // 006A9440..006A944A
        add_event(self, 0x70 + i * 0x140, 1, self, fn(&kotor_fastcall_006a6fe0));         // 006A944F..006A9459
    }
    {
        void* msg_proto = lp(self, 0x2FC8);                                              // 006A9467
        label_initialize(at(self, 0x1CA4), static_cast<U8*>(msg_proto) + 4, msg_proto, 1.0f); // 006A946D..006A9483
    }
    gui_load_text_params_set_color_00414e10(at(self, 0x1D8C), nullptr, at(self, 0x1DE4)); // 006A9489..006A9496
    u.dismiss();
    frame_LogStringDtor_005e5c20(scratch, nullptr);                                      // 006A949B..006A94A4
    return self;
}
// 006A8110 CSWGuiDialogComputer::~CSWGuiDialogComputer (complete); 006A94C0 scalar deleting.
void __fastcall kotor_fastcall_006a8110(void* self, void*) {
    set_vptr(self, vt_dialog_computer());                                                // 006A812E
    if (void* source = lp(self, 0x361C)) {                                               // 006A8134..006A8144
        vcall<void*, U32>(source, 0x00, 1);                                              // 006A8146..006A814A
        zero(self, 0x361C);                                                              // 006A814C
    }
    gui_load_listbox_clear_items_00419a90(at(self, 0x2CFC), nullptr);                    // 006A8156..006A815E
    gui_load_label_destruct_00419d10(at(self, 0x34DC), nullptr);                         // 006A8163..006A816E
    vector_destruct(at(self, 0x2FDC), 0x140, 4, &gui_load_label_destruct_00419d10);      // 006A8173..006A818B
    gui_load_listbox_destruct_0041b060(at(self, 0x2CFC), nullptr);                       // 006A8197
    vector_destruct(at(self, 0x1DFC), 0x3C0, 4, &kotor_fastcall_006a80b0);               // 006A819C..006A81B4
    kotor_fastcall_006a6f30(self, nullptr);                                              // 006A81C3
}
void* __fastcall kotor_fastcall_006a94c0(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006a8110, flags); }

// ---------------------------------------------------------------------- CSWGuiDialogComputerCamera (table 00755958, 0x2080)
void* __fastcall kotor_fastcall_006a95f0(void* self, void*, void* manager) {
    kotor_fastcall_006a85b0(self, nullptr, manager);                                     // 006A960E..006A9615
    Unwind u; u.push(&kotor_fastcall_006a6f30, self);
    set_vptr(self, vt_dialog_computer_camera());                                         // 006A9628 (00755958)
    label(self, 0x1DFC, u);                                                              // 006A961A..006A962E
    void* back = label(self, 0x1F3C, u);                                                 // 006A9633..006A9640
    set_vptr(back, vt_blackened_label());                                                // 006A9645 (00753CC0)
    zero(self, 0x207C);                                                                  // 006A9659
    start_layout(self, "computercamera");                                                // 006A964B..006A966F (00755C30)
    {
        U32 e[4]; std::memcpy(e, at(self, 4), 16);                                       // 006A9674..006A9694
        e[2] = kotor_renderer_gl_cells::width_0078d1d4;                                  // 006A9698..006A96A8
        e[3] = kotor_menu_window_cells::height_0078d1d8;                                 // 006A968A..006A96A4
        gui_load_panel_set_extent_0040a5a0(self, nullptr, e);                            // 006A969D..006A96AC (direct)
    }
    init_control(self, 0x1F3C, "LBL_RETURN");                                            // 006A96B1..006A96DC (00755C24)
    {
        U32 e[4]; std::memcpy(e, at(self, 0x1F40), 16);                                  // 006A96E1..006A96F6
        const I32 w = static_cast<I32>(e[2]);
        e[0] = static_cast<U32>((static_cast<I32>(kotor_renderer_gl_cells::width_0078d1d4) - w) / 2); // 006A96FA..006A9719
        e[1] = static_cast<U32>(static_cast<I32>(kotor_menu_window_cells::height_0078d1d8) - 0x4B);   // 006A970E..006A9721
        vcall<void, const void*>(back, 0x04, e);                                         // 006A9725..006A9728 SetExtent
    }
    stop_layout(self);                                                                   // 006A972B..006A972D
    u.dismiss();
    return self;
}
// 006A8520 CSWGuiDialogComputerCamera::~CSWGuiDialogComputerCamera (complete); 006A9750 scalar deleting.
void __fastcall kotor_fastcall_006a8520(void* self, void*) {
    set_vptr(self, vt_dialog_computer_camera());                                         // 006A853D
    if (void* o = lp(self, 0x207C)) { vcall<void*, U32>(o, 0x00, 1); zero(self, 0x207C); }   // 006A8543..006A855B
    gui_load_label_destruct_00419d10(at(self, 0x1F3C), nullptr);                         // 006A8565..006A8570
    gui_load_label_destruct_00419d10(at(self, 0x1DFC), nullptr);                         // 006A8575..006A8580
    kotor_fastcall_006a6f30(self, nullptr);                                              // 006A8585..006A858F
}
void* __fastcall kotor_fastcall_006a9750(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006a8520, flags); }
}  // extern "C"
