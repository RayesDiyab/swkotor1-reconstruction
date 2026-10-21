// lane-w3 explicit D83 traps for the client object path (see w3_traps.cpp for the convention).
#include "0bc18821ad-w3_client_objects.h"

#define W3_TRAP(addr, name) kotor_unreachable_trap(addr, name)

extern "C" {
// ---- (t2) 005F2BB0, 005F5CF0, 0063F9D0, 006364C0, 00635480, 006349F0, 005E9BD0 and 006A0920 are lane-w4 bodies
// (kotor_fastcall_*); the former lane-w3 traps were removed (coordinator ruling t2-3).
// ---- GameObjectUpdate leaves not in this delivery
}
