// lane-m46-startup: WndProc route leaf 00401E90 toggleFullScreen (routes-lane route_table.cpp;
// kotor_original_calls.h:92 `void __cdecl kotor_cdecl_00401e90(uint32,uint32)`). Evidence: dis/00401e90.asm.
#include "6bdce58525-winmain_routes.h"
#include "f0ccbd8c44-services.h"                 // client_services::Client::input_005eda10 (client-services owner)
#include "5413f244d4-window_shared_cells.h"      // 007A39FC / 007A3A2C / 007A39D8 / 007A39F8 (M40 owners)
#include "6a2ab66935-aurora_window_slots.h"      // typed Aurora table (D41) for [aurora]+4 FullScreen
#include <cstdint>
#include <cstring>

extern "C" std::uint32_t kotor_cell_allow_windowed_007a3a30;   // lane-prepare prepare_image_data.cpp (game_prepare_producers.h:26)

// 00401E90..00401EF3 toggleFullScreen(mode, value), cdecl:
//  [[007A39FC]+4]->Input(1, 0) 005EDA10; if [007A3A30]==0 { SendMessageA([007A39D8], 1Ch, 0, 0); return }
//  a=[007A39F8]; if !a return; if mode==1 { [007A3A2C] = (value==1)+1; return }
//  [007A3A2C] = a->vtable[+4]() ? 1 : 2 (neg/sbb/add 2).
extern "C" void __cdecl kotor_cdecl_00401e90(std::uint32_t mode, std::uint32_t value) {
    void* app_client;
    std::memcpy(&app_client, static_cast<unsigned char*>(kotor_menu_window_cells::manager_007a39fc) + 4, 4);   // 00401E90..00401E95
    static_cast<client_services::Client*>(app_client)->input_005eda10(1, 0);                                   // 00401E98..00401E9C
    if (kotor_cell_allow_windowed_007a3a30 == 0) {                                                             // 00401EA1..00401EA8
        (void)kotor_renderer_init::SendMessageA_0073d428(kotor_menu_window_cells::render_007a39d8, 0x1c, 0, 0); // 00401EE1..00401EED
        return;
    }
    void* const aurora = kotor_menu_window_cells::aurora_007a39f8;                                             // 00401EAA
    if (!aurora) return;                                                                                       // 00401EB0..00401EB2
    if (mode == 1) {                                                                                           // 00401EB4..00401EB9
        kotor_menu_window_cells::reinit_007a3a2c = (value == 1 ? 1u : 0u) + 1u;                                // 00401EBB..00401EC8
        return;
    }
    kotor_renderer_init::Renderer* const r = static_cast<kotor_renderer_init::Renderer*>(aurora);
    const std::uint32_t full = kotor_aurora_window::table_of(r)->fullscreen(r, nullptr);                      // 00401ECF..00401ED1
    kotor_menu_window_cells::reinit_007a3a2c = full != 0 ? 1u : 2u;                                            // 00401ED4..00401EDB
}
