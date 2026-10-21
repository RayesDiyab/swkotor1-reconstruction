#pragma once
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#if !defined(_MSC_VER) || !defined(_M_IX86) || !defined(_MT) || defined(_DLL)
#error One pinned MSVC x86 static CRT is required.
#endif

namespace kotor_functional { class LocaleScope; }
namespace kotor_canonical_string_format {
using Word = std::uint32_t;
struct Dependencies {
    void* (__cdecl* allocate_006fa7e6)(Word);
    void (__cdecl* free_006fa390)(void*);
    std::int32_t (__cdecl* vsnprintf_006fd5a8)(char*,Word,const char*,va_list);
    char* (__cdecl* strncpy_006fab80)(char*,const char*,Word);
    char* volatile* format_buffer_008328a8;
    Word volatile* format_capacity_008328ac;
};
// One six-field immutable projection of the existing canonical formatter's
// dependencies. This is not a partially populated Menu20 Bindings object.
extern const Dependencies dependencies;
extern char* volatile format_buffer_008328a8;
extern Word volatile format_capacity_008328ac;
static_assert(sizeof(Dependencies)==24 &&
              offsetof(Dependencies,format_buffer_008328a8)==16 &&
              offsetof(Dependencies,format_capacity_008328ac)==20,
              "Exact six-word x86 dependency projection");

class FormatterUse;
class FormatterScope {
    enum class Phase { Inactive, Active, CheckingDrain, DrainAdmitted, Releasing };
    Phase phase_ = Phase::Inactive;
    void* borrowed_heap_ = nullptr;
    void* volatile* borrowed_heap_cell_ = nullptr;
    const kotor_functional::LocaleScope* borrowed_locale_ = nullptr;
    unsigned active_uses_ = 0;
    friend class FormatterUse;
    friend bool formatter_claim_admitted() noexcept;
    friend bool ini_detach_admitted() noexcept;
public:
    // The graph owner passes its actual retained canonical Locale scope.
    bool claim(const kotor_functional::LocaleScope&) noexcept;
    bool identity() const noexcept;
    using DrainCheck = bool (__cdecl*)(void*) noexcept;
    // False refuses retirement and retains the INI table's publication,
    // coupled cache ownership, this owner, Locale and borrowed heap.
    // Permitted late formatting may change cache contents/capacity. The graph
    // keeps every owner alive and retries factual drain later.
    // Callback execution is explicitly guarded against recursive release or
    // admission; ordinary late warnings remain allowed during this check.
    bool prepare_release(DrainCheck actual_consumer_drain,void* actual_context) noexcept;
    // Only after successful factual drain and the INI scope's detach. No
    // cached-buffer release is implied by a constructor/destructor or claim.
    bool finish_release() noexcept;
    FormatterScope() = default;
    ~FormatterScope();
    FormatterScope(const FormatterScope&) = delete;
    FormatterScope& operator=(const FormatterScope&) = delete;
};

// One quiescent owner, serialized nonrecursive use; no thread synchronization.
// Modern borrow only: keeps recursive formatter release from invalidating the
// real existing loop. It neither constructs a String nor implements formatting.
class FormatterUse {
    FormatterScope* owner_ = nullptr;
public:
    FormatterUse();
    ~FormatterUse();
    const Dependencies& get() const noexcept;
    FormatterUse(const FormatterUse&) = delete;
    FormatterUse& operator=(const FormatterUse&) = delete;
};
bool formatter_claim_admitted() noexcept;
bool ini_detach_admitted() noexcept;
} // namespace kotor_canonical_string_format
