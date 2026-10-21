// t12: the four CSWCProjectile update builders.
//   006D64F0 path builder (SetMode 8)         object_projectile_006d64f0  void  fastcall @8   RET 0    (FuncInfo 00786228)
//   006D5B10 path builder (SetMode 11 / 12)   object_projectile_006d5b10  void  fastcall @12  RET 4    (FuncInfo 00786228)
//   006D6A90 path builder (SetMode 13)        object_projectile_006d6a90  void  fastcall @8   RET 0    (FuncInfo 00786228)
//   006D70D0 next path event (motion 006D7590) object_projectile_006d70d0  U     fastcall @8   RET 0    (FuncInfo 0078626C)
// Ownership: reassigned by user from the sol projectile lanes, 2026-10-15, PENDING-REGISTRATION.
// Written from my own static disassembly of swkotor.exe. The paused sol-projectile-updates t1 drafts were read and
// checked line by line; their three naked whole-body cores and their inline-asm bridges are not used (see
// INTEGRATION.md for the corrections). Declarations of the four exports: shared/object_projectile_internal.h
// (unchanged pinned copy). Every callee is a selected provider, another projectile export under its exact name, or
// UNRESOLVED_BY_DESIGN (006D33C0 motion).
//
// Path events. Each builder appends 34h-byte events (operator new, only the first 18h bytes cleared) to the list at
// [self+194h]; 006D70D0 takes them one by one. Layout as read and written by the five functions:
//   +00 Vector point   +0C Vector second point   +18 target id   +1C SetMode argument   +20 duration
//   +24 0   +28 float -1.0   +2C BYTE spawn count   +2D BYTE type (2 = path point, 0Bh = jump)   +30 sound row
//   +2E/+2F are never written; +30 is written only by the 006D5B10 four-point path, and 006D70D0 reads it for
//   every type-2 event (an uncleared heap word otherwise; reproduced).
//
// x87: see x87_arith.h. Arithmetic is C++ (double for register values, M32 for DWORD memory operands, with the
// x87 NaN selection). Only fsin, fcos and fpatan, which C++ cannot express, are minimal x87 helpers below.
#include "4bb421d64d-object_projectile_internal.h"
#include "5f8cab8099-object_projectile_providers.h"
#include "5f608976b4-object_projectile_literals.h"
#include "8fe1a9cf4a-m15e.h"
#include "afcc5e5f4a-m15e_animbase.h"
#include "f58c269829-scene3d.h"
#include "9963eab06a-vtables_gen.h"
#include "643f5e824d-startup_string.h"
#include "a8c83630a7-resources.h"
#include "c03e24938a-allocator_live.h"
#include "02addda1c6-x87_arith.h"
#include <cstdint>
#include <cstring>
#include <type_traits>

using U = std::uint32_t;
using B = std::uint8_t;
using S = kotor_reconstruction::StartupString;
using kotor_scene3d::Vector;
using x87::M32;

namespace menu_tables_m41 { namespace cells { extern const unsigned int invalid_object_00746644; } }
namespace base_ini_reconstruction { extern const char warning_empty_0073d71c[]; }
namespace menu_tables_required { void __fastcall AnimateExternal_0043e150(void* object, void* unused_edx, float weight); }
extern "C" {
extern void* kotor_cell_rules_007a3a28;
extern void* kotor_cell_sound_007a39ec;
void* __fastcall kotor_fastcall_005ed530(void* client, void*);
std::uint16_t __fastcall kotor_m15a_object_get_current_animation_0063c060(void* self, void*);
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* self, void*, std::int32_t id);
std::int32_t __fastcall c2da_current_get_string_00413270(void* table, void*, std::int32_t row, S* column, S* out);
menu_resources::Ref* __fastcall async_ref_construct_string_00406d60(menu_resources::Ref* self, void*, S* name);
U __fastcall kotor_m15_sound_play_3d_one_shot_005d5e10(void* sound, void*, void* ref, Vector position, float a5, U a6, U a7, U a8,
                                                       float a9, float a10);
U __fastcall recovered_client_area_apply_fire_and_forget_visual_effect_00604c90(void* area, void*, U effect, float x, float y, float z);
}

// The exports are the shared-header declarations (the t9 SetMode 006D6EE0 imports 64F0 / 5B10 / 6A90 by these names).
static_assert(std::is_same_v<decltype(&object_projectile_006d64f0), void (__fastcall*)(void*, void*)>, "64F0");
static_assert(std::is_same_v<decltype(&object_projectile_006d5b10), void (__fastcall*)(void*, void*, U)>, "5B10");
static_assert(std::is_same_v<decltype(&object_projectile_006d6a90), void (__fastcall*)(void*, void*)>, "6A90");
static_assert(std::is_same_v<decltype(&object_projectile_006d70d0), U (__fastcall*)(void*, void*)>, "70D0");

namespace {
// Original immediates (instruction bytes, not image data).
constexpr U kPlus1000 = 0x447a0000u;    // 1000.0f
constexpr U kMinus1000 = 0xc47a0000u;   // -1000.0f
constexpr U kMinusOne = 0xbf800000u;    // -1.0f
constexpr U kInvalidId = 0x7f000000u;   // the OBJECT_INVALID immediate of 006D61EB / 006D62E4 / 006D647E / 006D6E65
constexpr U kOneFloat = 0x3f800000u;    // 1.0f pushed at 006D7217 / 006D722A / 006D72B9 / 006D72DF / 006D73F7
constexpr M32 kQuarter{0x3e800000u};    // 0.25f, 006D65B6

template<class T> T rd(const void* p, U n) { T v; std::memcpy(&v, static_cast<const B*>(p) + n, sizeof v); return v; }
template<class T> void wr(void* p, U n, T v) { std::memcpy(static_cast<B*>(p) + n, &v, sizeof v); }
void* at(void* p, U n) { return static_cast<B*>(p) + n; }
void* ptr(const void* p, U n) { return rd<void*>(p, n); }
void* client() { return ptr(kotor_menu_window_cells::manager_007a39fc, 4); }   // [[007A39FC]+4]
M32 lit(const unsigned char* bytes) { return x87::m32(bytes); }                 // t9 literal arrays (image bytes)
M32 f(const void* p, U n) { return x87::m32_at(p, n); }                         // DWORD float field
const KotorM15ObjectMethods_007516c8* object_table(const void* o) { return rd<const KotorM15ObjectMethods_007516c8*>(o, 0); }
const KotorM15eAnimBaseVtbl* model_table(const void* m) { return rd<const KotorM15eAnimBaseVtbl*>(m, 0); }
const kotor_scene3d::SceneVTable* scene_table(const void* s) { return rd<const kotor_scene3d::SceneVTable*>(s, 0); }

// Three DWORDs as the original moves them (integer copies; never through the FPU).
struct Point { M32 x, y, z; };
Point point_at(const void* p, U n) { return {f(p, n), f(p, n + 4), f(p, n + 8)}; }
void put_point(void* p, U n, Point v) { x87::put_at(p, n, v.x); x87::put_at(p, n + 4, v.y); x87::put_at(p, n + 8, v.z); }
Vector vector_of(Point v) { Vector r; std::memcpy(&r.x, &v.x.bits, 4); std::memcpy(&r.y, &v.y.bits, 4); std::memcpy(&r.z, &v.z.bits, 4); return r; }

// ---- minimal x87 helpers (C++ has no fsin / fcos / fpatan)
// 006D6C29..006D6C31: fld1; fpatan (arctan(q / 1), 64-bit significand: fpatan ignores precision control); then
// faddp onto the wide ST1 = rand * 0.001 (rounded to 53 bits by precision control). Returns that 53-bit sum.
__declspec(noinline) double x87_atan_plus(double ratio, double addend) {
    double result;
    __asm {
        fld addend
        fld ratio
        fld1
        fpatan
        faddp st(1), st
        fstp result
    }
    return result;
}
// 006D6C4E..006D6C52: fld st(0); fcos; fstp dword
// (Scalar U operands: clang's MS-asm parser treats a struct-typed operand as an input only.)
__declspec(noinline) M32 x87_cos_to_m32(double angle) {
    U result;
    __asm {
        fld angle
        fcos
        fstp dword ptr [result]
    }
    return M32{result};
}
// 006D6C56 fsin, then 006D6C69 fchs / 006D6C8D fstp dword: the negative-x branch stores the 64-bit sine as a float.
__declspec(noinline) M32 x87_sin_to_m32(double angle) {
    U result;
    __asm {
        fld angle
        fsin
        fstp dword ptr [result]
    }
    return M32{result};
}
// 006D6C56 fsin, then 006D6CA9..006D6CB1 (fxch; fmul dword [00740C84]; fstp dword): the other branch multiplies the
// 64-bit sine (rounded to 53 bits by precision control) before the float store.
__declspec(noinline) M32 x87_sin_times_to_m32(double angle, M32 factor) {
    const U f = factor.bits;
    U result;
    __asm {
        fld angle
        fsin
        fmul dword ptr [f]
        fstp dword ptr [result]
    }
    return M32{result};
}

// ---- CHitInfo local (unwind state 0 of FuncInfo 00786228: ~CHitInfo 00456B30, inlined at each normal exit)
struct HitInfo {
    alignas(4) B bytes[0x64];
    HitInfo() { (void)aurora_hit_info_construct_004578f0(bytes, nullptr, 0u, 0xbfffffffu, 0u); }   // push 0 / BFFFFFFFh / 0
    ~HitInfo() noexcept(false) { chit_info_destruct_00456b30(bytes, nullptr); }
    HitInfo(const HitInfo&) = delete;
    HitInfo& operator=(const HitInfo&) = delete;
};


// [[[[007A39FC]+4]] module (005ED530)]+48h area]+184h scene; the original dereferences module and area unchecked.
void* scene_of() {
    void* const module = kotor_fastcall_005ed530(client(), nullptr);
    void* const area = ptr(module, 0x48);
    return ptr(area, 0x184);
}

// operator new(34h) (006FA7E6); the first six DWORDs are cleared. A null block is used unchecked by the original
// The caller performs the original first point store, including native failure on null.
void* new_event() {
    void* const event = live_operator_new(0x34u);
    if (event != nullptr) for (U i = 0; i < 0x18; i += 4) wr<U>(event, i, 0);
    return event;
}
void append(void* self, void* event) {
    resource_append_005e9dc0(static_cast<kotor_list::Internal*>(ptr(self, 0x194)), nullptr, event);   // AddTail 005E9DC0
}

// Scene::HitCheck (scene slot +38h) of a vertical segment from z = +1000 to z = -1000 through (x, y).
std::int32_t vertical_hit(void* scene, M32 x, M32 y, Vector* out, HitInfo& hit) {
    return scene_table(scene)->s038_scene_hit_check(scene, nullptr, vector_of({x, y, M32{kPlus1000}}),
                                                    vector_of({x, y, M32{kMinus1000}}), out, hit.bytes);
}

// 006D5B95..006D5C29 / 006D6AE3..006D6B5B: the destination. An invalid target id uses [self+114h]; a target found
// by 005ED580 gives (0, 0, 0); a missing target falls back to [self+114h].
Point destination(void* self) {
    const U id = rd<U>(self, 0x124);
    if (id == menu_tables_m41::cells::invalid_object_00746644) return point_at(self, 0x114);
    if (kotor_fastcall_005ed580(client(), nullptr, id) != nullptr) return {M32{0}, M32{0}, M32{0}};
    return point_at(self, 0x114);
}

// float((a - b) per component), stored x, y, z after computing z, y, x (the operands are DWORD fields).
Point difference(Point a, Point b) {
    const double z = x87::sub(x87::ld(a.z), b.z), y = x87::sub(x87::ld(a.y), b.y), x = x87::sub(x87::ld(a.x), b.x);
    return {x87::st(x), x87::st(y), x87::st(z)};
}
// 004AB100 |v| (ST0, 53 bits).
double magnitude(Point v) {
    const Vector vector = vector_of(v);
    return vector_magnitude_004ab100(&vector);
}

// 006D5B10 four-point path event: point = second point = the ground-snapped probe, +30h = the probe's CHitInfo+34h.
void four_point_event(void* self, Point probe, U time, U id, U row) {
    void* const event = new_event();
    put_point(event, 0, probe);
    put_point(event, 0xc, probe);
    wr<U>(event, 0x20, time);
    wr<B>(event, 0x2d, 2);
    wr<B>(event, 0x2c, 0);
    wr<U>(event, 0x24, 0);
    wr<U>(event, 0x18, id);
    wr<U>(event, 0x1c, 0);
    wr<U>(event, 0x28, kMinusOne);
    wr<U>(event, 0x30, row);
    append(self, event);
}

// ---- 006D70D0 helpers
// 006D7136..006D71F7: an event of type 2 with a sound row ([event+30h] != -1) plays the row's sound
// (2DA [[[007A3A28]+B8h]+100h], column "sound") as a 3D one-shot at the projectile position.
void play_event_sound(void* self) {
    void* const event = ptr(self, 0x198);
    if (rd<B>(event, 0x2d) != 2 || rd<U>(event, 0x30) == 0xffffffffu) return;              // 006D7136..006D7148
    void* const table = ptr(ptr(kotor_cell_rules_007a3a28, 0xb8), 0x100);                 // 006D714E..006D7159
    S sound;                                                                               // 006D715F 005B3190 (state 0)
    std::int32_t found;
    {
        S column(object_projectile_literal_00757e10);                                     // 006D7168..006D7175 "sound" (state 1)
        found = c2da_current_get_string_00413270(table, nullptr, rd<std::int32_t>(ptr(self, 0x198), 0x30), &column, &sound);   // 006D717A..006D7195
    }                                                                                      // 006D719A..006D71A5 (state 0)
    if (found != 0) {                                                                      // 006D71AA
        menu_resources::Ref ref;                                                           // [esp+48h]
        (void)async_ref_construct_string_00406d60(&ref, nullptr, &sound);                  // 006D71AE..006D71B7
        (void)kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec, nullptr, &ref, vector_of(point_at(self, 0x24)),
                                                        0.0f, 0xfu, 0u, 0xffu, 0.0f, 0.0f);   // 006D71BC..006D71EA
    }
}                                                                                          // 006D71EF..006D71F7 ~sound (state -1)

// SetAnimation (anim-base slot +54h)(animation, 1.0f, 0, 0); a non-zero result stores the WORD at [object+48h].
void set_animation(void* object, U animation) {
    void* const model = ptr(object, 0x68);
    if (model == nullptr) return;
    if (model_table(model)->s054(model, nullptr, animation, kOneFloat, 0u, 0u) != 0)
        wr<std::uint16_t>(object, 0x48, static_cast<std::uint16_t>(animation));
}
// GetModel(FFh, 1) (object slot +098h), then CAurObject::AnimateExternal 0043E150(model, 1.0f) (the 1.0f is pushed
// before the slot call and consumed by 0043E150).
void animate_external(void* self) {
    void* const model = object_table(self)->s098_get_model(self, nullptr, 0xffu, 1u);
    menu_tables_required::AnimateExternal_0043e150(model, nullptr, 1.0f);
}

// operator new(1C8h) + CSWCProjectile constructor 006D57B0 (unwind state 2 frees the block); a null block skips the
// constructor and the null projectile is used unchecked.
void* new_projectile() {
    void* const memory = live_operator_new(0x1c8u);                                        // 006D7332..006D7337
    if (memory == nullptr) return nullptr;                                                 // 006D734D
    try { return object_projectile_006d57b0(memory, nullptr); }                            // 006D7351 (state 2)
    catch (...) { live_free(memory); throw; }
}
}  // namespace

extern "C" {

// 006D64F0..006D6A8F: two downward sweeps of vertical hit checks between the projectile and its target, then 1..3
// path events. RET 0.
void __fastcall object_projectile_006d64f0(void* self, void*) {
    HitInfo hit;                                                                           // [esp+98h] (state 0)
    Vector out;                                                                            // [esp+64h]
    std::memset(&out, 0, sizeof out);                                                      // 006D6531..006D6539
    Point points[3];                                                                       // [esp+34h] / [esp+40h] / [esp+4Ch]
    std::memset(points, 0, sizeof points);                                                 // 006D653D..006D6552
    const M32 dz = x87::st(x87::sub(x87::ld(f(self, 0x11c)), f(self, 0x2c)));             // 006D6554..006D6576 [esp+2Ch]
    wr<U>(hit.bytes, 0, 1);                                                                // 006D6568
    const M32 dy = x87::st(x87::sub(x87::ld(f(self, 0x118)), f(self, 0x28)));             // 006D657A..006D6580 [esp+14h]
    const M32 dx = x87::st(x87::sub(x87::ld(f(self, 0x114)), f(self, 0x24)));             // 006D6584..006D6589 [esp+70h]
    void* const scene = scene_of();  // 006D658D..006D6595
    points[2] = point_at(self, 0x114);                                                     // 006D659B..006D65B2
    B count = 3;                                                                           // [esp+1Bh]
    M32 t = kQuarter;                                                                      // [esp+10h]
    U times[3] = {0, 0, 0};                                                                // [esp+20h]
    // Sweep A (006D65C0..006D66D6): t = 0.25 - 0.05 k; probe at target - delta * t until the ground there is below
    // the target by less than 1.0.
    for (;;) {
        t = x87::st(x87::sub(x87::ld(t), lit(object_projectile_literal_007415bc)));        // 006D65C0..006D65CA
        const double pz = x87::mul(x87::ld(dz), t);                                        // 006D65CE..006D65D2 (kept wide)
        const M32 py = x87::st(x87::mul(x87::ld(dy), t));                                 // [esp+1Ch]
        const M32 px = x87::st(x87::mul(x87::ld(dx), t));                                 // [esp+8Ch]
        const double z = x87::subr(pz, f(self, 0x11c));                                   // 006D65F1 fsubr [esi+8]
        const double y = x87::sub(x87::ld(f(self, 0x118)), py);
        const double x = x87::sub(x87::ld(f(self, 0x114)), px);
        points[1] = {x87::st(x), x87::st(y), x87::st(z)};                                  // [esp+40h..48h]
        if (vertical_hit(scene, points[1].x, points[1].y, &out, hit) != 0)                 // 006D6686
            points[1].z = x87::m32(&out.z);                                                // 006D6691..006D6695
        else                                                                               // 006D669B..006D66AA midpoint height
            points[1].z = x87::st(x87::mul(x87::add(x87::ld(f(self, 0x11c)), f(self, 0x2c)), lit(object_projectile_literal_0073e9ac)));
        if (!(x87::ld(t) > x87::ld(lit(object_projectile_literal_00747504)))) goto direct;  // 006D66AE..006D66BD (<= or unordered)
        if (!(x87::sub(x87::ld(points[2].z), points[1].z) >= x87::ld(lit(object_projectile_literal_0073d764)))) break;   // 006D66C3..006D66D6
    }
    if (!(x87::ld(t) > x87::ld(lit(object_projectile_literal_00747504)))) goto direct;      // 006D66DC..006D66EB (same test again)
    {
        // Sweep B (006D66F1..006D6812): s = 0.55 - 0.05 k (ST0 carries s between passes); probe at target - delta * s
        // while s > t and the ground at the first probe is at least 0.5 above the ground here.
        double s = x87::ld(lit(object_projectile_literal_00747b84));                      // 006D66F1
        M32 s32;
        for (;;) {
            s32 = x87::st(x87::sub(s, lit(object_projectile_literal_007415bc)));          // 006D6700..006D6706 [esp+1Ch]
            const double pz = x87::mul(x87::ld(dz), s32);
            const M32 py = x87::st(x87::mul(x87::ld(dy), s32));                           // [esp+88h]
            const M32 px = x87::st(x87::mul(x87::ld(dx), s32));                           // [esp+8Ch]
            const double z = x87::subr(pz, f(self, 0x11c));
            const double y = x87::sub(x87::ld(f(self, 0x118)), py);
            const double x = x87::sub(x87::ld(f(self, 0x114)), px);
            points[0] = {x87::st(x), x87::st(y), x87::st(z)};                              // [esp+34h..3Ch]
            if (vertical_hit(scene, points[0].x, points[0].y, &out, hit) != 0)             // 006D67C8
                points[0].z = x87::m32(&out.z);
            else
                points[0].z = x87::st(x87::mul(x87::add(x87::ld(f(self, 0x11c)), f(self, 0x2c)), lit(object_projectile_literal_0073e9ac)));
            s = x87::ld(s32);                                                              // 006D67F0 fld [esp+1Ch]
            if (!(s > x87::ld(t))) break;                                                  // 006D67F4..006D67FD fcom (s stays in ST0)
            if (!(x87::sub(x87::ld(points[1].z), points[0].z) >= x87::ld(lit(object_projectile_literal_0073e9ac)))) break;   // 006D67FF..006D6812
        }
        // 006D6818..006D6898: squared lengths of (P0 - P1) and (P1 - P2); the shorter second leg compares t with
        // itself, otherwise s with t; jp takes the three-point path when the compared value is above t or unordered.
        const double a1 = x87::sub(x87::ld(points[0].z), points[1].z);
        const double a2 = x87::sub(x87::ld(points[0].y), points[1].y);
        const double a3 = x87::sub(x87::ld(points[0].x), points[1].x);
        const double b1 = x87::sub(x87::ld(points[1].z), points[2].z);
        const M32 b2 = x87::st(x87::sub(x87::ld(points[1].y), points[2].y));               // [esp+14h]
        const M32 b3 = x87::st(x87::sub(x87::ld(points[1].x), points[2].x));               // [esp+70h]
        const double first = x87::add(x87::add(x87::mul(a3, a3), x87::mul(a2, a2)), x87::mul(a1, a1));
        const double second = x87::add(x87::add(x87::mul(x87::ld(b3), b3), x87::mul(x87::ld(b2), b2)), x87::mul(b1, b1));
        const double compared = second > first ? x87::ld(t) : s;                           // 006D6878..006D688B
        const double tt = x87::ld(t);
        const U d = rd<U>(self, 0x190);
        if (compared > tt || x87::is_nan(compared) || x87::is_nan(tt)) {                   // 006D688F..006D6898 jp -> 006D6953
            const U d2 = static_cast<U>(2u * d) / 10u, d3 = static_cast<U>(3u * d) / 10u;  // lea; mul 0CCCCCCCDh; shr 3
            times[0] = d - d2 - d3; times[1] = d3; times[2] = d2;
            count = 3;                                                                     // cl = [esp+1Bh]
        } else {                                                                           // 006D689E: two points
            points[0] = points[1];
            points[1] = point_at(self, 0x114);
            const U d3 = static_cast<U>(3u * d) / 10u;
            times[0] = d - d3; times[1] = d3;
            count = 2;
        }
        goto events;
    }
direct:                                                                                    // 006D6929: one point, the target
    points[0] = point_at(self, 0x114);
    times[0] = rd<U>(self, 0x190); times[1] = 0;
    count = 1;
events:                                                                                    // 006D68ED..006D69FF
    for (U i = 0; i < count; ++i) {
        void* const event = new_event();                         // 006D6908..006D698C
        put_point(event, 0, points[i]);                                                    // 006D698C..006D699D
        wr<B>(event, 0x2d, 2);
        put_point(event, 0xc, point_at(self, 0x114));                                      // 006D69A4..006D69BA
        wr<B>(event, 0x2c, 0);
        wr<U>(event, 0x20, times[i]);
        wr<U>(event, 0x24, 0);
        wr<U>(event, 0x18, menu_tables_m41::cells::invalid_object_00746644);              // 006D69C9 [00746644]
        wr<U>(event, 0x1c, 0);
        wr<U>(event, 0x28, kMinusOne);
        append(self, event);                                                               // 006D69DC..006D69E7
    }
}                                                                                          // 006D6A05..006D6A77 ~CHitInfo

// 006D5B10..006D64E6: hit checks at 0.8 / 0.9 / 0.95 / 1.0 of the way to the destination; four ground-snapped path
// events when all four hit, else one event at the destination. RET 4; the argument is never read.
void __fastcall object_projectile_006d5b10(void* self, void*, U) {
    void* const scene = scene_of();  // 006D5B2B..006D5B41 (before CHitInfo)
    U rows[4] = {0xffffffffu, 0xffffffffu, 0xffffffffu, 0};                                // [esp+B4h] [esp+A8h] [esp+B0h] [esp+ACh]
    Vector out;                                                                            // [esp+84h]
    std::memset(&out, 0, sizeof out);                                                      // 006D5B6F..006D5B85
    HitInfo hit;                                                                           // [esp+B8h] (state 0)
    bool all_hit = true;                                                                   // [esp+48h] = 1
    const Point to = destination(self);                                                    // [esp+28h]
    // 006D5C2D..006D5DE7: P = self + (to - self) * k, k = 0.8, 0.9, 0.95 (x is rounded to a float before the scaling)
    const M32 factors[3] = {lit(object_projectile_literal_0074b2e0), lit(object_projectile_literal_00747da8), lit(object_projectile_literal_00745e50)};
    Point probe[4];                                                                        // [esp+6Ch] [esp+4Ch] [esp+78h], 4th = [esp+1Ch]
    for (int k = 0; k < 3; ++k) {
        const double a = x87::sub(x87::ld(to.z), f(self, 0x2c));
        const double b = x87::sub(x87::ld(to.y), f(self, 0x28));
        const M32 c = x87::st(x87::sub(x87::ld(to.x), f(self, 0x24)));
        const M32 az = x87::st(x87::mul(a, factors[k]));
        const M32 by = x87::st(x87::mul(b, factors[k]));
        const M32 cx = x87::st(x87::mul(x87::ld(c), factors[k]));
        const double z = x87::add(x87::ld(az), f(self, 0x2c));
        const double y = x87::add(x87::ld(by), f(self, 0x28));
        const double x = x87::add(x87::ld(cx), f(self, 0x24));
        probe[k] = {x87::st(x), x87::st(y), x87::st(z)};
    }
    // 006D5CA7..006D5DF4: durations 0.6 / 0.2 / 0.1 of [self+190h] (fild + 2^32 when negative: unsigned) via _ftol2.
    const double duration = static_cast<double>(rd<U>(self, 0x190));
    const U t1 = x87::ftol2_low(x87::ld(lit(object_projectile_literal_0075147c)) * duration);   // [esp+68h]
    const U t2 = x87::ftol2_low(x87::ld(lit(object_projectile_literal_0073e46c)) * duration);   // [esp+64h]
    const U t3 = x87::ftol2_low(duration * x87::ld(lit(object_projectile_literal_0073e798)));   // [esp+40h]
    probe[3] = to;                                                                         // 006D5DFB..006D5E13
    if (scene != nullptr) {                                                                // 006D5DF9..006D5E17
        wr<U>(hit.bytes, 0, 1);                                                            // 006D5E81
        for (int k = 0; k < 3; ++k) {                                                      // 006D5E93 / 006D5F37 / 006D5FDB
            if (vertical_hit(scene, probe[k].x, probe[k].y, &out, hit) != 0) {
                probe[k].z = x87::m32(&out.z);
                rows[k] = rd<U>(hit.bytes, 0x34);                                          // [esp+ECh] = CHitInfo+34h
            } else {
                probe[k].z = M32{0};
                all_hit = false;
            }
        }
        if (vertical_hit(scene, to.x, to.y, &out, hit) != 0) {                             // 006D607C..006D6081
            probe[3].z = x87::m32(&out.z);                                                 // 006D6099
            rows[3] = rd<U>(hit.bytes, 0x34);                                              // 006D609D
            if (all_hit) {                                                                 // 006D6087..006D60A4
                // 006D60AA..006D648F: four events; every magnitude below is computed and discarded (fstp st(0)).
                four_point_event(self, probe[0], t1, rd<U>(self, 0x124), rows[0]);   // E1 (+18h = [self+124h])
                (void)magnitude(difference(point_at(self, 0x24), point_at(self, 0x114)));     // 006D612A..006D615B
                (void)magnitude(difference(probe[0], probe[1]));                           // 006D615D..006D618D
                four_point_event(self, probe[1], t2, kInvalidId, rows[1]);   // E2
                (void)magnitude(difference(point_at(self, 0x24), point_at(self, 0x114)));     // 006D620B..006D623C
                const Point d23 = difference(probe[1], probe[2]);                          // [esp+44h] / [esp+68h] / [esp+64h]
                (void)magnitude(d23);                                                      // 006D623E..006D6283
                four_point_event(self, probe[2], t3, kInvalidId, rows[2]);   // E3
                (void)magnitude(difference(point_at(self, 0x24), point_at(self, 0x114)));     // 006D6304..006D6339
                (void)magnitude(d23);                                                      // 006D6335..006D635B (the stored floats)
                four_point_event(self, probe[3], t3, kInvalidId, rows[3]);   // E4 (+20h reuses [esp+40h])
                return;
            }
        }
    }
    // 006D63BE..006D648F: one event at the destination.
    (void)magnitude(difference(point_at(self, 0x24), point_at(self, 0x114)));             // 006D63BE..006D63EF (discarded)
    (void)magnitude(difference(point_at(self, 0x24), to));                                 // 006D63F1..006D641E (discarded)
    void* const event = new_event();
    put_point(event, 0, to);
    put_point(event, 0xc, to);
    wr<B>(event, 0x2d, 2);
    wr<B>(event, 0x2c, 0);
    wr<U>(event, 0x20, rd<U>(self, 0x190));
    wr<U>(event, 0x28, kMinusOne);
    wr<U>(event, 0x1c, 0);
    wr<U>(event, 0x18, kInvalidId);
    wr<U>(event, 0x24, 0);
    append(self, event);
}                                                                                          // 006D6494..006D64CE ~CHitInfo

// 006D6A90..006D6ED1: an event at the destination, then one at a random point 3 units from [self+114h] in a
// direction turned 45..135 degrees from the facing (x, y), snapped to the ground, timed at 0.4 x the average speed.
// RET 0.
void __fastcall object_projectile_006d6a90(void* self, void*) {
    Vector out;                                                                            // [esp+3Ch]
    std::memset(&out, 0, sizeof out);                                                      // 006D6AC6..006D6AD6
    HitInfo hit;                                                                           // [esp+48h] (state 0)
    const Point to = destination(self);                                                    // 006D6AE3..006D6B5B (ebp, [esp+20h], [esp+24h])
    void* const scene = scene_of();  // 006D6B5F..006D6B70
    void* const first = new_event();                             // 006D6B76..006D6B99
    put_point(first, 0, to);
    put_point(first, 0xc, to);
    wr<B>(first, 0x2d, 2);
    wr<B>(first, 0x2c, 0);
    wr<U>(first, 0x20, rd<U>(self, 0x190));
    wr<U>(first, 0x24, 0);
    wr<U>(first, 0x18, rd<U>(self, 0x124));
    wr<U>(first, 0x1c, 0);
    wr<U>(first, 0x28, kMinusOne);
    append(self, first);                                                                   // 006D6BE0..006D6BE7
    // 006D6BEC..006D6C4E: angle = min(atan(-fy / -fx) + (rand % 1570) * 0.001 + pi/4, 3pi/4)
    const M32 ny = x87::st(-x87::ld(f(self, 0x34)));                                       // fld; fchs; fstp [esp+28h]
    wr<U>(hit.bytes, 0, 1);                                                                // 006D6BF1
    const M32 nx = x87::st(-x87::ld(f(self, 0x30)));                                       // [esp+10h]
    const std::int32_t r = crt_rand_006fc447() % 0x622;                                    // 006D6C06..006D6C13 cdq; idiv
    const double scaled = x87::mul(static_cast<double>(r), lit(object_projectile_literal_0073d708));   // fild; fmul dword (0.001f)
    const double q = x87::div(x87::ld(ny), nx);                                            // 006D6C21..006D6C25
    double angle = x87::add(x87_atan_plus(q, scaled), lit(object_projectile_literal_00741c3c));   // 006D6C29..006D6C33
    if (angle > x87::ld(lit(object_projectile_literal_00741c38))) angle = x87::ld(lit(object_projectile_literal_00741c38));   // 006D6C39..006D6C48
    const M32 three = lit(object_projectile_literal_00740c84);                             // fmul dword [00740C84] (3.0f)
    M32 cosine = x87_cos_to_m32(angle);                                                    // [esp+2Ch]
    M32 along_y, along_x;                                                                  // [esp+28h] / [esp+10h]
    if (x87::ld(nx) < x87::ld(lit(object_projectile_literal_0073d700))) {                   // 006D6C58..006D6C67 (jp: >= 0 or unordered)
        const M32 sine = M32{x87_sin_to_m32(angle).bits ^ 0x80000000u};                    // fchs; fstp [esp+14h]; fld [esp+30h]
        cosine = M32{cosine.bits ^ 0x80000000u};                                           // fld [esp+2Ch]; fchs; fstp
        along_y = x87::st(x87::mul(x87::ld(sine), three));
    } else {
        along_y = x87_sin_times_to_m32(angle, three);                                     // fsin result times 3 (wide)
    }
    along_x = x87::st(x87::mul(x87::ld(cosine), three));                                  // 006D6CB5..006D6CBF
    const double zero = x87::mul(x87::ld(M32{0}), three);                                 // fld [esp+34h] (= 0); fmul 3.0
    Point p = {x87::st(x87::add(x87::ld(along_x), f(self, 0x114))),                        // 006D6CC3..006D6CF5
               x87::st(x87::add(x87::ld(along_y), f(self, 0x118))),
               x87::st(x87::add(zero, f(self, 0x11c)))};
    if (scene != nullptr && vertical_hit(scene, p.x, p.y, &out, hit) != 0)                 // 006D6C9D / 006D6D01..006D6D68
        p.z = x87::m32(&out.z);                                                            // 006D6D6A..006D6D6E
    else
        p.z = M32{0};                                                                      // 006D6D74
    // 006D6D7C..006D6E07: time = |p - [self+114h]| / (0.4 * |self - [self+114h]| / [self+190h]) via _ftol2
    const double travelled = magnitude(difference(point_at(self, 0x24), point_at(self, 0x114)));
    const double duration = static_cast<double>(rd<U>(self, 0x190));                       // fild (+2^32 when negative)
    const M32 speed = x87::st(x87::mul(x87::div(travelled, duration), lit(object_projectile_literal_0073e320)));   // [esp+28h]
    const double remaining = magnitude(difference(point_at(self, 0x114), p));
    const U time = x87::ftol2_low(x87::div(remaining, speed));                             // 006D6E03..006D6E07
    void* const second = new_event();                            // 006D6E0C..006D6E31
    put_point(second, 0, p);
    put_point(second, 0xc, p);
    wr<B>(second, 0x2d, 2);
    wr<B>(second, 0x2c, 0);
    wr<U>(second, 0x20, time);
    wr<U>(second, 0x24, 0);
    wr<U>(second, 0x18, kInvalidId);
    wr<U>(second, 0x1c, 0);
    wr<U>(second, 0x28, kMinusOne);
    append(self, second);                                                                  // 006D6E76..006D6E7D
}                                                                                          // 006D6E82..006D6EB9 ~CHitInfo

// 006D70D0..006D7585: frees the previous event, then takes events from [self+194h] until one is not a jump (type
// 0Bh) and its duration exceeds the elapsed time [self+44h]: returns 1; 0 when the list runs empty. Per event: the
// type-2 sound, the type-0Bh reposition with animations 0 / 1, the spawned child projectiles, then the projectile
// takes the event (+120h/+190h duration, +114h point, motion 006D33C0) and its mode 15 / 16 effect. Events taken
// after the first are not freed here (only [self+198h] at entry). RET 0.
U __fastcall object_projectile_006d70d0(void* self, void*) {
    if (void* const previous = ptr(self, 0x198)) {                                         // 006D70F3..006D70FC
        live_free(previous);                                                               // 006D70FE..006D7104
        wr<U>(self, 0x198, 0);                                                             // 006D7107
    }
    for (;;) {
        auto* const list = static_cast<kotor_list::Internal*>(ptr(self, 0x194));          // 006D7111
        if (rd<U>(list, 0) == 0) return 0;                                                 // 006D7117..006D711D (head null)
        void* const event = base_ini_reconstruction::remove_head_005e9c80(list, nullptr);  // 006D7123
        wr(self, 0x198, event);                                                            // 006D712A
        if (event == nullptr) return 0;                                                    // 006D7130
        play_event_sound(self);                                                            // 006D7136..006D71F7
        if (rd<B>(ptr(self, 0x198), 0x2d) == 0x0b) {                                       // 006D71FC..006D7206
            set_animation(self, 0u);                                                       // 006D720C..006D7224
            animate_external(self);                                                        // 006D7228..006D7240
            const U id = rd<U>(ptr(self, 0x198), 0x18);                                    // 006D7245..006D724B
            if (id == menu_tables_m41::cells::invalid_object_00746644) {                   // 006D724E..006D7254
                object_table(self)->s08c_set_position(self, nullptr, static_cast<const float*>(ptr(self, 0x198)));   // 006D72AC..006D72B1 (EAX = event)
            } else {
                void* const array = kotor_fastcall_005ed570(client(), nullptr);            // 006D7256..006D725E
                void* object;                                                              // [esp+14h]
                if (kotor_fastcall_004d8230(array, nullptr, rd<U>(ptr(self, 0x198), 0x18), &object) ==
                    object_projectile_literal_0074666c[0]) {                               // 006D7263..006D727F
                    void* const now = ptr(self, 0x198);                                    // 006D7285
                    const double z = x87::add(x87::ld(f(object, 0x2c)), f(now, 8));        // 006D728B..006D729A
                    const double y = x87::add(x87::ld(f(object, 0x28)), f(now, 4));
                    const double x = x87::add(x87::ld(f(object, 0x24)), f(now, 0));
                    float position[3];                                                     // [esp+3Ch]
                    x87::put(&position[0], x87::st(x));
                    x87::put(&position[1], x87::st(y));
                    x87::put(&position[2], x87::st(z));
                    object_table(self)->s08c_set_position(self, nullptr, position);        // 006D72AC..006D72B1
                }
            }
            animate_external(self);                                                        // 006D72B7..006D72CF
            set_animation(self, 1u);                                                       // 006D72D4..006D72ED
        }
        // 006D72F3..006D747C: [event+2Ch] children (BYTE counter, count re-read after each child)
        B spawned = 0;
        if (rd<B>(ptr(self, 0x198), 0x2c) != 0) {
            do {
                void* const spell = kotor_m15b_spell_array_get_spell_0059b6d0(ptr(kotor_cell_rules_007a3a28, 0x8c), nullptr,
                                                                              rd<std::int32_t>(self, 0x174));   // 006D7310..006D7323
                if (spell == nullptr) break;                                               // 006D732C
                void* const child = new_projectile();                                      // 006D7332..006D735A
                object_projectile_006d41b0(child, nullptr, rd<U>(self, 0x174));            // 006D735C..006D736D
                (void)object_table(child)->s078_load_model(child, nullptr, at(spell, 0xf8), 0xffu, 0u);   // 006D7372..006D7384
                if (void* const module = kotor_fastcall_005ed530(client(), nullptr)) {     // 006D7387..006D7396
                    if (void* const area = ptr(module, 0x48))                              // 006D7398..006D739D
                        (void)object_table(child)->s094_attach_to_scene(child, nullptr, ptr(area, 0x184));   // 006D739F..006D73AA
                }
                object_table(child)->s08c_set_position(child, nullptr, static_cast<const float*>(at(self, 0x24)));      // 006D73B0..006D73B8
                object_table(child)->s088_set_orientation(child, nullptr, static_cast<const float*>(at(self, 0x30)));   // 006D73BE..006D73C6
                const U duration = rd<U>(ptr(self, 0x198), 0x20);                          // 006D73CC..006D73D2 (EAX)
                wr(child, 0x120, duration);
                wr(child, 0x190, duration);
                // 006D73E3: 0063C060 writes AX only; EBP = EAX keeps the high word of the duration loaded at 006D73D2.
                const U animation = (duration & 0xffff0000u) | kotor_m15a_object_get_current_animation_0063c060(self, nullptr);
                set_animation(child, animation);                                           // 006D73E8..006D7404 (WORD BP)
                put_point(child, 0x114, point_at(ptr(self, 0x198), 0));                    // 006D7408..006D7421
                {
                    S name(base_ini_reconstruction::warning_empty_0073d71c);               // 006D7424..006D742D "" (state 3)
                    object_projectile_006d42d0(child, nullptr, rd<U>(self, 0x124), &name);   // 006D7432..006D7448
                }                                                                          // 006D744D..006D7459
                object_projectile_006d6ee0(child, nullptr, rd<U>(ptr(self, 0x198), 0x1c));   // 006D745E..006D746A SetMode
                ++spawned;                                                                 // 006D7478 inc bl
            } while (spawned < rd<B>(ptr(self, 0x198), 0x2c));                             // 006D746F..006D747C (jb)
        }
        void* const current = ptr(self, 0x198);                                            // 006D7482
        const U duration = rd<U>(current, 0x20);
        wr(self, 0x120, duration);                                                         // 006D748B
        wr(self, 0x190, duration);                                                         // 006D7491
        put_point(self, 0x114, point_at(current, 0));                                      // 006D7497..006D74AA
        object_projectile_006d33c0(self, nullptr, static_cast<Vector*>(at(self, 0x30)));   // 006D74AD..006D74B3 motion
        const U mode = rd<U>(self, 0x16c);                                                 // 006D74B8
        if (mode == 0xf) {                                                                 // 006D74BE..006D74C1
            wr<U>(self, 0x1a4, 1);                                                         // 006D74CC (before the string)
            S name(object_projectile_literal_00743318);                                    // 006D74C3..006D74D6 "impact" (state 4)
            object_projectile_006d42d0(self, nullptr, rd<U>(ptr(self, 0x198), 0x18), &name);   // 006D74DB..006D74F4
        } else if (mode == 0x10) {                                                         // 006D750C..006D750F
            if (void* const area = ptr(self, 0x20)) {                                      // 006D7511..006D7516
                // 006D751B..006D7532: the position goes onto the stack as three DWORD copies (never through the FPU,
                // which would quiet a signalling NaN); the floats are filled by memcpy here, in this frame.
                float x, y, z;
                std::memcpy(&x, at(self, 0x24), 4);
                std::memcpy(&y, at(self, 0x28), 4);
                std::memcpy(&z, at(self, 0x2c), 4);
                (void)recovered_client_area_apply_fire_and_forget_visual_effect_00604c90(area, nullptr, 0xfabu, x, y, z);   // 006D7535
            }
        }
        if (rd<B>(ptr(self, 0x198), 0x2d) == 0x0b) continue;                               // 006D753A..006D7544
        if (rd<U>(self, 0x44) >= rd<U>(self, 0x120)) continue;                             // 006D754A..006D7553 (jae)
        return 1;                                                                          // 006D7559
    }
}

}  // extern "C"
