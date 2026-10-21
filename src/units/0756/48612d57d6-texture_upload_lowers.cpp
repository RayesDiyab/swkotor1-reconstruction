// lane-m46-startup: upload-chain image lowers declared in texture-upload/upload.h
// (0045DAD0 ImageScale, 0041FBD0 ImageQuarterRotated). Evidence: lane-m46-startup/dis/0045dad0.asm
// (plus tail 0045DD75..0045DD8D, Ghidra range is short) and dis/0041fbd0.asm. Integer-only bodies.
// Allocator family: 006FA7E6 operator new (live_operator_new) / 006FA390 free (live_free).
#include "be7d9420d7-upload.h"
#include "d08b97bcca-allocator_live.h"
#include <cstdint>
#include <cstring>

namespace {
using texture_upload::I;
using texture_upload::U;
}

namespace texture_upload {
extern "C" {

// 0045DAD0..0045DD8D ImageScale(src, src_w, src_h, comps, dst_w, dst_h): area-weighted box resample.
//  out = new(comps*dst_w*dst_h) (0 product -> return 0, nothing allocated, 0045DAED..0045DAF8);
//  acc = new(comps*dst_w*4) zero-filled (0045DAFF..0045DB28).
//  Vertical: src row share = dst_h units (srem, L14), dst row share = src_h units (drem, L20);
//  wy = min(srem, drem) (0045DB60..0045DB68). Horizontal per (src row, dst row) pair: src pixel share
//  dst_w (sx_rem, L1C), dst pixel share src_w (dx_rem, EDI):
//    sx_rem >= dx_rem: acc[dx] += px*dx_rem*wy; sx_rem -= dx_rem; if 0 {sx++; sx_rem=dst_w}; dx++; dx_rem=src_w
//    else:             acc[dx] += px*wy*sx_rem; dx_rem -= sx_rem; sx++; sx_rem=dst_w
//  (32-bit wrapping products/sums, 0045DBB5..0045DBC5 / 0045DC30..0045DC40).
//  srem < drem: drem -= srem; srem=dst_h; src_row += src_w (0045DD55..0045DD63).
//  else: emit row: byte = (acc + div/2) / div, div = src_w*src_h, div/2 = signed (cdq/sub/sar),
//  unsigned 32-bit DIV (0045DC7D..0045DCF8); srem -= drem; if 0 {src_row += src_w; srem=dst_h};
//  dst_y++; drem = src_h; out_row += dst_w; acc zeroed (0045DCFA..0045DD51). Loop while dst_y < dst_h.
//  free(acc) (0045DD79..0045DD7A); return out.
void* __cdecl image_scale_0045dad0(const void* source, I src_w, I src_h, I comps, I dst_w, I dst_h) {
    const U row_pixels = static_cast<U>(comps) * static_cast<U>(dst_w);                  // 0045DAE3..0045DAE5
    const U total = row_pixels * static_cast<U>(dst_h);                                   // 0045DAE8..0045DAEA
    if (total == 0) return nullptr;                                                       // 0045DAED..0045DAF8
    unsigned char* const out = static_cast<unsigned char*>(live_operator_new(total));     // 0045DAF9..0045DAFA
    const U acc_bytes = row_pixels << 2;                                                  // 0045DAFF
    U* const acc = static_cast<U*>(live_operator_new(acc_bytes));                        // 0045DB02..0045DB0B
    std::memset(acc, 0, acc_bytes);                                                       // 0045DB12..0045DB28
    const unsigned char* const src = static_cast<const unsigned char*>(source);
    I srem = dst_h;                                                                       // L14
    I drem = src_h;                                                                       // L20
    I dst_y = 0;                                                                          // L34
    I out_row = 0;                                                                        // L30 (pixels)
    I src_row = 0;                                                                        // L18 (pixels = row*src_w)
    if (dst_h > 0) {                                                                      // 0045DB33..0045DB43
        do {
            const I wy = srem < drem ? srem : drem;                                       // 0045DB60..0045DB68 (L2C)
            I sx = 0;                                                                     // [esp+50h] slot
            I sx_rem = dst_w;                                                             // L1C
            I dx = 0;                                                                     // L10
            I dx_rem = src_w;                                                             // EDI (0045DB6E)
            if (dst_w > 0) {                                                              // 0045DB6C..0045DB84
                U* cell = acc;                                                            // L28
                do {
                    U* a = cell;                                                          // 0045DB94
                    const unsigned char* px = src + static_cast<U>((src_row + sx) * comps);   // 0045DBA2..0045DBB3
                    if (sx_rem >= dx_rem) {                                               // 0045DB9A..0045DB9C
                        for (I c = 0; c < comps; ++c)                                     // 0045DBB5..0045DBCD
                            a[c] = a[c] + static_cast<U>(px[c]) * static_cast<U>(dx_rem) * static_cast<U>(wy);
                        sx_rem -= dx_rem;                                                 // 0045DBD7..0045DBD9
                        if (sx_rem == 0) { ++sx; sx_rem = dst_w; }                        // 0045DBDD..0045DBEA
                        ++dx;                                                             // 0045DBFA
                        cell += comps;                                                    // 0045DBFB..0045DC02
                        dx_rem = src_w;                                                   // 0045DBF6
                    } else {
                        for (I c = 0; c < comps; ++c)                                     // 0045DC30..0045DC48
                            a[c] = a[c] + static_cast<U>(px[c]) * static_cast<U>(wy) * static_cast<U>(sx_rem);
                        dx_rem -= sx_rem;                                                 // 0045DC52
                        ++sx;                                                             // 0045DC56
                        sx_rem = dst_w;                                                   // 0045DC54..0045DC57
                    }
                } while (dx < dst_w);                                                     // 0045DC5F..0045DC63
            }
            if (srem < drem) {                                                            // 0045DC75..0045DC77
                drem -= srem;                                                             // 0045DD55..0045DD57
                srem = dst_h;                                                             // 0045DD5B..0045DD5F
                src_row += src_w;                                                         // 0045DD63
            } else {
                const U divisor = static_cast<U>(src_w * src_h);                          // 0045DC7D..0045DC88
                if (dst_w > 0) {                                                          // 0045DC86..0045DC90
                    const U* a = acc;                                                     // L2C
                    for (I x = 0; x < dst_w; ++x) {                                       // 0045DCE0..0045DCF8
                        if (comps > 0) {                                                  // 0045DCA0..0045DCA2
                            const U half = static_cast<U>(static_cast<I>(divisor) / 2);   // 0045DCA4..0045DCB6 (cdq/sub/sar)
                            unsigned char* o = out + static_cast<U>((out_row + x) * comps);   // 0045DCA9..0045DCB8
                            for (I c = 0; c < comps; ++c)                                 // 0045DCC0..0045DCD2
                                o[c] = static_cast<unsigned char>((a[c] + half) / divisor);
                        }
                        a += comps;                                                       // 0045DCE5..0045DCEC
                    }
                }
                srem -= drem;                                                             // 0045DCFA..0045DD04
                if (srem == 0) { src_row += src_w; srem = dst_h; }                        // 0045DD08..0045DD1C
                ++dst_y;                                                                  // 0045DD20..0045DD2D
                drem = src_h;                                                             // 0045DD31..0045DD35
                out_row += dst_w;                                                         // 0045DD3D..0045DD44
                std::memset(acc, 0, acc_bytes);                                           // 0045DD39..0045DD51
                continue;                                                                 // 0045DD53 -> 0045DD67
            }
        } while (dst_y < dst_h);                                                          // 0045DD67..0045DD6F
    }
    live_free(acc);                                                                       // 0045DD75..0045DD7A
    return out;                                                                           // 0045DD7F
}

// 0041FBD0..0041FCFB ImageQuarterRotated(src, width, height, bpp, quarter), cdecl.
//  out = new(width*height*bpp); r = quarter % 4 (C remainder, 0041FBF9..0041FC08) then +4 if negative
//  (0041FC0D..0041FC12). Source pixels are read in row-major order; for pixel (x,y), with
//  A = {y, x, h-1-y, w-1-x}[r] and B = {x, h-1-y, w-1-x, y}[r] (the two 4-entry local tables at
//  [esp+24h]/[esp+34h], 0041FC77..0041FC88), it is copied (bpp bytes, rep movsd/movsb) to
//  out + (A*width + B)*bpp. The stride is always `width` (quirk kept for non-square images).
//  height <= 0 returns the allocation unwritten (0041FCF3..0041FCFB).
void* __cdecl image_quarter_rotated_0041fbd0(const void* source, I width, I height, I bpp, U quarter) {
    unsigned char* const out = static_cast<unsigned char*>(live_operator_new(static_cast<U>(width * height * bpp)));   // 0041FBE2..0041FBEB
    I r = static_cast<I>(quarter) % 4;                                                    // 0041FBF9..0041FC09
    if (r < 0) r += 4;                                                                    // 0041FC0D..0041FC12
    const unsigned char* row = static_cast<const unsigned char*>(source);                 // [esp+0Ch]
    const U row_bytes = static_cast<U>(width * bpp);                                      // 0041FC28..0041FC34
    I hy = height - 1;                                                                    // 0041FC31..0041FC38
    for (I y = 0; y < height; ++y, --hy) {                                                // 0041FC16..0041FCE1
        const unsigned char* p = row;
        I wx = width - 1;                                                                 // 0041FC62
        for (I x = 0; x < width; ++x, --wx) {                                             // 0041FC40..0041FCB7
            const I a[4] = {y, x, hy, wx};                                                // [esp+24h..30h]
            const I b[4] = {x, hy, wx, y};                                                // [esp+34h..40h]
            unsigned char* d = out + static_cast<U>((a[r] * width + b[r]) * bpp);         // 0041FC77..0041FC93
            std::memcpy(d, p, static_cast<U>(bpp));                                       // 0041FC95..0041FCA7
            p += bpp;                                                                     // 0041FCA9..0041FCAD
        }
        row += row_bytes;                                                                 // 0041FCCB..0041FCD1
    }
    return out;                                                                           // 0041FCE7 / 0041FCF5
}

}  // extern "C"
}  // namespace texture_upload
