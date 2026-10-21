// Normalization cube map closure of the ATI route (lane-renderer-bodies, D13 round):
// 00425EA0
// GLRender::MakeNormalizationCubeMap and its inline helper 00425D80.
// x87 work runs at the CRT's 53-bit precision control: the double expressions
// below reproduce it, and each fstp to a float cell is a (float) conversion.
#include "536853f13e-renderer_gl_cells.h"
#include "2078381901-renderer_gl_imports.h"
#include "ceeafce552-renderer_gl_imports_gen.h"
#include "f13dfb9d7c-renderer_gl_procs.h"
#include "a3091fe3c6-render_shared_lowers.h"
#include "ed8f77ad6d-allocator_live.h"
#include <cmath>
#include <cstdint>
#include <cstring>
namespace kotor_renderer_gl_cells {
// 0078D47C (GL_NORMALIZE enum cell) is owned by lane-gui-draw (D32); not used here.
// 0078DB68 .data face -> target-table index {2,3,4,5,6,7} (read 00425FBC only).
U32 cube_face_index_0078db68[6] = {2u, 3u, 4u, 5u, 6u, 7u};
// 0073F23C .rdata texture-target table; entries 0..7 are read (00425FC3, 00420D09/00420D20).
// lane-m48-a: `extern` gives the namespace-scope const table external linkage (C++ const => internal by default), so the
// texture-epoch consumer (texture_normalize_cubemap.cpp, 00420D09/00420D20) links to this single .rdata image (no second copy).
extern const U32 texture_target_table_0073f23c[8] = {0x0de1u, 0x8513u, 0x8515u, 0x8516u, 0x8517u, 0x8518u, 0x8519u, 0x851au};
}
namespace {
using namespace kotor_renderer_gl_cells;
using kotor_renderer_init::I32;
template<class T> T load(const T& cell) { return *static_cast<const volatile T*>(&cell); }
template<class T> void store(T& cell, T value) { *static_cast<volatile T*>(&cell) = value; }
std::uint8_t to_byte(float component) {
    // fld [v]; fmul 127.0f [0073EDDC]; fadd 128.0f [0073EDD8]; call _ftol2 -> AL.
    return static_cast<std::uint8_t>(static_cast<std::int64_t>(static_cast<double>(component) * 127.0 + 128.0));
}
// 00425D80 inline_MakeNormalizationCubeMap: ECX=face, EAX=out float[3], cdecl
// stack (size, column, row). a=((column+0.5)/(float)size)*2-1 stays on the x87
// stack; b (row) is rounded to float at 00425DB7. Jump table 00425E80:
//   0 00425DC8 ( 1,-b,-a)  1 00425DDE (-1,-b, a)  2 00425DF2 ( a, 1, b)
//   3 00425E04 ( a,-1,-b)  4 00425E18 ( a,-b, 1)  5 00425E2C (-a,-b,-1)
// then 00425E44..00425E7B: len=sqrt((x*x+y*y)+z*z), inv=1/len, each *=inv.
// Faces >5 (00425E42) normalise the caller's untouched vector; the only caller
// (00425F43) passes 0..5, so that path is unreachable here.
void face_vector_00425d80(I32 face, float* out, I32 size, I32 column, I32 row) {
    const float size_f = static_cast<float>(size);
    const double a = ((static_cast<double>(column) + 0.5) / size_f) * 2.0 - 1.0;
    const float b = static_cast<float>(((static_cast<double>(row) + 0.5) / size_f) * 2.0 - 1.0);
    switch (face) {
    case 0: out[0] = 1.0f; out[1] = -b; out[2] = static_cast<float>(-a); break;
    case 1: out[0] = -1.0f; out[1] = -b; out[2] = static_cast<float>(a); break;
    case 2: out[0] = static_cast<float>(a); out[1] = 1.0f; out[2] = b; break;
    case 3: out[0] = static_cast<float>(a); out[1] = -1.0f; out[2] = -b; break;
    case 4: out[0] = static_cast<float>(a); out[2] = 1.0f; out[1] = -b; break;
    default: out[2] = -1.0f; out[0] = static_cast<float>(-a); out[1] = -b; break;  // case 5
    }
    const double x = out[0], y = out[1], z = out[2];
    const double inv = 1.0 / std::sqrt((x * x + y * y) + z * z);
    out[0] = static_cast<float>(inv * x);
    out[1] = static_cast<float>(inv * y);
    out[2] = static_cast<float>(inv * z);
}
}
namespace kotor_renderer_gl {
using namespace kotor_renderer_init;
// 00425EA0 GLRender::MakeNormalizationCubeMap (cdecl, one stack word, RET at 00425FF9).
//   00425EB4 buffer = operator new(size*size*3) (006FC0D2); no null test.
//   00425EC2..00425F0A four glTexParameteri(0x8513,...) through the IAT word cached in ESI.
//   Per face 0..5 (EBP): rows x columns fill RGB bytes from 00425D80 (00425F36..00425FB2);
//   00425FBC..00425FDA gluBuild2DMipmaps(table[face_index[face]],3,size,size,0x1907,0x1401,buffer)
//   through thunk 0070C120 (jmp [0073D030]). 00425FEA free(buffer) (006FC0CD).
void __cdecl MakeNormalizationCubeMap_00425ea0(I32 size) {
    auto* buffer = static_cast<std::uint8_t*>(live_operator_new(static_cast<U32>(size * size * 3)));
    glTexParameteri_0073d23c(0x8513, 0x2802, 0x812f);
    glTexParameteri_0073d23c(0x8513, 0x2803, 0x812f);
    glTexParameteri_0073d23c(0x8513, 0x2800, 0x2601);
    glTexParameteri_0073d23c(0x8513, 0x2801, 0x2601);
    for (I32 face = 0; face < 6; ++face) {
        std::uint8_t* row_start = buffer;
        for (I32 row = 0; row < size; ++row) {
            std::uint8_t* out = row_start;
            for (I32 column = 0; column < size; ++column) {
                float vector[3];
                face_vector_00425d80(face, vector, size, column, row);
                out[0] = to_byte(vector[0]);
                out[1] = to_byte(vector[1]);
                out[2] = to_byte(vector[2]);
                out += 3;
            }
            row_start += size * 3;
        }
        const U32 target = texture_target_table_0073f23c[load(cube_face_index_0078db68[face])];
        gluBuild2DMipmaps_0073d030(target, 3, size, size, 0x1907, 0x1401, buffer);
    }
    live_free(buffer);
}
}
// 00420C20 CAurTextureBasic::AsNormalizeVectorCubeMap is owned by lane-texture-epoch (D32/D48);
// it calls MakeNormalizationCubeMap_00425ea0 above.
