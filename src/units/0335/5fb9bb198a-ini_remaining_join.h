#pragma once
#include "2e42bf2295-ini_write.h"
#include "d4ed6ec73b-ini_list_next.h"
#include "d735aa7dd1-string_priority.h"
#include <type_traits>
namespace base_ini_reconstruction {
// Support-only attachment to the sole parent-owned tables; no storage owner.
void install_existing_ini_joins(Bindings&,WriteBindings&);
}
