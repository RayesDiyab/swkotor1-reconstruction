// Lane W4 G14: camera collision. 0063B050 CSWCameraOnAStick::Control_HitCheckCamera (full),
// 006422E0 CSWCModule::UpdateNormalCameraCollision (full, incl. the CAurBehaviorCameraFollow body 00642388..),
// 006441D0 CSWCModule::FindClearView (full), CHitInfo::Reset, CSWCCreature::HasGobModel,
// CSWCArea::ClearLineOfSightOneWay. Transcribed from dis/<ADDR>.asm.
#include "e103c18d18-camera_stick.hpp"
#include <cmath>

namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth {
void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations* list, void*, std::uint32_t count); // CExoArrayList::Allocate
}
extern "C" {
void* __fastcall aurora_hit_info_construct_004578f0(void* self, void*, std::uint32_t a, std::uint32_t b, std::uint32_t c); // CHitInfo::CHitInfo (RET 0Ch)
void __fastcall aurora_hit_info_clear_output_00456a60(void* self, void*);                   // CHitInfo::ClearOutput
// CServerExoApp::ShrinkLineByCreatures (004AEBB0 -> 004B2200 -> CSWSArea 004BF650). The two thunks are provided here;
// the 2000-byte CSWSArea body is cross-lane pending (server area creature geometry; see integration/camera.json).
std::int32_t __fastcall kotor_fastcall_004aebb0(void* server, void*, W4Vector* from, W4Vector* to, W4Vector* out);
std::int32_t __fastcall kotor_fastcall_004b2200(void* internal, void*, W4Vector* from, W4Vector* to, W4Vector* out);
std::int32_t __fastcall kotor_fastcall_004bf650(void* area, void*, W4Vector* from, W4Vector* to, W4Vector* out);
void* __fastcall server_internal_get_module_004b14f0(void* server_internal, void*);
void* __fastcall module_get_area_004c30f0(void* module, void*);
void __fastcall kotor_fastcall_00456af0(void* hit, void*, std::uint32_t a, std::uint32_t b, std::uint32_t c);   // CHitInfo::Reset
void __fastcall chit_info_destruct_00456b30(void* hit, void*);                              // CHitInfo::~CHitInfo
int __fastcall kotor_fastcall_006402f0(void* module, void*, float delta, W4Vector* out);      // GetNextCameraPosition RET8
std::uint32_t __fastcall kotor_fastcall_0063f5e0(void* creature, void*);                    // CSWCCreature::HasGobModel
std::uint32_t __fastcall kotor_fastcall_00605370(void* area, void*, W4Vector, W4Vector, std::uint32_t); // ClearLineOfSightOneWay (RET 1Ch)
std::uint32_t __fastcall kotor_fastcall_006441d0(void* module, void*, float delta, std::uint32_t* out_a, std::uint32_t* out_b);
}

namespace kotor_scene3d { struct Vector; }
namespace kotor_world_los { extern kotor_scene3d::Vector up_007a19b0; }

using namespace w4;
using namespace w4cam;

namespace w4cam { float probe_radius_007a241c = 0.35f; }   // .data 007A241C (also read by CSWBehaviorCameraCombat)

namespace {
const float k_zero = 0.0f, k_one = 1.0f;
const float k_probe_extra_0074a604 = 0.15000000596046448f;   // bits 3E19999A
const W4Vector& up_axis() { return *reinterpret_cast<const W4Vector*>(&kotor_world_los::up_007a19b0); }
W4Vector sub(const W4Vector& a, const W4Vector& b) {
    return W4Vector{static_cast<float>(static_cast<double>(a.x) - b.x), static_cast<float>(static_cast<double>(a.y) - b.y),
                    static_cast<float>(static_cast<double>(a.z) - b.z)};
}
// One scene probe: hit.ClearOutput(); hit+5Eh = 1; owner->vtbl[104h]() (scene); scene->vtbl[38h](from, to, &point, &hit).
bool probe(void* stick, U8* hit, const W4Vector& from, const W4Vector& to, W4Vector* point) {
    aurora_hit_info_clear_output_00456a60(hit, nullptr);
    hit[0x5E] = 1;
    void* scene = vcall<void*>(lp(stick, 0x10), 0x104);
    return vcall<U32, W4Vector, W4Vector, W4Vector*, void*>(scene, 0x38, from, to, point, hit) != 0;
}
}  // namespace

// 004AEBB0 CServerExoApp::ShrinkLineByCreatures (mov ecx,[ecx+4]; jmp 004B2200).
extern "C" I32 __fastcall kotor_fastcall_004aebb0(void* server, void*, W4Vector* from, W4Vector* to, W4Vector* out) {
    return kotor_fastcall_004b2200(lp(server, 4), nullptr, from, to, out);
}
// 004B2200 CServerExoAppInternal::ShrinkLineByCreatures (RET 0Ch): GetModule()->GetArea()->ShrinkLineByCreatures(...).
extern "C" I32 __fastcall kotor_fastcall_004b2200(void* internal, void*, W4Vector* from, W4Vector* to, W4Vector* out) {
    void* area = module_get_area_004c30f0(server_internal_get_module_004b14f0(internal, nullptr), nullptr); // 004B220F..004B2216
    return kotor_fastcall_004bf650(area, nullptr, from, to, out);                         // 004B221B..004B221D
}

// 00456AF0 CHitInfo::Reset(a, b, c) (thiscall, RET 0Ch).
extern "C" void __fastcall kotor_fastcall_00456af0(void* hit, void*, U32 a, U32 b, U32 c) {
    sw(hit, 0, a); sw(hit, 0x14, c); sw(hit, 4, b);                                        // 00456AF0..00456B03
    sw(hit, 0x0C, 0); sw(hit, 0x58, 0);                                                    // 00456B06..00456B09
    sb(hit, 0x5C, 0); sb(hit, 0x5D, 1); sb(hit, 0x5E, 0); sb(hit, 0x5F, 0); sb(hit, 0x60, 0); // 00456B0C..00456B19
    aurora_hit_info_clear_output_00456a60(hit, nullptr);                                   // 00456B1C
}

// 0063F5E0 CSWCCreature::HasGobModel() (thiscall, RET 0).
extern "C" U32 __fastcall kotor_fastcall_0063f5e0(void* creature, void*) {
    void* model = lp(creature, 0x68);                                                      // 0063F5E0
    if (model == nullptr) return 0;                                                        // 0063F5E3..0063F5E5
    return vcall<U32, U32>(model, 0x08, 0xFF) != 0 ? 1u : 0u;                              // 0063F5E7..0063F5FA
}

// 00605370 CSWCArea::ClearLineOfSightOneWay(...) (thiscall, RET 1Ch): always 1.
extern "C" U32 __fastcall kotor_fastcall_00605370(void*, void*, W4Vector, W4Vector, U32) { return 1; }

// 006441D0 CSWCModule::FindClearView(delta, out_a, out_b) (RET 0Ch), full body.
// Reached only while module+8Ch is set (0063FB95); default camera mode 3 never requests it.
namespace {
struct HitStorage {
    alignas(4) U8 bytes[0x64];
    bool armed;
    HitStorage() {
        aurora_hit_info_construct_004578f0(bytes, nullptr, 0, 0xBFFFFFFF, 0);
        armed = true;
    }
    ~HitStorage() {
        if (armed) chit_info_destruct_00456b30(bytes, nullptr);
    }
    void early() {
        armed = false; // state -1 before explicit cleanup; no later dtor obligation
        void* owner = *reinterpret_cast<void**>(bytes + 0x38);
        if (owner != nullptr)
            exo_array_list_remove_00445080(reinterpret_cast<U8*>(owner) + 4, nullptr, bytes + 0x38);
        void* data = *reinterpret_cast<void**>(bytes + 0x08);
        if (data != nullptr) live_free(data);
    }
    void normal() {
        armed = false;
        chit_info_destruct_00456b30(bytes, nullptr);
    }
};

bool trace(void* scene, const W4Vector& from, const W4Vector& to, W4Vector* point, void* hit) {
    sw(hit, 0, 1); // 00644378/644455/644526/6445C5: skip Scene::HitCheck's gob pass
    return vcall<U32, W4Vector, W4Vector, W4Vector*, void*>(scene, 0x38, from, to, point, hit) != 0;
}
}

// 006441D0 CSWCModule::FindClearView(float delta, U32* outA, U32* outB), RET 0Ch.
extern "C" U32 __fastcall kotor_fastcall_006441d0(void* module, void*, float delta, U32* outA, U32* outB) {
    HitStorage hs;
    void* camera = lp(module, 0x40);
    void* behavior = vcall<void*, I32>(camera, 0x80, -1); // camera unguarded, behavior checked
    void* stick = behavior != nullptr ? vcall<void*>(behavior, 0x1C) : nullptr;
    sw(module, 0x88, 1);
    sw(module, 0x84, 0);

    if (lp(module, 0x48) == nullptr) { hs.early(); return 0; }
    void* object = kotor_fastcall_005ed580(client_app(), nullptr, lw(module, 0x70));
    if (object == nullptr) { hs.early(); return 0; }
    void* client_object = vcall<void*>(object, 0x0C);
    if (client_object == nullptr) { hs.early(); return 0; }

    // +0x2C is tested, then called a second time; +0x1D0 applies to the second
    // returned pointer. False uses client_object+0x30. No caching/null guard.
    const void* first = vcall<void*>(client_object, 0x2C);
    const U8* position;
    if (first != nullptr) {
        // The second returned pointer is unguarded; a null second result still
        // becomes 0x1D0 (no fallback to client_object+0x30).
        position = reinterpret_cast<const U8*>(vcall<void*>(client_object, 0x2C)) + 0x1D0;
    } else {
        position = reinterpret_cast<const U8*>(client_object) + 0x30;
    }
    const double yaw = kotor_m15a_yaw_004aa0f0(reinterpret_cast<const W4Vector*>(position));
    if (stick != nullptr) sf(stick, 0x3C, static_cast<float>(yaw));

    W4Vector T;
    if (kotor_fastcall_006402f0(module, nullptr, delta, &T) == 0) { hs.normal(); return 0; }
    void* area = lp(module, 0x48);
    void* scene = lp(area, 0x184);
    if (scene == nullptr) { hs.normal(); return 0; }

    W4Vector O;
    vector_sum_00423b90(&O, reinterpret_cast<const W4Vector*>(reinterpret_cast<const U8*>(client_object) + 0x24),
                        reinterpret_cast<const W4Vector*>(reinterpret_cast<const U8*>(module) + 0x64));
    W4Vector P = {0, 0, 0};
    void* hit = hs.bytes;

    if (!trace(scene, T, O, &P, hit)) { hs.normal(); return 0; }

    auto clear = [&](float offset, bool subtract) -> U32 {
        const double base_yaw = vcall<double>(camera, 0x64);
        const float yaw_out = static_cast<float>(subtract ? base_yaw - static_cast<double>(offset)
                                                         : base_yaw + static_cast<double>(offset));
        const float pitch_out = static_cast<float>(vcall<double>(camera, 0x68));
        *reinterpret_cast<float*>(outA) = yaw_out;
        *reinterpret_cast<float*>(outB) = pitch_out;
        hs.normal();
        return 1;
    };

    kotor_fastcall_00456af0(hit, nullptr, 0, 0xBFFFFFFF, 0);
    const float dx = static_cast<float>(static_cast<double>(T.x) - static_cast<double>(O.x));
    const float dy = static_cast<float>(static_cast<double>(T.y) - static_cast<double>(O.y));
    const float ndx = -dx;
    const float ndy = -dy;
    const W4Vector f2 = {static_cast<float>(static_cast<double>(O.x) + static_cast<double>(ndx)),
                         static_cast<float>(static_cast<double>(O.y) + static_cast<double>(ndy)), T.z};
    if (!trace(scene, f2, O, &P, hit)) return clear(180.0f, false);

    kotor_fastcall_00456af0(hit, nullptr, 0, 0xBFFFFFFF, 0);
    const W4Vector f3 = {static_cast<float>(static_cast<double>(O.x) + static_cast<double>(ndy)),
                         static_cast<float>(static_cast<double>(O.y) + static_cast<double>(dx)), T.z};
    if (!trace(scene, f3, O, &P, hit)) return clear(90.0f, true);

    kotor_fastcall_00456af0(hit, nullptr, 0, 0xBFFFFFFF, 0);
    if (!trace(scene, T, O, &P, hit)) return clear(90.0f, false);

    hs.normal();
    return 0;
}

// 006422E0 CSWCModule::UpdateNormalCameraCollision(delta, zoom) (thiscall, RET 8; ST0).
// Only CAurBehaviorCameraFollow (vtable 00743924, type 3EDh, installed by Camera::SetFollow 0045C0C0) answers
// behaviour slot 1Ch with a non-null pointer; every other camera behaviour (OnAStick 106Ah, dialog 106Dh, combat,
// free-look, navigate, death) returns 0 there and leaves at 00642329. Static state 00833A68..00833A78 is private to
// this function (all original references are inside 006422E0).
namespace w4cam {
float normal_last_distance_00833a68;                                    // .bss 00833A68
float normal_cell_00833a6c, normal_cell_00833a70, normal_cell_00833a74; // .bss 00833A6C (function static Vector)
U32 normal_static_guard_00833a78;                                       // .bss 00833A78 (bit 0 = constructed)
extern U32 push_area_flag_00833a64;                                     // module_camera.obj (shared with 006433B0)
}
extern "C" {
W4CamQuaternion* __cdecl quaternion_slerp_004a9e00(W4CamQuaternion* out, W4CamQuaternion from,
                                                   const W4CamQuaternion* to, float t);       // slerp
W4CamQuaternion* __cdecl quaternion_normalize_004a9d60(W4CamQuaternion* out, W4CamQuaternion q); // normalize
}
namespace {
const float k_probe_step_0073d708 = 0.0010000000474974513f;   // bits 3A83126F
const float k_minus_half_00743884 = -0.5f;                    // bits BF000000
const float k_rate_0075183c = 750.0f;                         // bits 443B8000
const float k_pull_rate_007518bc = 3.5f;                      // bits 40600000
const float k_minus_eps_007518b8 = -9.999999747378752e-05f;   // bits B8D1B717
const float k_eps_00741870 = 9.999999747378752e-05f;          // bits 38D1B717
const double k_two_00741df8 = 2.0, k_half_0073f3d0 = 0.5;

struct NormalHit {                                            // CHitInfo [esp+C4h]
    alignas(4) U8 b[0x64];
    NormalHit() { aurora_hit_info_construct_004578f0(b, nullptr, 0, 0xBFFFFFFFu, 0); }   // 00642437..00642445
    ~NormalHit() { chit_info_destruct_00456b30(b, nullptr); }                            // 00643372..00643384
};

// fld v; fcomp hi; test ah,5; jp -> hi | v;  fld lo; fcomp st(1); test ah,41h; jne ...;  else v vs hi again.
float clamp_follow(float v, float lo, float hi) {
    const double candidate = jcc_jp(v, hi, 0x05) ? static_cast<double>(hi) : static_cast<double>(v);
    if (!jcc_jne(lo, candidate, 0x41)) return lo;             // 0064265F..0064266F / 006426B3..006426C4
    if (!jcc_jnp(v, hi, 0x05)) return hi;                     // 00642675..0064268C / 006426CA..006426E0
    return v;
}

// One collision trace: CHitInfo::Reset(0, BFFFFFFFh, 0); hit+0 = 1; then either
// !CSWCArea::ClearLineOfSightOneWay(from, to, out) ([00833A64] != 0) or Scene::HitCheck vtbl[38h](from, to, out, hit).
U32 normal_trace(void* module, void* scene, U8* hit, const W4Vector& from, const W4Vector& to, W4Vector* out) {
    kotor_fastcall_00456af0(hit, nullptr, 0, 0xBFFFFFFFu, 0);
    sw(hit, 0, 1);
    if (push_area_flag_00833a64 != 0)
        return kotor_fastcall_00605370(lp(module, 0x48), nullptr, from, to, ad(out)) == 0 ? 1u : 0u; // neg/sbb/inc
    return vcall<U32, W4Vector, W4Vector, W4Vector*, void*>(scene, 0x38, from, to, out, hit);
}

// H +/-= N * 0.001 as the original x87 sequences: x and y use the product stored as float, or (where noted) z/y the
// unrounded stack product.
struct Step { double x, y, z; };
Step probe_step(const W4Vector& n) {
    return Step{static_cast<double>(n.x) * k_probe_step_0073d708, static_cast<double>(n.y) * k_probe_step_0073d708,
                static_cast<double>(n.z) * k_probe_step_0073d708};
}
}  // namespace

extern "C" double __fastcall kotor_fastcall_006422e0(void* module, void*, float delta, float zoom) {
    void* behavior = vcall<void*, I32>(lp(module, 0x40), 0x80, -1);                        // 00642300..00642308
    void* follow = behavior ? vcall<void*>(behavior, 0x1C) : nullptr;                      // 00642310..0064231B [esp+7Ch]
    if (follow == nullptr) return k_zero;                                                  // 00642321..00642329 (fld 0.0)
    void* scene = lw(module, 0x48) != 0 ? lp(lp(module, 0x48), 0x184) : nullptr;          // 0064232E..00642341 [esp+20h]
    void* player = client_get_player_creature_005ed540(client_app(), nullptr);            // 00642345..0064234D
    if (collision_enabled_007a2468 == 0 || lw(module, 0x40) == 0 || scene == nullptr || player == nullptr ||
        kotor_fastcall_0063f5e0(player, nullptr) == 0)                                     // 00642354..00642382
        return zoom;                                                                       // 00643389 fld [esp+158h]

    // 00642388..006423B0: camera position and target = player position + module+64h (offset).
    W4Vector cam_tmp;
    const W4Vector cam = *vcall<W4Vector*, W4Vector*>(lp(module, 0x40), 0x20, &cam_tmp);   // [esp+10h]
    const W4Vector* const player_pos = reinterpret_cast<const W4Vector*>(static_cast<U8*>(player) + 0x24); // esi
    const W4Vector player_copy = *player_pos;                                              // [esp+38h]
    W4Vector sum_tmp;
    const W4Vector T = *vector_sum_00423b90(&sum_tmp, &player_copy,
                                            reinterpret_cast<const W4Vector*>(static_cast<U8*>(module) + 0x64)); // [esp+38h]
    const double ddz = static_cast<double>(T.z) - cam.z;                                   // 006423E5..006423E9
    const double ddy = static_cast<double>(T.y) - cam.y;                                   // 006423F8..006423FC
    const double ddx = static_cast<double>(T.x) - cam.x;                                   // 00642404..00642408
    const double len2 = ddx * ddx + ddy * ddy + ddz * ddz;                                 // 00642410..0064241E
    if (jcc_jne(len2, k_zero, 0x41)) return zoom;                                          // 00642420..00642431

    NormalHit hit;                                                                         // [esp+C4h], state 0
    W4Vector D = {static_cast<float>(ddx), static_cast<float>(ddy), static_cast<float>(ddz)}; // [esp+10h] (fst dwords)
    float ret = zoom;                                                                      // [esp+58h]
    W4Vector H1 = {0.0f, 0.0f, 0.0f};                                                      // [esp+28h]
    W4Vector H2 = {0.0f, 0.0f, 0.0f};                                                      // [esp+5Ch]
    float res = 0.0f;                                                                      // [esp+24h]
    U32 second_missed = 0;                                                                 // [esp+74h]
    U32 camera_valid = 1;                                                                  // [esp+78h]
    U32 pulled = 0;                                                                        // [esp+80h]
    W4Vector N;
    vector_normalized_004ab180(&N, &D);                                                    // 006424CD [esp+68h]
    W4Vector scaled;
    vector_component_mult_00423bf0(&scaled, &N, follow_7a2454);                            // 006424F7
    W4Vector F;
    kotor_cdecl_00423bc0(&F, &T, &scaled);                                                 // 0064250A [esp+44h] = T - N*max

    if ((normal_static_guard_00833a78 & 1u) == 0) {                                        // 0064251B..00642549
        normal_static_guard_00833a78 |= 1u;
        normal_cell_00833a6c = 0.0f; normal_cell_00833a70 = 0.0f; normal_cell_00833a74 = 0.0f;
    }
    W4CamQuaternion Q = {1.0f, 0.0f, 0.0f, 0.0f};                                          // 00642559..00642580 [esp+84h]
    if (!jcc_jp(normal_cell_00833a6c, k_zero, 0x44) && !jcc_jp(normal_cell_00833a70, k_zero, 0x44) &&
        !jcc_jp(normal_cell_00833a74, k_zero, 0x44)) {                                     // 00642553..006425B6
        normal_cell_00833a6c = player_pos->x;                                              // 006425B8..006425CD
        normal_cell_00833a70 = player_pos->y;
        normal_cell_00833a74 = player_pos->z;
    }

    // 006425D3..00642605: desired yaw/pitch.
    const double cam_yaw = vcall<double>(lp(module, 0x40), 0x64);
    const float yaw = static_cast<float>(cam_yaw + static_cast<double>(delta) * lf(module, 0x98)); // [esp+1Ch]
    const double cam_pitch = vcall<double>(lp(module, 0x40), 0x68);
    float pitch = static_cast<float>(cam_pitch + static_cast<double>(delta) * lf(module, 0x94)); // [esp+54h]
    // 00642609..00642638: dist = GetDist * 2^(delta*zoom*-0.5).
    const float factor = static_cast<float>(
        x87_pow(k_two_00741df8, static_cast<double>(delta) * zoom * k_minus_half_00743884)); // [esp+34h]
    float dist = static_cast<float>(vcall<double>(lp(module, 0x40), 0x6C) * factor);       // [esp+50h]
    pitch = clamp_follow(pitch, follow_7a2458, follow_pitch_007a245c);                      // 0064263C..0064268C
    dist = clamp_follow(dist, follow_7a2450, follow_7a2454);                               // 00642690..006426E0
    // 006426E4..00642710: t = 1 - 0.5^(delta*750).
    const float t = static_cast<float>(
        k_one - x87_pow(k_half_0073f3d0, static_cast<double>(delta) * k_rate_0075183c));    // [esp+34h]
    const W4Vector up = {0.0f, 0.0f, 1.0f};                                                // [esp+10h]
    W4Vector head;
    vector_sum_00423b90(&head, player_pos, &up);                                           // 0064272C
    W4Vector head_t;
    vector_component_mult_00423bf0(&head_t, &head, t);                                     // 00642757
    W4Vector cell_t;
    const W4Vector* const cell = reinterpret_cast<const W4Vector*>(&normal_cell_00833a6c);
    vector_component_mult_00423bf0(&cell_t, cell, static_cast<float>(k_one - static_cast<double>(t))); // 0064275C..0064277B
    W4Vector I;
    vector_sum_00423b90(&I, &cell_t, &head_t);                                             // 0064278C [esp+10h]
    W4CamQuaternion ypr;
    Q = *static_cast<W4CamQuaternion*>(yaw_pitch_roll_004acac0(&ypr, yaw, pitch, 0.0f));   // 006427B9..006427D4
    W4CamQuaternion cur_tmp;
    const W4CamQuaternion* cur = vcall<W4CamQuaternion*, W4CamQuaternion*>(lp(module, 0x40), 0x24, &cur_tmp); // 006427F5
    W4CamQuaternion slerped;
    W4CamQuaternion* s = quaternion_slerp_004a9e00(&slerped, *cur, &Q, t);                 // 00642824
    W4CamQuaternion normalized;
    const W4CamQuaternion NQ = *quaternion_normalize_004a9d60(&normalized, *s);            // 0064284C [esp+94h]
    W4Vector zdir;
    W4Vector* z = quaternion_zdir_0043e250(&NQ, nullptr, &zdir);                           // 0064288F
    W4Vector offset;
    W4Vector* o = vector_component_mult_00423bf0(&offset, z, dist);                        // 0064289D
    W4Vector P;
    P = *vector_sum_00423b90(&P, &I, o);                                                   // 006428B0 (esi, edi, ebp)
    normal_cell_00833a6c = I.x; normal_cell_00833a70 = I.y; normal_cell_00833a74 = I.z;     // 006428D6..006428E9

    const Step st = probe_step(N);
    W4Vector scratch;                                                                      // [esp+10h] scratch
    if (!jcc_jp(zoom, k_zero, 0x05)) {                                                     // 006428B5..006428EF zoom < 0
        // 006428F5..00642BF0: zooming in.
        if (normal_trace(module, scene, hit.b, P, F, &H1) != 0) {                          // 006428F5..00642998
            // 006429A9..006429FE: H1 += N*0.001 (x, y via float products; z unrounded).
            const float nx = static_cast<float>(st.x), ny = static_cast<float>(st.y);
            H1.x = static_cast<float>(static_cast<double>(nx) + H1.x);
            H1.y = static_cast<float>(static_cast<double>(H1.y) + ny);
            H1.z = static_cast<float>(st.z + H1.z);
            second_missed = normal_trace(module, scene, hit.b, H1, P, &H2) == 0;           // 00642A0E..00642AA4
        } else {
            second_missed = normal_trace(module, scene, hit.b, F, P, &H2) == 0;            // 00642AA9..00642B40
        }
        if (second_missed == 0) {
            // 00642B4B..00642BB1: H2 -= N*0.001 (x via float product; y, z unrounded).
            const float nx = static_cast<float>(st.x);
            H2.z = static_cast<float>(static_cast<double>(H2.z) - st.z);
            H2.y = static_cast<float>(static_cast<double>(H2.y) - st.y);
            H2.x = static_cast<float>(static_cast<double>(H2.x) - nx);
            scratch = W4Vector{static_cast<float>(static_cast<double>(H2.x) - T.x),
                               static_cast<float>(static_cast<double>(H2.y) - T.y),
                               static_cast<float>(static_cast<double>(H2.z) - T.z)};       // 00642BAD..00642BD6
            res = static_cast<float>(vector_magnitude_004ab100(&scratch));                 // 00642BDA..00642BDF
            normal_last_distance_00833a68 = 0.0f;                                          // 00642BE6
            goto lock_camera;                                                              // 00642BF0
        }
        second_missed = 1;                                                                 // 00642BF5
    }
    {
        // 00642BFD..: zoom >= 0 (or unordered), or the zoom-in second trace missed.
        const U32 not_zooming_out = jcc_je(zoom, k_zero, 0x41) ? 0u : 1u;                  // 00642BFD..00642C19 [esp+34h]
        U32 second;
        if (normal_trace(module, scene, hit.b, P, T, &H1) != 0) {                          // 00642C21..00642CC4
            // 00642CD5..00642D2A: H1 -= N*0.001 (x, y float products; z unrounded).
            const float nx = static_cast<float>(st.x), ny = static_cast<float>(st.y);
            H1.z = static_cast<float>(static_cast<double>(H1.z) - st.z);
            H1.y = static_cast<float>(static_cast<double>(H1.y) - ny);
            H1.x = static_cast<float>(static_cast<double>(H1.x) - nx);
            second = normal_trace(module, scene, hit.b, H1, P, &H2);                       // 00642D3A..00642DD0
        } else {
            second = normal_trace(module, scene, hit.b, T, P, &H2);                        // 00642DD5..00642E6C
        }
        const float last = normal_last_distance_00833a68;                                  // 00642E6F (loaded before the branch)
        if (second != 0) {
            if (!jcc_jp(last, k_zero, 0x44))                                               // 00642E85..00642E92
                normal_last_distance_00833a68 = static_cast<float>(vcall<double>(lp(module, 0x40), 0x6C));
            if (second_missed != 0) ret = 0.0f;                                            // 00642E98..00642EA0
            // 00642EA8..00642F02: H2 += N*0.001 (x via float product; y, z unrounded).
            const float nx = static_cast<float>(st.x);
            H2.z = static_cast<float>(static_cast<double>(H2.z) + st.z);
            H2.y = static_cast<float>(static_cast<double>(H2.y) + st.y);
            H2.x = static_cast<float>(static_cast<double>(nx) + H2.x);
            scratch = W4Vector{static_cast<float>(static_cast<double>(H2.x) - T.x),
                               static_cast<float>(static_cast<double>(H2.y) - T.y),
                               static_cast<float>(static_cast<double>(H2.z) - T.z)};       // 00642F0A..00642F33
            res = static_cast<float>(vector_magnitude_004ab100(&scratch));                 // 00642F37..00642F3C
        } else if (!jcc_jne(last, k_zero, 0x41)) {                                         // 00642F48..00642F4B last > 0
            // 00642F51..00642FA0: F = T - N*last.
            W4Vector n_last;
            W4Vector* nl = vector_component_mult_00423bf0(&n_last, &N, normal_last_distance_00833a68);
            W4Vector back;
            F = *kotor_cdecl_00423bc0(&back, &T, nl);
            kotor_fastcall_00456af0(hit.b, nullptr, 0, 0xBFFFFFFFu, 0);                   // 00642FA4
            sw(hit.b, 0, 1);                                                               // 00642FAC
            const double cur_dist = vcall<double>(lp(module, 0x40), 0x6C);                 // 00642FB9
            U32 third;
            const bool nearer = !jcc_jp(cur_dist, normal_last_distance_00833a68, 0x05);    // 00642FBC..00642FCC
            if (push_area_flag_00833a64 != 0) {
                third = nearer ? (kotor_fastcall_00605370(lp(module, 0x48), nullptr, P, F, ad(&H1)) == 0 ? 1u : 0u)
                               : (kotor_fastcall_00605370(lp(module, 0x48), nullptr, F, P, ad(&H1)) == 0 ? 1u : 0u);
            } else {
                third = nearer ? vcall<U32, W4Vector, W4Vector, W4Vector*, void*>(scene, 0x38, P, F, &H1, hit.b)
                               : vcall<U32, W4Vector, W4Vector, W4Vector*, void*>(scene, 0x38, F, P, &H1, hit.b);
            }                                                                              // 00642FD2..006430C9
            if (third != 0) {
                // 006430D7..00643131: H1 += N*0.001 (x, y float products; z unrounded).
                const float nx = static_cast<float>(st.x), ny = static_cast<float>(st.y);
                H1.z = static_cast<float>(st.z + H1.z);
                H1.y = static_cast<float>(static_cast<double>(H1.y) + ny);
                H1.x = static_cast<float>(static_cast<double>(nx) + H1.x);
                const float adjust = static_cast<float>(
                    (static_cast<double>(normal_last_distance_00833a68) - vcall<double>(lp(module, 0x40), 0x6C)) *
                    delta * k_pull_rate_007518bc);                                         // 0064313D..00643157
                res = static_cast<float>(vcall<double>(lp(module, 0x40), 0x6C) + adjust);  // 0064315B..00643162
                const double ex = static_cast<double>(H1.x) - T.x, ey = static_cast<double>(H1.y) - T.y,
                             ez = static_cast<double>(H1.z) - T.z;                         // 00643166..0064317A
                const double d2 = ex * ex + ey * ey + ez * ez;                             // 0064317E..0064318C
                const double r2 = static_cast<double>(res) * res;                          // 00643194..00643198
                if (!jcc_jne(r2, d2, 0x01)) res = static_cast<float>(x87_fsqrt(d2));       // 0064319C..006431A7
            } else {
                const float adjust = static_cast<float>(
                    (static_cast<double>(normal_last_distance_00833a68) - vcall<double>(lp(module, 0x40), 0x6C)) *
                    delta * k_pull_rate_007518bc);                                         // 006431B8..006431D2
                res = static_cast<float>(vcall<double>(lp(module, 0x40), 0x6C) + adjust); // 006431D6..006431E8
                pulled = 1;                                                                // 006431DD
                if (!jcc_jne(static_cast<double>(res) - normal_last_distance_00833a68, k_minus_eps_007518b8, 0x01)) {
                    res = normal_last_distance_00833a68;                                   // 00643203..00643209
                    normal_last_distance_00833a68 = 0.0f;                                  // 0064320D
                }
            }
        }
        // 0064321B..006432A0.
        if (not_zooming_out == 0) normal_last_distance_00833a68 = 0.0f;                    // 0064321B..00643223
        bool clamp_to_min;
        if (!jcc_jp(res, k_zero, 0x44) &&
            jcc_jnp(vcall<double>(lp(module, 0x40), 0x6C), follow_7a2450, 0x05)) {         // 0064322D..00643251
            clamp_to_min = true;
        } else {
            clamp_to_min = !jcc_jne(res, k_zero, 0x41) &&
                           !jcc_jne(static_cast<double>(follow_7a2450) - res, k_eps_00741870, 0x01); // 00643253..00643279
        }
        if (clamp_to_min) {
            camera_valid = 0;                                                              // 00643281
            if (!jcc_jp(normal_last_distance_00833a68, follow_7a2450, 0x05))               // 0064327B..00643294
                normal_last_distance_00833a68 = follow_7a2450;                             // 00643296..0064329B
            ret = 0.0f;                                                                    // 006432A0
        }
    }
lock_camera:
    if (kotor_cdecl_0060b2a0(nullptr) == 0) return zoom;                                   // 006432A8..006432B4 -> 00643372
    sf(follow, 0x64, camera_valid != 0 ? k_one : k_zero);                                  // 006432BA..006432CF
    if (!jcc_jne(res, k_zero, 0x41)) {                                                     // 006432D6..006432E5 res > 0
        sf(follow, 0x30, res);                                                             // 006432F0..006432F4
        if (pulled == 0) ret = 0.0f;                                                       // 006432E7..006432F9
        sw(module, 0x80, 1);                                                               // 00643301
    }
    if (!jcc_jp(vcall<double>(lp(module, 0x40), 0x6C), follow_7a2450, 0x41) &&
        !jcc_jne(ret, k_zero, 0x41))                                                       // 0064330B..0064332F
        ret = 0.0f;                                                                        // 00643331
    if (!jcc_jne(vcall<double>(lp(module, 0x40), 0x6C), follow_7a2454, 0x01) &&
        !jcc_jp(ret, k_zero, 0x05))                                                        // 00643339..0064335D
        ret = 0.0f;                                                                        // 0064335F
    return ret;                                                                            // 00643367..00643389
}

// 0063B050 CSWCameraOnAStick::Control_HitCheckCamera(Vector* out, Vector position) (thiscall, RET 10h).
extern "C" W4Vector* __fastcall kotor_fastcall_0063b050(void* s, void*, W4Vector* out, W4Vector position) {
    W4Vector result = position;                                                            // [esp+28h] 0063B08D..0063B0A2
    W4Vector hit1 = {0, 0, 0}, hit2 = {0, 0, 0}, hit3 = {0, 0, 0}, hit4 = {0, 0, 0};       // 0063B0A6..0063B0FE
    alignas(4) U8 hit[0x64];                                                               // CHitInfo [esp+188h]
    aurora_hit_info_construct_004578f0(hit, nullptr, 0, 0xBFFFFFFFu, 0);                   // 0063B086..0063B106
    {   // 0063B10B..0063B161: hit.ignore list (+08 data, +0C count, +10 capacity) += target
        void* target = lp(s, 0x14);
        if (li(hit, 0x0C) == li(hit, 0x10)) {
            const I32 capacity = li(hit, 0x10);
            menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(hit + 8), nullptr,
                                                static_cast<U32>(capacity != 0 ? capacity * 2 : 0x10));
        }
        sw(lp(hit, 8), lw(hit, 0x0C) * 4u, ad(target));
        sw(hit, 0x0C, lw(hit, 0x0C) + 1);
    }
    sb(hit, 0x5C, 1);                                                                      // 0063B12C
    for (I32 pass = 1; pass-- > 0;) {                                                      // 0063B127/0063B168..0063B174 (runs once)
        aurora_hit_info_clear_output_00456a60(hit, nullptr);                               // 0063B17A..0063B181
        W4Vector head;
        vcall<W4Vector*, W4Vector*>(lp(s, 0x14), 0x64, &head);                             // 0063B186..0063B193
        head.z = static_cast<float>(static_cast<double>(head.z) + lf(s, 0x6C));            // 0063B196..0063B1B7
        W4Vector shrunk = {0, 0, 0};                                                       // [esp+C0h]
        if (kotor_fastcall_004aebb0(lp(app_manager(), 8), nullptr, &head, &position, &shrunk) != 0) { // 0063B1B2..0063B20D
            position = shrunk;                                                             // 0063B20F..0063B232
            result = shrunk;                                                               // 0063B239..0063B241
        }
        const W4Vector to_position = sub(position, head);                                  // 0063B245..0063B289 [esp+ECh]
        const W4Vector to_head = sub(head, position);                                      // 0063B290..0063B2BF [esp+128h]
        W4Vector cross, side;
        vector_cross_004ab210(&cross, &up_axis(), &to_head);                               // 0063B2C6
        vector_normalized_004ab180(&side, &cross);                                         // 0063B2CB..0063B2D4
        const float oz = static_cast<float>(static_cast<double>(side.z) * probe_radius_007a241c); // 0063B2D9..0063B2F0
        const float oy = static_cast<float>(static_cast<double>(side.y) * probe_radius_007a241c); // 0063B2F4..0063B301
        const float ox = static_cast<float>(static_cast<double>(side.x) * probe_radius_007a241c); // 0063B305..0063B312
        const W4Vector p1 = {static_cast<float>(static_cast<double>(ox) + position.x), static_cast<float>(static_cast<double>(position.y) + oy),
                             static_cast<float>(static_cast<double>(position.z) + oz)};  // 0063B319..0063B353
        const bool r1 = probe(s, hit, head, p1, &hit1);                                    // 0063B35A..0063B3C5
        const W4Vector p2 = {static_cast<float>(static_cast<double>(position.x) - ox), static_cast<float>(static_cast<double>(position.y) - oy),
                             static_cast<float>(static_cast<double>(position.z) - oz)};  // 0063B3C7..0063B40A
        const bool r2 = probe(s, hit, head, p2, &hit2);                                    // 0063B411..0063B49D
        const float up_r = probe_radius_007a241c;                                          // 0063B484 [esp+78h]
        const float zero_r = static_cast<float>(static_cast<double>(probe_radius_007a241c) * k_zero); // 0063B478..0063B496
        const W4Vector p3 = {static_cast<float>(static_cast<double>(zero_r) + position.x), static_cast<float>(static_cast<double>(position.y) + zero_r),
                             static_cast<float>(static_cast<double>(position.z) + up_r)}; // 0063B49F..0063B4E2
        const bool r3 = probe(s, hit, head, p3, &hit3);                                    // 0063B4E9..0063B55F
        const W4Vector p4 = {static_cast<float>(static_cast<double>(position.x) - zero_r), static_cast<float>(static_cast<double>(position.y) - zero_r),
                             static_cast<float>(static_cast<double>(position.z) - up_r)}; // 0063B550..0063B59C



        const bool r4 = probe(s, hit, head, p4, &hit4);                                    // 0063B5A3..0063B610

        if (!r1 && !r2 && !r3 && !r4) break;                                               // 0063B612..0063B620
        const float length = static_cast<float>(vector_magnitude_004ab100(&to_position));  // 0063B626..0063B633
        float best = 0.0f;                                                                 // [esp+10h]
        struct Probe { bool hit; const W4Vector* point; const W4Vector* end; };
        const Probe probes[4] = {{r1, &hit1, &p1}, {r2, &hit2, &p2}, {r3, &hit3, &p3}, {r4, &hit4, &p4}};
        for (int i = 0; i < 4; ++i) {                                                      // 0063B63D..0063B8A6
            if (!probes[i].hit) continue;
            const W4Vector e = sub(*probes[i].point, *probes[i].end);
            const double m = vector_magnitude_004ab100(&e);
            const float bound = i == 0 ? k_zero : best;                                    // first compares 0.0 (0063B68A)
            if (jcc_jnp(m, bound, 0x05)) continue;                                         // m < bound -> keep
            best = static_cast<float>(vector_magnitude_004ab100(&e));                      // recomputed (0063B6D2/0063B769/...)
        }
        W4Vector n;
        vector_normalized_004ab180(&n, &to_position);                                      // 0063B8A9..0063B8B9
        const W4Vector back = {static_cast<float>(static_cast<double>(best) * n.x), static_cast<float>(static_cast<double>(best) * n.y),
                               static_cast<float>(static_cast<double>(best) * n.z)};       // 0063B8BE..0063B8EF
        vector_subtract_assign_004a9770(&result, nullptr, &back);                          // 0063B8F6
        W4CamQuaternion q;
        vcall<W4CamQuaternion*, W4CamQuaternion*>(lp(s, 0x14), 0x68, &q);                        // 0063B8FB..0063B90C
        // 0063B90F..0063B967: z axis of the target orientation {2(zx+yw), 2(zy-xw), 1-2(x^2+y^2)}.
        const W4Vector z = {static_cast<float>((static_cast<double>(q.y) * q.w + static_cast<double>(q.z) * q.x) * 2.0),
                            static_cast<float>((static_cast<double>(q.z) * q.y - static_cast<double>(q.x) * q.w) * 2.0),
                            static_cast<float>(k_one - (static_cast<double>(q.x) * q.x + static_cast<double>(q.y) * q.y) * 2.0)};
        const float k = static_cast<float>((static_cast<double>(best) / length) *
                                           (static_cast<double>(probe_radius_007a241c) + k_probe_extra_0074a604)); // 0063B96E..0063B987
        W4Vector zn;
        vector_normalized_004ab180(&zn, &z);                                               // 0063B98B
        W4Vector lift = {static_cast<float>(static_cast<double>(k) * zn.x), static_cast<float>(static_cast<double>(k) * zn.y),
                         static_cast<float>(static_cast<double>(k) * zn.z)};               // 0063B990..0063B9EB
        vector_add_assign_004a9740(&result, nullptr, &lift);                               // 0063B9F2
    }
    *out = result;                                                                         // 0063BA07..0063BA21
    if (void* list = lp(hit, 0x38)) exo_array_list_remove_00445080(static_cast<U8*>(list) + 4, nullptr, hit + 0x38); // 0063BA24..0063BA45
    if (void* data = lp(hit, 8)) live_free(data);                                          // 0063BA4A..0063BA5B
    return out;
}
