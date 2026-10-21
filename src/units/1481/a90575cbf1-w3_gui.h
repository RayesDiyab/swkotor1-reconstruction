#pragma once
#include <cstdint>
namespace menu_continuation { struct GuiManager; }
extern "C" {
// Exact selected0458 public declaration, not a void* redeclaration.
unsigned __fastcall frame_ContainsPanel_0040bd70(menu_continuation::GuiManager*,void*,void*);
std::uint32_t __fastcall recovered_gui_in_game_get_party_access_panel_up_0062b4d0(void*,void*,std::uint32_t);
std::int32_t __fastcall recovered_gui_in_game_get_current_equip_screen_npc_0062fea0(void*,void*);
}
