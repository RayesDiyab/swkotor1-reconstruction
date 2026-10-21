#pragma once
#include "8670ef407f-w5_types.hpp"
extern "C" {
using w5::U;
U __fastcall w5_message_input_abort_drive_00525450(void* message, void*, void* player);         // RET 4
U __fastcall w5_message_input_cancel_gui_timing_00523570(void* message, void*, void* player);   // RET 4
U __fastcall w5_message_input_walk_to_waypoint_005235b0(void* message, void*, void* player);    // RET 4 (trap)
U __fastcall w5_message_input_dispatch_005254c0(void* message, void*, void* player, U minor);  // RET 8
}
