#pragma once
#include "93d73ae698-w1_ports.hpp"
// Exact x87 sequences of the original (extended-precision intermediates, current FPU control word).
// x86-only (MSVC/clang-cl __asm). Values are passed as IEEE single bit patterns (U) or signed ints.
namespace w1::x87 {
// 005A5C70 / 005A5CC0 body: acc += add (fadd/fst float), n=(short)ftol(floor((double)extended sum)),
// acc = acc_float - n. Returns EAX (the full __ftol2 low dword; callers use AX) and the new accumulator bits in *acc.
U accumulator_adjust(U& acc, U add);
// 004FE6B1..004FE6C9 / 004FE7F7..004FE812: true when rate*0.01*scale < 1.0e-4 (ordered, strict).
bool regen_below_minimum(U rate, S scale);
// 004FE6CE/004FE6E9..004FE725 (and FP twin): (use_minimum ? 1e-4 : rate*0.01*scale) * ((float)(unsigned)dt * 0.001f),
// rounded to single (fstp dword).
U regen_amount(U rate, S scale, bool use_minimum, U dt);
// fild dword v; fstp dword -> single bits.
U int_to_float(S v);
// fild a; fcomp dword f; true when a < f (C0 set, ordered).
bool int_less_than_float(S a, U f);
// __ftol2 on a single value: truncation to int64, low dword returned.
U ftol_float(U f);
// 0051B14F..0051B19E / 0051B2BE..0051B30D: d = b - a per axis (extended), ((dz*dz + dy*dy) + dx*dx) fcomp r2;
// true when the original falls through to the perception call (sum <= r2, ordered).
bool within_squared(U ax, U ay, U az, U bx, U by, U bz, U r2);
// 00502D4B..00502D88: d = m - t per axis (extended), sum=((dx*dx + dy*dy) + dz*dz); fld r; fmul r; fcompp
// (r*r kept extended). True when the original takes the jp to the line-of-sight test (r*r > sum, or unordered).
bool perception_range_open(U mx, U my, U mz, U tx, U ty, U tz, U r);
// fld dword a; fadd dword b; fstp dword.
U add_float(U a, U b);
// fld dword a; fsub dword b; fstp dword.
U sub_float(U a, U b);
// ---- 80-bit ST0 values (functions returning in ST0 keep extended precision in the original)
struct X80 { unsigned char b[10]; };
X80 x80_from_float(U f);                          // fld dword
X80 x80_add_float(const X80& a, U f);             // fld tbyte a; fadd dword f
X80 x80_add_two_floats(U a, U b, U c);            // fld a; fadd b; fadd c
U x80_to_float(const X80& a);                     // fstp dword
// fld st(1) (r); fmul st(2): r*r vs sum=((a*a)+(b*b))+(c*c) with float loads; fcompp. Returns FPU status word.
unsigned short cmp_r2_sum(const X80& r, U a, U b, U c);
// 004FB7C8..004FB7D5: __ftol2(fsqrt(fld dword v)).
U ftol_sqrt_float(U v);
// fld tbyte m; fmul dword k; __ftol2.
U ftol_mul_float(const X80& m, U k);
// 004F2399..004F23C4: m*k1 fcom k2; if (m*k1 > k2, ordered) out=__ftol2(m*k1-k2) and true; else false.
bool mul_above_ftol(const X80& m, U k1, U k2, U& out);
// 004FB749..004FB793: blocker positions h2-h1 per axis; returns float((dz*dz + dy*dyf) + dxf*dx) as the original mixes.
U listen_blocker_sum(U h2x, U h2y, U h2z, U h1x, U h1y, U h1z);
// ((z*z + y*y) + x*x) fcomp qword k -> true when the original takes `jne` (sum <= k or unordered).
bool sumsq_not_above_double(U x, U y, U z, double k);
// ((az*bz + ay*by) + ax*bx) fcomp dword c -> true when the original applies the penalty (dot <= c, ordered).
bool dot_not_above(U ax, U ay, U az, U bx, U by, U bz, U c);
// ---- 0058C8A0 CSWSTrigger::CalculateNearestPoint pieces
// Radial branch, part 1 (0058C959..0058C9C2): d = in - pos as floats into d[3];
// true when (dz*dz + dy*dy) + dx*dx > 1e-4f (ordered), i.e. the original normalizes.
bool trigger_radial_far(const U in[3], const U pos[3], U d[3]);
// Radial branch, part 2 (0058C9D7..0058CA42): n = normalized direction, r = radius:
// out = (float(n.x*r + pos.x), float(n.y*r + pos.y), float(float(n.z*r) + pos.z)).
void trigger_radial_point(const U n[3], U r, const U pos[3], U out[3]);
// Polygon branch, one edge (0058CAAE..0058CC14): projects `in` onto segment a->b with the original mixed precision;
// when the squared distance is below `best` (ordered strict), stores the new best (float) and the point; returns true.
bool trigger_edge_nearest(const U in[3], const U a[3], const U b[3], U& best, U point[3]);
// 004FA601..004FA638: d = pos - q; ((dx*dx + dy*dy) + dz*dz) fcomp r2; true when within (sum <= r2, ordered).
bool mine_within_trigger(const U pos[3], const U q[3], U r2);
// 004FA7F1..004FA82E / 004FA944..004FA981: d = pos - obj; ((dx*dx + dz*dz) + dy*dy) fcomp r2; true when within.
bool mine_within_object(const U pos[3], const U obj[3], U r2);
// 004EFF4C..004EFF7F: f = float(cur) (fild/fstp dword); (f / (extended) max) fcomp dword k; true when below (C0 alone).
bool hp_ratio_below(S cur, S max, U k);
// ---- 004FC4C0 CSWSCreature::GetNearestObjects pieces
// 004FC5B7..004FC616: P = pos + o*k with the original rounding: P.z = float(o.z*k + pos.z), P.y = float(o.y*k + pos.y),
// P.x = float(float(o.x*k) + pos.x).
void nearest_behind_point(const U o[3], const U pos[3], U k, U out[3]);
// 004FC73B..004FC78A: sum = ((dx*dx) + (dz*dz)) + (dy*dy) (extended); *dist2 = float(sum); fld r; r*r (extended); fcompp;
// true when the original continues (sum < r*r, ordered).
bool nearest_in_range(U dx, U dy, U dz, U r, U& dist2);
// fld tbyte a; fsub dword f.
X80 x80_sub_float(const X80& a, U f);
// 004FC7A4..004FC803: wraps the relative yaw (extended) the original way: while a < zero (ordered) a += full; while
// a > limit (ordered) a -= full. *stored receives every fst dword the original performs (the last one wins).
X80 nearest_wrap_angle(const X80& a, U zero, U full, U limit, U& stored);
// fld tbyte a; fcomp dword f; true when a < f (C0 alone).
bool x80_less_than_float(const X80& a, U f);
// ((a.z*b.z + a.y*b.y) + a.x*b.x) fcomp dword c / qword k: FPU status word (AH in bits 8..15).
unsigned short dot_status_float(const U a[3], const U b[3], U c);
unsigned short dot_status_double(const U a[3], const U b[3], double k);
// fld dword v; fstp dword (004F78C0 moves floats through the FPU).
U load_store_float(U v);
// ---- 00542830 ExecuteCommandSetFacing
// 0054289F..005428D5: a = angle * k (extended); cos -> out[0], sin -> out[1] (fcos on a copy, fsin on the original).
void facing_from_angle(U angle, U k, U out[2]);
// 00542914..00542969: d = p - pos per axis (extended), stored as floats into d[]; returns true unless all three extended
// differences compare equal to 0.0f (ordered), i.e. when the original normalizes.
bool facing_point_delta(const U pt[3], const U pos[3], U d[3]);
// ---- 0054B550 ExecuteCommandGetNearestObject
// 0054BD9D..0054BDD1: float(((x*x) + (z*z)) + (y*y)).
U sum_sq_xzy(U x, U y, U z);
// 0054BDD5..0054BDEC: d = a - b (extended); d*d fcomp dword v; true when d*d > v (ordered).
bool delta_sq_above(U a, U b, U v);
// fld dword a; fcomp dword b; test ah,1: true when C0 is set (a < b, or unordered).
bool float_below_or_unordered(U a, U b);
// ---- 004C0FA0 ComputeSafeLocationInDirection
// 004C11A0..004C11B1: __ftol2((d + c) / r) with d, r single and c double.
U ftol_add_double_div(U d, double c, U r);
// 004C11E0..004C1281: one sideways candidate (see safe_location.cpp); stores exactly the floats the original stores.
struct SafeStep { U oy, oz, fz_offset, f[3], g[3]; };
SafeStep safe_direction_point(S i, U kf, const U side[2], const U n[3], const U start[3]);
// 004C1366..004C1381: d = float(d + r) (fst), compared unrounded: true when d + r < limit (C0 alone).
bool add_store_below(U& d, U r, U limit);
}
