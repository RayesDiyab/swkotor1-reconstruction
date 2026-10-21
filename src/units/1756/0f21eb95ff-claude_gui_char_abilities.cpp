// Lane W4 GUI successor (worker "gui-char"): CSWGuiInGameAbilities (0x3F98, table 00755E50) slot functions.
//   006AB8B0 OnPanelRemoved        thiscall() RET            (slot)
//   006ADC20 OnPanelAdded          thiscall() RET            (slot)
//   006AE5F0 HandleInputEvent      thiscall(int, int) RET 8  (slot)
// helper reconstructed here:
//   006ABE70 DisplayPowers         thiscall() RET, returns int
// Transcribed from the original swkotor.exe linear sweep; each statement carries its original PCs.
#include "6030a9ee65-claude_gui_char.hpp"

namespace kotor { namespace server_mainloop { unsigned __cdecl creature_force_class_005be4a0(unsigned cls); } } // CSWClass::IsJedi (static)

extern "C" {
void __fastcall kotor_m15d_chart_clear_006cd240(void* chart, void*);                            // CSWGuiSkillFlowChart::ClearChart
w4::I32 __fastcall kotor_m15d_chart_handle_input_006cdd80(void* chart, void*, w4::I32 event);    // CSWGuiSkillFlowChart::HandleInput
w4::U8 __fastcall kotor_m15b_creature_stats_get_class_00647730(void* stats, void*, w4::U32 index); // CSWCCreatureStats::GetClass
// ---- CSWGuiInGameAbilities helpers (claude_menu_handlers_b2.cpp)
void __fastcall kotor_fastcall_006adb00(void* self, void*, void* creature);                     // SetCharacter
void __fastcall kotor_fastcall_006abec0(void* self, void*);                                     // UpdatePortraits
void __fastcall kotor_fastcall_006ad560(void* self, void*);                                     // UpdateView
void __fastcall kotor_fastcall_006acce0(void* self, void*, w4::U32 spell);                      // OnEnterPower
void __fastcall kotor_fastcall_006ad410(void* self, void*, w4::U32 feat);                       // OnEnterFeat
// ---- defined here
w4::U32 __fastcall kotor_fastcall_006abe70(void* self, void*);
void __fastcall kotor_fastcall_006ab8b0(void* self, void*);
void __fastcall kotor_fastcall_006adc20(void* self, void*);
void __fastcall kotor_fastcall_006ae5f0(void* self, void*, std::uint32_t event, std::uint32_t down);
}

using namespace gchar;

extern "C" {
// 006ABE70 CSWGuiInGameAbilities::DisplayPowers: the character's last class is a Jedi class and it knows powers.
w4::U32 __fastcall kotor_fastcall_006abe70(void* self, void*) {
    void* stats = lp(lp(self, 0x3F74), 0x2F8);                                                 // 006ABE73..006ABE79
    const U32 last = static_cast<U8>(lb(stats, 0x32) - 1);                                     // 006ABE7F..006ABE84 (eax = byte)
    const U8 cls = kotor_m15b_creature_stats_get_class_00647730(stats, nullptr, last);         // 006ABE86..006ABE87
    if (kotor::server_mainloop::creature_force_class_005be4a0(cls) != 0 && li(self, 0x3F7C) > 0) return 1; // 006ABE8C..006ABEAB
    return 0;                                                                                  // 006ABEAD
}

// 006AB8B0 OnPanelRemoved: clear the ability list box and both flow charts (the last one is a tail call).
void __fastcall kotor_fastcall_006ab8b0(void* self, void*) {
    gui_load_listbox_clear_items_00419a90(at(self, 0x30DC), nullptr);                          // 006AB8B3..006AB8B9
    kotor_m15d_chart_clear_006cd240(at(self, 0x3F78), nullptr);                                // 006AB8BE..006AB8C4
    kotor_m15d_chart_clear_006cd240(at(self, 0x3F88), nullptr);                                // 006AB8C9..006AB8D0 jmp
}

// 006ADC20 OnPanelAdded.
void __fastcall kotor_fastcall_006adc20(void* self, void*) {
    sb(self, 0x3F71, 0xFF);                                                                    // 006ADC28
    sw(self, 0x38B8, 0xFFFFFFFEu); sw(self, 0x3A7C, 0xFFFFFFFEu);                              // 006ADC23..006ADC35
    void* character = frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, 0); // 006ADC3B..006ADC4C
    kotor_fastcall_006adb00(self, nullptr, character);                                         // 006ADC51..006ADC54 SetCharacter
    kotor_fastcall_006abec0(self, nullptr);                                                    // 006ADC59..006ADC5B UpdatePortraits
    vcall<void, U32>(at(self, 0x369C), 0x0C, 0);                                               // 006ADC60..006ADC6E [vtbl+0Ch](0)
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 006ADC74 jmp
}

// 006AE5F0 HandleInputEvent(int event, int down) RET 8. Jump table 006AE8A8 / index bytes 006AE8C8 (event - 28h, 0..B7h).
void __fastcall kotor_fastcall_006ae5f0(void* self, void*, std::uint32_t event, std::uint32_t down) {
    if (down != 0 && event - 0x28u <= 0xB7u) {                                                 // 006AE5F0..006AE60C
        switch (event) {
        case 0xCE: {                                                                           // 006AE620 next party member
            play_sound(self, 1);                                                               // 006AE620..006AE627
            if ((lb(recovered_server_info_004aec90(server_app(), nullptr), 0x104) & 1) != 0) { // 006AE62C..006AE641
                I32 npc = static_cast<std::int8_t>(lb(self, 0x3F71)) + 1;                      // 006AE647..006AE650
                U32 found = 0;                                                                 // 006AE64E ebp
                for (; npc < 9 && found == 0; ++npc) {                                         // 006AE651..006AE6BE
                    if (kotor_fastcall_005636b0(party_table(), nullptr, npc) == 0) continue;   // 006AE65E..006AE676
                    void* client = client_app();                                               // 006AE678..006AE680 esi
                    const U32 object = recovered_npc_object_00564700(party_table(), nullptr, npc, found, 1); // 006AE683..006AE68E
                    void* creature = pt(frame_R4_005ed5b0(client, nullptr, kotor_fastcall_005eda50(client, nullptr, object))); // 006AE693..006AE69E
                    if (creature != nullptr) {                                                 // 006AE6A3..006AE6A5
                        kotor_fastcall_006adb00(self, nullptr, creature);                      // 006AE6A7..006AE6AA SetCharacter
                        sb(self, 0x3F71, static_cast<U8>(npc));                                // 006AE6AF
                        found = 1;                                                             // 006AE6B5
                    }
                }
                if (found == 0) {
                    sb(self, 0x3F71, 0xFF);                                                    // 006AE6C4
                    kotor_fastcall_006adb00(self, nullptr,
                        kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr)); // 006AE6CB..006AE701
                }
            } else {
                kotor_fastcall_005edf80(client_app(), nullptr, 0, 0, 1);                       // 006AE6D5..006AE6E4
                kotor_fastcall_006adb00(self, nullptr,
                    kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr)); // 006AE6E9..006AE701
            }
            kotor_fastcall_006abec0(self, nullptr);                                            // 006AE706..006AE708 UpdatePortraits
            break;
        }
        case 0x29: {                                                                           // 006AE714 cycle the view
            play_sound(self, 1);                                                               // 006AE714..006AE719
            const U8 view = lb(ingame_gui(), 0xBC0);                                           // 006AE71E..006AE72B
            if (view == 2) {                                                                   // 006AE73D
                sb(ingame_gui(), 0xBC0, 0);                                                    // 006AE73D..006AE74D
                kotor_fastcall_006ad560(self, nullptr);                                        // 006AE754 UpdateView
            } else if (view == 1) {                                                            // 006AE75E
                sb(ingame_gui(), 0xBC0, 2);                                                    // 006AE75E..006AE7A2
                kotor_fastcall_006ad560(self, nullptr);                                        // 006AE7A9..006AE7AB
            } else if (view == 0) {                                                            // 006AE769
                if (kotor_fastcall_006abe70(self, nullptr) != 0) {                             // 006AE769..006AE772 DisplayPowers
                    sb(ingame_gui(), 0xBC0, 1);                                                // 006AE774..006AE783
                    kotor_fastcall_006ad560(self, nullptr);                                    // 006AE78A
                } else {
                    sb(ingame_gui(), 0xBC0, 2);                                                // 006AE794..006AE7A2
                    kotor_fastcall_006ad560(self, nullptr);                                    // 006AE7A9..006AE7AB
                }
            } else {
                kotor_fastcall_006ad560(self, nullptr);                                        // 006AE73B jne 006AE7A9
            }
            break;
        }
        case 0x3A:                                                                             // 006AE7B5
            vcall<void, U32, U32>(at(self, 0x33BC), 0x3C, 0x32, 1);                            // 006AE7B5..006AE7C5 description scroll
            break;
        case 0x39:                                                                             // 006AE7CD
            vcall<void, U32, U32>(at(self, 0x33BC), 0x3C, 0x31, 1);                            // 006AE7CD..006AE7DD
            break;
        case 0x28: case 0x2D: case 0x2E: case 0xDF:                                            // 006AE7E5 close
            if (frame_R4_0062cba0(ingame_gui(), nullptr, 0) != 0)                              // 006AE7E5..006AE7FE
                gui_m48_client_set_input_class_005eda60(client_app(), nullptr, 0, 1);          // 006AE804..006AE811
            break;
        case 0x2F: case 0x30: case 0x3F: case 0x40:                                            // 006AE818 chart navigation (sound)
            if (lb(ingame_gui(), 0xBC0) != 0) play_sound(self, 1);                             // 006AE818..006AE834
            [[fallthrough]];
        case 0x31: case 0x32: case 0x3D: case 0x3E:                                            // 006AE839 chart navigation
            if (lb(ingame_gui(), 0xBC0) == 1) {                                                // 006AE839..006AE84E
                const I32 power = kotor_m15d_chart_handle_input_006cdd80(at(self, 0x3F78), nullptr, static_cast<I32>(event)); // 006AE850..006AE857
                kotor_fastcall_006acce0(self, nullptr, static_cast<U32>(power));               // 006AE85C..006AE85F OnEnterPower
            } else if (lb(ingame_gui(), 0xBC0) == 2) {                                         // 006AE866..006AE87B
                const I32 feat = kotor_m15d_chart_handle_input_006cdd80(at(self, 0x3F88), nullptr, static_cast<I32>(event)); // 006AE87D..006AE884
                kotor_fastcall_006ad410(self, nullptr, static_cast<U32>(feat));                // 006AE889..006AE88C OnEnterFeat
            }
            break;
        default: break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);                          // 006AE892..006AE89E
}
}  // extern "C"
