// lane-m48-traps / f-keys: AurPrintString 0044CE50 (__cdecl, text, column, row) and its bitmap-font text path.
// Consumers: r4-lowers aur_strings.cpp (PostedString::Post 0044D164: void __cdecl kotor_cdecl_0044ce50(void*, U32, U32))
// and WinMain's console line 00404886. Callees authored here: GLRender::SetUpTextFormat 0044CB10,
// GLRender::PrintStringAtScreenPos 0044CB90, AurInitFont 0044CC40, SpitLetters 00432040 (OpenGL 1.1 imports called
// directly through opengl32.lib: same entry points as the original IAT slots cited per call).
// D83 branch trap: the texture-font branch 0044CF81 (PrintStringScreenPos_UseTextureFont 0044CCA0 -> CAurFont::TextOutA)
// is unreachable in the whole program: see FORK.json traps (byte 0078E35C is never written non-zero, and every write
// that makes 007B90F0 non-zero is preceded by a store of 0 to 0078E35C).
// Glyph bitmaps: generated/debug_font_0078d498.h (D13 local-only, generated/gen_debug_font.py).
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <cstdint>
#include <cstring>
#include "5907a9dfa7-kotor_unreachable_trap.h"
#include <kotor_generated/font/debug_font_0078d498.h>

using U32 = std::uint32_t;

namespace kotor_aurora_window { extern U32 font_loaded_007b90f0; extern U32 font_lists_007a47c0; }   // aurora-window owner (.bss)
namespace kotor_renderer_cells { extern std::uint32_t disable_0078d45c; }                            // GL_LIGHTING word (0B50h)

namespace kotor_aur_print_cells {
// 0078E360: print-enabled word, .data 1. Only other writer: CSWSModule::LoadModuleFinish 004C5954 (= 0, server module load).
std::uint32_t volatile print_enabled_0078e360 = 1u;
// 0078E35C: use-texture-font byte, .data 01. Writers: 0044CC40 (= 0), 0044CF62 (= 0); no non-zero writer exists.
volatile std::uint8_t use_texture_font_0078e35c = 1u;
}

extern "C" {
void __cdecl kotor_cdecl_0044cb10();
void __cdecl kotor_cdecl_0044cc40();
void __cdecl kotor_cdecl_00432040(const char* text);
void __cdecl kotor_cdecl_0044cb90(const char* text, float x, float y);
void __cdecl kotor_cdecl_0044ce50(const char* text, std::int32_t column, std::int32_t row);

// ---- 0044CB10 GLRender::SetUpTextFormat ----
void __cdecl kotor_cdecl_0044cb10() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);                                       // 0044CB15..1C (0CF5h, IAT 0073D338)
    kotor_aurora_window::font_lists_007a47c0 = glGenLists(0x80);                 // 0044CB22..39 (IAT 0073D334)
    const unsigned char* glyph = kotor_debug_font::glyphs_0078d498;              // 0044CB43 edi = 0078D498
    U32 list = 0x20u;                                                            // 0044CB3E esi = 20h
    for (int count = 0x5F; count != 0; --count) {                                // 0044CB48 / 0044CB82..87
        glNewList(kotor_aurora_window::font_lists_007a47c0 + list, GL_COMPILE);  // 0044CB50..5D (1300h, IAT 0073D330)
        glBitmap(8, 0xD, 0.0f, 2.0f, 10.0f, 0.0f, glyph);                        // 0044CB5F..72 (IAT 0073D32C)
        glEndList();                                                             // 0044CB74 (IAT 0073D328)
        ++list;                                                                  // 0044CB7E
        glyph += 0xD;                                                            // 0044CB7F
    }
}
// ---- 0044CC40 AurInitFont ----
void __cdecl kotor_cdecl_0044cc40() {
    kotor_aur_print_cells::use_texture_font_0078e35c = 0;                        // 0044CC40
    kotor_cdecl_0044cb10();                                                      // 0044CC47
    kotor_aurora_window::font_loaded_007b90f0 = 1;                               // 0044CC4C
}
// ---- 00432040 SpitLetters ----
void __cdecl kotor_cdecl_00432040(const char* text) {
    if (!kotor_aurora_window::font_loaded_007b90f0) kotor_cdecl_0044cc40();      // 00432040..49
    glPushAttrib(GL_LIST_BIT);                                                   // 0043204F..54 (20000h, IAT 0073D268)
    glListBase(kotor_aurora_window::font_lists_007a47c0);                        // 0043205A..60 (IAT 0073D320)
    glCallLists(static_cast<GLsizei>(std::strlen(text)), GL_UNSIGNED_BYTE, text); // 00432066..80 (1401h, IAT 0073D31C)
    glPopAttrib();                                                               // 00432087 (tail JMP, IAT 0073D26C)
}
// ---- 0044CB90 GLRender::PrintStringAtScreenPos(text, x, y) ----
void __cdecl kotor_cdecl_0044cb90(const char* text, float x, float y) {
    glPushAttrib(0x40140u);                                                      // 0044CB92..97 (40140h = TEXTURE_BIT|DEPTH_BUFFER_BIT|LIGHTING_BIT)
    glDisable(GL_TEXTURE_2D);                                                    // 0044CBA3..A8 (0DE1h, IAT 0073D22C)
    glDisable(GL_LIGHTING);                                                      // 0044CBAA..AF (0B50h)
    glDisable(GL_DEPTH_TEST);                                                    // 0044CBB1..B6 (0B71h)
    glMatrixMode(GL_PROJECTION);                                                 // 0044CBBE..C3 (1701h, IAT 0073D228)
    glPushMatrix();                                                              // 0044CBC5 (IAT 0073D300)
    glLoadIdentity();                                                            // 0044CBCB (IAT 0073D230)
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);                                      // 0044CBD1..F5 (IAT 0073D234)
    glRasterPos2f(x, y);                                                         // 0044CBFB..0B (IAT 0073D340)
    kotor_cdecl_00432040(text);                                                  // 0044CC0D..12
    glRasterPos2f(0.0f, 0.0f);                                                   // 0044CC1A..1E
    glPopMatrix();                                                               // 0044CC20 (IAT 0073D2FC)
    glMatrixMode(GL_MODELVIEW);                                                  // 0044CC26..2B (1700h)
    glPopAttrib();                                                               // 0044CC2F (tail JMP, IAT 0073D26C)
}
// ---- 0044CE50 AurPrintString(text, column, row) ----
void __cdecl kotor_cdecl_0044ce50(const char* text, std::int32_t column, std::int32_t row) {
    if (!kotor_aur_print_cells::print_enabled_0078e360) return;                  // 0044CE50..5A
    GLint viewport[4];                                                           // [esp+0Ch]
    glGetIntegerv(GL_VIEWPORT, viewport);                                        // 0044CE61..6B (0BA2h, IAT 0073D244)
    if (viewport[2] < 0x12C) return;                                             // 0044CE71..7B (signed JL)
    if (viewport[3] < 0x64) return;                                              // 0044CE81..88 (signed JL)
    const std::int32_t rows = viewport[3] / 14;                                  // 0044CE8E..A3 (magic 92492493h, SAR 3)
    const std::int32_t columns = viewport[2] / 10;                               // 0044CEA5..BA (magic 66666667h, SAR 2)
    if (row >= rows) row = rows - 1;                                             // 0044CEB6..CA (signed JL skips)
    if (row < 0) {                                                               // 0044CECE..D4
        row += rows;                                                             // 0044CED6..D8
        if (row < 0) row = 0;                                                    // 0044CEDC..DE (JNS)
    }
    if (column < 0)                                                              // 0044CEE6..F0
        column = column + (columns - static_cast<std::int32_t>(std::strlen(text))) + 1;  // 0044CEF2..08
    if (static_cast<std::int32_t>(std::strlen(text)) + column > columns)         // 0044CF0C..1E (signed JLE)
        column = columns - static_cast<std::int32_t>(std::strlen(text));         // 0044CF20..32
    if (column < 0) column = 0;                                                  // 0044CF36..3A
    const std::int32_t from_bottom = rows - row - 1;                             // 0044CF42..4F (stored over the row slot)
    glDisable(kotor_renderer_cells::disable_0078d45c);                           // 0044CF46..53 ([0078D45C])
    if (!kotor_aurora_window::font_loaded_007b90f0) {                            // 0044CF59..60
        kotor_aur_print_cells::use_texture_font_0078e35c = 0;                    // 0044CF62
        kotor_cdecl_0044cb10();                                                  // 0044CF69
        kotor_aurora_window::font_loaded_007b90f0 = 1;                           // 0044CF6E
    }
    if (kotor_aur_print_cells::use_texture_font_0078e35c)                        // 0044CF78..7F
        kotor_unreachable_trap(0x0044CF81ul, "AurPrintString@0044CF81 texture-font path (0044CCA0 PrintStringScreenPos_UseTextureFont): "
                                             "0078E35C is never written non-zero and is cleared before every non-zero store to 007B90F0");
    if (text && *text) {                                                         // 0044CFCD..D4
        // FILD/FIDIV then FSTP float (x87, PC53 in the original): integer quotient in double, rounded once to float.
        const float y = static_cast<float>(static_cast<double>(from_bottom) / static_cast<double>(rows));     // 0044CFD6..E1
        const float x = static_cast<float>(static_cast<double>(column) / static_cast<double>(columns));       // 0044CFE5..ED
        kotor_cdecl_0044cb90(text, x, y);                                        // 0044CFF0..F6
    }
    glEnable(kotor_renderer_cells::disable_0078d45c);                            // 0044CFF9..D000 ([0078D45C], IAT 0073D390)
}
}
