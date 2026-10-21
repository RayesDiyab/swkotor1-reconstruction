#include "ea366da309-ini_lifetime.h"
#include "5fb9bb198a-ini_remaining_join.h"
#include "967a8c4a8f-string_transforms.h"
#include "7fa9bc840c-warning_runtime.h"
#include "61ae7fa68b-canonical_formatter_owner.h"
#include "a1c18d6a8c-alias_add.h"
#include "cea03ab609-baseinternal.h"
#include "4459defebf-ini_c_primitives.h"
#include "f48e4ce86b-ini_file_primitives.h"
#include "056ae408ac-string_join.h"
#include "d735aa7dd1-string_priority.h"
#include "40141ae87f-ini_cleanup.h"
#include "908fcc9941-functional_runtime.h"
#include "7edb7fc583-ini_addhead.h"
#include "2246169d43-ini_section.h"
#include "c46300269d-allocator_live.h"
#include "77e8b6221f-menu_targets.h"
#include "f5434bb15b-modern_locale.h"
#include <exception>
#include <type_traits>

namespace kotor_functional {
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::next_005e9b90),
                          decltype(&base_ini_reconstruction::next_005e9b90)>::value,"Exact genuine Next carrier");
static_assert(std::is_same<decltype(base_ini_reconstruction::WriteBindings::concat_005e5d10),
                          decltype(&GuiConcat005e5d10)>::value,"Exact potentially throwing genuine Concat carrier");
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::right_005e6170),
                          decltype(&base_ini_reconstruction::right_005e6170)>::value,"Exact genuine Right carrier");
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::left_005e5f70),
                          decltype(&base_ini_reconstruction::left_005e5f70)>::value,"Exact genuine Left carrier");
// Exact canonical Assign needs only the admitted source allocator/free.
// Format publication requires its retained coupled-cell owner admission.
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::assign_string_005e5c50),
                          decltype(&kotor_reconstruction::StartupString::Assign005e5c50)>::value,"Exact canonical AssignString member carrier");
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::format_string_005e5680),
                          decltype(&kotor_reconstruction::StartupFormat005e5680)>::value,"Exact admitted genuine variadic Format carrier");
static_assert(sizeof(base_ini_reconstruction::Bindings)==112 && sizeof(base_ini_reconstruction::WriteBindings)==12 &&
              offsetof(base_ini_reconstruction::Bindings,next_005e9b90)==80 &&
              offsetof(base_ini_reconstruction::Bindings,right_005e6170)==88 &&
              offsetof(base_ini_reconstruction::Bindings,left_005e5f70)==92 &&
              offsetof(base_ini_reconstruction::WriteBindings,concat_005e5d10)==0,"Original sole table slot layout");
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::add_alias_005e7760), decltype(&alias_add::AddAlias005e7760)>::value,"Exact genuine Alias108 fastcall carrier");
static_assert(sizeof(base_ini_reconstruction::Bindings)==112 && offsetof(base_ini_reconstruction::Bindings,add_alias_005e7760)==108,"Actual Alias108 table slot unchanged");
static_assert(std::is_same<decltype(base_ini_reconstruction::WriteBindings::add_head_005e9d70),
                          decltype(&ini_addhead::AddHead005e9d70)>::value,"Exact genuine AddHead carrier");
static_assert(offsetof(base_ini_reconstruction::WriteBindings,add_head_005e9d70)==4 &&
              sizeof(base_ini_reconstruction::WriteBindings)==12,"Original Write4 table slot");
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::section_005ea600),
                          decltype(&ini_section::ConstructSection005ea600)>::value,"Exact genuine Section carrier");
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::add_tail_005e9dc0),
                          decltype(&ini_section::AddTail005e9dc0)>::value,"Exact genuine AddTail carrier");
static_assert(offsetof(base_ini_reconstruction::Bindings,section_005ea600)==32 &&
              offsetof(base_ini_reconstruction::Bindings,add_tail_005e9dc0)==76 &&
              sizeof(base_ini_reconstruction::Bindings)==112,"Original table slots unchanged");
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::internal_005e8ba0),
                          decltype(&base_resource_table::ConstructBaseInternal005e8ba0)>::value,"Exact genuine BaseInternal carrier");
static_assert(sizeof(base_ini_reconstruction::Bindings)==112 &&
              offsetof(base_ini_reconstruction::Bindings,internal_005e8ba0)==24 &&
              sizeof(decltype(base_ini_reconstruction::Bindings::internal_005e8ba0))==4,
              "Canonical BaseInternal table slot");
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::compare_nc_005e58c0),
                          decltype(&ini_comparison::CompareNC005e58c0)>::value,"Exact genuine CompareNC carrier");
static_assert(sizeof(base_ini_reconstruction::Bindings)==112 &&
              offsetof(base_ini_reconstruction::Bindings,compare_nc_005e58c0)==44,
              "Canonical Main44 table slot");
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::random_005ea420),
                          decltype(&startup_rand::ConstructRand005ea420)>::value,"Exact genuine Rand carrier");
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::timers_005e91e0),
                          decltype(&kotor_startup::ConstructTimers005e91e0)>::value,"Exact genuine Timers carrier");
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::aliases_005e6810),
                          decltype(&alias_path::ConstructAlias005e6810)>::value,"Exact genuine Alias carrier");
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::debug_005e9370),
                          decltype(&kotor_menu20::ConstructDebug005e9370)>::value,"Exact genuine Debug carrier");
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::destroy_005ea650),
                          decltype(&base_ini_reconstruction::destroy_cache_005ea650)>::value,"Exact cleanup fastcall carrier");
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::assign_cstr_005e5140),
                          decltype(&GuiAssign005e5140)>::value,"Exact assignment fastcall carrier");
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::sprintf_006fadb0),
                          decltype(&modern_ini_sprintf)>::value,"Exact cdecl variadic callback");
static_assert(std::is_same<decltype(base_ini_reconstruction::Bindings::chr_006fb5e0),
                          decltype(&modern_ini_strchr)>::value,"Exact cdecl character lookup callback");
namespace {
IniProviderScope* active_scope = nullptr; // One scalar owner, no second table.
bool allocator_matches() noexcept {
    return source_heap_identity() &&
        base_ini_reconstruction::bindings.allocate_006fa7e6 == &live_operator_new;
}
bool providers_empty() noexcept {
    const auto& b = base_ini_reconstruction::bindings;
    const auto& w = base_ini_reconstruction::write_bindings;
    return !b.debug_005e9370 && !b.timers_005e91e0 && !b.aliases_005e6810 &&
        !b.ini_005e6750 && !b.random_005ea420 && !b.internal_005e8ba0 &&
        !b.list_005ea4d0 && !b.section_005ea600 && !b.c_str_005e5670 &&
        !b.sprintf_006fadb0 && !b.compare_nc_005e58c0 && !b.destroy_005ea650 &&
        !b.assign_string_005e5c50 && !b.assign_cstr_005e5140 && !b.open_006fad9d &&
        !b.gets_006fc469 && !b.seek_006fbb19 && !b.close_006facf0 &&
        !b.add_tail_005e9dc0 && !b.next_005e9b90 && !b.chr_006fb5e0 &&
        !b.right_005e6170 && !b.left_005e5f70 && !b.equals_005e5290 &&
        !b.length_005e5790 && !b.format_string_005e5680 && !b.add_alias_005e7760 &&
        !w.concat_005e5d10 && !w.add_head_005e9d70 && !w.print_006faf01;
}
bool destruction_matches(const base_ini_reconstruction::PublicIni& ini) noexcept {
    return active_scope && active_scope->current_heap_matches() &&
        kotor_menu20::bindings.free_006fa390 == &live_free &&
        (!ini.internal || kotor_menu20::bindings.ini_destroy_005eb500);
}
}

bool IniProviderScope::claim() noexcept {
    if (claimed_ || active_scope || !allocator_matches() || !providers_empty() || !locale_identity() ||
        !kotor_canonical_string_format::formatter_claim_admitted()) return false;
    borrowed_heap_ = *live_globals.heap;
    active_scope = this;
    claimed_ = true;
    base_ini_reconstruction::bindings.sprintf_006fadb0 = &modern_ini_sprintf;
    base_ini_reconstruction::bindings.chr_006fb5e0 = &modern_ini_strchr;
    base_ini_reconstruction::bindings.open_006fad9d = &modern_ini_open;
    base_ini_reconstruction::bindings.gets_006fc469 = &modern_ini_gets;
    base_ini_reconstruction::bindings.seek_006fbb19 = &modern_ini_seek;
    base_ini_reconstruction::bindings.close_006facf0 = &modern_ini_close;
    base_ini_reconstruction::write_bindings.print_006faf01 = &modern_ini_print;
    base_ini_reconstruction::write_bindings.add_head_005e9d70 = &ini_addhead::AddHead005e9d70;
    // Publish the three genuine typed String carriers only after claim proved
    // all borrowed slots empty and captured the current stable heap HANDLE.
    ini_string_join::bind(base_ini_reconstruction::bindings);
    base_ini_reconstruction::bindings.destroy_005ea650 = &base_ini_reconstruction::destroy_cache_005ea650;
    base_ini_reconstruction::bindings.assign_cstr_005e5140 = &GuiAssign005e5140;
    base_ini_reconstruction::bindings.ini_005e6750 = &base_ini_reconstruction::PublicIni::construct_005e6750;
    base_ini_reconstruction::bindings.list_005ea4d0 = &base_ini_reconstruction::ListWrapper::construct_005ea4d0;
    base_ini_reconstruction::bindings.debug_005e9370 = &kotor_menu20::ConstructDebug005e9370;
    base_ini_reconstruction::bindings.timers_005e91e0 = &kotor_startup::ConstructTimers005e91e0;
    base_ini_reconstruction::bindings.aliases_005e6810 = &alias_path::ConstructAlias005e6810;
    base_ini_reconstruction::bindings.compare_nc_005e58c0 = &ini_comparison::CompareNC005e58c0;
    base_ini_reconstruction::bindings.random_005ea420 = &startup_rand::ConstructRand005ea420;
    base_ini_reconstruction::bindings.section_005ea600 = &ini_section::ConstructSection005ea600;
    base_ini_reconstruction::bindings.add_tail_005e9dc0 = &ini_section::AddTail005e9dc0;
    base_ini_reconstruction::bindings.internal_005e8ba0 = &base_resource_table::ConstructBaseInternal005e8ba0;
    // Callable publication only. Actual Base/PublicIni/AliasList referents must
    // be genuinely published and alive before AddAlias executes.
    base_ini_reconstruction::bindings.add_alias_005e7760 = &alias_add::AddAlias005e7760;
    // Reuse the existing selected attachment once, only after the original
    // empty-table, heap and Locale admission. No installer/table clone.
    base_ini_reconstruction::install_existing_ini_joins(
        base_ini_reconstruction::bindings,base_ini_reconstruction::write_bindings);
    base_ini_reconstruction::bindings.right_005e6170 = &base_ini_reconstruction::right_005e6170;
    base_ini_reconstruction::bindings.left_005e5f70 = &base_ini_reconstruction::left_005e5f70;
    // Actual canonical String member, after unchanged occupancy/heap/Locale
    // admission. Its sole algorithm borrows the existing live allocator/free.
    base_ini_reconstruction::bindings.assign_string_005e5c50 =
        &kotor_reconstruction::StartupString::Assign005e5c50;
    // One exact Format104 address, after the retained owner admission above.
    base_ini_reconstruction::bindings.format_string_005e5680 =
        &kotor_reconstruction::StartupFormat005e5680;
    return true;
}

bool IniProviderScope::current_heap_matches() const noexcept {
    return claimed_ && active_scope == this && allocator_matches() &&
        borrowed_heap_ && *live_globals.heap == borrowed_heap_ && locale_identity();
}

IniProviderScope::~IniProviderScope() {
    if (!claimed_) return;
    if (!current_heap_matches() || !kotor_canonical_string_format::ini_detach_admitted()) std::terminate();
    // GameLease's genuine release has already completed. Allocation remains
    // bootstrap-owned until IniAllocatorBinding's later destructor.
    auto& b = base_ini_reconstruction::bindings;
    b.debug_005e9370 = nullptr;
    b.timers_005e91e0 = nullptr;
    b.aliases_005e6810 = nullptr;
    b.ini_005e6750 = nullptr;
    b.random_005ea420 = nullptr;
    b.internal_005e8ba0 = nullptr;
    b.list_005ea4d0 = nullptr;
    b.section_005ea600 = nullptr;
    b.c_str_005e5670 = nullptr;
    b.sprintf_006fadb0 = nullptr;
    b.compare_nc_005e58c0 = nullptr;
    b.destroy_005ea650 = nullptr;
    b.assign_string_005e5c50 = nullptr;
    b.assign_cstr_005e5140 = nullptr;
    b.open_006fad9d = nullptr;
    b.gets_006fc469 = nullptr;
    b.seek_006fbb19 = nullptr;
    b.close_006facf0 = nullptr;
    b.add_tail_005e9dc0 = nullptr;
    b.next_005e9b90 = nullptr;
    b.chr_006fb5e0 = nullptr;
    b.right_005e6170 = nullptr;
    b.left_005e5f70 = nullptr;
    b.equals_005e5290 = nullptr;
    b.length_005e5790 = nullptr;
    b.format_string_005e5680 = nullptr;
    b.add_alias_005e7760 = nullptr;
    auto& w = base_ini_reconstruction::write_bindings;
    w.concat_005e5d10 = nullptr;
    w.add_head_005e9d70 = nullptr;
    w.print_006faf01 = nullptr;
    active_scope = nullptr;
}

bool try_destroy_borrowed_ini(base_ini_reconstruction::PublicIni& ini) {
    if (!destruction_matches(ini)) return false;
    ini.destroy_005e67e0(); // Existing task19 algorithm, one canonical receiver.
    return true;
}

bool try_release_heap_ini(base_ini_reconstruction::PublicIni* ini) {
    if (!ini || !try_destroy_borrowed_ini(*ini)) return false;
    ini->~PublicIni();
    live_free(ini); // Outer4-byte allocation, distinct from the internal12 bytes.
    return true;
}
}
