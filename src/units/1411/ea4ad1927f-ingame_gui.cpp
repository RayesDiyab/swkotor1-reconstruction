// Lane W4 G15: CGuiInGame in-game GUI show/hide/pause (P3 domain: in_game+108h set by CGuiInGame::Initialize) and
// the small leaves they reach. Replaces the menu-premise traps of frame_menu_path_client.cpp (0062DEF0, 0062CBA0)
// and m48t input-frame (0062C9B0, 005F4BE0) under the SAME exported symbols (see integration/hud.json).
// Transcribed from dis/hud_p3.asm, 0062B1E0, 005ED6F0, 005EDB70, 0040A140, 006A9BB0, 006A9C10, 00624BD0.
#include "33eaec5f22-hud_providers.hpp"
using namespace w4;

namespace kotor_input_cells { extern void* volatile gui_manager_007a39f4; }  // ?gui_manager_007a39f4@kotor_input_cells@@3RAXA

extern "C" {
// existing runtime1115 providers
void* __fastcall kotor_m15e_client_get_mini_game_005edb00(void* app, void*);                    // CClientExoApp::GetMiniGame
void __fastcall kotor_fastcall_00624a00(void* panel, void*, std::uint32_t);                      // CSWGuiPanel::setToDestroy
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, std::uint32_t, std::uint32_t); // CSWGuiManager::AddPanel
void __fastcall kotor_fastcall_0040bd20(void* manager, void*, void* panel);                      // CSWGuiManager::SendPanelToBack
void __fastcall frame_RemovePanel_0040c830(void* manager, void*, void* panel);                   // CSWGuiManager::RemovePanel
std::uint32_t __fastcall frame_TogglePauseRequest_00677800(void* message, void*);               // SendPlayerToServerInput_TogglePauseRequest
void __fastcall kotor_fastcall_005f2c60(void* client, void*, void* object, std::uint32_t);      // CClientExoAppInternal::IndicateObject
void __fastcall frame_LogStringCtor_005e5a90(void* self, void*, const char* text);              // CExoString::CExoString(const char*)
void __fastcall frame_R4_005d0fc0(void* vm, void*, unsigned* script, unsigned object, unsigned flag); // CVirtualMachine::RunScript
void* __fastcall kotor_fastcall_00635460(void* party, void*);                                   // CSWParty::GetPlayerCharacter
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);       // CSWCCreature::GetServerCreature
void* __fastcall kotor_fastcall_005edeb0(void* app, void*, void* out_string, std::uint32_t strref); // CClientExoApp::GetGUIString (RET 8)
void __fastcall gui_load_text_params_set_text_00415e00(void* params, void*, const void* text); // CSWGuiTextParams::SetText (RET 4)
// cross-lane pending (see integration/hud.json): trap stubs in w4_pending_traps.cpp until the owner links
int __fastcall kotor_fastcall_005a6810(void* stats, void*);                                     // CSWSCreatureStats::CanLevelUp (server, W1/W5)
std::uint32_t __fastcall kotor_fastcall_005d5930(void* source, void*);                          // CExoSoundSource::Play (sound owner)
void __fastcall kotor_fastcall_005d5b20(void* source, void*);                                   // CExoStreamingSoundSource::Pause (sound owner)
}
namespace menu_tables_required { void __fastcall StreamResume_005d5b30(void*, void*); }       // CExoStreamingSoundSource::Resume

namespace w4 {
// [007A2370] (.data, initial 1) and [007B92AC] (.bss, 0): the original image only READS these cells (11 / 2 readers,
// no writer, see receipts). Owned here as read-only cells with the original initial values.
extern const volatile U32 ingame_gui_single_panel_007a2370 = 1;
extern const volatile U32 ingame_gui_never_set_007b92ac = 0;
}

extern "C" {
// 006C00C0 CSWGuiInGamePause::SetPauseReason(BYTE reason) (thiscall, RET 4; EH frame 00733E18 owns the CExoString).
// Layout (Ghidra CSWGuiInGamePause): +0 CSWGuiPanel, +64h LBL_PAUSEREASON (CSWGuiLabel; +D0h CSWGuiText, +E8h its
// CSWGuiTextParams), +1A4h LBL_PRESS, +2E4h BTN_UNPAUSE. Text vtable +8 = CSWGuiText::GetIdealHeight (00414EB0);
// label/button/panel vtable +4 = SetExtent(CSWGuiExtent*). Jump table 006C02BC decoded (index = reason-1):
// 1 -> BC54h (press label hidden), 5 -> A5C0h, 7 -> A5F2h, 8 -> A5F1h, 9 -> A59Dh, 10 -> BD27h, 11 -> BFDEh (hidden),
// 2/3/4/6 and out-of-range -> 5E4h. Extents are passed by address; every field the original re-reads from the stack
// after a SetExtent call is re-read here from the same buffer.
struct PauseReasonStringGuard {                                                                // EH state 0 (006C0150)
    U32* text;
    ~PauseReasonStringGuard() { frame_LogStringDtor_005e5c20(text, nullptr); }                  // 006C0293..006C029F
};
void __fastcall kotor_fastcall_006c00c0(void* self, void*, std::uint32_t reason_word) {
    U8* const p = static_cast<U8*>(self);
    U32 strref;
    U32 show_press = 1;                                                                         // 006C00E9 ebp = 1
    switch (static_cast<U8>(reason_word & 0xFF)) {                                              // 006C00CE..006C00F0
    case 1: strref = 0xBC54u; show_press = 0; break;                                            // 006C00F7..006C00FE
    case 5: strref = 0xA5C0u; break;                                                            // 006C0100
    case 7: strref = 0xA5F2u; break;                                                            // 006C0107
    case 8: strref = 0xA5F1u; break;                                                            // 006C010E
    case 9: strref = 0xA59Du; break;                                                            // 006C0115
    case 10: strref = 0xBD27u; break;                                                           // 006C011C
    case 11: strref = 0xBFDEu; show_press = 0; break;                                           // 006C0123..006C0128
    default: strref = 0x5E4u; break;                                                            // 006C012C
    }
    U32 text[2];                                                                                // [esp+10h] CExoString
    kotor_fastcall_005edeb0(client_app(), nullptr, text, strref);                               // 006C0131..006C0140 GetGUIString
    PauseReasonStringGuard guard{text};
    gui_load_text_params_set_text_00415e00(p + 0x14C, nullptr, text);                          // 006C0145..006C0154
    U32 extent[4] = {0, 0, 0, 0};                                                               // [esp+18h] (zeroed 006C0169..006C0171)
    U32 button_extent[4] = {0, 0, 0, 0};                                                        // [esp+28h] (zeroed 006C0175..006C0181)
    const U32 reason_height = vcall<U32>(p + 0x134, 0x08);                                      // 006C0159..006C0188 GetIdealHeight
    std::memcpy(extent, p + 0x68, 16);                                                          // 006C018A..006C01A7 label extent
    extent[3] = reason_height;                                                                  // 006C01B3
    vcall<void, void*>(p + 0x64, 0x04, extent);                                                 // 006C01AB..006C01B7 SetExtent
    std::memcpy(button_extent, extent, 16);                                                     // 006C01BC..006C01DC (re-read)
    const U32 press_top = extent[1] + reason_height + 2u;                                       // 006C01D0 ebx = y + height + 2
    U32 last_top = extent[1];                                                                   // 006C01C0 eax
    if (show_press != 0) {                                                                      // 006C01BA/006C01E0
        sw(p, 0x1E8, lw(p, 0x1E8) | 2u);                                                        // 006C01E2..006C01F7 LBL_PRESS visible
        const U32 press_height = vcall<U32>(p + 0x274, 0x08);                                   // 006C01F1..006C01FC GetIdealHeight
        std::memcpy(extent, p + 0x1A8, 16);                                                     // 006C01FF..006C0218
        extent[3] = press_height;                                                               // 006C021C
        extent[1] = press_top;                                                                  // 006C022D
        vcall<void, void*>(p + 0x1A4, 0x04, extent);                                            // 006C0220..006C0231 SetExtent
        last_top = extent[1];                                                                   // 006C0234 (re-read)
    } else {
        sw(p, 0x1E8, lw(p, 0x1E8) & 0xFFFFFFFDu);                                               // 006C023A LBL_PRESS hidden
    }
    U32 panel_extent[4];                                                                        // [esp+38h]
    std::memcpy(panel_extent, p + 4, 16);                                                       // 006C0241..006C0265
    panel_extent[3] = last_top + extent[3] + 5u;                                                // 006C025B..006C0269 ([esp+24h] re-read)
    vcall<void, void*>(self, 0x04, panel_extent);                                               // 006C026D..006C0274 panel SetExtent
    button_extent[3] = panel_extent[3];                                                         // 006C0277..006C0281 (re-read)
    vcall<void, void*>(p + 0x2E4, 0x04, button_extent);                                         // 006C027B..006C0290 BTN_UNPAUSE SetExtent
}                                                                                               // 006C0293..006C02B6 ~CExoString, RET 4

// 005ED6F0 CClientExoApp::GetSWCMessage() (thiscall, RET 0).
void* __fastcall kotor_fastcall_005ed6f0(void* app, void*) { return lp(lp(app, 4), 0x14C); }   // 005ED6F0..005ED6F3

// 005EDB70 CClientExoApp::IndicateObject(obj, int) (thiscall: mov ecx,[ecx+4]; jmp 005F2C60).
void __fastcall kotor_fastcall_005edb70(void* app, void*, void* object, std::uint32_t flag) {
    kotor_fastcall_005f2c60(lp(app, 4), nullptr, object, flag);
}

// 0040A140 CSWGuiManager::PlayGuiSound(BYTE) (thiscall, RET 4).
void __fastcall kotor_fastcall_0040a140(void* manager, void*, std::uint32_t sound) {
    const std::int8_t id = static_cast<std::int8_t>(sound & 0xFF);                             // 0040A140
    if (static_cast<U8>(id) == 0xFF) return;                                                    // 0040A144..0040A147
    if (!(id < static_cast<std::int8_t>(lb(manager, 0xA4)))) return;                            // 0040A149..0040A14F (signed)
    void* table = lp(manager, 0xA0);                                                            // 0040A151
    if (table == nullptr) return;                                                               // 0040A157..0040A159
    void* source = lp(table, static_cast<U32>(static_cast<I32>(id)) * 4u);                      // 0040A15B..0040A15E
    if (source != nullptr) (void)kotor_fastcall_005d5930(source, nullptr);                      // 0040A161..0040A167
}

// 006A9BB0 CSWGuiBarkBubble::Pause() (thiscall, RET 0).
void __fastcall kotor_fastcall_006a9bb0(void* bubble, void*) {
    if (jcc_je(lf(bubble, 0x1B4), 0.0, 0x41) || lw(bubble, 0x1B8) != 0) {                      // 006A9BB3..006A9BCE
        if (void* stream = lp(bubble, 0x1B8)) kotor_fastcall_005d5b20(stream, nullptr);         // 006A9BD0..006A9BDA
        U32 flags = lw(bubble, 0x44) & 0xFFFFFEFFu;                                             // 006A9BDF..006A9BE2
        const bool destroying = (flags & 0x600u) == 0x400u;                                     // 006A9BE7..006A9BEF
        sw(bubble, 0x44, flags);                                                                // 006A9BF5
        if (!destroying) sw(bubble, 0x44, (flags & 0xFFFFFBFFu) | 0x200u);                      // 006A9BFA..006A9C04
    }
}

// 006A9C10 CSWGuiBarkBubble::Resume() (thiscall, RET 0).
void __fastcall kotor_fastcall_006a9c10(void* bubble, void*) {
    if (jcc_je(lf(bubble, 0x1B4), 0.0, 0x41) || lw(bubble, 0x1B8) != 0) {                      // 006A9C13..006A9C2E
        if (void* stream = lp(bubble, 0x1B8)) menu_tables_required::StreamResume_005d5b30(stream, nullptr); // 006A9C30..006A9C3A
        kotor_fastcall_0040bc70(kotor_fastcall_005eda40(client_app(), nullptr), nullptr, bubble, 4, 1); // 006A9C3F..006A9C53
    }
}

// 00624BD0 CSWGuiInGameMenu::SetActiveControlID(int index, int arg) (thiscall, RET 8).
void __fastcall kotor_fastcall_00624bd0(void* menu, void*, std::int32_t index, std::uint32_t arg) {
    if (index < 0 || index >= li(menu, 0x24)) return;                                           // 00624BD0..00624BDB
    vcall<void, void*, std::uint32_t>(menu, 0x08, lp(lp(menu, 0x20), static_cast<U32>(index) * 4u), arg); // 00624BDD..00624BEC
}

extern "C" void __fastcall kotor_fastcall_0062b1e0(void*, void*, std::int32_t, std::int32_t);

// 0062DEF0 CGuiInGame::SetPaused(int paused, BYTE reason) (thiscall, RET 8).
void __fastcall frame_SetPaused_0062def0(void* in_game, void*, unsigned paused, unsigned reason_word) {
    if (lw(in_game, 0x108) == 0) return;                                                        // 0062DEF3..0062DEFB
    void* pause_panel = lp(in_game, 0x7C);
    if (paused == 0) {                                                                          // 0062DF06..0062DF08 -> 0062DFD3
        if (pause_panel != nullptr && frame_ContainsPanel_0040bd70(lp(in_game, 0x38), nullptr, pause_panel) != 0) // 0062DFD3..0062DFE5
            kotor_fastcall_00624a00(lp(in_game, 0x7C), nullptr, 0);                             // 0062DFE7..0062DFEC
        sw(in_game, 0xB38, paused);                                                             // 0062DFF1
        sw(in_game, 0xB3C, 0);                                                                  // 0062DFF7
        return;
    }
    const U8 reason = static_cast<U8>(reason_word & 0xFF);                                      // 0062DF0E..0062DF12
    U32 shown = 0;                                                                              // edi



    static const U8 map[11] = {0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0};                                // 0062E010

    const U32 slot = static_cast<U32>(reason) - 1u;                                             // 0062DF18
    if (slot <= 10 && map[slot] == 0) {                                                         // 0062DF19..0062DF29 -> 0062DF30
        shown = 1;                                                                              // 0062DF35
        if (lp(in_game, 0x7C) != nullptr) kotor_fastcall_006c00c0(lp(in_game, 0x7C), nullptr, reason_word); // 0062DF30..0062DF3D
        if (lw(in_game, 0x34) != 1 && kotor_m15e_client_get_mini_game_005edb00(client_app(), nullptr) == nullptr) { // 0062DF42..0062DF57
            void* panel = lp(in_game, 0x7C);                                                    // 0062DF59
            if (panel != nullptr && frame_ContainsPanel_0040bd70(lp(in_game, 0x38), nullptr, panel) != 0) { // 0062DF5C..0062DF6B
                kotor_fastcall_00624a00(lp(in_game, 0x7C), nullptr, 0);                         // 0062DF6D..0062DF72
            }
        } else {
            void* panel = lp(in_game, 0x7C);                                                    // 0062DF89
            if (panel != nullptr && frame_ContainsPanel_0040bd70(lp(in_game, 0x38), nullptr, panel) == 0 &&
                ingame_gui_never_set_007b92ac == 0) {                                           // 0062DF8C..0062DFA4
                kotor_fastcall_0040bc70(lp(in_game, 0x38), nullptr, lp(in_game, 0x7C), 4, shown); // 0062DFA6..0062DFB0
                kotor_fastcall_0040bd20(lp(in_game, 0x38), nullptr, lp(in_game, 0x7C));         // 0062DFB5..0062DFBC
            }
        }
    }
    sw(in_game, 0xB3C, shown);                                                                  // 0062DF77 / 0062DFC1
    sw(in_game, 0xB38, paused);                                                                 // 0062DF7E / 0062DFC8
}

// 0062CBA0 CGuiInGame::HideSWInGameGui(int) (thiscall, RET 4; returns 0/1).
int __fastcall frame_R4_0062cba0(void* in_game, void*, unsigned keep_paused) {
    if (lw(in_game, 0x108) == 0) return 0;                                                      // 0062CBA3..0062CBAC
    void* manager = kotor_fastcall_005eda40(client_app(), nullptr);                             // 0062CBB2..0062CBBA
    if (manager == nullptr || lw(manager, 0x98) != 0) return 0;                                 // 0062CBBF..0062CBD1
    frame_R4_0062aa00(in_game, nullptr, 4, 1);                                                  // 0062CBD7..0062CBDD
    frame_RemovePanel_0040c830(manager, nullptr, lp(in_game, 8));                               // 0062CBE2..0062CBE8
    if (ingame_gui_single_panel_007a2370 != 0)                                                  // 0062CBED..0062CBF4
        frame_RemovePanel_0040c830(manager, nullptr, lp(in_game, 0xC + lw(in_game, 0x2C) * 4u)); // 0062CBF6..0062CC00
    sw(in_game, 0x30, 0);                                                                       // 0062CC0D
    kotor_fastcall_0062b1e0(in_game, nullptr, li(in_game, 0x2C), -1);                           // 0062CC05..0062CC14
    bool destroy = false;
    if (lw(in_game, 0xB38) != 0) {                                                              // 0062CC19..0062CC21
        if (keep_paused != 0) {                                                                 // 0062CC23..0062CC29
            destroy = true;                                                                     // -> 0062CC7F
        } else if (ingame_gui_never_set_007b92ac == 0 &&
                   frame_ContainsPanel_0040bd70(lp(in_game, 0x38), nullptr, lp(in_game, 0x7C)) == 0) { // 0062CC2B..0062CC42
            kotor_fastcall_0040bc70(lp(in_game, 0x38), nullptr, lp(in_game, 0x7C), 4, 1);      // 0062CC44..0062CC4F
            kotor_fastcall_0040bd20(lp(in_game, 0x38), nullptr, lp(in_game, 0x7C));             // 0062CC54..0062CC5B
        }
    } else {
        (void)frame_TogglePauseRequest_00677800(kotor_fastcall_005ed6f0(client_app(), nullptr), nullptr); // 0062CC62..0062CC72
        destroy = keep_paused != 0;                                                             // 0062CC77..0062CC7D
    }
    if (destroy && frame_ContainsPanel_0040bd70(lp(in_game, 0x38), nullptr, lp(in_game, 0x7C)) != 0) // 0062CC7F..0062CC8D
        kotor_fastcall_00624a00(lp(in_game, 0x7C), nullptr, 0);                                 // 0062CC8F..0062CC94
    kotor_fastcall_006a9c10(lp(in_game, 0x4C), nullptr);                                        // 0062CC99..0062CC9C
    kotor_fastcall_0040a140(lp(in_game, 0x38), nullptr, 5);                                     // 0062CCA1..0062CCA6
    frame_R4_005d5e80(pt(frame_control::sound_manager_007a39ec), nullptr, kotor_rdata_sound_mode_0074c5ec); // 0062CCAB..0062CCB8
    return 1;                                                                                   // 0062CCBE
}
}  // extern "C"

extern "C" const std::uint32_t kotor_rdata_sound_mode_0074c5fc;   // _kotor_rdata_sound_mode_0074c5fc (.rdata 4)

// 0062C9B0 CGuiInGame::ShowSWInGameGui(int panel) (thiscall, RET 4; returns in_game+30h).
extern "C" std::uint32_t __fastcall kotor_fastcall_0062c9b0(void* in_game, void*, std::int32_t panel) {
    if (lw(in_game, 0x108) == 0) return lw(in_game, 0x30);                                      // 0062C9CB..0062C9D4 -> 0062CB7D
    void* manager = kotor_fastcall_005eda40(client_app(), nullptr);                             // 0062C9DA..0062C9E2
    if (manager == nullptr) return lw(in_game, 0x30);                                           // 0062C9E9..0062C9EB
    frame_R4_005d5e80(pt(frame_control::sound_manager_007a39ec), nullptr, kotor_rdata_sound_mode_0074c5fc); // 0062C9F1..0062C9FE
    kotor_fastcall_005edb70(client_app(), nullptr, nullptr, 0);                                 // 0062CA03..0062CA10
    frame_R4_0062aa00(in_game, nullptr, 3, 1);                                                  // 0062CA15..0062CA1B
    U32 script[2];                                                                              // [esp+10h] CExoString
    frame_LogStringCtor_005e5a90(script, nullptr, "k_sup_guiopen");                             // 0062CA20..0062CA29 (007513BC)
    frame_R4_005d0fc0(pt(frame_control::message_manager_007a3a00), nullptr, script, 0, 0);      // 0062CA2E..0062CA45
    if (panel >= 0 && panel < 8) {                                                              // 0062CA4A..0062CA55
        sw(in_game, 0x2C, static_cast<U32>(panel));                                             // 0062CA57
    } else if (kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr) != nullptr) { // 0062CA5C..0062CA73
        void* character = kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr); // 0062CA75..0062CA85
        void* server = kotor_m15a_creature_get_server_creature_0060fb20(character, nullptr);   // 0062CA8A..0062CA8C
        if (kotor_fastcall_005a6810(lp(server, 0xA74), nullptr) != 0) sw(in_game, 0x2C, 2);    // 0062CA91..0062CAA0
    }
    kotor_fastcall_0062b1e0(in_game, nullptr, -1, li(in_game, 0x2C));                           // 0062CAA7..0062CAAF
    kotor_fastcall_0040bc70(manager, nullptr, lp(in_game, 8), 0, 1);                            // 0062CAB4..0062CABE
    kotor_fastcall_0040bc70(manager, nullptr, lp(in_game, 0xC + lw(in_game, 0x2C) * 4u), 2, 1); // 0062CAC3..0062CAD1
    kotor_fastcall_0040bd20(manager, nullptr, lp(in_game, 0xC + lw(in_game, 0x2C) * 4u));       // 0062CAD6..0062CAE0
    kotor_fastcall_00624bd0(lp(in_game, 8), nullptr, li(in_game, 0x2C), 0);                     // 0062CAE5..0062CAEE
    const U32 paused = lw(in_game, 0xB38);                                                      // 0062CAF3
    sw(in_game, 0x30, 1);                                                                       // 0062CAFB
    if (paused == 0)                                                                            // 0062CAF9..0062CB02
        (void)frame_TogglePauseRequest_00677800(kotor_fastcall_005ed6f0(client_app(), nullptr), nullptr); // 0062CB04..0062CB14
    kotor_fastcall_006a9bb0(lp(in_game, 0x4C), nullptr);                                        // 0062CB19..0062CB1C
    if (frame_control::message_manager_007a3a00 != 0) {                                         // 0062CB21..0062CB28
        U32 change[2];                                                                          // [esp+8]
        frame_LogStringCtor_005e5a90(change, nullptr, "k_pend_screenchg");                      // 0062CB2A..0062CB33 (007513A8)
        frame_R4_005d0fc0(pt(frame_control::message_manager_007a3a00), nullptr, change, 0x7F000000u, 0); // 0062CB38..0062CB4F
        frame_LogStringDtor_005e5c20(change, nullptr);                                          // 0062CB54..0062CB5D
    }
    kotor_fastcall_0040a140(lp(in_game, 0x38), nullptr, 4);                                     // 0062CB62..0062CB67
    frame_LogStringDtor_005e5c20(script, nullptr);                                              // 0062CB6C..0062CB78
    return lw(in_game, 0x30);                                                                   // 0062CB7D
}

// 006856E0 CSWGuiMainInterface::DisplayToolTipMessage() (thiscall, RET 0).
extern "C" void __fastcall kotor_fastcall_006856e0(void* mi, void*) {
    sw(mi, 0x73A0, lw(mi, 0x73A0) & 0xFFFFFFFDu);                                               // 006856E0
    sw(mi, 0x7620, lw(mi, 0x7620) | 2u);                                                        // 006856E7..006856F4
    sw(mi, 0x74E0, lw(mi, 0x74E0) | 2u);                                                        // 006856FA
    sw(mi, 0x771C, 1);                                                                          // 00685700
}

// 00685710 CSWGuiMainInterface::RemoveToolTipMessage() (thiscall, RET 0).
extern "C" void __fastcall kotor_fastcall_00685710(void* mi, void*) {
    sw(mi, 0x7620, lw(mi, 0x7620) & 0xFFFFFFFDu);                                               // 00685710
    if (lb(mi, 0xA45C) & 2) sw(mi, 0x73A0, lw(mi, 0x73A0) | 2u);                                // 00685717..00685726
    const U32 a = lw(mi, 0x73A0), b = lw(mi, 0x74E0);                                           // 0068572C..00685733
    sw(mi, 0x74E0, b ^ ((a ^ b) & 2u));                                                         // 00685739..00685741
    sw(mi, 0x771C, 0);                                                                          // 00685747
}

// 005F4BE0 CClientExoAppInternal::SetToolTipInstantMode(int) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_005f4be0(void* client, void*, std::uint32_t instant) {
    if (lw(client, 0x40) == 0) return;                                                          // 005F4BE3..005F4BE8
    void* tooltip_manager = kotor_input_cells::gui_manager_007a39f4;
    if (instant != 0) {                                                                         // 005F4BEA..005F4BF0
        sw(tooltip_manager, 0x48, 0);                                                           // 005F4BF2..005F4BF7
        if (void* mi = lp(lp(client, 0x40), 0x90)) kotor_fastcall_006856e0(mi, nullptr);       // 005F4BFE..005F4C0B
        return;
    }
    void* options = kotor_fastcall_005ed700(client_app(), nullptr);                             // 005F4C14..005F4C1D
    sf(kotor_input_cells::gui_manager_007a39f4, 0x48,
       static_cast<float>(static_cast<double>(lf(options, 0x0C))));                              // 005F4C22..005F4C2A fld/fstp dword
    if (void* mi = lp(lp(client, 0x40), 0x90)) kotor_fastcall_00685710(mi, nullptr);           // 005F4C2D..005F4C3A
}

// 005D5B20 CExoStreamingSoundSource::Pause() (thiscall RET 0; W4-owned per t1 review): mov ecx,[ecx]; test; je ret;
// jmp 005DC3C0 CExoStreamingSoundSourceInternal::Pause (existing provider sound_pause29_required::stream_pause_005dc3c0).
namespace sound_pause29_required { unsigned __fastcall stream_pause_005dc3c0(void* internal, void*); }
extern "C" void __fastcall kotor_fastcall_005d5b20(void* source, void*) {
    void* internal = w4::lp(source, 0);                                                      // 005D5B20
    if (internal == nullptr) return;                                                         // 005D5B22..005D5B24
    (void)sound_pause29_required::stream_pause_005dc3c0(internal, nullptr);                  // 005D5B26 tail jmp
}
