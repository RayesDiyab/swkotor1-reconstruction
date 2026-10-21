#pragma once
// lane-3d-bsp (SUBLANES.json area "bsp_visibility", lane-3d-render/SUBLANES.json, D35): BSP tree, scene BSP
// bookkeeping (rooms / buckets / visibility), tiles and CAurTriangleBin grass bins of swkotor.exe (GOG K1,
// image base 00400000). Includes the canonical 3D header (lane-3d-render scene3d.h, which includes vtables_gen.h).
// The declarations below are PROPOSED for append to scene3d.h (see ../scene3d.h.proposed.diff); until adopted they
// live here.
//
// Conventions (DECISIONS D1 / D1-amendment / D21 / D24 / D30-FINAL / D35 / D42):
//  * original __thiscall bodies = __fastcall(Self*, void* unused_edx, stack...); callers pass nullptr for EDX;
//    no slot used or defined here returns a struct by value and none is variadic (D1-amendment scan: clean).
//  * Scene vtable slot bodies authored here keep EXACTLY the raw-word declarations generated in vtables_gen.h
//    (lane-3d-render owns the typed Scene table, D42): every stack argument is one U, EAX is returned as U.
//  * virtual calls reload the receiver's vtable at every call through typed slot VIEWS (object-pointer view of the
//    table, D24); no function-pointer cast anywhere.
//  * GL entry points: single adapter owner kotor_renderer_init::<import>_<IAT> (glEnable/glDisable: M40
//    aurora-windows-gl-imports; the others: lane-renderer-bodies renderer_gl_imports_gen.h, exact copies below).
//  * D30-FINAL: x87 wide intermediates are `double` (PC_53, RC nearest); `float` only where the original stores with
//    `fstp dword`; operand order of every fadd/fmul chain is kept. x87 compare idioms are written as the exact C++
//    predicate that selects the same branch including the unordered (NaN) case (see the table below).
//  * allocator family: 006FA7E6 operator new -> live_operator_new, 006FA390 free -> live_free (M41);
//    CExoArrayList::Allocate 005E03C0 -> menu_list_growth::allocate_005e03c0 (M41).
//  * Objects without a published layout are byte-offset views (kotor_scene3d::field / at); every offset is cited.
//
// x87 compare idioms (fcom/fcomp ST(0) vs operand b, fnstsw ax):
//   test ah,41h ; je  -> taken iff  a >  b            jne -> taken iff !(a >  b)
//   test ah,41h ; jp  -> taken iff !(a <= b)          jnp -> taken iff   a <= b
//   test ah,05h ; jp  -> taken iff !(a <  b)          jnp -> taken iff   a <  b
//   test ah,01h ; je  -> taken iff   a >= b           jne -> taken iff !(a >= b)
//   test ah,44h ; jp  -> taken iff !(a == b)          jnp -> taken iff   a == b
#include <cstddef>
#include <cstdint>
#include <cstring>
#include "6c81d8874d-scene3d.h"

// ---------------------------------------------------------------- single GL adapter owner (exact copies)
namespace kotor_renderer_init {
using U32 = std::uint32_t; using I32 = std::int32_t; using U8 = std::uint8_t;
extern void __stdcall glColor4f_0073d224(float, float, float, float);
extern void __stdcall glTexParameteri_0073d23c(U32, U32, I32);
extern void __stdcall glBindTexture_0073d240(U32, U32);
extern void __stdcall glEnableClientState_0073d260(U32);
extern void __stdcall glDisableClientState_0073d264(U32);
extern void __stdcall glStencilOp_0073d270(U32, U32, U32);
extern void __stdcall glStencilMask_0073d278(U32);
extern void __stdcall glStencilFunc_0073d298(U32, I32, U32);
extern void __stdcall glTexEnvi_0073d2f4(U32, U32, I32);
extern void __stdcall glColorMask_0073d358(U8, U8, U8, U8);
}

// ---------------------------------------------------------------- lane-3d-model providers (exact declarations copied
// from lane-3d-model/source/model_load.hpp; that header cannot be included together with scene3d.h, see lane-3d-room)
namespace aurora_model { struct MdlNode; }
extern "C" {
aurora_model::MaxTree* __fastcall aurora_tree_as_model_0043e1c0(aurora_model::MaxTree* self, void* unused_edx);
aurora_model::MaxTree* __fastcall aurora_tree_as_animation_0043e1d0(aurora_model::MaxTree* self, void* unused_edx);
aurora_model::I __fastcall aurora_model_del_ref_0043e210(aurora_model::MaxTree* self, void* unused_edx, void* unused);
aurora_model::MdlNode* __fastcall aurora_node_as_emitter_0043e3c0(aurora_model::MdlNode* self, void* unused_edx);
const char* __fastcall aurora_node_translate_name_00438a00(aurora_model::MdlNode* self, void* unused_edx);
}

namespace kotor_scene3d_bsp {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::U8; using kotor_scene3d::U16;
using kotor_scene3d::Vector; using kotor_scene3d::Quaternion; using kotor_scene3d::Plane;
using kotor_scene3d::PlaneList; using kotor_scene3d::PointerList;
using kotor_scene3d::field; using kotor_scene3d::at; using kotor_scene3d::bits_to_float;
using kotor_scene3d::float_to_bits;

// D30-FINAL: W = value loaded onto the x87 stack (exact widening), S = original `fstp dword`.
inline double W(float v) { return static_cast<double>(v); }
inline float S(double v) { return static_cast<float>(v); }
inline double Wb(U bits) { return static_cast<double>(bits_to_float(bits)); }

// ---------------------------------------------------------------- callback types (cdecl, caller cleans)
using BspVisit = void (__cdecl*)(void* node, void* context);                          // 004ACEB5 call ebx (2 args)
using BspVisitSphere = void (__cdecl*)(void* node, const Vector* center, float radius_sq); // 004ACF93 call ebx (3)
using BspSearch = I (__cdecl*)(void* node, void* context);                            // 004AD1AE call [esp+2Ch] (2)
using BspHit = U (__cdecl*)(void* node, Vector start, Vector end, Vector* end_inout);  // 004AD2BF call edi (8 dwords)
using BspVisitPlanes = void (__cdecl*)(void* node, void* context, PlaneList* planes);  // 004AD71A / 004AD80B (3)

// ---------------------------------------------------------------- typed slot views (object view of the tables, D24)
// Part family (Part vtable 00740AE8 and derived tables; slot RETs from the 00740AE8 bodies).
struct PartSlots {
    const void* o00[3];                                                                   // +00..+08
    void (__fastcall* get_world)(void* self, void*, Vector* position, Quaternion* orientation); // +0C 0043E4A0 RET 8
    const void* o10[3];                                                                   // +10..+18
    I (__fastcall* hit_check)(void* self, void*, Vector start, Vector end, Vector* hit, U mask, float scale); // +1C 004429D0 RET 24h
    const void* o20;                                                                      // +20
    void* (__fastcall* as_light)(void* self, void*);                                      // +24 (0046E2FC, 0046C579)
    const void* o28;                                                                      // +28
    void* (__fastcall* as_trimesh)(void* self, void*);                                    // +2C (0046E388, 0046C749)
    const void* o30;                                                                      // +30
    void* (__fastcall* as_emitter)(void* self, void*);                                    // +34 (0046E477, 0046C5EE)
    const void* o38;                                                                      // +38
    void* (__fastcall* query_3c)(void* self, void*);                                      // +3C (0046E3ED, 0046C64E)
    void* (__fastcall* query_40)(void* self, void*);                                      // +40 (0046E444, 0046C690)
    void* (__fastcall* query_44)(void* self, void*);                                      // +44 (0046E462, 0046C6BF)
    void* (__fastcall* query_48)(void* self, void*);                                      // +48 (0046E35A, 0046C6F2)
    const void* o4c[2];                                                                   // +4C..+50
    void* (__fastcall* query_54)(void* self, void*);                                      // +54 (0046E4FB, 0046C7A9)
};
static_assert(offsetof(PartSlots, get_world) == 0x0C && offsetof(PartSlots, hit_check) == 0x1C &&
              offsetof(PartSlots, as_light) == 0x24 && offsetof(PartSlots, as_trimesh) == 0x2C &&
              offsetof(PartSlots, as_emitter) == 0x34 && offsetof(PartSlots, query_3c) == 0x3C &&
              offsetof(PartSlots, query_48) == 0x48 && offsetof(PartSlots, query_54) == 0x54, "Part slots");
// Scalar deleting destructor at +00 of every polymorphic object deleted here (Part / TileData+24 / Gob): RET 4.
struct DeletingSlot { void (__fastcall* destroy)(void* self, void*, U flags); };

// LightManager vtable 00743958 (owner unassigned; slot RETs read from the bodies listed).
struct LightManagerSlots {
    void (__fastcall* prioritize_lights)(void* self, void*, const Vector* position, U flag);      // +00 004A2F30 RET 8
    const void* o04[2];                                                                       // +04..+08
    void (__fastcall* add_light)(void* self, void*, void* light);                             // +0C 004A39F0 RET 4
    void (__fastcall* remove_light)(void* self, void*, void* light, U flag);                  // +10 004A3AE0 RET 8
    const void* o14;                                                                          // +14
    void (__fastcall* reset)(void* self, void*);                                              // +18 004A2E00 RET 0
    void (__fastcall* update_lights)(void* self, void*);                                      // +1C 004A2E10 RET 0
    void (__fastcall* enable_highest_priority_lights)(void* self, void*, U count);            // +20 004A2E70 RET 4
    const void* o24[4];                                                                       // +24..+30
    void (__fastcall* collect_lights_for_given_room)(void* self, void*, void* room);          // +34 0046DE10 RET 4
    void (__fastcall* setup_lights_for_this_gob)(void* self, void*, void* position, U radius_bits); // +38 004A2DB0 RET 8
    void (__fastcall* setup_lights_for_this_room)(void* self, void*, const Vector* position, U radius_bits); // +3C 004A2DE0 RET 8
    void (__fastcall* setup_lights_for_position)(void* self, void*, const Vector* position, U radius_bits);  // +40 004A36B0 RET 8
};
static_assert(offsetof(LightManagerSlots, add_light) == 0x0C && offsetof(LightManagerSlots, reset) == 0x18 &&
              offsetof(LightManagerSlots, collect_lights_for_given_room) == 0x34 &&
              offsetof(LightManagerSlots, setup_lights_for_position) == 0x40, "LightManager slots");

template<class Slots> inline const Slots& slots(void* object) {
    return *static_cast<const Slots*>(*static_cast<void* const volatile*>(object));   // reload [object] per call
}
inline const kotor_scene3d::SceneVTable& scene_slots(void* scene) { return slots<kotor_scene3d::SceneVTable>(scene); }
inline const aurora_model::GobVTable& gob_slots(void* gob) { return slots<aurora_model::GobVTable>(gob); }
inline aurora_model::Gob* as_gob(void* object) { return static_cast<aurora_model::Gob*>(object); }

// ---------------------------------------------------------------- CExoArrayList<T*> helpers
inline void** elements(const PointerList* list) {
    return reinterpret_cast<void**>(static_cast<std::uintptr_t>(list->data));
}
inline PointerList* list_at(void* object, U offset) { return static_cast<PointerList*>(at(object, offset)); }
// Add inlined with the M41 growth owner 005E03C0: `mov eax,[cap]; cmp [size],eax; jne; test eax,eax; je 8;
// add eax,eax; push eax; call 005E03C0` then `[data+size*4]=v; inc [size]`.
inline void add_with_growth(PointerList* list, void* value) {
    const I capacity = list->capacity;
    if (list->size == capacity)
        menu_list_growth::allocate_005e03c0(list, nullptr, static_cast<U>(capacity != 0 ? capacity + capacity : 8));
    elements(list)[list->size] = value;
    list->size = list->size + 1;
}
// Add fully inlined with operator new / element copy (size re-read per iteration) / _free (old buffer when non-null):
// capacity is stored before the allocation in every site of this area (e.g. 0046D008, 0046EA72, 0046D5E0).
inline void add_inline_growth(PointerList* list, void* value) {
    const I capacity = list->capacity;
    if (list->size == capacity) {
        const I grown = capacity != 0 ? capacity + capacity : 8;
        void** previous = elements(list);
        list->capacity = grown;
        void* fresh = live_operator_new(static_cast<U>(grown) << 2);
        list->data = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(fresh));
        for (I index = 0; index < list->size; ++index) elements(list)[index] = previous[index];
        if (previous != nullptr) live_free(previous);
    }
    elements(list)[list->size] = value;
    list->size = list->size + 1;
}
// Inlined CExoArrayList<T*>::Count (same loop as 004AF240): number of elements equal to value.
inline I count_of(const PointerList* list, const void* value) {
    I found = 0;
    const I size = list->size;
    void** data = elements(list);
    for (I index = 0; index < size; ++index)
        if (data[index] == value) ++found;
    return found;
}

// ---------------------------------------------------------------- borrowed original globals shared with other areas
// (Main publishes once; pointers, never copies). Area-exclusive cells are extern "C" globals defined in bsp_cells.cpp.
using GlEnumCell = const volatile U*;
struct BspCells {
    // GL enum cells 0078D448..0078D474 (lane-gui-draw kotor_renderer_cells::enable_*; .data), indexed view for
    // GLRender::BindTexture 00420445 `mov eax,[esi*4+0078D448]` (12 cells).
    const GlEnumCell* gl_enum_table_0078d448;
    const volatile U* gl_texture_2d_0078d458;      // 0x0DE1 (00450AD8)
    const volatile U* gl_lighting_0078d45c;        // 0x0B50 (00450AE1)
    const volatile U* gl_stencil_test_0078d470;    // 0x0B90 (004503A0) = scene3d cells.gl_stencil_test_0078d470
    // texture binding cache (lane-renderer-bodies kotor_renderer_gl_cells / texture_upload_cells)
    volatile U* current_tid_007a683c;              // U[6] glCurrentTID (00420478)
    const volatile I* anisotropy_007a685c;         // 00420459
    const volatile U* current_stage_007a6898;      // 00420473
    // scene-wide (.data)
    const volatile U* gate_0078e614;               // cold 1 (00450AC0; lane-3d-mesh MeshCells::gate_0078e614)
    const volatile U* gate_0078e5a8;               // cold 1 (0046B663; also 0060F1E0)
    volatile U* vertex_programs_reset_007a193c;    // cold 1; ManageSceneBSP writes 1 (0046E5E8); VP/LightManager read
    const volatile float* tile_step_0078e428;      // cold 2.6f (00456E16; also 00452CC0)
    // BSS shared with ProcessParts 0046D3B0 / CollectBins 0046CAE0 / HitCheck 00456B60 / footsteps
    volatile I* bins_collected_007fbf28;           // ClearBuckets = 0 (0046BF93); ProcessParts ++
    volatile I* bins_visited_007fbf2c;             // ClearBuckets = 0 (0046BF99); CollectBins ++
    volatile I* stat_007fbf38;                     // ManageSceneBSP = 0 (0046E85F); ProcessParts ++
    volatile I* stat_007fbf3c;                     // ManageSceneBSP = 0 (0046E865); ProcessParts ++
    volatile I* stat_007fbf44;                     // ManageSceneBSP = 0 (0046E871); ProcessParts ++
    const volatile U* hit_mask_007fbf48;           // HitPartOnBSPNode pushes raw (0046B524); HitCheck 00456B60 writes
    const volatile U* hit_trimeshes_007fbf4c;      // 0046B5A0 gate (footstep / SetPosition writers)
    volatile U* model_7a8a68_copy_007fbf30;        // ClearBuckets = [007A8A68] (0046BF9F)
    const volatile U* models_007a8a68;             // 0046BF88 (lane-3d-model FindModel family BSS)
    // render bucket lists (BSS CExoArrayList {data,size,capacity}; readers in core / emitter / shadow areas)
    PointerList* beam_bucket_007fbf6c;             // size 7FBF70 cleared (0046BF59)
    PointerList* emitter_bucket_007fbf78;          // size 7FBF7C cleared (0046BF12)
    PointerList* static_shadow_bucket_007fbf84;    // size 7FBF88 cleared (0046BF53)
    PointerList* blur_bucket_007fbf90;             // = scene3d cells.deferred_gobs_007fbf90; size 7FBF94
    PointerList* active_bins_007fbf9c;             // 7FBFA0 (0046BBC0 / 0046BF8D); CollectBins adds
    PointerList* fading_out_bucket_007fbfa8;       // size 7FBFAC cleared (0046BF77)
    PointerList* mesh_shadow_bucket_007fbfb4;      // size 7FBFB8 cleared (0046BF5F)
    PointerList* distortion_bucket_007fbfc0;       // 7FBFC0..C8 (004503F1 / 0046EA4A); RenderSinglePass reads size
    PointerList* gob_bucket_007fbfcc;              // 7FBFCC..D4 (0046EB5D); DoGobBuckets reads
    PointerList* fading_in_bucket_007fbfd8;        // size 7FBFDC cleared (0046BF7D)
    PointerList* static_light_bucket_007fbfe4;     // size 7FBFE8 cleared (0046BF6B)
    PointerList* light_saber_bucket_007fbff0;      // 7FBFF0..F8 (0046EADC)
    PointerList* old_active_bins_007fbffc;         // 7FBFFC..C004 (0046BF83 operator=; 0046BBE8)
    PointerList* cap_buckets_007fc008;             // PointerList[5000] (0046BF1A sizes); DoCapBucket reads
    PointerList* bin_bucket_0080aa68;              // size 80AA6C cleared (0046BF71); DoBinBucket reads
    PointerList* trigger_bucket_0080aa74;          // 80AA74..7C (00450AEA / 0046BF65)
    PointerList* fade_buckets_0080aa80;            // PointerList[5000] (0046BF37 sizes); DoFadeBuckets reads
    PointerList* mesh_buckets_008194e0;            // PointerList[5000] = scene3d cells.mesh_buckets_008194e0
    PointerList* part_list_00827f40;               // = scene3d cells.part_list_00827f40; size 827F44 cleared
    // camera snapshot cells shared with core readers
    void* volatile* current_camera_007bb4e8;       // = scene3d cells.current_camera_007bb4e8
    volatile I* gob_bucket_snapshot_007b92ec;      // = scene3d cells.gob_bucket_snapshot_007b92ec (004504DD)
    kotor_scene3d::Callback const volatile* callback_0078e39c; // = scene3d cells.callback_0078e39c (004504E5)
};
extern const BspCells& bsp_cells;
} // namespace kotor_scene3d_bsp

// ---------------------------------------------------------------- cells owned by this area (only bsp_visibility
// functions and the unassigned BSP callbacks listed in PACKET.json reference them). Storage: bsp_cells.cpp.
extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I;
// .data, cold values read from the pinned image; no writer instruction outside this area.
extern volatile U bsp_manage_enabled_0078e5cc;          // cold 1 (0046E5CE)
extern volatile I bsp_forced_room_0078e5d0;             // cold -1 (0046E5F4)
extern volatile U bsp_use_current_room_0078e5c8;        // cold 1 (0046E618 writes 1; 0046D0A6 / 0046E732)
extern volatile U bsp_first_frame_0078e5d4;             // cold 1 (0046ECCF writes 0)
extern volatile float bsp_tile_spacing_0078e424;        // cold 10.0f (00454EFD / 004550EE)
extern volatile float bsp_bin_radius_sq_007a1968;       // cold 900.0f (0046E8B0; also grass 004A6D99)
// BSS (cold 0)
extern volatile I bsp_unlit_lights_007fbf18;            // 0046E94C / 0046E9B8
extern volatile I bsp_stat_007fbf34;                    // 0046E859
extern volatile I bsp_stat_007fbf40;                    // 0046E86B
extern volatile U bsp_hit_plane_007fbf50;               // 0046B66A = 0, 0046B76D = 1
extern volatile U bsp_hit_ground_007fbf54;              // 0046B6D9 gate
extern volatile I bsp_hit_calls_007fbf58;               // 0046B4C3..0046B4D2 ++
extern volatile I bsp_room_index_007fbf68;              // 0046CFAB = room index, 0046D059 = 999
extern volatile U bsp_camera_far_00827f78;              // 0046ECC9 = camera+1E0
extern volatile U bsp_camera_near_00827f7c;             // 0046ECC3 = camera+1DC
extern volatile U bsp_camera_orientation_00827f80[4];   // static Quaternion (guard bit 2), 0046E691..0046E6AF / 0046EC90..
extern volatile U bsp_camera_position_00827f90[3];      // static Vector (guard bit 1), 0046E667..0046E67B / 0046EC70..
extern volatile U bsp_camera_guard_00827f9c;            // static-init guard bits (0046E64D..0046E68C)
}

// ---------------------------------------------------------------- lowers with no provider in the strict link
// (owners per SUBLANES.json in brackets; names reuse an existing lane spelling where one exists)
extern "C" {
using kotor_scene3d::Vector; using kotor_scene3d::Quaternion; using kotor_scene3d::PlaneList;
using kotor_scene3d::PointerList; using kotor_scene3d::Plane;
// [core] 0041FEB0 AurTextureGetMaxTexID: cdecl, EAX consumed (0046BEF1)
I __cdecl aur_texture_get_max_tex_id_0041feb0();
// [core] 00437A90 operator/(Vector*, const Vector*, float): cdecl, EAX = out (0046CEE9 / 004A67E3)
Vector* __cdecl vector_divide_00437a90(Vector* out, const Vector* v, float divisor);
// [core] 00445080 CExoArrayList<T*>::Remove (reverse search, unconditional size-1): thiscall RET 4.
// Donor candidate: task-14 kotor_reconstruction::ArrayMutations::RemoveReverse32 (same algorithm, not in strict link).
void __fastcall exo_array_list_remove_00445080(PointerList* list, void*, void* value);
// [core] 0045D790 CExoArrayList<Plane>::CExoArrayList(int): thiscall RET 4, EAX = this
PlaneList* __fastcall plane_list_construct_0045d790(PlaneList* list, void*, I size);
// [core] 0046B7C0 ConstructCameraFadePlane: original passes the scene in EAX (0046E6B9 mov eax,ebp) - no
// standard convention expresses it; proposed reconstruction convention cdecl(scene) (only caller is 0046E6BB).
void __cdecl construct_camera_fade_plane_0046b7c0(void* scene);
// [core] 0046BC80 CExoArrayList<T*>::operator= (append-copy after size=0): thiscall RET 4, EAX = this
PointerList* __fastcall exo_array_list_assign_0046bc80(PointerList* list, void*, const PointerList* source);
// [core] 004AA610 BoxIntersect(start*, end*, min*, max*): cdecl, EAX tested (0046B520)
I __cdecl box_intersect_004aa610(const Vector* start, const Vector* end, const Vector* minimum, const Vector* maximum);
// [core] 004AAF50 BoxAbovePlane(min*, max*, plane*): cdecl, result in ST0 (0046D132 fcomp)
double __cdecl box_above_plane_004aaf50(const Vector* minimum, const Vector* maximum, const Plane* plane);
// [core] 004AB250 planelineintersection(out*, normal*, d, p0*, p1*): cdecl, EAX = out (0046B755)
Vector* __cdecl plane_line_intersection_004ab250(Vector* out, const Vector* normal, float distance,
                                                 const Vector* start, const Vector* end);
// [core] 004AD5E0 CExoArrayList<Plane>::Add(Plane by value): thiscall RET 10h
void __fastcall plane_list_add_004ad5e0(PlaneList* list, void*, Plane plane);
// [core] 004AF240 CExoArrayList<T*>::Count: thiscall RET 4 (EAX tested)
I __fastcall exo_array_list_count_004af240(const PointerList* list, void*, void* value);
// [core] 0046E240 SafePointer<WindManager>::operator=: thiscall RET 4
void* __fastcall wind_safe_pointer_assign_0046e240(void* safe_pointer, void*, void* wind);
// [lights] 0046CC00 AddLightToRooms / 0046DDA0 DoStaticLightBuckets (lane-3d-construct spellings)
void __cdecl add_light_to_rooms_0046cc00(void* area, void* light);
void __cdecl do_static_light_buckets_0046dda0(void* scene);
// [part_emitter] 0046CD70 AddEmitterToRooms(scene, emitter): cdecl
void __cdecl add_emitter_to_rooms_0046cd70(void* scene, void* emitter);
// [part_emitter] 0048D780 MdlNodeEmitter::IsAffectedByWind: thiscall RET 0, AL
bool __fastcall emitter_node_is_affected_by_wind_0048d780(aurora_model::MdlNode* node, void*);
// [part_emitter] 004A6470 WindManager::RegisterWindyEmitter: thiscall RET 4
void __fastcall wind_manager_register_windy_emitter_004a6470(void* wind, void*, void* emitter);
// UNASSIGNED (BSP family callbacks referenced only by address immediates; not in SUBLANES.json, see PACKET open_gates)
I __cdecl node_has_aabb_0046a660(void* node, void* part);           // pushed at 0046C704
I __cdecl node_has_light_0046b340(void* node, void* part);          // pushed at 0046C58B
I __cdecl node_has_emitter_0046b370(void* node, void* part);        // pushed at 0046C600
I __cdecl node_has_projection_0046b3a0(void* node, void* part);     // pushed at 0046C664
I __cdecl node_has_trimesh_0046b400(void* node, void* part);        // pushed at 0046C75B
I __cdecl node_has_trigger_0046b430(void* node, void* part);        // pushed at 0046C7BB
I __cdecl node_has_other_0046b460(void* node, void* part);          // pushed at 0046C802
I __cdecl node_has_gob_0046b490(void* node, void* gob);             // pushed at 0046C4B5
void __cdecl collect_bins_0046cae0(void* node, const Vector* center, float radius_sq); // pushed at 0046E8BF
void __cdecl process_parts_0046d3b0(void* node, void* scene, PlaneList* planes);       // pushed at 0046E8F5 / 0046E91E
}

// ---------------------------------------------------------------- authored in this area
extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I;
// Scene vtable slots: declared by vtables_gen.h (raw-word contract) and DEFINED here:
//   scene_spawn_tile_00454c80 (+24), scene_set_tile_flames_0044fc20 (+2C), scene_delete_tile_0044fca0 (+34),
//   scene_set_current_room_00452c90 (+7C), scene_do_blur_buckets_004504d0 (+DC),
//   scene_do_distortion_buckets_004503a0 (+E4), scene_do_trigger_bucket_00450ac0 (+114),
//   scene_set_single_tile_flame_00455370 (+11C).
// GLRender
void __cdecl gl_render_bind_texture_00420440(U target_index, U texture);
// TileData
void __fastcall tile_data_destruct_00444e20(void* tile, void*);                                    // RET 0
void* __fastcall tile_data_construct_004473b0(void* tile, void*);                                  // RET 0, EAX = this
bool __cdecl process_tile_part_004528b0(void* tile, void* part, const char* name);                // AL
void* __cdecl spawn_parts_for_tile_00454a20(const char* name, const Vector* position, const Quaternion* orientation,
                                            I texture_rotate, I instance, PointerList* parts);     // EAX = Gob or 0
// CExoArrayList<CExoArrayList<CAurTriangleBin*>>
void __fastcall bin_lists_allocate_004555b0(PointerList* list, void*, I size);                    // RET 4
void __fastcall bin_lists_set_size_00456a00(PointerList* list, void*, I size);                    // RET 4
void __cdecl init_grass_bins_00456e00(Vector minimum, Vector maximum, PointerList* bins);          // (lane-3d-room spelling)
// Scene BSP
void* __cdecl scene_bsp_get_tile_data_0046a680(void* scene, const Vector* point);
U __cdecl hit_part_on_bsp_node_0046b4c0(void* node, Vector start, Vector end, Vector* hit);
void __cdecl aur_process_active_bins_0046bbc0();
void __cdecl clear_scene_rooms_0046bc40(void* scene);
void __fastcall nodedata_add_object_0046bd70(void* nodedata, void*, void* gob);                    // RET 4
void __cdecl clear_buckets_0046bef0(void* scene);                                                 // argument unused
void* __fastcall nodedata_construct_0046bfb0(void* self, void* unused_edx);                        // (lane-3d-room spelling)
void __cdecl scene_bsp_add_gob_0046c420(void* scene, void* gob);
void __cdecl scene_bsp_remove_gob_0046c4a0(void* scene, void* gob);
void __cdecl scene_bsp_remove_part_0046c510(void* scene, void* part);
void __cdecl add_object_to_rooms_0046ce70(void* scene, void* gob);
void __cdecl collect_active_rooms_0046d070(void* scene, PointerList* rooms);
void __cdecl scene_bsp_remove_tiledata_0046d2f0(void* scene, const Vector* point);
I __cdecl scene_bsp_add_part_0046e290(void* scene, void* part, aurora_model::MaxTree* model);     // EAX 0/1
// manage_scene_bsp_0046e5c0(void* scene): declared by scene3d.h, DEFINED here
void __cdecl scene_bsp_add_tile_data_0046ed00(void* scene, void* tile, Vector point);
// CAurTriangleBin (grass bins)
void __fastcall triangle_bin_finish_geometry_004a67b0(void* bin, void* unused_edx);               // (lane-3d-room spelling)
void __fastcall triangle_bin_update_grass_for_wind_004a68b0(void* bin, void*, void* wind);        // RET 4
// triangle_bin_render_grass_polys_004a6d30: declared by scene3d.h, DEFINED here (RET 0Ch)
void __fastcall triangle_bin_destroy_grass_polys_004a8460(void* bin, void*);                      // RET 0
void __fastcall triangle_bin_sort_triangles_004a8500(void* bin, void*);                           // RET 0
void __fastcall triangle_bin_create_arrays_004a87c0(void* bin, void*);                            // RET 0
void __fastcall triangle_bin_build_grass_polys_004a9630(void* bin, void*);                        // RET 0 (tail jmp)
// BSP tree
void __cdecl bsp_traverse_004acea0(void* node, void (__cdecl* fn)(void* node, void* context), void* context); // (lane-3d-construct spelling)
void __cdecl bsp_traverse_sphere_004acf60(void* node, const Vector* center, float radius_sq,
                                          kotor_scene3d_bsp::BspVisitSphere fn);
U __cdecl rface_split_test_004acff0(const PointerList* face, const Plane* plane);                // CExoArrayList<Vector>*
void* __cdecl bsp_fit_sphere_004ad060(void* node, const Vector* center, float radius);
void* __cdecl bsp_fit_box_004ad0d0(void* node, const Vector* minimum, const Vector* maximum);
void* __cdecl bsp_search_004ad1a0(void* node, Vector point, kotor_scene3d_bsp::BspSearch fn, void* context);
U __cdecl bsp_hit_check_004ad260(void* node, Vector start, Vector end, Vector* hit, kotor_scene3d_bsp::BspHit fn);
void __cdecl bsp_sphere_check_004ad560(void* bsp, const void* center, float radius, kotor_reconstruction::ArrayMutations* out); // (lane-3d-construct spelling)
void* __fastcall bsp_node_construct_004ad630(void* self, void* unused_edx, Vector v, I flag);       // (lane-3d-room spelling), RET 10h
void __cdecl bsp_traverse_point_004ad6d0(void* node, Vector point, kotor_scene3d_bsp::BspVisitPlanes fn, void* context);
void __cdecl bsp_traverse_volume_approx_004ad7e0(void* node, const Vector* point, PlaneList* planes,
                                                 kotor_scene3d_bsp::BspVisitPlanes fn, void* context);
}
