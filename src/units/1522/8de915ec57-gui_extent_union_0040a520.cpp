// Original swkotor.exe 0040A520 CSWGuiExtent::Union (120 bytes, [0040A520,0040A598)).
// cdecl (out, a, b), caller cleans, plain RET; EAX = out (0040A591).
// Extents are four signed DWORDs {x, y, width, height}. Result:
//   left   = a.x < b.x ? a.x : b.x                       (0040A531 cmp/jl, signed)
//   top    = a.y < b.y ? a.y : b.y                       (0040A544 cmp/jl, signed)
//   right  = (a.x+a.w) > (b.x+b.w) ? a.x+a.w : b.x+b.w   (0040A554..0040A560, 32-bit wrapping adds, signed jg)
//   bottom = (a.h+a.y) > (b.h+b.y) ? a.h+a.y : b.h+b.y   (0040A56C..0040A576, wrapping adds, signed jg)
//   out = {left, top, right-left, bottom-top}            (0040A582..0040A58E, wrapping subs; DWORD stores in this order)
// All eight input DWORDs are read before the first store, so out may alias a or b.
// The declaration comes from tier t4 (anim_base_get_bounding_rect.h), so the compiler checks
// that this definition has exactly t4's declared type.
#include "156fb31a97-anim_base_get_bounding_rect.h"
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
using B = std::uint8_t;
U word(const B* p, U o) { U v; std::memcpy(&v, p + o, 4); return v; }
I as_signed(U v) { I s; std::memcpy(&s, &v, 4); return s; }
U pick_less(U a, U b) { return as_signed(a) < as_signed(b) ? a : b; }      // jl: keep a only when a < b
U pick_greater(U a, U b) { return as_signed(a) > as_signed(b) ? a : b; }   // jg: keep a only when a > b
}  // namespace

extern "C" void* __cdecl gui_extent_union_0040a520(void* out, const void* a, const void* b) {
    const B* pa = static_cast<const B*>(a);
    const B* pb = static_cast<const B*>(b);
    const U ax = word(pa, 0), bx = word(pb, 0);                 // 0040A52B / 0040A52F
    const U ay = word(pa, 4), by = word(pb, 4);                 // 0040A53E / 0040A541
    const U aw = word(pa, 8), bw = word(pb, 8);                 // 0040A551 / 0040A556
    const U ah = word(pa, 12), bh = word(pb, 12);               // 0040A562 / 0040A565
    const U left = pick_less(ax, bx);
    const U top = pick_less(ay, by);
    const U right = pick_greater(ax + aw, bx + bw);
    const U bottom = pick_greater(ah + ay, bh + by);
    const U result[4] = {left, top, right - left, bottom - top};
    B* po = static_cast<B*>(out);
    std::memcpy(po + 0, &result[0], 4);                         // 0040A582
    std::memcpy(po + 4, &result[1], 4);                         // 0040A584
    std::memcpy(po + 8, &result[2], 4);                         // 0040A589
    std::memcpy(po + 12, &result[3], 4);                        // 0040A58E
    return out;                                                 // 0040A591 mov eax,edx
}
