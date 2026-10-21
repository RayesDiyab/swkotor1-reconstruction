#pragma once
// lane-routes provider contract: borrowed cells for the authored route leaves.
#include "dc5456660e-winmain.h"
#include "aefc11afe2-kotor_original_calls.h"
#include "7213aec6ae-window_shared_cells.h"
#include <cstdint>
namespace kotor_reconstruction { struct StartupString; }
// Dispatchers (kotor_winmain_invoke etc.) are owned by lane-prepare (window_dispatch.cpp);
// lane-routes defines kotor_dispatch::route_table (kotor_dispatch_routes.h).
namespace winmain_routes {
// Borrowed original game cells touched by the authored WinMain-direct leaves. The sole
// owner (Main/lane-prepare game_prepare) publishes the actual storage; no storage here.
// WinMain's own cells (mouse trails 7A3CA8, screen saver 7A3CAC, manager 7A39FC) are
// reached through kotor_winmain::bound_context->globals, never duplicated.
struct Cells {
    std::uint16_t volatile* headlight_0078e388;       // AurEnable/DisableHeadLight 0044DA60/0044DA80
    std::uint32_t volatile* pause_overlay_0078e394;   // AurSetPauseOverlayColor 0044EF4E
    char* dofile_message_007b8fe8;                    // dofile sprintf target / result
    std::uint32_t volatile* resource_table_007b8fd8;  // dofile handle table pointer cell
    std::uint32_t volatile* resource_count_007b8fdc;  // dofile handle count cell
    std::uint32_t volatile* posted_table_007b90fc;    // AurRenderStrings PostedString* array cell
    std::uint32_t volatile* posted_count_007b9100;    // AurRenderStrings count cell
    char* version_007b9228;                           // AurGetVersionString 0044EEC0 sprintf target
    unsigned char volatile* vbo_disabled_007b927d;    // 0044EF60
    unsigned char volatile* vbo_write_only_0078e398;  // 0044EF70 / 0044EF80
    kotor_reconstruction::StartupString* product_version_007a3cd4;  // GetGameVersionInfo 005E6C0A
    void* volatile* object_008338fc;                  // Uninitialize 005F84C3 (deleting dtor, cleared)
    void* volatile* object_008338ec;                  // Uninitialize 005F84D9
    void* volatile* vfx_list_00833bc8;                // UnloadCachedVFX 00690910 (list pointer cell)
    void* volatile* gui_007a39f4;                     // StopServices 00604280 (cleared)
};
extern const Cells& cells;
}
