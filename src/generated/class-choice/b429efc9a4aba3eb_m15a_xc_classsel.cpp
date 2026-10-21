// lane-m15-a (L15-A) unit m15a-xc-classsel: storage owner of the .data class-choice table 007A2684 read by
// CSWGuiClassSelection (ctor 006DC7B5.., OnEnterButton 006DBAC0 / 006DBB21 / 006DBCDC) and its original dynamic
// initializer 0073B690 (__xc slot 0078D184, between 0073B670 [0078D180] and 0073B6D0 [0078D188], walked by _cinit
// 006FB11A..006FB138). The compiler-emitted .CRT$XCU record of the named-namespace root below is the modern
// _initterm entry for the same role (one record in this object); the composer places this object between
// xc-0073b670-vfx-list.obj and xc-0073b6d0-requested-models.obj (D81 original __xc order).
// Initializer 0073B690 (no atexit):
//   0073B690 mov al,[0074B31Bh] / 0073B695 mov cl,[0074B31Ah] / 0073B69B mov dl,[0074B319h]
//   0073B6A1 mov [007A2684h],al / 0073B6A6 mov [007A268Ch],cl / 0073B6AC mov [007A2694h],dl
//   0073B6B2 mov [007A269Ch],dl / 0073B6B8 mov [007A26A4h],cl / 0073B6BE mov [007A26ACh],al / 0073B6C3 ret
// .rdata 0074B319 = 00h, 0074B31A = 01h, 0074B31B = 02h (immutable bytes of dword 0074B318 = 02010000h; no other
// owner in the M60 map): the three class-type constants.
#include "523fcf42472a3b45_m15a_gui.h"
#include <cstdint>

extern "C" KotorM15aClassChoice kotor_m15a_class_choices_007a2684[6];
static_assert(sizeof(kotor_m15a_class_choices_007a2684) == 0x30, "007A2684..007A26B3");

namespace {
constexpr std::uint8_t kClass0074b319 = 0x00;   // .rdata 0074B319
constexpr std::uint8_t kClass0074b31a = 0x01;   // .rdata 0074B31A
constexpr std::uint8_t kClass0074b31b = 0x02;   // .rdata 0074B31B
}

extern "C" __declspec(noinline) int __cdecl kotor_m15a_xc_init_0073b690() {
    const std::uint8_t a = kClass0074b31b;                        // 0073B690 mov al,[0074B31Bh]
    const std::uint8_t c = kClass0074b31a;                        // 0073B695 mov cl,[0074B31Ah]
    const std::uint8_t d = kClass0074b319;                        // 0073B69B mov dl,[0074B319h]
    kotor_m15a_class_choices_007a2684[0].class_id = a;            // 0073B6A1 [007A2684]
    kotor_m15a_class_choices_007a2684[1].class_id = c;            // 0073B6A6 [007A268C]
    kotor_m15a_class_choices_007a2684[2].class_id = d;            // 0073B6AC [007A2694]
    kotor_m15a_class_choices_007a2684[3].class_id = d;            // 0073B6B2 [007A269C]
    kotor_m15a_class_choices_007a2684[4].class_id = c;            // 0073B6B8 [007A26A4]
    kotor_m15a_class_choices_007a2684[5].class_id = a;            // 0073B6BE [007A26AC]
    return 0;                                                     // 0073B6C3 ret (no atexit registration)
}
namespace kotor_xc_0078d184 {
extern const int root_0073b690;
const int root_0073b690 = kotor_m15a_xc_init_0073b690();   // .CRT$XCU record
}
