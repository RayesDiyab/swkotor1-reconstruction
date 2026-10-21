// M50 composer merge (m50-compose A1): lane-m50-heap G-HEAP replacement (14fc8a62) + lane-m50-diag D94 instrumentation (ec8ae93c).
// lane-m50-diag NON-ORIGINAL HOST DIAGNOSTIC REPLACEMENT COPY of M49 unit functional-bootstrap
// (functional_bootstrap.cpp sha c842b160...). Only change: kotor_host_diag* report calls (D94 proposal)
// immediately before each existing refusal/exit; compound refusal conditions are split into
// sequential ifs in the SAME short-circuit order so each check has its own stable id. Every
// return value, exit code, termination call and destructor order is unchanged.
#include "99112abac6-functional_bootstrap.h"
#include "4e2136d0b8-functional_runtime.h"
#include "af34ecd272-release_contract.h"
#include "4ca1fc7c3d-modern_locale.h"
#include "1a238b49b7-ini_lifetime.h"
#include "a5849aad54-canonical_formatter_owner.h"
#include "25a8ff2aee-base_ini.h"
#include "17f3e6b8af-ini_write.h"
#include "6e77177947-ini_addhead.h"
#include "f96fb2c5c4-window_shared_cells.h"
namespace kotor_renderer_gl_cells { extern std::uint32_t width_0078d1d4; }   // W-5 single owner (gl-renderer-gl-cells, D45; cold 800 = original .data 0078D1D4)
#include "76c86825db-window_live_views.h"
#include "05a3c5e4fb-allocator_live.h"
#include <exception>

namespace kotor_crt_heap { bool published() noexcept; }   // crt-heap-cells (lane-m50-heap)

namespace kotor_functional {
static_assert(sizeof(base_ini_reconstruction::Bindings) == 112 &&
              offsetof(base_ini_reconstruction::Bindings, allocate_006fa7e6) == 0 &&
              offsetof(base_ini_reconstruction::Bindings, add_alias_005e7760) == 108,
              "Exact existing28-slot INI provider contract required");
namespace {
constexpr int invalid_bootstrap = 86;
// lane-m50-heap (G-HEAP): the former run()-scoped GameHeap (HeapCreate inside WinMain, HeapDestroy plus
// require_empty_source_heap when run() returned) is removed. The original CRT heap is created by _heap_init
// 007026DC from WinMainCRTStartup (006FB456) before _cinit runs any __xi/__xc initializer (006FB4C9) and is never
// destroyed (sole HeapDestroy 00702720 is the _heap_init failure path; doexit 006FB13F runs atexit, __xp, __xt,
// then ExitProcess). Its single owner is now unit crt-heap-cells, published from .CRT$XIB (unit
// xi-007026dc-heap-init) before every original initializer, and live until process exit.

// Bootstrap owns this one existing table slot. Keep it live through genuine
// partial/full game destruction; detach when run() returns (the CRT heap itself stays live).
class IniAllocatorBinding {
    bool published_ = false;
public:
    bool publish() noexcept {
        auto& slot = base_ini_reconstruction::bindings.allocate_006fa7e6;
        if (!source_heap_identity()) { ; return false; }
        if (slot) { ; return false; }
        slot = &live_operator_new; // Exact cdecl signature and source flag1 path.
        published_ = true;
        return true;
    }
    ~IniAllocatorBinding() {
        if (!published_) return;
        auto& slot = base_ini_reconstruction::bindings.allocate_006fa7e6;
        if (!source_heap_identity() || slot != &live_operator_new) {
            ;
            std::terminate();
        }
        slot = nullptr;
    }
    IniAllocatorBinding() = default;
    IniAllocatorBinding(const IniAllocatorBinding&) = delete;
    IniAllocatorBinding& operator=(const IniAllocatorBinding&) = delete;
};

// The existing genuine checked release is the factual producer operation.
// It must return with empty partial/full metadata and detached actual owners;
// a failed source drain keeps every INI/cache/Locale/heap owner alive and stops.
bool __cdecl release_and_observe_formatter_consumers(void* opaque) noexcept {
    auto* prepared = static_cast<PreparedGame*>(opaque); // Actual caller object.
    if (!prepared) { ; return false; }
    release_game_checked(prepared); // Existing source owner, once; no fake drain.
    const bool observed = source_heap_identity() && !prepared->lifetime && !prepared->window &&
        !prepared->start && aggregate_lifetime_idle();
    return observed;
}
struct FormatterRetirement {
    kotor_canonical_string_format::FormatterScope& formatter;
    ~FormatterRetirement() {
        // Declared before IniProviderScope: its destructor has already detached
        // the admitted table. Cached storage still uses the live heap/Locale.
        if (!formatter.finish_release()) {
            ;
            fail_closed_release(ReleaseRefusal::InvalidMetadata);
        }
    }
};
struct GameLease {
    PreparedGame prepared{};
    kotor_canonical_string_format::FormatterScope& formatter;
    ~GameLease() {
        if (!formatter.prepare_release(&release_and_observe_formatter_consumers,&prepared)) {
            ;
            fail_closed_release(ReleaseRefusal::AggregateOwnershipRetained);
        }
    }
    explicit GameLease(kotor_canonical_string_format::FormatterScope& owner) : formatter(owner) {}
    GameLease(const GameLease&) = delete;
    GameLease& operator=(const GameLease&) = delete;
};

struct ContextScope {
    kotor_winmain::Context* winmain = kotor_winmain::bound_context;
    kotor_window_shell::Context* shell = kotor_window_shell::bound_context;
    ~ContextScope() {
        kotor_winmain::bound_context = winmain;
        kotor_window_shell::bound_context = shell;
    }
};

// Diagnostic refusal for valid_publication: report, then the unchanged false result.
bool refuse(const char*) noexcept {
    ;
    return false;
}

bool valid_publication(const PreparedGame& game) noexcept {
    const auto* p = game.window;
    const auto* cells = kotor_menu_window_shared_cells;
    // The original single || condition, split in identical order (pure reads / canonical()).
    if (!game.lifetime) return refuse("vp.lifetime_null");
    if (!p) return refuse("vp.window_null");
    if (p != kotor_menu_window_publication) return refuse("vp.window_not_published");
    if (!game.start) return refuse("vp.start_null");
    if (game.start != client_services_start::globals) return refuse("vp.start_not_published");
    if (!cells) return refuse("vp.shared_cells_null");
    if (!kotor_menu_live_views::canonical(*cells)) return refuse("vp.shared_cells_not_canonical");
    if (!p->winmain) return refuse("vp.pub.winmain_null");
    if (!p->shell) return refuse("vp.pub.shell_null");
    if (!p->enter_reinit) return refuse("vp.pub.enter_reinit_null");
    if (!p->leave_reinit) return refuse("vp.pub.leave_reinit_null");
    if (!p->enter_message) return refuse("vp.pub.enter_message_null");
    if (!p->leave_message) return refuse("vp.pub.leave_message_null");
    const auto& w = *p->winmain;
    const auto& s = *p->shell;
    if (!w.globals) return refuse("vp.winmain.globals_null");
    if (!w.frame) return refuse("vp.winmain.frame_null");
    if (!w.dependencies.invoke) return refuse("vp.winmain.invoke_null");
    if (!w.dependencies.read_word) return refuse("vp.winmain.read_word_null");
    if (!s.globals) return refuse("vp.shell.globals_null");
    if (!s.constants) return refuse("vp.shell.constants_null");
    if (!s.frame) return refuse("vp.shell.frame_null");
    if (!s.dependencies.invoke) return refuse("vp.shell.invoke_null");
    if (w.globals->manager_7a39fc.address() != cells->manager_007a39fc) return refuse("vp.cell.manager_007a39fc");
    if (w.globals->reinit_7a3a2c.address() != cells->reinit_007a3a2c) return refuse("vp.cell.reinit_007a3a2c");
    if (w.globals->height_78d1d8.address() != cells->height_0078d1d8) return refuse("vp.cell.height_0078d1d8");
    if (w.globals->hwnd_7a39d8.address() != cells->render_007a39d8) return refuse("vp.cell.winmain_hwnd_007a39d8");
    if (s.globals->top_window_7a39d4.address() != cells->top_007a39d4) return refuse("vp.cell.top_007a39d4");
    if (s.globals->render_window_7a39d8.address() != cells->render_007a39d8) return refuse("vp.cell.shell_render_007a39d8");
    if (s.globals->instance_7a39c0.address() != cells->instance_007a39c0) return refuse("vp.cell.instance_007a39c0");
    using client_services_start::Global;
    const auto* start = game.start;
    for (unsigned n = 0; n < static_cast<unsigned>(Global::Count); ++n)
        if (!start->words[n]) { ; return false; }
    if (!start->logging_byte_008328e1) return refuse("vp.start.logging_008328e1_null");
    if (!start->depth_0078d1e0) return refuse("vp.start.depth_0078d1e0_null");
    if (start->words[static_cast<unsigned>(Global::Base)] != w.globals->base_7a39e0.address()) return refuse("vp.start.join_base");
    if (start->words[static_cast<unsigned>(Global::Manager)] != cells->manager_007a39fc) return refuse("vp.start.join_manager");
    if (start->words[static_cast<unsigned>(Global::Input)] != cells->input_007a39e4) return refuse("vp.start.join_input");
    if (start->words[static_cast<unsigned>(Global::Aurora)] != cells->aurora_007a39f8) return refuse("vp.start.join_aurora");
    if (start->words[static_cast<unsigned>(Global::Height)] != cells->height_0078d1d8) return refuse("vp.start.join_height");
    if (start->words[static_cast<unsigned>(Global::Width)] != &kotor_renderer_gl_cells::width_0078d1d4) return refuse("vp.start.join_width");
    if (start->depth_0078d1e0 != &w.globals->depth_78d1e0) return refuse("vp.start.join_depth");
    // Storage/body linkage does not establish the genuine INI provider family.
    // Every slot is used by an existing Base/INI body; no null-success table.
    const auto& ini = base_ini_reconstruction::bindings;
    if (ini.allocate_006fa7e6 != &live_operator_new) return refuse("vp.ini.allocate_006fa7e6");
    if (!ini.debug_005e9370) return refuse("vp.ini.debug_005e9370");
    if (!ini.timers_005e91e0) return refuse("vp.ini.timers_005e91e0");
    if (!ini.aliases_005e6810) return refuse("vp.ini.aliases_005e6810");
    if (!ini.ini_005e6750) return refuse("vp.ini.ini_005e6750");
    if (!ini.random_005ea420) return refuse("vp.ini.random_005ea420");
    if (!ini.internal_005e8ba0) return refuse("vp.ini.internal_005e8ba0");
    if (!ini.list_005ea4d0) return refuse("vp.ini.list_005ea4d0");
    if (!ini.section_005ea600) return refuse("vp.ini.section_005ea600");
    if (!ini.c_str_005e5670) return refuse("vp.ini.c_str_005e5670");
    if (!ini.sprintf_006fadb0) return refuse("vp.ini.sprintf_006fadb0");
    if (!ini.compare_nc_005e58c0) return refuse("vp.ini.compare_nc_005e58c0");
    if (!ini.destroy_005ea650) return refuse("vp.ini.destroy_005ea650");
    if (!ini.assign_string_005e5c50) return refuse("vp.ini.assign_string_005e5c50");
    if (!ini.assign_cstr_005e5140) return refuse("vp.ini.assign_cstr_005e5140");
    if (!ini.open_006fad9d) return refuse("vp.ini.open_006fad9d");
    if (!ini.gets_006fc469) return refuse("vp.ini.gets_006fc469");
    if (!ini.seek_006fbb19) return refuse("vp.ini.seek_006fbb19");
    if (!ini.close_006facf0) return refuse("vp.ini.close_006facf0");
    if (!ini.add_tail_005e9dc0) return refuse("vp.ini.add_tail_005e9dc0");
    if (!ini.next_005e9b90) return refuse("vp.ini.next_005e9b90");
    if (!ini.chr_006fb5e0) return refuse("vp.ini.chr_006fb5e0");
    if (!ini.right_005e6170) return refuse("vp.ini.right_005e6170");
    if (!ini.left_005e5f70) return refuse("vp.ini.left_005e5f70");
    if (!ini.equals_005e5290) return refuse("vp.ini.equals_005e5290");
    if (!ini.length_005e5790) return refuse("vp.ini.length_005e5790");
    if (!ini.format_string_005e5680) return refuse("vp.ini.format_string_005e5680");
    if (!ini.add_alias_005e7760) return refuse("vp.ini.add_alias_005e7760");
    // INI Write consumes this separate table before menu construction too.
    const auto& write = base_ini_reconstruction::write_bindings;
    if (!write.concat_005e5d10) return refuse("vp.ini_write.concat_005e5d10");
    if (!write.print_006faf01) return refuse("vp.ini_write.print_006faf01");
    if (write.add_head_005e9d70 != &ini_addhead::AddHead005e9d70) return refuse("vp.ini_write.add_head_005e9d70");
    const auto& d = client_services::dependencies;
    // The original single && return expression, split in identical order.
    if (!d.allocate_006fa7e6) return refuse("vp.client.allocate_006fa7e6");
    if (!d.tick_0073d0f8) return refuse("vp.client.tick_0073d0f8");
    if (!d.stats_004010c0) return refuse("vp.client.stats_004010c0");
    if (!d.object_table_004ae5d0) return refuse("vp.client.object_table_004ae5d0");
    if (!d.internal_005fbfe0) return refuse("vp.client.internal_005fbfe0");
    if (!d.rules_00552c50) return refuse("vp.client.rules_00552c50");
    if (!d.load_screen_0067a710) return refuse("vp.client.load_screen_0067a710");
    if (!d.string_005e5a90) return refuse("vp.client.string_005e5a90");
    if (!d.destroy_string_005e5c20) return refuse("vp.client.destroy_string_005e5c20");
    if (!d.get_int_00413660) return refuse("vp.client.get_int_00413660");
    if (!d.gui_initialize_00632720) return refuse("vp.client.gui_initialize_00632720");
    if (!d.event_descriptions_005eeb10) return refuse("vp.client.event_descriptions_005eeb10");
    if (!d.movie_playing_0070c1f0) return refuse("vp.client.movie_playing_0070c1f0");
    if (!d.unpauseable_0062b410) return refuse("vp.client.unpauseable_0062b410");
    if (!d.clear_events_005df4e0) return refuse("vp.client.clear_events_005df4e0");
    if (!d.sound_mode_005d5e80) return refuse("vp.client.sound_mode_005d5e80");
    if (!d.input_active_005df540) return refuse("vp.client.input_active_005df540");
    if (!d.keyboard_off_005f2980) return refuse("vp.client.keyboard_off_005f2980");
    if (!d.show_mouse_0061fa00) return refuse("vp.client.show_mouse_0061fa00");
    if (!d.rules_007a3a28) return refuse("vp.client.rules_007a3a28");
    if (!d.sound_007a39ec) return refuse("vp.client.sound_007a39ec");
    if (!d.sound_mode_0074c5f8) return refuse("vp.client.sound_mode_0074c5f8");
    if (d.input_007a39e4 != &kotor_menu_window_cells::input_007a39e4) return refuse("vp.client.input_007a39e4");
    return true;
}
}

bool source_heap_identity() noexcept {
    return kotor_crt_heap::published();
}

int run(const StartupArguments& args) {
    // The CRT heap was published before every C/C++ initializer and outlives WinMain (see crt-heap-cells).
    if (!source_heap_identity()) { ; return invalid_bootstrap; }
    LocaleScope locale;
    if (!locale.claim()) { ; return invalid_bootstrap; }
    IniAllocatorBinding ini_allocator;
    if (!ini_allocator.publish()) { ; return invalid_bootstrap; }
    // Actual heap and Locale precede the one coupled formatter owner. Claim
    // performs no buffer allocation and no Menu20 descriptor publication.
    kotor_canonical_string_format::FormatterScope formatter;
    if (!formatter.claim(locale)) { ; return invalid_bootstrap; }
    FormatterRetirement formatter_retirement{formatter};
    // Snapshot before preparation, but retain published contexts through game
    // release/quiescence. Successful detach precedes fallback restoration.
    IniProviderScope ini_callbacks;
    if (!ini_callbacks.claim()) { ; return invalid_bootstrap; }
    ContextScope contexts;
    GameLease game{formatter};
    // Original `!prepare || !valid_publication` split in identical short-circuit order.
    if (!kotor_functional_game_prepare(&args, &game.prepared)) {
        ;
        return invalid_bootstrap;
    }
    if (!valid_publication(game.prepared)) {
        ;
        return invalid_bootstrap;
    }
    kotor_menu_bind_winmain_context();
    return kotor_winmain::winmain_004041f0(
        reinterpret_cast<kotor_winmain::Word>(args.instance),
        reinterpret_cast<kotor_winmain::Word>(args.previous), args.command_line, args.show);
}
}
