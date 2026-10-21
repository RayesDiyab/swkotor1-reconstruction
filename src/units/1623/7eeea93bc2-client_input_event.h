#pragma once
// CClientExoAppInternal::HandleInputEvent 00621210 (whole function incl. all
// jump-table cases, tables 00622128 / 006221FC / 00622154) plus the left mouse
// button actions 0061F880 / 00620530 (lane-input, fork H).
// External callees not provided by M41 are D1 declarations: an original
// __thiscall is ABI-identical to __fastcall(self, unused EDX, stack args...).
// Return types: the consumed EAX is typed; "void" means EAX is never consumed
// at any call site in this unit (provider return type itself is not claimed).
#include <cstdint>
#include "cff764feb1-hover_path.h"

namespace kotor_input_required {
using U32 = std::uint32_t;
// CClientExoApp (ECX = *(007A39FC)+4)
void* __fastcall get_in_game_gui_005ed690(void* app, void*);                  // RET, EAX=[[app+4]+40]
void* __fastcall get_sw_party_005ed8b0(void* app, void*);                     // RET
void* __fastcall get_sw_gui_manager_005eda40(void* app, void*);               // RET
void* __fastcall get_player_creature_005ed540(void* app, void*);              // tail JMP 005F1420
void* __fastcall get_game_object_005ed580(void* app, void*, U32 id);          // tail JMP 005F10F0 (RET 4)
int __fastcall get_paused_by_combat_005edc10(void* app, void*);               // tail JMP 005F2DF0
int __fastcall get_auto_paused_005edef0(void* app, void*);                    // RET
void __fastcall set_auto_paused_005edee0(void* app, void*, U32, U32);         // tail JMP 005F3F10 (RET 8)
int __fastcall show_tutorial_window_005edf40(void* app, void*, U32, U32, U32, U32); // tail JMP 005F4120 (RET 10h)
void __fastcall reset_drive_acceleration_005edba0(void* app, void*);          // tail JMP 005F1400
void __fastcall player_flourish_weapons_005ede90(void* app, void*);           // tail JMP 005F4460
int __fastcall get_combat_mode_005ede70(void* app, void*);                    // RET, EAX=[[app+4]+320]
// CClientExoAppInternal (ECX = this)
void* __fastcall get_property_0x18_00644760(void* self, void*);               // RET, EAX=[this+18]
void* __fastcall get_player_creature_005f1420(void* self, void*);             // RET
int __fastcall get_controlled_character_dead_005f2ed0(void* self, void*);     // RET
int __fastcall get_paused_by_combat_005f2df0(void* self, void*);              // RET
void __fastcall set_paused_by_combat_005f2e10(void* self, void*, U32, U32, U32); // RET 0Ch
void __fastcall set_input_class_006200e0(void* self, void*, U32, U32);        // RET 8
void __fastcall change_character_to_next_living_party_member_005f7960(void* self, void*, U32, U32, U32); // RET 0Ch
void __fastcall show_mouse_0061fa00(void* self, void*, U32);                  // RET 4
void __fastcall hurry_death_sequence_005f2f20(void* self, void*);             // RET
void __fastcall select_nearest_object_005fb050(void* self, void*, U32, U32);  // RET 8
void __fastcall set_auto_paused_005f3f10(void* self, void*, U32, U32);        // RET 8
void __fastcall do_quick_save_005f4b50(void* self, void*);                    // RET
void __fastcall set_tool_tip_instant_mode_005f4be0(void* self, void*, U32);   // RET 4
void __fastcall get_default_actions_00620620(void* self, void*);              // RET
void __fastcall perform_rbutton_down_action_0061f940(void* self, void*);      // tail JMP 0040C610
void __fastcall perform_rbutton_up_action_0061f960(void* self, void*);        // tail JMP 0040A1A0
void __fastcall handle_mouse_click_in_world_00620350(void* self, void*);      // RET
// CExoInput (ECX = *(007A39E4))
void __fastcall clear_events_005df4e0(void* input, void*);                    // tail JMP 005E0710
void __fastcall cool_down_event_005df4b0(void* input, void*, U32, U32);       // tail JMP 005E0670 (RET 8)
// CSWGuiManager
void __fastcall handle_z_mouse_0040c650(void* manager, void*, U32);           // RET 4
void __fastcall add_panel_0040bc70(void* manager, void*, void* panel, U32, U32); // RET 0Ch
void __fastcall set_software_mouse_cursor_0040a270(void* manager, void*, U32, U32, U32); // RET 0Ch
// CSWCModule
void __fastcall restore_camera_00641dc0(void* module, void*);                 // RET
void __fastcall set_free_look_camera_006413c0(void* module, void*, U32);      // RET 4
void __fastcall set_navigate_camera_00641610(void* module, void*);            // RET
// CGuiInGame
void __fastcall change_character_0062b0f0(void* gui, void*);                  // tail JMP 00687FD0
void __fastcall show_solo_mode_query_0062e550(void* gui, void*, U32);         // RET 4
int __fastcall hide_sw_in_game_gui_0062cba0(void* gui, void*, U32);           // RET 4
int __fastcall show_sw_in_game_gui_0062c9b0(void* gui, void*, U32);           // RET 4
void __fastcall switch_to_sw_in_game_gui_0062cf10(void* gui, void*, U32);     // RET 4
void __fastcall set_sw_gui_status_0062aa00(void* gui, void*, U32, U32);       // RET 8
void __fastcall do_quick_load_00633c50(void* gui, void*);                     // RET
// CSWGuiMessageBox
void __fastcall set_allow_cancel_00627130(void* box, void*, U32);             // RET 4
void __fastcall set_callback_00624a40(void* box, void*, U32, U32, U32);       // RET 0Ch
// CSWParty / CSWCCreature / CSWCObject / CSWMiniPlayer
void* __fastcall get_player_character_00635460(void* party, void*);           // RET
void* __fastcall get_server_creature_0060fb20(void* creature, void*);         // RET
int __fastcall is_stealth_capable_00610ac0(void* creature, void*);            // RET
void __fastcall party_menu_stealth_0060f4b0(void* creature, void*, U32, U32); // RET 8
void __fastcall set_combat_mode_00610a10(void* creature, void*, U32);         // RET 4
void __fastcall clear_all_actions_0063d470(void* object, void*);              // RET
void __fastcall fire_0066dc50(void* mini_player, void*);                      // RET
// CSWGuiMainInterface / CSWGuiTargetActionMenu (ECX = main interface or +0xBC)
void __fastcall select_next_action_00688820(void* main_interface, void*, U32);   // RET 4
void __fastcall do_personal_action_0068ad60(void* main_interface, void*, U32, U32); // RET 8
void __fastcall do_target_action_0068ad20(void* main_interface, void*, U32, U32);   // RET 8
void __fastcall select_next_action_006865b0(void* target_menu, void*, U32);      // RET 4
void __fastcall clear_one_action_00688790(void* main_interface, void*);         // RET
unsigned char __fastcall is_target_action_menu_control_00684ed0(void* target_menu, void*, void* control); // RET 4, AL consumed
// Original CRT (cdecl)
int __cdecl sprintf_006fadb0(char* buffer, const char* format, ...);
void* __cdecl fopen_006fad9d(const char* name, const char* mode);
int __cdecl fclose_006facf0(void* file);
}
// M41 provider (aurora-peer-void.obj): 004015A0 plain RET (ECX ignored).
extern "C" void __cdecl renderer_void_004015a0();

// Original cells with no M41 owner (declared only; owner to be assigned).
namespace kotor_input_cells_required {
extern volatile std::int32_t camera_restore_007a2288;   // written 00621322/00621866/0062175E
extern volatile std::int32_t camera_toggle_008338e8;    // 00621757 read, 006217CA ++
extern volatile std::int32_t word_008338f0;             // 00621AF2, 00621CE6 (also 0061F8C6, 00620576 reads)
extern volatile std::int32_t word_008338f4;             // 00621AEC, 0061F88B, 0062053B
extern volatile std::int32_t word_008338f8;             // 00621CFF; read by action cases
extern const char text_0074fbfc[];                      // .rdata "KotOR%04i.tga" (D13 extraction)
extern const char text_0074fbec[];                      // .rdata "KotOR%04i%c.tga"
extern const char text_0073d5b0[];                      // .rdata "rb"
}

// Default-action element at CClientExoAppInternal+0x4C8 (00621FEE..00622046):
// +0x08 argument word, +0x0C code word and +0x10 this-adjustment (a member
// function pointer called with ECX = object_base + adjustment), +0x1C object id.
namespace kotor_input_client {
struct DefaultAction {
    unsigned char opaque00[8];
    std::uint32_t argument;                                                   // +0x08
    void (__fastcall* function)(void* self, void*, std::uint32_t, void* pc);  // +0x0C (RET 8, D1)
    std::int32_t adjustment;                                                  // +0x10
    unsigned char opaque14[8];
    std::uint32_t object_id;                                                  // +0x1C
};
struct GameObjectVtable { const void* slots00_08[3]; void* (__fastcall* base)(void* self, void*); };  // +0x0C (00622031)
struct GameObject { const GameObjectVtable* vtable; };
struct GuiMessageBoxVtable { const void* slots00_68[27]; void (__fastcall* set_text)(void* self, void*, std::uint32_t); }; // +0x6C (00621525)
struct GuiMessageBox { const GuiMessageBoxVtable* vtable; };
static_assert(offsetof(DefaultAction, function) == 0x0C && offsetof(DefaultAction, adjustment) == 0x10 && offsetof(DefaultAction, object_id) == 0x1C, "default action");
static_assert(offsetof(GameObjectVtable, base) == 0x0C && offsetof(GuiMessageBoxVtable, set_text) == 0x6C, "vtable slots");

void __fastcall perform_lbutton_down_0061f880(kotor_input_hover::ClientInternal* self, void*, int at_end);  // RET 4
void __fastcall perform_lbutton_up_00620530(kotor_input_hover::ClientInternal* self, void*);               // RET
}
