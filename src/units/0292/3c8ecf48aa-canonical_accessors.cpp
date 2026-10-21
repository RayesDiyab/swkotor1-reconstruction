#include "a5016d1616-binding.h"
#include "6e25a82454-startup_views.h"
#include "ca45871aaa-normal.h"
#include "ace6bb46fb-global_aliases.h"
#include <type_traits>
static_assert(sizeof(menu_normal_40_49::Providers)==160,"one exact GUI provider table");
static_assert(offsetof(menu_normal_40_49::Providers,resource_007a39e8)==128,"resource cell address field");
static_assert(offsetof(menu_normal_40_49::Providers,empty_0073d71c)==152,"borrowed empty literal field");
static_assert(std::is_same<decltype(menu_normal_40_49::Providers::resource_007a39e8),task10_resource_cells::ManagerCell>::value,"sole typed volatile Manager cell declaration");
static_assert(std::is_same<decltype(menu_normal_40_49::Providers::empty_0073d71c),const char*>::value,"existing literal pointer declaration");
static_assert(std::is_same<decltype(kotor_menu_10_19::dependencies::base_007a39e0),void*>::value,"canonical existing Base pointer slot");
static_assert(sizeof(menu_resources::Manager*)==4 && sizeof(kotor_menu_aliases::Cell32)==4,"native x86 cell transport");
extern "C" __declspec(noinline) menu_resources::Manager* __cdecl c2da_manager_007a39e8() {
    return menu_resources::BorrowCanonicalManager007a39e8();
}
extern "C" __declspec(noinline) void* __cdecl key_debug_root_007a39e0() {
    kotor_menu_aliases::Cell32 cell(&kotor_menu_10_19::dependencies::base_007a39e0);
    return reinterpret_cast<void*>(cell.load());
}
extern "C" __declspec(noinline) const char* __cdecl c2da_empty_0073d71c() {
    return menu_normal_40_49::providers.empty_0073d71c;
}
