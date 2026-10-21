#pragma once
// lane-3d-bsp / triangle_bin: CAurTriangleBin grass-bin bodies (004A67B0 FinishGeometry, 004A68B0 UpdateGrassForWind,
// 004A6D30 RenderGrassPolys, 004A8460 DestroyGrassPolys, 004A8500 SortTriangles, 004A87C0 CreateArrays,
// 004A9630 BuildGrassPolys). Conventions: see bsp.h (D1 / D24 / D30-FINAL). PROPOSED for append to scene3d.h.
//
// CAurTriangleBin byte view (offsets cited at the first instruction that uses them):
//   +00 U16 vertex stride (004A9653 = 20h/24h/28h; 004A87D0 movzx)      +02 U8 flags (1 arrays, 4 built, 8 lightmapped,
//   10h vertex colour)                                                  +04 float view distance (004A6DB1)
//   +08 float radius (004A682C)   +0C I triangle count (004A6D70)       +10 I grass material index (004A6D83)
//   +14/+20 Vector min/max (004A67B6)  +2C Vector centre (004A67ED)      +38 vertex buffer (004A87E4)
//   +3C U write cursor / wind-record base (004A87FB, 004A6E6B)          +40/+44 (004A71CE lea, 004A87F1 = 48*count)
//   +48 CExoArrayList<CAurTriangleListElem> (stride 18h; 004A852C)      +54 SafePointer<CAurTexture> (004A6E81)
//   +58 CExoArrayList<I> run counts (004A8532)                          +64 CExoArrayList<SafePointer<CAurTexture>>
// CAurTriangleListElem (18h): +00 CExoArrayList<float> corner floats (9 used, 004A892C), +0C face (normal +0..+8,
// corner indices +1A/+1C/+1E words, 004A8BEF), +10 Part (004A8A98), +14 I blade count (004A89B5).
// Grass info records [00831E44] stride 54h (004A6A01 imul 54h): +24 height (004A6A04), +28/+34 Vector colours
// (004A6E35 / 004A6E0F), +40 float[4] atlas weights (004A9129).
#include "11b317539a-bsp.h"
#include "e4678f005c-vertex_program.h"

// ---------------------------------------------------------------- existing owners (exact declarations copied)
namespace texture { struct Basic; }                      // CAurTexture (lane-texture-epoch E1 layout)
namespace kotor_renderer_init {
// lane-renderer-bodies extension_loader.h (cell 007A47E0, written by LoadExtensions 00436490)
using EnableDispatch=void(__cdecl*)(U32,texture::Basic*,texture::Basic*,texture::Basic*,texture::Basic*);
}
namespace kotor_renderer_gl {
void __cdecl ActivateTextureStage_0041fe80(std::uint32_t stage);          // lane-renderer-bodies render_shared_lowers.h
}
namespace kotor_vertex_program {
using U8 = std::uint8_t;
extern U8 __cdecl ValidateNonRadeon_0045f5e0();                         // lane-renderer-bodies enable_bindings.h
}
namespace services_tail5 {
using U32=std::uint32_t;
U32 __cdecl Seed(U32 value);                                            // strict link service-tail-8.obj (srand 006FC43A)
}
namespace kotor_trimesh_data {
// D13 original .data program texts (writable char arrays; definitions from the LOCAL generated header)
extern char nv_program_0079ab28[574];                                    // lane-3d-mesh spec entry (exists)
extern char arb_grass_program_0078feb8[1095];                            // NEW spec entry needed (cstring, 0078feb8, 1095)
}
extern "C" {
// lane-3d-model (model_lifetime.cpp / model_load.hpp / model_texture_lists.cpp spellings)
std::int32_t __cdecl crt_rand_006fc447();
void* __cdecl aurora_pool_lock_00423f10(aurora_model::U handle, aurora_model::U unused2, aurora_model::U unused3, aurora_model::U a4);
aurora_model::U __cdecl aurora_pool_unlock_00423f30(aurora_model::U handle);
aurora_model::MdlNode* __fastcall aurora_node_as_trimesh_0043e400(aurora_model::MdlNode* self, void* unused_edx);
void __fastcall aurora_texture_safe_pointer_destroy_00422380(void* self, void* unused_edx);
// strict link task10-c2da-array-normal.obj (lane-3d-room spelling)
void __stdcall c2da_destroy_array_006fa852(void* first, std::uint32_t size, std::int32_t count, void (__fastcall* destroy)(void*, void*));
}

namespace kotor_scene3d_bsp {
// SafePointer<CAurTexture> passed BY VALUE to CExoArrayList<SafePointer<CAurTexture>>::Add 004A8230 (thiscall RET 4):
// the original builds it directly in the outgoing argument slot (004A863E push ecx; 004A863F mov edi,esp;
// 004A8645 [edi] = target; when target != 0 the slot ADDRESS is appended to target+4 (registrations) with inline
// growth cap ? cap*2 : 8 through 005E03C0, 004A8649..004A8673) and the callee destroys it (00422380). A class with a
// non-trivial destructor passed by value is constructed in place in the argument area and destroyed by the callee
// (MSVC x86), which reproduces the registered address exactly.
class SafeTextureArg {
public:
    explicit SafeTextureArg(void* texture) noexcept : target(texture) {
        if (texture != nullptr) add_with_growth(list_at(texture, 4), this);
    }
    SafeTextureArg(const SafeTextureArg&) = delete;
    SafeTextureArg& operator=(const SafeTextureArg&) = delete;
    ~SafeTextureArg() { aurora_texture_safe_pointer_destroy_00422380(this, nullptr); }
    void* target;
};
static_assert(sizeof(SafeTextureArg) == 4, "SafePointer<CAurTexture> is one word");

// CAurTexture vtable view: +B4 bind (004A6E86 / 004A70A0 / 004A70E5, RET 0).
struct TextureSlots {
    const void* o00[0xB4 / 4];
    void (__fastcall* bind)(void* self, void*);
};
static_assert(offsetof(TextureSlots, bind) == 0xB4, "texture +B4");

// Borrowed original globals shared with other areas (Main publishes once).
struct GrassCells {
    const volatile U* grass_info_00831e44;                   // address of the 54h-stride records (BSS; also 004A6840/004A74F0/004A7860)
    kotor_renderer_init::EnableDispatch const volatile* enable_dispatch_007a47e0; // = MeshCells::enable_dispatch_007a47e0
    void* volatile* bss_00830560;                            // = MeshCells::bss_00830560 (VertexProgram parameter words)
    volatile I* triangles_00827fb0;                          // = MeshCells::triangles_00827fb0 (+= count, 004A71E9)
    const volatile U* vertex_program_0078d46c;               // = MeshCells::vertex_program_0078d46c (0x8620)
    const volatile U* programs_0078e5ec;                     // = scene3d cells.programs_0078e5ec (004A6F17)
    void* volatile* active_scene_007b92a0;                   // = scene3d cells.active_scene_007b92a0 (004A68D6)
    const volatile U* frame_time_0078e574;                   // = scene3d cells.frame_time_0078e574 (004A68EC)
};
extern const GrassCells& grass_cells;
} // namespace kotor_scene3d_bsp

// ---------------------------------------------------------------- cells owned by this area (only 004A68B0 /
// 004A6D30 / 004A87C0 reference them; .data cold values from the pinned image). Storage: triangle_bin_cells.cpp.
extern "C" {
extern volatile float grass_near_007a1964;                  // .data 225.0f (004A6D91, 004A6E57)
extern volatile kotor_scene3d::U grass_wind_enabled_007a196c;     // .data 1 (004A6E48)
extern volatile kotor_scene3d::U grass_textured_007a1970;         // .data 1 (004A6E79, 004A6EC3)
extern volatile kotor_scene3d::U grass_wind_update_007a1974;      // .data 1 (004A68C5)
extern volatile float grass_wind_scale_007a1978;            // .data 1.0f (004A6BEF)
extern volatile kotor_scene3d::U grass_no_fade_00831e40;          // BSS (004A6D8C)
// UpdateGrassForWind function statics (guard 00831E8C bits 1/2/4/8/10h)
extern kotor_scene3d::Vector grass_point_00831e50;        // passed by address to 004A5950 / 004A6440
extern kotor_scene3d::Vector grass_point_wind_00831e5c;
extern kotor_scene3d::Vector grass_global_wind_00831e68;
extern kotor_scene3d::Vector grass_wind_sum_00831e74;
extern kotor_scene3d::Vector grass_offset_00831e80;
extern volatile kotor_scene3d::U grass_wind_guard_00831e8c;
// RenderGrassPolys function static VertexProgram* (guard 00831E94 bit 1)
extern kotor_vertex_program::VertexProgram* volatile grass_vertex_program_00831e90;
extern volatile kotor_scene3d::U grass_program_guard_00831e94;
// CreateArrays function statics (guard 00831EE4 bits 1/2/4): triangle corners V2 / V1 / V0
extern kotor_scene3d::Vector grass_corner2_00831ec0;
extern kotor_scene3d::Vector grass_corner1_00831ecc;
extern kotor_scene3d::Vector grass_corner0_00831ed8;
extern volatile kotor_scene3d::U grass_corner_guard_00831ee4;
}

// ---------------------------------------------------------------- lowers with no provider (proposed names; owners)
extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::Vector; using kotor_scene3d::Quaternion;
using kotor_scene3d::PointerList;
// [core] WindManager::GetGlobalWindNoPoint 004A5950 thiscall RET 0Ch, EAX = out (004A6AB7 fmul [eax])
Vector* __fastcall wind_manager_get_global_wind_no_point_004a5950(void* wind, void*, Vector* out, const Vector* point, float scale);
// [core] WindManager::GetGlobalPointWind 004A6440 thiscall RET 0Ch, EAX = out (004A6B01)
Vector* __fastcall wind_manager_get_global_point_wind_004a6440(void* wind, void*, Vector* out, const Vector* point, float time);
// [core] glMaterialfv wrapper 0044F880 cdecl(face index, pname index, params) (0073F2EC / 0073F2F8 tables)
void __cdecl gl_material_fv_0044f880(U face_index, U pname_index, const float* params);
// [core] GLRender client texture stage 00425250 cdecl(stage) (glClientActiveTextureARB + [007A6898])
void __cdecl gl_render_activate_client_texture_stage_00425250(U stage);
// [core] GLRender::DrawLightmappedGrass 00426230 cdecl(vertices, 0Ch, attributes, extra stride, first, count)
void __cdecl gl_render_draw_lightmapped_grass_00426230(const void* vertices, U position_stride, const void* attributes,
                                                       I attribute_stride, I first, I count);
// [vp_nv_ati] aurATIFragmentShadersBumpMapAvailable 0045FB90 cdecl, AL
bool __cdecl aur_ati_fragment_shaders_bump_map_available_0045fb90();
// [core] Quaternion::Quaternion(Vector axis, float angle) 004AC960 thiscall RET 10h, EAX = this
Quaternion* __fastcall quaternion_from_axis_angle_004ac960(Quaternion* self, void*, Vector axis, float angle);
// [core] CExoArrayList<CAurTriangleListElem>::CExoArrayList(const&) 004A81B0 thiscall RET 4
PointerList* __fastcall triangle_list_copy_construct_004a81b0(PointerList* self, void*, const PointerList* source);
// [core] CAurTriangleListElem::~CAurTriangleListElem 004A72D0 thiscall RET 0 (eh vector destructor element)
void __fastcall triangle_list_elem_destruct_004a72d0(void* self, void*);
// [core] CExoArrayList<SafePointer<CAurTexture>>::Add(SafePointer by value) 004A8230 thiscall RET 4 (callee destroys)
void __fastcall safe_texture_list_add_004a8230(PointerList* list, void*, kotor_scene3d_bsp::SafeTextureArg value);
// UNASSIGNED: GrassSortTris 004A7690 qsort comparator (pushed at 004A85D5)
int __cdecl grass_sort_tris_004a7690(const void* a, const void* b);
}
