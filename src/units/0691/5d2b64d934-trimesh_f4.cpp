// trimesh_f4.cpp - lane-3d-mesh fork F4: RenderBumpMapped 00476690, RenderNormalMapped 00477050,
// RenderSkinned 0048A0D0 (swkotor.exe GOG K1). Evidence: lane-3d-mesh/d/<ADDR>.s.txt (dumpbin of the pinned exe);
// every statement cites the instruction(s) it reproduces. Stack-slot offsets are given relative to the
// frame after the callee-saved pushes (E = esp after the last register push; see F4.json notes).
//
// D30-FINAL: x87 intermediates are written as double, float only at the original `fstp dword` stores.
// EH: the original SEH states only guard the function-static `new VertexProgram` (unwind funclets
// 007170EF / 00717127 clear the guard bit and free the block if the ctor throws) and the local IntList
// (free of its data). operator new 006FA7E6 returns NULL (live_operator_new) and every callee is an
// extern "C" body (nothrow under /EHsc): no throw is reachable, so the funclets are cited, not emitted (D30-FINAL).
#include "df9dab63cf-trimesh_skin.h"
#include <cstdlib>   // atexit (D35: CRT runtime 006FB332 -> modern CRT atexit)

using kotor_scene3d::cells;
using kotor_trimesh::mesh_cells;
using kotor_trimesh::Texture;
using kotor_trimesh::Material;
using kotor_trimesh::as_object;
using kotor_scene3d::field;
using kotor_scene3d::at;
using kotor_scene3d::bits_to_float;
using VertexProgram = kotor_vertex_program::VertexProgram;

// LOWERS hoisted to trimesh_skin.h (lane merge)

// LightManager vtable 00743958 has no published typed table (lane-3d-construct: owner unassigned).
// VTABLE REQUEST: slots used here, targets read from .rdata: +2C = 004A5680, +30 = 004A5710
// (both __thiscall, no stack arguments, plain `ret`; EAX not consumed by these callers).

// ---------------------------------------------------------------- function-static storage (only these bodies)
extern "C" {
// RenderBumpMapped 00476690: guard 008301D4 bits 1/2/4 -> VertexProgram* 008301D0 / 008301CC / 008301C8
volatile U vp_guard_008301d4 = 0;
VertexProgram* volatile vp_bump_mapped_008301d0 = nullptr;
VertexProgram* volatile vp_bump_flat_008301cc = nullptr;
VertexProgram* volatile vp_bump_fog_008301c8 = nullptr;
// RenderNormalMapped 00477050: guard 008301DC bit 1 -> VertexProgram* 008301D8
volatile U vp_guard_008301dc = 0;
VertexProgram* volatile vp_normal_mapped_008301d8 = nullptr;
// Parameter-word cells passed to VertexProgram::VertexProgram (BSS 0; no writer instruction anywhere).
std::uint32_t* volatile vp_parameters_00830550 = nullptr;   // 00476835
std::uint32_t* volatile vp_parameters_00830574 = nullptr;   // 00476F3D
// RenderSkinned 0048A0D0: guard 008303F4 bits 1/2/4/8/10h
volatile U skin_static_guard_008303f4 = 0;
PointerList skin_bone_orientations_008303e8;   // {data E8, size EC, capacity F0} Quaternion[] (bit 1, atexit 0073BD50)
PointerList skin_bone_matrices_008303dc;       // {data DC, size E0, capacity E4} 3x3 float[9] (bit 2, atexit 0073BD30)
PointerList skin_bone_positions_008303d0;      // {data D0, size D4, capacity D8} Vector[] (bit 4, atexit 0073BD10)
volatile U skin_static_008303c4[3];            // bit 8: three zero stores, no other use (type unknown)
volatile U skin_static_008303b8[3];            // bit 10h: three zero stores, no other use (type unknown)
// Compiler-generated atexit destructor thunks of the three static lists (original 0073BD10/30/50):
// mov eax,[data]; test eax,eax; je; push eax; call 006FA390 (_free); pop ecx; ret.
void __cdecl skin_bone_positions_atexit_0073bd10() {
    const U data = skin_bone_positions_008303d0.data;                    // 0073BD10
    if (data != 0) live_free(as_object(data));                         // 0073BD15..0073BD1A
}
void __cdecl skin_bone_matrices_atexit_0073bd30() {
    const U data = skin_bone_matrices_008303dc.data;                     // 0073BD30
    if (data != 0) live_free(as_object(data));                         // 0073BD35..0073BD3A
}
void __cdecl skin_bone_orientations_atexit_0073bd50() {
    const U data = skin_bone_orientations_008303e8.data;                 // 0073BD50
    if (data != 0) live_free(as_object(data));                         // 0073BD55..0073BD5A
}
}

namespace {

// `push 201Ch; call 006FA7E6; test eax,eax; je -> 0; push params; push text; mov ecx,eax; call 004A27D0`
VertexProgram* new_vertex_program(char* text, std::uint32_t* parameters) {
    void* const memory = live_operator_new(0x201C);
    if (memory == nullptr) return nullptr;
    return static_cast<VertexProgram*>(memory)->Construct_004a27d0(text, parameters);
}
// `mov eax,[0078E5EC]; test; je; push 8620h | push [0078D46C]` (0047678B / 00476855 / 00477184 / 00477242)
U program_target() {
    return *cells.programs_0078e5ec != 0 ? 0x8620u : *mesh_cells.vertex_program_0078d46c;
}
void* active_scene() { return *cells.active_scene_007b92a0; }
const kotor_trimesh::LightManagerTablePrefix* light_slots(void* lights) {
    return static_cast<const kotor_trimesh::LightManagerTablePrefix*>(field<void*>(lights, 0));
}
// Scene+80 light manager, vtable +2C / +30 (00476A10..00476A1D, 00476D03..00476D11)
void lights_slot_2c() { void* const l = field<void*>(active_scene(), 0x80); light_slots(l)->s2c(l, nullptr); }
void lights_slot_30() { void* const l = field<void*>(active_scene(), 0x80); light_slots(l)->s30(l, nullptr); }
texture::Basic* basic(Texture* t) { return reinterpret_cast<texture::Basic*>(t); }  // object view (E1 Basic prefix = Ref)
texture::Basic* scene_texture_a4() { return field<texture::Basic*>(active_scene(), 0xA4); }
std::uintptr_t word_of(const void* p) { return reinterpret_cast<std::uintptr_t>(p); }
// VertexPrimitive+8 -> part; part vtable +2C (PartTriMesh: 00641DB0 self_return) -> part as trimesh.
void* primitive_part_as_trimesh(void* primitive) {
    void* const part = field<void*>(primitive, 8);
    return as_object(kotor_trimesh::part_table(part)->s02c(part, nullptr));
}
void get_world(void* part, Vector* position, Quaternion* orientation) {     // part vtable +0C (GetWorld, RET 8)
    kotor_trimesh::part_table(part)->s00c(part, nullptr, static_cast<U>(word_of(position)), static_cast<U>(word_of(orientation)));
}
void env(U index, float x, float y, float z, float w) { gl_program_env_parameter_0044f820(index, x, y, z, w); }
float raw(void* object, U offset) { return bits_to_float(field<U>(object, offset)); }   // raw DWORD push

// Shared light-loop pieces of 00476690 / 00477050 (identical instruction sequences, offsets cited per function).
struct EyeFrame { Vector position; Quaternion orientation; Vector eye_local; };

// Eye in part space: GetWorld(&P,&Q) (004769A7 / 00477352); Vector d = camera+7C - P (004769C7 / 00477372);
// q' = operator-(Q) (004769DD / 00477388); eye = q' * d (004769EE / 00477399).
void compute_eye(void* part, EyeFrame& f) {
    f.orientation = Quaternion{1.0f, 0.0f, 0.0f, 0.0f};   // 00476987 / 00477332 w=1.0, x/y/z=0
    f.position = Vector{0.0f, 0.0f, 0.0f};                // 0047698F..0047699F / 0047733A..0047734A
    get_world(part, &f.position, &f.orientation);
    Vector difference;
    void* const camera = *cells.current_camera_007bb4e8;
    kotor_cdecl_00423bc0(&difference.x, static_cast<const float*>(at(camera, 0x7C)), &f.position.x);
    Quaternion conjugate;
    Quaternion* const c = quaternion_negate_w_004a9b50(&conjugate, &f.orientation);
    quaternion_rotate_004a9bb0(&f.eye_local, c, &difference);
}
// Light position in part space: copy light+94 (raw), D = L - P (x87, fstp each), rotate by operator-(Q).
Vector light_local(void* light, const EyeFrame& f) {
    Vector l; std::memcpy(&l, at(light, 0x94), sizeof l);                       // 00476A35..00476A56
    Vector d;
    d.z = static_cast<float>(static_cast<double>(l.z) - f.position.z);          // fld [..28]; fsub P.z
    d.y = static_cast<float>(static_cast<double>(l.y) - f.position.y);
    d.x = static_cast<float>(static_cast<double>(l.x) - f.position.x);          // fstp x/y/z 00476A77..00476A86
    Quaternion conjugate;
    Quaternion* const c = quaternion_negate_w_004a9b50(&conjugate, &f.orientation);
    Vector r;
    std::memcpy(&r, quaternion_rotate_004a9bb0(&r, c, &d), sizeof r);
    return r;
}
// 0xE parameter: fld [light+58]; fld st0; fmul; fadd st0,st0; fdivr 9.0f (00742F00); fstp dword.
float attenuation(void* light) {
    const double r = static_cast<double>(field<float>(light, 0x58));
    const double rr = r * r;
    return static_cast<float>(9.0 / (rr + rr));
}
// constant dispatch: gate ? computed : default literal (00476B3E..00476B7F / 00476C2B..00476C6C)
void constant1(const Vector* computed, bool gate) {
    Vector fallback{1.0f, 0.0f, 0.0f};
    (*mesh_cells.constant1_dispatch_007a47dc)(gate ? &computed->x : &fallback.x, 1.0f);
}
void constant0(const Vector* computed, bool gate) {
    Vector fallback{0.0f, 1.0f, 0.0f};
    (*mesh_cells.constant0_dispatch_007a6820)(gate ? &computed->x : &fallback.x, 1.0f);
}
void* light_at(const PointerList& list, I index) { return as_object(field<U>(as_object(list.data), static_cast<U>(index) * 4)); }
void free_list(PointerList& list) { const U data = list.data; if (data != 0) live_free(as_object(data)); }
} // namespace

// ================================================================ 00476690 RenderBumpMapped (cdecl, plain ret)
extern "C" void __cdecl render_bump_mapped_00476690(void* primitive) {
    if (active_scene() == nullptr) {                                             // 0047669E..004766B2
        render_flat_0046f900(primitive);                                         // 004766B4..004766BC
        return;                                                                  // 004766C1 -> 0047702B (no list yet)
    }
    kotor_renderer_init::glBlendFunc_0073d38c(0x302, 0x303);                     // 004766C6..004766D4 ([0073F270],[0073F274])
    if (gate_0078e670 == 0) return;                                              // 004766DA
    if (*mesh_cells.gate_0078e614 == 0) return;                                  // 004766E7
    PointerList lights;                                                          // B+34 (E+44)
    int_list_copy_construct_0046d250(&lights, nullptr,
        light_manager_get_bump_map_lights_004a28a0(field<void*>(active_scene(), 0x80), nullptr)); // 004766F4..00476709
    if (lights.size == 0) {                                                      // 0047670E..0047671F
        render_flat_0046f900(primitive);                                         // 00476721..00476729
        free_list(lights);                                                       // 00477012..00477026
        return;
    }
    const U alpha_enabled = kotor_renderer_init::glIsEnabled_0073d37c(*cells.gl_alpha_test_0078d44c); // 00476740..0047674D movzx
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);                          // 00476750..0047675A
    if (aur_pixel_shaders_available_0046eda0() != 0 && gate_0078e62c != 0) {    // 0047675C..00476770
        (*mesh_cells.enable_dispatch_007a47e0)(0x0D, nullptr, nullptr, nullptr, nullptr);   // 00476776..00476780
        kotor_renderer_init::glDisable_0073d22c(program_target());               // 00476786..004767A0
        void* const part = primitive_part_as_trimesh(primitive);                 // 004767A2..004767B1 (ebp)
        void* const node = part != nullptr ? field<void*>(part, 4) : nullptr;    // 004767B3..004767B9 (esi)
        Vector specular{0.0f, 0.0f, 0.0f};                                       // E+38..E+40 (004767C3..004767D3)
        Vector diffuse{0.0f, 0.0f, 0.0f};                                        // E+2C..E+34 (004767DB..004767EB)
        if ((vp_guard_008301d4 & 1) == 0) {                                      // 004767BC
            vp_guard_008301d4 = vp_guard_008301d4 | 1;                           // 004767F5
            char* const text = *cells.programs_0078e5ec != 0 ? kotor_trimesh_data::arb_skin_program_0078e8c8
                                                             : kotor_trimesh_data::nv_program_00799dc8; // 00476821..00476830
            vp_bump_mapped_008301d0 = new_vertex_program(text, vp_parameters_00830550);  // 004767FC..00476848
        }
        kotor_renderer_init::glEnable_0073d390(program_target());                // 00476855..0047686B
        if (node != nullptr) {                                                   // 00476871
            Vector scratch;
            Vector* const s = material_get_bump_map_specular_color_00471430(field<void*>(part, 0x44), nullptr, &scratch); // 00476875..00476880
            std::memcpy(&specular, s, sizeof specular);                          // 00476885..004768A0 (raw)
            std::memcpy(&diffuse, at(node, 0x8C), sizeof diffuse);              // 00476891..004768AF (raw)
        }
        if (*cells.programs_0078e5ec != 0) {                                     // 004768B3
            (*mesh_cells.enable_vertex_attrib_007bb79c)(0x0C);                   // 004768BC
            (*mesh_cells.enable_vertex_attrib_007bb79c)(0x0D);                   // 004768C4
            (*mesh_cells.enable_vertex_attrib_007bb79c)(0x0E);                   // 004768CC
        } else {
            kotor_renderer_init::glEnableClientState_0073d260(0x865C);           // 004768D6 [0073F3B8]
            kotor_renderer_init::glEnableClientState_0073d260(0x865D);           // 004768E4 [0073F3BC]
            kotor_renderer_init::glEnableClientState_0073d260(0x865E);           // 004768ED [0073F3C0]
        }
        {   // 004768F6..00476910: dispatch(6, mat.tex[4], scene+A4, scene+A4, mat.tex[0])
            Material* const material = kotor_trimesh::material_of(part);
            Texture* const t0 = material->textures[0].target;                    // 004768F9
            texture::Basic* const a4 = scene_texture_a4();                       // 004768FB..00476901
            Texture* const t4 = material->textures[4].target;                    // 00476907
            (*mesh_cells.enable_dispatch_007a47e0)(6, basic(t4), a4, a4, basic(t0));
        }
        {   // 00476916..00476949
            void* const scene = active_scene();
            const float flag = field<U>(scene, 0x60) != 0 ? 1.0f : 0.0f;
            env(0x10, raw(scene, 0x64), raw(scene, 0x68), flag, 0.5f);
        }
        env(0x11, 1.0f, -1.0f, 0.0f, 0.0f);                                      // 0047694E..0047695E
        EyeFrame frame;
        compute_eye(part, frame);                                                // 00476966..004769EE
        if (lights.size > 0) {                                                   // 004769F3..004769FE
          I i = 0;                                                               // 004769FA xor edi,edi
          do {
            lights_slot_2c();                                                    // 00476A10..00476A1D
            vp_bump_mapped_008301d0->Enable_004a24d0(part, 1);                   // 00476A20..00476A29
            void* light = light_at(lights, i);                                   // 00476A2E..00476A32
            const Vector local = light_local(light, frame);                      // 00476A35..00476A9E
            const float dz = static_cast<float>(static_cast<double>(diffuse.z) * *mesh_cells.scale_0078e648);   // 00476AA3..00476AB8 -> E+1C
            const float dy = static_cast<float>(static_cast<double>(diffuse.y) * *mesh_cells.scale_0078e648); // 00476ABC..00476AD2 -> E+14
            const float dx = static_cast<float>(static_cast<double>(diffuse.x) * *mesh_cells.scale_0078e648); // 00476AD9..00476AEA -> E+CC
            light = light_at(lights, i);                                         // 00476AC9 (esi reload)
            Material* material = kotor_trimesh::material_of(part);
            Texture* t4 = material->textures[4].target;                          // 00476AB0..00476AB5
            const float diffuse_intensity = kotor_trimesh::texture_head(t4)->common.s84(t4, nullptr); // 00476AF1..00476AF7
            Vector weighted;                                                     // E+B4
            weighted.z = static_cast<float>(static_cast<double>(dz) * diffuse_intensity);
            weighted.y = static_cast<float>(static_cast<double>(dy) * diffuse_intensity);
            weighted.x = static_cast<float>(static_cast<double>(dx) * diffuse_intensity);   // 00476B02..00476B32
            Vector product;                                                      // E+5C
            vector_multiply_004ab1e0(&product, static_cast<const Vector*>(at(light, 0x4C)), &weighted); // 00476B39
            constant1(&product, *mesh_cells.gate_00798b48 != 0);                 // 00476B3E..00476B7F
            const float sz = static_cast<float>(static_cast<double>(specular.z) * *mesh_cells.scale_0078e630); // 00476B85..00476B97 -> E+14
            material = kotor_trimesh::material_of(part);
            t4 = material->textures[4].target;                                   // 00476B8F..00476B95
            const float sy = static_cast<float>(static_cast<double>(specular.y) * *mesh_cells.scale_0078e630); // 00476B9E..00476BA8 -> E+1C
            const float sx = static_cast<float>(static_cast<double>(specular.x) * *mesh_cells.scale_0078e630); // 00476BAC..00476BB6 -> E+C0
            const float specular_intensity = kotor_trimesh::texture_head(t4)->common.s80(t4, nullptr); // 00476BBD..00476BC7
            Vector specular_weighted;                                            // E+9C
            specular_weighted.z = static_cast<float>(static_cast<double>(sz) * specular_intensity);
            specular_weighted.y = static_cast<float>(static_cast<double>(sy) * specular_intensity);
            specular_weighted.x = static_cast<float>(static_cast<double>(sx) * specular_intensity); // 00476BCB..00476C03
            Vector specular_product;                                             // E+D8 -> copied to E+5C
            light = light_at(lights, i);                                         // 00476C0A
            Vector* const sp = vector_multiply_004ab1e0(&specular_product, static_cast<const Vector*>(at(light, 0x4C)), &specular_weighted); // 00476C12
            std::memcpy(&product, sp, sizeof product);                           // 00476C17..00476C27
            constant0(&product, *mesh_cells.gate_00798b4c != 0);                 // 00476C2B..00476C6C
            env(0x0C, local.x, local.y, local.z, 1.0f);                          // 00476C72..00476C87
            env(0x0D, frame.eye_local.x, frame.eye_local.y, frame.eye_local.z, 1.0f); // 00476C8C..00476CAB
            light = light_at(lights, i);                                         // 00476CB0
            env(0x0E, 1.0f, 0.0f, attenuation(light), 1.0f);                     // 00476CB3..00476CD9
            render_flat_0046f900(primitive);                                     // 00476CDE..00476CE6
            if (alpha_enabled != 0)                                              // 00476CEB..00476CF4
                kotor_renderer_init::glEnable_0073d390(*cells.gl_alpha_test_0078d44c); // 00476CF6..00476CFD
            lights_slot_30();                                                    // 00476D03..00476D11
            ++i;                                                                 // 00476D18
          } while (i < lights.size);                                             // 00476D14..00476D1B (count re-read)
        }
        if (*cells.programs_0078e5ec != 0) {                                     // 00476D27
            (*mesh_cells.disable_vertex_attrib_007bb770)(0x0C);                  // 00476D30
            (*mesh_cells.disable_vertex_attrib_007bb770)(0x0D);                  // 00476D38
            (*mesh_cells.disable_vertex_attrib_007bb770)(0x0E);                  // 00476D40
        } else {
            kotor_renderer_init::glDisableClientState_0073d264(0x865C);          // 00476D4A
            kotor_renderer_init::glDisableClientState_0073d264(0x865D);          // 00476D59
            kotor_renderer_init::glDisableClientState_0073d264(0x865E);          // 00476D62
        }
        void* const scene = active_scene();                                      // 00476D6A..00476D72 Scene vtbl+124 (DoFog)
        static_cast<const kotor_scene3d::SceneVTable*>(field<void*>(scene, 0))->s124_scene_do_fog(scene, nullptr);
        (*mesh_cells.enable_dispatch_007a47e0)(0x0D, nullptr, nullptr, nullptr, nullptr); // 00476D78..00476D82
        kotor_renderer_init::glDisable_0073d22c(program_target());               // 00476D88..00476DA6
        free_list(lights);                                                       // 0047700E..00477026
        return;
    }
    // ---- no pixel shaders (00476DAD)
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_fog_0078d450);             // 00476DAD..00476DB4
    if (gate_0078e5f8 != 0) render_diffuse_bump_mapped_pass_00475bb0(primitive); // 00476DB6..00476DC7
    if ((vp_guard_008301d4 & 2) == 0) {                                          // 00476DCF
        vp_guard_008301d4 = vp_guard_008301d4 | 2;                               // 00476DD8
        vp_bump_flat_008301cc = new_vertex_program(kotor_trimesh_data::nv_program_0079ab28,
            static_cast<std::uint32_t*>(*mesh_cells.bss_00830560));              // 00476DDF..00476E1B
    }
    if (gate_0078e600 != 0) {                                                    // 00476E28
        lights_slot_2c();                                                        // 00476E37..00476E44
        vp_bump_flat_008301cc->Enable_004a24d0(field<void*>(primitive, 8), 1);   // 00476E47..00476E53
        kotor_renderer_init::glEnable_0073d390(*mesh_cells.vertex_program_0078d46c); // 00476E58..00476E5F
        gl_blend_func_indexed_00401670(2, 2);                                    // 00476E61..00476E65
        render_flat_non_textured_00470750(primitive);                            // 00476E6A..00476E6B
        kotor_renderer_init::glDisable_0073d22c(*mesh_cells.vertex_program_0078d46c); // 00476E70..00476E7A
        lights_slot_30();                                                        // 00476E7C..00476E89
    }
    if (gate_0078e604 != 0) {                                                    // 00476E8C
        vp_bump_flat_008301cc->Enable_004a24d0(field<void*>(primitive, 8), 1);   // 00476E95..00476EA1
        kotor_renderer_init::glEnable_0073d390(*mesh_cells.vertex_program_0078d46c); // 00476EA6..00476EAD
        gl_blend_func_indexed_00401670(5, 3);                                    // 00476EAF..00476EB3
        render_flat_decal_004707a0(primitive);                                   // 00476EB8..00476EB9
        kotor_renderer_init::glDisable_0073d22c(*mesh_cells.vertex_program_0078d46c); // 00476EBE..00476EC8
    }
    if (gate_0078e5f4 != 0) render_specular_bump_mapped_pass_004760d0(primitive); // 00476ECA..00476ED4
    {
        void* const scene = active_scene();                                      // 00476EDC..00476EE4 Scene vtbl+124
        static_cast<const kotor_scene3d::SceneVTable*>(field<void*>(scene, 0))->s124_scene_do_fog(scene, nullptr);
    }
    kotor_scene3d::I fog_enabled;                                                // E+18 (slot reused by the compiler)
    kotor_renderer_init::glGetIntegerv_0073d244(0x0B60, &fog_enabled);           // 00476EEA..00476EF6 ([0073F268] = GL_FOG)
    if (fog_enabled != 0) {                                                      // 00476EFC..00476F02
        if ((vp_guard_008301d4 & 4) == 0) {                                      // 00476F08
            vp_guard_008301d4 = vp_guard_008301d4 | 4;                           // 00476F11
            vp_bump_fog_008301c8 = new_vertex_program(kotor_trimesh_data::nv_program_0079b358, vp_parameters_00830574); // 00476F18..00476F54
        }
        vp_bump_fog_008301c8->Enable_004a24d0(field<void*>(primitive, 8), 0);    // 00476F61..00476F6D
        kotor_renderer_init::glEnable_0073d390(*mesh_cells.vertex_program_0078d46c); // 00476F72..00476F78
        void* scene = active_scene();                                            // 00476F7A
        const float density = static_cast<float>(field<I>(scene, 0x6C));         // fild [eax+6Ch]; fstp dword
        env(0x14, raw(scene, 0x64), raw(scene, 0x68), density, 1.0f);            // 00476F82..00476F95
        scene = active_scene();                                                  // 00476F9A
        env(0x15, raw(scene, 0x74), raw(scene, 0x78), raw(scene, 0x7C), 1.0f);   // 00476F9F..00476FB2
        env(0x16, 1.0f, 0.0f, 0.0f, 0.0f);                                       // 00476FB7..00476FC4
        gl_blend_func_indexed_00401670(0, 1);                                    // 00476FC9..00476FCD
        render_flat_non_textured_00470750(primitive);                            // 00476FD5..00476FD6
        kotor_renderer_init::glDisable_0073d22c(*mesh_cells.vertex_program_0078d46c); // 00476FDB..00476FE5
    }
    if (alpha_enabled != 0)                                                      // 00476FE7 (ebp = movzx result)
        kotor_renderer_init::glEnable_0073d390(*cells.gl_alpha_test_0078d44c);   // 00476FEB..00476FF2
    {
        Material* const material = kotor_trimesh::material_of(field<void*>(primitive, 8)); // 00476FF4..00476FF7
        Texture* const t0 = material->textures[0].target;                        // 00476FFA
        (*mesh_cells.enable_dispatch_007a47e0)(0, basic(t0), nullptr, nullptr, nullptr); // 00476FFC..00477005
    }
    free_list(lights);                                                           // 00477012..00477026
}

// ================================================================ 00477050 RenderNormalMapped (cdecl, plain ret)
extern "C" void __cdecl render_normal_mapped_00477050(void* primitive) {
    if (active_scene() == nullptr) {                                             // 0047705E..00477075
        render_flat_0046f900(primitive);                                         // 00477077..0047707F
        return;                                                                  // 00477087..0047709C
    }
    kotor_renderer_init::glBlendFunc_0073d38c(0x302, 0x303);                     // 0047709D..004770AB
    if (gate_0078e670 == 0) return;                                              // 004770B1
    if (*mesh_cells.gate_0078e614 == 0) return;                                  // 004770BD
    PointerList lights;                                                          // B+48
    int_list_copy_construct_0046d250(&lights, nullptr,
        light_manager_get_bump_map_lights_004a28a0(field<void*>(active_scene(), 0x80), nullptr)); // 004770C9..004770DE
    if (lights.size == 0) {                                                      // 004770E3..004770EE
        render_flat_0046f900(primitive);                                         // 004770F0..004770F8
        free_list(lights);                                                       // 004770FD..00477118
        return;
    }
    const U alpha_enabled = kotor_renderer_init::glIsEnabled_0073d37c(*cells.gl_alpha_test_0078d44c); // 0047713E..00477151
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);                          // 00477145..00477159
    const U list_data = lights.data;                                             // 00477162 mov ebx,[esp+54h] (cached)
    if (aur_pixel_shaders_available_0046eda0() != 0 && gate_0078e62c != 0) {    // 0047715B..00477172
        (*mesh_cells.enable_dispatch_007a47e0)(0x0D, nullptr, nullptr, nullptr, nullptr);   // 00477178..0047717E
        kotor_renderer_init::glDisable_0073d22c(program_target());               // 00477184..0047719E
        void* const part = primitive_part_as_trimesh(primitive);                 // 004771A0..004771B0 (ebp)
        void* const node = part != nullptr ? field<void*>(part, 4) : nullptr;    // 004771B2..004771B6 (edi)
        Vector specular{0.0f, 0.0f, 0.0f};                                       // E+34..E+3C (004771C0..004771D0)
        Vector diffuse{0.0f, 0.0f, 0.0f};                                        // E+28..E+30 (004771D8..004771E8)
        if ((vp_guard_008301dc & 1) == 0) {                                      // 004771B9
            vp_guard_008301dc = vp_guard_008301dc | 1;                           // 004771F2
            vp_normal_mapped_008301d8 = new_vertex_program(kotor_trimesh_data::arb_program_0078eff8,
                static_cast<std::uint32_t*>(*mesh_cells.bss_00830294));          // 004771F9..00477235
        }
        kotor_renderer_init::glEnable_0073d390(program_target());                // 00477242..00477259
        if (node != nullptr) {                                                   // 0047725F
            Vector scratch;
            Vector* const s = material_get_bump_map_specular_color_00471430(field<void*>(part, 0x44), nullptr, &scratch); // 00477263..0047726E
            std::memcpy(&specular, s, sizeof specular);                          // 00477273..0047728E (raw)
            std::memcpy(&diffuse, at(node, 0x8C), sizeof diffuse);              // 0047727B..0047729D (raw)
        }
        {   // 004772A1..004772BB: dispatch(7, mat.tex[0], mat.tex[4], scene+A4, scene+A4)
            texture::Basic* const a4 = scene_texture_a4();                       // 004772A1..004772A7
            Material* const material = kotor_trimesh::material_of(part);
            Texture* const t4 = material->textures[4].target;                    // 004772B0
            Texture* const t0 = material->textures[0].target;                    // 004772B3
            (*mesh_cells.enable_dispatch_007a47e0)(7, basic(t0), basic(t4), a4, a4);
        }
        {   // 004772C1..004772F4
            void* const scene = active_scene();
            const float flag = field<U>(scene, 0x60) != 0 ? 1.0f : 0.0f;
            env(0x10, raw(scene, 0x64), raw(scene, 0x68), flag, 0.5f);
        }
        env(0x11, 1.0f, -1.0f, 0.0f, 0.0f);                                      // 004772F9..00477309
        EyeFrame frame;
        compute_eye(part, frame);                                                // 00477311..00477399
        if (lights.size > 0) {                                                   // 0047739E..004773A9
            I i = 0;
            do {
                lights_slot_2c();                                                // 004773B0..004773BD
                vp_normal_mapped_008301d8->Enable_004a24d0(part, 1);             // 004773C0..004773C9
                void* light = as_object(field<U>(as_object(list_data), static_cast<U>(i) * 4)); // 004773CE
                const Vector local = light_local(light, frame);                  // 004773D1..0047743A
                // QUIRK (original): the diffuse x component is read from the SPECULAR colour slot E+34
                // (0047747C fld [esp+34h] after add esp,0Ch), y/z from the node+8C slots E+2C/E+30.
                const float dz = static_cast<float>(static_cast<double>(diffuse.z) * *mesh_cells.scale_0078e648);  // 0047743F..00477451 -> E+18
                const float dy = static_cast<float>(static_cast<double>(diffuse.y) * *mesh_cells.scale_0078e648);  // 00477459..00477470 -> E+14
                Material* material = kotor_trimesh::material_of(part);
                Texture* t4 = material->textures[4].target;                      // 00477466..0047746D
                const float dx = static_cast<float>(static_cast<double>(specular.x) * *mesh_cells.scale_0078e648); // 00477478..0047748A -> E+CC
                light = as_object(field<U>(as_object(list_data), static_cast<U>(i) * 4)); // 0047744E (esi)
                const float diffuse_intensity = kotor_trimesh::texture_head(t4)->common.s84(t4, nullptr); // 00477491..00477497
                Vector weighted;                                                 // E+B4
                weighted.z = static_cast<float>(static_cast<double>(dz) * diffuse_intensity);
                weighted.y = static_cast<float>(static_cast<double>(dy) * diffuse_intensity);
                weighted.x = static_cast<float>(static_cast<double>(dx) * diffuse_intensity); // 004774A2..004774D2
                Vector product;                                                  // E+4C
                vector_multiply_004ab1e0(&product, static_cast<const Vector*>(at(light, 0x4C)), &weighted); // 004774D9
                constant1(&product, *mesh_cells.gate_00798b48 != 0);             // 004774DE..0047751F
                const float sz = static_cast<float>(static_cast<double>(specular.z) * *mesh_cells.scale_0078e630); // 00477525..00477537 -> E+14
                material = kotor_trimesh::material_of(part);
                t4 = material->textures[4].target;                               // 0047752F..00477535
                const float sy = static_cast<float>(static_cast<double>(specular.y) * *mesh_cells.scale_0078e630); // 0047753E..00477548 -> E+18
                const float sx = static_cast<float>(static_cast<double>(specular.x) * *mesh_cells.scale_0078e630); // 0047754C..00477556 -> E+C0
                const float specular_intensity = kotor_trimesh::texture_head(t4)->common.s80(t4, nullptr); // 0047755D..00477563
                Vector specular_weighted;                                        // E+9C
                specular_weighted.z = static_cast<float>(static_cast<double>(sz) * specular_intensity);
                specular_weighted.y = static_cast<float>(static_cast<double>(sy) * specular_intensity);
                specular_weighted.x = static_cast<float>(static_cast<double>(sx) * specular_intensity); // 0047756E..0047759F
                light = as_object(field<U>(as_object(list_data), static_cast<U>(i) * 4)); // 004775A6
                Vector specular_product;                                         // E+D8 -> copied to E+4C
                Vector* const sp = vector_multiply_004ab1e0(&specular_product, static_cast<const Vector*>(at(light, 0x4C)), &specular_weighted); // 004775AE
                std::memcpy(&product, sp, sizeof product);                       // 004775B3..004775C3
                constant0(&product, *mesh_cells.gate_00798b4c != 0);             // 004775C7..00477608
                env(0x0C, local.x, local.y, local.z, 1.0f);                      // 0047760E..00477627
                env(0x0D, frame.eye_local.x, frame.eye_local.y, frame.eye_local.z, 1.0f); // 0047762C..0047764B
                light = as_object(field<U>(as_object(list_data), static_cast<U>(i) * 4)); // 00477650
                env(0x0E, 1.0f, 0.0f, attenuation(light), 1.0f);                 // 00477653..00477679
                gl_render_activate_texture_stage_00425250(1);                    // 0047767E..00477680
                kotor_renderer_init::glEnableClientState_0073d260(0x8078);       // 00477685..0047768F ([0073F398])
                gl_render_activate_texture_stage_00425250(0);                    // 00477695..00477697
                render_flat_0046f900(primitive);                                 // 0047769C..004776A4
                gl_render_activate_texture_stage_00425250(1);                    // 004776A9..004776AB
                kotor_renderer_init::glDisableClientState_0073d264(0x8078);      // 004776B0..004776B9
                gl_render_activate_texture_stage_00425250(0);                    // 004776BF..004776C1
                if (alpha_enabled != 0)                                          // 004776C6..004776CF
                    kotor_renderer_init::glEnable_0073d390(*cells.gl_alpha_test_0078d44c); // 004776D1..004776D8
                lights_slot_30();                                                // 004776DE..004776EC
                ++i;                                                             // 004776F3
            } while (i < lights.size);                                           // 004776EF..004776F6
        }
        void* const scene = active_scene();                                      // 00477702..0047770A Scene vtbl+124
        static_cast<const kotor_scene3d::SceneVTable*>(field<void*>(scene, 0))->s124_scene_do_fog(scene, nullptr);
        (*mesh_cells.enable_dispatch_007a47e0)(0x0D, nullptr, nullptr, nullptr, nullptr); // 00477710..0047771A
        kotor_renderer_init::glDisable_0073d22c(program_target());               // 00477720..0047773A
        if (bss_00828000 != 0) {                                                 // 0047773C..00477743 debug axes
            kotor_renderer_init::glDisable_0073d22c(0x0B50);                     // 00477749
            kotor_renderer_init::glDisable_0073d22c(0x0DE1);                     // 00477750
            kotor_renderer_init::glDisable_0073d22c(0x0B71);                     // 00477757
            kotor_renderer_init::glLineWidth_0073d380(5.0f);                     // 0047775E..00477763
            kotor_renderer_init::glBegin_0073d2d0(1);                            // 00477769..0047776B (GL_LINES)
            kotor_renderer_init::glColor3f_0073d2ec(1.0f, 0.0f, 0.0f);           // 00477777..00477780
            kotor_renderer_init::glVertex3f_0073d2c8(0.0f, 0.0f, 0.0f);          // 00477788..0047778E
            kotor_renderer_init::glVertex3f_0073d2c8(5.0f, 0.0f, 0.0f);          // 00477790..00477799
            kotor_renderer_init::glColor3f_0073d2ec(0.0f, 1.0f, 0.0f);           // 0047779B..004777A4
            kotor_renderer_init::glVertex3f_0073d2c8(0.0f, 0.0f, 0.0f);          // 004777A6..004777AC
            kotor_renderer_init::glVertex3f_0073d2c8(0.0f, 5.0f, 0.0f);          // 004777AE..004777B7
            kotor_renderer_init::glColor3f_0073d2ec(0.0f, 0.0f, 1.0f);           // 004777B9..004777C2
            kotor_renderer_init::glVertex3f_0073d2c8(0.0f, 0.0f, 0.0f);          // 004777C4..004777CA
            kotor_renderer_init::glVertex3f_0073d2c8(0.0f, 0.0f, 5.0f);          // 004777CC..004777D5
            kotor_renderer_init::glEnd_0073d2c4();                               // 004777D7
            kotor_renderer_init::glEnable_0073d390(0x0B71);                      // 004777E3..004777E8
            kotor_renderer_init::glEnable_0073d390(0x0B50);                      // 004777EA..004777EF
            kotor_renderer_init::glEnable_0073d390(0x0DE1);                      // 004777F1..004777F6
        }
    }
    // 004777F8..00477808: free the cached list data (alpha test is NOT re-enabled on the no-shader path).
    if (list_data != 0) live_free(as_object(list_data));
}

// ================================================================ 0048A0D0 RenderSkinned (cdecl, plain ret)
// Callers: 0048D72D push 0 / 0048D6AF push 1 after `push [[part+70]]+8 result`: (VertexPrimitiveSkinned*, U flag).
extern "C" void __cdecl render_skinned_0048a0d0(void* primitive, U flag) {
    void* part = field<void*>(primitive, 8);                                     // 0048A0EB..0048A0FB (esi)
    void* const node = field<void*>(part, 4);                                    // 0048A0FF (edi, saved E+28)
    if ((skin_static_guard_008303f4 & 1) == 0) {                                 // 0048A0F2..0048A110 (byte test)
        skin_static_guard_008303f4 = skin_static_guard_008303f4 | 1;             // 0048A112
        skin_bone_orientations_008303e8.size = 0;                                // 0048A11F
        skin_bone_orientations_008303e8.capacity = 0;                            // 0048A125
        skin_bone_orientations_008303e8.data = 0;                                // 0048A12B
        std::atexit(&skin_bone_orientations_atexit_0073bd50);                   // 0048A11A..0048A131
    }
    if ((skin_static_guard_008303f4 & 2) == 0) {                                 // 0048A144..0048A151
        skin_static_guard_008303f4 = skin_static_guard_008303f4 | 2;             // 0048A153
        skin_bone_matrices_008303dc.size = 0;                                    // 0048A15E
        skin_bone_matrices_008303dc.capacity = 0;                                // 0048A164
        skin_bone_matrices_008303dc.data = 0;                                    // 0048A16A
        std::atexit(&skin_bone_matrices_atexit_0073bd30);                       // 0048A159..0048A170
    }
    if ((skin_static_guard_008303f4 & 4) == 0) {                                 // 0048A17F..0048A18C
        skin_static_guard_008303f4 = skin_static_guard_008303f4 | 4;             // 0048A18E
        skin_bone_positions_008303d0.size = 0;                                   // 0048A199
        skin_bone_positions_008303d0.capacity = 0;                               // 0048A19F
        skin_bone_positions_008303d0.data = 0;                                   // 0048A1A5
        std::atexit(&skin_bone_positions_atexit_0073bd10);                      // 0048A194..0048A1AB
    }
    skin_bone_orientations_008303e8.size = 0;                                    // 0048A1BA
    skin_bone_matrices_008303dc.size = 0;                                        // 0048A1C0
    skin_bone_positions_008303d0.size = 0;                                       // 0048A1C6
    Vector negated;                                                              // E+10 (fld; fchs; fstp dword)
    negated.x = -field<float>(part, 0x08);                                       // 0048A1CC..0048A1D5
    negated.y = -field<float>(part, 0x0C);                                       // 0048A1DA..0048A1E6
    negated.z = -field<float>(part, 0x10);                                       // 0048A1EA..0048A1F1
    const Quaternion* orientation = static_cast<const Quaternion*>(at(part, 0x14)); // 0048A1DD lea ebx,[esi+14h]
    Quaternion conjugate;                                                        // E+4C
    Vector translation;                                                          // E+1C
    quaternion_rotate_004a9bb0(&translation, quaternion_negate_w_004a9b50(&conjugate, orientation), &negated); // 0048A1F5..0048A203
    Quaternion rotation;                                                         // E+30
    quaternion_negate_w_004a9b50(&rotation, orientation);                        // 0048A208..0048A20E
    for (void* parent = field<void*>(part, 0x3C); parent != nullptr; parent = field<void*>(part, 0x3C)) { // 0048A213..0048A2EE
        part = parent;                                                           // 0048A221 mov esi,eax
        Vector d;                                                                // E+40
        d.x = -field<float>(part, 0x08);                                         // 0048A223..0048A22D
        d.y = -field<float>(part, 0x0C);                                         // 0048A234..0048A241
        d.z = -field<float>(part, 0x10);                                         // 0048A245..0048A24B
        const Quaternion* const parent_orientation = static_cast<const Quaternion*>(at(part, 0x14)); // 0048A231
        Quaternion c1;                                                           // E+88
        Vector r1;                                                               // E+6C
        quaternion_rotate_004a9bb0(&r1, quaternion_negate_w_004a9b50(&c1, parent_orientation), &d); // 0048A24F..0048A25D
        Vector r2;                                                               // E+4C
        Vector* const r = quaternion_rotate_004a9bb0(&r2, &rotation, &r1);       // 0048A262..0048A26D
        translation.x = static_cast<float>(static_cast<double>(translation.x) + r->x); // 0048A272..0048A279
        translation.y = static_cast<float>(static_cast<double>(translation.y) + r->y); // 0048A281..0048A288
        translation.z = static_cast<float>(static_cast<double>(translation.z) + r->z); // 0048A28C..0048A2A3
        Quaternion c2;                                                           // E+78
        Quaternion product;                                                      // E+5C
        Quaternion* const q = quaternion_multiply_004a9ac0(&product, &rotation,
                                  quaternion_negate_w_004a9b50(&c2, parent_orientation)); // 0048A2A2..0048A2C6
        std::memcpy(&rotation, q, sizeof rotation);                              // 0048A2CB..0048A2E2 (raw DWORDs)
    }
    gather_bones_00488930(part, &skin_bone_positions_008303d0, &skin_bone_orientations_008303e8,
                          translation, rotation);                                // 0048A2F4..0048A338 (by-value copies)
    if (skin_bone_positions_008303d0.size > 0) {                                 // 0048A33D..0048A34B
        I index = 0;
        U position_offset = 0, orientation_offset = 0;                           // ebp (x0C), ebx (x10)
        do {
            const U base_vectors = field<U>(node, 0x1C4);                        // 0048A359
            Vector* const source = static_cast<Vector*>(as_object(skin_bone_positions_008303d0.data + position_offset)); // 0048A353..0048A368
            Vector rotated;                                                      // E+4C
            Vector* const rv = quaternion_rotate_004a9bb0(&rotated,
                static_cast<const Quaternion*>(as_object(skin_bone_orientations_008303e8.data + orientation_offset)),
                static_cast<const Vector*>(as_object(base_vectors + position_offset))); // 0048A35F..0048A374
            const float x = static_cast<float>(static_cast<double>(rv->x) + source->x); // 0048A379..0048A386
            const float y = static_cast<float>(static_cast<double>(rv->y) + source->y); // 0048A38A..0048A390
            const float z = static_cast<float>(static_cast<double>(rv->z) + source->z); // 0048A394..0048A3A7 (loaded before the stores)
            Vector* const destination = static_cast<Vector*>(as_object(skin_bone_positions_008303d0.data + position_offset)); // 0048A37D..0048A383
            destination->x = x;                                                  // 0048A3A2
            destination->y = y;                                                  // 0048A3A4
            destination->z = z;                                                  // 0048A3AF
            const U base_orientations = field<U>(node, 0x1B8);                   // 0048A3B2
            Quaternion* const target = static_cast<Quaternion*>(as_object(skin_bone_orientations_008303e8.data + orientation_offset)); // 0048A3B8..0048A3C1
            Quaternion composed;                                                 // E+5C
            Quaternion* const cq = quaternion_multiply_004a9ac0(&composed, target,
                static_cast<const Quaternion*>(as_object(base_orientations + orientation_offset))); // 0048A3C0..0048A3CA
            std::memcpy(target, cq, sizeof *target);                             // 0048A3CF..0048A3E2 (raw)
            float matrix_out[9];                                                 // E+BC
            const float* const m = quaternion_get_matrix_0044f470(
                static_cast<const Quaternion*>(as_object(skin_bone_orientations_008303e8.data + orientation_offset)),
                nullptr, matrix_out);                                            // 0048A3E5..0048A3F8
            U matrix[9];                                                         // E+98 (rep movs 9 DWORDs)
            std::memcpy(matrix, m, sizeof matrix);                               // 0048A3FD..0048A410
            I count = skin_bone_matrices_008303dc.size;                          // 0048A3FF
            const I capacity = skin_bone_matrices_008303dc.capacity;             // 0048A412
            if (count == capacity) {                                             // 0048A418
                matrix_list_allocate_00488160(&skin_bone_matrices_008303dc, nullptr,
                                              capacity != 0 ? capacity + capacity : 8); // 0048A41C..0048A42E
                count = skin_bone_matrices_008303dc.size;                        // 0048A433
            }
            std::memcpy(as_object(skin_bone_matrices_008303dc.data + static_cast<U>(count) * 36), matrix, sizeof matrix); // 0048A438..0048A44F
            skin_bone_matrices_008303dc.size = skin_bone_matrices_008303dc.size + 1; // 0048A451..0048A461
            ++index;                                                             // 0048A466..0048A46A
            position_offset += 0x0C;                                             // 0048A46B
            orientation_offset += 0x10;                                          // 0048A46E
        } while (index < skin_bone_positions_008303d0.size);                     // 0048A456 / 0048A471..0048A477
    }
    U guard = skin_static_guard_008303f4;                                        // 0048A47D
    if ((guard & 8) == 0) {                                                      // 0048A482..0048A484
        guard |= 8; skin_static_guard_008303f4 = guard;                          // 0048A486..0048A489
        skin_static_008303c4[0] = 0; skin_static_008303c4[1] = 0; skin_static_008303c4[2] = 0; // 0048A48E..0048A4A2
    }
    if ((guard & 0x10) == 0) {                                                   // 0048A4AC..0048A4AE
        guard |= 0x10; skin_static_guard_008303f4 = guard;                       // 0048A4B0..0048A4B3
        skin_static_008303b8[0] = 0; skin_static_008303b8[1] = 0; skin_static_008303b8[2] = 0; // 0048A4B8..0048A4CC
    }
    execute_skin_transforms_00487570(node, &skin_bone_positions_008303d0, &skin_bone_matrices_008303dc,
                                     primitive, flag);                           // 0048A4D6..0048A4F1
}
