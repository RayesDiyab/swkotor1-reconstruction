#include "fd73ec7604-window_live_views.h"
#include "df72b7ab87-window_shared_cells.h"
#include "89b65663c9-startup_views.h"
#include <new>

// lane-sound sole owner of 00832894 (.bss 0; sound_owners.cpp).
extern "C" volatile std::uint32_t kotor_cell_disable_sound_00832894;
// D86(a): lane-m48-traps f-input sole owner of 008338F0 (.bss 0; process_input.cpp).
namespace kotor_input_cells_required { extern volatile std::int32_t word_008338f0; }

namespace kotor_menu_live_views {
using kotor_menu_aliases::Cell32;
bool canonical(const kotor_menu_aliases::Cells8& c) noexcept {
    using namespace kotor_menu_window_cells;
    return kotor_menu_aliases::valid(c) &&
        c.manager_007a39fc == &manager_007a39fc &&
        c.reinit_007a3a2c == &reinit_007a3a2c &&
        c.height_0078d1d8 == &height_0078d1d8 &&
        c.render_007a39d8 == &render_007a39d8 &&
        c.top_007a39d4 == &top_007a39d4 &&
        c.instance_007a39c0 == &instance_007a39c0 &&
        c.aurora_007a39f8 == &aurora_007a39f8 &&
        c.input_007a39e4 == &input_007a39e4;
}

Views::Views(const kotor_menu_aliases::Cells8& c, const Initial& i) noexcept
    : winmain{
        Cell32(&kotor_menu_10_19::dependencies::base_007a39e0), Cell32(c.manager_007a39fc), i.winmain.quit,
        i.winmain.argc, i.winmain.argv, i.winmain.callback,
        Cell32(&kotor_cell_disable_sound_00832894),
        i.winmain.priority_changed, i.winmain.priority_mode,
        Cell32(c.reinit_007a3a2c), i.winmain.width,
        Cell32(c.height_0078d1d8), i.winmain.depth,
        i.winmain.headlight, i.winmain.frame_word,
        i.winmain.bias, i.winmain.client_ms, i.winmain.server_ms, i.winmain.cap,
        i.winmain.start, i.winmain.end, i.winmain.cap_start, i.winmain.cap_end,
        i.winmain.debug, i.winmain.deferred, i.winmain.position, {i.winmain.text},
        Cell32(c.render_007a39d8), i.winmain.swapping,
        i.winmain.sleep_enabled, i.winmain.sleep_ms,
        i.winmain.mouse_trails, i.winmain.screen_saver},
      shell{
        i.shell.graphics_version, i.shell.title, i.shell.caption_prefix,
        Cell32(c.top_007a39d4), Cell32(c.render_007a39d8), Cell32(c.instance_007a39c0),
        i.shell.enable_78d454, i.shell.enable_78d474, i.shell.enable_78d45c,
        i.shell.enable_78d458, i.shell.enable_78d448, i.shell.enable_78d44c},
      reinit{
        Cell32(c.aurora_007a39f8), Cell32(c.manager_007a39fc),
        Cell32(c.input_007a39e4), Cell32(c.top_007a39d4),
        Cell32(c.render_007a39d8), Cell32(c.instance_007a39c0),
        i.reinit.guard, Cell32(c.reinit_007a3a2c), i.reinit.mode},
      message{
        Cell32(c.render_007a39d8), Cell32(c.input_007a39e4),
        Cell32(c.manager_007a39fc), Cell32(c.aurora_007a39f8),
        Cell32(c.reinit_007a3a2c), i.message.movie_fullscreen,
        i.message.movie_inactive, i.message.focus, i.message.last_tick,
        i.message.movie_window, Cell32(c.height_0078d1d8), Cell32(&kotor_input_cells_required::word_008338f0)} {}

Views* construct(void* storage, std::size_t bytes,
                 const kotor_menu_aliases::Cells8& c, const Initial& i) noexcept {
    // Check all eight identities before constructing any of the 20 fields.
    if (!canonical(c) || !storage || bytes < sizeof(Views) ||
        (reinterpret_cast<std::uintptr_t>(storage) % alignof(Views))) return nullptr;
    return new(storage) Views(c, i);
}
}
