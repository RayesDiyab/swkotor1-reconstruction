#pragma once
#include "5d852003b2-manager_cell.h"
namespace task10_resource_cells {
// Borrow the sole selected cell; this header defines no storage or getter.
using ManagerCell=menu_resources::Manager* const volatile*;
inline ManagerCell canonical_cell() noexcept {
 return &kotor_resource_demand::resource_manager_007a39e8;
}
}
