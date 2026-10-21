// lane-m50-diag NON-ORIGINAL HOST DIAGNOSTIC REPLACEMENT COPY of M49 unit game-lifetime-stages
// (game_lifetime.cpp sha 4a112bec...). Only change: kotor_host_diag* report calls (D94 proposal) before each
// existing false return / std::terminate in bind/detach/finish/dtor; the bind entry condition is split into
// sequential ifs in the SAME short-circuit order. Results, phases and termination are unchanged.
#include "2cc0e1a78a-game_lifetime.h"
#include "4e2136d0b8-functional_runtime.h"
#include "af34ecd272-release_contract.h"
#include "05a3c5e4fb-allocator_live.h"
#include "f96fb2c5c4-window_shared_cells.h"
namespace kotor_renderer_gl_cells { extern std::uint32_t width_0078d1d4; }   // W-5 single owner (gl-renderer-gl-cells, D45; cold 800 = original .data 0078D1D4)
#include <exception>

namespace kotor_functional {
namespace {
using Global = client_services_start::Global;
GameLifetime* active_lifetime = nullptr; // Aggregate metadata claim only.
constexpr Global joined[6] = {Global::Base, Global::Manager, Global::Input,
                            Global::Aurora, Global::Width, Global::Height};
bool canonical_attachment() noexcept {
    return kotor_menu_window_attachment &&
        kotor_menu_window_publication == &kotor_menu_window_attachment->publication;
}
}

bool aggregate_lifetime_idle() noexcept { return active_lifetime == nullptr; }

bool GameLifetime::heap_matches() const noexcept {
    return active_lifetime == this && source_heap_identity() && live_globals.heap && borrowed_heap_ &&
        *live_globals.heap == borrowed_heap_ &&
        base_ini_reconstruction::bindings.allocate_006fa7e6 == &live_operator_new;
}
bool GameLifetime::joins_match() const noexcept {
    const auto* cells = kotor_menu_window_shared_cells;
    if (!cells || !sources_.views || !sources_.start || !sources_.client || !sources_.gui) return false;
    auto& v = *sources_.views;
    void volatile* expected[6] = {
        v.winmain.base_7a39e0.address(),
        cells->manager_007a39fc,
        cells->input_007a39e4,
        cells->aurora_007a39f8,
        &kotor_renderer_gl_cells::width_0078d1d4,   // W-5 single owner
        cells->height_0078d1d8};
    for (unsigned i=0; i<6; ++i)
        if (sources_.start->words[static_cast<unsigned>(joined[i])] != expected[i]) return false;
    return sources_.start->depth_0078d1e0 == &v.winmain.depth_78d1e0 &&
        sources_.client->input_007a39e4 == &kotor_menu_window_cells::input_007a39e4 &&
        sources_.gui->manager_007a39fc == &kotor_menu_window_cells::manager_007a39fc;
}
void GameLifetime::restore_joins() noexcept {
    for (unsigned i=0; i<6; ++i)
        sources_.start->words[static_cast<unsigned>(joined[i])] = previous_words_[i];
    sources_.start->depth_0078d1e0 = previous_depth_;
    sources_.client->input_007a39e4 = previous_input_;
    sources_.gui->manager_007a39fc = previous_manager_;
}

bool GameLifetime::bind(const GameBindings& s) noexcept {
    // The original single || condition, split in identical order (pure reads / identity queries).
    if (phase_ != Phase::Empty) { ; return false; }
    if (active_lifetime) { ; return false; }
    if (!canonical_attachment()) { ; return false; }
    if (kotor_menu_window_attachment->phase != kotor_live_publication::Phase::Empty) { ; return false; }
    if (!kotor_menu_window_shared_cells) { ; return false; }
    if (!source_heap_identity()) { ; return false; }
    if (!live_globals.heap || !*live_globals.heap) { ; return false; }
    if (base_ini_reconstruction::bindings.allocate_006fa7e6 != &live_operator_new) { ; return false; }
    if (!s.views) { ; return false; }
    if (!s.winmain) { ; return false; }
    if (!s.shell) { ; return false; }
    if (!s.start) { ; return false; }
    if (!s.client) { ; return false; }
    if (!s.gui) { ; return false; }
    if (!s.window) { ; return false; }
    if (s.start != client_services_start::globals) { ; return false; }
    if (s.client != &client_services::dependencies) { ; return false; }
    if (s.gui != &menu_normal_40_49::providers) { ; return false; }
    active_lifetime = this;
    sources_ = s;
    borrowed_heap_ = *live_globals.heap;
    for (unsigned i=0; i<6; ++i)
        previous_words_[i] = s.start->words[static_cast<unsigned>(joined[i])];
    previous_depth_ = s.start->depth_0078d1e0;
    previous_input_ = s.client->input_007a39e4;
    previous_manager_ = s.gui->manager_007a39fc;
    // Existing transactional address helper, no constructor/body duplication.
    if (!kotor_base_gui_join::attach(*s.start,*s.client,*s.views,*s.gui,*kotor_menu_window_shared_cells)) {
        ;
        sources_ = {}; borrowed_heap_ = nullptr; active_lifetime = nullptr; return false;
    }
    if (!kotor_live_publication::attach(*kotor_menu_window_attachment,*s.views,
                                       *s.winmain,*s.shell,*s.window)) {
        ;
        if (!heap_matches() || !joins_match()) { ; std::terminate(); }
        restore_joins(); sources_ = {}; borrowed_heap_ = nullptr; active_lifetime = nullptr; return false;
    }
    phase_ = Phase::Live;
    if (!heap_matches() || !joins_match()) { ; std::terminate(); }
    return true; // Binding protocol only; aggregate readiness remains a real-owner gate.
}

bool GameLifetime::detach_windows() noexcept {
    if (phase_ == Phase::Empty || phase_ == Phase::Released) return true;
    if (phase_ == Phase::Detached) return true;
    if (phase_ == Phase::Finishing || !canonical_attachment() || !heap_matches() || !joins_match()) return false;
    phase_ = Phase::Draining;
    if (!kotor_live_publication::detach(*kotor_menu_window_attachment)) { ; return false; }
    if (!heap_matches() || !joins_match()) { ; std::terminate(); }
    phase_ = Phase::Detached;
    return true;
}

bool GameLifetime::finish(const GameDrain& drain) noexcept {
    if (phase_ == Phase::Empty || phase_ == Phase::Released) return true;
    if (phase_ != Phase::Detached || !canonical_attachment() || !heap_matches() || !joins_match() ||
        kotor_menu_window_attachment->phase != kotor_live_publication::Phase::Empty ||
        !drain.drain_source_objects) return false;
    phase_ = Phase::Finishing; // Reject recursive finish/detach during owner cleanup.
    const bool complete = drain.drain_source_objects(drain.owner);
    if (!heap_matches() || !joins_match()) { ; std::terminate(); }
    if (!complete) { phase_ = Phase::Detached; return false; }
    restore_joins(); sources_ = {}; borrowed_heap_ = nullptr;
    active_lifetime = nullptr;
    phase_ = Phase::Released;
    return true;
}
GameLifetime::~GameLifetime() {
    if ((phase_ != Phase::Empty && phase_ != Phase::Released) || active_lifetime == this) {
        ;
        std::terminate();
    }
}
}
