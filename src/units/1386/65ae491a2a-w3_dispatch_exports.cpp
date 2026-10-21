// lane-w3 tier t2: dispatcher-facing ABI check (coordinator ruling t2-2).
// Only the reviewer's merged patch (lane-w3rev/merged-dispatcher/client_player_dispatch_native.merged.patch) is
// applied. It declares and calls these six symbols directly; all six are DEFINED by the lane-w3 handler units under
// exactly these names, so the former forwarding veneers of this TU (recovered_client_login_0064ec00,
// recovered_client_module_006684f0, recovered_client_area_0064bab0, recovered_client_game_object_update_0066a4a0 -
// lane-w2's earlier route names, no longer referenced by the merged patch) were deleted:
//   0064EC00 recovered_client_handle_login_0064ec00              w3_client_module.cpp
//   006684F0 recovered_client_handle_module_006684f0             w3_client_module.cpp
//   0064BAB0 recovered_client_handle_area_0064bab0               w3_client_area_load.cpp
//   0066A4A0 recovered_client_handle_game_object_update_0066a4a0 w3_client_objects.cpp
//   00668640 recovered_client_player_list_00668640               w3_client_t2.cpp (t2; W3 owns it, ruling t2-9)
//   0064BF30 recovered_client_custom_token_0064bf30              w3_client_t2.cpp (t2; W3 owns it, ruling t2-9)
// lane-w2's w2_client_handler_pending_stubs.cpp (and the reviewer's trimmed copy, which still traps 00668640 and
// 0064BF30) must NOT be linked together with w3_client_t2.cpp: it defines the same two symbols.
// Original CSWCMessage handlers are thiscall(BYTE minor) RET 4 -> __fastcall(self, EDX, minor) returning EAX.
// The checks below pin each W3 definition to the merged patch's declaration type
// `extern "C" unsigned __fastcall f(void*, void*, unsigned)` (noexcept(false) is part of neither function type).
#include "835c32b896-w3_client_t2.h"
#include <type_traits>

namespace {
using MergedPatchHandler = unsigned(__fastcall*)(void*, void*, unsigned);
static_assert(std::is_same<decltype(&recovered_client_handle_login_0064ec00), MergedPatchHandler>::value, "0064EC00");
static_assert(std::is_same<decltype(&recovered_client_handle_module_006684f0), MergedPatchHandler>::value, "006684F0");
static_assert(std::is_same<decltype(&recovered_client_handle_area_0064bab0), MergedPatchHandler>::value, "0064BAB0");
static_assert(std::is_same<decltype(&recovered_client_handle_game_object_update_0066a4a0), MergedPatchHandler>::value,
              "0066A4A0");
static_assert(std::is_same<decltype(&recovered_client_player_list_00668640), MergedPatchHandler>::value, "00668640");
static_assert(std::is_same<decltype(&recovered_client_custom_token_0064bf30), MergedPatchHandler>::value, "0064BF30");
}  // namespace

// The TU defines one symbol so that it is never an empty object file.
extern "C" const unsigned recovered_w3_dispatch_abi_checked = 6;
