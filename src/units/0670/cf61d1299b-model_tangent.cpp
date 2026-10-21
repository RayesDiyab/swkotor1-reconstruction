// model_tangent.cpp - lane-3d-model rev2: MdlNodeTriMesh::CalculateTangentSpaceBasis 0043A820 (PartSkin::PostProcess
// 0048A081, bump-mapped skins only; the PostProcess body itself is skipped for binary MDL skins) and its math
// helpers Matrix::Matrix 00437AF0, Transpose 004A99B0, Vector operator== 004AA980, normalize 004AB180.
// x87 intermediates as double with every original fstp dword kept as a float rounding (D30); values the original
// keeps on the register stack across several operations stay double here. EH unwind state 0 cited.
#include "a7e200e217-model_instance.hpp"

using namespace aurora_model;
using kotor_scene3d::Vector;

namespace {
struct Matrix9 { Vector row[3]; };
static_assert(sizeof(Matrix9) == 36, "3x3 float matrix");
}

extern "C" {
Vector* __fastcall vector_add_assign_004a9740(Vector* self, void* unused_edx, const Vector* v); // model_skin.cpp
void __fastcall vector_normalize_004ab130(Vector* self, void* unused_edx);                      // model_skin.cpp
U __cdecl aurora_vector_equal_004aa980(const void* left, const void* right);
Vector* __cdecl vector_normalized_004ab180(Vector* out, const Vector* v);
void* __fastcall matrix_identity_construct_00437af0(void* self, void* unused_edx);
float* __cdecl matrix_transpose_004a99b0(float* out9, const float* m9);
void __fastcall aurora_trimesh_calculate_tangent_space_0043a820(MdlNode* self, void* unused_edx);
}

// 004AA980 operator==(Vector,Vector): single body = existing M44 provider _aurora_vector_equal_004aa980 - (gui-draw writers m30-r5 vector_equal.cpp, x87 asm); declaration above unchanged (same cdecl symbol).

// 004AB180 normalize(out, v), cdecl -> out: m = |v| (004AB100, ST0); m < 1e-9 (007440F0) -> (1, 0, 0); else
// inv = (float)(1 / m) and each component inv * v, rounded to float.
// [M45 compose] 004AB180 body removed: D55 owner lane-world-los normalize_004ab180 (definition removed; declaration kept)
Vector* __cdecl vector_normalized_004ab180(Vector* out, const Vector* v);

// 00437AF0 Matrix::Matrix(): zero the nine floats, then rows (1,0,0), (0,1,0), (0,0,1); EAX = this.
void* __fastcall matrix_identity_construct_00437af0(void* self, void*) {
    float* const m = static_cast<float*>(self);
    for (int i = 0; i < 9; ++i) m[i] = 0.0f;                                        // 00437AF7..00437B0E
    m[0] = 1.0f; m[1] = 0.0f; m[2] = 0.0f;                                          // 00437B11..00437B34
    m[3] = 0.0f; m[4] = 1.0f; m[5] = 0.0f;                                          // 00437B37..00437B70
    m[6] = 0.0f; m[7] = 0.0f; m[8] = 1.0f;                                          // 00437B6D..00437B8E
    return self;
}

// 004A99B0 Transpose(out, m), cdecl -> out: Matrix(out) from the rows (m0,m3,m6), (m1,m4,m7), (m2,m5,m8) via
// 00437BA0 (lane-3d-render).
float* __cdecl matrix_transpose_004a99b0(float* out9, const float* m9) {
    const Vector r0{m9[0], m9[3], m9[6]};                                           // 004A99CB..004A9A42
    const Vector r1{m9[1], m9[4], m9[7]};                                           // 004A99BD..004A9A25
    const Vector r2{m9[2], m9[5], m9[8]};                                           // 004A99BA..004A9A0C
    matrix_construct_00437ba0(out9, nullptr, r0, r1, r2);                           // 004A9A45..004A9A47
    return out9;                                                                    // 004A9A4C
}

// 0043A820 MdlNodeTriMesh::CalculateTangentSpaceBasis: per face, a basis (normalised s-direction, normalised
// t-direction flipped to agree with the face normal, their cross product); per face corner, the bases of every
// face sharing the corner position and a smoothing-group bit are summed, normalised, and written as nine floats
// at the vertex row (pool [AC] of the owner model, row node[194] + node[14C] * index + node[170]).
void __fastcall aurora_trimesh_calculate_tangent_space_0043a820(MdlNode* self, void*) {
    unsigned char* const pool = static_cast<unsigned char*>(aurora_pool_lock_00423f10(
        get<U>(aurora_tree_as_model_0043e1c0(get<MaxTree*>(self, 8), nullptr), 0xAC), 0, 0, 1)); // 0043A840..0043A856
    Vector t_dir{0.0f, 0.0f, 0.0f};                                                 // [esp+0D4h] (0043A86C..0043A88F)
    Vector s_dir{0.0f, 0.0f, 0.0f};                                                 // [esp+0C8h]
    Matrix9 face_basis;                                                             // [esp+12Ch]
    matrix_identity_construct_00437af0(&face_basis, nullptr);                       // 0043A85E..0043A896
    Vector n1{0.0f, 0.0f, 0.0f};                                                    // [esp+8Ch] (0043A8A5..0043A8B3)
    Vector n2{0.0f, 0.0f, 0.0f};                                                    // [esp+4Ch] (0043A8BA..0043A8C2)
    const I faces = get<I>(self, 0x5C);                                             // 0043A89B
    Matrix9* const bases = static_cast<Matrix9*>(live_operator_new(static_cast<U>(faces) * 36)); // 0043A89E..0043A8C6 (thunk 006FC0D2)
    if (bases != nullptr)                                                           // 0043A8D0..0043A8D2
        for (I i = 0; i < faces; ++i) matrix_identity_construct_00437af0(&bases[i], nullptr); // 0043A8D4..0043A8F5
    void* const pp = get<void*>(self, 0x134);
    for (I f = 0; f < get<I>(self, 0x5C); ++f) {                                    // 0043A901..0043ADE3
        const unsigned char* const face = ptr<unsigned char>(get<U>(self, 0x58) + static_cast<U>(f) * 0x20); // 0043A920..0043A92C
        const U* const verts = ptr<U>(get<U>(get<void*>(self, 0x134), 0x60));      // 0043A923..0043A929
        const float* const A = ptr<float>(verts[get<I16>(face, 0x1A)]);             // 0043A92F..0043A933
        const float* const B = ptr<float>(verts[get<I16>(face, 0x1C)]);             // 0043A948..0043A94C
        const float* const C = ptr<float>(verts[get<I16>(face, 0x1E)]);             // 0043A94F..0043A953
        const float a0 = A[0], a1 = A[1], a2 = A[2];                                // [esp+38h..40h]
        const double e1z = static_cast<double>(B[2]) - a2;                          // 0043A976..0043A97D (kept on the x87 stack)
        const double e1y = static_cast<double>(B[1]) - a1;                          // 0043A992..0043A999 (kept)
        const float e1x = static_cast<float>(static_cast<double>(B[0]) - a0);       // 0043A9A0..0043A9BE ([esp+15Ch])
        const float e2z = static_cast<float>(static_cast<double>(C[2]) - a2);       // 0043A9CC..0043A9E9 ([esp+24h])
        const float e2y = static_cast<float>(static_cast<double>(C[1]) - a1);       // 0043A9ED..0043AA0F ([esp+2Ch])
        const float e2x = static_cast<float>(static_cast<double>(C[0]) - a0);       // 0043AA13..0043AA38 ([esp+150h])
        const double du1 = static_cast<double>(B[3]) - A[3];                        // 0043AA3F..0043AA49 (kept)
        const double du2 = static_cast<double>(C[3]) - A[3];                        // 0043AA57..0043AA65 (kept)
        const float dv1 = static_cast<float>(static_cast<double>(B[4]) - A[4]);     // 0043AA6C..0043AA7A ([esp+14h])
        const float dv2 = static_cast<float>(static_cast<double>(C[4]) - A[4]);     // 0043AA7E..0043AA8C ([esp+10h])
        const float t58 = static_cast<float>(e1z * du2);                            // 0043AA90..0043AA94
        const float t48 = static_cast<float>(e1y * du2);                            // 0043AA98..0043AA9C
        const float t18c = static_cast<float>(static_cast<double>(e1x) * du2);      // 0043AAA0..0043AAA9
        const float t20 = static_cast<float>(static_cast<double>(e2z) * du1);       // 0043AAB2..0043AAB8
        const float t44 = static_cast<float>(static_cast<double>(e2y) * du1);       // 0043AABC..0043AAC2
        const float t168 = static_cast<float>(static_cast<double>(e2x) * du1);      // 0043AAC6..0043AACF
        t_dir.z = static_cast<float>(static_cast<double>(t20) - t58);               // 0043AAD8..0043AADC / 0043AB20
        t_dir.y = static_cast<float>(static_cast<double>(t44) - t48);               // 0043AAE0..0043AAE4 / 0043AB0B
        t_dir.x = static_cast<float>(static_cast<double>(t168) - t18c);             // 0043AAE8..0043AAF6
        const float u48 = static_cast<float>(e1z * dv2);                            // 0043AB2E..0043AB3B
        const float u20 = static_cast<float>(e1y * dv2);                            // 0043AB3F..0043AB43
        const float u174 = static_cast<float>(static_cast<double>(e1x) * dv2);      // 0043AB47..0043AB52
        const double p = static_cast<double>(e2z) * dv1;                            // 0043AB59..0043AB5D (kept)
        const float u44 = static_cast<float>(static_cast<double>(e2y) * dv1);       // 0043AB61..0043AB69
        const float u180 = static_cast<float>(static_cast<double>(e2x) * dv1);      // 0043AB6D..0043AB78
        s_dir.z = static_cast<float>(p - u48);                                      // 0043AB7F / 0043ABCA
        s_dir.y = static_cast<float>(static_cast<double>(u44) - u20);               // 0043AB83..0043AB87 / 0043ABAE
        s_dir.x = static_cast<float>(static_cast<double>(u180) - u174);             // 0043AB8B..0043AB99
        Vector tmp{};
        n1 = *vector_normalized_004ab180(&tmp, &t_dir);                             // 0043ABBC..0043ABE8
        Vector tmp2{};
        n2 = *vector_normalized_004ab180(&tmp2, &s_dir);                            // 0043AC03..0043AC1A
        Vector tmp3{};
        Vector cross = *vector_cross_004ab210(&tmp3, &n1, &n2);                     // 0043AC2F..0043AC48
        const U* const normal_index = ptr<U>(get<U>(ptr<void>(get<U>(pp, 0x48) + static_cast<U>(f) * 0x14), 0x10)); // 0043AC55..0043AC66
        const Vector* const normal = ptr<Vector>(get<U>(get<void*>(self, 0x134), 0x54)) + normal_index[0]; // 0043AC68..0043AC6E
        const double dot = (static_cast<double>(normal->x) * cross.x + static_cast<double>(normal->z) * cross.z) // 0043AC7A..0043ACB6
                           + static_cast<double>(normal->y) * cross.y;              // 0043ACBB..0043ACC3
        if (dot < 0.0) {                                                            // 0043ACC5..0043ACD0 (0073D700)
            n2.x = -n2.x; n2.y = -n2.y; n2.z = -n2.z;                               // 0043ACD2..0043AD08
            cross.x = -cross.x; cross.y = -cross.y; cross.z = -cross.z;             // 0043AD04..0043AD3D
        }
        face_basis.row[0] = n1;                                                     // 0043AD44..0043AD7A
        face_basis.row[1] = n2;                                                     // 0043AD5D..0043ADA5
        face_basis.row[2] = cross;                                                  // 0043AD85..0043AD9E
        bases[f] = face_basis;                                                      // 0043ADAC..0043ADBE (rep movsd, 9)
    }
    for (I f = 0; f < get<I>(self, 0x5C); ++f) {                                    // 0043ADEB..0043B302
        const Matrix9 zero{};                                                       // [esp+68h..88h]
        for (I k = 0; k < 3; ++k) {                                                 // 0043AE60..0043B2D8
            const unsigned char* const face = ptr<unsigned char>(get<U>(self, 0x58) + static_cast<U>(f) * 0x20);
            const I vi = get<I16>(face, 0x1A + static_cast<U>(k) * 2);              // 0043AE74..0043AE7B
            const float* const vrec = ptr<float>(ptr<U>(get<U>(get<void*>(self, 0x134), 0x60))[vi]); // 0043AE7F..0043AE88
            const Vector position{vrec[0], vrec[1], vrec[2]};                       // [esp+5Ch] (0043AE8B..0043AE9B)
            List sharing{};                                                         // [esp+38h] (EH state 0)
            sharing.capacity = 0x10;                                                // 0043AEAE..0043AEB5
            sharing.data = addr(live_operator_new(0x40));                           // 0043AEB3..0043AEBE (thunk 006FC0D2)
            ptr<I>(sharing.data)[0] = f;                                            // 0043AEC0..0043AEC4
            const U group = get<U>(ptr<void>(get<U>(get<void*>(self, 0x134), 0x48) + static_cast<U>(f) * 0x14), 8); // 0043AEC7..0043AED8
            sharing.size = 1;                                                       // 0043AEE6..0043AEEF
            for (I g = 0; g < get<I>(self, 0x5C); ++g) {                            // 0043AEF3..0043B03B
                if (g == f) continue;                                               // 0043AF10..0043AF14
                for (I j = 0; j < 3; ++j) {                                         // 0043AF22..0043B011
                    const unsigned char* const other = ptr<unsigned char>(get<U>(self, 0x58) + static_cast<U>(g) * 0x20);
                    const float* const orec = ptr<float>(ptr<U>(get<U>(get<void*>(self, 0x134), 0x60))[get<I16>(other, 0x1A + static_cast<U>(j) * 2)]); // 0043AF30..0043AF44
                    const Vector candidate{orec[0], orec[1], orec[2]};              // [esp+98h] (0043AF47..0043AF6A)
                    if (aurora_vector_equal_004aa980(&position, &candidate) == 0) continue; // 0043AF5D..0043AF7B
                    if ((get<U>(ptr<void>(get<U>(get<void*>(self, 0x134), 0x48) + static_cast<U>(g) * 0x14), 8) & group) == 0) continue; // 0043AF81..0043AF96
                    if (sharing.size == sharing.capacity) {                         // 0043AF98..0043AF9A (inline Add, growth 2x / 16)
                        const U old = sharing.data;
                        sharing.capacity = sharing.capacity ? sharing.capacity * 2 : 0x10; // 0043AF9C..0043AFA8
                        sharing.data = addr(live_operator_new(static_cast<U>(sharing.capacity) * 4)); // 0043AFAC..0043AFB6 (thunk)
                        for (I c = 0; c < sharing.size; ++c) ptr<U>(sharing.data)[c] = ptr<U>(old)[c]; // 0043AFBE..0043AFD9
                        if (old != 0) live_free(ptr<void>(old));                    // 0043AFDF..0043AFE4 (thunk)
                    }
                    ptr<I>(sharing.data)[sharing.size] = g;                         // 0043AFF0..0043AFF4
                    sharing.size = sharing.size + 1;                                // 0043AFF8..0043AFF9
                }
            }
            Matrix9 sum = zero;                                                     // [esp+0A4h] (0043B041..0043B0AA)
            for (I i = 0; i < sharing.size; ++i) {                                  // 0043B098..0043B109
                const Matrix9* const m = &bases[ptr<I>(sharing.data)[i]];
                vector_add_assign_004a9740(&sum.row[0], nullptr, &m->row[0]);       // 0043B0B3..0043B0C9
                vector_add_assign_004a9740(&sum.row[1], nullptr, &m->row[1]);       // 0043B0CE..0043B0E5
                vector_add_assign_004a9740(&sum.row[2], nullptr, &m->row[2]);       // 0043B0EA..0043B101
            }
            vector_normalize_004ab130(&sum.row[0], nullptr);                        // 0043B10B..0043B112
            vector_normalize_004ab130(&sum.row[1], nullptr);                        // 0043B117..0043B11E
            vector_normalize_004ab130(&sum.row[2], nullptr);                        // 0043B123..0043B12A
            Matrix9 transposed;
            matrix_transpose_004a99b0(&transposed.row[0].x, &sum.row[0].x);        // 0043B12F..0043B13F
            sum = transposed;                                                       // 0043B144..0043B156 (rep movsd, 9)
            const float* const s = &sum.row[0].x;
            const I vertex = get<I16>(ptr<void>(get<U>(self, 0x58) + static_cast<U>(f) * 0x20), 0x1A + static_cast<U>(k) * 2); // 0043B158..0043B162
            const U order[9] = {0, 3, 6, 1, 4, 7, 2, 5, 8};                         // store order of the nine fstp dword
            for (U c = 0; c < 9; ++c) {                                             // 0043B158..0043B2B5
                const U row = get<U>(self, 0x14C) * static_cast<U>(vertex) + get<U>(self, 0x194) + get<U>(self, 0x170);
                put<float>(pool, row + c * 4, s[order[c]]);
            }
            if (sharing.data != 0) live_free(ptr<void>(sharing.data));              // 0043B2A8..0043B2BC (thunk; EH state -1)
        }
    }
    live_free(bases);                                                               // 0043B308..0043B30D (thunk 006FC0CD)
    aurora_pool_unlock_00423f30(get<U>(aurora_tree_as_model_0043e1c0(get<MaxTree*>(self, 8), nullptr), 0xAC)); // 0043B312..0043B324
}
