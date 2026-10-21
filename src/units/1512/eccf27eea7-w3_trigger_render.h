#pragma once
#include <cstdint>
#include "400e8527db-w3_trigger_node.h"
namespace aurora_model {struct MdlNode;}
namespace kotor_reconstruction {struct ArrayMutations;}
namespace kotor_scene3d {struct Vector;}
extern "C" {
void __cdecl recovered_aur_part_trigger_set_highlight_params_0049d9b0(void*,void*,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t);
void __cdecl recovered_aur_part_trigger_set_highlight_0049e610(void*,std::uint32_t,float,std::uint32_t);
void* __cdecl recovered_aur_part_trigger_create_0049e630(void*,void*,std::uint32_t,std::uint32_t,void*,void*);
void* __fastcall w3_trigger_part_construct_0049db30(void*,void*,aurora_model::MdlNode*);
void __fastcall w3_trigger_part_highlight_0049dc10(void*,void*,std::uint32_t,float,std::uint32_t);
void __fastcall w3_trigger_part_triangulate_0049df10(void*,void*,kotor_reconstruction::ArrayMutations);
void __cdecl w3_trigger_part_add_scene_0046d130(void*,void*);
std::uint32_t __fastcall w3_trigger_part_hit_0049d810(void*,void*,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t);
void __fastcall w3_trigger_part_destruct_0049d8e0(void*,void*);
void* __fastcall w3_trigger_part_scalar_0049db10(void*,void*,std::uint32_t);
kotor_scene3d::Vector* __cdecl w3_trigger_vector_list_max_004aafc0(kotor_scene3d::Vector*,const kotor_reconstruction::ArrayMutations*);
kotor_scene3d::Vector* __cdecl w3_trigger_vector_list_min_004ab060(kotor_scene3d::Vector*,const kotor_reconstruction::ArrayMutations*);
}
