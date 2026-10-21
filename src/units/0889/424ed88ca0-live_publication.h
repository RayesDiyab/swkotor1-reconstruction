#pragma once
#include "9407903bbd-bridges.h"
#include "f9309c6a48-window_live_views.h"

namespace kotor_live_publication {
// Owner-managed invocation metadata. Neither this node nor Attachment owns a
// Context, Globals, Frame, game object, window, heap, or original-address cell.
template<class C> struct Invocation {
    C* context = nullptr;
    Invocation* previous = nullptr;
    C* prior_bound = nullptr;
    bool active = false;
};
using ReinitInvocation = Invocation<kotor_reinit::Context>;
using MessageInvocation = Invocation<kotor_message::Context>;
struct Providers {
    void* owner;
    // Verifies durable provider/heap lifetime and the sole UI dispatch thread.
    bool (__cdecl* ready)(void*) noexcept;
    // Record ownership before a throwing acquisition. Nodes/Contexts/Frames
    // must be distinct from every active or retiring invocation. The owner
    // retains rejected overlapping resources for safe quiescent cleanup.
    ReinitInvocation* (__cdecl* acquire_reinit)(void*);
    void (__cdecl* release_reinit)(void*, ReinitInvocation*) noexcept;
    MessageInvocation* (__cdecl* acquire_message)(void*, std::uint32_t,
        std::uint32_t, std::uint32_t, std::uint32_t);
    void (__cdecl* release_message)(void*, MessageInvocation*) noexcept;
    // Destroy/unregister real windows, drain/join real workers and reclaim any
    // retained partial acquisition while views/callbacks remain published.
    // False retains ownership for a later retry; no persistent view is destroyed.
    bool (__cdecl* quiesce)(void*) noexcept;
};
enum class Phase { Empty, Live, Draining };
struct Attachment {
    kotor_menu_window::Publication publication{};
    kotor_menu_live_views::Views* views = nullptr;
    Providers providers{};
    kotor_winmain::Context* prior_winmain = nullptr;
    kotor_window_shell::Context* prior_shell = nullptr;
    kotor_reinit::Context* prior_reinit = nullptr;
    kotor_message::Context* prior_message = nullptr;
    ReinitInvocation* reinit = nullptr;
    MessageInvocation* message = nullptr;
    ReinitInvocation* retiring_reinit = nullptr;
    MessageInvocation* retiring_message = nullptr;
    Phase phase = Phase::Empty;
    bool quiescing = false;
    unsigned violations = 0;
    Attachment() = default;
    Attachment(const Attachment&) = delete;
    Attachment& operator=(const Attachment&) = delete;
};
// All inputs are already constructed and durably owned by task6/host. No
// callback or frame acquisition runs until every pointer and alias is checked.
bool attach(Attachment&, kotor_menu_live_views::Views&,
            kotor_winmain::Context&, kotor_window_shell::Context&,
            const Providers&) noexcept;
// False means ownership is retained: do not destroy views/providers or heap.
// Empty/partial attachment is safe. Live invocations block quiescence.
bool detach(Attachment&) noexcept;
}
// Main6 defines one durable metadata owner and both constant address exports:
// Attachment owned; ...attachment=&owned; ...publication=&owned.publication;
extern kotor_live_publication::Attachment* const kotor_menu_window_attachment;
