// render_func.cpp - lane-3d-emitter (fork F3): PartEmitter render dispatch and emitter-bucket entry.
//   0049B680 PartEmitter::RenderFunc (incl. LightningEmitter debug curves; 0049B950/0049BC20/0049C340/0049CA10
//            are loop labels inside 0049B680..0049CA9B, not separate functions)
//   00490700 PartEmitter::RenderNothing, 0048EF10 RenderBoundingSphere,
//   0048EFA0 StartEmitterLayer, 0048F000 StopEmitterLayer,
//   004509B0 Scene::DoEmitterBucket (Scene vtbl+110), 0044F5A0 EmitterOrderCmp (qsort comparator).
// Evidence: dumpbin /disasm of the pinned GOG swkotor.exe (lane d/*.txt); addresses cited per behaviour.
// Conventions: see emitter.h (D1 __fastcall transport, typed tables reloaded per call, D30-FINAL doubles).
#include "4ac9e30747-emitter.h"

// LOCAL-DECLS (merge into emitter.h): none - every callee is declared by emitter.h / scene3d.h.

namespace {
using namespace kotor_emitter;
using kotor_scene3d::cells;
namespace gl = kotor_renderer_init;

// .rdata constants read through memory operands (values from the pinned image).
constexpr U kGlKeep_0073f2c8 = 0x1E00;          // GL_KEEP
constexpr U kGlReplace_0073f2d0 = 0x1E01;       // GL_REPLACE
constexpr U kGlAlways_0073f2bc = 0x0207;        // GL_ALWAYS
constexpr U kGlVertexArray_0073f3a4 = 0x8074;
constexpr U kGlColorArray_0073f3a0 = 0x8076;
constexpr U kGlTexCoordArray_0073f398 = 0x8078;
constexpr U kGlNormalArray_0073f39c = 0x8075;
constexpr U kGlSrcAlpha_0073f270 = 0x0302;
constexpr U kGlOneMinusSrcAlpha_0073f274 = 0x0303;
constexpr float kZero_0073d700 = 0.0f;
constexpr float kOne_0073d764 = 1.0f;
constexpr float kThree_00740c84 = 3.0f;          // 40400000h
constexpr float kStep_007415bc = 0.05f;          // 3D4CCCCDh
constexpr float kTenth_0073e798 = 0.1f;          // 3DCCCCCDh
constexpr float kHundredth_0073dfec = 0.01f;     // 3C23D70Ah
constexpr U kGlLighting = 0x0B50;                // immediates (push 0B50h / 0DE1h)
constexpr U kGlTexture2D = 0x0DE1;

inline const Vector& vec_at(U base, I index) {     // CExoArrayList<Vector> element: base + index*12 (unchecked)
    return *static_cast<const Vector*>(as_object(base + static_cast<U>(index) * 12u));
}

// Gob stencil gate shared by the RenderFunc prologue (0049B68A..0049B6B1) / epilogue (0049CA53..0049CA72)
// and the chunk path (0049B77F..0049B7A7): gob [+40], its [+58] object, byte gob[+17C] or bit 1 of byte m[+50].
bool stencil_gate(void* emitter) {
    void* const gob = field<void*>(emitter, 0x40);
    if (gob == nullptr) return false;
    void* const m = field<void*>(gob, 0x58);
    if (m == nullptr) return false;
    if (field<U8>(gob, 0x17C) != 0) return true;
    return ((field<U8>(m, 0x50) >> 1) & 1u) != 0;
}
void stencil_restore() {                                                     // 0049CA74..0049CA91
    gl::glColorMask_0073d358(1, 1, 1, 1);
    gl::glStencilMask_0073d278(0);
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_stencil_test_0078d470);
}

// t = [+F8] / [+108] when [+108] != 0.0 (fcomp/test ah,44h/jnp: unequal or unordered), else 0
// (0049B838..0049B85F / 0049BB98..0049BBBF / 0049C296..0049C2BD).
float lightning_t(void* e) {
    float t = 0.0f;
    if (!(W(field<float>(e, 0x108)) == W(kZero_0073d700))) t = S(W(field<float>(e, 0xF8)) / W(field<float>(e, 0x108)));
    return t;
}
// glVertex3f(u*A[i] + t*C[i]) with both products wide (e.g. 0049B86B..0049B8C1, 0049BC20..0049BC6F).
void vertex_lerp(const Vector& a, const Vector& c, float t, float u) {
    gl::glVertex3f_0073d2c8(S(W(u) * W(a.x) + W(t) * W(c.x)), S(W(u) * W(a.y) + W(t) * W(c.y)),
                            S(W(u) * W(a.z) + W(t) * W(c.z)));
}

// One cubic-Bezier debug vertex of a LightningEmitter control strip (block 0049BD50..0049C1AE, repeated
// verbatim for branches at 0049C480..0049C8F6). Arrays of object e: A [+21C], C [+228], D [+24C], E [+240];
// segment end i, start i-1; parameter s. Wide/stored values follow the original fstp points exactly.
void bezier_vertex(void* e, I i, float t, float u, float s) {
    const Vector& a = vec_at(field<U>(e, 0x21C), i);  const Vector& a0 = vec_at(field<U>(e, 0x21C), i - 1);
    const Vector& c = vec_at(field<U>(e, 0x228), i);  const Vector& c0 = vec_at(field<U>(e, 0x228), i - 1);
    const Vector& d = vec_at(field<U>(e, 0x24C), i);  const Vector& d0 = vec_at(field<U>(e, 0x24C), i - 1);
    const Vector& ee = vec_at(field<U>(e, 0x240), i); const Vector& e0 = vec_at(field<U>(e, 0x240), i - 1);
    const double T = W(t), Uu = W(u), sw = W(s);
    // Bernstein weights (0049BD69..0049BDC3): B0 stored, B1..B3 stay wide.
    const double w = W(kOne_0073d764) - sw;
    const double w2 = w * w;
    const float b0 = S(w2 * w);                                               // fstp [28]
    const double b1 = w2 * sw * W(kThree_00740c84);
    const double b2 = w * sw * sw * W(kThree_00740c84);
    const double b3 = sw * sw * sw;
    // P3 = u*A[i] + t*C[i] (0049BDC7..0049BE20)
    const double qx = W(S(W(S(Uu * W(a.x))) + T * W(c.x)));                    // [F0]
    const double qy = W(S(Uu * W(a.y))) + T * W(c.y);                          // wide
    const double qz = W(S(W(S(Uu * W(a.z))) + W(S(T * W(c.z)))));             // [F8]
    const float t3x = S(qx * b3), t3y = S(qy * b3), t3z = S(qz * b3);          // [80] [84] [88]
    // R = u*E[i] + t*D[i] (0049BE52..0049BEB2)
    const float rx = S(Uu * W(ee.x) + T * W(d.x));                             // [15C]
    const double ry = W(S(Uu * W(ee.y))) + T * W(d.y);
    const double rz = W(S(Uu * W(ee.z))) + W(S(T * W(d.z)));
    // P2 offset D = P3' - R (0049BEB9..0049BF46)
    const float q2x = S(W(S(Uu * W(a.x))) + T * W(c.x));                       // [B4]
    const float q2y = S(W(S(Uu * W(a.y))) + T * W(c.y));                       // [B8]
    const double q2z = W(S(Uu * W(a.z))) + W(S(T * W(c.z)));
    const float dx = S(W(q2x) - W(rx));                                         // [108]
    const float dy = S(W(q2y) - ry);                                            // [10C]
    const double dz = q2z - rz;
    const float t2x = S(W(dx) * b2), t2y = S(W(dy) * b2), t2z = S(dz * b2);   // [68] [6C] [70]
    // P0 = u*A[i-1] + t*C[i-1] (0049BFAD..0049C017)
    const float p0x = S(W(S(Uu * W(a0.x))) + T * W(c0.x));                     // [44]
    const float p0y = S(W(S(Uu * W(a0.y))) + W(S(T * W(c0.y))));              // [48]
    const float p0z = S(W(S(Uu * W(a0.z))) + W(S(T * W(c0.z))));              // [4C]
    // P1 = P0 + u*E[i-1] + t*D[i-1] (0049BF6A..0049C067)
    const float hx = S((Uu * W(e0.x) + W(p0x)) + T * W(d0.x));                 // [114]
    const double hy = W(S(W(p0y) + W(S(Uu * W(e0.y))))) + T * W(d0.y);        // [E8] + t*d'.y wide
    const float hz = S(W(S(W(p0z) + W(S(Uu * W(e0.z))))) + W(S(T * W(d0.z)))); // [EC] + [140] -> [11C]
    const float t1x = S(W(hx) * b1), t1y = S(hy * b1);                         // [9C] [A0]
    const double t1z = W(hz) * b1;
    // P0 again, terms B0 (0049C094..0049C116)
    const float p0x2 = S(Uu * W(a0.x) + T * W(c0.x));                           // [168]
    const double p0y2 = W(S(Uu * W(a0.y))) + T * W(c0.y);
    const double p0z2 = W(S(Uu * W(a0.z))) + W(S(T * W(c0.z)));
    const float t0x = S(W(p0x2) * W(b0)), t0y = S(p0y2 * W(b0));               // [D8] [DC]
    const double t0z = p0z2 * W(b0);
    // Sum (0049C11A..0049C1AE)
    const float x = S((W(S(W(t0x) + W(t1x))) + W(t2x)) + W(t3x));
    const float y = S(W(S(W(S(W(t0y) + W(t1y))) + W(t2y))) + W(t3y));
    const float z = S(W(S((t0z + t1z) + W(t2z))) + W(t3z));
    gl::glVertex3f_0073d2c8(x, y, z);
}
// Bezier strip loop over segments 1..count-1 with s = 0, 0.05f, ... while the wide sum <= 1.0
// (0049BD40..0049C1EB / 0049C470..0049C938). Clamp: s = 1.0f unless s < 1 or s == 1 (0049BD50..0049BD61).
void bezier_strip(void* e, float t, float u) {
    for (I i = 1; i < field<I>(e, 0x220); ++i) {
        float s = 0.0f;
        for (;;) {
            if (W(s) > W(kOne_0073d764)) s = 1.0f;                               // test ah,41h / jne: keep when <, == or unordered
            bezier_vertex(e, i, t, u, s);
            const double next = W(s) + W(kStep_007415bc);                      // 0049C1B0..0049C1B4
            s = S(next);                                                        // fst [18]
            if (!(next <= W(kOne_0073d764))) break;                             // fcomp 1.0; jnp loop (< or ==)
        }
    }
}
// Gob::GetPosition on the target [+1E4]; the original discards the result (0049BD08..0049BD1C, 0049C42E..0049C447).
void target_position_unused(void* e) {
    void* const target = field<void*>(e, 0x1E4);
    Vector out;
    gob_table(target)->s064_gob_get_position(as_gob(target), nullptr, word_of(&out));
}
} // namespace

extern "C" {

// ================================================================ 0048EF10 PartEmitter::RenderBoundingSphere
void __fastcall part_emitter_render_bounding_sphere_0048ef10(void* emitter, void*) {
    Vector position;                                                            // [esp+4..C] = 0 (0048EF24..0048EF34)
    position.x = 0.0f; position.y = 0.0f; position.z = 0.0f;
    Quaternion orientation;                                                     // [esp+1C..28] = (1,0,0,0) (0048EF3C..0048EF54)
    orientation.w = 1.0f; orientation.x = 0.0f; orientation.y = 0.0f; orientation.z = 0.0f;
    part_table(emitter)->s00c(emitter, nullptr, word_of(&position), word_of(&orientation));   // 0048EF5C vtbl+0C GetWorld
    float color[3];
    color[0] = 1.0f; color[1] = 0.5f; color[2] = 1.0f;                          // 0048EF7A..0048EF8A
    gl_draw_sphere_bound_00431a50(field<float>(emitter, 0xD0), &position, color, 1.0f, 1);  // 0048EF5F..0048EF92
}

// ================================================================ 0048EFA0 PartEmitter::StartEmitterLayer
void __cdecl part_emitter_start_emitter_layer_0048efa0() {
    gl::glDepthMask_0073d28c(0);                                               // 0048EFA1
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_lighting_0078d45c);      // 0048EFA9..0048EFB5
    kotor_renderer_init::glEnable_0073d390(*cells.gl_blend_0078d448);          // 0048EFB7..0048EFBE
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);    // 0048EFC4..0048EFCB
    gl::glEnableClientState_0073d260(kGlVertexArray_0073f3a4);                 // 0048EFCD..0048EFD9
    gl::glEnableClientState_0073d260(kGlColorArray_0073f3a0);                  // 0048EFDB..0048EFE2
    gl::glEnableClientState_0073d260(kGlTexCoordArray_0073f398);               // 0048EFE4..0048EFEB
    gl::glDisableClientState_0073d264(kGlNormalArray_0073f39c);                // 0048EFED..0048EFF3
}

// ================================================================ 0048F000 PartEmitter::StopEmitterLayer
void __cdecl part_emitter_stop_emitter_layer_0048f000() {
    gl::glDepthMask_0073d28c(1);                                               // 0048F000
    kotor_renderer_init::glEnable_0073d390(*cells.gl_lighting_0078d45c);       // 0048F008..0048F00E
    gl::glBlendFunc_0073d38c(kGlSrcAlpha_0073f270, kGlOneMinusSrcAlpha_0073f274); // 0048F014..0048F022
    gl::glEnableClientState_0073d260(kGlNormalArray_0073f39c);                 // 0048F028..0048F02E
}

// ================================================================ 00490700 PartEmitter::RenderNothing
// Chunk emitters: positions/orients each particle's chunk Gob [p+74].
void __fastcall part_emitter_render_nothing_00490700(void* emitter, void*) {
    void* const gob = field<void*>(emitter, 0x40);                             // 00490709
    if (gob != nullptr) {
        void* const p = field<void*>(gob, 0x170);                              // 00490710
        if (p != nullptr && W(field<float>(p, 0x94)) < W(kOne_0073d764)) return;  // 0049071A..0049072B (C0 only -> jnp)
    }
    if (emitter_debug_spheres_00830498 != 0)                                   // 00490731..00490738
        part_emitter_render_bounding_sphere_0048ef10(emitter, nullptr);        // 0049073C
    for (I i = 0; i < field<I>(emitter, 0x1CC); ++i) {                         // 00490741..00490811 (count re-read)
        void* const particle = as_object(*static_cast<const U*>(as_object(field<U>(emitter, 0x1C8) + static_cast<U>(i) * 4u))); // 00490760..00490766
        void* const chunk = field<void*>(particle, 0x74);                      // 00490769
        if (chunk == nullptr) continue;                                        // 0049076E
        Vector moved;                                                          // [esp+14] (callee writes)
        const Vector position = field<Vector>(particle, 0x08);                  // 00490779..0049078B (12-byte copy)
        gob_table(chunk)->s05c_gob_set_position(as_gob(chunk), nullptr, &moved, position); // 0049078E..00490793 vtbl+5C
        if (W(field<float>(emitter, 0x88)) == W(kZero_0073d700)) continue;     // 00490796..004907A7 (jnp: equal)
        if (field<U8>(particle, 0x6C) != 0) continue;                          // 004907A9..004907AE
        Quaternion built;                                                      // [esp+20]
        const Vector axis = field<Vector>(particle, 0x48);                      // 004907B4..004907CD
        const Quaternion* const q = quaternion_construct_004ac960(&built, nullptr, axis, field<float>(particle, 0x14)); // 004907B0..004907D0
        const Quaternion value = *q;                                            // 004907D8..004907F2 (copy via EAX)
        void* const chunk2 = field<void*>(particle, 0x74);                     // 004907D5 / 004907FA (re-read)
        Quaternion result;                                                     // [esp+30]
        gob_table(chunk2)->s060_gob_set_orientation(as_gob(chunk2), nullptr, word_of(&result),
            float_to_bits(value.w), float_to_bits(value.x), float_to_bits(value.y), float_to_bits(value.z)); // 004907FD vtbl+60
    }
}

// ================================================================ 0049B680 PartEmitter::RenderFunc
void __fastcall part_emitter_render_func_0049b680(void* emitter, void*) {
    // Stencil prologue (0049B68A..0049B749).
    if (stencil_gate(emitter)) {
        kotor_renderer_init::glEnable_0073d390(*cells.gl_stencil_test_0078d470);   // 0049B6B3..0049B6BA
        gl::glColorMask_0073d358(0, 0, 0, 0);                                       // 0049B6C0..0049B6C8
        gl::glStencilOp_0073d270(kGlKeep_0073f2c8, kGlKeep_0073f2c8, kGlReplace_0073f2d0); // 0049B6CE..0049B6DC
        gl::glStencilMask_0073d278(1);                                              // 0049B6E2
        gl::glStencilFunc_0073d298(kGlAlways_0073f2bc, 1, 0xFF);                    // 0049B6EA..0049B6F7
    } else {
        stencil_restore();                                                          // 0049B6FF..0049B722 / 0049B726..0049B743
    }
    void* const node = aurora_node_as_emitter_0043e3c0(field<void*>(emitter, 4), nullptr);   // 0049B74F..0049B752
    if ((field<U8>(node, 0x12C) & 1u) != 0 && field<void*>(emitter, 0x1E4) == nullptr) {     // 0049B757..0049B768
        if (stencil_gate(emitter)) stencil_restore();                                         // -> 0049CA53
        return;
    }
    if (field<char>(node, 0xF0) != 0) {                                                       // 0049B76E (chunk name)
        part_emitter_render_nothing_00490700(emitter, nullptr);                               // 0049B77A
        if (stencil_gate(emitter)) stencil_restore();                                         // 0049B77F..0049B7C9
        return;
    }
    const U mode = field<U>(emitter, 0x114);                                                  // 0049B7D4
    if (mode == emitter_render_linked_00798b78) {                                             // 0049B7DA..0049B7E2
        void* const e = as_object(part_table(emitter)->s084(emitter, nullptr));               // 0049B7E8..0049B7ED vtbl+84
        if (e == nullptr) {                                                                   // 0049B7F5..0049B7F7
            part_emitter_render_linked_00495b20(emitter, nullptr);                            // 0049CA32..0049CA34
        } else {
            // ---- debug curve A (0049B7FD..0049BB87): green interpolated control polygon
            if (emitter_bss_0083048c != 0) {
                kotor_renderer_init::glDisable_0073d22c(kGlLighting);                         // 0049B810
                kotor_renderer_init::glDisable_0073d22c(kGlTexture2D);                        // 0049B817
                gl::glColor3f_0073d2ec(0.0f, 1.0f, 0.0f);                                     // 0049B81E..0049B827
                gl::glLineWidth_0073d380(2.5f);                                               // 0049B82D (40200000h)
                const float t = lightning_t(e);
                gl::glBegin_0073d2d0(1);                                                      // 0049B863
                const float u = S(W(kOne_0073d764) - W(t));                                   // 0049B86B..0049B884
                {
                    const Vector& a = vec_at(field<U>(e, 0x21C), 0); const Vector& c = vec_at(field<U>(e, 0x228), 0);
                    vertex_lerp(a, c, t, u);                                                  // 0049B888..0049B8C1
                    const Vector& d = vec_at(field<U>(e, 0x24C), 0); const Vector& ee = vec_at(field<U>(e, 0x240), 0);
                    gl::glVertex3f_0073d2c8(                                                  // 0049B8C3..0049B927
                        S((W(a.x) + W(ee.x)) * W(u) + (W(d.x) + W(c.x)) * W(t)),
                        S((W(d.y) + W(c.y)) * W(t) + (W(ee.y) + W(a.y)) * W(u)),
                        S((W(d.z) + W(c.z)) * W(t) + (W(ee.z) + W(a.z)) * W(u)));
                }
                for (I i = 1; i < field<I>(e, 0x220) - 1; ++i) {                              // 0049B929..0049BA6E
                    const Vector& a = vec_at(field<U>(e, 0x21C), i); const Vector& c = vec_at(field<U>(e, 0x228), i);
                    const Vector& d = vec_at(field<U>(e, 0x24C), i); const Vector& ee = vec_at(field<U>(e, 0x240), i);
                    gl::glVertex3f_0073d2c8(                                                  // 0049B950..0049B9E4
                        S((W(u) * W(a.x) + W(t) * W(c.x)) - (W(u) * W(ee.x) + W(t) * W(d.x))),
                        S((W(t) * W(c.y) + W(u) * W(a.y)) - (W(t) * W(d.y) + W(u) * W(ee.y))),
                        S((W(t) * W(c.z) + W(u) * W(a.z)) - (W(t) * W(d.z) + W(u) * W(ee.z))));
                    gl::glVertex3f_0073d2c8(                                                  // 0049B9E6..0049BA57
                        S((W(ee.x) + W(a.x)) * W(u) + (W(d.x) + W(c.x)) * W(t)),
                        S((W(ee.y) + W(a.y)) * W(u) + (W(d.y) + W(c.y)) * W(t)),
                        S((W(ee.z) + W(a.z)) * W(u) + (W(d.z) + W(c.z)) * W(t)));
                }
                {
                    const I n = field<I>(e, 0x220) - 1;                                       // 0049BA74..0049BA94
                    const Vector& a = vec_at(field<U>(e, 0x21C), n); const Vector& c = vec_at(field<U>(e, 0x228), n);
                    const Vector& d = vec_at(field<U>(e, 0x24C), n); const Vector& ee = vec_at(field<U>(e, 0x240), n);
                    gl::glVertex3f_0073d2c8(                                                  // 0049BA97..0049BB15
                        S((W(t) * W(c.x) + W(u) * W(a.x)) - (W(u) * W(ee.x) + W(t) * W(d.x))),
                        S((W(u) * W(a.y) + W(t) * W(c.y)) - (W(u) * W(ee.y) + W(t) * W(d.y))),
                        S((W(u) * W(a.z) + W(t) * W(c.z)) - (W(u) * W(ee.z) + W(t) * W(d.z))));
                    vertex_lerp(a, c, t, u);                                                  // 0049BB17..0049BB65
                }
                gl::glEnd_0073d2c4();                                                         // 0049BB67
                kotor_renderer_init::glEnable_0073d390(kGlTexture2D);                         // 0049BB73
                kotor_renderer_init::glEnable_0073d390(kGlLighting);                          // 0049BB7A
            }
            // ---- debug curve B (0049BB8B..0049C277): red points + cyan Bezier strip of this emitter
            if (emitter_bss_00830488 != 0) {
                const float t = lightning_t(e);                                               // 0049BB98..0049BBBF
                kotor_renderer_init::glDisable_0073d22c(kGlLighting);                         // 0049BBC3
                kotor_renderer_init::glDisable_0073d22c(kGlTexture2D);                        // 0049BBCA
                gl::glColor3f_0073d2ec(1.0f, 0.0f, 0.0f);                                     // 0049BBD1..0049BBDA
                gl::glPointSize_0073d384(10.0f);                                              // 0049BBE0 (41200000h)
                gl::glBegin_0073d2d0(0);                                                      // 0049BBEB
                if (field<I>(e, 0x220) > 0) {                                                 // 0049BBF3..0049BBFD
                    const float u = S(W(kOne_0073d764) - W(t));                               // 0049BC03..0049BC0F
                    for (I i = 0; i < field<I>(e, 0x220); ++i)                                // 0049BC20..0049BC7D
                        vertex_lerp(vec_at(field<U>(e, 0x21C), i), vec_at(field<U>(e, 0x228), i), t, u);
                }
                gl::glEnd_0073d2c4();                                                         // 0049BC85
                gl::glColor3f_0073d2ec(0.0f, 1.0f, 1.0f);                                     // 0049BC8B..0049BC97
                gl::glLineWidth_0073d380(1.0f);                                               // 0049BC9D
                gl::glBegin_0073d2d0(3);                                                      // 0049BCA8
                const float u = S(W(kOne_0073d764) - W(t));                                   // 0049BCB0..0049BCC9
                vertex_lerp(vec_at(field<U>(e, 0x21C), 0), vec_at(field<U>(e, 0x228), 0), t, u); // 0049BCCD..0049BD06
                target_position_unused(emitter);                                              // 0049BD08..0049BD1C (receiver's +1E4)
                bezier_strip(e, t, u);                                                        // 0049BD1F..0049C1EB
                {
                    const Vector& a = vec_at(field<U>(e, 0x21C), field<I>(e, 0x220) - 1);     // 0049C1F7..0049C20E
                    const Vector& c = vec_at(field<U>(e, 0x228), field<I>(e, 0x22C) - 1);     // 0049C211..0049C228
                    vertex_lerp(a, c, t, u);                                                  // 0049C22B..0049C25B
                }
                gl::glEnd_0073d2c4();                                                         // 0049C25D
                kotor_renderer_init::glEnable_0073d390(kGlTexture2D);                         // 0049C269
                kotor_renderer_init::glEnable_0073d390(kGlLighting);                          // 0049C270
            }
            // ---- debug curve C (0049C27B..0049C9E9): the same for every branch emitter [+258][j], j < [+288]
            if (emitter_bss_00830490 != 0 && field<I>(e, 0x288) > 0) {
                const float t = lightning_t(e);                                               // 0049C296..0049C2BD
                kotor_renderer_init::glDisable_0073d22c(kGlLighting);                         // 0049C2C1
                kotor_renderer_init::glDisable_0073d22c(kGlTexture2D);                        // 0049C2C8
                if (field<I>(e, 0x288) > 0) {                                                 // 0049C2CF..0049C2E2
                    const float u = S(W(kOne_0073d764) - W(t));                               // 0049C2E8..0049C2F8
                    for (I j = 0; j < field<I>(e, 0x288); ++j) {                              // 0049C300..0049C9C7
                        auto branch = [&]() { return as_object(*static_cast<const U*>(as_object(field<U>(e, 0x258) + static_cast<U>(j) * 4u))); };
                        gl::glColor3f_0073d2ec(1.0f, 0.0f, 0.0f);                             // 0049C302..0049C309
                        gl::glPointSize_0073d384(10.0f);                                      // 0049C30F
                        gl::glBegin_0073d2d0(0);                                              // 0049C31A
                        for (I i = 0; i < field<I>(branch(), 0x220); ++i)                     // 0049C321..0049C3AE (re-read)
                            vertex_lerp(vec_at(field<U>(branch(), 0x21C), i), vec_at(field<U>(branch(), 0x228), i), t, u);
                        gl::glEnd_0073d2c4();                                                 // 0049C3B0
                        gl::glColor3f_0073d2ec(0.0f, 1.0f, 1.0f);                             // 0049C3B6..0049C3C2
                        gl::glLineWidth_0073d380(1.0f);                                       // 0049C3C8
                        gl::glBegin_0073d2d0(3);                                              // 0049C3D3
                        vertex_lerp(vec_at(field<U>(branch(), 0x21C), 0), vec_at(field<U>(branch(), 0x228), 0), t, u); // 0049C3DB..0049C42C
                        target_position_unused(branch());                                     // 0049C42E..0049C447
                        bezier_strip(branch(), t, u);                                         // 0049C44A..0049C938
                        {
                            void* const b = branch();
                            const I n = field<I>(b, 0x220) - 1;                               // 0049C93E..0049C961 (same index for both)
                            vertex_lerp(vec_at(field<U>(b, 0x21C), n), vec_at(field<U>(b, 0x228), n), t, u); // 0049C964..0049C9A1
                        }
                        gl::glEnd_0073d2c4();                                                 // 0049C9A3
                    }
                }
                kotor_renderer_init::glEnable_0073d390(kGlTexture2D);                         // 0049C9DD
                kotor_renderer_init::glEnable_0073d390(kGlLighting);                          // 0049C9E4
            }
            // ---- linked rendering (0049C9EB..0049CA34)
            if (emitter_stencil_mode_00798b98 != 0) {
                for (I j = 0; j < field<I>(e, 0x288); ++j)                                    // 0049C9F4..0049CA27
                    part_emitter_render_linked_00495b20(
                        as_object(*static_cast<const U*>(as_object(field<U>(e, 0x258) + static_cast<U>(j) * 4u))), nullptr);
                if (emitter_stencil_mode_00798b98 != 0)                                       // 0049CA29..0049CA30 (re-read)
                    part_emitter_render_linked_00495b20(emitter, nullptr);                    // 0049CA32..0049CA34
            }
        }
    } else if (mode == emitter_render_motion_blur_00798b8c) {                                 // 0049CA3B..0049CA43
        part_emitter_render_motion_blurred_00490820(emitter, nullptr);                        // 0049CA45
    } else {
        part_emitter_render_standard_0048f040(emitter, nullptr);                              // 0049CA4C
    }
    if (stencil_gate(emitter)) stencil_restore();                                             // 0049CA53..0049CA91
}

// ================================================================ 0044F5A0 EmitterOrderCmp (qsort comparator)
int __cdecl emitter_order_cmp_0044f5a0(const void* pa, const void* pb) {
    void* const a = as_object(*static_cast<const U*>(pa));                     // 0044F5AF edi
    void* const b = as_object(*static_cast<const U*>(pb));                     // 0044F5AC esi
    void* const gob_a = field<void*>(a, 0x40);                                 // 0044F5B1
    void* const gob_b = field<void*>(b, 0x40);
    if (gob_a != nullptr && gob_b != nullptr && gob_a == gob_b) {              // 0044F5B4..0044F5C1
        void* const na = aurora_node_as_emitter_0043e3c0(field<void*>(a, 4), nullptr);   // 0044F5C3..0044F5C6
        void* const nb = aurora_node_as_emitter_0043e3c0(field<void*>(b, 4), nullptr);   // 0044F5CB..0044F5D0
        return static_cast<int>(field<std::int16_t>(na, 0x108)) - static_cast<int>(field<std::int16_t>(nb, 0x108)); // 0044F5D5..0044F5E4 (render order)
    }
    const I blend_a = field<I>(a, 0x4C), blend_b = field<I>(b, 0x4C);          // 0044F5EB / 0044F5F6
    if (blend_a == 2) { if (blend_b != 2) return 1; }                          // 0044F5F3..0044F601
    else if (blend_b == 2) return -1;                                          // 0044F60B..0044F615
    const Vector pos_a = gob_a != nullptr ? field<Vector>(gob_a, 0x78) : field<Vector>(a, 0x08);   // 0044F61D..0044F64D
    const Vector pos_b = field<void*>(b, 0x40) != nullptr ? field<Vector>(field<void*>(b, 0x40), 0x78) : field<Vector>(b, 0x08); // 0044F651..0044F684
    void* const camera = *cells.current_camera_007bb4e8;                       // 0044F688
    Vector da;
    da.z = S(W(field<float>(camera, 0x84)) - W(pos_a.z));                      // 0044F68D..0044F697
    da.y = S(W(field<float>(camera, 0x80)) - W(pos_a.y));
    da.x = S(W(field<float>(camera, 0x7C)) - W(pos_a.x));                      // 0044F6A6..0044F6B5
    const float mag_a = S(vector_magnitude_004ab100(&da));                     // 0044F6B9..0044F6C3 fstp [28]
    void* const camera2 = *cells.current_camera_007bb4e8;                      // 0044F6BE (re-read)
    Vector db;
    db.z = S(W(field<float>(camera2, 0x84)) - W(pos_b.z));
    db.y = S(W(field<float>(camera2, 0x80)) - W(pos_b.y));
    db.x = S(W(field<float>(camera2, 0x7C)) - W(pos_b.x));                     // 0044F6C7..0044F6EF
    const double mag_b = vector_magnitude_004ab100(&db);                       // 0044F6F3 (ST0, wide)
    const double d1 = W(mag_a) - mag_b;                                        // 0044F6F8..0044F6FF
    const float d2 = S(mag_b - W(mag_a));                                      // 0044F701..0044F707
    double diff = d1;
    if (!(d1 > W(d2))) diff = W(d2);                                           // 0044F70B..0044F718 (test ah,41h / je keeps d1)
    if (diff < W(kHundredth_0073dfec))                                         // 0044F71C..0044F727 (C0 only)
        return (word_of(b) < word_of(a)) ? -1 : 1;                             // 0044F729..0044F734 (cmp esi,edi; sbb; and -2; inc)
    return (W(mag_a) > mag_b) ? -1 : 1;                                        // 0044F73A..0044F74E (unordered -> 1)
}

// ================================================================ 004509B0 Scene::DoEmitterBucket (Scene vtbl+110)
U __fastcall scene_do_emitter_bucket_004509b0(void* scene, void*) {
    gl::glClearStencil_0073d36c(0);                                            // 004509B2..004509B6
    gl::glStencilMask_0073d278(0xFF);                                          // 004509BC..004509C1
    gl::glClear_0073d21c(0x400);                                               // 004509C7..004509CC (GL_STENCIL_BUFFER_BIT)
    const U enabled = emitter_bucket_enabled_0078e3d8;                         // 004509D2
    if (enabled == 0) return enabled;                                          // 004509D9 -> 00450AB9 (EAX = 0)
    kotor_renderer_init::glEnable_0073d390(*cells.gl_stencil_test_0078d470);   // 004509DF..004509E6
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_stencil_test_0078d470);  // 004509EC..004509F3
    part_emitter_start_emitter_layer_0048efa0();                               // 004509F9
    PointerList* const bucket = emitter_cells.emitter_parts_007fbf78;
    qsort_006fbe00(as_object(bucket->data), static_cast<U>(bucket->size), 4, &emitter_order_cmp_0044f5a0); // 004509FE..00450A12
    const bool animating = (*static_cast<const kotor_scene3d::SceneVTable* const volatile*>(scene))
                               ->s0a8_scene_is_animating(scene, nullptr) != 0;    // 00450A17..00450A24 (bl = al)
    if (*emitter_cells.gate_0078e614 != 0) {                                   // 00450A26..00450A2D
        for (I i = 0; i < bucket->size; ++i) {                                 // 00450A33..00450AAD (size re-read)
            void* const emitter = as_object(*static_cast<const U*>(as_object(bucket->data + static_cast<U>(i) * 4u))); // 00450A42..00450A47
            bool animate = animating;                                          // 00450A40 test bl,bl
            if (!animate) {
                void* const gob = field<void*>(emitter, 0x40);                 // 00450A4A
                animate = gob != nullptr && gob_table(gob)->s170_gob_is_force_animated(as_gob(gob), nullptr) != 0; // 00450A4F..00450A5D
            }
            if (animate) {
                // dt = frame time when it compares below 0.1f, else 0.1f (00450A5F..00450A85; test ah,5 / jp)
                const U frame_bits = *cells.frame_time_0078e574;
                const float dt = (W(F(frame_bits)) < W(kTenth_0073e798)) ? F(*cells.frame_time_0078e574) : kTenth_0073e798;
                part_emitter_animate_particles_00494da0(
                    as_object(*static_cast<const U*>(as_object(bucket->data + static_cast<U>(i) * 4u))), nullptr, dt); // 00450A89..00450A93
            }
            part_emitter_render_func_0049b680(
                as_object(*static_cast<const U*>(as_object(bucket->data + static_cast<U>(i) * 4u))), nullptr);         // 00450A98..00450AA0
        }
    }
    part_emitter_stop_emitter_layer_0048f000();                                // 00450AB4 tail jmp 0048F000
    return 0;   // EAX after the tail jump is whatever 0048F000 leaves (glEnableClientState); callers ignore it.
}

} // extern "C"
