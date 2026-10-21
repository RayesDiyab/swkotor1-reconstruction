#pragma once
#include "17f3e6b8af-ini_write.h"

namespace kotor_functional {
// Support ownership only. No original constructor, destructor or provider body
// is supplied here. One scope is placed between IniAllocatorBinding and GameLease.
// Its caller must quiesce every consumer and genuinely drain all game owners.
class IniProviderScope {
    bool claimed_ = false;
    void* borrowed_heap_ = nullptr; // Actual HANDLE identity, no heap owner copy.
public:
    bool claim() noexcept;
    bool current_heap_matches() const noexcept;
    ~IniProviderScope();
    IniProviderScope() = default;
    IniProviderScope(const IniProviderScope&) = delete;
    IniProviderScope& operator=(const IniProviderScope&) = delete;
};

// A borrowed outer (including StartServices' stack object) is never heap-freed.
// Valid, completely constructed receivers and nonthrowing original cleanup are
// required. A non-null internal needs the genuine005EB500 callback; absence is
// reported without freeing anything. Requires the active provider scope and its
// unchanged actual heap. A non-null callback is not provenance proof.
bool try_destroy_borrowed_ini(base_ini_reconstruction::PublicIni&);

// Only for a canonical PublicIni whose outer allocation is owned by the current
// source heap (e.g. Base's original4-byte child allocation). Never pass a stack
// outer. Successful original cleanup is followed by outer lifetime end/free.
bool try_release_heap_ini(base_ini_reconstruction::PublicIni*);
}
