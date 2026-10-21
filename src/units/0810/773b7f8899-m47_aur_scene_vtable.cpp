// lane-m47-3d rev2: CAurScene class table 00741610 + its own slot +00 (004579A0) and CAurScene::~CAurScene 00457990.
// Evidence: pinned image table words (see header); 00457990..0045799E; 004579A0..004579BB.
#include <cstring>
#include "9219c31d0e-m47_aur_scene_vtable.h"
extern "C" {
void __cdecl packed_purecall_006fb8de(void);   // owner packed-file-family (packed_file.h:84), _purecall 006FB8DE
void __fastcall aur_scene_destruct_00457820(void* list, void*);   // lane-m47-3d m47_scene.cpp (SafePointerList teardown, ECX=this+4)
void __cdecl live_free(void*);                 // M41 free 006FA390 family (stage scene3d.h:304)
// 00457990 CAurScene::~CAurScene (thiscall RET 0): [ecx] = 00741610; ecx += 4; jmp 00457820.
void __fastcall aur_scene_destruct_00457990(void* scene, void*) {
    const m47_aur_scene::AurSceneVTable* own = &aur_scene_vtable_00741610;
    std::memcpy(scene, &own, sizeof own);                                       // 00457990 mov dword ptr [ecx],741610h
    aur_scene_destruct_00457820(static_cast<unsigned char*>(scene) + 4, nullptr);   // 00457996 add ecx,4; 00457999 jmp
}
// 004579A0 CAurScene scalar deleting destructor (slot +00; thiscall RET 4, EAX = this).
kotor_scene3d::U __fastcall aur_scene_scalar_delete_004579a0(void* scene, void*, kotor_scene3d::U flags) {
    aur_scene_destruct_00457990(scene, nullptr);                                 // 004579A3
    if ((flags & 1u) != 0) live_free(scene);                                     // 004579A8..004579B0 test byte [esp+8],1
    return static_cast<kotor_scene3d::U>(reinterpret_cast<std::uintptr_t>(scene));   // 004579B8 mov eax,esi
}
}
// Purecall slots: a typed function of the slot's own type whose body is the original _purecall 006FB8DE (push 19h;
// call _amsg_exit; never returns). The value after the call is unreachable (D2 label: no original EAX/ST0 exists).
namespace m47_aur_scene {
template <class F> struct PureCall;
template <class R, class... A> struct PureCall<R (__fastcall*)(A...)> {
    static R __fastcall slot(A...) {
        packed_purecall_006fb8de();   // 006FB8DE (does not return)
        return R();                   // D2: unreachable
    }
};
}
extern "C" const m47_aur_scene::AurSceneVTable aur_scene_vtable_00741610 = {
    &aur_scene_scalar_delete_004579a0,   // +000 004579A0
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s004_scene_set_fog)>::slot,   // +004 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s008_scene_set_fog_color)>::slot,   // +008 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s00c_scene_get_object)>::slot,   // +00C 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s010_scene_set_object_scale)>::slot,   // +010 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s014_scene_get_object_scale)>::slot,   // +014 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s018_scene_render)>::slot,   // +018 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s01c_scene_animate)>::slot,   // +01C 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s020_void_return_14)>::slot,   // +020 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s024_scene_spawn_tile)>::slot,   // +024 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s028_scene_set_tile_lights)>::slot,   // +028 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s02c_scene_set_tile_flames)>::slot,   // +02C 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s030_scene_replace_tile_texture)>::slot,   // +030 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s034_scene_delete_tile)>::slot,   // +034 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s038_scene_hit_check)>::slot,   // +038 006FB8DE
    &caur_scene_compute_static_lighting_0044fcc0,   // +03C 0044FCC0 (M46 core46-scene-c1, vtables_gen.h:19)
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s040_scene_get_fog_range)>::slot,   // +040 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s044_scene_set_fog_range)>::slot,   // +044 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s048_scene_set_scene_focus)>::slot,   // +048 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s04c_scene_set_geom_fade_properties)>::slot,   // +04C 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s050_scene_is_geom_fade_enabled)>::slot,   // +050 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s054_scene_get_property_0x58)>::slot,   // +054 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s058_scene_void_return)>::slot,   // +058 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s05c_scene_get_num_rooms)>::slot,   // +05C 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s060_scene_enable_animations)>::slot,   // +060 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s064_scene_disable_animations)>::slot,   // +064 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s068_scene_set_blind_lights)>::slot,   // +068 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s06c_scene_spawn_rooms)>::slot,   // +06C 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s070_scene_spawn_room)>::slot,   // +070 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s074_scene_find_room)>::slot,   // +074 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s078_scene_get_room)>::slot,   // +078 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s07c_scene_set_current_room)>::slot,   // +07C 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s080_scene_set_current_room)>::slot,   // +080 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s084_scene_get_ignore_visibility_graph)>::slot,   // +084 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s088_scene_enable_visibility_graph)>::slot,   // +088 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s08c_scene_disable_visibility_graph)>::slot,   // +08C 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s090_scene_save_visibility)>::slot,   // +090 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s094_scene_load_visibility)>::slot,   // +094 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s098_scene_render_gob_first)>::slot,   // +098 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s09c_scene_render_gob_first)>::slot,   // +09C 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s0a0_scene_render_gob_after)>::slot,   // +0A0 006FB8DE
    &m47_aur_scene::PureCall<decltype(kotor_scene3d::SceneVTable::s0a4_scene_render_gob_before)>::slot,   // +0A4 006FB8DE
};
