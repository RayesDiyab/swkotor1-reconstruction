// Lane closeC (sub-fork "gui"): faithful bodies for the in-game debug-menu panel slots that were traps in
// gui_slot_traps_successor.obj (generated vtable slot traps) and 1205-23 panels_debug.obj (W4_TRAP_HANDLER), plus the
// CGuiInGame Hide* helpers they reach that had no provider in the c35 map. Transcribed from the original swkotor.exe
// (read-only); every comment address is an original instruction. No original address is dereferenced: [007A39FC] is
// kotor_menu_window_cells::manager_007a39fc (CAppManager*: +4 client, +8 server), [007A3A28] is kotor_cell_rules_007a3a28.
// ABI (verified against the original RET):
//   006BD980 CSWGuiCreateItemSubMenu::HandleInputEvent(event, down)          RET 8
//   006CF7B0 CSWGuiPowersFeatsSkillsDebugMenu::HandleInputEvent(event, down) RET 8
//   006D02C0 CSWGuiLoadModuleDebugMenu::HandleInputEvent(event, down)        RET 8
//   006D03A0 CSWGuiCreateItemMenu::HandleInputEvent(event, down)             RET 8
//   006E9B90 CSWGuiScriptSelect::OnPanelAdded()                              RET 0
//   006E9BC0 CSWGuiScriptSelect::HandleInputEvent(event, down)               RET 8
//   006D05A0 CSWGuiCreateItemMenu::Back(control)                             RET 4
//   006CF9D0 CSWGuiLoadModuleDebugMenu::OnModulePicked(control)              RET 4
//   006CF770 CSWGuiPowersFeatsSkillsDebugMenu::Back()                        RET 0
//   006CF9A0 CSWGuiLoadModuleDebugMenu::Back()                               RET 0
//   0062D2B0 CGuiInGame::HideItemCreateMenu()                                RET 0
//   0062D360 CGuiInGame::HideSubItemCreateMenu()                             RET 0
//   0062DA70 CGuiInGame::HideLoadModuleDebugMenu()                           RET 0
//   0062DAE0 CGuiInGame::HidePowersFeatsSkillsDebugMenu()                    RET 0
//   006BE2B0 CSWGuiCreateItemSubMenu::ClearItems()                           RET 0
// The HandleInputEvent jump tables (index byte table -> 4 targets) were decoded from the original: event - 28h in
// 0..16h with index bytes {0,3,3,3,3,3,0,3,3,1,2,3,3,3,3,3,3,3,3,3,3,1,2} (events 28h/2Eh -> case 0, 31h/3Dh ->
// case 1, 32h/3Eh -> case 2, else default); ScriptSelect: event - 27h in 0..7 with {0,1,2,2,2,2,0,1}.
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
inline unsigned char* at(void* base, U off) { return static_cast<unsigned char*>(base) + off; }
inline U ld(void* base, U off) { U v; std::memcpy(&v, at(base, off), 4); return v; }
inline I lds(void* base, U off) { I v; std::memcpy(&v, at(base, off), 4); return v; }
inline void st(void* base, U off, U v) { std::memcpy(at(base, off), &v, 4); }
inline void* lp(void* base, U off) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(ld(base, off))); }
}

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }    // [007A39FC] CAppManager*
namespace kotor_input_required {
void* __fastcall get_in_game_gui_005ed690(void* client, void*);                  // 005ED690 CClientExoApp::GetInGameGui
}
namespace kotor_sound_internal {
void __fastcall StringListClear005e9590(void* list, void*);                       // 005E9590 CExoStringList::Clear
}
namespace {
inline void* client() { return lp(kotor_menu_window_cells::manager_007a39fc, 4); }   // mov ecx,[[007A39FC]+4]
inline void* server() { return lp(kotor_menu_window_cells::manager_007a39fc, 8); }   // mov ecx,[[007A39FC]+8]
inline void* in_game_gui() { return kotor_input_required::get_in_game_gui_005ed690(client(), nullptr); }
}

extern "C" {
extern void* kotor_cell_rules_007a3a28;                                           // [007A3A28] CSWRules*
// ---------------- existing providers (c35 map, exact decorated arity) ----------------
void __fastcall gui_load_panel_handle_input_00409e60(void* panel, void*, U event, U down); // 00409E60 CSWGuiPanel::HandleInputEvent RET 8
void __fastcall kotor_fastcall_0040a140(void* manager, void*, U sound);           // 0040A140 CSWGuiManager::PlayGuiSound RET 4
void __fastcall gui_load_panel_on_added_0040b870(void* panel, void*);             // 0040B870 CSWGuiPanel::OnPanelAdded
int __fastcall show_tutorial_window_005edf40(void* app, void*, U id, U a, U b, U c); // 005EDF40 CClientExoApp::ShowTutorialWindow RET 10h
void* __fastcall kotor_fastcall_004ae770(void* server, void*, U id);              // 004AE770 CServerExoApp::GetCreatureByGameObjectID RET 4
void* __fastcall kotor_m1_listbox_get_selected_control_0041a5d0(void* listbox, void*); // 0041A5D0 CSWGuiListBox::GetSelectedControl
void* __fastcall kotor_fastcall_0040be00(void* manager, void*);                   // 0040BE00 CSWGuiManager::PopModalPanel
void __fastcall gui_m48_panel_set_to_destroy_00624a00(void* panel, void*, U flag); // 00624A00 CSWGuiPanel::setToDestroy RET 4
int __fastcall frame_ContainsPanel_0040bd70(void* manager, void*, void* panel);   // 0040BD70 CSWGuiManager::PanelExists RET 4
void __fastcall frame_RemovePanel_0040c830(void* manager, void*, void* panel);    // 0040C830 CSWGuiManager::RemovePanel RET 4
void* __fastcall kotor_m15e_client_get_mini_game_005edb00(void* app, void*);      // 005EDB00 CClientExoApp::GetMiniGame
void __fastcall kotor_fastcall_005eda60(void* client, void*, U cls, U flag);      // 005EDA60 CClientExoApp::SetInputClass RET 8
void __fastcall kotor_m15e_slot090_void0_004015a0(void* self, void*);             // 004015A0 void_return
void __fastcall kotor_fastcall_006cf410(void* menu, void*);                       // 006CF410 CreateMainPowersFeatsSkillsMenu (1205-23)
void __fastcall gui_load_listbox_clear_items_00419a90(void* listbox, void*);      // 00419A90 CSWGuiListBox::ClearItems
void* __fastcall WindowCopy005e5b00(void* self, void*, void* source);             // 005E5B00 CExoString::CExoString(const&)
void __fastcall frame_LogStringDtor_005e5c20(void* self, void*);                  // 005E5C20 CExoString::~CExoString
void* __fastcall recovered_server_set_move_to_module_string_004aecd0(void* server, void*, void* name) noexcept(false); // 004AECD0 RET 4
void* __fastcall recovered_server_set_move_to_module_pending_004aecc0(void* server, void*, U pending); // 004AECC0 RET 4
void live_free(void*);
// ---------------- defined here ----------------
void __fastcall kotor_fastcall_0062d2b0(void* gui, void*);
void __fastcall kotor_fastcall_0062d360(void* gui, void*);
void __fastcall kotor_fastcall_0062da70(void* gui, void*);
void __fastcall kotor_fastcall_0062dae0(void* gui, void*);
void __fastcall kotor_fastcall_006be2b0(void* menu, void*);
void __fastcall kotor_fastcall_006cf770(void* menu, void*);
void __fastcall kotor_fastcall_006cf9a0(void* menu, void*);
void __fastcall kotor_fastcall_006bd980(void* self, void*, U event, U down);
void __fastcall kotor_fastcall_006cf7b0(void* self, void*, U event, U down);
void __fastcall kotor_fastcall_006d02c0(void* self, void*, U event, U down);
void __fastcall kotor_fastcall_006d03a0(void* self, void*, U event, U down);
void __fastcall kotor_fastcall_006e9b90(void* self, void*);
void __fastcall kotor_fastcall_006e9bc0(void* self, void*, U event, U down);
void __fastcall kotor_fastcall_006d05a0(void* self, void*, void* control);
void __fastcall kotor_fastcall_006cf9d0(void* self, void*, void* control);
}

namespace {
// Shared jump-table decode of the four list-menu HandleInputEvent bodies (index byte table, see header).
int list_menu_case(U event) {
    static const unsigned char index[23] = {0, 3, 3, 3, 3, 3, 0, 3, 3, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 2};
    const U i = event - 0x28u;                                                         // lea eax,[ebx-28h]
    if (i > 0x16u) return 3;                                                           // cmp eax,16h; ja default
    return index[i];                                                                   // movzx eax,byte [eax+table]
}
// CGuiInGame::Hide*Menu common body: in-game GUI active (+108h), the panel at +off is in the manager (+38h):
// pop the modal panel, remove the panel, then SetInputClass(GetMiniGame() != 0 ? 1 : 0, 1).
bool hide_panel(void* gui, U off) {
    if (ld(gui, 0x108) == 0) return false;                                             // +108h test; je ret
    if (frame_ContainsPanel_0040bd70(lp(gui, 0x38), nullptr, lp(gui, off)) == 0) return false; // PanelExists
    (void)kotor_fastcall_0040be00(lp(gui, 0x38), nullptr);                            // PopModalPanel
    frame_RemovePanel_0040c830(lp(gui, 0x38), nullptr, lp(gui, off));                 // RemovePanel
    const void* const mini_game = kotor_m15e_client_get_mini_game_005edb00(client(), nullptr); // GetMiniGame
    kotor_fastcall_005eda60(client(), nullptr, mini_game != nullptr ? 1u : 0u, 1);    // SetInputClass(…, 1)
    return true;
}
}

// 0062D2B0 CGuiInGame::HideItemCreateMenu() (panel +58h).
extern "C" void __fastcall kotor_fastcall_0062d2b0(void* gui, void*) { (void)hide_panel(gui, 0x58); } // 0062D2B0..0062D317
// 0062DA70 CGuiInGame::HideLoadModuleDebugMenu() (panel +70h).
extern "C" void __fastcall kotor_fastcall_0062da70(void* gui, void*) { (void)hide_panel(gui, 0x70); } // 0062DA70..0062DAD7
// 0062DAE0 CGuiInGame::HidePowersFeatsSkillsDebugMenu() (panel +74h).
extern "C" void __fastcall kotor_fastcall_0062dae0(void* gui, void*) { (void)hide_panel(gui, 0x74); } // 0062DAE0..0062DB47
// 0062D360 CGuiInGame::HideSubItemCreateMenu() (panel +5Ch); both SetInputClass branches tail-jump to
// CSWGuiCreateItemSubMenu::ClearItems on [this+5Ch] (re-read).
extern "C" void __fastcall kotor_fastcall_0062d360(void* gui, void*) {
    if (!hide_panel(gui, 0x5C)) return;                                                // 0062D363..0062D3AF / 0062D3C8
    kotor_fastcall_006be2b0(lp(gui, 0x5C), nullptr);                                   // 0062D3B4..0062D3B8 / 0062D3CD..0062D3D1 jmp
}

// 006BE2B0 CSWGuiCreateItemSubMenu::ClearItems().
extern "C" void __fastcall kotor_fastcall_006be2b0(void* menu, void*) {
    gui_load_listbox_clear_items_00419a90(at(menu, 0x64), nullptr);                    // 006BE2B3..006BE2B6
    if (ld(menu, 0x488) != 0) {                                                        // 006BE2BB..006BE2C3
        live_free(lp(menu, 0x488));                                                    // 006BE2C5..006BE2CB
        st(menu, 0x488, 0);                                                            // 006BE2CE
        st(menu, 0x490, 0);                                                            // 006BE2D8
    }
    st(menu, 0x48C, 0);                                                                // 006BE2E8
    kotor_sound_internal::StringListClear005e9590(at(menu, 0x4A8), nullptr);           // 006BE2E2..006BE2F2
    kotor_sound_internal::StringListClear005e9590(at(menu, 0x494), nullptr);           // 006BE2F7..006BE2FD
    st(menu, 0x484, 0xFFFFFF9Du);                                                      // 006BE302 (-63h)
}

// 006CF770 CSWGuiPowersFeatsSkillsDebugMenu::Back(): top level (+48Ch == 0) hides the menu, else rebuilds the main menu.
extern "C" void __fastcall kotor_fastcall_006cf770(void* menu, void*) {
    if (ld(menu, 0x48C) != 0) { kotor_fastcall_006cf410(menu, nullptr); return; }      // 006CF770..006CF778 / 006CF7A3 jmp
    kotor_fastcall_0062dae0(in_game_gui(), nullptr);                                   // 006CF77A..006CF789
    kotor_m15e_slot090_void0_004015a0(in_game_gui(), nullptr);                         // 006CF78E..006CF79E jmp void_return
}
// 006CF9A0 CSWGuiLoadModuleDebugMenu::Back().
extern "C" void __fastcall kotor_fastcall_006cf9a0(void*, void*) {
    kotor_fastcall_0062da70(in_game_gui(), nullptr);                                   // 006CF9A0..006CF9AF
    kotor_m15e_slot090_void0_004015a0(in_game_gui(), nullptr);                         // 006CF9B4..006CF9C4 jmp void_return
}

// 006BD980 CSWGuiCreateItemSubMenu::HandleInputEvent(event, down). +484h selection (-63h = none), +48Ch count.
extern "C" void __fastcall kotor_fastcall_006bd980(void* self, void*, U event, U down) {
    if (down != 0) {                                                                   // 006BD98B..006BD98F
        switch (list_menu_case(event)) {                                               // 006BD995..006BD9A8
        case 0:                                                                        // 006BDA3B
            kotor_fastcall_0062d360(in_game_gui(), nullptr);                           // 006BDA3B..006BDA4B
            break;
        case 1: {                                                                      // 006BD9AF
            if (lds(self, 0x484) == -0x63) st(self, 0x484, 0xFFFFFFFFu);               // 006BD9AF..006BD9B8
            const I sel = lds(self, 0x484);                                            // 006BD9C2
            if (sel > 0) st(self, 0x484, static_cast<U>(sel - 1));                     // 006BD9C8..006BD9CD
            else if (sel == 0) st(self, 0x484, ld(self, 0x48C) - 1);                   // 006BD9D5..006BD9DE
            if (ld(self, 0x18) != 0) kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 1); // 006BDA1E..006BDA27
            break;
        }
        case 2: {                                                                      // 006BD9E6
            if (lds(self, 0x484) == -0x63) st(self, 0x484, 0xFFFFFFFFu);               // 006BD9E6..006BD9EF
            const I count = lds(self, 0x48C);                                          // 006BD9F9
            const I sel = lds(self, 0x484);                                            // 006BD9FF
            if (sel < count) st(self, 0x484, static_cast<U>(sel + 1));                 // 006BDA05..006BDA0A
            else if (sel == count) st(self, 0x484, 0);                                 // 006BDA12..006BDA14
            if (ld(self, 0x18) != 0) kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 1); // 006BDA1E..006BDA27
            break;
        }
        default: break;                                                                // 006BDA50
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);                  // 006BDA2C..006BDA30 / 006BDA50..006BDA54
}

namespace {
// 006CF7B0 / 006D02C0 shared body (+484h selection, +488h count); case 0 calls the menu's Back().
void debug_list_input(void* self, U event, U down, void (__fastcall* back)(void*, void*)) {
    if (down != 0) {                                                                   // test edi,edi; je base
        switch (list_menu_case(event)) {
        case 0: back(self, nullptr); break;                                            // mov ecx,esi; call Back
        case 1: {
            const I sel = lds(self, 0x484);
            if (sel > 0) st(self, 0x484, static_cast<U>(sel - 1));                     // jle / dec
            else if (sel == 0) st(self, 0x484, ld(self, 0x488) - 1);                   // jne / [+488h]-1
            if (ld(self, 0x18) != 0) kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 1);
            break;
        }
        case 2: {
            const I sel = lds(self, 0x484);
            const I count = lds(self, 0x488);
            if (sel < count) st(self, 0x484, static_cast<U>(sel + 1));                 // jge / inc
            else if (sel == count - 1) st(self, 0x484, 0);                             // dec ecx; cmp; jne (original quirk:
                                                                                       // only reachable with sel >= count)
            if (ld(self, 0x18) != 0) kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 1);
            break;
        }
        default: break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);
}
}
// 006CF7B0 CSWGuiPowersFeatsSkillsDebugMenu::HandleInputEvent (006CF7B0..006CF85B; case 0 006CF844 Back 006CF770).
extern "C" void __fastcall kotor_fastcall_006cf7b0(void* self, void*, U event, U down) {
    debug_list_input(self, event, down, &kotor_fastcall_006cf770);
}
// 006D02C0 CSWGuiLoadModuleDebugMenu::HandleInputEvent (006D02C0..006D036B; case 0 006D0354 Back 006CF9A0).
extern "C" void __fastcall kotor_fastcall_006d02c0(void* self, void*, U event, U down) {
    debug_list_input(self, event, down, &kotor_fastcall_006cf9a0);
}

// 006D03A0 CSWGuiCreateItemMenu::HandleInputEvent(event, down): count is [[rules+34h]] (base item count).
extern "C" void __fastcall kotor_fastcall_006d03a0(void* self, void*, U event, U down) {
    if (down != 0) {                                                                   // 006D03AB..006D03AF
        switch (list_menu_case(event)) {                                               // 006D03B5..006D03C8
        case 0:                                                                        // 006D043E
            kotor_fastcall_0062d2b0(in_game_gui(), nullptr);                           // 006D043E..006D044D
            break;
        case 1: {                                                                      // 006D03CF
            const I sel = lds(self, 0x484);
            if (sel > 0) st(self, 0x484, static_cast<U>(sel - 1));                     // 006D03D5..006D03DA
            else if (sel == 0) st(self, 0x484, ld(lp(kotor_cell_rules_007a3a28, 0x34), 0)); // 006D03E2..006D03EF (no -1)
            if (ld(self, 0x18) != 0) kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 1); // 006D0421..006D042A
            break;
        }
        case 2: {                                                                      // 006D03F7
            const I count = lds(lp(kotor_cell_rules_007a3a28, 0x34), 0);               // 006D03F7..006D0400
            const I sel = lds(self, 0x484);                                            // 006D0402
            if (sel < count) st(self, 0x484, static_cast<U>(sel + 1));                 // 006D0408..006D040D
            else if (sel == count) st(self, 0x484, 0);                                 // 006D0415..006D0417
            if (ld(self, 0x18) != 0) kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 1); // 006D0421..006D042A
            break;
        }
        default: break;                                                                // 006D0452
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);                  // 006D042F..006D0433 / 006D0452..
}

// 006E9B90 CSWGuiScriptSelect::OnPanelAdded(): base OnPanelAdded, then ShowTutorialWindow(0Ah, 7F000000h, 7F000000h, 0).
extern "C" void __fastcall kotor_fastcall_006e9b90(void* self, void*) {
    gui_load_panel_on_added_0040b870(self, nullptr);                                   // 006E9B90
    (void)show_tutorial_window_005edf40(client(), nullptr, 0x0A, 0x7F000000u, 0x7F000000u, 0); // 006E9B95..006E9BAB
}

// 006E9BC0 CSWGuiScriptSelect::HandleInputEvent(event, down): case 0 (27h/2Dh) stores the selected script entry's word
// (+64h table, 8-byte entries, +4) into the creature's +A74h -> +13Ah, then both case 0 and case 1 (28h/2Eh) play
// sound 0, pop the modal panel and set the panel to destroy. ([this+18h] is not null-checked here.)
extern "C" void __fastcall kotor_fastcall_006e9bc0(void* self, void*, U event, U down) {
    static const unsigned char index[8] = {0, 1, 2, 2, 2, 2, 0, 1};                    // 006E9C8C
    if (down != 0 && event - 0x27u <= 7u) {                                            // 006E9BC5..006E9BDB
        const unsigned char c = index[event - 0x27u];                                  // 006E9BE1..006E9BE8
        if (c == 0) {                                                                  // 006E9BEF
            void* const creature = kotor_fastcall_004ae770(server(), nullptr, ld(self, 0xC3C)); // 006E9BEF..006E9C00
            if (creature != nullptr) {                                                 // 006E9C07..006E9C09
                void* const control = kotor_m1_listbox_get_selected_control_0041a5d0(at(self, 0x70), nullptr); // 006E9C0B..006E9C0E
                const U idx = ld(control, 0x50);                                       // 006E9C13
                std::uint16_t word;
                std::memcpy(&word, at(lp(self, 0x64), idx * 8 + 4), 2);                // 006E9C16..006E9C1F
                std::memcpy(at(lp(creature, 0xA74), 0x13A), &word, 2);                 // 006E9C19..006E9C24
            }
        }
        if (c == 0 || c == 1) {                                                        // 006E9C2B.. / 006E9C56..
            kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 0);                       // PlayGuiSound(0)
            (void)kotor_fastcall_0040be00(lp(self, 0x18), nullptr);                    // PopModalPanel
            gui_m48_panel_set_to_destroy_00624a00(self, nullptr, 0);                   // setToDestroy(0)
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);                  // 006E9C47..006E9C4B / 006E9C71..75
}

// 006D05A0 CSWGuiCreateItemMenu::Back(control): only for an active control ([control+4Ch]).
extern "C" void __fastcall kotor_fastcall_006d05a0(void*, void*, void* control) {
    if (ld(control, 0x4C) == 0) return;                                                // 006D05A0..006D05A9
    kotor_fastcall_0062d2b0(in_game_gui(), nullptr);                                   // 006D05AB..006D05BB
    kotor_m15e_slot090_void0_004015a0(in_game_gui(), nullptr);                         // 006D05C0..006D05D0 void_return
}

// 006CF9D0 CSWGuiLoadModuleDebugMenu::OnModulePicked(control): hide the menu, then
// server SetMoveToModuleString(copy of the control's +16Ch string) and SetMoveToModulePending(1).
extern "C" void __fastcall kotor_fastcall_006cf9d0(void*, void*, void* control) {
    if (ld(control, 0x4C) == 0) return;                                                // 006CF9ED..006CF9F2
    kotor_fastcall_0062da70(in_game_gui(), nullptr);                                   // 006CF9F4..006CFA03
    U name[2];                                                                         // [esp+4] CExoString
    (void)WindowCopy005e5b00(name, nullptr, at(control, 0x16C));                       // 006CFA08..006CFA13
    try {
        (void)recovered_server_set_move_to_module_string_004aecd0(server(), nullptr, name); // 006CFA18..006CFA2E
        (void)recovered_server_set_move_to_module_pending_004aecc0(server(), nullptr, 1); // 006CFA33..006CFA3D
    } catch (...) {
        frame_LogStringDtor_005e5c20(name, nullptr);                                   // unwind state 0
        throw;
    }
    frame_LogStringDtor_005e5c20(name, nullptr);                                       // 006CFA42..006CFA4E
}
