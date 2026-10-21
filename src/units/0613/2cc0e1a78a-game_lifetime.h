#pragma once
#include "99112abac6-functional_bootstrap.h"
#include "da6673a8ff-base_gui_join.h"
#include "490217376c-live_publication.h"
#include <cstdint>

namespace kotor_functional {
// Borrowed, genuinely constructed owner objects. No Context/Globals/table or
// callback body is owned/created here. A successful bind is not game readiness.
struct GameBindings {
    kotor_menu_live_views::Views* views;
    kotor_winmain::Context* winmain;
    kotor_window_shell::Context* shell;
    client_services_start::Globals* start;
    client_services::Dependencies* client;
    menu_normal_40_49::Providers* gui;
    const kotor_live_publication::Providers* window;
};
// The genuine aggregate owner supplies this operation only after recording its
// actual objects/streams/workers and accepted destruction bodies. No operation
// is installed by this proposal. False retains every remaining ownership record.
// The operation may drain owned payloads; it must retain Views/Contexts, Start,
// client/GUI table storage and their nine joins until finish returns true. The
// aggregate owner destroys that borrowed metadata only after join restoration.
struct GameDrain {
    void* owner;
    bool (__cdecl* drain_source_objects)(void*) noexcept;
};

// Aggregate metadata only; the sole canonical Attachment/cells/tables stay with
// main6. Actual construction and destruction remain with each source owner.
struct GameLifetime {
    enum class Phase { Empty, Live, Draining, Detached, Finishing, Released };
private:
    GameBindings sources_{};
    void volatile* previous_words_[6]{};
    unsigned char volatile* previous_depth_ = nullptr;
    decltype(client_services::Dependencies::input_007a39e4) previous_input_ = nullptr;
    decltype(menu_normal_40_49::Providers::manager_007a39fc) previous_manager_ = nullptr;
    void* borrowed_heap_ = nullptr;
    Phase phase_ = Phase::Empty;
    bool heap_matches() const noexcept;
    bool joins_match() const noexcept;
    void restore_joins() noexcept;
public:
    GameLifetime() = default;
    GameLifetime(const GameLifetime&) = delete;
    GameLifetime& operator=(const GameLifetime&) = delete;
    ~GameLifetime(); // Fail closed if any published/retained ownership remains.
    Phase phase() const noexcept { return phase_; }
    bool bind(const GameBindings&) noexcept;
    bool detach_windows() noexcept;
    // Requires genuine object/stream/worker drain, after task20 detach succeeds.
    // No alias restoration, callback detachment or free is done on false.
    bool finish(const GameDrain&) noexcept;
};
}
