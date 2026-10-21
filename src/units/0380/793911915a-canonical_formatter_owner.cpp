#include "ba9c8cfd29-canonical_formatter_owner.h"
#include "9c615f09ed-allocator_live.h"
#include "5d98675ec4-modern_warning_primitives.h"
#include "9c02a993fc-functional_runtime.h"
#include "f61a42fe8f-modern_locale.h"
#include "8d2c715466-base_ini.h"
#include <cstring>
#include <exception>
#include <type_traits>

namespace kotor_canonical_string_format {
// Actual original cold cells are loader-zero. No eager formatter allocation,
// copied per-consumer capacity, generated data or second original storage.
char* volatile format_buffer_008328a8 = nullptr;
Word volatile format_capacity_008328ac = 0;
namespace {
FormatterScope* active_scope = nullptr;
char* __cdecl copy_text(char* output,const char* input,Word count) {
    return std::strncpy(output,input,count); // Real same-/MT CRT support.
}
[[noreturn]] void broken() { std::terminate(); }
}
extern const Dependencies dependencies = {
    &live_operator_new,&live_free,&kotor_functional::modern_warning_vsnprintf,
    &copy_text,&format_buffer_008328a8,&format_capacity_008328ac
};
static_assert(std::is_same<decltype(Dependencies::allocate_006fa7e6),decltype(&live_operator_new)>::value,
              "Exact source allocator cdecl Word carrier");
static_assert(std::is_same<decltype(Dependencies::free_006fa390),decltype(&live_free)>::value,
              "Exact source free cdecl carrier");
static_assert(std::is_same<decltype(Dependencies::vsnprintf_006fd5a8),
                          decltype(&kotor_functional::modern_warning_vsnprintf)>::value,
              "Existing legacy _vsnprintf boundary, no C99 replacement");
static_assert(std::is_same<decltype(Dependencies::strncpy_006fab80),decltype(&copy_text)>::value,
              "Exact supported x86 CRT copy transport");

bool FormatterScope::identity() const noexcept {
    return phase_ != Phase::Inactive && active_scope == this && borrowed_heap_ &&
        borrowed_heap_cell_ && live_globals.heap == borrowed_heap_cell_ &&
        *live_globals.heap == borrowed_heap_ && kotor_functional::source_heap_identity() &&
        borrowed_locale_ && borrowed_locale_->identity() && kotor_functional::locale_identity() &&
        dependencies.allocate_006fa7e6 == &live_operator_new && dependencies.free_006fa390 == &live_free &&
        dependencies.vsnprintf_006fd5a8 == &kotor_functional::modern_warning_vsnprintf &&
        dependencies.strncpy_006fab80 == &copy_text &&
        dependencies.format_buffer_008328a8 == &format_buffer_008328a8 &&
        dependencies.format_capacity_008328ac == &format_capacity_008328ac;
}
bool FormatterScope::claim(const kotor_functional::LocaleScope& locale) noexcept {
    if (phase_ != Phase::Inactive || active_scope || active_uses_ ||
        !kotor_functional::source_heap_identity() || !locale.identity() ||
        !kotor_functional::locale_identity() || !live_globals.heap || !*live_globals.heap ||
        format_buffer_008328a8 || format_capacity_008328ac) return false;
    borrowed_heap_cell_ = live_globals.heap;
    borrowed_heap_ = *live_globals.heap;
    borrowed_locale_ = &locale;
    phase_ = Phase::Active;
    active_scope = this;
    return true;
}
bool formatter_claim_admitted() noexcept {
    return active_scope && active_scope->phase_ == FormatterScope::Phase::Active && active_scope->identity();
}
bool ini_detach_admitted() noexcept {
    return active_scope && active_scope->phase_ == FormatterScope::Phase::DrainAdmitted &&
        !active_scope->active_uses_ && active_scope->identity();
}
bool FormatterScope::prepare_release(DrainCheck drain,void* context) noexcept {
    if (phase_ != Phase::Active || !identity() || active_uses_ || !drain || !context) return false;
    phase_ = Phase::CheckingDrain;
    const bool complete = drain(context);
    // Public admission/release APIs cannot alter CheckingDrain recursively.
    // A foreign mutation of retained heap/Locale is an intentional boundary.
    if (phase_ != Phase::CheckingDrain || !identity() || active_uses_) broken();
    phase_ = complete ? Phase::DrainAdmitted : Phase::Active;
    return complete;
}
bool FormatterScope::finish_release() noexcept {
    if (phase_ != Phase::DrainAdmitted || !identity() || active_uses_ ||
        base_ini_reconstruction::bindings.assign_string_005e5c50 ||
        base_ini_reconstruction::bindings.format_string_005e5680) return false;
    phase_ = Phase::Releasing; // HeapFree's opaque callback cannot reopen/release.
    char* buffer = format_buffer_008328a8;
    if (buffer) live_free(buffer);
    if (phase_ != Phase::Releasing || !identity() || active_uses_) broken();
    format_buffer_008328a8 = nullptr;
    format_capacity_008328ac = 0;
    active_scope = nullptr;
    phase_ = Phase::Inactive;
    borrowed_heap_ = nullptr;
    borrowed_heap_cell_ = nullptr;
    borrowed_locale_ = nullptr;
    return true;
}
FormatterScope::~FormatterScope() {
    // Automatic destruction never fabricates completed consumer drain.
    if (phase_ != Phase::Inactive) broken();
}
FormatterUse::FormatterUse() {
    FormatterScope* owner = active_scope;
    if (!owner || !owner->identity() ||
        (owner->phase_ != FormatterScope::Phase::Active && owner->phase_ != FormatterScope::Phase::CheckingDrain) ||
        owner->active_uses_ != 0) broken();
    owner_ = owner;
    ++owner->active_uses_;
}
FormatterUse::~FormatterUse() {
    if (!owner_ || !owner_->identity() || !owner_->active_uses_ ||
        (owner_->phase_ != FormatterScope::Phase::Active && owner_->phase_ != FormatterScope::Phase::CheckingDrain)) broken();
    --owner_->active_uses_;
}
const Dependencies& FormatterUse::get() const noexcept {
    if (!owner_ || !owner_->identity() || !owner_->active_uses_) broken();
    return dependencies;
}
} // namespace kotor_canonical_string_format
