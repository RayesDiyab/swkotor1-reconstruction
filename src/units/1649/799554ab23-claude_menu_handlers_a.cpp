// Lane W4 G15 (menus-a): in-game GUI event handlers registered during panel construction (thiscall(CSWGuiControl*)
// RET 4), plus the callees they reach that had no rebuilt provider. Transcribed from linear-sweep listings of
// swkotor.exe (claude-runs/tools/win.py <VA>); PCs cited per statement.
//
//   00624C00 CSWGuiInGameMenu::OnShoulderLeft          00624C30 CSWGuiInGameMenu::OnShoulderRight
//   00624CF0 OnEquipButtonPressed  00624D10 OnInvButtonPressed  00624D30 OnCharButtonPressed  00624D50 OnMapButtonPressed
//   00624D70 OnAbiButtonPressed    00624D90 OnJouButtonPressed  00624DB0 OnOptButtonPressed   00624DD0 OnMsgButtonPressed
//   00644730 CSWGuiInGameJournal::OnBlackButtonPressed
//   006927B0 CSWGuiInGameMap::OnUpArrowPressed          006927C0 CSWGuiInGameMap::OnDownArrowPressed
//   006935A0 CSWGuiInGameGalaxyMap::OnPlanetClicked
//   006AAAF0 CSWGuiInGameOptions::OnLoadGame  006AAB80 OnSaveGame  006AAC10 OnGameplay  006AAC90 OnFeedback
//   006AAD10 OnAutopause  006AAD90 OnGraphics  006AAE10 OnSound  006AB1B0 OnQuit
//   006BF2A0 CSWGuiPartySelection::OnToggled
// Callees reconstructed here (no rebuilt body existed):
//   0062CCD0 CGuiInGame::NextSWInGameGui()                          RET 0
//   0062CDF0 CGuiInGame::PrevSWInGameGui()                          RET 0
//   006AAE90 CSWGuiInGameOptions::ReallyQuit(int)                   RET 4 (message-box callback)
//   00563B60 CSWPartyTable::SetSelectedPlanet(int)                  RET 4
//   006933A0 CSWGuiInGameGalaxyMap::DisplayPlanet(int, int)         RET 8
//   00416710 CSWGuiScene::RemoveModel(int, int) -> int              RET 8
//   006BE370 CSWGuiPartySelectionButton::SetSelected(int)           RET 4
//   006BE4E0 CSWGuiPartySelection::UpdateCount()                    RET 0
// Not implemented here (missing-callee cascade beyond the lane budget): 006BF3B0 CSWGuiPartySelection::OnDone,
// 006C1B00 CSWGuiStore::OnControlXButton (see the lane report).
#include "78d2073e39-gui_panels.hpp"
using namespace w4gui;

namespace kotor_menu_10_19 {
struct String;
struct Tlk { String* __thiscall simple_0041e8f0(String* out, unsigned strref); };                 // CTlkTable::GetSimpleString
}
namespace kotor_input_required {
void __fastcall switch_to_sw_in_game_gui_0062cf10(void* in_game, void*, std::uint32_t index);      // CGuiInGame::SwitchToSWInGameGui
}
namespace client_movies { int __fastcall SaveOptionsApp_005edf70(void* app, void* unused_edx); }  // CClientExoApp::SaveOptions
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }   // CServerExoApp::GetPartyTable

extern "C" {
// ---- CSWGuiManager
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, U32, U32);              // CSWGuiManager::AddPanel
void __fastcall kotor_fastcall_0040bd20(void* manager, void*, void* panel);                        // CSWGuiManager::SendPanelToBack
void __fastcall frame_RemovePanel_0040c830(void* manager, void*, void* panel);                     // CSWGuiManager::RemovePanel
void __fastcall kotor_fastcall_0040a140(void* manager, void*, U32 sound);                          // CSWGuiManager::PlayGuiSound
// ---- CGuiInGame / CSWGuiInGameMenu
void __fastcall load_screen_update_created_in_game_gui_0062b1e0(void* in_game, void*, U32 old_index, U32 new_index); // CGuiInGame::UpdateCreatedInGameGUI
void __fastcall kotor_fastcall_00624bd0(void* menu, void*, std::int32_t index, U32 arg);           // CSWGuiInGameMenu::SetActiveControlID
void __fastcall frame_R4_005d0fc0(void* vm, void*, unsigned* script, unsigned object, unsigned flag); // CVirtualMachine::RunScript
// ---- message box
void __fastcall gui_load_messagebox_set_callback_00624a40(void* box, void*, void* object, const void* func, U32 extra); // CSWGuiMessageBox::SetCallback
void __fastcall kotor_fastcall_005ede10(void* app, void*, U32 pending, U32 b, U32 c);              // CClientExoApp::SetEndGamePending
// ---- options sub-panels (constructors: thiscall(manager[, ...]) return this)
void* __fastcall kotor_m1_saveload_construct_006cc680(void* self, void*, void* manager, U32 mode, U32 flag); // CSWGuiSaveLoad
void* __fastcall kotor_m1_opt_gameplay_construct_006e69a0(void* self, void*, void* manager);      // CSWGuiInGameGameplay
void* __fastcall kotor_m1_opt_feedback_construct_006e2a70(void* self, void*, void* manager);      // CSWGuiOptionsFeedback
void* __fastcall kotor_m1_opt_autopause_construct_006e76a0(void* self, void*, void* manager);     // CSWGuiInGameAutoPause
void* __fastcall kotor_m1_options_graphics_construct_006e2e70(void* self, void*, void* manager);  // CSWGuiOptionsGraphics
void* __fastcall kotor_m1_options_sound_construct_006e3550(void* self, void*, void* manager);     // CSWGuiOptionsSound
// ---- galaxy map
void __fastcall deleteAt_006857a0(void* list, void*, int index);                                  // CExoArrayList::deleteAt
U32 __fastcall gui_load_resref_is_valid_00406030(void* ref, void*);                              // CResRef::IsValid
void* __fastcall c2da_ref_to_string_00405f70(void* ref, void*, void* out);                       // CResRef::CopyToString
void* __fastcall aurora_gui_scene_add_model_name_00417620(void* scene, void*, void* name, I32 index); // CSWGuiScene::AddModel
extern volatile U32 kotor_cell_do3dgui_0078d1e4;                                                 // [0078D1E4]
// ---- party selection
void __fastcall gui_load_border_params_set_pulsing_00414d00(void*, void*, U32, U32, U32);         // CSWGuiBorderParams::SetPulsingAlpha
void __fastcall kotor_fastcall_006becc0(void* self, void*, I32 add);                              // CSWGuiPartySelection::SetAddRemoveLabel (claude_enter_handlers.cpp)
extern const unsigned char kotor_m15d_color_007a23e4[12];                                        // [007A23E4]
extern const unsigned char borrowed_enter_color_007a23c0[12];                                    // [007A23C0]
}

namespace {
void* in_game_gui() { return pt(frame_R4_005ed690(client_app(), nullptr)); }                     // [[7A39FC]+4]->GetInGameGui
// A stack CExoString destroyed at scope end (normally or while unwinding) once constructed.
struct Str {
    U32 s[2];
    bool live = false;
    Str() = default;
    Str(const Str&) = delete;
    Str& operator=(const Str&) = delete;
    ~Str() { if (live) frame_LogStringDtor_005e5c20(s, nullptr); }
    void born() { live = true; }
    void* p() { return s; }
};
// Shared tail of NextSWInGameGui / PrevSWInGameGui after the index update (0062CD5D..0062CDDA / 0062CE7C..0062CEF9).
void show_current(void* self, void* manager) {
    if (lw(self, 0x30) == 0 || manager == nullptr) return;                                     // 0062CD5D..0062CD66
    kotor_fastcall_0040bc70(manager, nullptr, lp(self, 0x0C + lw(self, 0x2C) * 4u), 2, 1);     // 0062CD68..0062CD76 AddPanel(p, 2, 1)
    kotor_fastcall_0040bd20(manager, nullptr, lp(self, 0x0C + lw(self, 0x2C) * 4u));           // 0062CD7B..0062CD85
    kotor_fastcall_00624bd0(lp(self, 8), nullptr, li(self, 0x2C), 1);                          // 0062CD8A..0062CD93
    if (frame_control::message_manager_007a3a00 != 0) {                                        // 0062CD98..0062CD9F
        ExoString script("k_pend_screenchg");                                                  // 0062CDA1..0062CDAA (007513A8)
        frame_R4_005d0fc0(pt(frame_control::message_manager_007a3a00), nullptr, script.storage, 0x7F000000u, 0); // 0062CDAF..0062CDC9
    }                                                                                          // 0062CDCE..0062CDDA ~CExoString
}
// CSWGuiInGameMenu::On*ButtonPressed: GetInGameGui()->SwitchToSWInGameGui(index).
void switch_to(U32 index) { kotor_input_required::switch_to_sw_in_game_gui_0062cf10(in_game_gui(), nullptr, index); }
// CSWGuiInGameOptions::On<Sub>: `new` sub-panel (block freed if the constructor throws, EH state 0 -> 00731860),
// then AddPanel(panel, 3, 1) on the options panel's manager (+18h). A null allocation still reaches AddPanel(0).
template <class Build> void open_sub_panel(void* self, U32 size, Build build) {
    void* block = live_operator_new(size);                                                     // operator new(size)
    void* panel = nullptr;
    if (block != nullptr) {
        auto guard = on_throw([&]() { live_free(block); });                                   // 00731860: free([ebp+4])
        panel = build(block);
        guard.armed = false;
    }
    kotor_fastcall_0040bc70(lp(self, 0x18), nullptr, panel, 3, 1);                             // AddPanel(panel, 3, 1)
}
void save_options() { (void)client_movies::SaveOptionsApp_005edf70(client_app(), nullptr); }    // CClientExoApp::SaveOptions
}

extern "C" {
// ======================= CGuiInGame panel cycling =======================
// 0062CCD0 CGuiInGame::NextSWInGameGui() (thiscall, RET 0).
void __fastcall kotor_fastcall_0062ccd0(void* self, void*) {
    if (lw(self, 0x108) == 0) return;                                                          // 0062CCEB..0062CCF3
    void* manager = kotor_fastcall_005eda40(client_app(), nullptr);                            // 0062CCF9..0062CD07
    if (w4::ingame_gui_single_panel_007a2370 != 0 && lw(self, 0x30) != 0 && manager != nullptr) // 0062CD09..0062CD1B
        frame_RemovePanel_0040c830(manager, nullptr, lp(self, 0x0C + lw(self, 0x2C) * 4u));   // 0062CD1D..0062CD27
    const U32 old_index = lw(self, 0x2C);                                                      // 0062CD2C (ecx, pushed at 0062CD55)
    const I32 next = static_cast<I32>(old_index) + 1;                                          // 0062CD2F
    sw(self, 0x2C, static_cast<U32>(next));                                                    // 0062CD35
    if (!(next < 8)) sw(self, 0x2C, 0);                                                        // 0062CD32..0062CD3A (signed jl)
    const U32 shown = (lw(self, 0x30) != 0 && manager != nullptr) ? lw(self, 0x2C) : 0xFFFFFFFFu; // 0062CD41..0062CD54
    load_screen_update_created_in_game_gui_0062b1e0(self, nullptr, old_index, shown);          // 0062CD54..0062CD58
    show_current(self, manager);                                                               // 0062CD5D..0062CDDA
}

// 0062CDF0 CGuiInGame::PrevSWInGameGui() (thiscall, RET 0).
void __fastcall kotor_fastcall_0062cdf0(void* self, void*) {
    if (lw(self, 0x108) == 0) return;                                                          // 0062CE0B..0062CE13
    void* manager = kotor_fastcall_005eda40(client_app(), nullptr);                            // 0062CE19..0062CE27
    if (w4::ingame_gui_single_panel_007a2370 != 0 && lw(self, 0x30) != 0 && manager != nullptr) // 0062CE29..0062CE3B
        frame_RemovePanel_0040c830(manager, nullptr, lp(self, 0x0C + lw(self, 0x2C) * 4u));   // 0062CE3D..0062CE47
    const U32 old_index = lw(self, 0x2C);                                                      // 0062CE4C
    const I32 prev = static_cast<I32>(old_index) - 1;                                          // 0062CE4F
    sw(self, 0x2C, static_cast<U32>(prev));                                                    // 0062CE54
    if (prev < 0) sw(self, 0x2C, 7);                                                           // 0062CE52..0062CE59 (signed jge)
    const U32 shown = (lw(self, 0x30) != 0 && manager != nullptr) ? lw(self, 0x2C) : 0xFFFFFFFFu; // 0062CE60..0062CE73
    load_screen_update_created_in_game_gui_0062b1e0(self, nullptr, old_index, shown);          // 0062CE73..0062CE77
    show_current(self, manager);                                                               // 0062CE7C..0062CEF9
}

// ======================= CSWGuiInGameMenu (thiscall(CSWGuiControl*) RET 4; `this` unused) =======================
// 00624C00 OnShoulderLeft
void __fastcall kotor_fastcall_00624c00(void*, void*, void* control) {
    if (lw(control, 0x4C) != 0) kotor_fastcall_0062cdf0(in_game_gui(), nullptr);              // 00624C00..00624C1B
}
// 00624C30 OnShoulderRight
void __fastcall kotor_fastcall_00624c30(void*, void*, void* control) {
    if (lw(control, 0x4C) != 0) kotor_fastcall_0062ccd0(in_game_gui(), nullptr);              // 00624C30..00624C4B
}
void __fastcall kotor_fastcall_00624cf0(void*, void*, void*) { switch_to(0); }                 // 00624CF0..00624D01 Equip
void __fastcall kotor_fastcall_00624d10(void*, void*, void*) { switch_to(1); }                 // 00624D10..00624D21 Inventory
void __fastcall kotor_fastcall_00624d30(void*, void*, void*) { switch_to(2); }                 // 00624D30..00624D41 Character
void __fastcall kotor_fastcall_00624d50(void*, void*, void*) { switch_to(6); }                 // 00624D50..00624D61 Map
void __fastcall kotor_fastcall_00624d70(void*, void*, void*) { switch_to(3); }                 // 00624D70..00624D81 Abilities
void __fastcall kotor_fastcall_00624d90(void*, void*, void*) { switch_to(5); }                 // 00624D90..00624DA1 Journal
void __fastcall kotor_fastcall_00624db0(void*, void*, void*) { switch_to(7); }                 // 00624DB0..00624DC1 Options
void __fastcall kotor_fastcall_00624dd0(void*, void*, void*) { switch_to(4); }                 // 00624DD0..00624DE1 Messages

// ======================= CSWGuiInGameJournal / CSWGuiInGameMap =======================
// 00644730 CSWGuiInGameJournal::OnBlackButtonPressed: tail jump to this->vtable[60h](control).
void __fastcall kotor_fastcall_00644730(void* self, void*, void* control) {
    vcall<void, void*>(self, 0x60, control);                                                   // 00644730..00644732
}
// 006927B0 CSWGuiInGameMap::OnUpArrowPressed: this->vtable[3Ch](31h, 1).
void __fastcall kotor_fastcall_006927b0(void* self, void*, void*) {
    (void)vcall<U32, U32, U32>(self, 0x3C, 0x31, 1);                                           // 006927B0..006927B6
}
// 006927C0 CSWGuiInGameMap::OnDownArrowPressed: this->vtable[3Ch](32h, 1).
void __fastcall kotor_fastcall_006927c0(void* self, void*, void*) {
    (void)vcall<U32, U32, U32>(self, 0x3C, 0x32, 1);                                           // 006927C0..006927C6
}

// ======================= CSWGuiInGameGalaxyMap =======================
// 00563B60 CSWPartyTable::SetSelectedPlanet(int planet) (thiscall, RET 4).
void __fastcall kotor_fastcall_00563b60(void* table, void*, I32 planet) {
    if (planet < 0 || planet >= 0x10) return;                                                  // 00563B64..00563B6B (signed)
    if (lw(table, 0x60 + static_cast<U32>(planet) * 4u) == 0) return;                          // 00563B6D..00563B73
    if (lw(table, 0xA0 + static_cast<U32>(planet) * 4u) == 0) return;                          // 00563B75..00563B7E
    sw(table, 0xE0, static_cast<U32>(planet));                                                 // 00563B80
}

// 00416710 CSWGuiScene::RemoveModel(int index, int destroy) -> int (thiscall, RET 8).
U32 __fastcall kotor_fastcall_00416710(void* scene, void*, I32 index, U32 destroy) {
    if (index < 0 || index >= li(scene, 0x20)) return 0;                                       // 00416710..0041671B -> 0041674A
    void* model = lp(lp(scene, 0x1C), static_cast<U32>(index) * 4u);                           // 0041671D..00416721
    deleteAt_006857a0(at(scene, 0x1C), nullptr, index);                                        // 00416724..00416728
    if (destroy != 0 && model != nullptr) vcall<void*, U32>(model, 0x00, 1);                   // 0041672D..0041673F scalar deleting dtor
    return 1;                                                                                  // 00416741
}

// 006933A0 CSWGuiInGameGalaxyMap::DisplayPlanet(int planet, int animate) (thiscall, RET 8).
// Planet records: this + planet*18h + 23CCh {strref name; strref description; CResRef model}.
void __fastcall kotor_fastcall_006933a0(void* self, void*, I32 planet, U32 animate) {
    Str name; exo_default(name.p()); name.born();                                              // 006933C3..006933C7 [esp+20] state 0
    Str description; exo_default(description.p()); description.born();                        // 006933CC..006933D8 [esp+18] state 1
    if (planet == -1 || static_cast<U32>(planet) == lw(self, 0x254C)) return;                  // 006933DD..006933F4
    U8* record = static_cast<U8*>(self) + static_cast<U32>(planet) * 0x18u;                    // 006933FA..006933FD (ebx)
    auto* tlk = static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08);
    if (const U32 strref = lw(record, 0x23CC)) {                                               // 00693400..00693408
        Str text;                                                                              // [esp+10] state 2
        kotor_menu_10_19::String* r = tlk->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(text.p()), strref); // 0069340A..00693416
        text.born();
        async_assign_string_005e5c50(name.p(), nullptr, r);                                    // 0069341B..00693425
    }                                                                                          // 0069342A..00693433 ~text
    if (const U32 strref = lw(record, 0x23D0)) {                                               // 00693438..00693444
        Str text;                                                                              // [esp+10] state 3
        kotor_menu_10_19::String* r = static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08)->simple_0041e8f0(
            reinterpret_cast<kotor_menu_10_19::String*>(text.p()), strref);                    // 00693446..00693452
        text.born();
        async_assign_string_005e5c50(description.p(), nullptr, r);                             // 00693457..00693461
    }                                                                                          // 00693466..0069346F ~text
    gui_load_text_params_set_text_00415e00(at(self, 0x1ECC), nullptr, description.p());        // 00693474..0069347F
    gui_load_text_params_set_text_00415e00(at(self, 0x1D8C), nullptr, name.p());               // 00693484..0069348F
    if (lw(self, 0x23B8) != 0) kotor_fastcall_00416710(at(self, 0x2398), nullptr, 0, 1);       // 00693494..006934A8 (unsigned jbe)
    void* model_ref = record + 0x23D4;                                                         // 006934AD
    if (gui_load_resref_is_valid_00406030(model_ref, nullptr) != 0) {                          // 006934B3..006934BC
        Str model_name; exo_default(model_name.p()); model_name.born();                        // 006934C2..006934D2 state 4
        c2da_ref_to_string_00405f70(model_ref, nullptr, model_name.p());                       // 006934CB..006934D7
        void* camera = lp(self, 0x23B0);                                                       // 006934E3 (read before the gate)
        if (kotor_cell_do3dgui_0078d1e4 != 0) {                                                // 006934DC..006934E9
            void* model = aurora_gui_scene_add_model_name_00417620(at(self, 0x2398), nullptr, model_name.p(), -1); // 006934EB..006934F8
            if (model != nullptr) {                                                            // 006934FD..00693501
                if (animate != 0) {                                                            // 00693503..00693508
                    vcall<void, const char*, float, U32, U32>(model, 0x18, "zoomin", 1.0f, 1, 0); // 0069350A..0069351C (007546F8)
                    vcall<void, const char*, float, U32, U32>(model, 0x18, "rotate", 1.0f, 0x20, 0); // 0069351F..00693531 (007546F0)
                }
                vcall<void, void*, const char*, U32>(camera, 0x74, model, "camerahook", 0);   // 00693534..00693540 (0074F42C)
                vcall<void, float>(camera, 0x44, bits_f(0x41B5CED9u));                         // 00693543..0069354C (22.726f)
            }
        }
    }                                                                                          // 00693552..0069355B ~model_name
    sw(self, 0x254C, static_cast<U32>(planet));                                                // 00693560
}                                                                                              // 00693566..00693580 ~description, ~name

// 006935A0 CSWGuiInGameGalaxyMap::OnPlanetClicked(CSWGuiControl*) (thiscall, RET 4).
void __fastcall kotor_fastcall_006935a0(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                        // 006935A5..006935AD
    void* table = lane_config_required::GetPartyTable_004aee70(server_app(), nullptr);         // 006935AF..006935BD
    const I32 selected = li(table, 0xE0);                                                      // 006935BF
    if (selected != -1) {                                                                      // 006935C5..006935C8
        if (static_cast<U32>(selected) == lw(control, 0x58)) return;                           // 006935CA..006935CD
        void* button = at(self, 0x64 + static_cast<U32>(selected) * 0x1C4u);                   // 006935D4..006935DE
        vcall<void, U32>(button, 0x40, 0);                                                     // 006935E2..006935E4
    }
    kotor_fastcall_00563b60(table, nullptr, li(control, 0x58));                                // 006935E7..006935ED
    kotor_fastcall_006933a0(self, nullptr, li(table, 0xE0), 1);                                // 006935F2..006935FD
}

// ======================= CSWGuiInGameOptions =======================
// 006AAAF0 OnLoadGame: SaveOptions; new CSWGuiSaveLoad(manager, 0, 0) (1160h).
void __fastcall kotor_fastcall_006aaaf0(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                        // 006AAB09..006AAB11
    save_options();                                                                            // 006AAB13..006AAB1C
    open_sub_panel(self, 0x1160, [&](void* p) {                                       // 006AAB21..006AAB61
        return kotor_m1_saveload_construct_006cc680(p, nullptr, lp(self, 0x18), 0, 0); });
}
// 006AAB80 OnSaveGame: SaveOptions; new CSWGuiSaveLoad(manager, 1, 0) (1160h).
void __fastcall kotor_fastcall_006aab80(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                        // 006AAB99..006AABA1
    save_options();                                                                            // 006AABA3..006AABAC
    open_sub_panel(self, 0x1160, [&](void* p) {                                       // 006AABB1..006AABF1
        return kotor_m1_saveload_construct_006cc680(p, nullptr, lp(self, 0x18), 1, 0); });
}
// 006AAC10 OnGameplay: new CSWGuiInGameGameplay(manager) (22A0h).
void __fastcall kotor_fastcall_006aac10(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                        // 006AAC29..006AAC31
    open_sub_panel(self, 0x22A0, [&](void* p) {                                       // 006AAC29..006AAC6F
        return kotor_m1_opt_gameplay_construct_006e69a0(p, nullptr, lp(self, 0x18)); });
}
// 006AAC90 OnFeedback: new CSWGuiOptionsFeedback(manager) (0C2Ch).
void __fastcall kotor_fastcall_006aac90(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                        // 006AACA9..006AACB1
    open_sub_panel(self, 0x0C2C, [&](void* p) {                                       // 006AACA9..006AACEF
        return kotor_m1_opt_feedback_construct_006e2a70(p, nullptr, lp(self, 0x18)); });
}
// 006AAD10 OnAutopause: new CSWGuiInGameAutoPause(manager) (1984h).
void __fastcall kotor_fastcall_006aad10(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                        // 006AAD29..006AAD31
    open_sub_panel(self, 0x1984, [&](void* p) {                                       // 006AAD29..006AAD6F
        return kotor_m1_opt_autopause_construct_006e76a0(p, nullptr, lp(self, 0x18)); });
}
// 006AAD90 OnGraphics: new CSWGuiOptionsGraphics(manager) (18C0h).
void __fastcall kotor_fastcall_006aad90(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                        // 006AADA9..006AADB1
    open_sub_panel(self, 0x18C0, [&](void* p) {                                       // 006AADA9..006AADEF
        return kotor_m1_options_graphics_construct_006e2e70(p, nullptr, lp(self, 0x18)); });
}
// 006AAE10 OnSound: new CSWGuiOptionsSound(manager) (16F0h).
void __fastcall kotor_fastcall_006aae10(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                        // 006AAE29..006AAE31
    open_sub_panel(self, 0x16F0, [&](void* p) {                                       // 006AAE29..006AAE6F
        return kotor_m1_options_sound_construct_006e3550(p, nullptr, lp(self, 0x18)); });
}
// 006AAE90 CSWGuiInGameOptions::ReallyQuit(int) (message-box callback, thiscall RET 4; `this` unused).
void __fastcall kotor_fastcall_006aae90(void*, void*, U32) {
    void* box = lp(in_game_gui(), 0x98);                                                       // 006AAE90..006AAE9D
    if ((lb(box, 0x64) & 1u) != 0)                                                             // 006AAEA3..006AAEA7
        kotor_fastcall_005ede10(client_app(), nullptr, 1, 0, 1);                               // 006AAEA9..006AAEB8 SetEndGamePending(1, 0, 1)
}
// 006AB1B0 OnQuit: SaveOptions; confirmation box (strref A56Ch) calling ReallyQuit.
void __fastcall kotor_fastcall_006ab1b0(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                        // 006AB1B7..006AB1BC
    save_options();                                                                            // 006AB1BE..006AB1C8
    void* box = lp(in_game_gui(), 0x98);                                                       // 006AB1CD..006AB1DB
    kotor_fastcall_00627130(box, nullptr, 1);                                                  // 006AB1E1..006AB1E5 SetAllowCancel(1)
    gui_load_messagebox_set_callback_00624a40(box, nullptr, self, fn(&kotor_fastcall_006aae90), 0); // 006AB1EA..006AB1F4
    vcall<void, U32>(box, 0x6C, 0xA56C);                                                       // 006AB1F9..006AB202
    kotor_fastcall_0040bc70(lp(self, 0x18), nullptr, box, 1, 1);                               // 006AB205..006AB20D AddPanel(box, 1, 1)
}

// ======================= CSWGuiPartySelection =======================
// 006BE370 CSWGuiPartySelectionButton::SetSelected(int selected) (thiscall, RET 4).
void __fastcall kotor_fastcall_006be370(void* button, void*, U32 selected) {
    sw(button, 0x1C4, selected);                                                               // 006BE37A
    if (selected != 0) {                                                                       // 006BE374..006BE382
        sw(button, 0x8C, 0x3F800000u);                                                         // 006BE384 (1.0f)
        std::memcpy(at(button, 0x90), kotor_m15d_color_007a23e4, 12);                          // 006BE38E..006BE3AB
        std::memcpy(at(button, 0x104), kotor_m15d_color_007a23e4, 12);                         // 006BE3AE..006BE3D1
    } else {
        sw(button, 0x8C, 0);                                                                   // 006BE3DC
        std::memcpy(at(button, 0x104), borrowed_enter_color_007a23c0, 12);                     // 006BE3E6..006BE409
    }
    gui_load_border_params_set_pulsing_00414d00(at(button, 0xF4), nullptr, 1, 1, 0);           // 006BE376..006BE380 pushes; 006BE3D4 / 006BE40C
}

// 006BE4E0 CSWGuiPartySelection::UpdateCount() (thiscall, RET 0): LBL_COUNT text = "%d" of 2 - selected.
void __fastcall kotor_fastcall_006be4e0(void* self, void*) {
    Str text; exo_default(text.p()); text.born();                                              // 006BE4FB..006BE4FF [esp+4]
    frame_LogFormat_005e5680(text.p(), "%d", 2 - li(self, 0x68));                              // 006BE504..006BE521 (0073D720)
    gui_load_text_params_set_text_00415e00(at(self, 0x2858), nullptr, text.p());              // 006BE529..006BE534
}                                                                                              // 006BE539..006BE545 ~CExoString

// 006BF2A0 CSWGuiPartySelection::OnToggled(CSWGuiControl*) (thiscall, RET 4).
void __fastcall kotor_fastcall_006bf2a0(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                        // 006BF2A6..006BF2AD
    if (lw(control, 0x48) == 0x2D)                                                             // 006BF2B3..006BF2B7
        kotor_fastcall_0040a140(lp(self, 0x18), nullptr, lb(control, 0x54));                   // 006BF2B9..006BF2C2 PlayGuiSound
    void* button = control;                                                                    // edi
    void* data;                                                                                // ebx
    if (control == at(self, 0x38B8)) {                                                         // 006BF2C7..006BF2D0 (BTN_ACCEPT)
        data = lp(self, 0x3A7C);                                                               // 006BF2D2
        if (data == nullptr) return;                                                           // 006BF2D8..006BF2DA
        button = data;                                                                         // 006BF2E0
    } else {
        data = lp(control, 0x58);                                                              // 006BF2E4
    }
    if ((lb(data, 0x448) & 1u) != 0) {                                                         // 006BF2E7..006BF2EE
        if (lw(button, 0x1C4) != 0) {                                                          // 006BF2F0..006BF2F8
            sw(self, 0x68, lw(self, 0x68) - 1u);                                               // 006BF2FA
            kotor_fastcall_006be370(button, nullptr, 0);                                       // 006BF2FD..006BF311
        } else if (li(self, 0x68) < 2) {                                                       // 006BF301..006BF307 (signed jge)
            sw(self, 0x68, lw(self, 0x68) + 1u);                                               // 006BF309..006BF30A
            kotor_fastcall_006be370(button, nullptr, 1);                                       // 006BF30D..006BF311
        }
        kotor_fastcall_006becc0(self, nullptr, lw(button, 0x1C4) == 0 ? 1 : 0);                // 006BF316..006BF326
        kotor_fastcall_006be4e0(self, nullptr);                                                // 006BF32B..006BF32D
        return;
    }
    void* box = lp(in_game_gui(), 0x98);                                                       // 006BF338..006BF345
    kotor_fastcall_00627130(box, nullptr, 0);                                                  // 006BF34B..006BF34F SetAllowCancel(0)
    gui_load_messagebox_set_callback_00624a40(box, nullptr, nullptr, nullptr, 0);              // 006BF354..006BF35C
    vcall<void, U32>(box, 0x6C, (lb(data, 0x448) & 4u) != 0 ? 0xA5A6u : 0xA588u);              // 006BF361..006BF373 / 006BF389..006BF390
    kotor_fastcall_0040bc70(lp(self, 0x18), nullptr, box, 1, 1);                               // 006BF376..006BF37E / 006BF393..006BF39B
}
}  // extern "C"
