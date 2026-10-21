// lane-gui-render: CSWGuiLabel/CSWGuiButton/CSWGuiBorder draw bodies.
// Every behaviour cites the original instruction addresses (GOG swkotor.exe).
#include "1277741e89-gui_render.h"
#include "4867c506f0-gui_cells.h"
#include <cstring>
#include <intrin.h>

using gui_render::I32;
using gui_render::Word;

namespace {
// Raw-offset accessors. Compiler barriers keep the original load order across
// the virtual calls (receivers are ordinary, non-concurrent game objects).
Word W(const void* p, unsigned off) {
    Word v;
    _ReadWriteBarrier();
    std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4);
    _ReadWriteBarrier();
    return v;
}
I32 I(const void* p, unsigned off) {
    I32 v;
    _ReadWriteBarrier();
    std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4);
    _ReadWriteBarrier();
    return v;
}
void* P(const void* p, unsigned off) {
    void* v;
    _ReadWriteBarrier();
    std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4);
    _ReadWriteBarrier();
    return v;
}
unsigned char B(const void* p, unsigned off) {
    unsigned char v;
    _ReadWriteBarrier();
    std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 1);
    _ReadWriteBarrier();
    return v;
}
void* at(void* p, unsigned off) { return static_cast<unsigned char*>(p) + off; }

template <class Slot> Slot slot(const void* object, unsigned offset) {
    Slot fn;
    const void* table = P(object, 0);
    std::memcpy(&fn, static_cast<const unsigned char*>(table) + offset, sizeof fn);
    return fn;
}
void delta_call(void* object, unsigned offset, Word delta_bits) {
    slot<gui_render::DeltaSlot>(object, offset)(object, nullptr, delta_bits);
}
I32 image_width(void* image) {   // vtable 74183C +10 GetImageWidth 0045A0A0
    return slot<gui_render::ImageMetricSlot>(image, 0x10)(image, nullptr);
}
I32 image_height(void* image) {  // vtable 74183C +0C GetImageHeight 0045A090
    return slot<gui_render::ImageMetricSlot>(image, 0x0C)(image, nullptr);
}
// +18 Draw (00459920) / +24 DrawBuffered (00459800); RET 20h.
void image_call(void* image, unsigned offset, I32 x, I32 y, I32 w, I32 h, Word flags,
                Word angle_bits, const void* color, Word alpha_bits) {
    slot<gui_render::ImageDrawSlot>(image, offset)(image, nullptr, x, y, w, h, flags,
                                                   angle_bits, color, alpha_bits);
}
// Reads the live pulse cell 0078D3D4 (MOV r32, m32 raw DWORD reads).
Word pulse() {
    return *static_cast<volatile Word*>(&kotor_renderer_cells::pulse_0078d3d4);
}
// FLD m32 / FSTP m32 exactly as 00416D76/00416D89 (binary32 through x87).
Word x87_float_copy(const void* source) {
    Word result;
    __asm {
        mov eax, source
        fld dword ptr [eax]
        fstp dword ptr [result]
    }
    return result;
}
constexpr Word angle_90 = 0x42B40000u;   // 00416B9D / 00416CD0 push 42B40000h
constexpr Word angle_180 = 0x43340000u;  // 00416BBD / 00416C6A push 43340000h
constexpr Word angle_270 = 0x43870000u;  // 00416BDE / 00416CF0 push 43870000h
}

// 00417750 CSWGuiLabel::Draw (vtable 73E5B8 +38), RET 4.
// 00417758..0041775F: border (+5C, CSWGuiBorder) vtbl+0C(delta);
// 00417762..0041776F: text (+D0, CSWGuiText) vtbl+0C(delta).
extern "C" void __fastcall kotor_fastcall_00417750(void* label, void*, Word delta_bits) {
    delta_call(at(label, 0x5C), 0x0C, delta_bits);
    delta_call(at(label, 0xD0), 0x0C, delta_bits);
}

// 00417AB0 CSWGuiButton::Draw (vtable 73E658 +38), RET 4.
// 00417AB3 test byte [esi+44],1: set -> hilight border +E0 (00417ABF), clear ->
// border +6C (00417AE3); then text +154 (00417ACE / 00417AEC), each vtbl+0C(delta).
extern "C" void __fastcall kotor_fastcall_00417ab0(void* button, void*, Word delta_bits) {
    if ((B(button, 0x44) & 1u) != 0) {
        delta_call(at(button, 0xE0), 0x0C, delta_bits);
    } else {
        delta_call(at(button, 0x6C), 0x0C, delta_bits);
    }
    delta_call(at(button, 0x154), 0x0C, delta_bits);
}

// 0060E760 void_return: owned by lane-gui-load (gui_load_slot_ret4_0060e760); withdrawn here.

// 00414CD0 CSWGuiBorderParams::GetBorderDim, thiscall no args, plain RET.
// [params+0] (border+14 DIMENSION) when nonzero (00414CD0..00414CDA); else
// owner=[params+50] (border+64), its corner [+68] and edge [+6C] must both be
// non-null (00414CDB..00414CF0), then tail-jump corner vtbl+0C GetImageHeight.
extern "C" I32 __fastcall kotor_fastcall_00414cd0(void* params, void*) {
    const I32 dimension = I(params, 0);
    if (dimension != 0) return dimension;
    void* owner = P(params, 0x50);
    if (owner == nullptr) return 0;
    void* corner = P(owner, 0x68);
    if (corner == nullptr) return 0;
    if (P(owner, 0x6C) == nullptr) return 0;
    return image_height(corner);
}

// 004157F0 CSWGuiBorder::FillCenter(h, w, x, y, alpha, color), RET 18h.
extern "C" void __fastcall kotor_fastcall_004157f0(void* border, void*, I32 h,
        I32 w, I32 x, I32 y, Word alpha_bits, const void* color) {
    const I32 image_w = image_width(P(border, 0x70));   // 004157FB +10
    const I32 image_h = image_height(P(border, 0x70));  // 00415805 +0C
    void* fill = P(border, 0x70);                       // 0041580E reload
    const Word angle = W(border, 0x1C);                 // 00415813 raw DWORD
    if (image_w < w) {                                  // 0041580C signed
        if (image_h < h) {                              // 00415820
            image_call(fill, 0x18, x + (w - image_w) / 2, y + (h - image_h) / 2, image_w,
                       image_h, 0, angle, color, alpha_bits);          // 00415826..00415858
        } else {
            image_call(fill, 0x18, x + (w - image_w) / 2, y, image_w, h, 0, angle, color,
                       alpha_bits);                                     // 00415863..0041588B
        }
    } else if (image_h < h) {                           // 00415895
        image_call(fill, 0x18, x, y + (h - image_h) / 2, w, image_h, 0, angle, color,
                   alpha_bits);                                         // 00415899..004158C1
    } else {
        image_call(fill, 0x18, x, y, w, h, 0, angle, color, alpha_bits); // 004158CB..004158EA
    }
}

// 004155B0 CSWGuiBorder::FillTile(h, w, x, y, alpha, color), RET 18h.
// Tiles the fill image; the integer remainder is spread one pixel per tile
// over the first tiles of each row/column (0041568A..00415693, 004156CA..004156D3).
extern "C" void __fastcall kotor_fastcall_004155b0(void* border, void*, I32 h,
        I32 w, I32 x, I32 y, Word alpha_bits, const void* color) {
    const I32 image_w = image_width(P(border, 0x70));    // 004155BD
    const I32 image_h = image_height(P(border, 0x70));   // 004155CB
    if (image_w <= 0) return;                            // 004155D6
    if (image_h <= 0) return;                            // 004155DE
    const I32 columns = w / image_w;                     // 004155EC
    const I32 rows = h / image_h;                        // 004155F7
    const Word angle = W(border, 0x1C);                  // 004155FB
    I32 rem_h = h - rows * image_h;                      // 00415610
    I32 rem_w = w - columns * image_w;                   // 00415612
    I32 extra_h = 0;  // [esp+1Ch]; read only when rows != 0 (0041563A load is dead)
    if (rows != 0) {                                     // 00415614
        extra_h = rem_h / rows;                          // 00415627
        rem_h -= extra_h * rows;                         // 0041562E
    }
    if (columns != 0) {                                  // 0041563E
        const I32 extra_w = rem_w / columns;             // 00415649
        const I32 rem_h_column = rem_h;                  // 0041564B [esp+2Ch]
        rem_w -= extra_w * columns;                      // 00415654
        I32 cur_x = x;                                   // 00415660 [esp+40h]
        if (columns <= 0) return;                        // 00415664
        const I32 column_w = extra_w + image_w;          // 0041566E
        I32 column_count = columns;                      // 00415674
        do {
            I32 cw = column_w;                           // 00415686
            if (rem_w != 0) { --rem_w; ++cw; }           // 0041568A..00415693
            if (rows == 0) {                             // 00415696 -> 0041572D
                image_call(P(border, 0x70), 0x18, cur_x, y, cw, rem_h, 0, angle, color,
                           alpha_bits);                  // 0041572D..0041574F
            } else {
                I32 rem_h_row = rem_h_column;            // 0041569C..004156A4
                I32 cur_y = y;                           // 004156A0
                if (rows > 0) {                          // 004156A8
                    const I32 row_h = extra_h + image_h; // 004156AA..004156B4
                    I32 row_count = rows;                // 004156B8
                    do {
                        I32 rh = row_h;                  // 004156C6
                        if (rem_h_row != 0) { --rem_h_row; ++rh; }   // 004156CA..004156D3
                        image_call(P(border, 0x70), 0x18, cur_x, cur_y, cw, rh, 0, angle,
                                   color, alpha_bits);   // 004156D4..004156F2
                        cur_y += rh;                     // 004156F9
                    } while (--row_count != 0);          // 004156FB..00415700
                }
            }
            cur_x += cw;                                 // 0041570A..00415715
        } while (--column_count != 0);                   // 00415714..0041571D
        return;
    }
    if (rows == 0) {                                     // 00415754 -> 004157B8
        image_call(P(border, 0x70), 0x18, x, y, rem_w, rem_h, 0, angle, color, alpha_bits);
        return;                                          // 004157B8..004157DC
    }
    if (rows <= 0) return;                               // 0041575C
    I32 cur_y = y;                                       // 00415758
    const I32 row_h = extra_h + image_h;                 // 00415762..00415768
    I32 rem_h_row = rem_h;                               // [esp+38h] from 00415634
    I32 row_count = rows;
    do {
        I32 rh = row_h;                                  // 00415776
        if (rem_h_row != 0) { --rem_h_row; ++rh; }       // 0041577A..00415783
        image_call(P(border, 0x70), 0x18, x, cur_y, rem_w, rh, 0, angle, color,
                   alpha_bits);                          // 00415784..004157A6
        cur_y += rh;                                     // 004157A9
    } while (--row_count != 0);                          // 004157AB..004157AC
}

// 004168C0 CSWGuiBorder::Draw (vtable 73E338 +0C), RET 4. Delta is not read.
// Layout (CSWGuiBorder 0x74): +04..+10 extent x,y,w,h; +14 CSWGuiBorderParams
// (+14 dimension, +1C fill rotation binary32, +20 alpha binary32, +24 Vector color,
// +30 flags: bits0-1 fill style, bit2 pulsing, bit3 edge pulse, bit4 fill pulse,
// +64 owner back pointer); +68 corner image, +6C edge image, +70 fill image.
extern "C" void __fastcall kotor_fastcall_004168c0(void* border, void*, Word) {
    if (I(border, 0x0C) == 0) return;                    // 004168C7..004168CE
    if (I(border, 0x10) == 0) return;                    // 004168D4..004168D7
    Word edge_alpha = W(border, 0x20);                   // 004168DD [esp+44h]
    Word fill_alpha = edge_alpha;                        // 004168EB [esp+30h]
    const Word flags = W(border, 0x30);                  // 004168E6
    if ((flags & 4u) != 0) {                             // 004168E9
        if ((flags & 8u) != 0) edge_alpha = pulse();     // 004168F1..004168FB
        if ((flags & 0x10u) != 0) fill_alpha = pulse();  // 004168FF..00416908
    }
    void* const color = at(border, 0x24);                // 00416B63 lea edi,[esi+24h]
    I32 near_w = 0;   // ebp at 00416D23 (S1C on the corner path, 0 at 00416916)
    I32 near_h = 0;   // ebx at 00416D23 (S20 on the corner path, 0 at 004168CA)
    I32 far_w = 0;    // [esp+30h] after 0041691A (0 at 00416927)
    I32 far_h = 0;    // [esp+2Ch] after 0041691A (0 at 00416923)
    if (P(border, 0x68) != nullptr) {                    // 0041690C..0041692B
        I32 x = I(border, 4);                            // 0041691B [esp+24h]
        I32 y = I(border, 8);                            // 0041691F [esp+28h]
        I32 corner_w, corner_h, edge_w, edge_h;
        if (kotor_fastcall_00414cd0(at(border, 0x14), nullptr) == 0) {   // 00416936
            void* corner = P(border, 0x68);
            corner_w = image_width(corner);              // 00416944 +10
            corner_h = image_height(P(border, 0x68));    // 00416952 +0C (reload [esi+68])
            void* edge = P(border, 0x6C);                // 00416955
            if (edge != nullptr) {
                edge_w = image_width(edge);              // 00416960
                edge_h = image_height(P(border, 0x6C));  // 0041696E
            } else {
                edge_w = corner_w;                       // 00416973..0041697B
                edge_h = corner_h;
            }
        } else {
            // 0041697F..00416992: the original calls GetBorderDim a second time.
            const I32 dimension = kotor_fastcall_00414cd0(at(border, 0x14), nullptr);
            corner_w = corner_h = edge_w = edge_h = dimension;
        }
        const I32 width = I(border, 0x0C);               // 00416998 [esp+2Ch]
        // Horizontal edge tiling (00416998..00416A0E).
        I32 rem_x = width - (corner_w + corner_w);       // 0041699B..004169A2
        I32 count_x = 0;                                 // 004169A8 [esp+10h]
        I32 extra_x = 0;                                 // [esp+3Ch]
        if (rem_x > 0) {                                 // 004169AC
            I32 n = 0;
            bool half_check = true;
            if (edge_w != 0) {                           // 004169AE
                n = rem_x / edge_w;                      // 004169B5
                half_check = (n == 0);                   // 004169BF
            }
            if (half_check && !(edge_w / 2 > rem_x)) n = 1;   // 004169C1..004169D4
            rem_x -= n * edge_w;                         // 004169D8..004169DD
            extra_x = (n != 0) ? rem_x / n : 0;          // 004169DF..004169EA / 004169F8
            count_x = n;
        }
        rem_x -= extra_x * count_x;                      // 00416A00..00416A0E [esp+40h]
        const I32 height = I(border, 0x10);              // 00416A09 ebp, [esp+50h]
        // Vertical edge tiling (00416A12..00416A9C). The original divides by the
        // edge WIDTH ([esp+18h]) and subtracts count*edge HEIGHT ([esp+34h]).
        I32 rem_y = height - (corner_h + corner_h);      // 00416A12..00416A19
        I32 count_y = 0;                                 // 00416A23 [esp+14h]
        I32 extra_y = 0;                                 // [esp+44h]
        if (rem_y > 0) {                                 // 00416A27
            I32 n = 0;
            bool half_check = true;
            if (edge_w != 0) {                           // 00416A2D
                n = rem_y / edge_w;                      // 00416A34
                half_check = (n == 0);                   // 00416A40
            }
            if (half_check && !(edge_w / 2 > rem_y)) n = 1;   // 00416A42..00416A57
            rem_y -= n * edge_h;                         // 00416A5B..00416A62
            extra_y = (n != 0) ? rem_y / n : 0;          // 00416A64..00416A71 / 00416A85
            count_y = n;
        }
        rem_y -= extra_y * count_y;                      // 00416A8D..00416A9C [esp+48h]
        // Corner extents (00416A9A..00416AFF).
        I32 near_corner_h;                               // [esp+20h]
        I32 near_corner_w;                               // [esp+1Ch]
        if (count_x != 0) {
            x += corner_w;                               // 00416AB2 / 00416AC8
            if (count_y != 0) {
                y += corner_h;                           // 00416AB4
                near_corner_h = corner_h;                // 00416ABE
            } else {
                near_corner_h = height / 2;              // 00416ACC..00416AD3
            }
            near_corner_w = corner_w;                    // 00416AFF / 00416AD7
        } else {
            if (count_y != 0) {
                y += corner_h;                           // 00416AE1
                near_corner_h = corner_h;                // 00416AE5
            } else {
                near_corner_h = height / 2;              // 00416AEB..00416AF2
            }
            near_corner_w = width / 2;                   // 00416AF6..00416AFF
        }
        const I32 right = I(border, 4) + width;          // 00416B07..00416B0D
        const I32 top = I(border, 8);                    // 00416B0A
        const I32 bottom = top + height;                 // 00416B17
        I32 corner_x2 = right - near_corner_w;           // 00416B13..00416B15
        I32 corner_y2 = bottom - near_corner_h;          // 00416B1A..00416B1C
        if (count_y == 0 && (height % 2) != 0) --corner_y2;   // 00416B20..00416B3B
        if (count_x == 0 && (width % 2) != 0) --corner_x2;    // 00416B3C..00416B58
        const Word alpha = edge_alpha;                   // 00416B5C ebx
        far_w = right - corner_x2;                       // 00416B61 -> [esp+30h]
        far_h = bottom - corner_y2;                      // 00416B6F -> [esp+2Ch]
        // Four corners through +24 DrawBuffered (00416B8F..00416BF4).
        image_call(P(border, 0x68), 0x24, I(border, 4), top, near_corner_w, near_corner_h,
                   0, 0, color, alpha);
        image_call(P(border, 0x68), 0x24, I(border, 4), corner_y2, near_corner_w, far_h,
                   0, angle_90, color, alpha);
        image_call(P(border, 0x68), 0x24, corner_x2, corner_y2, far_w, far_h, 0, angle_180,
                   color, alpha);
        image_call(P(border, 0x68), 0x24, corner_x2, I(border, 8), far_w, near_corner_h, 0,
                   angle_270, color, alpha);
        if (P(border, 0x6C) != nullptr) {                // 00416BF7..00416BFC
            if (count_x > 0) {                           // 00416C02..00416C08
                const I32 segment = extra_x + edge_w;    // 00416C0E..00416C18
                I32 counter = count_x;                   // 00416C1C
                do {
                    I32 sw = segment;                    // 00416C20
                    if (rem_x != 0) { ++sw; --rem_x; }   // 00416C28..00416C3A
                    image_call(P(border, 0x6C), 0x24, x, I(border, 8), sw, near_corner_h, 0,
                               0, color, alpha);         // 00416C3E..00416C5C
                    image_call(P(border, 0x6C), 0x24, x, corner_y2, sw, far_h, 0, angle_180,
                               color, alpha);            // 00416C5F..00416C7D
                    x += sw;                             // 00416C80..00416C8F
                } while (--counter != 0);                // 00416C8E..00416C97
            }
            if (count_y > 0) {                           // 00416C99..00416C9F
                const I32 segment = extra_y + edge_w;    // 00416CA1..00416CAB
                I32 counter = count_y;                   // 00416CAF
                do {
                    I32 sh = segment;                    // 00416CB9
                    if (rem_y != 0) { ++sh; --rem_y; }   // 00416CB3..00416CC1
                    image_call(P(border, 0x6C), 0x24, I(border, 4), y, near_corner_w, sh, 0,
                               angle_90, color, alpha);  // 00416CC5..00416CE2
                    image_call(P(border, 0x6C), 0x24, corner_x2, y, far_w, sh, 0, angle_270,
                               color, alpha);            // 00416CE5..00416D03
                    y += sh;                             // 00416D06..00416D11
                } while (--counter != 0);                // 00416D10..00416D19
            }
        }
        near_w = near_corner_w;                          // 00416D1B
        near_h = near_corner_h;                          // 00416D1F
    }
    // 00416D23..00416D2A FlushBuffer([0078E43C]).
    kotor_cdecl_0045a170(kotor_gui_render_cells::gui_image_alpha_0078e43c);
    void* fill = P(border, 0x70);                        // 00416D2F [esp+4Ch]
    if (fill != nullptr) {
        const I32 fill_h = I(border, 0x10) - far_h - near_h;   // 00416D45..00416D63
        const I32 fill_w = I(border, 0x0C) - far_w - near_w;   // 00416D4C..00416D59
        const I32 fill_x = I(border, 4) + near_w;              // 00416D51..00416D5B
        const I32 fill_y = I(border, 8) + near_h;              // 00416D5D..00416D65
        switch (W(border, 0x30) & 3u) {                        // 00416D56..00416D70
        case 0:  // 00416DAD..00416DBC
            kotor_fastcall_004155b0(border, nullptr, fill_h, fill_w, fill_x,
                                                 fill_y, fill_alpha, color);
            break;
        case 1:  // 00416D97..00416DA6
            kotor_fastcall_004157f0(border, nullptr, fill_h, fill_w, fill_x,
                                                   fill_y, fill_alpha, color);
            break;
        case 2:  // 00416D72..00416D92: rotation through FLD/FSTP m32.
            image_call(fill, 0x18, fill_x, fill_y, fill_w, fill_h, 0,
                       x87_float_copy(at(border, 0x1C)), color, fill_alpha);
            break;
        default:  // 3: no fill (00416D70 jne 00416DC1)
            break;
        }
    }
    // 00416DC1..00416DC8 FlushBuffer([0078E43C]) (cell re-read).
    kotor_cdecl_0045a170(kotor_gui_render_cells::gui_image_alpha_0078e43c);
}
