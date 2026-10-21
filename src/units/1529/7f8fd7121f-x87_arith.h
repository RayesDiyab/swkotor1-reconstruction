// t12: x87 arithmetic of the original projectile update builders, written as C++ (no inline asm).
//
// The game runs with the CRT default control word 027Fh: 53-bit precision, round to nearest, all exceptions masked
// (no game code outside the CRT loads a control word). Under that control word an x87 FADD/FSUB/FMUL/FDIV of
// operands that are single floats or earlier 53-bit results gives the IEEE double result, with two differences
// from the SSE double operation that C++ compiles to:
//   1. The exponent range of a register is wider. Not reachable here: every operand is a float or a short chain of
//      float sums/products/quotients (at most ~2^260, at least ~2^-300), all inside the normal double range.
//   2. NaN selection when both operands are NaN (Intel SDM vol. 1, 4.8.3.5 / table 4-7). SSE returns the first
//      operand; x87 returns the quiet NaN (a quiet operand wins over a signalling memory operand; between two quiet
//      NaNs the larger significand wins, and with equal significands the positive one). A NaN loaded with
//      FLD DWORD is quieted by the load; a DWORD memory operand of an arithmetic instruction is not.
//      Measured on this machine: 2,000,483 NaN pairs, FLD a / F{ADD,SUB,MUL,SUBR} b, zero mismatches against
//      this rule; tests/test_x87_arith.cpp checks every helper below against the x87 instructions themselves.
// The helpers keep register values as double (exact for every 53-bit x87 value reachable here) and memory operands
// as their float bit pattern (M32), so the signalling bit of a memory operand is still visible.
#pragma once
#include <cstdint>
#include <cstring>

namespace {   // internal linkage in every including TU: no exported COMDAT, no merge with other lanes' helpers
namespace x87 {
using U = std::uint32_t;
using U64 = std::uint64_t;

// A DWORD memory operand: the raw float bits.
struct M32 { U bits; };

inline M32 m32(const void* p) { M32 m; std::memcpy(&m.bits, p, sizeof m.bits); return m; }
inline M32 m32_at(const void* p, U offset) { return m32(static_cast<const unsigned char*>(p) + offset); }
inline void put(void* p, M32 m) { std::memcpy(p, &m.bits, sizeof m.bits); }
inline void put_at(void* p, U offset, M32 m) { put(static_cast<unsigned char*>(p) + offset, m); }

inline float as_float(M32 m) { float f; std::memcpy(&f, &m.bits, sizeof f); return f; }
inline U64 bits(double d) { U64 u; std::memcpy(&u, &d, sizeof u); return u; }
inline bool is_nan(double d) { return d != d; }
inline bool is_nan(M32 m) { return (m.bits & 0x7f800000u) == 0x7f800000u && (m.bits & 0x007fffffu) != 0; }
inline bool is_snan(M32 m) { return is_nan(m) && (m.bits & 0x00400000u) == 0; }

// FLD DWORD: exact widening; a signalling NaN is quieted (the C++ conversion does the same).
inline double ld(M32 m) { return static_cast<double>(as_float(m)); }
// FSTP DWORD: round to nearest single (NaN payload truncated, as the x87 store does).
inline M32 st(double v) { const float f = static_cast<float>(v); M32 m; std::memcpy(&m.bits, &f, sizeof m.bits); return m; }

// Two quiet NaNs (register values): larger significand; equal significands -> the positive one.
inline double pick(double a, double b) {
    const U64 ma = bits(a) & 0x7fffffffffffffffull, mb = bits(b) & 0x7fffffffffffffffull;
    if (ma != mb) return ma > mb ? a : b;
    return (bits(a) >> 63) == 0 ? a : b;
}
// ST(i) op ST(j): both register values (quiet when NaN).
inline double rr(double a, double b, double sse) { return is_nan(a) && is_nan(b) ? pick(a, b) : sse; }
// ST op DWORD memory: the register value is quiet when NaN; the memory operand may be signalling.
inline double rm(double st, M32 m, double sse) {
    if (is_nan(st) && is_nan(m)) return is_snan(m) ? st : pick(st, ld(m));
    return sse;
}

inline double add(double a, double b) { return rr(a, b, a + b); }                 // FADD(P) ST, ST(i)
inline double mul(double a, double b) { return rr(a, b, a * b); }                 // FMUL(P) ST, ST(i)
inline double div(double a, double b) { return rr(a, b, a / b); }                 // FDIV(P) a / b
inline double add(double st, M32 m) { return rm(st, m, st + ld(m)); }             // FADD DWORD
inline double sub(double st, M32 m) { return rm(st, m, st - ld(m)); }             // FSUB DWORD
inline double subr(double st, M32 m) { return rm(st, m, ld(m) - st); }            // FSUBR DWORD
inline double mul(double st, M32 m) { return rm(st, m, st * ld(m)); }             // FMUL DWORD
inline double div(double st, M32 m) { return rm(st, m, st / ld(m)); }             // FDIV DWORD

// 006FAE8C _ftol2 (MSVC, x87 only): FISTP QWORD under the caller's rounding (nearest), then a correction by the
// sign of the residual stored as a float, so the result is the 64-bit truncation toward zero; EAX is its low dword.
// NaN and |x| >= 2^63 give the integer indefinite 8000000000000000h, whose low dword is 0. The operand is a 53-bit
// register value at every call site of this tier.
inline U ftol2_low(double x) {
    if (!(x > -9223372036854775808.0 && x < 9223372036854775808.0)) return 0u;
    return static_cast<U>(static_cast<U64>(static_cast<std::int64_t>(x)));
}
}  // namespace x87
}  // namespace
