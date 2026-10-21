// lane-m48-b fork CLIENT, unit client-resolution: ChangeResolutionForMovie 00403CF0 / RestoreResolutionFromMovie 00403DE0
// (lane-config intro-queue consumer, movies-enabled path 00602B6D / 00602BAE; routes table row 00403316) and
// CClientExoAppInternal::HideMoviePlayerFadePanel 005F4C50.
// Evidence: dumpbin /disasm swkotor.exe 00403CF0..00403DD9, 00403DE0..00403F11, 005F4C50..005F4C5F; .data image values
// (dumpbin /rawdata) 0078D1F0 = 280h, 0078D1F4 = 1E0h, 0078D1F8 = 20h, 0078D1FC = 1; 007A3C54/007A3CB8/007A3CBC are
// file-backed zero (.data raw size 17000h covers them). IAT: 0073D1E8 GetCurrentThread, 0073D1F4 SetThreadPriority
// (original kernel32 imports, D6 precedent). Renderer slot +04 is read through the published typed renderer table (D41).
#include "88d4066358-renderer_adoption.h"
#include "df72b7ab87-window_shared_cells.h"
#include "fd73ec7604-window_live_views.h"
#include "35e3e29e81-live_publication.h"
#include <cstdint>
#include <cstring>

namespace { using U32 = std::uint32_t; using I32 = std::int32_t; using U8 = std::uint8_t; }

namespace kotor_renderer_gl_cells { extern std::uint32_t width_0078d1d4; }   // gl-renderer-gl-cells owner
namespace kotor_movie { extern U32 volatile movie_restore_007a3cc0; }        // lane-m47-core rev2 movie_player owner
namespace kotor_menu20 {   // token-identical to canonical menu_targets.h / input-hover hover_path.h (task-19 menu-startup-20-29)
using Word=std::uint32_t;
struct ClientInternal {Word unknown;void* volatile options;};
struct ClientApp {Word unknown;ClientInternal* volatile internal;__declspec(noinline) void* __thiscall get_options_005ed700();};
}
namespace options_video { struct Client;
    I32 __fastcall ReadVideoModeSettings_005f0ce0(Client* self, void* unused_edx, I32* width, I32* height, U8* depth, U32* refresh); }
namespace kotor_renderer_init { I32 __cdecl SetAntiAliasingMode_0044f2f0(I32); }
extern "C" {
void __cdecl kotor_window_reinit_aurora_00403800(U32 width, U32 height, U32 depth, U32 fullscreen, U32 check_movies); // D46
void __cdecl kotor_cdecl_00401e00();                                         // ActivateRenderWindow (aurora-window)
void __fastcall kotor_fastcall_0070c220(void* player, void*);                // CExoMoviePlayer::HideFadePanel (startup46)
void __cdecl kotor_cdecl_00403cf0();
void __cdecl kotor_cdecl_00403de0();
void __fastcall kotor_fastcall_005f4c50(void* internal, void*);
}

// Cells with no prior owner (single storage here).
namespace kotor_menu_window_cells {
extern U32 movie_flag_007a3c54;           // written 0 at 00403DA8; set by ReadVideoModeSettings path (task-28 video donor)
U32 movie_flag_007a3c54 = 0;
}
namespace kotor_movie_resolution_cells {
extern volatile I32 nesting_007a3cb8;
extern volatile U32 changed_007a3cbc;
extern I32 saved_width_0078d1f0;
extern I32 saved_height_0078d1f4;
extern U8 saved_depth_0078d1f8;
extern volatile U32 reread_0078d1fc;
volatile I32 nesting_007a3cb8 = 0;        // ChangeResolutionForMovie depth counter (00403D01 inc / 00403E34 dec)
volatile U32 changed_007a3cbc = 0;        // 1 when the fullscreen branch ran (00403D38 / 00403DC2)
I32 saved_width_0078d1f0 = 0x280;
I32 saved_height_0078d1f4 = 0x1e0;
U8 saved_depth_0078d1f8 = 0x20;  // byte stores 00403D1F; bytes 0078D1F9..FB are image zero
volatile U32 reread_0078d1fc = 1;
}
using namespace kotor_movie_resolution_cells;

namespace {
void* word_ptr(const void* p, U32 off) { void* v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
U32 word(const void* p, U32 off) { U32 v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
void* client() { return word_ptr(kotor_menu_window_cells::manager_007a39fc, 4); }   // [[007A39FC]+4]
kotor_renderer_init::Renderer* aurora() { return static_cast<kotor_renderer_init::Renderer*>(kotor_menu_window_cells::aurora_007a39f8); }
const renderer_adoption::VTable* aurora_table(kotor_renderer_init::Renderer* r) {
    return reinterpret_cast<const renderer_adoption::VTable*>(static_cast<std::uintptr_t>(r->vtable_word));
}
U8 current_depth() { return kotor_menu_window_attachment->views->winmain.depth_78d1e0; }   // 0078D1E0
}

// 00403CF0 ChangeResolutionForMovie (no arguments, plain RET).
extern "C" void __cdecl kotor_cdecl_00403cf0() {
    nesting_007a3cb8 = nesting_007a3cb8 + 1;                            // 00403CF0 / 00403D01..00403D02
    saved_height_0078d1f4 = static_cast<I32>(kotor_menu_window_cells::height_0078d1d8); // 00403CF6 / 00403D0E
    saved_width_0078d1f0 = static_cast<I32>(kotor_renderer_gl_cells::width_0078d1d4);   // 00403CFC / 00403D1A
    saved_depth_0078d1f8 = current_depth();                             // 00403D08 / 00403D1F
    kotor_renderer_init::Renderer* r = aurora();                        // 00403D14
    if (aurora_table(r)->fullscreen(r, nullptr) == 0) {                 // 00403D25..00403D2C
        changed_007a3cbc = 0;                                           // 00403DC2
        SetThreadPriority(GetCurrentThread(), -2);                      // 00403DC0..00403DD3
        return;
    }
    changed_007a3cbc = 1;                                               // 00403D38
    void* internal = word_ptr(client(), 4);                             // 00403D32..00403D4D
    bool change = true;
    if (kotor_menu_window_cells::movie_flag_007a3c54 != 0 &&            // 00403D45..00403D50
        word(internal, 0x1f8) == 0 && word(internal, 0x1fc) == 0)       // 00403D52..00403D64
        change = false;
    if (change && !(kotor_renderer_gl_cells::width_0078d1d4 == 0x280 && // 00403D66..00403D70
                    kotor_menu_window_cells::height_0078d1d8 == 0x1e0 && // 00403D72..00403D7C
                    current_depth() == 0x20)) {                         // 00403D7E..00403D85
        kotor_renderer_init::SetAntiAliasingMode_0044f2f0(0);           // 00403D87..00403D89
        kotor_window_reinit_aurora_00403800(0x280, 0x1e0, 0x20, 1, 0);  // 00403D8E..00403DA3
    }
    kotor_menu_window_cells::movie_flag_007a3c54 = 0;                   // 00403DA8
    SetThreadPriority(GetCurrentThread(), -2);                          // 00403DA6 / 00403DB2..00403DB9
}

// 005F4C50 CClientExoAppInternal::HideMoviePlayerFadePanel: player [this+12Ch]; tail call 0070C220 when non-null.
extern "C" void __fastcall kotor_fastcall_005f4c50(void* internal, void*) {
    if (void* player = word_ptr(internal, 0x12c)) kotor_fastcall_0070c220(player, nullptr); // 005F4C50..005F4C5A
}

// 00403DE0 RestoreResolutionFromMovie (no arguments, plain RET).
extern "C" void __cdecl kotor_cdecl_00403de0() {
    void* internal = word_ptr(client(), 4);                             // 00403DE1..00403DEA
    if (internal && word(internal, 0x4bc) != 0) {                       // 00403DED..00403DF9
        SetThreadPriority(GetCurrentThread(), 0);                       // 00403DFB..00403E04
        kotor_fastcall_005f4c50(internal, nullptr);                     // 00403E0A..00403E0C
        if (changed_007a3cbc == 1 && kotor_movie::movie_restore_007a3cc0 == 0) // 00403E11..00403E25
            kotor_cdecl_00401e00();                                     // 00403E2F (tail jump)
        return;
    }
    const I32 depth = nesting_007a3cb8 - 1;                             // 00403E34 (dec; jns)
    nesting_007a3cb8 = depth;
    if (depth < 0) { nesting_007a3cb8 = 0; return; }                    // 00403E3A..00403E48
    SetThreadPriority(GetCurrentThread(), 0);                           // 00403E49..00403E52
    if (internal) kotor_fastcall_005f4c50(internal, nullptr);           // 00403E58..00403E5E
    if (reread_0078d1fc != 0) {                                         // 00403E63..00403E6A
        reread_0078d1fc = 0;                                            // 00403E6E (also when internal is null)
        if (internal) {                                                 // 00403E6C / 00403E78
            U32 refresh = 0;   // D2 (00403E7A): out-only local [esp+4]; the callee writes it, the value is never read here
            options_video::ReadVideoModeSettings_005f0ce0(static_cast<options_video::Client*>(internal), nullptr,
                                                          &saved_width_0078d1f0, &saved_height_0078d1f4,
                                                          &saved_depth_0078d1f8, &refresh); // 00403E7A..00403E90 (cell addresses)
        }
    }
    if (changed_007a3cbc != 1 || kotor_movie::movie_restore_007a3cc0 != 0) return; // 00403E95..00403EA5
    if (saved_width_0078d1f0 == 0x280 && saved_height_0078d1f4 == 0x1e0 && saved_depth_0078d1f8 == 0x20) { // 00403EA7..00403EC6
        kotor_cdecl_00401e00();                                         // 00403ECC (tail jump)
        return;
    }
    void* options = static_cast<kotor_menu20::ClientApp*>(client())->get_options_005ed700(); // 00403ED1..00403ED9
    kotor_renderer_init::SetAntiAliasingMode_0044f2f0(static_cast<I32>(static_cast<const U8*>(options)[0x44])); // 00403EDE..00403EE3
    kotor_window_reinit_aurora_00403800(static_cast<U32>(saved_width_0078d1f0), static_cast<U32>(saved_height_0078d1f4),
                                        saved_depth_0078d1f8, changed_007a3cbc, 0); // 00403EE8..00403F0C
}
