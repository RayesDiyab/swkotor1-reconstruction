// 005EDE90 — CClientExoApp::PlayerFlourishWeapons (8 bytes, 0x005EDE90..0x005EDE98).
//
// Original body is an 8-byte forwarding thunk to 005F4460 (raw instruction bytes omitted).
// Exact accepted 0769 import:
//   ?player_flourish_weapons_005ede90@kotor_input_required@@YIXPAX0@Z
//
// ABI: __fastcall(ECX=CClientExoApp*, EDX unused, 0 stack args), RET 0 inherited.
// The 651-byte 005F4460 body is nested-providers owned; this unit binds the exact
// consumer symbol @kotor_fastcall_005f4460@8 and authors no part of it.
#include "1f393e8651-ui_save_state_types.h"

namespace kotor_input_required {

void __fastcall player_flourish_weapons_005ede90(void* app, void*) {
    void* const internal = ui_load_at<void*>(app, 4);
    kotor_fastcall_005f4460(internal, nullptr);
}

}  // namespace kotor_input_required
