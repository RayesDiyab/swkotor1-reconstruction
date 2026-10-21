#include "da6673a8ff-base_gui_join.h"
#include "e347588a95-startup_views.h"
#include "f96fb2c5c4-window_shared_cells.h"
namespace kotor_renderer_gl_cells { extern std::uint32_t width_0078d1d4; }   // W-5 single owner (gl-renderer-gl-cells, D45; cold 800 = original .data 0078D1D4)
namespace kotor_base_gui_join {
bool attach(client_services_start::Globals& start,client_services::Dependencies& client,
            kotor_menu_live_views::Views& views,menu_normal_40_49::Providers& gui,
            const kotor_menu_aliases::Cells8& cells) noexcept {
    using client_services_start::Global;
    if(!kotor_menu_live_views::canonical(cells)||&gui!=&menu_normal_40_49::providers||
       views.winmain.base_7a39e0.address()!=&kotor_menu_10_19::dependencies::base_007a39e0||
       views.winmain.manager_7a39fc.address()!=cells.manager_007a39fc||
       views.winmain.height_78d1d8.address()!=cells.height_0078d1d8||
       views.reinit.input_7a39e4.address()!=cells.input_007a39e4||
       views.reinit.aurora_7a39f8.address()!=cells.aurora_007a39f8)return false;
    struct Join {Global slot;void volatile* address;};
    const Join joins[]={
       {Global::Base,views.winmain.base_7a39e0.address()},
       {Global::Manager,cells.manager_007a39fc},{Global::Input,cells.input_007a39e4},
       {Global::Aurora,cells.aurora_007a39f8},{Global::Width,&kotor_renderer_gl_cells::width_0078d1d4},
       {Global::Height,cells.height_0078d1d8}};
    // Reject every conflicting old binding before writing any destination.
    for(const auto& join:joins){auto prior=start.words[static_cast<unsigned>(join.slot)];
       if(prior&&prior!=join.address)return false;}
    auto* depth=&views.winmain.depth_78d1e0;
    auto* input=&kotor_menu_window_cells::input_007a39e4;
    auto* manager=&kotor_menu_window_cells::manager_007a39fc;
    if((start.depth_0078d1e0&&start.depth_0078d1e0!=depth)||
       (client.input_007a39e4&&client.input_007a39e4!=input)||
       (gui.manager_007a39fc&&gui.manager_007a39fc!=manager))return false;
    for(const auto& join:joins)start.words[static_cast<unsigned>(join.slot)]=join.address;
    start.depth_0078d1e0=depth;client.input_007a39e4=input;gui.manager_007a39fc=manager;
    return true;
}
}
