// lane-resource-lowers: the sole immutable resource_ref_assignment::bindings
// descriptor required by task-2 assign6120-r3/assignment.cpp (assignment.h:10).
// It selects the EXISTING genuine lowercase provider _kotor_lower_006fb766
// (M40 resource-lowercase.obj, key-chain/lowercase.cpp). No replacement locale,
// no second lowercase body. Original evidence: 00406120 lowers each of the 16
// Ref bytes through CALL 006FB766 (_tolower, cdecl, one int argument, caller
// pops) -- see assign6120-r3 MACHINE-ABI-REVIEW.json.
#include "896b5ec2dc-assignment.h"
#include "e16cd58d1e-lowercase.h"
#include <type_traits>
static_assert(std::is_same<resource_ref_assignment::Lowercase,decltype(&kotor_lower_006fb766)>::value,
 "descriptor slot is exactly the genuine cdecl int(int) provider type; no cast");
namespace resource_ref_assignment {
// Constant (address) initialization only: no dynamic initializer, so the
// descriptor is valid before any C++ static constructor runs.
extern const Bindings bindings{&kotor_lower_006fb766};
}
