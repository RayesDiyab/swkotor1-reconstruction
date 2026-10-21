// trimesh_f3.cpp - lane-3d-mesh fork F3: light-mapped / bump-mapped per-light vertex-program passes.
//   00474EA0 RenderLightMappedBumpDiffuse   00475450 RenderLightMappedBumpSpecular
//   00475BB0 RenderDiffuseBumpMappedPass    004760D0 RenderSpecularBumpMappedPass
// Evidence: lane-3d-mesh/d/<ADDR>.s.txt (dumpbin of the pinned GOG swkotor.exe); every statement cites the
// original instruction(s). Stack-local offsets in comments are relative to the function's body frame.
//
// EH (D30-FINAL): each body has an x86 FS:[0] frame (handlers 00716FE1 / 00717017 / 00717051 / 00717084) whose
// funclets only (a) clear the function-local-static guard bit (00716FC8, 00716FFB, 00717030, 00717060),
// (b) free the operator-new block if VertexProgram::VertexProgram throws (00716FD6, 00717009, 0071703E, 0071706E),
// (c) free the copied light list (00716FC0 / 00716FF0 / 00717049 / 0071707C -> 0047ABC0). They run only on a C++
// exception. live_operator_new returns NULL (no throw) and no callee here throws on reachable paths, so the funclets
// are cited, not emitted; all locals are trivially destructible, so /EHsc emits no unwind code either.
#include "df9dab63cf-trimesh_skin.h"

// LOWERS hoisted to trimesh_skin.h (lane merge)

// LightManagerTablePrefix hoisted to trimesh_skin.h

// ---------------------------------------------------------------- function-local statics owned here
// Guard words (bit 0 = constructed; `test byte [g],1` / `or dword [g],1`), VertexProgram pointers, and the
// parameter-pointer cells passed to VertexProgram::VertexProgram (BSS, no writer instruction anywhere -> NULL).
extern "C" {
kotor_vertex_program::VertexProgram* volatile vertex_program_008301a8 = nullptr;   // 00474FCA
volatile kotor_scene3d::U vertex_program_guard_008301ac = 0;                         // 00474F66 / 00474F87
kotor_vertex_program::VertexProgram* volatile vertex_program_008301b0 = nullptr;   // 004755A8
volatile kotor_scene3d::U vertex_program_guard_008301b4 = 0;                         // 0047552C / 00475565
kotor_vertex_program::VertexProgram* volatile vertex_program_008301b8 = nullptr;   // 00475D6A
volatile kotor_scene3d::U vertex_program_guard_008301bc = 0;                         // 00475D1B / 00475D24
kotor_vertex_program::VertexProgram* volatile vertex_program_008301c0 = nullptr;   // 00476279
volatile kotor_scene3d::U vertex_program_guard_008301c4 = 0;                         // 0047622A / 00476233
std::uint32_t* volatile program_parameters_00830554 = nullptr;                     // 00475591
std::uint32_t* volatile program_parameters_00830558 = nullptr;                     // 00474FB3
std::uint32_t* volatile program_parameters_0083055c = nullptr;                     // 00475D53 / 00476262
}

namespace {
using namespace kotor_trimesh;
using kotor_vertex_program::VertexProgram;
namespace gl = kotor_renderer_init;

// Original .rdata GL constants (literals; addresses cited at each use).
constexpr U kSrcAlpha = 0x302, kOneMinusSrcAlpha = 0x303;   // 0073F270 / 0073F274
constexpr U kGlOne = 1, kGlZero = 0;                         // 0073F278 / 0073F27C
constexpr U kGlLinear = 0x2601;                              // 0073F25C
constexpr U kTexCoordArray = 0x8078;                         // 0073F398
constexpr U kNormalArray = 0x8075, kColorArray = 0x8076;     // 0073F39C / 0073F3A0
constexpr U kAttrib5 = 0x8655, kAttrib12 = 0x865C, kAttrib13 = 0x865D, kAttrib14 = 0x865E, kAttrib15 = 0x865F; // 0073F3B4..0073F3C4
constexpr U kOneBits = 0x3F800000u, kMinusOneBits = 0xBF800000u, kHalfBits = 0x3F000000u;

const kotor_scene3d::Cells& sc() { return kotor_scene3d::cells; }
const MeshCells& mc() { return mesh_cells; }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* light_at(const PointerList& list, I index) { return field<void*>(as_object(list.data), static_cast<U>(index) * 4u); }
inline Texture* texture_slot(Material* material, int index) { return material->textures[index].target; }
inline texture::Basic* basic(Texture* t) { return reinterpret_cast<texture::Basic*>(t); }   // object view (D24)
inline texture::Basic* basic(void* t) { return static_cast<texture::Basic*>(t); }
inline void copy3(Vector* out, const void* source) { std::memcpy(out, source, sizeof(Vector)); }   // raw DWORD moves
inline float sub_store(float a, float b) { return static_cast<float>(static_cast<double>(a) - static_cast<double>(b)); }
inline float mul_store(float a, float b) { return static_cast<float>(static_cast<double>(a) * static_cast<double>(b)); }
inline float scale_cell(const volatile float* cell) { return *cell; }

void enable_dispatch(U op, texture::Basic* a, texture::Basic* b, texture::Basic* c, texture::Basic* d) {
    (*mc().enable_dispatch_007a47e0)(op, a, b, c, d);        // call dword ptr ds:[007A47E0h] (cell re-read per call)
}
// `mov eax,[0078E5EC]; test eax,eax; ... call [007BB850] | call [007BB7AC]` with target 8620h.
void vertex_program_parameter(U index, U x, U y, U z, U w) {
    if (*sc().programs_0078e5ec != 0)
        (*sc().program_env_parameter_007bb850)(0x8620u, index, bits_to_float(x), bits_to_float(y), bits_to_float(z), bits_to_float(w));
    else
        (*sc().program_parameter_nv_007bb7ac)(0x8620u, index, bits_to_float(x), bits_to_float(y), bits_to_float(z), bits_to_float(w));
}
void env_parameter(U index, U x, U y, U z, U w) {   // 0044F820 glProgramEnvParameter4fARB wrapper (cdecl, raw DWORDs)
    gl_program_env_parameter_0044f820(index, bits_to_float(x), bits_to_float(y), bits_to_float(z), bits_to_float(w));
}
// Function-local static `new VertexProgram(text, *params)` under guard bit 0.
void construct_static_program(volatile U& guard, VertexProgram* volatile& program, char* text,
                              std::uint32_t* volatile& parameters) {
    if ((guard & 1u) != 0) return;                               // test byte ptr [g],1 ; jne
    guard = guard | 1u;                                          // or dword ptr [g],1
    void* const memory = live_operator_new(0x201Cu);             // push 201Ch ; call 006FA7E6
    VertexProgram* result = nullptr;                             // xor eax,eax (NULL path)
    if (memory != nullptr)                                       // test eax,eax ; je
        result = static_cast<VertexProgram*>(memory)->Construct_004a27d0(text, parameters); // push [params]; push text; call 004A27D0
    program = result;                                            // mov [vp],eax
}
// glProgramEnvParameter 0x10/0x11 block shared by 00474EA0 (00475073..004750B9) and 00475450 (00475786..004757CB).
void scene_fog_parameters(void* scene) {
    U flag = kOneBits;                                           // mov [esp+10h],3F800000h
    if (field<U>(scene, 0x60) == 0) flag = 0;                    // cmp [scene+60h],0 ; mov [esp+10h],0
    env_parameter(0x10, field<U>(scene, 0x64), field<U>(scene, 0x68), flag, kHalfBits);
    env_parameter(0x11, kOneBits, kMinusOneBits, 0, 0);
}
// 9 / (2 r^2) -> fstp dword (004752CB..004752E6): x87 wide (double) then one float store.
U attenuation_bits(void* light) {
    const double r = static_cast<double>(field<float>(light, 0x58));   // fld [light+58h]
    double t = r * r;                                                   // fld st(0) ; fmul st,st(1)
    t = t + t;                                                          // fadd st(0),st
    return float_to_bits(static_cast<float>(9.0 / t));                  // fdivr [00742F00h] (9.0f) ; fstp dword
}
const kotor_scene3d::SceneVTable* scene_table(void* scene) { return static_cast<const kotor_scene3d::SceneVTable*>(field<void*>(scene, 0)); }
const LightManagerTablePrefix* lights_table(void* lights) { return static_cast<const LightManagerTablePrefix*>(field<void*>(lights, 0)); }
} // namespace

extern "C" {
// ============================================================== 00474EA0 RenderLightMappedBumpDiffuse (cdecl)
void __cdecl render_light_mapped_bump_diffuse_00474ea0(void* primitive) {
    if (*sc().active_scene_007b92a0 == nullptr) {                         // 00474EAE / 00474EC3
        render_flat_0046f900(primitive);                                   // 00474EC7..00474ECF ; jmp 00475435
        return;
    }
    gl::glBlendFunc_0073d38c(kSrcAlpha, kOneMinusSrcAlpha);               // 00474ED9..00474EE7
    PointerList lights;                                                    // [esp+2Ch] (B frame)
    int_list_copy_construct_0046d250(&lights, nullptr,                     // 00474EFE..00474F02
        light_manager_get_bump_map_lights_004a28a0(field<void*>(*sc().active_scene_007b92a0, 0x80), nullptr)); // 00474EED..00474EF8
    if (lights.size == 0) {                                                // 00474F07 cmp [esp+30h],0
        render_light_mapped_0046fa40(primitive);                           // 00474F14..00474F1C ; jmp 0047541C
    } else if (aur_pixel_shaders_available_0046eda0() != 0) {              // 00474F29..00474F30
        gl::glDisable_0073d22c(*sc().gl_fog_0078d450);                     // 00474F36..00474F46
        void* const host = field<void*>(primitive, 8);                     // 00474F48..00474F4F
        void* const part = as_object(part_table(host)->s02c(host, nullptr)); // 00474F52..00474F57 (esi; [S+48h])
        void* node = nullptr;                                              // 00474F59 xor edi,edi
        if (part != nullptr) node = field<void*>(part, 4);                 // 00474F5B..00474F63
        Vector bump_scale{};                                               // [S+2Ch..34h] = 0 (00474F6D..00474F7D)
        construct_static_program(vertex_program_guard_008301ac, vertex_program_008301a8,   // 00474F66..00474FCF
            kotor_trimesh_data::nv_program_0079a600, program_parameters_00830558);
        gl::glEnable_0073d390(*mc().vertex_program_0078d46c);              // 00474FD7..00474FDE
        if (node != nullptr) copy3(&bump_scale, at(node, 0x8C));            // 00474FE4..00474FFE
        const U alpha_was = gl::glIsEnabled_0073d37c(*sc().gl_alpha_test_0078d44c); // 00475002..00475018 (movzx -> [S+44h])
        gl::glDisable_0073d22c(*sc().gl_alpha_test_0078d44c);              // 0047500E / 0047501C
        gl_render_activate_texture_stage_00425250(1);                      // 0047501E..00475020
        gl::glEnableClientState_0073d260(kTexCoordArray);                  // 00475025..00475034
        gl::glEnableClientState_0073d260(kAttrib12);                       // 00475036..0047503D
        gl::glEnableClientState_0073d260(kAttrib13);                       // 0047503F..00475046
        gl::glEnableClientState_0073d260(kAttrib14);                       // 00475048..0047504E
        Material* const material = material_of(part);                      // 00475050 [part+44h]
        enable_dispatch(8, basic(texture_slot(material, 4)),               // 00475050..0047506D: (8, tex[4], scene+A4, tex[1], tex[0])
            basic(field<void*>(*sc().active_scene_007b92a0, 0xA4)),
            basic(texture_slot(material, 1)), basic(texture_slot(material, 0)));
        scene_fog_parameters(*sc().active_scene_007b92a0);                 // 00475073..004750BE
        Quaternion orientation{1.0f, 0.0f, 0.0f, 0.0f};                    // [S+58h..64h] (004750CA..004750E2)
        Vector position{0.0f, 0.0f, 0.0f};                                 // [S+14h..1Ch] (004750EA..004750FA)
        part_table(part)->s00c(part, nullptr, word(&position), word(&orientation)); // 00475102..00475107 GetWorld
        void* const camera = *sc().current_camera_007bb4e8;                // 0047510A
        Vector eye_delta;                                                  // [S+20h]
        eye_delta.x = sub_store(field<float>(camera, 0x7C), position.x);   // 0047512F..0047513C
        eye_delta.y = sub_store(field<float>(camera, 0x80), position.y);   // 0047511E..00475140
        eye_delta.z = sub_store(field<float>(camera, 0x84), position.z);   // 0047510F..00475144
        Quaternion inverse;                                                // [S+8Ch]
        Vector eye_local;                                                  // [S+4Ch]
        quaternion_rotate_004a9bb0(&eye_local, quaternion_negate_w_004a9b50(&inverse, &orientation), &eye_delta); // 00475148..00475156
        for (I i = 0; i < lights.size; ++i) {                              // 0047515B..00475164 / 0047535F..00475362
            void* const manager = field<void*>(*sc().active_scene_007b92a0, 0x80);   // 00475170..00475175
            lights_table(manager)->s2c(manager, nullptr);                  // 0047517B..0047517D DisableBumpMapLights
            vertex_program_008301a8->Enable_004a24d0(part, 1);             // 00475180..00475189
            Vector light_position;                                         // [S+20h..28h] raw (0047518E..004751B7)
            copy3(&light_position, at(light_at(lights, i), 0x94));
            Vector light_delta;                                            // [S+74h]
            light_delta.x = sub_store(light_position.x, position.x);       // 004751C3..004751D8
            light_delta.y = sub_store(light_position.y, position.y);       // 004751B3..004751DD
            light_delta.z = sub_store(light_position.z, position.z);       // 004751A7..004751E4
            Quaternion inverse_light;                                      // [S+9Ch]
            Vector light_local;                                            // [S+8Ch]
            Vector* const rotated = quaternion_rotate_004a9bb0(&light_local,   // 004751EB..004751FC
                quaternion_negate_w_004a9b50(&inverse_light, &orientation), &light_delta);
            const U lx = float_to_bits(rotated->x), ly = float_to_bits(rotated->y), lz = float_to_bits(rotated->z); // 0047520B / 00475211 / 0047521A (raw)
            const float s = scale_cell(mc().scale_0078e648);
            Vector scaled;                                                 // [S+68h]
            scaled.z = mul_store(bump_scale.z, s);                         // 00475201..00475205 -> 0047524A
            scaled.y = mul_store(bump_scale.y, s);                         // 0047520D..00475214 -> 0047523B
            scaled.x = mul_store(bump_scale.x, s);                         // 0047521D..00475225 -> 00475233
            Vector color;                                                  // [S+80h]
            vector_multiply_004ab1e0(&color, static_cast<const Vector*>(at(light_at(lights, i), 0x4C)), &scaled); // 00475221..00475251
            (*mc().constant1_dispatch_007a47dc)(&color.x, 1.0f);           // 00475256..00475263
            vertex_program_parameter(0x0C, lx, ly, lz, kOneBits);          // 00475269..0047528C
            vertex_program_parameter(0x0D, float_to_bits(eye_local.x), float_to_bits(eye_local.y),  // 00475292..004752BE
                float_to_bits(eye_local.z), kOneBits);
            const U attenuation = attenuation_bits(light_at(lights, i));   // 004752C4..004752EA
            vertex_program_parameter(0x0E, kOneBits, 0, attenuation, kOneBits); // 004752CE..0047531C
            render_flat_0046f900(primitive);                               // 00475322..0047532A
            if (alpha_was != 0) gl::glEnable_0073d390(*sc().gl_alpha_test_0078d44c); // 0047532F..00475340
            void* const manager_after = field<void*>(*sc().active_scene_007b92a0, 0x80); // 00475346..0047534C
            lights_table(manager_after)->s30(manager_after, nullptr);      // 00475352..00475354 EnableBumpMapLights
        }
        if (aur_multi_texture_available_0045f710() != 0)                   // 0047536E..00475375
            (*sc().client_active_texture_007bb6b0)(0x84C1);                // 00475377..0047537C
        *sc().current_stage_007a6898 = 1;                                  // 0047538E (before the call)
        gl::glDisableClientState_0073d264(kTexCoordArray);                 // 00475382..00475398
        if (aur_multi_texture_available_0045f710() != 0)                   // 0047539A..004753A1
            (*sc().client_active_texture_007bb6b0)(0x84C0);                // 004753A3..004753A8
        *sc().current_stage_007a6898 = 0;                                  // 004753B7
        gl::glDisableClientState_0073d264(kAttrib12);                      // 004753AE..004753BD
        gl::glDisableClientState_0073d264(kAttrib13);                      // 004753BF..004753C6
        gl::glDisableClientState_0073d264(kAttrib14);                      // 004753C8..004753CE
        void* const scene = *sc().active_scene_007b92a0;                   // 004753D0
        scene_table(scene)->s124_scene_do_fog(scene, nullptr);             // 004753D6..004753D8 Scene::DoFog
        enable_dispatch(0x20, nullptr, nullptr, nullptr, nullptr);         // 004753DE..004753E4
        enable_dispatch(0x0D, nullptr, nullptr, nullptr, nullptr);         // 004753EA..004753F0
        enable_dispatch(0x00, nullptr, nullptr, nullptr, nullptr);         // 004753F6..004753FB
        gl::glDisable_0073d22c(*mc().vertex_program_0078d46c);             // 00475401..0047540A
        Texture* const base = material_of(part)->textures[0].target;       // 0047540C..0047540F
        texture_vtable::bind_tail(base)->sb4(base, nullptr);               // 00475411..00475413 call [vt+B4h]
    }
    if (lights.data != 0) live_free(as_object(lights.data));               // 0047541C..00475430 (inline list dtor)
}

// ============================================================== 00475450 RenderLightMappedBumpSpecular (cdecl)
void __cdecl render_light_mapped_bump_specular_00475450(void* primitive) {
    if (*sc().active_scene_007b92a0 == nullptr) {                         // 0047545E / 00475473
        render_flat_0046f900(primitive);                                   // 00475477..0047547F ; jmp 00475B88
        return;
    }
    gl::glBlendFunc_0073d38c(kSrcAlpha, kOneMinusSrcAlpha);               // 00475489..00475497
    PointerList lights;                                                    // [esp+3Ch] (B frame)
    int_list_copy_construct_0046d250(&lights, nullptr,                     // 004754AD..004754B2
        light_manager_get_bump_map_lights_004a28a0(field<void*>(*sc().active_scene_007b92a0, 0x80), nullptr)); // 0047549D..004754A8
    if (lights.size == 0) {                                                // 004754B7
        render_light_mapped_0046fa40(primitive);                           // 004754C4..004754CC ; jmp 00475B75
    } else {
        (void)gl::glIsEnabled_0073d37c(*sc().gl_alpha_test_0078d44c);       // 004754DF..004754EE (result discarded: eax reloaded at 004754F0)
        gl::glDisable_0073d22c(*sc().gl_alpha_test_0078d44c);              // 004754F0..004754FC
        if (aur_pixel_shaders_available_0046eda0() != 0) {                 // 004754FE..00475505
            void* const host = field<void*>(primitive, 8);                 // 0047550B..00475512
            void* const part = as_object(part_table(host)->s02c(host, nullptr)); // 00475515..0047551B
            gl::glDisable_0073d22c(*sc().gl_fog_0078d450);                 // 0047551D..00475523
            void* node = nullptr;                                          // 00475471 xor edi,edi
            if (part != nullptr) node = field<void*>(part, 4);             // 00475525..00475529
            Vector specular{};                                             // [S+30h..38h] = 0 (00475533..00475543)
            Vector bump_scale{};                                           // [S+24h..2Ch] = 0 (0047554B..0047555B)
            construct_static_program(vertex_program_guard_008301b4, vertex_program_008301b0,   // 0047552C..004755AD
                kotor_trimesh_data::nv_program_0079a218, program_parameters_00830554);
            gl::glEnable_0073d390(*mc().vertex_program_0078d46c);          // 004755B5..004755BC
            if (node != nullptr) {                                         // 004755C2..004755C4
                Texture* const bump = material_of(part)->textures[4].target;    // 004755CA..004755CD
                Vector color;                                              // [S+18h]
                texture_head(bump)->common.s88(bump, nullptr, &color);     // 004755CF..004755D7 GetSpecularColor (out)
                specular = color;                                          // 004755DD..004755F1 raw DWORD moves
                copy3(&bump_scale, at(node, 0x8C));                        // 004755F5..00475610
                const float magnitude = static_cast<float>(vector_magnitude_004ab100(&bump_scale)); // 00475614..00475619 fstp [S+14h]
                double specular_magnitude = 0.0;                           // 00475644 fld [0073D700h] (0.0f)
                if (gate_0078e5f4 != 0 &&                                  // 0047561D..00475627
                    material_bump_map_is_specular_0047af50(material_of(part), nullptr) != 0) // 00475629..00475633
                    specular_magnitude = vector_magnitude_004ab100(&specular);           // 00475635..0047563A (ST0, wide)
                Vector weighted;                                           // [S+18h]
                weighted.z = mul_store(bump_scale.z, magnitude);           // 0047564A..0047564F -> 00475676
                weighted.y = mul_store(bump_scale.y, magnitude);           // 00475657..00475662 -> 00475672
                weighted.x = mul_store(bump_scale.x, magnitude);           // 00475666..0047566A -> 0047566E
                const float divisor = static_cast<float>(specular_magnitude + static_cast<double>(magnitude)); // 0047567A..0047567E
                Vector quotient;                                           // [S+94h]
                const Vector* const q = vector_divide_00437a90(&quotient, &weighted, divisor); // 00475681..00475683
                copy3(&bump_scale, q);                                     // 00475688..0047569B
            }
            const U alpha_was = gl::glIsEnabled_0073d37c(*sc().gl_alpha_test_0078d44c); // 0047569F..004756B1 ([S+54h])
            gl::glDisable_0073d22c(*sc().gl_alpha_test_0078d44c);          // 004756AB..004756B5
            gl_render_activate_texture_stage_00425250(1);                  // 004756B7..004756B9
            gl::glEnableClientState_0073d260(kTexCoordArray);              // 004756BE..004756CE
            gl::glEnableClientState_0073d260(kAttrib12);                   // 004756D0..004756D7
            gl::glEnableClientState_0073d260(kAttrib13);                   // 004756D9..004756DF
            gl::glEnableClientState_0073d260(kAttrib14);                   // 004756E1..004756E8
            const U mode = gate_0078e634;                                  // 004756EA
            U op = 0;
            if (mode == 1) op = 9;                                         // 004756EF..0047570F
            else if (mode == 2) op = 0x0A;                                 // 00475713..00475733
            else if (mode == 4) op = 0x0B;                                 // 00475737..00475757
            else if (mode == 8) op = 0x0C;                                 // 0047575B..0047577B
            if (op != 0) {                                                 // other values: 0047575E jne 00475786
                Material* const material = material_of(part);
                enable_dispatch(op, basic(texture_slot(material, 4)),      // 0047577D (op, tex[4], scene+A4, tex[1], tex[0])
                    basic(field<void*>(*sc().active_scene_007b92a0, 0xA4)),
                    basic(texture_slot(material, 1)), basic(texture_slot(material, 0)));
            }
            scene_fog_parameters(*sc().active_scene_007b92a0);             // 00475786..004757D0
            Quaternion orientation{1.0f, 0.0f, 0.0f, 0.0f};                // [S+6Ch..78h] (004757DC..004757F4)
            Vector position{0.0f, 0.0f, 0.0f};                             // [S+3Ch..44h] (004757FC..0047580C)
            part_table(part)->s00c(part, nullptr, word(&position), word(&orientation)); // 00475814..00475819 GetWorld
            void* const camera = *sc().current_camera_007bb4e8;            // 0047581C
            Vector eye_delta;                                              // [S+18h]
            eye_delta.x = sub_store(field<float>(camera, 0x7C), position.x);   // 00475840..0047584E
            eye_delta.y = sub_store(field<float>(camera, 0x80), position.y);   // 00475836..00475852
            eye_delta.z = sub_store(field<float>(camera, 0x84), position.z);   // 00475821..00475856
            Quaternion inverse;                                            // [S+ACh]
            Vector eye_local;                                              // [S+58h]
            quaternion_rotate_004a9bb0(&eye_local, quaternion_negate_w_004a9b50(&inverse, &orientation), &eye_delta); // 0047585A..00475868
            for (I i = 0; i < lights.size; ++i) {                          // 0047586D..0047587E / 00475A9D..00475AA8 ([S+14h])
                vertex_program_008301b0->Enable_004a24d0(part, 1);         // 00475884..0047588D
                Vector light_position;                                     // [S+18h..20h] raw (00475892..004758BE)
                copy3(&light_position, at(light_at(lights, i), 0x94));
                Vector light_delta;                                        // [S+88h]
                light_delta.x = sub_store(light_position.x, position.x);   // 004758CD..004758E2
                light_delta.y = sub_store(light_position.y, position.y);   // 004758BA..004758EA
                light_delta.z = sub_store(light_position.z, position.z);   // 004758AE..004758F1
                Quaternion inverse_light;                                  // [S+BCh]
                Vector light_local;                                        // [S+ACh]
                Vector* const rotated = quaternion_rotate_004a9bb0(&light_local,   // 004758F8..00475909
                    quaternion_negate_w_004a9b50(&inverse_light, &orientation), &light_delta);
                const float s = scale_cell(mc().scale_0078e630);
                const float sz = mul_store(specular.z, s);                 // 0047590E..00475923 [S+68h]
                const U lx = float_to_bits(rotated->x), ly = float_to_bits(rotated->y), lz = float_to_bits(rotated->z); // 0047591E / 00475920 / 0047592B
                const float sy = mul_store(specular.y, s);                 // 00475927..00475939 [S+64h]
                const float sx = mul_store(specular.x, s);                 // 0047593D..00475947 [S+A0h]
                Texture* const bump = material_of(part)->textures[4].target;    // 00475918..0047591B
                const float intensity = texture_head(bump)->common.s80(bump, nullptr); // 00475934..00475958 GetSpecularBumpIntensity -> fstp [S+10h]
                Vector tint;                                               // [S+7Ch]
                tint.z = mul_store(sz, intensity);                         // 0047595C..00475964 -> 0047598E
                tint.y = mul_store(sy, intensity);                         // 0047596C..00475971 -> 00475987
                tint.x = mul_store(sx, intensity);                         // 00475975..0047597C -> 00475980
                Vector color;                                              // [S+94h]
                vector_multiply_004ab1e0(&color, static_cast<const Vector*>(at(light_at(lights, i), 0x4C)), &tint); // 00475995..004759A4
                (*mc().constant0_dispatch_007a6820)(&color.x, 1.0f);       // 004759A9..004759B6
                vertex_program_parameter(0x0C, lx, ly, lz, kOneBits);      // 004759BC..004759DF
                vertex_program_parameter(0x0D, float_to_bits(eye_local.x), float_to_bits(eye_local.y),   // 004759E5..00475A11
                    float_to_bits(eye_local.z), kOneBits);
                const U attenuation = attenuation_bits(light_at(lights, i));   // 00475A17..00475A41
                vertex_program_parameter(0x0E, kOneBits, 0, attenuation, kOneBits); // 00475A25..00475A73
                render_flat_0046f900(primitive);                           // 00475A79..00475A81
                if (alpha_was != 0) gl::glEnable_0073d390(*sc().gl_alpha_test_0078d44c); // 00475A86..00475A97
            }
            gl::glDisableClientState_0073d264(kAttrib5);                   // 00475AB4..00475AC1 (0x8655 never enabled here: original quirk)
            gl::glDisableClientState_0073d264(kAttrib12);                  // 00475AC3..00475ACA
            gl::glDisableClientState_0073d264(kAttrib13);                  // 00475ACC..00475AD2
            gl::glDisableClientState_0073d264(kAttrib14);                  // 00475AD4..00475ADB
            if (aur_multi_texture_available_0045f710() != 0)               // 00475ADD..00475AE4
                (*sc().client_active_texture_007bb6b0)(0x84C1);            // 00475AE6..00475AEB
            *sc().current_stage_007a6898 = 1;                              // 00475AF8
            gl::glDisableClientState_0073d264(kTexCoordArray);             // 00475AF1..00475B02
            if (aur_multi_texture_available_0045f710() != 0)               // 00475B04..00475B0C
                (*sc().client_active_texture_007bb6b0)(0x84C0);            // 00475B0E..00475B13
            *sc().current_stage_007a6898 = 0;                              // 00475B1F
            void* const scene = *sc().active_scene_007b92a0;               // 00475B19
            scene_table(scene)->s124_scene_do_fog(scene, nullptr);         // 00475B29..00475B2B Scene::DoFog
            enable_dispatch(0x20, nullptr, nullptr, nullptr, nullptr);     // 00475B31..00475B3B
            enable_dispatch(0x0D, nullptr, nullptr, nullptr, nullptr);     // 00475B41..00475B4B
            enable_dispatch(0x00, nullptr, nullptr, nullptr, nullptr);     // 00475B51..00475B5B
            gl::glDisable_0073d22c(*mc().vertex_program_0078d46c);         // 00475B61..00475B6B
        }
    }
    if (lights.data != 0) live_free(as_object(lights.data));               // 00475B6D / 00475B75..00475B83 (inline list dtor)
}

// ============================================================== 00475BB0 RenderDiffuseBumpMappedPass (cdecl)
void __cdecl render_diffuse_bump_mapped_pass_00475bb0(void* primitive) {
    if (*mc().gate_0078e614 == 0) return;                                  // 00475BBE..00475BD2 je 004760AD
    if (*sc().active_scene_007b92a0 == nullptr) {                         // 00475BD8..00475BDF
        render_flat_0046f900(primitive);                                   // 00475BE1..00475BE9 ; jmp 004760AA
        return;
    }
    void* const part = field<void*>(primitive, 8);                         // 00475BF4..00475BFE (edi: the [prim+8] word itself)
    const U self = part_table(part)->s02c(part, nullptr);                  // 00475C01..00475C05
    void* node = nullptr;                                                  // 00475C08 xor esi,esi
    if (self != 0) node = field<void*>(part, 4);                           // 00475C0A..00475C0E ([edi+4], not [eax+4])
    Vector bump_scale{};                                                   // [S+14h..1Ch] = 0 (00475C13..00475C23)
    Vector specular{};                                                     // [S+3Ch..44h] = 0 (00475C2B..00475C3B)
    if (node != nullptr) {                                                 // 00475C11 / 00475C43
        Texture* const bump = material_of(part)->textures[4].target;       // 00475C49..00475C4C
        Vector color;                                                      // [S+20h]
        texture_head(bump)->common.s88(bump, nullptr, &color);             // 00475C4F..00475C56 GetSpecularColor (out)
        specular = color;                                                  // 00475C5C..00475C70 raw DWORD moves
        copy3(&bump_scale, at(node, 0x8C));                                // 00475C74..00475C8F
        const float magnitude = static_cast<float>(vector_magnitude_004ab100(&bump_scale)); // 00475C93..00475C98 fstp [S+10h]
        double specular_magnitude = 0.0;                                   // 00475CC3 fld [0073D700h]
        if (gate_0078e5f4 != 0 &&                                          // 00475C9C..00475CA6
            material_bump_map_is_specular_0047af50(material_of(part), nullptr) != 0) // 00475CA8..00475CB2
            specular_magnitude = vector_magnitude_004ab100(&specular);     // 00475CB4..00475CB9 (ST0, wide)
        Vector weighted;                                                   // [S+20h]
        weighted.z = mul_store(bump_scale.z, magnitude);                   // 00475CC9..00475CCE -> 00475CF2
        weighted.y = mul_store(bump_scale.y, magnitude);                   // 00475CD6..00475CDE -> 00475CEE
        weighted.x = mul_store(bump_scale.x, magnitude);                   // 00475CE2..00475CE6 -> 00475CEA
        const float divisor = static_cast<float>(specular_magnitude + static_cast<double>(magnitude)); // 00475CF6..00475CFA
        Vector quotient;                                                   // [S+6Ch]
        const Vector* const q = vector_divide_00437a90(&quotient, &weighted, divisor); // 00475CFD..00475CFF
        copy3(&bump_scale, q);                                             // 00475D04..00475D17
    }
    construct_static_program(vertex_program_guard_008301bc, vertex_program_008301b8,   // 00475D1B..00475D6F
        kotor_trimesh_data::nv_program_0079a970, program_parameters_0083055c);
    vertex_program_008301b8->Enable_004a24d0(part, 0);                     // 00475D7A..00475D83
    gl::glEnable_0073d390(*mc().vertex_program_0078d46c);                  // 00475D88..00475D95
    env_parameter(0x11, kOneBits, kMinusOneBits, 0, 0);                    // 00475D97..00475DA7
    gl::glEnable_0073d390(*sc().gl_fog_0078d450);                          // 00475DAC..00475DB5
    gl::glFogi_0073d34c(0x0B65, static_cast<std::int32_t>(kGlLinear));     // 00475DB7..00475DC3 (GL_FOG_MODE, [0073F25C])
    gl::glFogf_0073d350(*sc().gl_fog_start_0078d484, 0.0f);                // 00475DC9..00475DD8
    gl::glFogf_0073d350(*sc().gl_fog_end_0078d488, 1.0f);                  // 00475DDA..00475DE5
    // 00475DE7..00475E0B: glFogfv(GL_FOG_COLOR, {0,0,0}) reads 4 floats; the 4th ([S+60h]) has no store before this
    // call in the original (indeterminate stack). D43/D2-style labelled deviation: 0.0f (fog alpha unused for RGB).
    const float fog_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    gl::glFogfv_0073d354(*sc().gl_fog_color_0078d490, fog_color);
    PointerList lights;                                                    // [S+20h]
    int_list_copy_construct_0046d250(&lights, nullptr,                     // 00475E21..00475E26
        light_manager_get_bump_map_lights_004a28a0(field<void*>(*sc().active_scene_007b92a0, 0x80), nullptr)); // 00475E11..00475E1C
    (*mc().constant0_dispatch_007a6820)(static_cast<const float*>(at(*sc().active_scene_007b92a0, 0x88)), 1.0f); // 00475E2B..00475E46
    gl::glPushClientAttrib_0073d344(2);                                    // 00475E4F..00475E51
    gl::glEnableClientState_0073d260(kAttrib5);                            // 00475E57..00475E64
    gl::glEnableClientState_0073d260(kAttrib12);                           // 00475E66..00475E6D
    gl::glEnableClientState_0073d260(kAttrib13);                           // 00475E6F..00475E75
    gl::glEnableClientState_0073d260(kAttrib14);                           // 00475E77..00475E7E
    gl::glEnableClientState_0073d260(kAttrib15);                           // 00475E80..00475E87
    gl::glDisableClientState_0073d264(kNormalArray);                       // 00475E89..00475E95
    gl::glDisableClientState_0073d264(kColorArray);                        // 00475E97..00475E9E
    enable_dispatch(0x0F, basic(texture_slot(material_of(part), 4)),       // 00475EA0..00475EBA (0x0F, tex[4], scene+A4, 0, 0)
        basic(field<void*>(*sc().active_scene_007b92a0, 0xA4)), nullptr, nullptr);
    for (I i = 0; i < lights.size; ++i) {                                  // 00475EC0..00475ECB / 00475FF4..00475FFE
        void* const light = light_at(lights, i);
        Vector light_position;                                             // [S+2Ch..34h] raw (00475ED1..00475EEE)
        copy3(&light_position, at(light, 0x94));
        const Vector* const part_position = static_cast<const Vector*>(at(part, 0x08));   // [part+8..10h]
        Vector delta;                                                      // [S+48h]
        delta.x = sub_store(light_position.x, part_position->x);           // 00475F09..00475F1C
        delta.y = sub_store(light_position.y, part_position->y);           // 00475EFC..00475F28
        delta.z = sub_store(light_position.z, part_position->z);           // 00475EF2..00475F31
        const void* const color_source = at(light, 0x4C);                  // 00475F06 / 00475F18 [S+38h]
        const U radius = field<U>(light, 0x58);                            // 00475F00 / 00475F2D [S+10h] raw
        Quaternion inverse;                                                // [S+84h]
        Vector local;                                                      // [S+78h]
        const Vector* const rotated = quaternion_rotate_004a9bb0(&local,   // 00475F35..00475F46
            quaternion_negate_w_004a9b50(&inverse, static_cast<const Quaternion*>(at(part, 0x14))), &delta);
        copy3(&light_position, rotated);                                   // 00475F4B..00475F64 -> [S+2Ch]
        primitive_table(primitive)->s02c(primitive, nullptr, word(&light_position)); // 00475F5B..00475F6B
        primitive_table(primitive)->s034(primitive, nullptr, word(&light_position), radius); // 00475F6E..00475F7C
        const float s = scale_cell(mc().scale_0078e648);
        Vector scaled;                                                     // [S+60h]
        scaled.z = mul_store(bump_scale.z, s);                             // 00475F7F..00475F83 -> 00475FB4
        scaled.y = mul_store(bump_scale.y, s);                             // 00475F8D..00475F95 -> 00475FB0
        scaled.x = mul_store(bump_scale.x, s);                             // 00475F9C..00475FA1 -> 00475FAC
        Vector color;                                                      // [S+6Ch]
        vector_multiply_004ab1e0(&color, static_cast<const Vector*>(color_source), &scaled); // 00475F89..00475FB8
        (*mc().constant1_dispatch_007a47dc)(&color.x, 1.0f);               // 00475FBD..00475FC7
        gl::glBlendFunc_0073d38c(kGlOne, i != 0 ? kGlOne : kGlZero);       // 00475FD0..00475FE8 (neg/sbb index into 0073F27C/0073F278)
        render_flat_0046f900(primitive);                                   // 00475FEE..00475FEF
    }
    gl::glDisableClientState_0073d264(kAttrib5);                           // 00476004..00476010
    gl::glDisableClientState_0073d264(kAttrib12);                          // 00476012..00476019
    gl::glDisableClientState_0073d264(kAttrib13);                          // 0047601B..00476022
    gl::glDisableClientState_0073d264(kAttrib14);                          // 00476024..0047602A
    gl::glDisableClientState_0073d264(kAttrib15);                          // 0047602C..00476033
    gl::glEnableClientState_0073d260(kNormalArray);                        // 00476035..0047603C
    gl::glEnableClientState_0073d260(kColorArray);                         // 0047603E..00476044
    gl::glPopClientAttrib_0073d348();                                      // 00476046
    void* const scene = *sc().active_scene_007b92a0;                       // 0047604C
    scene_table(scene)->s124_scene_do_fog(scene, nullptr);                 // 00476052..00476054 Scene::DoFog
    gl::glBlendFunc_0073d38c(kSrcAlpha, kOneMinusSrcAlpha);               // 0047605A..00476067
    enable_dispatch(0x00, nullptr, nullptr, nullptr, nullptr);             // 0047606D..00476077
    gl::glDisable_0073d22c(*mc().vertex_program_0078d46c);                 // 0047607D..00476087
    if (lights.data != 0) live_free(as_object(lights.data));               // 0047608D..004760A5 (inline list dtor)
}

// ============================================================== 004760D0 RenderSpecularBumpMappedPass (cdecl)
void __cdecl render_specular_bump_mapped_pass_004760d0(void* primitive) {
    if (*mc().gate_0078e614 == 0) return;                                  // 004760DE..004760F2 je 0047666D
    if (*sc().active_scene_007b92a0 == nullptr) {                         // 004760F8..004760FF
        render_flat_0046f900(primitive);                                   // 00476101..0047610E
        return;                                                            // 00476111..00476125
    }
    void* const host = field<void*>(primitive, 8);                         // 00476127..0047612E
    void* const part = as_object(part_table(host)->s02c(host, nullptr));   // 00476131..00476139 (esi = result)
    void* node = nullptr;                                                  // 0047613B xor edi,edi
    if (part != nullptr) node = field<void*>(part, 4);                     // 0047613D..00476141
    Vector specular{};                                                     // [S+14h..1Ch] = 0 (00476144..00476154)
    Vector bump_scale{};                                                   // [S+38h..40h] = 0 (0047615C..0047616C)
    {
        Texture* const bump = material_of(part)->textures[4].target;       // 00476174..00476177 (unconditional: no part test)
        Vector color;                                                      // [S+20h]
        texture_head(bump)->common.s88(bump, nullptr, &color);             // 0047617A..00476181 GetSpecularColor (out)
        specular = color;                                                  // 00476189..0047619D raw DWORD moves
    }
    if (node != nullptr) {                                                 // 00476187 / 004761A1
        copy3(&bump_scale, at(node, 0x8C));                                // 004761A7..004761C2
        const float magnitude = static_cast<float>(vector_magnitude_004ab100(&bump_scale)); // 004761C6..004761CB fstp [S+10h]
        const double specular_magnitude = vector_magnitude_004ab100(&specular);           // 004761CF..004761D4 (ST0, wide; no gate)
        Vector weighted;                                                   // [S+20h]
        weighted.z = static_cast<float>(static_cast<double>(bump_scale.z) * specular_magnitude); // 004761D9..004761DD -> 00476201
        weighted.y = static_cast<float>(static_cast<double>(bump_scale.y) * specular_magnitude); // 004761E2..004761EA -> 004761FD
        weighted.x = static_cast<float>(static_cast<double>(bump_scale.x) * specular_magnitude); // 004761F3..004761F7 -> 004761F9
        const float divisor = static_cast<float>(specular_magnitude + static_cast<double>(magnitude)); // 00476205..00476209
        Vector quotient;                                                   // [S+8Ch]
        const Vector* const q = vector_divide_00437a90(&quotient, &weighted, divisor); // 0047620C..0047620E
        copy3(&specular, q);                                               // 00476213..00476226 -> [S+14h..1Ch]
    }
    construct_static_program(vertex_program_guard_008301c4, vertex_program_008301c0,   // 0047622A..0047627E
        kotor_trimesh_data::nv_program_0079a970, program_parameters_0083055c);
    vertex_program_008301c0->Enable_004a24d0(part, 0);                     // 00476289..00476292
    gl::glEnable_0073d390(*mc().vertex_program_0078d46c);                  // 00476297..004762A4
    env_parameter(0x11, kOneBits, kMinusOneBits, 0, 0);                    // 004762A6..004762B6
    gl::glBlendFunc_0073d38c(kSrcAlpha, kGlOne);                           // 004762BB..004762CB ([0073F270], [0073F278])
    gl::glEnable_0073d390(*sc().gl_fog_0078d450);                          // 004762D1..004762D8
    gl::glFogi_0073d34c(0x0B65, static_cast<std::int32_t>(kGlLinear));     // 004762DA..004762E5
    gl::glFogf_0073d350(*sc().gl_fog_start_0078d484, 0.0f);                // 004762EB..004762FA
    gl::glFogf_0073d350(*sc().gl_fog_end_0078d488, 1.0f);                  // 004762FC..00476308
    // 0047630A..0047632E: glFogfv(GL_FOG_COLOR, {0,0,0}); 4th float [S+68h] has no prior store (indeterminate).
    // D43/D2-style labelled deviation: 0.0f.
    const float fog_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    gl::glFogfv_0073d354(*sc().gl_fog_color_0078d490, fog_color);
    PointerList lights;                                                    // [S+80h]
    int_list_copy_construct_0046d250(&lights, nullptr,                     // 00476345..0047634D
        light_manager_get_bump_map_lights_004a28a0(field<void*>(*sc().active_scene_007b92a0, 0x80), nullptr)); // 00476334..00476340
    gl::glPushClientAttrib_0073d344(2);                                    // 00476352..0047635F
    gl::glEnableClientState_0073d260(kAttrib5);                            // 00476365..00476371
    gl::glEnableClientState_0073d260(kAttrib12);                           // 00476373..0047637A
    gl::glEnableClientState_0073d260(kAttrib13);                           // 0047637C..00476383
    gl::glEnableClientState_0073d260(kAttrib14);                           // 00476385..0047638B
    gl::glEnableClientState_0073d260(kAttrib15);                           // 0047638D..00476394
    gl::glDisableClientState_0073d264(kNormalArray);                       // 00476396..004763A3
    gl::glDisableClientState_0073d264(kColorArray);                        // 004763A5..004763AB
    {
        Material* const material = material_of(part);                      // 004763B3
        enable_dispatch(0x12, basic(texture_slot(material, 0)),            // 004763AD..004763C8 (0x12, tex[0], tex[4], 0, scene+A4)
            basic(texture_slot(material, 4)), nullptr,
            basic(field<void*>(*sc().active_scene_007b92a0, 0xA4)));
    }
    const U data = lights.data;                                            // 004763D5 ebp = list data (held for the loop and the free)
    for (I i = 0; i < lights.size; ++i) {                                  // 004763CE..004763E3 / 004765AE..004765BB
        void* const light = field<void*>(as_object(data), static_cast<U>(i) * 4u);   // 004763F0 [ebp+edi*4]
        Vector light_position;                                             // [S+2Ch..34h] raw (004763F0..0047640F)
        copy3(&light_position, at(light, 0x94));
        void* const camera = *sc().current_camera_007bb4e8;                // 00476403
        Vector eye;                                                        // [S+44h..4Ch] raw (0047641A..0047643B)
        copy3(&eye, at(camera, 0x7C));
        const Vector* const part_position = static_cast<const Vector*>(at(part, 0x08));
        Vector delta;                                                      // [S+20h]
        delta.x = sub_store(light_position.x, part_position->x);           // 0047642D..00476443
        delta.y = sub_store(light_position.y, part_position->y);           // 0047641F..0047644E
        delta.z = sub_store(light_position.z, part_position->z);           // 00476413..00476456
        const U radius = field<U>(light, 0x58);                            // 00476447 / 00476463 [S+54h] raw
        const Quaternion* const part_orientation = static_cast<const Quaternion*>(at(part, 0x14));
        Quaternion inverse;                                                // [S+BCh]
        Vector light_local;                                                // [S+B0h]
        const Vector* const rotated = quaternion_rotate_004a9bb0(&light_local,   // 00476467..00476478
            quaternion_negate_w_004a9b50(&inverse, part_orientation), &delta);
        Vector eye_delta;                                                  // [S+74h]
        eye_delta.z = sub_store(eye.z, part_position->z);                  // 0047647D..00476481 -> 004764C1
        eye_delta.y = sub_store(eye.y, part_position->y);                  // 00476486..0047648E -> 004764B5
        eye_delta.x = sub_store(eye.x, part_position->x);                  // 00476494..0047649C -> 004764A9
        copy3(&light_position, rotated);                                   // 00476484..004764AD -> [S+2Ch]
        Quaternion inverse_eye;                                            // [S+CCh]
        Vector eye_local;                                                  // [S+A4h]
        const Vector* const eye_rotated = quaternion_rotate_004a9bb0(&eye_local,   // 004764C9..004764DA
            quaternion_negate_w_004a9b50(&inverse_eye, part_orientation), &eye_delta);
        copy3(&eye, eye_rotated);                                          // 004764DF..004764F4 -> [S+44h]
        primitive_table(primitive)->s030(primitive, nullptr, word(&light_position), word(&eye)); // 004764EF..00476504
        primitive_table(primitive)->s034(primitive, nullptr, word(&light_position), radius);     // 00476507..00476515
        const float s = scale_cell(mc().scale_0078e630);
        const float sz = mul_store(specular.z, s);                         // 00476518..0047652A [S+58h]
        const float sy = mul_store(specular.y, s);                         // 0047652E..00476538 [S+50h]
        const float sx = mul_store(specular.x, s);                         // 0047653C..00476546 [S+98h]
        Texture* const bump = material_of(part)->textures[4].target;       // 00476522..00476525
        const float intensity = texture_head(bump)->common.s7c(bump, nullptr); // 00476528 / 0047654D..00476550 GetBumpIntensity
        Vector tint;                                                       // [S+68h]
        tint.z = mul_store(sz, intensity);                                 // 00476558..0047655D -> 00476583
        tint.y = mul_store(sy, intensity);                                 // 00476568..0047656C -> 0047657F
        tint.x = mul_store(sx, intensity);                                 // 00476570..00476577 -> 0047657B
        Vector color;                                                      // [S+8Ch]
        vector_multiply_004ab1e0(&color, static_cast<const Vector*>(at(light, 0x4C)), &tint); // 00476587..00476590
        (*mc().constant1_dispatch_007a47dc)(&color.x, 1.0f);               // 00476595..004765A2
        render_flat_0046f900(primitive);                                   // 004765A8..004765A9
    }
    gl::glDisableClientState_0073d264(kAttrib5);                           // 004765C1..004765CE
    gl::glDisableClientState_0073d264(kAttrib12);                          // 004765D0..004765D7
    gl::glDisableClientState_0073d264(kAttrib13);                          // 004765D9..004765DF
    gl::glDisableClientState_0073d264(kAttrib14);                          // 004765E1..004765E8
    gl::glDisableClientState_0073d264(kAttrib15);                          // 004765EA..004765F1
    gl::glEnableClientState_0073d260(kNormalArray);                        // 004765F3..004765FF
    gl::glEnableClientState_0073d260(kColorArray);                         // 00476601..00476608
    gl::glPopClientAttrib_0073d348();                                      // 0047660A
    void* const scene = *sc().active_scene_007b92a0;                       // 00476610
    scene_table(scene)->s124_scene_do_fog(scene, nullptr);                 // 00476616..00476618 Scene::DoFog
    gl::glBlendFunc_0073d38c(kSrcAlpha, kOneMinusSrcAlpha);               // 0047661E..0047662B
    enable_dispatch(0x00, nullptr, nullptr, nullptr, nullptr);             // 00476631..0047663B
    gl::glDisable_0073d22c(*mc().vertex_program_0078d46c);                 // 00476641..0047664B
    if (data != 0) live_free(as_object(data));                             // 00476651..00476661 (ebp)
}
} // extern "C"
