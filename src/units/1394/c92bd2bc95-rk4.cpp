// Lane W4 G14: CSWRK4Acceleration / CSWRK4SplitAcceleration (original 006D0A40..006D12B0, 00670830, 006707A0).
// Object: +00 vtable (slot 00h = GetAccelerationVector), +04..+0C last input, +10..+24 state0, +28..+3C state1,
// +40 mass, +44 gain, (split) +48 release gain. State = {first triple, second triple}; derivative =
// {accel(first, input), first}. Size 44h (base) / 4Ch (split).
#include "4644e3d982-camera_stick.hpp"

using namespace w4;

namespace {
const float k_rk4_max_step_0073e46c = 0.2f;     // bits 3E4CCCCD
const float k_half_0073e9ac = 0.5f;
void* const rk4_vtable_00757bd4[1] = {reinterpret_cast<void*>(&kotor_fastcall_006d0fc0)};
void* const rk4_split_vtable_00757bd8[1] = {reinterpret_cast<void*>(&kotor_fastcall_006d10d0)};
inline float* fp(void* o, U32 off) { return reinterpret_cast<float*>(static_cast<U8*>(o) + off); }
inline W4RK4State& st(void* o, U32 off) { return *reinterpret_cast<W4RK4State*>(static_cast<U8*>(o) + off); }
W4Vector* accel(void* self, W4Vector* out, const W4Vector* p, const W4Vector* q) {   // call [eax] (slot 00h)
    return reinterpret_cast<W4Vector*(__thiscall*)(void*, W4Vector*, const W4Vector*, const W4Vector*)>(vslot(self, 0))(self, out, p, q);
}
// out = gain*q - (gain/mass)*p with the original rounding points (006D0FC0 / 006D110A).
void spring(W4Vector* out, double gain, double mass, const W4Vector* p, const W4Vector* q) {
    const double k = gain / mass;                                                          // fld gain; fdiv mass
    const double kpx = k * p->x;                                                           // kept on ST
    const float kpy = static_cast<float>(k * p->y), kpz = static_cast<float>(k * p->z);   // fstp float
    const float gqx = static_cast<float>(gain * q->x), gqy = static_cast<float>(gain * q->y), gqz = static_cast<float>(gain * q->z);
    out->x = static_cast<float>(static_cast<double>(gqx) - kpx);
    out->y = static_cast<float>(static_cast<double>(gqy) - kpy);
    out->z = static_cast<float>(static_cast<double>(gqz) - kpz);
}
W4RK4State scale(const W4RK4State& s, float k) { W4RK4State r; kotor_cdecl_00670830(&r, &s, k); return r; }
W4RK4State add(const W4RK4State& a, const W4RK4State& b) { W4RK4State r; kotor_cdecl_006707a0(&r, &a, &b); return r; }
W4RK4State divide(const W4RK4State& s, float d) { W4RK4State r; kotor_cdecl_006d0a40(&r, &s, d); return r; }
W4RK4State derivative(void* self, const W4RK4State& s, const W4Vector* q) {
    W4Vector a;
    W4RK4State d;
    d.position = *accel(self, &a, &s.position, q);                                         // {accel(first, q), first}
    d.velocity = s.position;
    return d;
}
}  // namespace

// 00670830 operator*(state, float) (cdecl; returns out).
extern "C" W4RK4State* __cdecl kotor_cdecl_00670830(W4RK4State* out, const W4RK4State* s, float k) {
    const float* in = &s->position.x;
    float r[6];
    for (int i = 0; i < 6; ++i) r[i] = static_cast<float>(static_cast<double>(k) * in[i]);   // fld k; fmul s[i]; fstp
    std::memcpy(out, r, 24);
    return out;
}
// 006707A0 operator+(state, state).
extern "C" W4RK4State* __cdecl kotor_cdecl_006707a0(W4RK4State* out, const W4RK4State* a, const W4RK4State* b) {
    const float* x = &a->position.x; const float* y = &b->position.x;
    float r[6];
    for (int i = 0; i < 6; ++i) r[i] = static_cast<float>(static_cast<double>(x[i]) + y[i]);
    std::memcpy(out, r, 24);
    return out;
}
// 006D0A40 operator/(state, float): zero divisor -> zero state; else both triples via Vector operator/ 00437A90.
extern "C" W4RK4State* __cdecl kotor_cdecl_006d0a40(W4RK4State* out, const W4RK4State* s, float d) {
    W4RK4State r = {{0, 0, 0}, {0, 0, 0}};
    if (jcc_jp(d, 0.0f, 0x44)) {                                                           // 006D0A40..006D0A54 (d != 0)
        vector_divide_00437a90(&r.velocity, &s->velocity, d);                              // 006D0A5F..006D0A69 (second triple first)
        vector_divide_00437a90(&r.position, &s->position, d);                              // 006D0A6E..006D0A77
    }
    *out = r;                                                                              // 006D0AF2..006D0B0D
    return out;
}

// 006D0B20 CSWRK4Acceleration::CSWRK4Acceleration().
extern "C" void* __fastcall kotor_fastcall_006d0b20(void* self, void*) {
    sw(self, 0, ad(rk4_vtable_00757bd4));                                                  // 006D0B27
    for (U32 off = 4; off < 0x40; off += 4) sw(self, off, 0);                              // 006D0B2D..006D0B86
    sw(self, 0x40, 0x42C80000u); sw(self, 0x44, 0x42C80000u);                              // 006D0BB9..006D0BBC 100.0f
    return self;
}
// 006D10B0 CSWRK4SplitAcceleration::CSWRK4SplitAcceleration().
extern "C" void* __fastcall kotor_fastcall_006d10b0(void* self, void*) {
    kotor_fastcall_006d0b20(self, nullptr);                                                // 006D10B3
    sw(self, 0, ad(rk4_split_vtable_00757bd8));                                            // 006D10B8
    sw(self, 0x48, 0x42C80000u);                                                           // 006D10BE
    return self;
}
// 006D0FC0 CSWRK4Acceleration::GetAccelerationVector(out, p, q) (RET 0Ch).
extern "C" W4Vector* __fastcall kotor_fastcall_006d0fc0(void* self, void*, W4Vector* out, const W4Vector* p, const W4Vector* q) {
    spring(out, lf(self, 0x44), lf(self, 0x40), p, q);
    return out;
}
// 006D10D0 CSWRK4SplitAcceleration::GetAccelerationVector(out, p, q) (RET 0Ch).
extern "C" W4Vector* __fastcall kotor_fastcall_006d10d0(void* self, void*, W4Vector* out, const W4Vector* p, const W4Vector* q) {
    if (jcc_jp(q->x, 0.0f, 0x44) || jcc_jp(q->y, 0.0f, 0x44) || jcc_jp(q->z, 0.0f, 0x44)) // 006D10D0..006D1108
        return kotor_fastcall_006d0fc0(self, nullptr, out, p, q);                          // 006D116F..006D117A
    spring(out, lf(self, 0x48), lf(self, 0x40), p, q);                                     // 006D110A..006D1166
    return out;
}
// 006D0BD0 CSWRK4Acceleration::Accelerate(const Vector* input, float h) (RET 8).
extern "C" void __fastcall kotor_fastcall_006d0bd0(void* self, void*, const W4Vector* input, float h) {
    if (!jcc_jnp(h, k_rk4_max_step_0073e46c, 0x05)) h = k_rk4_max_step_0073e46c;          // 006D0BD0..006D0BED
    const W4Vector* last = reinterpret_cast<const W4Vector*>(fp(self, 4));                 // ebp
    W4Vector avg;                                                                          // [esp+28h]
    avg.x = static_cast<float>((static_cast<double>(input->x) + last->x) * k_half_0073e9ac);       // 006D0C1F..006D0C79
    avg.y = static_cast<float>((static_cast<double>(last->y) + input->y) * k_half_0073e9ac);       // 006D0C2F..006D0C91
    const float zsum = static_cast<float>(static_cast<double>(last->z) + input->z);                // 006D0C41..006D0C5D
    avg.z = static_cast<float>(static_cast<double>(zsum) * k_half_0073e9ac);                       // 006D0C95..006D0CA5
    const W4RK4State s0 = st(self, 0x10);
    const W4RK4State k1 = scale(derivative(self, s0, last), h);                            // 006D0CA9..006D0CE8
    const W4RK4State k2 = scale(derivative(self, add(s0, scale(k1, 0.5f)), &avg), h);     // 006D0CED..006D0D89
    const W4RK4State k3 = scale(derivative(self, add(s0, scale(k2, 0.5f)), &avg), h);     // 006D0D8E..006D0E30
    const W4RK4State k4 = scale(derivative(self, add(s0, k3), input), h);                  // 006D0E35..006D0EC1
    const W4RK4State d4 = divide(k4, 6.0f);                                                // 006D0ED3..006D0EE5
    const W4RK4State d3 = divide(k3, 3.0f);                                                // 006D0EED..006D0F00



    const W4RK4State d2 = divide(k2, 3.0f);                                                // 006D0F08..006D0F1E

    const W4RK4State d1 = divide(k1, 6.0f);                                                // 006D0F26..006D0F39
    st(self, 0x28) = add(add(add(add(s0, d1), d2), d3), d4);                               // 006D0F3E..006D0F94
    std::memcpy(fp(self, 4), input, 12);                                                   // 006D0F96..006D0FA9
}
// 006D1190 CSWRK4Acceleration::Update(out, Vector input, h, clear_a, clear_b, clear_c, clear_d) (RET 24h).
extern "C" W4RK4State* __fastcall kotor_fastcall_006d1190(void* self, void*, W4RK4State* out, W4Vector input, float h,
                                                          U32 a, U32 b, U32 c, U32 d) {
    if (c != 0) std::memset(fp(self, 0x34), 0, 12);                                        // 006D1192..006D11C9
    if (d != 0) std::memset(fp(self, 0x28), 0, 12);                                        // 006D11CC..006D1200
    st(self, 0x10) = st(self, 0x28);                                                       // 006D1203..006D1215
    if (a != 0) std::memset(fp(self, 0x1C), 0, 12);                                        // 006D1217..006D124B
    if (b != 0) std::memset(fp(self, 0x10), 0, 12);                                        // 006D124E..006D127F
    kotor_fastcall_006d0bd0(self, nullptr, &input, h);                                     // 006D1282..006D128E
    *out = st(self, 0x28);                                                                 // 006D1293..006D12A0
    return out;
}
// 006D1030 CSWRK4Acceleration::SetVelocity(float y) (RET 4).
extern "C" void __fastcall kotor_fastcall_006d1030(void* self, void*, float y) {
    st(self, 0x10) = st(self, 0x28);                                                       // 006D1035..006D1044
    std::memset(fp(self, 0x34), 0, 12);                                                    // 006D1046..006D1072
    sw(self, 0x28, 0); sf(self, 0x2C, y); sw(self, 0x30, 0);                               // 006D1075..006D109B
}
