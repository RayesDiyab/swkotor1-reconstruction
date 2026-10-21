#include "1a847f7684-w1_x87.hpp"
#include <cmath>
#include <cstring>
#if !defined(_M_IX86)
#error "w1_x87.cpp reproduces x87 sequences and requires an x86 (32-bit) MSVC-compatible target"
#endif
namespace w1::x87 {
namespace {
const double k_percent = 0.01;          // .rdata 00743310 (qword)
const double k_minimum = 9.999999747378752e-05; // .rdata 007471D0 (qword, == (double)1.0e-4f)
const float k_two32 = 4294967296.0f;    // .rdata 0073D770
const float k_milli = 0.001f;           // .rdata 0073D708 (3A83126F)
// __ftol2 equivalent: truncating conversion of ST0 to int64 (fistp qword under RC=chop).
S ftol_st0_from_double(double v) {
  unsigned short saved, chop; long long res;
  __asm {
    fnstcw saved
    mov ax, saved
    or ax, 0x0c00
    mov chop, ax
    fldcw chop
    fld v
    fistp res
    fldcw saved
  }
  return static_cast<S>(static_cast<U>(res & 0xffffffffull));
}
}
U accumulator_adjust(U& acc, U add) {
  float a, b; U ab = add, accbits = acc; double floor_in; float stored;
  std::memcpy(&a, &ab, 4); std::memcpy(&b, &accbits, 4);
  __asm {
    fld a
    fadd b
    fst stored
    fstp floor_in
  }
  double floored = std::floor(floor_in);                 // 005A5C89 floor (CRT, double)
  S n32;
  {
    unsigned short saved, chop; long long res;           // 005A5C91 __ftol2 on floor result
    __asm {
      fnstcw saved
      mov ax, saved
      or ax, 0x0c00
      mov chop, ax
      fldcw chop
      fld floored
      fistp res
      fldcw saved
    }
    n32 = static_cast<S>(static_cast<U>(res & 0xffffffffull));
  }
  S n = static_cast<S>(static_cast<std::int16_t>(n32 & 0xffff)); // movsx ecx, ax
  float result;
  __asm {
    fild n
    fsubr stored
    fstp result
  }
  std::memcpy(&acc, &result, 4);
  return static_cast<U>(n32);                              // EAX = full __ftol2 low dword (callers use AX)
}
bool regen_below_minimum(U rate, S scale) {
  float r; std::memcpy(&r, &rate, 4); unsigned short sw;
  __asm {
    fild scale
    fld r
    fmul k_percent
    fmulp st(1), st(0)
    fcomp k_minimum
    fnstsw ax
    mov sw, ax
  }
  return ((sw >> 8) & 5) == 1;                            // test ah,5 ; jp -> fallthrough only for C0 alone
}
U regen_amount(U rate, S scale, bool use_minimum, U dt) {
  float r; std::memcpy(&r, &rate, 4); float res; int m = use_minimum ? 1 : 0; S d = static_cast<S>(dt);
  __asm {
    cmp m, 0
    je compute
    fld k_minimum
    jmp scaled
  compute:
    fild scale
    fld r
    fmul k_percent
    fmulp st(1), st(0)
  scaled:
    fild d
    cmp d, 0
    jge positive
    fadd k_two32
  positive:
    fmul k_milli
    fxch st(1)
    fmulp st(1), st(0)
    fstp res
  }
  U bits; std::memcpy(&bits, &res, 4); return bits;
}
U int_to_float(S v) {
  float res;
  __asm {
    fild v
    fstp res
  }
  U bits; std::memcpy(&bits, &res, 4); return bits;
}
bool int_less_than_float(S a, U f) {
  float x; std::memcpy(&x, &f, 4); unsigned short sw;
  __asm {
    fild a
    fcomp x
    fnstsw ax
    mov sw, ax
  }
  return ((sw >> 8) & 5) == 1;
}
U ftol_float(U f) {
  float x; std::memcpy(&x, &f, 4);
  double v = x;                                           // exact widening; fld dword then __ftol2
  return static_cast<U>(ftol_st0_from_double(v));
}
U add_float(U a, U b) {
  float x, y, res; std::memcpy(&x, &a, 4); std::memcpy(&y, &b, 4);
  __asm {
    fld x
    fadd y
    fstp res
  }
  U bits; std::memcpy(&bits, &res, 4); return bits;
}
U sub_float(U a, U b) {
  float x, y, res; std::memcpy(&x, &a, 4); std::memcpy(&y, &b, 4);
  __asm {
    fld x
    fsub y
    fstp res
  }
  U bits; std::memcpy(&bits, &res, 4); return bits;
}
bool within_squared(U ax, U ay, U az, U bx, U by, U bz, U r2) {
  float a0, a1, a2, b0, b1, b2, r; unsigned short sw;
  std::memcpy(&a0, &ax, 4); std::memcpy(&a1, &ay, 4); std::memcpy(&a2, &az, 4);
  std::memcpy(&b0, &bx, 4); std::memcpy(&b1, &by, 4); std::memcpy(&b2, &bz, 4); std::memcpy(&r, &r2, 4);
  __asm {
    fld b0
    fsub a0
    fld b1
    fsub a1
    fld b2
    fsub a2
    fld st(0)
    fmul st(0), st(1)
    fld st(2)
    fmul st(0), st(3)
    faddp st(1), st(0)
    fld st(3)
    fmul st(0), st(4)
    faddp st(1), st(0)
    fcomp r
    fstp st(0)
    fnstsw ax
    fstp st(0)
    fstp st(0)
    mov sw, ax
  }
  const unsigned masked = (sw >> 8) & 0x41;
  return masked == 0x01 || masked == 0x40;
}
bool perception_range_open(U mx, U my, U mz, U tx, U ty, U tz, U r) {
  float m0, m1, m2, t0, t1, t2, rr; unsigned short sw;
  std::memcpy(&m0, &mx, 4); std::memcpy(&m1, &my, 4); std::memcpy(&m2, &mz, 4);
  std::memcpy(&t0, &tx, 4); std::memcpy(&t1, &ty, 4); std::memcpy(&t2, &tz, 4); std::memcpy(&rr, &r, 4);
  __asm {
    fld m2
    fsub t2
    fld m1
    fsub t1
    fld m0
    fsub t0
    fld st(0)
    fmul st(0), st(1)
    fld st(2)
    fmul st(0), st(3)
    faddp st(1), st(0)
    fld st(3)
    fmul st(0), st(4)
    faddp st(1), st(0)
    fld rr
    fmul rr
    fcompp
    fstp st(0)
    fnstsw ax
    fstp st(0)
    fstp st(0)
    mov sw, ax
  }
  const unsigned masked = (sw >> 8) & 0x41;
  return masked == 0 || masked == 0x41;
}
namespace {
inline float F(U u) { float f; std::memcpy(&f, &u, 4); return f; }
inline U B(float f) { U u; std::memcpy(&u, &f, 4); return u; }
U chop_ext(const X80& v) {                                // __ftol2 on an 80-bit value
  unsigned short saved, chop; long long res; const unsigned char* q = v.b;
  __asm {
    fnstcw saved
    mov ax, saved
    or ax, 0x0c00
    mov chop, ax
    mov eax, q
    fldcw chop
    fld tbyte ptr [eax]
    fistp res
    fldcw saved
  }
  return static_cast<U>(res & 0xffffffffull);
}
}
X80 x80_from_float(U f) {
  X80 r; float x = F(f); unsigned char* q = r.b;
  __asm {
    fld x
    mov eax, q
    fstp tbyte ptr [eax]
  }
  return r;
}
X80 x80_add_float(const X80& a, U f) {
  X80 r; float x = F(f); const unsigned char* pa = a.b; unsigned char* q = r.b;
  __asm {
    mov eax, pa
    fld tbyte ptr [eax]
    fadd x
    mov eax, q
    fstp tbyte ptr [eax]
  }
  return r;
}
X80 x80_add_two_floats(U a, U b, U c) {
  X80 r; float x = F(a), y = F(b), z = F(c); unsigned char* q = r.b;
  __asm {
    fld x
    fadd y
    fadd z
    mov eax, q
    fstp tbyte ptr [eax]
  }
  return r;
}
U x80_to_float(const X80& a) {
  float res; const unsigned char* pa = a.b;
  __asm {
    mov eax, pa
    fld tbyte ptr [eax]
    fstp res
  }
  U u; std::memcpy(&u, &res, 4); return u;
}
unsigned short cmp_r2_sum(const X80& r, U a, U b, U c) {
  float x = F(a), y = F(b), z = F(c); const unsigned char* pr = r.b; unsigned short sw;
  __asm {
    mov eax, pr
    fld tbyte ptr [eax]
    fld x
    fmul x
    fld y
    fmul y
    faddp st(1), st(0)
    fld z
    fmul z
    faddp st(1), st(0)
    fld st(1)
    fmul st(0), st(2)
    fcompp
    fnstsw ax
    fstp st(0)
    mov sw, ax
  }
  return sw;
}
U ftol_sqrt_float(U v) {
  float x = F(v); X80 r; unsigned char* q = r.b;
  __asm {
    fld x
    fsqrt
    mov eax, q
    fstp tbyte ptr [eax]
  }
  return chop_ext(r);
}
U ftol_mul_float(const X80& m, U k) {
  float x = F(k); X80 r; const unsigned char* pm = m.b; unsigned char* q = r.b;
  __asm {
    mov eax, pm
    fld tbyte ptr [eax]
    fmul x
    mov eax, q
    fstp tbyte ptr [eax]
  }
  return chop_ext(r);
}
bool mul_above_ftol(const X80& m, U k1, U k2, U& res) {
  float a = F(k1), b = F(k2); X80 r; const unsigned char* pm = m.b; unsigned char* q = r.b; unsigned short sw;
  __asm {
    mov eax, pm
    fld tbyte ptr [eax]
    fmul a
    fcom b
    fnstsw ax
    mov sw, ax
    fsub b
    mov eax, q
    fstp tbyte ptr [eax]
  }
  if ((sw >> 8) & 0x41) return false;                       // jne: below, equal or unordered
  res = chop_ext(r);
  return true;
}
U listen_blocker_sum(U h2x, U h2y, U h2z, U h1x, U h1y, U h1z) {
  float p2x = F(h2x), p2y = F(h2y), p2z = F(h2z), p1x = F(h1x), p1y = F(h1y), p1z = F(h1z);
  float dyf, dzf, dzf2, dxf, res;
  __asm {
    fld p2x
    fsub p1x
    fld st(0)
    fld p2y
    fsub p1y
    fst dyf
    fld p2z
    fsub p1z
    fst dzf
    fstp dzf2
    fxch st(2)
    fstp dxf
    fld dzf
    fmul dzf2
    fxch st(2)
    fmul dyf
    faddp st(2), st(0)
    fld dxf
    fmul st(0), st(1)
    faddp st(2), st(0)
    fxch st(1)
    fstp res
    fstp st(0)
  }
  U u; std::memcpy(&u, &res, 4); return u;
}
bool sumsq_not_above_double(U x, U y, U z, double k) {
  float a = F(x), b = F(y), c = F(z); unsigned short sw;
  __asm {
    fld c
    fmul c
    fld b
    fmul b
    faddp st(1), st(0)
    fld a
    fmul a
    faddp st(1), st(0)
    fcomp k
    fnstsw ax
    mov sw, ax
  }
  return ((sw >> 8) & 0x41) != 0;
}
bool dot_not_above(U ax, U ay, U az, U bx, U by, U bz, U c) {
  float a0 = F(ax), a1 = F(ay), a2 = F(az), b0 = F(bx), b1 = F(by), b2 = F(bz), k = F(c); unsigned short sw;
  __asm {
    fld a2
    fmul b2
    fld a1
    fmul b1
    faddp st(1), st(0)
    fld a0
    fmul b0
    faddp st(1), st(0)
    fcomp k
    fnstsw ax
    mov sw, ax
  }
  const unsigned m = (sw >> 8) & 0x41;
  return m == 0x01 || m == 0x40;
}
bool trigger_radial_far(const U in[3], const U pos[3], U d[3]) {
  float i0 = F(in[0]), i1 = F(in[1]), i2 = F(in[2]), p0 = F(pos[0]), p1 = F(pos[1]), p2 = F(pos[2]), vx, vy, vz;
  const float k = F(0x38d1b717u); unsigned short sw;
  __asm {
    fld i0
    fsub p0
    fstp vx
    fld i1
    fsub p1
    fstp vy
    fld i2
    fsub p2
    fstp vz
    fld vz
    fmul vz
    fld vy
    fmul vy
    faddp st(1), st(0)
    fld vx
    fmul vx
    faddp st(1), st(0)
    fcomp k
    fnstsw ax
    mov sw, ax
  }
  std::memcpy(&d[0], &vx, 4); std::memcpy(&d[1], &vy, 4); std::memcpy(&d[2], &vz, 4);
  return ((sw >> 8) & 0x41) == 0;
}
void trigger_radial_point(const U n[3], U r, const U pos[3], U res[3]) {
  float n0 = F(n[0]), n1 = F(n[1]), n2 = F(n[2]), rr = F(r), p0 = F(pos[0]), p1 = F(pos[1]), p2 = F(pos[2]);
  float zp, o0, o1, o2;
  __asm {
    fld n0
    fmul rr
    fld n1
    fmul rr
    fld n2
    fmul rr
    fstp zp
    fxch st(1)
    fadd p0
    fstp o0
    fadd p1
    fstp o1
    fld zp
    fadd p2
    fstp o2
  }
  std::memcpy(&res[0], &o0, 4); std::memcpy(&res[1], &o1, 4); std::memcpy(&res[2], &o2, 4);
}
bool trigger_edge_nearest(const U in[3], const U a[3], const U b[3], U& best, U point[3]) {
  float i0 = F(in[0]), i1 = F(in[1]), i2 = F(in[2]), a0 = F(a[0]), a1 = F(a[1]), a2 = F(a[2]), b0 = F(b[0]), b1 = F(b[1]), b2 = F(b[2]);
  float ex, ey, ez, t, q0, q1, q2, zprod, bst = F(best);
  const double one = 1.0, zero = 0.0; unsigned short sw1, sw2, sw3;
  __asm {
    fld i0
    fsub a0
    fld i1
    fsub a1
    fld i2
    fsub a2
    fld b0
    fsub a0
    fstp ex
    fld b1
    fsub a1
    fstp ey
    fld b2
    fsub a2
    fst ez
    fmul st(0), st(1)
    fld ey
    fmul st(0), st(3)
    faddp st(1), st(0)
    fld ex
    fmul st(0), st(4)
    faddp st(1), st(0)
    fld ez
    fmul ez
    fld ey
    fmul ey
    faddp st(1), st(0)
    fld ex
    fmul ex
    faddp st(1), st(0)
    fdivp st(1), st(0)
    fstp t
    fstp st(0)
    fstp st(0)
    fstp st(0)
    fld t
    fcomp one
    fnstsw ax
    mov sw1, ax
  }
  if (((sw1 >> 8) & 0x41) == 0) {                         // t > 1: the far end (computed as a + e)
    __asm {
      fld ex
      fadd a0
      fstp q0
      fld ey
      fadd a1
      fstp q1
      fld ez
      fadd a2
      fstp q2
    }
  } else {
    __asm {
      fld t
      fcomp zero
      fnstsw ax
      mov sw2, ax
    }
    const unsigned m = (sw2 >> 8) & 5;
    if (!(m == 0 || m == 5)) {                            // t < 0: the near end
      q0 = a0; q1 = a1; q2 = a2;
    } else {
      __asm {
        fld ex
        fmul t
        fld ey
        fmul t
        fld ez
        fmul t
        fstp zprod
        fxch st(1)
        fadd a0
        fstp q0
        fadd a1
        fstp q1
        fld zprod
        fadd a2
        fstp q2
      }
    }
  }
  float nb;
  __asm {
    fld i0
    fsub q0
    fld i1
    fsub q1
    fld i2
    fsub q2
    fld st(0)
    fmul st(0), st(1)
    fld st(2)
    fmul st(0), st(3)
    faddp st(1), st(0)
    fld st(3)
    fmul st(0), st(4)
    faddp st(1), st(0)
    fstp st(3)
    fstp st(0)
    fstp st(0)
    fcom bst
    fnstsw ax
    mov sw3, ax
    fstp nb
  }
  const unsigned m3 = (sw3 >> 8) & 5;
  if (m3 == 0 || m3 == 5) return false;                     // jp: not closer
  std::memcpy(&best, &nb, 4);
  std::memcpy(&point[0], &q0, 4); std::memcpy(&point[1], &q1, 4); std::memcpy(&point[2], &q2, 4);
  return true;
}
bool mine_within_trigger(const U pos[3], const U q[3], U r2) {
  float p0 = F(pos[0]), p1 = F(pos[1]), p2 = F(pos[2]), q0 = F(q[0]), q1 = F(q[1]), q2 = F(q[2]), r = F(r2); unsigned short sw;
  __asm {
    fld p2
    fsub q2
    fld p1
    fsub q1
    fld p0
    fsub q0
    fld st(0)
    fmul st(0), st(1)
    fld st(2)
    fmul st(0), st(3)
    faddp st(1), st(0)
    fld st(3)
    fmul st(0), st(4)
    faddp st(1), st(0)
    fcomp r
    fstp st(0)
    fnstsw ax
    fstp st(0)
    fstp st(0)
    mov sw, ax
  }
  const unsigned m = (sw >> 8) & 0x41;
  return m == 0x01 || m == 0x40;
}
bool mine_within_object(const U pos[3], const U obj[3], U r2) {
  float p0 = F(pos[0]), p1 = F(pos[1]), p2 = F(pos[2]), o0 = F(obj[0]), o1 = F(obj[1]), o2 = F(obj[2]), r = F(r2); unsigned short sw;
  __asm {
    fld p0
    fsub o0
    fld p1
    fsub o1
    fld p2
    fsub o2
    fld st(2)
    fmul st(0), st(3)
    fld st(1)
    fmul st(0), st(2)
    faddp st(1), st(0)
    fld st(2)
    fmul st(0), st(3)
    faddp st(1), st(0)
    fcomp r
    fstp st(0)
    fnstsw ax
    fstp st(0)
    fstp st(0)
    mov sw, ax
  }
  const unsigned m = (sw >> 8) & 0x41;
  return m == 0x01 || m == 0x40;
}
bool hp_ratio_below(S cur, S max, U k) {
  int c = cur, m = max; float cf, kk = F(k); unsigned short sw;
  __asm {
    fild c
    fstp cf
    fild m
    fdivr cf
    fcomp kk
    fnstsw ax
    mov sw, ax
  }
  return ((sw >> 8) & 5) == 1;
}
void nearest_behind_point(const U o[3], const U pos[3], U k, U out[3]) {
  float kk = F(k), ox = F(o[0]), oy = F(o[1]), oz = F(o[2]), px = F(pos[0]), py = F(pos[1]), pz = F(pos[2]), tmp, rx, ry, rz;
  __asm {
    fld oz
    fmul kk
    fld oy
    fmul kk
    fld ox
    fmul kk
    fstp tmp
    fxch st(1)
    fadd pz
    fstp rz
    fadd py
    fstp ry
    fld tmp
    fadd px
    fstp rx
  }
  out[0] = B(rx); out[1] = B(ry); out[2] = B(rz);
}
bool nearest_in_range(U dx, U dy, U dz, U r, U& dist2) {
  float x = F(dx), y = F(dy), z = F(dz), rr = F(r), d2; unsigned short sw;
  __asm {
    fld x
    fmul x
    fld z
    fmul z
    faddp st(1), st
    fld y
    fmul y
    faddp st(1), st
    fst d2
    fld rr
    fld st(0)
    fmul st, st(1)
    fxch st(1)
    fxch st(2)
    fcompp
    fnstsw ax
    mov sw, ax
    fstp st(0)
  }
  dist2 = B(d2);
  return ((sw >> 8) & 5) == 1;
}
X80 x80_sub_float(const X80& a, U f) {
  X80 r; float x = F(f); const unsigned char* pa = a.b; unsigned char* q = r.b;
  __asm {
    mov eax, pa
    fld tbyte ptr [eax]
    fsub x
    mov eax, q
    fstp tbyte ptr [eax]
  }
  return r;
}
X80 nearest_wrap_angle(const X80& a, U zero, U full, U limit, U& stored) {
  X80 r; float z = F(zero), fl = F(full), lim = F(limit), stv = 0; const unsigned char* pa = a.b; unsigned char* q = r.b;
  __asm {
    mov eax, pa
    fld tbyte ptr [eax]
    fcom z
    fst stv
    fnstsw ax
    test ah, 5
    jp no_low
  low_loop:
    fadd fl
    fcom z
    fnstsw ax
    test ah, 5
    jnp low_loop
    fst stv
  no_low:
    fcom lim
    fnstsw ax
    test ah, 0x41
    jne no_high
  high_loop:
    fsub fl
    fcom lim
    fnstsw ax
    test ah, 0x41
    je high_loop
    fst stv
  no_high:
    mov eax, q
    fstp tbyte ptr [eax]
  }
  stored = B(stv);
  return r;
}
bool x80_less_than_float(const X80& a, U f) {
  float x = F(f); const unsigned char* pa = a.b; unsigned short sw;
  __asm {
    mov eax, pa
    fld tbyte ptr [eax]
    fcomp x
    fnstsw ax
    mov sw, ax
  }
  return ((sw >> 8) & 5) == 1;
}
namespace {
unsigned short dot_status(const U a[3], const U b[3], const void* k, bool dbl) {
  float va0 = F(a[0]), va1 = F(a[1]), va2 = F(a[2]), vb0 = F(b[0]), vb1 = F(b[1]), vb2 = F(b[2]); bool is_double = dbl; unsigned short sw;
  __asm {
    fld va2
    fmul vb2
    fld va1
    fmul vb1
    faddp st(1), st
    fld va0
    fmul vb0
    faddp st(1), st
    mov eax, k
    mov cl, is_double
    test cl, cl
    je single
    fcomp qword ptr [eax]
    jmp done
  single:
    fcomp dword ptr [eax]
  done:
    fnstsw ax
    mov sw, ax
  }
  return sw;
}
}
unsigned short dot_status_float(const U a[3], const U b[3], U c) { float k = F(c); return dot_status(a, b, &k, false); }
unsigned short dot_status_double(const U a[3], const U b[3], double k) { return dot_status(a, b, &k, true); }
U load_store_float(U v) {
  float x = F(v), r;
  __asm {
    fld x
    fstp r
  }
  return B(r);
}
void facing_from_angle(U angle, U k, U out[2]) {
  float a = F(angle), kk = F(k), c, sn;
  __asm {
    fld a
    fmul kk
    fld st(0)
    fcos
    fstp c
    fsin
    fstp sn
  }
  out[0] = B(c); out[1] = B(sn);
}
bool facing_point_delta(const U pt[3], const U pos[3], U d[3]) {
  float px = F(pt[0]), py = F(pt[1]), pz = F(pt[2]), qx = F(pos[0]), qy = F(pos[1]), qz = F(pos[2]), z = 0.0f, fpdx, fpdy, fpdz;
  int result;
  __asm {
    fld pz
    fsub qz
    fld py
    fsub qy
    fld px
    fsub qx
    fst fpdx
    fld st(1)
    fstp fpdy
    fld st(2)
    fstp fpdz
    fcomp z
    fnstsw ax
    test ah, 0x44
    jp pop2
    fcomp z
    fnstsw ax
    test ah, 0x44
    jp pop1
    fcomp z
    fnstsw ax
    test ah, 0x44
    jnp zero
    jmp nonzero
  pop2:
    fstp st(0)
  pop1:
    fstp st(0)
  nonzero:
    mov result, 1
    jmp done
  zero:
    mov result, 0
  done:
  }
  d[0] = B(fpdx); d[1] = B(fpdy); d[2] = B(fpdz);
  return result != 0;
}
U sum_sq_xzy(U x, U y, U z) {
  float vx = F(x), vy = F(y), vz = F(z), r;
  __asm {
    fld vx
    fmul vx
    fld vz
    fmul vz
    faddp st(1), st
    fld vy
    fmul vy
    faddp st(1), st
    fstp r
  }
  return B(r);
}
bool delta_sq_above(U a, U b, U v) {
  float va = F(a), vb = F(b), vv = F(v); unsigned short sw;
  __asm {
    fld va
    fsub vb
    fld st(0)
    fmul st, st(1)
    fcomp vv
    fnstsw ax
    mov sw, ax
    fstp st(0)
  }
  return ((sw >> 8) & 0x41) == 0;
}
bool float_below_or_unordered(U a, U b) {
  float va = F(a), vb = F(b); unsigned short sw;
  __asm {
    fld va
    fcomp vb
    fnstsw ax
    mov sw, ax
  }
  return ((sw >> 8) & 1) != 0;
}
U ftol_add_double_div(U d, double c, U r) {
  float fd = F(d), fr = F(r); X80 v; unsigned char* q = v.b;
  __asm {
    fld fd
    fadd c
    fdiv fr
    mov eax, q
    fstp tbyte ptr [eax]
  }
  return chop_ext(v);                                     // __ftol2
}
SafeStep safe_direction_point(S i, U kf, const U side[2], const U n[3], const U start[3]) {
  SafeStep s; int ii = i; float zero = 0.0f, k = F(kf), sx = F(side[0]), sy = F(side[1]), nx = F(n[0]), ny = F(n[1]), nz = F(n[2]);
  float s0 = F(start[0]), s1 = F(start[1]), s2 = F(start[2]), oy, oz, fzo, fx, fy, fz, gx, gy, gz;
  __asm {
    fild ii
    fld sx
    fmul st, st(1)
    fld sy
    fmul st, st(2)
    fstp oy
    fxch st(1)
    fmul zero
    fstp oz
    fld k
    fmul nx
    fld k
    fmul ny
    fld nz
    fmul k
    fstp fzo
    fxch st(1)
    fadd s0
    fstp fx
    fadd s1
    fstp fy
    fld fzo
    fadd s2
    fstp fz
    fld fx
    fadd st, st(1)
    fstp gx
    fstp st(0)
    fld fy
    fadd oy
    fstp gy
    fld fz
    fadd oz
    fstp gz
  }
  s.oy = B(oy); s.oz = B(oz); s.fz_offset = B(fzo);
  s.f[0] = B(fx); s.f[1] = B(fy); s.f[2] = B(fz); s.g[0] = B(gx); s.g[1] = B(gy); s.g[2] = B(gz);
  return s;
}
bool add_store_below(U& d, U r, U limit) {
  float fd = F(d), fr = F(r), fl = F(limit), sto; unsigned short sw;
  __asm {
    fld fd
    fadd fr
    fst sto
    fcomp fl
    fnstsw ax
    mov sw, ax
  }
  d = B(sto);
  return ((sw >> 8) & 5) == 1;
}
}
