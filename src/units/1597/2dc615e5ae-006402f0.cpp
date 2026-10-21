#include "4644e3d982-camera_stick.hpp"
#include <cmath>

using namespace w4;
using namespace w4cam;

extern "C" {
W4CamQuaternion* __cdecl quaternion_slerp_004a9e00(W4CamQuaternion* out, W4CamQuaternion from,
                                                   const W4CamQuaternion* to, float t);
W4CamQuaternion* __cdecl quaternion_normalize_004a9d60(W4CamQuaternion* out, W4CamQuaternion q);
}

namespace {
float clamp_ordered_upper(float v, float lo, float hi) {
    // Original: min(v,hi) with unordered -> hi, then max(...,lo), then a
    // redundant min(v,hi) with unordered -> hi. NaN therefore ends at hi.
    if (!(v <= hi)) v = hi;
    if (v < lo) v = lo;
    if (!(v <= hi)) v = hi;
    return v;
}
}

// 006402F0 CSWCModule::GetNextCameraPosition(float delta, Vector* out), RET 8.
extern "C" int __fastcall kotor_fastcall_006402f0(void* module, void*, float delta, W4Vector* out) {
    void* camera = lp(module, 0x40);
    if (camera == nullptr) return 0;

    void* object = kotor_fastcall_005ed580(client_app(), nullptr, lw(module, 0x70));
    if (object == nullptr) return 0;
    void* client_object = vcall<void*>(object, 0x0C);
    if (client_object == nullptr) return 0;
    void* model = vcall<void*, U32, U32>(client_object, 0x98, 0xFF, 1);
    if (model == nullptr) return 0;

    // yaw/pitch/zoom are float stores; the exponent pow is x87 PC53.
    const float yaw = static_cast<float>(vcall<double>(camera, 0x64) +
                                         static_cast<double>(delta) * static_cast<double>(lf(module, 0x98)));
    const float pitch0 = static_cast<float>(vcall<double>(camera, 0x68) +
                                            static_cast<double>(delta) * static_cast<double>(lf(module, 0x94)));
    const float zoom_dist = static_cast<float>(vcall<double>(camera, 0x6C));
    const float zoom_pow = static_cast<float>(w4cam::x87_pow(2.0, static_cast<double>(delta) *
                                                                   static_cast<double>(lf(module, 0x9C)) * -0.5));
    const float zoom0 = static_cast<float>(static_cast<double>(zoom_dist) * static_cast<double>(zoom_pow));
    const float pitch = clamp_ordered_upper(pitch0, 1.0f, 50.0f);
    const float zoom = clamp_ordered_upper(zoom0, 5.0f, 20.0f);

    const float t = static_cast<float>(1.0 - w4cam::x87_pow(0.5, static_cast<double>(delta) * 750.0));

    W4Vector model_pos;
    vcall<W4Vector*, W4Vector*>(model, 0x64, &model_pos);
    const W4Vector target = {
        static_cast<float>(static_cast<double>(model_pos.x) + lf(module, 0x64)),
        static_cast<float>(static_cast<double>(model_pos.y) + lf(module, 0x68)),
        static_cast<float>(static_cast<double>(model_pos.z) + lf(module, 0x6C))};

    // Per-component (1-t)*old + t*target, with old at module+0x58..0x60.
    // 1-t and the products stay in PC53; only component results store to float.
    const double one_minus_t = 1.0 - static_cast<double>(t);
    const W4Vector base = {
        static_cast<float>(one_minus_t * static_cast<double>(lf(module, 0x58)) + static_cast<double>(t) * target.x),
        static_cast<float>(one_minus_t * static_cast<double>(lf(module, 0x5C)) + static_cast<double>(t) * target.y),
        static_cast<float>(one_minus_t * static_cast<double>(lf(module, 0x60)) + static_cast<double>(t) * target.z)};
    sf(module, 0x58, base.x);
    sf(module, 0x5C, base.y);
    sf(module, 0x60, base.z);

    W4CamQuaternion desired;
    yaw_pitch_roll_004acac0(&desired, yaw, pitch, 0.0f);
    W4CamQuaternion current;
    W4CamQuaternion* got = vcall<W4CamQuaternion*, W4CamQuaternion*>(camera, 0x24, &current);
    (void)got; // returns &current; selected getter RET4

    W4CamQuaternion slerped;
    quaternion_slerp_004a9e00(&slerped, current, &desired, t);
    quaternion_normalize_004a9d60(&slerped, slerped);
    W4Vector zdir;
    quaternion_zdir_0043e250(&slerped, nullptr, &zdir);

    *out = W4Vector{
        static_cast<float>(static_cast<double>(base.x) + static_cast<double>(zdir.x) * zoom),
        static_cast<float>(static_cast<double>(base.y) + static_cast<double>(zdir.y) * zoom),
        static_cast<float>(static_cast<double>(base.z) + static_cast<double>(zdir.z) * zoom)};
    return 1;
}
