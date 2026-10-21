#pragma once
// lane-m47-3d rev2: typed CAurScene class table 00741610 (42 slots, .rdata 00741610..007416B7; D1/D21/D24).
// Pinned-image words: +00 004579A0 (CAurScene scalar deleting dtor), +3C 0044FCC0 (CAurScene::ComputeStaticLighting),
// every other slot 006FB8DE (_purecall). +A8.. is string data ("c_focusgob", "distortiontex", "filmnoisetex", ...),
// so the table ends at +A4. Slot types = the first 42 slot types of the derived Scene table 00741708
// (kotor_scene3d::SceneVTable, vtables_gen.h), whose slots override exactly these CAurScene virtuals.
// Writer: Scene::~Scene 00458E52 (m47_scene.cpp) and CAurScene::~CAurScene 00457990 (m47_aur_scene_vtable.cpp).
#include "6c81d8874d-scene3d.h"
namespace m47_aur_scene {
struct AurSceneVTable {
    decltype(kotor_scene3d::SceneVTable::s000_scene_destruct_scene) s000;   // +000
    decltype(kotor_scene3d::SceneVTable::s004_scene_set_fog) s004;   // +004
    decltype(kotor_scene3d::SceneVTable::s008_scene_set_fog_color) s008;   // +008
    decltype(kotor_scene3d::SceneVTable::s00c_scene_get_object) s00c;   // +00C
    decltype(kotor_scene3d::SceneVTable::s010_scene_set_object_scale) s010;   // +010
    decltype(kotor_scene3d::SceneVTable::s014_scene_get_object_scale) s014;   // +014
    decltype(kotor_scene3d::SceneVTable::s018_scene_render) s018;   // +018
    decltype(kotor_scene3d::SceneVTable::s01c_scene_animate) s01c;   // +01C
    decltype(kotor_scene3d::SceneVTable::s020_void_return_14) s020;   // +020
    decltype(kotor_scene3d::SceneVTable::s024_scene_spawn_tile) s024;   // +024
    decltype(kotor_scene3d::SceneVTable::s028_scene_set_tile_lights) s028;   // +028
    decltype(kotor_scene3d::SceneVTable::s02c_scene_set_tile_flames) s02c;   // +02C
    decltype(kotor_scene3d::SceneVTable::s030_scene_replace_tile_texture) s030;   // +030
    decltype(kotor_scene3d::SceneVTable::s034_scene_delete_tile) s034;   // +034
    decltype(kotor_scene3d::SceneVTable::s038_scene_hit_check) s038;   // +038
    decltype(kotor_scene3d::SceneVTable::s03c_caur_scene_compute_static_lighting) s03c;   // +03C
    decltype(kotor_scene3d::SceneVTable::s040_scene_get_fog_range) s040;   // +040
    decltype(kotor_scene3d::SceneVTable::s044_scene_set_fog_range) s044;   // +044
    decltype(kotor_scene3d::SceneVTable::s048_scene_set_scene_focus) s048;   // +048
    decltype(kotor_scene3d::SceneVTable::s04c_scene_set_geom_fade_properties) s04c;   // +04C
    decltype(kotor_scene3d::SceneVTable::s050_scene_is_geom_fade_enabled) s050;   // +050
    decltype(kotor_scene3d::SceneVTable::s054_scene_get_property_0x58) s054;   // +054
    decltype(kotor_scene3d::SceneVTable::s058_scene_void_return) s058;   // +058
    decltype(kotor_scene3d::SceneVTable::s05c_scene_get_num_rooms) s05c;   // +05C
    decltype(kotor_scene3d::SceneVTable::s060_scene_enable_animations) s060;   // +060
    decltype(kotor_scene3d::SceneVTable::s064_scene_disable_animations) s064;   // +064
    decltype(kotor_scene3d::SceneVTable::s068_scene_set_blind_lights) s068;   // +068
    decltype(kotor_scene3d::SceneVTable::s06c_scene_spawn_rooms) s06c;   // +06C
    decltype(kotor_scene3d::SceneVTable::s070_scene_spawn_room) s070;   // +070
    decltype(kotor_scene3d::SceneVTable::s074_scene_find_room) s074;   // +074
    decltype(kotor_scene3d::SceneVTable::s078_scene_get_room) s078;   // +078
    decltype(kotor_scene3d::SceneVTable::s07c_scene_set_current_room) s07c;   // +07C
    decltype(kotor_scene3d::SceneVTable::s080_scene_set_current_room) s080;   // +080
    decltype(kotor_scene3d::SceneVTable::s084_scene_get_ignore_visibility_graph) s084;   // +084
    decltype(kotor_scene3d::SceneVTable::s088_scene_enable_visibility_graph) s088;   // +088
    decltype(kotor_scene3d::SceneVTable::s08c_scene_disable_visibility_graph) s08c;   // +08C
    decltype(kotor_scene3d::SceneVTable::s090_scene_save_visibility) s090;   // +090
    decltype(kotor_scene3d::SceneVTable::s094_scene_load_visibility) s094;   // +094
    decltype(kotor_scene3d::SceneVTable::s098_scene_render_gob_first) s098;   // +098
    decltype(kotor_scene3d::SceneVTable::s09c_scene_render_gob_first) s09c;   // +09C
    decltype(kotor_scene3d::SceneVTable::s0a0_scene_render_gob_after) s0a0;   // +0A0
    decltype(kotor_scene3d::SceneVTable::s0a4_scene_render_gob_before) s0a4;   // +0A4
};
static_assert(sizeof(AurSceneVTable) == 42 * 4, "CAurScene table 00741610..007416B7");
}
extern "C" const m47_aur_scene::AurSceneVTable aur_scene_vtable_00741610;
