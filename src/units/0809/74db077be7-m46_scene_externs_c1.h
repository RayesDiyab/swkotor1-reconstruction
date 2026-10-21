#pragma once
// lane-m46-3dcore fork C1: callees of the Scene vtable slot bodies that the canonical scene3d.h does not declare.
// Spellings reuse the EXISTING M45 provider symbols where one exists (verified in m45-compose symbols-m45.json);
// "NEW" marks names with no provider yet (listed as undefined externals in parts/C1.json).
#include <cstdio>
#include "6c81d8874d-scene3d.h"

extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::Vector; using kotor_scene3d::PointerList;

// 00445080 CExoArrayList<T>::Remove (thiscall, RET 4). M45 provider: texture-epoch-texture_x86 @texture_list_remove_00445080@12.
void __fastcall texture_list_remove_00445080(PointerList* list, void*, U value);
// 00445020 CExoArrayList<T>::Insert(value, index) (thiscall, RET 8; EAX = list data pointer, 00445071).
// M45 provider: font-texture-order @list_insert_00445020@16 (D-joins3d: aurora_list_insert_00445020 is the same body).
U __fastcall list_insert_00445020(PointerList* list, void*, U value, I index);
// 0070ACAF _stricmp port. M45 provider: string-compare-existing.
std::int32_t __cdecl kotor_stricmp_0070acaf(const char*, const char*);
// 006FAE08 sscanf. M45 provider: font-txi-readers (UCRT-backed provider, see its PACKET sscanf_analysis).
int __cdecl kotor_sscanf_006fae08(const char*, const char*, ...);
// 006FADB0 sprintf. M45 provider: tpc-joins naked port.
int __cdecl format_006fadb0(char*, const char*, ...);
// 0044BFA0 AurResGetNextLine / 0044C740 AurResGet / 0044C8F0 AurResFree. M45 provider: aurres-stream.
char* __cdecl resource_next_0044bfa0();
void* __cdecl aurres_get_0044c740(const char* name, const char* ext, volatile I* size, U memory_only);
U __cdecl aurres_release_0044c8f0(void* descriptor, I size);
// 00463530 firstword / 004635E0 parse int. M45 provider: font-txi-readers.
char* __cdecl firstword_00463530(char*);
void __cdecl parse_int_004635e0(char*, volatile std::int32_t*);
// 00449CC0 NewCAurObject. M45 provider: 3d-model-model-instance.
aurora_model::Gob* __cdecl aurora_new_aur_object_00449cc0(const char* name, const char* gob_name, std::FILE* mdl, std::FILE* mdx);
// 0046A680 Scene BSP tile lookup. M45 provider: 3d-bsp-bsp-scene.
void* __cdecl scene_bsp_get_tile_data_0046a680(void* scene, const Vector* point);
// 004AA980 vector equality. M45 provider: gui-writers-vector-equal.
std::uint32_t __cdecl aurora_vector_equal_004aa980(const void* left, const void* right);
// 0046CA30 UpdateStaticLighting. M45 provider: 3d-construct-construct3d.
void __cdecl update_static_lighting_0046ca30(void* area, void* light);

// NEW (no provider in M45):
// 004A6050 WindManager::GetGlobalWind(Vector* out, Vector point, float) thiscall RET 14h (004A6107/004A6430).
// Last argument passed as the raw stack DWORD (type float per caller usage unknown -> raw word, same ABI).
Vector* __fastcall wind_manager_get_global_wind_004a6050(void* wind, void*, Vector* out, Vector point, U last);
// 004237E0 AurTextureReplaceTileReferences (cdecl, plain RET 00423820/00423A0D/00423A55; 4 stack args at 0044FC88..0044FC95).
U __cdecl aur_texture_replace_tile_references_004237e0(void* scene, U a, U b, U c);
// 00454940 Scene::SetVisibility(room, room, flag) thiscall RET 0Ch (004549D4/00454A11).
U __fastcall scene_set_visibility_00454940(void* scene, void*, void* room, void* visible_room, U flag);
// 00458BE0 Scene::~Scene body (thiscall, plain RET 00458E6C).
void __fastcall scene_destruct_00458be0(void* scene, void*);

// Cells. 008305E0 (BSS) is the black-fog/blind-lights word read through kotor_scene3d::Cells::black_fog_008305e0
// (const view); its storage must be defined once by the Cells owner under this name (see parts/C1.json).
extern volatile U scene_black_fog_008305e0;
// 0078E3A0 geometry-fade mode (.data cold 1); only 0044FD10/00458BC0 touch it - storage in m46_scene_c1_cells.cpp.
extern volatile U scene_geom_fade_mode_0078e3a0;
}
