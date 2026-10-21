// lane-w3 explicit D83 traps for the creature appearance path (see w3_traps.cpp for the convention).
#include "8fdb3c6e33-w3_client_appearance.h"

#define W3_TRAP(addr, name) kotor_unreachable_trap(addr, name)

extern "C" {
// (empty) 0063C760 was rebound to lane-w4 kotor_fastcall_0063c760 (coordinator ruling t2-3); its only lane-w3 caller,
// 00610A70 PlayDrawAnimation, is now lane-w4 kotor_fastcall_00610a70 (ruling t2-5). No appearance-unit traps remain.
}
