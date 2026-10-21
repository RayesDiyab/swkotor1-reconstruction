// f2_part.cpp - lane-m46-3dmodel fork F2: Part vtable bodies PartTriMesh::Project 00474220 (+5C),
// PartSkin::Project 00477B90 (+5C) and PartDanglyMesh::Animate 00484290 (+68), as referenced by lane-3d-model
// model_vtables.cpp (U __fastcall(void* self, void* unused_edx, U...)). swkotor.exe GOG K1; listings
// lane-m46-3dmodel/d/<ADDR>.txt. Stack offsets in comments are the original's (after its prologue pushes).
//
// D30-FINAL: x87 intermediates are `double`; `float` only where the original stores with `fstp dword`
// (several temporaries below are deliberately rounded to float exactly where the original spills them).
// Function-static storage (guard words + objects) is reproduced as named cells with the original guard tests.
// EH: 00477B90 has an SEH frame whose funclets (00717140 / 0071714E) only clear the guard bits of its two static
// lists if their construction throws; nothing here can throw (extern "C" callees, live allocator), so they are
// cited, not emitted (D30-FINAL).
// EAX returns: the slot type is U; each body returns the original's EAX where it is defined (cited), else 0
// with a note (the original leaves a callee's EAX).
#include "df9dab63cf-trimesh_skin.h"
#include "5d11c14380-model_load.hpp"   // lane-3d-model node/tree casts, pool lock/unlock (exact owner declarations)
#include <cstdlib>          // atexit (D35: CRT runtime 006FB332 -> modern CRT atexit, lane-3d-mesh precedent D53)

using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::U8; using I16 = std::int16_t;
using kotor_scene3d::Vector; using kotor_scene3d::Quaternion; using kotor_scene3d::PointerList;
using kotor_scene3d::field; using kotor_scene3d::at; using kotor_scene3d::bits_to_float; using kotor_scene3d::float_to_bits;
using kotor_scene3d::cells;
using kotor_trimesh::mesh_cells; using kotor_trimesh::part_table; using kotor_trimesh::primitive_table; using kotor_trimesh::as_object;

namespace kotor_renderer_init {   // exact owner declarations (copies; see f2_gl.cpp)
int __cdecl VertexProgramsAvailable_0045f770();
extern void __stdcall glFrontFace_0073d274(U32);
extern void __stdcall glStencilOp_0073d270(U32, U32, U32);
}

extern "C" {
// model_vtables.cpp spellings (exact)
U __fastcall part_tri_mesh_project_00474220(void* self, void* unused_edx, U, U, U, U);
U __fastcall part_skin_project_00477b90(void* self, void* unused_edx, U, U, U, U);
U __fastcall part_dangly_mesh_animate_00484290(void* self, void* unused_edx, U);
// lane-3d-model model_tangent.cpp (M45 provider): Matrix::Matrix() identity 00437AF0
void* __fastcall matrix_identity_construct_00437af0(void* self, void* unused_edx);
// f2_gl.cpp (this lane)
void __cdecl gl_render_set_interleaved_buffer_00425520(U stride_unused, U flags, U pool, I count_unused, void* node, U offset);
// anim3d.h / emitter.h (exact; M45 provider 3d-model-model-skin @vector_add_assign_004a9740@12)
Vector* __fastcall vector_add_assign_004a9740(Vector* self, void* unused_edx, const Vector* other);
// M40 world-timer-x87-conversion.obj (anim3d.h exact copy): original _ftol2 006FAE8C, argument ST0, result EDX:EAX
unsigned __int64 __cdecl unresolved_ftol2_006fae8c();
// NEW names (no provider; ABI read from the call sites: cdecl, caller cleanup; parameter types inferred)
I __cdecl clip_projection_caps_0046f580(Vector* direction_scaled, Vector* point, U frustum);          // 00474467 add esp,0Ch; test eax
void __cdecl process_faces_004741a0(const Vector* light, void* face_records, void* node);              // 004744B3 add esp,0Ch
U __cdecl gl_render_build_shadow_array_004269f0(const Vector* light, float* length, U frustum,
    Vector* direction_scaled, Vector* point, I vertex_count, U pool);                                 // 0047463D (7 DWORDs)
void __cdecl gl_render_render_shadow_array_00426660(U array, I edges, U caps);                        // 00474649 (3 DWORDs)
}

// ---------------------------------------------------------------- storage owned here (first definer)
namespace {
struct FaceRecord {          // 16 bytes (004745D8 add ecx,10h): +0 float facing, +4/+6/+8 adjacent face (-1 = none),
    float facing;            // +A/+C/+E vertex indices (0047450D..00474511 movsx [ecx+4]/[ecx+6]/[ecx+8] with ecx = rec+6)
    I16 adjacent[3];
    I16 vertex[3];
};
static_assert(sizeof(FaceRecord) == 16, "face record");
}
extern "C" {
// BSS cells with no writer instruction in .text (raw-image search: only the reads below): zero for the process.
volatile float shadow_length_scale_0082804c = 0.0f;   // 004743A5 / 00477D04 fmul
volatile I shadow_mode_00828054 = 0;                  // 00474332 cmp 3 / 00477DDC
volatile U skin_shadow_enabled_00828058 = 0;          // 00477BAB
// BSS flag also read by ProjectDynamicShadow 0044FF50 / Gob::PartVolumeProject 00474710 (not authored; they borrow)
extern volatile U shadow_volume_programs_007b9374;   // [M46 compose] storage: lane-m46-3dcore project_shadow_gate_007b9374 (single storage)       // 00474344 / 0047447C
// ProcessFaces output: 2048 x 16-byte records (00828168 .. next referenced BSS word 00830168).
FaceRecord shadow_faces_00828168[0x800];
// Silhouette vertex marks: U16 per vertex, 007A4820 .. next referenced word 007A6820 (0x2000 bytes); also read by
// GLRender::BuildShadowArray 004269F0 / RenderShadowArray 00426660 (not authored; they borrow).
volatile std::uint16_t shadow_vertex_marks_007a4820[0x1000];
// Silhouette edge index pairs (U16), 007A69C0 (also RenderShadowArray 00426660). Extent: the original region has no
// further direct reference before 00828000; sized here to the bound implied by the face array
// (2048 faces x 3 edges x 2 indices). LABEL: extent inferred (unknown in the image).
volatile std::uint16_t shadow_edge_indices_007a69c0[0x800 * 3 * 2];

// PartTriMesh::Project statics, guard 008301A4 (bits 1/2/4/8/10h; only 00474220 references them)
volatile U tri_project_guard_008301a4 = 0;
Vector tri_project_unused_00830198 = {0.0f, 0.0f, 0.0f};    // bit 1 (zero-initialised only)
Vector tri_project_unused_0083018c = {0.0f, 0.0f, 0.0f};    // bit 2 (zero-initialised only)
Vector tri_project_direction_00830180 = {0.0f, 0.0f, 0.0f}; // bit 4
Vector tri_project_scaled_00830174 = {0.0f, 0.0f, 0.0f};    // bit 8
Vector tri_project_point_00830168 = {0.0f, 0.0f, 0.0f};     // bit 10h

// PartSkin::Project statics, guard 00830234 (bits 1..40h; 00477B90 and its atexit thunks only)
volatile U skin_project_guard_00830234 = 0;
Vector skin_project_unused_00830228 = {0.0f, 0.0f, 0.0f};   // bit 1
Vector skin_project_unused_0083021c = {0.0f, 0.0f, 0.0f};   // bit 2
Vector skin_project_direction_00830210 = {0.0f, 0.0f, 0.0f};// bit 4
Vector skin_project_scaled_00830204 = {0.0f, 0.0f, 0.0f};   // bit 8
Vector skin_project_point_008301f8 = {0.0f, 0.0f, 0.0f};    // bit 10h
PointerList skin_project_q_bones_008301ec;                   // bit 20h {data EC, size F0, capacity F4}, atexit 0073BC70
PointerList skin_project_t_bones_008301e0;                   // bit 40h {data E0, size E4, capacity E8}, atexit 0073BC50

// PartDanglyMesh::Animate statics, guard 00830374 (bits 1..10h; only 00484290)
volatile U dangly_guard_00830374 = 0;
Vector dangly_next_00830368 = {0.0f, 0.0f, 0.0f};    // bit 1
Vector dangly_rest_0083035c = {0.0f, 0.0f, 0.0f};    // bit 2
Vector dangly_target_00830350 = {0.0f, 0.0f, 0.0f};  // bit 4
Vector dangly_velocity_00830344 = {0.0f, 0.0f, 0.0f};// bit 8
Vector dangly_unused_00830338 = {0.0f, 0.0f, 0.0f};  // bit 10h (zero-initialised only)

// PartSkin::freeProjectTBones 0073BC50 / freeProjectQBones 0073BC70 (atexit thunks registered at 00477E59 / 00477E98):
// mov eax,[data]; test eax,eax; je; push eax; call 006FA390; pop ecx; ret.
void __cdecl part_skin_free_project_t_bones_0073bc50() {
    const U data = skin_project_t_bones_008301e0.data;
    if (data != 0) live_free(as_object(data));
}
void __cdecl part_skin_free_project_q_bones_0073bc70() {
    const U data = skin_project_q_bones_008301ec.data;
    if (data != 0) live_free(as_object(data));
}
}

namespace {
namespace gl = kotor_renderer_init;
constexpr U kCw_0073f2c0 = 0x900u, kCcw_0073f2c4 = 0x901u;          // .rdata 0073F2C0 / 0073F2C4
constexpr U kKeep_0073f2c8 = 0x1E00u, kIncr_0073f2d4 = 0x1E02u, kDecr_0073f2d8 = 0x1E03u; // .rdata 0073F2C8 / D4 / D8
constexpr U kAttribArray1_0073f3a8 = 0x8651u, kAttribArray4_0073f3b0 = 0x8654u;           // .rdata 0073F3A8 / 0073F3B0
constexpr double kZero_0073d700 = 0.0;
constexpr U kDtMax_00743220 = 0x3D0F5C29u;   // 0.035f
constexpr U kDtMin_00743224 = 0x3C4CCCCDu;   // 0.0125f
constexpr U kTight_00743228 = 0x3FC00000u;   // 1.5f
constexpr U kDisp_0074322c = 0x3F000000u;    // 0.5f
constexpr U kWind_00743230 = 0x41A00000u;    // 20.0f
constexpr U kHigh_0074325c = 0x501502F9u;    // 1.0e10f
constexpr U kLow_00743260 = 0xD01502F9u;     // -1.0e10f
constexpr U kJump_00743264 = 0x40A00000u;    // 5.0f
constexpr U kByte_0073ede0 = 0x3B808081u;    // 1/255
constexpr double kOne_0073d764 = 1.0;
double cst(U bits) { return static_cast<double>(bits_to_float(bits)); }
U word_of(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }

// anim3d.h ftol2 idiom: original _ftol2 006FAE8C on the exact x87 value.
I ftol2(double value) {
    I converted;
    __asm {
        fld qword ptr value
        call unresolved_ftol2_006fae8c
        mov converted, eax
    }
    return converted;
}
// fld/fstp of a float stored to a float cell: value copy (the original moves these words through ST0).
inline void fcopy(float& dst, float src) { dst = src; }

// Shared stencil-volume tail of 00474220 (004746A6..004746F8) / 00477B90 (004781E8..00478237):
// front-face CW + incr on zfail, draw; front-face CCW + decr on zfail, draw. `slot` = primitive vtable offset.
U stencil_volume_passes(void* self, bool slot8) {
    gl::glFrontFace_0073d274(kCw_0073f2c0);                                         // push [0073F2C0]; call [0073D274]
    gl::glStencilOp_0073d270(kKeep_0073f2c8, kIncr_0073f2d4, kKeep_0073f2c8);      // push eax; push edx; push eax; call [0073D270]
    void* prim = field<void*>(self, 0x70);
    if (slot8) primitive_table(prim)->s008(prim, nullptr); else primitive_table(prim)->s004(prim, nullptr);
    gl::glFrontFace_0073d274(kCcw_0073f2c4);
    gl::glStencilOp_0073d270(kKeep_0073f2c8, kDecr_0073f2d8, kKeep_0073f2c8);
    prim = field<void*>(self, 0x70);
    return slot8 ? primitive_table(prim)->s008(prim, nullptr) : primitive_table(prim)->s004(prim, nullptr);
}
// Emit one silhouette edge (00474515..00474533 pattern): two U16 indices and both vertex marks.
volatile std::uint16_t* emit_edge(volatile std::uint16_t* out, I16 a, I16 b) {
    out[0] = static_cast<std::uint16_t>(a);                                        // mov [esi],ax
    shadow_vertex_marks_007a4820[static_cast<I>(a)] = 1;                           // mov [eax*2+7A4820h],di (di = 1)
    out[1] = static_cast<std::uint16_t>(b);                                        // mov [esi],dx (esi += 2)
    shadow_vertex_marks_007a4820[static_cast<I>(b)] = 1;
    return out + 2;
}
// Edge test (004744E7..0047450B): emit when there is no neighbour or !(neighbour.facing * facing > 0).
bool silhouette(const FaceRecord& r, I16 adjacent) {
    if (adjacent == static_cast<I16>(-1)) return true;                             // cmp ax,0FFFFh; je
    const double product = static_cast<double>(shadow_faces_00828168[static_cast<I>(adjacent)].facing) * r.facing; // fld [eax+828168h]; fmul [ecx-6]
    return !(product > kZero_0073d700);                                            // fcomp 0.0; test ah,41h; je skip
}
} // namespace

extern "C" {
// ================================================================ 00474220 PartTriMesh::Project (vtbl +5C, RET 10h)
// (light, length, frustum, out_edges): builds the stencil shadow silhouette of the mesh for a point light.
U __fastcall part_tri_mesh_project_00474220(void* self, void*, U light_word, U length_word, U frustum, U out_word) {
    void* const node_self = field<void*>(self, 4);
    aurora_model::MdlNode* const saber = aurora_node_as_saber_0043e3a0(static_cast<aurora_model::MdlNode*>(node_self), nullptr); // 00474226..0047422D
    if (saber != nullptr) return word_of(saber);                                   // 00474232..00474234 jne 004746FE (EAX = saber)
    void* const node = field<void*>(field<void*>(self, 0x70), 4);                  // 0047423A..0047423F ebp = [[self+70]+4]
    U guard = tri_project_guard_008301a4;                                          // 00474242
    if (!(guard & 1u)) { guard |= 1u; tri_project_guard_008301a4 = guard; tri_project_unused_00830198 = Vector{0.0f, 0.0f, 0.0f}; }   // 00474247..00474268
    if (!(guard & 2u)) { guard |= 2u; tri_project_guard_008301a4 = guard; tri_project_unused_0083018c = Vector{0.0f, 0.0f, 0.0f}; }   // 00474272..00474292
    if (!(guard & 4u)) { guard |= 4u; tri_project_guard_008301a4 = guard; tri_project_direction_00830180 = Vector{0.0f, 0.0f, 0.0f}; } // 0047429C..004742BC
    if (!(guard & 8u)) { guard |= 8u; tri_project_guard_008301a4 = guard; tri_project_scaled_00830174 = Vector{0.0f, 0.0f, 0.0f}; }   // 004742C6..004742E6
    if (!(guard & 0x10u)) { guard |= 0x10u; tri_project_guard_008301a4 = guard; tri_project_point_00830168 = Vector{0.0f, 0.0f, 0.0f}; } // 004742F0..00474310
    if (field<U8>(node, 0x187) == 0 && field<U8>(node, 0x188) == 0) return 0;     // 0047431A..0047432C (original EAX: AL = 0, upper bytes = guard word; returned 0)
    const bool vp_volume = shadow_mode_00828054 == 3 && *mesh_cells.skin_programs_0078e8c0 != 0 &&
                           shadow_volume_programs_007b9374 != 0;                   // 00474332..0047434B
    if (!vp_volume) {
        const U caps = field<U8>(node, 0x188);                                     // 00474351 movzx esi ([esp+14h])
        U* const out = static_cast<U*>(as_object(out_word));
        if (out != nullptr) *out = 0;                                              // 00474358..00474364
        const Vector* const light = static_cast<const Vector*>(as_object(light_word)); // 0047436A ebx
        Vector negated;                                                            // [esp+18h]
        negated.x = static_cast<float>(-static_cast<double>(light->x));            // 0047436E..00474377
        negated.y = static_cast<float>(-static_cast<double>(light->y));            // 0047437F..00474385
        negated.z = static_cast<float>(-static_cast<double>(light->z));            // 00474389..0047438E
        Vector unit;                                                               // [esp+24h]
        tri_project_direction_00830180 = *normalize_004ab180(&unit, &negated);      // 00474392..004743C5 (DWORD copies)
        const double scale = shadow_length_scale_0082804c;
        tri_project_scaled_00830174.x = static_cast<float>(tri_project_direction_00830180.x * scale); // 0047439F..004743D0
        tri_project_scaled_00830174.y = static_cast<float>(tri_project_direction_00830180.y * scale); // 004743BF..004743EF
        tri_project_scaled_00830174.z = static_cast<float>(tri_project_direction_00830180.z * scale); // 004743DF..00474407
        const double length = bits_to_float(length_word);                          // [esp+38h]
        const double px = tri_project_direction_00830180.x * length;               // 004743F9..00474403
        const double py = tri_project_direction_00830180.y * length;               // 0047440C..00474412
        const float pz = static_cast<float>(tri_project_direction_00830180.z * length); // 00474416..00474420 fstp [esp+2Ch]
        tri_project_point_00830168.x = static_cast<float>(px + light->x);          // 00474424..00474442
        tri_project_point_00830168.y = static_cast<float>(py + light->y);          // 00474430..00474448
        tri_project_point_00830168.z = static_cast<float>(static_cast<double>(pz) + light->z); // 00474437..00474456
        if (caps == 0 &&                                                           // 004743D9 test esi; 0047445B jne
            clip_projection_caps_0046f580(&tri_project_scaled_00830174, &tri_project_point_00830168, frustum) == 0) // 0047445D..00474476
            return 0;                                                              // (EAX = 0)
        if (shadow_volume_programs_007b9374 == 0) {                                // 0047447C..00474483
            const I vertices = static_cast<I>(field<I16>(node, 0x180));            // 00474489
            for (I v = 0; v < vertices; ++v) shadow_vertex_marks_007a4820[v] = 0;  // 00474490..004744B1 rep stos (count*2 bytes)
            volatile std::uint16_t* edge_out = shadow_edge_indices_007a69c0;        // 004744AC esi = 7A69C0h
            process_faces_004741a0(light, shadow_faces_00828168, node);            // 004744A0..004744B3
            for (I f = 0; f < field<I>(node, 0x5C); ++f) {                         // 004744B8..004745E1 (count re-read)
                const FaceRecord& r = shadow_faces_00828168[f];
                // fld [rec]; fcomp 0.0; test ah,5; jnp next: skip back faces (facing < 0)
                if (static_cast<double>(r.facing) < kZero_0073d700) continue;      // 004744D3..004744E1
                if (silhouette(r, r.adjacent[0])) edge_out = emit_edge(edge_out, r.vertex[0], r.vertex[1]); // 004744E7..00474536
                if (silhouette(r, r.adjacent[1])) edge_out = emit_edge(edge_out, r.vertex[1], r.vertex[2]); // 0047453D..00474583
                if (silhouette(r, r.adjacent[2])) edge_out = emit_edge(edge_out, r.vertex[2], r.vertex[0]); // 00474586..004745CD
            }
            const I words = static_cast<I>(edge_out - shadow_edge_indices_007a69c0); // 004745E7..004745EE (sar 1)
            const I edges = words / 2;                                             // 004745F0..004745F5 cdq; sub; sar 1
            if (edges > 0) {                                                       // 004745F7..004745F9
                U pool;
                if (part_table(self)->s04c(self, nullptr) != 0)                    // 004745FB..00474608 call [edx+4Ch]
                    pool = field<U>(as_object(part_table(self)->s04c(self, nullptr)), 0xB4); // 0047460A..00474611
                else
                    pool = field<U>(node, 0x198);                                  // 00474619
                float length_arg = bits_to_float(length_word);                     // lea eax,[esp+4Ch] = &arg2 (in place)
                const U array = gl_render_build_shadow_array_004269f0(light, &length_arg, frustum, // 0047461F..0047463D
                    &tri_project_scaled_00830174, &tri_project_point_00830168, static_cast<I>(field<I16>(node, 0x180)), pool);
                gl_render_render_shadow_array_00426660(array, edges, caps);         // 00474642..00474649
            }
            if (out == nullptr) return 0;                                          // 00474651..00474657 (EAX = 0)
            *out = static_cast<U>(edges);                                          // 0047465D
            return out_word;                                                       // EAX = out
        }
    }
    // 00474669: vertex-program stencil volume path
    void* const tri = aurora_node_as_trimesh_0043e400(static_cast<aurora_model::MdlNode*>(field<void*>(self, 4)), nullptr); // 00474669..0047466C
    const I count = static_cast<I>(field<I16>(tri, 0x180)) + 1;                    // 00474673..00474683
    const U offset = field<U>(tri, 0x194);                                         // 0047467D
    aurora_model::MaxTree* const model = aurora_tree_as_model_0043e1c0(field<aurora_model::MaxTree*>(tri, 8), nullptr); // 0047467A..00474684
    gl_render_set_interleaved_buffer_00425520(field<U>(tri, 0x14C), field<U>(tri, 0x150), field<U>(model, 0xAC), // 00474689..004746A1
        count, tri, offset);
    return stencil_volume_passes(self, false);                                     // 004746A6..004746F8 (EAX = slot +4 result)
}

// ================================================================ 00477B90 PartSkin::Project (vtbl +5C, RET 10h)
U __fastcall part_skin_project_00477b90(void* self, void*, U light_word, U length_word, U frustum, U out_word) {
    if (skin_shadow_enabled_00828058 == 0) return 0;                               // 00477BAB..00477BBE (EAX = 0)
    const U caps = field<U8>(field<void*>(field<void*>(self, 0x70), 4), 0x188);    // 00477BC4..00477BCA movzx edi
    U* const out = static_cast<U*>(as_object(out_word));
    if (out != nullptr) *out = 0;                                                  // 00477BD1..00477BDC
    U guard = skin_project_guard_00830234;                                         // 00477BDE
    if (!(guard & 1u)) { guard |= 1u; skin_project_guard_00830234 = guard; skin_project_unused_00830228 = Vector{0.0f, 0.0f, 0.0f}; }    // 00477BE3..00477C03
    if (!(guard & 2u)) { guard |= 2u; skin_project_guard_00830234 = guard; skin_project_unused_0083021c = Vector{0.0f, 0.0f, 0.0f}; }    // 00477C0D..00477C2D
    if (!(guard & 4u)) { guard |= 4u; skin_project_guard_00830234 = guard; skin_project_direction_00830210 = Vector{0.0f, 0.0f, 0.0f}; } // 00477C37..00477C57
    if (!(guard & 8u)) { guard |= 8u; skin_project_guard_00830234 = guard; skin_project_scaled_00830204 = Vector{0.0f, 0.0f, 0.0f}; }    // 00477C61..00477C81
    if (!(guard & 0x10u)) { guard |= 0x10u; skin_project_guard_00830234 = guard; skin_project_point_008301f8 = Vector{0.0f, 0.0f, 0.0f}; } // 00477C8B..00477CAB
    const Vector* const light = static_cast<const Vector*>(as_object(light_word)); // 00477CB5 esi
    Vector negated;                                                                // [esp+10h]
    negated.z = static_cast<float>(-static_cast<double>(light->z));                // 00477CBC..00477CDC (stored last)
    negated.y = static_cast<float>(-static_cast<double>(light->y));
    negated.x = static_cast<float>(-static_cast<double>(light->x));
    Vector unit;                                                                   // [esp+3Ch]
    skin_project_direction_00830210 = *normalize_004ab180(&unit, &negated);         // 00477CE0..00477CF9
    const double scale = shadow_length_scale_0082804c;
    const double sz = skin_project_direction_00830210.z * scale;                   // 00477CFE..00477D04
    const double sy = skin_project_direction_00830210.y * scale;                   // 00477D0F..00477D15
    const double sx = skin_project_direction_00830210.x * scale;                   // 00477D1B..00477D21
    skin_project_scaled_00830204.x = static_cast<float>(sx);                       // 00477D27..00477D2F
    skin_project_scaled_00830204.y = static_cast<float>(sy);                       // 00477D35..00477D3D
    skin_project_scaled_00830204.z = static_cast<float>(sz);                       // 00477D43..00477D51
    const double length = bits_to_float(length_word);                              // [esp+FCh]
    const double pz = skin_project_direction_00830210.z * length;                  // 00477D4B..00477D56
    const float py = static_cast<float>(skin_project_direction_00830210.y * length); // 00477D5D..00477D6A fstp [esp+1Ch]
    const float px = static_cast<float>(skin_project_direction_00830210.x * length); // 00477D6E..00477D7B fstp [esp+3Ch]
    const double point_z = pz + light->z;                                          // 00477D7F fadd [esi+8]
    const double point_y = static_cast<double>(py) + light->y;                     // 00477D82..00477D86
    const double point_x = static_cast<double>(px) + light->x;                     // 00477D89..00477D8D
    skin_project_point_008301f8.x = static_cast<float>(point_x);                   // 00477D8F..00477D97
    skin_project_point_008301f8.y = static_cast<float>(point_y);                   // 00477D9D..00477DA5
    skin_project_point_008301f8.z = static_cast<float>(point_z);                   // 00477DAB..00477DB3
    if (caps == 0 &&                                                               // 00477D0D cmp edi,ebp ... 00477DB8 jne
        clip_projection_caps_0046f580(&skin_project_scaled_00830204, &skin_project_point_008301f8, frustum) == 0) // 00477DBA..00477DD6
        return 0;
    if (shadow_mode_00828054 == 0) return 0;                                       // 00477DDC..00477DE2
    if (*mesh_cells.skin_programs_0078e8c0 == 0) return 0;                         // 00477DE8..00477DEE
    if (kotor_renderer_init::VertexProgramsAvailable_0045f770() == 0) return 0;    // 00477DF4..00477DFB
    gl::glEnableClientState_0073d260(kAttribArray1_0073f3a8);                      // 00477E01..00477E0E
    gl::glEnableClientState_0073d260(kAttribArray4_0073f3b0);                      // 00477E10..00477E16
    void* const prim_part = field<void*>(field<void*>(self, 0x70), 8);             // 00477E18..00477E1B
    void* root = as_object(part_table(prim_part)->s050(prim_part, nullptr));       // 00477E1E..00477E23 call [edx+50h] -> esi
    void* const skin_node = field<void*>(root, 4);                                 // 00477E25 edi ([esp+50h])
    (void)part_table(root)->s02c(root, nullptr);                                   // 00477E28..00477E30 call [eax+2Ch] (EAX unused)
    void* const prim = field<void*>(self, 0x70);
    primitive_table(prim)->s018(prim, nullptr);                                    // 00477E33..00477E38 call [edx+18h]
    float matrix[9];                                                               // [esp+54h]
    matrix_identity_construct_00437af0(matrix, nullptr);                           // 00477E3B..00477E3F
    if (!(skin_project_guard_00830234 & 0x20u)) {                                  // 00477E44..00477E51
        skin_project_guard_00830234 |= 0x20u;                                      // 00477E53
        skin_project_q_bones_008301ec.size = 0; skin_project_q_bones_008301ec.capacity = 0; skin_project_q_bones_008301ec.data = 0; // 00477E5E..00477E6A
        (void)std::atexit(part_skin_free_project_q_bones_0073bc70);                // 00477E59..00477E70 (006FB332)
    }
    if (!(skin_project_guard_00830234 & 0x40u)) {                                  // 00477E83..00477E90
        skin_project_guard_00830234 |= 0x40u;                                      // 00477E92
        skin_project_t_bones_008301e0.size = 0; skin_project_t_bones_008301e0.capacity = 0; skin_project_t_bones_008301e0.data = 0; // 00477E9D..00477EA9
        (void)std::atexit(part_skin_free_project_t_bones_0073bc50);                // 00477E98..00477EAF
    }
    skin_project_q_bones_008301ec.size = 0;                                        // 00477ED4 mov [008301F0],ebp
    skin_project_t_bones_008301e0.size = 0;                                        // 00477EDA mov [008301E4],ebp
    gl_program_env_parameter_0044f820(0x10, 0.0f, 2.0f, 3.0f, 1.0f);               // 00477EC2..00477EE0
    // Inverse root transform: P0 = conj(Q_root) * (-pos_root), Q0 = conj(Q_root) (00477EE5..00477F2F)
    Vector position;                                                               // [esp+10h]
    position.x = static_cast<float>(-static_cast<double>(field<float>(root, 8)));
    position.y = static_cast<float>(-static_cast<double>(field<float>(root, 0xC)));
    position.z = static_cast<float>(-static_cast<double>(field<float>(root, 0x10)));
    Quaternion conjugate;                                                          // [esp+78h]
    Vector p0;                                                                     // [esp+20h]
    quaternion_rotate_004a9bb0(&p0, quaternion_negate_w_004a9b50(&conjugate, static_cast<const Quaternion*>(at(root, 0x14))), &position);
    Quaternion q0;                                                                 // [esp+2Ch]
    quaternion_negate_w_004a9b50(&q0, static_cast<const Quaternion*>(at(root, 0x14)));
    for (void* parent = field<void*>(root, 0x3C); parent != nullptr; parent = field<void*>(parent, 0x3C)) { // 00477F2F..00478013
        root = parent;                                                             // 00477F40 esi = eax
        Vector local;                                                              // [esp+3Ch]
        local.x = static_cast<float>(-static_cast<double>(field<float>(root, 8)));    // 00477F42..00477F4C
        local.y = static_cast<float>(-static_cast<double>(field<float>(root, 0xC)));  // 00477F53..00477F60
        local.z = static_cast<float>(-static_cast<double>(field<float>(root, 0x10))); // 00477F64..00477F6A
        const Quaternion* const q = static_cast<const Quaternion*>(at(root, 0x14)); // 00477F50 ebx = esi+14h
        Quaternion qn;                                                             // [esp+B4h]
        Vector t;                                                                  // [esp+98h]
        Vector* const tr = quaternion_rotate_004a9bb0(&t, quaternion_negate_w_004a9b50(&qn, q), &local); // 00477F6E..00477F7F
        Vector u;                                                                  // [esp+78h]
        const Vector* const ur = quaternion_rotate_004a9bb0(&u, &q0, tr);          // 00477F84..00477F92
        const float nx = static_cast<float>(static_cast<double>(p0.x) + ur->x);    // 00477F97..00477F9E
        const float ny = static_cast<float>(static_cast<double>(p0.y) + ur->y);    // 00477FA2..00477FA9
        const float nz = static_cast<float>(static_cast<double>(p0.z) + ur->z);    // 00477FAD..00477FC7
        p0.x = nx; p0.y = ny; p0.z = nz;                                           // 00477FBC / 00477FD0 / 00477FD4
        Quaternion qn2;                                                            // [esp+A4h]
        Quaternion product;                                                        // [esp+88h]
        const Quaternion* const m = quaternion_multiply_004a9ac0(&product, &q0,
            quaternion_negate_w_004a9b50(&qn2, q));                                 // 00477FCF..00477FEB
        q0 = *m;                                                                   // 00477FF0..00478007 (4 DWORD copies)
    }
    gather_bones_00488930(root, &skin_project_t_bones_008301e0, &skin_project_q_bones_008301ec, p0, q0); // 00478019..0047805D
    for (I i = 0; i < field<I>(skin_node, 0x1B4); ++i) {                           // 00478062..004781E2 (count re-read)
        const I index = ftol2(static_cast<double>(field<const float*>(skin_node, 0x1B0)[i])); // 00478080..00478091
        if (index == -1) continue;                                                 // 0047808E..00478095
        Vector* const t_bone = reinterpret_cast<Vector*>(static_cast<std::uintptr_t>(skin_project_t_bones_008301e0.data) + static_cast<U>(i) * 12u); // 004780AF
        Quaternion* const q_bone = reinterpret_cast<Quaternion*>(static_cast<std::uintptr_t>(skin_project_q_bones_008301ec.data) + static_cast<U>(i) * 16u); // 004780B2
        const Vector* const bind = reinterpret_cast<const Vector*>(field<U>(skin_node, 0x1C4) + static_cast<U>(i) * 12u); // 004780A0..004780AC
        Vector rotated;                                                            // [esp+78h]
        const Vector* const r = quaternion_rotate_004a9bb0(&rotated, q_bone, bind); // 004780B6..004780BE
        const float bx = static_cast<float>(static_cast<double>(r->x) + t_bone->x); // 004780C3..004780CD
        const float by = static_cast<float>(static_cast<double>(t_bone->y) + r->y); // 004780D5..004780DB
        const float bz = static_cast<float>(static_cast<double>(t_bone->z) + r->z); // 004780DF..004780F1
        Vector* const t_store = reinterpret_cast<Vector*>(static_cast<std::uintptr_t>(skin_project_t_bones_008301e0.data) + static_cast<U>(i) * 12u); // 004780C7 / 004780E5 (data re-read)
        t_store->x = bx; t_store->y = by; t_store->z = bz;                         // 004780EC..004780F9
        Quaternion* const q_store = reinterpret_cast<Quaternion*>(static_cast<std::uintptr_t>(skin_project_q_bones_008301ec.data) + static_cast<U>(i) * 16u); // 00478102..0047810B
        const Quaternion* const bind_q = reinterpret_cast<const Quaternion*>(field<U>(skin_node, 0x1B8) + static_cast<U>(i) * 16u); // 004780FC..00478108
        Quaternion combined;                                                       // [esp+84h]
        *q_store = *quaternion_multiply_004a9ac0(&combined, q_store, bind_q);      // 0047810E..00478132
        Quaternion* const q_matrix = reinterpret_cast<Quaternion*>(static_cast<std::uintptr_t>(skin_project_q_bones_008301ec.data) + static_cast<U>(i) * 16u); // 00478135..00478143
        float rotation[9];                                                         // [esp+C4h]
        const float* const mr = quaternion_get_matrix_0044f470(q_matrix, nullptr, rotation); // 0047813B..00478146
        for (int k = 0; k < 9; ++k) matrix[k] = mr[k];                             // 0047814D..0047815B rep movs 9
        const Vector* const t = reinterpret_cast<const Vector*>(static_cast<std::uintptr_t>(skin_project_t_bones_008301e0.data) + static_cast<U>(i) * 12u); // 0047814D / 00478168
        const U base = static_cast<U>(index) * 3u + 0x12u;                         // 00478177 lea esi,[esi+esi*2+12h]
        gl_program_env_parameter_0044f820(base, matrix[0], matrix[3], matrix[6], t->x);         // 0047815D..0047817C (raw DWORDs)
        gl_program_env_parameter_0044f820(base + 1u, matrix[1], matrix[4], matrix[7], t->y);    // 00478181..00478199
        gl_program_env_parameter_0044f820(base + 2u, matrix[2], matrix[5], matrix[8], t->z);    // 0047819E..004781BC
    }
    return stencil_volume_passes(self, true);                                      // 004781E8..00478237 (EAX = slot +8 result)
}

// ================================================================ 00484290 PartDanglyMesh::Animate (vtbl +68, RET 4)
// Spring/constraint simulation of the dangly vertices, then upload of the positions into the model vertex pool and
// Animate of the children. EAX on return: 0 on the early returns (the tested words), else the child count [self+34].
U __fastcall part_dangly_mesh_animate_00484290(void* self, void*, U dt_word) {
    if (*mesh_cells.gate_0078e844 == 0) return 0;                                  // 00484290..004842A3
    if (*cells.active_scene_007b92a0 == nullptr) return 0;                         // 004842A9..004842B0
    float dt = bits_to_float(dt_word);                                             // [esp+E4h] (rewritten in place)
    // fld dt; fcomp 0.035f; test ah,41h; jne: clamp to 0.035f when dt > 0.035f
    if (static_cast<double>(dt) > cst(kDtMax_00743220)) {                          // 004842B6..004842C8
        dt_word = kDtMax_00743220; dt = bits_to_float(dt_word);                    // 004842CA..004842CF
    } else if (static_cast<double>(dt) < cst(kDtMin_00743224)) {                   // 004842D8..004842EA test ah,5; jp
        dt_word = kDtMin_00743224; dt = bits_to_float(dt_word);                    // 004842EC..004842F2
    }
    void* const node = field<void*>(self, 4);                                      // 00484300 esi
    U guard = dangly_guard_00830374;                                               // 004842F9
    if (!(guard & 1u)) { guard |= 1u; dangly_guard_00830374 = guard; dangly_next_00830368 = Vector{0.0f, 0.0f, 0.0f}; }      // 004842FE..00484321
    if (!(guard & 2u)) { guard |= 2u; dangly_guard_00830374 = guard; dangly_rest_0083035c = Vector{0.0f, 0.0f, 0.0f}; }      // 0048432B..0048434B
    if (!(guard & 4u)) { guard |= 4u; dangly_guard_00830374 = guard; dangly_target_00830350 = Vector{0.0f, 0.0f, 0.0f}; }    // 00484355..00484375
    if (!(guard & 8u)) { guard |= 8u; dangly_guard_00830374 = guard; dangly_velocity_00830344 = Vector{0.0f, 0.0f, 0.0f}; }  // 0048437F..0048439F
    if (!(guard & 0x10u)) { guard |= 0x10u; dangly_guard_00830374 = guard; dangly_unused_00830338 = Vector{0.0f, 0.0f, 0.0f}; } // 004843A9..004843C9
    Vector position{0.0f, 0.0f, 0.0f};                                             // [esp+1Ch] (004843E2..004843F2)
    Quaternion orientation{1.0f, 0.0f, 0.0f, 0.0f};                                // [esp+34h] (004843FA..00484412)
    part_table(self)->s00c(self, nullptr, word_of(&position), word_of(&orientation)); // 004843D3..0048441A GetWorld (vtbl +0C)
    void* const prim = field<void*>(self, 0x70);
    void* const dangly = as_object(primitive_table(prim)->s010(prim, nullptr));    // 0048441D..00484435 ([esp+5Ch])
    Vector* const previous = static_cast<Vector*>(at(self, 0x98));                 // 0048442F edi = self+98h
    Vector motion;                                                                 // [esp+50h]
    motion.x = static_cast<float>(static_cast<double>(position.x) - previous->x);  // 00484425..0048443D
    motion.y = static_cast<float>(static_cast<double>(position.y) - previous->y);  // 00484442..00484449
    motion.z = static_cast<float>(static_cast<double>(position.z) - previous->z);  // 0048444D..00484454
    const double moved = vector_magnitude_004ab100(&motion);                       // 00484458
    if (moved > cst(kJump_00743264)) {                                             // 0048445D..0048446B (test ah,41h; jne normal)
        // Teleport: reset every simulated vertex to its rest position (node+1B4).
        for (I i = 0; i < static_cast<I>(field<I16>(node, 0x180)); ++i) {         // 0048446D..004844BA (count re-read)
            U* const dst = reinterpret_cast<U*>(field<U>(self, 0xB4) + static_cast<U>(i) * 12u);   // 00484480..0048448F
            const U* const src = reinterpret_cast<const U*>(field<U>(node, 0x1B4) + static_cast<U>(i) * 12u);
            dst[0] = src[0]; dst[1] = reinterpret_cast<const U*>(field<U>(node, 0x1B4) + static_cast<U>(i) * 12u)[1];  // 00484491..0048449D (base re-read)
            dst[2] = reinterpret_cast<const U*>(field<U>(node, 0x1B4) + static_cast<U>(i) * 12u)[2];                   // 004844A0..004844AA
        }
    } else {
        Quaternion* const previous_q = static_cast<Quaternion*>(at(self, 0xA4));   // ebp+A4h
        // First frame: previous orientation x/y/z all exactly 0.0 -> seed previous pose with the current one.
        if (static_cast<double>(field<float>(self, 0xA8)) == kZero_0073d700 &&     // 004844C1..004844D6 (test ah,44h; jp)
            static_cast<double>(field<float>(self, 0xAC)) == kZero_0073d700 &&     // 004844D8..004844E9
            static_cast<double>(field<float>(self, 0xB0)) == kZero_0073d700) {     // 004844EB..004844FC
            std::memcpy(previous_q, &orientation, sizeof orientation);             // 004844FE..0048451C (DWORD copies)
            std::memcpy(previous, &position, sizeof position);                     // 0048451F..0048452E
        }
        const float wind_scale = bits_to_float(kWind_00743230);                    // 00484538 / 00484551 [esp+10h]
        Vector wind;                                                               // [esp+A8h]
        void* const scene = *cells.active_scene_007b92a0;                          // 00484555
        const U wind_word = (*static_cast<const kotor_scene3d::SceneVTable* const volatile*>(scene))->s0d0_scene_get_wind_vector( // 0048455B..00484568
            scene, nullptr, word_of(&wind), float_to_bits(position.x), float_to_bits(position.y), float_to_bits(position.z), dt_word);
        Quaternion inverse;                                                        // [esp+B4h]
        Vector wind_local;                                                         // [esp+78h]
        const Vector* const wl = quaternion_rotate_004a9bb0(&wind_local,          // 0048456E..0048458D
            quaternion_negate_w_004a9b50(&inverse, &orientation), static_cast<const Vector*>(as_object(wind_word)));
        Vector force;                                                              // [esp+50h]
        force.x = static_cast<float>(static_cast<double>(wind_scale) * wl->x);     // 00484592..004845A2
        force.y = static_cast<float>(static_cast<double>(wind_scale) * wl->y);     // 004845AD..004845B4
        force.z = static_cast<float>(static_cast<double>(wind_scale) * wl->z);     // 004845B8..004845C5
        Quaternion inverse2;                                                       // [esp+B4h]
        Quaternion rotation;                                                       // [esp+C4h]
        quaternion_multiply_004a9ac0(&rotation,                                    // 004845A1..004845DA (out, conj(Q), prevQ)
            quaternion_negate_w_004a9b50(&inverse2, &orientation), previous_q);
        Vector back;                                                               // [esp+44h]
        back.x = static_cast<float>(static_cast<double>(previous->x) - position.x); // 004845DF..004845ED
        back.y = static_cast<float>(static_cast<double>(previous->y) - position.y); // 004845F8..00484601
        back.z = static_cast<float>(static_cast<double>(previous->z) - position.z); // 00484605..0048460C
        Quaternion inverse3;                                                       // [esp+B4h]
        Vector back_local;                                                         // [esp+9Ch]
        quaternion_rotate_004a9bb0(&back_local, quaternion_negate_w_004a9b50(&inverse3, previous_q), &back); // 00484610..00484621
        const float stiffness = static_cast<float>(cst(kDisp_0074322c) *          // 00484626..00484642 fstp [esp+30h]
            field<float>(node, 0x1AC) * static_cast<double>(dt));
        for (I i = 0; i < static_cast<I>(field<I16>(node, 0x180)); ++i) {          // 00484635..00484B22 ([esp+18h])
            const float weight = field<const float*>(node, 0x19C)[i];              // 00484660..00484666
            if (static_cast<double>(weight) == kZero_0073d700) continue;           // 00484669..00484674 (test ah,44h; jnp)
            const U row = static_cast<U>(i) * 12u;                                 // ebx
            const float* const rest = reinterpret_cast<const float*>(field<U>(node, 0x1B4) + row);
            fcopy(dangly_rest_0083035c.x, rest[0]);                                // 0048467A..00484689
            fcopy(dangly_rest_0083035c.y, reinterpret_cast<const float*>(field<U>(node, 0x1B4) + row)[1]); // 0048468F..0048469B
            fcopy(dangly_rest_0083035c.z, reinterpret_cast<const float*>(field<U>(node, 0x1B4) + row)[2]); // 004846A1..004846B0
            Vector* const cur = reinterpret_cast<Vector*>(field<U>(self, 0xB4) + row); // 00484683..00484699 edi
            std::memcpy(&dangly_target_00830350, cur, sizeof(Vector));             // 004846B6..004846E5 (DWORD moves)
            const float tx = static_cast<float>(static_cast<double>(back_local.x) + dangly_target_00830350.x); // 004846B8..004846DA
            Vector* const vel = reinterpret_cast<Vector*>(field<U>(dangly, 0x1C) + row); // 004846EB..004846F4 ([prim+10 result]+1C)
            std::memcpy(&dangly_velocity_00830344, vel, sizeof(Vector));           // 004846F6..00484736
            const float ty = static_cast<float>(static_cast<double>(back_local.y) + dangly_target_00830350.y); // 004846DE..00484705
            const float tz = static_cast<float>(static_cast<double>(back_local.z) + dangly_target_00830350.z); // 00484709..0048473B
            dangly_target_00830350.x = tx; dangly_target_00830350.y = ty; dangly_target_00830350.z = tz; // 00484723 / 00484730 / 0048474C
            Vector rt;                                                             // [esp+D4h]
            dangly_target_00830350 = *quaternion_rotate_004a9bb0(&rt, &rotation, &dangly_target_00830350); // 00484743..00484784
            Vector rv;                                                             // [esp+B4h]
            dangly_velocity_00830344 = *quaternion_rotate_004a9bb0(&rv, &rotation, &dangly_velocity_00830344); // 0048476A..004847A3
            const double gain = cst(kTight_00743228) * field<float>(node, 0x1B0) * static_cast<double>(dt); // 00484789..004847B1 (ST0, kept)
            const double gx = dangly_velocity_00830344.x * gain;                   // 004847B8..004847BE (ST0, kept)
            const float gy = static_cast<float>(dangly_velocity_00830344.y * gain); // 004847C0..004847C8 fstp [esp+94h]
            const float gz = static_cast<float>(dangly_velocity_00830344.z * gain); // 004847CF..004847D7 fstp [esp+98h]
            const double dx = static_cast<double>(dangly_rest_0083035c.x) - dangly_target_00830350.x; // 004847E4..004847EE
            const double dy = static_cast<double>(dangly_rest_0083035c.y) - dangly_target_00830350.y; // 004847FB..00484802
            const float dz = static_cast<float>(static_cast<double>(dangly_rest_0083035c.z) - dangly_target_00830350.z); // 00484808..00484814
            const float spring = static_cast<float>(static_cast<double>(stiffness) * weight); // 0048481B..00484827 fstp [esp+14h]
            const float ex = static_cast<float>(dx * spring);                      // 0048482B..00484831
            const float ey = static_cast<float>(dy * spring);                      // 00484835..00484839
            const float ez = static_cast<float>(static_cast<double>(dz) * spring); // 0048483D..00484848
            Vector change;                                                         // [esp+84h]
            change.x = static_cast<float>(static_cast<double>(ex) - gx);           // 0048484C..00484852
            change.y = static_cast<float>(static_cast<double>(ey) - gy);           // 0048485D..00484868
            change.z = static_cast<float>(static_cast<double>(ez) - gz);           // 0048486F..0048487A
            vector_add_assign_004a9740(&dangly_velocity_00830344, nullptr, &change); // 004847F4 / 00484822..00484881
            std::memcpy(reinterpret_cast<void*>(field<U>(dangly, 0x1C) + row), &dangly_velocity_00830344, sizeof(Vector)); // 00484886..004848A4
            dangly_next_00830368.x = static_cast<float>(static_cast<double>(dangly_velocity_00830344.x) * dt); // 004848A7..004848C7
            dangly_next_00830368.y = static_cast<float>(static_cast<double>(dangly_velocity_00830344.y) * dt); // 004848C1..004848EE
            dangly_next_00830368.z = static_cast<float>(static_cast<double>(dangly_velocity_00830344.z) * dt); // 004848DD..004848FC
            vector_add_assign_004a9740(&dangly_next_00830368, nullptr, &dangly_target_00830350); // 004848AD..00484901
            if (*cells.active_scene_007b92a0 != nullptr) {                         // 00484906..0048490D
                const double m = (kOne_0073d764 - static_cast<double>(field<const float*>(node, 0x19C)[i]) * cst(kByte_0073ede0)) // 0048490F..0048492C
                                 * field<float>(node, 0x1A8);                      // 00484932
                Vector gust;                                                       // [esp+78h]
                gust.x = static_cast<float>(static_cast<double>(force.x) * m);     // 00484938..0048493E
                gust.y = static_cast<float>(static_cast<double>(force.y) * m);     // 00484942..00484948
                gust.z = static_cast<float>(static_cast<double>(force.z) * m);     // 0048494F..00484955
                vector_add_assign_004a9740(&dangly_next_00830368, nullptr, &gust); // 0048491C..0048495E
            }
            const float limit = static_cast<float>((kOne_0073d764 - static_cast<double>(field<const float*>(node, 0x19C)[i]) * cst(kByte_0073ede0)) // 00484963..00484976
                                                   * field<float>(node, 0x1A8));   // 0048497C..00484982 fstp [esp+14h]
            const float ox = static_cast<float>(static_cast<double>(dangly_next_00830368.x) - dangly_rest_0083035c.x); // 00484986..00484992
            const float oy = static_cast<float>(static_cast<double>(dangly_next_00830368.y) - dangly_rest_0083035c.y); // 00484996..004849A2
            const float oz = static_cast<float>(static_cast<double>(dangly_next_00830368.z) - dangly_rest_0083035c.z); // 004849A6..004849B2
            const double low = cst(kLow_00743260), high = cst(kHigh_0074325c);
            // fcomp low; test ah,5; jnp fallback (x < low) ... fcomp high; test ah,41h; je fallback (x > high)
            const bool in_range = !(ox < low) && !(oy < low) && !(oz < low) &&     // 004849B6..004849EF
                                  !(ox > high) && !(oy > high) && !(oz > high);    // 004849F5..00484A2E
            if (!in_range) {
                std::memcpy(cur, &dangly_rest_0083035c, sizeof(Vector));          // 00484AF4..00484B0A
                continue;
            }
            // Clamp each axis to rest +- limit (00484A34..00484AD6): only the over-limit components are rewritten.
            if (static_cast<double>(ox) > limit)                                    // fcomp [esp+14h]; test ah,41h; jne
                dangly_next_00830368.x = static_cast<float>(static_cast<double>(limit) + dangly_rest_0083035c.x); // 00484A47..00484A66
            else if (-static_cast<double>(limit) > ox)                              // 00484A4F..00484A5A fchs; fcomp
                dangly_next_00830368.x = static_cast<float>(static_cast<double>(dangly_rest_0083035c.x) - limit); // 00484A5C..00484A66
            if (static_cast<double>(oy) > limit)
                dangly_next_00830368.y = static_cast<float>(static_cast<double>(limit) + dangly_rest_0083035c.y); // 00484A6C..00484A9E
            else if (-static_cast<double>(limit) > oy)
                dangly_next_00830368.y = static_cast<float>(static_cast<double>(dangly_rest_0083035c.y) - limit);
            if (static_cast<double>(oz) > limit)
                dangly_next_00830368.z = static_cast<float>(static_cast<double>(limit) + dangly_rest_0083035c.z); // 00484AA4..00484AD6
            else if (-static_cast<double>(limit) > oz)
                dangly_next_00830368.z = static_cast<float>(static_cast<double>(dangly_rest_0083035c.z) - limit);
            std::memcpy(cur, &dangly_next_00830368, sizeof(Vector));               // 00484ADC..00484B0A (DWORD moves)
        }
    }
    // 00484B28: copy the simulated positions into the model vertex pool.
    aurora_model::MaxTree* const model = aurora_tree_as_model_0043e1c0(
        field<aurora_model::MaxTree*>(field<void*>(self, 4), 8), nullptr);         // 00484B28..00484B2E
    const U locked = word_of(aurora_pool_lock_00423f10(field<U>(model, 0xAC), 0, 0, 1)); // 00484B33..00484B51 ([esp+30h])
    for (I i = 0; i < static_cast<I>(field<I16>(node, 0x180)); ++i) {              // 00484B45..00484BA2
        U* const dst = reinterpret_cast<U*>(field<U>(node, 0x14C) * static_cast<U>(i) + field<U>(node, 0x194) +
                                            field<U>(node, 0x154) + locked);       // 00484B60..00484B83
        const U* const src = reinterpret_cast<const U*>(field<U>(self, 0xB4) + static_cast<U>(i) * 12u); // 00484B6F..00484B81
        dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];                         // 00484B85..00484B92
    }
    aurora_model::MaxTree* const model2 = aurora_tree_as_model_0043e1c0(
        field<aurora_model::MaxTree*>(field<void*>(self, 4), 8), nullptr);         // 00484BA4..00484BAA
    aurora_pool_unlock_00423f30(field<U>(model2, 0xAC));                           // 00484BAF..00484BB6
    std::memcpy(at(self, 0x98), &position, sizeof position);                       // 00484BBB..00484BE3 (previous pose)
    std::memcpy(at(self, 0xA4), &orientation, sizeof orientation);                 // 00484BD3..00484C01
    I count = field<I>(self, 0x34);                                                // 00484BF7
    for (I i = 0; i < count; i = i + 1, count = field<I>(self, 0x34)) {            // 00484BFD..00484C29 (count re-read)
        void* const child = reinterpret_cast<void**>(field<U>(self, 0x30))[i];     // 00484C10..00484C13
        part_table(child)->s068(child, nullptr, dt_word);                          // 00484C16..00484C20 push [esp+E4h]; call [eax+68h]
    }
    return static_cast<U>(count);                                                  // EAX = [ebp+34h]
}
}
