#pragma once
#include "9ef8b88f20-winmain.h"
#include "c56b927861-window_shell.h"
#include "ad600036ed-reinit.h"
#include "f1111b348a-message.h"
#include <cstddef>

namespace kotor_menu_live_views {
using Word = std::uint32_t;
using Tick = std::uint64_t;
// Construction inputs for nonshared fields only. Actual cold-state/string
// producers must supply them. These are not live shadow Globals or a sync buffer.
struct WinMainInitial {
    Word quit, argc, argv, callback; // disable_sound: borrowed Cell32 (lane-sound owner)
    Word priority_changed, priority_mode, width;
    std::uint8_t depth;
    Word headlight, frame_word;
    float bias, client_ms, server_ms, cap;
    Tick start, end, cap_start, cap_end;
    Word debug, deferred, position;
    char text;
    Word swapping, sleep_enabled, sleep_ms;
    std::int32_t mouse_trails, screen_saver;
};
struct ShellInitial {
    kotor_window_shell::String graphics_version, title, caption_prefix;
    Word enable_78d454, enable_78d474, enable_78d45c;
    Word enable_78d458, enable_78d448, enable_78d44c;
};
struct ReInitInitial { Word guard, mode; };
struct MessageInitial {
    Word movie_fullscreen, movie_inactive, focus, last_tick, movie_window, flag;
};
struct Initial {
    WinMainInitial winmain;
    ShellInitial shell;
    ReInitInitial reinit;
    MessageInitial message;
};

// The four actual source Globals types, with 20 Cell32 members explicitly
// constructed over the sole eight cells. No casts between Globals layouts.
// The caller owns uninitialized aligned storage and its lifetime; construction
// requires exact canonical cell identities. No static instance is published.
class Views {
    Views(const kotor_menu_aliases::Cells8&, const Initial&) noexcept;
    friend Views* construct(void*, std::size_t,
                            const kotor_menu_aliases::Cells8&, const Initial&) noexcept;
public:
    kotor_winmain::Globals winmain;
    kotor_window_shell::Globals shell;
    kotor_reinit::Globals reinit;
    kotor_message::Globals message;
    Views(const Views&) = delete;
    Views& operator=(const Views&) = delete;
};
bool canonical(const kotor_menu_aliases::Cells8&) noexcept;
Views* construct(void*, std::size_t,
                 const kotor_menu_aliases::Cells8&, const Initial&) noexcept;
}
