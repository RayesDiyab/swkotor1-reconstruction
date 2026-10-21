// lane-r4-lowers: Aurora posted/trace string lowers called by AurRenderStrings 0044D6A0
// (lane-routes winmain_bodies.cpp) every WinMain frame: PostTraceStrings 0044D620,
// PostedString::Post 0044D010, countreset 0046EE00. Evidence: dumpbin of swkotor.exe.
#include "e6446a7354-r4_lowers.h"
#include <cstdio>
#include <windows.h>
#include <GL/gl.h>

using namespace r4_lowers;

// 0046EE00 countreset, cdecl RET.
extern "C" void __cdecl kotor_cdecl_0046ee00() {
    texture_upload_cells::texture_binds_count_007a4710 = 0;        // 0046EE02
    r4_lowers_cells::render_counts_00827fa8[1] = 0;                // 0046EE07 (00827FAC)
    r4_lowers_cells::render_counts_00827fa8[2] = 0;                // 0046EE0C (00827FB0)
    r4_lowers_cells::render_counts_00827fa8[3] = 0;                // 0046EE11 (00827FB4)
    r4_lowers_cells::render_counts_00827fa8[4] = 0;                // 0046EE16 (00827FB8)
    r4_lowers_cells::render_counts_00827fa8[0] = 0;                // 0046EE1B (00827FA8)
    r4_lowers_cells::render_counts_00827fa8[5] = 0;                // 0046EE20 (00827FBC)
    r4_lowers_cells::render_count_007b92a8 = 0;                    // 0046EE25
}

// 0044D620 PostTraceStrings, cdecl RET. Frame 1: trace count 007B910C is 0 (.bss;
// sole writer 0044D2BC is the trace registration path), so only the gate byte is
// saved, cleared and restored.
extern "C" void __cdecl kotor_cdecl_0044d620() {
    char line[0x200];                                              // 0044D625 sub esp,200h
    const std::uint8_t saved = aurora_model::aurora_model_gate_007b8fc0; // 0044D62C
    aurora_model::aurora_model_gate_007b8fc0 = 0;                  // 0044D637
    I32 slot = -1;                                                 // 0044D642 or ebp,-1
    for (I32 i = 0; i < r4_lowers_cells::trace_count_007b910c; ++i, --slot) { // 0044D635..0044D63E / 0044D678..0044D684
        char* entry = reinterpret_cast<char*>(w(at(r4_lowers_cells::trace_table_007b9108), static_cast<U32>(i) * 4u)); // 0044D645..0044D64A
        char* value = kotor_cdecl_0044c1f0(entry);                 // 0044D650..0044D651
        std::sprintf(line, "%s: %s", reinterpret_cast<char*>(w(at(r4_lowers_cells::trace_table_007b9108), static_cast<U32>(i) * 4u)), value); // 0044D656..0044D664 (0074 14DC)
        kotor_cdecl_0044d490(line, 0, slot, 0);                    // 0044D669..0044D673
    }
    aurora_model::aurora_model_gate_007b8fc0 = saved;              // 0044D689
}

// 0044D010 PostedString::Post, RET (D1 form). Not reached on frame 1: posted count
// 007B9100 is 0 unless AurPostString 0044D490 ran (debug/console paths only).
// x87 arithmetic is reproduced in binary64 (CRT precision control 53-bit; D30).
extern "C" void __fastcall kotor_fastcall_0044d010(void* posted, void*) {
    const float total = f(posted, 0x408), remaining = f(posted, 0x404);
    const float t = total > 0.0f                                   // 0044D016..0044D027
        ? static_cast<float>((static_cast<double>(total) - remaining) / total) // 0044D029..0044D03B
        : 0.0f;                                                    // 0044D041
    const float u = static_cast<float>(1.0 - static_cast<double>(t)); // 0044D049..0044D059
    auto mix = [&](U32 a, U32 b) { return static_cast<float>(static_cast<double>(t) * f(posted, a) + static_cast<double>(u) * f(posted, b)); };
    const float alpha = mix(0x428, 0x418);                         // 0044D05D..0044D073
    const float blue = mix(0x424, 0x414);                          // 0044D077..0044D08D
    const float green = mix(0x420, 0x410);                         // 0044D091..0044D0A7
    const float red = mix(0x41c, 0x40c);                           // 0044D0AB..0044D0C1
    glColor4f(red, green, blue, alpha);                            // 0044D0C4 call [0073D224]
    U32 saved[4];
    for (int i = 0; i < 4; ++i) saved[i] = r4_lowers_cells::string_color_00798bb4[i]; // 0044D0D4..0044D0F5
    const float color[4] = {mix(0x41c, 0x40c), mix(0x420, 0x410), mix(0x424, 0x414), mix(0x428, 0x418)}; // 0044D0CA..0044D14F
    for (int i = 0; i < 4; ++i) { U32 bits; std::memcpy(&bits, &color[i], 4); r4_lowers_cells::string_color_00798bb4[i] = bits; }
    kotor_cdecl_0044ce50(posted, w(posted, 0x42c), w(posted, 0x430)); // 0044D155..0044D164
    for (int i = 0; i < 4; ++i) r4_lowers_cells::string_color_00798bb4[i] = saved[i]; // 0044D169..0044D17F
    const double left = static_cast<double>(f(posted, 0x404)) - menu_continuation::frame_delta_0078e574; // 0044D184..0044D18A
    sf(posted, 0x404, static_cast<float>(left));                   // 0044D193 fst (ST0 keeps the unrounded value)
    if (!(f(posted, 0x408) > 0.0f)) return;                        // 0044D199..0044D1AD
    if (!(left < 0.0)) return;                                     // 0044D1AF..0044D1BA (test ah,5 / jp)
    kotor_fastcall_00445080(&r4_lowers_cells::posted_list_007b90fc, nullptr, posted); // 0044D1BC..0044D1C2
    live_free(posted);                                             // 0044D1C7..0044D1C8 _free
}
