// 006FC0E0 _CIacos (MSVC CRT x87 arc cosine, statically linked into swkotor.exe), reconstructed in C++.
// Ownership: 006FC0E0 implemented by Claude projectile lane because no provider exists; canonical0464
// (animation/sound) claim to be reconciled by coordinator.
//
// Original flow (all addresses in swkotor.exe):
//   006FC0E0  sub esp,0Ch; fst qword [esp]; call 00704288; call 006FC0FD; add esp,0Ch; ret
//   00704288  EAX = hi & 7FF00000h, ZF = (exponent all ones); on all ones EAX = hi
//   006FC0FD  push edx; wait; fnstcw [esp]; je 006FC171 (Inf/NaN)
//   006FC104  cw != 027Fh -> 00704215: fldcw (cw & 0300h) | 007Fh   (keep PC, nearest, all exceptions masked)
//   006FC111  |x| < 1 -> fld1; fadd st,st1; fld1; fsub st,st2; fmulp; fsqrt; fxch; fpatan
//   006FC145  |x| == 1 -> fstp st0; fldpi (x < 0) / fldz (x > 0);   |x| > 1 -> 006FC17F
//   006FC128  [00833BDC] != 0 -> 0070429E (never: .data zero tail, no writer); else 007042AB:
//             cw == 027Fh -> return; (cw & 20h) && (SW & 20h) -> 007041B7 type 8; else fldcw cw; return
//   006FC171  Inf/NaN: NaN -> 0070422C (quiet: type 7; signalling: fadd [0075A960], type 1); Inf -> 006FC17F
//   006FC17F  fstp st0; fld tbyte [007A30E0] (negative QNaN indefinite); type 1 -> 007041B7
//   007041B7  fstp qword [ebp-8]; 007096B2 _87except(0Dh, &exception, &cw); fld qword [ebp-8]; fldcw cw if != 027Fh
//   007096B2  type 7 -> type 1 without _handle_exc; type 1 -> _handle_exc(8): IM masked -> 00707E49(1)
//             (fld tbyte [007A33C0]; fistp dword: raises IE), IM unmasked -> _raise_exc 0070773F;
//             type 8 -> _handle_exc(10h) -> 00707E49(20h) (fldpi; fstp qword: raises PE);
//             _ctrlfp(cw, FFFFh) 00707E25: fldcw cw; type != 8 -> [007A36D0] = 2694h (no writer) skips _matherr
//             00706A22; _set_errno 00707C06: type 1 -> *(_getptd()+8) = 21h (EDOM) at 00707C22 via 006FFE78.
// The control flow and every special case are C++; the x87 operations whose results C++ cannot express (80-bit
// values, fpatan/fsqrt under the caller's precision control, fldpi under the current rounding control, sticky
// status flags) are minimal inline helpers that execute the original instruction sequence.
#include "e23e1bf538-crt_acos_006fc0e0.h"
#include <cstddef>
#include <cstdint>
#include <cstring>

// The original per-thread errno (_getptd()+8, 006FFE78): declared exactly as the pow 0741 bridge declares it
// (errno_bridge.cpp); provider 0743-crt-pow-errno_only (compat_getptd()+8 from 0564), as the CRT math family uses.
namespace kotor_descriptor_support { int* __cdecl errno_006ffe78(); }
// Existing selected0739, assembly-only original _87except protocol. Its genuine
// implementation owns the six-word0070773F call and the native RaiseException.
extern "C" void __cdecl task8_math_x87_error_007096b2();

namespace {
using U = std::uint32_t;
using W = std::uint16_t;
using X = crt_x87::Extended;

// Original constants (tools/check_literals_t11.py compares each array with the image bytes at its address).
constexpr unsigned char kLiteral_007a30e0[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff};  // tbyte indefinite
constexpr unsigned char kLiteral_0075a960[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x3f};               // qword 1.0
constexpr unsigned char kLiteral_007a33c0[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x10, 0x44};  // tbyte 2^1041

X extended_from(const unsigned char (&bytes)[10]) { X v; std::memcpy(&v, bytes, sizeof v); return v; }

// ---- minimal x87 helpers (each executes exactly the original instructions named in its comment)
// wait; fnstcw (006FC0FE..006FC0FF)
__declspec(noinline) W x87_control_word() {
    W cw;
    __asm {
        fwait
        fnstcw cw
    }
    return cw;
}
// fldcw (00704227 / 007042D0 / 007041EE / 00707E40)
__declspec(noinline) void x87_load_control_word(W cw) {
    __asm { fldcw cw }
}
// wait; fnstsw ax (007042BB..007042BC)
__declspec(noinline) W x87_status_word() {
    W sw;
    __asm {
        fwait
        fnstsw sw
    }
    return sw;
}
// fst qword [esp] (006FC0E3) / fstp qword [ebp-8] (007041C0): ST0 rounded to double under the current CW.
__declspec(noinline) void x87_store_double(const X* value, double* destination) {
    __asm {
        mov eax, value
        fld tbyte ptr [eax]
        mov eax, destination
        fstp qword ptr [eax]
    }
}
// fld qword [ebp-8] (007041E3)
__declspec(noinline) void x87_load_double(const double* source, X* value) {
    __asm {
        mov eax, source
        fld qword ptr [eax]
        mov eax, value
        fstp tbyte ptr [eax]
    }
}
// 006FC118..006FC126: fld1; fadd st,st(1); fld1; fsub st,st(2); fmulp st(1),st; fsqrt; fxch st(1); fpatan
__declspec(noinline) void x87_acos_core(X* value) {
    __asm {
        mov eax, value
        fld tbyte ptr [eax]
        fld1
        fadd st, st(1)
        fld1
        fsub st, st(2)
        fmulp st(1), st
        fsqrt
        fxch st(1)
        fpatan
        fstp tbyte ptr [eax]
    }
}
// fldpi (006FC162; rounded under the current rounding control)
__declspec(noinline) void x87_load_pi(X* value) {
    __asm {
        mov eax, value
        fldpi
        fstp tbyte ptr [eax]
    }
}
// fadd qword [0075A960] (00704239; signalling-NaN quieting)
__declspec(noinline) void x87_add_double(X* value, const unsigned char* addend) {
    __asm {
        mov eax, value
        mov edx, addend
        fld tbyte ptr [eax]
        fadd qword ptr [edx]
        fstp tbyte ptr [eax]
    }
}
// 00707E54..00707E5E: fld tbyte [007A33C0]; fistp dword; wait (raises the invalid-operation flag)
__declspec(noinline) void x87_raise_invalid(const unsigned char* operand) {
    U sink;
    __asm {
        mov eax, operand
        fld tbyte ptr [eax]
        fistp sink
        fwait
    }
}
// 00707E98..00707E9E: fldpi; fstp qword; wait (raises the precision flag)
__declspec(noinline) void x87_raise_inexact() {
    double sink;
    __asm {
        fldpi
        fstp sink
        fwait
    }
}

// Original7041B7 unary _exception layout. arg2 is deliberately unwritten:
// _87except operation0D reads arg1 and retval only. All copies preserve raw bits.
struct UnaryException {
    std::int32_t type;
    const char* name;
    double arg1;
    double arg2;
    double retval;
};
static_assert(sizeof(UnaryException) == 0x20 && offsetof(UnaryException, arg1) == 8 &&
              offsetof(UnaryException, retval) == 0x18, "7041B7 unary exception ABI");
constexpr char kAcosName_007a2770[] = "acos";
__declspec(noinline) void unmasked_domain(const double* argument, double* result, W original_cw) {
    UnaryException record;
    record.type = 1;
    record.name = kAcosName_007a2770;
    std::memcpy(&record.arg1, argument, sizeof record.arg1);
    std::memcpy(&record.retval, result, sizeof record.retval);
    // _87except reads the low word then makes its own mutable dword. The
    // caller's original CW remains distinct from the handler-edited scratch CW.
    U caller_cw = original_cw;
    UnaryException* const recordPointer = &record;
    U* const controlPointer = &caller_cw;
    __asm {
        push eax
        push ecx
        push edx
        push controlPointer
        push recordPointer
        push 0dh
        call task8_math_x87_error_007096b2
        add esp, 12
        pop edx
        pop ecx
        pop eax
    }
    std::memcpy(result, &record.retval, sizeof record.retval);
}
}  // namespace

extern "C" void __cdecl crt_acos_x87_006fc0e0(const X* st0_in, X* st0_out) {
    X value = *st0_in;                                                // ST0
    double stored;                                                    // [esp] of 006FC0E0
    x87_store_double(&value, &stored);                                // 006FC0E3 fst qword [esp]
    U lo, hi;
    std::memcpy(&lo, &stored, 4);
    std::memcpy(&hi, reinterpret_cast<const unsigned char*>(&stored) + 4, 4);
    const bool not_finite = (hi & 0x7ff00000u) == 0x7ff00000u;        // 00704288..00704296 (ZF)
    const W cw = x87_control_word();                                  // 006FC0FE..006FC0FF
    int type = 0;                                                     // EAX at 007041B7 (0: normal exit)
    if (not_finite) {                                                 // 006FC102 je 006FC171
        if ((hi & 0x000fffffu) != 0 || lo != 0) {                     // 006FC171..006FC17D: NaN -> 0070422C
            if ((hi & 0x00080000u) != 0) {                            // 0070422C test eax,80000h
                type = 7;                                             // 00704233 quiet NaN: ST0 kept
            } else {                                                  // signalling NaN (the fst at 006FC0E3 has
                x87_add_double(&value, kLiteral_0075a960);            // already quieted any NaN, so this branch is
                type = 1;                                             // not reached from this entry point)
            }
        } else {                                                      // infinity -> 006FC17F
            value = extended_from(kLiteral_007a30e0);
            type = 1;
        }
    } else {
        if (cw != 0x027fu) x87_load_control_word(static_cast<W>((cw & 0x0300u) | 0x007fu));   // 006FC104..006FC10C
        const U exponent = hi & 0x7ff00000u;
        if (exponent < 0x3ff00000u) {                                 // 006FC111..006FC116 jae
            x87_acos_core(&value);                                    // 006FC118..006FC126
        } else if (exponent == 0x3ff00000u && ((hi & 0x000fffffu) | lo) == 0) {   // 006FC145..006FC156
            if ((hi & 0x80000000u) != 0) x87_load_pi(&value);         // 006FC158..006FC162 fldpi
            else value = X{};                                         // 006FC166 fldz
        } else {                                                      // 006FC17F: |x| > 1
            value = extended_from(kLiteral_007a30e0);
            type = 1;
        }
        if (type == 0) {                                              // 006FC128 -> 007042AB
            if (cw == 0x027fu) { *st0_out = value; return; }          // 007042AF..007042B3
            if ((cw & 0x0020u) == 0 || (x87_status_word() & 0x0020u) == 0) {   // 007042B5..007042C2
                x87_load_control_word(cw);                            // 007042D0
                *st0_out = value;
                return;
            }
            type = 8;                                                 // 007042C4
        }
    }
    // 007041B7: the exception record's result is ST0 rounded to double under the CW in force.
    double result;
    x87_store_double(&value, &result);                                // 007041C0 fstp qword [ebp-8]
    // 007096B2 _87except(0Dh, &exception, &cw)
    if (type == 7) {
        type = 1;                                                     // 007096F9 (no _handle_exc)
    } else if (type == 1) {                                           // 0070970D push 8 -> 007079E4
        if ((cw & 0x0001u) == 0) {
            unmasked_domain(&stored, &result, cw);
            X returned;
            x87_load_double(&result, &returned);                      // 007041E3
            if (cw != 0x027fu) x87_load_control_word(cw);             // 007041E6..007041EE
            *st0_out = returned;
            return;
        }
        x87_raise_invalid(kLiteral_007a33c0);                         // 00707A03 -> 00707E49(1)
    } else {                                                          // type 8: 007096F5 push 10h -> 007079E4
        x87_raise_inexact();                                          // 00707BF0 -> 00707E49(20h); PM is masked here
    }
    x87_load_control_word(cw);                                        // 00709772 _ctrlfp(cw, FFFFh)
    if (type == 1) *kotor_descriptor_support::errno_006ffe78() = 0x21;   // 00707C22 via 006FFE78 (type 8: 00707C06 writes nothing)
    X out;
    x87_load_double(&result, &out);                                   // 007041E3 fld qword [ebp-8]
    if (cw != 0x027fu) x87_load_control_word(cw);                     // 007041E6..007041EE
    *st0_out = out;
}
