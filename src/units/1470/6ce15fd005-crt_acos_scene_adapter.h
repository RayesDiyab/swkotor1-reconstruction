#pragma once
// Caller adapter only: sole 006FC0E0 body remains the accepted t11b provider.
// These three scene callers load a float and retain the returned ST0 image until
// the original arithmetic/store boundary. In particular fpatan's result is not
// rounded through double before FSIN or the degree conversion.
#include "3ff20be999-crt_acos_006fc0e0.h"

namespace {
namespace scene_acos {
using X = crt_x87::Extended;

// 00423C43..00423C54: y/z pass through FLD/FSTP DWORD before acos;
// x is copied by MOV. The early load quiets y/z sNaNs and raises IE even
// when the later small-angle branch returns without multiplying them.
inline void save_component(const float* source, float* destination) {
    __asm {
        mov eax, source
        fld dword ptr [eax]
        mov eax, destination
        fstp dword ptr [eax]
    }
}

inline X evaluate(float input) {
    X operand, result;
    X* const destination = &operand;
    __asm {
        fld dword ptr input
        mov eax, destination
        fstp tbyte ptr [eax]
    }
    crt_acos_x87_006fc0e0(&operand, &result);
    return result;
}

// 00423C5F, 00425D34, 0046F188: FADD ST0,ST0.
inline X twice(const X& value) {
    X result;
    const X* const source = &value;
    X* const destination = &result;
    __asm {
        mov eax, source
        fld tbyte ptr [eax]
        fadd st(0), st(0)
        mov eax, destination
        fstp tbyte ptr [eax]
    }
    return result;
}

// 00423C63..00423C81: the original FCOM/test/parity/FCHS sequence,
// then FCOMP epsilon. Keep the NaN comparison and exception side effects.
inline bool axis_is_small(const X& angle, double epsilon) {
    const X* const source = &angle;
    const float zero = 0.0f;
    unsigned short status;
    __asm {
        mov eax, source
        fld tbyte ptr [eax]
        fcom dword ptr zero
        fnstsw ax
        test ah, 5
        jp keep_sign
        fchs
    keep_sign:
        fcomp qword ptr epsilon
        fnstsw status
    }
    return (status & 0x4500u) == 0x0100u;
}

// 00423C99..00423CA1: FMUL qword half; FSIN; FDIVR qword one.
inline X axis_scale(const X& angle) {
    const double half = 0.5, one = 1.0;
    const X* const source = &angle;
    X result;
    X* const destination = &result;
    __asm {
        mov eax, source
        fld tbyte ptr [eax]
        fmul qword ptr half
        fsin
        fdivr qword ptr one
        mov eax, destination
        fstp tbyte ptr [eax]
    }
    return result;
}

// 00423CA7..00423CBE: FLD dword component; FMUL ST1; FSTP dword.
inline float scale_component(float component, const X& scale) {
    const X* const source = &scale;
    float result;
    __asm {
        mov eax, source
        fld tbyte ptr [eax]
        fld dword ptr component
        fmul st(0), st(1)
        fstp dword ptr result
        fstp st(0)
    }
    return result;
}

// 00425D37..00425D3D / 0046F18B..0046F191: FMUL dword degrees;
// FSTP dword, after the retained FADD ST0,ST0 result.
inline float degrees(const X& angle, float factor) {
    const X* const source = &angle;
    float result;
    __asm {
        mov eax, source
        fld tbyte ptr [eax]
        fmul dword ptr factor
        fstp dword ptr result
    }
    return result;
}
} // namespace scene_acos
} // namespace
