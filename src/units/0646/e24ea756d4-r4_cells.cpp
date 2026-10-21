// lane-r4-lowers: sole storage for original cells consumed by the R4 MainLoop/GUI
// frame and its lowers that have NO owner in M41 or in any lane packet (census:
// m41-symbols.json + claude-takeover lane sources, 2026-10-06). Values are the
// pinned swkotor.exe image words (.data file bytes; .bss = loader zero).
// Cells with an existing owner are NOT defined here; PACKET.json lists the
// /alternatename joins for those (007A39FC, 007A39E0, 0078E574, 007A39E8, 007A39EC).
#include "e6446a7354-r4_lowers.h"

namespace menu_continuation {
U32 high_resolution_low_008328f0 = 0;      // .bss; writers 00603DB2.. (MainLoop profiling)
U32 high_resolution_high_008328f4 = 0;     // .bss
const float zero_0073d700 = 0.0f;          // .rdata 00000000
const float scale_007439bc = 1e-06f;       // .rdata 358637BD
const float threshold_0073edf0 = 0.25f;    // .rdata 3E800000
}
namespace frame_control {
U32 draw_tail_007a3d4c = 0;                // .data 0; no writer in the image (DrawSafeRegion gate)
U32 end_timer_low_008328e8 = 0;            // .bss
U32 end_timer_high_008328ec = 0;           // .bss
float draw_elapsed_00832914 = 0.0f;        // .bss
const float profile_scale_0073d708 = 0.001f; // .rdata 3A83126F
U32 snapshot_enabled_007a2284 = 1;         // .data 00000001
float sound_elapsed_00832918 = 0.0f;       // .bss
const float milliseconds_0073d6fc = 1000.0f; // .rdata 447A0000
U32 log_request_008328d8[2] = {0, 0};      // .bss CExoString {data,length}
U32 message_manager_007a3a00 = 0;          // .data 00000000
U32 quiet_mode_0083291c = 0;               // .bss
}
namespace frame_log {
extern const char empty_0073d5d4[] = "a";
extern const char filename_0074edcc[] = "HD0:FILEERROR";
extern const char header_0074ed88[] = "Star Wars Knights of the Old Republic Patch 2 Error Logging File\n";
extern const char header_0074ed30[] = "This file has been generated to help provide information on read/write irregularities\n";
extern const char format_0074ed28[] = "\n%s\n%s";
extern const char error_0074ed00[] = "%sERROR - Failed to open error file\n";
extern const char clear_0073d71c[] = "";
}
namespace kotor_input_cells_required {
// 008338F4: .bss 0; writers 004BA88D / 0061F88B / 0062053B / 00621AEC; read by
// ShowMouse 0061FA6C. Declared (owner unassigned) by lane-input client_input_event.h.
volatile std::int32_t word_008338f4 = 0;
}
namespace r4_lowers_cells {
PostedList posted_list_007b90fc = {0, 0, 0};          // .bss 007B90FC..007B9107 {data,count,alloc}; Add via AurPostString 0044D490, Remove 0044D1C2
volatile float arrow_pulse_timer_0078d3e8 = 0.75f;   // .data 3F400000; only 00418300 reads/writes
volatile std::uint8_t arrow_pulse_up_007a457c = 0;   // .bss; only 00418300
volatile float arrow_pulse_alpha_0078d3e4 = 1.0f;    // .data 3F800000; written 00418374
std::uint32_t volatile trace_table_007b9108 = 0;     // .bss
std::int32_t volatile trace_count_007b910c = 0;      // .bss; writer 0044D2BC
std::uint32_t volatile string_color_00798bb4[4] = {0x3F800000u, 0x3F800000u, 0x3F800000u, 0x3F800000u}; // .data
std::uint32_t volatile render_counts_00827fa8[6] = {0, 0, 0, 0, 0, 0}; // .bss 00827FA8..00827FBC
std::uint32_t volatile render_count_007b92a8 = 0;    // .bss
}
