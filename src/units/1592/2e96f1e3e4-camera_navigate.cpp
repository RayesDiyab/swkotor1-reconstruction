// CSWCameraNavigate table 007515F4 and the five R1 bodies.
// Source-only reconstruction from the pinned original disassembly. The table and
// all five bodies are the only new definitions here; every helper is an existing
// selected provider (see ../PROVIDERS.md). No original address is dereferenced and
// no original byte payload is embedded.
#include "39f0b22b2a-camera_navigate.h"

#include <cstddef>

namespace {

using kotor_scene3d::bits_to_float;
using kotor_scene3d::field;
using kotor_scene3d::Quaternion;
using kotor_scene3d::Vector;

// Exact float constants from the pinned .rdata reads.
constexpr float kFive = 5.0f;                                          // 00743264; bits 0x40A00000
constexpr float kThree = 3.0f;                                         // 00740C84; bits 0x40400000
constexpr float kThreePointThreeThree = 3.3299999237060546875f;        // 00751624; bits 0x40551EB8
constexpr float kTwoPointSixSeven = 2.6700000762939453125f;            // 00751620; bits 0x402AE148
constexpr float kNinety = 90.0f;                                       // 0073E40C; bits 0x42B40000
constexpr float kMinusNinety = -90.0f;                                 // 0074FC44; bits 0xC2B40000
constexpr float kLowpassC1 = 0.96700000762939453125f;                  // 0075161C; bits 0x3F778D50
constexpr float kLowpassC2 = 0.032999999821186065673828125f;           // 00743330; bits 0x3D072B02

// The four genuine camera-Gob vtable methods used by Control. The out buffer is an
// explicit caller stack argument; the callee owns its stack cleanup (0x10/0x14/4/4).
struct OwnerVTable {
    void* slots[0x5c / 4];
    void (__thiscall* set_position)(void*, Vector*, Vector);
    void (__thiscall* set_orientation)(void*, Quaternion*, Quaternion);
    Vector* (__thiscall* get_position)(void*, Vector*);
    Quaternion* (__thiscall* get_orientation)(void*, Quaternion*);
};
static_assert(offsetof(OwnerVTable, set_position) == 0x5c, "owner +0x5c");
static_assert(offsetof(OwnerVTable, set_orientation) == 0x60, "owner +0x60");
static_assert(offsetof(OwnerVTable, get_position) == 0x64, "owner +0x64");
static_assert(offsetof(OwnerVTable, get_orientation) == 0x68, "owner +0x68");

inline void* owner_ptr(void* self) {   // 0063884E / 00638B55 / ... reload [self+0x10]
    return *reinterpret_cast<void* volatile*>(static_cast<unsigned char*>(self) + 0x10);
}

inline Quaternion owner_orientation(void* self) {
    Quaternion q;
    void* const o = owner_ptr(self);
    OwnerVTable* const vt = *reinterpret_cast<OwnerVTable* volatile*>(o);
    vt->get_orientation(o, &q);
    return q;
}

inline Vector owner_position(void* self) {
    Vector p;
    void* const o = owner_ptr(self);
    OwnerVTable* const vt = *reinterpret_cast<OwnerVTable* volatile*>(o);
    vt->get_position(o, &p);
    return p;
}

inline void owner_set_orientation(void* self, Quaternion q) {
    Quaternion out;
    void* const o = owner_ptr(self);
    OwnerVTable* const vt = *reinterpret_cast<OwnerVTable* volatile*>(o);
    vt->set_orientation(o, &out, q);
}

inline void owner_set_position(void* self, Vector v) {
    Vector out;
    void* const o = owner_ptr(self);
    OwnerVTable* const vt = *reinterpret_cast<OwnerVTable* volatile*>(o);
    vt->set_position(o, &out, v);
}

inline Quaternion ypr(float yaw, float pitch, float roll) {
    Quaternion q;
    yaw_pitch_roll_004acac0(&q, yaw, pitch, roll);
    return q;
}

inline Quaternion qmul(const Quaternion& a, const Quaternion& b) {
    Quaternion out;
    quaternion_multiply_004a9ac0(&out, &a, &b);
    return out;
}

inline Vector qrotate(const Quaternion& q, const Vector& v) {
    Vector out;
    quaternion_rotate_004a9bb0(&out, &q, &v);
    return out;
}

inline Quaternion qnormalize(const Quaternion& q) {
    Quaternion out;
    quaternion_normalize_004a9d60(&out, q);
    return out;
}


// f2c term first: the original's final faddp st(1) computes st1(f2c) + st0(f28).
inline float lowpass(float delta, float f28, float f2c) {
    const double c1 = static_cast<double>(kLowpassC1);
    const double c2 = static_cast<double>(kLowpassC2);
    const double two_delta = static_cast<double>(delta) + static_cast<double>(delta);
    const double f2c_term = (c1 - two_delta) * static_cast<double>(f2c);
    const double f28_term = (c2 + two_delta) * static_cast<double>(f28);
    return static_cast<float>(f2c_term + f28_term);
}

// Branch B inline up-axis sign: the original evaluates (B+A)*0.0, so a non-finite
// B+A makes the comparison NaN. sign=-1 only for ordered cmp >= 0; NaN leaves +1.
inline bool sign_is_minus(const Quaternion& q) {
    const double W = static_cast<double>(q.w);
    const double X = static_cast<double>(q.x);
    const double Y = static_cast<double>(q.y);
    const double Z = static_cast<double>(q.z);
    const double zx = Z * X;
    const double wy = W * Y;
    const double asum = zx + wy;
    const double a = asum + asum;
    const double zy = Z * Y;
    const double wx = W * X;
    const double bsum = zy - wx;
    const double b = bsum + bsum;
    const double yy = Y * Y;
    const double xx = X * X;
    const double sumsq = yy + xx;
    const double zdir = 1.0 - (sumsq + sumsq);
    volatile double zero = 0.0;
    const double cmp = (b + a) * zero + zdir;
    return cmp >= 0.0;
}

inline Vector scale_delta(const Vector& rot, float delta) {
    Vector r;
    r.x = static_cast<float>(static_cast<double>(delta) * static_cast<double>(rot.x));
    r.y = static_cast<float>(static_cast<double>(delta) * static_cast<double>(rot.y));
    r.z = static_cast<float>(static_cast<double>(delta) * static_cast<double>(rot.z));
    return r;
}

// dv first, then pos: the original fld dv; fadd [owner position].
inline Vector add_dv_pos(const Vector& dv, const Vector& pos) {
    Vector r;
    r.x = static_cast<float>(static_cast<double>(dv.x) + static_cast<double>(pos.x));
    r.y = static_cast<float>(static_cast<double>(dv.y) + static_cast<double>(pos.y));
    r.z = static_cast<float>(static_cast<double>(dv.z) + static_cast<double>(pos.z));
    return r;
}

// Pitch clamp: the original compares 0 against the double result before the
// fstp dword, so clamp on the double and narrow afterwards. NaN and -0.0 are kept.
inline float clamp_pitch(double p) {
    return (p < 0.0) ? 0.0f : static_cast<float>(p);
}

} // namespace

extern "C" {

// 006386A0 GetType.
std::uint32_t __fastcall kotor_fastcall_006386a0(void*, void*) { return 0x1071u; }

// 006386D0 non-deleting derived destructor: write the derived table, then the base dtor.
void __fastcall kotor_fastcall_006386d0(void* self, void*) {
    using Slots = kotor_scene3d_construct::BehaviorBaseSlots;
    *reinterpret_cast<const Slots**>(self) = &w4cam::camera_navigate_vtable_007515f4;
    behavior_destruct_0049fa70(self, nullptr);
}

// 006386B0 scalar deleting destructor: derived/base destructor, then free only for flags&1.
void* __fastcall kotor_fastcall_006386b0(void* self, void*, std::uint32_t flags) {
    kotor_fastcall_006386d0(self, nullptr);
    if ((flags & 1u) != 0u) live_free(self);
    return self;
}

// 006386E0 Message: six independent keyword/parse-float pairs, exact order and offsets.
void __fastcall kotor_fastcall_006386e0(void* self, void*, char* line) {
    static const char kTurn[] = "turn";
    static const char kTilt[] = "tilt";
    static const char kSpeed[] = "speed";
    static const char kRaise[] = "raise";
    static const char kStrafe[] = "strafe";
    static const char kRoll[] = "roll";

    if (kotor_stricmp_0070acaf(kTurn, firstword_00463530(line)) == 0)
        parse_float_004635c0(
            line + 4, reinterpret_cast<volatile float*>(static_cast<unsigned char*>(self) + 0x14));
    if (kotor_stricmp_0070acaf(kTilt, firstword_00463530(line)) == 0)
        parse_float_004635c0(
            line + 4, reinterpret_cast<volatile float*>(static_cast<unsigned char*>(self) + 0x18));
    if (kotor_stricmp_0070acaf(kSpeed, firstword_00463530(line)) == 0)
        parse_float_004635c0(
            line + 5, reinterpret_cast<volatile float*>(static_cast<unsigned char*>(self) + 0x1c));
    if (kotor_stricmp_0070acaf(kRaise, firstword_00463530(line)) == 0)
        parse_float_004635c0(
            line + 5, reinterpret_cast<volatile float*>(static_cast<unsigned char*>(self) + 0x20));
    if (kotor_stricmp_0070acaf(kStrafe, firstword_00463530(line)) == 0)
        parse_float_004635c0(
            line + 6, reinterpret_cast<volatile float*>(static_cast<unsigned char*>(self) + 0x24));
    if (kotor_stricmp_0070acaf(kRoll, firstword_00463530(line)) == 0)
        parse_float_004635c0(
            line + 4, reinterpret_cast<volatile float*>(static_cast<unsigned char*>(self) + 0x28));
}

// 006387E0 Control.
std::uint32_t __fastcall kotor_fastcall_006387e0(void* self, void*, std::uint32_t raw_delta) {
    if (behavior_control_0049f270(self, nullptr, raw_delta) == 0) return 0;

    const float delta = bits_to_float(raw_delta);
    const std::int32_t toggle = kotor_input_cells_required::camera_toggle_008338e8;
    (void)owner_ptr(self); // original loads [self+0x10] here; every vcall reloads it

    if (toggle != 0) {
        // Branch A.
        field<float>(self, 0x1c) =
            static_cast<float>(static_cast<double>(field<float>(self, 0x1c)) * static_cast<double>(kFive));
        field<float>(self, 0x24) =
            static_cast<float>(static_cast<double>(field<float>(self, 0x24)) * static_cast<double>(kThree));
        field<float>(self, 0x18) = static_cast<float>(static_cast<double>(field<float>(self, 0x18)) *
                                                      static_cast<double>(kThreePointThreeThree));
        field<float>(self, 0x14) = static_cast<float>(static_cast<double>(field<float>(self, 0x14)) *
                                                      static_cast<double>(kTwoPointSixSeven));

        Quaternion P = owner_orientation(self);
        Quaternion Q = ypr(static_cast<float>(static_cast<double>(delta) *
                                             static_cast<double>(field<float>(self, 0x14))),
                          0.0f, 0.0f);
        Quaternion R = qmul(Q, P);

        P = R;
        field<float>(self, 0x14) = 0.0f;
        Q = ypr(0.0f, kMinusNinety, 0.0f);
        R = qmul(P, Q);

        P = R;
        const float f2c = field<float>(self, 0x2c);
        const float f28 = field<float>(self, 0x28);
        const float s = lowpass(delta, f28, f2c);
        field<float>(self, 0x28) = s;
        field<float>(self, 0x2c) = s;

        const double roll = kotor_cdecl_004acb90(R);
        Q = ypr(0.0f, 0.0f,
                static_cast<float>(static_cast<double>(field<float>(self, 0x28)) - roll));
        R = qmul(P, Q);

        const float pb =
            static_cast<float>(static_cast<double>(delta) * static_cast<double>(field<float>(self, 0x18)));
        P = R;
        const double pitch = quaternion_pitch_004aa020(&P);
        const double diff = pitch - static_cast<double>(pb);
        if (diff > static_cast<double>(kMinusNinety) && diff < static_cast<double>(kNinety)) {
            Q = ypr(0.0f, -pb, 0.0f);
            R = qmul(P, Q);
            P = R;
        }

        const Quaternion Pcopy = P;
        field<float>(self, 0x18) = 0.0f;
        Q = ypr(0.0f, kNinety, 0.0f);
        R = qmul(P, Q);
        P = R;
        Q = qnormalize(R);
        owner_set_orientation(self, Q);

        const float old24 = field<float>(self, 0x24);
        const float old1c = field<float>(self, 0x1c);
        field<float>(self, 0x24) = 0.0f;
        const float neg1c = -old1c;
        field<float>(self, 0x1c) = 0.0f;
        if (old24 == 0.0f && neg1c == 0.0f) return 1;

        const Vector v{old24, neg1c, 0.0f};
        const Vector rot = qrotate(Pcopy, v);
        const Vector dv = scale_delta(rot, delta);
        const Vector pos = owner_position(self);
        const Vector np = add_dv_pos(dv, pos);
        owner_set_position(self, np);
        return 1;
    }

    // Branch B.
    Quaternion O = owner_orientation(self);
    const float yaw = static_cast<float>(quaternion_yaw_004a9f40(&O));
    O = owner_orientation(self);
    const float pitch = static_cast<float>(quaternion_pitch_004aa020(&O));

    float sign = 1.0f;
    O = owner_orientation(self);
    if (sign_is_minus(O)) sign = -1.0f;

    const double dy = static_cast<double>(delta) * static_cast<double>(field<float>(self, 0x14));
    field<float>(self, 0x14) = 0.0f;
    const float yaw2 = static_cast<float>(dy + static_cast<double>(yaw));

    const double p_double =
        (static_cast<double>(sign) * static_cast<double>(field<float>(self, 0x18))) *
            static_cast<double>(delta) +
        static_cast<double>(pitch);
    const float p = clamp_pitch(p_double);
    field<float>(self, 0x18) = 0.0f;

    O = owner_orientation(self);
    const float roll = static_cast<float>(kotor_cdecl_004acb90(O));
    const Quaternion moveQ =
        ypr(yaw2, static_cast<float>(static_cast<double>(p) - 90.0), roll);

    const float strafe = field<float>(self, 0x24);
    const float speed =
        static_cast<float>(static_cast<double>(sign) * static_cast<double>(field<float>(self, 0x1c)));
    if (!(strafe == 0.0f && speed == 0.0f)) {
        const Vector v{strafe, speed, 0.0f};
        const Vector rot = qrotate(moveQ, v);
        const Vector dv = scale_delta(rot, delta);
        const Vector pos = owner_position(self);
        const Vector np = add_dv_pos(dv, pos);
        owner_set_position(self, np);
    }

    O = owner_orientation(self);
    const float roll2 = static_cast<float>(kotor_cdecl_004acb90(O));
    const Quaternion finalQ = ypr(yaw2, p, roll2);
    owner_set_orientation(self, finalQ);
    return 1;
}

} // extern "C"

// The one external typed table definition. Relocations point to the five bodies and
// the three existing 0471 helpers; the table is retained by the Navigate ctor import.
namespace w4cam {
const kotor_scene3d_construct::BehaviorBaseSlots camera_navigate_vtable_007515f4 = {
    kotor_fastcall_006386b0,
    behavior_void_return_004015a0,
    kotor_fastcall_006387e0,
    kotor_fastcall_006386e0,
    kotor_fastcall_006386a0,
    behavior_null_return_0063e7f0,
    behavior_return_zero_0063e7f0,
    behavior_return_zero_0063e7f0,
};
} // namespace w4cam
