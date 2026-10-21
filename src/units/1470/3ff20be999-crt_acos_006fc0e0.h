#pragma once
// 006FC0E0 _CIacos: the MSVC CRT x87 arc cosine statically linked into swkotor.exe (t11 own unit and export).
// The original takes its operand in ST0 and returns the result in ST0. Both are transported here as explicit
// 80-bit register images, so no precision is lost at the boundary (fpatan/fldpi results carry a 64-bit
// significand, which a float or double return would round).
#include <cstdint>
namespace crt_x87 {
#pragma pack(push, 1)
// Raw x87 register image as FSTP TBYTE stores it: 64-bit significand (explicit integer bit), then sign:exponent.
struct Extended {
    std::uint64_t significand;
    std::uint16_t sign_exponent;
};
#pragma pack(pop)
static_assert(sizeof(Extended) == 10, "x87 80-bit register image");
}  // namespace crt_x87
extern "C" {
// st0_in: the caller's ST0 at 006FC0E0. st0_out: ST0 at the RET (006FC0F3). The x87 control word and the sticky
// status flags change exactly as the original's do; the original per-thread errno (_getptd()+8 via 006FFE78,
// kotor_descriptor_support::errno_006ffe78) is set to EDOM (21h) on the domain path (00707C22).
void __cdecl crt_acos_x87_006fc0e0(const crt_x87::Extended* st0_in, crt_x87::Extended* st0_out);
}
