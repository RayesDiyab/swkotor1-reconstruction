// Original swkotor.exe 0069F630, 00699C10, 00699D20. Binds 0069EDC0 to tier t1.
#include "156fb31a97-anim_base_get_bounding_rect.h"
#include "4c326ffcda-anim_base_get_bounding_rect_0069edc0.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

using kotor_m15e::U;
using I = std::int32_t;
using B = std::uint8_t;

static_assert(std::is_same_v<decltype(KotorM15eAnimBaseVtbl::s0d8), U (__fastcall*)(void*, void*, void*)>,
              "requires the t4-patched m15e_animbase.h (s0d8 returns kotor_m15e::U)");
static_assert(std::is_same_v<decltype(&anim_base_get_bounding_rect_0069f630), decltype(KotorM15eAnimBaseVtbl::s0d8)>, "s0d8");
static_assert(std::is_same_v<decltype(&anim_base_head_wield_get_bounding_rect_00699c10), decltype(KotorM15eAnimBaseVtbl::s0d8)>, "s0d8");
static_assert(std::is_same_v<decltype(&anim_base_head_wield_get_bounding_rect_vtordisp_00699d20), decltype(KotorM15eAnimBaseVtbl::s0d8)>, "s0d8");
static_assert(offsetof(KotorM15eAnimBaseVtbl, s0d8) == 0xD8, "slot");

namespace {
template<class T> T rd(const B* p) { T v; std::memcpy(&v, p, sizeof v); return v; }
aurora_model::Gob* gob_at(const B* p) { return rd<aurora_model::Gob*>(p); }
}  // namespace

// 0069F630..0069F641: EAX of 0069EDC0 (0/1) passes through.
extern "C" U __fastcall anim_base_get_bounding_rect_0069f630(void* self, void*, void* rect) {
    return anim_base_get_bounding_rect_0069edc0(gob_at(static_cast<const B*>(self) + 0xB8), rect);
}

// 00699C10..00699CDB. self: subobject with vbptr at self-8.
extern "C" U __fastcall anim_base_head_wield_get_bounding_rect_00699c10(void* self, void*, void* rect) {
    const B* s = static_cast<const B*>(self);
    const B* vbtable = rd<const B*>(s - 8);                                   // 00699C16
    const I second = rd<I>(vbtable + 8);                                      // 00699C19
    if (!gob_at(s + second + 0x3C)) {                                         // 00699C1D..00699C25
        // 00699CC5..00699CDB: forward to the CSWCAnimBase base (vbase1 = self + vbtbl[1] - 8);
        // its EAX is returned unchanged.
        return anim_base_get_bounding_rect_0069f630(const_cast<B*>(s) + rd<I>(vbtable + 4) - 8, nullptr, rect);
    }
    // Locals (original ESP after the two register pushes = L): extent R2 at L+8, R1 at L+18,
    // union result T at L+28. All eight DWORDs of R2/R1 are zeroed first (00699C2B..00699C47).
    alignas(4) B second_rect[16];   // R1 (L+18)
    alignas(4) B base_rect[16];     // R2 (L+8)
    alignas(4) B joined[16];        // T  (L+28)
    std::memset(second_rect, 0, sizeof second_rect);
    std::memset(base_rect, 0, sizeof base_rect);
    // 00699C4B..00699C5C: 0069EDC0(Gob [self+vbtbl[2]+3C] re-read, R1).
    if (!anim_base_get_bounding_rect_0069edc0(gob_at(s + second + 0x3C), second_rect)) return 0;   // 00699C63
    // 00699C65..00699C74: 0069F630 on vbase1 (vbtable re-read), R2.
    const B* vbtable_again = rd<const B*>(s - 8);
    if (!anim_base_get_bounding_rect_0069f630(const_cast<B*>(s) + rd<I>(vbtable_again + 4) - 8, nullptr, base_rect)) return 0;  // 00699C7B
    // 00699C7D..00699C8C: T = CSWGuiExtent::Union(R1, R2); 00699C91..00699CAC: four DWORD copies to rect.
    const B* u = static_cast<const B*>(gui_extent_union_0040a520(joined, second_rect, base_rect));
    B* out = static_cast<B*>(rect);
    std::memcpy(out + 0, u + 0, 4);
    std::memcpy(out + 4, u + 4, 4);
    std::memcpy(out + 8, u + 8, 4);
    std::memcpy(out + 12, u + 12, 4);
    return 1;                                                                 // 00699CAF
}

// 00699D20: ECX -= [ECX-4]; jmp 00699C10.
extern "C" U __fastcall anim_base_head_wield_get_bounding_rect_vtordisp_00699d20(void* self, void*, void* rect) {
    B* s = static_cast<B*>(self);
    return anim_base_head_wield_get_bounding_rect_00699c10(s - rd<I>(s - 4), nullptr, rect);
}
