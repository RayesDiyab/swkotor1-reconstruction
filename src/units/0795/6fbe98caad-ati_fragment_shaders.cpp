// Fork F3 (lane-renderer-bodies): ATI fragment-shader route. Every statement cites the
// original instruction address it reconstructs. Extension slots are re-read at each call
// (original call dword ptr [slot]); imports go through the single kotor_renderer_init owner.
// InitializeATIFragmentShaders' 153 statements are mechanically translated from the original
// straight-line sequence by fork-f3/tools/translate_linear.py (every operand is an immediate).
#include "fadbb6dacf-ati_fragment_shaders.h"
#include "d700c0f93a-fork_cells_f3.h"
#include "fd1e4c71e2-texture_externals_f3.h"
#include "ccf5cb551c-render_shared_lowers.h"
#include "5e2fd05edc-renderer_gl_cells.h"
#include "61e3cacb4e-renderer_gl_procs.h"
#include "75b7842bf0-renderer_gl_imports_gen.h"
#include "39c670fc66-allocator_live.h"
namespace {
namespace P = kotor_renderer_gl_procs;
using U32 = std::uint32_t;
using kotor_renderer_gl::ActivateTextureStage_0041fe80;
using kotor_renderer_gl::MakeLookUpTexture_00423d80;
using kotor_renderer_gl::Homogenize_004aaab0;
void enable(U32 cap) { kotor_renderer_init::glEnable_0073d390(cap); }
void disable(U32 cap) { kotor_renderer_init::glDisable_0073d22c(cap); }
// glTexGeni(coord, GL_TEXTURE_GEN_MODE 2500h, param) via IAT 0073D314.
void tex_gen(U32 coord, U32 param) { kotor_renderer_init::glTexGeni_0073d314(coord, 0x2500u, static_cast<std::int32_t>(param)); }
U32 load(const U32& cell) { return *static_cast<const volatile U32*>(&cell); }
void store(U32& cell, U32 value) { *static_cast<volatile U32*>(&cell) = value; }
// [reg*4+007A683C] = 0 with reg = the live 007A6898 word (e.g. 0047A4A6..0047A4B0).
void clear_current_stage_texture() {
    using namespace kotor_renderer_gl_cells;
    const U32 stage = load(current_stage_007a6898);
    store(current_tid_007a683c[stage], 0u);
}
void bind_shader(const U32& cell) { P::glBindFragmentShaderATI_007bb6cc(load(cell)); }
}
namespace kotor_renderer_gl {
const std::uint8_t* __cdecl AurGetGLError_0044eee0() {
    const U32 error = kotor_renderer_init::glGetError_0073d2c0();      // 0044EEE0 call [0073D2C0]
    if (error == 0) return nullptr;                                    // 0044EEE8 je: returns EAX = 0
    return kotor_renderer_init::gluErrorString_0073d028(error);        // 0044EEEB call 0070C12C (jmp [0073D028])
}
}
namespace kotor_renderer_init {
void __stdcall InitializeATIFragmentShaders_004794e0() {
    using kotor_renderer_gl::AurGetGLError_0044eee0;
    kotor_renderer_init::glEnable_0073d390(0x8920u);  // 004794E6
    const auto r1 = P::glGenFragmentShadersATI_007bb754(0x1u);  // 004794EE
    *static_cast<volatile U32*>(&kotor_renderer_gl_cells::ati_fragment_shader_00830244) = r1;  // 004794F5
    P::glBindFragmentShaderATI_007bb6cc(r1);  // 004794FA
    P::glBeginFragmentShaderATI_007bb7f4();  // 00479500
    P::glSampleMapATI_007bb620(0x8921u, 0x84c0u, 0x8976u);  // 00479515
    P::glSampleMapATI_007bb620(0x8922u, 0x84c1u, 0x8976u);  // 0047952A
    P::glColorFragmentOp2ATI_007bb840(0x8963u, 0x8923u, 0x0u, 0x0u, 0x8577u, 0x0u, 0x0u, 0x8922u, 0x0u, 0x0u);  // 00479550
    P::glAlphaFragmentOp2ATI_007bb5e8(0x8964u, 0x8921u, 0x0u, 0x8577u, 0x0u, 0x0u, 0x8921u, 0x0u, 0x0u);  // 00479574
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8921u, 0x0u, 0x0u, 0x8921u, 0x0u, 0x0u, 0x8923u, 0x0u, 0x0u);  // 0047959A
    P::glEndFragmentShaderATI_007bb554();  // 004795A0
    const auto r2 = P::glGenFragmentShadersATI_007bb754(0x1u);  // 004795A8
    *static_cast<volatile U32*>(&kotor_renderer_gl_cells::ati_fragment_shader_0083024c) = r2;  // 004795AF
    P::glBindFragmentShaderATI_007bb6cc(r2);  // 004795B4
    P::glBeginFragmentShaderATI_007bb7f4();  // 004795BA
    P::glSampleMapATI_007bb620(0x8921u, 0x84c0u, 0x8976u);  // 004795CF
    P::glSampleMapATI_007bb620(0x8922u, 0x84c1u, 0x8976u);  // 004795E4
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8921u, 0x7u, 0x0u, 0x8921u, 0x0u, 0x0u, 0x8577u, 0x0u, 0x0u);  // 0047960A
    P::glColorFragmentOp3ATI_007bb7fc(0x8969u, 0x8921u, 0x7u, 0x0u, 0x8921u, 0x1906u, 0x0u, 0x8921u, 0x0u, 0x0u, 0x8922u, 0x0u, 0x0u);  // 0047963C
    P::glColorFragmentOp3ATI_007bb7fc(0x8969u, 0x8921u, 0x7u, 0x0u, 0x896du, 0x1903u, 0x0u, 0x8942u, 0x0u, 0x0u, 0x8921u, 0x0u, 0x0u);  // 0047966E
    P::glAlphaFragmentOp1ATI_007bb710(0x8961u, 0x8921u, 0x0u, 0x8941u, 0x0u, 0x0u);  // 00479689
    P::glEndFragmentShaderATI_007bb554();  // 0047968F
    const auto r3 = P::glGenFragmentShadersATI_007bb754(0x1u);  // 00479697
    *static_cast<volatile U32*>(&kotor_renderer_gl_cells::ati_fragment_shader_00830248) = r3;  // 0047969E
    P::glBindFragmentShaderATI_007bb6cc(r3);  // 004796A3
    P::glBeginFragmentShaderATI_007bb7f4();  // 004796A9
    P::glSampleMapATI_007bb620(0x8921u, 0x84c0u, 0x8976u);  // 004796BE
    P::glSampleMapATI_007bb620(0x8922u, 0x84c1u, 0x8976u);  // 004796D3
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8921u, 0x7u, 0x0u, 0x8921u, 0x0u, 0x0u, 0x8577u, 0x0u, 0x0u);  // 004796F9
    P::glColorFragmentOp3ATI_007bb7fc(0x8969u, 0x8921u, 0x7u, 0x0u, 0x8921u, 0x1906u, 0x0u, 0x8921u, 0x0u, 0x0u, 0x8922u, 0x0u, 0x0u);  // 0047972B
    P::glAlphaFragmentOp1ATI_007bb710(0x8961u, 0x8921u, 0x0u, 0x8941u, 0x0u, 0x0u);  // 00479746
    P::glEndFragmentShaderATI_007bb554();  // 0047974C
    const auto r4 = P::glGenFragmentShadersATI_007bb754(0x1u);  // 00479754
    *static_cast<volatile U32*>(&kotor_renderer_gl_cells::ati_fragment_shader_00830250) = r4;  // 0047975A
    P::glBindFragmentShaderATI_007bb6cc(r4);  // 00479760
    P::glBeginFragmentShaderATI_007bb7f4();  // 00479766
    P::glSampleMapATI_007bb620(0x8921u, 0x84c0u, 0x8976u);  // 0047977B
    P::glSampleMapATI_007bb620(0x8922u, 0x84c1u, 0x8976u);  // 00479790
    P::glSampleMapATI_007bb620(0x8923u, 0x84c2u, 0x8976u);  // 004797A5
    P::glSampleMapATI_007bb620(0x8924u, 0x84c3u, 0x8976u);  // 004797BA
    P::glColorFragmentOp2ATI_007bb840(0x8966u, 0x8926u, 0x0u, 0x0u, 0x8921u, 0x0u, 0x9u, 0x8923u, 0x0u, 0x9u);  // 004797E0
    P::glColorFragmentOp2ATI_007bb840(0x8966u, 0x8925u, 0x0u, 0x0u, 0x8921u, 0x0u, 0x9u, 0x8922u, 0x0u, 0x9u);  // 00479806
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8921u, 0x0u, 0x0u, 0x8926u, 0x0u, 0x0u, 0x8926u, 0x0u, 0x0u);  // 0047982C
    P::glColorFragmentOp1ATI_007bb6d4(0x8961u, 0x8926u, 0x0u, 0x0u, 0x8926u, 0x1905u, 0x0u);  // 0047984C
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8921u, 0x0u, 0x0u, 0x8925u, 0x0u, 0x0u, 0x8942u, 0x0u, 0x0u);  // 00479872
    P::glAlphaFragmentOp2ATI_007bb5e8(0x8964u, 0x8926u, 0x0u, 0x8926u, 0x1903u, 0x0u, 0x8926u, 0x1903u, 0x0u);  // 0047989C
    P::glPassTexCoordATI_007bb804(0x8921u, 0x8921u, 0x8976u);  // 004798B1
    P::glPassTexCoordATI_007bb804(0x8922u, 0x8922u, 0x8976u);  // 004798C6
    P::glPassTexCoordATI_007bb804(0x8923u, 0x8923u, 0x8976u);  // 004798DB
    P::glPassTexCoordATI_007bb804(0x8924u, 0x8924u, 0x8976u);  // 004798F0
    P::glPassTexCoordATI_007bb804(0x8925u, 0x8925u, 0x8976u);  // 00479905
    P::glPassTexCoordATI_007bb804(0x8926u, 0x8926u, 0x8976u);  // 0047991A
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8921u, 0x0u, 0x0u, 0x8925u, 0x0u, 0x0u, 0x896du, 0x0u, 0x0u);  // 00479940
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8922u, 0x0u, 0x0u, 0x896du, 0x0u, 0x0u, 0x8941u, 0x0u, 0x0u);  // 00479966
    P::glAlphaFragmentOp2ATI_007bb5e8(0x8964u, 0x8922u, 0x0u, 0x8926u, 0x1906u, 0x0u, 0x8926u, 0x1906u, 0x0u);  // 00479990
    P::glColorFragmentOp2ATI_007bb840(0x8963u, 0x8921u, 0x0u, 0x40u, 0x8577u, 0x0u, 0x0u, 0x8921u, 0x0u, 0x0u);  // 004799B6
    P::glAlphaFragmentOp2ATI_007bb5e8(0x8964u, 0x8922u, 0x0u, 0x8922u, 0x1906u, 0x0u, 0x8922u, 0x1906u, 0x0u);  // 004799E0
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8921u, 0x0u, 0x0u, 0x8921u, 0x0u, 0x0u, 0x8924u, 0x0u, 0x0u);  // 00479A06
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8922u, 0x0u, 0x0u, 0x8922u, 0x1906u, 0x0u, 0x8922u, 0x0u, 0x0u);  // 00479A2F
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8922u, 0x0u, 0x0u, 0x8922u, 0x0u, 0x0u, 0x8924u, 0x1906u, 0x2u);  // 00479A58
    P::glColorFragmentOp2ATI_007bb840(0x8963u, 0x8921u, 0x0u, 0x40u, 0x8922u, 0x0u, 0x0u, 0x8921u, 0x0u, 0x0u);  // 00479A7E
    P::glAlphaFragmentOp1ATI_007bb710(0x8961u, 0x8921u, 0x0u, 0x8577u, 0x1906u, 0x0u);  // 00479A9C
    P::glEndFragmentShaderATI_007bb554();  // 00479AA2
    { const U32 error = kotor_renderer_init::glGetError_0073d2c0();  // 00479AAE
      if (error != 0) (void)kotor_renderer_init::gluErrorString_0073d028(error); }  // 00479AB5 via thunk 0070C12C
    const auto r5 = P::glGenFragmentShadersATI_007bb754(0x1u);  // 00479ABC
    *static_cast<volatile U32*>(&kotor_renderer_gl_cells::ati_fragment_shader_00830254) = r5;  // 00479AC3
    P::glBindFragmentShaderATI_007bb6cc(r5);  // 00479AC8
    P::glBeginFragmentShaderATI_007bb7f4();  // 00479ACE
    P::glSampleMapATI_007bb620(0x8921u, 0x84c0u, 0x8976u);  // 00479AE3
    P::glSampleMapATI_007bb620(0x8922u, 0x84c1u, 0x8976u);  // 00479AF8
    P::glSampleMapATI_007bb620(0x8923u, 0x84c2u, 0x8976u);  // 00479B0D
    P::glSampleMapATI_007bb620(0x8924u, 0x84c3u, 0x8976u);  // 00479B22
    P::glColorFragmentOp2ATI_007bb840(0x8966u, 0x8926u, 0x0u, 0x0u, 0x8922u, 0x0u, 0x9u, 0x8924u, 0x0u, 0x9u);  // 00479B48
    P::glColorFragmentOp2ATI_007bb840(0x8966u, 0x8925u, 0x0u, 0x0u, 0x8922u, 0x0u, 0x9u, 0x8923u, 0x0u, 0x9u);  // 00479B6E
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8922u, 0x0u, 0x0u, 0x8926u, 0x0u, 0x0u, 0x8926u, 0x0u, 0x0u);  // 00479B94
    P::glColorFragmentOp1ATI_007bb6d4(0x8961u, 0x8926u, 0x0u, 0x0u, 0x8926u, 0x1905u, 0x0u);  // 00479BB4
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8922u, 0x0u, 0x0u, 0x8925u, 0x0u, 0x0u, 0x8942u, 0x0u, 0x0u);  // 00479BDA
    P::glAlphaFragmentOp2ATI_007bb5e8(0x8964u, 0x8926u, 0x0u, 0x8926u, 0x1903u, 0x0u, 0x8926u, 0x1903u, 0x0u);  // 00479C04
    P::glPassTexCoordATI_007bb804(0x8921u, 0x8921u, 0x8976u);  // 00479C19
    P::glPassTexCoordATI_007bb804(0x8922u, 0x8922u, 0x8976u);  // 00479C2E
    P::glPassTexCoordATI_007bb804(0x8923u, 0x8923u, 0x8976u);  // 00479C43
    P::glPassTexCoordATI_007bb804(0x8924u, 0x8924u, 0x8976u);  // 00479C58
    P::glPassTexCoordATI_007bb804(0x8925u, 0x8925u, 0x8976u);  // 00479C6D
    P::glPassTexCoordATI_007bb804(0x8926u, 0x8926u, 0x8976u);  // 00479C82
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8922u, 0x0u, 0x0u, 0x8925u, 0x0u, 0x0u, 0x896du, 0x0u, 0x0u);  // 00479CA8
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8923u, 0x0u, 0x0u, 0x896du, 0x0u, 0x0u, 0x8941u, 0x0u, 0x0u);  // 00479CCE
    P::glAlphaFragmentOp2ATI_007bb5e8(0x8964u, 0x8923u, 0x0u, 0x8926u, 0x1906u, 0x0u, 0x8926u, 0x1906u, 0x0u);  // 00479CF8
    P::glColorFragmentOp2ATI_007bb840(0x8963u, 0x8922u, 0x0u, 0x40u, 0x8577u, 0x0u, 0x0u, 0x8922u, 0x0u, 0x0u);  // 00479D1E
    P::glAlphaFragmentOp2ATI_007bb5e8(0x8964u, 0x8923u, 0x0u, 0x8923u, 0x1906u, 0x0u, 0x8923u, 0x1906u, 0x0u);  // 00479D48
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8922u, 0x0u, 0x0u, 0x8922u, 0x0u, 0x0u, 0x8921u, 0x0u, 0x0u);  // 00479D6E
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8923u, 0x0u, 0x0u, 0x8923u, 0x1906u, 0x0u, 0x8923u, 0x0u, 0x0u);  // 00479D97
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8923u, 0x0u, 0x0u, 0x8923u, 0x0u, 0x0u, 0x8921u, 0x1906u, 0x2u);  // 00479DC0
    P::glColorFragmentOp2ATI_007bb840(0x8963u, 0x8921u, 0x0u, 0x40u, 0x8923u, 0x0u, 0x0u, 0x8922u, 0x0u, 0x0u);  // 00479DE6
    P::glAlphaFragmentOp1ATI_007bb710(0x8961u, 0x8921u, 0x0u, 0x8577u, 0x1906u, 0x0u);  // 00479E04
    P::glEndFragmentShaderATI_007bb554();  // 00479E0A
    { const U32 error = kotor_renderer_init::glGetError_0073d2c0();  // 00479E10
      if (error != 0) (void)kotor_renderer_init::gluErrorString_0073d028(error); }  // 00479E17 via thunk 0070C12C
    const auto r6 = P::glGenFragmentShadersATI_007bb754(0x1u);  // 00479E1E
    *static_cast<volatile U32*>(&kotor_renderer_gl_cells::ati_fragment_shader_00830258) = r6;  // 00479E25
    P::glBindFragmentShaderATI_007bb6cc(r6);  // 00479E2A
    P::glBeginFragmentShaderATI_007bb7f4();  // 00479E30
    P::glSampleMapATI_007bb620(0x8921u, 0x84c0u, 0x8976u);  // 00479E45
    P::glSampleMapATI_007bb620(0x8922u, 0x84c1u, 0x8976u);  // 00479E5A
    P::glSampleMapATI_007bb620(0x8923u, 0x84c2u, 0x8976u);  // 00479E6F
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8924u, 0x7u, 0x0u, 0x8577u, 0x0u, 0x0u, 0x8921u, 0x0u, 0x0u);  // 00479E95
    P::glColorFragmentOp3ATI_007bb7fc(0x8968u, 0x8924u, 0x7u, 0x0u, 0x8922u, 0x0u, 0x0u, 0x8921u, 0x0u, 0x0u, 0x8924u, 0x0u, 0x0u);  // 00479EC4
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8925u, 0x7u, 0x0u, 0x8923u, 0x0u, 0x0u, 0x8921u, 0x1906u, 0x2u);  // 00479EED
    P::glColorFragmentOp2ATI_007bb840(0x8963u, 0x8921u, 0x7u, 0x0u, 0x8924u, 0x0u, 0x0u, 0x8925u, 0x0u, 0x0u);  // 00479F13
    P::glAlphaFragmentOp1ATI_007bb710(0x8961u, 0x8921u, 0x0u, 0x8941u, 0x1906u, 0x0u);  // 00479F31
    P::glEndFragmentShaderATI_007bb554();  // 00479F37
    const auto r7 = P::glGenFragmentShadersATI_007bb754(0x1u);  // 00479F3F
    *static_cast<volatile U32*>(&kotor_renderer_gl_cells::ati_fragment_shader_0083025c) = r7;  // 00479F46
    P::glBindFragmentShaderATI_007bb6cc(r7);  // 00479F4B
    P::glBeginFragmentShaderATI_007bb7f4();  // 00479F51
    P::glSampleMapATI_007bb620(0x8921u, 0x84c0u, 0x8976u);  // 00479F66
    P::glSampleMapATI_007bb620(0x8922u, 0x84c1u, 0x8976u);  // 00479F7B
    P::glSampleMapATI_007bb620(0x8923u, 0x84c2u, 0x8976u);  // 00479F90
    P::glSampleMapATI_007bb620(0x8924u, 0x84c3u, 0x8976u);  // 00479FA5
    P::glSampleMapATI_007bb620(0x8925u, 0x84c4u, 0x8976u);  // 00479FBA
    P::glColorFragmentOp2ATI_007bb840(0x8966u, 0x8926u, 0x1u, 0x0u, 0x8922u, 0x0u, 0x9u, 0x8921u, 0x0u, 0x9u);  // 00479FE0
    P::glColorFragmentOp2ATI_007bb840(0x8966u, 0x8926u, 0x2u, 0x0u, 0x8923u, 0x0u, 0x9u, 0x8921u, 0x0u, 0x9u);  // 0047A006
    P::glColorFragmentOp2ATI_007bb840(0x8966u, 0x8926u, 0x4u, 0x0u, 0x8924u, 0x0u, 0x9u, 0x8921u, 0x0u, 0x9u);  // 0047A02C
    P::glColorFragmentOp2ATI_007bb840(0x8966u, 0x8924u, 0x0u, 0x1u, 0x8926u, 0x0u, 0x0u, 0x8925u, 0x0u, 0x9u);  // 0047A052
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8924u, 0x0u, 0x0u, 0x8926u, 0x0u, 0x0u, 0x8924u, 0x0u, 0x0u);  // 0047A078
    P::glColorFragmentOp2ATI_007bb840(0x8966u, 0x8923u, 0x0u, 0x0u, 0x8926u, 0x0u, 0x0u, 0x8926u, 0x0u, 0x0u);  // 0047A09E
    P::glColorFragmentOp3ATI_007bb7fc(0x8968u, 0x8924u, 0x0u, 0x0u, 0x8925u, 0x0u, 0xdu, 0x8923u, 0x0u, 0x0u, 0x8924u, 0x0u, 0x0u);  // 0047A0CD
    P::glSampleMapATI_007bb620(0x8926u, 0x8924u, 0x8976u);  // 0047A0E2
    P::glColorFragmentOp1ATI_007bb6d4(0x8961u, 0x8921u, 0x0u, 0x0u, 0x8926u, 0x0u, 0x0u);  // 0047A0FF
    P::glAlphaFragmentOp1ATI_007bb710(0x8961u, 0x8921u, 0x0u, 0x8926u, 0x0u, 0x0u);  // 0047A11A
    P::glAlphaFragmentOp2ATI_007bb5e8(0x8964u, 0x8921u, 0x0u, 0x8921u, 0x0u, 0x0u, 0x8941u, 0x0u, 0x0u);  // 0047A13E
    P::glEndFragmentShaderATI_007bb554();  // 0047A144
    { const U32 error = kotor_renderer_init::glGetError_0073d2c0();  // 0047A14A
      if (error != 0) (void)kotor_renderer_init::gluErrorString_0073d028(error); }  // 0047A151 via thunk 0070C12C
    const auto r8 = P::glGenFragmentShadersATI_007bb754(0x1u);  // 0047A158
    *static_cast<volatile U32*>(&kotor_renderer_gl_cells::ati_fragment_shader_00830260) = r8;  // 0047A15F
    P::glBindFragmentShaderATI_007bb6cc(r8);  // 0047A164
    P::glBeginFragmentShaderATI_007bb7f4();  // 0047A16A
    P::glColorFragmentOp1ATI_007bb6d4(0x8961u, 0x8921u, 0x0u, 0x0u, 0x8577u, 0x0u, 0x0u);  // 0047A187
    P::glAlphaFragmentOp1ATI_007bb710(0x8961u, 0x8921u, 0x0u, 0x8577u, 0x1906u, 0x0u);  // 0047A1A5
    P::glEndFragmentShaderATI_007bb554();  // 0047A1AB
    kotor_renderer_init::glDisable_0073d22c(0x8920u);  // 0047A1BC
    (void)AurGetGLError_0044eee0();  // 0047A1BE
    const auto r10 = P::glGenFragmentShadersATI_007bb754(0x1u);  // 0047A1C5
    *static_cast<volatile U32*>(&kotor_renderer_gl_cells::ati_fragment_shader_00830264) = r10;  // 0047A1CC
    P::glBindFragmentShaderATI_007bb6cc(r10);  // 0047A1D1
    P::glBeginFragmentShaderATI_007bb7f4();  // 0047A1D7
    P::glSampleMapATI_007bb620(0x8921u, 0x84c0u, 0x8976u);  // 0047A1EC
    P::glColorFragmentOp2ATI_007bb840(0x8964u, 0x8921u, 0x0u, 0x0u, 0x8577u, 0x0u, 0x0u, 0x8921u, 0x0u, 0x0u);  // 0047A212
    P::glAlphaFragmentOp2ATI_007bb5e8(0x8961u, 0x8921u, 0x0u, 0x8577u, 0x0u, 0x0u, 0x8921u, 0x0u, 0x0u);  // 0047A236
    P::glColorFragmentOp3ATI_007bb7fc(0x8969u, 0x8921u, 0x7u, 0x0u, 0x896du, 0x1903u, 0x0u, 0x8942u, 0x0u, 0x0u, 0x8921u, 0x0u, 0x0u);  // 0047A268
    P::glEndFragmentShaderATI_007bb554();  // 0047A26E
    kotor_renderer_init::glDisable_0073d22c(0x8920u);  // 0047A279
}
}
namespace kotor_renderer_gl {
void __cdecl SetCurrentStageTID_00426550(std::uint32_t stage, std::uint32_t texture_id) {
    // 00426558 mov [ecx*4+007A683C],eax: unchecked index as in the original (callers pass 0..5).
    store(kotor_renderer_gl_cells::current_tid_007a683c[stage], texture_id);
}
texture::Basic* __cdecl TextureGetNormalizationCubeMap_004233e0() {
    static const char name[] = "NormCubeMap";                          // 0073EE34 (.rdata literal, pushed twice)
    if (kotor_renderer_init::CubeMapAvailable_0045f8b0() == 0) return nullptr;              // 004233F6..0042340D (EAX = 0)
    void* found = find_00420ac0(name, name, nullptr, 0);               // 0042340E..0042341D
    if (found != nullptr) return texture::texture_call_slot_04(found);  // 00423429..0042342D, EAX returned
    void* storage = kotor_menu_10_19::dependencies::allocate_006fa7e6(0xEC); // 00423440..00423445
    texture::Basic* texture = nullptr;
    if (storage != nullptr) {                                          // 00423451..00423457
        try {                                                          // EH state 0 (00423453)
            texture = static_cast<texture::Basic*>(basic_ctor_00423150(storage, nullptr, name, name));  // ctor returns this (EAX -> ESI) // 00423459..00423465
        } catch (...) {
            live_free(storage);                                        // unwind funclet 007157C0 -> 006FA390
            throw;
        }
    }
    basic_as_normalize_vector_cube_map_00420c20(texture, nullptr);  // thiscall ECX = ESI     // 0042346C..00423476 (ECX may be 0)
    return texture;                                                    // 0042347F
}
// 0047A280: Homogenize(xyz, &w) -> static float[4]; glSetFragmentShaderConstantATI(GL_CON_0_ATI, v).
void __cdecl SetFragmentShaderConstant0_0047a280(const float* xyz, float w) {
    float* value = Homogenize_004aaab0(xyz, &w);                       // 0047A280..0047A28A
    P::glSetFragmentShaderConstantATI_007bb5d8(0x8941u, value);        // 0047A292..0047A298
}
void __cdecl SetFragmentShaderConstant1_0047a2a0(const float* xyz, float w) {
    float* value = Homogenize_004aaab0(xyz, &w);                       // 0047A2A0..0047A2AA
    P::glSetFragmentShaderConstantATI_007bb5d8(0x8942u, value);        // 0047A2B2..0047A2B8 (GL_CON_1_ATI)
}
void __cdecl EnableFragmentShaders_0047a2f0(std::uint32_t mode, texture::Basic* t0, texture::Basic* t1, texture::Basic* t2, texture::Basic* t3) {
    using namespace kotor_renderer_gl_cells;
    switch (mode) {                                                    // 0047A2F4 cmp 25h/ja; byte map 0047AB8C, table 0047AB50
    case 0:                                                            // 0047A30E
        P::glBindFragmentShaderATI_007bb6cc(0u);                       // 0047A311
        disable(0x8920u);                                              // 0047A31C
        enable(0x0b50u);                                               // 0047A327
        for (std::uint32_t stage = 0; stage < 4; ++stage) {             // 0047A32E..0047A37C
            ActivateTextureStage_0041fe80(stage);
            kotor_renderer_init::glBindTexture_0073d240(0x0de1u, 0u);
        }
        ActivateTextureStage_0041fe80(0u);                              // 0047A37F
        SetCurrentStageTID_00426550(0, 0);                             // 0047A386
        SetCurrentStageTID_00426550(1, 0);                             // 0047A38D
        SetCurrentStageTID_00426550(2, 0);                             // 0047A395
        SetCurrentStageTID_00426550(3, 0);                             // 0047A39D
        return;
    case 2:                                                            // 0047AB32
        enable(0x8920u);                                               // 0047AB37
        bind_shader(ati_fragment_shader_00830260);                     // 0047AB3D..0047AB44
        return;
    case 3:                                                            // 0047A3A9
        ActivateTextureStage_0041fe80(0u);                              // 0047A3AB
        texture::texture_call_bind_b4(t0);                              // 0047A3B0..0047A3B9
        enable(0x8920u);                                               // 0047A3C4
        bind_shader(ati_fragment_shader_00830264);                     // 0047A3CA..0047A3D0
        return;
    case 6:                                                            // 0047A3DA
    case 7:                                                            // 0047A516
        store(tex1_on_00827fa4, 1u);                                   // 0047A3E4 / 0047A520
        disable(0x0b50u);                                              // 0047A3EA / 0047A526
        ActivateTextureStage_0041fe80(0u);                              // 0047A3F2 / 0047A52E
        enable(0x0de1u);                                               // 0047A405 / 0047A541
        texture::texture_call_bind_b4(t0);                              // 0047A407 / 0047A543
        ActivateTextureStage_0041fe80(1);                              // 0047A414 / 0047A550
        enable(mode == 6 ? 0x8513u : 0x0de1u);                         // 0047A421 / 0047A55D
        texture::texture_call_bind_b4(t1);                              // 0047A423 / 0047A55F
        ActivateTextureStage_0041fe80(2);                              // 0047A431 / 0047A56D
        enable(0x8513u);                                               // 0047A43E / 0047A57A
        texture::texture_call_bind_b4(t2);                              // 0047A440 / 0047A57C
        ActivateTextureStage_0041fe80(3);                              // 0047A44E / 0047A58A
        enable(mode == 6 ? 0x0de1u : 0x8513u);                         // 0047A45B / 0047A597
        texture::texture_call_bind_b4(t3);                              // 0047A45D / 0047A599
        enable(0x8920u);                                               // 0047A46E / 0047A5AA
        bind_shader(mode == 6 ? ati_fragment_shader_00830250 : ati_fragment_shader_00830254); // 0047A470 / 0047A5AC
        return;
    case 13:                                                           // 0047A481 (esi = glDisable)
        disable(0x8920u);                                              // 0047A48C
        ActivateTextureStage_0041fe80(1);                              // 0047A490
        disable(0x8513u);                                              // 0047A49D
        disable(0x0de1u);                                              // 0047A4A4
        clear_current_stage_texture();                                 // 0047A4A6..0047A4B0
        ActivateTextureStage_0041fe80(2);                              // 0047A4B7
        disable(0x8513u);                                              // 0047A4C4
        disable(0x0de1u);                                              // 0047A4CB
        clear_current_stage_texture();                                 // 0047A4CD..0047A4D4
        ActivateTextureStage_0041fe80(3);                              // 0047A4DB
        disable(0x0de1u);                                              // 0047A4E8
        disable(0x8513u);                                              // 0047A4EF
        clear_current_stage_texture();                                 // 0047A4F1..0047A4FC
        enable(0x0b50u);                                               // 0047A503
        ActivateTextureStage_0041fe80(0u);                              // 0047A50A
        return;
    case 19:                                                           // 0047A5BD
    case 20: {                                                         // 0047A6CB
        store(env_map_on_00827fa0, 1u);                                // 0047A5C4 / 0047A6D2
        store(tex1_on_00827fa4, 1u);                                   // 0047A5CA / 0047A6D8
        ActivateTextureStage_0041fe80(0u);                              // 0047A5D0 / 0047A6DE
        texture::texture_call_bind_b4(t0);                              // 0047A5D5..0047A5DE
        ActivateTextureStage_0041fe80(1);                              // 0047A5E5 / 0047A6F3
        texture::texture_call_bind_b4(t1);                              // 0047A5EA..0047A5F5
        const U32& shader = mode == 19 ? ati_fragment_shader_00830248 : ati_fragment_shader_0083024c;
        if (texture::texture_call_flag_0c_al(t1) != 0) {                // 0047A5FB..0047A604
            tex_gen(0x2000u, 0x8512u);                                 // 0047A61B
            tex_gen(0x2001u, 0x8512u);                                 // 0047A62C
            tex_gen(0x2002u, 0x8512u);                                 // 0047A63D
            enable(0x0c60u);                                           // 0047A64A
            enable(0x0c61u);                                           // 0047A651
            enable(0x0c62u);                                           // 0047A658
        } else {                                                       // 0047A671 / 0047A77F
            enable(0x0de1u);                                           // 0047A67C
            tex_gen(0x2000u, 0x2402u);                                 // 0047A693
            tex_gen(0x2001u, 0x2402u);                                 // 0047A6A4
            enable(0x0c60u);                                           // 0047A6AB
            enable(0x0c61u);                                           // 0047A6B2
        }
        enable(0x8920u);                                               // 0047A65F / 0047A6B9
        bind_shader(shader);                                           // 0047A661 / 0047A6BB (00830248 / 0083024C)
        return;
    }
    case 22:                                                           // 0047A8AF
        store(env_map_on_00827fa0, 1u);                                // 0047A8B6
        store(tex1_on_00827fa4, 1u);                                   // 0047A8BC
        ActivateTextureStage_0041fe80(0u);                              // 0047A8C2
        texture::texture_call_bind_b4(t0);                              // 0047A8C7..0047A8D0
        ActivateTextureStage_0041fe80(2);                              // 0047A8D8
        texture::texture_call_bind_b4(t1);                              // 0047A8DD..0047A8E8
        if (texture::texture_call_flag_0c_al(t1) != 0) {                // 0047A8EE..0047A8F7
            tex_gen(0x2000u, 0x8512u);                                 // 0047A90E
            tex_gen(0x2001u, 0x8512u);                                 // 0047A91F
            tex_gen(0x2002u, 0x8512u);                                 // 0047A930
            enable(0x0c60u);                                           // 0047A93D
            enable(0x0c61u);                                           // 0047A944
            enable(0x0c62u);                                           // 0047A946 push; 0047A98E call (shared tail)
        } else {                                                       // 0047A94D
            enable(0x0de1u);                                           // 0047A958
            tex_gen(0x2000u, 0x2402u);                                 // 0047A96F
            tex_gen(0x2001u, 0x2402u);                                 // 0047A980
            enable(0x0c60u);                                           // 0047A987
            enable(0x0c61u);                                           // 0047A989 push; 0047A98E call
        }
        ActivateTextureStage_0041fe80(1);                              // 0047A990..0047A991 (edi = 1)
        enable(0x0de1u);                                               // 0047A99E
        texture::texture_call_bind_b4(t2);                              // 0047A9A0..0047A9A6 ([esp+1Ch] = arg 4)
        enable(0x8920u);                                               // 0047A9B1
        bind_shader(ati_fragment_shader_00830258);                     // 0047A9B3..0047A9BA
        return;
    case 24:                                                           // 0047AA3E
        enable(0x8920u);                                               // 0047AA43
        bind_shader(ati_fragment_shader_0083025c);                     // 0047AA49..0047AA50
        return;
    case 26:                                                           // 0047A84D
        ActivateTextureStage_0041fe80(0u);                              // 0047A84F
        enable(0x0de1u);                                               // 0047A862
        texture::texture_call_bind_b4(t0);                              // 0047A864..0047A86A
        ActivateTextureStage_0041fe80(1);                              // 0047A876
        enable(0x0de1u);                                               // 0047A883
        texture::texture_call_bind_b4(t1);                              // 0047A885..0047A88B
        store(tex1_on_00827fa4, 1u);                                   // 0047A896 (before the next call)
        enable(0x8920u);                                               // 0047A89C
        bind_shader(ati_fragment_shader_00830244);                     // 0047A89E..0047A8A5
        return;
    case 29:                                                           // 0047AA5A
        MakeLookUpTexture_00423d80(5.0f);                              // 0047AA5A push 40A00000h; 0047AA5F
        ActivateTextureStage_0041fe80(0u);                              // 0047AA67
        enable(0x0de1u);                                               // 0047AA7A
        texture::texture_call_bind_b4(t0);                              // 0047AA7C..0047AA82
        for (std::uint32_t stage = 1; stage <= 4; ++stage) {            // 0047AA88..0047AB02
            ActivateTextureStage_0041fe80(stage);
            enable(0x8513u);
            texture::texture_call_bind_b4(TextureGetNormalizationCubeMap_004233e0()); // EAX receiver, no null test
        }
        ActivateTextureStage_0041fe80(5);                              // 0047AB0A
        enable(0x8513u);                                               // 0047AB17
        texture::texture_call_bind_b4(t3);                              // 0047AB19..0047AB1F ([esp+20h] = arg 5)
        ActivateTextureStage_0041fe80(0u);                              // 0047AB26
        return;
    case 32:                                                           // 0047A7D9 (esi = glDisable)
        if (load(tex1_on_00827fa4) != 0) {                             // 0047A7D9..0047A7E8
            ActivateTextureStage_0041fe80(1);                          // 0047A7EC
            if (load(env_map_on_00827fa0) != 0) {                      // 0047A7F1..0047A7FB
                disable(0x8513u);                                      // 0047A802
                disable(0x0c60u);                                      // 0047A809
                disable(0x0c61u);                                      // 0047A810
                disable(0x0c62u);                                      // 0047A817
                const U32 stage = load(current_stage_007a6898);        // 0047A819
                store(env_map_on_00827fa0, 0u);                        // 0047A81F
                store(current_tid_007a683c[stage], 0u);             // 0047A825
            }
            disable(0x0de1u);                                          // 0047A831
            store(tex1_on_00827fa4, 0u);                               // 0047A833
        }
        ActivateTextureStage_0041fe80(0u);                              // 0047A83A
        disable(0x8920u);                                              // 0047A847
        return;
    case 37:                                                           // 0047A9C4 (esi = glDisable)
        ActivateTextureStage_0041fe80(1);                              // 0047A9C6
        disable(0x8513u);                                              // 0047A9D9
        disable(0x0de1u);                                              // 0047A9E0
        clear_current_stage_texture();                                 // 0047A9E2..0047A9EC
        ActivateTextureStage_0041fe80(2);                              // 0047A9F3
        disable(0x8513u);                                              // 0047AA00
        disable(0x0de1u);                                              // 0047AA07
        clear_current_stage_texture();                                 // 0047AA09..0047AA13 (before 0047AA1A)
        disable(0x0c60u);                                              // 0047AA1A
        disable(0x0c61u);                                              // 0047AA21
        disable(0x0c62u);                                              // 0047AA28
        disable(0x8920u);                                              // 0047AA2F
        ActivateTextureStage_0041fe80(0u);                              // 0047AA32
        return;
    default:  // 1,4,5,8-12,14-18,21,23,25,27,28,30,31,33-36 -> 0047AB4A; mode > 25h -> 0047AB4C
        return;
    }
}
}
