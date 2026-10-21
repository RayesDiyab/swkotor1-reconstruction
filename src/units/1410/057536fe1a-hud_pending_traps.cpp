// Lane W4 HUD/G19-client: LINK-ONLY-IF-ABSENT trap stubs for cross-lane callees (no reconstruction credit).
// Remove each stub as soon as its owner's object provides the symbol (see integration/hud.json).
#include "3f50283d60-w4_native.hpp"
extern "C" {
unsigned __fastcall recovered_can_level_up_005a6810(void*, void*);   // W2 t5
int __fastcall kotor_fastcall_005a6810(void* stats, void*) { return static_cast<int>(recovered_can_level_up_005a6810(stats, nullptr)); }   // 1127 forwarder
}
