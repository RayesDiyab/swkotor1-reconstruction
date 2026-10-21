#pragma once
// lane-3d-vp (SUBLANES.json area "vp_nv_ati"): ARB/NV vertex-program, NV register-combiner and ATI
// fragment-shader render paths of swkotor.exe (GOG K1, image base 00400000, sha256 9c10e045...).
// Includes the canonical 3D header (lane-3d-render scene3d.h). Declarations in this header that other
// 3D areas need are PROPOSED for append to scene3d.h (../scene3d.h.proposed.diff).
//
// Conventions (DECISIONS D1 + amendment, D13, D21, D24, D30-FINAL, D31, D35):
//  * original __thiscall bodies = __fastcall(Self*, void* unused_edx, stack...); callers pass nullptr for EDX.
//    Existing member-function spellings (kotor_vertex_program::VertexProgram) are kept as declared by their
//    canonical header (vertex_program.h). No slot here returns a struct by value; none is variadic.
//  * GL entry points: the single adapter owner kotor_renderer_init::<import>_<IAT> (lane-renderer-bodies),
//    extension procedures: kotor_renderer_gl_procs::<name>_<cell> (cell reloaded at every call, as the
//    original `call dword ptr [cell]`).
//  * D30-FINAL: x87 wide intermediates are `double`; `float` only where the original stores with fstp dword.
//    _ftol2 006FAE8C = M40 port unresolved_ftol2_006fae8c (ST0 -> EDX:EAX) through vp_ftol2 below.
//  * Objects without a published layout are byte-offset views (kotor_scene3d::field/at); every offset is cited.
//  * D13: original .data/.rdata texts come only from the LOCAL generated headers (generated-local/{data,rdata};
//    spec/EXTRACT-SPEC-{DATA,RDATA}.json; extractor lane-renderer-bodies/tools/extract_original_data.py).
#include "6c81d8874d-scene3d.h"
#include "e4678f005c-vertex_program.h"     // stage canonical-inputs/renderer-interface (== lane-renderer-bodies copy)
#include "61e3cacb4e-renderer_gl_procs.h"  // lane-renderer-bodies: 201 typed extension cells + loader masks

namespace texture { struct Basic; }   // CAurTexture (lane-texture-epoch E1); identical forward declaration

// ---------------------------------------------------------------- existing owners (exact copies)
namespace kotor_renderer_init {
using U32 = std::uint32_t; using I32 = std::int32_t; using U8 = std::uint8_t;
// lane-renderer-bodies renderer_gl_imports_gen.h / renderer_gl_imports.h / M40 adapters (single adapter owner)
extern void __stdcall glColor4f_0073d224(float, float, float, float);
extern void __stdcall glMatrixMode_0073d228(U32);
extern void __stdcall glLoadIdentity_0073d230();
extern void __stdcall glOrtho_0073d234(double, double, double, double, double, double);
extern void __stdcall glBindTexture_0073d240(U32, U32);
extern void __stdcall glGetIntegerv_0073d244(U32, I32*);
extern void __stdcall glEnableClientState_0073d260(U32);
extern void __stdcall glDisableClientState_0073d264(U32);
extern void __stdcall glPushAttrib_0073d268(U32);
extern void __stdcall glPopAttrib_0073d26c();
extern void __stdcall glFrontFace_0073d274(U32);
extern void __stdcall glDepthMask_0073d28c(U8);
extern void __stdcall glViewport_0073d2bc(I32, I32, I32, I32);
extern void __stdcall glEnd_0073d2c4();
extern void __stdcall glVertex3f_0073d2c8(float, float, float);
extern void __stdcall glTexCoord2f_0073d2cc(float, float);
extern void __stdcall glBegin_0073d2d0(U32);
extern void __stdcall glTexEnvi_0073d2f4(U32, U32, I32);
extern void __stdcall glFlush_0073d2f8();
extern void __stdcall glDrawBuffer_0073d304(U32);
extern void __stdcall glTexGeni_0073d314(U32, U32, I32);
extern void __stdcall glFogf_0073d350(U32, float);
extern void __stdcall glFogfv_0073d354(U32, const float*);
extern void __stdcall glBlendFunc_0073d38c(U32, U32);
// lane-renderer-bodies extension_loader.h (dispatch cells 007A47E0 / 007A6820 / 007A47DC)
using EnableDispatch=void(__cdecl*)(U32,texture::Basic*,texture::Basic*,texture::Basic*,texture::Basic*);
using ConstantDispatch=void(__cdecl*)(const float*,float);
// lane-renderer-bodies arb_error.h (0047A2C0 AurGetVertexProgramARBError; single owner arb_error.cpp)
void __cdecl ReportARBError_0047a2c0(const char* current_program);
// lane-upload upload.h (capabilities_0045f.cpp)
bool __cdecl ATIValidate_0045f640();               // AL
U32 __cdecl CubeMapAvailable_0045f8b0();           // full EAX
U32 __cdecl BumpMapAvailable_0045fb10();           // full EAX 0/1
// lane-renderer-bodies extension_loader.h (0045FC90; renderer_gl_loader_lowers.cpp)
extern U32 __stdcall VertexBufferAvailable_0045fc90();
}
namespace kotor_renderer_gl {
// lane-renderer-bodies renderer_gl_loader_lowers.cpp (declaration as lane-3d-mesh trimesh_f2.cpp)
std::uint32_t __cdecl BumpyShinyAvailable_0045f680();
// lane-renderer-bodies render_shared_lowers.h
void __cdecl ActivateTextureStage_0041fe80(std::uint32_t stage);
std::uint32_t __cdecl MultitextureAvailable_0045f710();
}
namespace kotor_vertex_program {
// lane-renderer-bodies validator.cpp (enable_bindings.h spelling)
extern std::uint8_t __cdecl ValidateNonRadeon_0045f5e0();
}
namespace kotor_renderer_gl_cells {   // lane-renderer-bodies renderer_gl_cells.cpp / *_cells.cpp (storage owner)
extern std::int32_t programs_0078e5ec;               // cold 1, no writer (D31 gate); D5: I32
extern std::uint32_t capabilities_007bb85c;          // extension bits (InitExtensions)
extern std::uint32_t control_0078e4fc;               // .data
extern std::int32_t vertex_buffer_level_0078e500;    // cold 2, compared signed
extern std::uint32_t vertex_enum_0078d46c;           // cold 0x8620 (GL_VERTEX_PROGRAM_ARB)
extern std::uint16_t format_0078e388;                // cold 0xFFFF (word compared at 004A25A7)
extern std::uint32_t current_stage_007a6898;         // uiCurrentStage
extern std::uint32_t width_0078d1d4;                 // screen width (fild signed)
extern std::uint32_t main_context_007a6854, main_dc_007a47e4;
extern std::uint32_t power_width_007a6890, power_height_007a6894;   // fild + 2^32 fix-up = unsigned
extern void* bloom_dc_007a68c8;                      // renderer_gl_framebuffer_cells.cpp
extern void* bloom_context_007a68cc;
extern kotor_renderer_init::EnableDispatch enable_dispatch_007a47e0;      // renderer_gl_loader_cells.cpp
extern kotor_renderer_init::ConstantDispatch constant0_dispatch_007a6820;
extern kotor_renderer_init::ConstantDispatch constant1_dispatch_007a47dc;
}
namespace kotor_menu_window_cells { extern volatile std::uint32_t height_0078d1d8; }   // M40 window-shared-cells.obj
namespace kotor_aurora_window {   // lane-window vertex_programs.cpp / window_lists.h:15 (identical declaration; D60: constant-initialised,
                                  // atexit 0073BE70 owned by lane-window) - CExoArrayList<CAurVertexProgram*> 008305CC
extern kotor_reconstruction::ArrayMutations vertex_programs_008305cc;  // 008305CC data / 008305D0 size / 008305D4 capacity
}
namespace renderer_program_cells {   // task-22 renderer-adoption-packet-r1 program_cells.cpp / lane-window aurora_editor.cpp
extern std::uint32_t* parameters_00830560;            // written by CAurInternalGL::Initialize 0044E0F6/0044E315
extern std::uint32_t* parameters_00830580;            // lane-window (BSS, no writer)
extern char source_0079C310[];                        // lane-window D13 (NV skinned program, 1682 bytes incl. NUL)
extern void* output_00830388;
extern void* output_0083038C;
}
namespace kotor_trimesh_data { extern char nv_program_0079ab28[574]; }   // lane-3d-mesh D13 (trimesh_skin_cells.cpp)
extern "C" {
// M40 world-timer-x87-conversion.obj: original _ftol2 006FAE8C, argument ST0, result EDX:EAX.
unsigned __int64 __cdecl unresolved_ftol2_006fae8c();
}

// ---------------------------------------------------------------- cells owned by this area
// Storage: vp_cells.cpp (original cold values from the pinned image; BSS = 0). Each cell is referenced only by
// functions of this area unless noted (full-image xref over lane-gui-draw/exe-text.asm).
namespace kotor_vp_cells {
using U = std::uint32_t; using I = std::int32_t; using U8 = std::uint8_t;
using kotor_vertex_program::VertexProgram;
extern volatile U cache_0078e504;          // getATIFragmentShadersDisabled cache, .data cold 0xFFFFFFFF (0045F5A0/0045F5CC)
extern volatile U cache_0078e518;          // AurVertexProgramsNVAvailable cache, .data cold 0xFFFFFFFF (0045F740/0045F75E)
extern volatile U gate_0078e628;           // RenderLightMappedNoShaders gate, .data cold 1 (0046FB11)
// VertexProgram::Enable shared state (BSS). 00830540 is also incremented by AurVertexProgramMakeCurrent 004A23B0
// (lane-3d-render, borrowed through kotor_scene3d::Cells::vertex_program_epoch_00830540).
extern char* volatile current_source_008305dc;   // 004A24D0 / 004A251B
extern volatile U8 lighting_008305d8;            // 004A24EB / 004A2521
extern volatile U generation_00830540;           // 004A2560 / 004A25C3 (004A23B0 inc)
// .data cold 1. Writers: ManageSceneBSP 0046E5E8 (=1), Enable 004A2527 (=0); readers LightManager 004A43C5/004A56D5/
// 004A575E (lane-3d-bsp bsp.h borrows it as vertex_programs_reset_007a193c).
extern volatile U refresh_007a193c;
// Function-local static VertexProgram* + guard word pairs (guard bit 0 set before construction).
extern VertexProgram* volatile flat_program_008280f0;            extern volatile U flat_guard_008280f4;          // 00470FE0
extern VertexProgram* volatile lightmapped_program_008280f8;     extern volatile U lightmapped_guard_008280fc;   // 004710B0
extern VertexProgram* volatile env_program_00828100;             extern volatile U env_guard_00828104;           // 00471180
extern VertexProgram* volatile emlm_program_00828108;            extern volatile U emlm_guard_0082810c;          // 004712B0
extern VertexProgram* volatile skin_program_00830390;            extern volatile U skin_guard_00830394;          // 00487310
extern VertexProgram* volatile skin_sphere_program_00830398;     extern volatile U skin_sphere_guard_0083039c;   // 004873C0
extern VertexProgram* volatile skin_cube_program_008303a0;       extern volatile U skin_cube_guard_008303a4;     // 00487460
// Parameter-word cells (BSS, no store anywhere in the image -> stay null; read as the VertexProgram ctor arg).
extern std::uint32_t* volatile parameters_00830578;   // 0048736C
extern std::uint32_t* volatile parameters_0083058c;   // 00487408
extern std::uint32_t* volatile parameters_00830590;   // 004874BC
// Cells shared with other areas whose storage is defined in this lane (other areas bind to these symbols):
// vp_f2_skinned.cpp: .data gates/scales also read by lane-3d-mesh trimesh_skin bodies; 00830294 also 0047721E.
extern volatile U gate_0078e5fc, gate_0078e650, gate_0078e66c, gate_00798b48, gate_00798b4c;   // .data cold 1
extern volatile float scale_0078e630, scale_0078e648;                                          // .data cold 1.0f
extern std::uint32_t* volatile parameters_00830294;                                            // BSS, no store
// vp_f4_lights.cpp: headlight colours also read by LightManager::EnableHeadLight 004A2FF0 (lights sublane).
extern kotor_scene3d::Vector headlight_a_007a194c;   // .data {0.8f x3}
extern kotor_scene3d::Vector headlight_b_007a1958;   // .data {0.2f x3}
}

// ---------------------------------------------------------------- D13 original texts (vp_cells.cpp)
namespace kotor_vp_data {
// .data program texts: writable char arrays (the original passes their addresses as char* to 004A27D0,
// which stores the pointer at VertexProgram+4).
extern char arb_flat_vp_0078feb8[1095];
extern char arb_env_vp_00790300[1463];
extern char arb_emlm_vp_007908b8[1463];
extern char arb_skin_vp_00792658[2926];
extern char arb_skin_vp_007931c8[1788];
extern char arb_skin_cube_vp_007938c8[3377];
extern char arb_skin_vp_00795170[3758];
extern char arb_skin_vp_00796020[5220];
extern char arb_skin_vp_007978b0[4571];
extern char nv_skin_vp_0079b5a8[1682];
extern char nv_skin_vp_0079ce58[2244];
extern char nv_skin_vp_0079d720[3072];
extern char nv_skin_sphere_vp_0079e958[1994];
extern char nv_skin_cube_vp_0079f128[1923];
extern char nv_blur_vp_0079fe50[2509];
// .rdata strings (const).
extern const char program_end_00743950[4];          // 4-byte store at 004A2668..004A266E
extern const char fmt_nv_light_y_00743aa0[48];
extern const char fmt_nv_light_x_00743ad0[48];
extern const char fmt_nv_point_00743b00[170];
extern const char fmt_arb_light_y_00743bb0[99];
extern const char fmt_arb_light_x_00743c18[99];
extern const char fmt_arb_point_00743c80[281];
extern const char text_nv_head_00743da0[196];
extern const char text_arb_head_00743e68[369];
extern const char text_nv_tail_007439c0[39];
extern const char text_nv_alpha_007439e8[19];
extern const char text_nv_add_007439fc[34];
extern const char text_arb_tail_00743a20[33];
extern const char text_arb_alpha_00743a44[38];
extern const char text_arb_add_00743a6c[50];
}

// ---------------------------------------------------------------- authored in this area
// (extern "C" spellings already used by consumers: scene3d.h, lane-3d-mesh trimesh_f1.cpp / trimesh_f2.cpp)
extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I;
// GLRender::RenderFrameToTextureATI 00433580: cdecl(texture name), plain RET (caller 00435BD0)
void __cdecl gl_render_frame_to_texture_ati_00433580(U texture);
// glProgramEnvParameter4fARB wrapper 0044F820 (scene3d.h)
void __cdecl gl_program_env_parameter_0044f820(U index, float x, float y, float z, float w);
// capability predicates
I __cdecl aur_ati_fragment_shader_available_0045f560();                       // full EAX (scene3d.h)
std::uint8_t __cdecl get_ati_fragment_shaders_disabled_0045f5a0();             // AL (sete)
U __cdecl aur_vertex_programs_nv_available_0045f740();                         // full EAX = cache 0078E518
std::uint8_t __cdecl aur_ati_fragment_shaders_bump_map_available_0045fb90();   // AL
bool __cdecl aur_nv2x_level_shaders_available_0045fbd0();                      // AL (scene3d.h)
// TriMesh render paths: cdecl(VertexPrimitive*) / cdecl(VertexPrimitive*, Part*)
void __cdecl render_light_mapped_no_shaders_0046fb10(void* primitive);
void __cdecl render_nv1x_emlm_00470590(void* primitive);
void __cdecl render_flat_vp_00470fe0(void* primitive, void* part);
void __cdecl render_lightmapped_vp_004710b0(void* primitive, void* part);
void __cdecl render_environment_mapped_vp_00471180(void* primitive, void* part);
void __cdecl render_emlm_vp_004712b0(void* primitive, void* part);
// skinned vertex programs (function-local statics): cdecl, no args, EAX = VertexProgram*
kotor_vertex_program::VertexProgram* __cdecl get_skin_vp_00487310();
kotor_vertex_program::VertexProgram* __cdecl get_skin_sphere_map_vp_004873c0();
kotor_vertex_program::VertexProgram* __cdecl get_skin_cube_map_vp_00487460();
void __cdecl render_skinned_vp_0048a520(void* primitive);
void __cdecl render_skinned_vp_blur_0048c650(void* primitive);
void __fastcall part_skin_draw_bumped_out_shell_vp_0048d040(void* part, void* unused_edx);   // thiscall, RET 0
// LightManager::SetUpVertexProgramLightShaderConstants 004A50D0: thiscall(LightManager*), 2 stack words, RET 8
void __fastcall light_manager_set_up_vertex_program_light_shader_constants_004a50d0(void* lights, void* unused_edx,
    I allowed_lights, I condition);
}
namespace kotor_vertex_program {
// enable_bindings.h spellings (lane-renderer-bodies donor header; same mangled names)
void __cdecl BindVertexProgram_0046f550(std::uint32_t program);
char* __cdecl SetupVertexProgramLights_004a30e0(char* text, std::int32_t allowed_lights, std::int32_t condition);
void __cdecl LoadVertexProgram_004a23c0(std::uint32_t program, std::uint32_t length, const char* text,
    std::uint32_t* parameters);
// vertex_program.h declares AddVertexProgramToManager_004a2770 and VertexProgram::Enable_004a24d0 (defined here).
}

// ---------------------------------------------------------------- shared helpers (header-only, no storage)
namespace kotor_vp {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::U8; using kotor_scene3d::U16;
using kotor_scene3d::field; using kotor_scene3d::at; using kotor_scene3d::bits_to_float; using kotor_scene3d::float_to_bits;
using kotor_vertex_program::VertexProgram;
inline void* as_object(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
inline U word_of(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline bool programs_arb() { return kotor_renderer_gl_cells::programs_0078e5ec != 0; }   // mov eax,[0078E5EC]; test
// D30-FINAL: W = value kept on the x87 stack (double), S = original fstp dword store.
inline double W(float v) { return static_cast<double>(v); }
inline float S(double v) { return static_cast<float>(v); }
inline float F(U bits) { return bits_to_float(bits); }
// _ftol2 006FAE8C through the M40 port (fld qword -> call -> EAX low dword of the truncation).
inline I vp_ftol2(double value) {
    I result;
    __asm {
        fld qword ptr value
        call unresolved_ftol2_006fae8c
        mov result, eax
    }
    return result;
}
}
