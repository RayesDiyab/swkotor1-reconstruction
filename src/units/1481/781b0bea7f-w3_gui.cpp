#include "a90575cbf1-w3_gui.h"
#include <cstring>
namespace {
template<class T> T load(const void* p,std::uint32_t offset) {
    T value; std::memcpy(&value,static_cast<const unsigned char*>(p)+offset,sizeof value); return value;
}
}
// 0062B4D0 RET4. The alternate party panel is considered only when the argument is zero.
extern "C" std::uint32_t __fastcall recovered_gui_in_game_get_party_access_panel_up_0062b4d0(
    void* gui,void*,std::uint32_t equip_only) {
    auto* manager=load<menu_continuation::GuiManager*>(gui,0x38);
    if(manager==nullptr) return 0;
    if(frame_ContainsPanel_0040bd70(manager,nullptr,load<void*>(gui,0x0c))!=0) return 1;
    if(equip_only!=0) return 0;
    return frame_ContainsPanel_0040bd70(load<menu_continuation::GuiManager*>(gui,0x38),nullptr,
                                     load<void*>(gui,0x10))!=0 ? 1u : 0u;
}
// 0062FEA0 RET. Reused t3v native body with its selected provider's exact public type.
extern "C" std::int32_t __fastcall recovered_gui_in_game_get_current_equip_screen_npc_0062fea0(
    void* gui,void*) {
    auto* manager=load<menu_continuation::GuiManager*>(gui,0x38);
    if(manager==nullptr) return -1;
    if(frame_ContainsPanel_0040bd70(manager,nullptr,load<void*>(gui,0x0c))==0) return -1;
    return static_cast<std::int32_t>(load<std::int8_t>(load<void*>(gui,0x0c),0x4274));
}
