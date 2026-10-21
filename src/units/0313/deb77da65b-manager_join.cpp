#include "cfd53d6e4f-joins.h"
#include <type_traits>
extern "C" menu_resources::Manager* __cdecl c2da_manager_007a39e8();
static_assert(std::is_same<decltype(&rim_current_manager_007a39e8),kotor_missing_odd::ManagerGate>::value,"canonical Manager accessor slot");
extern "C" menu_resources::Manager* __cdecl rim_current_manager_007a39e8() {
    // Borrow the existing accessor; no duplicate cell, snapshot or publisher.
    // Main6 must establish the actual Manager lifetime behind the returned bits.
    return c2da_manager_007a39e8();
}
