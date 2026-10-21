// lane-m46-startup: texture scheduler lowers declared in texture-epoch/include/scheduler.h
// (0041E9C0 InsertionSortTexture, 0045DE10 ImageDecode, 0045F0D0 ImageFillAlpha, 0041F9B0 LoadImagePLT)
// and the two ImageDecode-private cells 007B9334 imagesDecoded / 007BB510 shortCircuitDecode.
// Evidence: lane-m46-startup/dis/<addr>.asm (swkotor.exe GOG K1). Float code follows D30-FINAL:
// x87 wide intermediates are doubles, float only at the original `fstp dword` stores, frndint/fistp
// are round-to-nearest (std::nearbyint under the default FE_TONEAREST mode, original CW 0x027F).
#include "50aeb789e5-scheduler.h"
#include "54a084bbbf-texture_cells.hpp"
#include "d08b97bcca-allocator_live.h"     // live_operator_new = 006FA7E6
#include <cmath>
#include <cstdint>
#include <cstring>

namespace kotor_texture_lowers {
using texture::U;
using texture::I;
using texture::Basic;

// 007B9334 imagesDecoded (.bss 0) and 007BB510 shortCircuitDecode (.bss 0). Full-image scan of
// swkotor.exe: the only references are 0045DE1A/0045DE2A (read+write 007B9334) and 0045DE20 (read
// 007BB510) - no writer of 007BB510 exists in the image. Single owner: this unit.
U volatile images_decoded_007b9334 = 0;
U volatile short_circuit_decode_007bb510 = 0;

inline float load_f20(U word) {   // [basic+20h] float read through the Basic* held as a DWORD
    const Basic* basic = reinterpret_cast<const Basic*>(static_cast<std::uintptr_t>(word));
    return basic->f20;
}
inline std::int32_t round_nearest_i32(double value) {   // frndint + fistp dword (RC = nearest)
    return static_cast<std::int32_t>(std::nearbyint(value));
}
}  // namespace kotor_texture_lowers

using namespace kotor_texture_lowers;
namespace texture_scheduler {

// 0041E9C0..0041EA1C InsertionSortTexture(CAurTextureBasic** list, int count), cdecl.
// for i=1..count-1: key=list[i]; if !(key.f20 > list[i-1].f20) (fcomp + test ah,41h: <=, == or
// unordered) continue; j=i-1; do { j--; swap(list[j+1], list[j+2]); if (j<0) break; } while
// (key.f20 > list[j].f20)  (0041E9F0..0041EA11). Descending by +20h float; NaN stops the walk.
extern "C" void __cdecl scheduler_sort_words_0041e9c0(U* list, I count) {
    for (I i = 1; i < count; ++i) {                                               // 0041E9C6..0041E9CD, 0041EA13..0041EA16
        const U key = list[i];                                                    // 0041E9D5
        if (!(load_f20(key) > load_f20(list[i - 1]))) continue;                   // 0041E9D8..0041E9EA
        I j = i - 1;                                                              // 0041E9E2
        for (;;) {
            --j;                                                                  // 0041E9F0
            const U lower = list[j + 1];                                          // 0041E9F1
            const U upper = list[j + 2];                                          // 0041E9F5
            list[j + 2] = lower;                                                  // 0041E9F9
            list[j + 1] = upper;                                                  // 0041E9FD
            if (j < 0) break;                                                     // 0041EA01 js
            if (!(load_f20(key) > load_f20(list[j]))) break;                      // 0041EA03..0041EA11
        }
    }
}

// 0045DE10..0045DF04 ImageDecode(src, width, height, palettes, unused, index_list, palette_count), cdecl.
//  ++[007B9334]; if [007BB510] return 0.  out = new(width*height*4).  Copy palette_count 256-DWORD
//  palettes palettes+index_list[k]*1024 into the 0x3C00-byte stack table (0045DE80..0045DEA2), then
//  each 16-bit little-endian source index selects table DWORD (hi*256+lo) (0045DED0..0045DEE9).
//  The 5th argument is never read. Quirks kept: index >= 0xF00 or palette_count > 15 address beyond the
//  original 0x3C00-byte local (not guarded in the original either); width<=0 rows still advance src by
//  2*width (0045DEEB).
extern "C" void* __cdecl scheduler_decode_0045de10(void* source, I width, I height, const unsigned char* palettes,
                                                                     I unused_fifth, U* index_list, I palette_count) {
    (void)unused_fifth;
    images_decoded_007b9334 = images_decoded_007b9334 + 1;                        // 0045DE1A..0045DE28
    if (short_circuit_decode_007bb510 != 0) return nullptr;                       // 0045DE20..0045DE38
    U table[0xF00];                                                               // [esp+18h], 3C00h bytes (__chkstk 006FC090)
    U* const out = static_cast<U*>(live_operator_new(static_cast<U>(width * height) << 2));   // 0045DE4B..0045DE54
    for (I k = 0; k < palette_count; ++k)                                         // 0045DE67..0045DEA2
        std::memcpy(&table[k * 0x100], palettes + (index_list[k] << 10), 0x400);
    U* dest = out;                                                                // 0045DEB4
    const unsigned char* row = static_cast<const unsigned char*>(source);         // 0045DEBA
    for (I y = height; y > 0; --y) {                                              // 0045DEB2..0045DEF4
        const unsigned char* p = row;
        for (I x = width; x > 0; --x) {                                           // 0045DEC5..0045DEE9
            const U index = (static_cast<U>(p[1]) << 8) + p[0];                   // 0045DED0..0045DEDA
            *dest++ = table[index];                                               // 0045DEDC..0045DEE2
            p += 2;
        }
        row += width + width;                                                     // 0045DEEB..0045DEEE
    }
    return out;                                                                   // 0045DEF6
}

// 0045F0D0..0045F29F ImageFillAlpha(image, width, height, bytes_per_pixel, const float color[3]), cdecl.
//  Only bpp == 4 and height > 0. Per pixel (k = [0073EDE0] = 1/255 as binary32, 0x3B808081):
//   r'=r*k (wide), g'=g*k (wide), b'=(float)(b*k), a=(float)(a*k), inv=1-a (wide, [0073D764]=1.0)
//   R=(float)((float)(a*r') + (float)(inv*cx)); G=(float)(a*g' + (float)(inv*cy)); B=(float)(a*b' + (float)(inv*cz))
//   each channel: (float)(C*255.0) -> frndint -> fistp; low byte stored; alpha byte = 0FFh.
//  The R integer is written into the caller's 4th argument slot (0045F110/0045F214) - a callee-owned
//  cdecl slot, modelled as the local `bpp_slot`.
extern "C" void __cdecl scheduler_fill_alpha_0045f0d0(void* image, I width, I height, U bytes_per_pixel, const float* color) {
    if (bytes_per_pixel != 4) return;                                             // 0045F0D7..0045F0DA
    if (height <= 0) return;                                                      // 0045F0E0..0045F0E6
    const float k = 0.0039215688593685627f;                                       // [0073EDE0] 8180803Bh
    const float one = 1.0f;                                                       // [0073D764]
    const float scale = 255.0f;                                                   // [0073EDE4]
    unsigned char* const base = static_cast<unsigned char*>(image);
    U row_offset = 0;                                                             // 0045F0FC [esp+1Ch]
    for (I rows = height; rows != 0; --rows) {                                    // 0045F104, 0045F285..0045F292
        if (width > 0) {                                                          // 0045F108..0045F10A
            unsigned char* p = base + row_offset;                                 // 0045F12C..0045F137 (eax = p+1)
            for (I n = width; n != 0; --n) {                                      // 0045F147, 0045F25E..0045F26E
                const double r_wide = static_cast<double>(p[0]) * k;              // 0045F150..0045F167
                const double g_wide = static_cast<double>(p[1]) * k;              // 0045F158..0045F179
                const float b_f = static_cast<float>(static_cast<double>(p[2]) * k);   // 0045F17F..0045F18D [esp+54h]
                const float a_f = static_cast<float>(static_cast<double>(p[3]) * k);   // 0045F191..0045F19B [esp+18h]
                const double inv = static_cast<double>(one) - a_f;                // 0045F19F..0045F1A5
                const float cx = static_cast<float>(inv * color[0]);              // 0045F1A9..0045F1AE [esp+34h]
                const float cy = static_cast<float>(inv * color[1]);              // 0045F1B2..0045F1B7 [esp+38h]
                const float cz = static_cast<float>(inv * color[2]);              // 0045F1BB..0045F1BE [esp+3Ch]
                const float ar = static_cast<float>(a_f * r_wide);                // 0045F1C2..0045F1C8 [esp+58h]
                const double ag = a_f * g_wide;                                   // 0045F1CC..0045F1D4 (kept on the stack)
                const double ab = static_cast<double>(a_f) * b_f;                 // 0045F1D6..0045F1DA
                const float R = static_cast<float>(static_cast<double>(ar) + cx); // 0045F1DE..0045F1E6 [esp+40h]
                const float G = static_cast<float>(ag + cy);                      // 0045F1EA..0045F1F0 [esp+44h]
                const float B = static_cast<float>(ab + cz);                      // 0045F1F4..0045F1F8 [esp+48h]
                const float r255 = static_cast<float>(static_cast<double>(R) * scale);   // 0045F1FC..0045F206
                const std::int32_t bpp_slot = round_nearest_i32(r255);           // 0045F20A..0045F214 -> [esp+74h]
                const float g255 = static_cast<float>(static_cast<double>(G) * scale);   // 0045F216..0045F220
                const std::int32_t g_int = round_nearest_i32(g255);              // 0045F224..0045F22E -> [esp+10h]
                const float b255 = static_cast<float>(static_cast<double>(B) * scale);   // 0045F230..0045F23A
                const std::int32_t b_int = round_nearest_i32(b255);              // 0045F23E..0045F248 -> [esp+14h]
                p[0] = static_cast<unsigned char>(bpp_slot);                      // 0045F24A..0045F24E
                p[1] = static_cast<unsigned char>(g_int);                         // 0045F251..0045F255
                p[2] = static_cast<unsigned char>(b_int);                         // 0045F257..0045F25B
                p[3] = 0xff;                                                      // 0045F262
                p += 4;                                                           // 0045F266
            }
        }
        row_offset += static_cast<U>(width) * 4u;                                 // 0045F274..0045F283 (width reloaded)
    }
}

// 0041F9B0..0041FA2F CAurTextureBasic::LoadImagePLT (thiscall, RET 0).
//  [vtable+B0] FreeImage (0041F9B4..0041F9B7, result ignored); if derived[0] == 0 copy name -> derived
//  (unbounded byte copy 0041F9CD..0041F9DC); r = [0078D41C](derived, &+68, &+6C, &local) (local is the
//  pushed ECX slot, never read afterwards); +70 = 2; +44 = r; +74 = 0; +E3 = ([007A46F8] != 0 && r != 0).
extern "C" void __fastcall scheduler_load_plt_0041f9b0(Basic* self, void*) {
    texture::texture_call_free_b0(self);                                          // 0041F9B4..0041F9B7
    if (self->derived[0] == 0) {                                                  // 0041F9BD..0041F9CB
        const char* from = self->name;
        char* to = self->derived;
        char c;
        do { c = *from++; *to++ = c; } while (c != 0);                            // 0041F9D4..0041F9DC
    }
    acquisition::Signed layers;                                                   // [esp+8] (push ecx at 0041F9B0)
    char* const raster = aurora_cell_0078d41c(self->derived, &self->f68, &self->f6c, &layers);   // 0041F9DE..0041F9EC
    self->f70 = 2;                                                                // 0041F9F2
    self->f44 = static_cast<U>(reinterpret_cast<std::uintptr_t>(raster));         // 0041F9FF
    self->f74 = 0;                                                                // 0041FA02
    self->flags[11] = (aurora_cell_007a46f8 != nullptr && raster != nullptr) ? 1 : 0;   // 0041FA09..0041FA26 (+E3)
}
}  // namespace texture_scheduler
