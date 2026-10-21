#pragma once
#include "65f0dc72b4-internal.h"
#include "21712cb4d5-path.h"
#include "bab7a6ef86-canonical_formatter_owner.h"

namespace file_engine_owner {
// Explicit lifetime support; not a replacement game algorithm or drain routine.
// The graph serializes publication/retirement and retains heap, Locale,
// formatter, Base/alias providers and real FILE bindings throughout admission.
class Owner {
    file_internal::EngineBindings engine_{};
    path_resolution::Bindings resolver_{};
    kotor_canonical_string_format::FormatterScope* formatter_ = nullptr;
    kotor_reconstruction::StartupString* log_ = nullptr;
    bool retired_ = false;
public:
    bool publish(kotor_canonical_string_format::FormatterScope&);
    bool identity() const noexcept;
    // Caller must first stop admission and perform actual worker/caller drain.
    // Metadata checks below do not prove worker quiescence or synchronization.
    bool retire_after_drain() noexcept;
    Owner() = default;
    ~Owner();
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;
};
}
