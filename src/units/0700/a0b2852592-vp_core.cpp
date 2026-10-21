// vp_core.cpp - lane-3d-vp core bodies (SUBLANES "vp_nv_ati"): capability predicates, ARB/NV program wrappers,
// the vertex-program manager add, VertexProgram::Enable, the four TriMesh VP render paths, the three skinned-VP
// function-local statics and GLRender::RenderFrameToTextureATI.
// Evidence: dumpbin /disasm swkotor.exe (lane-3d-vp/d/<ADDR>.s); each statement cites the instruction(s) it
// reproduces. Donor reused: VertexProgram::Enable logic = task-22 renderer-residual-delivery-r1
// enable-checkpoint/source/vertex_enable.cpp (sha256 acc3f4af...), re-verified against 004A24D0..004A2761 and
// re-bound to the owner cells/procedure slots directly (the donor's EnableBindings view and its
// `extern ... const` procedure-pointer copies are not used: a const copy of a loader-filled slot would freeze the
// cold null value).
#include "4bf744a1f4-vp_nv_ati.h"
#include "7ee14b5ec3-model_vtables.hpp"   // lane-3d-model typed VertexPrimitive table (D39/D41)
#include <cstring>

// ---------------------------------------------------------------- lowers (exact copies of existing owners)
extern "C" {
// lane-3d-mesh trimesh_skin.h (trimesh_skin owner): TriMesh render paths, cdecl(VertexPrimitive*)
void __cdecl render_flat_0046f900(void* primitive);
void __cdecl render_light_mapped_0046fa40(void* primitive);
void __cdecl render_environment_mapped_0046ff70(void* primitive, kotor_scene3d::U arg1, kotor_scene3d::U arg2);
void __cdecl render_emlm_004703f0(void* primitive);
// lane-3d-fb fb_bloom.h / fb_pixel_shaders.cpp (framebuffer_bloom owner)
void __cdecl gl_frame_buffer_modification_pixel_shaders_ati_004293f0(kotor_scene3d::U mode, kotor_scene3d::U t0,
    kotor_scene3d::U t1, kotor_scene3d::U t2, kotor_scene3d::U t3);
}

namespace {
using namespace kotor_vp;
using namespace kotor_renderer_init;
namespace procs = kotor_renderer_gl_procs;
namespace rc = kotor_renderer_gl_cells;
namespace vc = kotor_vp_cells;
using kotor_scene3d::cells;
using PrimitiveTable = aurora_vt::VTable_00742f74;   // VertexPrimitive layout (common prefix; lane-3d-mesh spelling)

constexpr U kVertexProgramTarget = 0x8620;           // GL_VERTEX_PROGRAM_ARB / GL_VERTEX_PROGRAM_NV immediates
constexpr U kProgramFormatAscii = 0x8875;            // GL_PROGRAM_FORMAT_ASCII_ARB (004A23D3)
constexpr U kProgramSize = 0x201C;                   // push 201Ch (00471012 ...)
constexpr U kTwoPow32_0073d770 = 0x4F800000;         // .rdata 0073D770 = 4294967296.0f (unsigned fild fix-up)

// [[primitive]+4](primitive): VertexPrimitive slot +04 (self_return on every primitive table), reloaded per call.
void* primitive_slot4(void* primitive) {
    const PrimitiveTable* table = static_cast<const PrimitiveTable*>(field<void*>(primitive, 0));
    return as_object(table->s004(primitive, nullptr));
}

// Shared shape of the four function-local VertexProgram statics in 00470FE0/004710B0/00471180/004712B0:
//   test byte [guard],1; jne have; or [guard],1; push 201Ch; call operator_new; test eax,eax; je null;
//   ecx = [0078E5EC] ? arb_text : nv_text; push [00830560]; push ecx; call 004A27D0; [static] = eax (or 0).
// The SEH state (FuncInfo 00716Exx) only covers the constructor call; operator new returns NULL (no throw)
// and 004A27D0 has no throwing callee, so no unwind action is reachable (D30-FINAL).
VertexProgram* trimesh_static_program(VertexProgram* volatile& program, volatile U& guard, char* arb_text, char* nv_text) {
    if ((guard & 1u) == 0) {
        guard = guard | 1u;
        void* storage = live_operator_new(kProgramSize);
        VertexProgram* constructed = nullptr;
        if (storage != nullptr) {
            char* text = programs_arb() ? arb_text : nv_text;
            constructed = static_cast<VertexProgram*>(storage)->Construct_004a27d0(
                text, renderer_program_cells::parameters_00830560);
        }
        program = constructed;
    }
    return program;
}
} // namespace

// ================================================================ 00433580 GLRender::RenderFrameToTextureATI
extern "C" void __cdecl gl_render_frame_to_texture_ati_00433580(U texture) {
    // 00433589 ebx=[007A47E4] main DC, 00433590 ebp=[007A6854] main context (read at entry, used at 00433783).
    void* const main_dc = as_object(rc::main_dc_007a47e4);
    void* const main_context = as_object(rc::main_context_007a6854);
    // 00433580..004335A0 wglMakeContextCurrentARB(bloom DC, bloom DC, bloom context) through [007BB818].
    void* const bloom_context = rc::bloom_context_007a68cc;
    void* const bloom_dc = rc::bloom_dc_007a68c8;
    (void)procs::wglMakeContextCurrentARB_007bb818(bloom_dc, bloom_dc, bloom_context);
    glDrawBuffer_0073d304(0x404);                                          // 004335A6 GL_FRONT
    glViewport_0073d2bc(0, 0, 0x80, 0x80);                                 // 004335B1..004335BF
    glMatrixMode_0073d228(0x1701);                                         // 004335C5..004335D0 GL_PROJECTION
    glLoadIdentity_0073d230();                                             // 004335D8
    glOrtho_0073d234(0.0, 128.0, 0.0, 128.0, -1.0, 1.0);                   // 004335DA..004335FE
    glMatrixMode_0073d228(0x1700);                                         // 00433604 GL_MODELVIEW
    glLoadIdentity_0073d230();                                             // 0043360B
    gl_frame_buffer_modification_pixel_shaders_ati_004293f0(4, 0, 0, 0, 0);   // 0043360D..00433622
    glDisable_0073d22c(0x0B71);                                            // 00433625 GL_DEPTH_TEST
    procs::glActiveTextureARB_007bb80c(0x84C0);                            // 0043362C GL_TEXTURE0_ARB
    glEnable_0073d390(0x0DE1);                                             // 00433637 GL_TEXTURE_2D
    glBindTexture_0073d240(0x0DE1, texture);                               // 00433642..0043364C
    glBlendFunc_0073d38c(1, 0);                                            // 00433652..00433656 (GL_ONE, GL_ZERO)
    // 0043365C..00433681: fild [0078D1D4] (signed) / fild [007A6890] (+2^32 if negative = unsigned) -> fstp dword
    double power_width = static_cast<double>(static_cast<I>(rc::power_width_007a6890));
    if (static_cast<I>(rc::power_width_007a6890) < 0) power_width += static_cast<double>(F(kTwoPow32_0073d770));
    const float u = S(static_cast<double>(static_cast<I>(rc::width_0078d1d4)) / power_width);
    // 00433685..0043369D: fild [0078D1D8] / fild [007A6894] (unsigned fix-up) -> fstp dword
    double power_height = static_cast<double>(static_cast<I>(rc::power_height_007a6894));
    if (static_cast<I>(rc::power_height_007a6894) < 0) power_height += static_cast<double>(F(kTwoPow32_0073d770));
    const float v = S(static_cast<double>(static_cast<I>(kotor_menu_window_cells::height_0078d1d8)) / power_height);
    glBegin_0073d2d0(7);                                                   // 0043369B..004336A1 GL_QUADS
    for (I corner = 0; corner < 4; ++corner) {                             // 004336A7 / 00433754..00433758
        glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                        // 004336B0..004336C4
        const float t = (corner == 0 || corner == 3) ? v : 0.0f;           // 004336CA..004336E1 -> [esp+10h]
        const float s = (corner == 0 || corner == 1) ? 0.0f : u;           // 004336E5..004336F8 -> [esp+14h]
        glTexCoord2f_0073d2cc(s, t);                                       // 00433700..0043370A
        const float y = (corner == 0 || corner == 3) ? 128.0f : 0.0f;      // 00433710..00433721 -> [esp+18h]
        const float x = (corner == 0 || corner == 1) ? 0.0f : 128.0f;      // 00433729..0043373A -> [esp+1Ch]
        glVertex3f_0073d2c8(x, y, 0.0f);                                   // 00433742..0043374E
    }
    glEnd_0073d2c4();                                                      // 0043375E
    glDisable_0073d22c(0x8920);                                            // 00433764..00433769 GL_FRAGMENT_SHADER_ATI
    gl_frame_buffer_modification_pixel_shaders_ati_004293f0(2, 0, 0, 0, 0);   // 0043376B..0043377A
    glFlush_0073d2f8();                                                    // 0043377D
    (void)procs::wglMakeContextCurrentARB_007bb818(main_dc, main_dc, main_context);   // 00433783..00433786
    glDrawBuffer_0073d304(0x405);                                          // 0043378C..00433791 GL_BACK
}

// ================================================================ 0044F820 glProgramEnvParameter4fARB wrapper
extern "C" void __cdecl gl_program_env_parameter_0044f820(U index, float x, float y, float z, float w) {
    if (programs_arb()) {                                                  // 0044F820..0044F827
        procs::glProgramEnvParameter4fARB_007bb850(kVertexProgramTarget, index, x, y, z, w);   // 0044F829..0044F847
        return;
    }
    procs::glProgramParameter4fNV_007bb7ac(kVertexProgramTarget, index, x, y, z, w);         // 0044F84E..0044F86C
}

// ================================================================ capability predicates
// 0045F560 AurATIFragmentShaderAvailable: EAX = [0078E4FC] & ((caps & [0078E4D0]) == [0078E4D0]).
extern "C" I __cdecl aur_ati_fragment_shader_available_0045f560() {
    const U mask = procs::mask_0078e4d0;                                   // 0045F560
    const U included = ((rc::capabilities_007bb85c & mask) == mask) ? 1u : 0u;   // 0045F565..0045F578 and/sub/neg/sbb/inc
    return static_cast<I>(rc::control_0078e4fc & included);                // 0045F56F / 0045F579
}

// 0045F5A0 getATIFragmentShadersDisabled: cache 0078E504 = ((control & included) == 0); AL = (cache == 1).
extern "C" std::uint8_t __cdecl get_ati_fragment_shaders_disabled_0045f5a0() {
    U cache = vc::cache_0078e504;                                          // 0045F5A0
    if (cache == 0xFFFFFFFFu) {                                            // 0045F5A6..0045F5A9
        const U mask = procs::mask_0078e4d0;                               // 0045F5AB
        const U included = ((rc::capabilities_007bb85c & mask) == mask) ? 1u : 0u;   // 0045F5B1..0045F5C4
        cache = ((rc::control_0078e4fc & included) == 0) ? 1u : 0u;        // 0045F5BA / 0045F5C5..0045F5CB
        vc::cache_0078e504 = cache;                                        // 0045F5CC
    }
    return cache == 1u ? 1 : 0;                                            // 0045F5D2..0045F5D5 cmp ecx,1; sete al
}

// 0045F740 AurVertexProgramsNVAvailable: cache 0078E518 = ((caps & [0078E49C]) == [0078E49C]); EAX = cache.
extern "C" U __cdecl aur_vertex_programs_nv_available_0045f740() {
    U cache = vc::cache_0078e518;                                          // 0045F740
    if (cache == 0xFFFFFFFFu) {                                            // 0045F745..0045F748
        const U mask = procs::mask_0078e49c;                               // 0045F74A
        cache = ((rc::capabilities_007bb85c & mask) == mask) ? 1u : 0u;    // 0045F750..0045F75D
        vc::cache_0078e518 = cache;                                        // 0045F75E
    }
    return cache;                                                          // 0045F763 (full EAX)
}

// 0045FB90 aurATIFragmentShadersBumpMapAvailable.
extern "C" std::uint8_t __cdecl aur_ati_fragment_shaders_bump_map_available_0045fb90() {
    const I level = rc::vertex_buffer_level_0078e500;                      // 0045FB90 (edx, kept)
    if (level < 1) return 0;                                               // 0045FB96..0045FB99 jl -> xor al,al
    const U mask = procs::mask_0078e4d0;                                   // 0045FB9B
    const U included = ((rc::capabilities_007bb85c & mask) == mask) ? 1u : 0u;   // 0045FBA1..0045FBB4
    if ((included & rc::control_0078e4fc) != 0 && level == 1) return 0;    // 0045FBAA / 0045FBB5..0045FBBC
    return BumpMapAvailable_0045fb10() != 0 ? 1 : 0;                       // 0045FBC1..0045FBC8 setne al
}

// 0045FBD0 AurNV2xLevelShadersAvailable: [0078E500] >= 2 && AurBumpyShinyAvailable().
extern "C" bool __cdecl aur_nv2x_level_shaders_available_0045fbd0() {
    if (rc::vertex_buffer_level_0078e500 < 2) return false;                // 0045FBD0..0045FBDB (signed jge)
    return kotor_renderer_gl::BumpyShinyAvailable_0045f680() != 0;         // 0045FBDC..0045FBE3 setne al
}

// ================================================================ ARB/NV program wrappers
namespace kotor_vertex_program {
// 0046F550 bindVertexProgram: cdecl(program), plain RET.
void __cdecl BindVertexProgram_0046f550(std::uint32_t program) {
    if (kotor_renderer_gl_cells::programs_0078e5ec != 0) {                 // 0046F550..0046F557
        kotor_renderer_gl_procs::glBindProgramARB_007bb788(0x8620, program);   // 0046F559..0046F563
        return;
    }
    kotor_renderer_gl_procs::glBindProgramNV_007bb834(0x8620, program);    // 0046F56A..0046F574
}

// 004A23C0 loadVertexProgram: cdecl(program, length, text, parameters); [esp+10h] (parameters) is never read.
void __cdecl LoadVertexProgram_004a23c0(std::uint32_t program, std::uint32_t length, const char* text,
                                        std::uint32_t* parameters) {
    (void)parameters;
    if (kotor_renderer_gl_cells::programs_0078e5ec != 0) {                 // 004A23C0..004A23C7
        // 004A23C9..004A23DD glProgramStringARB(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, length, text);
        // the program id ([esp+4]) is not passed (the caller bound it at 004A26A3..004A26B3).
        kotor_renderer_gl_procs::glProgramStringARB_007bb580(0x8620, 0x8875, static_cast<std::int32_t>(length), text);
        return;
    }
    // 004A23E4..004A23F8 glLoadProgramNV(GL_VERTEX_PROGRAM_NV, program, length, text)
    kotor_renderer_gl_procs::glLoadProgramNV_007bb5e4(0x8620, program, static_cast<std::int32_t>(length),
                                                      reinterpret_cast<const std::uint8_t*>(text));
}

// 004A2770 addVertexProgramToManager: cdecl(program); CExoArrayList<CAurVertexProgram*>::Add on list 008305CC
// (owner lane-window). Growth: capacity ? capacity*2 : 8 through CExoArrayList::Allocate 005E03C0.
void __cdecl AddVertexProgramToManager_004a2770(VertexProgram* program) {
    auto& list = kotor_aurora_window::vertex_programs_008305cc;
    const std::int32_t size = list.size;                                   // 004A2770 ecx=[008305D0]
    const std::int32_t capacity = list.capacity;                           // 004A2776 eax=[008305D4]
    if (size == capacity) {                                                // 004A277B..004A277D
        const std::uint32_t grown = capacity != 0 ? static_cast<std::uint32_t>(capacity + capacity) : 8u; // 004A277F..004A2787
        menu_list_growth::allocate_005e03c0(&list, nullptr, grown);        // 004A278C..004A2792 (ecx=8305CCh)
        const std::int32_t index = list.size;                              // 004A2797 reload [008305D0]
        reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(list.data))[index] =
            static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(program));   // 004A279C..004A27A6
        list.size = list.size + 1;                                         // 004A27A9 inc [008305D0]
        return;
    }
    reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(list.data))[size] =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(program));       // 004A27B0..004A27BA
    list.size = list.size + 1;                                             // 004A27BD
}

// ================================================================ 004A24D0 CAurVertexProgram::Enable
// __thiscall(lighting_input, lighting byte), RET 8 (canonical member spelling of vertex_program.h).
void __thiscall VertexProgram::Enable_004a24d0(const void* lighting_input, std::uint8_t lighting) {
    namespace vc = kotor_vp_cells;
    namespace procs = kotor_renderer_gl_procs;
    // [0078E5EC] is re-read at every ARB/NV test, as in the original.
    const std::uint8_t* const input = static_cast<const std::uint8_t*>(lighting_input);
    // 004A24D0..004A24FA: unchanged program/lighting and no pending refresh -> bind only.
    if (vc::current_source_008305dc == original_source && vc::refresh_007a193c == 0 &&
        vc::lighting_008305d8 == lighting && ValidateNonRadeon_0045f5e0() != 0) {
        BindVertexProgram_0046f550(program_id);                            // 004A24FC..004A2503
        return;                                                            // 004A250D ret 8
    }
    vc::current_source_008305dc = original_source;                         // 004A2510 / 004A251B
    vc::lighting_008305d8 = lighting;                                      // 004A2521
    vc::refresh_007a193c = 0;                                              // 004A2527
    if (input != nullptr && lighting != 0) {                               // 004A2531..004A2535
        const std::int32_t condition = kotor_scene3d::field<std::uint32_t>(const_cast<std::uint8_t*>(input), 0x40) == 0 ? 1 : 0; // 004A2537..004A254A
        void* const scene = *kotor_scene3d::cells.active_scene_007b92a0;   // 004A2544 (no null test, as observed)
        light_manager_set_up_vertex_program_light_shader_constants_004a50d0(
            kotor_scene3d::field<void*>(scene, 0x80), nullptr, allowed_lights, condition);   // 004A254D..004A2555
    }
    if (static_cast<std::uint32_t>(generation) == vc::generation_00830540) {   // 004A255A..004A2566
        goto bind_loaded;                                                  // je 004A2735
    }
    {
        const std::uint8_t exists = kotor_renderer_gl_cells::programs_0078e5ec != 0
            ? procs::glIsProgramARB_007bb5b8(program_id)                   // 004A256C..004A2583
            : procs::glIsProgramNV_007bb5ac(program_id);                   // 004A258B
        if (exists != 0 && lighting == 0) goto bind_loaded;                // 004A2591..004A2597
    }
    if (enabled != 0 && kotor_renderer_gl_cells::format_0078e388 == 0) {   // 004A259D..004A25AF (word compare)
        BindVertexProgram_0046f550(program_id);                            // 004A25B1..004A25B4
        return;                                                            // 004A25C0 ret 8
    }
    generation = static_cast<std::int32_t>(vc::generation_00830540);       // 004A25C3..004A25CC
    {
        // 004A25D2..004A25DD unchecked NUL-terminated copy of the source into the 0x2000 text buffer.
        const char* from = original_source;
        char* to = program_text;
        char c;
        do { c = *from++; *to++ = c; } while (c != 0);
    }
    if (lighting != 0 && allowed_lights > 0) {                             // 004A25DF..004A25EB (signed jle)
        char* const light_text = static_cast<char*>(live_operator_new(0x1000));   // 004A25ED..004A25F7
        light_text[0] = 0;                                                 // 004A2600 (no NULL test, as observed)
        std::int32_t condition = 0;                                        // 004A25FC xor eax,eax
        if (input != nullptr)                                              // 004A25FE..004A2603
            condition = kotor_scene3d::field<std::uint32_t>(const_cast<std::uint8_t*>(input), 0x40) == 0 ? 1 : 0; // 004A2605..004A260A
        (void)SetupVertexProgramLights_004a30e0(light_text, allowed_lights, condition);   // 004A260D..004A2616
        // 004A261B..004A2646 inline strcat(program_text, light_text): length+1 bytes including the NUL.
        std::size_t light_length = 0;
        while (light_text[light_length] != 0) ++light_length;
        char* end = program_text;
        while (*end != 0) ++end;
        std::memcpy(end, light_text, light_length + 1);
        live_free(light_text);                                             // 004A2645..004A2648
    }
    {
        // 004A2658..004A266E: 4-byte store of .rdata 00743950 ("END" + NUL) at the end of the text.
        char* end = program_text;
        while (*end != 0) ++end;
        std::memcpy(end, kotor_vp_data::program_end_00743950, 4);
    }
    if (program_id == 0) {                                                 // 004A2670..004A267E
        if (kotor_renderer_gl_cells::programs_0078e5ec != 0)               // 004A2680..004A268A
            procs::glGenProgramsARB_007bb528(1, &program_id);              // 004A2687..004A268C
        else
            procs::glGenProgramsNV_007bb7b0(1, &program_id);               // 004A2694
    }
    if (kotor_renderer_gl_cells::programs_0078e5ec != 0)                   // 004A269A..004A26A9
        procs::glBindProgramARB_007bb788(0x8620, program_id);              // 004A26AB
    else
        procs::glBindProgramNV_007bb834(0x8620, program_id);               // 004A26B3
    {
        std::uint32_t length = 0;                                          // 004A26B9..004A26C7 strlen
        while (program_text[length] != 0) ++length;
        LoadVertexProgram_004a23c0(program_id, length, program_text, parameter_words);   // 004A26C9..004A26DC
    }
    if (kotor_renderer_gl_cells::programs_0078e5ec != 0)                   // 004A26E1..004A26EB
        kotor_renderer_init::ReportARBError_0047a2c0(program_text);        // 004A26ED..004A26EE
    if (input != nullptr && lighting != 0) {                               // 004A26F6..004A26FC
        void* const scene = *kotor_scene3d::cells.active_scene_007b92a0;   // 004A26FE
        if (scene != nullptr) {                                            // 004A2703..004A2705
            const std::int32_t condition = kotor_scene3d::field<std::uint32_t>(const_cast<std::uint8_t*>(input), 0x40) == 0 ? 1 : 0; // 004A2707..004A2714
            light_manager_set_up_vertex_program_light_shader_constants_004a50d0(
                kotor_scene3d::field<void*>(scene, 0x80), nullptr, allowed_lights, condition);   // 004A2717..004A271F
        }
    }
    enabled = 1;                                                           // 004A2726
    return;                                                                // 004A2732 ret 8
bind_loaded:
    // 004A2735..004A2761: bind the already-loaded program (ARB or NV by [0078E5EC]).
    if (kotor_renderer_gl_cells::programs_0078e5ec != 0)
        procs::glBindProgramARB_007bb788(0x8620, program_id);              // 004A274A
    else
        procs::glBindProgramNV_007bb834(0x8620, program_id);               // 004A2757
}
} // namespace kotor_vertex_program

// ================================================================ TriMesh VP render paths (cdecl(primitive, part))
// 00470FE0 RenderFlatVP: glEnable(GL_VERTEX_PROGRAM); static program (ARB 0078FEB8 / NV 0079AB28, params [00830560]);
// Enable(part, 1); RenderFlat([[primitive]+4](primitive)); glDisable(GL_VERTEX_PROGRAM).
extern "C" void __cdecl render_flat_vp_00470fe0(void* primitive, void* part) {
    glEnable_0073d390(rc::vertex_enum_0078d46c);                           // 00470FF6..00470FFC
    VertexProgram* const program = trimesh_static_program(vc::flat_program_008280f0, vc::flat_guard_008280f4,
        kotor_vp_data::arb_flat_vp_0078feb8, kotor_trimesh_data::nv_program_0079ab28);   // 00471002..0047105B
    program->Enable_004a24d0(part, 1);                                     // 00471068..00471075
    render_flat_0046f900(primitive_slot4(primitive));                      // 0047107A..00471084
    glDisable_0073d22c(rc::vertex_enum_0078d46c);                          // 00471089..00471092
}

// 004710B0 RenderLightmappedVP: as 00470FE0 with static 008280F8 (same texts) and RenderLightMapped 0046FA40.
extern "C" void __cdecl render_lightmapped_vp_004710b0(void* primitive, void* part) {
    glEnable_0073d390(rc::vertex_enum_0078d46c);                           // 004710C6..004710CC
    VertexProgram* const program = trimesh_static_program(vc::lightmapped_program_008280f8, vc::lightmapped_guard_008280fc,
        kotor_vp_data::arb_flat_vp_0078feb8, kotor_trimesh_data::nv_program_0079ab28);   // 004710D2..0047112B
    program->Enable_004a24d0(part, 1);                                     // 00471138..00471145
    render_light_mapped_0046fa40(primitive_slot4(primitive));              // 0047114A..00471154
    glDisable_0073d22c(rc::vertex_enum_0078d46c);                          // 00471159..00471162
}

// Program environment parameters 0x0F = (0,0,1,0) and 0x10 = (0,2,3,1) written directly through the
// ARB/NV procedure slots (00471221..00471272 / 00471351..004713A2); [0078E5EC] read once per call.
namespace {
void env_parameters_0f_10() {
    if (programs_arb()) procs::glProgramEnvParameter4fARB_007bb850(kVertexProgramTarget, 0x0F, 0.0f, 0.0f, 1.0f, 0.0f);
    else procs::glProgramParameter4fNV_007bb7ac(kVertexProgramTarget, 0x0F, 0.0f, 0.0f, 1.0f, 0.0f);
    if (programs_arb()) procs::glProgramEnvParameter4fARB_007bb850(kVertexProgramTarget, 0x10, 0.0f, 2.0f, 3.0f, 1.0f);
    else procs::glProgramParameter4fNV_007bb7ac(kVertexProgramTarget, 0x10, 0.0f, 2.0f, 3.0f, 1.0f);
}
}

// 00471180 RenderEnvironmentMappedVP: static 00828100 (ARB 00790300 / NV 0079AB28); Enable(part,1);
// env params 0F/10; RenderEnvironmentMapped([[primitive]+4](primitive), 0, 0).
extern "C" void __cdecl render_environment_mapped_vp_00471180(void* primitive, void* part) {
    glEnable_0073d390(rc::vertex_enum_0078d46c);                           // 00471196..0047119C
    VertexProgram* const program = trimesh_static_program(vc::env_program_00828100, vc::env_guard_00828104,
        kotor_vp_data::arb_env_vp_00790300, kotor_trimesh_data::nv_program_0079ab28);    // 004711A2..004711FB
    program->Enable_004a24d0(part, 1);                                     // 00471208..00471215
    // 0047121A..00471243 / 00471243..00471272 (each test reads [0078E5EC] before its pushes)
    if (programs_arb()) procs::glProgramEnvParameter4fARB_007bb850(kVertexProgramTarget, 0x0F, 0.0f, 0.0f, 1.0f, 0.0f);
    else procs::glProgramParameter4fNV_007bb7ac(kVertexProgramTarget, 0x0F, 0.0f, 0.0f, 1.0f, 0.0f);
    if (programs_arb()) procs::glProgramEnvParameter4fARB_007bb850(kVertexProgramTarget, 0x10, 0.0f, 2.0f, 3.0f, 1.0f);
    else procs::glProgramParameter4fNV_007bb7ac(kVertexProgramTarget, 0x10, 0.0f, 2.0f, 3.0f, 1.0f);
    render_environment_mapped_0046ff70(primitive_slot4(primitive), 0, 0);  // 00471272..00471280
    glDisable_0073d22c(rc::vertex_enum_0078d46c);                          // 00471285..0047128E
}

// 004712B0 RenderEMLMVP: static 00828108 (ARB 007908B8 / NV 0079AB28); Enable(part,1); env params 0F/10;
// RenderEMLM([[primitive]+4](primitive)).
extern "C" void __cdecl render_emlm_vp_004712b0(void* primitive, void* part) {
    glEnable_0073d390(rc::vertex_enum_0078d46c);                           // 004712C6..004712CC
    VertexProgram* const program = trimesh_static_program(vc::emlm_program_00828108, vc::emlm_guard_0082810c,
        kotor_vp_data::arb_emlm_vp_007908b8, kotor_trimesh_data::nv_program_0079ab28);   // 004712D2..0047132B
    program->Enable_004a24d0(part, 1);                                     // 00471338..00471345
    env_parameters_0f_10();                                                // 0047134A..004713A2
    render_emlm_004703f0(primitive_slot4(primitive));                      // 004713A2..004713AC
    glDisable_0073d22c(rc::vertex_enum_0078d46c);                          // 004713B1..004713BA
}

// ================================================================ skinned VP function-local statics
// Shape (00487310/004873C0/00487460): test byte [guard],1; jne return [static]; or [guard],1; operator_new(201Ch);
// NULL -> [static]=0, return 0; else [static] = Construct(text, [params]) and return it. SEH state covers only the
// constructor (no throwing callee; operator new returns NULL).
namespace {
VertexProgram* skin_static_program(VertexProgram* volatile& program, volatile U& guard, char* (*select)(),
                                   std::uint32_t* volatile& parameters) {
    if ((guard & 1u) != 0) return program;                                 // test/jne -> mov eax,[static]
    guard = guard | 1u;
    void* storage = live_operator_new(kProgramSize);
    if (storage == nullptr) { program = nullptr; return nullptr; }
    char* const text = select();
    VertexProgram* const constructed = static_cast<VertexProgram*>(storage)->Construct_004a27d0(text, parameters);
    program = constructed;
    return constructed;
}
char* skin_text() { return programs_arb() ? kotor_vp_data::arb_skin_vp_00792658 : kotor_vp_data::nv_skin_vp_0079b5a8; }    // 00487358..00487367
char* skin_sphere_text() { return kotor_vp_data::nv_skin_sphere_vp_0079e958; }                                                // 0048740F (no ARB test)
char* skin_cube_text() { return programs_arb() ? kotor_vp_data::arb_skin_cube_vp_007938c8 : kotor_vp_data::nv_skin_cube_vp_0079f128; } // 004874A8..004874B7
}

// 00487310 GetSkinVP (static 00830390, guard 00830394, params [00830578]).
extern "C" VertexProgram* __cdecl get_skin_vp_00487310() {
    return skin_static_program(vc::skin_program_00830390, vc::skin_guard_00830394, &skin_text, vc::parameters_00830578);  // 00487326..004873B8
}
// 004873C0 GetSkinSphereMapVP (static 00830398, guard 0083039C, NV text 0079E958 only, params [0083058C]).
extern "C" VertexProgram* __cdecl get_skin_sphere_map_vp_004873c0() {
    return skin_static_program(vc::skin_sphere_program_00830398, vc::skin_sphere_guard_0083039c, &skin_sphere_text,
                               vc::parameters_0083058c);                   // 004873D6..00487458
}
// 00487460 GetSkinCubeMapVP (static 008303A0, guard 008303A4, params [00830590]).
extern "C" VertexProgram* __cdecl get_skin_cube_map_vp_00487460() {
    return skin_static_program(vc::skin_cube_program_008303a0, vc::skin_cube_guard_008303a4, &skin_cube_text,
                               vc::parameters_00830590);                   // 00487476..00487508
}
