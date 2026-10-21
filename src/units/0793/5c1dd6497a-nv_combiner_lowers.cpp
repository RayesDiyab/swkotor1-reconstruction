// Fork F2 lowers: ActivateTextureStage 0041FE80, AurMultiTextureAvailable 0045F710,
// DisableCombiners 00423CD0, MakeLookUpTexture 00423D80, Homogenize 004AAAB0,
// SetFragmentShaderConstant0 00424940, SetCombinerConstant1 00424960.
#include "0a27d352e1-nv_combiners.h"
#include "fee770f519-renderer_gl_imports.h"
#include "75b7842bf0-renderer_gl_imports_gen.h"
#include "61e3cacb4e-renderer_gl_procs.h"
#include "5e2fd05edc-renderer_gl_cells.h"
#include "3462b4664f-fork_cells_f2.h"
#include "39c670fc66-allocator_live.h"
#include <cstring>
// Original x87 helpers: 006FAE8C __ftol2 (M41 owner world-timer-x87-conversion.obj,
// ST0 in, EAX out) and 006FC1F0 _CIpow (ST1^ST0, statically linked MSVC CRT);
// the pinned libucrt supplies the same _CIpow entry.
extern "C" unsigned __int64 __cdecl unresolved_ftol2_006fae8c();
extern "C" void __cdecl _CIpow();
namespace kotor_renderer_gl {
namespace {
using U32 = std::uint32_t;
using I32 = std::int32_t;
using U8 = std::uint8_t;
U32 load(const U32& cell) { return *static_cast<const volatile U32*>(&cell); }
void store(U32& cell, U32 value) { *static_cast<volatile U32*>(&cell) = value; }
// .rdata constants read by 00423DC4/00423DD3 (double) and 00423DF4/00423DFA (float).
const double color_unit_1_255_0073f3f0 = 0x1.0101010101010p-8;  // bits 3F70101010101010
const double scale_0073f3e8 = 0x1.FFFAE147AE148p+7;             // bits 406FFFAE147AE148
const float one_255th_0073ede0 = 0x1.010102p-8f;                // bits 3B808081
const float scale_0073f3e0 = 0x1.FFFAE2p+7f;                    // bits 437FFD71
}
using namespace kotor_renderer_gl_cells;

// 0045F710: cache 0078E514 (-1 cold); mask 0078E484 loaded before caps (0045F71A/
// 0045F720); and/sub/neg/sbb/inc => (caps&mask)==mask; warm path returns raw cache.
// [M47 compose] 0045F710 single body = earliest-staged 3d-render scene3d aur_multi_texture_available_0045f710 (M45; D68); name joined (definition removed; declaration kept)
std::uint32_t __cdecl MultitextureAvailable_0045f710();

// 0041FE80: store stage (0041FE84); predicate (0041FE89); reread 007A6898
// (0041FE92) and glActiveTextureARB(stage+84C0h) through slot 007BB80C (0041FE9F).
void __cdecl ActivateTextureStage_0041fe80(std::uint32_t stage) {
    store(current_stage_007a6898, stage);
    if (MultitextureAvailable_0045f710() != 0)
        kotor_renderer_gl_procs::glActiveTextureARB_007bb80c(load(current_stage_007a6898) + 0x84c0u);
}

// 00423CD0. ESI = glDisable IAT 0073D22C (00423CD1).
void __cdecl DisableCombiners_00423cd0(texture::Basic*) {
    kotor_renderer_init::glDisable_0073d22c(0x8522u);                       // 00423CDC
    store(current_stage_007a6898, 1);                                       // 00423CDE
    if (MultitextureAvailable_0045f710() != 0)                              // 00423CE8
        kotor_renderer_gl_procs::glActiveTextureARB_007bb80c(load(current_stage_007a6898) + 0x84c0u); // 00423CF1..00423CFC
    kotor_renderer_init::glDisable_0073d22c(0x8513u);                       // 00423D07
    kotor_renderer_init::glDisable_0073d22c(0x0c60u);                       // 00423D0E
    kotor_renderer_init::glDisable_0073d22c(0x0c61u);                       // 00423D15
    kotor_renderer_init::glDisable_0073d22c(0x0c62u);                       // 00423D1C
    kotor_renderer_init::glDisable_0073d22c(0x0de1u);                       // 00423D23
    const U32 stage = load(current_stage_007a6898);                         // 00423D25
    store(env_map_on_00827fa0, 0);                                          // 00423D2D
    store(current_tid_007a683c[stage], 0);                                  // 00423D33
    store(tex1_on_00827fa4, 0);                                             // 00423D3A
    store(current_stage_007a6898, 0);                                       // 00423D40
    if (MultitextureAvailable_0045f710() != 0)                              // 00423D46
        kotor_renderer_gl_procs::glActiveTextureARB_007bb80c(load(current_stage_007a6898) + 0x84c0u); // 00423D4F..00423D5C
    kotor_renderer_init::glColor4f_0073d224(0.0f, 0.0f, 0.0f, 1.0f);        // 00423D62..00423D6A (ESI=0, 3F800000h)
}

// 00423D80. glIsTexture(lookUpTexture) AL nonzero -> return (00423D8F/00423D91).
void __cdecl MakeLookUpTexture_00423d80(float exponent) {
    if (kotor_renderer_init::glIsTexture_0073d27c(load(look_up_texture_007bb868)) != 0) return;
    // 00423DA0: operator new(0x20000); the original performs no null check.
    U8* const buffer = static_cast<U8*>(live_operator_new(0x20000u));
    U8* out = buffer;
    for (I32 row = 0; row < 0x100; ++row) {                                 // [esp+10h], 00423E18..00423E26
        U8 high_byte;
        const double* unit = &color_unit_1_255_0073f3f0;
        const double* scale = &scale_0073f3e8;
        __asm {
            fild row                       // 00423DC0
            mov eax, unit
            fmul qword ptr [eax]           // 00423DC4
            fld exponent                   // 00423DCA
            call _CIpow                    // 00423DCE (ST1^ST0)
            mov eax, scale
            fmul qword ptr [eax]           // 00423DD3
            call unresolved_ftol2_006fae8c // 00423DD9
            mov high_byte, al                   // 00423DE0
        }
        for (I32 column = 0; column < 0x100; ++column) {                    // [esp+14h], 00423E0B..00423E16
            U8 low_byte;
            const float* f1 = &one_255th_0073ede0;
            const float* f2 = &scale_0073f3e0;
            __asm {
                fild column                    // 00423DF0
                mov eax, f1
                fmul dword ptr [eax]           // 00423DF4
                mov eax, f2
                fmul dword ptr [eax]           // 00423DFA
                call unresolved_ftol2_006fae8c // 00423E00
                mov low_byte, al
            }
            *out++ = low_byte;                                                   // 00423E05
            *out++ = high_byte;                                                  // 00423E08
        }
    }
    kotor_renderer_init::glGenTextures_0073d24c(1, &look_up_texture_007bb868);           // 00423E2F
    kotor_renderer_init::glBindTexture_0073d240(0x0de1u, load(look_up_texture_007bb868)); // 00423E35..00423E41
    const U32 stage = load(current_stage_007a6898);                                      // 00423E47
    store(current_tid_007a683c[stage], 0);                                               // 00423E6C
    // 00423E77 thunk 0070C120 -> glu32 gluBuild2DMipmaps (IAT 0073D030); result ignored.
    (void)kotor_renderer_init::gluBuild2DMipmaps_0073d030(0x0de1u, 0x190a, 0x100, 0x100, 0x190au, 0x1401u, buffer);
    kotor_renderer_init::glTexParameteri_0073d23c(0x0de1u, 0x2802u, 0x812f);            // 00423E91
    kotor_renderer_init::glTexParameteri_0073d23c(0x0de1u, 0x2803u, 0x812f);            // 00423EA2
    kotor_renderer_init::glTexParameteri_0073d23c(0x0de1u, 0x2800u, 0x2601);            // 00423EB3
    kotor_renderer_init::glTexParameteri_0073d23c(0x0de1u, 0x2801u, 0x2703);            // 00423EC4
    live_free(buffer);                                                                   // 00423EC7 (006FC0CD)
}

// 004AAAB0: raw DWORD copies in original order xyz[0],xyz[1],xyz[2],*w; returns 00831F18.
// [M47 compose] 004AAAB0 single body = earliest-staged 3d-render scene3d homogenize_004aaab0 (M45; D68); name joined (definition removed; declaration kept)
float* __cdecl Homogenize_004aaab0(const float* xyz, const float* w);

// 00424940: Homogenize(vector, &w) then glCombinerParameterfvNV(852Ah) via slot 007BB7A8.
void __cdecl SetFragmentShaderConstant0_00424940(const float* vector, float w) {
    const float* constant = Homogenize_004aaab0(vector, &w);                      // 0042494A
    kotor_renderer_gl_procs::glCombinerParameterfvNV_007bb7a8(0x852au, constant); // 00424958
}
// 00424960: same with 852Bh (00424973/00424978).
void __cdecl SetCombinerConstant1_00424960(const float* vector, float w) {
    const float* constant = Homogenize_004aaab0(vector, &w);                      // 0042496A
    kotor_renderer_gl_procs::glCombinerParameterfvNV_007bb7a8(0x852bu, constant); // 00424978
}
}
