// t13: the four CSWCProjectile motion functions ("motion4").
//   006D33C0 launch / motion setup    object_projectile_006d33c0  void  fastcall @12  RET 4   (no FuncInfo)
//   006D7590 per-frame motion step    object_projectile_006d7590  void  fastcall @8   RET 0   (no FuncInfo)
//   006D2E00 homing velocity update   object_projectile_006d2e00  void  fastcall @8   RET 0   (no FuncInfo)
//   006D30C0 curving velocity update  object_projectile_006d30c0  void  fastcall @8   RET 0   (no FuncInfo)
// Ownership: reassigned by user from the sol projectile lanes, 2026-10-15, PENDING-REGISTRATION.
// Written from my own static disassembly of swkotor.exe. The paused sol-projectile-motion t2 source (four naked
// whole-body asm transcriptions) was read and checked line by line; none of its asm is used (see INTEGRATION.md).
// Declarations of the four exports: shared/object_projectile_internal.h (unchanged pinned copy). Every callee is a
// selected provider, another projectile export under its exact name (t9 006D4250, t12 006D70D0, t11b 006FC0E0), or one
// of the four functions here.
//
// Projectile fields used (as read and written by the four functions):
//   +024 Vector position (SetPosition slot +08Ch writes it)   +030 Vector orientation   +044 elapsed ms
//   +100 Vector previous position   +114 Vector destination   +120 remaining ms   +124 target id   +128 CExoString node
//   +158 BYTE attach mode   +164 done flag   +178 Vector velocity   +184 Vector acceleration   +198 current path event
//   +19C Gob   +1A0 BYTE curve side   +1A1 BYTE orientation mode
// Path event (34h bytes, see t12): +00 point  +0C second point  +18 target id  +24 lead time  +28 float radius
//   +2D BYTE type (1..13)
//
// x87: see x87_arith.h (t12, unchanged). Register values are doubles, DWORD memory operands M32, with the x87 NaN
// selection. The minimal x87 helpers below exist only where C++ has no operation: fcos, fsin, fldln2/fyl2x, and the
// 80-bit ST0 transport around the CRT acos 006FC0E0 (t11b export).
#include "8b103f2541-object_projectile_internal.h"
#include "74c2dbe6b5-object_projectile_providers.h"
#include "ed1c25ba9a-object_projectile_literals.h"
#include "fd26c081f1-crt_acos_006fc0e0.h"
#include "62caa84031-m15e.h"
#include "f58c269829-scene3d.h"
#include "9963eab06a-vtables_gen.h"
#include "b6906bba0c-startup_string.h"
#include "7f8fd7121f-x87_arith.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <type_traits>

using U = std::uint32_t;
using B = std::uint8_t;
using S = kotor_reconstruction::StartupString;
using kotor_scene3d::Quaternion;
using kotor_scene3d::Vector;
using x87::M32;

namespace menu_tables_m41 { namespace cells { extern const unsigned int invalid_object_00746644; } }
namespace base_ini_reconstruction { extern const char warning_empty_0073d71c[]; }
extern "C" {
// Selected providers (scene3d.h declares 004AB100 / 004AB180 / 00423B90).
float* __cdecl kotor_cdecl_00423bc0(float* out, const float* a, const float* b);               // out = a - b
float* __cdecl vector_component_mult_00423bf0(float* out, const float* v, float scale);        // out = scale * v
float* __cdecl float_times_vector_00437a60(float* out, float scale, const float* v);           // out = scale * v
float* __fastcall vector_add_assign_004a9740(float* self, void*, const float* other);          // self += other, RET 4
aurora_model::Gob* __cdecl aurora_new_aur_object_00449cc0(const char* name, const char* gob_name, std::FILE* mdl, std::FILE* mdx);
}

// The exports are the shared-header declarations (t9, t11b and t12 import them by these names).
static_assert(std::is_same_v<decltype(&object_projectile_006d33c0), void (__fastcall*)(void*, void*, Vector*)>, "33C0");
static_assert(std::is_same_v<decltype(&object_projectile_006d7590), void (__fastcall*)(void*, void*)>, "7590");
static_assert(std::is_same_v<decltype(&object_projectile_006d2e00), void (__fastcall*)(void*, void*)>, "2E00");
static_assert(std::is_same_v<decltype(&object_projectile_006d30c0), void (__fastcall*)(void*, void*)>, "30C0");

namespace {
// Original immediates (instruction bytes, not image data).
constexpr U kInvalidId = 0x7f000000u;    // cmp dword [esi+124h], 7F000000h (006D3422)
constexpr U kGravity = 0xc11cf5c3u;      // -9.81f, mov [esp+18h], 0C11CF5C3h (006D378F)
constexpr U kLobSpeed = 0x40f00000u;     // 7.5f, push 40F00000h (006D36A9)
constexpr U kTwo = 0x40000000u;          // 2.0f, push 40000000h (006D390A / 006D3AD6 / 006D3B66 / 006D3F85)
constexpr U kOne = 0x3f800000u;          // 1.0f, the identity quaternion w (006D34A3 / 006D3CE2 / 006D7618 / 006D7897)
constexpr U kPart = 0xffu, kForce = 1u;  // GetModel(FFh, 1)

template<class T> T rd(const void* p, U n) { T v; std::memcpy(&v, static_cast<const B*>(p) + n, sizeof v); return v; }
template<class T> void wr(void* p, U n, T v) { std::memcpy(static_cast<B*>(p) + n, &v, sizeof v); }
void* at(void* p, U n) { return static_cast<B*>(p) + n; }
void* ptr(const void* p, U n) { return rd<void*>(p, n); }
void* client() { return ptr(kotor_menu_window_cells::manager_007a39fc, 4); }   // [[007A39FC]+4]
U word_of(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
void* pointer_of(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
M32 lit(const unsigned char* bytes) { return x87::m32(bytes); }                 // t9 literal arrays (image bytes)
M32 f(const void* p, U n) { return x87::m32_at(p, n); }                         // DWORD float field
double literal_double(const unsigned char* bytes) { double d; std::memcpy(&d, bytes, sizeof d); return d; }
const KotorM15ObjectMethods_007516c8* object_table(const void* o) { return rd<const KotorM15ObjectMethods_007516c8*>(o, 0); }
const aurora_model::GobVTable* gob_table(const void* g) { return rd<const aurora_model::GobVTable*>(g, 0); }
aurora_model::Gob* as_gob(void* g) { return static_cast<aurora_model::Gob*>(g); }
S* string_at(void* p, U n) { return static_cast<S*>(at(p, n)); }

// Three DWORDs as the original moves them (integer copies; never through the FPU).
struct Point { M32 x, y, z; };
static_assert(sizeof(Point) == 12, "Vector-sized");
constexpr Point kZero = {M32{0}, M32{0}, M32{0}};
Point point_at(const void* p, U n) { return {f(p, n), f(p, n + 4), f(p, n + 8)}; }
void put_point(void* p, U n, Point v) { x87::put_at(p, n, v.x); x87::put_at(p, n + 4, v.y); x87::put_at(p, n + 8, v.z); }
Vector vector_of(Point v) { Vector r; std::memcpy(&r, &v, sizeof r); return r; }
Quaternion identity() { Quaternion q; const U w[4] = {kOne, 0, 0, 0}; std::memcpy(&q, w, sizeof q); return q; }
float float_of(U bits) { float v; std::memcpy(&v, &bits, sizeof v); return v; }

// x87 forms x87_arith.h does not name.
double sub_rr(double a, double b) { return x87::rr(a, b, a - b); }              // FSUBP / FSUBRP of two registers: a - b
double divr(double st, M32 m) { return x87::rm(st, m, x87::ld(m) / st); }      // FDIVR DWORD: m / ST0
// fild dword [field]; test; jge; fadd dword [0073D770] (2^32): the field as unsigned, exactly.
double unsigned_field(const void* p, U n) {
    const std::int32_t v = rd<std::int32_t>(p, n);
    const double d = static_cast<double>(v);
    return v < 0 ? x87::add(d, lit(object_projectile_literal_0073d770)) : d;
}
// fild ...; fmul dword [0073D708] (0.001f): milliseconds as seconds (53 bits, kept wide).
double seconds_of(const void* p, U n) { return x87::mul(unsigned_field(p, n), lit(object_projectile_literal_0073d708)); }

// ---- minimal x87 helpers (C++ has no fcos / fsin / fyl2x and no 80-bit value)
// fld st(0); fcos; fstp dword (006D3825..006D3829 and the two other copies)
__declspec(noinline) M32 x87_cos_to_m32(double angle) {
    U result;
    __asm {
        fld angle
        fcos
        fstp dword ptr [result]
    }
    return M32{result};
}
// fsin; fstp dword (006D382D..006D382F and the two other copies)
__declspec(noinline) M32 x87_sin_to_m32(double angle) {
    U result;
    __asm {
        fld angle
        fsin
        fstp dword ptr [result]
    }
    return M32{result};
}
// ST0 = |v| (the 53-bit 004AB100 result); fldln2; fxch st(1); fyl2x; fmul qword [00753D98] (006D386F..006D3875):
// ln|v| with a 64-bit significand (fyl2x is not precision controlled), times 5.0 rounded once to 53 bits.
__declspec(noinline) double x87_ln_times(double magnitude, double factor) {
    double result;
    __asm {
        fld magnitude
        fldln2
        fxch st(1)
        fyl2x
        fmul factor
        fstp result
    }
    return result;
}
// fld dword [esp+28h] (006D37B4): the 006FC0E0 operand as the caller's ST0 (an sNaN is quieted by the load)
__declspec(noinline) void x87_load_m32(const M32* source, crt_x87::Extended* st0) {
    __asm {
        mov eax, source
        fld dword ptr [eax]
        mov eax, st0
        fstp tbyte ptr [eax]
    }
}
// fstp dword [esp+24h] (006D37CE): the 006FC0E0 result, rounded once from 64 bits
__declspec(noinline) M32 x87_store_m32(const crt_x87::Extended* st0) {
    U result;
    __asm {
        mov eax, st0
        fld tbyte ptr [eax]
        fstp dword ptr [result]
    }
    return M32{result};
}


void* get_model(void* object) { return object_table(object)->s098_get_model(object, nullptr, kPart, kForce); }   // slot +098 (FFh, 1)
// Gob::GetPartPosition (Gob slot +098)([self+128h] node name, &position, &orientation)
U part_position(void* gob, void* self, Point* position, Quaternion* orientation) {
    const char* const name = WindowCStr005e5670(string_at(self, 0x128), nullptr);
    return gob_table(gob)->s098_gob_get_part_position(as_gob(gob), nullptr, word_of(name), word_of(position), word_of(orientation));
}
// Gob::ReattachReference (Gob slot +0A4)(other, name)
void reattach(void* gob, void* other, const char* name) {
    (void)gob_table(gob)->s0a4_gob_reattach_reference(as_gob(gob), nullptr, word_of(other), word_of(name));
}

// ======================================================================================== 006D7590 helpers
// 006D764A..006D7744 / 006D78C9..006D79DB: the target object's node [self+128h], else its position raised by 1.25
// (attach modes 2 and 3 also move it by -0.5 x the projectile's orientation). The second GetModel result is used
// unchecked by the original.
void target_point(void* self, void* object, Point& p, Quaternion& q) {
    if (get_model(object) == nullptr) {                                                  // 006D7659..006D7661
        p = point_at(self, 0x114);                                                       // 006D7746..006D775C
        return;
    }
    void* const gob = get_model(object);                                                 // 006D7672 / 006D78F1
    if (part_position(gob, self, &p, &q) != 0) return;                                   // 006D7694..006D769C
    const B mode = rd<B>(self, 0x158);                                                   // 006D76A2
    p = point_at(object, 0x24);                                                          // 006D76B0..006D76C3
    p.z = x87::st(x87::add(x87::ld(p.z), lit(object_projectile_literal_00757ca8)));       // fadd dword [00757CA8] (1.25f)
    if (mode == 3 || mode == 2) {                                                         // 006D76A8..006D76AE
        // 006D7708..006D773B: -orientation * 0.5 (the negated z is stored once before its product)
        const M32 negative_z = x87::st(-x87::ld(f(self, 0x38)));                          // [esp+3Ch]
        const M32 half = lit(object_projectile_literal_0073e9ac);
        float offset[3];                                                                 // [esp+24h]
        x87::put(&offset[0], x87::st(x87::mul(-x87::ld(f(self, 0x30)), half)));
        x87::put(&offset[1], x87::st(x87::mul(-x87::ld(f(self, 0x34)), half)));
        x87::put(&offset[2], x87::st(x87::mul(x87::ld(negative_z), half)));
        (void)vector_add_assign_004a9740(reinterpret_cast<float*>(&p), nullptr, offset);  // 006D773F
    }
}
// 006D7760..006D77C0 / 006D79DF..006D7A35: event +0Ch = p; destination = p + event point
void place_event(void* self, Point p) {
    put_point(ptr(self, 0x198), 0xc, p);                                                 // 006D7760..006D777A
    void* const event = ptr(self, 0x198);                                                // 006D7781
    const M32 x = x87::st(x87::add(x87::ld(p.x), f(event, 0)));
    const M32 y = x87::st(x87::add(x87::ld(p.y), f(event, 4)));
    const M32 z = x87::st(x87::add(x87::ld(p.z), f(event, 8)));
    put_point(self, 0x114, {x, y, z});
}
// 006D7A93..006D7AB5: take the next path event, keeping the remaining time (006D70D0's result is not tested)
void next_event_keep_time(void* self) {
    const U remaining = rd<U>(self, 0x120);
    (void)object_projectile_006d70d0(self, nullptr);
    wr<U>(self, 0x120, rd<U>(self, 0x120) + remaining);
}

// ======================================================================================== 006D33C0 helpers
// 006D3B8F tail: acceleration = (1 / t^2) * (2 * (toward - t * velocity)); every vector op is the selected provider.
void accelerate(void* self, const Point& toward, M32 seconds) {
    float scaled[3];                                                                     // [esp+B8h]-class scratch, one per call
    const float* const travel = float_times_vector_00437a60(scaled, x87::as_float(seconds), static_cast<const float*>(at(self, 0x178)));
    float difference[3];
    const Vector to = vector_of(toward);                                                 // [esp+40h]
    const float* const rest = kotor_cdecl_00423bc0(difference, &to.x, travel);
    float doubled[3];
    const float* const twice = float_times_vector_00437a60(doubled, float_of(kTwo), rest);
    const M32 inverse = x87::st(divr(x87::mul(x87::ld(seconds), seconds), lit(object_projectile_literal_0073d764)));   // fld; fmul; fdivr [0073D764]
    float accel[3];
    const float* const a = float_times_vector_00437a60(accel, x87::as_float(inverse), twice);
    put_point(self, 0x184, point_at(a, 0));                                              // 006D3B94..006D3BAA
}

enum class Spread { hundredths, coin, hundredths_gob };

// 006D379C..006D3933 (mode 4), 006D3938..006D3AFF (mode 9), 006D3E17..006D3FAE (mode 12): a random launch.
// heading = acos(dir.x) of the normalized direction (2pi - heading for dir.y < 0), turned by +-(rand % 900) / 10
// degrees (the sign alternates through the shared toggle 00833BD8), lifted by a random z, at speed
// 3 + rand % (|ftol(5 ln|toward|)| + 1).
void random_launch(void* self, Point& dir, const Point& toward, M32 seconds, Spread spread) {
    Vector unit;                                                                         // [esp+1CCh]
    const Vector original = vector_of(dir);
    dir = point_at(normalize_004ab180(&unit, &original), 0);                             // [esp+28h..30h], DWORD copies
    crt_x87::Extended in, out;
    x87_load_m32(&dir.x, &in);
    crt_acos_x87_006fc0e0(&in, &out);                                                    // 006D37C9
    M32 heading = x87_store_m32(&out);                                                   // [esp+24h]
    if (x87::ld(dir.y) < x87::ld(lit(object_projectile_literal_0073d700)))               // 006D37D2..006D37E1 (jp: >= 0 or unordered)
        heading = x87::st(x87::sub(x87::ld(lit(object_projectile_literal_00740c88)), heading));   // 2pi - heading
    const std::int32_t turn = crt_rand_006fc447() % 0x384;                               // 006D37F1..006D37FC cdq; idiv
    const U side = object_projectile::toggle_00833bd8;                                   // 006D37FE
    const double degrees = x87::mul(x87::mul(static_cast<double>(turn), lit(object_projectile_literal_0073e798)),
                                    lit(object_projectile_literal_007415f8));            // fild; fmul [0073E798]; fmul [007415F8]
    const double angle = side != 0 ? x87::subr(degrees, heading) : x87::add(degrees, heading);   // fsubr / fadd [esp+24h]
    Point aim;                                                                           // [esp+34h..3Ch]
    aim.x = x87_cos_to_m32(angle);
    aim.y = x87_sin_to_m32(angle);
    double lift;
    if (spread == Spread::coin) {                                                        // 006D39CF..006D3A14
        if (crt_rand_006fc447() % 2 != 0)                                                // and eax,80000001h; jns; dec; or; inc
            lift = x87::mul(static_cast<double>(crt_rand_006fc447() % 0x64), lit(object_projectile_literal_0073dfec));
        else
            lift = x87::mul(static_cast<double>(crt_rand_006fc447() % 0x32), lit(object_projectile_literal_00757ca4));
    } else {
        lift = x87::mul(static_cast<double>(crt_rand_006fc447() % 0x64), lit(object_projectile_literal_0073dfec));   // 006D3833..006D385A
    }
    const U flip = object_projectile::toggle_00833bd8;                                   // 006D3840 (re-read)
    aim.z = x87::st(lift);                                                               // [esp+3Ch]
    object_projectile::toggle_00833bd8 = flip == 0 ? 1u : 0u;                            // xor edx,edx; test edi,edi; sete dl
    const Vector to = vector_of(toward);
    const double distance = vector_magnitude_004ab100(&to);                              // 006D386A
    const U scaled = x87::ftol2_low(x87_ln_times(distance, literal_double(object_projectile_literal_00753d98)));   // 006D387B _ftol2
    const U sign = static_cast<U>(static_cast<std::int32_t>(scaled) >> 31);             // cdq
    const U range = ((scaled ^ sign) - sign) + 1u;                                       // xor; sub; inc
    const std::int32_t pick = crt_rand_006fc447() % static_cast<std::int32_t>(range);   // 006D3888..006D388E idiv
    const M32 speed = x87::st(x87::add(static_cast<double>(pick), lit(object_projectile_literal_00740c84)));   // fild; fadd [00740C84]; fstp [esp]
    Vector direction;                                                                    // [esp+A0h]
    const Vector raw = vector_of(aim);
    const Vector* const n = normalize_004ab180(&direction, &raw);
    float velocity[3];                                                                   // [esp+1A8h]
    const float* const v = vector_component_mult_00423bf0(velocity, &n->x, x87::as_float(speed));
    put_point(self, 0x178, point_at(v, 0));                                              // 006D38C4..006D38DE
    accelerate(self, toward, seconds);
}

// 006D36A9..006D378C (mode 10): a lob. acceleration = 7.5 * unit(dir.x, dir.y, 0); velocity = (2 / t) * toward -
// acceleration; then acceleration x and y become (a^2 - v^2) / (2 toward) per axis.
void lob(void* self, Point dir, const Point& toward, M32 seconds) {
    dir.z = M32{0};                                                                      // 006D36BB
    Vector unit;                                                                         // [esp+7Ch]
    const Vector flat = vector_of(dir);
    const Vector* const n = normalize_004ab180(&unit, &flat);
    float pushed[3];                                                                     // [esp+4Ch]
    const float* const a = vector_component_mult_00423bf0(pushed, &n->x, float_of(kLobSpeed));
    const double rate = x87::div(x87::ld(lit(object_projectile_literal_00741be0)), seconds);   // fld [00741BE0]; fdiv [esp+20h]
    put_point(self, 0x184, point_at(a, 0));                                              // 006D36E0..006D36FF
    float scaled[3];                                                                     // [esp+100h]
    const Vector to = vector_of(toward);
    const float* const s = float_times_vector_00437a60(scaled, x87::as_float(x87::st(rate)), &to.x);
    float velocity[3];                                                                   // [esp+1B4h]
    const float* const v = kotor_cdecl_00423bc0(velocity, s, static_cast<const float*>(at(self, 0x184)));
    put_point(self, 0x178, point_at(v, 0));                                              // 006D3725..006D373A
    for (U axis = 0; axis < 8; axis += 4) {                                              // 006D3723..006D3754, 006D375A..006D3778
        const double ac = x87::ld(f(self, 0x184 + axis)), ve = x87::ld(f(self, 0x178 + axis));
        const double top = sub_rr(x87::mul(ac, ac), x87::mul(ve, ve));                    // fld st(1); fmul st(2) twice; fsubp
        const double bottom = x87::add(x87::ld(f(&to, axis)), x87::ld(f(&to, axis)));   // fld [esp+40h+axis]; fadd st,st
        x87::put_at(self, 0x184 + axis, x87::st(x87::div(top, bottom)));                // fdivp; fstp [edi] / [esi+188h]
    }
}

// 006D3BF9..006D3DA3 (mode 7): attach the projectile's Gob to the target's node, or to a "dummytarget" reference
// Gob placed at the path event when the target is not an object.
void attach_to_target(void* self) {
    void* const model = get_model(self);                                                 // 006D3C5C..006D3C6E
    if (model == nullptr) return;
    void* const array = kotor_fastcall_005ed570(client(), nullptr);                      // 006D3C78..006D3C80
    if (array == nullptr) return;
    void* object = nullptr;                                                              // [esp+58h]
    if (kotor_fastcall_004d8230(array, nullptr, rd<U>(self, 0x124), &object) == object_projectile_literal_0074666c[0]) {
        Point part = kZero;                                                              // [esp+10h]
        Quaternion q = identity();                                                       // [esp+6Ch]
        if (get_model(object) == nullptr) return;                                        // 006D3CEC..006D3CFB
        void* const gob = get_model(object);                                             // 006D3D0C
        if (part_position(gob, self, &part, &q) != 0) {                                  // 006D3D31..006D3D47
            const char* const name = WindowCStr005e5670(string_at(self, 0x128), nullptr);   // 006D3D4D
            void* const other = get_model(object);                                       // 006D3D5E
            reattach(model, other, name);                                                // 006D3D67
        } else {
            reattach(model, get_model(object), object_projectile_literal_00740c00);       // 006D3D7A..006D3D93 "root"
        }
        return;
    }
    aurora_model::Gob* const reference =
        aurora_new_aur_object_00449cc0(object_projectile_literal_00757c94, object_projectile_literal_00757c98, nullptr, nullptr);   // "ref", "dummytarget"
    wr(self, 0x19c, reference);                                                          // 006D3DBE
    if (reference == nullptr) return;
    Vector placed;                                                                       // [esp+C4h]
    (void)gob_table(reference)->s05c_gob_set_position(reference, nullptr, &placed, vector_of(point_at(ptr(self, 0x198), 0)));   // 006D3DCA..006D3DF1
    reattach(model, ptr(self, 0x19c), object_projectile_literal_00740c00);               // 006D3DF4..006D3E04 "root"
}

// 006D3FB3..006D40FF (mode 12, after the launch): a reference Gob at the own model's position, the model raised by
// 0.01, the reference oriented as the model, attached to the model's scene and reattached as "root".
void own_reference(void* self) {
    aurora_model::Gob* const reference =
        aurora_new_aur_object_00449cc0(object_projectile_literal_00757c94, base_ini_reconstruction::warning_empty_0073d71c, nullptr, nullptr);   // "ref", ""
    wr(self, 0x19c, reference);                                                          // 006D3FE1
    if (reference == nullptr) return;
    void* const model = get_model(self);                                                 // 006D3FED..006D3FFE
    Vector position;                                                                     // [esp+184h]
    const Point here = point_at(pointer_of(gob_table(model)->s064_gob_get_position(as_gob(model), nullptr, word_of(&position))), 0);   // [esp+10h..18h]
    Vector placed;                                                                       // [esp+19Ch]
    (void)gob_table(ptr(self, 0x19c))->s05c_gob_set_position(as_gob(ptr(self, 0x19c)), nullptr, &placed, vector_of(here));   // 006D4033..006D404A
    Point raised = here;
    raised.z = x87::st(x87::add(x87::ld(here.z), lit(object_projectile_literal_0073dfec)));   // fld [esp+18h]; fadd [0073DFEC]; fstp
    Vector moved;                                                                        // [esp+5Ch]
    (void)gob_table(model)->s05c_gob_set_position(as_gob(model), nullptr, &moved, vector_of(raised));   // 006D407D
    Quaternion orientation;                                                              // [esp+1F0h]
    U q[4];
    std::memcpy(q, pointer_of(gob_table(model)->s068_gob_get_orientation(as_gob(model), nullptr, word_of(&orientation))), sizeof q);   // 006D408C..006D40AF
    Quaternion turned;                                                                   // [esp+200h]
    (void)gob_table(ptr(self, 0x19c))->s060_gob_set_orientation(as_gob(ptr(self, 0x19c)), nullptr, word_of(&turned), q[0], q[1], q[2], q[3]);   // 006D40C0
    const aurora_model::GobVTable* const table = gob_table(ptr(self, 0x19c));          // 006D40C3..006D40CB (table read first)
    const U scene = gob_table(model)->s104_gob_get_property_0x68(as_gob(model), nullptr);   // 006D40CF
    (void)table->attach_to_scene(as_gob(ptr(self, 0x19c)), nullptr, static_cast<aurora_model::Scene*>(pointer_of(scene)));   // 006D40D5..006D40DC
    reattach(model, ptr(self, 0x19c), object_projectile_literal_00740c00);               // 006D40DF..006D40EF "root"
}

void stop(void* self) {   // velocity and acceleration zero (the original stores x, y, z of +178h, then of +184h)
    put_point(self, 0x178, kZero);
    put_point(self, 0x184, kZero);
}

// ======================================================================================== 006D2E00 / 006D30C0 tail
// 006D2F5C..006D30BE / 006D325A..006D33BC: the new position p = position + t * velocity is clamped to the destination
// when it would pass it; then the orientation (mode [+1A1h]: 1 toward the event's second point, 2 along the step,
// otherwise the initial difference) and the position are set. `order` is the function's own order of the squared
// sums (006D2E00: (z^2 + y^2) + x^2; 006D30C0: (z^2 + x^2) + y^2).
struct Squares { int a, b, c; };   // component indices of ((a^2 + b^2) + c^2)
double sum_squares(Point v, Squares o) {
    const M32 c[3] = {v.x, v.y, v.z};
    return x87::add(x87::add(x87::mul(x87::ld(c[o.a]), c[o.a]), x87::mul(x87::ld(c[o.b]), c[o.b])), x87::mul(x87::ld(c[o.c]), c[o.c]));
}
void step_and_face(void* self, Point difference, Squares order) {
    const double elapsed = seconds_of(self, 0x44);                                       // fild [esi+44h]; (+2^32); fmul
    const double px = x87::mul(elapsed, f(self, 0x178));                                 // fld st(0); fmul [eax] (kept wide)
    const M32 py = x87::st(x87::mul(elapsed, f(self, 0x17c)));                           // [esp+3Ch]
    const M32 pz = x87::st(x87::mul(elapsed, f(self, 0x180)));                           // [esp+40h]
    Point p = {x87::st(x87::add(px, f(self, 0x24))), x87::st(x87::add(x87::ld(py), f(self, 0x28))),
               x87::st(x87::add(x87::ld(pz), f(self, 0x2c)))};
    const double a = x87::sub(x87::ld(f(self, 0x24)), p.x);
    const double b = x87::sub(x87::ld(f(self, 0x28)), p.y);
    const double c = x87::sub(x87::ld(f(self, 0x2c)), p.z);
    const double moved = x87::add(x87::add(x87::mul(c, c), x87::mul(b, b)), x87::mul(a, a));   // ((c^2 + b^2) + a^2), registers
    const double whole = sum_squares(difference, order);
    if (whole < moved) p = point_at(self, 0x114);                                        // fcompp; test ah,5; jp (copy only when less)
    Point dir = difference;                                                              // [esp+8]
    const B mode = rd<B>(self, 0x1a1);
    if (mode == 1) {                                                                     // event second point - p
        void* const event = ptr(self, 0x198);
        dir = {x87::st(x87::sub(x87::ld(f(event, 0xc)), p.x)), x87::st(x87::sub(x87::ld(f(event, 0x10)), p.y)),
               x87::st(x87::sub(x87::ld(f(event, 0x14)), p.z))};
    } else if (mode == 2) {                                                              // p - position
        dir = {x87::st(x87::sub(x87::ld(p.x), f(self, 0x24))), x87::st(x87::sub(x87::ld(p.y), f(self, 0x28))),
               x87::st(x87::sub(x87::ld(p.z), f(self, 0x2c)))};
    }
    if (mode != 0 && sum_squares(dir, order) > x87::ld(lit(object_projectile_literal_00747504))) {   // fcomp; test ah,41h; jne
        Vector unit;                                                                     // [esp+3Ch] / [esp+44h]
        const Vector raw = vector_of(dir);
        const Vector* const n = normalize_004ab180(&unit, &raw);
        object_table(self)->s088_set_orientation(self, nullptr, &n->x);                  // slot +088
    }
    const Vector position = vector_of(p);                                                // [esp+20h] / [esp+14h]
    object_table(self)->s08c_set_position(self, nullptr, &position.x);                   // slot +08C
}
}  // namespace

extern "C" {

// 006D2E00..006D30BE: homing. velocity = (2 d - a T^2) / (2 T) for d = destination - position, T = [+120h] / 1000
// and a = [+184h] (x and y and z each in the original's own operand form), then step_and_face. RET 0.
void __fastcall object_projectile_006d2e00(void* self, void*) {
    const M32 dx = x87::st(x87::sub(x87::ld(f(self, 0x114)), f(self, 0x24)));             // [esp+14h] = [esp+8]
    const M32 dy = x87::st(x87::sub(x87::ld(f(self, 0x118)), f(self, 0x28)));             // [esp+18h] = [esp+0Ch]
    const M32 dz = x87::st(x87::sub(x87::ld(f(self, 0x11c)), f(self, 0x2c)));             // [esp+1Ch] = [esp+10h]
    const double t = seconds_of(self, 0x120);                                            // 006D2E49..006D2E5B
    const double tt = x87::mul(t, t);                                                    // fld st(0); fmul st(1)
    const double ax = x87::mul(tt, f(self, 0x184));                                      // fld st(0); fmul [esi+184h] (wide)
    const M32 ay = x87::st(x87::mul(tt, f(self, 0x188)));                                // [esp+24h]
    const M32 az = x87::st(x87::mul(tt, f(self, 0x18c)));                                // [esp+28h]
    const double twice_x = x87::add(x87::ld(dx), x87::ld(dx));                           // fadd st(0), st(0) (wide)
    const M32 twice_y = x87::st(x87::add(x87::ld(dy), x87::ld(dy)));                     // [esp+30h]
    const M32 twice_z = x87::st(x87::add(x87::ld(dz), x87::ld(dz)));                     // [esp+34h]
    const double vx = sub_rr(twice_x, ax);                                               // fsubrp st(1): ST1 = ST0 - ST1
    const M32 vy = x87::st(x87::sub(x87::ld(twice_y), ay));                              // [esp+3Ch]
    const M32 vz = x87::st(x87::sub(x87::ld(twice_z), az));                              // [esp+40h]
    const M32 inverse = x87::st(divr(x87::add(t, t), lit(object_projectile_literal_0073d764)));   // fadd st,st; fdivr [0073D764]; [esp+4]
    put_point(self, 0x178, {x87::st(x87::mul(vx, inverse)), x87::st(x87::mul(x87::ld(vy), inverse)),
                            x87::st(x87::mul(x87::ld(vz), inverse))});                  // 006D2ECF..006D2F03
    step_and_face(self, {dx, dy, dz}, Squares{2, 1, 0});                                 // S2 and the facing length: (z^2 + y^2) + x^2
}

// 006D30C0..006D33BC: curving. velocity = (destination - position) / T + 7.5 * unit(side), side = (-dy, dx, 0) when
// [+1A0h] is set, else (dy, -dx, 0); then step_and_face. RET 0.
void __fastcall object_projectile_006d30c0(void* self, void*) {
    const M32 dx = x87::st(x87::sub(x87::ld(f(self, 0x114)), f(self, 0x24)));             // [esp+20h] = [esp+8]
    const M32 dy = x87::st(x87::sub(x87::ld(f(self, 0x118)), f(self, 0x28)));             // [esp+24h] = [esp+0Ch]
    const M32 dz = x87::st(x87::sub(x87::ld(f(self, 0x11c)), f(self, 0x2c)));             // [esp+28h] = [esp+10h]
    const double t = seconds_of(self, 0x120);                                            // 006D3121..006D3139
    Point side;                                                                          // [esp+2Ch..34h]
    if (rd<B>(self, 0x1a0) != 0) {                                                        // 006D3133..006D3149
        side.x = x87::st(-x87::ld(dy));
        side.y = dx;
    } else {
        side.x = dy;
        side.y = x87::st(-x87::ld(dx));
    }
    side.z = M32{0};                                                                     // 006D3141
    const double inverse = divr(t, lit(object_projectile_literal_0073d764));             // fdivr [0073D764] (wide)
    const M32 ux = x87::st(x87::mul(x87::ld(dx), inverse));                              // [esp+38h] (fmul st(1))
    const M32 uy = x87::st(x87::mul(x87::ld(dy), inverse));                              // [esp+3Ch]
    const M32 uz = x87::st(x87::mul(x87::ld(dz), inverse));                              // [esp+40h]
    Vector unit;                                                                         // [esp+44h]
    const Vector raw = vector_of(side);
    const Vector* const n = normalize_004ab180(&unit, &raw);                             // 006D31A1
    const M32 k = lit(object_projectile_literal_00749d80);                              // 7.5f
    const double kx = x87::mul(x87::ld(f(n, 0)), k);
    const double ky = x87::mul(x87::ld(f(n, 4)), k);
    const M32 kz = x87::st(x87::mul(x87::ld(f(n, 8)), k));                               // [esp+4Ch]
    put_point(self, 0x178, {x87::st(x87::add(kx, ux)), x87::st(x87::add(ky, uy)), x87::st(x87::add(x87::ld(kz), uz))});   // 006D31CF..006D3201
    step_and_face(self, {dx, dy, dz}, Squares{2, 0, 1});                                 // S2 and the facing length: (z^2 + x^2) + y^2
}

// 006D33C0..006D4170: motion setup for the current path event. The aim point is the event point (target id
// 7F000000h), the target object's node [+128h] plus the event point (object type > 4), else (0, 0, 0). A zero
// distance stops the projectile; otherwise the event type [+2Dh] (1..13) selects the launch. `output` is read only
// (its three DWORDs are copied). RET 4.
void __fastcall object_projectile_006d33c0(void* self, void*, Vector* output) {
    Point toward = kZero;                                                                // [esp+40h]
    Point target = kZero;                                                                // [esp+4Ch]
    if (ptr(self, 0x198) == nullptr) return;                                             // 006D33CB..006D341C
    if (rd<U>(self, 0x124) == kInvalidId) {                                              // 006D3422
        target = point_at(ptr(self, 0x198), 0);                                          // 006D356C..006D357C
    } else {
        void* const array = kotor_fastcall_005ed570(client(), nullptr);                  // 006D3432..006D343A
        void* object = nullptr;                                                          // [esp+20h]
        if (kotor_fastcall_004d8230(array, nullptr, rd<U>(self, 0x124), &object) == object_projectile_literal_0074666c[0] &&
            rd<B>(object, 8) > object_projectile_literal_00746633[0]) {                 // 006D3452..006D346B (jbe: unsigned)
            Point part = kZero;                                                          // [esp+10h]
            Quaternion q = identity();                                                   // [esp+5Ch]
            if (get_model(object) == nullptr) {                                          // 006D34AB..006D34BC
                target = point_at(self, 0x114);                                          // 006D3550..006D3566
            } else {
                void* const gob = get_model(object);                                     // 006D34CD
                if (part_position(gob, self, &part, &q) == 0) {                          // 006D34E1..006D34F7
                    part = point_at(object, 0x24);                                       // 006D34F9..006D350C
                    part.z = x87::st(x87::add(x87::ld(part.z), lit(object_projectile_literal_00757ca8)));   // + 1.25f
                }
                Vector sum;                                                              // [esp+7Ch]
                const Vector node = vector_of(part);
                target = point_at(vector_sum_00423b90(&sum, &node, static_cast<const Vector*>(ptr(self, 0x198))), 0);   // 006D351E..006D354A
            }
        }
    }
    toward = {x87::st(x87::sub(x87::ld(target.x), f(self, 0x24))), x87::st(x87::sub(x87::ld(target.y), f(self, 0x28))),
              x87::st(x87::sub(x87::ld(target.z), f(self, 0x2c)))};                     // 006D3580..006D35AD
    const double length = sum_squares(toward, Squares{2, 1, 0});                         // (z^2 + y^2) + x^2
    if (length <= x87::ld(lit(object_projectile_literal_00747504))) {                    // test ah,41h; jp: > or unordered goes on
        stop(self);                                                                      // 006D35E2..006D3643
        return;
    }
    const M32 seconds = x87::st(seconds_of(self, 0x120));                                // [esp+20h]
    Point dir = point_at(output, 0);                                                     // [esp+28h..30h], DWORD copies
    switch (static_cast<U>(rd<B>(ptr(self, 0x198), 0x2d)) - 1u) {                        // 006D3694..006D36A2, table 006D4174
    case 0:    // type 1
    case 5:    // type 6
    case 12:   // type 13
        put_point(self, 0x184, kZero);                                                   // 006D3BBA..006D3BE9
        return;
    case 1:    // type 2: gravity
        put_point(self, 0x184, {M32{0}, M32{0}, M32{kGravity}});                         // 006D378F..006D3BE9
        return;
    case 3:    // type 4
        random_launch(self, dir, toward, seconds, Spread::hundredths);
        return;
    case 4:    // type 5: from rest
        put_point(self, 0x178, kZero);                                                   // 006D3B04..006D3B3A
        accelerate(self, toward, seconds);
        return;
    case 6:    // type 7
        stop(self);                                                                      // 006D3BF9..006D3C5E
        attach_to_target(self);
        return;
    case 8:    // type 9
        random_launch(self, dir, toward, seconds, Spread::coin);
        return;
    case 9:    // type 10
        lob(self, dir, toward, seconds);
        return;
    case 11:   // type 12
        random_launch(self, dir, toward, seconds, Spread::hundredths_gob);
        own_reference(self);
        return;
    default:   // types 3, 8, 11 and every value outside 1..13 (ja 006D4102)
        stop(self);                                                                      // 006D4102..006D4163
        return;
    }
}

// 006D7590..006D7B93: one frame of path motion. Re-aims a tracked target, advances the path event when it is used up
// (006D70D0), stops at the destination or within the event radius, otherwise records the previous position and
// runs the velocity update the event type selects (table 006D7B94 by index bytes 006D7BA4). RET 0.
void __fastcall object_projectile_006d7590(void* self, void*) {
    Point p = kZero;                                                                     // [esp+18h] (offsets after the four pushes)
    if (ptr(self, 0x198) == nullptr) {                                                   // 006D7596..006D75B6
        wr<U>(self, 0x164, 1);                                                           // 006D75B8
        return;
    }
    bool advanced = false;                                                               // EDI
    Quaternion q;                                                                        // [esp+3Ch]
    if (rd<U>(ptr(self, 0x198), 0x18) == menu_tables_m41::cells::invalid_object_00746644) {   // 006D75C7..006D75DB
        put_point(self, 0x114, point_at(ptr(self, 0x198), 0));                           // 006D75DD..006D75F2
    } else {
        q = identity();                                                                  // 006D7600..006D7618
        void* const array = kotor_fastcall_005ed570(client(), nullptr);                  // 006D75FA..006D7623
        void* object = nullptr;                                                          // [esp+10h]
        if (kotor_fastcall_004d8230(array, nullptr, rd<U>(ptr(self, 0x198), 0x18), &object) == object_projectile_literal_0074666c[0]) {
            target_point(self, object, p, q);
            place_event(self, p);
        } else {
            put_point(self, 0x114, point_at(ptr(self, 0x198), 0));                       // 006D77C5..006D77E0
        }
    }
    const U elapsed = rd<U>(self, 0x44), duration = rd<U>(self, 0x120);                 // 006D77E3..006D77EE
    if (elapsed >= duration) {                                                           // jb (unsigned)
        wr<U>(self, 0x44, elapsed - duration);
        wr<U>(self, 0x120, 0);
        object_table(self)->s08c_set_position(self, nullptr, static_cast<const float*>(at(self, 0x114)));   // 006D7804
        advanced = true;
    }
    const U left = rd<U>(self, 0x120) - rd<U>(self, 0x44);                               // 006D780F..006D7818
    if (left <= rd<U>(ptr(self, 0x198), 0x24) || advanced) {                             // jbe / test edi,edi
        if (object_projectile_006d70d0(self, nullptr) == 0) return;                      // 006D7829..006D7832
    }
    wr<U>(self, 0x120, rd<U>(self, 0x120) - rd<U>(self, 0x44));                          // 006D7838..006D784B
    if (ptr(self, 0x198) == nullptr) return;                                             // 006D7849..006D7851
    if (rd<U>(ptr(self, 0x198), 0x18) == menu_tables_m41::cells::invalid_object_00746644) {   // 006D7857..006D7860
        put_point(self, 0x114, point_at(ptr(self, 0x198), 0));                           // 006D7862..006D7871
    } else {
        q = identity();                                                                  // 006D787F..006D7897
        void* const array = kotor_fastcall_005ed570(client(), nullptr);
        void* object = nullptr;                                                          // [esp+14h]
        if (kotor_fastcall_004d8230(array, nullptr, rd<U>(ptr(self, 0x198), 0x18), &object) == object_projectile_literal_0074666c[0]) {
            target_point(self, object, p, q);              // p keeps its value from above
            place_event(self, p);
        }                                                                                // not found: the destination is kept
    }
    // 006D7A38..006D7A6C: at the destination (|destination - position|^2 <= eps)?
    const double dx = x87::sub(x87::ld(f(self, 0x114)), f(self, 0x24));
    const double dy = x87::sub(x87::ld(f(self, 0x118)), f(self, 0x28));
    const double dz = x87::sub(x87::ld(f(self, 0x11c)), f(self, 0x2c));
    const double rest = x87::add(x87::add(x87::mul(dz, dz), x87::mul(dy, dy)), x87::mul(dx, dx));
    if (rest <= x87::ld(lit(object_projectile_literal_00747504))) {                      // jp: > or unordered goes to 006D7AB6
        object_table(self)->s08c_set_position(self, nullptr, static_cast<const float*>(at(self, 0x114)));   // 006D7A73
        if (!(x87::ld(f(ptr(self, 0x198), 0x28)) >= x87::ld(lit(object_projectile_literal_0073d700)))) return;   // test ah,1; jne (< or unordered)
        next_event_keep_time(self);
        return;
    }
    void* const event = ptr(self, 0x198);                                                // EBX (006D7AB6)
    if (x87::ld(f(event, 0x28)) >= x87::ld(lit(object_projectile_literal_0073d700)) &&   // 006D7ABC..006D7ACA
        rd<U>(event, 0x18) != menu_tables_m41::cells::invalid_object_00746644) {         // 006D7ACC..006D7AD5
        float away[3];                                                                   // [esp+30h]
        const float* const d = kotor_cdecl_00423bc0(away, static_cast<const float*>(at(self, 0x24)), reinterpret_cast<const float*>(&p));   // position - p
        const double radius = x87::ld(f(event, 0x28));
        const Point gap = point_at(d, 0);                                                // [esp+24h..2Ch]
        const double distance = sum_squares(gap, Squares{2, 1, 0});                      // (z^2 + y^2) + x^2
        if (x87::mul(radius, radius) >= distance) {                                      // fcompp; test ah,1; je
            next_event_keep_time(self);
            return;
        }
    }
    put_point(self, 0x100, point_at(self, 0x24));                                        // 006D7B30..006D7B43 previous position
    const U index = static_cast<U>(rd<B>(event, 0x2d)) - 1u;                             // movzx; dec
    if (index > 12u) return;                                                             // ja 006D7B8C
    static constexpr B kCase[13] = {0, 0, 3, 0, 0, 1, 3, 3, 0, 0, 3, 0, 2};              // index bytes 006D7BA4
    switch (kCase[index]) {                                                              // targets 006D7B94
    case 0:                                                                              // 006D7B85
        object_projectile_006d2e00(self, nullptr);
        return;
    case 1:                                                                              // 006D7B76
        object_projectile_006d30c0(self, nullptr);
        return;
    case 2:                                                                              // 006D7B5E
        object_projectile_006d2e00(self, nullptr);
        (void)object_projectile_006d4250(self, nullptr, 4u);                             // PlayAnimation(4)
        return;
    default:                                                                             // 006D7B8C
        return;
    }
}

}  // extern "C"
