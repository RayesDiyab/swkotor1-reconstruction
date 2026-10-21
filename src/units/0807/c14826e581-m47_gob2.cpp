// lane-m47-3d fork gob2: Gob-tree / camera / tile callees left unresolved by lane-m46-3dcore (swkotor.exe GOG K1,
// image base 00400000). Every statement cites the original instruction(s) (d/<addr>.txt, local disassembly only).
//
// Consumer ABIs (M46 stage, read-only): 3d/core46/m46_gob_externs.h (gob-tree/-small/-hit), m46_camera_externs.h
// (camera), m46_tile.cpp:64 (tile). This unit does NOT include m46_gob_externs.h because the CAurObject table
// 00741108 is defined here with its typed (D21) type; extern "C" names decorate only by name/convention/stack bytes,
// so the definitions below link to the consumers' declarations (typed-declaration header diff:
// parts/gob2-m46_gob_externs.h.diff).
//
// Dispatch (D21/D41): Gob slots through aurora_model::GobVTable (vtables_gen.h), Part slots through
// aurora_vt::VTable_00740ae8 (model_vtables.hpp; +2C Slot0 / +5C Slot4 are typed identically in every Part-family
// table), behavior tables through kotor_scene3d_construct::BehaviorBaseSlots (construct3d.h).
// D30-FINAL: x87 values kept wide (double) and rounded to float only at the original `fstp dword` stores; fsqrt is
// IEEE sqrt (exact under PC_53). Allocator family: 006FA7E6/006FC0D2 -> live_operator_new, 006FA390/006FC0CD ->
// live_free (M41). Unwind: the EH frames of 004487C0 / 0049FE80 guard only callees that cannot throw (no STL, no
// throwing new: live_operator_new returns NULL) - cleanup not emitted (D30-FINAL /EHsc note).
#include <cmath>
#include <cstdint>
#include <cstring>
#include "6c81d8874d-scene3d.h"                 // kotor_scene3d types/helpers, GL/quaternion lowers, GobVTable (vtables_gen.h)
#include "7ee14b5ec3-model_vtables.hpp"         // aurora_vt::VTable_00740ae8 (Part)
#include "516b075ab1-construct3d.h"             // BehaviorBaseSlots, behavior_* providers
#include "cb0b2681a6-m46_cells_storage.h"       // scene3d_transform_enabled_0078e5f0, scene3d_gob_bucket_007fbfcc
#include "e04ccf8d50-m46_tile_decls.h"          // project_shadow_gate_007b9374
#include "647ed12135-m46_camera_externs.h"      // texture_list_remove_00445080, assign_00447fa0, behavior_camera_follow_construct
#include "7a7bf89a27-m47_aurora_object_vtable.h" // m47_gob2::AuroraObjectVTable (typed table 00741108)

namespace m47_gob2 {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::U8;
using kotor_scene3d::Vector; using kotor_scene3d::Quaternion; using kotor_scene3d::Plane;
using kotor_scene3d::PointerList; using kotor_scene3d::field; using kotor_scene3d::at;
using kotor_scene3d::bits_to_float; using kotor_scene3d::float_to_bits;
inline double W(float v) { return static_cast<double>(v); }
inline float S(double v) { return static_cast<float>(v); }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
template <class T> inline T* ptr(U w) { return reinterpret_cast<T*>(static_cast<std::uintptr_t>(w)); }
inline const aurora_vt::VTable_00740ae8* part_vt(void* part) {
    return *static_cast<const aurora_vt::VTable_00740ae8* const volatile*>(part);
}
inline const aurora_model::GobVTable* gob_vt(void* object) {
    return *static_cast<const aurora_model::GobVTable* const volatile*>(object);
}
constexpr U kOne = 0x3F800000u;   // .rdata 0073D764 = 1.0f; immediate 3F800000h (004747EC / 00474902 / 0049FF15)
constexpr double kZero = 0.0;     // .rdata 0073D700 = 0.0f
constexpr double kEpsilon = 1e-9; // .rdata 007440F0 qword 3E112E0BE826D695
}

extern "C" {
// ---- existing providers whose declaring headers are not on this unit's include path (identical declarations)
float* __cdecl kotor_cdecl_00423bc0(float* out, const float* a, const float* b);   // r4-lowers r4_lowers.h:85 (out = a - b)
kotor_scene3d::Quaternion* __cdecl quaternion_scalar_multiply_004a9b80(kotor_scene3d::Quaternion* out,
    const kotor_scene3d::Quaternion* q, float s);                                  // anim3d.h:209 (3d-anim-anim-contested)
void __cdecl packed_purecall_006fb8de(void);                                       // packed_file.h:84 (_purecall 006FB8DE)
}
// aurora-windows-gl-imports (renderer_gl_imports_gen.h:53/54/81, exact copies; that header needs roots this unit
// does not have - same precedent as bsp.h:36).
namespace kotor_renderer_init {
extern void __stdcall glPopMatrix_0073d2fc();
extern void __stdcall glPushMatrix_0073d300();
extern void __stdcall glScalef_0073d374(float, float, float);
}
namespace r4_lowers_cells { extern std::uint32_t volatile render_counts_00827fa8[6]; }   // r4-lowers r4_cells.cpp (00827FA8..BC)

using namespace m47_gob2;

extern "C" {
// =====================================================================================================================
// SafePointerList<CAurObject>::~SafePointerList 004487C0: __thiscall, RET 0 (0044888C). List {data +0, count +4,
// capacity +8} of SafePointer* (each {CAurObject* target}); the target keeps its back-reference list at target+4
// {data +4, count +8}. Walks the entries from last to first (count read once, 004487DE), unlinks each SafePointer
// from its target's list and nulls it, then frees the array.
void __fastcall safe_pointer_list_destruct_004487c0(void* list, void* /*unused_edx*/) {
    for (I index = field<I>(list, 4) - 1; index >= 0; --index) {          // 004487DE..004487EA dec ebp; js / 00448837 dec; jns
        void* const safe = ptr<void*>(field<U>(list, 0))[index];         // 004487F0..004487F2 (data re-read per entry)
        void* const target = *static_cast<void* volatile*>(safe);        // 004487F5 mov ecx,[edi]
        if (target != nullptr) {                                          // 004487F7 test ecx,ecx
            const I last = field<I>(target, 8) - 1;                        // 004487FB..004487FE esi = count-1
            I found = last;                                                // 00448801 mov eax,esi
            if (found >= 0) {                                              // 00448803..00448805 jl
                // 00448807..0044881A: search downwards; leaves -1 when the SafePointer is not registered.
                while (ptr<void*>(field<U>(target, 4))[found] != safe) {
                    --found;
                    if (found < 0) break;
                }
            }
            field<I>(target, 8) = last;                                    // 0044881E count = count-1 (unconditional)
            // 00448821..00448835: shift down from `found` (data and count re-read each step). With found == -1 the
            // original writes data[-1] (reachable only if the back-reference invariant is already broken).
            for (I k = found; k < field<I>(target, 8); ++k) {
                void** const data = ptr<void*>(field<U>(target, 4));
                data[k] = data[k + 1];
            }
        }
        *static_cast<void* volatile*>(safe) = nullptr;                    // 00448838 mov [edi],0
    }
    if (field<U>(list, 0) != 0) {                                          // 00448842..00448846
        live_free(ptr<void>(field<U>(list, 0)));                           // 00448848..0044884E
        field<U>(list, 0) = 0;                                             // 00448851
    }
    field<U>(list, 8) = 0;                                                 // 0044885C
    field<U>(list, 4) = 0;                                                 // 00448863
    if (field<U>(list, 0) != 0) live_free(ptr<void>(field<U>(list, 0)));   // 00448857/00448873..0044887B (re-read; always 0)
}

// CAurObject::~CAurObject 004488C0 (authored callee, new name): mov [ecx],741108h; add ecx,4; jmp 004487C0.
void __fastcall aurora_object_destruct_004488c0(void* self, void* /*unused_edx*/);
// CAurObject scalar deleting destructor 004488D0 (slot +00 of 00741108): __thiscall RET 4, EAX = this.
void* __fastcall aurora_object_scalar_delete_004488d0(void* self, void* /*unused_edx*/, U flags) {
    aurora_object_destruct_004488c0(self, nullptr);                        // 004488D1..004488D3
    if ((flags & 1u) != 0) live_free(self);                                // 004488D8..004488E5 test byte [esp+8],1
    return self;                                                           // 004488E8 mov eax,esi
}
}

// =====================================================================================================================
// CAurObject vtable 00741108 (88 slots, .rdata 00741108..00741267): +00 = 004488D0, +04..+15C = _purecall 006FB8DE
// (read from the pinned image: d0884400 followed by 87 x deb86f00). D21/D24: a typed constant table; each purecall
// slot is a typed function of the slot's own type (the slot types of the derived Gob table 00741268, whose first 88
// slots override exactly these CAurObject virtuals) whose body is the original _purecall (push 19h; call _amsg_exit,
// never returns). The value after the call is unreachable (D2-style label: no original value exists).
namespace m47_gob2 {
template <class F> struct PureCall;
template <class R, class... A> struct PureCall<R (__fastcall*)(A...)> {
    static R __fastcall slot(A...) {
        packed_purecall_006fb8de();                                        // 006FB8DE (does not return)
        return R();                                                        // D2: unreachable (no original EAX/ST0)
    }
};
}
extern "C" const m47_gob2::AuroraObjectVTable aurora_object_vtable_00741108 = {
    &aurora_object_scalar_delete_004488d0,   // +000 004488D0
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s004_gob_return_zero)>::slot,   // +004
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s008_gob_get_name)>::slot,   // +008
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s00c_gob_get_model_name)>::slot,   // +00C
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s010_gob_render)>::slot,   // +010
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::animate)>::slot,   // +014
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s018_gob_play_animation)>::slot,   // +018
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s01c_gob_get_animation)>::slot,   // +01C
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s020_gob_load_add_in_animations)>::slot,   // +020
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s024_gob_remove_add_in_animations)>::slot,   // +024
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::register_callback)>::slot,   // +028
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s02c_gob_deregister_callback)>::slot,   // +02C
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s030_gob_replace_texture)>::slot,   // +030
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s034_gob_replace_texture)>::slot,   // +034
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s038_gob_replace_texture_subtree)>::slot,   // +038
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s03c_gob_restore_texture)>::slot,   // +03C
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s040_gob_set_can_down_sample)>::slot,   // +040
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s044_gob_set_texture_attributes)>::slot,   // +044
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s048_gob_set_environment_map)>::slot,   // +048
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::attach_to_scene)>::slot,   // +04C
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s050_gob_attach_to_object)>::slot,   // +050
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s054_gob_attach_to_part)>::slot,   // +054
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s058_gob_set_behavior)>::slot,   // +058
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s05c_gob_set_position)>::slot,   // +05C
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s060_gob_set_orientation)>::slot,   // +060
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s064_gob_get_position)>::slot,   // +064
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s068_gob_get_orientation)>::slot,   // +068
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s06c_gob_get_previous_position)>::slot,   // +06C
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s070_gob_get_external_data)>::slot,   // +070
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s074_gob_set_external_data)>::slot,   // +074
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s078_gob_get_external_data_type)>::slot,   // +078
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s07c_gob_set_external_data_type)>::slot,   // +07C
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s080_gob_inherit_external)>::slot,   // +080
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s084_gob_hit_check_hack)>::slot,   // +084
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s088_gob_hit_check_part)>::slot,   // +088
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s08c_gob_hit_check)>::slot,   // +08C
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s090_gob_set_ignore_hit_check)>::slot,   // +090
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s094_gob_set_part_position)>::slot,   // +094
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s098_gob_get_part_position)>::slot,   // +098
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s09c_gob_set_part_local_position)>::slot,   // +09C
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0a0_gob_get_part_local_position)>::slot,   // +0A0
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0a4_gob_reattach_reference)>::slot,   // +0A4
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0a8_gob_behavior_message)>::slot,   // +0A8
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0ac_gob_append)>::slot,   // +0AC
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0b0_gob_amputate)>::slot,   // +0B0
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0b4_gob_set_color_shifting)>::slot,   // +0B4
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0b8_gob_set_illumination)>::slot,   // +0B8
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0bc_gob_turn_off_shadows)>::slot,   // +0BC
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0c0_gob_turn_on_shadows)>::slot,   // +0C0
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0c4_gob_set_light_priority)>::slot,   // +0C4
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0c8_gob_get_maximum_light_radius)>::slot,   // +0C8
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0cc_gob_set_object_scale)>::slot,   // +0CC
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0d0_gob_enable_render_gob_bbox)>::slot,   // +0D0
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0d4_gob_disable_render_gob_bbox)>::slot,   // +0D4
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0d8_gob_set_wind)>::slot,   // +0D8
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0dc_gob_enable_force_animations)>::slot,   // +0DC
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0e0_gob_disable_force_animations)>::slot,   // +0E0
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0e4_gob_disable_fog)>::slot,   // +0E4
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0e8_gob_enable_fog)>::slot,   // +0E8
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0ec_gob_get_bounding_box)>::slot,   // +0EC
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0f0_gob_get_minimum_bounding_box)>::slot,   // +0F0
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0f4_gob_get_minimum_bounding_sphere)>::slot,   // +0F4
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0f8_gob_enable_distortion)>::slot,   // +0F8
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s0fc_gob_disable_distortion)>::slot,   // +0FC
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s100_gob_set_as_character)>::slot,   // +100
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s104_gob_get_property_0x68)>::slot,   // +104
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s108_gob_get_behavior)>::slot,   // +108
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s10c_gob_get_part)>::slot,   // +10C
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s110_gob_enum_parts)>::slot,   // +110
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s114_gob_set_render_personal_space)>::slot,   // +114
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s118_gob_begin_look_at_animate)>::slot,   // +118
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s11c_gob_end_look_at_animate)>::slot,   // +11C
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s120_gob_play_out_of_order_animation)>::slot,   // +120
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s124_gob_stop_out_of_order_animation)>::slot,   // +124
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s128_gob_add_animation_flag)>::slot,   // +128
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s12c_gob_remove_animation_flag)>::slot,   // +12C
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s130_gob_enable_always_render)>::slot,   // +130
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s134_gob_disable_always_render)>::slot,   // +134
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s138_gob_enable_in_cutscene)>::slot,   // +138
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s13c_gob_disable_in_cutscene)>::slot,   // +13C
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s140_gob_get_cutscene_dummy_position)>::slot,   // +140
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s144_gob_set_motion_blurred)>::slot,   // +144
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s148_gob_get_motion_blurred)>::slot,   // +148
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s14c_gob_render_blur)>::slot,   // +14C
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s150_gob_enable_render_bumped_out)>::slot,   // +150
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s154_gob_disable_render_bumped_out)>::slot,   // +154
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s158_gob_set_max_blur_length)>::slot,   // +158
    &m47_gob2::PureCall<decltype(aurora_model::GobVTable::s15c_gob_get_max_blur_length)>::slot,   // +15C
};

extern "C" {
// CAurObject::~CAurObject 004488C0: __thiscall RET 0; re-points the object at the CAurObject table, then destroys
// the SafePointerList member at +4 (tail jump).
void __fastcall aurora_object_destruct_004488c0(void* self, void* /*unused_edx*/) {
    field<const void*>(self, 0) = &aurora_object_vtable_00741108;       // 004488C0 mov dword ptr [ecx],741108h
    safe_pointer_list_destruct_004487c0(at(self, 4), nullptr);            // 004488C6..004488C9 add ecx,4; jmp 004487C0
}

// =====================================================================================================================
// Scene::RemoveFromSceneManager 00455540: ECX unused (Ghidra __thiscall), cdecl gob at [esp+4], RET 0. Counts the
// occurrences of gob in the scene-manager gob bucket 007FBFCC {data, size 007FBFD0}; removes one when present.
void __cdecl scene_remove_from_scene_manager_00455540(void* gob) {
    const I size = scene3d_gob_bucket_007fbfcc.size;                       // 00455541 mov esi,[007FBFD0]
    I occurrences = 0;                                                     // 00455548 xor edi,edi
    if (size > 0) {                                                        // 0045554C..0045554E
        void* const* const data = ptr<void*>(scene3d_gob_bucket_007fbfcc.data);   // 00455554 (read once)
        for (I index = 0; index < size; ++index)                           // 00455560..00455569
            if (data[index] == gob) ++occurrences;                         // 00455560..00455565
        if (occurrences != 0)                                              // 0045556B..0045556D
            texture_list_remove_00445080(&scene3d_gob_bucket_007fbfcc, nullptr, gob);   // 0045556F..00455575 (ECX = 007FBFCC)
    }
}

// =====================================================================================================================
// ImposterManager object 0078DB30 (.data: data 0, count 2; the image's only reference is 0043216B). No owner in M46
// or any lane: storage defined here. {data, count}; records of 1Ch bytes whose byte +00 is the "in use" flag.
struct ImposterManager_0078db30 { U data; I count; };
ImposterManager_0078db30 imposter_manager_0078db30 = {0u, 2};             // cold words 00000000 00000002

// ImposterManager::AllocateImposter 00425AF0: __thiscall(index, a, b) RET 0Ch (004320F1 / 00432149). NEW external
// (pbuffer/GL render-to-texture body, not authored by this fork).
void __fastcall imposter_manager_allocate_imposter_00425af0(ImposterManager_0078db30* manager, void* unused_edx,
                                                             I index, U a, U b);

// ImposterManager::AllocateImposterRecord 00432090 (authored callee): __thiscall(a, b) RET 8, EAX = record index.
I __fastcall imposter_manager_allocate_imposter_record_00432090(ImposterManager_0078db30* manager, void* /*edx*/,
                                                                 U a, U b) {
    const I count = manager->count;                                        // 00432094 mov ecx,[ebx+4]
    I index = 0;                                                           // 00432098 xor ebp,ebp
    if (count > 0) {                                                       // 0043209A..0043209C
        // 0043209E..004320AE: first free record (byte +00 == 0). With the cold image (data 0, count 2) the
        // original reads byte [0] here (access violation); reproduced through the stored word.
        const volatile U8* record = ptr<const volatile U8>(manager->data);
        do {
            if (*record == 0) {                                            // 004320A0 cmp byte ptr [eax],0; je 004320E4
                imposter_manager_allocate_imposter_00425af0(manager, nullptr, index, a, b); // 004320E4..004320F1
                return index;                                              // 004320F6 mov eax,ebp; ret 8
            }
            ++index;                                                       // 004320A8 inc ebp
            record += 0x1C;                                                // 004320A9 add eax,1Ch
        } while (index < manager->count);                                  // 004320A5 / 004320AC (count re-read)
    }
    // 004320B0..004320DE: grow to 2*count records (count from 00432094), clearing each new record's flag byte.
    const U old_data = manager->data;                                      // 004320B0 / 004320BD (saved in the frame)
    const I doubled = count + count;                                       // 004320B3 lea esi,[ecx+ecx]
    void* const block = live_operator_new(static_cast<U>(doubled * 0x1C)); // 004320B6..004320C6 (006FC0D2 -> operator new)
    volatile U8* fresh = nullptr;                                          // 004320FE xor edi,edi (allocation failed)
    if (block != nullptr) {                                                // 004320C9..004320CB
        volatile U8* const records = static_cast<volatile U8*>(block);
        if (doubled - 1 >= 0)                                              // 004320CD..004320D4 lea edx,[esi-1]; test; jl
            for (I k = 0; k < doubled; ++k) records[k * 0x1C] = 0;         // 004320D6..004320DE mov byte ptr [ecx],0
        fresh = records;                                                   // 004320E0 mov edi,eax
    }
    // 00432100..00432115: data = new block, then zero (count*2) BYTES from its start (rep stosd + rep stosb; the
    // original length is count*2, not count*2*1Ch), through the stored pointer (a NULL block faults as in the
    // original when count*2 != 0).
    const U clear = static_cast<U>(manager->count) << 1;                   // 00432100..00432103 shl ecx,1
    manager->data = word(const_cast<U8*>(fresh));                          // 0043210A mov [ebx],edi
    for (U k = 0; k < clear; ++k) fresh[k] = 0;                            // 0043210E / 00432115 rep stos
    // 00432117..00432132: copy count*1Ch bytes of the old records to the new block (rep movsd + rep movsb).
    const U copy = static_cast<U>(manager->count) * 0x1Cu;                 // 00432117..0043211E imul ecx,ecx,1Ch
    volatile U8* const to = ptr<volatile U8>(manager->data);               // 00432121 mov edi,[ebx]
    const volatile U8* const from = ptr<const volatile U8>(old_data);      // 0043211A..00432128 mov esi,eax
    for (U k = 0; k < copy; ++k) to[k] = from[k];
    live_free(ptr<void>(old_data));                                        // 00432131..00432134 (006FC0CD -> free)
    imposter_manager_allocate_imposter_00425af0(manager, nullptr, index, a, b); // 00432139..00432149
    const I grown = static_cast<I>(static_cast<U>(manager->count) << 1);   // 0043214E..00432153 shl eax,1
    manager->count = grown;                                                // 00432155 mov [ebx+4],eax
    return grown >> 1;                                                     // 00432159 sar eax,1
}

// GLRender::SetUpImposters 00432160: __cdecl(a, b), EAX = 00432090's result (consumer 004404C0 stores it).
U __cdecl gl_render_set_up_imposters_00432160(U a, U b) {
    return static_cast<U>(imposter_manager_allocate_imposter_record_00432090(&imposter_manager_0078db30, nullptr,
                                                                              a, b));   // 00432160..00432174 (ECX = 0078DB30)
}

// =====================================================================================================================
// normalize(Quaternion*, Quaternion) 004A9D60: __cdecl, EAX = out. Layout (w,x,y,z) (scene3d.h).
kotor_scene3d::Quaternion* __cdecl quaternion_normalize_004a9d60(kotor_scene3d::Quaternion* out,
                                                                 kotor_scene3d::Quaternion q) {
    // 004A9D60..004A9D86: ((z*z + y*y) + x*x) + w*w, fsqrt (all on the x87 stack, PC_53 -> double).
    const double length = std::sqrt(((W(q.z) * W(q.z) + W(q.y) * W(q.y)) + W(q.x) * W(q.x)) + W(q.w) * W(q.w));
    if (!(length != kZero)) {                                              // 004A9D88..004A9D93 fcom 0.0; test ah,44h; jp
        // 004A9D95..004A9DD8: identity written to the argument copy then to *out (dword copies).
        out->w = bits_to_float(kOne); out->x = 0.0f; out->y = 0.0f; out->z = 0.0f;
        return out;                                                        // 004A9D95 mov eax,[esp+4]
    }
    const float scale = S(W(bits_to_float(kOne)) / length);               // 004A9DD9..004A9DEA fld 1.0; fdiv; fstp dword
    quaternion_scalar_multiply_004a9b80(out, &q, scale);                   // 004A9DED..004A9DF6 (out, &argument copy, s)
    return out;                                                            // 004A9DF9 mov eax,ecx (004A9B80 keeps ECX)
}

// Vector::isZero 0046A620 (authored callee, new name): __thiscall RET 0, EAX 1 iff x, y and z compare equal to 0.0.
U __fastcall vector_is_zero_0046a620(const kotor_scene3d::Vector* v, void* /*unused_edx*/) {
    if (W(v->x) != kZero) return 0;                                        // 0046A620..0046A62D (jp: not equal/unordered)
    if (W(v->y) != kZero) return 0;                                        // 0046A62F..0046A63D
    if (W(v->z) != kZero) return 0;                                        // 0046A63F..0046A64D
    return 1;                                                              // 0046A64F mov eax,1
}

// SphereIntersect(Vector* start, Vector* end, Vector* center, float radius) 004ABDD0: __cdecl, EAX 0/1.
// Consumer (m46_gob_externs.h:63) passes the radius as its raw DWORD.
I __cdecl sphere_intersect_004abdd0(const kotor_scene3d::Vector* start, const kotor_scene3d::Vector* end,
                                    const kotor_scene3d::Vector* center, U radius_bits) {
    const float dx = S(W(end->x) - W(start->x));                           // 004ABDDB..004ABDDF fstp [esp]
    const float dy = S(W(end->y) - W(start->y));                           // 004ABDE3..004ABDE9 fstp [esp+4]
    const double dz = W(end->z) - W(start->z);                             // 004ABDED..004ABDF0 (kept on the stack)
    const double a = (W(dx) * W(dx) + dz * dz) + W(dy) * W(dy);            // 004ABDF3..004ABE09
    if (a < kEpsilon) return 0;                                            // 004ABE0B..004ABE21 fcom 1e-9; test ah,5; jp
    // 004ABE22..004ABE48: t = ((cz*dz + cy*dy) + cx*dx) / a, c* = center - start (wide).
    const double cx = W(center->x) - W(start->x);
    const double cy = W(center->y) - W(start->y);
    const double cz = W(center->z) - W(start->z);
    double t = ((cz * dz + cy * W(dy)) + cx * W(dx)) / a;
    if (kZero > t) t = kZero;                                              // 004ABE4A..004ABE57 / 004ABE72..004ABE74
    else if (W(bits_to_float(kOne)) < t) t = W(bits_to_float(kOne));       // 004ABE59..004ABE6A (unordered keeps t)
    const float px = S(W(dx) * t);                                         // 004ABE7A..004ABE85 fstp [esp+0Ch]
    const float py = S(W(dy) * t);                                         // 004ABE89..004ABE8F fstp [esp+10h]
    const double pz = dz * t;                                              // 004ABE93..004ABE99 (wide)
    const double hx = W(px) + W(start->x);                                 // 004ABE9B..004ABE9F (wide)
    const float hy = S(W(py) + W(start->y));                               // 004ABEA1..004ABEA8 fstp
    const float hz = S(pz + W(start->z));                                  // 004ABEAC..004ABEB1 fstp
    kotor_scene3d::Vector delta;
    delta.x = S(hx - W(center->x));                                        // 004ABEB5..004ABEB7
    delta.y = S(W(hy) - W(center->y));                                     // 004ABEBB..004ABEC2
    delta.z = S(W(hz) - W(center->z));                                     // 004ABEC6..004ABECD
    const double distance = vector_magnitude_004ab100(&delta);             // 004ABED1 (ST0)
    if (!(distance < W(bits_to_float(radius_bits)))) return 0;             // 004ABED6..004ABEE2 fcomp radius; test ah,5; jp
    return 1;                                                              // 004ABEE8 mov eax,1
}

// =====================================================================================================================
// Gob::PartVolumeProject(Part*, Vector, float, Plane) 00474710: __thiscall RET 24h (00474C85); consumer
// m46_tile.cpp:64. Frame cells: position (arg, overwritten in place at 004747BA..004747DA / 00474883..004748AE),
// P1 [esp+2C] (local plane), per child: lpos [esp+10], Pc [esp+40], P2 [esp+1C].
void __fastcall gob_part_volume_project_00474710(void* gob, void* /*unused_edx*/, void* part,
                                                 kotor_scene3d::Vector position, float range,
                                                 kotor_scene3d::Plane plane) {
    using kotor_renderer_init::glPushMatrix_0073d300;
    using kotor_renderer_init::glPopMatrix_0073d2fc;
    using kotor_renderer_init::glScalef_0073d374;
    if (field<U8>(part, 0x2C) == 0) return;                                // 0047471F..0047472C je 00474C7B
    glPushMatrix_0073d300();                                               // 00474732 call [0073D300]
    if (part == field<void*>(gob, 0x6C)) {                                 // 00474738 cmp ebp,[ebx+6Ch]
        if (scene3d_transform_enabled_0078e5f0 != 0)                       // 00474741..00474748
            gl_transform_0046f060(static_cast<const Vector*>(at(gob, 0x78)),
                                  static_cast<const Quaternion*>(at(gob, 0x84)), 1);   // 0047474A..0047475C
        if (project_shadow_gate_007b9374 == 0) {                           // 0047475F..00474766
            const Vector* const gob_position = static_cast<const Vector*>(at(gob, 0x78));
            const Quaternion* const gob_orientation = static_cast<const Quaternion*>(at(gob, 0x84));
            Vector relative;
            relative.x = S(W(position.x) - W(gob_position->x));            // 00474768..0047477F
            relative.y = S(W(position.y) - W(gob_position->y));            // 00474787..00474793
            relative.z = S(W(position.z) - W(gob_position->z));            // 00474797..004747A1
            Quaternion inverse; Vector rotated;
            const Vector* const r = quaternion_rotate_004a9bb0(&rotated,
                quaternion_negate_w_004a9b50(&inverse, gob_orientation), &relative);   // 004747A5..004747B3
            position.x = r->x; position.y = r->y; position.z = r->z;        // 004747B8..004747DA (dword copies)
            plane_transform_004ab4c0(&plane, nullptr, gob_position, gob_orientation);  // 004747D1..004747E1
        }
        const U scale = field<U>(gob, 0x1B4);                              // 004747E6
        if (scale != kOne)                                                 // 004747EC..004747F1
            glScalef_0073d374(bits_to_float(scale), bits_to_float(scale), bits_to_float(scale)); // 004747F3..004747FA
    }
    Plane local; local.a = 0.0f; local.b = 0.0f; local.c = 0.0f; local.d = 0.0f;   // 00474807..0047481F
    if (project_shadow_gate_007b9374 == 0) {                               // 00474800 / 00474827
        const Vector* const part_position = static_cast<const Vector*>(at(part, 0x08));
        const Quaternion* const part_orientation = static_cast<const Quaternion*>(at(part, 0x14));
        Vector relative;
        relative.x = S(W(position.x) - W(part_position->x));               // 0047482D..00474841
        relative.y = S(W(position.y) - W(part_position->y));               // 00474849..00474855
        relative.z = S(W(position.z) - W(part_position->z));               // 00474859..00474863
        Quaternion inverse; Vector rotated;
        const Vector* const r = quaternion_rotate_004a9bb0(&rotated,
            quaternion_negate_w_004a9b50(&inverse, part_orientation), &relative);  // 00474867..00474875
        position.x = r->x; position.y = r->y; position.z = r->z;            // 0047487A..004748AE
        local = plane;                                                     // 004748A8..004748CE (dword copies; also a dead copy of .d to [esp+4C])
        plane_transform_004ab4c0(&local, nullptr, part_position, part_orientation);   // 004748BC..004748D2
    }
    if (scene3d_transform_enabled_0078e5f0 != 0)                           // 004748DE..004748E5
        gl_transform_0046f060(static_cast<const Vector*>(at(part, 0x08)),
                              static_cast<const Quaternion*>(at(part, 0x14)), 0);   // 004748E7..004748F6
    ++r4_lowers_cells::render_counts_00827fa8[0];                          // 004748F9 inc dword ptr [00827FA8]
    {
        const U scale = field<U>(part, 0x24);                              // 004748FF
        if (scale != kOne)                                                 // 00474902..00474907
            glScalef_0073d374(bits_to_float(scale), bits_to_float(scale), bits_to_float(scale)); // 00474909..00474910
    }
    // 00474916..00474959: project unless the gate is clear and the position is exactly (0,0,0).
    if (project_shadow_gate_007b9374 != 0 || W(position.x) != kZero || W(position.y) != kZero ||
        W(position.z) != kZero) {
        if (part_vt(part)->s02c(part, nullptr) != 0 &&                     // 0047495B..00474965 call [edx+2Ch]
            field<U8>(field<void*>(part, 0x04), 0x188) != 0)               // 00474967..00474973 movzx [owner+188h]
            set_beaming_00471450();                                        // 00474975
        part_vt(part)->s05c(part, nullptr, word(&position), float_to_bits(range), word(&local), 0);   // 0047497A..00474996 call [edx+5Ch]
    }
    for (I i = 0; i < field<I>(part, 0x34); ++i) {                         // 00474999..004749A4 / 00474C65..00474C6F (count re-read)
        Plane through = local;                                             // 004749B0..004749D3 [esp+40..4C]
        Vector lpos = position;                                            // 004749CC..004749F3 [esp+10..18]
        void* const child = ptr<void*>(field<U>(part, 0x30))[i];           // 004749ED..004749F0
        if (field<U8>(child, 0x2C) == 0) continue;                         // 004749F7..004749FC je 00474C65
        glPushMatrix_0073d300();                                           // 00474A02
        if (child == field<void*>(gob, 0x6C)) {                            // 00474A08..00474A0B
            if (scene3d_transform_enabled_0078e5f0 != 0)                   // 00474A11..00474A18
                gl_transform_0046f060(static_cast<const Vector*>(at(gob, 0x78)),
                                      static_cast<const Quaternion*>(at(gob, 0x84)), 1);   // 00474A1A..00474A2C
            if (project_shadow_gate_007b9374 == 0) {                       // 00474A2F..00474A36
                const Vector* const gob_position = static_cast<const Vector*>(at(gob, 0x78));
                const Quaternion* const gob_orientation = static_cast<const Quaternion*>(at(gob, 0x84));
                Vector difference; Quaternion inverse; Vector rotated;
                const float* const d = kotor_cdecl_00423bc0(&difference.x, &lpos.x, &gob_position->x);   // 00474A38..00474A51
                const Vector* const r = quaternion_rotate_004a9bb0(&rotated,
                    quaternion_negate_w_004a9b50(&inverse, gob_orientation),
                    reinterpret_cast<const Vector*>(d));                   // 00474A54..00474A6F (Vector's first member)
                lpos.x = r->x; lpos.y = r->y; lpos.z = r->z;                // 00474A74..00474A8D
                plane_transform_004ab4c0(&through, nullptr, gob_position, gob_orientation);   // 00474A87..00474A91
            }
            const U scale = field<U>(gob, 0x1B4);                          // 00474A9D
            if (scale != kOne)                                             // 00474AA3..00474AA8
                gl_scalef_00459150(bits_to_float(scale), bits_to_float(scale), bits_to_float(scale)); // 00474AAA..00474AB6 (call 00459150)
        }
        Plane projected; projected.a = 0.0f; projected.b = 0.0f; projected.c = 0.0f; projected.d = 0.0f; // 00474AC0..00474AD8
        if (project_shadow_gate_007b9374 == 0) {                           // 00474AB9 / 00474AE0
            const Vector* const child_position = static_cast<const Vector*>(at(child, 0x08));
            const Quaternion* const child_orientation = static_cast<const Quaternion*>(at(child, 0x14));
            Vector difference; Quaternion inverse; Vector rotated;
            const float* const d = kotor_cdecl_00423bc0(&difference.x, &lpos.x, &child_position->x);   // 00474AE6..00474AFF
            const Vector* const r = quaternion_rotate_004a9bb0(&rotated,
                quaternion_negate_w_004a9b50(&inverse, child_orientation),
                reinterpret_cast<const Vector*>(d));                       // 00474B02..00474B1A
            lpos.x = r->x; lpos.y = r->y; lpos.z = r->z;                    // 00474B1F..00474B3E
            projected = through;                                           // 00474B33..00474B5E (dword copies)
            plane_transform_004ab4c0(&projected, nullptr, child_position, child_orientation);   // 00474B4A..00474B62
        }
        if (scene3d_transform_enabled_0078e5f0 != 0)                       // 00474B72..00474B79
            gl_transform_0046f060(static_cast<const Vector*>(at(child, 0x08)),
                                  static_cast<const Quaternion*>(at(child, 0x14)), 0);   // 00474B7B..00474B8A
        ++r4_lowers_cells::render_counts_00827fa8[0];                      // 00474B8D
        {
            const U scale = field<U>(child, 0x24);                         // 00474B93
            if (scale != kOne)                                             // 00474B96..00474B9B
                gl_scalef_00459150(bits_to_float(scale), bits_to_float(scale), bits_to_float(scale)); // 00474B9D..00474BA9
        }
        if (project_shadow_gate_007b9374 != 0 || vector_is_zero_0046a620(&lpos, nullptr) == 0) {   // 00474BAC..00474BC0
            if (part_vt(child)->s02c(child, nullptr) != 0 &&               // 00474BC2..00474BCB
                field<U8>(field<void*>(child, 0x04), 0x188) != 0)          // 00474BCD..00474BD9
                set_beaming_00471450();                                    // 00474BDB
            part_vt(child)->s05c(child, nullptr, word(&lpos), float_to_bits(range), word(&projected), 0); // 00474BE0..00474BF8
        }
        for (I j = 0; j < field<I>(child, 0x34); ++j)                      // 00474BFB..00474C59 (count re-read)
            gob_part_volume_project_00474710(gob, nullptr, ptr<void*>(field<U>(child, 0x30))[j], lpos, range,
                                             projected);                  // 00474C04..00474C4E (ECX = gob)
        glPopMatrix_0073d2fc();                                            // 00474C5B
    }
    glPopMatrix_0073d2fc();                                                // 00474C75
}

// =====================================================================================================================
// CAurBehaviorCameraFollow (table 00743924, 8 slots 00743924..00743943; the next word is .rdata text).
// Slot bodies: +00 004A0040 (here), +04 004015A0 (void_return provider), +08 0049F6D0 Control and +0C 0049EE00
// Message (NEW externals, not authored: 916 / 693 bytes), +10 0049FF90 GetType (here), +14/+18 0063E7F0 (providers),
// +1C 00641DB0 self_return typed as the slot's U-returning type (D24: own typed body, below).
U __fastcall behavior_camera_follow_control_0049f6d0(void* self, void* unused_edx, U raw_delta);   // RET 4
void __fastcall behavior_camera_follow_message_0049ee00(void* self, void* unused_edx, char* line);   // RET 4
void* __fastcall behavior_camera_follow_scalar_delete_004a0040(void* self, void* unused_edx, U flags);
U __fastcall behavior_camera_follow_get_type_0049ff90(void* self, void* unused_edx);
U __fastcall behavior_camera_follow_slot1c_self_00641db0(void* self, void* unused_edx);
extern const kotor_scene3d_construct::BehaviorBaseSlots behavior_camera_follow_vtable_00743924;
const kotor_scene3d_construct::BehaviorBaseSlots behavior_camera_follow_vtable_00743924 = {
    &behavior_camera_follow_scalar_delete_004a0040,   // +00 004A0040
    &behavior_void_return_004015a0,                   // +04 004015A0
    &behavior_camera_follow_control_0049f6d0,         // +08 0049F6D0
    &behavior_camera_follow_message_0049ee00,         // +0C 0049EE00
    &behavior_camera_follow_get_type_0049ff90,        // +10 0049FF90
    &behavior_null_return_0063e7f0,                   // +14 0063E7F0
    &behavior_return_zero_0063e7f0,                   // +18 0063E7F0
    &behavior_camera_follow_slot1c_self_00641db0,     // +1C 00641DB0
};

// 0049FF90 GetType: mov eax,3EDh; ret.
U __fastcall behavior_camera_follow_get_type_0049ff90(void* /*self*/, void* /*unused_edx*/) { return 0x3EDu; }
// 00641DB0 (mov eax,ecx; ret) in the slot's own U-returning type (D24).
U __fastcall behavior_camera_follow_slot1c_self_00641db0(void* self, void* /*unused_edx*/) { return word(self); }

// CAurBehaviorCameraFollow::~CAurBehaviorCameraFollow 0049FFA0 (authored callee): __thiscall RET 0.
void __fastcall behavior_camera_follow_destruct_0049ffa0(void* self, void* /*unused_edx*/) {
    field<const void*>(self, 0) = &behavior_camera_follow_vtable_00743924;   // 0049FFA6 mov [esi],743924h
    void* const target = field<void*>(self, 0x14);                         // 0049FFA3..0049FFAC (SafePointer +14)
    if (target != nullptr)                                                 // 0049FFAE..0049FFB0
        texture_list_remove_00445080(at(target, 4), nullptr, at(self, 0x14));   // 0049FFB2..0049FFB6 (target+4 list, &safe)
    behavior_destruct_0049fa70(self, nullptr);                             // 0049FFBB..0049FFBE jmp 0049FA70
}
// Scalar deleting destructor 004A0040: __thiscall RET 4, EAX = this.
void* __fastcall behavior_camera_follow_scalar_delete_004a0040(void* self, void* /*unused_edx*/, U flags) {
    behavior_camera_follow_destruct_0049ffa0(self, nullptr);               // 004A0041..004A0043
    if ((flags & 1u) != 0) live_free(self);                                // 004A0048..004A0055
    return self;                                                           // 004A0058
}

// CAurBehaviorCameraFollow::CAurBehaviorCameraFollow(CAurObject* object, CAurObject* follow) 0049FE80:
// __thiscall RET 8, EAX = this. CAurBehavior base initialisation is inlined (no base-table store in the image).
void* __fastcall behavior_camera_follow_construct_0049fe80(void* self, void* /*unused_edx*/, void* gob,
                                                           void* target) {
    field<U>(self, 0x08) = 0;                                              // 0049FEA4
    field<U>(self, 0x0C) = 0;                                              // 0049FEA7
    field<U>(self, 0x04) = 0;                                              // 0049FEAA
    field<void*>(self, 0x10) = gob;                                        // 0049FEB1
    field<const void*>(self, 0) = &behavior_camera_follow_vtable_00743924; // 0049FEB7 mov [esi],743924h
    field<U>(self, 0x14) = 0;                                              // 0049FEC1 (SafePointer target)
    field<U>(self, 0x18) = 0;                                              // 0049FEC8
    field<U>(self, 0x1C) = 0;                                              // 0049FECB
    field<U>(self, 0x20) = 0;                                              // 0049FECE
    field<U>(self, 0x24) = 0;                                              // 0049FED7
    field<U>(self, 0x28) = 0;                                              // 0049FEDA
    field<U>(self, 0x2C) = 0;                                              // 0049FEDD
    behavior_add_dependency_0049f130(self, nullptr, target);               // 0049FED1..0049FEE0 (ECX = this)
    assign_00447fa0(at(self, 0x14), nullptr, target);                      // 0049FEE5..0049FEE8
    // 0049FEED..0049FEF6: target->GetPosition(&tmp) through the Gob table slot +64 (hidden result pointer, EAX).
    Vector tmp;
    const Vector* const p = ptr<const Vector>(gob_vt(target)->s064_gob_get_position(
        static_cast<aurora_model::Gob*>(target), nullptr, word(&tmp)));
    field<U>(self, 0x18) = float_to_bits(p->x);                            // 0049FEF9..0049FEFB (dword copies)
    field<U>(self, 0x1C) = float_to_bits(p->y);                            // 0049FEFE..0049FF01
    field<U>(self, 0x20) = float_to_bits(p->z);                            // 0049FF04..0049FF07
    field<U>(self, 0x24) = 0;                                              // 0049FF0C
    field<U>(self, 0x28) = 0;                                              // 0049FF21
    field<U>(self, 0x2C) = kOne;                                           // 0049FF15..0049FF24 (1.0f)
    field<U>(self, 0x3C) = 0;                                              // 0049FF2C
    field<U>(self, 0x40) = 0;                                              // 0049FF2F
    field<U>(self, 0x48) = 0;                                              // 0049FF32
    field<U>(self, 0x34) = 0;                                              // 0049FF35
    field<U>(self, 0x50) = 0;                                              // 0049FF38
    field<U>(self, 0x4C) = 0;                                              // 0049FF3B
    field<U>(self, 0x5C) = 0;                                              // 0049FF3E
    field<U>(self, 0x58) = 0;                                              // 0049FF41
    field<U>(self, 0x68) = 0;                                              // 0049FF44
    field<U>(self, 0x64) = 0;                                              // 0049FF47
    field<U>(self, 0x54) = 0x43B40000u;                                    // 0049FF4A (360.0f)
    field<U>(self, 0x60) = 0x43B40000u;                                    // 0049FF4D (360.0f)
    field<U>(self, 0x38) = kOne;                                           // 0049FF59 (1.0f)
    field<U>(self, 0x30) = 0x41000000u;                                    // 0049FF60 (8.0f)
    field<U>(self, 0x44) = 0x42340000u;                                    // 0049FF67 (45.0f)
    field<U>(self, 0x6C) = 0x41200000u;                                    // 0049FF6E (10.0f)
    return self;                                                           // 0049FF75 mov eax,esi
}
}

// =====================================================================================================================
// CAurBehaviorCameraFollow::Message 0049EE00 and ::Control 0049F6D0 (slots +0C / +08 of 00743924).
extern "C" {
// lane-font font-txi-readers (font_txi.hpp:64/66/69, exact copies) and string-compare-existing (font_txi.hpp:78).
char* __cdecl firstword_00463530(char*);
void __cdecl parse_float_004635c0(char*, volatile float*);
void __cdecl parse_vector_00463570(char*, volatile float*);
__declspec(noinline) std::int32_t __cdecl kotor_stricmp_0070acaf(const char*, const char*);
// x87-pow-entry: genuine CRT _CIpow 006FC1F0 (ST1 = base, ST0 = exponent -> ST0); called only from inline asm
// (same declaration as aurora-window/aurora_window_slots.cpp:31).
void __cdecl genuine_x87_pow_006fc1f0();
// core46-math / anim-contested / model46-f2-math (tile_scene.h:43, anim3d.h:211, emitter.h:186, exact copies).
kotor_scene3d::Quaternion* __cdecl yaw_pitch_roll_004acac0(kotor_scene3d::Quaternion* out, float yaw, float pitch,
                                                          float roll);
kotor_scene3d::Quaternion* __cdecl quaternion_slerp_004a9e00(kotor_scene3d::Quaternion* out,
    kotor_scene3d::Quaternion from, const kotor_scene3d::Quaternion* to, float t);
kotor_scene3d::Vector* __fastcall quaternion_zdir_0043e250(const kotor_scene3d::Quaternion* q, void* unused_edx,
                                                          kotor_scene3d::Vector* out);
}

namespace m47_gob2 {
// D30-FINAL: the original CRT pow 006FC1F0 on x87 operands (exact double operands under PC_53; result stored as a
// double - exact under PC_53).
__declspec(noinline) double x87_pow_006fc1f0(double base, double exponent) {
    double result;
    __asm {
        fld qword ptr base
        fld qword ptr exponent
        call genuine_x87_pow_006fc1f0
        fstp qword ptr result
    }
    return result;
}
inline const kotor_scene3d::SceneVTable* scene_vt(void* scene) {
    return *static_cast<const kotor_scene3d::SceneVTable* const volatile*>(scene);
}
// The three clamp blocks of 0049F74E.. / 0049F7AA.. / 0049F817..: when the clamp flag is not 0.0,
// value = (min(value,max) < min) ? min : min(value,max), with min(v,m) = (v < m) ? v : m (fcomp; test ah,5; jp
// takes the bound when v >= m or unordered). The second min(value,max) re-reads both cells (same values).
inline void clamp_cell(void* self, U value_offset, U flag_offset, U min_offset, U max_offset) {
    if (!(W(field<float>(self, flag_offset)) != kZero)) return;            // fcomp 0.0; test ah,44h; jnp (skip when equal)
    const float bounded = (W(field<float>(self, value_offset)) < W(field<float>(self, max_offset)))
                              ? field<float>(self, value_offset) : field<float>(self, max_offset);
    if (W(bounded) < W(field<float>(self, min_offset))) {
        field<float>(self, value_offset) = field<float>(self, min_offset);
    } else {
        field<float>(self, value_offset) = (W(field<float>(self, value_offset)) < W(field<float>(self, max_offset)))
                                               ? field<float>(self, value_offset) : field<float>(self, max_offset);
    }
}
struct CameraFollowKey { const char* word; U skip; U offset; bool vector; };
// 0049EE0F..0049F0A8: keyword (pushed .rdata string), line offset, member offset, parser (004635C0 / 00463570).
const CameraFollowKey kCameraFollowKeys[17] = {
    {"turn", 4, 0x40, false},        // 007437BC  0049EE0F..0049EE28
    {"spring", 6, 0x38, false},      // 0074385C  0049EE37..0049EE50
    {"dist", 4, 0x30, false},        // 00743854  0049EE5F..0049EE78
    {"offset", 6, 0x24, true},       // 0074384C  0049EE87..0049EEA0 (Parse_Vector)
    {"yaw", 3, 0x3C, false},         // 00743848  0049EEAF..0049EEC8
    {"pitch", 5, 0x44, false},       // 00743840  0049EED7..0049EEF0
    {"tilt", 4, 0x48, false},        // 007437B4  0049EEFF..0049EF18
    {"speed", 5, 0x34, false},       // 00741CD4  0049EF27..0049EF40
    {"minyaw", 6, 0x50, false},      // 00743838  0049EF4F..0049EF68
    {"maxyaw", 6, 0x54, false},      // 00743830  0049EF77..0049EF90
    {"minpitch", 8, 0x5C, false},    // 00743824  0049EF9F..0049EFB8
    {"maxpitch", 8, 0x60, false},    // 00743818  0049EFC7..0049EFE0
    {"mindist", 7, 0x68, false},     // 00743810  0049EFEF..0049F008
    {"maxdist", 7, 0x6C, false},     // 00743808  0049F017..0049F030
    {"clampyaw", 8, 0x4C, false},    // 007437FC  0049F03F..0049F058
    {"clamppitch", 0x0A, 0x58, false}, // 007437F0 0049F067..0049F080
    {"clampdist", 9, 0x64, false},   // 007437E4  0049F08F..0049F0A8
};
}

extern "C" {
// 0049EE00 Message(char* line): __thiscall RET 4. Every keyword test re-runs firstword on the line (no else-chain).
void __fastcall behavior_camera_follow_message_0049ee00(void* self, void* /*unused_edx*/, char* line) {
    for (const CameraFollowKey& key : kCameraFollowKeys) {
        if (kotor_stricmp_0070acaf(key.word, firstword_00463530(line)) == 0) {   // push word; push key; call 0070ACAF
            volatile float* const target = static_cast<volatile float*>(at(self, key.offset));
            if (key.vector) parse_vector_00463570(line + key.skip, target);
            else parse_float_004635c0(line + key.skip, target);
        }
    }
}                                                                          // 0049F0B0..0049F0B2 ret 4

// 0049F6D0 Control(float delta): __thiscall RET 4, EAX = 1 on every return path (0049F730 / 0049FA58).
U __fastcall behavior_camera_follow_control_0049f6d0(void* self, void* /*unused_edx*/, U raw_delta) {
    void* const scene = *kotor_scene3d::cells.active_scene_007b92a0;      // 0049F6D7 mov ecx,[007B92A0]
    if (scene != nullptr) (void)scene_vt(scene)->s0a8_scene_is_animating(scene, nullptr);   // 0049F6E4..0049F6E6 (result unused)
    // 0049F6F3..0049F70B: identity quaternion stored to [esp+58]; every later read follows the 0049F91D overwrite.
    if (behavior_control_0049f270(self, nullptr, raw_delta) == 0) return 1;   // 0049F6EC..0049F71A
    void* const target = field<void*>(self, 0x14);                         // 0049F720 mov edi,[esi+14h]
    void* const gob = field<void*>(self, 0x10);
    if (target == nullptr) {                                               // 0049F723..0049F725
        gob_vt(gob)->s058_gob_set_behavior(static_cast<aurora_model::Gob*>(gob), nullptr, nullptr);   // 0049F727..0049F72D push 0; call [edx+58h]
        return 1;                                                          // 0049F730
    }
    const double dt = W(bits_to_float(raw_delta));
    {   // 0049F73E..0049F74B: yaw += delta * turn; turn = 0 (turn read before the store).
        const double step = dt * W(field<float>(self, 0x40));
        field<U>(self, 0x40) = 0;
        field<float>(self, 0x3C) = S(step + W(field<float>(self, 0x3C)));
    }
    clamp_cell(self, 0x3C, 0x4C, 0x50, 0x54);                             // 0049F74E..0049F797 (clampyaw, minyaw, maxyaw)
    {   // 0049F79A..0049F7A7: pitch += delta * tilt; tilt = 0.
        const double step = dt * W(field<float>(self, 0x48));
        field<U>(self, 0x48) = 0;
        field<float>(self, 0x44) = S(step + W(field<float>(self, 0x44)));
    }
    clamp_cell(self, 0x44, 0x58, 0x5C, 0x60);                             // 0049F7AA..0049F7F3 (clamppitch, minpitch, maxpitch)
    {   // 0049F7F6..0049F814: dist *= pow(2.0, delta * speed * -0.5f); speed = 0.
        const double exponent = (dt * W(field<float>(self, 0x34))) * W(bits_to_float(0xBF000000u)); // fmul [esi+34h]; fmul [00743884] (-0.5f)
        const double factor = x87_pow_006fc1f0(2.0, exponent);             // fld qword [00741DF8] (2.0); call 006FC1F0
        const double scaled = factor * W(field<float>(self, 0x30));        // 0049F80E fmul [esi+30h]
        field<U>(self, 0x34) = 0;                                          // 0049F811
        field<float>(self, 0x30) = S(scaled);                              // 0049F814
    }
    clamp_cell(self, 0x30, 0x64, 0x68, 0x6C);                             // 0049F817..0049F860 (clampdist, mindist, maxdist)
    // 0049F863..0049F882: k = 1.0 - pow(0.5, delta * spring), stored as a float into the delta argument cell.
    const float k = S(W(bits_to_float(kOne)) - x87_pow_006fc1f0(0.5, dt * W(field<float>(self, 0x38))));
    // 0049F87B..0049F88B: target->GetPosition(&tmp) (Gob table +64, hidden result pointer).
    Vector tmp;
    const Vector* const at_target = ptr<const Vector>(gob_vt(target)->s064_gob_get_position(
        static_cast<aurora_model::Gob*>(target), nullptr, word(&tmp)));
    const double ox = W(at_target->x) + W(field<float>(self, 0x24));      // 0049F88E..0049F890 (wide)
    const double oy = W(at_target->y) + W(field<float>(self, 0x28));      // 0049F896..0049F89C (wide)
    const float oz = S(W(at_target->z) + W(field<float>(self, 0x2C)));    // 0049F8A0..0049F8AF fstp [esp+2Ch]
    const float bx = S(ox * W(k));                                         // 0049F8B6..0049F8BD
    const double by = oy * W(k);                                           // 0049F8C1
    const double bz = W(oz) * W(k);                                        // 0049F8C8..0049F8CC
    const double keep = W(bits_to_float(kOne)) - W(k);                     // 0049F8D3..0049F8D9
    Vector* const current = static_cast<Vector*>(at(self, 0x18));          // 0049F893 lea edi,[esi+18h]
    const double cx = keep * W(current->x);                                // 0049F8E0..0049F8E2
    const float cy = S(keep * W(current->y));                              // 0049F8E4..0049F8E9
    const float cz = S(keep * W(current->z));                              // 0049F8ED..0049F8F2
    Vector blended;
    blended.x = S(cx + W(bx));                                             // 0049F8F6..0049F8FA
    blended.y = S(W(cy) + by);                                             // 0049F8FE..0049F904
    blended.z = S(W(cz) + bz);                                             // 0049F908..0049F90E
    // 0049F916..0049F93C: YawPitchRoll(&q, yaw, pitch, 0) copied to [esp+58] (raw words of +3C / +44, roll 0).
    Quaternion scratch;
    const Quaternion goal = *yaw_pitch_roll_004acac0(&scratch, field<float>(self, 0x3C), field<float>(self, 0x44),
                                                     bits_to_float(0u));
    // 0049F940..0049F943: gob->GetOrientation(&scratch) (Gob table +68; EBX keeps the gob table for +60 below).
    const Quaternion* const facing = ptr<const Quaternion>(gob_vt(gob)->s068_gob_get_orientation(
        static_cast<aurora_model::Gob*>(gob), nullptr, word(&scratch)));
    Quaternion slerped;
    const Quaternion* const s = quaternion_slerp_004a9e00(&slerped, *facing, &goal, k);   // 0049F946..0049F970
    Quaternion normalized;
    const Quaternion* const n = quaternion_normalize_004a9d60(&normalized, *s);           // 0049F975..0049F995
    Quaternion orientation_result;                                         // [esp+18] hidden result of +60
    gob_vt(gob)->s060_gob_set_orientation(static_cast<aurora_model::Gob*>(gob), nullptr, word(&orientation_result),
        float_to_bits(n->w), float_to_bits(n->x), float_to_bits(n->y), float_to_bits(n->z));   // 0049F99A..0049F9BD call [ebx+60h]
    const double dist = W(field<float>(self, 0x30));                       // 0049F9C0 / 0049F9CF (stored into the argument cell)
    Quaternion facing_buffer;
    const Quaternion* const facing_now = ptr<const Quaternion>(gob_vt(gob)->s068_gob_get_orientation(
        static_cast<aurora_model::Gob*>(gob), nullptr, word(&facing_buffer)));   // 0049F9C3..0049F9D9
    Vector z;
    const Vector* const zd = quaternion_zdir_0043e250(facing_now, nullptr, &z);   // 0049F9DC..0049F9DE
    const double zx = dist * W(zd->x);                                     // 0049F9E3..0049F9E7
    const double zy = dist * W(zd->y);                                     // 0049F9EC..0049F9F2
    const float zz = S(dist * W(zd->z));                                   // 0049F9F8..0049FA04
    Vector placed;
    placed.x = S(zx + W(blended.x));                                       // 0049FA0A..0049FA0E
    placed.y = S(zy + W(blended.y));                                       // 0049FA18..0049FA1C
    placed.z = S(W(zz) + W(blended.z));                                    // 0049FA20..0049FA2F
    Vector position_result;                                                // [esp+48] hidden result of +5C
    gob_vt(gob)->s05c_gob_set_position(static_cast<aurora_model::Gob*>(gob), nullptr, &position_result, placed);   // 0049FA3A..0049FA3F call [edx+5Ch]
    current->x = blended.x;                                                // 0049FA42..0049FA4E (dword copies)
    current->y = blended.y;                                                // 0049FA50
    current->z = blended.z;                                                // 0049FA53
    return 1;                                                              // 0049FA58 mov eax,1
}
}
