// Lane W4 G15 (gui-journal): CSWGuiInGameGalaxyMap (table 00754910) and CSWGuiInGameOptions (table 00755DE0) panel
// slots whose W4 trap stubs are replaced, plus the callees they reach that had no rebuilt provider. Transcribed from
// swkotor.exe linear-sweep listings (claude-runs/tools/win.py <VA>); original PCs cited per statement.
//
//   006927D0 CSWGuiInGameGalaxyMap::OnPanelRemoved()                          thiscall RET 0   (slot 4Ch)
//   00693610 CSWGuiInGameGalaxyMap::Draw(float)                               thiscall RET 4   (slot 34h)
//   00694B40 CSWGuiInGameGalaxyMap::OnPanelAdded()                            thiscall RET 0   (slot 48h)
//   00695980 CSWGuiInGameGalaxyMap::HandleInputEvent(int, int)                thiscall RET 8   (slot 3Ch)
//   006AAAD0 CSWGuiInGameOptions::OnPanelRemoved()                            thiscall RET 0   (slot 4Ch)
//   006AAEC0 CSWGuiInGameOptions::HandleInputEvent(int, int)                  thiscall RET 8   (slot 3Ch)
//   006AB140 CSWGuiInGameOptions::OnPanelAdded()                              thiscall RET 0   (slot 48h)
// Callees reconstructed here (no rebuilt body existed in the c24 map):
//   00563B00 CSWPartyTable::GetPlanetSelectable(int)                          RET 4
//   00563B40 CSWPartyTable::GetPlanetAvailable(int)                           RET 4
//   00694BF0 CSWGuiInGameGalaxyMap::NextPlanet()                              RET 0
//   00694CA0 CSWGuiInGameGalaxyMap::PrevPlanet()                              RET 0
//   0062D1D0 CGuiInGame::HideGalaxyMapGui()                                   RET 0
#include "78d2073e39-gui_panels.hpp"
using namespace w4gui;

namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }   // CServerExoApp::GetPartyTable
namespace client_movies { int __fastcall SaveOptionsApp_005edf70(void* app, void* unused_edx); }  // CClientExoApp::SaveOptions
namespace frame_control {
extern std::uint32_t message_manager_007a3a00;                                                // [007A3A00] CVirtualMachine*
extern std::uint32_t sound_manager_007a39ec;                                                  // [007A39EC] CExoSound*
}

extern "C" {
void __fastcall kotor_panel_draw_0040B760(void* panel, void*, U32 delta_bits);               // CSWGuiPanel::Draw (float on stack)
void __fastcall gui_load_panel_handle_input_00409e60(void* panel, void*, U32 id, U32 down);   // CSWGuiPanel::HandleInputEvent
void __fastcall gui_load_panel_on_added_0040b870(void* panel, void*);                         // CSWGuiPanel::OnPanelAdded
void __fastcall gui_load_panel_on_removed_0040c170(void* panel, void*);                       // CSWGuiPanel::OnPanelRemoved
void __fastcall kotor_fastcall_0040a140(void* manager, void*, U32 sound);                     // CSWGuiManager::PlayGuiSound
void __fastcall frame_RemovePanel_0040c830(void* manager, void*, void* panel);                // CSWGuiManager::RemovePanel
int __fastcall frame_R4_0062cba0(void* in_game, void*, unsigned keep_paused);                 // CGuiInGame::HideSWInGameGui
void __fastcall kotor_fastcall_005eda60(void* app, void*, U32 input_class, U32 show);         // CClientExoApp::SetInputClass
std::uint32_t __fastcall kotor_fastcall_005edc10(void* app, void*);                           // CClientExoApp::GetPausedByCombat
void __fastcall kotor_fastcall_005edc20(void* app, void*, std::uint32_t, std::uint32_t, std::uint32_t); // SetPausedByCombat
void __fastcall frame_R4_005d0fc0(void* vm, void*, unsigned* script, unsigned object, unsigned flag); // CVirtualMachine::RunScript
void __fastcall kotor_fastcall_00563b60(void* table, void*, I32 planet);                      // CSWPartyTable::SetSelectedPlanet
void __fastcall kotor_fastcall_006933a0(void* self, void*, I32 planet, U32 animate);          // CSWGuiInGameGalaxyMap::DisplayPlanet
// ---- defined here
void __fastcall kotor_fastcall_006927d0(void* self, void*);
void __fastcall kotor_fastcall_00693610(void* self, void*, std::uint32_t delta_bits);
void __fastcall kotor_fastcall_00694b40(void* self, void*);
void __fastcall kotor_fastcall_00695980(void* self, void*, std::uint32_t id, std::uint32_t down);
void __fastcall kotor_fastcall_006aaad0(void* self, void*);
void __fastcall kotor_fastcall_006aaec0(void* self, void*, std::uint32_t id, std::uint32_t down);
void __fastcall kotor_fastcall_006ab140(void* self, void*);
U32 __fastcall kotor_fastcall_00563b00(void* table, void*, U32 planet);
U32 __fastcall kotor_fastcall_00563b40(void* table, void*, U32 planet);
void __fastcall kotor_fastcall_00694bf0(void* self, void*);
void __fastcall kotor_fastcall_00694ca0(void* self, void*);
void __fastcall kotor_fastcall_0062d1d0(void* in_game, void*);
}

namespace {
void* in_game_gui() { return pt(frame_R4_005ed690(client_app(), nullptr)); }               // [[7A39FC]+4]->GetInGameGui
void* party_table() { return lane_config_required::GetPartyTable_004aee70(server_app(), nullptr); }
void* planet_button(void* self, I32 planet) { return at(self, 0x64 + static_cast<U32>(planet) * 0x1C4u); }
void set_button_selected(void* button, U32 selected) { vcall<void, U32>(button, 0x40, selected); } // CSWGuiControl slot 40h

struct Exo {
    U32 s[2]; bool live = false;
    Exo() = default;
    Exo(const Exo&) = delete;
    Exo& operator=(const Exo&) = delete;
    void text(const char* t) { frame_LogStringCtor_005e5a90(s, nullptr, t); live = true; }
    void kill() { if (live) { live = false; frame_LogStringDtor_005e5c20(s, nullptr); } }
    ~Exo() { kill(); }
};
}  // namespace

extern "C" {
// ======================= CSWPartyTable =======================
// 00563B00 GetPlanetSelectable(int): planet < 10h (unsigned) ? [+A0 + planet*4] : 0.
U32 __fastcall kotor_fastcall_00563b00(void* table, void*, U32 planet) {
    if (planet >= 0x10) return 0;                                                              // 00563B04..00563B07 (jae)
    return lw(table, 0xA0 + planet * 4u);                                                      // 00563B09
}
// 00563B40 GetPlanetAvailable(int): planet < 10h (unsigned) ? [+60 + planet*4] : 0.
U32 __fastcall kotor_fastcall_00563b40(void* table, void*, U32 planet) {
    if (planet >= 0x10) return 0;                                                              // 00563B44..00563B47 (jae)
    return lw(table, 0x60 + planet * 4u);                                                      // 00563B49
}

// ======================= CGuiInGame =======================
// 0062D1D0 HideGalaxyMapGui: when the galaxy map is up (+108): unpause combat pause, GUI status 4, input class 0,
// remove the panel (+80; deleted unless [007A2370] keeps single panels), close sound 5 and restore the sound mode.
void __fastcall kotor_fastcall_0062d1d0(void* in_game, void*) {
    if (lw(in_game, 0x108) == 0) return;                                                       // 0062D1D3..0062D1DB
    if (kotor_fastcall_005edc10(client_app(), nullptr) != 0)                                   // 0062D1E1..0062D1F0
        kotor_fastcall_005edc20(client_app(), nullptr, 0, 0, 0);                               // 0062D1F2..0062D201
    frame_R4_0062aa00(in_game, nullptr, 4, 1);                                                 // 0062D206..0062D20C SetSWGuiStatus
    kotor_fastcall_005eda60(client_app(), nullptr, 0, 1);                                      // 0062D211..0062D21E
    frame_RemovePanel_0040c830(lp(in_game, 0x38), nullptr, lp(in_game, 0x80));                 // 0062D223..0062D22D
    if (w4::ingame_gui_single_panel_007a2370 == 0) {                                           // 0062D232..0062D239
        if (void* panel = lp(in_game, 0x80)) vcall<void*, U32>(panel, 0x00, 1);                // 0062D23B..0062D249
        sw(in_game, 0x80, 0);                                                                  // 0062D24B
    }
    kotor_fastcall_0040a140(lp(in_game, 0x38), nullptr, 5);                                   // 0062D255..0062D25A
    frame_R4_005d5e80(pt(frame_control::sound_manager_007a39ec), nullptr, kotor_rdata_sound_mode_0074c5ec); // 0062D25F..0062D26B
}

// ======================= CSWGuiInGameGalaxyMap =======================
// 006927D0 OnPanelRemoved: deselect all 16 planet buttons, forget the displayed planet (+254C = -1), base.
void __fastcall kotor_fastcall_006927d0(void* self, void*) {
    for (I32 i = 0; i < 0x10; ++i) set_button_selected(planet_button(self, i), 0);             // 006927D5..006927F0
    sw(self, 0x254C, 0xFFFFFFFFu);                                                             // 006927F4
    gui_load_panel_on_removed_0040c170(self, nullptr);                                         // 006927FE..00692801
}

// 00693610 Draw(float): keep the selected planet's button selected, then CSWGuiPanel::Draw.
void __fastcall kotor_fastcall_00693610(void* self, void*, std::uint32_t delta_bits) {
    const I32 selected = li(party_table(), 0xE0);                                              // 00693610..00693620
    if (selected != -1) set_button_selected(planet_button(self, selected), 1);                 // 00693626..0069363B
    kotor_panel_draw_0040B760(self, nullptr, delta_bits);                                      // 0069363E..00693645
}

// 00694B40 OnPanelAdded: select the first available + selectable planet when none is selected, show it, mark the
// available planets' buttons (bit 1 of [+44]), then CSWGuiPanel::OnPanelAdded.
void __fastcall kotor_fastcall_00694b40(void* self, void*) {
    void* table = party_table();                                                               // 00694B40..00694B53 (ebx)
    if (li(table, 0xE0) == -1) {                                                               // 00694B55..00694B5C
        for (U32 i = 0; i < 0x10; ++i) {                                                       // 00694B5E..00694B7C
            if (kotor_fastcall_00563b40(table, nullptr, i) == 0) continue;                     // 00694B60..00694B6A
            if (kotor_fastcall_00563b00(table, nullptr, i) == 0) continue;                     // 00694B6C..00694B76
            kotor_fastcall_00563b60(table, nullptr, static_cast<I32>(i));                      // 00694B80..00694B83
            break;
        }
    }
    const I32 selected = li(table, 0xE0);                                                      // 00694B88
    if (selected != -1) set_button_selected(planet_button(self, selected), 1);                 // 00694B8E..00694BA3
    kotor_fastcall_006933a0(self, nullptr, li(table, 0xE0), 1);                                // 00694BA6..00694BB1
    for (U32 i = 0; i < 0x10; ++i) {                                                           // 00694BB6..00694BE0
        const U32 available = kotor_fastcall_00563b40(table, nullptr, i);                      // 00694BC0..00694BC3
        void* button = planet_button(self, static_cast<I32>(i));
        const U32 flags = lw(button, 0x44);                                                    // 00694BCB
        sw(button, 0x44, flags ^ (((available + available) ^ flags) & 2u));                    // 00694BC8..00694BD4
    }
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 00694BE4..00694BE8
}

// 00694BF0 NextPlanet: deselect the current planet, advance (wrapping 10h -> 0) to the next available + selectable
// planet, stopping when the scan returns to the current one ([+E0] re-read each step), reselect and display it.
void __fastcall kotor_fastcall_00694bf0(void* self, void*) {
    void* table = party_table();                                                               // 00694BF0..00694C01 (edi)
    const I32 current = li(table, 0xE0);                                                       // 00694C03
    if (current == -1) return;                                                                 // 00694C09..00694C0C
    set_button_selected(planet_button(self, current), 0);                                      // 00694C12..00694C23
    I32 planet = li(table, 0xE0) + 1;                                                          // 00694C26..00694C2C
    if (planet != li(table, 0xE0)) {                                                           // 00694C2F..00694C31
        do {
            if (planet >= 0x10) planet = 0;                                                    // 00694C33..00694C38
            if (kotor_fastcall_00563b40(table, nullptr, static_cast<U32>(planet)) != 0 &&      // 00694C3A..00694C44
                kotor_fastcall_00563b00(table, nullptr, static_cast<U32>(planet)) != 0) {      // 00694C46..00694C50
                kotor_fastcall_00563b60(table, nullptr, planet);                               // 00694C5F..00694C62
                break;
            }
            ++planet;                                                                          // 00694C58
        } while (planet != li(table, 0xE0));                                                   // 00694C52..00694C5B
    }
    set_button_selected(planet_button(self, li(table, 0xE0)), 1);                              // 00694C67..00694C7D
    kotor_fastcall_006933a0(self, nullptr, li(table, 0xE0), 1);                                // 00694C80..00694C8B
}

// 00694CA0 PrevPlanet: as NextPlanet, scanning backwards (wrapping -1 -> 0Fh).
void __fastcall kotor_fastcall_00694ca0(void* self, void*) {
    void* table = party_table();                                                               // 00694CA0..00694CB1 (edi)
    const I32 current = li(table, 0xE0);                                                       // 00694CB3
    if (current == -1) return;                                                                 // 00694CB9..00694CBC
    set_button_selected(planet_button(self, current), 0);                                      // 00694CC2..00694CD3
    I32 planet = li(table, 0xE0) - 1;                                                          // 00694CD6..00694CDC
    if (planet != li(table, 0xE0)) {                                                           // 00694CDF..00694CE1
        do {
            if (planet < 0) planet = 0x0F;                                                     // 00694CE3..00694CE7
            if (kotor_fastcall_00563b40(table, nullptr, static_cast<U32>(planet)) != 0 &&      // 00694CEC..00694CF6
                kotor_fastcall_00563b00(table, nullptr, static_cast<U32>(planet)) != 0) {      // 00694CF8..00694D02
                kotor_fastcall_00563b60(table, nullptr, planet);                               // 00694D11..00694D14
                break;
            }
            --planet;                                                                          // 00694D0A
        } while (planet != li(table, 0xE0));                                                   // 00694D04..00694D0D
    }
    set_button_selected(planet_button(self, li(table, 0xE0)), 1);                              // 00694D19..00694D2F
    kotor_fastcall_006933a0(self, nullptr, li(table, 0xE0), 1);                                // 00694D32..00694D3D
}

// 00695980 HandleInputEvent(int id, int down): CSWGuiPanel::HandleInputEvent first; then (down only) jump table
// 00695AF8 / index bytes 00695B0C (id - 27h, 0..B8h):
//   27h/2Dh -> run "k_sup_galaxymap" (when a VM exists; [CGuiInGame+C04] = 1 around it), hide the galaxy map;
//   28h/2Eh/DFh -> hide the galaxy map;  2Fh/31h/3Dh/3Fh -> PrevPlanet;  30h/32h/3Eh/40h -> NextPlanet.
void __fastcall kotor_fastcall_00695980(void* self, void*, std::uint32_t id, std::uint32_t down) {
    gui_load_panel_handle_input_00409e60(self, nullptr, id, down);                             // 0069599F..006959A7
    if (down == 0) return;                                                                     // 006959AC..006959AE
    switch (id) {                                                                              // 006959B4..006959C9
    case 0x27: case 0x2D:                                                                      // 006959D0
        kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 0);                                   // 006959D0..006959D5
        if (frame_control::message_manager_007a3a00 != 0) {                                    // 006959DA..006959E1
            sw(in_game_gui(), 0xC04, 1);                                                       // 006959E3..006959FA
            Exo script; script.text("k_sup_galaxymap");                                        // 006959F1..00695A04 (0075497C)
            frame_R4_005d0fc0(pt(frame_control::message_manager_007a3a00), nullptr, script.s, 0x7F000000u, 0); // 00695A09..00695A23
            sw(in_game_gui(), 0xC04, 0);                                                       // 00695A28..00695A39
            script.kill();                                                                     // 00695A35..00695A4B
        }
        kotor_fastcall_0062d1d0(in_game_gui(), nullptr);                                       // 00695A50..00695A60
        return;
    case 0x28: case 0x2E: case 0xDF:                                                           // 00695A79
        kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 0);                                   // 00695A79..00695A7E
        kotor_fastcall_0062d1d0(in_game_gui(), nullptr);                                       // 00695A83..00695A93
        return;
    case 0x2F: case 0x31: case 0x3D: case 0x3F:                                                // 00695AAC
        kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 1);                                   // 00695AAC..00695AB1
        kotor_fastcall_00694ca0(self, nullptr);                                                // 00695AB6..00695AB8
        return;
    case 0x30: case 0x32: case 0x3E: case 0x40:                                                // 00695AD1
        kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 1);                                   // 00695AD1..00695AD6
        kotor_fastcall_00694bf0(self, nullptr);                                                // 00695ADB..00695ADD
        return;
    default:
        return;
    }
}

// ======================= CSWGuiInGameOptions =======================
// 006AAAD0 OnPanelRemoved: this->SetActiveControl(0, 0) (slot 08h), then CSWGuiPanel::OnPanelRemoved.
void __fastcall kotor_fastcall_006aaad0(void* self, void*) {
    vcall<void, void*, U32>(self, 0x08, nullptr, 0);                                           // 006AAAD3..006AAAD9
    gui_load_panel_on_removed_0040c170(self, nullptr);                                         // 006AAADC..006AAADF
}

// 006AAEC0 HandleInputEvent(int id, int down). Jump table 006AAF34 / index bytes 006AAF40 (id - 28h, 0..B7h):
//   28h/2Dh/2Eh/DFh -> HideSWInGameGui(0) (input class 0 when hidden), then SaveOptions;  35h/36h -> SaveOptions.
// Every path ends in CSWGuiPanel::HandleInputEvent(id, down).
void __fastcall kotor_fastcall_006aaec0(void* self, void*, std::uint32_t id, std::uint32_t down) {
    if (down != 0) {                                                                           // 006AAEC7..006AAECF
        switch (id) {                                                                          // 006AAED1..006AAEE2
        case 0x28: case 0x2D: case 0x2E: case 0xDF:                                            // 006AAEE9
            if (frame_R4_0062cba0(in_game_gui(), nullptr, 0) != 0)                             // 006AAEE9..006AAF02
                kotor_fastcall_005eda60(client_app(), nullptr, 0, 1);                          // 006AAF04..006AAF11
            client_movies::SaveOptionsApp_005edf70(client_app(), nullptr);                     // 006AAF16..006AAF1E (falls through)
            break;
        case 0x35: case 0x36:                                                                  // 006AAF16
            client_movies::SaveOptionsApp_005edf70(client_app(), nullptr);                     // 006AAF16..006AAF1E
            break;
        default:
            break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, id, down);                             // 006AAF23..006AAF27
}

// 006AB140 OnPanelAdded: CSWGuiPanel::OnPanelAdded; when the manager's current control ([+18]+8) is active (slot
// 58h) and is one of the 8 option buttons, make it this panel's active control (slot 08h); then, when the panel has
// no active control (slot 44h == 0), activate the first button.
void __fastcall kotor_fastcall_006ab140(void* self, void*) {
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 006AB144
    void* control = lp(lp(self, 0x18), 8);                                                     // 006AB149..006AB14C
    if (control != nullptr && vcall<U32>(control, 0x58) != 0) {                                // 006AB14F..006AB15C
        I32 i = 0;                                                                             // 006AB15E
        for (; i < 8; ++i)                                                                     // 006AB160..006AB171
            if (control == planet_button(self, i)) break;
        if (i < 8 && i >= 0)                                                                   // 006AB175..006AB17C
            vcall<void, void*, U32>(self, 0x08, planet_button(self, i), 0);                    // 006AB17E..006AB18F
    }
    if (vcall<U32>(self, 0x44) == 0)                                                           // 006AB192..006AB19B
        vcall<void, void*, U32>(self, 0x08, at(self, 0x64), 0);                                // 006AB19D..006AB1A7
}
}  // extern "C"
