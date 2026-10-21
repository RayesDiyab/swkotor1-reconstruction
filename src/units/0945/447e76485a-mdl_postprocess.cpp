// mdl_postprocess.cpp - lane-m48-asciimdl fork D: ASCII-MDL mesh post-processing of the GOG swkotor.exe (image base
// 0x400000), reached only from MdlNode::PostProcessDispatch 0043FB90 (ParseNode 004680E0, ASCII models; the menu path
// loads binary MDL). Listings: lane-m48-asciimdl/d/<ADDR>.txt (local only).
//   004388A0 MdlNodeTriMesh::ComputeLocalSurfaceArea   thiscall RET 8
//   0043A120 MdlNodeTriMesh::ProcessVerts              thiscall RET 0
//   0043B380 MdlNodeLightsaber::InternalPostProcess    thiscall RET 0
//   0043CF00 MdlNodeTriMesh::InternalPostProcess       thiscall RET 0
//   0043D990 MdlNodeDanglyMesh::InternalPostProcess    thiscall RET 0
// Conventions: asciimdl.hpp (D1 __fastcall transport; raw observed-offset storage via model_load.hpp get/put).
// Node+134 is the MdlNodeTriMesh_PostProcess temporaries object ("post"; layout per f3_mdl_node.cpp + cited reads):
// post+00 position Vector data (count post+04), +0C tex0 Vector data, +3C colour Vector data, +48 face records
// (0x14 stride, count +4C), +54/+58/+5C normal Vector list, +60 vertex-record pointer data, +64 vertex count word.
// Floats (D30-FINAL): vertex/vector components are moved as raw DWORDs where the original uses mov; x87 arithmetic is
// mirrored with double intermediates and float only at the original fstp dword stores; _ftol 006FAE8C through the
// original-ABI port unresolved_ftol2_006fae8c (ST0 -> EDX:EAX, truncation) with MSVC inline asm (D21; same helper
// shape as lane-m48-a normal_map.cpp). fld dword / fstp dword float->float copies (min/max 0043A686..0043A715) are
// value-preserving except that x87 quiets an SNaN (recorded; reproduced as float copies).
// EH: 0043CF00 state 0 (0043D0B0..0043D0D8) only frees the 0x4C block if Material::Material throws; the Material
// donor body is the nonthrowing proposal (D71), so the funclet (00715A5B) is cited, not emitted (D30-FINAL rule).
#include "d51e09c5f8-asciimdl.hpp"
#include "1202076b9b-material.h"          // material_owner::Material (texture-epoch/include; 0047B560 / 0047B980 members)
#include "21ca817b3e-storage.h"     // historical_storage_007a8a70/7c/88 (owner model46-m46-model-storage)

using namespace aurora_model;

extern "C" {
// ---- existing providers (M47 objects; same extern "C" decorated names as the cited declarations)
float* __cdecl vector_cross_004ab210(float* out, const float* p, const float* q);         // 3d-render-scene3d (scene3d.h:349)
double __cdecl vector_magnitude_004ab100(const float* v);                                   // 3d-render-scene3d (ST0)
void __fastcall vector_normalize_004ab130(float* self, void* unused_edx);                   // 3d-model-model-skin (RET 0)
float* __fastcall vector_add_assign_004a9740(float* self, void* unused_edx, const float* v); // 3d-model-model-skin (RET 4)
void __fastcall vector_list_allocate_00421310(List* list, void* unused_edx, I size);        // font-info-parse (RET 4)
void __fastcall aurora_trimesh_build_neighbor_lists_004386b0(MdlNode* self, void* unused_edx);       // 3d-model-model-postprocess
void __fastcall aurora_trimesh_build_vertex_indices_0043c400(MdlNode* self, void* unused_edx);       // 3d-model-model-indices
void __fastcall aurora_trimesh_calculate_tangent_space_0043a820(MdlNode* self, void* unused_edx);    // 3d-model-model-tangent
void* __fastcall aurora_material_construct_0047b290(void* self, void* unused_edx);                    // 3d-model-model-material
std::uint8_t __fastcall aurora_material_has_bump_map_0047b8f0(void* material, void* unused_edx);      // lane-m48-a trimesh_f1
std::uint8_t __fastcall texture_controller_is_linear_0041f250(void*, void*);                          // texture-epoch-controller_base (0041F250: xor al,al; ret)
unsigned __int64 __cdecl unresolved_ftol2_006fae8c();                                                 // M40 _ftol port (custom x87 ABI)

// ---- authored here, cluster-internal (called only by 0043CF00 at 0043D42F)
void __fastcall mdl_trimesh_compute_local_surface_area_004388a0(MdlNode* self, void* unused_edx, List* faces, float* area);
}

namespace {
constexpr float kZero_0073d700 = 0.0f;     // .rdata 0073D700
constexpr float kOne_0073d764 = 1.0f;      // .rdata 0073D764
constexpr float kHalf_0073e9ac = 0.5f;     // .rdata 0073E9AC
constexpr float k255_0073ede4 = 255.0f;    // .rdata 0073EDE4
constexpr U kFar_4b189680 = 0x4B189680u;   // immediate 1.0e7f (0043D3AC / 0043DDA0)
constexpr char kNull_0073ee04[5] = {'N', 'U', 'L', 'L', '\0'};   // .rdata 0073EE04 (dword + byte copy)
constexpr U16 kSaberWord48_00740a48 = 0x14;   // .rdata 00740A48 (read from the image; movsx / word arithmetic)

void* vp(U a) { return ptr<void>(a); }
void* post(MdlNode* n) { return ptr<void>(get<U>(n, 0x134)); }                       // [esi+134h]
U face_at(MdlNode* n, I i) { return get<U>(post(n), 0x48) + static_cast<U>(i) * 0x14u; }   // post+48, 0x14 stride
List* scratch(CleanupStorage12& s) { return reinterpret_cast<List*>(&s); }           // {data,size,cap} views of 007A8A70/7C/88
MaxTree* model_of(MdlNode* n) { return aurora_tree_as_model_0043e1c0(ptr<MaxTree>(get<U>(n, 0x08)), nullptr); }

// _ftol 006FAE8C on a wide value; callers keep EAX.
I ftol_i32(double value) {
    I result;
    __asm {
        fld qword ptr value
        call unresolved_ftol2_006fae8c
        mov result, eax
    }
    return result;
}
void copy3(U dst, U src) {                         // three raw DWORD moves
    put<U>(vp(dst), 0, get<U>(vp(src), 0));
    put<U>(vp(dst), 4, get<U>(vp(src), 4));
    put<U>(vp(dst), 8, get<U>(vp(src), 8));
}
// Inline CExoArrayList<Vector>::Add (0043A188..0043A1CD, 0043A5D3..0043A60C): grow 2x (16 when empty) via 00421310.
void vector_list_add(List* l, U x, U y, U z) {
    if (l->size == l->capacity)
        vector_list_allocate_00421310(l, nullptr, l->capacity != 0 ? l->capacity * 2 : 0x10);
    const U slot = l->data + static_cast<U>(l->size) * 12u;
    put<U>(vp(slot), 0, x); put<U>(vp(slot), 4, y); put<U>(vp(slot), 8, z);
    l->size = l->size + 1;
}
} // namespace

extern "C" {
// ================================================================ 004388A0 MdlNodeTriMesh::ComputeLocalSurfaceArea
// thiscall(faces, area) RET 8: *area = sum over face records (0x20 stride; vertex index words +1A/+1C/+1E) of
// |(v1 - v0) x (v2 - v0)| * 0.5, positions read from the locked vertex pool at node+194 + index * node+14C.
void __fastcall mdl_trimesh_compute_local_surface_area_004388a0(MdlNode* self, void*, List* faces, float* area) {
    put<float>(area, 0, 0.0f);                                                       // 004388AF mov [ebp],ebx
    const U base = addr(aurora_pool_lock_00423f10(get<U>(model_of(self), 0xAC), 0, 0, 8)); // 004388B2..004388C9
    const U offset = get<U>(self, 0x194);                                            // 004388CE
    for (I i = 0; i < faces->size; ++i) {                                            // 004388DA..004388E2 / 004389D0
        const U f = faces->data + static_cast<U>(i) * 0x20u;                         // 004388F0..004388FA (+20h)
        const U stride = get<U>(self, 0x14C);                                        // 00438901..00438905
        const U p0 = static_cast<U>(static_cast<I>(get<I16>(vp(f), 0x1A)) * static_cast<I>(stride)) + offset + base; // 004388FD..00438910
        const U p1 = static_cast<U>(static_cast<I>(get<I16>(vp(f), 0x1C)) * static_cast<I>(stride)) + offset + base; // 0043891A..00438923
        const U p2 = static_cast<U>(static_cast<I>(get<I16>(vp(f), 0x1E)) * static_cast<I>(stride)) + offset + base; // 00438931..0043893D
        const float x0 = get<float>(vp(p0), 0), y0 = get<float>(vp(p0), 4), z0 = get<float>(vp(p0), 8);       // 00438912..00438917
        const float x1 = get<float>(vp(p1), 0), y1 = get<float>(vp(p1), 4), z1 = get<float>(vp(p1), 8);       // 00438929..0043892E
        float v2[3];                                                                 // [esp+14h] (raw x, fld/fstp y,z)
        put<U>(v2, 0, get<U>(vp(p2), 0));                                            // 0043893A..0043893F
        v2[1] = get<float>(vp(p2), 4);                                               // 0043894B..0043894E
        v2[2] = get<float>(vp(p2), 8);                                               // 00438953..0043895A
        float e1[3], e2[3], cross[3];                                                // [esp+20h], [esp+2Ch], [esp+50h]
        e1[0] = static_cast<float>(static_cast<double>(x1) - x0);                    // 00438961..00438968
        e1[1] = static_cast<float>(static_cast<double>(y1) - y0);                    // 0043896C..0043896E
        e1[2] = static_cast<float>(static_cast<double>(z1) - z0);                    // 00438972..0043897A (z0/z1 via float locals)
        e2[0] = static_cast<float>(static_cast<double>(v2[0]) - x0);                 // 0043897E..00438984
        e2[1] = static_cast<float>(static_cast<double>(v2[1]) - y0);                 // 00438988..0043898E
        e2[2] = static_cast<float>(static_cast<double>(v2[2]) - z0);                 // 00438996..0043899E
        vector_cross_004ab210(cross, e2, e1);                                        // 00438952..004389A2 (out, e2, e1)
        const double magnitude = vector_magnitude_004ab100(cross);                   // 004389A7..004389AC (ST0)
        put<float>(area, 0, static_cast<float>(magnitude * static_cast<double>(kHalf_0073e9ac) +
                                               static_cast<double>(get<float>(area, 0))));   // 004389B9..004389CD
    }
    aurora_pool_unlock_00423f30(get<U>(model_of(self), 0xAC));                      // 004389D9..004389EC
}

// ================================================================ 0043A120 MdlNodeTriMesh::ProcessVerts (thiscall RET 0)
void __fastcall mdl_trimesh_process_verts_0043a120(MdlNode* self, void*) {
    List* const s88 = scratch(historical_storage_007a8a88);                          // 007A8A88 data / 8C size / 90 cap
    List* const s7c = scratch(historical_storage_007a8a7c);                          // 007A8A7C / 80 / 84
    List* const s70 = scratch(historical_storage_007a8a70);                          // 007A8A70 / 74 / 78
    // (1) First pass: when the 007A8A88 list is empty and node byte +184 is set, push each face's first three
    // attribute indices (face+04 array [0..2]) converted to floats.
    if (s88->size == 0 && get<U8>(self, 0x184) != 0) {                               // 0043A120..0043A141
        for (I i = 0; i < get<I>(post(self), 0x4C); ++i) {                           // 0043A147..0043A154 / 0043A1D3..0043A1E2
            const U attributes = get<U>(vp(face_at(self, i)), 0x04);                 // 0043A160..0043A169
            const float x = static_cast<float>(get<I>(vp(attributes), 0));           // 0043A16D..0043A175 fild/fstp dword
            const float y = static_cast<float>(get<I>(vp(attributes), 4));           // 0043A179..0043A17C
            const float z = static_cast<float>(get<I>(vp(attributes), 8));           // 0043A180..0043A18A
            U bx, by, bz;
            std::memcpy(&bx, &x, 4); std::memcpy(&by, &y, 4); std::memcpy(&bz, &z, 4);
            vector_list_add(s88, bx, by, bz);                                        // 0043A183..0043A1CD
        }
    }
    // (2) Per face corner: smoothing-group normal.
    for (I i = 0; i < get<I>(post(self), 0x4C); ++i) {                               // 0043A1E8..0043A1F9 / 0043A623..0043A649
        const U mask = get<U>(vp(face_at(self, i)), 0x08);                           // 0043A212..0043A221 [esp+54h]
        for (I k = 0; k < 3; ++k) {                                                  // edi = 0,4,8 (0043A613..0043A61D)
            const U element = static_cast<U>(i) * 12u;                               // ebx = 8 + 12i (component -8/-4/0)
            if (s88->size != 0) {                                                    // 0043A230..0043A237
                const U arr = get<U>(vp(face_at(self, i)), 0x04);
                put<I>(vp(arr), 0x0C, ftol_i32(get<float>(vp(s88->data + element), 0)));   // 0043A239..0043A254
                put<I>(vp(get<U>(vp(face_at(self, i)), 0x04)), 0x10, ftol_i32(get<float>(vp(s88->data + element), 4))); // 0043A257..0043A273
                put<I>(vp(get<U>(vp(face_at(self, i)), 0x04)), 0x14, ftol_i32(get<float>(vp(s88->data + element), 8))); // 0043A276..0043A291
            }
            if (s7c->size != 0) {                                                    // 0043A294..0043A29B
                put<I>(vp(get<U>(vp(face_at(self, i)), 0x04)), 0x18, ftol_i32(get<float>(vp(s7c->data + element), 0))); // 0043A29D..0043A2B9
                put<I>(vp(get<U>(vp(face_at(self, i)), 0x04)), 0x1C, ftol_i32(get<float>(vp(s7c->data + element), 4))); // 0043A2BC..0043A2D8
                put<I>(vp(get<U>(vp(face_at(self, i)), 0x04)), 0x20, ftol_i32(get<float>(vp(s7c->data + element), 8))); // 0043A2DB..0043A2F6
            }
            if (s70->size != 0) {                                                    // 0043A2F9..0043A300
                put<I>(vp(get<U>(vp(face_at(self, i)), 0x04)), 0x24, ftol_i32(get<float>(vp(s70->data + element), 0))); // 0043A302..0043A31E
                put<I>(vp(get<U>(vp(face_at(self, i)), 0x04)), 0x28, ftol_i32(get<float>(vp(s70->data + element), 4))); // 0043A321..0043A33D
                put<I>(vp(get<U>(vp(face_at(self, i)), 0x04)), 0x2C, ftol_i32(get<float>(vp(s70->data + element), 8))); // 0043A340..0043A35B
            }
            put<I>(vp(get<U>(vp(face_at(self, i)), 0x10)), static_cast<U>(k) * 4u, -1);   // 0043A35E..0043A36B
            const I vertex = get<I>(vp(get<U>(vp(face_at(self, i)), 0x00)), static_cast<U>(k) * 4u); // 0043A372..0043A3A0 [esp+74h]
            float sum[3] = {0.0f, 0.0f, 0.0f};                                       // 0043A381..0043A391 [esp+2Ch]
            for (I j = 0; j < get<I>(post(self), 0x4C); ++j) {                       // 0043A399..0043A3A4 / 0043A537..0043A548
                const U fj = face_at(self, j);                                       // 0043A3B0..0043A3C1 [esp+70h]
                const U indices = get<U>(vp(fj), 0x00);                              // 0043A3BF
                I m = 3;                                                             // 0043A3C5
                bool found = false;
                do {                                                                 // 0043A3D0..0043A3DE: idx[2], idx[1], idx[0]
                    --m;
                    if (get<I>(vp(indices), static_cast<U>(m) * 4u) == vertex) { found = true; break; }
                } while (m != 0);
                if (!found) continue;                                                // 0043A3E0 (0043A3E5 jl never taken)
                if (j != i && (get<U>(vp(fj), 0x08) & mask) == 0) continue;           // 0043A3ED..0043A3FE
                const U positions = get<U>(post(self), 0x00);                        // 0043A404
                const U p0 = positions + static_cast<U>(get<I>(vp(indices), 0)) * 12u;   // 0043A406..0043A40B
                const U p1 = positions + static_cast<U>(get<I>(vp(indices), 4)) * 12u;   // 0043A421..0043A42A
                const U p2 = positions + static_cast<U>(get<I>(vp(indices), 8)) * 12u;   // 0043A441..0043A444
                float a[3], b[3], cross[3];                                          // [esp+84h], [esp+90h], [esp+9Ch]
                a[0] = static_cast<float>(static_cast<double>(get<float>(vp(p2), 0)) - get<float>(vp(p1), 0)); // 0043A453..0043A467
                a[1] = static_cast<float>(static_cast<double>(get<float>(vp(p2), 4)) - get<float>(vp(p1), 4)); // 0043A475..0043A48D
                a[2] = static_cast<float>(static_cast<double>(get<float>(vp(p2), 8)) - get<float>(vp(p1), 8)); // 0043A494..0043A49D
                b[0] = static_cast<float>(static_cast<double>(get<float>(vp(p1), 0)) - get<float>(vp(p0), 0)); // 0043A4A4..0043A4AF
                b[1] = static_cast<float>(static_cast<double>(get<float>(vp(p1), 4)) - get<float>(vp(p0), 4)); // 0043A4B6..0043A4C1
                b[2] = static_cast<float>(static_cast<double>(get<float>(vp(p1), 8)) - get<float>(vp(p0), 8)); // 0043A4C8..0043A4D3
                const float* const c = vector_cross_004ab210(cross, b, a);           // 0043A46E..0043A4DA (out, b, a)
                sum[0] = static_cast<float>(static_cast<double>(sum[0]) + c[0]);     // 0043A4DF..0043A4E8
                sum[1] = static_cast<float>(static_cast<double>(sum[1]) + c[1]);     // 0043A4EC..0043A4F3
                sum[2] = static_cast<float>(static_cast<double>(sum[2]) + c[2]);     // 0043A4F7..0043A50E
                if (i > j && get<U>(vp(face_at(self, j)), 0x08) == mask) {           // 0043A50A..0043A535
                    // An earlier face of the same smoothing group already owns this corner's normal: share it.
                    const U jn = get<U>(vp(face_at(self, j)), 0x10);                 // 0043A554..0043A564
                    put<I>(vp(get<U>(vp(face_at(self, i)), 0x10)), static_cast<U>(k) * 4u,
                           get<I>(vp(jn), static_cast<U>(m) * 4u));                  // 0043A568..0043A573
                    break;                                                           // -> 0043A576
                }
            }
            if (!(vector_magnitude_004ab100(sum) == static_cast<double>(kZero_0073d700)))   // 0043A57A..0043A592 (NaN normalises)
                vector_normalize_004ab130(sum, nullptr);                             // 0043A594..0043A598
            const U normal_slot = get<U>(vp(face_at(self, i)), 0x10) + static_cast<U>(k) * 4u;   // 0043A59D..0043A5AA
            if (get<I>(vp(normal_slot), 0) == -1) {                                  // 0043A5AD..0043A5B0
                put<I>(vp(normal_slot), 0, get<I>(post(self), 0x58));                // 0043A5B2..0043A5B5
                U sx, sy, sz;
                std::memcpy(&sx, &sum[0], 4); std::memcpy(&sy, &sum[1], 4); std::memcpy(&sz, &sum[2], 4);
                vector_list_add(asciimdl::list_at(post(self), 0x54), sx, sy, sz);    // 0043A5B7..0043A60C
            }
        }
    }
    // (3) Bounding box (node+64 min / +70 max), centroid (+80) and radius (+7C) over post positions.
    float total[3] = {0.0f, 0.0f, 0.0f};                                             // 0043A655..0043A665 [esp+48h]
    for (I v = 0; v < get<I>(post(self), 0x04); ++v) {                               // 0043A66D..0043A674 / 0043A721..0043A730
        const U p = get<U>(post(self), 0x00) + static_cast<U>(v) * 12u;             // 0043A680..0043A68E
        for (U c = 0; c < 3; ++c) {                                                  // min: test ah,5 / jp (keep min if min < p)
            const float low = get<float>(self, 0x64 + c * 4), value = get<float>(vp(p), c * 4);   // 0043A686..0043A6CE
            put<float>(self, 0x64 + c * 4, low < value ? low : value);
        }
        for (U c = 0; c < 3; ++c) {                                                  // max: test ah,41h / jne (keep max if max > p)
            const float high = get<float>(self, 0x70 + c * 4), value = get<float>(vp(p), c * 4);  // 0043A6D1..0043A715
            put<float>(self, 0x70 + c * 4, high > value ? high : value);
        }
        vector_add_assign_004a9740(total, nullptr, static_cast<const float*>(vp(p)));   // 0043A714..0043A71C
    }
    const I count = get<I>(post(self), 0x04);                                        // 0043A736..0043A73C fild
    float center[3];
    if (!(static_cast<double>(count) == static_cast<double>(kZero_0073d700))) {     // 0043A73F..0043A74A
        const double inverse = static_cast<double>(kOne_0073d764) / static_cast<double>(count);   // 0043A74C fdivr
        center[0] = static_cast<float>(static_cast<double>(total[0]) * inverse);     // 0043A752..0043A758
        center[1] = static_cast<float>(static_cast<double>(total[1]) * inverse);     // 0043A760..0043A766
        center[2] = static_cast<float>(static_cast<double>(total[2]) * inverse);     // 0043A76A..0043A774
    } else {
        center[0] = total[0]; center[1] = total[1]; center[2] = total[2];            // 0043A77E..0043A786 (raw moves)
    }
    put<float>(self, 0x80, center[0]); put<float>(self, 0x84, center[1]); put<float>(self, 0x88, center[2]); // 0043A78A..0043A799
    for (I v = 0; v < get<I>(post(self), 0x04); ++v) {                               // 0043A79C..0043A7A3 / 0043A7F5..0043A804
        const U p = get<U>(post(self), 0x00) + static_cast<U>(v) * 12u;             // 0043A7B0..0043A7BB
        float delta[3];                                                              // [esp+58h]
        delta[0] = static_cast<float>(static_cast<double>(get<float>(vp(p), 0)) - get<float>(self, 0x80)); // 0043A7B8..0043A7C4
        delta[1] = static_cast<float>(static_cast<double>(get<float>(vp(p), 4)) - get<float>(self, 0x84)); // 0043A7C8..0043A7CE
        delta[2] = static_cast<float>(static_cast<double>(get<float>(vp(p), 8)) - get<float>(self, 0x88)); // 0043A7D2..0043A7D8
        const double distance = vector_magnitude_004ab100(delta);                    // 0043A7DC
        if (distance > static_cast<double>(get<float>(self, 0x7C)))                  // 0043A7E1..0043A7EC
            put<float>(self, 0x7C, static_cast<float>(distance));                    // 0043A7EE
    }
}
} // extern "C"

namespace {
// ---- 0043B380 helpers: one ring vertex copy (0043B4E3..0043B5B9 / 0043B953..0043BA29) and a row copy
// (0043B630..0043B6B7 and the three following blocks of each replication pass).
void saber_take(MdlNode* self, I dst, I src_vertex) {
    const U s12 = static_cast<U>(src_vertex) * 12u, d12 = static_cast<U>(dst) * 12u;
    put<U>(vp(get<U>(self, 0x1A0)), static_cast<U>(dst) * 8u, get<U>(vp(get<U>(post(self), 0x0C)), s12));          // tex0 u
    put<U>(vp(get<U>(self, 0x1A0)), static_cast<U>(dst) * 8u + 4, get<U>(vp(get<U>(post(self), 0x0C)), s12 + 4));  // tex0 v
    put<U>(vp(get<U>(self, 0x1A4)), d12, get<U>(vp(get<U>(post(self), 0x54)), s12));                               // normal
    put<U>(vp(get<U>(self, 0x1A4)), d12 + 4, get<U>(vp(get<U>(post(self), 0x54)), s12 + 4));
    put<U>(vp(get<U>(self, 0x1A4)), d12 + 8, get<U>(vp(get<U>(post(self), 0x54)), s12 + 8));
    put<U>(vp(get<U>(self, 0x19C)), d12, get<U>(vp(get<U>(post(self), 0x00)), s12));                               // position
    put<U>(vp(get<U>(self, 0x19C)), d12 + 4, get<U>(vp(get<U>(post(self), 0x00)), s12 + 4));
    put<U>(vp(get<U>(self, 0x19C)), d12 + 8, get<U>(vp(get<U>(post(self), 0x00)), s12 + 8));
    copy3(get<U>(self, 0x198) + d12, get<U>(self, 0x19C) + d12);                                                    // 198 <- 19C
}
void saber_row(MdlNode* self, I dst, I src) {
    const U s12 = static_cast<U>(src) * 12u, d12 = static_cast<U>(dst) * 12u;
    put<U>(vp(get<U>(self, 0x1A0)), static_cast<U>(dst) * 8u, get<U>(vp(get<U>(self, 0x1A0)), static_cast<U>(src) * 8u));
    put<U>(vp(get<U>(self, 0x1A0)), static_cast<U>(dst) * 8u + 4, get<U>(vp(get<U>(self, 0x1A0)), static_cast<U>(src) * 8u + 4));
    copy3(get<U>(self, 0x1A4) + d12, get<U>(self, 0x1A4) + s12);
    copy3(get<U>(self, 0x19C) + d12, get<U>(self, 0x19C) + s12);
    copy3(get<U>(self, 0x198) + d12, get<U>(self, 0x19C) + d12);
}
// Ring scan: first colour element j (post+3C, component offset 0 or 8, 12-byte stride) whose
// _ftol(c * 255.0f + 0.5f) equals ring index; unbounded like the original (0043B450..0043B4BA / 0043B8C0..0043B92A).
I saber_find(MdlNode* self, U component, I ring) {
    U e = get<U>(post(self), 0x3C) + component;
    I j = 0;
    while (ftol_i32(static_cast<double>(get<float>(vp(e), 0)) * static_cast<double>(k255_0073ede4) +
                    static_cast<double>(kHalf_0073e9ac)) != ring) {
        e += 12; ++j;
    }
    return j;
}
} // namespace

extern "C" {
// ================================================================ 0043B380 MdlNodeLightsaber::InternalPostProcess
void __fastcall mdl_saber_internal_post_process_0043b380(MdlNode* self, void*) {
    if (get<I16>(self, 0x180) > 0) return;                                           // 0043B389..0043B390
    mdl_trimesh_process_verts_0043a120(self, nullptr);                               // 0043B398
    get<NodeMeshFn>(self, 0x50)(self);                                               // 0043B39D..0043B39E call [esi+50h]
    const U16 count = static_cast<U16>(static_cast<U16>((kSaberWord4C + kSaberWord48_00740a48) * kSaberWord54) +
                                       static_cast<U16>(kSaberWord50 << 1));        // 0043B3A1..0043B3C4 (word arithmetic)
    put<U16>(self, 0x180, count);                                                    // 0043B3C6
    const I n = get<I16>(self, 0x180);
    put<U>(self, 0x19C, addr(live_operator_new(static_cast<U>(n) * 12u)));           // 0043B3CD..0043B3DC
    put<U>(self, 0x1A4, addr(live_operator_new(static_cast<U>(get<I16>(self, 0x180)) * 12u)));        // 0043B3E2..0043B3F5
    const U block198 = addr(live_operator_new(static_cast<U>(get<I16>(self, 0x180) + 1) * 12u));     // 0043B3FB..0043B40A
    put<U16>(self, 0x182, static_cast<U16>(get<U16>(self, 0x182) + 1));              // 0043B416
    put<U>(self, 0x198, block198);                                                   // 0043B421
    put<U>(self, 0x1A0, addr(live_operator_new(static_cast<U>(get<I16>(self, 0x180)) * 8u)));         // 0043B40F..0043B433
    // Ring vertex slots v0 (ebx), v1..v3 ([esp+18h..20h]). D2: ebx is loaded from the never-written [esp+14h]
    // (0043B42C) and slots 1..3 have no initializing store; with the .rdata ring count 8 (00740A50) the first ring
    // pass writes all four before any read, so the value-initialization below is unobservable (D2 label, 0043B42C).
    I v[4] = {0, 0, 0, 0};
    I out = 0;                                                                       // edi
    for (I ring = 0; ring < static_cast<I>(kSaberWord50); ++ring) {                  // 0043B43B..0043B446 / 0043B5BC..0043B5D2
        const I j = saber_find(self, 0, ring);                                       // 0043B450..0043B4BA
        if (ring >= 0 && ring <= 3) v[ring] = out;                                   // 0043B4BC..0043B4DF
        saber_take(self, out, j);                                                    // 0043B4E3..0043B5B9
        ++out;
    }
    for (I r = 0; r < static_cast<I>(kSaberWord48_00740a48); ++r) {                  // 0043B5DC..0043B5EC / 0043B880..0043B89A
        saber_row(self, out, v[0]); saber_row(self, out + 1, v[1]);                  // 0043B630..0043B74F
        saber_row(self, out + 2, v[2]); saber_row(self, out + 3, v[3]);              // 0043B752..0043B87D
        out += 4;
    }
    for (I ring = 0; ring < static_cast<I>(kSaberWord50); ++ring) {                  // 0043B8A0..0043B8B0 / 0043BA2C..0043BA42
        const I j = saber_find(self, 8, ring);                                       // 0043B8C0..0043B92A (z component)
        if (ring >= 0 && ring <= 3) v[ring] = out;                                   // 0043B92C..0043B94F
        saber_take(self, out, j);                                                    // 0043B953..0043BA29
        ++out;
    }
    for (I r = 0; r < static_cast<I>(kSaberWord4C); ++r) {                           // 0043BA4C..0043BA5C / 0043BD06..0043BD1E
        saber_row(self, out, v[0]); saber_row(self, out + 1, v[1]);                  // 0043BAB0..0043BBD1
        saber_row(self, out + 2, v[2]); saber_row(self, out + 3, v[3]);              // 0043BBD4..0043BD03
        out += 4;
    }
    put<U>(self, 0x14C, 0);                                                          // 0043BD25
}
} // extern "C"

namespace {
// Shared vertex-buffer fill of 0043CF00 / 0043D990: pool reservation (0043D122..0043D1D8 / 0043DAEB..0043DB9D),
// per-vertex interleave (0043D200..0043D364 / 0043DBC3..0043DD58), terminator vertex (0043D36A..0043D3DE /
// 0043DD5E..0043DDD2) and unlock (0043D3E2..0043D3F1 / 0043DDD6..0043DDE5). `dangly` adds the 1B4 position copy.
void fill_vertex_buffer(MdlNode* self, U stride, U first, bool dangly) {
    const U align = get<U>(model_of(self), 0xB0) & 0x0Fu;                            // 0043D12B..0043D139 (ebx)
    const U reserve = get<U>(model_of(self), 0xB0) +
                      static_cast<U>(get<I16>(self, 0x180) + 1) * stride + align;   // 0043D13C..0043D157 (ebp)
    gl_render_resize_pool_00423f70(get<U>(model_of(self), 0xAC), reserve);           // 0043D159..0043D166
    const U offset = get<U>(model_of(self), 0xB0) + align;                           // 0043D16B..0043D17F
    put<U>(self, 0x194, offset);                                                     // 0043D181
    MaxTree* const model = model_of(self);                                           // 0043D187
    put<U>(model, 0xB0, get<U>(model, 0xB0) +
                        static_cast<U>(get<I16>(self, 0x180) + 1) * stride + align); // 0043D18C..0043D1A6
    const U base = addr(aurora_pool_lock_00423f10(get<U>(model_of(self), 0xAC), 0, 0, 1)); // 0043D1A8..0043D1C2 (ebx)
    put<U>(self, 0x198, addr(live_operator_new(static_cast<U>(get<I16>(self, 0x180) + 1) * 12u))); // 0043D1C4..0043D1D8
    U row = 0;                                                                       // [esp+18h] i*stride
    for (I i = 0; i < get<I16>(self, 0x180); ++i) {                                  // 0043D1E3..0043D1EE / 0043D350..0043D364
        const U vtx = get<U>(vp(get<U>(post(self), 0x60)), static_cast<U>(i) * 4u); // 0043D200..0043D209
        const U at_row = row + base + offset;                                        // dst = node+16x + row + ebx + ebp
        const U8 flags = get<U8>(vp(first), 0x54);                                   // test byte [first+54h] (first vertex record)
        if (flags & 1) {                                                             // 0043D20C..0043D233 tex0
            put<U>(vp(get<U>(self, 0x160) + at_row), 0, get<U>(vp(vtx), 0x0C));
            put<U>(vp(get<U>(self, 0x160) + at_row), 4, get<U>(vp(vtx), 0x10));
        }
        if (get<U8>(vp(first), 0x54) & 2) {                                          // 0043D237..0043D25E tex1
            put<U>(vp(get<U>(self, 0x164) + at_row), 0, get<U>(vp(vtx), 0x18));
            put<U>(vp(get<U>(self, 0x164) + at_row), 4, get<U>(vp(vtx), 0x1C));
        }
        if (get<U8>(vp(first), 0x54) & 4) {                                          // 0043D262..0043D28D tex2
            put<U>(vp(get<U>(self, 0x168) + at_row), 0, get<U>(vp(vtx), 0x24));
            put<U>(vp(get<U>(self, 0x168) + at_row), 4, get<U>(vp(vtx), 0x28));
        }
        if (get<U8>(vp(first), 0x54) & 8) {                                          // 0043D295..0043D2BC tex3
            put<U>(vp(get<U>(self, 0x16C) + at_row), 0, get<U>(vp(vtx), 0x30));
            put<U>(vp(get<U>(self, 0x16C) + at_row), 4, get<U>(vp(vtx), 0x34));
        }
        put<U>(vp(get<U>(self, 0x158) + at_row), 0, get<U>(vp(vtx), 0x3C));        // 0043D2C0..0043D2EE normal
        put<U>(vp(get<U>(self, 0x158) + at_row), 4, get<U>(vp(vtx), 0x40));
        put<U>(vp(get<U>(self, 0x158) + at_row), 8, get<U>(vp(vtx), 0x44));
        put<U>(vp(get<U>(self, 0x154) + at_row), 0, get<U>(vp(vtx), 0x00));        // 0043D2F2..0043D31F position
        put<U>(vp(get<U>(self, 0x154) + at_row), 4, get<U>(vp(vtx), 0x04));
        put<U>(vp(get<U>(self, 0x154) + at_row), 8, get<U>(vp(vtx), 0x08));
        copy3(get<U>(self, 0x198) + static_cast<U>(i) * 12u, vtx);                   // 0043D323..0043D33E
        if (dangly) copy3(get<U>(self, 0x1B4) + static_cast<U>(i) * 12u, vtx);       // 0043DD08..0043DD31 (dangly only)
        row += stride;                                                               // 0043D358
    }
    // Terminator vertex: zero `stride` bytes (rep stos dword + byte), then position = (1e7, 1e7, 1e7).
    const U last = static_cast<U>(static_cast<I>(get<I16>(self, 0x180)) * static_cast<I>(stride)) + get<U>(self, 0x154) + base + offset;
    std::memset(vp(last), 0, stride);                                                // 0043D36A..0043D392
    const U pos = static_cast<U>(static_cast<I>(get<I16>(self, 0x180)) * static_cast<I>(stride)) + get<U>(self, 0x154) + base + offset;
    put<U>(vp(pos), 0, kFar_4b189680); put<U>(vp(pos), 4, kFar_4b189680); put<U>(vp(pos), 8, kFar_4b189680); // 0043D394..0043D3DE
    aurora_pool_unlock_00423f30(get<U>(model_of(self), 0xAC));                      // 0043D3E2..0043D3F1
}
} // namespace

extern "C" {
// ================================================================ 0043CF00 MdlNodeTriMesh::InternalPostProcess
void __fastcall mdl_trimesh_internal_post_process_0043cf00(MdlNode* self, void*) {
    if (get<I16>(self, 0x180) > 0) return;                                           // 0043CF1E..0043CF25
    if (get<U8>(self, 0xA8) == 0) {                                                  // 0043CF2B..0043CF39
        std::memcpy(at(self, 0xA8), kNull_0073ee04, 4);                              // 0043CF3B..0043CF41 dword "NULL"
        put<U8>(self, 0xAC, static_cast<U8>(kNull_0073ee04[4]));                     // 0043CF43..0043CF49
    }
    U length = 0;                                                                    // 0043CF4C..0043CF5C strlen(node+C8)
    while (get<U8>(self, 0xC8 + length) != 0) ++length;
    if (length > 0x10) put<U8>(self, 0xD8, 0);                                       // 0043CF5E..0043CF63 (cl = 0)
    mdl_trimesh_process_verts_0043a120(self, nullptr);                               // 0043CF69..0043CF6B
    I bump = 0;                                                                      // 0043CF73 [esp+20h] (E-4)
    if (get<I>(self, 0x5C) > 0) {                                                    // 0043CF70..0043CF77
        get<NodeMeshFn>(self, 0x50)(self);                                           // 0043CF7D..0043CF7E
        put<U16>(self, 0x180, get<U16>(post(self), 0x64));                           // 0043CF81..0043CF8B
        aurora_trimesh_build_neighbor_lists_004386b0(self, nullptr);                 // 0043CF95..0043CF97
        aurora_trimesh_build_vertex_indices_0043c400(self, nullptr);                 // 0043CF9C..0043CF9E
        // First vertex record (flags byte +54 selects the texture channels). NULL when there are no vertices: the
        // original then faults reading [0+54h] at 0043CFE7 (reproduced read, recorded).
        const U first = get<I16>(self, 0x180) > 0 ? get<U>(vp(get<U>(post(self), 0x60)), 0) : 0u;   // 0043CFA3..0043CFBD
        U flags150 = get<U>(self, 0x150) | 0x21u;                                    // 0043CFC3..0043CFC9
        put<U>(self, 0x150, flags150);                                               // 0043CFCC
        U next = 0x18;                                                               // ebx
        put<U>(self, 0x154, 0);                                                      // 0043CFD7
        put<U>(self, 0x158, 0x0C);                                                   // 0043CFDD
        U stride = next;                                                             // 0043CFED..0043CFEF [esp+10h]
        if (get<U8>(vp(first), 0x54) & 1) {                                          // 0043CFE7..0043CFF3
            put<U16>(self, 0x182, static_cast<U16>(get<U16>(self, 0x182) + 1));      // 0043CFF8
            stride = 0x20;                                                           // 0043CFFF..0043D007
            put<U>(self, 0x160, next);                                               // 0043D00B
            put<U>(self, 0x150, flags150 | 2u);                                      // 0043CFF5 / 0043D011
            next = 0x20;                                                             // 0043D017
        }
        if (get<U8>(vp(first), 0x54) & 2) {                                          // 0043D01C..0043D045
            const U f = get<U>(self, 0x150) | 4u; stride += 8;
            put<U16>(self, 0x182, static_cast<U16>(get<U16>(self, 0x182) + 1));
            put<U>(self, 0x164, next); put<U>(self, 0x150, f); next += 8;
        }
        if (get<U8>(vp(first), 0x54) & 4) {                                          // 0043D048..0043D071
            const U f = get<U>(self, 0x150) | 8u; stride += 8;
            put<U16>(self, 0x182, static_cast<U16>(get<U16>(self, 0x182) + 1));
            put<U>(self, 0x168, next); put<U>(self, 0x150, f); next += 8;
        }
        if (get<U8>(vp(first), 0x54) & 8) {                                          // 0043D074..0043D09D
            const U f = get<U>(self, 0x150) | 0x10u; stride += 8;
            put<U16>(self, 0x182, static_cast<U16>(get<U16>(self, 0x182) + 1));
            put<U>(self, 0x16C, next); put<U>(self, 0x150, f); next += 8;
        }
        void* const block = live_operator_new(0x4C);                                 // 0043D0A0..0043D0AA (state 0)
        void* const material = block != nullptr ? aurora_material_construct_0047b290(block, nullptr) : nullptr; // 0043D0AE..0043D0C5
        material_owner::Material* const m = static_cast<material_owner::Material*>(material);   // object-pointer view (D71)
        // state -1 (0043D0D8); thiscall on the (possibly NULL) material exactly as the original (0043D0C9..0043D0E0)
        m->initialize_textures_0047b560(static_cast<const char*>(static_cast<void*>(at(self, 0xA8))), nullptr, nullptr, nullptr);
        if (aurora_material_has_bump_map_0047b8f0(material, nullptr) != 0 &&         // 0043D0E5..0043D0EE
            texture_controller_is_linear_0041f250(material, nullptr) == 0) {         // 0043D0F0..0043D0F9 (ECX = material)
            put<U>(self, 0x150, get<U>(self, 0x150) | 0x80u);                        // 0043D0FB..0043D112
            stride += 0x24;                                                          // 0043D107
            bump = 1;                                                                // 0043D10A
            put<U>(self, 0x170, next);                                               // 0043D11C
        }
        put<U>(self, 0x14C, stride);                                                 // 0043D125
        fill_vertex_buffer(self, stride, first, false);                              // 0043D122..0043D3F1
        if (material != nullptr) {                                                   // 0043D3F6..0043D3FF
            m->~Material();                                                          // 0043D401..0043D403 (0047B980)
            live_free(material);                                                     // 0043D408..0043D409 (006FA390)
        }
        if (bump != 0) aurora_trimesh_calculate_tangent_space_0043a820(self, nullptr);   // 0043D411..0043D41D
    }
    mdl_trimesh_compute_local_surface_area_004388a0(self, nullptr, asciimdl::list_at(self, 0x58),
                                                    static_cast<float*>(static_cast<void*>(at(self, 0x18C)))); // 0043D422..0043D42F
    get<NodeMeshFn>(self, 0x54)(self);                                               // 0043D434..0043D438 call [esi+54h]
}

// ================================================================ 0043D990 MdlNodeDanglyMesh::InternalPostProcess
void __fastcall mdl_dangly_internal_post_process_0043d990(MdlNode* self, void*) {
    if (get<I16>(self, 0x180) > 0) return;                                           // 0043D999..0043D9A0
    mdl_trimesh_process_verts_0043a120(self, nullptr);                               // 0043D9A6
    if (get<I>(self, 0x5C) > 0) {                                                    // 0043D9AB..0043D9AE
        get<NodeMeshFn>(self, 0x50)(self);                                           // 0043D9B6..0043D9B7
        put<U16>(self, 0x180, get<U16>(post(self), 0x64));                           // 0043D9BA..0043D9C4
        aurora_trimesh_build_neighbor_lists_004386b0(self, nullptr);                 // 0043D9CE..0043D9D0
        aurora_trimesh_build_vertex_indices_0043c400(self, nullptr);                 // 0043D9D5..0043D9D7
        const I16 count = get<I16>(self, 0x180);                                     // 0043D9DC
        // First vertex record; NULL without vertices -> the original faults at 0043DA44 (reproduced read, recorded).
        const U first = count > 0 ? get<U>(vp(get<U>(post(self), 0x60)), 0) : 0u;   // 0043D9E3..0043D9FD
        put<U>(self, 0x14C, 0);                                                      // 0043DA09
        put<U>(self, 0x154, 0);                                                      // 0043DA0F
        put<U>(self, 0x1B4, addr(live_operator_new(static_cast<U>(static_cast<I>(count)) * 12u)));   // 0043D9FF..0043DA20
        const U flags150 = get<U>(self, 0x150) | 0x21u;                              // 0043DA1A..0043DA26
        U stride = 0x18;                                                             // 0043DA29
        put<U>(self, 0x158, 0x0C);                                                   // 0043DA2E
        put<U>(self, 0x14C, stride);                                                 // 0043DA38
        put<U>(self, 0x150, flags150);                                               // 0043DA3E
        if (get<U8>(vp(first), 0x54) & 1) {                                          // 0043DA44..0043DA4F
            put<U16>(self, 0x182, static_cast<U16>(get<U16>(self, 0x182) + 1));      // 0043DA51
            put<U>(self, 0x160, stride);                                             // 0043DA58
            stride = 0x20;                                                           // 0043DA5E
            put<U>(self, 0x14C, stride);                                             // 0043DA66
            put<U>(self, 0x150, flags150 | 2u);                                      // 0043DA63..0043DA6C
        }
        if (get<U8>(vp(first), 0x54) & 2) {                                          // 0043DA72..0043DA97
            const U f = get<U>(self, 0x150) | 4u;
            put<U16>(self, 0x182, static_cast<U16>(get<U16>(self, 0x182) + 1));
            put<U>(self, 0x164, stride); stride += 8;
            put<U>(self, 0x14C, stride); put<U>(self, 0x150, f);
        }
        if (get<U8>(vp(first), 0x54) & 4) {                                          // 0043DA9D..0043DAC2
            const U f = get<U>(self, 0x150) | 8u;
            put<U16>(self, 0x182, static_cast<U16>(get<U16>(self, 0x182) + 1));
            put<U>(self, 0x168, stride); stride += 8;
            put<U>(self, 0x14C, stride); put<U>(self, 0x150, f);
        }
        if (get<U8>(vp(first), 0x54) & 8) {                                          // 0043DAC8..0043DAE4
            put<U16>(self, 0x182, static_cast<U16>(get<U16>(self, 0x182) + 1));
            put<U>(self, 0x16C, stride); stride += 8;
            put<U>(self, 0x14C, stride);
            put<U>(self, 0x150, get<U>(self, 0x150) | 0x10u);
        }
        fill_vertex_buffer(self, get<U>(self, 0x14C), first, true);                  // 0043DAEB..0043DDE5 (edi = [esi+14Ch])
    }
    get<NodeMeshFn>(self, 0x54)(self);                                               // 0043DDEF..0043DDF3
}
} // extern "C"
