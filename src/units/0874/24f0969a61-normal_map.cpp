// lane-m48-a: bump-map normal-map build chain called from BuildTextureNormalMap 00420EA0 (texture-upload upload.cpp:215):
//   004260A0 GLRender::ConvertHeightFieldAndLoadNormalMapTexture (cdecl, 8 DWORDs; caller 00420F09 add esp,20h)
//   0041FEC0 convertHeightFieldToNormalMap (cdecl, 6 DWORDs, EAX = new RGBA buffer)
//   00420030 downSampleNormalMap          (cdecl, 5 DWORDs, EAX = new buffer; FREES its source at 00420423)
// Evidence: swkotor.exe GOG K1 dumpbin /disasm (lane-m48-a/d/004260A0.txt, d/0041FEC0.txt, d/00420030.txt).
// .rdata constants (pinned exe bytes): 0073EDE0 1/255 (8180803B), 0073EE00 0.00787402 (0402013C), 0073D764 1.0f,
// 0073EDDC 127.0f, 0073EDD8 128.0f, 0073D700 0.0f, 0073EDF8 0.0 (qword), 0073E9AC 0.5f, 0073EDF0 0.25f,
// 0073EDE8 1.0 (qword), 0073EDE4 255.0f.
// D30-FINAL: x87 runs at PC_53 (CW 0x027F): every x87 intermediate is a double here; float only where the original
// stores `fstp/fst dword`; fsqrt at 53-bit precision == std::sqrt(double) (IEEE correctly rounded); _ftol 006FAE8C
// through the M40 original-ABI port unresolved_ftol2_006fae8c (ST0 -> EDX:EAX, popped) with MSVC inline asm (D21; same
// helper shape as 3d/vp/vp_nv_ati.h vp_ftol2). Only AL of each conversion is stored (mov byte ptr [..],al).
// Allocator family: 006FA7E6 operator new / 006FC0D2 (jmp 006FA7E6) -> live_operator_new; 006FA390 free / 006FC0CD
// (jmp 006FA390) -> live_free (M41). No allocation result is tested (as original).
#include "536853f13e-renderer_gl_cells.h"
#include "2078381901-renderer_gl_imports.h"
#include "ceeafce552-renderer_gl_imports_gen.h"
#include <cmath>
#include <cstdint>
#include <cstring>

extern "C" {
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
unsigned __int64 __cdecl unresolved_ftol2_006fae8c();
}

namespace {
using U = std::uint32_t; using I = std::int32_t; using U8 = std::uint8_t;
constexpr float kInv255_0073ede0 = 0.003921568859368563f;   // 8180803B
constexpr float kInv127_0073ee00 = 0.007874015718698502f;   // 0402013C
constexpr float kOne_0073d764 = 1.0f, k127_0073eddc = 127.0f, k128_0073edd8 = 128.0f, kZero_0073d700 = 0.0f;
constexpr double kZero_0073edf8 = 0.0, kOne_0073ede8 = 1.0;
constexpr float kHalf_0073e9ac = 0.5f, kQuarter_0073edf0 = 0.25f, k255_0073ede4 = 255.0f;
// _ftol 006FAE8C on a wide value; the caller keeps AL only.
U8 ftol_byte(double value) {
    I result;
    __asm {
        fld qword ptr value
        call unresolved_ftol2_006fae8c
        mov result, eax
    }
    return static_cast<U8>(result);
}
} // namespace

extern "C" {
// 0041FEC0 convertHeightFieldToNormalMap(height map, width, height, map_width, map_height, scale) -> BGRA normal map.
void* __cdecl convert_height_field_to_normal_map_0041fec0(const U8* heights, I width, I height, I map_width,
                                                          I map_height, float scale) {
    U8* const out = static_cast<U8*>(live_operator_new(static_cast<U>(width) * static_cast<U>(height) * 4u)); // 0041FEC9..0041FED6
    if (height <= 0) return out;                                               // 0041FEE0..0041FEEA (EAX = out)
    I row_offset = 0;                                                          // 0041FEF6 ebp = y * map_width
    U8* row_out = out + 1;                                                     // 0041FEF8 [esp+18h] = out+1
    for (I y = 0; y < height; ++y) {                                           // [esp+14h] .. 00420011
        if (width > 0) {                                                       // 0041FF02..0041FF04
            const U8* const next_row = heights + ((y + 1) % map_height) * map_width; // 0041FF0C..0041FF1B
            U8* p = row_out;                                                   // esi
            for (I x = 0; x < width; ++x) {                                    // ecx / edi = x+1 .. 0041FFEF
                const double centre = static_cast<double>(heights[row_offset + x]) * kInv255_0073ede0;           // 0041FF23..0041FF39
                const double right = static_cast<double>(heights[row_offset + (x + 1) % map_width]) * kInv255_0073ede0; // 0041FF2B..0041FF59
                const double dx = (centre - right) * static_cast<double>(scale);   // 0041FF5F fsubr st,st(1); 0041FF61
                const double below = static_cast<double>(next_row[x]) * kInv255_0073ede0;                           // 0041FF4D..0041FF69
                const double dy = (centre - below) * static_cast<double>(scale);   // 0041FF6F fsubr st,st(2); 0041FF71
                const double inverse = static_cast<double>(kOne_0073d764) /
                    std::sqrt((dy * dy + dx * dx) + static_cast<double>(kOne_0073d764)); // 0041FF75..0041FF87
                const float inverse_stored = static_cast<float>(inverse);           // 0041FF8D fst dword [esp+10h]
                p[1] = ftol_byte(inverse * dy * k127_0073eddc + k128_0073edd8);     // 0041FF91..0041FF9F (wide inverse); 0041FFAA
                p[0] = ftol_byte(-(static_cast<double>(inverse_stored) * dx) * k127_0073eddc + k128_0073edd8); // 0041FFA6..0041FFC6
                p[-1] = ftol_byte(static_cast<double>(inverse_stored) * k127_0073eddc + k128_0073edd8); // 0041FFC8..0041FFDD
                p[2] = 0xFF;                                                   // 0041FFE4
                p += 4;                                                        // 0041FFEA
            }
        }
        row_offset += map_width;                                               // 0041FFFF add ebp,[esp+34h]
        row_out += static_cast<U>(width) * 4u;                                 // 00420003..00420017 (esi = width)
    }
    return out;                                                                // 00420021
}

// 00420030 downSampleNormalMap(source, width, height, new_width, new_height): alpha-weighted 2x2 average of the
// unpacked normals; frees `source`. Note: the next-row wrap uses (y+1) % width (00420081 idiv esi, esi = width).
void* __cdecl down_sample_normal_map_00420030(U8* source, I width, I height, I new_width, I new_height) {
    U8* const out = static_cast<U8*>(live_operator_new(static_cast<U>(new_width) * static_cast<U>(new_height) * 4u)); // 00420033..00420046
    I row_base = 0;                                                            // [esp+24h] pixel index y*width
    U8* row = source + 2;                                                      // [esp+28h]
    for (I y = 0; y < height; y += 2) {                                        // [esp+2Ch] .. 00420415
        if (width > 0) {                                                       // 00420075..00420077
            const I next_base = ((y + 1) % width) * width;                     // 0042007D..0042009E [esp+34h]
            const I out_row = (y >> 1) * new_width;                            // 00420083..004200A2 [esp+3Ch]
            const U8* p00 = row;                                               // esi (pixel (x,y) byte 2)
            const U8* p01 = source + next_base * 4 + 2;                        // edi (pixel (x,y+1) byte 2)
            for (I x = 0; x < width; x += 2) {                                 // ebp = x+1; 004203D0..004203DE
                const I right = (x + 1) % width;                               // 004200B8..004200BF
                const U8* const p10 = source + (row_base + right) * 4;         // eax (pixel base)
                const U8* const p11 = source + (right + next_base) * 4;        // ecx (pixel base)
                const float a0 = static_cast<float>(static_cast<double>(p00[1]) * kInv255_0073ede0); // 004200B0..004200C9 [14]
                const float a1 = static_cast<float>(static_cast<double>(p10[3]) * kInv255_0073ede0); // 004200DA..004200FD [18]
                const float a2 = static_cast<float>(static_cast<double>(p01[1]) * kInv255_0073ede0); // 004200E2..0042011E [1C]
                const float a3 = static_cast<float>(static_cast<double>(p11[3]) * kInv255_0073ede0); // 004200F9..0042012C [20]
                auto unpack = [](U8 b, float weight) {                         // fild; fmul 0073EE00; fsub 1.0; fmul weight
                    return (static_cast<double>(b) * kInv127_0073ee00 - kOne_0073d764) * static_cast<double>(weight);
                };
                double z = unpack(p10[2], a1) + unpack(p00[0], a0);            // 00420130..00420167 (byte 2: p10 then p00)
                z = z + unpack(p01[0], a2);                                    // 00420169..00420185
                z = z + unpack(p11[2], a3);                                    // 00420187..004201A3
                double yv = unpack(p00[-1], a0) + unpack(p10[1], a1);          // 004201A5..004201D1 (p10 byte 1 weighted by a1)
                yv = yv + unpack(p01[-1], a2);                                 // 004201DF..00420206
                yv = yv + unpack(p11[1], a3);                                  // 00420208..0042021C
                float y_stored = static_cast<float>(yv);                       // 0042021E fstp [esp+10h]
                double xv = unpack(p00[-2], a0) + unpack(p10[0], a1);          // 00420222..00420255
                xv = xv + unpack(p01[-2], a2);                                 // 00420257..00420276
                xv = xv + unpack(p11[0], a3);                                  // 00420278..0042028C
                float x_stored = static_cast<float>(xv);                       // 0042028E fstp [esp+14h]
                const bool flat = new_width == 1 || new_height == 1 || (new_width == 2 && new_height == 2); // 0042025F..004202A4
                if (flat) {                                                    // 004202A6..004202B6
                    z = kZero_0073d700; y_stored = 0.0f; x_stored = 1.0f;
                } else if (new_width == 4 && new_height == 4) {                // 00420303..0042031C
                    z = z * kHalf_0073e9ac;
                    y_stored = static_cast<float>(static_cast<double>(y_stored) * kHalf_0073e9ac);
                }
                const double length = std::sqrt((static_cast<double>(x_stored) * x_stored +
                    static_cast<double>(y_stored) * y_stored) + z * z);        // 004202BE..004202D6
                const float length_stored = static_cast<float>(length);        // 004202D8 fst [esp+1Ch]
                if (!(length != kZero_0073edf8)) {                             // 004202DC..004202E7 (equal: no jp)
                    z = kZero_0073d700; y_stored = 0.0f; x_stored = 1.0f;      // 004202E9..004202F9
                } else {
                    const double inverse = static_cast<double>(kOne_0073d764) / static_cast<double>(length_stored); // 00420322..00420328
                    const float inverse_stored = static_cast<float>(inverse);  // 0042032C fst [esp+20h]
                    z = z * inverse;                                           // 00420330 fmulp (wide inverse)
                    y_stored = static_cast<float>(static_cast<double>(inverse_stored) * y_stored); // 00420332..0042033A
                    x_stored = static_cast<float>(static_cast<double>(inverse_stored) * x_stored); // 0042033E..00420346
                }
                U8* const target = out + (out_row + (x >> 1)) * 4;             // 00420350..00420365
                target[2] = ftol_byte(z * k127_0073eddc + k128_0073edd8);      // 0042034A..00420377
                target[1] = ftol_byte(static_cast<double>(y_stored) * k127_0073eddc + k128_0073edd8); // 0042036D..0042038F
                target[0] = ftol_byte(static_cast<double>(x_stored) * k127_0073eddc + k128_0073edd8); // 00420385..004203A7
                const double quarter = static_cast<double>(length_stored) * kQuarter_0073edf0; // 0042039D..004203A1
                if (quarter > kOne_0073ede8) target[3] = 0xFF;                 // 004203A9..004203B8 (above only)
                else target[3] = ftol_byte(quarter * k255_0073ede4);           // 004203BE..004203C9
                p00 += 8; p01 += 8;                                            // 004203D6 / 004203D9
            }
        }
        row_base += width * 2;                                                 // 004203EA..004203F5
        row += static_cast<U>(width) * 8u;                                     // 004203F7..004203FE
    }
    live_free(source);                                                         // 0042041E..00420423 call 006FA390
    return out;                                                                // 00420428
}

// 004260A0 GLRender::ConvertHeightFieldAndLoadNormalMapTexture(data, width, height, map_width, map_height, scale,
// sub_image, is_normal_map). EAX on return is the free() scratch (unspecified); consumer upload.cpp:215 discards it.
U __cdecl convert_height_field_and_load_normal_map_004260a0(void* data, I width, I height, I map_width, I map_height,
                                                            float scale, I sub_image, U is_normal_map) {
    using namespace kotor_renderer_init;
    U8* buffer;
    if (static_cast<U8>(is_normal_map) != 0) {                                 // 004260A0..004260AE (AL)
        const U bytes = static_cast<U>(width) * static_cast<U>(height) * 4u;  // 004260B0..004260B7
        buffer = static_cast<U8*>(live_operator_new(bytes));                   // 004260BA..004260BB call 006FC0D2
        std::memcpy(buffer, data, bytes);                                      // 004260C0..004260D9 rep movs
    } else {
        buffer = static_cast<U8*>(convert_height_field_to_normal_map_0041fec0(static_cast<const U8*>(data), width, height,
            map_width, map_height, scale));                                    // 004260E1..00426103
    }
    I level = 0;                                                               // 0042611A [esp+3Ch] (is_normal_map slot reused)
    glTexParameteri_0073d23c(0x0DE1u, 0x2800u, 0x2601);                        // 00426105..00426122 MAG LINEAR
    glTexParameteri_0073d23c(0x0DE1u, 0x2801u, 0x2703);                        // 00426124..00426133 MIN LINEAR_MIPMAP_LINEAR
    glTexImage2D_0073d280(0x0DE1u, 0, 0x8058, width, height, 0, 0x80E1u, 0x1401u, buffer); // 00426135..00426150
    glTexParameteri_0073d23c(0x0DE1u, 0x84FEu, texture_upload_cells::anisotropy_007a685c); // 00426156..00426167
    while (width > 1 || height > 1) {                                          // 00426170..00426178
        ++level;                                                               // 0042617E
        I next_width = width >> 1, next_height = height >> 1;                  // 00426182..00426186 sar
        if (next_width == 0) next_width = 1;                                   // 00426188..0042618C
        if (next_height == 0) next_height = 1;                                 // 00426191..00426195
        buffer = static_cast<U8*>(down_sample_normal_map_00420030(buffer, width, height, next_width, next_height)); // 0042619A..004261AB
        if (sub_image != 0) {                                                  // 004261AD..004261B9
            glTexSubImage2D_0073d294(0x0DE1u, level, 0, 0, next_width, next_height, 0x1908u, 0x1401u, buffer); // 004261BB..004261D0
        } else {
            glTexImage2D_0073d280(0x0DE1u, level, 0x8058, next_width, next_height, 0, 0x80E1u, 0x1401u, buffer); // 004261DE..004261F6
            glTexParameteri_0073d23c(0x0DE1u, 0x84FEu, texture_upload_cells::anisotropy_007a685c); // 004261FC..0042620C
        }
        width = next_width;                                                    // 004261D6 / 00426212 ebp = esi
        height = next_height;                                                  // 004261D8 / 00426214 [esp+1Ch] = edi
    }
    live_free(buffer);                                                         // 0042621D..0042621E call 006FC0CD
    return 0;  // D2 label: EAX after 0042621E is free()'s scratch; the only caller (00420F04) discards it
}
} // extern "C"
