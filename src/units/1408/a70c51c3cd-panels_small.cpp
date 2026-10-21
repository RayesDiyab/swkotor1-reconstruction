// Lane W4 G15: small in-game panels built by CGuiInGame::Initialize (ctor / dtor / scalar deleting dtor and the
// event callbacks their constructors register). Transcribed from dis/ctor_<ADDR>.asm and the dtor listings.
#include "37dfeb87ca-gui_panels.hpp"
using namespace w4gui;

namespace gui_load { struct Vector { float x, y, z; }; }
namespace kotor_gui_m48_cells { extern gui_load::Vector viewport_color_00833910; }   // [00833910] (m48b owner)
extern "C" {
void __fastcall kotor_fastcall_005d5b80(void* stream, void*);                        // CExoStreamingSoundSource::Stop
void __fastcall kotor_fastcall_005d6200(void* stream, void*);                        // ~CExoStreamingSoundSource
std::uint32_t __fastcall kotor_fastcall_005edc10(void* app, void*);                  // CClientExoApp::GetPausedByCombat
void __fastcall kotor_fastcall_005edc20(void* app, void*, std::uint32_t, std::uint32_t, std::uint32_t); // SetPausedByCombat
void __fastcall frame_SetAutoPaused_005f3f10(void* client, void*, unsigned, unsigned); // CClientExoAppInternal::SetAutoPaused
}


extern "C" {
// ---- CClientExoApp small wrappers used by the pause panel callback
// 005EDEF0 CClientExoApp::GetAutoPaused() (thiscall, RET 0).
U32 __fastcall kotor_fastcall_005edef0(void* app, void*) { return lw(lp(app, 4), 0x384) & 1u; }
// 005EDEE0 CClientExoApp::SetAutoPaused(int, BYTE): mov ecx,[ecx+4]; jmp 005F3F10.
void __fastcall kotor_fastcall_005edee0(void* app, void*, U32 a, U32 b) { frame_SetAutoPaused_005f3f10(lp(app, 4), nullptr, a, b); }

// ======================= 006A8930 CSWGuiDialogLetterbox (0xA4) =======================
void* __fastcall kotor_fastcall_006a8930(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                            // 006A894E..006A8953
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_letterbox());                                                       // 006A895A (00755A78)
    for (U32 off : {0x70u, 0x6Cu, 0x68u, 0x64u}) zero(self, off);                         // 006A8960..006A8969
    for (U32 off : {0x80u, 0x7Cu, 0x78u, 0x74u}) zero(self, off);                         // 006A8976..006A8982
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(static_cast<U8*>(self) + 0x8C), nullptr); // 006A896C..006A8985
    u.dismiss();
    zero(self, 0x84); zero(self, 0x88);                                                   // 006A898A..006A8990
    std::memcpy(static_cast<U8*>(self) + 0x50, &kotor_gui_m48_cells::viewport_color_00833910, 12); // 006A8996..006A89AF
    gui_load_panel_set_alpha_00404b00(self, nullptr, 1.0f);                               // 006A89B2..006A89B9
    for (U32 off : {0x94u, 0x98u, 0x9Cu, 0xA0u}) zero(self, off);                         // 006A89C2..006A89D4
    return self;
}
// 006A8A10 ~CSWGuiDialogLetterbox
void __fastcall kotor_fastcall_006a8a10(void* self, void*) {
    frame_LogStringDtor_005e5c20(static_cast<U8*>(self) + 0x8C, nullptr);                // 006A8A2D..006A8A3B
    gui_load_panel_destruct_0040cf70(self, nullptr);                                     // 006A8A40..006A8A4A
}
void* __fastcall kotor_fastcall_006a89f0(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006a8a10, flags); }

// ======================= 006A9770 CSWGuiBarkBubble (0x1CC) =======================
void* __fastcall kotor_fastcall_006a9770(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                            // 006A978F..006A9796
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_bark_bubble());                                                     // 006A97A6 (00755C60)
    label(self, 0x64, u);                                                                 // 006A979B..006A97AC
    for (U32 off : {0x1B0u, 0x1ACu, 0x1A8u, 0x1A4u}) zero(self, off);                     // 006A97BF..006A97D1
    start_layout(self, "barkbubble");                                                     // 006A97B1..006A97E3 (00755C50)
    init_control(self, 0x64, "LBL_BARKTEXT");                                             // 006A97E8..006A9813 (00755C40)
    stop_layout(self);                                                                    // 006A9818..006A981A
    u.dismiss();
    std::memcpy(static_cast<U8*>(self) + 0x1A4, static_cast<U8*>(self) + 4, 16);          // 006A981F..006A986B extent copy
    zero(self, 0x1B4); zero(self, 0x1B8); zero(self, 0x1C4); zero(self, 0x1BC);           // 006A9849..006A985E
    sw(self, 0x1C8, lw(self, 0x1C8) & 0xFFFFFFFEu);                                       // 006A9839..006A9864
    return self;
}
// 006A9890 ~CSWGuiBarkBubble
void __fastcall kotor_fastcall_006a9890(void* self, void*) {
    set_vptr(self, vt_bark_bubble());                                                     // 006A98AD
    if (void* stream = lp(self, 0x1B8)) {                                                 // 006A98B3..006A98C3
        kotor_fastcall_005d5b80(stream, nullptr);                                         // 006A98C6 Stop
        if (void* s = lp(self, 0x1B8)) { kotor_fastcall_005d6200(s, nullptr); live_free(s); } // 006A98CB..006A98E2
        zero(self, 0x1B8);                                                                // 006A98E5
    }
    gui_load_label_destruct_00419d10(static_cast<U8*>(self) + 0x64, nullptr);           // 006A98F0..006A98F8
    gui_load_panel_destruct_0040cf70(self, nullptr);                                     // 006A98FD..006A9907
}
void* __fastcall kotor_fastcall_006aa020(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006a9890, flags); }

// ======================= 006C7D50 CSWGuiInGameAreaTransition (0x438) =======================
void* __fastcall kotor_fastcall_006c7d50(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                            // 006C7D70..006C7D77
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_area_transition());                                                 // 006C7D89 (00757508)
    label(self, 0x64, u); label(self, 0x1A4, u); label(self, 0x2E4, u);                   // 006C7D7C..006C7DB3
    zero(self, 0x424); zero(self, 0x428); zero(self, 0x42C);                              // 006C7DC8..006C7DD4
    start_layout(self, "areatransition");                                                 // 006C7DBA..006C7DE6 (007574F4)
    init_control(self, 0x64, "LBL_TEXTBG");                                               // 006C7DEB..006C7E16 (007574E8)
    init_control(self, 0x1A4, "LBL_ICON");                                                // 006C7E1B..006C7E46 (007574DC)
    init_control(self, 0x2E4, "LBL_DESCRIPTION");                                         // 006C7E4B..006C7E76 (007574CC)
    stop_layout(self);                                                                    // 006C7E7B..006C7E7D
    u.dismiss();
    sw(self, 0x434, lw(self, 8));                                                         // 006C7E85..006C7E8F
    sw(self, 0x430, 0x7F000000u);                                                         // 006C7E99
    sw(self, 0x44, (lw(self, 0x44) & 0xFFFFFFBFu) | 0x20u);                               // 006C7E82..006C7EA3
    return self;
}
void __fastcall kotor_fastcall_006c8190(void* self, void*) {                              // ~CSWGuiInGameAreaTransition
    gui_load_label_destruct_00419d10(static_cast<U8*>(self) + 0x2E4, nullptr);           // 006C81AD..006C81BB
    gui_load_label_destruct_00419d10(static_cast<U8*>(self) + 0x1A4, nullptr);           // 006C81C0..006C81CB
    gui_load_label_destruct_00419d10(static_cast<U8*>(self) + 0x64, nullptr);            // 006C81D0..006C81D8
    gui_load_panel_destruct_0040cf70(self, nullptr);                                     // 006C81DD..006C81E7
}
void* __fastcall kotor_fastcall_006c8170(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006c8190, flags); }

// ======================= 006C03B0 CSWGuiInGamePause (0x4B0) =======================
// 006C0350 CSWGuiInGamePause::OnUnpauseButtonPressed(CSWGuiControl*) (thiscall PMF target, RET 4).
void __fastcall kotor_fastcall_006c0350(void*, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                   // 006C0350..006C0359
    const U32 paused = kotor_fastcall_005edc10(client_app(), nullptr);                    // 006C035B..006C0364
    kotor_fastcall_005edc20(client_app(), nullptr, paused == 0 ? 1u : 0u, 4, 0);          // 006C0369..006C037D
    if (kotor_fastcall_005edef0(client_app(), nullptr) == 0) return;                      // 006C0382..006C0392
    kotor_fastcall_005edee0(client_app(), nullptr, 0, 0);                                 // 006C0394..006C03A1
}
void* __fastcall kotor_fastcall_006c03b0(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                            // 006C03D0..006C03D7
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_pause());                                                           // 006C03E9 (00756DC8)
    label(self, 0x64, u); label(self, 0x1A4, u); button(self, 0x2E4, u);                  // 006C03DC..006C0413
    start_layout(self, "pause");                                                          // 006C0418..006C0432 (00756DBC)
    init_control(self, 0x64, "LBL_PAUSEREASON");                                          // 006C0437..006C0462 (00756DAC)
    init_control(self, 0x1A4, "LBL_PRESS");                                               // 006C0467..006C0492 (00756DA0)
    init_control(self, 0x2E4, "BTN_UNPAUSE");                                             // 006C0497..006C04C2 (00756D94)
    sw(self, 0x44, lw(self, 0x44) | 1u);                                                  // 006C04C7..006C04CF
    stop_layout(self);                                                                    // 006C04CD..006C04D2
    sw(self, 0x4A8, lw(self, 8));                                                         // 006C04D7..006C04E8
    sw(self, 0x4AC, lw(self, 4));                                                         // 006C04DA..006C04EE
    {
        U32 text[2];                                                                      // [esp+14h]
        WindowCopy005e5b00(text, nullptr, static_cast<U8*>(self) + 0x28C);               // 006C04DD..006C04F4 (label text)
        tlk_core::ParseStr0041dd30(static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08), nullptr,
                                   reinterpret_cast<kotor_reconstruction::StartupString*>(text));   // 006C04F9..006C0509
        gui_load_text_params_set_text_00415e00(static_cast<U8*>(self) + 0x28C, nullptr, text);       // 006C050E..006C0515
        sw(self, 0x184, lw(self, 0x184) | 0x40u);                                         // 006C051A..006C0527
        sw(self, 0x28C + 0x38, lw(self, 0x28C + 0x38) | 0x40u);                           // 006C052D
        sw(self, 0x2E4 + 0x44, lw(self, 0x2E4 + 0x44) & 0xFFFFFFFBu);                     // 006C0530..006C053C
        add_event(self, 0x2E4, 0x27, self, pmf_code(&kotor_m1_shared::PanelEventThunks::AcceptButtonCallback_00624ba0)); // 006C0533..006C0543
        add_event(self, 0x2E4, 0x27, self, fn(&kotor_fastcall_006c0350));                 // 006C0548..006C0552
        frame_LogStringDtor_005e5c20(text, nullptr);                                      // 006C0557..006C0560
    }
    u.dismiss();
    return self;
}
void __fastcall kotor_fastcall_006c05a0(void* self, void*) {                              // ~CSWGuiInGamePause
    gui_load_button_destruct_00419da0(static_cast<U8*>(self) + 0x2E4, nullptr);          // 006C05BD..006C05CB
    gui_load_label_destruct_00419d10(static_cast<U8*>(self) + 0x1A4, nullptr);           // 006C05D0..006C05DB
    gui_load_label_destruct_00419d10(static_cast<U8*>(self) + 0x64, nullptr);            // 006C05E0..006C05E8
    gui_load_panel_destruct_0040cf70(self, nullptr);                                     // 006C05ED..006C05F7
}
void* __fastcall kotor_fastcall_006c0580(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006c05a0, flags); }

// ======================= 006C2270 CSWGuiInGameSoloModeQuery (0xFC8) =======================
void* __fastcall kotor_fastcall_006c2270(void* self, void*, void* manager) {
    kotor_original::kotor_fastcall_00626df0(self, nullptr, manager);                      // 006C228B..006C2292
    Unwind u; u.push(&gui_m48_message_box_destruct_006270a0, self);
    set_vptr(self, vt_solo_mode_query());                                                 // 006C22A5 (00756F28)
    for (U32 off : {0x984u, 0xAC4u, 0xC04u, 0xD44u, 0xE84u}) label(self, off, u);         // 006C2297..006C22EB
    u.dismiss();
    zero(self, 0xFC4);                                                                    // 006C22F4
    return self;
}
void __fastcall kotor_fastcall_006c2560(void* self, void*) {                              // ~CSWGuiInGameSoloModeQuery
    for (U32 off : {0xE84u, 0xD44u, 0xC04u, 0xAC4u, 0x984u})                              // 006C257D..006C25CB
        gui_load_label_destruct_00419d10(static_cast<U8*>(self) + off, nullptr);
    gui_m48_message_box_destruct_006270a0(self, nullptr);                                // 006C25D0..006C25DA
}
void* __fastcall kotor_fastcall_006c2540(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006c2560, flags); }

// ======================= 006CE9C0 CSWGuiExamine (0x988) =======================
void* __fastcall kotor_fastcall_006ce9c0(void* self, void*, void* manager) {
    kotor_original::kotor_fastcall_00626df0(self, nullptr, manager);                      // 006CE9DB..006CE9E2
    Unwind u; u.push(&gui_m48_message_box_destruct_006270a0, self);
    set_vptr(self, vt_examine());                                                         // 006CE9FA (007579C8)
    gui_load_text_params_set_strref_00415e50(static_cast<U8*>(self) + 0x460, nullptr, 0x62E); // 006CE9E7..006CEA00
    kotor_fastcall_00627130(self, nullptr, 0);                                            // 006CEA05..006CEA09 SetAllowCancel(0)
    u.dismiss();
    zero(self, 0x984);                                                                    // 006CEA12
    return self;
}
}  // extern "C"
