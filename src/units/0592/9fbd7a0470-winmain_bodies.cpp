// Authored WinMain-direct leaves (swkotor.exe 9c10e045...1435, dumpbin evidence in
// lane-routes/evidence/full.asm.txt). Each body cites its instruction range.
#include "81dc466724-winmain_routes.h"
#include "27f068960c-startup_string.h"
#include "255876c9ce-string_priority.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include "8af36a49a4-allocator_live.h"

namespace {
using Word = std::uint32_t;
Word word_at(Word address, Word offset) { Word v; std::memcpy(&v, reinterpret_cast<const void*>(address + offset), 4); return v; }
void put_word(Word address, Word offset, Word v) { std::memcpy(reinterpret_cast<void*>(address + offset), &v, 4); }
// [[007A39FC]+4]: the outer CClientExoApp, reloaded at every original use.
void* outer_client() { return reinterpret_cast<void*>(word_at(kotor_winmain::bound_context->globals->manager_7a39fc.load(), 4)); }
const float scale_0073ede4 = 255.0f;   // .rdata 0073EDE4
const char ecf_0073d608[] = "-ecf";
const char epf_0073d5f8[] = "-epf";
const char epr_0073d5f0[] = "-epr";
const char epm_0073d5e8[] = "-epm";
const char capture_0073d600[] = "capture";
const char missing_007414a0[] = "ERROR:  file '%s' doesn't exist";
const char done_00741484[] = "Done executing file '%s'";
}

// 004015B0..004015EB StoreMouseTrailsState: SPI(5Eh,0,&v,0) with v=0 first; if the call
// succeeds and v>=1 store v at 007A3CA8 then SPI(5Dh,0,0,0).
extern "C" void __cdecl kotor_cdecl_004015b0() {
    int value = 0;
    if (SystemParametersInfoA(0x5e, 0, &value, 0) && value >= 1) {
        kotor_winmain::bound_context->globals->mouse_trails_7a3ca8 = value;
        SystemParametersInfoA(0x5d, 0, nullptr, 0);
    }
}
// 00401610..0040164B StoreScreenSaverState: SPI(10h,0,&v,0); v>=1 -> 007A3CAC, SPI(11h,0,0,0).
extern "C" void __cdecl kotor_cdecl_00401610() {
    int value = 0;
    if (SystemParametersInfoA(0x10, 0, &value, 0) && value >= 1) {
        kotor_winmain::bound_context->globals->screen_saver_7a3cac = value;
        SystemParametersInfoA(0x11, 0, nullptr, 0);
    }
}

// 00401700..0040186B checkCommandLineArgs(argc, argv). _strncmp 006FAFF0 / _atol 006FAFE7
// are CRT library functions (Ghidra library matches): UCRT strncmp/atol are used.
// String temporaries: 005E5A90 ctor, 005E5140 assign (M40 @GuiAssign005e5140@12), 005E5C20
// dtor; the original EH states 0/1 destroy them on unwind, as the C++ automatic objects do.
extern "C" void __cdecl kotor_cdecl_00401700(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        if (std::strncmp(argv[i], ecf_0073d608, 4) == 0) {                // 0040173F
            ++i;
            kotor_reconstruction::StartupString name(capture_0073d600);    // 00401755
            if (i < argc) GuiAssign005e5140(&name, nullptr, argv[i]);      // 0040176E
            kotor_fastcall_005ed820(outer_client(), nullptr, &name);       // 00401781
        } else if (std::strncmp(argv[i], epf_0073d5f8, 4) == 0) {         // 004017A7
            ++i;
            kotor_reconstruction::StartupString name(capture_0073d600);    // 004017BD
            if (i < argc) GuiAssign005e5140(&name, nullptr, argv[i]);      // 004017D6
            kotor_fastcall_005ed830(outer_client(), nullptr, &name);       // 004017E8
        } else if (std::strncmp(argv[i], epr_0073d5f0, 4) == 0) {         // 0040180B
            ++i;
            std::int32_t rate = 0x1e;                                      // 0040181A
            if (i < argc) rate = static_cast<std::int32_t>(std::atol(argv[i])); // 00401825
            kotor_fastcall_005ed840(outer_client(), nullptr, rate);        // 00401837
        } else {
            (void)std::strncmp(argv[i], epm_0073d5e8, 4);                  // 00401849, result unused
        }
    }
}

// 0044EF00..0044EF55 AurSetPauseOverlayColor(c0,c1,c2,c3), plain RET (caller cleans).
// Each channel: fld arg; fmul [0073EDE4]=255.0; call 006FAE8C (existing FTOL body). Order
// c3,c2,c1,c0; result ((((f3<<8)|f2)<<8|f1)<<8)|f0 (unmasked OR, as 0044EF1C..0044EF4C).
namespace {
Word scaled_ftol(const float* value) {
    Word result;
    __asm {
        mov ecx, value
        fld dword ptr [ecx]
        fmul dword ptr scale_0073ede4
        call unresolved_ftol2_006fae8c
        mov result, eax
    }
    return result;
}
}
extern "C" void __cdecl kotor_cdecl_0044ef00(float c0, float c1, float c2, float c3) {
    Word color = scaled_ftol(&c3);
    color <<= 8; color |= scaled_ftol(&c2);
    color <<= 8; color |= scaled_ftol(&c1);
    color <<= 8; color |= scaled_ftol(&c0);
    *winmain_routes::cells.pause_overlay_0078e394 = color;
}

// 0044DA60..0044DA7E AurEnableHeadLight: if word[0078E388]!=1 glEnable(4000h); word=1.
extern "C" void __cdecl kotor_cdecl_0044da60() {
    if (*winmain_routes::cells.headlight_0078e388 != 1) glEnable(0x4000);
    *winmain_routes::cells.headlight_0078e388 = 1;
}
// 0044DA80..0044DA9E AurDisableHeadLight: if word[0078E388]!=0 glDisable(4000h); word=0.
extern "C" void __cdecl kotor_cdecl_0044da80() {
    if (*winmain_routes::cells.headlight_0078e388 != 0) glDisable(0x4000);
    *winmain_routes::cells.headlight_0078e388 = 0;
}

// 0044D6A0..0044D6CE AurRenderStrings: PostTraceStrings; glLoadIdentity; count read once
// from 007B9100; for n=count..1: Post([[007B90FC]+4n-4]) (table cell reloaded each pass);
// tail JMP countreset 0046EE00.
extern "C" void __cdecl kotor_cdecl_0044d6a0() {
    kotor_cdecl_0044d620();
    glLoadIdentity();
    for (Word n = *winmain_routes::cells.posted_count_007b9100; n != 0; --n) {
        const Word table = *winmain_routes::cells.posted_table_007b90fc;
        kotor_fastcall_0044d010(reinterpret_cast<void*>(word_at(table, n * 4 - 4)), nullptr);
    }
    kotor_cdecl_0046ee00();
}

// 0044C980 dofile: single body = lane-aurres _aurres_dofile_0044c980 (D59/D65 C2); this lane name is
// joined to it by /alternatename in m45-joins/integrator_joins_m45.cpp (cdecl char*(const char*) both).
