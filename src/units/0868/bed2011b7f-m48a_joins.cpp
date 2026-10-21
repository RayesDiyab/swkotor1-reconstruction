// lane-m48-a composition-time joins (M48 proposal). Linker directives only: this translation unit defines no code
// and no data. Each /alternatename resolves a consumer spelling onto the single existing provider (D9 / D59 / D65
// mechanism; ABI-identical: same calling convention, argument list and stack cleanup; no thunk, no cast).
//
// 00420D70 AurTextureRelease -> lane-window aurora-window/font_release.cpp (texture_release_00420d70).
// cdecl, one pointer argument, caller cleanup, both. Consumer header texture-epoch/include/font.hpp spells the
// return as U; the original leaves EAX as a callee scratch value (00420D9C/00420DA4 tail calls, 00420DA6 path) and
// every consumer discards it ((void) casts / statement calls: 3d/construct/light_nodes.cpp:73,
// 3d/model/model_texture_lists.cpp:106, 3d/mesh/trimesh_f1.cpp:44/135), so the void owner is ABI-identical for them.
#pragma comment(linker, "/alternatename:_release_00420d70=_texture_release_00420d70")
// 004233E0 AurTextureGetNormalizationCubeMap -> lane-renderer-bodies gl-cohort/ati_fragment_shaders.cpp.
// cdecl, no arguments, EAX = texture pointer, both (scene3d.h:499 void* vs texture::Basic*: same register return).
#pragma comment(linker, "/alternatename:_aur_texture_get_normalization_cube_map_004233e0=?TextureGetNormalizationCubeMap_004233e0@kotor_renderer_gl@@YAPAUBasic@texture@@XZ")
// 0078D47C GL_NORMALIZE enum cell (D32/D51 owner lane-gui-draw; lane-m48-a adds the storage to gui-draw/writers/
// gl_enum_cells.cpp). texture-epoch consumer spelling (texture_normalize_cubemap.cpp:26, extern std::uint32_t) ->
// owner (std::uint32_t): same width, same single .data storage.
#pragma comment(linker, "/alternatename:?normalize_enum_0078d47c@kotor_renderer_gl_cells@@3IA=?enable_0078d47c@kotor_renderer_cells@@3IA")
