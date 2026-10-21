// Authored LoadExtensions_00436490 lowers (lane-renderer-bodies, D13 round).
// Names/conventions follow the reviewed extension_loader.h declarations; all
// are zero-stack-argument functions with plain RET in the original, so the
// declared __stdcall equals the original physical contract.
#include "5e2fd05edc-renderer_gl_cells.h"
#include "fee770f519-renderer_gl_imports.h"
#include "75b7842bf0-renderer_gl_imports_gen.h"
#include "97677fa6bb-extension_loader.h"
#include "706e60d17a-enable_bindings.h"
#include <cstdint>
namespace {
using namespace kotor_renderer_gl_cells;
template<class T> T load(T& cell) { return *static_cast<volatile T*>(&cell); }
template<class T> void store(T& cell, T value) { *static_cast<volatile T*>(&cell) = value; }
}
namespace kotor_renderer_init {
// 0045F8E0 AurAnisotropicFilterAvailable: cache 0078E534 (-1 cold), mask
// 0078E4A0 (=0x100) read first (0045F8EA), caps 007BB85C (0045F8F0),
// (caps&mask)==mask via and/sub/neg/sbb/inc, store (0045F8FE); warm path
// returns the raw cache word.
I32 __stdcall AnisotropicAvailable_0045f8e0() {
    U32 value = load(cache_anisotropic_0078e534);
    if (value == 0xffffffffu) {
        const U32 mask = load(mask_0078e4a0);
        value = ((load(capabilities_007bb85c) & mask) == mask) ? 1u : 0u;
        store(cache_anisotropic_0078e534, value);
    }
    return static_cast<I32>(value);
}
}
namespace kotor_renderer_gl {
using namespace kotor_renderer_init;
// 0045F680 AurBumpyShinyAvailable (cdecl, no args). Cold path:
//   0045F68D..0045F6BE required1 = 49C|4A8|490|48C|484|480 (EDI is reloaded
//   with 484 at 0045F6B6, so 48C participates only in required1);
//   0045F6C0..0045F6CB cache = (caps&required1)==required1.
//   0045F6D0 nonzero result returns. Otherwise 0045F6D2..0045F6FF:
//   required2 = 4D4|4D0|490|484|480; cache = ((caps&required2)==required2)
//   & control 0078E4FC & programs 0078E5EC (full-word ANDs); returned in EAX.
// Warm path returns the raw cache word.
U32 __cdecl BumpyShinyAvailable_0045f680() {
    U32 value = load(cache_bumpy_shiny_0078e510);
    if (value != 0xffffffffu) return value;
    const U32 m4a8 = load(mask_0078e4a8), m49c = load(mask_0078e49c), m490 = load(mask_0078e490);
    const U32 m48c = load(mask_0078e48c), m480 = load(mask_0078e480);
    const U32 caps = load(capabilities_007bb85c);
    const U32 m484 = load(mask_0078e484);
    const U32 required1 = m49c | m4a8 | m490 | m48c | m484 | m480;
    value = ((caps & required1) == required1) ? 1u : 0u;
    store(cache_bumpy_shiny_0078e510, value);
    if (value != 0) return value;
    const U32 m4d0 = load(mask_0078e4d0), m4d4 = load(mask_0078e4d4);
    const U32 control = load(control_0078e4fc);
    const U32 required2 = m4d4 | m4d0 | m490 | m484 | m480;
    U32 second = ((caps & required2) == required2) ? 1u : 0u;
    second = second & control & static_cast<U32>(load(programs_0078e5ec));
    store(cache_bumpy_shiny_0078e510, second);
    return second;
}
}
namespace kotor_renderer_init {
// 0045FC90 AurVertexBufferObjectARBAvailable.
//   0045FC90 byte 007B927D == 1 -> return 0 (no cache access).
//   0045FC9D warm cache 0078E548 returned raw.
//   Cold: 0045FCA8 mask 0078E4D8, caps -> v=(caps&m)==m; 0045FCB7 level
//   0078E500 read; 0045FCC4 store v; 0045FCC1/CA level<2 (signed) -> v=0,store;
//   else 0045FCCC BumpyShiny, nonzero EAX continues, zero -> v=0,store;
//   0045FCE2 ValidateNonRadeon (AL) zero -> v=0,store (0045FCEB); return v.
U32 __stdcall VertexBufferAvailable_0045fc90() {
    if (load(vertex_buffer_disabled_007b927d) == 1) return 0;
    U32 value = load(cache_vertex_buffer_0078e548);
    if (value != 0xffffffffu) return value;
    const U32 mask = load(mask_0078e4d8);
    value = ((load(capabilities_007bb85c) & mask) == mask) ? 1u : 0u;
    const I32 level = load(vertex_buffer_level_0078e500);
    store(cache_vertex_buffer_0078e548, value);
    if (level < 2 || kotor_renderer_gl::BumpyShinyAvailable_0045f680() == 0) {
        value = 0;
        store(cache_vertex_buffer_0078e548, value);
    }
    if (kotor_vertex_program::ValidateNonRadeon_0045f5e0() == 0) {
        value = 0;
        store(cache_vertex_buffer_0078e548, value);
    }
    return value;
}
// 004263C0 GLRender::InitAttenuationTexture. 0x4004-byte frame (_chkstk
// 006FC090): 64x64 RGBA bytes at esp+14h. For row r (EBP, 004263CE..00426469)
// and column c (EDI): x=(r-32)*0.125f [0073F400], y=(c-32)*0.125f,
// v=(float)(1.0f[0073D764] / ((y*y+x*x)*4.5f[0073F3FC] + 1.0f)) stored to a
// float (00426422); v=0 when c==0||r==0||c==63||r==63 (004263F7..00426438);
// byte=_ftol2(v*255.0f[0073EDE4]) low byte (0042644A) written to R,G,B and
// A=0xFF (BL). x87 work is at the CRT's 53-bit precision control, i.e. IEEE
// double arithmetic, which the double expressions below reproduce.
// Then 0042646F..00426534: glGenTextures(1,&007A68A0); glBindTexture(
// [0078D458], tex); glTexParameteri([0078D458],0x84FE,[007A685C]);
// stage_textures[[007A6898]] = tex (004264C0, before the next call);
// four glTexParameteri(0x0DE1,...); glTexImage2D(0x0DE1,0,0x8051,64,64,0,
// 0x1908,0x1401,buffer); glTexParameteri(0x0DE1,0x84FE,[007A685C]).
void __stdcall InitAttenuationTexture_004263c0() {
    U8 pixels[64 * 64 * 4];
    U8* out = pixels;
    for (I32 row = 0; row < 64; ++row) {
        const double x = static_cast<double>(row - 32) * 0.125;
        const double x2 = x * x;
        for (I32 column = 0; column < 64; ++column) {
            const double y = static_cast<double>(column - 32) * 0.125;
            float value = static_cast<float>(1.0 / ((y * y + x2) * 4.5 + 1.0));
            if (column == 0 || row == 0 || column == 63 || row == 63) value = 0.0f;
            const U8 level = static_cast<U8>(static_cast<std::int64_t>(static_cast<double>(value) * 255.0));
            out[0] = level; out[1] = level; out[2] = level; out[3] = 0xff;
            out += 4;
        }
    }
    glGenTextures_0073d24c(1, &attenuation_texture_007a68a0);
    const U32 texture = load(attenuation_texture_007a68a0);
    glBindTexture_0073d240(load(kotor_renderer_cells::enable_0078d458), texture);
    const I32 anisotropy = load(texture_upload_cells::anisotropy_007a685c);  // 00426491 read before target 00426496
    glTexParameteri_0073d23c(load(kotor_renderer_cells::enable_0078d458), 0x84fe, anisotropy);
    const U32 stage = load(current_stage_007a6898);
    store(current_tid_007a683c[stage], texture);
    glTexParameteri_0073d23c(0x0de1, 0x2802, 0x812f);
    glTexParameteri_0073d23c(0x0de1, 0x2803, 0x812f);
    glTexParameteri_0073d23c(0x0de1, 0x2800, 0x2601);
    glTexParameteri_0073d23c(0x0de1, 0x2801, 0x2601);
    glTexImage2D_0073d280(0x0de1, 0, 0x8051, 64, 64, 0, 0x1908, 0x1401, pixels);
    glTexParameteri_0073d23c(0x0de1, 0x84fe, load(texture_upload_cells::anisotropy_007a685c));
}
}
